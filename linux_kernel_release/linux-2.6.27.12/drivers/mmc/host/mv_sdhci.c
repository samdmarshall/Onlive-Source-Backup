/*
 * 
 *  Copyright (C) 2008 hzchen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/bitops.h>
#include <linux/leds.h>
#include <linux/list.h>
#include <linux/mmc/host.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <mach/galois_generic.h>
#include "sdhci.h"

DEFINE_LED_TRIGGER(mv_nand_led_trigger);

#define MAX_SLOTS			1
struct sdhci_mv_chip;
struct sdhci_mv_slot;

struct sdhci_mv_fixes {
	unsigned int		quirks;
	int			(*probe)(struct sdhci_mv_chip*);
	int			(*probe_slot)(struct sdhci_mv_slot*);
	void			(*remove_slot)(struct sdhci_mv_slot*, int);
	int			(*suspend)(struct sdhci_mv_chip*,
					pm_message_t);
	int			(*resume)(struct sdhci_mv_chip*);
};
struct sdhci_mv_slot {
	struct sdhci_mv_chip	*chip;
	struct sdhci_host	*host;
};
struct sdhci_mv_chip {
	unsigned int		quirks;
	const struct sdhci_mv_fixes *fixes;
	int			num_slots;	/* Slots on controller */
	struct sdhci_mv_slot	*slots[MAX_SLOTS]; /* Pointers to host slots */
};
static void mv_enable_mmc(struct sdhci_host *host, int on)
{
	u8 scratch;
	scratch = readb(host->ioaddr + 0xC0);
	if (on)
		scratch |= 0x01;
	else
		scratch &= ~0x01;
	writeb(scratch, host->ioaddr + 0xC0);
}

static int mv_probe_slot(struct sdhci_mv_slot *slot)
{
	//if (slot->chip->pdev->revision == 0) {
		u16 version;

		version = readl(slot->host->ioaddr + SDHCI_HOST_VERSION);
		version = (version & SDHCI_VENDOR_VER_MASK) >>
			SDHCI_VENDOR_VER_SHIFT;

		/*
		 * Older versions of the chip have lots of nasty glitches
		 * in the ADMA engine. It's best just to avoid it
		 * completely.
		 */
		if (version < 0xAC)
			slot->host->quirks |= SDHCI_QUIRK_BROKEN_ADMA;
	//}

	/*
	 * The secondary interface requires a bit set to get the
	 * interrupts.
	 */
	//if (slot->chip->pdev->device == PCI_DEVICE_ID_JMICRON_JMB38X_MMC)
		mv_enable_mmc(slot->host, 1);

	return 0;
}

static void sdhci_mv_remove_slot(struct sdhci_mv_slot *slot)
{
	int dead;
	u32 scratch;
	dead = 0;
	scratch = readl(slot->host->ioaddr + SDHCI_INT_STATUS);
	if (scratch == (u32)-1)
		dead = 1;
	sdhci_remove_host(slot->host, dead);
	if (slot->chip->fixes && slot->chip->fixes->remove_slot)
		slot->chip->fixes->remove_slot(slot, dead);
	sdhci_free_host(slot->host);
}
static int sdhci_mvfix_probe(struct sdhci_mv_chip *chip)
{
	return 0;
}

static const struct sdhci_mv_fixes sdhci_fixes = {
	.probe		= sdhci_mvfix_probe,
	.quirks     = //SDHCI_QUIRK_32BIT_DMA_ADDR | 
		          SDHCI_QUIRK_BROKEN_ADMA |
		          //SDHCI_QUIRK_32BIT_ADMA_SIZE |
				  SDHCI_QUIRK_BROKEN_TIMEOUT_VAL,
};


static struct sdhci_ops sdhci_mv_ops = {
};
static struct sdhci_mv_slot * __init sdhci_mv_probe_slot(
	struct platform_device *pdev, struct sdhci_mv_chip *chip, int bar)
{
	struct sdhci_mv_slot *slot;
	struct sdhci_host *host;
	struct resource *res;
	int ret = 0;;
	res = platform_get_resource_byname(pdev, IORESOURCE_IRQ, "sdio_irq");
	if (!res) {
		printk("There's no irq resource for platform device.\n");
		return ERR_PTR(-ENODEV);
	}
	host = sdhci_alloc_host(&pdev->dev, sizeof(struct sdhci_mv_slot));
	if (IS_ERR(host)) {
		return (void *)host;
	}
	slot = sdhci_priv(host);
	slot->chip = chip;
	slot->host = host;
	//slot->pci_bar = bar;
	host->hw_name = "MVSD";
	host->ops = &sdhci_mv_ops;
	host->quirks = chip->quirks;
	host->irq = res->start;
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "sdio_mem");
	if (!res) {
		printk("There's no mem resource for platform device.\n");
		goto release;
	}
	host->ioaddr = (void __iomem *)res->start;
	
	if (chip->fixes && chip->fixes->probe_slot) {
		ret = chip->fixes->probe_slot(slot);
		if (ret)
			goto release;
	}
    
	ret = sdhci_add_host(host);
	if (ret)
		goto remove;
	
	mv88de3010_open_irq(host->irq);
	return slot;
remove:
	if (chip->fixes && chip->fixes->remove_slot)
		chip->fixes->remove_slot(slot, 0);
release:
	sdhci_free_host(host);
	return ERR_PTR(ret);
}

/*
 * Probe for the SDHCI device.
 */
static int __init sdhci_mv_probe(struct platform_device *pdev)
{
	struct sdhci_mv_chip *chip;
	struct sdhci_mv_slot *slot;
	u8 slots = 1,first_bar = 0;
	int ret = 0, i;

	/* Allocate memory for the device structure (and zero it) */
	mv_sdio_setup_pinmux();
	chip = kzalloc(sizeof(struct sdhci_mv_chip), GFP_KERNEL);
	if (!chip) {
		ret = -ENOMEM;
		goto err;
	}
	
	chip->fixes = &sdhci_fixes;
	if (chip->fixes)
	    chip->quirks = chip->fixes->quirks;
	//chip->num_slots = slots;
	
	platform_set_drvdata(pdev, chip);
	if (chip->fixes && chip->fixes->probe) {
		ret = chip->fixes->probe(chip);
		if (ret)
			goto free;
	}

	for (i = 0;i < slots;i++) {
		slot = sdhci_mv_probe_slot(pdev, chip, first_bar + i);
		if (IS_ERR(slot)) {
			for (i--;i >= 0;i--)
				sdhci_mv_remove_slot(chip->slots[i]);
			ret = PTR_ERR(slot);
			goto free;
		}

		chip->slots[i] = slot;
	}
	chip->num_slots = slots;
	return 0;
free:
	platform_set_drvdata(pdev, NULL);
	kfree(chip);
err:
	return ret;

}

static int __devexit sdhci_mv_remove(struct platform_device *pdev)
{
	int i;
	struct sdhci_mv_chip *chip;
	chip = platform_get_drvdata(pdev);
	if (chip) {
		for (i = 0;i < chip->num_slots; i++)
			sdhci_mv_remove_slot(chip->slots[i]);
		platform_set_drvdata(pdev, NULL);
		kfree(chip);
	}
	return 0;
}
#ifdef CONFIG_PM

static int __devexit sdhci_mv_suspend( struct platform_device *pdev, pm_message_t state)
{
	struct sdhci_mv_chip *chip;
	struct sdhci_mv_slot *slot;
	int i,ret;
	chip = platform_get_drvdata(pdev);
	//printk("sdhci_mv_suspend chip = %x\n",chip);
	if (chip) {
		for (i = 0;i < chip->num_slots; i++) {
			slot = chip->slots[i];
			if (!slot)
				continue;
			ret = sdhci_suspend_host(slot->host, state);
			if (ret) {
				for (i--;i >= 0;i--)
					sdhci_resume_host(chip->slots[i]->host);
				return ret;
			}
		}

				
	}
	/*TBD Power manager left do we need to do something related to our SDHC controller for suspend? */
	return 0;
}

static int __devexit sdhci_mv_resume( struct platform_device *pdev)
{
	struct sdhci_mv_chip *chip;
	struct sdhci_mv_slot *slot;
	int ret, i;
	//printk("sdhci_mv_resume \n");
	mv_sdio_setup_pinmux();
	chip = platform_get_drvdata(pdev);
	//printk("sdhci_mv_resume chip = 0x%x\n",chip);
	if (!chip)
		return 0;
	/* TBD do we need to do something related to our SDHC controller for resume */
	for (i = 0;i < chip->num_slots;i++) {
		slot = chip->slots[i];
		if (!slot)
			continue;
		//printk("call sdhci_resume_host slot->host->irq = 0x%x\n",slot->host->irq );
		ret = sdhci_resume_host(slot->host);
		if (ret)
			return ret;
	}
	//printk(" leave sdhci_mv_resume \n");
	return 0;
}

#else

#define sdhci_mv_suspend NULL
#define sdhci_mv_resume NULL

#endif

static struct platform_driver mv_sdhci_driver = {
	.probe		= sdhci_mv_probe,
	.remove		= sdhci_mv_remove,
	.driver		= {
	    .name	= "mv_sdhci",
	    .owner	= THIS_MODULE,
	},

	.suspend        = sdhci_mv_suspend,
	.resume	        = sdhci_mv_resume,
};

static int __init mv_sdhci_init(void)
{   
	led_trigger_register_simple("mv-sdhci", &mv_sdhci_led_trigger);
	return platform_driver_register(&mv_sdhci_driver);
}


static void __exit mv_sdhci_exit(void)
{
	led_trigger_unregister_simple(mv_sdhci_led_trigger);
	platform_driver_unregister(&mv_sdhci_driver);
}


module_init(mv_sdhci_init);
module_exit(mv_sdhci_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hz Chen");
MODULE_DESCRIPTION("SDHCI Controller driver for Marvell");
