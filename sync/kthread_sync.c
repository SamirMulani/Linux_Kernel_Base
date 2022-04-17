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
	const char *name;	/* kthread's name */
	struct semaphore *sem1; /* kthread waits for this semaphore */
	struct semaphore *sem2; /* kthread posts this sempahore */ 
};

static struct kthr_data prod, cons;
static struct semaphore psem, csem;
static struct task_struct *pthr, *cthr;

int prod_fct(void *data)
{
	struct kthr_data *pdata = (struct kthr_data*)data;
	while(1) {
		down(pdata->sem1); // wait for consumer
		pr_info("%s: Generating data\n", pdata->name);
		mdelay(100);
		pr_info("%s: notify consumer\n",pdata->name);
		up(pdata->sem2);// notify consumer
		if (kthread_should_stop())
			break;
	}
	return 0;
}

int cons_fct(void *data)
{
        struct kthr_data *pdata = (struct kthr_data*)data;
        while(1) {
                down(pdata->sem1);// wait for producer
                pr_info("%s: signal recvd form producer\n", pdata->name);
                mdelay(100);
                pr_info("%s: Done consuming, notify producer & wait for next chunck\n",pdata->name);
                up(pdata->sem2); // notify producer
                if (kthread_should_stop())
                        break;
        }
        return 0;
}

int __init kthr_init(void)
{
	sema_init(&psem, 1); 
	sema_init(&csem, 0);

	prod.name = "producer"; 
	cons.name = "consumer";

	prod.sem1 = &psem; 
	prod.sem2 = &csem;

	cons.sem1 = &csem; 
	cons.sem2 = &psem;

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
MODULE_LICENSE("Dual MIT/GPL");

