/*
 * Copyright (C) 2009 Marvell Technology Group Ltd.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; see the file COPYING. If not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Test read and write speed of a file system.
 *
 * Author: Jisheng Zhang <jszhang@marvell.com>
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/version.h>

#define PRINT_PREF KERN_INFO "<fsspeed> "
#define DNAMESIZE 32
static char fname[DNAMESIZE];
static int count;
module_param(count, int, S_IRUGO);
module_param_string(fname, fname, DNAMESIZE, 0600);
MODULE_PARM_DESC(fname, "fname=\"name1\"");
MODULE_PARM_DESC(count, "count*PAGE_SIZE Byte to read and write");

static char *iobuf;

static struct timeval start, finish;
static unsigned long next = 1;

static inline unsigned int simple_rand(void)
{
	next = next * 1103515245 + 12345;
	return (unsigned int)((next / 65536) % 32768);
}

static inline void simple_srand(unsigned long seed)
{
	next = seed;
}

static void set_random_data(unsigned char *buf, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		buf[i] = simple_rand();
}

static inline void start_timing(void)
{
	do_gettimeofday(&start);
}

static inline void stop_timing(void)
{
	do_gettimeofday(&finish);
}

static long calc_speed(void)
{
	long ms, k, speed;

	ms = (finish.tv_sec - start.tv_sec) * 1000 +
	     (finish.tv_usec - start.tv_usec) / 1000;
	k = count * PAGE_SIZE / 1024;
	speed = (k * 1000) / ms;
	return speed;
}

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,29))
static int vfs_fsync(struct file *file, struct dentry *dentry, int datasync)
{
	const struct file_operations *fop;
	struct address_space *mapping;
	int err, ret;

	/*
	 * Get mapping and operations from the file in case we have
	 * as file, or get the default values for them in case we
	 * don't have a struct file available.  Damn nfsd..
	 */
	if (file) {
		mapping = file->f_mapping;
		fop = file->f_op;
	} else {
		mapping = dentry->d_inode->i_mapping;
		fop = dentry->d_inode->i_fop;
	}

	if (!fop || !fop->fsync) {
		ret = -EINVAL;
		goto out;
	}

	ret = filemap_fdatawrite(mapping);

	/*
	 * We need to protect against concurrent writers, which could cause
	 * livelocks in fsync_buffers_list().
	 */
	mutex_lock(&mapping->host->i_mutex);
	err = fop->fsync(file, dentry, datasync);
	if (!ret)
		ret = err;
	mutex_unlock(&mapping->host->i_mutex);
	err = filemap_fdatawait(mapping);
	if (!ret)
		ret = err;
out:
	return ret;
}
#endif

static int __init fsspeed_init(void)
{
	int err, i;
	long speed;
	struct file *fp;
	mm_segment_t fs;

	printk(KERN_INFO "\n");
	printk(KERN_INFO "=================================================\n");
	printk(PRINT_PREF "rw %d PAGES using file: %s\n", count, fname);

	err = -ENOMEM;
	iobuf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!iobuf) {
		printk(PRINT_PREF "error: cannot allocate memory\n");
		goto out;
	}

	simple_srand(1);
	set_random_data(iobuf, PAGE_SIZE);

	fp = filp_open(fname, O_RDWR|O_CREAT, 0600);
	if (IS_ERR(fp)) {
		printk("open file %s failed.\n", fname);
		err = PTR_ERR(fp);
		goto out;
	}

	/* Write all eraseblocks, 1 eraseblock at a time */
	printk(PRINT_PREF "testing file system write speed\n");
	fs = get_fs();
	set_fs(KERNEL_DS);
	start_timing();
	for (i = 0; i < count; ++i) {
		err = vfs_write(fp, iobuf, PAGE_SIZE, &fp->f_pos);
		if (err < 0)
			goto out2;
//		cond_resched();
	}
	stop_timing();
	speed = calc_speed();
	printk(PRINT_PREF "write speed is %ld KiB/s\n", speed);
	
	vfs_fsync(fp, fp->f_path.dentry, 0);
	invalidate_mapping_pages(fp->f_dentry->d_inode->i_mapping, 0, -1);
	vfs_llseek(fp, 0, SEEK_SET);
	
	/* Read all eraseblocks, 1 eraseblock at a time */
	printk(PRINT_PREF "testing file system read speed\n");
	start_timing();
	for (i = 0; i < count; ++i) {
		err = vfs_read(fp, iobuf, PAGE_SIZE, &fp->f_pos);
		if (err < 0)
			goto out2;
//		cond_resched();
	}
	stop_timing();
	speed = calc_speed();
	printk(PRINT_PREF "read speed is %ld KiB/s\n", speed);

	printk(PRINT_PREF "finished\n");
	err = 0;
out2:
	filp_close(fp, NULL);
	set_fs(fs);
out:
	kfree(iobuf);
	if (err)
		printk(PRINT_PREF "error %d occurred\n", err);
	printk(KERN_INFO "=================================================\n");
	return err;
}
module_init(fsspeed_init);

static void __exit fsspeed_exit(void)
{
	return;
}
module_exit(fsspeed_exit);

MODULE_DESCRIPTION("File system speed test module");
MODULE_AUTHOR("Jisheng Zhang");
MODULE_LICENSE("GPL");
