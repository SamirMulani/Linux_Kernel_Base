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

#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/slab.h>
#include<linux/gfp.h>
#include<linux/types.h>
#include <asm/atomic.h>

#define SUCCESS 0
#define DRVNAME "drv_privpool"

struct kmem_cache *my_cache;

typedef struct {
	u32 devid;
	s8 *name;
	u16 irq;
} drv_priv;

drv_priv *handle;

/*
 * cache constructor routine:
 * invoked with address of each object as parameter, 
 * This call gets invoked whenever new objects are setup.
 */
static void cache_init(void *data)
{
	drv_priv *mydata = (drv_priv *) data;
	mydata->devid = 0xff;
	mydata->name = DRVNAME;
	mydata->irq = 21;
}
/* 
 * cache specific derived object allocator / de-allocator routines
 * use these routines from rest of driver code for alloc/dealloc ops.
 */
static drv_priv *myalloc(void)
{
	drv_priv *mydata;
	mydata = (drv_priv *) kmem_cache_alloc(my_cache, GFP_KERNEL);
	return mydata;
}

static void myfree(drv_priv * free)
{
	kmem_cache_free(my_cache, free);
	pr_info("%s: myfree invoked\n",__func__);
}

static int __init myinit(void)
{

	/* create  private slab cache list and align it with hardware cache */
	my_cache =
	    kmem_cache_create("mycache", sizeof(drv_priv), 0,
			      SLAB_HWCACHE_ALIGN, cache_init);
	if (my_cache == NULL)
		return -ENOMEM;

	/* Alloc an object from list */
	handle = myalloc();
	pr_info(" private data state object.a=%d object.b=%s object.c=%d",
		handle->devid, handle->name, handle->irq);

	/* release the object */
	myfree(handle);

	return SUCCESS;
}

static void __exit myexit(void)
{
	/* delete cache list */
	kmem_cache_destroy(my_cache);
}

module_init(myinit);
module_exit(myexit);

MODULE_AUTHOR("Samir Mulani");
MODULE_DESCRIPTION("LKD_CW: cache allocations");
MODULE_LICENSE("Dual MIT/GPL");
