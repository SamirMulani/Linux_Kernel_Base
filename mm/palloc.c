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
#include <linux/gfp.h>
#include <linux/printk.h>

struct page *pg;
char *ptr;
int init_module(void)
{
	pg = alloc_page(GFP_KERNEL);	
	if(!pg) 
		return -ENOMEM;
	
	ptr = page_address(pg); 	
	strcpy(ptr, "Hello World\n");	
	
	pr_info("%s : Page Addr = %p\n", __func__, pg);	
	pr_info("%s : Page to Frame Number = %ld\n", __func__, page_to_pfn(pg));	
	pr_info("%s : Frame to page Number = %p\n", __func__, pfn_to_page(page_to_pfn(pg)));	
	pr_info("%s : %p : %s\n", __func__, ptr, ptr);	
	free_page((unsigned long)ptr);
	return -EINVAL;
}


void cleanup_module(void)
{
	pr_info("%s : Invoked\n", __func__);
	
}

MODULE_AUTHOR("Samir Mulani");
MODULE_DESCRIPTION("page allocations");
MODULE_LICENSE("Dual MIT/GPL");
