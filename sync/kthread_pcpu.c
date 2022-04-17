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
#include <linux/init.h>
#include <linux/percpu.h>
#include <linux/cpumask.h>

#define MODNAME "[PERCPU] "

static DEFINE_PER_CPU(long, cpuvar) = 10;
static long __percpu *cpualloc;

static int __init pcpu_init(void)
{
	int cpu;

	pr_info(MODNAME"module loaded at 0x%p\n", pcpu_init);

	/* modify the cpuvar value */
	for_each_possible_cpu(cpu)
		per_cpu(cpuvar, cpu) = 15;

	pr_info(MODNAME"cpuvar on cpu%d  = %ld\n",smp_processor_id(), get_cpu_var(cpuvar)++);
	put_cpu_var(cpuvar);

	/* alloc a percpu value */
	cpualloc = alloc_percpu(long);

	/* set all cpu for this value */
	for_each_possible_cpu(cpu)
		*per_cpu_ptr(cpualloc, cpu) = 100;

	return 0;
}

static void __exit pcpu_exit(void)
{
	int cpu;
	pr_info(MODNAME"exit module...\n");

	for_each_possible_cpu(cpu)
		pr_info("cpuvar cpu%d = %ld\n", cpu, per_cpu(cpuvar, cpu));

	pr_info(MODNAME"cpualloc = %ld\n", *per_cpu_ptr(cpualloc, smp_processor_id()));
	free_percpu(cpualloc);

	pr_info(MODNAME"module unloaded from 0x%p\n", pcpu_exit);
}

module_init(pcpu_init);
module_exit(pcpu_exit);

MODULE_AUTHOR("Samir Mulani");
MODULE_DESCRIPTION("percpu data");
MODULE_LICENSE("Dual MIT/GPL");
