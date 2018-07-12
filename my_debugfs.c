/*
 */
#include <linux/hash.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/stddef.h>
#include <linux/export.h>
#include <linux/moduleloader.h>
#include <linux/kallsyms.h>
#include <linux/freezer.h>
#include <linux/seq_file.h>
#include <linux/debugfs.h>
#include <linux/sysctl.h>
#include <linux/kdebug.h>
#include <linux/memory.h>
#include <linux/cpu.h>
#include <linux/jump_label.h>

#include <asm/sections.h>
#include <asm/cacheflush.h>
#include <asm/errno.h>
#include <linux/uaccess.h>


static int show_kprobe_addr(struct seq_file *pi, void *v)
{
	struct task_struct *task = current;

	seq_printf(pi, "pid = %d ",task->pid);
	return 0;
}

static const struct seq_operations sqh_seq_ops = {
	.show  = show_kprobe_addr
};

static int sqh_open(struct inode *inode, struct file *filp)
{
	return seq_open(filp, &sqh_seq_ops);
}

static const struct file_operations debugfs_sqh_operations = {
	.open           = sqh_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = seq_release,
};


static int __init debugfs_sqh_init(void)
{
	struct dentry *dir, *file;

	dir = debugfs_create_dir("sqh_debug", NULL);
	if (!dir)
		return -ENOMEM;

	file = debugfs_create_file("current_pid", 0444, dir, NULL,
				&debugfs_sqh_operations);
	if (!file)
		goto error;


	return 0;

error:
	debugfs_remove(dir);
	return -ENOMEM;
}

MODULE_LICENSE("GPL");
late_initcall(debugfs_sqh_init);
