#include <pkgconf/devs_eth_arm_galois.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_cache.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/hal/drv_api.h>

#include <cyg/io/eth/netdev.h>
#include <cyg/io/eth/eth_drv.h>

#ifdef CYGPKG_NET
#include <pkgconf/net.h>
#endif

#include <stdlib.h>
#include "eth.h"
#include "pic.h"
#include "galois_int.h"
#include "galois_int_register.h"
#include "galois_cache_flush.h"

/*
 * Diagnostic printf
 */
#define os_printf diag_printf
#define os_dbgprintf(...)

/*
 * Redboot config
 */
#ifdef CYGPKG_REDBOOT
#include <pkgconf/redboot.h>
#ifdef CYGSEM_REDBOOT_FLASH_CONFIG
#include <redboot.h>
#include <flash_config.h>
#ifndef CONFIG_ESA
#define CONFIG_ESA 6	/* ethernet address length */
#endif
#ifndef CONFIG_BOOL
#define CONFIG_BOOL 1
#endif
RedBoot_config_option("Network hardware address [MAC]",
                      galois_esa,
                      ALWAYS_ENABLED, true,
                      CONFIG_ESA, 0
    );
RedBoot_config_option("Attempt to find 100 Mbps Ethernet",
                      galois_100,
                      ALWAYS_ENABLED, true,
                      CONFIG_BOOL, 0
    );
#endif	/* CYGSEM_REDBOOT_FLASH_CONFIG */
#endif	/* CYGPKG_REDBOOT */

/*
 * Galois ethernet definitions
 */
#define GALOIS_ETH_CPU 			CYGNUM_DEVS_ETH_ARM_GALOIS_CPUINDEX
#define GALOIS_ETH_IRQ 			IRQ_ethIrq
unsigned char GALOIS_ETH_MACADDR[] = "001679850105";
#define GALOIS_MAC_ADDR_SIZE	6
#define GALOIS_ETH_MAX_RXSIZE	(1500+2+14+4+2+32) /* IP+MACheader+MACaddr+CRC+Pad+Cache */
#define GALOIS_ETH_DEFAULT_RXQ	0
#define GALOIS_ETH_TXQLEN		CYGNUM_DEVS_ETH_ARM_GALOIS_TXQLEN
//#define GALOIS_ETH_RXQLEN		CYGNUM_DEVS_ETH_ARM_GALOIS_RXQLEN
#define GALOIS_ETH_RXQLEN		128

/* rx-ready-q0, rx-error-q0, tx-done-q0, rx-overrun, tx-underrun */
#define GETH_TXQ_MASK		(ETH_CAUSE_TX_BUF_MASK(0))
#define GETH_TXQ_RES_MASK	(ETH_CAUSE_TX_UNDERRUN_ERROR | ETH_CAUSE_TX_ERROR_MASK(0))
#define GETH_RXQ_MASK		(ETH_CAUSE_RX_READY_MASK(0))
#define GETH_RXQ_RES_MASK	(ETH_CAUSE_RX_OVERRUN_ERROR | ETH_CAUSE_RX_ERROR_MASK(0))
#define GETH_PICR_MASK		(GETH_RXQ_MASK | GETH_RXQ_RES_MASK | \
							 GETH_TXQ_MASK | GETH_TXQ_RES_MASK)

#define DCACHE_ALIGN_ADDR(x)	(((x) & 31)? (((x)+32) & (~31)) : (x))

/*
 * Galois ethernet private data structure.
 */
typedef struct galois_eth_priv {
	ETH_PORT_CTRL port_ctrl;
	unsigned char *rxbuf;
	MV_BUF_INFO tx_buf_info[16];	/* This's my guess:-) */
	MV_PKT_INFO tx_pkt_info;
	unsigned char macaddr[GALOIS_MAC_ADDR_SIZE];
	unsigned int intr_mask;
	int rxq_count[MV_ETH_RX_Q_NUM];
	int start_status;
} galois_eth_priv_t;

galois_eth_priv_t galois_eth_info;

/*
 * the buffer filled into rx descriptor
 */
typedef struct eth_buffer {
	unsigned char *buf;
	unsigned char *aligned_buf;
	unsigned int size;
} eth_buffer_t;

/*
 * Galois ethernet driver misc functions
 */
static void galois_eth_isr(cyg_vector_t vector, cyg_addrword_t data);
static void galois_eth_dsr(cyg_vector_t vector, cyg_addrword_t data);
static bool galois_eth_init(struct cyg_netdevtab_entry *tab);

static void galois_eth_interrupt_acknowledge(void);
static int galois_eth_rxqfill(galois_eth_priv_t *priv, int queue, int queue_len);
static void galois_eth_rx_event(struct eth_drv_sc *sc);
static void galois_eth_tx_event(struct eth_drv_sc *sc);

/* user code */
static unsigned int char2hex(char ch);
static void str2macaddr(unsigned char *dest, unsigned char *source);

/* struct eth_hwr_funs members */
static void galois_eth_start(struct eth_drv_sc *sc, unsigned char *enaddr, int flags);
static void galois_eth_stop(struct eth_drv_sc *sc);
static int galois_eth_control(struct eth_drv_sc *sc, 
			unsigned long key, void *data, int length);
static int galois_eth_can_send(struct eth_drv_sc *sc);
static void galois_eth_send(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, 
			int sg_len, int total_len, unsigned long key);
static void galois_eth_recv(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, int sg_len);
static void galois_eth_deliver(struct eth_drv_sc * sc);
static void galois_eth_poll(struct eth_drv_sc *sc);
static int galois_eth_int_vector(struct eth_drv_sc *sc);

/*
 * struct eth_hwr_funs {
 * 		void (*start)(struct eth_drv_sc *sc, ...);
 * 		...
 * 		struct eth_drv_funs *eth_drv, *eth_drv_old;
 * }
 * struct eth_drv_sc {
 * 		struct eth_hwr_funs *funs;
 * 		void *driver_private;
 * 		const char *dev_name;
 * 		int state;
 * 		struct arpcom sc_arpcom;
 * }
 */
ETH_DRV_SC(galois_eth0_sc,		/* struct eth_drv_sc galois_eth0_sc */
           &galois_eth_info,
           "eth0",
           galois_eth_start,	/* struct eth_hwr_funs galois_eth0_sc_funs */
           galois_eth_stop,
           galois_eth_control,
           galois_eth_can_send,
           galois_eth_send,
           galois_eth_recv,
           galois_eth_deliver,
           galois_eth_poll,
           galois_eth_int_vector
);

/*
 * struct cyg_netdevtab_entry {
 * 		const char *name;
 * 		bool (*init)(struct cyg_netdevtab_entry *tab);
 * 		void *device_instance;
 * 		unsigned long status;
 * }
 */
NETDEVTAB_ENTRY(galois_netdev, 	/* struct cyg_netdevtab_entry galois_netdev */
                "eth0", 
                galois_eth_init,
                &galois_eth0_sc
);

/*
 * Galois ISR is register to global GALOIS_ISR
 */
#ifdef CYGPKG_NET
static void galois_eth_isr(cyg_vector_t vector, cyg_addrword_t data)
{
	galois_interrupt_mask(GALOIS_ETH_CPU, GALOIS_ETH_IRQ);
	galois_interrupt_vectors[vector].dsr_count++;
	os_dbgprintf("in isr (dsr count=%d).\n", galois_interrupt_vectors[vector].dsr_count);
	/* return (CYG_ISR_HANDLED | CYG_ISR_CALL_DSR);  Ethernet needs DSR */
}

/*
 * hw_dsr=>eth_drv_dsr=>deliver-thread=>hw_deliver=>hw_poll=>
 * eth_drv.recv=>hw_recv
 */
static void galois_eth_dsr(cyg_vector_t vector, cyg_addrword_t data)
{
	unsigned int count;
	/* 
	 * Galois specific ethernet DSR process, count = 0? FIXED
	 */
	os_dbgprintf("in dsr.\n");
	count = galois_interrupt_vectors[vector].dsr_count;
	galois_interrupt_vectors[vector].dsr_count = DSR_COUNT_ZERO;
	eth_drv_dsr(vector, count, data);
}

static void galois_eth_interrupt_acknowledge(void)
{
	GA_REG_WORD32_WRITE(ETH_EICR, 0);
	/* TODO: ack the ethernet controller */
}
#endif

/*
 * Called when system startup (exactly in ecos.table section)
 */
static bool galois_eth_init(struct cyg_netdevtab_entry *tab)
{
	struct eth_drv_sc *sc = (struct eth_drv_sc *)tab->device_instance;
	galois_eth_priv_t *priv = (galois_eth_priv_t *)sc->driver_private;
	MV_ETH_PORT_INIT eth_port_init;
	int status;
	int i;

#if 0
	/* Just for Galois ASIC */
	os_printf("Galois phy init.\n");
	{
		int i, tmp = 0;

		*(volatile unsigned int *)0xf7f30c08 = 0x0;	/* software control */
		for (i = 0; i < 1000000; i++) tmp++;
		HAL_DELAY_US(100);
		*(volatile unsigned int *)0xf7f30c00 = 0x1;
		for (i = 0; i < 1000000; i++) tmp++;
		HAL_DELAY_US(100);
		*(volatile unsigned int *)0xf7f30c00 = 0x0;
		for (i = 0; i < 1000000; i++) tmp++;
		HAL_DELAY_US(100);
		*(volatile unsigned int *)0xf7f30c00 = 0x1;
		for (i = 0; i < 1000000; i++) tmp++;
		HAL_DELAY_US(100);
		os_printf("delay tmp = %d.\n", tmp);
	}
#endif

	os_printf("Galois ethernet init.\n");
	/*
	 * Disable interrupt and clear interrupt status
	 */
	mvEthInit();

	str2macaddr(priv->macaddr, GALOIS_ETH_MACADDR);
	memcpy(eth_port_init.macAddr, priv->macaddr, GALOIS_MAC_ADDR_SIZE);
	eth_port_init.maxRxPktSize = GALOIS_ETH_MAX_RXSIZE;
	eth_port_init.rxDefQ = GALOIS_ETH_DEFAULT_RXQ;
	for (i = 0; i < MV_ETH_TX_Q_NUM; i++)
		eth_port_init.txDescrNum[i] = GALOIS_ETH_TXQLEN;
	for (i = 0; i < MV_ETH_RX_Q_NUM; i++)
		eth_port_init.rxDescrNum[i] = GALOIS_ETH_RXQLEN;

	/*
	 * Reset ethernet port, disable interrupt, disable port tx/rx.
	 * Initialize registers
	 * Allocate tx/rx buffer descriptor ring and set to register
	 * Initialize Hash table
	 */
	status = mvEthPortInit(&priv->port_ctrl, &eth_port_init);
	if (status != 0) {
		os_printf("mvEthPortInit: Failed to init ethernet port.\n");
		return false;
	}

	/*
	 * For current development only
	 */
#if 1
//#ifdef CYGSEM_DEVS_ETH_ARM_GALOIS_10M
	//os_printf("Setting ethernet to full-duplex 100Mbps.\n");
	os_printf("Setting ethernet to half-duplex 100Mbps.\n");
	{
		//status = mvEthSpeedDuplexSet(NULL, MV_ETH_SPEED_100, MV_ETH_DUPLEX_FULL);
		status = mvEthSpeedDuplexSet(NULL, MV_ETH_SPEED_100, MV_ETH_DUPLEX_HALF);
		if (status != 0) {
			os_printf("Failed to set ethernet port speed/plex.\n");
			return false;
		}
	}
//#endif
#endif

#ifdef CYGSEM_DEVS_ETH_ARM_GALOIS_2BSTUFF
	os_printf("Setting ethernet to 2B stuffing mode.\n");
	{
		unsigned int pcxr;
		pcxr = MV_MEMIO_LE32_READ(ETH_EPCXR);
		pcxr |= 0x10000000;
		GA_REG_WORD32_WRITE(ETH_EPCXR, pcxr);
		pcxr = MV_MEMIO_LE32_READ(ETH_EPCXR);
	}
#endif

#ifdef CYGPKG_NET
	/*
	 * Register ISR/DSR to system
	 */
	Galois_interrupt_vector_registration(
		GALOIS_ETH_IRQ,
		NULL,					/* (*irq_init_func)(cyg_uint32 cpu_id) */
		galois_interrupt_unmask,/* (*irq_enable_func)(cyg_uint32 cpu_id, cyg_uint32 vec) */
		galois_interrupt_mask,	/* (*irq_disable_func)(cyg_uint32 cpu_id, cyg_uint32 vec) */
		NULL,					/* (*irq_ack_func)(cyg_uint32 cpu_id, cyg_uint32 vec) */
		galois_eth_isr,			/* (*irq_isr_func)(cyg_uint32 vec, cyg_addrword_t data) */
		galois_eth_dsr,     	/* (*irq_dsr_func)(cyg_uint32 vec, cyg_addrword_t data) */
		(cyg_addrword_t)sc,		/* cyg_addrword_t data */
		DSR_COUNT_ZERO			/* cyg_ucount32 dsr_count */
	);

	/*
	 * Clear all interrupt status and enable interrupt
	 */
	galois_eth_interrupt_acknowledge();
	galois_interrupt_unmask(GALOIS_ETH_CPU, GALOIS_ETH_IRQ);
#endif
    /*
	 * Initialize upper level driver for ecos
	 */
	os_printf("Initialize upper level ethernet driver.\n");
	(sc->funs->eth_drv->init)(sc, priv->macaddr);

	return true;
}
 
/*
 * Called by "deliver thread" to do rx/tx data from/to device.
 * "deliver thread" is waked up by eth_drv_dsr() to alert that
 * something requires attention.
 */
static void galois_eth_deliver(struct eth_drv_sc * sc)
{
#ifdef CYGPKG_NET
	os_dbgprintf("in deliver.\n");
	galois_eth_poll(sc);
	//galois_eth_interrupt_acknowledge();	/* poll needs picr, don't enable */
	galois_interrupt_unmask(GALOIS_ETH_CPU, GALOIS_ETH_IRQ);
#endif
}

/*
 * Fill the buffer to rx descriptor of @queue
 */
static int galois_eth_rxqfill(galois_eth_priv_t *priv, int queue, int queue_len)
{
	MV_PKT_INFO pkt_info;
	MV_BUF_INFO buf_info;
	eth_buffer_t eth_buf;
	int status, i;

	//if (mvEthRxResourceGet(&priv->port_ctrl, queue));
	for (i = 0; i < queue_len; i++) {
		eth_buf.buf = malloc(GALOIS_ETH_MAX_RXSIZE);
		os_dbgprintf("buf=%08x, ", (unsigned int)eth_buf.buf);
		if (!eth_buf.buf) {
			os_printf("galois_eth_rxqfill: failed to allocate buffer for rx descriptor.\n");
			break;
		}
		eth_buf.aligned_buf = (unsigned char *)DCACHE_ALIGN_ADDR((unsigned int)(eth_buf.buf));
		os_dbgprintf("aligned_buf=%08x, ", (unsigned int)eth_buf.aligned_buf);

		buf_info.bufVirtPtr = eth_buf.aligned_buf;
		buf_info.bufSize = GALOIS_ETH_MAX_RXSIZE - 32;
		pkt_info.osInfo = (unsigned long)(eth_buf.aligned_buf);
		pkt_info.pFrags = &buf_info;
		pkt_info.pktSize = GALOIS_ETH_MAX_RXSIZE - 32;

		/* Fill rx buffer to rx bd ring */
		status = mvEthPortRxDone(&priv->port_ctrl, queue, &pkt_info);
		if (status == MV_RES_ETH_OK) {
			os_dbgprintf("galois_eth_rxqfill: added one rxq buffer.\n");
			priv->rxq_count[queue]++;
		} else if (status == MV_RES_ETH_FULL) {
			os_printf("galois_eth_rxqfill: rxq full.\n");
			priv->rxq_count[queue]++;
			break;
		} else {
			os_printf("galois_eth_rxqfill: rxq fill error. free buffer\n");
			free(eth_buf.buf);	/* right? */
			break;
		}
	}
	return 0;
}

/*
 * Start up the ethernet interface, maybe called multiple times. whenever any
 * hardware oriented changes, it will be called.
 */
static void galois_eth_start(struct eth_drv_sc *sc, unsigned char *enaddr, int flags)
{
	galois_eth_priv_t *priv = (galois_eth_priv_t *)sc->driver_private;
	int queue;
	int status;
	int retry_time;

	os_printf("Galois ethernet start.\n");
	for (queue = 0; queue < MV_ETH_RX_Q_NUM; queue++) {
		galois_eth_rxqfill(priv, queue, GALOIS_ETH_RXQLEN);
	}

	GA_REG_WORD32_WRITE(ETH_EICR, 0);

	for (retry_time = 0; retry_time < 16; retry_time++) {
		/* Enable ethernet, enable RX DMA */
		status = mvEthPortEnable(&priv->port_ctrl);
		if (status == 0) {
			os_printf("mvEthPortEnable: [%d]success, link up.\n", retry_time);
			break;
		} else {
			os_printf("mvEthPortEnable: [%d]enable ethernet port failed.(status=%d, link down.)\n", 
					retry_time, status);
		}
		HAL_DELAY_US(1000000);
	}

	GA_REG_WORD32_WRITE(ETH_EIMR, GETH_PICR_MASK);
	priv->intr_mask = GETH_PICR_MASK;
	priv->start_status = 1;

	return;
}

/*
 * Stop the ethernet interface
 */
static void galois_eth_stop(struct eth_drv_sc *sc)
{
	galois_eth_priv_t *priv = (galois_eth_priv_t *)sc->driver_private;

	if (priv->start_status != 1) {
		os_printf("Galois ethernet not started, stop exits.\n");
		return;
	}
	os_printf("Galois ethernet stop.\n");
	GA_REG_WORD32_WRITE(ETH_EIMR, 0);
	mvEthPortDisable(&priv->port_ctrl);
	GA_REG_WORD32_WRITE(ETH_EICR, 0);
	priv->intr_mask = 0;

	return;
}

/*
 * low-level driver control
 */
static int galois_eth_control(struct eth_drv_sc *sc, 
			unsigned long key, void *data, int length)
{
	galois_eth_priv_t *priv = (galois_eth_priv_t *)sc->driver_private;

	os_printf("Galois ethernet control.\n");

	/* TODO */
	switch (key) {
#ifdef ETH_DRV_GET_MAC_ADDRESS
	case ETH_DRV_GET_MAC_ADDRESS:
	  os_printf("ETH_DRV_GET_MAC_ADDRESS\n");
      if (length != GALOIS_MAC_ADDR_SIZE)
        return 1;
      memcpy(data, priv->macaddr, GALOIS_MAC_ADDR_SIZE);
      return 0;
#endif
	default:
		return 1;
	}

	/* Invalid by default */
	return 1;
}

/*
 * Called to see if packet can be sent, if OK return 0
 */
static int galois_eth_can_send(struct eth_drv_sc *sc)
{
	galois_eth_priv_t *priv = (galois_eth_priv_t *)sc->driver_private;
	int queue = 0;

	return mvEthTxResourceGet(&priv->port_ctrl, queue);
}

/*
 * Send the packet out
 */
static void galois_eth_send(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, 
			int sg_len, int total_len, unsigned long key)
{
	galois_eth_priv_t *priv = (galois_eth_priv_t *)sc->driver_private;
	int queue = 0;
	int status;
	int i;

	/* 
	 * key and IRQ disabled ? 
	 */
	priv->tx_pkt_info.osInfo = key;
	priv->tx_pkt_info.status = 0;
	priv->tx_pkt_info.pFrags = &priv->tx_buf_info[0];

	for (i = 0; i < sg_len; i++) {
		priv->tx_buf_info[i].bufSize = sg_list[i].len;
		priv->tx_buf_info[i].bufVirtPtr = (unsigned char *)sg_list[i].buf;
	}
	priv->tx_pkt_info.numFrags = sg_len;
	priv->tx_pkt_info.pktSize = total_len;

	status = mvEthPortTx(&priv->port_ctrl, queue, &priv->tx_pkt_info);
	if (status == MV_RES_ETH_OK) {
		os_dbgprintf("galois_eth_send: tx ok.\n");
	} else {
		os_printf("galois_eth_send: tx error.\n");
		if (status == MV_RES_ETH_NORESOURCE)
			os_printf("tx queue full.\n");
		if (status == MV_RES_ETH_FAILED)
			os_printf("error on transmit.\n");
	}
}

/*
 * Actually copy packet from rx ring bd's buffer to tcp/ip stack
 */
static void galois_eth_recv(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, int sg_len)
{
	galois_eth_priv_t *priv = (galois_eth_priv_t *)sc->driver_private;
	unsigned char *bp;
	int i;

	bp = (unsigned char *)priv->rxbuf;
	os_dbgprintf("priv->rxbuf=%08x, sg_len=%d\n", (unsigned int)bp, sg_len);

	for (i = 0;  i < sg_len;  i++) {
		os_dbgprintf("sglist[%d].buf=%08x, sglist[%d].len=%d\n", i, sg_list[i].buf, i, sg_list[i].len);
		if ((!sg_list[i].buf) || (sg_list[i].len <= 0))
			os_printf("#WARNING(%d): failed to allocate mbuf for packet!!!\n", i);
		if (sg_list[i].buf && sg_list[i].len) {
			memcpy((void *)sg_list[i].buf, bp, sg_list[i].len);
			bp += sg_list[i].len;
		}
	}
}

/*
 * Called when rx interrupts, it calls the upper-layer recv function to 
 * actually receive the packet
 */
static void galois_eth_rx_event(struct eth_drv_sc *sc)
{
	galois_eth_priv_t *priv = (galois_eth_priv_t *)sc->driver_private;
	unsigned int queue = 0;
	MV_BUF_INFO buf_info;
	MV_PKT_INFO pkt_info;
	unsigned char *buf;
	int status;
#if 0
	int cache_state;
	HAL_DCACHE_IS_ENABLED(cache_state);
#endif
	os_dbgprintf("start to mvEthPortRx.\n");
	status = mvEthPortRx(&priv->port_ctrl, queue, &pkt_info);
	os_dbgprintf("mvEthPortRx status=%d\n", status);
	while (status == 0) {
		os_dbgprintf("mvEthPortRx ok.\n");
		if (!pkt_info.osInfo) {
			os_printf("galois_eth_rx_event: rx error.\n");
			continue;
		}
		buf = (unsigned char *)pkt_info.osInfo;	/* rx aligned buf */
#if 0
		if (cache_state)
			HAL_DCACHE_INVALIDATE(buf, pkt_info.pktSize);
#else
		InvalidateDCacheRegion((int *)buf, pkt_info.pktSize);
#endif

#if CYGSEM_DEVS_ETH_ARM_GALOIS_2BSTUFF
		buf += 2;	/* 2B stuff */
		priv->rxbuf = buf;
		(sc->funs->eth_drv->recv)(sc, pkt_info.pktSize - 4 - 2);
#else
		priv->rxbuf = buf;
		(sc->funs->eth_drv->recv)(sc, pkt_info.pktSize - 4);
#endif
#if 0
		if (cache_state)
			HAL_DCACHE_FLUSH(buf, pkt_info.pktSize);
#else
		CleanAndInvalidateDCacheRegion((int *)buf, pkt_info.pktSize);
#endif

		/*
		 * Return the buffer back to rx ring
		 */
		buf_info.bufVirtPtr = (unsigned char *)pkt_info.osInfo;
		buf_info.bufSize = GALOIS_ETH_MAX_RXSIZE - 32;
		pkt_info.pFrags = &buf_info;
		pkt_info.pktSize = GALOIS_ETH_MAX_RXSIZE - 32;
		status = mvEthPortRxDone(&priv->port_ctrl, queue, &pkt_info);
		if (status == MV_RES_ETH_OK) {
			os_dbgprintf("galois_eth_rx_event: added one rxq buffer.\n");
			priv->rxq_count[queue]++;
		} else if (status == MV_RES_ETH_FULL) {
			os_dbgprintf("galois_eth_rx_event: rxq full.\n");
			priv->rxq_count[queue]++;
		} else {
			os_printf("galois_eth_rx_event: rxq fill error. free buffer. serial/strange error!!!\n");
			free(buf_info.bufVirtPtr);	/* right? */
		}

		os_dbgprintf("mvEthPortRxDone status=%d\n", status);
		
		/*
		 * Receive next packet 
		 */
		status = mvEthPortRx(&priv->port_ctrl, queue, &pkt_info);
		os_dbgprintf("mvEthPortRx status=%d\n", status);
	}

	/*
	 * Judge if the resource is used up
	 */
	if ((status != MV_RES_ETH_OK) && (status != MV_RES_ETH_RXBUSY))
		os_printf("#WARNING: mvEthPortRx returns bad status(%d).\n", status);
}

/*
 * Called when tx interrupts
 */
static void galois_eth_tx_event(struct eth_drv_sc *sc)
{
	galois_eth_priv_t *priv = (galois_eth_priv_t *)sc->driver_private;
	MV_PKT_INFO pkt_info;
	int queue = 0;
	int status;

	while (1) {
		status = mvEthPortTxDone(&priv->port_ctrl, queue, &pkt_info);
		if (status == MV_RES_ETH_OK) {
			if (!pkt_info.osInfo) {	/* This's key */
				os_printf("error in tx done.\n");
				continue;
			}
			if (pkt_info.status & ETH_ERROR_SUMMARY_BIT)
				os_printf("bad tx done status.\n");

			os_dbgprintf("tx done ok.\n");
			/* Tell the upper layer to free packet space */
			(sc->funs->eth_drv->tx_done)(sc, pkt_info.osInfo, 0);
		} else {
			if (status == MV_RES_ETH_NORESOURCE)
				os_dbgprintf("no more work.\n");
			else if (status == MV_RES_ETH_TXBUSY)
				os_dbgprintf("hw still in tx.\n");
			else 
				os_printf("unrecognized status in tx done.\n");
			break;
		}
	}
}

/*
 * Called by deliver function to handle interrupts
 */
static void galois_eth_poll(struct eth_drv_sc *sc)
{
	galois_eth_priv_t *priv = (galois_eth_priv_t *)sc->driver_private;
	unsigned int picr;
   
	os_dbgprintf("poll picr eicr=%08x, eimr=%08x[%08x]\n", 
			MV_MEMIO_LE32_READ(ETH_EICR), MV_MEMIO_LE32_READ(ETH_EIMR), priv->intr_mask);

	while ((picr = MV_MEMIO_LE32_READ(ETH_EICR)) != 0 ) {
		os_dbgprintf("picr=%08x.\n", picr);
		picr &= priv->intr_mask;
		GA_REG_WORD32_WRITE(ETH_EICR, ~picr);	/* clear cause */

		/* tx event */
		if (picr & (GETH_TXQ_MASK | GETH_TXQ_RES_MASK)) {
			os_dbgprintf("poll to tx.\n");
			galois_eth_tx_event(sc);
		}
	
		/* rx event */
		if (picr & (GETH_RXQ_MASK | GETH_RXQ_RES_MASK)) {
			os_dbgprintf("poll to rx.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			galois_eth_rx_event(sc);
		}
	}
}

static int galois_eth_int_vector(struct eth_drv_sc *sc)
{
    return (GALOIS_ETH_IRQ);
}

/*
 * Some common code for convert string to mac address
 */
static unsigned int char2hex(char ch)
{
	if( (ch >= '0') && (ch <= '9') ) 
		return( ch - '0' );
	if( (ch >= 'a') && (ch <= 'f') ) 
		return( ch - 'a' + 10 );
	if( (ch >= 'A') && (ch <= 'F') ) 
		return( ch - 'A' + 10 );
	return 0;
}

static void str2macaddr(unsigned char *dest, unsigned char *source)
{
	dest[0] = (char2hex(source[0]) << 4) + char2hex(source[1]);
	dest[1] = (char2hex(source[2]) << 4) + char2hex(source[3]);
	dest[2] = (char2hex(source[4]) << 4) + char2hex(source[5]);
	dest[3] = (char2hex(source[6]) << 4) + char2hex(source[7]);
	dest[4] = (char2hex(source[8]) << 4) + char2hex(source[9]);
	dest[5] = (char2hex(source[10]) << 4) + char2hex(source[11]);

#if 0
	/* Random MAC address. */
	{
		cyg_uint32 clk;
		HAL_CLOCK_READ(&clk);
		dest[3] = clk & 0xff;
		dest[4] = (clk >> 8) & 0xff;
	}
#endif
}

