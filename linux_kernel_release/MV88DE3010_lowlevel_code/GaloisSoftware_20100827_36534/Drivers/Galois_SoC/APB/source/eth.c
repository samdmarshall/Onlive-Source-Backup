/*******************************************************************************
* mvEth.c - Marvell's Ethernet controller low level driver
*
* DESCRIPTION:
*       This file introduce OS independent APIs to Marvell's Ethernet
*       controller. This Ethernet Controller driver API controls
*       1) Operations (i.e. port Init, Finish, Up, Down, PhyReset etc').
*       2) Data flow (i.e. port Send, Receive etc').
*       3) MAC Filtering functions (ethSetMcastAddr, ethSetRxFilterMode, etc.)
*       4) MIB counters support (ethReadMibCounter)
*       5) Debug functions (ethPortRegs, ethPortCounters, ethPortQueues, etc.)
*       Each Ethernet port is controlled via ETH_PORT_CTRL struct.
*       This struct includes configuration information as well as driver
*       internal data needed for its operations.
*
*       Supported Features:
*       - OS independent. All required OS services are implemented via external 
*       OS dependent components 
*       - The user is free from Rx/Tx queue managing.
*       - Simple Ethernet port operation API.
*       - Simple Ethernet port data flow API.
*       - Data flow and operation API support per queue functionality.
*       - Support cached descriptors for better performance.
*       - PHY access and control API.
*       - Port Configuration API.
*       - Full control over Special and Other Multicast MAC tables.
*
*******************************************************************************/
#include "galois_common.h"
#include "galois_io.h"
#include "mv_os_header.h"
#include "eth.h"

/*******************************************************************************
* ethAllocDescrMemory - Alloc memory for RX and TX descriptors.
* INPUT:
*       int size - size of memory should be allocated.
* RETURN: None
*******************************************************************************/
static unsigned char *ethAllocDescrMemory(int descSize, unsigned long *pPhysAddr)
{
    unsigned char *pVirt;
    pVirt = (char*)mvOsIoCachedMalloc(NULL, descSize, pPhysAddr);
    return pVirt;
}

/*******************************************************************************
* ethFreeDescrMemory - Free memory allocated for RX and TX descriptors.
* INPUT:
*       void* pVirtAddr - virtual pointer to memory allocated for RX and TX
*                       desriptors.
* RETURN: None
*******************************************************************************/
static void ethFreeDescrMemory(MV_BUF_INFO* pDescBuf)
{
    if( (pDescBuf == NULL) || (pDescBuf->bufVirtPtr == NULL) )
        return;
    mvOsIoCachedFree(NULL, pDescBuf->bufSize, pDescBuf->bufPhysAddr, pDescBuf->bufVirtPtr);

    return;
}

/*******************************************************************************
* ethBCopy - Copy bytes from source to destination
* DESCRIPTION:
*       This function supports the eight bytes limitation on Tx buffer size.
*       The routine will zero eight bytes starting from the destination address
*       followed by copying bytes from the source address to the destination.
* INPUT:
*       unsigned int srcAddr    32 bit source address.
*       unsigned int dstAddr    32 bit destination address.
*       int        byteCount    Number of bytes to copy.
* OUTPUT:
*       See description.
* RETURN:
*       None.
*******************************************************************************/
static void ethBCopy(char* srcAddr, char* dstAddr, int byteCount)
{
    while (byteCount != 0)
    {
        *dstAddr = *srcAddr;
        dstAddr++;
        srcAddr++;
        byteCount--;
    }
	return;
}

/*******************************************************************************
* mvEthMruGet - Get MRU configuration for Max Rx packet size.
*
* INPUT:
*           MV_U32 maxRxPktSize - max  packet size.
*
* RETURN:   MV_U32 - MRU configuration.
*
*******************************************************************************/
unsigned int mvEthMruGet(int maxRxPktSize)
{
    unsigned int pcxrReg;

    if(maxRxPktSize > 2048)
        pcxrReg = ETH_MRU_64K;
    else if(maxRxPktSize > 1536)
        pcxrReg = ETH_MRU_2048;
    else if(maxRxPktSize > 1518)
        pcxrReg = ETH_MRU_1536;
    else
        pcxrReg = ETH_MRU_1518;

    return pcxrReg;
}

/*******************************************************************************
* mvEthInit - Initialize the Ethernet unit
* DESCRIPTION:
*       This function initialize the Ethernet unit.
*       1) Clear and Disable interrupts
* INPUT:  NONE
* RETURN: NONE
*******************************************************************************/
void mvEthInit(void)
{
    /* Disable Ethernet interrupts and clear interrupts */
    GA_REG_WORD32_WRITE(ETH_EIMR, 0);
    GA_REG_WORD32_WRITE(ETH_EICR, 0);

    return;
}

/*******************************************************************************
* mvEthPortInit - Initialize the Ethernet port driver
* DESCRIPTION:
*       This function initialize the ethernet port.
*       1) Allocate and initialize internal port Control structure.
*       2) Create RX and TX descriptor rings for default RX and TX queues
*       3) Disable RX and TX operations, clear cause registers and 
*          mask all interrupts.
*       4) Set all registers to default values and clean all MAC tables. 
* INPUT:
*       int             portNo          - Ethernet port number
*       ETH_PORT_INIT   *pEthPortInit   - Ethernet port init structure
* RETURN:
*       void* - ethernet port handler, that should be passed to the most other
*               functions dealing with this port.
* NOTE: This function is called once per port when loading the eth module.
*******************************************************************************/
int mvEthPortInit(ETH_PORT_CTRL *pPortCtrl, MV_ETH_PORT_INIT *pEthPortInit)
{
    int queue, descSize;

    /* Check validity of parameters */
    if((pEthPortInit->rxDefQ >= MV_ETH_RX_Q_NUM) || (pEthPortInit->maxRxPktSize < 1518))
    {
        mvOsPrintf("EthPort: Bad initialization parameters\n");
        return MV_RES_ETH_BADPARAM; /* bad parameter */
    }
    
    if((pEthPortInit->rxDescrNum[pEthPortInit->rxDefQ]) == 0)
    {
        mvOsPrintf("EthPort: rxDefQ (%d) must be created\n", pEthPortInit->rxDefQ);
        return MV_RES_ETH_BADPARAM; /* bad parameter */
    }
        
    mvOsMemset((unsigned char *)pPortCtrl, 0, (int)sizeof(ETH_PORT_CTRL));
    pPortCtrl->portState = MV_UNDEFINED_STATE;
    pPortCtrl->portNo = 0;

    /* Copy Configuration parameters */
    mvOsMemcpy((unsigned char *)(pPortCtrl->portConfig.macAddr), pEthPortInit->macAddr, 
			MV_MAC_ADDR_SIZE);
    pPortCtrl->portConfig.maxRxPktSize = pEthPortInit->maxRxPktSize;
    pPortCtrl->portConfig.rxDefQ = pEthPortInit->rxDefQ;

    for( queue=0; queue<MV_ETH_RX_Q_NUM; queue++ )
    	pPortCtrl->rxQueueConfig[queue].descrNum = pEthPortInit->rxDescrNum[queue];

    for( queue=0; queue<MV_ETH_TX_Q_NUM; queue++ )
	    pPortCtrl->txQueueConfig[queue].descrNum = pEthPortInit->txDescrNum[queue];

    /* 
	 * Hardware reset: 
	 * disable/clear interrupts, disable port TX/RX, 
	 * set default value for some registers, set PHY addr
	 */
    mvEthHwReset(pPortCtrl);

    /* Create all requested RX queues */
    for(queue=0; queue<MV_ETH_RX_Q_NUM; queue++)
    {
        if(pPortCtrl->rxQueueConfig[queue].descrNum == 0)
            continue;

        /* Allocate memory for RX descriptors */
        descSize = ((pPortCtrl->rxQueueConfig[queue].descrNum * ETH_RX_DESC_ALIGNED_SIZE) 
				+ CPU_D_CACHE_LINE_SIZE);
 
        pPortCtrl->rxQueue[queue].descBuf.bufVirtPtr = 
			ethAllocDescrMemory(descSize, &pPortCtrl->rxQueue[queue].descBuf.bufPhysAddr);
        pPortCtrl->rxQueue[queue].descBuf.bufSize = descSize;
        if (pPortCtrl->rxQueue[queue].descBuf.bufVirtPtr == NULL)
        {
            mvOsPrintf("EthPort, rxQ=%d: Can't allocate %d bytes for %d RX descr\n", 
					queue, descSize, pPortCtrl->rxQueueConfig[queue].descrNum);
            return MV_RES_ETH_FAILED; /* port init fail */
        }

		/* link and initialize RX descriptors */
        ethInitRxDescRing(pPortCtrl, queue);
    }
    
    /* Create TX queues */
    for(queue=0; queue<MV_ETH_TX_Q_NUM; queue++)
    {
        if(pPortCtrl->txQueueConfig[queue].descrNum == 0)
            continue;

        /* Allocate memory for RX descriptors */
        descSize = ((pPortCtrl->txQueueConfig[queue].descrNum * ETH_TX_DESC_ALIGNED_SIZE) 
				+ CPU_D_CACHE_LINE_SIZE);
 
        pPortCtrl->txQueue[queue].descBuf.bufVirtPtr = 
			ethAllocDescrMemory(descSize, &pPortCtrl->txQueue[queue].descBuf.bufPhysAddr);
        pPortCtrl->txQueue[queue].descBuf.bufSize = descSize;
        if(pPortCtrl->txQueue[queue].descBuf.bufVirtPtr == NULL)
        {
            mvOsPrintf("EthPort, txQ=%d: Can't allocate %d bytes for %d TX descr\n", 
					queue, descSize, pPortCtrl->txQueueConfig[queue].descrNum);
            return MV_RES_ETH_FAILED; /* port init fail */
        }

		/* link and initialize TX descriptors */
        ethInitTxDescRing(pPortCtrl, queue);
    }
    
    /* 
	 * Create Address Hash table
	 */
    if (mvEthInitAddrTbl(pPortCtrl, MV_ETH_HASH_MODE_0, MV_ETH_HASH_HALF_K) != 0){
        mvOsPrintf("EthPort: cann't create Address Hash table!\n");
        return MV_RES_ETH_FAILED; /* port init fail */
    }
    
    /* set MAC source address register, add MAC source address into hash table */
    mvEthMacAddrSet(pPortCtrl, pPortCtrl->portConfig.macAddr, 0);

    pPortCtrl->portState = MV_IDLE;
    
    return MV_RES_ETH_OK;
}

/*******************************************************************************
* ethPortFinish - Finish the Ethernet port driver(exit)
* DESCRIPTION:
*       This function finish the ethernet port.
*       1) Down ethernet port if needed.
*       2) Delete RX and TX descriptor rings for all created RX and TX queues
*       3) Free internal port Control structure.
* INPUT:
*       void*   pEthPortHndl  - Ethernet port handler
* RETURN: NONE.
*
*******************************************************************************/
void mvEthPortFinish(void* pPortHndl)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pPortHndl;
    int             queue;

    if (pPortCtrl->portState == MV_ACTIVE)
    {
        mvEthPortDisable(pPortHndl);
    }

    /* Free all allocated RX queues */
    for (queue=0; queue<MV_ETH_RX_Q_NUM; queue++)
    {
        ethFreeDescrMemory(&pPortCtrl->rxQueue[queue].descBuf);
    }

    /* Free all allocated TX queues */
    for (queue=0; queue<MV_ETH_TX_Q_NUM; queue++)
    {
        ethFreeDescrMemory(&pPortCtrl->txQueue[queue].descBuf);
    }

    /* Free Hash table */
    ethFreeDescrMemory(&pPortCtrl->hash_tbl.hashBuf);
	return;
}

/*******************************************************************************
* mvEthHwReset - Set defaults to the ethernet port
* DESCRIPTION:
*       This function set default values to the ethernet port.
*       1) Clear Cause registers and Mask all interrupts
*       2) disable port
*       3) Set defaults to all registers
*       4) set PHY address
* INPUT:
*       void*   pEthPortHndl  - Ethernet port handler
* RETURN:   MV_STATUS  
*               MV_OK - Success, Others - Failure
* NOTE:
*   This function update all the port configuration except those set
*   Initialy by the OsGlue by MV_ETH_PORT_INIT.
*   This function can be called after portDown to return the port setting 
*   to defaults.
*******************************************************************************/
int mvEthHwReset(void* pPortHndl)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pPortHndl;

    /* Mask all interrupts */
    GA_REG_WORD32_WRITE(ETH_EIMR, 0);
    /* Clear Cause registers */
    GA_REG_WORD32_WRITE(ETH_EICR, 0);
    
    /* disable port TX/RX */
    mvEthPortDisable(pPortCtrl);

    /* Set PHY address */
    mvEthPhyAddrSet(pPortCtrl, PHY_ADDR);
    
    /* set default value for port config register */
    GA_REG_WORD32_WRITE(ETH_EPCR, 0);
    
    /* 
	 * SDMA config register 
	 * burst limit 8*64bits, enable RX interrupt at frame boundary, 
	 * little endian rx/tx to/from memory, maximum 16 retransmits
	 */
    GA_REG_WORD32_WRITE(ETH_ESDCR, 
			ETH_BURST_SIZE_MASK(ETH_BURST_SIZE_8_64BIT_VALUE) | 
			ETH_RX_FRAME_INTERRUPT_MASK | ETH_RX_LE_MASK | ETH_TX_LE_MASK | 0x3c);
    
    /* 
	 * Port config extra register: 
	 * Disable auto-negotiation for flow control, don't Force Link Pass
	 */
    GA_REG_WORD32_WRITE(ETH_EPCXR, 
			mvEthMruGet(pPortCtrl->portConfig.maxRxPktSize) | 
			ETH_FC_AN_DISABLE_MASK | ETH_FLP_DISABLE_MASK);

#if 1
	/* Disable auto-negotiation for flow control, enable flow control mode */
	mvEthFlowCtrlSet(pPortHndl, MV_ETH_FC_ENABLE);
#endif

	/* By default, packets are rx into lowest-priority RX queue.(PCXR[7:6]=00) */
    /* set TX queue priority weight - 7: all from highest queue */
    return mvEthTxQueueWeightConfig(pPortCtrl, 0);
}

/*******************************************************************************
* ethPortUp - Start the Ethernet port RX and TX activity.
* DESCRIPTION:
*       This routine start Rx and Tx activity:
*       Note: Each Rx and Tx queue descriptor's list must be initialized prior
*       to calling this function (use etherInitTxDescRing for Tx queues and
*       etherInitRxDescRing for Rx queues).
* INPUT:
*       void*   pEthPortHndl  - Ethernet port handler
* RETURN:   MV_STATUS
*           MV_OK - Success, Others - Failure.
* NOTE : used for port link up.
*******************************************************************************/
int mvEthPortUp(void* pEthPortHndl)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pEthPortHndl;

    if ((pPortCtrl->portState != MV_ACTIVE) && (pPortCtrl->portState != MV_PAUSED))
    {
        mvOsPrintf("ethDrv port: Unexpected port state %d\n", pPortCtrl->portState);
        return MV_RES_ETH_BADSTATE; /* bad state */
    }
    
    /* Enable port RX. */
    GA_REG_WORD32_WRITE(ETH_ESDCMR, ETH_RX_ENABLE_MASK);
    pPortCtrl->portState = MV_ACTIVE;

    return MV_RES_ETH_OK;
}

/*******************************************************************************
* ethPortDown - Stop the Ethernet port activity.
* DESCRIPTION:
* INPUT:
*       void*   pEthPortHndl  - Ethernet port handler
* RETURN:   MV_STATUS
*       MV_OK - Success, Others - Failure.
* NOTE : used for port link down.
*******************************************************************************/
int mvEthPortDown(void* pEthPortHndl)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pEthPortHndl;
    volatile int    regData;
    volatile int    txrxTimeout, txFifoTimeout;

    /* Abort Rx/Tx port activity. */
    GA_REG_WORD32_WRITE(ETH_ESDCMR, ETH_RX_ABORT_MASK | ETH_TX_ABORT_MASK);

    /* Wait for all Rx/TX activity to terminate. */
    txrxTimeout = 0;
    do
    {
        if (txrxTimeout >= TX_RX_DISABLE_TIMEOUT)
        {
            mvOsPrintf("ethPort_ETH_ESDCMR: TIMEOUT for TX/RX stopped !!!\n");
            break;
        }
        txrxTimeout++;
        
        /* Check that current RX/TX operation is aborted, and moves to IDLE. */
        regData = MV_MEMIO_LE32_READ(ETH_ESDCMR);
    } while (regData & (ETH_RX_ABORT_MASK | ETH_TX_ABORT_MASK));
    
    /* Double check to Verify that TX FIFO is Empty */
    txFifoTimeout = 0;
    do
    {
        if(txFifoTimeout >= TX_FIFO_EMPTY_TIMEOUT)
        {
            mvOsPrintf("\n ethPort: TIMEOUT for TX FIFO empty !!!\n");
            break;
        }
        txFifoTimeout++;

        regData = MV_MEMIO_LE32_READ(ETH_EPSR);
    } while (regData & ETH_TX_IN_PROGRESS_MASK);

    pPortCtrl->portState = MV_PAUSED;

    return MV_RES_ETH_OK;   
}

/*******************************************************************************
* ethPortEnable - Enable the Ethernet port and Start RX and TX.
* DESCRIPTION:
*       This routine enable the Ethernet port and Rx and Tx activity:
*       Note: Each Rx and Tx queue descriptor's list must be initialized prior
*       to calling this function (use etherInitTxDescRing for Tx queues and
*       etherInitRxDescRing for Rx queues).
* INPUT:
*       void*   pEthPortHndl  - Ethernet port handler
* RETURN:   MV_STATUS
*               MV_OK - Success, Others - Failure.
* NOTE: main usage is to enable the port after ifconfig up.
*******************************************************************************/
int mvEthPortEnable(void* pEthPortHndl)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pEthPortHndl;
    unsigned int    epcrReg;

    /* Enable port */
    epcrReg = MV_MEMIO_LE32_READ(ETH_EPCR);
    GA_REG_WORD32_WRITE(ETH_EPCR, epcrReg | ETH_PORT_ENABLE_MASK);

    /* clear MIB counters */
    mvEthMibCountersClear(pEthPortHndl);

    pPortCtrl->portState = MV_PAUSED;

    /* If Link is UP, Start RX and TX traffic */
    if (MV_MEMIO_LE32_READ(ETH_EPSR) & ETH_LINK_UP_MASK)
        return mvEthPortUp(pEthPortHndl);
	else 
	{
		/* even the link is down, just enable Rx, but return not ready */
		mvEthPortUp(pEthPortHndl);
		return MV_RES_ETH_NOTREADY; /* the otherside not ready yet */
	}
}

/*******************************************************************************
* mvEthPortDisable - Stop RX and TX activities and Disable the Ethernet port.
* INPUT:
*       void*   pEthPortHndl  - Ethernet port handler
* RETURN:   MV_STATUS
*               MV_OK - Success, Others - Failure.
* NOTE: main usage is to disable the port after ifconfig down.
*******************************************************************************/
int mvEthPortDisable(void* pEthPortHndl)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pEthPortHndl;
    unsigned int    regData;

    /* Stop RX and TX activities */
    if(pPortCtrl->portState == MV_ACTIVE)           
    {    
        mvEthPortDown(pEthPortHndl);
    }

    /* Reset the Enable bit in the Serial Control Register */
    regData = MV_MEMIO_LE32_READ(ETH_EPCR);
    regData &= ~(ETH_PORT_ENABLE_MASK);
    GA_REG_WORD32_WRITE(ETH_EPCR, regData);

    pPortCtrl->portState = MV_IDLE;
    return MV_RES_ETH_OK;
}

/*******************************************************************************
* mvEthPortTx - Send an Ethernet packet
* DESCRIPTION:
*       This routine send a given packet described by pBufInfo parameter. It
*       supports transmitting of a packet spaned over multiple buffers. 
* INPUT:
*       void*       pEthPortHndl  - Ethernet Port handler.
*       int         txQueue       - Number of Tx queue.
*       MV_PKT_INFO *pPktInfo     - User packet to send.
* RETURN:
*       MV_NO_RESOURCE  - No enough resources to send this packet.
*       MV_ERROR        - Unexpected Fatal error.
*       MV_OK           - Packet send successfully.
*******************************************************************************/
int mvEthPortTx(void* pEthPortHndl, int txQueue, MV_PKT_INFO* pPktInfo)
{
    ETH_PORT_CTRL   *pPortCtrl = (ETH_PORT_CTRL*)pEthPortHndl;
    ETH_TX_DESC     *pTxFirstDesc;
    ETH_TX_DESC     *pTxCurrDesc;
    ETH_QUEUE_CTRL  *pQueueCtrl;
    int             bufCount;
    MV_BUF_INFO     *pBufInfo = pPktInfo->pFrags;
    unsigned char   *pTxBuf;

    if ((txQueue<0) || (txQueue>=MV_ETH_TX_Q_NUM))
        return MV_RES_ETH_BADPARAM; /* bad parameter */

    pQueueCtrl = &pPortCtrl->txQueue[txQueue];

    /* Get the Tx Desc ring indexes */
    pTxCurrDesc = pQueueCtrl->pCurrentDescr;

    /* Check if there is enough resources to send the packet */
    if(pQueueCtrl->resource < pPktInfo->numFrags)
        return MV_RES_ETH_NORESOURCE; /* resource not available */

    /* Remember first desc */
    pTxFirstDesc = pTxCurrDesc;

    bufCount = 0;
    while (1)
    {   
        if (pBufInfo[bufCount].bufSize <= MIN_TX_BUFF_LOAD)
        {
            /* 
			 * Buffers with a payload smaller than MIN_TX_BUFF_LOAD(8B) must 
			 * be aligned to 64-bit boundary. Two options here:
			 * Usually, copy the payload to the reserved 8 bytes inside descriptor.
			 */
            pTxBuf = ((unsigned char *)pTxCurrDesc)+TX_BUF_OFFSET_IN_DESC;
            ethBCopy(pBufInfo[bufCount].bufVirtPtr, pTxBuf, pBufInfo[bufCount].bufSize);
            pTxCurrDesc->bufPtr = ETH_VIRT_TO_PHY(pTxBuf);
        }
        else
        {
            pTxCurrDesc->bufPtr = ETH_VIRT_TO_PHY(pBufInfo[bufCount].bufVirtPtr);
            ETH_PACKET_CACHE_FLUSH_INV(pBufInfo[bufCount].bufVirtPtr, 
					pBufInfo[bufCount].bufSize);
        }

        pTxCurrDesc->byteCnt = pBufInfo[bufCount].bufSize;
        bufCount++;

        if (bufCount >= pPktInfo->numFrags)
            break;

        if (bufCount > 1)
        {
            /* There is middle buffer of the packet Not First and Not Last */
            pTxCurrDesc->cmdSts = ETH_BUFFER_OWNED_BY_DMA | ETH_TX_GENERATE_CRC_MASK;
            ETH_DESCR_FLUSH_INV(pPortCtrl, pTxCurrDesc);
        }
        /* Go to next descriptor and next buffer */
        pTxCurrDesc = TX_NEXT_DESC_PTR(pTxCurrDesc, pQueueCtrl);
    }
    
    /* Set last desc with DMA ownership and interrupt enable. */
    pTxCurrDesc->returnInfo = pPktInfo->osInfo;
    if (bufCount == 1)
    {
        /* 
		 * There is only one buffer in the packet, the OSG might set 
		 * some bits for checksum offload, so add them to first descriptor
		 */
        pTxCurrDesc->cmdSts = pPktInfo->status           |
                              ETH_BUFFER_OWNED_BY_DMA    |
                              ETH_TX_GENERATE_CRC_MASK   |
                              ETH_ENABLE_INTERRUPT_MASK  |
                              ETH_TX_ZERO_PADDING_MASK   |
                              ETH_FIRST_DESC_MASK        |
                              ETH_LAST_DESC_MASK;

        ETH_DESCR_FLUSH_INV(pPortCtrl, pTxCurrDesc);
    }
    else
    {
        /* Last but not First */
        pTxCurrDesc->cmdSts = ETH_BUFFER_OWNED_BY_DMA       |
                              ETH_TX_GENERATE_CRC_MASK      |
                              ETH_ENABLE_INTERRUPT_MASK  |
                              ETH_TX_ZERO_PADDING_MASK      |
                              ETH_LAST_DESC_MASK;

        ETH_DESCR_FLUSH_INV(pPortCtrl, pTxCurrDesc);

        /* Update First when more than one buffer in the packet */
        /* The OSG might set some bits for checksum offload, so add them to first descriptor */
        pTxFirstDesc->cmdSts = pPktInfo->status             |
                               ETH_BUFFER_OWNED_BY_DMA      |
                               ETH_TX_GENERATE_CRC_MASK     |
                               ETH_FIRST_DESC_MASK;

        ETH_DESCR_FLUSH_INV(pPortCtrl, pTxFirstDesc);
    }

    /* Apply send command, txQueue=0: low-queue, 1: high-queue */
    GA_REG_WORD32_WRITE(ETH_ESDCMR, ETH_TX_START_QUEUE(txQueue));

    /* Update txQueue state */
    pQueueCtrl->resource -= bufCount;
    pQueueCtrl->pCurrentDescr = TX_NEXT_DESC_PTR(pTxCurrDesc, pQueueCtrl);

    return MV_RES_ETH_OK;
}

/*******************************************************************************
* mvEthPortTxDone - Free all used Tx descriptors and mBlks.
* DESCRIPTION:
*       This routine returns the transmitted packet information to the caller.
* INPUT:
*       void*       pEthPortHndl    - Ethernet Port handler.
*       int         txQueue         - Number of Tx queue.
* OUTPUT:
*       MV_PKT_INFO *pPktInfo       - Pointer to packet was sent.
* RETURN:
*       MV_NOT_FOUND    - No transmitted packets to return. Transmit in progress.
*       MV_EMPTY        - No transmitted packets to return. TX Queue is empty.
*       MV_ERROR        - Unexpected Fatal error.
*       MV_OK           - There is transmitted packet in the queue, 
*                       'pPktInfo' filled with relevant information.
*******************************************************************************/
int mvEthPortTxDone(void* pEthPortHndl, int txQueue, MV_PKT_INFO *pPktInfo)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pEthPortHndl;
    ETH_TX_DESC*    pTxCurrDesc;
    ETH_TX_DESC*    pTxUsedDesc;
    ETH_QUEUE_CTRL* pQueueCtrl;
    unsigned int    commandStatus;

    if ((txQueue<0) || (txQueue>=MV_ETH_TX_Q_NUM))
        return MV_RES_ETH_BADPARAM; /* bad parameter */       

    pQueueCtrl = &pPortCtrl->txQueue[txQueue];

    pTxUsedDesc = pQueueCtrl->pUsedDescr;
    pTxCurrDesc = pQueueCtrl->pCurrentDescr;

    while(1)
    {
        /* sync D-cache with DRAM */
        ETH_DESCR_INV_LINE(pPortCtrl, pTxUsedDesc);

        /* No more used descriptors */
        commandStatus = pTxUsedDesc->cmdSts;
        if (commandStatus  & (ETH_BUFFER_OWNED_BY_DMA))
            return MV_RES_ETH_TXBUSY; /* item not found */

        if ((pTxUsedDesc == pTxCurrDesc) && (pQueueCtrl->resource != 0))
            return MV_RES_ETH_NORESOURCE; /* item is empty */

		/*
		 * Release UsedDescr until the last buff.
		 */
        pQueueCtrl->resource++;
        pQueueCtrl->pUsedDescr = TX_NEXT_DESC_PTR(pTxUsedDesc, pQueueCtrl);
        if (commandStatus & (ETH_LAST_DESC_MASK)) 
        {
            pPktInfo->status  = commandStatus;
            pPktInfo->osInfo  = pTxUsedDesc->returnInfo;
            return MV_RES_ETH_OK;
        }
        pTxUsedDesc = pQueueCtrl->pUsedDescr;
    }
}

/*******************************************************************************
* mvEthPortForceTxDone - Get next buffer from TX queue in spite of buffer ownership.
* INPUT:
*       void*       pEthPortHndl    - Ethernet Port handler.
*       int         txQueue         - Number of TX queue.
* OUTPUT:
*       MV_PKT_INFO *pPktInfo       - Pointer to packet was sent.
* RETURN:
*       MV_EMPTY    - There is no more buffers in this queue.
*       MV_OK       - Buffer detached from the queue and pPktInfo structure 
*                   filled with relevant information.
*******************************************************************************/
int mvEthPortForceTxDone(void* pEthPortHndl, int txQueue, MV_PKT_INFO *pPktInfo)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pEthPortHndl;
    ETH_QUEUE_CTRL* pQueueCtrl;
       
    if ((txQueue<0) || (txQueue>=MV_ETH_TX_Q_NUM))
        return MV_RES_ETH_BADPARAM; /* bad parameter */
        
    pQueueCtrl = &pPortCtrl->txQueue[txQueue];

	/*
	 * Release UsedDescr to List
	 */
    while ((pQueueCtrl->pUsedDescr != pQueueCtrl->pCurrentDescr) 
			|| (pQueueCtrl->resource == 0))
    {
        pQueueCtrl->resource++;
        pPktInfo->status = ((ETH_TX_DESC*)pQueueCtrl->pUsedDescr)->cmdSts;
        pPktInfo->osInfo  = ((ETH_TX_DESC*)pQueueCtrl->pUsedDescr)->returnInfo;
        ((ETH_TX_DESC*)pQueueCtrl->pUsedDescr)->cmdSts = ETH_BUFFER_OWNED_BY_HOST;
        ((ETH_TX_DESC*)pQueueCtrl->pUsedDescr)->returnInfo = 0x0;
        ETH_DESCR_FLUSH_INV(pPortCtrl, pQueueCtrl->pUsedDescr);

        pQueueCtrl->pUsedDescr = TX_NEXT_DESC_PTR(pQueueCtrl->pUsedDescr, pQueueCtrl);

        if (pPktInfo->status & ETH_LAST_DESC_MASK) 
            return MV_RES_ETH_OK;
    }

    /* update current TX descriptor register */
    GA_REG_WORD32_WRITE(ETH_ECTDP0 + (txQueue*4),
			(unsigned int)ETH_VIRT_TO_PHY(pQueueCtrl->pCurrentDescr));
    return MV_RES_ETH_NORESOURCE; /* item is empty */
}
        
/*******************************************************************************
* mvEthPortRx - Get new received packets from Rx queue.(from CurrDesc directly)
* DESCRIPTION:
*       This routine returns the received data to the caller. There is no
*       data copying during routine operation. All information is returned
*       using pointer to packet information struct passed from the caller.
* INPUT:
*       void*       pEthPortHndl    - Ethernet Port handler.
*       int         rxQueue         - Number of Rx queue.
* OUTPUT:
*       MV_PKT_INFO *pPktInfo       - Pointer to received packet.
* RETURN:
*       MV_NO_RESOURCE  - No free resources in RX queue.
*       MV_ERROR        - Unexpected Fatal error.
*       MV_OK           - New packet received and 'pBufInfo' structure filled
*                       with relevant information.
*******************************************************************************/
int mvEthPortRx(void* pEthPortHndl, int rxQueue, MV_PKT_INFO *pPktInfo)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pEthPortHndl;
    ETH_RX_DESC     *pRxCurrDesc;
    unsigned int    commandStatus;
    ETH_QUEUE_CTRL* pQueueCtrl;

    if ((rxQueue < 0) || (rxQueue >= MV_ETH_RX_Q_NUM)) 
        return MV_RES_ETH_BADPARAM; /* bad parameter */
    pQueueCtrl = &(pPortCtrl->rxQueue[rxQueue]);

    /* Check resources */
    if(pQueueCtrl->resource == 0)
        return MV_RES_ETH_NORESOURCE; /* resource not available */
    
    while(1)
    {
        /* Get the Rx Desc ring 'curr and 'used' indexes */
        pRxCurrDesc = pQueueCtrl->pCurrentDescr;

        /* sync D-cache with DRAM */
        ETH_DESCR_INV_LINE(pPortCtrl, pRxCurrDesc);

        commandStatus = pRxCurrDesc->cmdSts;
        if (commandStatus & (ETH_BUFFER_OWNED_BY_DMA))
            return MV_RES_ETH_RXBUSY; /* Nothing to rx, no more items found */

        /* Valid RX only if FIRST buffer and LAST buffer bits are set */
        if((commandStatus & (ETH_LAST_DESC_MASK | ETH_FIRST_DESC_MASK)) == 
                             (ETH_LAST_DESC_MASK | ETH_FIRST_DESC_MASK))
        {
            pPktInfo->pktSize    = pRxCurrDesc->byteCnt;
            pPktInfo->status     = commandStatus;
            pPktInfo->osInfo     = pRxCurrDesc->returnInfo;

            pQueueCtrl->resource--;
            /* Update 'curr' in data structure */
            pQueueCtrl->pCurrentDescr = RX_NEXT_DESC_PTR(pRxCurrDesc, pQueueCtrl);

            return MV_RES_ETH_OK;
        }
        else
        {
            ETH_RX_DESC*    pRxUsedDesc = pQueueCtrl->pUsedDescr;

            mvOsPrintf("ethDrv: Unexpected Jumbo frame: "
					"status=0x%08x, byteCnt=%d, pData=0x%x\n", 
					commandStatus, pRxCurrDesc->byteCnt, pRxCurrDesc->bufPtr);

            /* move buffer from pCurrentDescr position to pUsedDescr position */
            pRxUsedDesc->bufPtr     = pRxCurrDesc->bufPtr;
            pRxUsedDesc->returnInfo = pRxCurrDesc->returnInfo;
            pRxUsedDesc->bufSize    = pRxCurrDesc->bufSize & ETH_RX_BUFFER_MASK;

            /* Return the descriptor to DMA ownership */
            pRxUsedDesc->cmdSts = ETH_BUFFER_OWNED_BY_DMA | ETH_ENABLE_INTERRUPT_MASK;

            /* Flush descriptor and CPU pipe */
            ETH_DESCR_FLUSH_INV(pPortCtrl, pRxUsedDesc);

            /* Move the used descriptor pointer to the next descriptor */
            pQueueCtrl->pUsedDescr = RX_NEXT_DESC_PTR(pRxUsedDesc, pQueueCtrl);
            pQueueCtrl->pCurrentDescr = RX_NEXT_DESC_PTR(pRxCurrDesc, pQueueCtrl);            
            
            return MV_RES_ETH_FAILED;
        }
    }
}

/*******************************************************************************
* mvEthPortRxDone - Returns a Rx buffer(Used Rx desc) back to the Rx ring.
* DESCRIPTION:
*       This routine returns a Rx buffer back to the Rx ring. 
* INPUT:
*       void*       pEthPortHndl    - Ethernet Port handler.
*       int         rxQueue         - Number of Rx queue.
*       MV_PKT_INFO *pPktInfo       - Pointer to received packet.
* RETURN:
*       MV_ERROR        - Unexpected Fatal error.
*       MV_OUT_OF_RANGE - RX queue is already FULL, so this buffer can't be 
*                       returned to this queue.
*       MV_FULL         - Buffer returned successfully and RX queue became full.
*                       More buffers should not be returned at the time.
*       MV_OK           - Buffer returned successfully and there are more free 
*                       places in the queue.
*******************************************************************************/
int mvEthPortRxDone(void* pEthPortHndl, int rxQueue, MV_PKT_INFO *pPktInfo)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pEthPortHndl;
    ETH_RX_DESC*    pRxUsedDesc;
    ETH_QUEUE_CTRL* pQueueCtrl;
            
    if ((rxQueue < 0) || (rxQueue >= MV_ETH_RX_Q_NUM)) 
        return MV_RES_ETH_BADPARAM; /* bad parameter */
        
    pQueueCtrl = &pPortCtrl->rxQueue[rxQueue];

    /* Get 'used' Rx descriptor */
    pRxUsedDesc = pQueueCtrl->pUsedDescr;

    /* Check that ring is not FULL */
    if( (pQueueCtrl->pUsedDescr == pQueueCtrl->pCurrentDescr) && (pQueueCtrl->resource != 0) )
    {
        mvOsPrintf("%s %d: out of range Error resource=%d, curr=%p, used=%p\n", 
                    __FUNCTION__, pPortCtrl->portNo, pQueueCtrl->resource, 
                    pQueueCtrl->pCurrentDescr, pQueueCtrl->pUsedDescr);
        return MV_RES_ETH_ERROR; /* the value is out of range */
    }

    pRxUsedDesc->bufPtr     = ETH_VIRT_TO_PHY(pPktInfo->pFrags->bufVirtPtr);
    pRxUsedDesc->returnInfo = pPktInfo->osInfo;
    pRxUsedDesc->bufSize    = pPktInfo->pFrags->bufSize & ETH_RX_BUFFER_MASK;

    /* Invalidate data buffer accordingly with pktSize */
    ETH_PACKET_CACHE_FLUSH_INV(pPktInfo->pFrags->bufVirtPtr, pPktInfo->pktSize);

    /* Return the descriptor to DMA ownership */
    pRxUsedDesc->cmdSts = ETH_BUFFER_OWNED_BY_DMA | ETH_ENABLE_INTERRUPT_MASK;

    /* Flush descriptor and CPU pipe */
    ETH_DESCR_FLUSH_INV(pPortCtrl, pRxUsedDesc);

    pQueueCtrl->resource++;

    /* Move the used descriptor pointer to the next descriptor */
    pQueueCtrl->pUsedDescr = RX_NEXT_DESC_PTR(pRxUsedDesc, pQueueCtrl);
    
    /* If ring became Full return MV_FULL */
    if (pQueueCtrl->pUsedDescr == pQueueCtrl->pCurrentDescr) 
        return MV_RES_ETH_FULL; /* item is full */

    return MV_RES_ETH_OK;
}

/*******************************************************************************
* mvEthPortForceRx - Get next buffer from RX queue in spite of buffer ownership.
* INPUT:
*       void*       pEthPortHndl    - Ethernet Port handler.
*       int         rxQueue         - Number of Rx queue.
* OUTPUT:
*       MV_PKT_INFO *pPktInfo       - Pointer to received packet.
* RETURN:
*       MV_EMPTY    - There is no more buffers in this queue.
*       MV_OK       - Buffer detached from the queue and pBufInfo structure 
*                   filled with relevant information.
*******************************************************************************/
int mvEthPortForceRx(void* pEthPortHndl, int rxQueue, MV_PKT_INFO *pPktInfo)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pEthPortHndl;
    ETH_QUEUE_CTRL* pQueueCtrl;
    
    if ((rxQueue < 0) || (rxQueue >= MV_ETH_RX_Q_NUM)) 
        return MV_RES_ETH_BADPARAM; /* bad parameter */
        
    pQueueCtrl = &pPortCtrl->rxQueue[rxQueue];

    if( pQueueCtrl->resource == 0)
    {
    	/* update current/first RX descriptor registers */
        GA_REG_WORD32_WRITE(ETH_EFRDP0 + (rxQueue*4),
				(unsigned int)ETH_VIRT_TO_PHY(pQueueCtrl->pCurrentDescr));
        GA_REG_WORD32_WRITE(ETH_ECRDP0 + (rxQueue*4),
				(unsigned int)ETH_VIRT_TO_PHY(pQueueCtrl->pCurrentDescr));
        return MV_RES_ETH_NORESOURCE; /* item is empty */
    }
    
    /* Free next descriptor */
    pQueueCtrl->resource--;
    pPktInfo->status  = ((ETH_RX_DESC*)pQueueCtrl->pCurrentDescr)->cmdSts;
    pPktInfo->osInfo  = ((ETH_RX_DESC*)pQueueCtrl->pCurrentDescr)->returnInfo;

    ((ETH_RX_DESC*)pQueueCtrl->pCurrentDescr)->cmdSts = ETH_BUFFER_OWNED_BY_HOST;
    ((ETH_RX_DESC*)pQueueCtrl->pCurrentDescr)->returnInfo = 0x0;
    ETH_DESCR_FLUSH_INV(pPortCtrl, pQueueCtrl->pCurrentDescr);

    pQueueCtrl->pCurrentDescr = RX_NEXT_DESC_PTR(pQueueCtrl->pCurrentDescr, pQueueCtrl);
    return 0;
}

/* 
 * Get number of Free resources in specific TX queue
 */
int mvEthTxResourceGet(void* pPortHndl, int txQueue)
{
    ETH_PORT_CTRL* pPortCtrl = (ETH_PORT_CTRL*)pPortHndl;

    return (pPortCtrl->txQueue[txQueue].resource);      
}

/* 
 * Get number of Free resources in specific RX queue
 */
int mvEthRxResourceGet(void* pPortHndl, int rxQueue)
{
    ETH_PORT_CTRL* pPortCtrl = (ETH_PORT_CTRL*)pPortHndl;

    return (pPortCtrl->rxQueue[rxQueue].resource);      
}

/*
 * mvEthResetAddrTbl: Reset Hash table.
 * 1.Initialize the hash table to zero.
 * 2.Set PCR of hash size, hash function, hash default mode(1:pass addresses)
 * 3.Write the hash table entry into HTPR
 */
void mvEthResetAddrTbl(void* pPortHndl)
{
    ETH_PORT_CTRL *pPortCtrl = (ETH_PORT_CTRL*)pPortHndl;
    ETH_HASH_TABLE *pHashTbl = &(pPortCtrl->hash_tbl);
    int addrTableSize, portControlReg;

    if (pHashTbl->size == MV_ETH_HASH_8K)
        addrTableSize = 0x2000*4*8;
    else 
        addrTableSize = 0x200*4*8;

    /* fill all the address table with 0's */
    mvOsMemset(pHashTbl->hash_entry, 0, addrTableSize);

    /* set hash table registers */
    portControlReg = MV_MEMIO_LE32_READ(ETH_EPCR);

    if (pHashTbl->size == MV_ETH_HASH_8K)
        portControlReg &= ~ETH_HASH_SIZE_HALF_K_MASK;
    else
        portControlReg |= ETH_HASH_SIZE_HALF_K_MASK;

    if (pHashTbl->mode == MV_ETH_HASH_MODE_0)
        portControlReg &= ~ETH_HASH_FUNCTION_1_MASK;
    else
        portControlReg |= ETH_HASH_FUNCTION_1_MASK;

    /* reset HDM to 0: discard addresses not found in hash table */
    portControlReg &= ~ETH_HASH_PASS_MODE_MASK;

    /* update port configuration register */
    GA_REG_WORD32_WRITE(ETH_EPCR, portControlReg);

    /* set Hash table base address register, this should be the bus address/physical address */
    GA_REG_WORD32_WRITE(ETH_EHTPR, (unsigned int)ETH_VIRT_TO_PHY(pHashTbl->hash_entry));

    return;
}

/*
 * mvEthInitAddrTbl: Initialize hash table
 * 1. allocate space for hash table
 * 2. call reset function to reset hash table and register
 */
int mvEthInitAddrTbl(void* pPortHndl, int hashMode, int hashSize)
{
    ETH_PORT_CTRL *pPortCtrl = (ETH_PORT_CTRL*)pPortHndl;
    ETH_HASH_TABLE *pHashTbl = &(pPortCtrl->hash_tbl);
    int addrTableSize;

    if (hashSize == MV_ETH_HASH_8K) {
        addrTableSize = 0x2000*4*8; /* total 256KB DRAM */
    } else if (hashSize == MV_ETH_HASH_HALF_K) {
        addrTableSize = 0x200*4*8; /* total 16KB DRAM */
    } else
        return MV_RES_ETH_BADPARAM; /* bad parameter */
        
    if ((hashMode != MV_ETH_HASH_MODE_0) && (hashMode != MV_ETH_HASH_MODE_1))
    	return MV_RES_ETH_BADPARAM; /* bad parameter */
    	
    /* Address table base address must be 8 byte aligned. */
    pHashTbl->hashBuf.bufVirtPtr = 
		ethAllocDescrMemory(addrTableSize + CPU_D_CACHE_LINE_SIZE, 
				&(pHashTbl->hashBuf.bufPhysAddr));
    if (pHashTbl->hashBuf.bufVirtPtr == NULL) {
        mvOsPrintf("EthPort #%d: Can't allocate %d bytes for Hash table\n", 
				pPortCtrl->portNo, addrTableSize+CPU_D_CACHE_LINE_SIZE);
        return MV_RES_ETH_FAILED;
    }
    pHashTbl->hashBuf.bufSize = addrTableSize + CPU_D_CACHE_LINE_SIZE;
    
    /* Make sure hash table address is cache line size aligned  */
    pHashTbl->hash_entry = (void *)
		MV_ALIGN_UP((unsigned long)pHashTbl->hashBuf.bufVirtPtr, CPU_D_CACHE_LINE_SIZE);
    
    if ((int)(pHashTbl->hash_entry) & 0x07){
    	ethFreeDescrMemory(&(pHashTbl->hashBuf));
    	return MV_RES_ETH_FAILED; /* operatioin failed */
    }
    
    /* fill other fields in HASH_TBL struction */
    pHashTbl->mode = hashMode;
    pHashTbl->size = hashSize;
            
    /* update Hash table register settings */
    mvEthResetAddrTbl((void *)pPortCtrl);
        
    return MV_RES_ETH_OK;
}

/*
 * mvHashTableFunction - Hash calculation function
 */
unsigned int mvHashTableFunction(unsigned int macH, unsigned int macL, 
		int HashSize, int hash_mode)
{
    unsigned int hashResult;
    unsigned int ethernetAddH;
    unsigned int ethernetAddL;
    unsigned int ethernetAdd0;
    unsigned int ethernetAdd1;
    unsigned int ethernetAdd2;
    unsigned int ethernetAdd3;
    unsigned int ethernetAddHSwapped = 0;
    unsigned int ethernetAddLSwapped = 0;

	ethernetAddH = NIBBLE_SWAPPING_16_BIT(macH);
	ethernetAddL = NIBBLE_SWAPPING_32_BIT(macL);

	ethernetAddHSwapped = GT_NIBBLE(ethernetAddH&0xf)+
        ((GT_NIBBLE((ethernetAddH>>4)&0xf))<<4)+
		((GT_NIBBLE((ethernetAddH>>8)&0xf))<<8)+
        ((GT_NIBBLE((ethernetAddH>>12)&0xf))<<12);

	ethernetAddLSwapped = GT_NIBBLE(ethernetAddL&0xf)+
        ((GT_NIBBLE((ethernetAddL>>4)&0xf))<<4)+
		((GT_NIBBLE((ethernetAddL>>8)&0xf))<<8)+
        ((GT_NIBBLE((ethernetAddL>>12)&0xf))<<12)+
		((GT_NIBBLE((ethernetAddL>>16)&0xf))<<16)+
        ((GT_NIBBLE((ethernetAddL>>20)&0xf))<<20)+
		((GT_NIBBLE((ethernetAddL>>24)&0xf))<<24)+
        ((GT_NIBBLE((ethernetAddL>>28)&0xf))<<28);

	ethernetAddH = ethernetAddHSwapped;
	ethernetAddL = ethernetAddLSwapped;

	if(hash_mode == MV_ETH_HASH_MODE_0)
	{
		ethernetAdd0 = (ethernetAddL>>2) & 0x3f;
		ethernetAdd1 = (ethernetAddL & 0x3) | ((ethernetAddL>>8) & 0x7f)<<2;
		ethernetAdd2 = (ethernetAddL>>15) & 0x1ff;
		ethernetAdd3 = ((ethernetAddL>>24) & 0xff) | ((ethernetAddH & 0x1)<<8);
	}
	else
	{
		ethernetAdd0 = FLIP_6_BITS((ethernetAddL) & 0x3f);
		ethernetAdd1 = FLIP_9_BITS(((ethernetAddL>>6) & 0x1ff));
		ethernetAdd2 = FLIP_9_BITS((ethernetAddL>>15) & 0x1ff);
		ethernetAdd3 = FLIP_9_BITS((((ethernetAddL>>24) & 0xff) | ((ethernetAddH & 0x1)<<8)));
	}

	hashResult = (ethernetAdd0<<9) | (ethernetAdd1^ethernetAdd2^ethernetAdd3);

	if(HashSize == MV_ETH_HASH_8K)
		hashResult = hashResult & 0x7fff; /* 8-k */
	else
		hashResult = hashResult & 0x7ff; /* half-k */

	return hashResult;
}

/*
 * Calculate the hash value of a MAC address and then add into hash table
 */
int mvAddAddressTableEntry(void *pEthPortHndl, 
		unsigned int macH, unsigned int macL, unsigned int rd, unsigned int skip)
{
    ETH_PORT_CTRL *pPortCtrl = (ETH_PORT_CTRL*)pEthPortHndl;
    ETH_HASH_TABLE *pHashTbl = &(pPortCtrl->hash_tbl);
        
    unsigned int    addressHighValue;
    unsigned int    addressLowValue;
    void *          entryNumber; 
    int             i;
    unsigned int    addressLowRead;
    unsigned int    addressHighRead;

    entryNumber = (void*)((unsigned int)(pHashTbl->hash_entry) + 
			(8 * mvHashTableFunction(macH, macL, pHashTbl->size, pHashTbl->mode)));
    
	addressLowValue = ETH_HASH_TABLE_ENTRY_VALID_MASK | (rd<<2) |
        (((macH>>8)&0xf)<<3) |
        (((macH>>12)&0xf)<<7) |
        (((macH>>0)&0xf)<<11) | (((macH>>4)&0xf)<<15) |
        (((macL>>24)&0xf)<<19) | (((macL>>28)&0xf)<<23) |
        (((macL>>16)&0xf)<<27) |
        ((((macL>>20)&0x1)<<31));

	addressHighValue = ((macL>>21)&0x7) | (((macL>>8)&0xf)<<3) |
        (((macL>>12)&0xf)<<7) |
        (((macL>>0)&0xf)<<11) | (((macL>>4)&0xf)<<15);

    if (rd)
		addressLowValue |= ETH_HASH_TABLE_ENTRY_RECEIVE_MASK;
		
	if (skip) 
		addressLowValue |= ETH_HASH_TABLE_ENTRY_SKIP_MASK;
		
    /* find a free place */
    for(i = 0 ; i < 12 ; i++)
    {
        addressLowRead = *(unsigned int *)((unsigned int)entryNumber + (i*8));
        if (!(addressLowRead & ETH_HASH_TABLE_ENTRY_VALID_MASK) 
				|| (addressLowRead & ETH_HASH_TABLE_ENTRY_SKIP_MASK))
        {
            entryNumber = (void *)((unsigned int)entryNumber + (i * 8));
            break;
        }
        else /* if the address is the same locate it at the same position */
        {
            addressHighRead = *(unsigned int *)((unsigned int)entryNumber + (i*8) + 4);
            if (((addressLowRead>>3)&0x1fffffff) == ((addressLowValue>>3)&0x1fffffff)
					&& ((addressHighRead)==(addressHighValue)))
            {
                entryNumber = (void *)((unsigned int)entryNumber + (i*8));
                break;
            }
        }
            
    }

    if (i == 12)
        return MV_RES_ETH_FAILED; /* operation fail */
      
    /* update address entry */   
    *(unsigned int *)(entryNumber) = addressLowValue;
    *(unsigned int *)((unsigned int)entryNumber+4) = addressHighValue;
    
    /* flush update */
    ETH_DESCR_FLUSH_INV(pPortCtrl, (unsigned int)entryNumber & (~(CPU_D_CACHE_LINE_SIZE-1)));
	
    return MV_RES_ETH_OK;
}

/*******************************************************************************
* mvEthRxFilterModeSet - Configure Fitering mode of Ethernet port
* INPUT:
*       void*   pEthPortHndl    - Ethernet Port handler.
*       int     isPromisc       - Promiscous mode
*               MV_TRUE  - accept all Broadcast, Multicast and Unicast packets
*               MV_FALSE - accept all Broadcast, specially added Multicast and
*                          single Unicast packets
* RETURN:   MV_STATUS
*******************************************************************************/
int   mvEthRxFilterModeSet(void* pEthPortHndl, int isPromisc)
{
    unsigned int    portCfgReg;

    portCfgReg = MV_MEMIO_LE32_READ(ETH_EPCR);
    
	if (isPromisc)
        /* Accept all broadcast, multicast and unicast packets to RX default queue */
        portCfgReg |= ETH_UNICAST_PROMISCUOUS_MODE_MASK;
    else
        /* receive all broadcast, only unicast and multicast address in filter table */
        portCfgReg &= ~(ETH_UNICAST_PROMISCUOUS_MODE_MASK | ETH_BROADCAST_REJECT_MODE_MASK);

    GA_REG_WORD32_WRITE(ETH_EPCR, portCfgReg);
    return MV_RES_ETH_OK;
}

/*******************************************************************************
* mvEthMacAddrSet - This function Set the port Unicast address.
* DESCRIPTION:
*       This function Set the port Ethernet MAC address. This address
*       will be used to send Pause frames if enabled(flow control). 
*       Packets with this address will be accepted and dispatched to 
*       default RX queue
* INPUT:
*       void*   pEthPortHndl    - Ethernet port handler.
*       char*   pAddr           - Address to be set
* RETURN:   MV_STATUS
*******************************************************************************/
int mvEthMacAddrSet(void* pPortHndl, unsigned char *pAddr, int queue)
{
    ETH_PORT_CTRL* pPortCtrl = (ETH_PORT_CTRL*)pPortHndl;
    unsigned int macH;
    unsigned int macL;

    mvOsMemcpy(pPortCtrl->portConfig.macAddr, pAddr, MV_MAC_ADDR_SIZE);

    macL =  (pAddr[4] << 8) | (pAddr[5]);
    macH =  (pAddr[0] << 24)| (pAddr[1] << 16) | (pAddr[2] << 8) | (pAddr[3] << 0);

    GA_REG_WORD32_WRITE(ETH_EFCSAL, macL);
    GA_REG_WORD32_WRITE(ETH_EFCSAH, macH);

    macH =  (pAddr[0] << 8) | (pAddr[1]);
    macL =  (pAddr[2] << 24) | (pAddr[3] << 16) | (pAddr[4] << 8) | (pAddr[5]);
    
    /* add unicast address to filter table, and accept frames of this address */
    mvAddAddressTableEntry(pPortCtrl, macH, macL, 1/*receive*/, 0/*not skip*/);

    return MV_RES_ETH_OK;
}

/*******************************************************************************
* mvEthMacAddrGet - Returns the flow control source address.
* DESCRIPTION:
*       This function returns the port Ethernet MAC address which is 
*       set as flow control source address in mvEthMacAddrSet.
* INPUT:
*       int     portNo          - Ethernet port number.
*       char*   pAddr           - Pointer where address will be written to
* RETURN:   MV_STATUS
*               MV_OK - Success,  Other - Faulure
*******************************************************************************/
int mvEthMacAddrGet(unsigned char *pAddr)
{
    unsigned int macH;
    unsigned int macL;

    if(pAddr == NULL)
    {
        mvOsPrintf("mvEthMacAddrGet: NULL pointer.\n");
        return MV_RES_ETH_BADPARAM; /* bad parameter */
    }

    macH = MV_MEMIO_LE32_READ(ETH_EFCSAH);
    macL = MV_MEMIO_LE32_READ(ETH_EFCSAL);
    pAddr[0] = (macH >> 24) & 0xff;
    pAddr[1] = (macH >> 16) & 0xff;
    pAddr[2] = (macH >> 8) & 0xff;
    pAddr[3] = macH & 0xff;
    pAddr[4] = (macL >> 8) & 0xff;
    pAddr[5] = macL & 0xff;

    return MV_RES_ETH_OK;
}

/*******************************************************************************
* mvEthMibCountersClear - Clear all MIB counters
* DESCRIPTION:
*       This function clears all MIB counters
* INPUT:
*       int           ethPortNum  - Ethernet Port number.
* RETURN:   void
*******************************************************************************/
void mvEthMibCountersClear(void* pPortHandle)
{
	unsigned int pcxrReg, i, dummy;
	
	/* enable MIB read clear */
	pcxrReg = MV_MEMIO_LE32_READ(ETH_EPCXR);
	pcxrReg &= ~ETH_MIB_CLEAR_MASK; /* 0 to read-clear */
	GA_REG_WORD32_WRITE(ETH_EPCXR, pcxrReg);
	
    /* Perform dummy reads from MIB registers to clear them */
    for(i=ETH_MIB_GOOD_OCTETS_RECEIVED; i<=ETH_MIB_UNDSZ_FRAMES_RECEIVED; i+=4)
        dummy = MV_MEMIO_LE32_READ(i);
    
	/* disable MIB read clear */
	pcxrReg |= ETH_MIB_CLEAR_MASK; /* 1 to read-no-effect */
	GA_REG_WORD32_WRITE(ETH_EPCXR, pcxrReg);
	return;
}

/*******************************************************************************
* mvEthMibCounterRead - Read a MIB counter
* INPUT:
*       int           ethPortNum  - Ethernet Port number.
*       unsigned int  mibOffset   - MIB counter offset.
* RETURN:
*       32 low sgnificant bits of MIB counter value.
*******************************************************************************/
unsigned int mvEthMibCounterRead(void* pPortHandle, unsigned int mibOffset)
{
    return (MV_MEMIO_LE32_READ(mibOffset));
}

/*******************************************************************************
* mvEthTxQueueWeightConfig - Configure Transmit (TX) queue weight.
* DESCRIPTION:
*       This function configures parameters of specific TX queue.
* INPUT:
*   void*  pPortHandle - Pointer to port specific handler;
*   int    txWeight    - weighted round-robin priority scheduling. 
*					0 - 1 pkt from High queue, 1 pkt from Low queue
* 					1 - 2 pkt from High queue, 1 pkt from Low queue
* 					2 - 4 pkt from High queue, 1 pkt from Low queue
* 					3 - 6 pkt from High queue, 1 pkt from Low queue
* 					4 - 8 pkt from High queue, 1 pkt from Low queue
* 					5 - 10 pkt from High queue, 1 pkt from Low queue
* 					6 - 12 pkt from High queue, 1 pkt from Low queue
* 					7 - all pkt from High queue, 1 pkt from Low queue
* RETURN:   MV_STATUS
*******************************************************************************/
int mvEthTxQueueWeightConfig(void* pPortHandle, int txWeight)
{
    int pcxrReg;
    
    if (txWeight<0 || txWeight>7) 
        return MV_RES_ETH_BADPARAM; /* bad parameter */

    pcxrReg = MV_MEMIO_LE32_READ(ETH_EPCXR);
    pcxrReg &= ~ETH_TX_PRIO_WEIGHT_ALL_MASK;
    pcxrReg |= ETH_TX_PRIO_WEIGHT_SET(txWeight);
    
    GA_REG_WORD32_WRITE(ETH_EPCXR, pcxrReg);

    return MV_RES_ETH_OK;
}

/******************************************************************************
 * mvEthRxEnable2BStuff - Stuffing 2 Bytes before a received frame
 * DESCRIPTION:
 * 		Enable bit[28] of ETH_EPCXR will enable 2B stuffing.
 * 		This feature isn't listed in the datasheet. XXX
 * INPUT:
 * 		none
 * OUTPUT:
 * 		none
 ******************************************************************************/
void mvEthRxEnable2BStuff(void* pPortHandle)
{
	int pcxrReg;

	pcxrReg = MV_MEMIO_LE32_READ(ETH_EPCXR);
	pcxrReg |= ETH_MAC_RX_2BSTUFF_MASK;
	GA_REG_WORD32_WRITE(ETH_EPCXR, pcxrReg);
}

/******************************************************************************
 * mvEthRxDisable2BStuff - Don't stuff 2 Bytes before a received frame
 * DESCRIPTION:
 * 		disable bit[28] of ETH_EPCXR will disable 2B stuffing.
 * 		This feature isn't listed in the datasheet. XXX
 * INPUT:
 * 		none
 * OUTPUT:
 * 		none
 ******************************************************************************/
void mvEthRxDisable2BStuff(void* pPortHandle)
{
	int pcxrReg;

	pcxrReg = MV_MEMIO_LE32_READ(ETH_EPCXR);
	pcxrReg &= ~ETH_MAC_RX_2BSTUFF_MASK;
	GA_REG_WORD32_WRITE(ETH_EPCXR, pcxrReg);
}

/*******************************************************************************
* mvEthSpeedDuplexSet - Set Speed and Duplex of the port.
* DESCRIPTION:
*       This function configure the port to work with desirable Duplex and Speed.
*       Changing of these parameters are allowed only when port is disabled.
*       This function disable the port if was enabled, change duplex and speed 
*       and, enable the port back if needed.
* INPUT:
*       void*           pPortHandle - Pointer to port specific handler;
*       ETH_PORT_SPEED  speed       - Speed of the port.
*       ETH_PORT_SPEED  duplex      - Duplex of the port.
* RETURN:   MV_STATUS
*       MV_OK           - Success
*       MV_OUT_OF_RANGE - Failed. Port is out of valid range
*       MV_NOT_FOUND    - Failed. Port is not initialized.
*       MV_BAD_PARAM    - Input parameters (speed/duplex) in conflict.
*       MV_BAD_VALUE    - Value of one of input parameters (speed, duplex) 
*                       is not valid
*******************************************************************************/
int mvEthSpeedDuplexSet(void* pPortHandle, MV_ETH_PORT_SPEED speed, MV_ETH_PORT_DUPLEX duplex)
{
    unsigned int pcrReg, pcxrReg, phyReg;
    unsigned int phy_id;
    
    pcxrReg = MV_MEMIO_LE32_READ(ETH_EPCXR);
    pcrReg = MV_MEMIO_LE32_READ(ETH_EPCR);
    phyReg = 0;

    /* Set Speed */
    switch(speed)
    {
        case MV_ETH_SPEED_AN:
            pcxrReg &= ~ETH_SPEED_AN_DISABLE_MASK;
            phyReg |= (1<<12);
            break;
        case MV_ETH_SPEED_10:
            pcxrReg |= ETH_SPEED_AN_DISABLE_MASK;
            pcxrReg &= ~ETH_SPEED_100_MASK;
            break;
        case MV_ETH_SPEED_100:
            pcxrReg |= ETH_SPEED_AN_DISABLE_MASK;
            pcxrReg |= ETH_SPEED_100_MASK;
            phyReg |= (1<<13);
            break;
        default:
            mvOsPrintf("ethDrv: Unexpected Speed value %d\n", speed);
            return MV_RES_ETH_BADPARAM; /* bad parameter */
    }

    /* Set duplex */
    switch (duplex)
    {
        case MV_ETH_DUPLEX_AN:
            pcxrReg &= ~ETH_DUPLEX_AN_DISABLE_MASK;
            phyReg |= (1<<12);
            break;
        case MV_ETH_DUPLEX_HALF:
            pcxrReg |= ETH_DUPLEX_AN_DISABLE_MASK;
            pcrReg &= ~ETH_FULL_DUPLEX_MODE_MASK;
            break;
        case MV_ETH_DUPLEX_FULL:
            pcxrReg |= ETH_DUPLEX_AN_DISABLE_MASK;
            pcrReg |= ETH_FULL_DUPLEX_MODE_MASK;
            phyReg |= (1<<8);
            break;
        default:
            mvOsPrintf("ethDrv: Unexpected Duplex value %d\n", duplex);
            return MV_RES_ETH_BADPARAM; /* bad parameter */
    }
    
    GA_REG_WORD32_WRITE(ETH_EPCXR, pcxrReg);
    GA_REG_WORD32_WRITE(ETH_EPCR, pcrReg);

	/*
	 * Setting the ethernet phy
	 */
	phy_id = mvEthPhyRead(ETHPHY_ID1);
	if (phy_id == MARVELL_PHY_ID1) {
    	mvEthPhyWrite(ETHPHY_CONTROL, phyReg);
		phyReg |= 0x8000;	/* SW-Reset PHY */
    	mvEthPhyWrite(ETHPHY_CONTROL, phyReg);
	} else if (phy_id == NATIONAL_PHY_ID1) {
    	mvEthPhyWrite(ETHPHY_CONTROL, phyReg);
	} else {
		mvOsPrintf("ethDrv: unsupported phy id, setting without sw-reset phy.\n");
    	mvEthPhyWrite(ETHPHY_CONTROL, phyReg);
	}

    return MV_RES_ETH_OK;
}

/*******************************************************************************
* mvEthFlowCtrlSet - Set Flow Control of the port.
* INPUT:
*       void*           pPortHandle - Pointer to port specific handler;
*       MV_ETH_PORT_FC  flowControl - Flow control of the port.
* RETURN:   MV_STATUS
*       MV_OK           - Success
*       MV_OUT_OF_RANGE - Failed. Port is out of valid range
*       MV_NOT_FOUND    - Failed. Port is not initialized.
*       MV_BAD_VALUE    - Value flowControl parameters is not valid
*******************************************************************************/
int mvEthFlowCtrlSet(void* pPortHandle, MV_ETH_PORT_FC flowControl)
{
    unsigned int    pcxrReg;
    
    pcxrReg = MV_MEMIO_LE32_READ(ETH_EPCXR);
    
    switch(flowControl)
    {
        case MV_ETH_FC_AN: /* Enable Auto-negotiation for flow control */
            pcxrReg &= ~ETH_FC_AN_DISABLE_MASK;
            break;

        case MV_ETH_FC_DISABLE:
            pcxrReg |= ETH_FC_AN_DISABLE_MASK;
            pcxrReg &= ~ETH_FC_ENABLE_MASK;
            break;

        case MV_ETH_FC_ENABLE:
            pcxrReg |= ETH_FC_AN_DISABLE_MASK;
            pcxrReg |= ETH_FC_ENABLE_MASK;
            break;

        default:
            mvOsPrintf("ethDrv: Unexpected FlowControl value %d\n", flowControl);
            return -4; /* bad parameter */
    }
    GA_REG_WORD32_WRITE(ETH_EPCXR, pcxrReg);

    return 0;
}

/*******************************************************************************
* mvEthStatusGet - Get major properties of the port .
* DESCRIPTION:
*       This function get major properties of the port (link, speed, duplex, 
*       flowControl, etc) and return them using the single structure.
* INPUT:
*       void*           pPortHandle - Pointer to port specific handler;
* OUTPUT:
*       MV_ETH_PORT_STATUS* pStatus - Pointer to structure, were port status 
*                                   will be placed.
* RETURN:   None.
*******************************************************************************/
void mvEthStatusGet(void* pPortHandle, MV_ETH_PORT_STATUS* pStatus)
{
    unsigned int  regValue;

    regValue = MV_MEMIO_LE32_READ(ETH_EPSR);

    if(regValue & ETH_MII_SPEED_100_MASK)
        pStatus->speed = MV_ETH_SPEED_100;
    else
        pStatus->speed = MV_ETH_SPEED_10;

    if(regValue & ETH_LINK_UP_MASK) 
        pStatus->isLinkUp = 1;
    else
        pStatus->isLinkUp = 0;

    if(regValue & ETH_FULL_DUPLEX_MASK) 
        pStatus->duplex = MV_ETH_DUPLEX_FULL; 
    else
        pStatus->duplex = MV_ETH_DUPLEX_HALF; 

    if(regValue & ETH_ENABLE_RCV_FLOW_CTRL_MASK) 
        pStatus->flowControl = MV_ETH_FC_ENABLE;
    else
        pStatus->flowControl = MV_ETH_FC_DISABLE;
	return;
}

/*******************************************************************************
* etherInitRxDescRing - Curve a Rx chain desc list and buffer in memory.
* DESCRIPTION:
*       This function prepares a Rx chained list of descriptors and packet
*       buffers in a form of a ring. The routine must be called after port
*       initialization routine and before port start routine.
*       The Ethernet SDMA engine uses CPU bus addresses to access the various
*       devices in the system (i.e. DRAM). This function uses the ethernet
*       struct 'virtual to physical' routine (set by the user) to set the ring
*       with physical addresses.
* INPUT:
*       ETH_QUEUE_CTRL  *pEthPortCtrl   Ethernet Port Control srtuct.
*       int             rxQueue         Number of Rx queue.
*       int             rxDescNum       Number of Rx descriptors
*       unsigned char   *rxDescBaseAddr  Rx descriptors memory area base addr.
* OUTPUT:
*       The routine updates the Ethernet port control struct with information
*       regarding the Rx descriptors and buffers.
* RETURN: None
*******************************************************************************/
void ethInitRxDescRing(void *pPortHndl, int queue)
{
    ETH_PORT_CTRL *pPortCtrl = (ETH_PORT_CTRL *)pPortHndl;
    ETH_RX_DESC *pRxDescBase, *pRxDesc, *pRxPrevDesc; 
    int ix, rxDescNum = pPortCtrl->rxQueueConfig[queue].descrNum;
    ETH_QUEUE_CTRL *pQueueCtrl = &pPortCtrl->rxQueue[queue];

    /* Make sure descriptor address is cache line size aligned  */        
    pRxDescBase = (ETH_RX_DESC *)
		MV_ALIGN_UP((unsigned long)pQueueCtrl->descBuf.bufVirtPtr, CPU_D_CACHE_LINE_SIZE);

    pRxDesc = (ETH_RX_DESC*)pRxDescBase;
    pRxPrevDesc = pRxDesc;

    /* initialize the Rx descriptors ring */
    for (ix=0; ix<rxDescNum; ix++)
    {
        pRxDesc->bufSize     = 0x0;
        pRxDesc->byteCnt     = 0x0;
        pRxDesc->cmdSts      = ETH_BUFFER_OWNED_BY_HOST;
        pRxDesc->bufPtr      = 0x0;
        pRxDesc->returnInfo  = 0x0;
        pRxPrevDesc = pRxDesc;
        if(ix == (rxDescNum-1))
        {
            /* Closing Rx descriptors ring */
            pRxPrevDesc->nextDescPtr = (unsigned int)ETH_VIRT_TO_PHY((void*)pRxDescBase);
        }
        else
        {
            pRxDesc = (ETH_RX_DESC*)((unsigned long)pRxDesc + ETH_RX_DESC_ALIGNED_SIZE);
            pRxPrevDesc->nextDescPtr = (unsigned int)ETH_VIRT_TO_PHY((void*)pRxDesc);
        }
        ETH_DESCR_FLUSH_INV(pPortCtrl, pRxPrevDesc);
    }

    pQueueCtrl->pCurrentDescr = pRxDescBase;
    pQueueCtrl->pUsedDescr = pRxDescBase;
    
    pQueueCtrl->pFirstDescr = pRxDescBase;
    pQueueCtrl->pLastDescr = pRxDesc;
    pQueueCtrl->resource = 0;

    /* set current/first RX descriptor register */
    GA_REG_WORD32_WRITE(ETH_EFRDP0+(queue*4), 
			(unsigned int)ETH_VIRT_TO_PHY(pQueueCtrl->pCurrentDescr) );
    GA_REG_WORD32_WRITE(ETH_ECRDP0+(queue*4), 
			(unsigned int)ETH_VIRT_TO_PHY(pQueueCtrl->pCurrentDescr) );
	return;
}

/*
 * Reset the Rx Descriptor chains in a Rx queue
 */
void ethResetRxDescRing(void* pPortHndl, int queue)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL*)pPortHndl;
    ETH_QUEUE_CTRL* pQueueCtrl = &pPortCtrl->rxQueue[queue];
    ETH_RX_DESC*    pRxDesc = (ETH_RX_DESC*)pQueueCtrl->pFirstDescr;

    pQueueCtrl->resource = 0;
    if(pQueueCtrl->pFirstDescr != NULL)
    {
        while(1)
        {
            pRxDesc->bufSize     = 0x0;
            pRxDesc->byteCnt     = 0x0;
            pRxDesc->cmdSts      = ETH_BUFFER_OWNED_BY_HOST;
            pRxDesc->bufPtr      = 0x0;
            pRxDesc->returnInfo  = 0x0;
            ETH_DESCR_FLUSH_INV(pPortCtrl, pRxDesc);
            if((void*)pRxDesc == pQueueCtrl->pLastDescr)
                    break;
            pRxDesc = RX_NEXT_DESC_PTR(pRxDesc, pQueueCtrl);
        }
        pQueueCtrl->pCurrentDescr = pQueueCtrl->pFirstDescr;
        pQueueCtrl->pUsedDescr = pQueueCtrl->pFirstDescr;

        /* set current/first RX descriptor register */
        GA_REG_WORD32_WRITE(ETH_EFRDP0+(queue*4), 
				(unsigned int)ETH_VIRT_TO_PHY(pQueueCtrl->pCurrentDescr));
        GA_REG_WORD32_WRITE(ETH_ECRDP0+(queue*4), 
				(unsigned int)ETH_VIRT_TO_PHY(pQueueCtrl->pCurrentDescr));
    }
    else
    {
        /* set current/first RX descriptor register */
        GA_REG_WORD32_WRITE(ETH_EFRDP0+(queue*4), 0);
        GA_REG_WORD32_WRITE(ETH_ECRDP0+(queue*4), 0);
    }
}

/*******************************************************************************
* etherInitTxDescRing - Curve a Tx chain desc list and buffer in memory.
* DESCRIPTION:
*       This function prepares a Tx chained list of descriptors and packet
*       buffers in a form of a ring. The routine must be called after port
*       initialization routine and before port start routine.
*       The Ethernet SDMA engine uses CPU bus addresses to access the various
*       devices in the system (i.e. DRAM). This function uses the ethernet
*       struct 'virtual to physical' routine (set by the user) to set the ring
*       with physical addresses.
* INPUT:
*       ETH_PORT_CTRL   *pEthPortCtrl   Ethernet Port Control srtuct.
*       int             txQueue         Number of Tx queue.
*       int             txDescNum       Number of Tx descriptors
*       int             txBuffSize      Size of Tx buffer
*       unsigned char   *pTxDescBase     Tx descriptors memory area base addr.
* OUTPUT:
*       The routine updates the Ethernet port control struct with information
*       regarding the Tx descriptors and buffers.
* RETURN:   None.
*
*******************************************************************************/
void ethInitTxDescRing(void *pPortHndl, int queue)
{
    ETH_PORT_CTRL   *pPortCtrl = (ETH_PORT_CTRL *)pPortHndl;
    ETH_TX_DESC     *pTxDescBase, *pTxDesc, *pTxPrevDesc; 
    int             ix, txDescNum = pPortCtrl->txQueueConfig[queue].descrNum;
    ETH_QUEUE_CTRL  *pQueueCtrl = &pPortCtrl->txQueue[queue];

    /* Make sure descriptor address is cache line size aligned  */
    pTxDescBase = (ETH_TX_DESC *) 
		MV_ALIGN_UP((unsigned long)pQueueCtrl->descBuf.bufVirtPtr, CPU_D_CACHE_LINE_SIZE);
    pTxDesc = (ETH_TX_DESC *)pTxDescBase;
    pTxPrevDesc = pTxDesc;

    /* initialize the Tx descriptors ring */
    for (ix=0; ix<txDescNum; ix++)
    {
        pTxDesc->byteCnt     = 0x0000;
        pTxDesc->L4iChk      = 0x0000;
        pTxDesc->cmdSts      = ETH_BUFFER_OWNED_BY_HOST;
        pTxDesc->bufPtr      = 0x0;
        pTxDesc->returnInfo  = 0x0;

        pTxPrevDesc = pTxDesc;
        if(ix == (txDescNum-1))
        {
            /* Closing Tx descriptors ring */
            pTxPrevDesc->nextDescPtr = (unsigned int)ETH_VIRT_TO_PHY((void*)pTxDescBase);
        }
        else
        {
            pTxDesc = (ETH_TX_DESC *)((unsigned long)pTxDesc + ETH_TX_DESC_ALIGNED_SIZE);
            pTxPrevDesc->nextDescPtr = (unsigned int)ETH_VIRT_TO_PHY((void*)pTxDesc);
        }
        ETH_DESCR_FLUSH_INV(pPortCtrl, pTxPrevDesc);
    }

    pQueueCtrl->pCurrentDescr = pTxDescBase;
    pQueueCtrl->pUsedDescr = pTxDescBase;
    
    pQueueCtrl->pFirstDescr = pTxDescBase;
    pQueueCtrl->pLastDescr = pTxDesc;
    pQueueCtrl->resource = txDescNum;

    /* set current TX descriptor register */
    GA_REG_WORD32_WRITE(ETH_ECTDP0+(queue*4), 
			(unsigned int)ETH_VIRT_TO_PHY(pQueueCtrl->pCurrentDescr));
	return;
}

/*
 * Reset the Rx descriptor chain of queue
 */
void ethResetTxDescRing(void* pPortHndl, int queue)
{
    ETH_PORT_CTRL*  pPortCtrl = (ETH_PORT_CTRL *)pPortHndl;
    ETH_QUEUE_CTRL* pQueueCtrl = &(pPortCtrl->txQueue[queue]);
    ETH_TX_DESC*    pTxDesc = (ETH_TX_DESC *)(pQueueCtrl->pFirstDescr);
    
    pQueueCtrl->resource = 0;
    if(pQueueCtrl->pFirstDescr != NULL){
        while(1){
            pTxDesc->byteCnt     = 0x0000;
            pTxDesc->L4iChk      = 0x0000;
            pTxDesc->cmdSts      = ETH_BUFFER_OWNED_BY_HOST;
            pTxDesc->bufPtr      = 0x0;
            pTxDesc->returnInfo  = 0x0;
            ETH_DESCR_FLUSH_INV(pPortCtrl, pTxDesc);
            pQueueCtrl->resource++;
            if( (void*)pTxDesc == pQueueCtrl->pLastDescr)
                    break;
            pTxDesc = TX_NEXT_DESC_PTR(pTxDesc, pQueueCtrl);
        }
        pQueueCtrl->pCurrentDescr = pQueueCtrl->pFirstDescr;
        pQueueCtrl->pUsedDescr = pQueueCtrl->pFirstDescr;

        /* set current TX descriptor register */
        GA_REG_WORD32_WRITE(ETH_ECTDP0+(queue*4), 
				(unsigned int)ETH_VIRT_TO_PHY(pQueueCtrl->pCurrentDescr));
    } else {
        /* set current TX descriptor register */
        GA_REG_WORD32_WRITE(ETH_ECTDP0+(queue*4), 0);
    }

    return;
}

/*******************************************************************************
* mvEthPhyAddrSet - Set the ethernet port PHY address.
* INPUT:
*       void*   pPortHandle     - Pointer to port specific handler;
*       int     phyAddr         - PHY address       
* RETURN:
*       None.
*******************************************************************************/
void mvEthPhyAddrSet(void* pPortHandle, int phyAddr)
{
    int dataReg;
    
    dataReg = MV_MEMIO_LE32_READ(ETH_EPCXR);
    /* FC_AN must be disabled before changing PHY address: page302 */
    GA_REG_WORD32_WRITE(ETH_EPCXR, dataReg | ETH_FC_AN_DISABLE_MASK);
    
    /* change PHY address */
    GA_REG_WORD32_WRITE(ETH_EPAR, phyAddr & 0x1f);
    
    /* recover EPCXR register setting */
    GA_REG_WORD32_WRITE(ETH_EPCXR, dataReg);
    
    return;
}

/*******************************************************************************
* mvEthPhyAddrGet - Get the ethernet port PHY address.
* INPUT:
*       void*   pPortHandle - Pointer to port specific handler;
* RETURN: int - PHY address.
*******************************************************************************/
int mvEthPhyAddrGet(void* pPortHandle)
{
    int regData;
	
    regData = MV_MEMIO_LE32_READ(ETH_EPAR);
    return (regData & 0x1f);
}

/*
 * Read PHY register(addr) contents via SMI interface.
 */
unsigned int mvEthPhyRead(unsigned int addr)
{
     volatile unsigned int regData;

     do {
         regData = MV_MEMIO_LE32_READ(ETH_ESMIR);
     } while (regData & ETH_SMIR_BUSY_MASK);

     regData = 
		 ETH_SMIR_OP_READ_MASK | ETH_SMIR_REGADDR_SET(addr) | ETH_SMIR_PHYADDR_SET(PHY_ADDR);
     GA_REG_WORD32_WRITE(ETH_ESMIR, regData);

     do {
         regData = MV_MEMIO_LE32_READ(ETH_ESMIR);
     } while (!(regData & ETH_SMIR_READ_VALID_MASK));

     return (regData&0x00ffff);
}

/*
 * Write PHY register(addr) contents to data via SMI interface.
 * NOTE: some PHY chip needs to be reset after writing.
 */
void mvEthPhyWrite(unsigned int addr, unsigned int data)
{
     volatile unsigned int regData;

     do {
         regData = MV_MEMIO_LE32_READ(ETH_ESMIR);
     } while (regData & ETH_SMIR_BUSY_MASK);

     data &= 0x00ffff;
     regData = ETH_SMIR_REGADDR_SET(addr) | ETH_SMIR_PHYADDR_SET(PHY_ADDR) | data;
     GA_REG_WORD32_WRITE(ETH_ESMIR, regData);
	 return;
}
