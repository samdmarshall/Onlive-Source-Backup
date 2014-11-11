/*
Function: Power Manager module, including stand by function
Started on 04-02-2009,by hjcai
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/kernel_stat.h>
#include <linux/input.h>
#include <mach/galois_generic.h>
#include "SM_common.h"
#include "global.h"
#include "galois_io.h"
#include "mach/freq.h"

#define	BFM_HOST_Bus_Write32	GA_REG_WORD32_WRITE
#define	BFM_HOST_Bus_Read32	GA_REG_WORD32_READ
//#define AutoWarmdown
void diag_clock_change_cpu1Clk(unsigned int pllSwitch, unsigned int pllSelect, unsigned int divider);
void diag_clock_change_otherClk(unsigned int index, unsigned int pllSwitch, unsigned int pllSelect, unsigned int divider);

void print_clock_info(void) ; 

static unsigned long Mvpm_InputEvt;
extern unsigned int init_cpu0_cpu1_clock_divider[2] ;

typedef struct {
	int	CPUBusyLimit; //when cpu usage > (CPUBusyLimit)%, the cpu is defined as busy and Berlin can not enter standby
	int InputIdleLimit;//If the input has no event in (InputIdleLimit)seconds,Berlin may enter standby, also, the cpu usage should be considered.
 	int CPUTimerInv; //5 seconds a boot 
	int DetectTimerInv; //30 seconds a boot 
	int cpucurrusage;
	cputime64_t lastidle;
	cputime64_t lasttotal;
	struct timer_list DetectTimer;
	struct timer_list CPUUsageTimer;
} PM_DEV;

static PM_DEV dev;


static int EnterStandby(void)
{
	int ret=0;	
	int msg=MV_SM_POWER_WARMDOWN_REQUEST;
	//printk("To enter Standby\n");
	bsm_msg_send(MV_SM_ID_POWER,&msg,sizeof(int));
	return ret;
}

static int enter_low_speed_mode(void)
{
	int ret = 0 ; 
	TGbl_clkSelect		clkSelect;
	TGbl_ClkSwitch		ClkSwitch;
	unsigned int 		pllSwitch;
	unsigned int 		pllSelect;

	printk("enter low speed mode\n") ; 
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);


	pllSwitch = ClkSwitch.uClkSwitch_cpu1ClkPllSwitch ; 
	pllSelect = clkSelect.uclkSelect_cpu1ClkPllSel ;
	printk("change cpu1 clock, set divider to 3\n") ;
	diag_clock_change_cpu1Clk(pllSwitch, pllSelect, 3) ;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &clkSelect.u32[2]);
	pllSelect = clkSelect.uclkSelect_cpu0ClkPllSel ;
	pllSwitch = ClkSwitch.uClkSwitch_cpu0ClkPllSwitch ;
	printk("change cpu0 clock, set divider to 3\n") ;
	diag_clock_change_otherClk(0, pllSwitch, pllSelect, 3) ;

	print_clock_info() ;
	return ret ; 
}

static int enter_sleep_mode(void)
{
	int ret = 0 ; 
	TGbl_clkSelect		clkSelect;
	TGbl_ClkSwitch		ClkSwitch;
	unsigned int 		pllSwitch;
	unsigned int 		pllSelect;

	printk("enter sleep mode\n") ; 

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);


	pllSwitch = ClkSwitch.uClkSwitch_cpu1ClkPllSwitch ; 
	pllSelect = clkSelect.uclkSelect_cpu1ClkPllSel ;
	printk("change cpu1 clock, set divider to 8\n") ;
	diag_clock_change_cpu1Clk(pllSwitch, pllSelect, 8) ;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &clkSelect.u32[2]);
	pllSelect = clkSelect.uclkSelect_cpu0ClkPllSel ;
	pllSwitch = ClkSwitch.uClkSwitch_cpu0ClkPllSwitch ;
	printk("change cpu0 clock, set divider to 8\n") ;
	diag_clock_change_otherClk(0, pllSwitch, pllSelect, 8) ;

	print_clock_info() ;

	return ret ; 
}

static int enter_normal_speed_mode(void)
{
	int ret = 0 ; 
	TGbl_clkSelect		clkSelect;
	TGbl_ClkSwitch		ClkSwitch;
	unsigned int 		pllSwitch;
	unsigned int 		pllSelect;

	printk("enter normal speed mode\n") ; 
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);


	pllSwitch = ClkSwitch.uClkSwitch_cpu1ClkPllSwitch ; 
	pllSelect = clkSelect.uclkSelect_cpu1ClkPllSel ;
	printk("change cpu1 clock, set divider to %d\n", init_cpu0_cpu1_clock_divider[1]) ;
	diag_clock_change_cpu1Clk(pllSwitch, pllSelect, init_cpu0_cpu1_clock_divider[1]) ;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &clkSelect.u32[2]);
	pllSelect = clkSelect.uclkSelect_cpu0ClkPllSel ;
	pllSwitch = ClkSwitch.uClkSwitch_cpu0ClkPllSwitch ;
	printk("change cpu0 clock, set divider to %d\n", init_cpu0_cpu1_clock_divider[0]) ;
	diag_clock_change_otherClk(0, pllSwitch, pllSelect, init_cpu0_cpu1_clock_divider[0]) ;

	print_clock_info() ;
	return ret ; 

}


//calculate cpu usage rate, return percentage,eg.40% will return 40
static int CPUStatCalc(void)
{
	int pcnt=0;

	cputime64_t user, nice, system, idle, iowait, irq, softirq, steal,total;
	int realidle,realtotal;
	user = nice = system = idle = iowait =irq = softirq = steal = cputime64_zero;
	
	//single cpu,so it will be cpu(0)
	user = kstat_cpu(0).cpustat.user;
	nice = kstat_cpu(0).cpustat.nice;
	system = kstat_cpu(0).cpustat.system;
	idle = kstat_cpu(0).cpustat.idle;
	iowait = kstat_cpu(0).cpustat.iowait;
	irq = kstat_cpu(0).cpustat.irq;
	softirq = kstat_cpu(0).cpustat.softirq;
	//steal = kstat_cpu(0).cpustat.steal;
	
	total=user+nice+system+idle+iowait+irq+softirq;
	
	realtotal=(int)(total-dev.lasttotal);
	realidle=(int)(idle-dev.lastidle);
	
	dev.lastidle=idle;
	dev.lasttotal=total;

	pcnt=(dev.CPUTimerInv*HZ-realidle)*100/(dev.CPUTimerInv*HZ);//pcnt=(realtotal-realidle)/realtotal,acutally realtotal=HZ*CPUTimerInv
	return pcnt;		
}
//Timer Boots every DetectTimerInv seconds
static void DetectTimerBoot(unsigned long arg)
{
	//Policy maker:decide whether enter standby
	unsigned long inputjifdiff;
	int inputsecdiff;
	
	//printk("DetectTimer boot\n");
	inputjifdiff=(long)jiffies-(long)Mvpm_InputEvt;
	inputsecdiff=inputjifdiff/HZ;
	
	//printk("no input for %d seconds\n", inputsecdiff);
	if (dev.cpucurrusage < dev.CPUBusyLimit && inputsecdiff > dev.InputIdleLimit)
		EnterStandby();

	mod_timer(&dev.DetectTimer,jiffies+dev.DetectTimerInv*HZ);
}

static void CPUUsageTimerBoot(unsigned long arg)
{		
	dev.cpucurrusage=CPUStatCalc();
	
	//printk("CPU current usage is %d %%\n",cpucurrusage);
	mod_timer(&dev.CPUUsageTimer,jiffies+dev.CPUTimerInv*HZ);
}

static int galois_mvpm_open(struct inode *inode, struct file *file)
{
#ifdef AutoWarmdown
	cputime64_t user, nice, system, idle, iowait, irq, softirq, steal,total;
	
	Mvpm_InputEvt=jiffies;
	
	init_timer(&dev.DetectTimer);
	dev.DetectTimer.function=&DetectTimerBoot;
	dev.DetectTimer.expires=jiffies+dev.DetectTimerInv*HZ;//30 seconds
	add_timer(&dev.DetectTimer);
		
	user = nice = system = idle = iowait =irq = softirq = steal = cputime64_zero;

	user = kstat_cpu(0).cpustat.user;
	nice = kstat_cpu(0).cpustat.nice;
	system = kstat_cpu(0).cpustat.system;
	idle = kstat_cpu(0).cpustat.idle;
	iowait = kstat_cpu(0).cpustat.iowait;
	irq = kstat_cpu(0).cpustat.irq;
	softirq = kstat_cpu(0).cpustat.softirq;
	steal = kstat_cpu(0).cpustat.steal;

	total=user+nice+system+idle+iowait+irq+softirq+steal;

	dev.lastidle=idle;
	dev.lasttotal=total;
		
	//cpu usage timer setting
	init_timer(&dev.CPUUsageTimer);
	dev.CPUUsageTimer.function=&CPUUsageTimerBoot;
	dev.CPUUsageTimer.expires=jiffies+dev.CPUTimerInv*HZ;//5 seconds
	add_timer(&dev.CPUUsageTimer);
#endif 
	return 0;
}

static int galois_mvpm_release(struct inode *inode, struct file *file)
{
#ifdef AutoWarmdown
	del_timer(&dev.DetectTimer);
	del_timer(&dev.CPUUsageTimer);
#endif
	return 0;
}
static ssize_t galois_mvpm_read(struct file *file, char __user * buf,
		size_t count, loff_t *ppos)
{
	return 0;

}

static ssize_t galois_mvpm_write(struct file *file, const char __user * buf,
		size_t count, loff_t *ppos)
{
	int ret=0;
/*	char * mybuf;
	
	if(count>SM_MSG_BODY_SIZE) 
		return -1;
	
	mybuf=(char *)kmalloc(SM_MSG_BODY_SIZE,GFP_KERNEL);
	ret=copy_from_user(mybuf,buf,count);
	if(ret)
		return ret;
	ret=sm_write(mybuf,count,ppos);
	kfree(mybuf);
*/	return ret;
}

#include <linux/suspend.h>

static int galois_mvpm_ioctl(struct inode *inode, struct file *file, 
		unsigned int cmd, unsigned long arg)
{
	int ret=0;
	switch(cmd){
		case 	CMD_WARMDOWN:
			EnterStandby();
			break;
		case	CMD_WARMDOWN_2:
			pm_suspend(PM_SUSPEND_MEM);
			break;
		case	CMD_WARMDOWN_ONTIME:
			{
				int msgbody[2];
				msgbody[0]=MV_SM_POWER_WARMDOWN_TIME;
				msgbody[1]=arg;
				bsm_msg_send(MV_SM_ID_POWER,msgbody,sizeof(int)*2);
				printk("mvpm warmdown on time\n");
				break;
			}
		case	CMD_WARMUP_ONTIME:
			{
				int msgbody[2];
				msgbody[0]=MV_SM_POWER_WARMUP_TIME;
				msgbody[1]=arg;
				bsm_msg_send(MV_SM_ID_POWER,msgbody,sizeof(int)*2);
				printk("mvpm warmup on time\n");
				break;
			}
		case	CMD_SET_CPUBusyLimit:
			dev.CPUBusyLimit=arg;
			break;
		case	CMD_SET_InputIdleLimit:
			dev.InputIdleLimit=arg;
			break;
		case	CMD_SET_CPUTimerInv:
			dev.CPUTimerInv=arg;
			break;
		case	CMD_SET_DetectTimerInv:
			dev.DetectTimerInv=arg;
			break;
		case 	CMD_ENTER_LOW_SPEED_MODE:
			//	enter a mode which CPU runs at a lower speed
			//	but user's experience do not changed
			enter_low_speed_mode() ;
			break;
		case	CMD_ENTER_SLEEP_MODE:
			//	enter a mode which CPU runs at a very low speed
			//	if user want to wake up system. it takes some time
			enter_sleep_mode() ; 
			break; 
		case	CMD_PRINT_CLOCK_INFO:
			print_clock_info() ;
			break ; 
		case	CMD_ENTER_NORMAL_SPEED_MODE:
			enter_normal_speed_mode() ;
			break ; 
		default:
		;			
	}
	return ret;
}

static struct file_operations galois_mvpm_fops = {
	.owner		= THIS_MODULE,
	.open		= galois_mvpm_open,
	.release	= galois_mvpm_release,
	.write		= galois_mvpm_write,
	.read		= galois_mvpm_read,
	.ioctl		= galois_mvpm_ioctl,
};

static struct miscdevice mvpm_dev = {
	.minor	= BERLIN_MVPM_MISC_MINOR,
	.name	= BERLIN_MVPM_NAME,
	.fops	= &galois_mvpm_fops,
};

static void mvpm_event(struct input_handle *handle, unsigned int type,
			unsigned int code, int data)
{
	Mvpm_InputEvt = jiffies;
}

static int mvpm_connect(struct input_handler *handler, struct input_dev *dev,
			  const struct input_device_id *id)
{
	struct input_handle *handle;
	int error;

	handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
	if (!handle)
		return -ENOMEM;

	handle->dev = dev;
	handle->handler = handler;
	handle->name = "mvpm";

	error = input_register_handle(handle);
	if (error)
		goto err_free_handle;

	error = input_open_device(handle);
	if (error)
		goto err_unregister_handle;

	return 0;

 err_unregister_handle:
	input_unregister_handle(handle);
 err_free_handle:
	kfree(handle);
	return error;
}

static void mvpm_disconnect(struct input_handle *handle)
{
	input_close_device(handle);
	input_unregister_handle(handle);
	kfree(handle);
}

static const struct input_device_id mvpm_ids[] = {
	{
		.flags = INPUT_DEVICE_ID_MATCH_EVBIT,
		.evbit = { BIT_MASK(EV_KEY) },
	},
	{ },
};

static struct input_handler mvpm_handler = {
	.name = "mvpm",
	.event = mvpm_event,
	.connect = mvpm_connect,
	.disconnect = mvpm_disconnect,
	.id_table = mvpm_ids,
};

static int __init galois_mvpm_init(void)
{
//	printk("mvpm module init\n");
	dev.CPUBusyLimit=5;//when cpu usage > (CPUBusyLimit)%, the cpu is defined as busy and Berlin can not enter standby
	dev.InputIdleLimit=300;//If the input has no event in (InputIdleLimit)seconds,Berlin may enter standby, also, the cpu usage should be considered.
	dev.CPUTimerInv=5;//5 seconds a boot 
	dev.DetectTimerInv=30;//30 seconds a boot 
	dev.lasttotal=0;
	dev.lastidle=0;
	if (misc_register(&mvpm_dev))
		return -ENODEV;
	else
		return input_register_handler(&mvpm_handler);
}

static void __exit galois_mvpm_exit(void)
{
	input_unregister_handler(&mvpm_handler);
	misc_deregister(&mvpm_dev);
	return;
}

module_init(galois_mvpm_init);
module_exit(galois_mvpm_exit);

MODULE_AUTHOR("Marvell-Galois");
MODULE_DESCRIPTION("Galois PowerManager Driver");
MODULE_LICENSE("GPL");

//----------------------------------- clockDiag.c 
enum divider_index
{
	DIVIDED_BY_2 = 1,
	DIVIDED_BY_4,
	DIVIDED_BY_6,
	DIVIDED_BY_8,
	DIVIDED_BY_12
};

static char *AVPLL[] =
{
	"AVPllA5",
	"AVPllA6",
	"AVPllB6",
	"AVPllB7",
	"CPU1Pll"
};

static float VcoDiv[] = 
{
	1,		/// * 0000: 1
	1.5,	/// * 0001: 1.5
	2,		/// * 0010: 2
	2.5,	/// * 0011: 2.5
	3,		/// * 0100: 3
	4,		/// * 0101: 4
	8,		/// * 0110: 8
	6,		/// * 0111: 6
	1,		/// * 1000: undefined
	10,		/// * 1001: 10
	12,		/// * 1010: 12
	14,		/// * 1011: 14
	1,		/// * 1100: undefined
	1,		/// * 1101: undefined
	1,		/// * 1110: undefined
	1		/// * 1111: undefined
};

enum Clocks
{
	CPU0CLK,
	SYSCLK,
	DRMCLK,
	CFGCLK,
	GFXCLK,
	ZSPCLK,
	PERIFCLK,
	PCUBECLK,
	VSCOPECLK,
	NFCECCCLK,
	VPPSYSCLK,
	APPCLK,
	NONCLK,
};

static const char* g_aClocks[] =
{
	"cpu0Clk",
	"sysClk",
	"drmClk",
	"cfgClk",
	"gfxClk",
	"zspClk",
	"perifClk",
	"pCubeClk",
	"vScopeClk",
	"nfcEccClk",
	"vppSysClk",
	"appClk",
};


void diag_clock_change_cpu1Clk(unsigned int pllSwitch, unsigned int pllSelect, unsigned int divider)
{
	TGbl_clkSelect		clkSelect;
	TGbl_ClkSwitch		ClkSwitch;
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);
	
#ifdef ALWAYS_BYPASS
	// cpu1Pll bypass ON
	ClkSwitch.uClkSwitch_cpu1PLLSWBypass = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
#endif

#if 0
	//	do not change swith first
	// cpu1Clk use default cpu1Pll
	ClkSwitch.uClkSwitch_cpu1ClkPllSwitch = 0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);


	// change divider to divided-by-3 first
	ClkSwitch.uClkSwitch_cpu1ClkD3Switch = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

#endif
	// change divider to target
	switch (divider)
	{
		case 1:
			ClkSwitch.uClkSwitch_cpu1ClkSwitch = 0;
			break;
		case 2:
			ClkSwitch.uClkSwitch_cpu1ClkSwitch = 1;
			clkSelect.uclkSelect_cpu1ClkSel = DIVIDED_BY_2;
			break;
		case 3:
			ClkSwitch.uClkSwitch_cpu1ClkD3Switch = 1;
			break;
		case 4:
			ClkSwitch.uClkSwitch_cpu1ClkSwitch = 1;
			clkSelect.uclkSelect_cpu1ClkSel = DIVIDED_BY_4;
			break;
		case 6:
			ClkSwitch.uClkSwitch_cpu1ClkSwitch = 1;
			clkSelect.uclkSelect_cpu1ClkSel = DIVIDED_BY_6;
			break;
		case 8:
			ClkSwitch.uClkSwitch_cpu1ClkSwitch = 1;
			clkSelect.uclkSelect_cpu1ClkSel = DIVIDED_BY_8;
			break;
		case 12:
			ClkSwitch.uClkSwitch_cpu1ClkSwitch = 1;
			clkSelect.uclkSelect_cpu1ClkSel = DIVIDED_BY_12;
			break;
		default:
			//dbg_printf(PRN_ERR, " this is impossible\n");
			printk("<PRN_ERR>this is impossible\n");
			break;
	}
#if 0
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

	// turn off divided-by-3 if not divided by 3
	if (divider != 3)
	{
		ClkSwitch.uClkSwitch_cpu1ClkD3Switch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
	}
#endif

	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
	if (divider != 3)
	{
		ClkSwitch.uClkSwitch_cpu1ClkD3Switch = 0;
	}

	// change Pll Select
	clkSelect.uclkSelect_cpu1ClkPllSel = pllSelect;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);

	// change Pll Switch
	ClkSwitch.uClkSwitch_cpu1ClkPllSwitch = pllSwitch;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
#ifdef ALWAYS_BYPASS
	// bypass OFF
	ClkSwitch.uClkSwitch_cpu1PLLSWBypass = 0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
#endif
}

void diag_clock_change_otherClk(unsigned int index, unsigned int pllSwitch, unsigned int pllSelect, unsigned int divider)
{
	TGbl_clkSelect		clkSelect;
	TGbl_ClkSwitch		ClkSwitch;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &clkSelect.u32[2]);
	
#ifdef ALWAYS_BYPASS
	// sysPll bypass ON
	ClkSwitch.uClkSwitch_sysPLLSWBypass = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
#endif
	if (index == CPU0CLK)
	{
#if 0
		// cpu0Clk use default sysPll
		ClkSwitch.uClkSwitch_cpu0ClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_cpu0ClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
#endif
		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_cpu0ClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_cpu0ClkSwitch = 1;
				clkSelect.uclkSelect_cpu0ClkSel = DIVIDED_BY_2;
				break;
			case 3:
				ClkSwitch.uClkSwitch_cpu0ClkD3Switch = 1;
				break;
			case 4:
				ClkSwitch.uClkSwitch_cpu0ClkSwitch = 1;
				clkSelect.uclkSelect_cpu0ClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_cpu0ClkSwitch = 1;
				clkSelect.uclkSelect_cpu0ClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_cpu0ClkSwitch = 1;
				clkSelect.uclkSelect_cpu0ClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_cpu0ClkSwitch = 1;
				clkSelect.uclkSelect_cpu0ClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
#if 0
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_cpu0ClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}
#endif

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_cpu0ClkD3Switch = 0;
		}

		// change Pll Select
		clkSelect.uclkSelect_cpu0ClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_cpu0ClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == SYSCLK)
	{
		// sysClk use default sysPll
		ClkSwitch.uClkSwitch_sysClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_sysClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_sysClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_sysClkSwitch = 1;
				clkSelect.uclkSelect_sysClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_sysClkSwitch = 1;
				clkSelect.uclkSelect_sysClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_sysClkSwitch = 1;
				clkSelect.uclkSelect_sysClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_sysClkSwitch = 1;
				clkSelect.uclkSelect_sysClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_sysClkSwitch = 1;
				clkSelect.uclkSelect_sysClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_sysClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_sysClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_sysClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	} 
	else if (index == DRMCLK)
	{
		// drmFigoClk use default sysPll
		ClkSwitch.uClkSwitch_drmFigoClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_drmFigoClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 1;
				clkSelect.uclkSelect_drmFigoClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 1;
				clkSelect.uclkSelect_drmFigoClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 1;
				clkSelect.uclkSelect_drmFigoClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 1;
				clkSelect.uclkSelect_drmFigoClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 1;
				clkSelect.uclkSelect_drmFigoClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_drmFigoClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_drmFigoClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_drmFigoClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == CFGCLK)
	{
		// cfgClk use default sysPll
		ClkSwitch.uClkSwitch_cfgClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_cfgClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 1;
				clkSelect.uclkSelect_cfgClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 1;
				clkSelect.uclkSelect_cfgClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 1;
				clkSelect.uclkSelect_cfgClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 1;
				clkSelect.uclkSelect_cfgClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 1;
				clkSelect.uclkSelect_cfgClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_cfgClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_cfgClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_cfgClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == GFXCLK)
	{
		// gfxClk use default sysPll
		ClkSwitch.uClkSwitch_gfxClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_gfxClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 1;
				clkSelect.uclkSelect_gfxClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 1;
				clkSelect.uclkSelect_gfxClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 1;
				clkSelect.uclkSelect_gfxClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 1;
				clkSelect.uclkSelect_gfxClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 1;
				clkSelect.uclkSelect_gfxClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_gfxClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_gfxClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_gfxClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == ZSPCLK)
	{
		// zspClk use default sysPll
		ClkSwitch.uClkSwitch_zspClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_zspClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_zspClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_zspClkSwitch = 1;
				clkSelect.uclkSelect_zspClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_zspClkSwitch = 1;
				clkSelect.uclkSelect_zspClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_zspClkSwitch = 1;
				clkSelect.uclkSelect_zspClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_zspClkSwitch = 1;
				clkSelect.uclkSelect_zspClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_zspClkSwitch = 1;
				clkSelect.uclkSelect_zspClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_zspClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_zspClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_zspClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == PERIFCLK)
	{
		// perifClk use default sysPll
		ClkSwitch.uClkSwitch_perifClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_perifClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_perifClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_perifClkSwitch = 1;
				clkSelect.uclkSelect_perifClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_perifClkSwitch = 1;
				clkSelect.uclkSelect_perifClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_perifClkSwitch = 1;
				clkSelect.uclkSelect_perifClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_perifClkSwitch = 1;
				clkSelect.uclkSelect_perifClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_perifClkSwitch = 1;
				clkSelect.uclkSelect_perifClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_perifClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_perifClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_perifClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == PCUBECLK)
	{
		// pCubeClk use default sysPll
		ClkSwitch.uClkSwitch_pCubeClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_pCubeClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 1;
				clkSelect.uclkSelect_pCubeClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 1;
				clkSelect.uclkSelect_pCubeClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 1;
				clkSelect.uclkSelect_pCubeClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 1;
				clkSelect.uclkSelect_pCubeClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 1;
				clkSelect.uclkSelect_pCubeClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_pCubeClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_pCubeClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_pCubeClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == VSCOPECLK)
	{
		// vScopeClk use default sysPll
		ClkSwitch.uClkSwitch_vScopeClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_vScopeClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 1;
				clkSelect.uclkSelect_vScopeClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 1;
				clkSelect.uclkSelect_vScopeClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 1;
				clkSelect.uclkSelect_vScopeClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 1;
				clkSelect.uclkSelect_vScopeClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 1;
				clkSelect.uclkSelect_vScopeClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_vScopeClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_vScopeClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_vScopeClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == NFCECCCLK)
	{
		// nfcEccClk use default sysPll
		ClkSwitch.uClkSwitch_nfcEccClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_nfcEccClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 1;
				clkSelect.uclkSelect_nfcEccClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 1;
				clkSelect.uclkSelect_nfcEccClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 1;
				clkSelect.uclkSelect_nfcEccClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 1;
				clkSelect.uclkSelect_nfcEccClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 1;
				clkSelect.uclkSelect_nfcEccClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_nfcEccClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_nfcEccClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_nfcEccClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == VPPSYSCLK)
	{
		// vppSysClk use default sysPll
		ClkSwitch.uClkSwitch_vppSysClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_vppSysClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 1;
				clkSelect.uclkSelect_vppSysClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 1;
				clkSelect.uclkSelect_vppSysClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 1;
				clkSelect.uclkSelect_vppSysClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 1;
				clkSelect.uclkSelect_vppSysClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 1;
				clkSelect.uclkSelect_vppSysClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_vppSysClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_vppSysClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_vppSysClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == APPCLK)
	{
		// appClk use default sysPll
		ClkSwitch.uClkSwitch_appClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_appClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_appClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_appClkSwitch = 1;
				clkSelect.uclkSelect_appClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_appClkSwitch = 1;
				clkSelect.uclkSelect_appClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_appClkSwitch = 1;
				clkSelect.uclkSelect_appClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_appClkSwitch = 1;
				clkSelect.uclkSelect_appClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_appClkSwitch = 1;
				clkSelect.uclkSelect_appClkSel = DIVIDED_BY_12;
				break;
			default:
				//dbg_printf(PRN_ERR, " this is impossible\n");
				printk("<PRN_ERR>this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_appClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_appClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_appClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else
		//dbg_printf(PRN_ERR, " this is impossible\n");
		printk("<PRN_ERR>this is impossible\n");
#ifdef ALWAYS_BYPASS
	// bypass OFF
	ClkSwitch.uClkSwitch_sysPLLSWBypass = 0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
#endif
}

#if 0
int diag_clock_select_profile(unsigned int profile)
{
	int iResult = 0;
	switch (profile)
	{
		case DEFAULT_400_400_400_200:
//			diag_clock_change_cpu1Pll(400);
			diag_clock_change_sysPll(400);
			diag_clock_change_memPll(400);
			diag_clock_change_cpu1Clk(0, 0, 1);
			diag_clock_change_otherClk(CPU0CLK, 0, 0, 1);
			diag_clock_change_otherClk(SYSCLK, 0, 0, 2);
			diag_clock_change_otherClk(DRMCLK, 0, 0, 2);
			diag_clock_change_otherClk(CFGCLK, 0, 0, 8);
			diag_clock_change_otherClk(GFXCLK, 0, 0, 2);
			diag_clock_change_otherClk(ZSPCLK, 0, 0, 2);
			diag_clock_change_otherClk(PERIFCLK, 0, 0, 4);
			diag_clock_change_otherClk(PCUBECLK, 0, 0, 1);
			diag_clock_change_otherClk(VSCOPECLK, 0, 0, 1);
			diag_clock_change_otherClk(NFCECCCLK, 0, 0, 2);
			diag_clock_change_otherClk(VPPSYSCLK, 0, 0, 2);
			diag_clock_change_otherClk(APPCLK, 0, 0, 2);
			break;
		case ONE_900_600_600_350:
//			diag_clock_change_memPll(700);
			diag_clock_change_cpu1Pll(900);
			diag_clock_change_sysPll(600);
			diag_clock_change_cpu1Clk(0, 0, 1);
			diag_clock_change_otherClk(CPU0CLK, 0, 0, 1);
			diag_clock_change_otherClk(SYSCLK, 0, 0, 2);
			diag_clock_change_otherClk(DRMCLK, 0, 0, 2);
			diag_clock_change_otherClk(CFGCLK, 0, 0, 8);
			diag_clock_change_otherClk(GFXCLK, 0, 0, 2);
			diag_clock_change_otherClk(ZSPCLK, 0, 0, 2);
			diag_clock_change_otherClk(PERIFCLK, 0, 0, 4);
			diag_clock_change_otherClk(PCUBECLK, 0, 0, 1);
			diag_clock_change_otherClk(VSCOPECLK, 0, 0, 1);
			diag_clock_change_otherClk(NFCECCCLK, 0, 0, 2);
			diag_clock_change_otherClk(VPPSYSCLK, 0, 0, 2);
			diag_clock_change_otherClk(APPCLK, 0, 0, 2);
			break;
		case TWO_900_900_600_400:
//			diag_clock_change_memPll(800);
			diag_clock_change_cpu1Pll(900);
			diag_clock_change_sysPll(600);
			diag_clock_change_cpu1Clk(0, 0, 1);
			diag_clock_change_otherClk(CPU0CLK, 1, 4, 1);
			diag_clock_change_otherClk(SYSCLK, 0, 0, 2);
			diag_clock_change_otherClk(DRMCLK, 0, 0, 2);
			diag_clock_change_otherClk(CFGCLK, 0, 0, 8);
			diag_clock_change_otherClk(GFXCLK, 0, 0, 2);
			diag_clock_change_otherClk(ZSPCLK, 0, 0, 2);
			diag_clock_change_otherClk(PERIFCLK, 0, 0, 4);
			diag_clock_change_otherClk(PCUBECLK, 0, 0, 1);
			diag_clock_change_otherClk(VSCOPECLK, 0, 0, 1);
			diag_clock_change_otherClk(NFCECCCLK, 0, 0, 2);
			diag_clock_change_otherClk(VPPSYSCLK, 0, 0, 2);
			diag_clock_change_otherClk(APPCLK, 0, 0, 2);
			break;
		default:
			iResult = 1;
			dbg_printf(PRN_ERR, " this is impossible\n");
	}
	return iResult;
}
#endif


//-------------------------------------freq.c

static int speed_cpu[2];

static unsigned int clock_divider[] =
{
	1,
	2,
	4,
	6,
	8,
	12,
	1,
	1
};


static unsigned int get_divider(unsigned int D3Switch, unsigned int Switch, unsigned int Select)
{
	unsigned int divider;
	if (D3Switch)
		divider = 3;
	else
	{
		if (!Switch)
			divider = 1;
		else
			divider = clock_divider[Select];
	}
	return divider;
}

#define MAKE_DEFAULT_CLK_600
#define	HW_show_speed	printk

void print_clock_info(void)
{
    unsigned int FbDiv, RfDiv, VcoDivSel;
    unsigned int memPll;
    unsigned int sysPll;
    unsigned int cpu1Pll;
    unsigned int divider;
    unsigned int D3Switch, Switch, Select;
    int nfccswitch;
    T32Gbl_ClkSwitch    ClkSwitch;
    T32Gbl_ClkSwitch1   ClkSwitch1;
    T32Gbl_clkSelect    clkSelect;
    T32Gbl_clkSelect1   clkSelect1;
    T32Gbl_clkSelect2   clkSelect2;
    unsigned int PllSel;
	SOC_FREQ_VECTOR_Linux freq ;

    HW_show_speed(("\n -------- Clock setting --------\n"));
#if 1
    {
        // sysPll
        T32Gbl_sysPllCtl    sysPllCtl;
        T32Gbl_sysPllCtl1   sysPllCtl1;
        GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl), &sysPllCtl.u32);
        FbDiv = sysPllCtl.usysPllCtl_sysPllFbDiv;
        RfDiv = sysPllCtl.usysPllCtl_sysPllRfDiv;
        GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl1), &sysPllCtl1.u32);
        VcoDivSel = sysPllCtl1.usysPllCtl_sysPllVcoDivSel;
        sysPll = (FbDiv + 2) * 25 / (RfDiv + 2) / (1 << VcoDivSel);
        freq.syspll = sysPll*1000000;
        printk(" sysPLL       frequency %ld\n", freq.syspll);
    }
#endif

    {
        // cpu1Pll
        T32Gbl_cpu1PllCtl   cpu1PllCtl;
        T32Gbl_cpu1PllCtl1  cpu1PllCtl1;
        GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpu1PllCtl), &cpu1PllCtl.u32);
        FbDiv = cpu1PllCtl.ucpu1PllCtl_cpu1PllFbDiv;
        RfDiv = cpu1PllCtl.ucpu1PllCtl_cpu1PllRfDiv;
        if (RfDiv & 0xF)    // (M=REFDIV[3:0]+2, when REFDIV[3:0] not zero.)
            RfDiv = (RfDiv & 0xF) + 2;
        else                // (M=REFDIV[4]+1, when REFDIV[3:0] = zero.)
            RfDiv = (RfDiv >> 4) + 1;
        GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpu1PllCtl1), &cpu1PllCtl1.u32);
        if (cpu1PllCtl1.ucpu1PllCtl_cpu1PllDiffClkEn)       // Differential clock output
            VcoDivSel = cpu1PllCtl1.ucpu1PllCtl_cpu1PllVcoDivSelDiff;
        else                                                // Single-ended clock output
            VcoDivSel = cpu1PllCtl1.ucpu1PllCtl_cpu1PllVcoDivSelSe;
	
        cpu1Pll = (FbDiv) * 25 / RfDiv;
#if 0
        cpu1Pll = (float)cpu1Pll / VcoDiv[VcoDivSel];
        HW_show_speed((" cpu1PLL    frequency %d\n", cpu1Pll));
#endif
    }


    GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
    GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch1.u32);
    GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32);
    GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
    GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &clkSelect2.u32);


    // CPU1
    D3Switch = ClkSwitch.uClkSwitch_cpu1ClkD3Switch;
    Switch = ClkSwitch.uClkSwitch_cpu1ClkSwitch;
    Select = clkSelect.uclkSelect_cpu1ClkSel;
    divider = get_divider(D3Switch, Switch, Select);
    PllSel = clkSelect.uclkSelect_cpu1ClkPllSel;

    freq.cpu1 = cpu1Pll*1000000/divider;
    if (ClkSwitch.uClkSwitch_cpu1ClkPllSwitch) {        // AVPLL
        printk((" cpu1Clk    from %s\n", AVPLL[PllSel]));
    } else {
        printk(" cpu1Clk     frequency %ld\n", freq.cpu1);
    }

    // CPU0
    D3Switch = ClkSwitch.uClkSwitch_cpu0ClkD3Switch;
    Switch = ClkSwitch.uClkSwitch_cpu0ClkSwitch;
    Select = clkSelect.uclkSelect_cpu0ClkSel;
    divider = get_divider(D3Switch, Switch, Select);
    PllSel = clkSelect.uclkSelect_cpu0ClkPllSel;
    if (ClkSwitch.uClkSwitch_cpu0ClkPllSwitch) {        // AVPLL or cpu1Pll
        if (PllSel == 4) {
            freq.cpu0 = cpu1Pll*1000000/divider;
            HW_show_speed(" cpu0Clk    frequency %ld (use cpu1Pll)\n", freq.cpu0);
        } else if (clkSelect.uclkSelect_cpu0ClkPllSel < 4) {
            HW_show_speed(" cpu0Clk    from %s\n", AVPLL[PllSel]);
#ifdef MAKE_DEFAULT_CLK_600
            freq.cpu0=600000000;
#endif
        } else {
            HW_show_speed(" cpu0Clk    invalid PllSel %d\n", PllSel);
#ifdef MAKE_DEFAULT_CLK_600
            freq.cpu0=600000000;
#endif
        }
    } else {    // sysPll
        freq.cpu0 = sysPll/divider ;
        HW_show_speed(" cpu0Clk    frequency %ld (use sysPll)\n", freq.cpu0);
    }

}

