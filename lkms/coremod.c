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
#include<linux/module.h>
#include<linux/kernel.h>

static int veda_counter;

int set_counter(int a)
{
	veda_counter = a;
	return 0;
}

int get_counter(void)
{
	return veda_counter;
}

static int myinit(void)
{
	printk("intializing counter\n ");
	veda_counter = 100;
	return 0;
}

static void myexit(void)
{
	printk("value of veda_counter %d\n ", veda_counter);
}

module_init(myinit);
module_exit(myexit);

EXPORT_SYMBOL_GPL(set_counter);
EXPORT_SYMBOL(get_counter);

MODULE_AUTHOR ("Samir Mulani");
MODULE_DESCRIPTION ("LKD_CW: exporting symbols");
MODULE_LICENSE ("Dual MIT/GPL");
