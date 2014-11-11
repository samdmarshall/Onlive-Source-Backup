#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <mach/galois_generic.h>
#include <linux/input.h>
#include "ir_main.h"
#include "apb_gpio.h"
#include "apb_gpio_ir.h"
#include "SM_common.h"

/*///////////////////////////////////////////////////////////////////////////////////////
Macro explanation:
CONFIG_MV88DE3010_IR_SM_INT:	If it defines, it means IR is from SM, and it works in interrup mode(interrrupt invoked in SM). (Default,defined) 
CONFIG_MV88DE3010_IR_SM:	If it defines, it means IR is from SM.(Default,defined) 
SM_TEST:			If it defines, it means IR devices is working in a special mode that tests SM module.(Default,undefined) 
////////////////////////////////////////////////////////////////////////////////////// */
//#define SM_TEST
#ifndef CONFIG_MV88DE3010_IR_SM
#define IRQ_APB_GPIOINST_NUM \
		(IR_PIN >= 24 ? IRQ_APB_GPIOINST3 : \
		 (IR_PIN >= 16 ? IRQ_APB_GPIOINST2 : \
		  (IR_PIN >= 8 ? IRQ_APB_GPIOINST1 : IRQ_APB_GPIOINST0)))
#define GPIO_IR_IRQ IRQ_APB_GPIOINST_NUM
static unsigned int girl_dev_id = 0x64143701;
#endif /* CONFIG_MV88DE3010_IR_SM */
#define GIRL_KEY_BUF_MAX_LEN 10
static DECLARE_WAIT_QUEUE_HEAD(girl_wait);

static DEFINE_SPINLOCK(my_lock);

typedef struct
{
	int girl_key_number;
 	int girl_buf[GIRL_KEY_BUF_MAX_LEN];
 	int girl_buf_write_pointer;
 	int girl_buf_read_pointer;
}GIRL_DEV;

static GIRL_DEV mygirl;
static int mygirl_init(void);
static int girl_key_buf_write(int key);
static int girl_key_buf_read(void);

static int mygirl_init(void)
{
	mygirl.girl_buf_write_pointer=0;
	mygirl.girl_buf_read_pointer=0;
	mygirl.girl_key_number=0;
	return 0;
}
static int girl_key_buf_write(int key)
{
	spin_lock(&my_lock);
	mygirl.girl_buf[mygirl.girl_buf_write_pointer]=key;
	mygirl.girl_buf_write_pointer++;
	mygirl.girl_key_number++;
	if(mygirl.girl_buf_write_pointer>=GIRL_KEY_BUF_MAX_LEN)
		mygirl.girl_buf_write_pointer%=GIRL_KEY_BUF_MAX_LEN;
	spin_unlock(&my_lock);
	return 0;
}
static int girl_key_buf_read(void)
{
	int key;
	spin_lock(&my_lock);
	key=mygirl.girl_buf[mygirl.girl_buf_read_pointer];
	mygirl.girl_buf_read_pointer++;
	mygirl.girl_key_number--;
	if(mygirl.girl_buf_read_pointer>=GIRL_KEY_BUF_MAX_LEN)
		mygirl.girl_buf_read_pointer%=GIRL_KEY_BUF_MAX_LEN;
	spin_unlock(&my_lock);
	return key;
}

static struct input_dev *ir_input = NULL;

#ifdef CONFIG_MV88DE3010_IR_MOUSE
static int ir_input_open(struct input_dev *dev);
static struct input_dev *irmouse;
static int emu_mouse = 0;
static struct input_dev *alloc_and_register(void)
{
	int error;
	struct input_dev *tmp = input_allocate_device();
	if (!tmp)
		return ERR_PTR(-ENOMEM);

	tmp->name = "IR emulated Mouse";
	tmp->phys = "irmouse/input0";
	tmp->id.bustype = BUS_HOST;
	tmp->id.vendor = 0x0001;
	tmp->id.product = 0x0001;
	tmp->id.version = 0x0100;
	tmp->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
	tmp->keybit[BIT_WORD(BTN_LEFT)] = BIT_MASK(BTN_LEFT) |
		BIT_MASK(BTN_RIGHT);
	tmp->relbit[0]	= BIT_MASK(REL_X) | BIT_MASK(REL_Y);
	tmp->open = ir_input_open;
	error = input_register_device(tmp);
	if (error) {
		printk("unable to register IR-emu mouse\n");
		input_free_device(tmp);
		return ERR_PTR(error);
	}
	return tmp;

}
static void mouse_device_fn(struct work_struct *dummy)
{
	if (emu_mouse) {
		irmouse = alloc_and_register();
		if (IS_ERR(irmouse)) {
			irmouse = NULL;
			emu_mouse = 0;
		}
	} else { 
		input_unregister_device(irmouse);
		input_free_device(irmouse);
	}

}
static DECLARE_WORK(irmouse_work, mouse_device_fn);
#define dx	10
#define dy	10
#define IR_MOUSE_UP	MV_IR_KEY_UP
#define IR_MOUSE_DOWN	MV_IR_KEY_DOWN
#define IR_MOUSE_LEFT	MV_IR_KEY_LEFT
#define IR_MOUSE_RIGHT	MV_IR_KEY_RIGHT
#define IR_MOUSE_LBTN	MV_IR_KEY_ENTER
#define IR_MOUSE_SWITCH MV_IR_KEY_INFO
#define REPORT_MOUSE(RELXY, xy)	do {				\
	if (ir_key & 0x80000000)				\
		input_report_rel(irmouse, RELXY, 2*xy);		\
	else							\
		input_report_rel(irmouse, RELXY, xy);		\
	input_sync(irmouse);					\
	} while(0)
#endif

static int ir_input_open(struct input_dev *dev)
{
	int msg;
	msg=MV_SM_IR_Linuxready;
	bsm_msg_send(MV_SM_ID_IR,&msg,4);
	return 0;
}

static void ir_report_key(int ir_key)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(keymap_tab); i++) {
		if (keymap_tab[i].ir_key == (ir_key & 0xFFFF)) {
			if (ir_key & 0x80000000)
				input_event(ir_input, EV_KEY, keymap_tab[i].input_key, 2);
			else
				input_event(ir_input, EV_KEY, keymap_tab[i].input_key, !(ir_key & 0x8000000));
			input_sync(ir_input);
		}
	}
}

static inline void ir_handle_key(int ir_key)
{
#ifdef CONFIG_MV88DE3010_IR_MOUSE
	if ((ir_key & 0x8000000) && (ir_key & 0xFFFF) == IR_MOUSE_SWITCH) {
		emu_mouse ^= 1;
		schedule_work(&irmouse_work);
	} else if (emu_mouse) {
		if ((ir_key & 0xFFFF) == IR_MOUSE_UP)
			REPORT_MOUSE(REL_Y, -dy);
		else if ((ir_key & 0xFFFF) == IR_MOUSE_DOWN)
			REPORT_MOUSE(REL_Y, dy);
		else if ((ir_key & 0xFFFF) == IR_MOUSE_LEFT)
			REPORT_MOUSE(REL_X, -dx);
		else if ((ir_key & 0xFFFF) == IR_MOUSE_RIGHT)
			REPORT_MOUSE(REL_X, dx);
		else if ((ir_key & 0xFFFF) == IR_MOUSE_LBTN)
			input_report_key(irmouse, BTN_LEFT, !(ir_key&0x8000000));
		else
			ir_report_key(ir_key);
	} else
#endif
	ir_report_key(ir_key);
}

/*
 * if ir works in interrup mode(interrrupt invoked in SM), 
 * don't need the poll or interrupt(IR's interrupt) method.
 */
#ifndef CONFIG_MV88DE3010_IR_SM_INT
static struct task_struct *girl_task;

#ifdef CONFIG_MV88DE3010_IR_SM
unsigned int ir_key = 0;
unsigned int ir_key_recvflag = 0;
#else
static irqreturn_t girl_intr(int irq, void *dev_id)
{
	if (infrared_int_handler(0))
		return IRQ_NONE;
	else
		return IRQ_HANDLED;
}
#endif /* CONFIG_MV88DE3010_IR_SM */

static int girl_receive_key(void *arg)
{
	while (!kthread_should_stop()) {
		//printk("girl_receive_key\n");
		//set_current_state(TASK_INTERRUPTIBLE);
#ifndef CONFIG_MV88DE3010_IR_SM
		readout_ircbuffer();
#else /* CONFIG_MV88DE3010_IR_SM */
		{
			int iMsgBody[8], iMsgLen = 0, iRet;

			iRet = bsm_msg_receive( MV_SM_ID_IR/*IR module id*/, iMsgBody, &iMsgLen );
			if( iRet != 0 ||  iMsgLen != 4 )
			{
				msleep(100);
				continue;
			}

			ir_key_recvflag = 1;
			ir_key = iMsgBody[0];
		}
#endif /* CONFIG_MV88DE3010_IR_SM */

		if (ir_key_recvflag) {
			ir_key_recvflag = 0;
			ir_handle_key(ir_key);
			//printk("[GIRL key]=0x%08x\n", ir_key);

			/* wake up userland process */
			girl_key_buf_write(ir_key);
			wake_up_interruptible(&girl_wait);
		}
		//schedule();
		msleep(100);
	}

	return 0;
}

#else /* CONFIG_MV88DE3010_IR_SM_INT */

/*
 * when sm receives msg and the msg is to ir, 
 * sm's interrupt function will call this function.
 * (And sm needs to work under interrupt mode) 
 */
int girl_sm_int(void )
{	
	int iMsgBody[8], iMsgLen = 0, iRet;
	int ir_key;

	iRet = bsm_msg_receive( MV_SM_ID_IR, iMsgBody, &iMsgLen );
	
	if( iRet != 0 ||  iMsgLen != 4 )
	return 0;//no new ir key
	
	ir_key = iMsgBody[0];;
	ir_handle_key(ir_key);
	girl_key_buf_write(ir_key);
//	printk("Girl sm int in ir.c,key is %#X \n",ir_key);
	wake_up_interruptible(&girl_wait);
	return 0;
	
}
EXPORT_SYMBOL(girl_sm_int);
#endif /* CONFIG_MV88DE3010_IR_SM_INT */

static int galois_ir_open(struct inode *inode, struct file *file)
{
	int msg;

#ifndef CONFIG_MV88DE3010_IR_SM_INT
	int ret;

	/* create ir thread */
	girl_task = kthread_create(girl_receive_key, NULL, GALOIS_IR_NAME);
	if (IS_ERR(girl_task)) {
		ret = PTR_ERR(girl_task);
		girl_task = NULL;
		return ret;
	}
	girl_task->flags |= PF_NOFREEZE;
	wake_up_process(girl_task);

#ifndef CONFIG_MV88DE3010_IR_SM
	/* register irq */
	ret = request_irq(GPIO_IR_IRQ, girl_intr, IRQF_SHARED, GALOIS_IR_NAME, &girl_dev_id);
	if (ret) {
		//ir_gpio_exit();
		kthread_stop(girl_task);
		printk("error: failed to request_irq %d.\n", GPIO_IR_IRQ);
		return ret;
	}
	ir_gpio_init();
	mv88de3010_open_irq(GPIO_IR_IRQ);
#endif
#endif /* CONFIG_MV88DE3010_IR_SM_INT */

#ifndef SM_TEST
	msg=MV_SM_IR_Linuxready;
	bsm_msg_send(MV_SM_ID_IR,&msg,4);
#endif

	return 0;
}

static int galois_ir_release(struct inode *inode, struct file *file)
{
#ifndef CONFIG_MV88DE3010_IR_SM_INT
#ifndef CONFIG_MV88DE3010_IR_SM
	mv88de3010_close_irq(GPIO_IR_IRQ);
	ir_gpio_exit();
	free_irq(GPIO_IR_IRQ, &girl_dev_id);
#endif
	kthread_stop(girl_task);
#endif /* CONFIG_MV88DE3010_IR_SM_INT */
	return 0;
}

static ssize_t galois_ir_read(struct file *file, char __user * buf,
		size_t count, loff_t *ppos)
{
	int ret;
	int girl_key=girl_key_buf_read();
	ret = put_user(girl_key, (unsigned int __user *)buf);

	if (ret)
		return ret;
	else
		return sizeof(girl_key);
}

static ssize_t galois_ir_write(struct file *file, const char __user * buf,
		size_t count, loff_t *ppos)
{
	printk("error: doesn't support write.\n");
	return -EFAULT;
}

#define GIRL_READ	0x1234
static int galois_ir_ioctl(struct inode *inode, struct file *file, 
		unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	switch (cmd) {
		case GIRL_READ:
			/* GIRL_READ */
			ret = -EINVAL;
			break;
		default:
			ret = -EINVAL;
			break;
	}
	return 0;
}

static unsigned int galois_ir_poll(struct file *file, poll_table *wait)
{
	/* 
	 * poll_wait will call wait->qproc(file, &girl_wait, wait)
	 * i.e. __pollwait(...) to add itself into &girl_wait queue.
	 */
	int tmp;
 	spin_lock(&my_lock);
	tmp=mygirl.girl_key_number;
 	spin_unlock(&my_lock);
	if (!tmp)
		poll_wait(file, &girl_wait, wait);
 	spin_lock(&my_lock);
	tmp=mygirl.girl_key_number;
 	spin_unlock(&my_lock);
	if (tmp)
		return (POLLIN | POLLRDNORM);
	return 0;
}

static struct file_operations galois_ir_fops = {
	.owner		= THIS_MODULE,
	.open		= galois_ir_open,
	.release	= galois_ir_release,
	.write		= galois_ir_write,
	.read		= galois_ir_read,
	.ioctl		= galois_ir_ioctl,
	.poll		= galois_ir_poll,
};

static struct miscdevice ir_dev = {
	.minor	= GALOIS_IR_MISC_MINOR,
	.name	= GALOIS_IR_NAME,
	.fops	= &galois_ir_fops,
};

static int __init galois_ir_init(void)
{
	int i, error;

	if (misc_register(&ir_dev))
		return -ENODEV;
	
	mygirl_init();

	ir_input = input_allocate_device();
	if (!ir_input) {
		printk("error: failed to alloc input device for IR.\n");
		misc_deregister(&ir_dev);
		return -ENOMEM;
	}
	
	ir_input->name = "Inafra-Red";
	ir_input->phys = "Inafra-Red/input0";
	ir_input->id.bustype = BUS_HOST;
	ir_input->id.vendor = 0x0001;
	ir_input->id.product = 0x0001;
	ir_input->id.version = 0x0100;
	
	ir_input->open = ir_input_open;
	
	for(i = 0; i < ARRAY_SIZE(keymap_tab); i++)
		__set_bit(keymap_tab[i].input_key, ir_input->keybit);
	__set_bit(EV_KEY, ir_input->evbit);
	
	error = input_register_device(ir_input);
	if (error) {
		printk("error: failed to register input device for IR\n");
		misc_deregister(&ir_dev);
		input_free_device(ir_input);
		return error;
	}
	return 0;
}

static void __exit galois_ir_exit(void)
{
	misc_deregister(&ir_dev);
	input_unregister_device(ir_input);
	input_free_device(ir_input);
#ifdef CONFIG_MV88DE3010_IR_MOUSE
	if (emu_mouse) {
		input_unregister_device(irmouse);
		input_free_device(irmouse);
	}
#endif
	return;
}

module_init(galois_ir_init);
module_exit(galois_ir_exit);

MODULE_AUTHOR("Marvell-Galois");
MODULE_DESCRIPTION("Galois Infra-Red Driver");
MODULE_LICENSE("GPL");
