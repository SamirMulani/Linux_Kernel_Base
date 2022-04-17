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
#include <linux/semaphore.h>    
#include <linux/delay.h>	

/*
 * data package passed to threads
 */
struct kthr_data {
        const char *name;       /* kthread's name */
        wait_queue_head_t wqh;  /* kthread waits on this wq */
        int cond;               /* condidition associated with wq*/
        struct kthr_data *other;
};

static struct kthr_data prod, cons;
static struct task_struct *pthr, *cthr;

int prod_fct(void *data)
{
	struct kthr_data *pdata = (struct kthr_data*)data;
	while(1) {

		wait_event_interruptible(pdata->wqh, pdata->cond);
                pdata->cond = 0;
		pr_info("%s:producer woken up,Generating data...\n", __func__);
		mdelay(100);
		msleep(1);

		pdata->other->cond = 1;
		pr_info("%s: wake up consumer\n",__func__);
                wake_up_interruptible(&pdata->other->wqh);
		if (kthread_should_stop())
			break;
	}
	return 0;
}

int cons_fct(void *data)
{
        struct kthr_data *pdata = (struct kthr_data*)data;
        while(1) {

		wait_event_interruptible(pdata->wqh, pdata->cond);
                pdata->cond = 0;
                pr_info("%s: wake up event from producer,starting to consume...\n", __func__);
                mdelay(100);
		msleep(1);

		pdata->other->cond = 1;
                pr_info("%s: Done consuming , notify producer & wait for next chunck\n",__func__);
		wake_up_interruptible(&pdata->other->wqh);
                if (kthread_should_stop())
                        break;
        }
        return 0;
}

int __init kthr_init(void)
{

	init_waitqueue_head(&prod.wqh);
        init_waitqueue_head(&cons.wqh);

	prod.name = "producer"; 
	cons.name = "consumer";
	prod.cond = 1;
        cons.cond = 0;
        prod.other = &cons;
        cons.other =&prod;


	pthr = kthread_run(prod_fct, &prod,"producer");
	if(IS_ERR(pthr)){
                pr_err("%s: unable to start kernel thread\n",__func__);
                return PTR_ERR(pthr);
        }

	cthr = kthread_run(cons_fct, &cons,"consumer");
	if(IS_ERR(cthr)){
                pr_err("%s: unable to start kernel thread\n",__func__);
                return PTR_ERR(cthr);
        }

	return 0;
}

void __exit kthr_exit(void)
{
	kthread_stop(pthr);
	kthread_stop(cthr);
}

module_init(kthr_init);
module_exit(kthr_exit);

MODULE_AUTHOR("Samir Mulani");
MODULE_DESCRIPTION("sync with kernel semaphores");
MODULE_LICENSE("GPL");

