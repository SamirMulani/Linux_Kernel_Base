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
#include <linux/slab.h>

static int inc=500;
module_param(inc, int, 0);
MODULE_PARM_DESC(inc, "Number of bytes to increment for each attempt");

const char *hdr=
"[1] kmalloc(n)   :[2]actual: wastage : %age\n"
"      bytes      :   bytes :  [2-1]  : waste\n";

static int  test_init(void)
{
	int num=20, actual_alloc=0;
	void *p;
	pr_info("%s", hdr);

	while (1) {
		p = kmalloc(num, GFP_KERNEL);
		if (!p) {
			pr_alert("kmalloc fail, num=%d\n", num);
			return -ENOMEM;
		}
		actual_alloc = ksize(p);
		pr_info("kmalloc(%7d) : %7d : %7d : %3d%%\n",
			num, actual_alloc, actual_alloc-num, (((actual_alloc-num)*100/num)));
		kfree(p);
		num += inc;
	}
	return 0; // success
}

static void test_exit(void)
{
}

module_init(test_init);
module_exit(test_exit);


MODULE_AUTHOR("Samir Mulani");
MODULE_DESCRIPTION("test case for slab allocations");
MODULE_LICENSE("Dual MIT/GPL");
