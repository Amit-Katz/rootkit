#include <linux/ftrace.h>
#include <linux/linkage.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/kprobes.h>

typedef unsigned long m_address;

static struct kprobe kp = {.symbol_name = "kallsyms_lookup_name"};

#define USE_FENTRY_OFFSET 0

#if !USE_FENTRY_OFFSET
#pragma GCC optimize("-fno-optimize-sibling-calls")
#endif

struct ftrace_hook
{
    const char *name;
    void *function;
    void *original;

    m_address address;
    struct ftrace_ops ops;
};

static int fh_resolve_hook_address(struct ftrace_hook *hook)
{
    typedef m_address (*kallsyms_lookup_name_t)(const char *name);
    kallsyms_lookup_name_t kallsyms_lookup_name;

    register_kprobe(&kp);
    kallsyms_lookup_name = (kallsyms_lookup_name_t)kp.addr;
    unregister_kprobe(&kp);

    hook->address = kallsyms_lookup_name(hook->name);

    if (hook->address == 0)
        return -ENOENT;

    *((m_address *)hook->original) = hook->address;

#if USE_FENTRY_OFFSET
    *((m_address *)hook->original) += MCOUNT_INSN_SIZE;
#endif

    return 0;
}

static void notrace fh_ftrace_thunk(m_address ip, m_address parent_ip, struct ftrace_ops *ops, struct ftrace_regs *regs)
{
    struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);

    if (!within_module(parent_ip, THIS_MODULE))
    {
        regs->regs.ip = (m_address)hook->function;
    }
}

int fh_install_hook(struct ftrace_hook *hook)
{
    int err = fh_resolve_hook_address(hook);

    if (err)
        return err;

    hook->ops.func = fh_ftrace_thunk;
    hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_RECURSION | FTRACE_OPS_FL_IPMODIFY;

    err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
    if (err)
        return err;

    err = register_ftrace_function(&hook->ops);
    if (err)
        return err;

    return 0;
}

void fh_remove_hook(struct ftrace_hook *hook)
{
    unregister_ftrace_function(&hook->ops);
    ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
}
