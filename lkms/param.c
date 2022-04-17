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
#include <linux/moduleparam.h>


static int base_port[5];	/* array of integers, minimum 1, max. 5 */
static int irq;			/* single integer */

static char *name = 0;		/* string name, passed as a char pointer */


module_param_array (base_port, int, NULL, 0);
MODULE_PARM_DESC (base_port,"Int. base_port=v1,v2,... Min 1, Max 5 values");

module_param (irq, int, 0644);
MODULE_PARM_DESC (irq, "Integer");

module_param (name, charp, 0644);
MODULE_PARM_DESC (name, "String. Device Driver Name");


int init_module (void)
{
	int i;
	pr_info ("IRQ=%d, Device Name=%s\n", irq, name);
	for (i = 0; i < 5; ++i)
		pr_info ("Base Port %d = %x\n", i + 1, base_port[i]);
	return 0;
}

void cleanup_module (void)
{
        pr_info ("IRQ=%d, Device Name=%s", irq, name);
	pr_info ("Unloading");
}


MODULE_AUTHOR ("samir@linux.vnet.ibm.com");
MODULE_DESCRIPTION ("LKD_CW: module params");
MODULE_LICENSE ("Dual MIT/GPL");
