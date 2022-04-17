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
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/semaphore.h>
#include <linux/slab.h>

typedef struct {
	int a;
	int b;
} priv_data;

static priv_data *p;
struct task_struct *t1, *t2;

DEFINE_SEMAPHORE(sem);

int kthr_reader(void *arg)
{
	pr_info("%s: attempting to lock \n",__func__);
	down_interruptible(&sem); //lock
	pr_info("%s: read a = %d, b = %d\n", __func__, p->a, p->b);
	up(&sem);// unlock

	do_exit(0);
}

int kthr_writer(void *arg)
{
	down_interruptible(&sem);//lock
	p->a = 10;
	ssleep(10);
	p->b = 20;
	up(&sem);//unlock
	do_exit(0);
}

void data_init(priv_data * data)
{
	data->a = 0;
	data->b = 0;
}
int kthr_init(void)
{
	p = (priv_data *) kmalloc(sizeof(priv_data), GFP_KERNEL);

	data_init(p);

	t1 = kthread_run(kthr_writer, NULL, "Kwriter");
	if(IS_ERR(t1)){
                pr_err("%s: unable to start kernel thread\n",__func__);
                return PTR_ERR(t1);
        }

	t2 = kthread_run(kthr_reader, NULL, "Kreader");
	if(IS_ERR(t2)){
                pr_err("%s: unable to start kernel thread\n",__func__);
                return PTR_ERR(t2);
        }

	return 0;
}

void kthr_exit(void)
{
	kfree(p);
}

module_init(kthr_init);
module_exit(kthr_exit);

MODULE_AUTHOR("Samir Mulani");
MODULE_DESCRIPTION("Kernel semaphore Interface");
MODULE_LICENSE("Dual MIT/GPL");
