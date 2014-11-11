#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/poll.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/galois_generic.h>
#include <mach/kgpio.h>

#include "galois_common.h"
#include "galois_io.h"
#ifndef BERLIN
#include "spi_master.h"
#include "dv_iomapper.h"
#endif
#include "gpio.h"

#define IRQ_APB_GPIOINST_NUM(port) \
		(port >= 24 ? IRQ_APB_GPIOINST3 : \
		 (port >= 16 ? IRQ_APB_GPIOINST2 : \
		  (port >= 8 ? IRQ_APB_GPIOINST1 : IRQ_APB_GPIOINST0)))

#define IRQ_APB_SMGPIOINST_NUM(port) (port >= 8 ? G_IRQ_SM_GPIO1 : G_IRQ_SM_GPIO0)


#undef ENABLE_GPIO_DEBUG
#ifdef ENABLE_GPIO_DEBUG
#define DEBUG_PRINT	printk
#else
#define DEBUG_PRINT(...)
#endif

static spinlock_t gpio_irq_lock;
static DEFINE_MUTEX(irq_init_mutex);
static int irq_init = 0;	/* ioctl IRQ_INIT count by multiple processes */

static unsigned int gpio_dev_id = 0x12345678;
static DECLARE_WAIT_QUEUE_HEAD(gpio_wait_queue);
static unsigned int gpio_irq_mode = 0;
static unsigned int gpio_irq_status = 0;
static int gpio_mode[GPIO_PORT_NUM];
static unsigned int gpio_mode_status = 0;
static unsigned int sm_gpio_irq_mode = 0;
static unsigned int sm_gpio_irq_status = 0;
static int sm_gpio_mode[SM_GPIO_PORT_NUM];
static unsigned int sm_gpio_mode_status = 0;

static irqreturn_t gpio_interrupt_handle(int irq, void *dev_id)
{
	int i;

	spin_lock(&gpio_irq_lock);
	for (i = 0; i < GPIO_PORT_NUM; i++) {
		if ((gpio_irq_mode & (1 << i)) && (GPIO_PortHasInterrupt(i) == 1)) {
			DEBUG_PRINT("[K]irq:%d, port:%d\n", irq, i);
			GPIO_PortClearInterrupt(i);
			DEBUG_PRINT("[port:%d, %d]\n", i, GPIO_PortHasInterrupt(i));
			gpio_irq_status |= (1 << i);
		}
	}

	for (i = 0; i < SM_GPIO_PORT_NUM; i++) {
		if ((sm_gpio_irq_mode & (1 << i)) && (SM_GPIO_PortHasInterrupt(i) == 1)) {
			DEBUG_PRINT("[K]irq:%d, smport:%d\n", irq, i);
			SM_GPIO_PortClearInterrupt(i);
			DEBUG_PRINT("[port:%d, %d]\n", i, SM_GPIO_PortHasInterrupt(i));
			sm_gpio_irq_status |= (1 << i);
		}
	}

	if (gpio_irq_status || sm_gpio_irq_status)
		wake_up_interruptible(&gpio_wait_queue);
	spin_unlock(&gpio_irq_lock);
	return IRQ_HANDLED;
}

static int galois_gpio_open(struct inode *inode, struct file *file)
{
#ifndef BERLIN
	unsigned int reg;

	spi_master_init_iomapper(IOMAPPER_SPI_MASTER);
	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 
			1, IOMAPPER_REG_MAP_10, &reg);
	if (reg & 0x1800) {	/* [12:11]=b00, GPIOmatrix0 */
		printk("GPIO matrix isn't configured correctly.\n");
		spi_master_exit_iomapper(IOMAPPER_SPI_MASTER);
		return -ENODEV;
	}
#endif
	return 0;
}

static int galois_gpio_release(struct inode *inode, struct file *file)
{
#if 0
	int i;

	/* disable all GPIO interrupts */
	for (i = 0; i < GPIO_PORT_NUM; i++) {
		if (gpio_irq_mode & (1 << i))
			GPIO_PortDisableIRQ(i);
	}

	/* unregisteer handler registered by GPIO */
	free_irq(IRQ_APB_GPIOINST0, &gpio_dev_id);
	free_irq(IRQ_APB_GPIOINST1, &gpio_dev_id);
	free_irq(IRQ_APB_GPIOINST2, &gpio_dev_id);
	free_irq(IRQ_APB_GPIOINST3, &gpio_dev_id);
	free_irq(G_IRQ_SM_GPIO1, &gpio_dev_id);
	free_irq(G_IRQ_SM_GPIO0, &gpio_dev_id);
#endif
#ifndef BERLIN
	spi_master_exit_iomapper(IOMAPPER_SPI_MASTER);
#endif
	return 0;
}

static ssize_t galois_gpio_read(struct file *file, char __user * buf,
		size_t count, loff_t *ppos)
{
	printk("error: please use ioctl.\n");
	return -EFAULT;
}

static ssize_t galois_gpio_write(struct file *file, const char __user * buf,
		size_t count, loff_t *ppos)
{
	printk("error: please use ioctl.\n");
	return -EFAULT;
}

/*
 * gpio_set_mode: set SoC GPIO port to specific mode
 * @port: port number
 * @mode: mode
 */
static int gpio_set_mode(int port, int mode)
{
	if (mv_gpio_setup_pinmux(port) != 0) {
		printk("GPIO pinmux init failed.\n");
		return -1;
	}

	switch (mode) {
		case GPIO_MODE_DATA_IN:
#ifndef BERLIN
			GPIO_IOmapperSetInOut(port, 1);
#endif
			GPIO_PortSetInOut(port, 1);
			break;
		case GPIO_MODE_DATA_OUT:
#ifndef BERLIN
			GPIO_IOmapperSetInOut(port, 0);
#endif
			GPIO_PortSetInOut(port, 0);
			break;
		case GPIO_MODE_IRQ_LOWLEVEL:
#ifndef BERLIN
			GPIO_IOmapperSetInOut(port, 1);
#endif
			GPIO_PortSetInOut(port, 1);
			GPIO_PortInitIRQ(port, 0, 0);
			break;
		case GPIO_MODE_IRQ_HIGHLEVEL:
#ifndef BERLIN
			GPIO_IOmapperSetInOut(port, 1);
#endif
			GPIO_PortSetInOut(port, 1);
			GPIO_PortInitIRQ(port, 0, 1);
			break;
		case GPIO_MODE_IRQ_RISEEDGE:
#ifndef BERLIN
			GPIO_IOmapperSetInOut(port, 1);
#endif
			GPIO_PortSetInOut(port, 1);
			GPIO_PortInitIRQ(port, 1, 1);
			break;
		case GPIO_MODE_IRQ_FALLEDGE:
#ifndef BERLIN
			GPIO_IOmapperSetInOut(port, 1);
#endif
			GPIO_PortSetInOut(port, 1);
			GPIO_PortInitIRQ(port, 1, 0);
			break;
		default:
			printk("Unknown GPIO mode.\n");
			return -1;
	}

	gpio_mode_status |= (1<<port);
	gpio_mode[port] = mode;
	return 0;
}

/*
 * sm_gpio_set_mode: set SM GPIO port to specific mode
 * @port: port number
 * @mode: mode
 */
static int sm_gpio_set_mode(int port, int mode)
{
	switch (mode) {
		case GPIO_MODE_DATA_IN:
			SM_GPIO_PortSetInOut(port, 1);
			break;
		case GPIO_MODE_DATA_OUT:
			SM_GPIO_PortSetInOut(port, 0);
			break;
		case GPIO_MODE_IRQ_LOWLEVEL:
			SM_GPIO_PortSetInOut(port, 1);
			SM_GPIO_PortInitIRQ(port, 0, 0);
			break;
		case GPIO_MODE_IRQ_HIGHLEVEL:
			SM_GPIO_PortSetInOut(port, 1);
			SM_GPIO_PortInitIRQ(port, 0, 1);
			break;
		case GPIO_MODE_IRQ_RISEEDGE:
			SM_GPIO_PortSetInOut(port, 1);
			SM_GPIO_PortInitIRQ(port, 1, 1);
			break;
		case GPIO_MODE_IRQ_FALLEDGE:
			SM_GPIO_PortSetInOut(port, 1);
			SM_GPIO_PortInitIRQ(port, 1, 0);
			break;
		default:
			printk("Unknown SM GPIO mode.\n");
			return -1;
	}

	sm_gpio_mode_status |= (1<<port);
	sm_gpio_mode[port] = mode;

	return 0;
}

static int gpio_get_info(galois_gpio_reg_t *gpio_reg)
{
	int i, inout, data;
#ifndef BERLIN
	unsigned int tmp_reg;
	for (i = 0; i < IOMAPPER_REG_NUM; i++) {
		spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 
				1, IOMAPPER_REG_MAP_0 + i, &tmp_reg);
		gpio_reg->iomapper_reg[i] = tmp_reg;
	}
#endif
	for (i = 0; i < GPIO_PORT_NUM; i++) {
		GPIO_PortGetInOut(i, &inout);
		gpio_reg->port_ddr |= (inout << i);
		GPIO_PortGetData(i, &data);
		gpio_reg->port_dr |= (data << i);
	}

	for (i = 0; i < GPIO_PORT_NUM; i++) {
		gpio_reg->gpio_mode[i] = gpio_mode[i];
	}

	return 0;
}

static int sm_gpio_get_info(galois_gpio_reg_t *gpio_reg)
{
	int i, inout, data;

	for (i = 0; i < SM_GPIO_PORT_NUM; i++) {
		SM_GPIO_PortGetInOut(i, &inout);
		gpio_reg->port_ddr |= (inout << i);
		SM_GPIO_PortGetData(i, &data);
		gpio_reg->port_dr |= (data << i);
	}

	for (i = 0; i < SM_GPIO_PORT_NUM; i++) {
		gpio_reg->gpio_mode[i] = sm_gpio_mode[i];
	}

	return 0;
}

static int galois_gpio_ioctl(struct inode *inode, struct file *file, 
		unsigned int cmd, unsigned long arg)
{
	galois_gpio_data_t *arg_data = (galois_gpio_data_t *)arg;
	galois_gpio_reg_t *kreg;
	galois_gpio_data_t gpio_data;
	unsigned int __gpio_irq_status, __sm_gpio_irq_status;
	int ret = 0;

	switch(cmd) {
		case GPIO_IOCTL_SET:
		case SM_GPIO_IOCTL_SET:
			if (copy_from_user(&gpio_data, (void __user *)arg, sizeof(galois_gpio_data_t)))
				return -EFAULT;
			if (cmd == GPIO_IOCTL_SET)
				gpio_set_mode(gpio_data.port, gpio_data.mode);
			else
				sm_gpio_set_mode(gpio_data.port, gpio_data.mode);
			break;
		case GPIO_IOCTL_GET:
		case SM_GPIO_IOCTL_GET:
			kreg = kmalloc(sizeof(galois_gpio_reg_t), GFP_KERNEL);
			if (kreg == NULL) {
				printk("error: failed to kmalloc memory.\n");
				return -ENOMEM;
			}
			if (cmd == GPIO_IOCTL_GET)
				gpio_get_info(kreg);
			else
				sm_gpio_get_info(kreg);
			if (copy_to_user((void __user *)arg, kreg, sizeof(galois_gpio_reg_t))) {
				kfree(kreg);
				return -EFAULT;
			}
			kfree(kreg);
			break;
		case GPIO_IOCTL_READ:
		case SM_GPIO_IOCTL_READ:
			if (copy_from_user(&gpio_data, (void __user *)arg, sizeof(galois_gpio_data_t)))
				return -EFAULT;
			if (cmd == GPIO_IOCTL_READ) {
				if (gpio_mode[gpio_data.port] != GPIO_MODE_DATA_IN) {
					printk("GPIO port %d isn't set to data in.\n", gpio_data.port);
					return -ENODEV;
				}
				if (GPIO_PortRead(gpio_data.port, &gpio_data.data)) {
					printk("GPIO_PortRead error (port=%d)\n", gpio_data.port);
					return -ENODEV;
				}
			} else {
				if (sm_gpio_mode[gpio_data.port] != GPIO_MODE_DATA_IN) {
					printk("SM GPIO port %d isn't set to data in.\n", gpio_data.port);
					return -ENODEV;
				}
				if (SM_GPIO_PortRead(gpio_data.port, &gpio_data.data)) {
					printk("SM_GPIO_PortRead error (port=%d)\n", gpio_data.port);
					return -ENODEV;
				}
			}
			if (put_user(gpio_data.data, &arg_data->data))
				return -EFAULT;
			break;
		case GPIO_IOCTL_WRITE:
		case SM_GPIO_IOCTL_WRITE:
			if (copy_from_user(&gpio_data, (void __user *)arg, sizeof(galois_gpio_data_t)))
				return -EFAULT;
			if (cmd == GPIO_IOCTL_WRITE) {
				if (gpio_mode[gpio_data.port] != GPIO_MODE_DATA_OUT) {
					printk("GPIO port %d isn't set to data out.\n", gpio_data.port);
					return -ENODEV;
				}
				if (GPIO_PortWrite(gpio_data.port, gpio_data.data)) {
					printk("GPIO_PortRead error (port=%d)\n", gpio_data.port);
					return -ENODEV;
				}
			} else {
				if (sm_gpio_mode[gpio_data.port] != GPIO_MODE_DATA_OUT) {
					printk("SM GPIO port %d isn't set to data out.\n", gpio_data.port);
					return -ENODEV;
				}
				if (SM_GPIO_PortWrite(gpio_data.port, gpio_data.data)) {
					printk("SM_GPIO_PortRead error (port=%d)\n", gpio_data.port);
					return -ENODEV;
				}
			}
			break;
		case GPIO_IOCTL_ENABLE_IRQ:
		case SM_GPIO_IOCTL_ENABLE_IRQ:
			if (copy_from_user(&gpio_data, (void __user *)arg, sizeof(galois_gpio_data_t)))
				return -EFAULT;
			if (cmd == GPIO_IOCTL_ENABLE_IRQ) {
				if (GPIO_PortEnableIRQ(gpio_data.port)) {
					printk("GPIO_PortEnableIRQ error (port=%d)\n", gpio_data.port);
					return -ENODEV;
				}
				spin_lock(&gpio_irq_lock);
				gpio_irq_mode |= (1 << gpio_data.port);
				spin_unlock(&gpio_irq_lock);
	
				/* enable ictl */	
				mv88de3010_open_irq(IRQ_APB_GPIOINST_NUM(gpio_data.port));
			} else {
				if (SM_GPIO_PortEnableIRQ(gpio_data.port)) {
					printk("SM_GPIO_PortEnableIRQ error (port=%d)\n", gpio_data.port);
					return -ENODEV;
				}
				spin_lock(&gpio_irq_lock);
				sm_gpio_irq_mode |= (1 << gpio_data.port);
				spin_unlock(&gpio_irq_lock);
	
				/* enable ictl */	
				mv88de3010_open_irq(IRQ_APB_SMGPIOINST_NUM(gpio_data.port));
			}
			break;
		case GPIO_IOCTL_DISABLE_IRQ:
		case SM_GPIO_IOCTL_DISABLE_IRQ:
			if (copy_from_user(&gpio_data, (void __user *)arg, sizeof(galois_gpio_data_t)))
				return -EFAULT;
			if (cmd == GPIO_IOCTL_DISABLE_IRQ) {
				if (GPIO_PortDisableIRQ(gpio_data.port)) {
					printk("GPIO_PortDisableIRQ error (port=%d)\n", gpio_data.port);
					return -ENODEV;
				}
				spin_lock(&gpio_irq_lock);
				gpio_irq_mode &= ~(1 << gpio_data.port);
				spin_unlock(&gpio_irq_lock);

				/* disable ictl */	
				//mv88de3010_close_irq(IRQ_APB_GPIOINST_NUM(gpio_data.port));
			} else {
				if (SM_GPIO_PortDisableIRQ(gpio_data.port)) {
					printk("SM_GPIO_PortDisableIRQ error (port=%d)\n", gpio_data.port);
					return -ENODEV;
				}
				spin_lock(&gpio_irq_lock);
				sm_gpio_irq_mode &= ~(1 << gpio_data.port);
				spin_unlock(&gpio_irq_lock);
				/* */
			}
			break;
		case GPIO_IOCTL_CLEAR_IRQ:
		case SM_GPIO_IOCTL_CLEAR_IRQ:
			if (copy_from_user(&gpio_data, (void __user *)arg, sizeof(galois_gpio_data_t)))
				return -EFAULT;
			if (cmd == GPIO_IOCTL_CLEAR_IRQ) {
				/* GPIO_PortClearInterrupt isn't necessary */
				if (GPIO_PortClearInterrupt(gpio_data.port)) {
					printk("GPIO_PortClearIRQ error (port=%d)\n", gpio_data.port);
					return -ENODEV;
				}
				spin_lock(&gpio_irq_lock);
				gpio_irq_status &= ~(1 << gpio_data.port);
				spin_unlock(&gpio_irq_lock);
			} else {
				/* GPIO_PortClearInterrupt isn't necessary */
				if (SM_GPIO_PortClearInterrupt(gpio_data.port)) {
					printk("SM_GPIO_PortClearIRQ error (port=%d)\n", gpio_data.port);
					return -ENODEV;
				}
				spin_lock(&gpio_irq_lock);
				sm_gpio_irq_status &= ~(1 << gpio_data.port);
				spin_unlock(&gpio_irq_lock);
			}
			break;
		case GPIO_IOCTL_READ_IRQ:
		case SM_GPIO_IOCTL_READ_IRQ:
			spin_lock(&gpio_irq_lock);
			__gpio_irq_status = gpio_irq_status;
			__sm_gpio_irq_status = sm_gpio_irq_status;
			spin_unlock(&gpio_irq_lock);
			if (cmd == GPIO_IOCTL_READ_IRQ) {
				if (put_user(__gpio_irq_status, (unsigned int *)arg))
					return -EFAULT;
			} else {
				if (put_user(__sm_gpio_irq_status, (unsigned int *)arg))
					return -EFAULT;
			}
			break;
		case GPIO_IOCTL_INIT_IRQ:
			mutex_lock(&irq_init_mutex);
			if (irq_init > 0) {
				irq_init++;
				printk("GPIO irq init++ %d\n", irq_init);
				mutex_unlock(&irq_init_mutex);
				return 0;
			}
			/* reigster GPIO inst0 interrupt */
			ret = request_irq(IRQ_APB_GPIOINST0, gpio_interrupt_handle, IRQF_SHARED, 
					GALOIS_GPIO_NAME, &gpio_dev_id);
			if (ret) {
				printk("error: failed to request_irq %d.\n", IRQ_APB_GPIOINST0);
				goto init_irq_exit;
			}
			/* reigster GPIO inst1 interrupt */
			ret = request_irq(IRQ_APB_GPIOINST1, gpio_interrupt_handle, IRQF_SHARED, 
					GALOIS_GPIO_NAME, &gpio_dev_id);
			if (ret) {
				free_irq(IRQ_APB_GPIOINST0, &gpio_dev_id);
				printk("error: failed to request_irq %d.\n", IRQ_APB_GPIOINST1);
				goto init_irq_exit;
			}
			/* reigster GPIO inst2 interrupt */
			ret = request_irq(IRQ_APB_GPIOINST2, gpio_interrupt_handle, IRQF_SHARED, 
					GALOIS_GPIO_NAME, &gpio_dev_id);
			if (ret) {
				free_irq(IRQ_APB_GPIOINST0, &gpio_dev_id);
				free_irq(IRQ_APB_GPIOINST1, &gpio_dev_id);
				printk("error: failed to request_irq %d.\n", IRQ_APB_GPIOINST2);
				goto init_irq_exit;
			}
			/* reigster GPIO inst3 interrupt */
			ret = request_irq(IRQ_APB_GPIOINST3, gpio_interrupt_handle, IRQF_SHARED, 
					GALOIS_GPIO_NAME, &gpio_dev_id);
			if (ret) {
				free_irq(IRQ_APB_GPIOINST0, &gpio_dev_id);
				free_irq(IRQ_APB_GPIOINST1, &gpio_dev_id);
				free_irq(IRQ_APB_GPIOINST2, &gpio_dev_id);
				printk("error: failed to request_irq %d.\n", IRQ_APB_GPIOINST3);
				goto init_irq_exit;
			}

			/* reigster SM GPIO inst1 interrupt */
			ret = request_irq(G_IRQ_SM_GPIO1, gpio_interrupt_handle, IRQF_SHARED, 
					GALOIS_GPIO_NAME, &gpio_dev_id);
			if (ret) {
				free_irq(IRQ_APB_GPIOINST0, &gpio_dev_id);
				free_irq(IRQ_APB_GPIOINST1, &gpio_dev_id);
				free_irq(IRQ_APB_GPIOINST2, &gpio_dev_id);
				free_irq(IRQ_APB_GPIOINST3, &gpio_dev_id);
				printk("error: failed to request_irq %d.\n", G_IRQ_SM_GPIO1);
				goto init_irq_exit;
			}
			/* reigster SM GPIO inst0 interrupt */
			ret = request_irq(G_IRQ_SM_GPIO0, gpio_interrupt_handle, IRQF_SHARED, 
					GALOIS_GPIO_NAME, &gpio_dev_id);
			if (ret) {
				free_irq(IRQ_APB_GPIOINST0, &gpio_dev_id);
				free_irq(IRQ_APB_GPIOINST1, &gpio_dev_id);
				free_irq(IRQ_APB_GPIOINST2, &gpio_dev_id);
				free_irq(IRQ_APB_GPIOINST3, &gpio_dev_id);
				free_irq(G_IRQ_SM_GPIO1, &gpio_dev_id);
				printk("error: failed to request_irq %d.\n", G_IRQ_SM_GPIO0);
				goto init_irq_exit;
			}
			irq_init++;
			printk("GPIO irq init++ %d\n", irq_init);
init_irq_exit:
			mutex_unlock(&irq_init_mutex);
			return ret;
			//break;
		case GPIO_IOCTL_EXIT_IRQ:
			mutex_lock(&irq_init_mutex);
			if (irq_init > 0) {
				irq_init--;
				printk("GPIO irq init-- %d", irq_init);
			}
			if (irq_init > 0) {
				mutex_unlock(&irq_init_mutex);
				return 0;
			}
			/* can it disable the irq when last handle is freed? */
			free_irq(IRQ_APB_GPIOINST0, &gpio_dev_id);
			free_irq(IRQ_APB_GPIOINST1, &gpio_dev_id);
			free_irq(IRQ_APB_GPIOINST2, &gpio_dev_id);
			free_irq(IRQ_APB_GPIOINST3, &gpio_dev_id);
			free_irq(G_IRQ_SM_GPIO1, &gpio_dev_id);
			free_irq(G_IRQ_SM_GPIO0, &gpio_dev_id);
			mutex_unlock(&irq_init_mutex);
			break;
		default:
			return -EPERM;
	}
	return 0;
}

static unsigned int galois_gpio_poll(struct file *file, poll_table *wait)
{
	unsigned int __gpio_irq_status, __sm_gpio_irq_status;

	poll_wait(file, &gpio_wait_queue, wait);
	
	spin_lock(&gpio_irq_lock);
	__gpio_irq_status = gpio_irq_status;
	__sm_gpio_irq_status = sm_gpio_irq_status;
	spin_unlock(&gpio_irq_lock);

	if (__gpio_irq_status || __sm_gpio_irq_status)
		return (POLLIN | POLLRDNORM);
	return 0;
}

static struct file_operations galois_gpio_fops = {
	.owner		= THIS_MODULE,
	.open		= galois_gpio_open,
	.release	= galois_gpio_release,
	.write		= galois_gpio_write,
	.read		= galois_gpio_read,
	.ioctl		= galois_gpio_ioctl,
	.poll		= galois_gpio_poll,
};

static struct miscdevice gpio_dev = {
	.minor	= GALOIS_GPIO_MISC_MINOR,
	.name	= GALOIS_GPIO_NAME,
	.fops	= &galois_gpio_fops,
};

static int __init galois_gpio_init(void)
{
	int i;
	
	spin_lock_init(&gpio_irq_lock);

	for (i = 0; i < GPIO_PORT_NUM; i++)
		gpio_mode[i] = 0;
	for (i = 0; i < SM_GPIO_PORT_NUM; i++)
		sm_gpio_mode[i] = 0;

	if (misc_register(&gpio_dev))
		return -ENODEV;
	else
		return 0;
}

static void __exit galois_gpio_exit(void)
{
	int i;

	/* disable all GPIO interrupts */
	for (i = 0; i < GPIO_PORT_NUM; i++) {
		if (gpio_irq_mode & (1 << i))
			GPIO_PortDisableIRQ(i);
	}
	for (i = 0; i < SM_GPIO_PORT_NUM; i++) {
		if (sm_gpio_irq_mode & (1 << i))
			SM_GPIO_PortDisableIRQ(i);
	}

	/* un-registeer handler registered by GPIO */
	free_irq(IRQ_APB_GPIOINST0, &gpio_dev_id);
	free_irq(IRQ_APB_GPIOINST1, &gpio_dev_id);
	free_irq(IRQ_APB_GPIOINST2, &gpio_dev_id);
	free_irq(IRQ_APB_GPIOINST3, &gpio_dev_id);
	free_irq(G_IRQ_SM_GPIO1, &gpio_dev_id);
	free_irq(G_IRQ_SM_GPIO0, &gpio_dev_id);

	misc_deregister(&gpio_dev);
	return;
}

#ifdef CONFIG_PM
static galois_gpio_reg_t g_gpio_reg[2];
void galois_gpio_suspend(void)
{
	gpio_get_info(&g_gpio_reg[0]);
	sm_gpio_get_info(&g_gpio_reg[1]);
}

void galois_gpio_resume(void)
{
	int i;
	
	/* init io mapper */
#ifndef BERLIN
	unsigned int reg;

	spi_master_init_iomapper(IOMAPPER_SPI_MASTER);
	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 
			1, IOMAPPER_REG_MAP_10, &reg);
	if (reg & 0x1800) {	/* [12:11]=b00, GPIOmatrix0 */
		printk("GPIO matrix isn't configured correctly.\n");
		spi_master_exit_iomapper(IOMAPPER_SPI_MASTER);
		return -ENODEV;
	}
#endif

	/* restore irq */
	for (i = 0; i < SM_GPIO_PORT_NUM; i++) {
		if (gpio_irq_mode & (1 << i)) {
			GPIO_PortEnableIRQ(i);
			mv88de3010_open_irq(IRQ_APB_GPIOINST_NUM(i));
			galois_printk("Resume GPIO setting: enable IRQ for port %d\n", i);
		}
	}

	for (i = 0; i < SM_GPIO_PORT_NUM; i++) {
		if (sm_gpio_irq_mode & (1 << i)) {
			SM_GPIO_PortEnableIRQ(i);
			mv88de3010_open_irq(IRQ_APB_SMGPIOINST_NUM(i));
			galois_printk("Resume GPIO setting: enable IRQ for SM port %d\n", i);
		}
	}
	/* restore mode */
	for (i = 0; i < GPIO_PORT_NUM; i++) {
		int mode = gpio_mode[i];
		int data = g_gpio_reg[0].port_dr & (1<<i);	

		/* if mode is not changed */
		if (!(gpio_mode_status & (1<<i)))
			continue;
		
		/* restore mode of the port */
		gpio_set_mode(i, mode);
		galois_printk("Resume GPIO setting: port %d, mode %d", i, mode);
		
		/* restore data if port is out */
		if (mode == GPIO_MODE_DATA_OUT) {
			GPIO_PortWrite(i, data);
			galois_printk(", data %d", data);
		}
		galois_printk("\n");
	}

		/* restore mode */
	for (i = 0; i < GPIO_PORT_NUM; i++) {
		int mode = sm_gpio_mode[i];
		int data = g_gpio_reg[1].port_dr & (1<<i);	

		/* if mode is not changed */
		if (!(sm_gpio_mode_status & (1<<i)))
			continue;
		
		/* restore mode of the port */
		sm_gpio_set_mode(i, mode);
		galois_printk("Resume GPIO setting: SM port %d, mode %d", i, mode);
		
		/* restore data if port is out */
		if (mode == GPIO_MODE_DATA_OUT) {
			SM_GPIO_PortWrite(i, data);
			galois_printk(", data %d", data);
		}
		galois_printk("\n");
	}
}
#endif

module_init(galois_gpio_init);
module_exit(galois_gpio_exit);

MODULE_AUTHOR("Marvell-Galois");
MODULE_DESCRIPTION("Galois GPIO Driver");
MODULE_LICENSE("GPL");
