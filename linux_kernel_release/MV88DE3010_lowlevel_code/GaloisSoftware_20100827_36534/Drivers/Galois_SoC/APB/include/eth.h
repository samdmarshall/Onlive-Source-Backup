#ifndef __MV_ETH_H__
#define __MV_ETH_H__

#include "galois_cache_flush.h"
#include "Galois_memmap.h"
#include "galois_common.h"
#include "galois_type.h"
#include "galois_io.h"

/* Cache operations */
#define ETH_PACKET_CACHE_FLUSH_INV(pAddr, size) \
	CleanAndInvalidateDCacheRegion((int *)(pAddr), (size))
#define ETH_DESCR_FLUSH_INV(pPortCtrl, pDescr) \
	CleanAndInvalidateDCacheRegion((int *)(pDescr), CPU_D_CACHE_LINE_SIZE)
#define ETH_DESCR_INV_LINE(pPortCtrl, pDescr) \
	InvalidateDCacheRegion((int *)(pDescr), CPU_D_CACHE_LINE_SIZE)

/* MACRO to do Virtual address -> Physical address convertion */
#define ETH_VIRT_TO_PHY(pDesc)  mvOsIoVirtToPhy(NULL, pDesc)

/* MAC address length */
#define MV_MAC_ADDR_SIZE 6

/* PHY address */
#define PHY_ADDR 0x01

#define TX_RX_DISABLE_TIMEOUT       0x1000000
#define TX_FIFO_EMPTY_TIMEOUT       0x1000000
#define PORT_DISABLE_WAIT_TCLOCKS   5000

/* MACROs of TX/RX queue numbers */
#define MV_ETH_RX_Q_NUM 1
#define MV_ETH_TX_Q_NUM 1

/* Rx descriptor bufSize field: buffer is 64bit aligned, bit[2:0]=0 */
#define ETH_RX_BUFFER_MASK 0xFFFFFFF8

/* Size of a Tx/Rx descriptor used in chain list data structure */
#define ETH_RX_DESC_ALIGNED_SIZE	32
#define ETH_TX_DESC_ALIGNED_SIZE	32

/* An offest in Tx descriptors to store data for buffers less than 8 Bytes */
#define MIN_TX_BUFF_LOAD			8
#define TX_BUF_OFFSET_IN_DESC		(ETH_TX_DESC_ALIGNED_SIZE - MIN_TX_BUFF_LOAD)

/* Macros that save access to desc in order to find next desc pointer  */
#define RX_NEXT_DESC_PTR(pRxDescr, pQueueCtrl)	\
	((pRxDescr) == (pQueueCtrl)->pLastDescr) ?	\
	(ETH_RX_DESC*)((pQueueCtrl)->pFirstDescr) :	\
	(ETH_RX_DESC*)(((unsigned long)(pRxDescr)) + ETH_RX_DESC_ALIGNED_SIZE)

#define TX_NEXT_DESC_PTR(pTxDescr, pQueueCtrl)	\
	((pTxDescr) == (pQueueCtrl)->pLastDescr) ?	\
	(ETH_TX_DESC*)((pQueueCtrl)->pFirstDescr) :	\
	(ETH_TX_DESC*)(((unsigned long)(pTxDescr)) + ETH_TX_DESC_ALIGNED_SIZE)

#define MV_ALIGN_UP(number, align) \
	(((number) & ((align) - 1)) ? (((number) + (align)) & ~((align)-1)) : (number))

#undef MV_ETH_DEBUG
#ifdef MV_ETH_DEBUG
#define mvOsPrintf printf
#else
#define mvOsPrintf(fmt,args...)         
#endif

/* 
 * This structure contains information describing one of buffers 
 * (fragments) they are built Ethernet packet.
 */
typedef struct
{
	MV_U8*	    bufVirtPtr;	
	MV_U32		bufPhysAddr;	
    MV_U32   	bufSize; 	
} MV_BUF_INFO;

/* 
 * This structure contains information describing Ethernet packet. 
 * The packet can be divided for few buffers (fragments)
 */
typedef struct
{
    MV_U32		osInfo;   
    MV_BUF_INFO *pFrags; 
    MV_U32      status;  
    MV_U16      pktSize;  
    MV_U16      numFrags;
    MV_U32      fragIP;
} MV_PKT_INFO;

typedef enum
{
    MV_ETH_SPEED_AN,
    MV_ETH_SPEED_10,
    MV_ETH_SPEED_100

}MV_ETH_PORT_SPEED;

typedef enum
{
    MV_ETH_DUPLEX_AN,
    MV_ETH_DUPLEX_HALF,
    MV_ETH_DUPLEX_FULL
    
}MV_ETH_PORT_DUPLEX;

typedef enum
{
    MV_ETH_FC_AN,
    MV_ETH_FC_DISABLE,
    MV_ETH_FC_ENABLE
}MV_ETH_PORT_FC;

typedef enum
{
	MV_ETH_HASH_HALF_K,
	MV_ETH_HASH_8K
}MV_ETH_HASH_SIZE;

typedef enum
{
	MV_ETH_HASH_MODE_0,
	MV_ETH_HASH_MODE_1
}MV_ETH_HASH_MODE;

typedef enum _mvState
{
    MV_IDLE,               
    MV_ACTIVE,             
    MV_PAUSED,             
    MV_UNDEFINED_STATE
} MV_STATE;


/* Ethernet port specific infomation */
typedef struct
{
    unsigned char   macAddr[MV_MAC_ADDR_SIZE];
    int     maxRxPktSize;
    int     rxDefQ;
} MV_ETH_PORT_CFG;

typedef struct
{
    int     descrNum;
} MV_ETH_RX_Q_CFG;

typedef struct
{
    int     descrNum;
} MV_ETH_TX_Q_CFG;

typedef struct
{
    unsigned char macAddr[MV_MAC_ADDR_SIZE];
    int     maxRxPktSize;
    int     rxDefQ;
    int     txDescrNum[MV_ETH_TX_Q_NUM];
    int     rxDescrNum[MV_ETH_RX_Q_NUM];
} MV_ETH_PORT_INIT;

typedef struct
{
    int             	isLinkUp;
    MV_ETH_PORT_SPEED   speed;
    MV_ETH_PORT_DUPLEX  duplex;
    MV_ETH_PORT_FC      flowControl;
} MV_ETH_PORT_STATUS;

/*************************************
 * MAC hardware internal data structures
 ************************************/

/* Address Hash table information */
typedef struct
{
	int size;
	int mode;
	void *hash_entry;
	MV_BUF_INFO hashBuf;
} ETH_HASH_TABLE;

/* Queue specific information */
typedef struct 
{
    void*       pFirstDescr;
    void*       pLastDescr;
    void*       pCurrentDescr;
    void*       pUsedDescr;
    int         resource;
    MV_BUF_INFO descBuf;
} ETH_QUEUE_CTRL;

/* Ethernet port specific infomation */
typedef struct _ethPortCtrl
{
    int             portNo;
    ETH_QUEUE_CTRL  rxQueue[MV_ETH_RX_Q_NUM]; /* Rx ring resource  */
    ETH_QUEUE_CTRL  txQueue[MV_ETH_TX_Q_NUM]; /* Tx ring resource  */

    ETH_HASH_TABLE	hash_tbl;
	
    MV_ETH_PORT_CFG portConfig;
    MV_ETH_RX_Q_CFG rxQueueConfig[MV_ETH_RX_Q_NUM];
    MV_ETH_TX_Q_CFG txQueueConfig[MV_ETH_TX_Q_NUM];

    int        portState;
} ETH_PORT_CTRL; 

/* RX descriptor information */
typedef struct _ethRxDesc
{
    unsigned int      cmdSts     ;    /* Descriptor command status        */
    unsigned short    byteCnt    ;    /* Descriptor buffer byte count     */
    unsigned short    bufSize    ;    /* Buffer size                      */
    unsigned int      bufPtr     ;    /* Descriptor buffer pointer        */
    unsigned int      nextDescPtr;    /* Next descriptor pointer          */
    unsigned long     returnInfo ;    /* User resource return information */
} ETH_RX_DESC;

/* TX descriptor information */
typedef struct _ethTxDesc
{
    unsigned int      cmdSts     ;    /* Descriptor command status        */
    unsigned short    L4iChk     ;    /* CPU provided TCP Checksum        */
    unsigned short    byteCnt    ;    /* Descriptor buffer byte count     */
    unsigned int      bufPtr     ;    /* Descriptor buffer pointer        */
    unsigned int      nextDescPtr;    /* Next descriptor pointer          */
    unsigned long     returnInfo ;    /* User resource return information */
} ETH_TX_DESC;

/****************************************/
/*        Ethernet Unit Registers       */
/****************************************/
#define ETH_EPAR	(MEMMAP_ETHERNET_REG_BASE + 0x000)
#define ETH_ESMIR	(MEMMAP_ETHERNET_REG_BASE + 0x010)
#define ETH_EPCR	(MEMMAP_ETHERNET_REG_BASE + 0x400)
#define ETH_EPCXR	(MEMMAP_ETHERNET_REG_BASE + 0x408)
#define ETH_EPCMR	(MEMMAP_ETHERNET_REG_BASE + 0x410)
#define ETH_EPSR	(MEMMAP_ETHERNET_REG_BASE + 0x418)
#define ETH_ESPR	(MEMMAP_ETHERNET_REG_BASE + 0x420)
#define ETH_EHTPR	(MEMMAP_ETHERNET_REG_BASE + 0x428)
#define ETH_EFCSAL	(MEMMAP_ETHERNET_REG_BASE + 0x430)
#define ETH_EFCSAH	(MEMMAP_ETHERNET_REG_BASE + 0x438)
#define ETH_ESDCR	(MEMMAP_ETHERNET_REG_BASE + 0x440)
#define ETH_ESDCMR	(MEMMAP_ETHERNET_REG_BASE + 0x448)
#define ETH_EICR	(MEMMAP_ETHERNET_REG_BASE + 0x450)
#define ETH_EIMR	(MEMMAP_ETHERNET_REG_BASE + 0x458)
#define ETH_EDSCP2P0L	(MEMMAP_ETHERNET_REG_BASE + 0x460)
#define ETH_EDSCP2P0H	(MEMMAP_ETHERNET_REG_BASE + 0x464)
#define ETH_EDSCP2P1L	(MEMMAP_ETHERNET_REG_BASE + 0x468)
#define ETH_EDSCP2P1H	(MEMMAP_ETHERNET_REG_BASE + 0x46C)
#define ETH_EVPT2P	(MEMMAP_ETHERNET_REG_BASE + 0x470)
#define ETH_EFRDP0	(MEMMAP_ETHERNET_REG_BASE + 0x480)
#define ETH_EFRDP1	(MEMMAP_ETHERNET_REG_BASE + 0x484)
#define ETH_EFRDP2	(MEMMAP_ETHERNET_REG_BASE + 0x488)
#define ETH_EFRDP3	(MEMMAP_ETHERNET_REG_BASE + 0x48C)
#define ETH_ECRDP0	(MEMMAP_ETHERNET_REG_BASE + 0x4A0)
#define ETH_ECRDP1	(MEMMAP_ETHERNET_REG_BASE + 0x4A4)
#define ETH_ECRDP2	(MEMMAP_ETHERNET_REG_BASE + 0x4A8)
#define ETH_ECRDP3	(MEMMAP_ETHERNET_REG_BASE + 0x4AC)
#define ETH_ECTDP0	(MEMMAP_ETHERNET_REG_BASE + 0x4E0)
#define ETH_ECTDP1	(MEMMAP_ETHERNET_REG_BASE + 0x4E4)

/* MIB Counters register definitions */
#define ETH_MIB_GOOD_OCTETS_RECEIVED	(MEMMAP_ETHERNET_REG_BASE + 0x0500)
#define ETH_MIB_GOOD_OCTETS_SENT		(MEMMAP_ETHERNET_REG_BASE + 0x0504)
#define ETH_MIB_GOOD_FRAMES_RECEIVED	(MEMMAP_ETHERNET_REG_BASE + 0x0508)
#define ETH_MIB_GOOD_FRAMES_SENT		(MEMMAP_ETHERNET_REG_BASE + 0x050C)
#define ETH_MIB_TOTAL_OCTETS_RECEIVED	(MEMMAP_ETHERNET_REG_BASE + 0x0510)
#define ETH_MIB_TOTAL_FRAMES_RECEIVED	(MEMMAP_ETHERNET_REG_BASE + 0x0514)
#define ETH_MIB_BRDCAST_FRAMES_RECEIVED	(MEMMAP_ETHERNET_REG_BASE + 0x0518)
#define ETH_MIB_MULCAST_FRAMES_RECEIVED	(MEMMAP_ETHERNET_REG_BASE + 0x051C)
#define ETH_MIB_CRCERR_FRAMES_RECEIVED	(MEMMAP_ETHERNET_REG_BASE + 0x0520)
#define ETH_MIB_OVSIZE_FRAMES_RECEIVED	(MEMMAP_ETHERNET_REG_BASE + 0x0524)
#define ETH_MIB_FRG_FRAMES_RECEIVED		(MEMMAP_ETHERNET_REG_BASE + 0x0528)
#define ETH_MIB_JABBER_FRAMES_RECEIVED	(MEMMAP_ETHERNET_REG_BASE + 0x052C)
#define ETH_MIB_COL_FRAMES_RECEIVED		(MEMMAP_ETHERNET_REG_BASE + 0x0530)
#define ETH_MIB_LTCOL_FRAMES_RECEIVED	(MEMMAP_ETHERNET_REG_BASE + 0x0534)
#define ETH_MIB_FRAMES_64_OCTETS		(MEMMAP_ETHERNET_REG_BASE + 0x0538)
#define ETH_MIB_FRAMES_65_127_OCTETS	(MEMMAP_ETHERNET_REG_BASE + 0x053C)
#define ETH_MIB_FRAMES_128_255_OCTETS	(MEMMAP_ETHERNET_REG_BASE + 0x0540)
#define ETH_MIB_FRAMES_256_511_OCTETS	(MEMMAP_ETHERNET_REG_BASE + 0x0544)
#define ETH_MIB_FRAMES_512_1023_OCTETS	(MEMMAP_ETHERNET_REG_BASE + 0x0548)
#define ETH_MIB_FRAMES_1024_MAX_OCTETS	(MEMMAP_ETHERNET_REG_BASE + 0x054C)
#define ETH_MIB_RX_ERROR				(MEMMAP_ETHERNET_REG_BASE + 0x0550)
#define ETH_MIB_DROPPED_FRAMES			(MEMMAP_ETHERNET_REG_BASE + 0x0554)
#define ETH_MIB_MULCAST_FRAMES_SENT		(MEMMAP_ETHERNET_REG_BASE + 0x0558)
#define ETH_MIB_BRDCAST_FRAMES_SENT		(MEMMAP_ETHERNET_REG_BASE + 0x055C)
#define ETH_MIB_UNDSZ_FRAMES_RECEIVED	(MEMMAP_ETHERNET_REG_BASE + 0x0560)

/****************************************/
/*        Ethernet Unit Register BITs   */
/****************************************/

/***** BITs of Ethernet Port SMI reg (ETH_ESMIR) *****/
#define ETH_SMIR_PHYADDR_BIT	16
#define ETH_SMIR_PHYADDR_MASK   (0x1f<<ETH_SMIR_PHYADDR_BIT)
#define ETH_SMIR_PHYADDR_SET(a) ((a)<<ETH_SMIR_PHYADDR_BIT)

#define ETH_SMIR_REGADDR_BIT    21
#define ETH_SMIR_REGADDR_MASK   (0x1f<<ETH_SMIR_REGADDR_BIT)
#define ETH_SMIR_REGADDR_SET(a) ((a)<<ETH_SMIR_REGADDR_BIT)

#define ETH_SMIR_OP_READ_BIT    26
#define ETH_SMIR_OP_READ_MASK   (1<<ETH_SMIR_OP_READ_BIT)

#define ETH_SMIR_READ_VALID_BIT  27
#define ETH_SMIR_READ_VALID_MASK (1<<ETH_SMIR_READ_VALID_BIT)

#define ETH_SMIR_BUSY_BIT  28
#define ETH_SMIR_BUSY_MASK (1<<ETH_SMIR_BUSY_BIT)

/***** BITs of Port Configuration reg (ETH_EPCR) *****/
#define ETH_UNICAST_PROMISCUOUS_MODE_BIT    0
#define ETH_UNICAST_PROMISCUOUS_MODE_MASK   (1<<ETH_UNICAST_PROMISCUOUS_MODE_BIT)

#define ETH_BROADCAST_REJECT_MODE_BIT    	1
#define ETH_BROADCAST_REJECT_MODE_MASK   	(1<<ETH_BROADCAST_REJECT_MODE_BIT)

#define ETH_PORT_ENABLE_BIT					7
#define ETH_PORT_ENABLE_MASK				(1<<ETH_PORT_ENABLE_BIT)

#define ETH_HASH_SIZE_HALF_K_BIT			12
#define ETH_HASH_SIZE_HALF_K_MASK			(1<<ETH_HASH_SIZE_HALF_K_BIT)

#define ETH_HASH_FUNCTION_1_BIT				13
#define ETH_HASH_FUNCTION_1_MASK			(1<<ETH_HASH_FUNCTION_1_BIT)

#define ETH_HASH_PASS_MODE_BIT				14
#define ETH_HASH_PASS_MODE_MASK				(1<<ETH_HASH_PASS_MODE_BIT)

#define ETH_FULL_DUPLEX_MODE_BIT			15
#define ETH_FULL_DUPLEX_MODE_MASK			(1<<ETH_FULL_DUPLEX_MODE_BIT)

/***** BITs of Port Configuration Extend reg (ETH_EPCXR) *****/
#define ETH_CAPTURE_IGMP_ENABLE_BIT			0
#define ETH_CAPTURE_IGMP_ENABLE_MASK		(1<<ETH_CAPTURE_IGMP_ENABLE_BIT)

#define ETH_CAPTURE_SPAN_BPDU_ENABLE_BIT    1
#define ETH_CAPTURE_SPAN_BPDU_ENABLE_MASK   (1<<ETH_CAPTURE_SPAN_BPDU_ENABLE_BIT)

#define ETH_PARTITION_ENABLE_BIT            2
#define ETH_PARTITION_ENABLE_MASK           (1<<ETH_PARTITION_ENABLE_BIT)

#define ETH_TX_PRIO_WEIGHT_OFFSET			3
#define ETH_TX_PRIO_WEIGHT_ALL_MASK			(0x07<<ETH_TX_PRIO_WEIGHT_OFFSET)
#define ETH_TX_PRIO_WEIGHT_SET(weight)		((weight)<<ETH_TX_PRIO_WEIGHT_OFFSET)

#define ETH_DEF_RX_PRIO_OFFSET				6
#define ETH_DEF_RX_PRIO_ALL_MASK			(0x03<<ETH_DEF_RX_PRIO_OFFSET)
#define ETH_DEF_RX_PRIO_SET(prio)			((prio)<<ETH_DEF_RX_PRIO_OFFSET)

#define ETH_RX_PRIO_OVRIDE_BIT				8
#define ETH_RX_PRIO_OVRIDE_MASK				(1<<ETH_RX_PRIO_OVRIDE_BIT)

#define ETH_DUPLEX_AN_DISABLE_BIT			9
#define ETH_DUPLEX_AN_DISABLE_MASK			(1<<ETH_DUPLEX_AN_DISABLE_BIT)

#define ETH_FC_AN_DISABLE_BIT				10
#define ETH_FC_AN_DISABLE_MASK				(1<<ETH_FC_AN_DISABLE_BIT)

#define ETH_FLP_DISABLE_BIT					11
#define ETH_FLP_DISABLE_MASK				(1<<ETH_FLP_DISABLE_BIT)

#define ETH_FC_ENABLE_BIT					12
#define ETH_FC_ENABLE_MASK					(1<<ETH_FC_ENABLE_BIT)

#define ETH_MRU_OFFSET						14
#define ETH_MRU_ALL_MASK					(0x03<<ETH_MRU_OFFSET)
#define ETH_MRU_1518						(0<<ETH_MRU_OFFSET)
#define ETH_MRU_1536						(1<<ETH_MRU_OFFSET)
#define ETH_MRU_2048						(2<<ETH_MRU_OFFSET)
#define ETH_MRU_64K							(3<<ETH_MRU_OFFSET)

#define ETH_MIB_CLEAR_BIT					16
#define ETH_MIB_CLEAR_MASK					(1<<ETH_MIB_CLEAR_BIT)

#define ETH_SPEED_100_BIT					18
#define ETH_SPEED_100_MASK					(1<<ETH_SPEED_100_BIT)

#define ETH_SPEED_AN_DISABLE_BIT			19
#define ETH_SPEED_AN_DISABLE_MASK			(1<<ETH_SPEED_AN_DISABLE_BIT)

#define ETH_MAC_RX_2BSTUFF_BIT				28
#define ETH_MAC_RX_2BSTUFF_MASK				(1<<ETH_MAC_RX_2BSTUFF_BIT)

/***** BITs of Ethernet Port Status reg (ETH_EPSR) *****/
#define ETH_MII_SPEED_100_BIT               0
#define ETH_MII_SPEED_100_MASK              (1<<ETH_MII_SPEED_100_BIT)

#define ETH_FULL_DUPLEX_BIT                 1
#define ETH_FULL_DUPLEX_MASK                (1<<ETH_FULL_DUPLEX_BIT)

#define ETH_ENABLE_RCV_FLOW_CTRL_BIT        2
#define ETH_ENABLE_RCV_FLOW_CTRL_MASK       (1<<ETH_ENABLE_RCV_FLOW_CTRL_BIT)

#define ETH_LINK_UP_BIT                     3
#define ETH_LINK_UP_MASK                    (1<<ETH_LINK_UP_BIT)
                     
#define ETH_TXLOW_RUNNING_BIT				5
#define ETH_TXLOW_RUNNING_MASK				(1<<ETH_TXLOW_RUNNING_BIT)                     
                     
#define ETH_TXHIGH_RUNNING_BIT				6
#define ETH_TXHIGH_RUNNING_MASK				(1<<ETH_TXHIGH_RUNNING_BIT)                     
                     
#define ETH_TX_IN_PROGRESS_BIT              7
#define ETH_TX_IN_PROGRESS_MASK             (1<<ETH_TX_IN_PROGRESS_BIT)

/***** BITs of Port Sdma Configuration reg (SDCR) *****/
#define ETH_RX_LE_BIT             			6
#define ETH_RX_LE_MASK            			(1<<ETH_RX_LE_BIT)

#define ETH_TX_LE_BIT			            7
#define ETH_TX_LE_MASK            			(1<<ETH_TX_LE_BIT)

#define ETH_RX_FRAME_INTERRUPT_BIT          9
#define ETH_RX_FRAME_INTERRUPT_MASK         (1<<ETH_RX_FRAME_INTERRUPT_BIT)

#define ETH_BURST_SIZE_1_64BIT_VALUE        0   
#define ETH_BURST_SIZE_2_64BIT_VALUE        1
#define ETH_BURST_SIZE_4_64BIT_VALUE        2
#define ETH_BURST_SIZE_8_64BIT_VALUE        3

#define ETH_BURST_SIZE_OFFSET          		12
#define ETH_BURST_SIZE_ALL_MASK          	(0x3<<ETH_BURST_SIZE_OFFSET)
#define ETH_BURST_SIZE_MASK(burst)       	((burst)<<ETH_BURST_SIZE_OFFSET)

/***** BITs of Port Sdma command reg (SDCMR) *****/
#define ETH_RX_ENABLE_BIT             		7
#define ETH_RX_ENABLE_MASK            		(1<<ETH_RX_ENABLE_BIT)

#define ETH_RX_ABORT_BIT             		15
#define ETH_RX_ABORT_MASK            		(1<<ETH_RX_ABORT_BIT)

#define ETH_TX_START_OFFSET         		23
#define ETH_TX_START_QUEUE(q)				(1<<(ETH_TX_START_OFFSET+(1-(q))))

#define ETH_TX_ABORT_BIT             		31
#define ETH_TX_ABORT_MASK            		(1<<ETH_TX_ABORT_BIT)

/* Tx & Rx descriptor bits */
#define ETH_ERROR_SUMMARY_BIT               15
#define ETH_ERROR_SUMMARY_MASK              (1<<ETH_ERROR_SUMMARY_BIT)

#define ETH_LAST_DESC_BIT                	16
#define ETH_LAST_DESC_MASK               	(1<<ETH_LAST_DESC_BIT)

#define ETH_FIRST_DESC_BIT               	17
#define ETH_FIRST_DESC_MASK              	(1<<ETH_FIRST_DESC_BIT)

#define ETH_ENABLE_INTERRUPT_BIT         	23
#define ETH_ENABLE_INTERRUPT_MASK        	(1<<ETH_ENABLE_INTERRUPT_BIT)

#define ETH_BUFFER_OWNER_BIT                31             
#define ETH_BUFFER_OWNED_BY_DMA             (1<<ETH_BUFFER_OWNER_BIT)
#define ETH_BUFFER_OWNED_BY_HOST            (0<<ETH_BUFFER_OWNER_BIT)

/* Tx descriptor bits */
#define ETH_TX_ZERO_PADDING_BIT             18
#define ETH_TX_ZERO_PADDING_MASK            (1<<ETH_TX_ZERO_PADDING_BIT)

#define ETH_TX_GENERATE_CRC_BIT             22
#define ETH_TX_GENERATE_CRC_MASK            (1<<ETH_TX_GENERATE_CRC_BIT)

/* Rx descriptor bits */
#define ETH_RX_CRC_ERROR_BIT				0
#define ETH_RX_CRC_ERROR_MASK               (1<<ETH_RX_CRC_ERROR_BIT)
#define ETH_RX_COLLISION_BIT				4
#define ETH_RX_COLLISION_MASK               (1<<ETH_RX_COLLISION_BIT)
#define ETH_RX_OVERRUN_BIT					6
#define ETH_RX_OVERRUN_MASK               	(1<<ETH_RX_OVERRUN_BIT)
#define ETH_RX_BIG_FRAME_BIT				7
#define ETH_RX_BIG_FRAME_MASK               (1<<ETH_RX_BIG_FRAME_BIT)
#define ETH_RX_SMALL_FRAME_BIT				8
#define ETH_RX_SMALL_FRAME_MASK             (1<<ETH_RX_SMALL_FRAME_BIT)

/* Ethernet Cause Register BITs */
#define ETH_CAUSE_RX_OVERRUN_ERROR			(1<<12)

#define ETH_CAUSE_RX_READY_OFFSET           16
#define ETH_CAUSE_RX_READY_BIT(queue)       (ETH_CAUSE_RX_READY_OFFSET + (queue))
#define ETH_CAUSE_RX_READY_MASK(queue)      (1 << (ETH_CAUSE_RX_READY_BIT(queue))) 

#define ETH_CAUSE_RX_ERROR_OFFSET           20
#define ETH_CAUSE_RX_ERROR_BIT(queue)       (ETH_CAUSE_RX_ERROR_OFFSET + (queue))
#define ETH_CAUSE_RX_ERROR_MASK(queue)      (1 << (ETH_CAUSE_RX_ERROR_BIT(queue))) 

#define ETH_CAUSE_TX_UNDERRUN_ERROR			(1<<13)

#define ETH_CAUSE_TX_BUF_OFFSET             2
#define ETH_CAUSE_TX_BUF_BIT(queue)         (ETH_CAUSE_TX_BUF_OFFSET + (1-(queue)))
#define ETH_CAUSE_TX_BUF_MASK(queue)        (1 << (ETH_CAUSE_TX_BUF_BIT(queue)))
         
#define ETH_CAUSE_TX_ERROR_OFFSET           10
#define ETH_CAUSE_TX_ERROR_BIT(queue)       (ETH_CAUSE_TX_ERROR_OFFSET + (1-(queue)))
#define ETH_CAUSE_TX_ERROR_MASK(queue)      (1 << (ETH_CAUSE_TX_ERROR_BIT(queue)))

/* Address filter table entry BITs*/
#define ETH_HASH_TABLE_ENTRY_VALID_BIT		0
#define ETH_HASH_TABLE_ENTRY_VALID_MASK		(1 << ETH_HASH_TABLE_ENTRY_VALID_BIT)

#define ETH_HASH_TABLE_ENTRY_SKIP_BIT		1
#define ETH_HASH_TABLE_ENTRY_SKIP_MASK		(1 << ETH_HASH_TABLE_ENTRY_SKIP_BIT)

#define ETH_HASH_TABLE_ENTRY_RECEIVE_BIT	2
#define ETH_HASH_TABLE_ENTRY_RECEIVE_MASK	(1 << ETH_HASH_TABLE_ENTRY_RECEIVE_BIT)

#define ETH_HASH_TABLE_ENTRY_PRIO_OFFSET	19
#define ETH_HASH_TABLE_ENTRY_PRIO_SET(prio)	((prio) << ETH_HASH_TABLE_ENTRY_PRIO_OFFSET)

/*
 * Hash function macroes
 */
#define NIBBLE_SWAPPING_16_BIT(X)   \
        (((X&0xf) << 4) |     \
         ((X&0xf0) >> 4) |    \
         ((X&0xf00) << 4) |   \
         ((X&0xf000) >> 4))

#define NIBBLE_SWAPPING_32_BIT(X)   \
        (((X&0xf) << 4) |       \
         ((X&0xf0) >> 4) |      \
         ((X&0xf00) << 4) |     \
         ((X&0xf000) >> 4) |    \
         ((X&0xf0000) << 4) |   \
         ((X&0xf00000) >> 4) |  \
         ((X&0xf000000) << 4) | \
         ((X&0xf0000000) >> 4))

#define GT_NIBBLE(X)          \
        (((X&0x1) << 3 ) +    \
         ((X&0x2) << 1 ) +    \
         ((X&0x4) >> 1 ) +    \
         ((X&0x8) >> 3 ) )

#define FLIP_6_BITS(X)        \
        (((X&0x1) << 5 ) +    \
         ((X&0x2) << 3 ) +    \
         ((X&0x4) << 1 ) +    \
         ((X&0x8) >> 1 ) +    \
         ((X&0x10) >> 3) +    \
         ((X&0x20) >> 5) )

#define FLIP_9_BITS(X)        \
        (((X&0x1 ) << 8) +    \
         ((X&0x2 ) << 6) +    \
         ((X&0x4 ) << 4) +    \
         ((X&0x8 ) << 2) +    \
         ((X&0x10) << 0) +    \
         ((X&0x20) >> 2) +    \
         ((X&0x40) >> 4) +    \
         ((X&0x80) >> 6) +    \
         ((X&0x100) >> 8) )

/*
 * Marvell Ethnernet APIs
 */
void  mvEthInit(void);
unsigned int mvEthMruGet(int maxRxPktSize);

void  ethInitTxDescRing(void *pPortHndl, int queue);
void  ethResetTxDescRing(void* pPortHndl, int queue);
void  ethInitRxDescRing(void *pPortHndl, int queue);
void  ethResetRxDescRing(void* pPortHndl, int queue);

/* Port Initalization routines */
int   mvEthHwReset(void* pEthPortHndl);
int   mvEthPortInit (ETH_PORT_CTRL *pPortCtrl, MV_ETH_PORT_INIT *pPortInit);

void  mvEthPortFinish(void* pEthPortHndl);
int   mvEthPortDown(void* pEthPortHndl);
int  mvEthPortDisable(void* pEthPortHndl);
int   mvEthPortUp(void* pEthPortHndl);
int   mvEthPortEnable(void* pEthPortHndl);

/* Port data flow routines */
int   mvEthPortTx(void* pEthPortHndl, int txQueue, MV_PKT_INFO *pPktInfo);
int   mvEthPortTxDone(void* pEthPortHndl, int txQueue, MV_PKT_INFO *pPktInfo);
int   mvEthPortForceTxDone(void* pEthPortHndl, int txQueue, MV_PKT_INFO *pPktInfo);

int   mvEthPortRx(void* pEthPortHndl, int rxQueue, MV_PKT_INFO *pPktInfo);
int   mvEthPortRxDone(void* pEthPortHndl, int rxQueue, MV_PKT_INFO *pPktInfo);
int   mvEthPortForceRx(void* pEthPortHndl, int rxQueue, MV_PKT_INFO *pPktInfo);

/* Port RX MAC Filtering control routines */
int   mvEthMacAddrSet(void* pPortHandle, unsigned char *pMacAddr, int queue);
int   mvEthMacAddrGet(unsigned char *pAddr);
int   mvEthRxFilterModeSet(void* pPortHndl, int isPromisc);
void  mvEthResetAddrTbl(void* pPortHndl);
int   mvEthInitAddrTbl(void* pPortHndl, int hashMode, int hashSize);
int   mvAddAddressTableEntry(void* pEthPortHndl, unsigned int macH, unsigned int macL, unsigned int rd, unsigned int skip);

/* Port status APIs */ 
int  mvEthRxResourceGet(void* pPortHndl, int rxQueue);
int  mvEthTxResourceGet(void* pPortHndl, int txQueue);

/* MIB Counters APIs */
unsigned int    mvEthMibCounterRead(void* pPortHndl, unsigned int mibOffset);
void        	mvEthMibCountersClear(void* pPortHandle);

/* TX Scheduling configuration routines */
int   mvEthTxQueueWeightConfig(void* pPortHandle, int txWeight);

/* Rx 2B stuffing mode */
void mvEthRxEnable2BStuff(void* pPortHandle);
void mvEthRxDisable2BStuff(void* pPortHandle);

/* Speed, Duplex, FlowControl routines */
int   mvEthSpeedDuplexSet(void* pPortHandle, 
		MV_ETH_PORT_SPEED speed, MV_ETH_PORT_DUPLEX duplex);

int   mvEthFlowCtrlSet(void* pPortHandle, MV_ETH_PORT_FC flowControl);

void  mvEthStatusGet(void* pPortHandle, MV_ETH_PORT_STATUS* pStatus);

/* PHY routines */
void       mvEthPhyAddrSet(void* pPortHandle, int phyAddr);
int        mvEthPhyAddrGet(void* pPortHandle);
unsigned int mvEthPhyRead(unsigned int addr);
void mvEthPhyWrite(unsigned int addr, unsigned int data);

/*
 * Ethernet API returns
 */
#define MV_RES_ETH_NORESOURCE -0x15
#define MV_RES_ETH_NORESOURCE_ -0x13
#define MV_RES_ETH_FULL -0x14
#define MV_RES_ETH_RXBUSY -0x0C
#define MV_RES_ETH_TXBUSY -0x0B
#define MV_RES_ETH_NOTREADY -0x1a
#define MV_RES_ETH_BADSTATE -7
#define MV_RES_ETH_BADPARAM -4
#define MV_RES_ETH_ERROR -3
#define MV_RES_ETH_FAILED -1
#define MV_RES_ETH_OK 0

/*
 * PHY chip control macroes
 */
#define ETHPHY_CONTROL	0x0
#define ETHPHY_STATUS	0x1
#define ETHPHY_ID1		0x2
#define ETHPHY_ID2		0x3

#define MARVELL_PHY_ID1		0x0141
#define NATIONAL_PHY_ID1	0x2000

#endif /* __MV_ETH_H__ */

