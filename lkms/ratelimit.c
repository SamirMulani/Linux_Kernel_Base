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
#include <linux/delay.h>
#include <linux/jiffies.h>

int rate_limit_init(void)
{
	int count;

	for (count = 0; count < 20; count++) {
		if (printk_ratelimit()) {
			pr_info("Printing %d", count);
		}
		
		else
		    ssleep(10);
		
	}

	return 0;
}

void rate_limit_exit(void)
{
	pr_info("Done");
}

module_init(rate_limit_init);
module_exit(rate_limit_exit);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("samir@linux.vnet.ibm.com");
MODULE_DESCRIPTION("printk rate limit");

