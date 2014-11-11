#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <mach/galois_generic.h>

#include "softkeyboard.h"

static DECLARE_WAIT_QUEUE_HEAD(soft_keyboard_wait);

static DEFINE_SPINLOCK(my_lock);

static struct input_dev *ir_input = NULL;
typedef struct
{
	int soft_keyboard_key_number;
 	int soft_keyboard_buf[SOFTIR_KEY_BUF_MAX_LEN];
 	int soft_keyboard_buf_write_pointer;
 	int soft_keyboard_buf_read_pointer;
}SOFTIR_DEV;

static SOFTIR_DEV softkeyboard;
static int softkeyboard_init(void);
static int soft_keyboard_key_buf_read(void);
static void soft_keyboard_key_buf_write(int key);

static int softkeyboard_init(void)
{
	softkeyboard.soft_keyboard_buf_write_pointer=0;
	softkeyboard.soft_keyboard_buf_read_pointer=0;
	softkeyboard.soft_keyboard_key_number=0;
	return SUCCESS;
}

static int soft_keyboard_key_buf_read(void)
{
	int key;
	spin_lock(&my_lock);
	key=softkeyboard.soft_keyboard_buf[softkeyboard.soft_keyboard_buf_read_pointer];
	softkeyboard.soft_keyboard_buf_read_pointer++;
	softkeyboard.soft_keyboard_key_number--;
	if(softkeyboard.soft_keyboard_buf_read_pointer>=SOFTIR_KEY_BUF_MAX_LEN)
		softkeyboard.soft_keyboard_buf_read_pointer%=SOFTIR_KEY_BUF_MAX_LEN;
	spin_unlock(&my_lock);
	return key;
}

static void soft_keyboard_key_buf_write(int key)
{
	spin_lock(&my_lock);
	softkeyboard.soft_keyboard_buf[softkeyboard.soft_keyboard_buf_write_pointer]= key;
	softkeyboard.soft_keyboard_buf_write_pointer++;
	softkeyboard.soft_keyboard_key_number++;
	if(softkeyboard.soft_keyboard_buf_write_pointer>=SOFTIR_KEY_BUF_MAX_LEN)
		softkeyboard.soft_keyboard_buf_write_pointer%=SOFTIR_KEY_BUF_MAX_LEN;
	spin_unlock(&my_lock);
	return;
}

static int galois_soft_keyboard_open(struct inode *inode, struct file *file)
{
	return SUCCESS;
}

static int galois_soft_keyboard_release(struct inode *inode, struct file *file)
{

	return SUCCESS;
}

static ssize_t galois_soft_keyboard_read(struct file *file, char __user * buf,
		size_t count, loff_t *ppos)
{
	int ret;
	int soft_keyboard_key=soft_keyboard_key_buf_read();
	ret = put_user(soft_keyboard_key, (unsigned int __user *)buf);

	if (ret)
		return ret;
	else
		return sizeof(soft_keyboard_key);
}

static ssize_t galois_soft_keyboard_write(struct file *file, const char __user * buf,
		size_t count, loff_t *ppos)
{
	struct input_event levt;

	if(count != sizeof(levt)) 
		return -EFAULT;

	if (copy_from_user(&levt, buf, sizeof(levt)))
		return -EFAULT;

	spin_lock(&my_lock);
	input_event(ir_input, EV_KEY, levt.code , levt.value);
	input_sync(ir_input);
	spin_unlock(&my_lock);
	return SUCCESS;
}

static int galois_soft_keyboard_ioctl(struct inode *inode, struct file *file, 
		unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	switch (cmd) {
		case SOFTIR_READ:
			ret = -EINVAL;
			break;
		default:
			ret = -EINVAL;
			break;
	}

	return ret;
}

static unsigned int galois_soft_keyboard_poll(struct file *file, poll_table *wait)
{
	int tmp;
 	spin_lock(&my_lock);
	tmp=softkeyboard.soft_keyboard_key_number;
 	spin_unlock(&my_lock);
	 if(!tmp)
		poll_wait(file, &soft_keyboard_wait, wait);
 	spin_lock(&my_lock);
	tmp=softkeyboard.soft_keyboard_key_number;
 	spin_unlock(&my_lock);
	if (tmp)
		return (POLLIN | POLLRDNORM);
	return SUCCESS;
}

static struct file_operations galois_soft_keyboard_fops = {
	.owner		= THIS_MODULE,
	.open		= galois_soft_keyboard_open,
	.release	= galois_soft_keyboard_release,
	.write		= galois_soft_keyboard_write,
	.read		= galois_soft_keyboard_read,
	.ioctl		= galois_soft_keyboard_ioctl,
	.poll		= galois_soft_keyboard_poll,
};

static struct miscdevice swkbd_dev = {
	.minor	= GALOIS_SOFT_KEYBOARD_MISC_MINOR,
	.name	= GALOIS_SOFT_KEYBOARD_NAME,
	.fops	= &galois_soft_keyboard_fops,
};

static int ir_input_open(struct input_dev *dev){

	return SUCCESS;
}

static void ir_input_close(struct input_dev *dev){

	return;
}

static int __init galois_soft_keyboard_init(void)
{
	int i, error;

	if (misc_register(&swkbd_dev)) {
		printk("misc_register failed \n");
		return -ENODEV; 
	}
	
	softkeyboard_init();

	ir_input = input_allocate_device();
	if(!ir_input){
		printk("error: failed to alloc input device for IR.\n");
		misc_deregister(&swkbd_dev);
		return -ENOMEM;
	}

	ir_input->name = "sw_kbd";
	ir_input->phys = "sw_kbd/input2";

	ir_input->id.bustype = BUS_HOST;
	ir_input->id.vendor = 0x0001;
	ir_input->id.product = 0x0001;
	ir_input->id.version = 0x0100;

	ir_input->open = ir_input_open;
	ir_input->close = ir_input_close;

	for(i = 0; i < ARRAY_SIZE(soft_keyboard_tab); i++)
		__set_bit(soft_keyboard_tab[i].input_key, ir_input->keybit);

	__set_bit(EV_KEY, ir_input->evbit);
	__set_bit(LED_NUML,    ir_input->ledbit);
	__set_bit(LED_CAPSL,   ir_input->ledbit);
	__set_bit(LED_SCROLLL, ir_input->ledbit);

	error = input_register_device(ir_input);

	if(error){
		printk("error: failed to register input device for IR\n");
		misc_deregister(&swkbd_dev);
		input_free_device(ir_input);
		return -EFAULT;
	}

	printk("Inside galois_soft_keyboard_init() SUCCESS\n");
	return SUCCESS;
}

static void __exit galois_soft_keyboard_exit(void)
{
	misc_deregister(&swkbd_dev);
	input_unregister_device(ir_input);
	input_free_device(ir_input);
	printk("Inside galois_soft_keyboard_exit() SUCCESS\n");
	return;
}

module_init(galois_soft_keyboard_init);
module_exit(galois_soft_keyboard_exit);

MODULE_AUTHOR("Marvell-Galois");
MODULE_DESCRIPTION("Galois Soft-Keyboard Driver");
MODULE_LICENSE("GPL");
