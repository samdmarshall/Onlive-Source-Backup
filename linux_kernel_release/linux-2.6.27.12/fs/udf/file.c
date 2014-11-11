/*
 * file.c
 *
 * PURPOSE
 *  File handling routines for the OSTA-UDF(tm) filesystem.
 *
 * COPYRIGHT
 *  This file is distributed under the terms of the GNU General Public
 *  License (GPL). Copies of the GPL can be obtained from:
 *    ftp://prep.ai.mit.edu/pub/gnu/GPL
 *  Each contributing author retains all rights to their own work.
 *
 *  (C) 1998-1999 Dave Boynton
 *  (C) 1998-2004 Ben Fennema
 *  (C) 1999-2000 Stelias Computing Inc
 *
 * HISTORY
 *
 *  10/02/98 dgb  Attempt to integrate into udf.o
 *  10/07/98      Switched to using generic_readpage, etc., like isofs
 *                And it works!
 *  12/06/98 blf  Added udf_file_read. uses generic_file_read for all cases but
 *                ICBTAG_FLAG_AD_IN_ICB.
 *  04/06/99      64 bit file handling on 32 bit systems taken from ext2 file.c
 *  05/12/99      Preliminary file write support
 */

#include "udfdecl.h"
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/string.h> /* memset */
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/smp_lock.h>
#include <linux/pagemap.h>
#include <linux/buffer_head.h>
#include <linux/aio.h>
#ifdef CONFIG_MV88DE3010_BERLIN_UDF_SPEEDUP_TRACK_SEEKING
#include <linux/cdrom.h>
#include <linux/fsnotify.h>
#endif
#include "udf_i.h"
#include "udf_sb.h"

static int udf_adinicb_readpage(struct file *file, struct page *page)
{
	struct inode *inode = page->mapping->host;
	char *kaddr;
	struct udf_inode_info *iinfo = UDF_I(inode);

	BUG_ON(!PageLocked(page));

	kaddr = kmap(page);
	memset(kaddr, 0, PAGE_CACHE_SIZE);
	memcpy(kaddr, iinfo->i_ext.i_data + iinfo->i_lenEAttr, inode->i_size);
	flush_dcache_page(page);
	SetPageUptodate(page);
	kunmap(page);
	unlock_page(page);

	return 0;
}

static int udf_adinicb_writepage(struct page *page,
				 struct writeback_control *wbc)
{
	struct inode *inode = page->mapping->host;
	char *kaddr;
	struct udf_inode_info *iinfo = UDF_I(inode);

	BUG_ON(!PageLocked(page));

	kaddr = kmap(page);
	memcpy(iinfo->i_ext.i_data + iinfo->i_lenEAttr, kaddr, inode->i_size);
	mark_inode_dirty(inode);
	SetPageUptodate(page);
	kunmap(page);
	unlock_page(page);

	return 0;
}

static int udf_adinicb_write_end(struct file *file,
			struct address_space *mapping,
			loff_t pos, unsigned len, unsigned copied,
			struct page *page, void *fsdata)
{
	struct inode *inode = mapping->host;
	unsigned offset = pos & (PAGE_CACHE_SIZE - 1);
	char *kaddr;
	struct udf_inode_info *iinfo = UDF_I(inode);

	kaddr = kmap_atomic(page, KM_USER0);
	memcpy(iinfo->i_ext.i_data + iinfo->i_lenEAttr + offset,
		kaddr + offset, copied);
	kunmap_atomic(kaddr, KM_USER0);

	return simple_write_end(file, mapping, pos, len, copied, page, fsdata);
}

const struct address_space_operations udf_adinicb_aops = {
	.readpage	= udf_adinicb_readpage,
	.writepage	= udf_adinicb_writepage,
	.sync_page	= block_sync_page,
	.write_begin = simple_write_begin,
	.write_end = udf_adinicb_write_end,
};

static ssize_t udf_file_aio_write(struct kiocb *iocb, const struct iovec *iov,
				  unsigned long nr_segs, loff_t ppos)
{
	ssize_t retval;
	struct file *file = iocb->ki_filp;
	struct inode *inode = file->f_path.dentry->d_inode;
	int err, pos;
	size_t count = iocb->ki_left;
	struct udf_inode_info *iinfo = UDF_I(inode);

	if (iinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB) {
		if (file->f_flags & O_APPEND)
			pos = inode->i_size;
		else
			pos = ppos;

		if (inode->i_sb->s_blocksize <
				(udf_file_entry_alloc_offset(inode) +
						pos + count)) {
			udf_expand_file_adinicb(inode, pos + count, &err);
			if (iinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB) {
				udf_debug("udf_expand_adinicb: err=%d\n", err);
				return err;
			}
		} else {
			if (pos + count > inode->i_size)
				iinfo->i_lenAlloc = pos + count;
			else
				iinfo->i_lenAlloc = inode->i_size;
		}
	}

	retval = generic_file_aio_write(iocb, iov, nr_segs, ppos);
	if (retval > 0)
		mark_inode_dirty(inode);

	return retval;
}
int udf_ioctl(struct inode *inode, struct file *filp, unsigned int cmd,
	      unsigned long arg)
{
	long old_block, new_block;
#ifdef CONFIG_MV88DE3010_BERLIN_UDF_GET_LBA4FILE
	struct super_block *i_super = inode->i_sb;
	sector_t iblock,phys;
#endif

	int result = -EINVAL;

	if (file_permission(filp, MAY_READ) != 0) {
		udf_debug("no permission to access inode %lu\n",
			  inode->i_ino);
		return -EPERM;
	}

	if (!arg) {
		udf_debug("invalid argument to udf_ioctl\n");
		return -EINVAL;
	}

	switch (cmd) {
	case UDF_GETVOLIDENT:
		if (copy_to_user((char __user *)arg,
				 UDF_SB(inode->i_sb)->s_volume_ident, 32))
			return -EFAULT;
		else
			return 0;
	case UDF_RELOCATE_BLOCKS:
		if (!capable(CAP_SYS_ADMIN))
			return -EACCES;
		if (get_user(old_block, (long __user *)arg))
			return -EFAULT;
		result = udf_relocate_blocks(inode->i_sb,
						old_block, &new_block);
		if (result == 0)
			result = put_user(new_block, (long __user *)arg);
		return result;
	case UDF_GETEASIZE:
		result = put_user(UDF_I(inode)->i_lenEAttr, (int __user *)arg);
		break;
	case UDF_GETEABLOCK:
		result = copy_to_user((char __user *)arg,
				      UDF_I(inode)->i_ext.i_data,
				      UDF_I(inode)->i_lenEAttr) ? -EFAULT : 0;
		break;
#ifdef CONFIG_MV88DE3010_BERLIN_UDF_GET_LBA4FILE
	case UDF_GETFILELBA:
		iblock = (sector_t)(filp->f_pos >> (loff_t)i_super->s_blocksize_bits);
		phys = udf_block_map(inode, iblock);
		result = put_user(phys, (int __user *)arg);
		break;
#endif
	}

	return result;
}

static int udf_release_file(struct inode *inode, struct file *filp)
{
	if (filp->f_mode & FMODE_WRITE) {
		lock_kernel();
		udf_discard_prealloc(inode);
		unlock_kernel();
	}
	return 0;
}
#ifdef CONFIG_MV88DE3010_BERLIN_UDF_SPEEDUP_TRACK_SEEKING
loff_t mv_udf_speedup_file_llseek_unlocked(struct file *file, loff_t offset, int origin)
{
	loff_t retval,oldpos;
	struct inode *inode = file->f_mapping->host;
	struct super_block	*i_super = inode->i_sb;
	sector_t phys = 0;
	sector_t iblock, lblock;
	loff_t  blocksize;
	switch (origin) {
		case SEEK_END:
			offset += inode->i_size;
			break;
		case SEEK_CUR:
			offset += file->f_pos;
	}
	
	retval = -EINVAL;
	oldpos = file->f_pos;
	if (offset>=0 && offset<=inode->i_sb->s_maxbytes) {
		/* Special lock needed here? */
		if (offset != file->f_pos) {
			file->f_pos = offset;
			file->f_version = 0;
		}
		retval = offset;
	}
	if (retval >= 0 && oldpos != file->f_pos) {
		blocksize = i_super->s_blocksize;
		
		iblock = (sector_t)( retval >> (loff_t)i_super->s_blocksize_bits);
		lblock = (i_size_read(inode)+blocksize-1) >> i_super->s_blocksize_bits;
		if (iblock < lblock) {
			phys = udf_block_map(inode, iblock);
			if (phys) {
				struct block_device *bdev = inode->i_sb->s_bdev;	
				struct cdrom_generic_command cgc;
				unsigned char cmd_desc[12];
				int error;
				struct request_sense s;
				iblock = phys;
				memset(&cgc, 0, sizeof(cgc));
				memset(&s, 0, sizeof(s));
				#if 0
				cmd_desc[0] = GPCMD_SET_READ_AHEAD;
				cmd_desc[2] = (tiggerblock >> 24) & 0xff;
				cmd_desc[3] = (tiggerblock >> 16)& 0xff;
				cmd_desc[4] = (tiggerblock >> 8)& 0xff;
				cmd_desc[5] = (tiggerblock)& 0xff;
				cmd_desc[6] = (iblock >> 24)& 0xff;
				cmd_desc[7] = (iblock >> 16)& 0xff;
				cmd_desc[8] = (iblock >> 8)& 0xff;
				cmd_desc[9] = (iblock )& 0xff;
				cmd_desc[10] = 0;
				cmd_desc[11] = 0;
				#endif
				memset(&cmd_desc[0],0,12);
				cmd_desc[0] = GPCMD_READ_10;
				cmd_desc[2] = (iblock >> 24) & 0xff;
				cmd_desc[3] = (iblock >> 16)& 0xff;
				cmd_desc[4] = (iblock >> 8)& 0xff;
				cmd_desc[5] = (iblock)& 0xff;
				memcpy(cgc.cmd, cmd_desc, 12);
				cgc.buffer      = NULL;
				cgc.buflen      = 0;
				cgc.data_direction  = CGC_DATA_READ;
				cgc.sense = &s;
				cgc.timeout = 100000;	
				cgc.quiet = 0;
				error = ioctl_by_bdev(bdev, CDROM_SEND_PACKET, &cgc);
				if(error)
					galois_printk("udf llseek error  0x%x\n",error);
#if 0	
				char dev_name[32];
			    unsigned char cmd_desc[12];
				struct cdrom_generic_command cgc;
				struct request_sense s;
				sector_t tiggerblock;	
				dev_name[0] = 0;
				tiggerblock = iblock - 1;
				sprintf(dev_name,"/dev/scd%d",MINOR(inode->i_sb->s_dev));
				struct file *filp =  do_filp_open(AT_FDCWD,dev_name,O_RDONLY,0);
				struct files_struct *files = current->files;
				if(IS_ERR(filp)) {
					galois_printk("do_filp_open error in udf\n");
					goto dirout;
				} else {
					int error;
					fsnotify_open(filp->f_path.dentry);
					if (!filp->f_op)
						goto out;
					memset(&cgc, 0, sizeof(cgc));
					memset(&s, 0, sizeof(s));

					cmd_desc[0] = GPCMD_SET_READ_AHEAD;
					cmd_desc[2] = tiggerblock & 0xff;
					cmd_desc[3] = (tiggerblock >> 8)& 0xff;
					cmd_desc[4] = (tiggerblock >> 16)& 0xff;
					cmd_desc[5] = (tiggerblock >> 24)& 0xff;
					cmd_desc[6] = iblock & 0xff;
					cmd_desc[7] = (iblock >> 8)& 0xff;
					cmd_desc[8] = (iblock >> 16)& 0xff;
					cmd_desc[9] = (iblock >> 24)& 0xff;
					memcpy(cgc.cmd, cmd_desc, 12);
					cgc.buffer      = NULL;
					cgc.buflen      = 0;
					cgc.data_direction  = CGC_DATA_READ;
					cgc.sense = &s;
					cgc.timeout = 100000;
					cgc.quiet = 0;

					if (filp->f_op->unlocked_ioctl) {
						error = filp->f_op->unlocked_ioctl(filp, CDROM_SEND_PACKET, &cgc);
						if(error)
							galois_printk("udf filp->f_op->unlocked_ioctl error =0x%x\n",error);
						if (error == -ENOIOCTLCMD)
							error = -EINVAL;
						goto out;
					} else if (filp->f_op->ioctl) {
						lock_kernel();
						error = filp->f_op->ioctl(filp->f_path.dentry->d_inode,filp, CDROM_SEND_PACKET, &cgc);
						if(error) 
							galois_printk("udf filp->f_op->ioctl error = 0x%x\n",error);
						unlock_kernel();
					}
				}
out:
				galois_printk("ioctl finish\n");
				filp_close(filp,files);
			}
		}
	
#endif
			}
		}
	}
	return retval;
}


loff_t mv_udf_speedup_file_llseek(struct file *file, loff_t offset, int origin)
{
	loff_t n;
	mutex_lock(&file->f_dentry->d_inode->i_mutex);
	n = mv_udf_speedup_file_llseek_unlocked(file, offset, origin);
	mutex_unlock(&file->f_dentry->d_inode->i_mutex);
	return n;
}
#endif
const struct file_operations udf_file_operations = {
	.read			= do_sync_read,
	.aio_read		= generic_file_aio_read,
	.ioctl			= udf_ioctl,
	.open			= generic_file_open,
	.mmap			= generic_file_mmap,
	.write			= do_sync_write,
	.aio_write		= udf_file_aio_write,
	.release		= udf_release_file,
	.fsync			= udf_fsync_file,
	.splice_read		= generic_file_splice_read,
#ifdef CONFIG_MV88DE3010_BERLIN_UDF_SPEEDUP_TRACK_SEEKING
	.llseek			= mv_udf_speedup_file_llseek,
#else
	.llseek			= generic_file_llseek,
#endif
};

const struct inode_operations udf_file_inode_operations = {
	.truncate = udf_truncate,
};
