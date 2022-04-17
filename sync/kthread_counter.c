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

/* Kernel Atomic macros

   The simplest of the approaches to kernel synchronization
   The easiest to understand and use. 
   Complex locking primitives are built from them.
   Atomic operations operate in one **uninterruptible* operation

   Usage Examples:
     atomic_t v;         // declaration (signed integer)

     atomic_set(&v, 5);  // assignment
     atomic_add(3, &v);  // atomic addition
     atomic_dec(&v);     // atomic decrement
     atomic_read(&v));   // return value 
     atomic_inc(&v);	 // atomic increment

   Under the hood:
   ==============
     typedef struct { 
		int counter; 
	} atomic_t;

     #define atomic_set(v, i)  ((v)->counter = (i))
     #define atomic_read(v)  ((v)->counter)
     void atomic_add(int i, atomic_t *v); // these implemented in assembly 
     void atomic_sub(int i, atomic_t *v);
     void atomic_inc(atomic_t *v);
     void atomic_dec(atomic_t *v);

*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/current.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/atomic.h>

#define LOOPS 100000

unsigned int counter;
struct task_struct *t1, *t2;

int Kthread_start(void *arg)
{
	int local;
	int i;
	for (i = 0; i < LOOPS; i++){
		local = counter;
		local++;
		counter = local;
	}
		

	return 0;
}

int kthr_init(void)
{
	t1 = kthread_create(Kthread_start, NULL, "Kthread1");
	if(IS_ERR(t1)){
		pr_err("%s: unable to start kernel thread\n",__func__);
		return PTR_ERR(t1);
		
	}
	t2 = kthread_create(Kthread_start, NULL, "Kthread2");
	if(IS_ERR(t1)){
		pr_err("%s: unable to start kernel thread\n",__func__);
		return PTR_ERR(t1);
	}
	wake_up_process(t2);
	wake_up_process(t1);
	return 0;
}

void kthr_exit(void)
{
	pr_info("%s: counter %d\n",__func__, counter);
}

module_init(kthr_init);
module_exit(kthr_exit);

MODULE_AUTHOR("Samir Mulani");
MODULE_DESCRIPTION("Kernel Atomic macros");
MODULE_LICENSE("GPL");

