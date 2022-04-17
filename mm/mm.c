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
#include <linux/mm.h>
#include <linux/slab.h>

int init_module(void)
{
	unsigned char *ptr;
	unsigned char *pa;

	pr_info("%s : Invoked\n", __func__);
	pr_info("%s : Size of struct page = %ld\n", __func__, sizeof(struct page));
	pr_info("%s : Number of Pages = %ld\n", __func__, get_num_physpages());
	
	ptr = kmalloc(4096, GFP_KERNEL);
       	pr_info("%s : Value of ptr (Linear Address) = %p\n", __func__, ptr);
	
	pa = (unsigned char *) __pa(ptr);	
	
	pr_info("%s : Value of pa = %p\n", __func__, pa);
	strcpy(ptr, "data written to page\n");
	kfree(ptr);

	return -EINVAL;
}


void cleanup_module(void)
{
}

MODULE_AUTHOR("Samir Mulani");
MODULE_DESCRIPTION("Page allocation");
MODULE_LICENSE("Dual MIT/GPL");
