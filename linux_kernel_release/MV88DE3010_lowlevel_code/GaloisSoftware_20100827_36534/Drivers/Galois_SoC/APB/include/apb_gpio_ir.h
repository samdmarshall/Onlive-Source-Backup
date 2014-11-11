#ifndef __APB_GPIO_IR_H__
#define __APB_GPIO_IR_H__

#if defined(__ECOS__)
 #define uart_putc(d)		//diag_printf("%c", d)
 #define dbgprintf(...)		//diag_printf(...)
#elif defined(__LINUX_KERNEL__)
 #define uart_putc(d)		//printk("%c", d)
 #define dbgprintf(...)		//printk(...)
 #define diag_printf		printk
#elif defined( NON_OS )
 #define uart_putc(d)		//printk("%c", d)
 #define dbgprintf(...)		//printk(...)
#else
 #define uart_putc(d) 
 #define dbgprintf(...) 
 #define diag_printf
#endif

/*
 * Ugly macro :-)
 */
#define DISABLE_IRINT()
#define ENABLE_IRINT()

/*
 * Infrared pin definition
 */
#define IR_PIN	16
#define IR_PIN_BIT (IR_PIN % 8)
#define IR_GPIO_BASE \
	(IR_PIN >= 24 ? APB_GPIO_INST3_BASE : \
	 (IR_PIN >= 16 ? APB_GPIO_INST2_BASE : \
	  (IR_PIN >= 8 ? APB_GPIO_INST1_BASE : APB_GPIO_INST0_BASE)))

#define IR_LSbGpioInst \
	(IR_PIN >= 24 ? LSbGpioInst3 : \
	 (IR_PIN >= 16 ? LSbGpioInst2 : \
	  (IR_PIN >= 8 ? LSbGpioInst1 : LSbGpioInst0)))

/*
 * SYSTEM_HZ is OS's scheduler tick frequecy, in Hz
 */
#if defined(__LINUX_KERNEL__) || defined(__ECOS__)
#define SYSTEM_HZ	100
#else
#define SYSTEM_HZ	20 /* ? */
#endif
#define IR_IDLE_JIFFIES	(100 * SYSTEM_HZ / 1000) /* SYSTEM_HZ/10 tick, i.e.100ms */
#define IR_IDLE_LENGTH	0x7fffffff

/*
 * the key matching condition is:
 * if (((received_code >> key_bitpos) & key_mask) == ir_encode), 
 * then output "ir_key"
 */
typedef struct {
	unsigned int key_bitpos;
	unsigned int key_mask;
	unsigned int ir_encode;
	unsigned int ir_key;
} ir_key_table_t;

//#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

extern unsigned int ir_key;
extern unsigned int ir_key_recvflag;

void readout_ircbuffer(void);
int infrared_int_handler(unsigned int data);
void ir_gpio_init(void);
void ir_gpio_exit(void);

int infrared_init(void);	/* deprecated, use ir_gpio_init instead */

#endif
