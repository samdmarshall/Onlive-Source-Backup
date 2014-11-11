#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/mutex.h>
#include <asm/uaccess.h>
#include <mach/galois_generic.h>
#include <mach/spi_bus.h>

#include "Galois_memmap.h"
#include "galois_io.h"
#include "global.h"
#include "SysMgr.h"
#include "apb_perf_base.h"

#include "spi_master.h"

#define DBGPRINTK(...)
#define SPI_DEBUG

/*
 * Berlin has only GALOIS_SPIBUS1 
 */
#define GALOIS_SPIBUS0		0
#define GALOIS_SPIBUS1		1
#define GALOIS_SPIBUS2		2
#define GALOIS_SPIBUS_NUM	3

/* SPI device operation defines */
#define MAX_RW_COUNT 256 /* in byte */
#define SPI_SUB_LENGTH 4

static DEFINE_MUTEX(ll_spi_mutex);
galois_spi_info_t *galois_spi_info[GALOIS_SPIBUS_NUM];
int spi_opentime[GALOIS_SPIBUS_NUM];
static char spi_name[10];

static ssize_t parse_spi_info(galois_spi_info_t *spi_info)
{
	/* todo */
	return 0;
}

static ssize_t process_spi_cmd(galois_spi_info_t *spi_info, galois_spi_cmd_t *spi_cmd, int wr)
{
	uint *kbuf;
	uint sub_buf[SPI_SUB_LENGTH];
	int sub_len;
	ssize_t ret;
	int i;

	if (!spi_info->usable) {
		printk("error: usable = 0.\n");
		return -ENODEV;
	}
	if ((spi_cmd->count > MAX_RW_COUNT) || (spi_cmd->count < 1)) {
		printk("error: rw count %d is invalid.\n", spi_cmd->count);
		return -EINVAL;
	}

	switch (spi_info->xd_mode) {
		case SPIDEV_XDMODE0_WOA7D9:
			if (!wr) {
				printk("error: write only.\n");
				return -EINVAL;
			} else {
				int packet;
				int num;

				if (spi_cmd->count != 1) {
					printk("error: rw count %d too large.\n", spi_cmd->count);
					return -EINVAL;
				}
				if (copy_from_user((void *)sub_buf, (void __user *)spi_cmd->buf, 4)) {
					printk("error: copy_from_user failed\n");
					return -EFAULT;
				}

				packet = (spi_cmd->sub_cmd << 9) | (sub_buf[0] & 0x0000ffff);
				num = (spi_info->frame_width + 15) / (spi_info->frame_width);

				/* allocate the buffer to store data */
				kbuf = (uint *)kmalloc(num*4, GFP_KERNEL);
				if (kbuf == NULL) {
					printk("error: failed to kmalloc memory.\n");
					return -ENOMEM;
				}

				/* MSB first */
				for (i = 0; i < num; i++) {
					kbuf[num - 1 - i] = packet & ((1 << spi_info->frame_width) - 1);
					packet >>= spi_info->frame_width;
				}

				/* write out */
				ret = spi_master_halfduplex_writeread_frames(
						spi_info->mst_id,	/* master_id */
						spi_info->slave_id,	/* slave_addr */
						kbuf, num, 			/* pwbuf, wnum */
						NULL, 0);			/* prbuf, rnum */
				if (ret != SSI_OK) {
					kfree(kbuf);
					printk("error: spi_master_halfduplex_writeread_frames failed.\n");
					return -EACCES;
				}
			}
			break;
		case SPIDEV_XDMODE1_RWI8A24D8n:
			if (!wr) {
				if ((spi_cmd->sub_cmd & 0x00ffffff) == 0x00ffffff) {
					sub_buf[0] = spi_cmd->sub_cmd >> 24;
					sub_len = 1;
				} else {
					sub_buf[0] = spi_cmd->sub_cmd >> 24; /* flash op instruction */
					sub_buf[1] = (spi_cmd->sub_cmd >> 16) & 0xff; /* addr */
					sub_buf[2] = (spi_cmd->sub_cmd >> 8) & 0xff; /* addr */
					sub_buf[3] = spi_cmd->sub_cmd & 0xff; /* addr */
					sub_len = 4;
				}

				/* allocate the buffer to store data */
				kbuf = (uint *)kmalloc(spi_cmd->count*4, GFP_KERNEL);
				if (kbuf == NULL) {
					printk("error: failed to kmalloc memory.\n");
					return -ENOMEM;
				}

				/* issue rw */
				ret = spi_master_halfduplex_writeread_frames(
						spi_info->mst_id,	/* master_id */
						spi_info->slave_id,	/* slave_addr */
						sub_buf, sub_len, 		/* pwbuf, wnum */
						kbuf, spi_cmd->count);	/* prbuf, rnum */
				if (ret != SSI_OK) {
					kfree(kbuf);
					printk("error: spi_master_halfduplex_writeread_frames failed.\n");
					return -EACCES;
				}

				/* copy data to user */
				if (copy_to_user((void __user *)spi_cmd->buf, kbuf, spi_cmd->count*4)) {
					kfree(kbuf);
					printk("error: copy_to_user failed\n");
					return -EFAULT;
				}
			} else {
				/* allocate the buffer to store data */
				kbuf = (uint *)kmalloc((spi_cmd->count + SPI_SUB_LENGTH)*4, GFP_KERNEL);
				if (kbuf == NULL) {
					printk("error: failed to kmalloc memory.\n");
					return -ENOMEM;
				}

				sub_len = 0;
				if ((spi_cmd->sub_cmd & 0x00ffffff) == 0x00ffffff) {
					kbuf[sub_len++] = spi_cmd->sub_cmd >> 24;
				} else {
					kbuf[sub_len++] = spi_cmd->sub_cmd >> 24; /* flash op instruction */
					kbuf[sub_len++] = (spi_cmd->sub_cmd >> 16) & 0xff; /* addr */
					kbuf[sub_len++] = (spi_cmd->sub_cmd >> 8) & 0xff; /* addr */
					kbuf[sub_len++] = spi_cmd->sub_cmd & 0xff; /* addr */
				}

				if (copy_from_user((void *)(&kbuf[sub_len]), (void __user *)spi_cmd->buf, 
							spi_cmd->count*4)) {
					kfree(kbuf);
					printk("error: copy_from_user failed\n");
					return -EFAULT;
				}

				/* write out */
				ret = spi_master_halfduplex_writeread_frames(
						spi_info->mst_id,	/* master_id */
						spi_info->slave_id,	/* slave_addr */
						kbuf, sub_len + spi_cmd->count,	/* pwbuf, wnum */
						NULL, 0);			/* prbuf, rnum */
				if (ret != SSI_OK) {
					kfree(kbuf);
					printk("error: spi_master_halfduplex_writeread_frames failed.\n");
					return -EACCES;
				}
			}
			break;
		case SPIDEV_XDMODE2_WOA8D8n:
			if (!wr) {
				printk("error: write only.\n");
				return -EINVAL;
			} else {
				/* allocate the buffer to store data */
				kbuf = (uint *)kmalloc((spi_cmd->count + SPI_SUB_LENGTH)*4, GFP_KERNEL);
				if (kbuf == NULL) {
					printk("error: failed to kmalloc memory.\n");
					return -ENOMEM;
				}

				kbuf[0] = spi_cmd->sub_cmd;
				if (copy_from_user((void *)(&kbuf[1]), (void __user *)spi_cmd->buf, 
							spi_cmd->count*4)) {
					kfree(kbuf);
					printk("error: copy_from_user failed\n");
					return -EFAULT;
				}

				/* write out */
				ret = spi_master_halfduplex_writeread_frames(
						spi_info->mst_id,	/* master_id */
						spi_info->slave_id,	/* slave_addr */
						kbuf, 1 + spi_cmd->count,	/* pwbuf, wnum */
						NULL, 0);			/* prbuf, rnum */
				if (ret != SSI_OK) {
					kfree(kbuf);
					printk("error: spi_master_halfduplex_writeread_frames failed.\n");
					return -EACCES;
				}
			}
			break;
		default:
			printk("error: unknown mode %d.\n", spi_info->xd_mode);
			return -EINVAL;
	}

	if (kbuf)
		kfree(kbuf);
	return 0;
}

static irqreturn_t galois_spi_intr(int irq, void *dev_id)
{
	galois_spi_info_t *spi_info = (galois_spi_info_t *)dev_id;

	if ((spi_info->mst_id >= GALOIS_SPIBUS_NUM)
			|| (!spi_info->usable))
		return IRQ_NONE;

	DBGPRINTK("into irs.\n");
	spi_master_irs(spi_info->mst_id);
	DBGPRINTK("out of irs.\n");
	return IRQ_HANDLED;
}

static int galois_spi_open(struct inode *inode, struct file *file)
{
	galois_spi_info_t *spi_info;
	int spi_irq, spi_master_id;
	int res;

	switch (iminor(inode)) {
#ifndef CONFIG_MV88DE3010_BERLIN
		case GALOIS_SPI0_MINOR:
			spi_irq = IRQ_APB_SSIINST0;
			spi_master_id = GALOIS_SPIBUS0;
			break;
#endif
		case GALOIS_SPI1_MINOR:
			spi_irq = IRQ_APB_SSIINST1;
			spi_master_id = GALOIS_SPIBUS1;
			break;
		case GALOIS_SPI2_MINOR:
#ifndef CONFIG_MV88DE3010_BERLIN
			spi_irq = IRQ_APB_SSIINST2;
#else
			spi_irq = G_IRQ_SM_SPI;
#endif
			spi_master_id = GALOIS_SPIBUS2;
			break;
		default:
			printk("error: unknown SPI bus %d.\n", iminor(inode));
			return -ENODEV;
	}

	mutex_lock(&ll_spi_mutex);
	/* Only one can open a bus at a same time */
	if (galois_spi_info[spi_master_id]) {
		file->private_data = galois_spi_info[spi_master_id];
		spi_opentime[spi_master_id]++;
		printk("Info: spi bus %d opened %d times.\n", spi_master_id, spi_opentime[spi_master_id]);
		mutex_unlock(&ll_spi_mutex);
		return 0;
	} else {
		spi_info = kzalloc(sizeof(galois_spi_info_t), GFP_KERNEL);
		if (spi_info == NULL) {
			printk("spi%d: couldn't allocate memory for data.\n", spi_master_id);
			mutex_unlock(&ll_spi_mutex);
			return -ENOMEM;
		}

		/* initialize spi_info */
		spi_info->irq = spi_irq;
		spi_info->mst_id = spi_master_id;
		spi_info->usable = 0; /* usable=0: can't be used */

		galois_spi_info[spi_master_id] = spi_info;
		file->private_data = spi_info;

		/* set pinmux */
		set_spi_pinmux(spi_master_id);

	/* initialize spi master: spi proto, half-duplex, intr masked, spi disabled */
#if 0
		if (spi_master_is_idle(spi_master_id) != SSI_OK) {
			kfree(spi_info);
			printk("spi%d: busy.\n", spi_master_id);
			mutex_unlock(&ll_spi_mutex);
			return -EBUSY;
		}
#endif
		spi_master_init(spi_master_id);

		snprintf(spi_name, 8, "spi%d", spi_master_id);
		res = request_irq(spi_irq, galois_spi_intr, IRQF_SHARED, spi_name, spi_info);
		if (res) {
			kfree(spi_info);
			printk("spi%d: failure requesting irq %i\n", spi_master_id, spi_irq);
			mutex_unlock(&ll_spi_mutex);
			return -EBUSY;
		}

		/* Caution: CPU#1 may be using it */
		mv88de3010_open_irq(spi_irq); /* enable irq in PIC */
	}
	spi_opentime[spi_master_id]++;
	mutex_unlock(&ll_spi_mutex);
	return 0;
}

static int galois_spi_release(struct inode *inode, struct file *file)
{
	galois_spi_info_t *spi_info = (galois_spi_info_t *)file->private_data;

	mutex_lock(&ll_spi_mutex);
	spi_opentime[spi_info->mst_id]--;
	printk("Info: spi bus %d open time reduces to %d.\n", spi_info->mst_id, spi_opentime[spi_info->mst_id]);
	if (spi_opentime[spi_info->mst_id] <= 0) {
		galois_spi_info[spi_info->mst_id] = NULL;
		mv88de3010_close_irq(spi_info->irq);
		free_irq(spi_info->irq, spi_info);
		spi_info->usable = 0;
		kfree(spi_info);
	}
	mutex_unlock(&ll_spi_mutex);
	return 0;
}

static ssize_t galois_spi_read(struct file *file, char __user * buf,
		size_t count, loff_t *ppos)
{
	printk("error: please use ioctl.\n");
	return -EFAULT;
}

static ssize_t galois_spi_write(struct file *file, const char __user * buf,
		size_t count, loff_t *ppos)
{
	printk("error: please use ioctl.\n");
	return -EFAULT;
}

static int galois_spi_ioctl(struct inode *inode, struct file *file, 
		unsigned int cmd, unsigned long arg)
{
	galois_spi_info_t *kern_spi_info = (galois_spi_info_t *)file->private_data;
	galois_spi_info_t user_spi_info;
	galois_spi_cmd_t user_spi_cmd;
	ssize_t ret = 0;

	switch(cmd) {
		case SPI_IOCTL_SETINFO:
			if (copy_from_user(&user_spi_info, (void __user *)arg, sizeof(galois_spi_info_t)))
				return -EFAULT;
			if (user_spi_info.mst_id != kern_spi_info->mst_id) {
				printk("error: invalid master id %d (%d).\n", 
						user_spi_info.mst_id, kern_spi_info->mst_id);
				return -EACCES;
			}
			
			mutex_lock(&ll_spi_mutex);
			kern_spi_info->slave_id = user_spi_info.slave_id;
			kern_spi_info->xd_mode = user_spi_info.xd_mode;
			kern_spi_info->clock_mode = user_spi_info.clock_mode;
			kern_spi_info->speed = user_spi_info.speed;
			kern_spi_info->frame_width = user_spi_info.frame_width;
			kern_spi_info->usable = 1;

			ret = parse_spi_info(kern_spi_info);
			if (ret) {
				kern_spi_info->usable = 0;
				return ret;
			}

#if 0 /* use mutex to protect, don't use it any more */
			/* setup spi master */
			if (spi_master_is_idle(kern_spi_info->mst_id) != SSI_OK) {
				printk("error: spi%d is busy.\n", kern_spi_info->mst_id);
				return -EBUSY;
			}
#endif
			set_spi_slave_pinmux(kern_spi_info->mst_id, kern_spi_info->slave_id);
			spi_master_device_select(kern_spi_info->mst_id, kern_spi_info->slave_id, 1);
			/* select_slave_device(kern_spi_info->mst_id, kern_spi_info->slave_id, 1); */
			spi_master_set_clock_mode(kern_spi_info->mst_id, kern_spi_info->clock_mode);
			spi_master_set_speed(kern_spi_info->mst_id, kern_spi_info->speed);
			spi_master_set_framewidth(kern_spi_info->mst_id, kern_spi_info->frame_width);
			/* spi_master_enable(kern_spi_info->mst_id); */
			mutex_unlock(&ll_spi_mutex);
			break;
		case SPI_IOCTL_GETINFO:
			if (copy_from_user(&user_spi_info, (void __user *)arg, sizeof(galois_spi_info_t)))
				return -EFAULT;
			if (user_spi_info.mst_id != kern_spi_info->mst_id) {
				printk("error: invalid master id %d (%d).\n", 
						user_spi_info.mst_id, kern_spi_info->mst_id);
				return -EACCES;
			}
			if (copy_to_user((void __user *)arg, kern_spi_info, sizeof(galois_spi_info_t)))
				return -EFAULT;
			break;
		case SPI_IOCTL_READ:
			if (copy_from_user(&user_spi_cmd, (void __user *)arg, sizeof(galois_spi_cmd_t)))
				return -EFAULT;

#if 0 /* use mutex to protect, don't use it any more */
			if (spi_master_is_idle(kern_spi_info->mst_id) != SSI_OK) {
				printk("error: spi%d is busy.\n", kern_spi_info->mst_id);
				return -EBUSY;
			}
#endif

			/* prepare the sub address written to device before data */
			mutex_lock(&ll_spi_mutex);
			ret = process_spi_cmd(kern_spi_info, &user_spi_cmd, 0);
			mutex_unlock(&ll_spi_mutex);
			if (ret) {
				printk("error: parse_spi_cmd error.\n");
				return ret;
			}
			break;
		case SPI_IOCTL_WRITE:
			if (copy_from_user(&user_spi_cmd, (void __user *)arg, sizeof(galois_spi_cmd_t)))
				return -EFAULT;

#if 0 /* use mutex to protect, don't use it any more */
			if (spi_master_is_idle(kern_spi_info->mst_id) != SSI_OK) {
				printk("error: spi%d is busy.\n", kern_spi_info->mst_id);
				return -EBUSY;
			}
#endif

			/* prepare the sub address written to device before data */
			mutex_lock(&ll_spi_mutex);
			ret = process_spi_cmd(kern_spi_info, &user_spi_cmd, 1);
			mutex_unlock(&ll_spi_mutex);
			if (ret) {
				printk("error: parse_spi_cmd error.\n");
				return ret;
			}
			break;
		case SPI_IOCTL_READWRITE:
			{
			galois_spi_rw_t user_spi_rw;
			uint *rd_kbuf = NULL, *wr_kbuf = NULL;
			int res;

			if (copy_from_user(&user_spi_rw, (void __user *)arg, sizeof(galois_spi_rw_t)))
				return -EFAULT;

			/* judge if this access is valid */
			if (user_spi_rw.mst_id != kern_spi_info->mst_id) {
				printk("error: invalid master id %d.\n", user_spi_rw.mst_id);
				return -EACCES;
			}
#if 0 /* use mutex to protect, don't use it any more */
			if (spi_master_is_idle(user_spi_rw.mst_id) != SSI_OK) {
				printk("spi%d: busy.\n", user_spi_rw.mst_id);
				return -EBUSY;
			}
#endif

			/* allocate the buffer to store data */
			if (user_spi_rw.rd_cnt > 0) {
				rd_kbuf = kmalloc(user_spi_rw.rd_cnt*4, GFP_KERNEL);
				if (rd_kbuf == NULL) {
					printk("error: failed to kmalloc rd memory(0x%x).\n", user_spi_rw.rd_cnt*4);
					return -ENOMEM;
				}
			}
			if (user_spi_rw.wr_cnt > 0) {
				wr_kbuf = kmalloc(user_spi_rw.wr_cnt*4, GFP_KERNEL);
				if (wr_kbuf == NULL) {
					ret = -ENOMEM;
					printk("error: failed to kmalloc wr memory(0x%x).\n", user_spi_rw.wr_cnt*4);
					goto read_write_out;
				}
				if (copy_from_user((void *)wr_kbuf, (void __user *)user_spi_rw.wr_buf, 
							user_spi_rw.wr_cnt*4)) {
					ret = -EFAULT;
					printk("error: copy_from_user failed\n");
					goto read_write_out;
				}
			}

			/* issue rw */
			mutex_lock(&ll_spi_mutex);
			res = spi_master_halfduplex_writeread_frames(
					user_spi_rw.mst_id,		/* master_id */
					user_spi_rw.slave_id,	/* slave_addr */
					wr_kbuf, user_spi_rw.wr_cnt, 	/* pwbuf, wnum */
					rd_kbuf, user_spi_rw.rd_cnt);	/* prbuf, rnum, dum? */
			mutex_unlock(&ll_spi_mutex);
			if (res != SSI_OK) {
				ret = -EACCES;
				printk("error: spi_master_halfduplex_writeread_frames failed.\n");
				goto read_write_out;
			}

			/* return to userspace */
			if (user_spi_rw.rd_cnt > 0) {
				if (copy_to_user((void __user *)user_spi_rw.rd_buf, (void *)rd_kbuf, 
							user_spi_rw.rd_cnt*4)) {
					ret = -EFAULT;
					printk("error: copy_to_user failed\n");
					goto read_write_out;
				}
			}
read_write_out:
			if (rd_kbuf) 
				kfree(rd_kbuf);
			if (wr_kbuf) 
				kfree(wr_kbuf);
			return ret;
			}

		case SPI_IOCTL_READWRITE_CHAR:
			{
			galois_spi_rw_char_t user_spi_rw_char;
			unsigned char *rd_char_kbuf = NULL, *wr_char_kbuf = NULL;
			uint *rd_kbuf = NULL, *wr_kbuf = NULL;
			uint rd_fcnt, wr_fcnt;
			int res;
			int i;

			if (copy_from_user(&user_spi_rw_char, (void __user *)arg, sizeof(galois_spi_rw_t)))
				return -EFAULT;

			/* judge if this access is valid */
			if (user_spi_rw_char.mst_id != kern_spi_info->mst_id) {
				printk("error: invalid master id %d.\n", user_spi_rw_char.mst_id);
				return -EACCES;
			}
#if 0 /* use mutex to protect, don't use it any more */
			if (spi_master_is_idle(user_spi_rw_char.mst_id) != SSI_OK) {
				printk("spi%d: busy.\n", user_spi_rw_char.mst_id);
				return -EBUSY;
			}
#endif

			/* allocate the buffer to store data */
			if (user_spi_rw_char.rd_cnt > 0) {
				rd_kbuf = kmalloc(user_spi_rw_char.rd_cnt*4, GFP_KERNEL);
				if (rd_kbuf == NULL) {
					ret = -ENOMEM;
					printk("error: failed to kmalloc rd memory(0x%x).\n", user_spi_rw_char.rd_cnt*4);
					goto read_write_char_out;
				}
				rd_char_kbuf = kmalloc(user_spi_rw_char.rd_cnt, GFP_KERNEL);
				if (rd_char_kbuf == NULL) {
					ret = -ENOMEM;
					printk("error: failed to kmalloc rd memory(0x%x).\n", user_spi_rw_char.rd_cnt);
					goto read_write_char_out;
				}
			}
			if (user_spi_rw_char.wr_cnt > 0) {
				wr_kbuf = kmalloc(user_spi_rw_char.wr_cnt*4, GFP_KERNEL);
				if (wr_kbuf == NULL) {
					ret = -ENOMEM;
					printk("error: failed to kmalloc wr memory(0x%x).\n", user_spi_rw_char.wr_cnt*4);
					goto read_write_char_out;
				}
				wr_char_kbuf = kmalloc(user_spi_rw_char.wr_cnt, GFP_KERNEL);
				if (wr_char_kbuf == NULL) {
					ret = -ENOMEM;
					printk("error: failed to kmalloc wr memory(0x%x).\n", user_spi_rw_char.wr_cnt);
					goto read_write_char_out;
				}
				if (copy_from_user((void *)wr_char_kbuf, (void __user *)user_spi_rw_char.wr_buf, 
							user_spi_rw_char.wr_cnt)) {
					ret = -EFAULT;
					printk("error: copy_from_user failed\n");
					goto read_write_char_out;
				}
			}

			/* set count of rd/wr in frame width and prepare rd/wr buffer in int */
			if (kern_spi_info->frame_width == 8) {
				rd_fcnt = user_spi_rw_char.rd_cnt;
				wr_fcnt = user_spi_rw_char.wr_cnt;
				for (i=0; i<wr_fcnt; i++) {
					wr_kbuf[i] = wr_char_kbuf[i];
				}
			} else if (kern_spi_info->frame_width == 16) {
				if (user_spi_rw_char.rd_cnt%2 || user_spi_rw_char.wr_cnt%2) {
					printk("error: unaligned(2) count rd:%d, wr:%d\n", user_spi_rw_char.rd_cnt, user_spi_rw_char.wr_cnt);
					ret = -EFAULT;
					goto read_write_char_out;
				}
 				rd_fcnt = user_spi_rw_char.rd_cnt/2;
				wr_fcnt = user_spi_rw_char.wr_cnt/2;
				for (i=0; i<wr_fcnt; i++) {
					wr_kbuf[i] = (wr_char_kbuf[2*i+1] << 8) | wr_char_kbuf[2*i];
				}
			} else if (kern_spi_info->frame_width == 24) {
				if (user_spi_rw_char.rd_cnt%3 || user_spi_rw_char.wr_cnt%3) {
					printk("error: unaligned(3) count rd:%d, wr:%d\n", user_spi_rw_char.rd_cnt, user_spi_rw_char.wr_cnt);
					ret = -EFAULT;
					goto read_write_char_out;
				}
 				rd_fcnt = user_spi_rw_char.rd_cnt/3;
				wr_fcnt = user_spi_rw_char.wr_cnt/3;
				for (i=0; i<wr_fcnt; i++) {
					wr_kbuf[i] = (wr_char_kbuf[3*i+2] << 16) | (wr_char_kbuf[3*i+1] << 8) | wr_char_kbuf[3*i];
				}
			} else if (kern_spi_info->frame_width == 32) {
				if (user_spi_rw_char.rd_cnt%4 || user_spi_rw_char.wr_cnt%4) {
					printk("error: unaligned count(4) rd:%d, wr:%d\n", user_spi_rw_char.rd_cnt, user_spi_rw_char.wr_cnt);
					ret = -EFAULT;
					goto read_write_char_out;
				}
				rd_fcnt = user_spi_rw_char.rd_cnt/4;
				wr_fcnt = user_spi_rw_char.wr_cnt/4;
				for (i=0; i<wr_fcnt; i++) {
					wr_kbuf[i] = (wr_char_kbuf[4*i+3] << 24) | (wr_char_kbuf[4*i+2] << 16) | (wr_char_kbuf[4*i+1] << 8) | wr_char_kbuf[4*i];
				}
			} else {
				printk("error: unaligned frame width(8) %d, rd:%d, wr:%d\n", kern_spi_info->frame_width, user_spi_rw_char.rd_cnt, user_spi_rw_char.wr_cnt);
				ret = -EFAULT;
				goto read_write_char_out;
			}

			/* issue rw */
			mutex_lock(&ll_spi_mutex);
			res = spi_master_halfduplex_writeread_frames(
					user_spi_rw_char.mst_id,	/* master_id */
					user_spi_rw_char.slave_id,	/* slave_addr */
					wr_kbuf, wr_fcnt, 	/* pwbuf, wnum */
					rd_kbuf, rd_fcnt);	/* prbuf, rnum, dum? */
			mutex_unlock(&ll_spi_mutex);
			if (res != SSI_OK) {
				ret = -EACCES;
				printk("error: spi_master_halfduplex_writeread_frames failed.\n");
				goto read_write_char_out;
			}

			/* return to userspace */
			if (user_spi_rw_char.rd_cnt > 0) {
				if (kern_spi_info->frame_width == 8) {
					for (i=0; i<rd_fcnt; i++) {
						rd_char_kbuf[i] = rd_kbuf[i] & 0xff;
					}
				} else if (kern_spi_info->frame_width == 16) {
					for (i=0; i<rd_fcnt; i++) {
						rd_char_kbuf[2*i] = rd_kbuf[i] & 0xff;
						rd_char_kbuf[2*i+1] = (rd_kbuf[i] >> 8) & 0xff;
					}
				} else if (kern_spi_info->frame_width == 24) {
					for (i=0; i<rd_fcnt; i++) {
						rd_char_kbuf[3*i] = rd_kbuf[i] & 0xff;
						rd_char_kbuf[3*i+1] = (rd_kbuf[i] >> 8) & 0xff;
						rd_char_kbuf[3*i+2] = (rd_kbuf[i] >> 16) & 0xff;
					}
				} else if (kern_spi_info->frame_width == 32) {
					for (i=0; i<rd_fcnt; i++) {
						rd_char_kbuf[4*i] = rd_kbuf[i] & 0xff;
						rd_char_kbuf[4*i+1] = (rd_kbuf[i] >> 8) & 0xff;
						rd_char_kbuf[4*i+2] = (rd_kbuf[i] >> 16) & 0xff;
						rd_char_kbuf[4*i+3] = (rd_kbuf[i] >> 24) & 0xff;
					}
				} else {
					ret = -EFAULT;
					printk("error: wrong frame width %d\n", kern_spi_info->frame_width);
					goto read_write_char_out;
				}

				if (copy_to_user((void __user *)user_spi_rw_char.rd_buf, (void *)rd_char_kbuf, 
							user_spi_rw_char.rd_cnt)) {
					ret = -EFAULT;
					printk("error: copy_to_user failed\n");
					goto read_write_char_out;
				}
			}
read_write_char_out:
			if (rd_kbuf) 
				kfree(rd_kbuf);
			if (wr_kbuf) 
				kfree(wr_kbuf);
			if (rd_char_kbuf) 
				kfree(rd_char_kbuf);
			if (wr_char_kbuf) 
				kfree(wr_char_kbuf);

			return ret;
			}

		default:
			return -EINVAL;
	}
	return 0;
}

static struct file_operations galois_spi_fops = {
	.owner		= THIS_MODULE,
	.open		= galois_spi_open,
	.release	= galois_spi_release,
	.write		= galois_spi_write,
	.read		= galois_spi_read,
	.ioctl		= galois_spi_ioctl,
};

/*
 * SPI controller list
 */
static struct {
	uint minor;
	char *name;
} spidev_list[] = {
#ifndef CONFIG_MV88DE3010_BERLIN
	{GALOIS_SPI0_MINOR, GALOIS_SPI0_NAME},
#endif
	{GALOIS_SPI1_MINOR, GALOIS_SPI1_NAME},
	{GALOIS_SPI2_MINOR, GALOIS_SPI2_NAME},
};

/* SPI may be needed to bring up other drivers */
static struct class *spi_class;
static int __init galois_spi_init_driver(void)
{
#ifdef CONFIG_MV88DE3010_PM_DISABLE_SPI_TWSI
	return 0;
#else
	int i;

	for (i = 0; i < GALOIS_SPIBUS_NUM; i++) {
		galois_spi_info[i] = NULL;
		spi_opentime[i] = 0;
	}

	/* register char device */
	printk("register major %d\n", GALOIS_SPI_MAJOR);
	if (register_chrdev(GALOIS_SPI_MAJOR, "spi", &galois_spi_fops))
		printk("unable to get major %d for spi.\n", GALOIS_SPI_MAJOR);

	spi_class = class_create(THIS_MODULE, "spi");
	for (i = 0; i < ARRAY_SIZE(spidev_list); i++) {
		printk("minor=%d\n", spidev_list[i].minor);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
		device_create(spi_class, NULL, 
				MKDEV(GALOIS_SPI_MAJOR, spidev_list[i].minor), spidev_list[i].name);
#else
		device_create(spi_class, NULL,
				MKDEV(GALOIS_SPI_MAJOR, spidev_list[i].minor), 
				NULL, spidev_list[i].name);
#endif
	}

	return 0;
#endif
}

static void __exit galois_spi_exit_driver(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(spidev_list); i++) {
		printk("minor=%d\n", spidev_list[i].minor);
		device_destroy(spi_class, MKDEV(GALOIS_SPI_MAJOR, spidev_list[i].minor));
	}
	class_destroy(spi_class);

	printk("unregister major %d\n", GALOIS_SPI_MAJOR);
	unregister_chrdev(GALOIS_SPI_MAJOR, "spi");
	return;
}

void galois_spi_resume(void)
{
	int i;
	for (i = 0; i < GALOIS_SPIBUS_NUM; i++) {
		
		if (galois_spi_info[i] == NULL)
			continue;
		{
		uint mst_id = galois_spi_info[i]->mst_id;
		int irq = galois_spi_info[i]->irq;
		int usable = galois_spi_info[i]->usable;
		
		mutex_lock(&ll_spi_mutex);
		
		/* restore master info */
		set_spi_pinmux(mst_id);
		spi_master_init(mst_id);
		
		/* open irq for the spi master */
		mv88de3010_open_irq(irq);
		
		/* restore slave info */
		if (usable) {
			uint slave_id = galois_spi_info[i]->slave_id;
			uint clock_mode = galois_spi_info[i]->clock_mode;
			uint speed = galois_spi_info[i]->speed;
			uint frame_width = galois_spi_info[i]->frame_width;

			galois_printk("Restore SPI master [%d], slave [%d]\n", mst_id, slave_id);
			/* only one slave linked on each master, so only one slave will be restored */
			set_spi_slave_pinmux(mst_id, slave_id);
			spi_master_device_select(mst_id, slave_id, 1);
			spi_master_set_clock_mode(mst_id, clock_mode);
			spi_master_set_speed(mst_id, speed);
			spi_master_set_framewidth(mst_id, frame_width);
		}
		
		mutex_unlock(&ll_spi_mutex);
		}
	}
}

module_init(galois_spi_init_driver);
module_exit(galois_spi_exit_driver);

MODULE_AUTHOR("Marvell-Galois");
MODULE_DESCRIPTION("Galois SPI Driver");
MODULE_LICENSE("GPL");
