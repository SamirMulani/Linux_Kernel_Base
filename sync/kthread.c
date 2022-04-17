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
#include <linux/kthread.h>	
#include <linux/sched.h>	
#include <linux/delay.h>	

struct task_struct *ts;

int Kthread_start(void *data)
{
	while(1) {
		pr_info("%s: Thread start routine\n",__func__);
		/* Introduce delay to simulate some work */
		msleep(500);
		if (kthread_should_stop())
			break;
	}
	return 0;
}

int __init kthr_init(void)
{
	ts = kthread_create(Kthread_start, NULL, "Mykthread");
	if(IS_ERR(ts)){
                pr_err("%s: unable to start kernel thread\n",__func__);
                return PTR_ERR(ts);
        }
	/* start kthread execution */
        wake_up_process(ts);
	
	pr_info("%s:Kthread created with id %d\n", __func__, ts->pid);
	return 0;
}

void __exit kthr_exit(void)
{
	kthread_stop(ts);
}

module_init(kthr_init);
module_exit(kthr_exit);

MODULE_AUTHOR("Samir Mulani");
MODULE_DESCRIPTION("Simple Kthread");
MODULE_LICENSE("GPL");

