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
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/completion.h>

#define MODNAME "[SYNC_COMP]: "

/* shared data: */
unsigned int counter;

struct completion *comp;
struct task_struct *read_thread, *write_thread;

static int writer_function(void *data)
{
	while (counter != 1234)
		counter++;
	complete(comp); //wakeup
	do_exit(0);
}

static int read_function(void *data)
{
	wait_for_completion(comp); //wait
	pr_info(MODNAME "counter: %d\n", counter);
	do_exit(0);
}

static int __init my_mod_init(void)
{
	counter = 0;
	comp = kmalloc(sizeof(struct completion), GFP_KERNEL);
	if (!comp)
		return -1;
	init_completion(comp);
	read_thread = kthread_run(read_function, NULL, "read-thread");
	write_thread = kthread_run(writer_function, NULL, "write-thread");
	return 0;
}

static void __exit my_mod_exit(void)
{
	kfree(comp);
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_AUTHOR("Samir Mulani");
MODULE_DESCRIPTION("completions");
MODULE_LICENSE("Dual MIT/GPL");
