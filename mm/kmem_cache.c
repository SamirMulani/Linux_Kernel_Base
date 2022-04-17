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
#include <linux/slab.h>
#include <linux/gfp.h>

struct kmem_cache *mycache;
void *object;

int kmem_init(void)
{

       mycache = kmem_cache_create("test_cache", 128, 0, SLAB_POISON, NULL);
       if (!mycache)
	    return -ENOMEM;
	
	pr_info("cache size %d\n", kmem_cache_size(mycache));

	object = kmem_cache_alloc(mycache, GFP_KERNEL);
	 if (!object)
                return -ENOMEM;

	pr_info("hexdump after alloc\n");
        print_hex_dump_bytes("kptr: ", DUMP_PREFIX_OFFSET, object, 136);

	return 0;
}

void kmem_exit(void)
{
	kmem_cache_free(mycache, object);
	kmem_cache_destroy(mycache);
}

module_init(kmem_init);
module_exit(kmem_exit);

MODULE_AUTHOR("Samir Mulani");
MODULE_DESCRIPTION("LKD_CW: cache");
MODULE_LICENSE("Dual MIT/GPL");
