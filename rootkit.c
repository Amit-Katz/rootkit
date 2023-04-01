#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Amit Katz");
MODULE_DESCRIPTION("A simple Linux kernel module");
MODULE_VERSION("1.0");

static int __init rootkit_init(void)
{
    printk(KERN_INFO "Hello World!\n");
    return 0;
}

static void __exit rootkit_exit(void)
{
}

module_init(rootkit_init);
module_exit(rootkit_exit);
