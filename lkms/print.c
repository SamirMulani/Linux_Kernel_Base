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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
 
/*
  #define KERN_EMERG      KERN_SOH "0"     system is unusable 
  #define KERN_ALERT      KERN_SOH "1"     action must be taken immediately 
  #define KERN_CRIT       KERN_SOH "2"     critical conditions 
  #define KERN_ERR        KERN_SOH "3"     error conditions 
  #define KERN_WARNING    KERN_SOH "4"     warning conditions 
  #define KERN_NOTICE     KERN_SOH "5"     normal but significant condition 
  #define KERN_INFO       KERN_SOH "6"     informational 
  #define KERN_DEBUG      KERN_SOH "7"     debug-level messages 
*/


static __init int log_init(void)
{
	printk(KERN_EMERG "loglevel 0\n");
	printk(KERN_ALERT "loglevel 1\n");
	printk(KERN_CRIT "loglevel 2\n");
	printk(KERN_ERR "loglevel 3\n");
	printk(KERN_WARNING "loglevel 4\n");
	printk(KERN_NOTICE "loglevel 5\n");
	printk(KERN_INFO "loglevel 6\n");
	printk(KERN_DEBUG "loglevel 7\n");

	return 0;
}

static __exit void log_exit(void)
{
	pr_emerg("loglevel 0\n");
	pr_alert("loglevel 1\n");
	pr_crit("loglevel 2\n");
	pr_err("loglevel 3\n");
	pr_warn("loglevel 4\n");
	pr_notice("loglevel 5\n");
	pr_info("loglevel 6\n");
	pr_debug("loglevel 7\n");
}

module_init(log_init);
module_exit(log_exit);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("samir@linux.vnet.ibm.com");
MODULE_DESCRIPTION("printk log levels");
