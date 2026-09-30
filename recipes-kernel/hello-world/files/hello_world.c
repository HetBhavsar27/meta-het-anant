// SPDX-License-Identifier: GPL-2.0-only

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define DRIVER_NAME "hello_world"

static int __init hello_world_init(void)
{
	pr_info("%s: Hello World from the Raspberry Pi 5 kernel module!\n",
		DRIVER_NAME);

	return 0;
}

static void __exit hello_world_exit(void)
{
	pr_info("%s: Goodbye from the Raspberry Pi 5 kernel module!\n",
		DRIVER_NAME);
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Het Bhavsar");
MODULE_DESCRIPTION("Simple Hello World kernel module for Raspberry Pi 5");
MODULE_VERSION("1.0");
