/*
 * ******************************************************************************
 * This program is part of the source code provided with "Linux Kernel Programming"
 * (C) 2022  Samir Mulani
 *
 * Git repository:
 * https://github.com/SamirMulani/Linux_Kernel_Base
 * ******************************************************************************
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/rculist.h>
#include <linux/spinlock.h>
#include <linux/preempt.h>

/**
 * struct book - a book
 *
 * @borrow:	If it is 0, book is not borrowed. it is 1, book is borrowed.
 */
struct book {
	int id;
	char name[64];
	char author[64];
	int borrow;
	struct list_head node;
	struct rcu_head rcu;
};

static LIST_HEAD(books);
static spinlock_t books_lock;

/**
 * callback function for async-reclaim
 *
 * call_rcu() 		:  callback function is called when finish to wait every grace periods (async)
 * synchronize_rcu() :  wait to finish every grace periods (sync)
 *
*/
static void Reclaim_callback(struct rcu_head *rcu) {
	struct book *b = container_of(rcu, struct book, rcu);

	/**
	 * Why print preemt_count??
	 *
	 * To check whether this callback is atomic context or not.
	 * preempt_count here is more than 0. Because it is irq context.
	 *
	*/
	pr_info("%s: callback free : %lx, preempt_count : %d\n", __func__, (unsigned long)b, preempt_count());
	kfree(b);
}

static void Add_book(int id, const char *name, const char *author) {
	struct book *b;

	b = kzalloc(sizeof(struct book), GFP_KERNEL);
	if(!b)
		return;

	b->id = id;
	strncpy(b->name, name, sizeof(b->name));
	strncpy(b->author, author, sizeof(b->author));
	b->borrow = 0;

	/**
	 * list_add_rcu
	 *
	 * add_node(writer - add) use spin_lock()
	 *
	*/
	spin_lock(&books_lock);
	list_add_rcu(&b->node, &books);
	spin_unlock(&books_lock);
	pr_info("%s: New title %s stocked\n",__func__,name);  
}

static int Borrow_book(int id, int async) {
	struct book *b = NULL;
	struct book *new_b = NULL;
	struct book *old_b = NULL;

	/**
	 * updater
	 *
	 * (updater) require that alloc new node & copy, update new node & reclaim old node
	 * list_replace_rcu() is used to do that.
	 *
	*/
	rcu_read_lock();

	list_for_each_entry(b, &books, node) {
		if(b->id == id) {
			if(b->borrow) {
				rcu_read_unlock();
				return -1;
			}

			old_b = b;
			break;
		}
	}

	if(!old_b) {
		rcu_read_unlock();
		return -1;
	}

	new_b = kzalloc(sizeof(struct book), GFP_ATOMIC);
	if(!new_b) {
		rcu_read_unlock();
		return -1;
	}

	memcpy(new_b, old_b, sizeof(struct book));
	new_b->borrow = 1;
	
	spin_lock(&books_lock);
	list_replace_rcu(&old_b->node, &new_b->node);
	spin_unlock(&books_lock);

	rcu_read_unlock();

	if(async) {
		call_rcu(&old_b->rcu, Reclaim_callback);
	}else {
		synchronize_rcu();
		kfree(old_b);
	}

	pr_info("%s:Successfully borrowed  %d, preempt_count : %d\n", __func__, id, preempt_count());
	return 0;
}


static void List_books(void) {
        struct book *b;
        /**
         * reader
         *
         * iteration(read) require rcu_read_lock(), rcu_read_unlock()
         * and use list_for_each_entry_rcu()
         *
        */
	pr_info("%s: Traversing...\n",__func__);
        rcu_read_lock();
        list_for_each_entry_rcu(b, &books, node) {
			pr_info("%s :id : %d, name : %s, author : %s, borrow : %d, addr : %lx\n", \
						__func__, b->id, b->name, b->author, b->borrow, (unsigned long)b);
                }
        rcu_read_unlock();
}


static int Is_borrowed(int id) {
	struct book *b;
	int ret;
	/**
	 * reader
	 *
	 * iteration(read) require rcu_read_lock(), rcu_read_unlock()
	 * and use list_for_each_entry_rcu()
	 *
	*/
	rcu_read_lock();
	list_for_each_entry_rcu(b, &books, node) {
		if(b->id == id) {
			ret = b->borrow;
		}
	}
	rcu_read_unlock();
	return ret;
}

static int Return_book(int id, int async) {
	struct book *b = NULL;
	struct book *new_b = NULL;
	struct book *old_b = NULL;

	/**
	 * updater
	 *
	 * (updater) require that alloc new node & copy, update new node & reclaim old node
	 * list_replace_rcu() is used to do that.
	 *
	*/
	rcu_read_lock();

	list_for_each_entry(b, &books, node) {
		if(b->id == id) {
			if(!b->borrow) {
				rcu_read_unlock();
				return -1;
			}

			old_b = b;
			break;
		}
	}
	if(!old_b) {
		rcu_read_unlock();
		return -1;
	}

	new_b = kzalloc(sizeof(struct book), GFP_ATOMIC);
	if(!new_b) {
		rcu_read_unlock();
		return -1;
	}

	memcpy(new_b, old_b, sizeof(struct book));
	new_b->borrow = 0;
	
	spin_lock(&books_lock);
	list_replace_rcu(&old_b->node, &new_b->node);
	spin_unlock(&books_lock);

	rcu_read_unlock();

	if(async) {
		call_rcu(&old_b->rcu, Reclaim_callback);
	}else {
		synchronize_rcu();
		kfree(old_b);
	}

	pr_info("%s: return success %d, preempt_count : %d\n",__func__, id, preempt_count());
	return 0;
}

static void Delete_book(int id, int async) {
	struct book *b;

	spin_lock(&books_lock);
	list_for_each_entry(b, &books, node) {
		if(b->id == id) {
			/**
			 * list_del
			 *
			 * del_node(writer - delete) require locking mechanism.
			 * we can choose 3 ways to lock. Use 'a' here.
			 *
			 *	a.	locking,
			 *	b.	atomic operations, or
			 *	c.	restricting updates to a single task.
			 *
			*/
			list_del_rcu(&b->node);
			spin_unlock(&books_lock);

			if(async) {
				call_rcu(&b->rcu, Reclaim_callback);
			}else {
				synchronize_rcu();
				kfree(b);
			}
			return;
		}
	}
	spin_unlock(&books_lock);

	pr_info("%s: Book does not exist\n",__func__);
}

static void Test_example(int async) {


	if(async)
		pr_info("%s: Executing operations in asynchrounous mode\n\n",__func__);
	else
		pr_info("%s: Executing operations in synchrounous mode\n\n",__func__);

	Add_book(102, "BOOK1", "xyz");
	Add_book(114, "BOOK2", "zbc");

	List_books();

	Borrow_book(114, async);

	if(Is_borrowed(114))
		pr_info("%s: Book 114 is borrowed\n",__func__);


	Add_book(119, "BOOK3", "rubini");

	List_books();
	Return_book(114, async);

	if(!Is_borrowed(114))
		pr_info("%s: Book 114 is available\n",__func__);

	Delete_book(114, async);
	List_books();

	Delete_book(119, async);
	Delete_book(102, async);
	List_books();
}

static int list_rcu_example_init(void)
{
	spin_lock_init(&books_lock);

	/* Execute operations in synchronous mode */
	Test_example(0);

	/* Execute operations in asynchornous mode */
	Test_example(1);
	return 0;
}

static void list_rcu_example_exit(void)
{
	return;
}

module_init(list_rcu_example_init);
module_exit(list_rcu_example_exit);

MODULE_DESCRIPTION("RCU list");
MODULE_LICENSE("GPL");
