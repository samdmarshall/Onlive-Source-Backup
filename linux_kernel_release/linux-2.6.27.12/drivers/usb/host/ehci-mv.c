/*
 * EHCI HCD (Host Controller Driver) for USB.
 *
 * (C) Copyright 2008 hzchen 
 *
 * Bus Glue for Chipidea
 *
 * Based on "ehci-au1xxx.c" by hzchen 
 * Modified for Marvell EHC
 * This file is licenced under the GPL.
 */

#include <linux/platform_device.h>
#include <linux/version.h>
#include <mach/galois_generic.h>
#include <mach/gpio.h>

//#undef CONFIG_PM

/* PCI-based HCs are common, but plenty of non-PCI HCs are used too */

/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */


/* ChipIdea relevant */

#define MV_USB_CHIP_ID_OFFSET   0
#define MV_USB_CHIP_ID_MASK     0x3F
#define MV_USB_CHIP_NID_MASK    0x3F00
#define MV_USB_CHIP_REVERSION_MASK 0xFF0000
#define MV_USB_CHIP_USB20       0x05

#define MV_USB_CHIP_CAP_OFFSET (0x100)
#define MV_USB_HCS_PORT_POWER_CONTROL (0x10)
#define MV_HOST_INTR_EN_BITS  	(0x37)
#define MV_PORTSCX_W1C_BITS		(0x2A)
#define MV_PORTSCX_PORT_POWER   (0x00001000)
#define MV_HCC_PGM_FRM_FLAG 	(0x02)
#define MV_GPIO_POWER0 			(12)
#define MV_GPIO_POWER1 			(14)

#define USB_MCFG_SSDEN    (1<<23)
#define USB_MCFG_PHYPLLEN (1<<19)
#define USB_MCFG_EHCCLKEN (1<<17)
#define USB_MCFG_UCAM     (1<<7)
#define USB_MCFG_EBMEN    (1<<3)
#define USB_MCFG_EMEMEN   (1<<2)


extern int usb_disabled(void);

static void usb_ehci_mv_stop(struct platform_device *dev)
{
	printk(__FILE__ ": stopping Marvell EHCI USB Controller\n");
}

/* This is hardware specific init function, do following 
 * halt ==> init ==> reset ==> power_off 
 * please notice, ehci_init constructs the internal data structures,
 * so  needs to be invoked once before any operations, but once only.
 * Any hardware specific stuff can be added here.
 * 			He */
static int ehci_mv_setup(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval;
	//u32 temp;
	
	retval = ehci_halt(ehci);
	if (retval)
		return retval;
	/*
	 * data structure init
	 */
	retval = ehci_init(hcd);
	if (retval)
		return retval;
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
	ehci->is_tdi_rh_tt = 1;
#else 
	hcd->has_tt = 1;
#endif
	ehci->sbrn = 0x20;

	ehci_reset(ehci);

	ehci_port_power(ehci, 0);

#ifndef CONFIG_MV88DE3010_USB_NOGPIO
	mv_usb_setup_pinmux();
	GPIO_PortSetInOut(MV_GPIO_POWER0, 0); /* 0:output mode */
	GPIO_PortWrite(MV_GPIO_POWER0, 1);
	GPIO_PortSetInOut(MV_GPIO_POWER1, 0); /* 0:output mode */
	GPIO_PortWrite(MV_GPIO_POWER1, 1);
#endif

#ifdef CONFIG_MV88DE3010_EHSET
	*(volatile unsigned int *)(0xF7ED0808) = 0x41C04FC3;
	*(volatile unsigned int *)(0xF7ED0810) = 0x0000097E;
	*(volatile unsigned int *)(0xF7EE0808) = 0x41C04FC3;
	*(volatile unsigned int *)(0xF7EE0810) = 0x0000097E;
#endif
	return retval;
}

static int ehci_mv_start( struct usb_hcd *hcd)
{
	int retval;
	struct ehci_hcd  *ehci = hcd_to_ehci(hcd);

	retval = ehci_run(hcd);

	ehci_writel(ehci, 0x07, hcd->regs + 0x090);
	ehci_writel(ehci, 0x13, hcd->regs + 0x1a8);

	return retval;

}

int usb_ehci_mv_probe(const struct hc_driver *driver,
			  struct usb_hcd **hcd_out, struct platform_device *pdev)
{
	int retval;
	struct usb_hcd *hcd;
	struct ehci_hcd *ehci;
	int tmp;
		
	/* get chip ID register*/
	tmp = readl(pdev->resource[0].start+MV_USB_CHIP_ID_OFFSET);
  	if ((tmp&MV_USB_CHIP_ID_MASK)!=MV_USB_CHIP_USB20){
    	dev_dbg(&pdev->dev, "chip version is not USB2.0\n");
    	return -ENODEV;
		goto err1;
 	}

	if (pdev->resource[1].flags != IORESOURCE_IRQ) {
		dev_dbg(&pdev->dev,"resource[1] is not IORESOURCE_IRQ\n");
		retval = -ENOMEM;
		goto err1;
	}

	hcd = usb_create_hcd(driver, &pdev->dev, pdev->dev.bus_id);
	if (!hcd) {
		return -ENOMEM;
	    goto err1;
	}

	hcd->rsrc_start = pdev->resource[0].start;
	hcd->rsrc_len = pdev->resource[0].end - pdev->resource[0].start + 1;
   
	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		dev_dbg(&pdev->dev, "request_mem_region failed\n");
		retval = -EBUSY;
		goto err2;
	}	
	
	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);

	if (!hcd->regs) {
		dev_dbg(&pdev->dev, "ioremap failed \n");
		retval = -EFAULT;
		goto err3;
	}
	
	ehci = hcd_to_ehci(hcd);
	ehci->caps = hcd->regs + MV_USB_CHIP_CAP_OFFSET;
	ehci->regs = hcd->regs + MV_USB_CHIP_CAP_OFFSET + HC_LENGTH(readl(&ehci->caps->hc_capbase));

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = readl(&ehci->caps->hcs_params);

	retval =
	    usb_add_hcd(hcd, pdev->resource[1].start, IRQF_DISABLED | IRQF_SHARED);
	
	if (retval == 0) {
		printk("%s : usb_add_hcd successful\n", __FUNCTION__);
		return retval;
	} 

	usb_ehci_mv_stop(pdev);
	iounmap(hcd->regs);

err3:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
err2:
	usb_put_hcd(hcd);
err1:
	dev_err(&pdev->dev, "init %s fail, %d\n", 
			pdev->name, retval);

	return retval;
}

/**
 * usb_ehci_hcd_mv_remove - shutdown processing for ci13511-based HCDs
 * @dev: USB Host Controller being removed
 * Context: !in_interrupt()
 *
 * Reverses the effect of usb_ehci_hcd_mv_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, normally "rmmod", "apmd", or something similar.
 *
 */
void usb_ehci_mv_remove(struct usb_hcd *hcd, struct platform_device *pdev)
{
#ifndef CONFIG_MV88DE3010_USB_NOGPIO
	mv_usb_setup_pinmux();
	GPIO_PortSetInOut(MV_GPIO_POWER0, 0); /* 0:output mode */
	GPIO_PortWrite(MV_GPIO_POWER0, 0);
	GPIO_PortSetInOut(MV_GPIO_POWER1, 0); /* 0:output mode */
	GPIO_PortWrite(MV_GPIO_POWER1, 0);
#endif

	usb_remove_hcd(hcd);
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);
	usb_ehci_mv_stop(pdev);
}

/*-------------------------------------------------------------------------*/

static const struct hc_driver ehci_mv_hc_driver = {
	.description = hcd_name,
	.product_desc = "Marvell On-Chip EHCI Host Controller",
	.hcd_priv_size = sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq = ehci_irq,
	.flags = HCD_MEMORY | HCD_USB2,

	/*
	 * basic lifecycle operations
	 */
	.reset = ehci_mv_setup,
	.start = ehci_mv_start,
	.stop = ehci_stop,
	.shutdown = ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue = ehci_urb_enqueue,
	.urb_dequeue = ehci_urb_dequeue,
	.endpoint_disable = ehci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number = ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data = ehci_hub_status_data,
	.hub_control = ehci_hub_control,
	.bus_suspend = ehci_bus_suspend,
	.bus_resume = ehci_bus_resume,
	.relinquish_port = ehci_relinquish_port,
	.port_handed_over = ehci_port_handed_over,
};

/*-------------------------------------------------------------------------*/

static int ehci_hcd_mv_drv_probe(struct platform_device *pdev)
{
	struct usb_hcd *hcd = NULL;
	int ret;

	if (usb_disabled()){
		return -ENODEV;  
	}
   
	ret = usb_ehci_mv_probe(&ehci_mv_hc_driver, &hcd, pdev);

	return ret;
}

static int ehci_hcd_mv_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	usb_ehci_mv_remove(hcd, pdev);

	return 0;
}

#ifdef CONFIG_PM
static int ehci_hcd_mv_drv_suspend(struct platform_device *pdev, 
				   pm_message_t message)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	unsigned long flags;
	int rc = 0;

	return 0;

	if (time_before(jiffies, ehci->next_statechange))
		msleep(10);

	/* Root hub was already suspended. Disable irq emission and
	 * mark HW unaccessible, bail out if RH has been resumed. Use
	 * the spinlock to properly synchronize with possible pending
	 * RH suspend or resume activity.
	 *
	 * This is still racy as hcd->state is manipulated outside of
	 * any locks =P But that will be a different fix.
	 */
	spin_lock_irqsave(&ehci->lock, flags);
	if (hcd->state != HC_STATE_SUSPENDED) {
		rc = -EINVAL;
		goto bail;
	}
	
	ehci_writel(ehci, 0, &ehci->regs->intr_enable);
	(void)ehci_readl(ehci, &ehci->regs->intr_enable);

	/* make sure snapshot being resumed re-enumerates everything */
	if (message.event == PM_EVENT_PRETHAW) {
		ehci_halt(ehci);
		ehci_reset(ehci);
	}

	clear_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);

bail:
	spin_unlock_irqrestore(&ehci->lock, flags);

	// could save FLADJ in case of Vaux power loss
	// ... we'd only use it to handle clock skew

	return rc;
}

static int ehci_hcd_mv_drv_resume(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);

	mv88de3010_open_irq(IRQ_USB0);
	mv88de3010_open_irq(IRQ_USB1);

#ifndef CONFIG_MV88DE3010_USB_NOGPIO
	mv_usb_setup_pinmux();
	GPIO_PortSetInOut(MV_GPIO_POWER0, 0); /* 0:output mode */
	GPIO_PortWrite(MV_GPIO_POWER0, 1);
	GPIO_PortSetInOut(MV_GPIO_POWER1, 0); /* 0:output mode */
	GPIO_PortWrite(MV_GPIO_POWER1, 1);
#endif

	// maybe restore FLADJ

	if (time_before(jiffies, ehci->next_statechange))
		msleep(100);

	/* Mark hardware accessible again as we are out of D3 state by now */
	set_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);

	/* If CF is still set, we maintained PCI Vaux power.
	 * Just undo the effect of ehci_pci_suspend().
	 */
	if (ehci_readl(ehci, &ehci->regs->configured_flag) == FLAG_CF) {
		int	mask = INTR_MASK;

		if (!hcd->self.root_hub->do_remote_wakeup)
			mask &= ~STS_PCD;
		ehci_writel(ehci, mask, &ehci->regs->intr_enable);
		ehci_readl(ehci, &ehci->regs->intr_enable);
		return 0;
	}

	ehci_dbg(ehci, "lost power, restarting\n");
	usb_root_hub_lost_power(hcd->self.root_hub);

	/* Else reset, to cope with power loss or flush-to-storage
	 * style "resume" having let BIOS kick in during reboot.
	 */
	(void) ehci_halt(ehci);
	(void) ehci_reset(ehci);

	/* emptying the schedule aborts any urbs */
	spin_lock_irq(&ehci->lock);
	if (ehci->reclaim)
		end_unlink_async(ehci);
	ehci_work(ehci);
	spin_unlock_irq(&ehci->lock);

	ehci_writel(ehci, ehci->command, &ehci->regs->command);
	ehci_writel(ehci, FLAG_CF, &ehci->regs->configured_flag);
	ehci_readl(ehci, &ehci->regs->command);	/* unblock posted writes */

	/* here we "know" root ports should always stay powered */
	ehci_port_power(ehci, 1);

	hcd->state = HC_STATE_SUSPENDED;

	return 0;
}
#else
#define ehci_hcd_mv_drv_suspend NULL
#define ehci_hcd_mv_drv_resume NULL
#endif

MODULE_ALIAS("mv88de3010_ehci");

static struct platform_driver ehci_hcd_mv_driver = {
	.probe = ehci_hcd_mv_drv_probe,
	.remove = ehci_hcd_mv_drv_remove,
	.shutdown = usb_hcd_platform_shutdown,
	.suspend = ehci_hcd_mv_drv_suspend,
	.resume = ehci_hcd_mv_drv_resume,
	.driver = {
		.name = "mv88de3010_ehci",
	}
};
