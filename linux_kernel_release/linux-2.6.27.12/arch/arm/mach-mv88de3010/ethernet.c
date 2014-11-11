#include "galois_common.h"
#include "galois_io.h"
#include "eth.h"
#include <mach/galois_platform.h>
#include <mach/irqs.h>

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/pci.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/platform_device.h>

#define ETH_MTU 1500
static unsigned char galois_mac_addr[12] = "00188b7b6e77";
static int galois_2bstuff_enable = 0;
static struct net_device *galois_eth_dev = NULL;
#ifdef CONFIG_MV88DE3010_BERLIN_A0
static int berlin_a0_stopqueue = 0;
#endif

/* default TX/RX queue */
#define ETH_DEF_TXQ	0
#define ETH_DEF_RXQ	0

/* 
 * Cause register bits:
 * rx-ready-q0, rx-error-q0, tx-done-q0, rx-overrun, tx-underrun
 */
#define ETH_TXQ_GOOD_MASK	(ETH_CAUSE_TX_BUF_MASK(ETH_DEF_TXQ))
#define ETH_TXQ_ERROR_MASK	\
	(ETH_CAUSE_TX_UNDERRUN_ERROR | ETH_CAUSE_TX_ERROR_MASK(ETH_DEF_TXQ))

#define ETH_RXQ_GOOD_MASK	(ETH_CAUSE_RX_READY_MASK(ETH_DEF_RXQ))
#define ETH_RXQ_ERROR_MASK	\
	(ETH_CAUSE_RX_OVERRUN_ERROR | ETH_CAUSE_RX_ERROR_MASK(ETH_DEF_RXQ))

#define ETH_PICR_MASK \
	(ETH_RXQ_GOOD_MASK | ETH_RXQ_ERROR_MASK | ETH_TXQ_GOOD_MASK | ETH_TXQ_ERROR_MASK)

/* 
 * rx buffer size:
 * 2(HW padding) 14(MAC hdr) 4(CRC)
 */
#define RX_BUFFER_SIZE(mtu, priv) (((mtu + 2 + 14 + 4) & (~ETH_RX_BUFFER_MASK)) ? \
	(((mtu + 2 + 14 + 4) & ETH_RX_BUFFER_MASK) + 8) : (mtu + 2 + 14 + 4))

#ifdef CONFIG_MV88DE3010_BERLIN_A0
int gethDescRxQ[MV_ETH_RX_Q_NUM] = { 1 }; /* MV_ETH_RX_Q_NUM=1 */
#else
int gethDescRxQ[MV_ETH_RX_Q_NUM] = { 128 }; /* MV_ETH_RX_Q_NUM=1 */
#endif
int gethDescTxQ[MV_ETH_TX_Q_NUM] = { 256 }; /* MV_ETH_TX_Q_NUM=1 */

#define ETH_RXQ_DESC(q)  (gethDescRxQ[q])


/****************************************************** 
 * driver debug control --                            *
 ******************************************************/
/* debug main on/off switch (more in debug control below ) */
#undef GALOIS_ETHDEBUG

#define GALOIS_ETHDBG_OFF     0x0000
#define GALOIS_ETHDBG_RX      0x0001
#define GALOIS_ETHDBG_TX      0x0002
#define GALOIS_ETHDBG_RX_FILL 0x0004
#define GALOIS_ETHDBG_TX_DONE 0x0008
#define GALOIS_ETHDBG_LOAD    0x0010
#define GALOIS_ETHDBG_IOCTL   0x0020
#define GALOIS_ETHDBG_INT     0x0040
#define GALOIS_ETHDBG_STATS   0x0080
#define GALOIS_ETHDBG_ALL     0xffff

#ifdef GALOIS_ETHDEBUG
# define GALOIS_ETHDBG(FLG, X) if( (geth_dbg & (FLG)) == (FLG) ) printk X
#else
# define GALOIS_ETHDBG(FLG, X) 
#endif

u32 geth_dbg = GALOIS_ETHDBG_LOAD | GALOIS_ETHDBG_INT | GALOIS_ETHDBG_TX | GALOIS_ETHDBG_TX_DONE | GALOIS_ETHDBG_RX | GALOIS_ETHDBG_RX_FILL | GALOIS_ETHDBG_STATS;

/****************************************************** 
 * driver statistics control --                       *
 ******************************************************/
/* statistics main on/off switch (more in statistics control below ) */
#define GALOIS_ETHSTATISTICS

#define GALOIS_ETHSTAT_OFF     0x0000
#define GALOIS_ETHSTAT_RX      0x0001
#define GALOIS_ETHSTAT_TX      0x0002
#define GALOIS_ETHSTAT_RX_FILL 0x0004
#define GALOIS_ETHSTAT_TX_DONE 0x0008
#define GALOIS_ETHSTAT_LOAD    0x0010
#define GALOIS_ETHSTAT_IOCTL   0x0020
#define GALOIS_ETHSTAT_INT     0x0040
#define GALOIS_ETHSTAT_ALL     0xffff

#ifdef GALOIS_ETHSTATISTICS
# define GALOIS_ETHSTAT(FLG, CODE) if( (geth_stat & (FLG)) == (FLG) ) CODE;
#else
# define GALOIS_ETHSTAT(FLG, CODE)
#endif

u32 geth_stat =  GALOIS_ETHDBG_LOAD | GALOIS_ETHDBG_INT | GALOIS_ETHDBG_TX | GALOIS_ETHDBG_TX_DONE | GALOIS_ETHDBG_RX | GALOIS_ETHDBG_RX_FILL | GALOIS_ETHDBG_STATS;

extern u32 overEthAddr;

/****************************************************** 
 * device private information --                      *
 ******************************************************/
typedef struct _galois_eth_stat
{
    /* interrupt stats */
    u32 int_total, int_rx_events, int_tx_done_events;
    u32 int_phy_events, int_none_events;

    /* rx stats */
    u32 rx_poll_events, rx_poll_hal_ok[MV_ETH_RX_Q_NUM], rx_poll_hal_no_resource[MV_ETH_RX_Q_NUM];
    u32 rx_poll_hal_no_more[MV_ETH_RX_Q_NUM], rx_poll_hal_error[MV_ETH_RX_Q_NUM], rx_poll_hal_invalid_skb[MV_ETH_RX_Q_NUM];
    u32 rx_poll_hal_bad_stat[MV_ETH_RX_Q_NUM], rx_poll_netif_drop[MV_ETH_RX_Q_NUM], rx_poll_netif_complete;

    /* rx-fill stats */
    u32 rxbuf_fill_events[MV_ETH_RX_Q_NUM], rxbuf_fill_alloc_skb_fail[MV_ETH_RX_Q_NUM], rxbuf_fill_hal_ok[MV_ETH_RX_Q_NUM];
    u32 rxbuf_fill_hal_full[MV_ETH_RX_Q_NUM], rxbuf_fill_hal_error[MV_ETH_RX_Q_NUM], rxbuf_fill_timeout_events;

    /* tx stats */
    u32 tx_events, tx_hal_ok[MV_ETH_TX_Q_NUM], tx_hal_no_resource[MV_ETH_TX_Q_NUM], tx_hal_error[MV_ETH_TX_Q_NUM];
    u32 tx_hal_unrecognize[MV_ETH_TX_Q_NUM], tx_netif_stop[MV_ETH_TX_Q_NUM], tx_timeout;

    /* tx-done stats */
    u32 tx_done_events, tx_done_hal_invalid_skb[MV_ETH_TX_Q_NUM], tx_done_hal_bad_stat[MV_ETH_TX_Q_NUM];
    u32 tx_done_hal_still_tx[MV_ETH_TX_Q_NUM], tx_done_hal_ok[MV_ETH_TX_Q_NUM], tx_done_hal_no_more[MV_ETH_TX_Q_NUM];
    u32 tx_done_hal_unrecognize[MV_ETH_TX_Q_NUM], tx_done_max[MV_ETH_TX_Q_NUM], tx_done_min[MV_ETH_TX_Q_NUM], tx_done_netif_wake[MV_ETH_TX_Q_NUM];

} galois_eth_stat;

/*
 * private struct of net_device 
 */
typedef struct _galois_eth_priv
{
    spinlock_t lock;
	struct net_device *netdev;
    struct net_device_stats stats;

    u32 rxq_count[MV_ETH_RX_Q_NUM];
    u32 txq_count[MV_ETH_TX_Q_NUM];
    MV_BUF_INFO tx_buf_info_array[MAX_SKB_FRAGS+3];
    MV_PKT_INFO tx_pkt_info;
    ETH_PORT_CTRL port_ctrl;

    struct napi_struct napi;
	struct mii_if_info mii;
	struct timer_list watchdog;
    struct timer_list rxbuf_fill_timer;
    unsigned rxbuf_fill_flag;

    u32 intr_mask;
    u32 rxcause;
    u32 txcause;

#ifdef GALOIS_ETHSTATISTICS
    galois_eth_stat geth_stat;
#endif
} galois_eth_priv; 

/****************************************************** 
 * functions prototype --                             *
 ******************************************************/
static int geth_start_internals( struct net_device *dev );
static int geth_open( struct net_device *dev );
static int geth_stop_internals( struct net_device *dev );
static int geth_close( struct net_device *dev );
static int geth_start_xmit( struct sk_buff *skb, struct net_device *dev );

static void geth_tx_done( struct net_device *dev );
static void geth_tx_timeout( struct net_device *dev );
#ifdef CONFIG_MV88DE3010_BERLIN_A0
static int geth_rx_pkt( struct net_device *dev );
#else
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
static int geth_rx_poll( struct net_device *dev, int *budget );
#else
static int geth_rx_poll(struct napi_struct *napi, int budget);
#endif
#endif
static u32 geth_rxbuf_fill( struct net_device *dev, unsigned int queue, int count );
static void geth_rxbuf_fill_on_timeout( unsigned long data );
static irqreturn_t geth_interrupt_handler( int rq , void *dev_id );

static struct net_device_stats* geth_get_stats( struct net_device *dev );
static void geth_set_multicast_list(struct net_device *dev);
static int geth_set_mac_address( struct net_device *dev, void *addr );
static int geth_change_mtu_internals( struct net_device *dev, int mtu );
static int geth_change_mtu( struct net_device *dev, int mtu );
static void convert_str_to_mac( char *source , char *dest );
static unsigned int str_to_hex( char ch );
void print_geth_stat(void);


/*********************************************************** 
 *   start a network device. connect and enable interrupts *
 *   set hw defaults. fill rx buffers. restart phy link    *
 *   auto neg. set device link flags. report status.       *
 ***********************************************************/
static int geth_start_internals(struct net_device *dev)
{
    unsigned int status;
    unsigned int queue;

    galois_eth_priv *priv = netdev_priv(dev);
 
    /* fill rx ring with buffers */
    for (queue = 0; queue < MV_ETH_RX_Q_NUM; queue++) {
    	geth_rxbuf_fill(dev, queue, ETH_RXQ_DESC(queue));
    }

    /* start the hal - rx/tx activity */
    status = mvEthPortEnable( &(priv->port_ctrl) );
    if (status == MV_RES_ETH_OK)
		printk(KERN_INFO "Enable eth port, and initial link is up\n");
	else if (status == MV_RES_ETH_NOTREADY) 
		printk(KERN_INFO "Enable eth port, but initial link is down\n");
	else {
		printk( KERN_ERR "%s: ethPortEnable failed", dev->name );
		return -1;
    }

    /* clear all ethernet port interrupts */
    GA_REG_WORD32_WRITE(ETH_EICR, 0);

    /* unmask rx-ready-q0, tx-done-q0, rx-error, rx-overrun, tx-underrun interrupts */
    GA_REG_WORD32_WRITE(ETH_EIMR, ETH_PICR_MASK);
    priv->intr_mask = ETH_PICR_MASK;

    return 0;
}

static int geth_open(struct net_device *dev) 
{
    unsigned long flags;
    galois_eth_priv *priv = netdev_priv(dev);

    GALOIS_ETHDBG( GALOIS_ETHDBG_LOAD, ("%s: starting... ", dev->name ) );

    spin_lock_irqsave(&(priv->lock), flags);

    /* connect to port interrupt line */
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
	if (request_irq(dev->irq, geth_interrupt_handler,
	   (SA_INTERRUPT | SA_SAMPLE_RANDOM | SA_SHIRQ) ,dev->name, dev)) {
#else
    if (request_irq(dev->irq, geth_interrupt_handler,
				(IRQF_DISABLED | IRQF_SAMPLE_RANDOM | IRQF_SHARED) ,
				dev->name, dev)) {
#endif
        printk(KERN_ERR "cannot assign irq%d to %s\n", dev->irq, dev->name);
        dev->irq = 0;
		goto error;
    }

    netif_carrier_off(dev);
    netif_stop_queue(dev);

    /* fill rx buffers, start rx/tx activity, set coalescing */
    if (geth_start_internals(dev) != 0 ) {
        printk(KERN_ERR "%s: start internals failed\n", dev->name );
		goto error;
    }

    /* Kick the napi to working. make sure invoking napi_enable here */
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,24)) && !defined(CONFIG_MV88DE3010_BERLIN_A0)
    napi_enable(&priv->napi);
#endif

	mod_timer(&priv->watchdog, jiffies);
	netif_carrier_on(dev);
	netif_wake_queue(dev);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
    netif_poll_enable(dev); /* ? */
#endif
    GALOIS_ETHDBG(GALOIS_ETHDBG_LOAD, ("%s: start ok\n", dev->name));

    spin_unlock_irqrestore( &(priv->lock), flags);
    return 0;

error:
    spin_unlock_irqrestore( &(priv->lock), flags);

    if (dev->irq != 0)
       	free_irq( dev->irq, dev );

    printk(KERN_ERR "%s: start failed\n", dev->name);
    return -1;
}

/*********************************************************** 
 *   stop interface with linux core. stop port activity.   *
 *   clear and mask all interrupts.                        *
 *   free skb's from rx/tx rings. reset Hw                 *
 ***********************************************************/
static int geth_stop_internals( struct net_device *dev )
{
    galois_eth_priv *priv = netdev_priv(dev);
    MV_PKT_INFO pkt_info;
    unsigned int queue;

    /* stop the port activity, mask all interrupts */
    mvEthPortDisable(&(priv->port_ctrl));
    
    /* clear and mask all ethernet port interrupts */
    GA_REG_WORD32_WRITE(ETH_EICR, 0);
    GA_REG_WORD32_WRITE(ETH_EIMR, 0);
    priv->intr_mask = 0;

    /* free the skb's in the hal tx ring */
    for (queue = 0; queue < MV_ETH_TX_Q_NUM; queue++)
    {
    	while (mvEthPortForceTxDone(&(priv->port_ctrl), queue, &pkt_info) == 0 ) {
        	priv->txq_count[queue]--;
			if (pkt_info.osInfo)
				dev_kfree_skb_any((struct sk_buff *)pkt_info.osInfo);
			else {
				printk(KERN_ERR "%s: error in ethGetNextRxBuf\n", dev->name);
				goto error;
			}
    	}
    }

    /* free the skb's in the hal rx ring */
    for(queue=0; queue<MV_ETH_RX_Q_NUM; queue++)
    {    
    	while (mvEthPortForceRx( &(priv->port_ctrl), queue, &pkt_info) == 0) {
        	priv->rxq_count[queue]--;
			if (pkt_info.osInfo)
	    		dev_kfree_skb_any( (struct sk_buff *)pkt_info.osInfo );
			else {
	    		printk( KERN_ERR "%s: error in ethGetNextRxBuf\n", dev->name );
	    		goto error;
			}
    	}
    }

    /* Reset Rx/Tx descriptors ring */
    for (queue=0; queue<MV_ETH_RX_Q_NUM; queue++)
		ethResetRxDescRing(&(priv->port_ctrl), queue);
    for (queue=0; queue<MV_ETH_TX_Q_NUM; queue++)
		ethResetTxDescRing(&(priv->port_ctrl), queue);
#if 0
    /* 
	 * NOTE:
	 * if you reset Address hash table, mac address in hash table will be lost
	 * so next time, please reset it yourself.
	 */
    mvEthResetAddrTbl(&(priv->port_ctrl));
#endif

    return 0;

 error:
    printk( KERN_ERR "%s: stop internals failed\n", dev->name );
    return -1;
}

static int geth_close( struct net_device *dev )
{
    unsigned long flags;
    galois_eth_priv *priv = netdev_priv(dev);

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
    netif_poll_disable(dev);
#endif
    spin_lock_irqsave(&(priv->lock), flags);
    /* stop upper layer */
    netif_carrier_off(dev);
    netif_stop_queue(dev);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,24)) && !defined(CONFIG_MV88DE3010_BERLIN_A0)
	napi_disable(&priv->napi);
#endif
	del_timer_sync(&priv->watchdog);

	/* stop tx/rx activity, mask all interrupts, relese skb in rings,*/
	geth_stop_internals(dev);
    spin_unlock_irqrestore(&priv->lock, flags);

    if (dev->irq != 0)
       	free_irq(dev->irq, dev);

    return 0;
}

/*********************************************************** 
 *   send a packet.                                        *
 ***********************************************************/
static int geth_start_xmit(struct sk_buff *skb , struct net_device *dev)
{
    galois_eth_priv *priv = netdev_priv(dev);
	int queue = ETH_DEF_TXQ;
    struct net_device_stats *stats = &priv->stats;
    unsigned long flags;
    int status;
	int i;
    int ret = 0;

    if (netif_queue_stopped(dev)) {
        printk( KERN_ERR "%s: transmitting while stopped\n", dev->name );
        return 1;
    }

    local_irq_save(flags);
    if (!spin_trylock(&priv->lock)) {
        local_irq_restore(flags);
        return NETDEV_TX_LOCKED;
    }

    GALOIS_ETHDBG(GALOIS_ETHDBG_TX, ("%s: tx, #%d frag(s), csum by %s\n",
				dev->name, skb_shinfo(skb)->nr_frags+1, 
				(skb->ip_summed==CHECKSUM_PARTIAL)?"HW":"CPU"));
    GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX, (priv->geth_stat.tx_events++));

	/* prepare packet info */
    priv->tx_pkt_info.osInfo = (unsigned long)skb;
    priv->tx_pkt_info.pktSize = skb->len;
    priv->tx_pkt_info.pFrags = &priv->tx_buf_info_array[1];
    priv->tx_pkt_info.status = 0;
    
    /* see if this is a single/multiple buffered skb */
    if (skb_shinfo(skb)->nr_frags == 0) {
        priv->tx_pkt_info.pFrags->bufVirtPtr = skb->data;
        priv->tx_pkt_info.pFrags->bufSize = skb->len;
        priv->tx_pkt_info.numFrags = 1;
    }
    else {
        MV_BUF_INFO *p_buf_info = priv->tx_pkt_info.pFrags;

        /* first skb fragment */
        p_buf_info->bufSize = skb_headlen(skb);
        p_buf_info->bufVirtPtr = skb->data;
        p_buf_info++;

        /* now handle all other skb fragments */
        for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
            skb_frag_t *frag = &skb_shinfo(skb)->frags[i];

            p_buf_info->bufSize = frag->size;
            p_buf_info->bufVirtPtr = page_address(frag->page) + frag->page_offset;
            p_buf_info++;
        }

        priv->tx_pkt_info.numFrags = skb_shinfo(skb)->nr_frags + 1;
    }

    /* now send the packet */
    status = mvEthPortTx(&(priv->port_ctrl), queue, &priv->tx_pkt_info);

    /* check Tx status */
    if( status == MV_RES_ETH_OK) {
#ifdef CONFIG_MV88DE3010_BERLIN_A0
		netif_stop_queue(dev); /* Temporary fix for Berlin A0 bug */
		berlin_a0_stopqueue = 1;
#endif
        dev->trans_start = jiffies;
        stats->tx_bytes += skb->len;
        stats->tx_packets ++;
        priv->txq_count[queue]++;
        GALOIS_ETHDBG( GALOIS_ETHDBG_TX, ("ok (%d); ", priv->txq_count[queue]) );
        GALOIS_ETHSTAT( GALOIS_ETHSTAT_TX, (priv->geth_stat.tx_hal_ok[queue]++) );
    }
    else {
        /* tx failed. higher layers will free the skb */
        ret = 1;
        stats->tx_dropped++;

        if (status == MV_RES_ETH_NORESOURCE) {
            /* it must not happen because we call to netif_stop_queue in advance. */
            GALOIS_ETHDBG(GALOIS_ETHDBG_TX, 
					("%s: queue is full, stop transmit\n", dev->name));
            netif_stop_queue(dev);
            GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX, (priv->geth_stat.tx_hal_no_resource[queue]++));
            GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX, (priv->geth_stat.tx_netif_stop[queue]++));
        }
        else if (status == MV_RES_ETH_FAILED) {
            printk(KERN_ERR "%s: error on transmit\n", dev->name);
            GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX, (priv->geth_stat.tx_hal_error[queue]++));
        }
        else {
            printk(KERN_ERR "%s: unrecognize status on transmit\n", dev->name);
            GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX, (priv->geth_stat.tx_hal_unrecognize[queue]++));
        }
    }

	/* If the rest resource is too less, stop queue */
    if (mvEthTxResourceGet(&(priv->port_ctrl), queue) <= MAX_SKB_FRAGS ) {
        GALOIS_ETHDBG(GALOIS_ETHDBG_TX, ("%s: stopping network tx interface\n", dev->name));
        netif_stop_queue(dev);
        GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX, (priv->geth_stat.tx_netif_stop[queue]++));
    }
    spin_unlock_irqrestore(&(priv->lock), flags);

    return ret;
}

/*********************************************************** 
 *   release transmitted packets. interrupt context.       *
 ***********************************************************/
static void geth_tx_done( struct net_device *dev)
{
    galois_eth_priv *priv = netdev_priv(dev);
    unsigned int queue = ETH_DEF_TXQ;
    struct net_device_stats *stats = &priv->stats;
    MV_PKT_INFO pkt_info;
    u32 count = 0;
    int status;

    GALOIS_ETHDBG(GALOIS_ETHDBG_TX_DONE, ("%s: tx-done ", dev->name));
    GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX_DONE, (priv->geth_stat.tx_done_events++));

    /* release the transmitted packets */
    while (1) {
        /* get a packet */  
        status = mvEthPortTxDone( &(priv->port_ctrl), queue, &pkt_info );

		if (status == MV_RES_ETH_OK) {
			priv->txq_count[queue]--;

	    	/* validate skb */
	    	if (!(pkt_info.osInfo)) {
	        	printk(KERN_ERR "%s: error in tx-done\n", dev->name);
				stats->tx_errors++;
				GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX_DONE, 
						(priv->geth_stat.tx_done_hal_invalid_skb[queue]++));
				continue;
			}

	    	/* handle tx error */
			if (pkt_info.status & (ETH_ERROR_SUMMARY_BIT)) {
	        	printk(KERN_ERR "%s: bad tx-done status\n", dev->name);
				GALOIS_ETHDBG(GALOIS_ETHDBG_TX_DONE, ("%s: bad tx-done status\n",dev->name));
				GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX_DONE, 
						(priv->geth_stat.tx_done_hal_bad_stat[queue]++));
				stats->tx_errors++;
			}

			/* it transmission was previously stopped, now it can be restarted. */
			if (netif_queue_stopped(dev) && (dev->flags & IFF_UP)) {
				GALOIS_ETHDBG(GALOIS_ETHDBG_TX_DONE, ("%s: restart transmit\n", dev->name));
				GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX_DONE, 
						(priv->geth_stat.tx_done_netif_wake[queue]++));
				netif_wake_queue(dev);	
			}

	    	/* release the skb */
			dev_kfree_skb_irq((struct sk_buff *)pkt_info.osInfo);
			count++;

			GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX_DONE, 
					(priv->geth_stat.tx_done_hal_ok[queue]++));
			GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX_DONE, 
					if(priv->geth_stat.tx_done_max[queue] < count) 
					priv->geth_stat.tx_done_max[queue] = count);
			GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX_DONE, 
					if(priv->geth_stat.tx_done_min[queue] > count) 
					priv->geth_stat.tx_done_min[queue] = count);
		} else if (status == MV_RES_ETH_NORESOURCE) {/*EMPTY*/
			/* no more work */
			GALOIS_ETHDBG(GALOIS_ETHDBG_TX_DONE, ("no more work "));
			GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX_DONE, 
					(priv->geth_stat.tx_done_hal_no_more[queue]++));
			break;
		} else if (status == MV_RES_ETH_TXBUSY) {
			/* hw still in tx */
			GALOIS_ETHDBG(GALOIS_ETHDBG_TX_DONE, ("hw still in tx "));
			GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX_DONE, 
					(priv->geth_stat.tx_done_hal_still_tx[queue]++));
			break;
		} else {
			printk(KERN_ERR "%s: unrecognize status on tx done\n", dev->name);
			GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX_DONE, 
					(priv->geth_stat.tx_done_hal_unrecognize[queue]++));
			stats->tx_errors++;
			break;
		}
	} /* while(1) */

	GALOIS_ETHDBG(GALOIS_ETHDBG_TX_DONE, 
			("%s: tx-done %d (%d)\n", dev->name, count, priv->txq_count[queue]));
}

/*********************************************************** 
 *   nothing to be done (?)                                *
 ***********************************************************/
static void geth_tx_timeout( struct net_device *dev ) 
{
	galois_eth_priv *priv = netdev_priv(dev);
	GALOIS_ETHSTAT(GALOIS_ETHSTAT_TX, priv->geth_stat.tx_timeout++);
}

#ifdef CONFIG_MV88DE3010_BERLIN_A0
static int geth_rx_pkt( struct net_device *dev )
{
    galois_eth_priv *priv = netdev_priv(dev);
    struct net_device_stats *stats = &(priv->stats);
    struct sk_buff *skb;
    MV_PKT_INFO pkt_info;
    int status;
    unsigned int queue = 0;
    unsigned int done_per_q[MV_ETH_RX_Q_NUM] = {0,};

    /* fairness NAPI loop */
	queue = ETH_DEF_RXQ;

    /* get rx packet */ 
	status = mvEthPortRx( &(priv->port_ctrl), queue, &pkt_info );

    /* check status */
	if( status == MV_RES_ETH_OK ) {
	   	done_per_q[queue]++;
	   	priv->rxq_count[queue]--;
	   	GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, (priv->geth_stat.rx_poll_hal_ok[queue]++) );
	} else {
		if( status == MV_RES_ETH_NORESOURCE ) {	/* no buffers for rx */
	   		stats->rx_errors++;
	   		GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
					(priv->geth_stat.rx_poll_hal_no_resource[queue]++) );
		} else if( status == MV_RES_ETH_RXBUSY ) {	/* no more rx packets ready */
	   		GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
						(priv->geth_stat.rx_poll_hal_no_more[queue]++) );
		} else {
	   		printk( KERN_ERR "%s: unrecognize status on rx poll\n", dev->name );
	   		stats->rx_errors++;
	   		GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
					(priv->geth_stat.rx_poll_hal_error[queue]++) );
		}
		goto rx_out;
	}
		
	/* validate skb */ 
	if( !(pkt_info.osInfo) ) {
    	printk( KERN_ERR "%s: error in rx\n",dev->name );
    	stats->rx_errors++;
    	GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
				(priv->geth_stat.rx_poll_hal_invalid_skb[queue]++) );
		goto rx_out;
	}

	skb = (struct sk_buff *)( pkt_info.osInfo );

	/* handle rx error */
	if( pkt_info.status & (ETH_ERROR_SUMMARY_MASK) ) {
            /* RX resource error is likely to happen when receiving packets, which are     */
            /* longer then the Rx buffer size, and they are spreading on multiple buffers. */
            /* Rx resource error - No descriptor in the middle of a frame.                 */
			if( pkt_info.status & ETH_RX_CRC_ERROR_MASK ) {
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("%s: bad rx status %08x, (CRC error)\n",
							dev->name, (unsigned int)pkt_info.status));
			} else if( pkt_info.status & ETH_RX_COLLISION_MASK ) {
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("%s: bad rx status %08x, (col error)\n",
							dev->name, (unsigned int)pkt_info.status));
        	} else if( pkt_info.status & ETH_RX_OVERRUN_MASK ){
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("%s: bad rx status %08x, (overrun error)\n",
							dev->name, (unsigned int)pkt_info.status));
			} else if( pkt_info.status & ETH_RX_BIG_FRAME_MASK ){
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, 
						("%s: bad rx status %08x, (too big frame error)\n",
						 dev->name, (unsigned int)pkt_info.status));
			} else if( pkt_info.status & ETH_RX_SMALL_FRAME_MASK ){
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, 
						("%s: bad rx status %08x, (short frame error)\n",
						 dev->name, (unsigned int)pkt_info.status));
			} else {
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("%s: bad rx status %08x, (unknown error)\n",
							dev->name, (unsigned int)pkt_info.status));
			}
			
			dev_kfree_skb( skb );
			stats->rx_errors++;
			GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
					(priv->geth_stat.rx_poll_hal_bad_stat[queue]++) );
			goto rx_out;
	}

	/* good rx */
	GALOIS_ETHDBG(GALOIS_ETHDBG_RX, ("good rx. skb=%p, skb->data=%p\n", skb, skb->data));
	stats->rx_packets++;
	stats->rx_bytes += pkt_info.pktSize; /* include 4B crc */

	/* Don't need to flush&invalidate skb->data, remember it! */
	prefetch( (void *)(skb->data) );
	skb->dev = dev;

	/* reduce 4B crc, 2B stuff */
	if (galois_2bstuff_enable)
		skb_put( skb, pkt_info.pktSize - 4 - 2);

	/* no checknum done by interface, system software should do it */ 
	skb->ip_summed = CHECKSUM_UNNECESSARY;
	skb->protocol = eth_type_trans(skb, dev); 

	GALOIS_ETHDBG( GALOIS_ETHDBG_RX, 
			("ethernet header skipped. skb=%p, skb->data=%p\n", skb, skb->data) );

	status = netif_rx( skb );
	GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
			if(status) (priv->geth_stat.rx_poll_netif_drop[queue]++) );

rx_out:	
    /* refill rx ring buf with new buffers */
    for (queue = 0; queue < MV_ETH_RX_Q_NUM; queue++) {
        if(done_per_q[queue] > 0) {
	        geth_rxbuf_fill( dev, queue, ETH_RXQ_DESC(queue) );
	    }
    }

    /* notify upper layer about more work to do */
    return 0;
}

#else

/*********************************************************** 
 *   NAPI rx polling method. deliver rx packets to linux   *
 *   core. refill new rx buffers. unmask rx interrupt only *
 *   if all packets were delivered.                        *
 ***********************************************************/
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
static int geth_rx_poll( struct net_device *dev, int *budget )
#else
static int geth_rx_poll(struct napi_struct *napi, int budget)
#endif
{
    galois_eth_priv *priv = container_of(napi, struct _galois_eth_priv, napi);
    struct net_device *dev = priv->netdev;
    struct net_device_stats *stats = &(priv->stats);
    struct sk_buff *skb;
    MV_PKT_INFO pkt_info;
    int work_to_do = budget;
    int work_done = 0;
    int status;
    unsigned long flags;
    unsigned int queue = 0;
    unsigned int done_per_q[MV_ETH_RX_Q_NUM] = {0,};

    GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("%s: rx_poll work_to_do %d\n", dev->name, work_to_do) );
    GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, (priv->geth_stat.rx_poll_events++) );

    /* fairness NAPI loop */
    while( work_done < work_to_do ) {
		queue = ETH_DEF_RXQ;

        /* get rx packet */ 
		status = mvEthPortRx( &(priv->port_ctrl), queue, &pkt_info );

        /* check status */
		if( status == MV_RES_ETH_OK ) {
	    	work_done++;
	    	done_per_q[queue]++;
	    	priv->rxq_count[queue]--;
	    	GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, (priv->geth_stat.rx_poll_hal_ok[queue]++) );
		} else {
			if( status == MV_RES_ETH_NORESOURCE ) {
	    		/* no buffers for rx */
	    		GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("%s: rx_poll no resource ", dev->name) );
	    		stats->rx_errors++;
	    		GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
						(priv->geth_stat.rx_poll_hal_no_resource[queue]++) );
			} else if( status == MV_RES_ETH_RXBUSY ) {
	    		/* no more rx packets ready */
	    		GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("%s: rx_poll no more ", dev->name) );
	    		GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
						(priv->geth_stat.rx_poll_hal_no_more[queue]++) );
			} else {
                printk( KERN_ERR "%s: unrecognize status on rx poll\n", dev->name );
	    		stats->rx_errors++;
	    		GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
						(priv->geth_stat.rx_poll_hal_error[queue]++) );
			}
			break;
		}

		/* validate skb */ 
		if( !(pkt_info.osInfo) ) {
	    	printk( KERN_ERR "%s: error in rx\n",dev->name );
	    	stats->rx_errors++;
	    	GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
					(priv->geth_stat.rx_poll_hal_invalid_skb[queue]++) );
	    	continue;
		}

		skb = (struct sk_buff *)( pkt_info.osInfo );

		/* handle rx error */
		if( pkt_info.status & (ETH_ERROR_SUMMARY_MASK) ) {
            /* RX resource error is likely to happen when receiving packets, which are     */
            /* longer then the Rx buffer size, and they are spreading on multiple buffers. */
            /* Rx resource error - No descriptor in the middle of a frame.                 */
			if( pkt_info.status & ETH_RX_CRC_ERROR_MASK ) {
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("%s: bad rx status %08x, (CRC error)\n",
							dev->name, (unsigned int)pkt_info.status));
			} else if( pkt_info.status & ETH_RX_COLLISION_MASK ) {
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("%s: bad rx status %08x, (col error)\n",
							dev->name, (unsigned int)pkt_info.status));
        	} else if( pkt_info.status & ETH_RX_OVERRUN_MASK ){
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("%s: bad rx status %08x, (overrun error)\n",
							dev->name, (unsigned int)pkt_info.status));
			} else if( pkt_info.status & ETH_RX_BIG_FRAME_MASK ){
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, 
						("%s: bad rx status %08x, (too big frame error)\n",
						 dev->name, (unsigned int)pkt_info.status));
			} else if( pkt_info.status & ETH_RX_SMALL_FRAME_MASK ){
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, 
						("%s: bad rx status %08x, (short frame error)\n",
						 dev->name, (unsigned int)pkt_info.status));
			} else {
				GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("%s: bad rx status %08x, (unknown error)\n",
							dev->name, (unsigned int)pkt_info.status));
			}

			dev_kfree_skb( skb );
			stats->rx_errors++;
			GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
					(priv->geth_stat.rx_poll_hal_bad_stat[queue]++) );
    			GA_REG_WORD32_WRITE(ETH_EICR, ~(ETH_RXQ_ERROR_MASK));
			continue;
		}

		/* good rx */
		GALOIS_ETHDBG(GALOIS_ETHDBG_RX, ("good rx. skb=%p, skb->data=%p\n", skb, skb->data));
		stats->rx_packets++;
		stats->rx_bytes += pkt_info.pktSize; /* include 4B crc */

		/* Don't need to flush&invalidate skb->data, remember it! */
		prefetch( (void *)(skb->data) );
		skb->dev = dev;

		/* reduce 4B crc, 2B stuff */
		if (galois_2bstuff_enable)
			skb_put( skb, pkt_info.pktSize - 4 - 2);

		/* no checknum done by interface, system software should do it */ 
		skb->ip_summed = CHECKSUM_UNNECESSARY;
		skb->protocol = eth_type_trans(skb, dev); 

		GALOIS_ETHDBG( GALOIS_ETHDBG_RX, 
				("ethernet header skipped. skb=%p, skb->data=%p\n", skb, skb->data) );

		status = netif_receive_skb( skb );
		GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, 
				if(status) (priv->geth_stat.rx_poll_netif_drop[queue]++) );
    		GA_REG_WORD32_WRITE(ETH_EICR, ~(ETH_RXQ_GOOD_MASK));
	} /* while( work_done < work_to_do ) */
	
	GALOIS_ETHDBG( GALOIS_ETHDBG_RX, 
			("\nwork_done %d (%d)", work_done, priv->rxq_count[queue]) );

    /* refill rx ring buf with new buffers */
    for (queue = 0; queue < MV_ETH_RX_Q_NUM; queue++) {
        if(done_per_q[queue] > 0) {
	        geth_rxbuf_fill( dev, queue, ETH_RXQ_DESC(queue) );
	    }
    }

    /* don't remove us from scheduling list if work_done is equal to work_to_do, */
    /* it means that there is *more to do*. leave the rx-interrupt masked.       */
    if( work_done < work_to_do ) {
	    /* need to be atomic */
	spin_lock_irqsave(&priv->lock, flags);
        __netif_rx_complete(dev, napi);
        priv->intr_mask = ETH_PICR_MASK;
        GA_REG_WORD32_WRITE(ETH_EIMR, ETH_PICR_MASK);
	spin_unlock_irqrestore(&priv->lock, flags);
        GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("unmask\n") );
        GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX, (priv->geth_stat.rx_poll_netif_complete++) );
    } else 
        GALOIS_ETHDBG( GALOIS_ETHDBG_RX, ("don't unmask\n") );

    /* notify upper layer about more work to do */
    return work_done;
}
#endif	/* CONFIG_MV88DE3010_BERLIN_A0 */

/*********************************************************** 
 *   fill new rx buffers to ring.                          *
 ***********************************************************/
static u32 geth_rxbuf_fill( struct net_device *dev, unsigned int queue, int total )
{
    galois_eth_priv *priv = netdev_priv(dev);
    MV_PKT_INFO pkt_info;
    MV_BUF_INFO bufInfo;
    struct sk_buff *skb;
    u32 count = 0, buf_size;
    int status;
    int alloc_skb_failed = 0;

    GALOIS_ETHDBG( GALOIS_ETHDBG_RX_FILL, ("%s: rx fill queue %d\n", dev->name, queue) );
    GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX_FILL, (priv->geth_stat.rxbuf_fill_events[queue]++) );

    while( total-- ) {

        /* allocate a buffer */
		/* 32(extra for cache prefetch) */ /* +8 to align on 8B */
		buf_size = RX_BUFFER_SIZE( dev->mtu, priv) + 32 + 8 ;

        skb = dev_alloc_skb( buf_size ); 
		if( !skb ) {
	    	GALOIS_ETHDBG( GALOIS_ETHDBG_RX_FILL, 
					("%s: rxbuf_fill cannot allocate skb\n", dev->name) );
	    	GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX_FILL, 
					(priv->geth_stat.rxbuf_fill_alloc_skb_fail[queue]++) );
			alloc_skb_failed = 1;
	    	break;
		}

		/* align the buffer on 8B */
		if( (unsigned long)(skb->data) & 0x7 ) {
	    	skb_reserve( skb, 8 - ((unsigned long)(skb->data) & 0x7) );
		}

		bufInfo.bufVirtPtr = skb->data;
		bufInfo.bufSize = RX_BUFFER_SIZE( dev->mtu, priv);
		pkt_info.osInfo = (unsigned long)skb;
		pkt_info.pFrags = &bufInfo;
		pkt_info.pktSize = RX_BUFFER_SIZE( dev->mtu, priv); /* how much to invalidate */

		/* skip on first 2B (HW header) */
		if (galois_2bstuff_enable)
			skb_reserve( skb, 2 );

		/* give the buffer to hal */
		status = mvEthPortRxDone( &(priv->port_ctrl), queue, &pkt_info );
	
		if( status == 0 /*OK*/ ) {
	    	count++;
	    	priv->rxq_count[queue]++;
	    	GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX_FILL, 
					(priv->geth_stat.rxbuf_fill_hal_ok[queue]++) );	    
		} else if( status == -0x14 /*FULL*/ ) {
	    	/* the ring is full */
	    	count++;
	    	priv->rxq_count[queue]++;
	    	GALOIS_ETHDBG( GALOIS_ETHDBG_RX_FILL, ("%s: rxq full\n", dev->name) );
	    	GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX_FILL, 
					(priv->geth_stat.rxbuf_fill_hal_full[queue]++) );
	    	if( priv->rxq_count[queue] != ETH_RXQ_DESC(queue))
				printk( KERN_ERR "%s Q %d: error in status fill (%d != %d)\n", 
						dev->name, queue, priv->rxq_count[queue], ETH_RXQ_DESC(queue));
	    	break;
		} else {
	    	printk( KERN_ERR "%s Q %d: error in rx-fill\n", dev->name, queue );
	    	GALOIS_ETHSTAT( GALOIS_ETHSTAT_RX_FILL, 
					(priv->geth_stat.rxbuf_fill_hal_error[queue]++) );
	    	break;
		}
    }

    /* if allocation failed and the number of rx buffers in the ring is less than */
    /* half of the ring size, then set a timer to try again later.                */
    if (alloc_skb_failed && (priv->rxq_count[queue] < (ETH_RXQ_DESC(queue)/2))) {
        if (priv->rxbuf_fill_flag == 0) {
	    	printk( KERN_INFO "%s Q %d: set rx timeout to allocate skb\n", dev->name, queue );
	    	priv->rxbuf_fill_timer.expires = jiffies + (HZ/10); /*100ms*/
	    	add_timer(&priv->rxbuf_fill_timer);
	    	priv->rxbuf_fill_flag = 1;
		}
    }

    GALOIS_ETHDBG( GALOIS_ETHDBG_RX_FILL, ("rx fill %d (total %d)", 
				count, priv->rxq_count[queue]) );
    
    return count;
}

/*********************************************************** 
 *   previous rx fill failed allocate skb. try now again.  *
 ***********************************************************/
static void geth_rxbuf_fill_on_timeout( unsigned long data ) 
{
    struct net_device *dev = (struct net_device *)data;
    galois_eth_priv *priv = netdev_priv(dev);
    unsigned int queue;

    GALOIS_ETHDBG(GALOIS_ETHDBG_RX_FILL, ("%s: rxbuf_fill_on_timeout", dev->name));
    GALOIS_ETHSTAT(GALOIS_ETHSTAT_RX_FILL, (priv->geth_stat.rxbuf_fill_timeout_events++));
   
    priv->rxbuf_fill_flag = 0;
    for (queue = 0; queue < MV_ETH_RX_Q_NUM; queue++)
    	geth_rxbuf_fill(dev, queue, ETH_RXQ_DESC(queue));
}

/*********************************************************** 
 *   serve rx-q0, tx-done-q0, phy/link state change.       *
 *   tx and phy are served in interrupt context.           *
 *   rx is scheduled out of interrupt context (NAPI poll)  *
 ***********************************************************/
static irqreturn_t geth_interrupt_handler(int irq , void *dev_id)
{
    struct net_device *dev = (struct net_device *)dev_id;
    galois_eth_priv *priv = netdev_priv(dev);
    u32 picr;
 
 	/* spin lock should not be used in ISR like this */
    spin_lock(&(priv->lock));
	
    GALOIS_ETHDBG( GALOIS_ETHDBG_INT, ("\n%s: isr, irq=%d ", dev->name, irq) );
    GALOIS_ETHSTAT( GALOIS_ETHSTAT_INT, (priv->geth_stat.int_total++) );

    /* read port interrupt cause register */
    picr = MV_MEMIO_LE32_READ(ETH_EICR);

    GALOIS_ETHDBG( GALOIS_ETHDBG_INT, ("[picr %08x]", picr) );
    if( !picr ) {
        GALOIS_ETHSTAT( GALOIS_ETHSTAT_INT, (priv->geth_stat.int_none_events++) );
		spin_unlock(&(priv->lock));
        return IRQ_NONE;
    }

    /* select relevant bits */
    picr &= priv->intr_mask;

    /* clear port interrupt cause register */
    GA_REG_WORD32_WRITE(ETH_EICR, ~picr);

    /* tx event */
 	if( picr & (ETH_TXQ_GOOD_MASK|ETH_TXQ_ERROR_MASK) ) {
        GALOIS_ETHSTAT( GALOIS_ETHSTAT_INT, (priv->geth_stat.int_tx_done_events++) );
		priv->txcause = picr & (ETH_TXQ_GOOD_MASK|ETH_TXQ_ERROR_MASK);
	    geth_tx_done( dev );
	}
 
    /* rx event */
    if( picr & (ETH_RXQ_GOOD_MASK|ETH_RXQ_ERROR_MASK) ) { 
        GALOIS_ETHSTAT( GALOIS_ETHSTAT_INT, (priv->geth_stat.int_rx_events++) );
       
#ifdef CONFIG_MV88DE3010_BERLIN_A0
		/*
		 * Because of A0 ethernet bug, we have to process the received packet 
		 * in the interrupt context.
		 */
		geth_rx_pkt(dev);
#else
        /* schedule the device to do the rx out of interrupt context (NAPI) */
        //if (netif_rx_schedule_prep(dev, &priv->napi)) {
		if (napi_schedule_prep(&priv->napi)) {
  		    priv->rxcause = picr & (ETH_RXQ_GOOD_MASK|ETH_RXQ_ERROR_MASK);
            /* mask rx-event */
            priv->intr_mask &= ~(ETH_RXQ_GOOD_MASK | ETH_RXQ_ERROR_MASK);
            GA_REG_WORD32_WRITE(ETH_EIMR, priv->intr_mask);
      
            /* schedule the work out of interrupt contxet */
            __netif_rx_schedule(dev, &priv->napi);
      	} else {
			if (netif_running(dev))
				printk("eth: driver bug! interrupt while in poll.\n");
        }
#endif
    }

    spin_unlock( &(priv->lock) );

#ifdef CONFIG_MV88DE3010_BERLIN_A0
	if (berlin_a0_stopqueue) {
		berlin_a0_stopqueue = 0;
		netif_wake_queue(dev);
	}
#endif

    return IRQ_HANDLED;
}

/*********************************************************** 
 *   return the device statistics.                         *
 *   print private statistics if compile flag set.         *
 ***********************************************************/
static struct net_device_stats* geth_get_stats( struct net_device *dev )
{
	galois_eth_priv *priv = netdev_priv(dev);
	return &priv->stats;
}

void insert_multicast_address(void* pPortHndl, unsigned char *pAddr)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pPortHndl;	
    unsigned int    macH;
    unsigned int    macL;

    macH =  (pAddr[0] << 8) | (pAddr[1]);
    macL =  (pAddr[2] << 24) | (pAddr[3] << 16) | (pAddr[4] << 8) | (pAddr[5]);
    
    /* add multicast address to filter table, and accept frames of this address */
    mvAddAddressTableEntry(pPortCtrl, macH, macL, 1/*receive*/, 0/*not skip*/);

    return;
}

/***********************************************************
 *   Add multicast addresses or set promiscuous mode.      *
 *   This function should have been but was not included   *
 *   by Marvell. -bbozarth                                 *
 ***********************************************************/ 
static void geth_set_multicast_list(struct net_device *dev) 
{
     galois_eth_priv *priv = netdev_priv(dev);
     struct dev_mc_list *curr_addr = dev->mc_list;
     int i;

     if (dev->flags & IFF_PROMISC){
         mvEthRxFilterModeSet(&(priv->port_ctrl), 1);
     } else if (dev->flags & IFF_ALLMULTI){
         mvEthRxFilterModeSet(&(priv->port_ctrl), 1);
     } else {
         mvEthRxFilterModeSet(&(priv->port_ctrl), 0);

         for (i=0; i<dev->mc_count; i++){
             insert_multicast_address(&(priv->port_ctrl), curr_addr->dmi_addr);
             curr_addr = curr_addr->next;
         }
     }
}

/*********************************************************** 
 *   stop port activity. set new addr in device and hw.    *
 *   restart port activity.                                *
 ***********************************************************/
static int geth_set_mac_addr_internals(struct net_device *dev, void *addr )
{
	galois_eth_priv *priv = netdev_priv(dev);

	/* set new addr in hw */
	if (mvEthMacAddrSet(&(priv->port_ctrl), addr, ETH_DEF_RXQ) != 0)
		return -1;
	else
		memcpy(dev->dev_addr, addr, MV_MAC_ADDR_SIZE);

	return 0;
}

static int geth_set_mac_address(struct net_device *dev, void *p)
{
	struct sockaddr *saddr = (struct sockaddr *)p;
	if (!netif_running(dev)) {
		if (geth_set_mac_addr_internals(dev, saddr->sa_data) == -1)
			return -1;
		else
			return 0;
	}

	if (geth_close(dev))
		return -1;

	if (geth_set_mac_addr_internals(dev, saddr->sa_data) == -1)
		return -1;

	if (geth_open(dev))
		return -1;

	return 0;
}

static int geth_change_mtu_internals(struct net_device *dev, int mtu)
{
    galois_eth_priv *priv = netdev_priv(dev);
    ETH_PORT_CTRL *pPortCtrl = &(priv->port_ctrl);
    int pcxrReg;
        	
   	if (mtu < ETH_MTU)
        mtu = ETH_MTU;
      
    /* set mtu in device and in hal sw structures */
    pPortCtrl->portConfig.maxRxPktSize = RX_BUFFER_SIZE(mtu, priv);

    /* set mru register */    
    pcxrReg = MV_MEMIO_LE32_READ(ETH_EPCXR);
    pcxrReg &= ~(ETH_MRU_ALL_MASK);
    pcxrReg |= mvEthMruGet(pPortCtrl->portConfig.maxRxPktSize);
    GA_REG_WORD32_WRITE(ETH_EPCXR, pcxrReg);
        
    /* update device's MTU */
    dev->mtu = mtu;

    return 0;    
}

/*********************************************************** 
 *   stop port activity. release skb from rings. set new   *
 *   mtu in device and hw. restart port activity and       *
 *   and fill rx-buiffers with size according to new mtu.  *
 ***********************************************************/
static int geth_change_mtu( struct net_device *dev, int mtu )
{
    if ((signed)(dev->mtu) == mtu)
    	return 0;
    	
    if (!netif_running(dev)) {
        geth_change_mtu_internals(dev, mtu);
        return 0;
    }

    if (geth_close(dev))
    	return -1;

    geth_change_mtu_internals(dev, mtu);

    if (geth_open(dev))
    	return -1;
    	 
    return 0;
}

/*********************************************************** 
 * string helpers for mac address setting                  *
 ***********************************************************/
static unsigned int str_to_hex( char ch ) 
{
    if ((ch >= '0') && (ch <= '9'))
        return (ch - '0');

    if ((ch >= 'a') && (ch <= 'f'))
		return (ch - 'a' + 10);

    if ((ch >= 'A') && (ch <= 'F'))
		return (ch - 'A' + 10);

    return 0;
}

static void convert_str_to_mac(char *source , char *dest) 
{
    dest[0] = (str_to_hex(source[0]) << 4) + str_to_hex(source[1]);
    dest[1] = (str_to_hex(source[2]) << 4) + str_to_hex(source[3]);
    dest[2] = (str_to_hex(source[4]) << 4) + str_to_hex(source[5]);
    dest[3] = (str_to_hex(source[6]) << 4) + str_to_hex(source[7]);
    dest[4] = (str_to_hex(source[8]) << 4) + str_to_hex(source[9]);
    dest[5] = (str_to_hex(source[10]) << 4) + str_to_hex(source[11]);
}

/***********************************************************************************
 ***  print port statistics
 ***********************************************************************************/
#define STAT_PER_Q(qnum,x) \
	for(queue = 0; queue < qnum; queue++) \
		printk("%10u ",x[queue]); \
	printk("\n");

void print_geth_stat(void)
{
#ifndef GALOIS_ETHSTATISTICS
	printk(" Error: eth is compiled without statistics support!! \n");
	return;
#else
	struct net_device *dev = galois_eth_dev;
	galois_eth_priv *priv = netdev_priv(dev);
	galois_eth_stat *stat = &(priv->geth_stat);
	unsigned int queue;

	printk("QUEUS:.........................");
	for (queue = 0; queue < MV_ETH_RX_Q_NUM; queue++) 
		printk( "%10d ",queue);
	printk("\n");

	if (geth_stat & GALOIS_ETHSTAT_INT) {
      printk( "\n====================================================\n" );
      printk( "%s: interrupt statistics", dev->name );
      printk( "\n-------------------------------\n" );
      printk( "int_total.....................%10u\n", stat->int_total );
      printk( "int_rx_events.................%10u\n", stat->int_rx_events );
      printk( "int_tx_done_events............%10u\n", stat->int_tx_done_events );
      printk( "int_phy_events................%10u\n", stat->int_phy_events );
      printk( "int_none_events...............%10u\n", stat->int_none_events );
  	}
  	if( geth_stat & GALOIS_ETHSTAT_RX ) {
      printk( "\n====================================================\n" );
      printk( "%s: rx statistics", dev->name );
      printk( "\n-------------------------------\n" );
      printk( "rx_poll_events................%10u\n", stat->rx_poll_events );
      printk( "rx_poll_hal_ok................");
	  STAT_PER_Q(MV_ETH_RX_Q_NUM, stat->rx_poll_hal_ok);
      printk( "rx_poll_hal_no_resource.......");
	  STAT_PER_Q(MV_ETH_RX_Q_NUM, stat->rx_poll_hal_no_resource );
      printk( "rx_poll_hal_no_more..........."); 
	  STAT_PER_Q(MV_ETH_RX_Q_NUM, stat->rx_poll_hal_no_more );
      printk( "rx_poll_hal_error............."); 
	  STAT_PER_Q(MV_ETH_RX_Q_NUM, stat->rx_poll_hal_error );
      printk( "rx_poll_hal_invalid_skb......."); 
	  STAT_PER_Q(MV_ETH_RX_Q_NUM, stat->rx_poll_hal_invalid_skb );
      printk( "rx_poll_hal_bad_stat.........."); 
	  STAT_PER_Q(MV_ETH_RX_Q_NUM, stat->rx_poll_hal_bad_stat );
      printk( "rx_poll_netif_drop............"); 
	  STAT_PER_Q(MV_ETH_RX_Q_NUM, stat->rx_poll_netif_drop );
      printk( "rx_poll_netif_complete........%10u\n",stat->rx_poll_netif_complete );
      printk( "Current Rx Cause is...........%10x\n",priv->rxcause);
  	}
	if( geth_stat & GALOIS_ETHSTAT_RX_FILL ) {
      printk( "\n====================================================\n" );
      printk( "%s: rx fill statistics", dev->name );
      printk( "\n-------------------------------\n" );
      printk( "rxbuf_fill_events................"); 
	  STAT_PER_Q(MV_ETH_RX_Q_NUM, stat->rxbuf_fill_events );
      printk( "rxbuf_fill_alloc_skb_fail........"); 
	  STAT_PER_Q(MV_ETH_RX_Q_NUM, stat->rxbuf_fill_alloc_skb_fail );
      printk( "rxbuf_fill_hal_ok................"); 
	  STAT_PER_Q(MV_ETH_RX_Q_NUM,stat->rxbuf_fill_hal_ok);
      printk( "rxbuf_fill_hal_full.............."); 
	  STAT_PER_Q(MV_ETH_RX_Q_NUM, stat->rxbuf_fill_hal_full );
      printk( "rxbuf_fill_hal_error............."); 
	  STAT_PER_Q(MV_ETH_RX_Q_NUM, stat->rxbuf_fill_hal_error );
      printk( "rxbuf_fill_timeout_events........%10u\n", stat->rxbuf_fill_timeout_events );
      printk( "rx buffer size................%10u\n",RX_BUFFER_SIZE(dev->mtu, priv));
	}
	if( geth_stat & GALOIS_ETHSTAT_TX ) {
      printk( "\n====================================================\n" );
      printk( "%s: tx statistics", dev->name );
      printk( "\n-------------------------------\n" );
      printk( "tx_events.....................%10u\n", stat->tx_events );
      printk( "tx_hal_ok.....................");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_hal_ok);
      printk( "tx_hal_no_resource............");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_hal_no_resource );
      printk( "tx_hal_no_error...............");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_hal_error );
      printk( "tx_hal_unrecognize............");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_hal_unrecognize );
      printk( "tx_netif_stop.................");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_netif_stop );
      printk( "tx_timeout....................%10u\n", stat->tx_timeout );
      printk( "Current Tx Cause is...........%10x\n",priv->txcause);
	}
	if( geth_stat & GALOIS_ETHSTAT_TX_DONE ) {
      printk( "\n====================================================\n" );
      printk( "%s: tx-done statistics", dev->name );
      printk( "\n-------------------------------\n" );
      printk( "tx_done_events................%10u\n", stat->tx_done_events );
      printk( "tx_done_hal_ok................");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_done_hal_ok);
      printk( "tx_done_hal_invalid_skb.......");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_done_hal_invalid_skb );
      printk( "tx_done_hal_bad_stat..........");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_done_hal_bad_stat );
      printk( "tx_done_hal_still_tx..........");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_done_hal_still_tx );
      printk( "tx_done_hal_no_more...........");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_done_hal_no_more );
      printk( "tx_done_hal_unrecognize.......");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_done_hal_unrecognize );
      printk( "tx_done_max...................");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_done_max );
      printk( "tx_done_min...................");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_done_min );
      printk( "tx_done_netif_wake............");
	  STAT_PER_Q(MV_ETH_TX_Q_NUM, stat->tx_done_netif_wake );
	}

	memset( stat, 0, sizeof(galois_eth_stat) );
#endif /*GALOIS_ETHSTATISTICS*/
}

/*
 * Read out @reg of PHY chip
 * NOTE: PHY's address @addr is always PHY_ADDR(0x01)
 */
static int geth_mdio_read(struct net_device *netdev, int addr, int reg)
{
	return mvEthPhyRead(reg);
}

/*
 * Write @data to @reg of PHY chip
 * NOTE: PHY's address @addr is always PHY_ADDR(0x01)
 */
static void geth_mdio_write(struct net_device *netdev, int addr, int reg, int data)
{
	mvEthPhyWrite(reg, data);
}

/*
 * Initialize MII library setting
 */
static void geth_phy_init(galois_eth_priv *priv)
{
	unsigned int value;
	unsigned int phy_id;
	//int retry_count;

	priv->mii.phy_id = PHY_ADDR;
	priv->mii.phy_id_mask = 0x1F;
	priv->mii.reg_num_mask = 0x1F;
	priv->mii.dev = priv->netdev;
	priv->mii.mdio_read = geth_mdio_read;
	priv->mii.mdio_write = geth_mdio_write;

	value = mvEthPhyRead(MII_PHYSID2);
	if ((value & 0xfff0) == 0x0e20) {
		/* configure Marvell fast ethernet phy */
		value = mvEthPhyRead(22);
		value &= 0xff00;
		value |= (0x00b5); /* LED2:link/act, LED1:act/blink, LED0:link */
		mvEthPhyWrite(22, value);
		printk(KERN_INFO "Marvell PHY, LED2:link/act, LED1:act, LED0:link.\n");
	} else {
		/* Disable 1000Base-T advertisement */
		value = mvEthPhyRead(MII_CTRL1000);
		value &= ((~ADVERTISE_1000HALF) & (~ADVERTISE_1000FULL));
		mvEthPhyWrite(MII_CTRL1000, value);
	}

	value = mvEthPhyRead(MII_ADVERTISE);
	value &= ~(ADVERTISE_100FULL | ADVERTISE_100HALF | ADVERTISE_10FULL | ADVERTISE_10HALF);
#ifdef CONFIG_MV88DE3010_BERLIN_A0	/* Enable 100Base-TX, 10Base-TX advertisement */
	value |= (ADVERTISE_10HALF | ADVERTISE_10FULL);
	//value |= (ADVERTISE_10HALF | ADVERTISE_10FULL | ADVERTISE_100HALF | ADVERTISE_100FULL);
#else
#ifdef CONFIG_MV88DE3010_ASIC		/* Enable 100Base-TX, 10Base-TX advertisement */
	value |= (ADVERTISE_100FULL | ADVERTISE_100HALF | ADVERTISE_10FULL | ADVERTISE_10HALF);
#else								/* Enable 10Base-TX advertisement only */
	value |= (ADVERTISE_10FULL | ADVERTISE_10HALF);
#endif
#endif
	mvEthPhyWrite(MII_ADVERTISE, value);
	
	printk(KERN_INFO "Setting ethernet to auto-negotiation mode.\n");
	mvEthSpeedDuplexSet(NULL, MV_ETH_SPEED_AN, MV_ETH_DUPLEX_AN);

	/* Sw-reset PHY to restart AN */
	phy_id = mvEthPhyRead(MII_PHYSID1);
	if (phy_id == MARVELL_PHY_ID1) {
		value = mvEthPhyRead(MII_BMCR);
		value |= BMCR_RESET;		/* SW-Reset PHY */
		mvEthPhyWrite(MII_BMCR, value);
	} else {
		value = mvEthPhyRead(MII_BMCR);
		value |= BMCR_ANRESTART;	/* restart AN */
		mvEthPhyWrite(MII_BMCR, value);
	}

#if 0
	/* wait for the PHY is link up */
	retry_count = 0;
	while (1) {
		if (mii_link_ok(&priv->mii)) {
			struct ethtool_cmd ecmd;
			mii_ethtool_gset(&priv->mii, &ecmd);
			printk(KERN_INFO "\n");
			printk(KERN_INFO "galois link up, %sMbps, %s-duplex\n",
					ecmd.speed == SPEED_100 ? "100" : "10",
					ecmd.duplex == DUPLEX_FULL ? "full" : "half");
			break;
		} else {
			printk(KERN_INFO ".");
			schedule_timeout_uninterruptible(100);
		}

		if (retry_count > 3) {
			printk(KERN_INFO "\n");
			printk(KERN_INFO "galois link down after retry %d times.\n", retry_count);
			break;
		}
		retry_count++;
	}
#endif

	return;
}

#ifdef CONFIG_NET_POLL_CONTROLLER
static void geth_netpoll(struct net_device *dev)
{
	galois_eth_priv *priv = netdev_priv(dev);
	GA_REG_WORD32_WRITE(ETH_EIMR, 0);
	geth_interrupt_handler(dev->irq, dev);
	GA_REG_WORD32_WRITE(ETH_EIMR, priv->intr_mask);
}
#endif

static void geth_watchdog(unsigned long data)
{
	struct net_device *dev = (struct net_device *)data;
	galois_eth_priv *priv = netdev_priv(dev);
	struct ethtool_cmd ecmd;

	mii_ethtool_gset(&priv->mii, &ecmd);
	if (mii_link_ok(&priv->mii) && !netif_carrier_ok(priv->netdev)) {
		printk(KERN_INFO "galois link up, %sMbps, %s-duplex\n",
				ecmd.speed == SPEED_100 ? "100" : "10",
				ecmd.duplex == DUPLEX_FULL ? "full" : "half");
	} else if (!mii_link_ok(&priv->mii) && netif_carrier_ok(priv->netdev)) {
		printk(KERN_INFO "galois link down\n");
	}
	mii_check_link(&priv->mii);

#ifdef CONFIG_MV88DE3010_BERLIN_A0
	mod_timer(&priv->watchdog, jiffies + 6*HZ); /* 5 seconds */
#else
	mod_timer(&priv->watchdog, jiffies + 2*HZ); /* x*HZ means x seconds */
#endif
}

/*
 * early parameter for setting up MAC address
 */
static int __init macaddr_setup(char *str)
{
	unsigned char *buf = str;
	int pos = 0;

	while (*buf != '\0') {
		if ((('0' <= *buf) && (*buf <= '9')) 
				|| (('a' <= *buf) && (*buf <= 'f'))
				|| (('A' <= *buf) && (*buf <= 'F'))) {
			galois_mac_addr[pos] = *buf;
			pos++;
			if (pos >= 12)
				break;
		}
		buf++;
	}

	printk("Galois ethernet MAC: ");
	for (pos = 0; pos < 12; pos++)
		printk("%c", galois_mac_addr[pos]);
	printk("\n");

	return 1;
}
__setup("macaddr=", macaddr_setup);

/*
 * platform specific functions
 */
static int mv_eth_probe(struct platform_device *pdev)
{
	struct resource *irq_res;
	struct net_device *dev = NULL;
	galois_eth_priv *priv = NULL;
	MV_ETH_PORT_INIT eth_init_struct;
	int ret = 0;

	/* get ethernet hardware resource */
	irq_res = platform_get_resource_byname(pdev, IORESOURCE_IRQ, "eth_irq");
	if (!irq_res) {
		printk(KERN_ERR "There's no irq resource for platform device.\n");
		return -ENODEV;
	}

	/* disable and clear all interrupts */
	mvEthInit();

	/* allocate net_device structure */
	dev = alloc_etherdev(sizeof(galois_eth_priv));
	if (!dev)
		return -ENOMEM;

	priv = netdev_priv(dev);

	/* init device methods */
	convert_str_to_mac(galois_mac_addr, dev->dev_addr);
	strcpy(dev->name, "eth0");
	dev->base_addr = 0; /* ? */
	dev->irq = irq_res->start;
	dev->open = geth_open;
	dev->stop = geth_close;
	dev->hard_start_xmit = geth_start_xmit;
#ifdef CONFIG_MV88DE3010_BERLIN_A0
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
	dev->poll = NULL;
#endif
#else
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
	dev->poll = geth_rx_poll;
#else
	netif_napi_add(dev, &priv->napi, geth_rx_poll, 64);
#endif /* Linux version > 2.6.24, we will use napi */
#endif /* CONFIG_MV88DE3010_BERLIN_A0 */
	dev->tx_timeout = geth_tx_timeout;
	dev->change_mtu = geth_change_mtu;
	dev->set_mac_address = geth_set_mac_address;
	dev->set_multicast_list = geth_set_multicast_list;
	dev->get_stats = geth_get_stats;
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
	dev->weight = 64;
#endif
	dev->watchdog_timeo = 5*HZ;
	dev->tx_queue_len = gethDescTxQ[ETH_DEF_TXQ];

	dev->features = NETIF_F_SG; /* ? */
#ifdef CONFIG_NET_POLL_CONTROLLER
	dev->poll_controller = geth_netpoll;
#endif

	/* init galois_eth_priv */
	spin_lock_init(&priv->lock);
	priv->netdev = dev;

	init_timer(&priv->rxbuf_fill_timer);
	priv->rxbuf_fill_timer.function = geth_rxbuf_fill_on_timeout;
	priv->rxbuf_fill_timer.data = (unsigned long)dev;
	priv->rxbuf_fill_flag = 0;

	init_timer(&priv->watchdog);
	priv->watchdog.function = geth_watchdog;
	priv->watchdog.data = (unsigned long)dev;

	/* init the hal */
	memcpy(eth_init_struct.macAddr, dev->dev_addr, MV_MAC_ADDR_SIZE);

	/* by default dev->mtu=1500 */
	eth_init_struct.maxRxPktSize = RX_BUFFER_SIZE(ETH_MTU, priv);
	eth_init_struct.rxDefQ = ETH_DEF_RXQ;
	memcpy(eth_init_struct.rxDescrNum, gethDescRxQ, sizeof(gethDescRxQ));
	memcpy(eth_init_struct.txDescrNum, gethDescTxQ, sizeof(gethDescTxQ));

	/* 
	 * initialize internal port contol structure, TX/RX descriptor rings and Hash table.
	 * disable rx and tx operations. mask interrupts
	 */
	if (mvEthPortInit(&(priv->port_ctrl), &eth_init_struct) != 0) {
		printk(KERN_ERR "%s: mvEthPortInit failed\n", dev->name);
		ret = -ENODEV;
		goto probe_error;
	}

	printk(KERN_INFO "Setting ethernet to 2B stuffing mode.\n");
	mvEthRxEnable2BStuff(&(priv->port_ctrl));
	galois_2bstuff_enable = 1;

	/* Initialize PHY */
	geth_phy_init(priv);

	/* register the device */
	if ((ret = register_netdev(dev))) {
		printk(KERN_ERR "%s: register failed\n", dev->name);
		goto probe_error;
	}
	platform_set_drvdata(pdev, dev);
	galois_eth_dev = dev;
	return 0;

probe_error:
	free_netdev(dev);
	return ret;
}

static int mv_eth_remove(struct platform_device *pdev)
{
	struct net_device *dev = platform_get_drvdata(pdev);
	galois_eth_priv *priv = netdev_priv(dev);

	platform_set_drvdata(pdev, NULL);

	/* 
	 * flush tasks, shutdown port, reset port
	 */
	flush_scheduled_work();
	if (geth_close(dev)) 
		return -1;

	unregister_netdev(dev);

	/* 
	 * free descriptor rings, free internal port control structure.
	 */
	mvEthPortFinish(&(priv->port_ctrl));

	/*
	*/
	free_netdev(dev);
	return 0;

}

#ifdef CONFIG_PM
static int mv_eth_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct net_device *dev = platform_get_drvdata(pdev);

	galois_printk("%s\n", __func__);

	if (!netif_running(dev))
		return 0;

	geth_close(dev);
	return 0;
}

static int mv_eth_resume(struct platform_device *pdev)
{
	struct net_device *dev = platform_get_drvdata(pdev);
	galois_eth_priv *priv = netdev_priv(dev);
	MV_ETH_PORT_INIT eth_init_struct;

	galois_printk("%s\n", __func__);

	/* disable and clear all interrupts */
	mvEthInit();

	/* init the hal */
	memcpy(eth_init_struct.macAddr, dev->dev_addr, MV_MAC_ADDR_SIZE);
	/* by default dev->mtu=1500 */
	eth_init_struct.maxRxPktSize = RX_BUFFER_SIZE(ETH_MTU, priv);
	eth_init_struct.rxDefQ = ETH_DEF_RXQ;
	memcpy(eth_init_struct.rxDescrNum, gethDescRxQ, sizeof(gethDescRxQ));
	memcpy(eth_init_struct.txDescrNum, gethDescTxQ, sizeof(gethDescTxQ));

	/* 
	 * initialize internal port contol structure, TX/RX descriptor rings and Hash table.
	 * disable rx and tx operations. mask interrupts
	 */
	if (mvEthPortInit(&(priv->port_ctrl), &eth_init_struct) != 0) {
		printk(KERN_ERR "%s: mvEthPortInit failed\n", dev->name);
		free_netdev(dev);
		return -ENODEV;
	}

	printk(KERN_INFO "Setting ethernet to 2B stuffing mode.\n");
	mvEthRxEnable2BStuff(&(priv->port_ctrl));

	/* Initialize PHY */
	geth_phy_init(priv);

	geth_open(dev);
	return 0;
}
#else
#define mv_eth_suspend NULL
#define mv_eth_resume NULL
#endif

static struct platform_driver mv_eth_driver = {
	.probe		= mv_eth_probe,
	.remove		= mv_eth_remove,
	.suspend	= mv_eth_suspend,
	.resume		= mv_eth_resume,
	.driver 	= {
		.name	= "mv88de3010_eth",
		//.multithread_probe = 1,
	},
};

static int __init geth_init_module(void) 
{
    printk( "[Galois]Init marvell Ethernet driver:\n" );
#ifdef GALOIS_ETHSTATISTICS
    printk( "  o Driver statistics enabled\n");
#endif
#ifdef GALOIS_ETHDEBUG
    printk( "  o Driver debug messages enabled\n");
#endif
    printk( "  o Initializing network interface eth0.\n" );

	return platform_driver_register(&mv_eth_driver);
}

static void __exit geth_exit_module(void) 
{
	printk(KERN_INFO "[Galois]Ethernet module unloading...");
	platform_driver_unregister(&mv_eth_driver);
}

module_init(geth_init_module);
module_exit(geth_exit_module);

