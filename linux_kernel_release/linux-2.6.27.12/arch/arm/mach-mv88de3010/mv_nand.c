/*
 * drivers/mtd/nand/mv_nand.c
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
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/compatmac.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/bitops.h>
#include <linux/leds.h>
#include <linux/list.h>
#include <linux/freezer.h>
#include <asm/io.h>

/*bbb*/
//extern int suspend_up;

#ifdef CONFIG_MTD_PARTITIONS
#include <linux/mtd/partitions.h>
#endif

#include <mach/nand_priv.h>

/* Currently we only support unthread nonDMA access to NAND flash
 */

#ifdef CONFIG_MTD_NAND_MARVELL_DMA
#include "pBridge.h"
#include "Galois_memmap.h"
#include "galois_io.h"
#include "api_dhub.h"
#include <mach/nand_bridge.h>
#include <asm/irq.h>
#include <linux/dma-mapping.h>
#endif

/* 09-03-20 zhengshi 
 * support bad block erasure
 */
#define MV_NAND_IGNORE_BADBLOCK
#ifdef MV_NAND_IGNORE_BADBLOCK
int mv_nand_bignore_badblock = 0;
#endif

/* 
 * NFC is shared by two CPUs by default, another CPU may be using it
 * we need sync up.  -Yufeng
 */
#ifndef CPU0_IS_NFC_OWNER
#include "Galois_memmap.h"
#include "global.h"
#endif

/* Currently we only support unthread nonDMA access to NAND flash
 */
#define MV_NAND_USE_THREAD 1

DEFINE_MUTEX(mv_nand_mutex);
static void dump_registers(struct nand_chip *chip);

/*
 *  
 * For devices which display every fart in the system on a seperate LED. Is
 * compiled away when LED support is disabled.
 */
DEFINE_LED_TRIGGER(mv_nand_led_trigger);

static DEFINE_SPINLOCK(mv_work_lock);

int mv_nand_update_bbt(struct mtd_info *mtd, loff_t offs)
{
	return 0;
}
int mv_nand_isbad_bbt(struct mtd_info *mtd, loff_t offs, int allowbbt)
{
	return 0;
}

/* Define default oob placement schemes for large and small page devices */
static struct nand_ecclayout nand_oob_8 = {
	.eccbytes = 0,
	.oobfree = {
		{.offset = 2,
		 .length = 6}}
};

static struct nand_ecclayout nand_oob_16 = {
	.eccbytes = 0,
	.oobfree = {
		{.offset = 2,
		 . length = 6}}
};

static struct nand_ecclayout nand_oob_64 = {
	.eccbytes = 24,
	.eccpos = {
		   40, 41, 42, 43, 44, 45, 46, 47,
		   48, 49, 50, 51, 52, 53, 54, 55,
		   56, 57, 58, 59, 60, 61, 62, 63},
	.oobfree = {
		{.offset = 2,
		 .length = 38}}
};

static struct nand_ecclayout nand_oob_32 = {
	.eccbytes = 0,
	.oobfree = {
		{.offset = 2,
		 .length = 30}}
};

static struct nand_ecclayout nand_oob_80 = {
	.eccbytes = 0,
	.oobfree = {
		{.offset = 2,
		 .length = 78}}
};

static struct nand_ecclayout nand_oob_64_lp = {
	.eccbytes = 0,
	.oobfree = {
		{.offset = 2,
		 .length = 62}}
};

/* special CMD for Marvell Nand Flash Controller */
#define MV_NAND_CMD_NFC 0x200  // STOP & CLEAR
/*Clear Command-Done bit CSx_CMDD and Page-Done bit CSx_PAGED */
#define MV_NAND_CMD_CCP 0x400  
/*RUN */
#define MV_NAND_CMD_STC 0x800  
//after reading status
#define MV_NAND_CMD_RTS 0x1000  

/*only before access oob  */
#define MV_NAND_CMD_COB 0x4000  

/*only after access oob  */
#define MV_NAND_CMD_COA 0x2000  

/*enable spare area */
#define MV_NAND_CMD_ENP 0x8000

/*after naked commed */
#define MV_NAND_CMD_ANK 0x10000

/* read check */
#define MV_NAND_CMD_ANR 0x20000

#define MV_NAND_CMD_AUT 0x40000

#ifdef CONFIG_MTD_NAND_MARVELL_DMA

#define MV_NAND_READ_PAGE_NUM 2
#define MV_NAND_READ_BUF_SIZE ((MV_NAND_READ_PAGE_NUM + 1)* ( 4096 + 64))
#define MV_NAND_DMA_FUN_SIZE ((4096*2)+256)

#define MV_NAND_DMA_INT 1

#define MV_NAND_MEM_WRITE_DWORD(base, offset, val) \
	((*(unsigned int*)(base + offset))=val)

typedef enum {
	nfc_stopped,
	nfc_started,
} nfc_startstop_t;

typedef enum {
    nfs_no_error,
    nfc_corerr,
    nfc_uncorerr,
} nfc_bchcor_t;


typedef void (rq_end_request_fn)(void *, int);

typedef struct mv_nand_request {
	struct list_head queue;
	unsigned int cmd_flags;
	unsigned int chip;
	unsigned int page; /*start page */
	unsigned int nr_pages; /* number of pages */
	unsigned char *databuffer; /*request data buffer */
	unsigned char *oobbuffer;  /*request oob buffer */
	dma_addr_t    dmaaddr; /*dma addr*/
	unsigned char *dataaddr; /*read buffer*/
	dma_addr_t    singledata;
        dma_addr_t    singleoob;
	unsigned int  singlelen;
	void *end_request_data;
	rq_end_request_fn *end_request;
	unsigned char status;
	unsigned char ctl;/* 0:use dmabuf 1: use databuff*/
	unsigned char corerr;
	spinlock_t lock;
	struct kref kref;
} mv_nand_request_t;

typedef struct request_queue {
	struct list_head	    queue_head;/*work queue*/
	mv_nand_request_t		*cur_request;/*work request*/
	struct list_head        done_head; /* read done queue*/
	struct list_head        idle_head; /*idle queue*/
	struct list_head        fly_head; /*fly queue*/
	struct tasklet_struct	finish_tasklet;
	struct task_struct	*thread;
	struct semaphore	thread_sem;
	spinlock_t queue_lock;
	int			(*issue_fn)(void *, mv_nand_request_t *);
	char  active;
} request_queue_t;

#endif
struct mv_nand_host {
	struct nand_chip	nand_chip;
	struct mtd_info		mtd;
	void __iomem		*io_base;
	// here we can add our special feature
	struct mv_nand_data	*board;
	uint32_t ctl1;
	uint32_t ctl2;
	int datalen;
	int nr_data;
	int data_pos;
	int page_addr;
	int oobsize;
	unsigned char *buf;
	struct platform_device *pdev;
	int           status;
	int           curcmd; /*current command */
	int           curpage;
	int         (*read_page)(struct mtd_info *mtd, \
			                 struct nand_chip *chip, \
							 uint8_t *buf);
	int         (*write_page)(struct mtd_info *mtd, \
			                 struct nand_chip *chip, \
							 const uint8_t *buf);
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	HDL_dhub PB_dhubHandle;
	unsigned int PB_mtu_size;
	dma_addr_t    funaddr;
	unsigned char * funbuf; /* function buffer */
	unsigned char * dmabuf;
    	dma_addr_t    dmaaddr;
	spinlock_t		lock;		/* Mutex */
#define MV_NAND_BUFFER_POOLS 1
	request_queue_t request_head;
	int           (*read_handler)(struct mv_nand_host *host, \
			                      mv_nand_request_t *req);
	int           (*write_handler)(struct mv_nand_host *host, \
			                       mv_nand_request_t *req);
	int irq;
#endif
};

struct mv_nand_host * mv_host = NULL;

#ifdef CONFIG_MTD_NAND_MARVELL_DMA

int mv_nand_clear_nfcrun(struct mv_nand_host *host);
int mv_nand_dma_wait_read_done(struct mv_nand_host *host)
{
	int iResult = 0;
	unsigned int read;
	//HDL_semaphore *pSemHdl;
	//pSemHdl = dhub_semaphore(&host->PB_dhubHandle);
	do {
		//read = semaphore_chk_full(pSemHdl ,PBSemaMap_dHubSemID_NFC_DATA_CP);
		read = MV_NAND_REG_READ_DWORD(MEMMAP_PB_REG_BASE,RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_full);
		if(iResult<100) {
			iResult ++;
		    //printk("wait");	
		}
		else {
			iResult = 0;
		}
	} while(!(read & (1 << PBSemaMap_dHubSemID_NFC_DATA_CP)));
	read = MV_NAND_REG_READ_DWORD(MEMMAP_PB_REG_BASE + RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub+ RA_SemaHub_Query  ,(0x80 |  PBSemaMap_dHubSemID_NFC_DATA_CP <<2));
	if((read & 0xFFFF)==1)
		MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE, RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_POP, (0x100 | PBSemaMap_dHubSemID_NFC_DATA_CP));
	MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE, RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_full,(1 << PBSemaMap_dHubSemID_NFC_DATA_CP));
	//semaphore_pop(pSemHdl, PBSemaMap_dHubSemID_NFC_DATA_CP, 1);
	//semaphore_clr_full(pSemHdl, PBSemaMap_dHubSemID_NFC_DATA_CP);
	return iResult;
}
#ifndef MV_NAND_DMA_INT
int mv_nand_dma_wait_write_done(void)
{
	int iResult = 0;
	unsigned int read;
	do {
		read = MV_NAND_REG_READ_DWORD(MEMMAP_PB_REG_BASE,RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_empty);
	} while(!(read & (1 << PBSemaMap_dHubSemID_NFC_DATA_CP)));
	MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE, RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_POP, (0x100 | PBSemaMap_dHubSemID_NFC_DATA_CP));
	MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE, RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_empty,(1 << PBSemaMap_dHubSemID_NFC_DATA_CP));
	return iResult;
	

}
#endif
void mv_end_sync_rq(void *req,int error);

enum {
	MV_DMA_CMD_READ = 1,
	MV_DMA_CMD_WRITE = 2,
	MV_DMA_CMD_ID = 3,
	MV_DMA_CMD_STATUS = 4,
	MV_DMA_CMD_ERASE = 5,
	MV_DMA_CMD_DONE  = 10
};
enum {
	MV_MTU_8_BYTE = 0,
	MV_MTU_32_BYTE,
	MV_MTU_128_BYTE,
	MV_MTU_1024_BYTE
};

enum {
	MV_REQ_READY = 0,/*get one request and init it*/
	MV_REQ_WAITING,  /*waiting the request to be done*/
	MV_REQ_FLYING,  /*request is flying*/
	MV_REQ_DONE,     /*handled by DMA */
};

#define to_request(d) container_of(d, mv_nand_request_t, kref)
#define list_to_request(d) container_of(d, mv_nand_request_t, queue)
void mv_nand_dma_rw_init(void *ht);

void mv_nand_dma_exrw_init(void *ht);
void elevator_linus(struct mv_nand_request *req ,struct list_head *real_head,struct list_head *head);

void fill_read_request(mv_nand_request_t *request,
		unsigned char *dataaddr,
		dma_addr_t  dmaaddr)
{
	if(request) {
		request->dataaddr = dataaddr;
		request->dmaaddr = dmaaddr;
		request->cmd_flags = MV_DMA_CMD_READ;
	}
}
void fill_nand_request(mv_nand_request_t *request,
		unsigned page,
		unsigned int nr_pages,
		unsigned char *databuffer,
		unsigned char *oobbuffer,
		unsigned int rw,
		char ctrl) 
{
	if(request) {
	    request->page = page;
	    request->nr_pages = nr_pages;
	    request->databuffer = databuffer;
	    request->oobbuffer = oobbuffer;
	    request->cmd_flags = rw;
		request->status = MV_REQ_READY;
		request->ctl = ctrl;
	}
	return ;

}
void init_nand_request(mv_nand_request_t *request)
{
	memset(request ,0 , sizeof(mv_nand_request_t));
	if(request) {
		kref_init(&request->kref);
		spin_lock_init(&request->lock);
		request->end_request = mv_end_sync_rq;
	}
}

mv_nand_request_t *mv_nand_alloc_request(void )
{
	mv_nand_request_t *req;
	req = kmalloc(sizeof(mv_nand_request_t),GFP_KERNEL);
	if(!req) {
		printk("alloc_request:kmalloc failed");
		return NULL;
	}
	init_nand_request(req);
	return req;
}

void flush_all_done_request(struct mv_nand_host *host)
{
	struct list_head *tmphead = NULL,*idlehead;
	mv_nand_request_t *req = NULL;
	tmphead = host->request_head.done_head.next;
	while(tmphead !=&host->request_head.done_head) {
		req = list_to_request(tmphead);
		list_del_init(&req->queue);
		req->status = MV_REQ_READY;
		req->databuffer = NULL;
		req->oobbuffer = NULL;
		idlehead = &host->request_head.idle_head;
		elevator_linus(req ,idlehead ,&host->request_head.idle_head);
		tmphead = host->request_head.done_head.next;
	}
}

void flush_all_waiting_preread_request(struct mv_nand_host *host)
{
	unsigned long flags;
	mv_nand_request_t *req = NULL;
	struct list_head *tmphead = NULL,*idlehead;
	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	if(!list_empty(&host->request_head.queue_head))
	{
		tmphead = host->request_head.queue_head.next;
		while(tmphead !=&host->request_head.queue_head) {
			req = list_to_request(tmphead);
			tmphead = tmphead->next;
			if((req->cmd_flags == MV_DMA_CMD_READ) && req->databuffer == NULL)
			{
				list_del_init(&req->queue);
				req->status = MV_REQ_READY;
				req->oobbuffer = NULL;
				idlehead = &host->request_head.idle_head;
				elevator_linus(req ,idlehead ,&host->request_head.idle_head);
			}
		}
		
	}
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
}
mv_nand_request_t *mv_nand_get_read_request(struct mv_nand_host *host,int page,int stress)
{
	unsigned long flags;
	mv_nand_request_t *req = NULL;
	struct list_head *tmphead = NULL;
	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	/*try to get from done-queue*/
	if(!list_empty(&host->request_head.done_head)) {
		tmphead = host->request_head.done_head.next;
		while(tmphead !=&host->request_head.done_head) {
			req = list_to_request(tmphead);
			if(req->status == MV_REQ_DONE && (req->page == page) && (req->end_request_data == NULL) && (req->oobbuffer == NULL)&& (req->databuffer == NULL)) {
				list_del_init(&req->queue);
				goto getreq;
			}
			tmphead = tmphead->next;
		}
	}
	if(stress){
		flush_all_done_request(host);
	}
	
	req = NULL;
	if(!list_empty(&host->request_head.idle_head)) {
		req = list_to_request(host->request_head.idle_head.next);
		list_del_init(&req->queue);
		goto getreq;
	} else {
		/*try to get is from done queue */
		if(!list_empty(&host->request_head.done_head)) {
			tmphead = host->request_head.done_head.next;
			while(tmphead !=&host->request_head.done_head) {
				req = list_to_request(tmphead);
				if((req->cmd_flags == MV_DMA_CMD_READ)&&(req->end_request_data == NULL)&&(req->oobbuffer == NULL)&&(req->databuffer == NULL)) {
					list_del_init(&req->queue);
					req->status = MV_REQ_READY;
					goto getreq;
				}
				tmphead = tmphead->next;
			}

		}
		req = NULL;
		/*try to get it from work queue*/
		if(!list_empty(&host->request_head.queue_head)) {
			tmphead = host->request_head.queue_head.next;
			
			while(tmphead !=&host->request_head.queue_head) {
			    req = list_to_request(tmphead);
				if((req->cmd_flags == MV_DMA_CMD_READ)&&(req->end_request_data == NULL)&&(req->oobbuffer == NULL)&&(req->databuffer == NULL)) {
					list_del_init(&req->queue);
					req->status = MV_REQ_READY;
					goto getreq;
				}
				tmphead = tmphead->next;
			}
		}



	}
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	return NULL;
getreq:
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	return req;
}
void debug_queue(struct mv_nand_host *host)
{
	struct list_head *tmphead = NULL;
	mv_nand_request_t *req = NULL;

	if(!list_empty(&host->request_head.done_head)) {
		tmphead = host->request_head.done_head.next;
		while(tmphead !=&host->request_head.done_head) {
			req = list_to_request(tmphead);
			galois_printk("done req->status = 0x%x req->end_request_data = 0x%x\n",req->status,req->end_request_data==NULL?1:0);
			tmphead = tmphead->next;
		}
	}

	if(!list_empty(&host->request_head.queue_head)) {
		tmphead = host->request_head.queue_head.next;
		while(tmphead !=&host->request_head.queue_head) {
			req = list_to_request(tmphead);
			galois_printk("queue req->status = 0x%x req->end_request_data = 0x%x\n",req->status,req->end_request_data==NULL?1:0);
			tmphead = tmphead->next;
		}
	}

	if(!list_empty(&host->request_head.idle_head)) {
		tmphead = host->request_head.idle_head.next;
		while(tmphead !=&host->request_head.idle_head) {
			req = list_to_request(tmphead);
			galois_printk("idle req->status = 0x%x req->end_request_data = 0x%x\n",req->status,req->end_request_data==NULL?1:0);
			tmphead = tmphead->next;
		}
	}


	if(!list_empty(&host->request_head.fly_head)) {
		tmphead = host->request_head.fly_head.next;
		while(tmphead !=&host->request_head.fly_head) {
			req = list_to_request(tmphead);
			galois_printk("fly req->status = 0x%x req->end_request_data = 0x%x\n",req->status,req->end_request_data==NULL?1:0);
			tmphead = tmphead->next;
		}
	}

	if(host->request_head.cur_request) {
		galois_printk("cur->status = 0x%x cur->end_request_data= 0x%x\n",host->request_head.cur_request->status,host->request_head.cur_request->end_request_data==NULL?1:0);
	}
}
void request_destroy(struct kref *kref)
{
	mv_nand_request_t *req = to_request(kref);
	kfree(req);

}

void mv_nand_release_request(mv_nand_request_t *req)
{
	//list_del_init(&req->queue);
	if(req && (req->cmd_flags != MV_DMA_CMD_READ))
		kref_put(&req->kref, &request_destroy);
}

void add_request_to_idle_queue(struct mv_nand_host *host,mv_nand_request_t *req)
{
	unsigned long flags;
	struct list_head *head;
	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	head = &host->request_head.idle_head;
	req->status = MV_REQ_READY;
	req->databuffer = req->oobbuffer = NULL;
	elevator_linus(req ,head ,&host->request_head.idle_head);
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	

}


void add_request_to_work_queue(struct mv_nand_host *host,mv_nand_request_t *req)
{
	unsigned long flags;
	struct list_head *head;
	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	head = &host->request_head.queue_head;
	req->status = MV_REQ_READY;
	elevator_linus(req ,head ,&host->request_head.queue_head);
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	

}

void add_request_to_fly_queue(struct mv_nand_host *host,mv_nand_request_t *req)
{
	unsigned long flags;
	struct list_head *head;
	if(req->cmd_flags != MV_DMA_CMD_READ || !req->end_request_data) 
		return;
	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	head = &host->request_head.fly_head;
	req->status = MV_REQ_FLYING;
	elevator_linus(req ,head ,&host->request_head.fly_head);
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	

}


int release_request_from_fly_queue(struct mv_nand_host *host,mv_nand_request_t *req)
{
	unsigned long flags;
	//struct list_head *head;
	if((req->cmd_flags != MV_DMA_CMD_READ) || ( req->status !=MV_REQ_FLYING)) 
		return -1;
	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	list_del_init(&req->queue);
	req->status = MV_REQ_READY;
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	return 0;
}

void add_request_to_done_queue(struct mv_nand_host *host,mv_nand_request_t *req)
{
	unsigned long flags;
	struct list_head *head;

	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	head = &host->request_head.done_head;
	req->status = MV_REQ_DONE;
	elevator_linus(req ,head ,&host->request_head.done_head);
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);

}
mv_nand_request_t *mv_nand_get_request(mv_nand_request_t *req)
{
	if(req)
		kref_get(&req->kref);
	return req;
}

void elevator_linus(struct mv_nand_request *req , 
		struct list_head *real_head,
		struct list_head *head)
{
	struct list_head *entry = real_head;
	while((entry = entry->prev) != head) {}
	if(!req->end_request_data) {
		entry = entry->prev;
	}
	list_add(&req->queue,entry);
}

request_queue_t *mv_nand_get_queue( struct mv_nand_host *host)
{
	request_queue_t *ret;
	unsigned long flags;
	spin_lock_irqsave(&mv_work_lock,flags);
	ret = &host->request_head;
	spin_unlock_irqrestore(&mv_work_lock,flags);
	return ret;
}

int request_exist_in_done_queue(struct mv_nand_host *host,mv_nand_request_t * req)
{
	int exist = 0;
	unsigned long flags;
	struct list_head *head = NULL;
	mv_nand_request_t *tmpreq = NULL;
	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	if(!list_empty(&host->request_head.done_head)) {
		head = host->request_head.done_head.next;		
		while(head !=&host->request_head.done_head) {
			tmpreq = list_to_request(head);
			if((tmpreq->page == req->page) &&(req->cmd_flags == tmpreq->cmd_flags)&&(tmpreq->end_request_data == NULL)) {
				exist = 1;
				break;
			}
			head = head->next;
		}
	}
		
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	return exist;
}
void mv_execute_rq(struct mv_nand_host *host,mv_nand_request_t * req)
{
	DECLARE_COMPLETION_ONSTACK(wait);
	struct mtd_info *mtd = (struct mtd_info *)&(host->mtd);
#ifdef MV_NAND_USE_THREAD
	unsigned long flags;
	struct list_head *head;
	req->end_request_data = (void *)&wait;
	req->status = MV_REQ_WAITING;
	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	head = &host->request_head.queue_head;
	elevator_linus(req ,head ,&host->request_head.queue_head);
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	wake_up_process(host->request_head.thread);
#else
	unsigned long flags;
	request_queue_t *mq = &host->request_head;
	req->end_request_data = (void *)&wait;
	req->status = MV_REQ_WAITING;
	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	list_del_init(&req->queue);
	mq->issue_fn((void *)host, req);
	host->request_head.cur_request = req;
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
#endif
	wait_for_completion(&wait);
	req->end_request_data = NULL;
	if(req->cmd_flags == MV_DMA_CMD_READ) {
		if(req->corerr) {
			mtd->ecc_stats.failed++ ;
			req->corerr = 0;
		}
	
		release_request_from_fly_queue(host,req);
		add_request_to_idle_queue(host,req);
	}
}

int mv_nand_make_request(struct mv_nand_host *host,
		int rw ,
		unsigned int page, 
		unsigned int nr_pages,
		char *databuffer,
		char *oobbuffer,
		char ctl)
{
	request_queue_t *q;
	mv_nand_request_t *req = NULL;
	struct mtd_info *mtd = (struct mtd_info *)&(host->mtd);
#ifdef MV_NAND_DMA_INT
	//struct list_head *head;
	//unsigned long flags;
#else
	int read,ctrl;
	struct nand_chip *chip = &host->nand_chip;
#endif
	int ret = 0;
	if(rw != MV_DMA_CMD_READ) {
		q = mv_nand_get_queue(host);
		if(!q) {
			printk("mv_nand_make_request : trying to access nonexeistent request queue\n");
			return -EIO;
		}
		req = mv_nand_alloc_request();
		if(!req)
			return -ENOMEM;
		req = mv_nand_get_request(req);
		fill_nand_request(req ,page ,nr_pages ,databuffer ,oobbuffer ,rw, ctl);
	} else {
		q = mv_nand_get_queue(host);
		req = mv_nand_get_read_request(host,page,1);
		if(req == NULL) {
			debug_queue(host);
			galois_printk("mv_nand_make_request : trying to get nonexeistent read queue\n");
			return -EIO;
		} else {
			if(req->status == MV_REQ_READY) {
				fill_nand_request(req ,page ,nr_pages ,databuffer ,oobbuffer ,rw, ctl);
			} else if(req->status == MV_REQ_DONE) {
				if(req->corerr) {
					mtd->ecc_stats.failed++ ;
					req->corerr = 0;
				}
				if(databuffer) {
					memcpy(databuffer , req->dataaddr ,mtd->writesize);
				}
				if(oobbuffer) {
					memcpy(oobbuffer ,  req->dataaddr + mtd->writesize ,host->oobsize);
				} else {
					memcpy(databuffer + mtd->writesize , req->dataaddr + mtd->writesize,host->oobsize);
				}
				add_request_to_idle_queue(host,req);
				return ret;
			} else {
				galois_printk("mv_nand_make_request :  get wrong read queue\n");
				return -EIO;
			}
		}

	}
#ifdef MV_NAND_DMA_INT	
	if(req->status != MV_REQ_DONE){
		mv_execute_rq(host,req);
	}
#else
	mutex_lock(&mv_nand_mutex);
	mv_nand_dma_rw_init(host);
	if(rw == MV_DMA_CMD_READ)
		host->read_handler(host ,req);
	else
		host->write_handler(host ,req);

	if(rw == MV_DMA_CMD_READ) {
		mv_nand_dma_wait_read_done(host);
		if(req->corerr)
			mtd->ecc_stats.failed++ ;

		if(mtd->writesize <= 2048) {
			if(req->databuffer)
				memcpy(req->databuffer , host->dmabuf ,mtd->writesize);
			if(req->oobbuffer)
				memcpy(req->oobbuffer , host->dmabuf + mtd->writesize ,host->oobsize);
			else
				memcpy(req->databuffer + mtd->writesize , host->dmabuf + mtd->writesize,host->oobsize);
		} else {
			if(req->databuffer) {
				memcpy(req->databuffer , host->dmabuf ,mtd->writesize / 2);
				memcpy(req->databuffer + mtd->writesize / 2 , host->dmabuf + mtd->writesize / 2 + host->oobsize/2,mtd->writesize/2);
			}
			if(req->oobbuffer) {
				memcpy(req->oobbuffer , host->dmabuf + mtd->writesize/2 ,host->oobsize/2);
				memcpy(req->oobbuffer + host->oobsize/2, host->dmabuf + mtd->writesize + host->oobsize/2 ,host->oobsize/2);
			} else {
				memcpy(req->databuffer + mtd->writesize , host->dmabuf + mtd->writesize/2,host->oobsize/2);
				memcpy(req->databuffer + mtd->writesize + host->oobsize/2, host->dmabuf + mtd->writesize + host->oobsize/2,host->oobsize/2);
			}
		}
	}
	else {
		mv_nand_dma_wait_read_done(host);
		mv_nand_clear_nfcrun(host);
	}
	mutex_unlock(&mv_nand_mutex);
#endif
	/*free the req*/
	mv_nand_release_request(req);
	return ret;

}

static inline void mv_nand_add_request(request_queue_t *q,mv_nand_request_t *req,struct list_head *head)
{
	elevator_linus(req,&q->queue_head,head);
	return ;
}

int mv_nand_clear_nfcrun(struct mv_nand_host *host)
{
	int read;
	do {
		read = MV_NAND_REG_READ_DWORD(host->io_base,MV_NAND_NDCR_OFFSET);
		read &= MV_NAND_NDCR_ND_RUN;
	}while(read);

	return 0;
}


int mv_nand_write_predma_2k_bch(struct mv_nand_host *host,mv_nand_request_t *req)
{
	struct mtd_info *mtd = &host->mtd;
	struct nand_chip *chip = &host->nand_chip;	
	unsigned int src_4321, src_xxx5;
	unsigned int cmd;
	unsigned int i = 0, j;
	int page;
	SIE_BCMCFGW cfgw;
	SIE_BCMRDAT rdat;
	SIE_BCMRCMD rcmd;
	SIE_BCMSEMA sema;
	unsigned int *p_u32;
	unsigned char *tmp;
	tmp = (unsigned char *)&sema;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&rcmd;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&cfgw;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&rdat;
	memset(tmp, 0,8);
	if(!req->databuffer)
		return -1;
	else {
		memcpy(host->dmabuf ,req->databuffer ,mtd->writesize);
		if(!req->oobbuffer) {
			memcpy(host->dmabuf + mtd->writesize , req->databuffer + mtd->writesize, host->oobsize);
		} else {
			memcpy(host->dmabuf + mtd->writesize , req->oobbuffer, host->oobsize);
		}
		
	}
	page = req->page << chip->page_shift;
	j = 16 - chip->page_shift;
	src_4321 = ((page & (mtd->writesize - 1)) | ((page<< j) & 0xFFFF0000));
	src_xxx5 = (((page >> (32 - j)) & ((1 << j) - 1)) | ((req->nr_pages - 1) << 8));
	cmd = 0x022D1080 | (req->chip ? MV_NAND_NDCB_CSEL : 0);
	/* SEMA CHECK NFC COMMAND */
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW WRITE CMD 0 */
	cfgw.u_dat = cmd;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/
	/*CFGW WRITE  CMD 1 (address cycle 1:4) */
	cfgw.u_dat = src_4321;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf,i ,p_u32[1]);
	i += 4;
	
	/* SEMA CHECK NFC Command */
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/
	/* CFGW WRITE CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = src_xxx5;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/* SEMA artificial producer CHECK */
	sema.u_pUpdId = 0;
	sema.u_pChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_cUpdId = 0;
	sema.u_cChkId = 0;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	for( j = 0;j < req->nr_pages; j++) {
		
		sema.u_pUpdId = 0;
		sema.u_pChkId = 0;
		sema.u_cUpdId = 0;
		sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
		sema.u_hdr = BCMINSFMT_hdr_SEMA;
		p_u32 = (unsigned int *)&sema;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;

		/*RCMD WRITE from  DDR to Dhub local */
		rcmd.u_ddrAdr = (unsigned int) host->dmaaddr;
		rcmd.u_size = host->oobsize + mtd->writesize;
		rcmd.u_chkSemId = 0;
		/*
		if (j == (req->nr_pages - 1))
			rcmd.u_updSemId = PBSemaMap_dHubSemID_NFC_DATA_CP;
		else
		*/
		rcmd.u_updSemId = 0;
		rcmd.u_hdr = BCMINSFMT_hdr_RCMD;
		p_u32 = (unsigned int *)&rcmd;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;

		/*forward data from dhub to NFC */
		rdat.u_size = mtd->writesize + host->oobsize;
		rdat.u_mode = 2;
		rdat.u_endian = 0;
		rdat.u_last = 1;
		if(j == (req->nr_pages - 1))
			rdat.u_cUpdId = PBSemaMap_dHubSemID_dHub_NFCDat;
		//rdat.u_cUpdId = 0;	
		rdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
		rdat.u_hdr = BCMINSFMT_hdr_RDAT;
		p_u32 = (unsigned int *)&rdat;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;

	}
	/* SEMA artificial producer CHECK */
	sema.u_pUpdId = 0;
	sema.u_pChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_cUpdId = 0;
	sema.u_cChkId = 0;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	sema.u_pUpdId = PBSemaMap_dHubSemID_NFC_DATA_CP;
	sema.u_pChkId = 0;
	sema.u_cUpdId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_cChkId = 0;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	dhub_channel_write_cmd(
			&host->PB_dhubHandle,				/*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funaddr),
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			0,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or */
			0							/*!	Pass in current cmdQ pointer (in 64b word),*/
			);
	return 0;

}


int mv_nand_write_predma_512(struct mv_nand_host *host,mv_nand_request_t *req)
{
	struct mtd_info *mtd = &host->mtd;	
	struct nand_chip *chip = &host->nand_chip;	
	unsigned int src_4321, src_xxx5;
	unsigned int cmd;
	unsigned int i = 0, j;
	int page;
	SIE_BCMCFGW cfgw;
	SIE_BCMRDAT rdat;
	SIE_BCMRCMD rcmd;
	SIE_BCMSEMA sema;
	unsigned int *p_u32;
	unsigned char *tmp;
	tmp = (unsigned char *)&sema;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&rcmd;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&cfgw;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&rdat;
	memset(tmp, 0,8);
	if(!req->databuffer)
		return -1;
	else {
		memcpy(host->dmabuf ,req->databuffer ,mtd->writesize);
		if(!req->oobbuffer) {
			memcpy(host->dmabuf + mtd->writesize , req->databuffer + mtd->writesize, host->oobsize);
		} else {
			memcpy(host->dmabuf + mtd->writesize , req->oobbuffer, host->oobsize);
		}
		
	}
	page = req->page << chip->page_shift;
	j = 16 - chip->page_shift;
	src_4321 = ((page & 0xFF) | ((page >> (chip->page_shift-8))));
	src_xxx5 = 0;
	cmd = 0x022C1080 | (req->chip ? MV_NAND_NDCB_CSEL : 0);
	/* SEMA CHECK NFC COMMAND */
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW WRITE CMD 0 */
	cfgw.u_dat = cmd;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/
	/*CFGW WRITE  CMD 1 (address cycle 1:4) */
	cfgw.u_dat = src_4321;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf,i ,p_u32[1]);
	i += 4;
	
	/* SEMA CHECK NFC Command */
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/
	/* CFGW WRITE CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = src_xxx5;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* SEMA artificial producer CHECK */
	sema.u_pUpdId = 0;
	sema.u_pChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_cUpdId = 0;
	sema.u_cChkId = 0;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	for( j = 0;j < req->nr_pages; j++) {
		
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/*RCMD WRITE from  DDR to Dhub local */
	rcmd.u_ddrAdr = (unsigned int) host->dmaaddr;
		rcmd.u_size = host->oobsize + mtd->writesize;
	rcmd.u_chkSemId = 0;
	rcmd.u_updSemId = 0;
	rcmd.u_hdr = BCMINSFMT_hdr_RCMD;
	p_u32 = (unsigned int *)&rcmd;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/*forward data from dhub to NFC */
	rdat.u_size = mtd->writesize + host->oobsize;
	rdat.u_mode = 2;
	rdat.u_endian = 0;
	rdat.u_last = 1;
	if(j == (req->nr_pages - 1))
		rdat.u_cUpdId = PBSemaMap_dHubSemID_dHub_NFCDat;
		//rdat.u_cUpdId = 0;	
	rdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
	rdat.u_hdr = BCMINSFMT_hdr_RDAT;
	p_u32 = (unsigned int *)&rdat;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	}
	/* SEMA artificial producer CHECK */
	sema.u_pUpdId = 0;
	sema.u_pChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_cUpdId = 0;
	sema.u_cChkId = 0;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	sema.u_pUpdId = PBSemaMap_dHubSemID_NFC_DATA_CP;
	sema.u_pChkId = 0;
	sema.u_cUpdId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_cChkId = 0;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	dhub_channel_write_cmd(
			&host->PB_dhubHandle,				/*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funaddr),
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			0,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or */
			0							/*!	Pass in current cmdQ pointer (in 64b word),*/
			);
	return 0;
}
int mv_nand_write_predma_4k_bch(struct mv_nand_host *host,mv_nand_request_t *req)
{
	struct mtd_info *mtd = &host->mtd;
	struct nand_chip *chip = &host->nand_chip;	
	unsigned int src_4321, src_xxx5;
	unsigned int cmd;
	unsigned int i = 0, j;
	int page;
	SIE_BCMCFGW cfgw;
	SIE_BCMRDAT rdat;
	SIE_BCMRCMD rcmd;
	SIE_BCMSEMA sema;
	unsigned int *p_u32;
	unsigned char *tmp;
	tmp = (unsigned char *)&sema;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&rcmd;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&cfgw;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&rdat;
	memset(tmp, 0,8);
	if(!req->databuffer)
		return -1;
	else {
		memcpy(host->dmabuf ,req->databuffer ,mtd->writesize/2);
		memcpy(host->dmabuf + mtd->writesize/2 + host->oobsize/2,req->databuffer + mtd->writesize/2,mtd->writesize/2);
		if(!req->oobbuffer) {
			memcpy(host->dmabuf + mtd->writesize/2 , req->databuffer + mtd->writesize, host->oobsize/2);
			memcpy(host->dmabuf + mtd->writesize + host->oobsize/2 , req->databuffer + mtd->writesize + host->oobsize/2, host->oobsize/2);
		} else {
			memcpy(host->dmabuf + mtd->writesize/2 , req->oobbuffer, host->oobsize/2);
			memcpy(host->dmabuf + mtd->writesize + host->oobsize/2 , req->oobbuffer + host->oobsize/2, host->oobsize/2);
		}
		
	}
	page = req->page << chip->page_shift;
	j = 16 - chip->page_shift;
	src_4321 = ((page & (mtd->writesize - 1)) | ((page<< j) & 0xFFFF0000));
	src_xxx5 = (((page >> (32 - j)) & ((1 << j) - 1)) | ((req->nr_pages - 1) << 8));
	cmd = 0x00D5FF80 | (req->chip ? MV_NAND_NDCB_CSEL : 0);
	/* SEMA CHECK NFC COMMAND */
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW WRITE CMD 0 */
	cfgw.u_dat = cmd;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/*CFGW WRITE  CMD 1 (address cycle 1:4) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf,i ,p_u32[1]);
	i += 4;
	
	/* CFGW WRITE CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	/* CFGW WRITE CMD 0 */
	cfgw.u_dat = 0x00F5FFFF| (req->chip ? MV_NAND_NDCB_CSEL : 0);
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/*CFGW WRITE  CMD 1 (address cycle 1:4) */
	cfgw.u_dat = src_4321;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf,i ,p_u32[1]);
	i += 4;
	
	/* CFGW WRITE CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = src_xxx5;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/* CFGW WRITE CMD 0 */
	cfgw.u_dat = 0xA0350000| (req->chip ? MV_NAND_NDCB_CSEL : 0);
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/*CFGW WRITE  CMD 1 (address cycle 1:4) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf,i ,p_u32[1]);
	i += 4;
	
	/* CFGW WRITE CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	rcmd.u_ddrAdr = (unsigned int) host->dmaaddr;
	rcmd.u_size = host->oobsize/2 + mtd->writesize/2;
	rcmd.u_chkSemId = 0;
	rcmd.u_updSemId = 0;
	rcmd.u_hdr = BCMINSFMT_hdr_RCMD;
	p_u32 = (unsigned int *)&rcmd;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	rdat.u_size = mtd->writesize/2 + host->oobsize/2;
	rdat.u_mode = 2;
	rdat.u_endian = 0;
	rdat.u_last = 1;
	rdat.u_pUpdId = 0;
	rdat.u_cUpdId = 0;
	rdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
	rdat.u_hdr = BCMINSFMT_hdr_RDAT;
	p_u32 = (unsigned int *)&rdat;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	
	/* CFGW WRITE CMD 0 */
	cfgw.u_dat = 0xA0350000| (req->chip ? MV_NAND_NDCB_CSEL : 0);
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/*CFGW WRITE  CMD 1 (address cycle 1:4) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf,i ,p_u32[1]);
	i += 4;
	
	/* CFGW WRITE CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/*RCMD WRITE from  DDR to Dhub local */
	rcmd.u_ddrAdr =  (unsigned int)host->dmaaddr + host->oobsize / 2 + mtd->writesize / 2;
	rcmd.u_size = host->oobsize / 2 + mtd->writesize / 2;
	rcmd.u_chkSemId = 0;
	rcmd.u_updSemId = 0;
	rcmd.u_hdr = BCMINSFMT_hdr_RCMD;
	p_u32 = (unsigned int *)&rcmd;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/*forward data from dhub to NFC */
	rdat.u_size = mtd->writesize / 2 + host->oobsize / 2;
	rdat.u_mode = 2;
	rdat.u_endian = 0;
	rdat.u_last = 1;
	rdat.u_cUpdId = 0;	
	rdat.u_pUpdId = PBSemaMap_dHubSemID_NFC_DATA_CP;
	rdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
	rdat.u_hdr = BCMINSFMT_hdr_RDAT;
	p_u32 = (unsigned int *)&rdat;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/* CFGW WRITE CMD 0 */
	cfgw.u_dat = 0x20C5FF10| (req->chip ? MV_NAND_NDCB_CSEL : 0);
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/*CFGW WRITE  CMD 1 (address cycle 1:4) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf,i ,p_u32[1]);
	i += 4;
	
	/* CFGW WRITE CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	
	dhub_channel_write_cmd(
			&host->PB_dhubHandle,				/*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funaddr),
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			0,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or */
			0							/*!	Pass in current cmdQ pointer (in 64b word),*/
			);
	return 0;

}

int mv_nand_read_predma_2k_bch(struct mv_nand_host *host ,mv_nand_request_t *req)
{
	struct mtd_info *mtd = &host->mtd;
	struct nand_chip *chip = &host->nand_chip;
	unsigned int src_4321, src_xxx5;
	unsigned int cmd,page;
	unsigned int i = 0, j,k;
	SIE_BCMCFGW cfgw;
	SIE_BCMWDAT wdat;
	SIE_BCMWCMD wcmd;
	SIE_BCMSEMA sema;
	unsigned int *p_u32;
	unsigned char *tmp;
	unsigned int datadmaaddr;
	unsigned int oobdmaaddr;
	tmp = (unsigned char *)&sema;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&wcmd;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&cfgw;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&wdat;
	memset(tmp, 0,8);
	//printk("page=0x%x nr_pages = 0x%x\n",req->page,req->nr_pages);
	if(req->end_request_data && req->databuffer && !req->oobbuffer) {
		req->singlelen = mtd->writesize + host->oobsize;
		req->singledata = datadmaaddr = dma_map_single(&host->pdev->dev,req->databuffer,req->singlelen,DMA_FROM_DEVICE);
		req->singleoob = oobdmaaddr = datadmaaddr + mtd->writesize;
	} else if(req->end_request_data && req->databuffer && req->oobbuffer) {
		req->singlelen = mtd->writesize;
		req->singledata = datadmaaddr = dma_map_single(&host->pdev->dev,req->databuffer,mtd->writesize,DMA_FROM_DEVICE);
		req->singleoob = oobdmaaddr =  dma_map_single(&host->pdev->dev,req->oobbuffer,host->oobsize,DMA_FROM_DEVICE);
	} else  {
		req->singlelen = 0;
		req->singledata  = 0;
		req->singleoob = 0;
		datadmaaddr = req->dmaaddr;
		oobdmaaddr = req->dmaaddr + mtd->writesize;
	}
	page = req->page << chip->page_shift;
	j = 16 - chip->page_shift;
	src_4321 = ((page & (mtd->writesize - 1)) | ((page<< j) & 0xFFFF0000));
	src_xxx5 = (((page >> (32 - j)) & ((1 << j) - 1)) | ((req->nr_pages - 1) << 8));
	cmd = 0x001D3000 | (req->chip ? MV_NAND_NDCB_CSEL : 0);
	/* SEMA CHECK NFC COMMAND */
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW READ CMD 0 */
	cfgw.u_dat = cmd;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/	
	/*CFGW REAd CMD 1 (address cycle 1:4) */
	cfgw.u_dat = src_4321;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* SEMA CHECK NFC Command */
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	*/
	/* CFGW READ CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = src_xxx5;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	for( j = 0;j < req->nr_pages; j++) {
		/*ok its BCH */
		for( k = 0; k <= mtd->writesize/32; k++) {
			sema.u_pUpdId = 0;
			sema.u_pChkId = 0;
			sema.u_cUpdId = 0;
			sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
			sema.u_hdr = BCMINSFMT_hdr_SEMA;
			p_u32 = (unsigned int *)&sema;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
			i += 4;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
			i += 4;
			
			if(k ==  mtd->writesize/32)
				wcmd.u_ddrAdr = (unsigned int)oobdmaaddr;
			else
				wcmd.u_ddrAdr = (unsigned int)datadmaaddr + k*32;
			wcmd.u_size = 32;
			wcmd.u_chkSemId = 0;
			if( (j == (req->nr_pages -1)) && (k ==  mtd->writesize/32))
				wcmd.u_updSemId = PBSemaMap_dHubSemID_NFC_DATA_CP;
			else
				wcmd.u_updSemId = 0;
			wcmd.u_hdr = BCMINSFMT_hdr_WCMD;
			p_u32 = (unsigned int *)&wcmd;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
			i += 4;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
			i += 4;
			
			wdat.u_size = 32;
			wdat.u_mode = 2;
			wdat.u_endian = 0;
			wdat.u_last = 1;
			wdat.u_cUpdId = 0;
			wdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
			wdat.u_hdr = BCMINSFMT_hdr_WDAT;
			p_u32 = (unsigned int *)&wdat;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
			i += 4;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
			i += 4;
		}

	}
	dhub_channel_write_cmd(
			&host->PB_dhubHandle,				/*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funaddr),	/*!	CMD: buffer address !*/
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			0,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or */
			0							/*!	Pass in current cmdQ pointer (in 64b word), */
			);
	return 0;
}

int mv_nand_read_predma_512(struct mv_nand_host *host ,mv_nand_request_t *req)
{
	struct mtd_info *mtd = &host->mtd;
	struct nand_chip *chip = &host->nand_chip;
	unsigned int src_4321, src_xxx5;
	unsigned int cmd,page;
	unsigned int i = 0, j;
	SIE_BCMCFGW cfgw;
	SIE_BCMWDAT wdat;
	SIE_BCMWCMD wcmd;
	SIE_BCMSEMA sema;
	unsigned int *p_u32;
	unsigned char *tmp;
	
	unsigned int datadmaaddr;
	unsigned int oobdmaaddr;
	if(req->nr_pages!= 1)
		return -1;
	/*
	if(req->end_request_data && req->databuffer && !req->oobbuffer) {
		req->singlelen = mtd->writesize + host->oobsize;
		req->singledata = datadmaaddr = dma_map_single(&host->pdev->dev,req->databuffer,req->singlelen,DMA_FROM_DEVICE);
		req->singleoob = oobdmaaddr = datadmaaddr + mtd->writesize;
	} else if(req->end_request_data && req->databuffer && req->oobbuffer) {
		req->singledata = datadmaaddr = dma_map_single(&host->pdev->dev,req->databuffer,mtd->writesize,DMA_FROM_DEVICE);
		req->singleoob = oobdmaaddr =  dma_map_single(&host->pdev->dev,req->oobbuffer,host->oobsize,DMA_FROM_DEVICE);
	} else */ 
	{
	req->singledata = req->singleoob = 0;
	req->singlelen = 0;
	datadmaaddr = req->dmaaddr;
	oobdmaaddr = req->dmaaddr + mtd->writesize;
	}
	tmp = (unsigned char *)&sema;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&wcmd;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&cfgw;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&wdat;
	memset(tmp, 0,8);
	page = req->page << chip->page_shift;
	j = 16 - chip->page_shift;
	src_4321 = ((page & 0xFF) | ((page >> (chip->page_shift - 8))));
	src_xxx5 = 0;
	cmd = 0x001C3000 | (req->chip ? MV_NAND_NDCB_CSEL : 0);
	/* SEMA CHECK NFC COMMAND */
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW READ CMD 0 */
	cfgw.u_dat = cmd;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/	
	/*CFGW REAd CMD 1 (address cycle 1:4) */
	cfgw.u_dat = src_4321;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* SEMA CHECK NFC Command */
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	*/
	/* CFGW READ CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = src_xxx5;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	for( j = 0;j < req->nr_pages; j++) {
		//for(k = 0;k <=1;k++) 
		{

			sema.u_pUpdId = 0;
			sema.u_pChkId = 0;
			sema.u_cUpdId = 0;
			sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
			sema.u_hdr = BCMINSFMT_hdr_SEMA;
			p_u32 = (unsigned int *)&sema;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
			i += 4;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
			i += 4;
			/*
			if(k == 1) {
				wcmd.u_ddrAdr = (unsigned int)oobdmaaddr;
				wcmd.u_size = host->oobsize;
			}
			else {  
				wcmd.u_size =  mtd->writesize;
				wcmd.u_ddrAdr = (unsigned int)datadmaaddr;
			}
			*/
			wcmd.u_ddrAdr = (unsigned int)datadmaaddr;
			wcmd.u_size =  mtd->writesize + host->oobsize;
			wcmd.u_chkSemId = 0;
			//if( (j == (req->nr_pages -1)) && (k ==1))
			if( (j == (req->nr_pages -1)))
				wcmd.u_updSemId = PBSemaMap_dHubSemID_NFC_DATA_CP;
			else
				wcmd.u_updSemId = 0;
			wcmd.u_hdr = BCMINSFMT_hdr_WCMD;
			p_u32 = (unsigned int *)&wcmd;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
			i += 4;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
			i += 4;
			/*
			if(k == 1)
				wdat.u_size =  host->oobsize;	
			else
				wdat.u_size = mtd->writesize;
			*/
			wdat.u_size = host->oobsize + mtd->writesize;
			wdat.u_mode = 2;
			wdat.u_endian = 0;
			wdat.u_last = 1;
			wdat.u_cUpdId = 0;
			wdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
			wdat.u_hdr = BCMINSFMT_hdr_WDAT;
			p_u32 = (unsigned int *)&wdat;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
			i += 4;
			MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
			i += 4;
		}

	}
	dhub_channel_write_cmd(
			&host->PB_dhubHandle,				/*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funaddr),	/*!	CMD: buffer address !*/
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			0,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or */
			0							/*!	Pass in current cmdQ pointer (in 64b word), */
			);
#if 0
    for (k = 0;k < i;k+=4) {
		printk("0x%x ",*((int *)&host->funbuf[k]));
		if(k % 32)
			printk("\n");
	}
#endif
	return 0;
}



int mv_nand_read_predma_4k_bch(struct mv_nand_host *host ,mv_nand_request_t *req)
{
	struct mtd_info *mtd = &host->mtd;
	struct nand_chip *chip = &host->nand_chip;
	unsigned int src_4321, src_xxx5;
	unsigned int cmd,page;
	unsigned int i = 0, j,k;
	SIE_BCMCFGW cfgw;
	SIE_BCMWDAT wdat;
	SIE_BCMWCMD wcmd;
	SIE_BCMSEMA sema;
	unsigned int *p_u32;
	unsigned char *tmp;
	unsigned int datadmaaddr;
	unsigned int oobdmaaddr;

	tmp = (unsigned char *)&sema;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&wcmd;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&cfgw;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&wdat;
	memset(tmp, 0,8);
	//printk("page=0x%x nr_pages = 0x%x 0x%x 0x%x\n",req->page,req->nr_pages,mtd->writesize,host->oobsize);
	if(req->end_request_data && req->databuffer && !req->oobbuffer) {
		req->singlelen =  mtd->writesize + host->oobsize;
		req->singledata = datadmaaddr = dma_map_single(&host->pdev->dev,req->databuffer,req->singlelen,DMA_FROM_DEVICE);
		req->singleoob = oobdmaaddr = datadmaaddr + mtd->writesize;
	} else if(req->end_request_data && req->databuffer && req->oobbuffer) {
		req->singledata = datadmaaddr = dma_map_single(&host->pdev->dev,req->databuffer,mtd->writesize,DMA_FROM_DEVICE);
		req->singleoob = oobdmaaddr = dma_map_single(&host->pdev->dev,req->oobbuffer,host->oobsize,DMA_FROM_DEVICE);
		req->singlelen =  mtd->writesize;

	} else {
		req->singlelen = 0;
		req->singleoob = 0;
		req->singledata = 0;
		datadmaaddr = req->dmaaddr;
		oobdmaaddr = req->dmaaddr + mtd->writesize;
		
	}
	page = req->page << chip->page_shift;
	j = 16 - chip->page_shift;
	src_4321 = ((page & (mtd->writesize - 1)) | ((page<< j) & 0xFFFF0000));
	src_xxx5 = (((page >> (32 - j)) & ((1 << j) - 1)) | ((req->nr_pages - 1) << 8));
	cmd = 0x00D5FF00 | (req->chip ? MV_NAND_NDCB_CSEL : 0);
	/* SEMA CHECK NFC COMMAND */
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW READ CMD 0 */
	cfgw.u_dat = cmd;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/*CFGW REAd CMD 1 (address cycle 1:4) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	
	/* CFGW READ CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	

	cmd = 0x00F5FFFF | (req->chip ? MV_NAND_NDCB_CSEL : 0);
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW READ CMD 0 */
	cfgw.u_dat = cmd;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/*CFGW REAd CMD 1 (address cycle 1:4) */
	cfgw.u_dat = src_4321;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	
	/* CFGW READ CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = src_xxx5;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

    
	cmd = 0x00D5FF30 | (req->chip ? MV_NAND_NDCB_CSEL : 0);
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW READ CMD 0 */
	cfgw.u_dat = cmd;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/*CFGW REAd CMD 1 (address cycle 1:4) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	
	/* CFGW READ CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	cmd = 0xA015FF30 | (req->chip ? MV_NAND_NDCB_CSEL : 0);
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW READ CMD 0 */
	cfgw.u_dat = cmd;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/*CFGW REAd CMD 1 (address cycle 1:4) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	
	/* CFGW READ CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	for( k = 0; k <= mtd->writesize/2/32; k++) {
		sema.u_pUpdId = 0;
		sema.u_pChkId = 0;
		sema.u_cUpdId = 0;
		sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
		sema.u_hdr = BCMINSFMT_hdr_SEMA;
		p_u32 = (unsigned int *)&sema;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;
			
		if( k == mtd->writesize/2/32)
			wcmd.u_ddrAdr = (unsigned int)oobdmaaddr;
		else
			wcmd.u_ddrAdr = (unsigned int)datadmaaddr + k*32;
		wcmd.u_size = 32;
		wcmd.u_chkSemId = 0;
		wcmd.u_updSemId = 0;
		wcmd.u_hdr = BCMINSFMT_hdr_WCMD;
		p_u32 = (unsigned int *)&wcmd;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;
			
		wdat.u_size = 32;
		wdat.u_mode = 2;
		wdat.u_endian = 0;
		wdat.u_last = 1;
		wdat.u_cUpdId = 0;
		wdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
		wdat.u_hdr = BCMINSFMT_hdr_WDAT;
		p_u32 = (unsigned int *)&wdat;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;
	}

	
	cmd = 0xA015FF30 | (req->chip ? MV_NAND_NDCB_CSEL : 0);
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW READ CMD 0 */
	cfgw.u_dat = cmd;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/*CFGW REAd CMD 1 (address cycle 1:4) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	
	/* CFGW READ CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	for( k = 0; k <= mtd->writesize/2/32; k++) {
		sema.u_pUpdId = 0;
		sema.u_pChkId = 0;
		sema.u_cUpdId = 0;
		sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
		sema.u_hdr = BCMINSFMT_hdr_SEMA;
		p_u32 = (unsigned int *)&sema;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;
			
		if(k == mtd->writesize/2/32)
			wcmd.u_ddrAdr = (unsigned int)oobdmaaddr + 32;

		else
			wcmd.u_ddrAdr = (unsigned int)datadmaaddr + mtd->writesize/2 + k*32;
		wcmd.u_size = 32;
		wcmd.u_chkSemId = 0;
		if( k == mtd->writesize/2/32)
			wcmd.u_updSemId = PBSemaMap_dHubSemID_NFC_DATA_CP;
		else
			wcmd.u_updSemId = 0;
		wcmd.u_hdr = BCMINSFMT_hdr_WCMD;
		p_u32 = (unsigned int *)&wcmd;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;
			
		wdat.u_size = 32;
		wdat.u_mode = 2;
		wdat.u_endian = 0;
		wdat.u_last = 1;
		wdat.u_cUpdId = 0;
		wdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
		wdat.u_hdr = BCMINSFMT_hdr_WDAT;
		p_u32 = (unsigned int *)&wdat;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;
	}

	dhub_channel_write_cmd(
			&host->PB_dhubHandle,				/*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funaddr),	/*!	CMD: buffer address !*/
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			0,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or */
			0							/*!	Pass in current cmdQ pointer (in 64b word), */
			);

	return 0;
}

int mv_nand_read_predma_2k_eccnobch(struct mv_nand_host *host ,mv_nand_request_t *req)
{
	struct mtd_info *mtd = &host->mtd;
	struct nand_chip *chip = &host->nand_chip;
	unsigned int src_4321, src_xxx5;
	unsigned int cmd,page;
	unsigned int i = 0, j;
	SIE_BCMCFGW cfgw;
	SIE_BCMWDAT wdat;
	SIE_BCMWCMD wcmd;
	SIE_BCMSEMA sema;
	unsigned int *p_u32;
	unsigned char *tmp;
	tmp = (unsigned char *)&sema;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&wcmd;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&cfgw;
	memset(tmp, 0,8);
	tmp = (unsigned char *)&wdat;
	memset(tmp, 0,8);
	//printk("page=0x%x nr_pages = 0x%x\n",req->page,req->nr_pages);
	page = req->page << chip->page_shift;
	j = 16 - chip->page_shift;
	src_4321 = ((page & (mtd->writesize - 1)) | ((page<< j) & 0xFFFF0000));
	src_xxx5 = (((page >> (32 - j)) & ((1 << j) - 1)) | ((req->nr_pages - 1) << 8));
	cmd = 0x000D3000 | (req->chip ? MV_NAND_NDCB_CSEL : 0);
	/* SEMA CHECK NFC COMMAND */
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW READ CMD 0 */
	cfgw.u_dat = cmd;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/*SEMA NFC COMMAND */
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/	
	/*CFGW REAd CMD 1 (address cycle 1:4) */
	cfgw.u_dat = src_4321;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* SEMA CHECK NFC Command */
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	*/
	/* CFGW READ CMD 2 (address cycle 5 and page count) */
	cfgw.u_dat = src_xxx5;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	for( j = 0;j < req->nr_pages; j++) {
		sema.u_pUpdId = 0;
		sema.u_pChkId = 0;
		sema.u_cUpdId = 0;
		sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
		sema.u_hdr = BCMINSFMT_hdr_SEMA;
		p_u32 = (unsigned int *)&sema;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;
			
		wcmd.u_ddrAdr = (unsigned int)host->dmaaddr + j*(mtd->writesize + host->oobsize) ;
		wcmd.u_size = mtd->writesize + host->oobsize;
		wcmd.u_chkSemId = 0;
		if( (j == (req->nr_pages -1)))
			wcmd.u_updSemId = PBSemaMap_dHubSemID_NFC_DATA_CP;
		else
			wcmd.u_updSemId = 0;
		wcmd.u_hdr = BCMINSFMT_hdr_WCMD;
		p_u32 = (unsigned int *)&wcmd;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;
			
		wdat.u_size = mtd->writesize + host->oobsize;
		wdat.u_mode = 2;
		wdat.u_endian = 0;
		wdat.u_last = 1;
		wdat.u_cUpdId = 0;
		wdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
		wdat.u_hdr = BCMINSFMT_hdr_WDAT;
		p_u32 = (unsigned int *)&wdat;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
		i += 4;
		MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
		i += 4;

	}
	dhub_channel_write_cmd(
			&host->PB_dhubHandle,				/*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funaddr),	/*!	CMD: buffer address !*/
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			0,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or */
			0							/*!	Pass in current cmdQ pointer (in 64b word), */
			);
	return 0;
}
int  mv_nand_read_status_dma(struct mv_nand_host *host , mv_nand_request_t * req)
{
	SIE_BCMCFGW cfgw;
	SIE_BCMWDAT wdat;
	SIE_BCMWCMD wcmd;
	SIE_BCMSEMA sema;
	unsigned int *p_u32;
    	
	unsigned int i = 0;
	//spin_lock_irq(&mv_work_lock);
	//printk("mv_nand_read_status_dma \n");
	memset(host->dmabuf , 0 ,8);
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW READ Status CMD 0 */
	cfgw.u_dat = 0x00800070;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/
	/* CFGW READ ID CMD 1 (address cycle 1:4)*/
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/	
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	
	wcmd.u_ddrAdr = (unsigned int)host->dmaaddr;
	wcmd.u_size = 8;
	wcmd.u_chkSemId = 0;
	wcmd.u_updSemId = PBSemaMap_dHubSemID_NFC_DATA_CP;
	wcmd.u_hdr = BCMINSFMT_hdr_WCMD;
	p_u32 = (unsigned int *)&wcmd;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* WDAT Read Status ID from dhub local to DDR */
	wdat.u_size = 8;
	wdat.u_mode = 2;
	wdat.u_endian = 0;
	wdat.u_last = 1;
	wdat.u_cUpdId = 0;
	wdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
	wdat.u_hdr = BCMINSFMT_hdr_WDAT;
	p_u32 = (unsigned int *)&wdat;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	dhub_channel_write_cmd(
			&host->PB_dhubHandle ,     /*!  Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,     /*! Channel ID in $dHubReg !*/
			(host->funaddr),         /*!  CMD: buffer address !*/
			i,                          /*! CMD: number of bytes to transfer !*/
			0,                          /*! CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,                          /*! CMD: non-zero to check semaphore !*/
			0,                          /*! CMD: non-zero to update semaphore !*/
			0,                          /*! CMD: raise interrupt at CMD finish !*/
			0,                          /*! Pass NULL to directly update dHub, or */
			0                           /*! Pass in current cmdQ pointer (in 64b word) */
			);
	//spin_unlock_irq(&mv_work_lock);
	return 0;

}

void mv_nand_erase_dma(struct mv_nand_host *host,int page)
{
	
	SIE_BCMCFGW cfgw;
	SIE_BCMSEMA sema;
	unsigned int i = 0 ,*p_u32;
	struct mtd_info *mtd = &host->mtd;
	unsigned int src_4321;
	src_4321 =  (page & ~((mtd->erasesize/ mtd->writesize) - 1));
	
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW erase CMD 0 */
	cfgw.u_dat = 0x004BD060;
	cfgw.u_devAdr = ((int)(host->io_base + MV_NAND_NDCB0_OFFSET)) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/
	/* CFGW erase CMD 1 (address cycle 1:4)*/
	cfgw.u_dat = src_4321;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

*/
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	dhub_channel_write_cmd(
			&host->PB_dhubHandle ,     /*!  Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,     /*! Channel ID in $dHubReg !*/
			(host->funaddr),         /*!  CMD: buffer address !*/
			i,                          /*! CMD: number of bytes to transfer !*/
			0,                          /*! CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,                          /*! CMD: non-zero to check semaphore !*/
			0,                          /*! CMD: non-zero to update semaphore !*/
			0,                          /*! CMD: raise interrupt at CMD finish !*/
			0,                          /*! Pass NULL to directly update dHub, or */
			0                           /*! Pass in current cmdQ pointer (in 64b word) */
			);
}

void mv_nand_reset_dma(struct mv_nand_host *host)
{
	
	SIE_BCMCFGW cfgw;
	SIE_BCMSEMA sema;
	unsigned int i = 0,*p_u32;
	
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* CFGW reset  CMD 0 */
	cfgw.u_dat = 0x00A000FF;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/
	/* CFGW reset CMD 1 (address cycle 1:4)*/
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	dhub_channel_write_cmd(
			&host->PB_dhubHandle ,     /*!  Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,     /*! Channel ID in $dHubReg !*/
			(host->funaddr),         /*!  CMD: buffer address !*/
			i,                          /*! CMD: number of bytes to transfer !*/
			0,                          /*! CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,                          /*! CMD: non-zero to check semaphore !*/
			0,                          /*! CMD: non-zero to update semaphore !*/
			0,                          /*! CMD: raise interrupt at CMD finish !*/
			0,                          /*! Pass NULL to directly update dHub, or */
			0                           /*! Pass in current cmdQ pointer (in 64b word) */
			);
}

void mv_nand_read_id_dma(struct mv_nand_host *host , char *id_dst)
{
	SIE_BCMCFGW cfgw;
	SIE_BCMWDAT wdat;
	SIE_BCMWCMD wcmd;
	SIE_BCMSEMA sema;
	unsigned int *p_u32;
	unsigned int i = 0;
	//HDL_semaphore *pSemHdl;
	//spin_lock_irq(&mv_work_lock);

	memset(host->dmabuf,0,8);
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	/* CFGW READ ID CMD 0 */
	cfgw.u_dat = 0x00610090;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

/*	
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/
	/* CFGW READ ID CMD 1 (address cycle 1:4)*/
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

/*
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCCmd;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
*/
	/* CFGW READ ID CMD 2 (address cycle 5 and page count)*/
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCB0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	
	wcmd.u_ddrAdr = (unsigned int )(host->dmaaddr);
	wcmd.u_size = 8;
	wcmd.u_chkSemId = 0;
	wcmd.u_updSemId = PBSemaMap_dHubSemID_NFC_DATA_CP;
	wcmd.u_hdr = BCMINSFMT_hdr_WCMD;
	p_u32 = (unsigned int *)&wcmd;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* WDAT Read Status ID from dhub local to DDR */

	wdat.u_size = 8;
	wdat.u_mode = 2;
	wdat.u_endian = 0;
	wdat.u_last = 1;
	wdat.u_cUpdId = 0;
	wdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
	wdat.u_hdr = BCMINSFMT_hdr_WDAT;
	p_u32 = (unsigned int *)&wdat;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
	dhub_channel_write_cmd(
			&host->PB_dhubHandle ,     /*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funaddr),	       /*!	CMD: buffer address !*/
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			0,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or */
			0							/*!	Pass in current cmdQ pointer (in 64b word) */
			);
	//spin_unlock_irq(&mv_work_lock);
	mv_nand_dma_wait_read_done(host);

}


void mv_end_sync_rq(void *req,int error)
{
	mv_nand_request_t *rq =  (mv_nand_request_t *)req;
	struct completion *waiting = rq->end_request_data;
	if(rq->end_request_data == NULL)
		return;
	
	rq->end_request_data = NULL;
	complete(waiting);
}

static void done(struct mv_nand_host *host, mv_nand_request_t *req, int status)
{
	
	/*wake up the waiting process */
	if(req->end_request) {
		req->end_request(req,0);
	}
}

void diag_PB_Nand_clear_interrupt(void)
{
	int read;
	
	read = MV_NAND_REG_READ_DWORD(MEMMAP_PB_REG_BASE + RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub+ RA_SemaHub_Query  ,(0x80 |  PBSemaMap_dHubSemID_NFC_DATA_CP <<2));
	if((read & 0xFFFF)==1)
	    MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE, RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_POP, (0x100 | PBSemaMap_dHubSemID_NFC_DATA_CP));
	MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE, RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_full,(1 << PBSemaMap_dHubSemID_NFC_DATA_CP));
}

static irqreturn_t mv_nand_dma_intr(int irq, void *dev_id)
{
	struct mv_nand_host *host = (struct mv_nand_host *)dev_id;
	mv_nand_request_t *curq;
	diag_PB_Nand_clear_interrupt();
	if(!(curq = host->request_head.cur_request))
		return IRQ_NONE;
	tasklet_schedule(&host->request_head.finish_tasklet);
	return IRQ_HANDLED;

}

#endif

static int mv_nand_get_device(struct nand_chip *chip, struct mtd_info *mtd, int new_state);

static int mv_nand_do_write_oob(struct mtd_info *mtd, loff_t to, struct mtd_oob_ops *ops);

static void mv_nand_command(struct mtd_info *mtd, unsigned int command, int column, int page_addr);

static int mv_nand_read_status(struct mtd_info *mtd,int nc);

static int mv_nand_read_status_nolock(struct mtd_info *mtd,int nc);

static int mv_nand_chip_init(struct mv_nand_host *host);

static void mv_nand_cmd_request(struct nand_chip *chip);
	
int mv_nand_read_page_bch_random(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf);

int mv_nand_read_page_raw_random(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf);

static int mv_nand_write_page_lp(struct mtd_info *mtd, struct nand_chip *chip,const uint8_t *buf, int page, int cached, int raw);

#define MV_NAND_DEBUG 1
static void dump_registers(struct nand_chip *chip)
{
#ifdef MV_NAND_DEBUG
	unsigned int read;

	// Read back for sanity check
	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDTR0CS0_OFFSET);				
	galois_printk("NDTR0CS0 reads 0x%X\n", read);
	
	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDTR1CS0_OFFSET);				
	galois_printk("NDTR1CS0 reads 0x%X\n", read);
	
	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDREDEL_OFFSET);				
	galois_printk("NDREDEL reads 0x%X\n", read);
	
	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);					
	galois_printk("NDSR reads 0x%X\n", read);
	
	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDCR_OFFSET);					
	galois_printk("NDCR reads 0x%X\n", read);

	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDECCCTRL_OFFSET);
	galois_printk("NDECCCTRL reads 0x%X\n", read);
#endif
	
}


/**
 * mv_nand_release_device - [GENERIC] release chip
 * @mtd:	MTD device structure
 *
 * Deselect, release chip lock and wake up anyone waiting on the device
 */
static void mv_nand_release_device(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;

	/* De-select the NAND device */
	chip->select_chip(mtd, -1);

	/* Release the controller and the chip */
	spin_lock(&chip->controller->lock);
	chip->controller->active = NULL;
	chip->state = FL_READY;
	wake_up(&chip->controller->wq);
	spin_unlock(&chip->controller->lock);
}

/**
 * mv_nand_read_byte - [DEFAULT] read one byte from the chip
 * @mtd:	MTD device structure
 *
 * Default read function for 8bit buswith
 */
static uint8_t mv_nand_read_byte(struct mtd_info *mtd)
{
//	struct nand_chip *chip = mtd->priv;
	return 0;
}

/**
 * mv_nand_read_word - [DEFAULT] read one word from the chip
 * @mtd:	MTD device structure
 *
 * Default read function for 16bit buswith without
 * endianess conversion
 */
static u16 mv_nand_read_word(struct mtd_info *mtd)
{
//	struct nand_chip *chip = mtd->priv;
	return 0;
}
/**
 * mv_nand_write_buf - [DEFAULT] write buffer to chip
 * @mtd:	MTD device structure
 * @buf:	data buffer
 * @len:	number of bytes to write
 *
 * Default write function for 8bit buswith
 */
static void mv_nand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	int i;
	struct nand_chip *chip = mtd->priv;
	unsigned int *buf_tsf = (unsigned int *)buf;

	for (i = 0; i < len/sizeof(int); i++)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_R,MV_NAND_NDDB_OFFSET,buf_tsf[i]);
}

/**
 * mv_nand_read_buf - [DEFAULT] read chip data into buffer
 * @mtd:	MTD device structure
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 *
 * Default read function for 8bit buswith
 */
static void mv_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	int i;
	struct nand_chip *chip = mtd->priv;
	unsigned int *buf_tsf = (unsigned int *)buf;
	//printk("read buf\n");  
	for (i = 0; i < len/sizeof(int); i++){
		buf_tsf[i] = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_R,MV_NAND_NDDB_OFFSET);
		//printk("nand read buf i = %d buf =%x \n",i,buf_tsf[i]);
	}
}
static void mv_nand_start(struct nand_chip *chip)
{
    unsigned int read;
	MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET , MV_NAND_NDSR_VAL);
	/* set ND_RUN bit */
	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET);
	MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET, (read | MV_NAND_NDCR_ND_RUN));
	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET);
}
static void mv_nand_disable(struct nand_chip *chip)
{
	unsigned int read;
	MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET , MV_NAND_NDSR_VAL);
	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET);
	MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET, (read &(~ MV_NAND_NDCR_ND_RUN)));
}
/*
 * Hardware specific access to control-lines
 */
static void mv_nand_cmd_ctrl(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *nand_chip = mtd->priv;
	struct mv_nand_host *host = nand_chip->priv;
	unsigned int read,ctrlcmd;
	if (cmd == NAND_CMD_NONE)
		return;
	if(MV_NAND_CMD_NFC==cmd){
		/* stop NFC */
		read = MV_NAND_REG_READ_DWORD(host->io_base, MV_NAND_NDCR_OFFSET);
		read &= ~MV_NAND_NDCR_ND_RUN;
		MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDCR_OFFSET, read);
		/* clear NFC status register*/
		read = MV_NAND_REG_READ_DWORD(host->io_base, MV_NAND_NDSR_OFFSET);
		MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDSR_OFFSET, read);
	} else if(MV_NAND_CMD_CCP==cmd){
		do{
			/* read status register */
			read = MV_NAND_REG_READ_DWORD(host->io_base,MV_NAND_NDSR_OFFSET);	
			//printk("MV_NAND_CMD_CCP read 1=0x%x\n",read);
			read &= (host->board->ncs ? (MV_NAND_NDSR_CS1_CMDD | MV_NAND_NDSR_CS1_PAGED) : (MV_NAND_NDSR_CS0_CMDD | MV_NAND_NDSR_CS0_PAGED));
			/*  check CSx_CMDD PAGED bit */
		} while (!read);														
		/* write 1 to clear sticky bits (CSx_CMDD and CSx_PAGED) */
		MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDSR_OFFSET, read);
	} else if(MV_NAND_CMD_STC==cmd) {
		/* make the chip start to receive cmd*/
		//mv_nand_start(nand_chip);
	} else if(MV_NAND_CMD_RTS==cmd) {
		/* Clear Command-Done bit CSx_CMDD */
		do {		
			/* read status register */
			read = MV_NAND_REG_READ_DWORD(host->io_base,MV_NAND_NDSR_OFFSET);
			//printk("MV_NAND_CMD_RTS read =0x%x\n",read);
			read &= (host->board->ncs ? (MV_NAND_NDSR_CS1_CMDD) : (MV_NAND_NDSR_CS0_CMDD ));
		} while (!read);
		MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDSR_OFFSET, read);			
	}else if(MV_NAND_CMD_ENP == cmd) {
		/* Clear Page-Done bit CSx_CMDD */
		if(!((host->curcmd == NAND_CMD_READ0) && (host->board->conbch & 0x7F))) {
		    do {
				/* read status register */
				read = MV_NAND_REG_READ_DWORD(host->io_base,MV_NAND_NDSR_OFFSET);
				read &= (host->board->ncs ? (MV_NAND_NDSR_CS1_PAGED) : (MV_NAND_NDSR_CS0_PAGED ));
			}while (!read);
			MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDSR_OFFSET, read);
		}
	}else if(MV_NAND_CMD_AUT == cmd){
		do {
			read = MV_NAND_REG_READ_DWORD(host->io_base,MV_NAND_NDSR_OFFSET);
			read &=(host->board->ncs?MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0);
		} while(!read);
		MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDSR_OFFSET, read);
		/* Clear Command-Done bit CSx_CMDD */
		do {
			read = MV_NAND_REG_READ_DWORD(host->io_base,MV_NAND_NDSR_OFFSET);
			read &= (host->board->ncs ? ( MV_NAND_NDSR_CS1_PAGED) : (MV_NAND_NDSR_CS0_PAGED));	
		} while (!read);
		/*  write 1 to clear sticky bit (CSx_CMDD only) */
		MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDSR_OFFSET, read);
	}else if (MV_NAND_CMD_ANR == cmd) {
		do {
			read = MV_NAND_REG_READ_DWORD(host->io_base,MV_NAND_NDSR_OFFSET);
		} while(!(read & MV_NAND_NDSR_RDDREQ));
		ctrlcmd = MV_NAND_NDSR_RDDREQ;
		if(host->board->conbch & 0x7F) {
			if(read & MV_NAND_NDSR_CORERR) {
				ctrlcmd |= MV_NAND_NDSR_CORERR;
			}
			if(read & MV_NAND_NDSR_UNCERR) {
				ctrlcmd |= MV_NAND_NDSR_UNCERR;
				mtd->ecc_stats.failed ++;
			}

		}
		MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDSR_OFFSET, ctrlcmd);
	}
	return ;
}
/**
 * mv_nand_default_block_markbad - [DEFAULT] mark a block bad
 * @mtd:	MTD device structure
 * @ofs:	offset from device start
 *
 * This is the default implementation, which can be overridden by
 * a hardware specific driver.
*/
static int mv_nand_default_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
	struct nand_chip *chip = mtd->priv;
	uint8_t buf[2] = { 0, 0 };
	int block, ret;
	printk("mv_nand_default_block_markbad ofs =0x%08Lx\n",(unsigned long long)ofs);
	/* Get block number */
	block = ((int)ofs) >> chip->bbt_erase_shift;
	if (chip->bbt)
		chip->bbt[block >> 2] |= 0x01 << ((block & 0x03) << 1);

	/* Do we have a flash based bad block table ? */
	if (chip->options & NAND_USE_FLASH_BBT)
		ret = mv_nand_update_bbt(mtd, ofs);
	else {
		/* We write two bytes, so we dont have to mess with 16 bit
		 * access
		 */
		ofs += mtd->oobsize;
		chip->ops.len = chip->ops.ooblen = 2;
		chip->ops.datbuf = NULL;
		chip->ops.oobbuf = buf;
		chip->ops.ooboffs = chip->badblockpos & ~0x01;

		ret = mv_nand_do_write_oob(mtd, ofs, &chip->ops);
	}
	if (!ret)
		mtd->ecc_stats.badblocks++;
	return ret;
}
/**
 * mv_nand_block_bad - [DEFAULT] Read bad block marker from the chip
 * @mtd:	MTD device structure
 * @ofs:	offset from device start
 * @getchip:	0, if the chip is already selected
 *
 * Check, if the block is bad.
 */
static int mv_nand_block_bad(struct mtd_info *mtd, loff_t ofs, int getchip)
{
	int pages_per_block ,page = 0, chipnr, res = 0;
	struct nand_chip *chip = mtd->priv;
	struct mv_nand_host *host = chip->priv;
	u16 bad;
	uint8_t *buf = host->buf;
	//printk("mv_nand_block_bad ofs = 0x%08Lx,getchip =0x%x\n",(unsigned long long )ofs,getchip);
	pages_per_block = 1 << (chip->phys_erase_shift - chip->page_shift);
	
	//printk(" check block page = 0x%x\n",page);
	if (getchip) {
		page = (int)(ofs >> chip->page_shift);
		chipnr = (int)(ofs >> chip->chip_shift);

		mv_nand_get_device(chip, mtd, FL_READING);

		/* Select the NAND device */
		chip->select_chip(mtd, chipnr);
	} 
    
	else
		page = (int)((ofs >> chip->page_shift)& ((((unsigned long long)(1 << chip->chip_shift)) >> chip->page_shift) - 1));

	mutex_lock(&mv_nand_mutex);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
	if (chip->options & NAND_BUSWIDTH_16) {
		chip->cmdfunc(mtd, NAND_CMD_READOOB, chip->badblockpos & 0xFE,page & chip->pagemask);
		bad = cpu_to_le16(chip->read_word(mtd));
		if (chip->badblockpos & 0x1)
			bad >>= 8;
		if ((bad & 0xFF) != 0xff)
			res = 1;
	} else {
		chip->cmdfunc( mtd, NAND_CMD_READ0, 0, page);
		host->read_page(mtd, chip, buf);
		/*  PAGE DONE */
		chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
		if((buf[chip->badblockpos + mtd->writesize] & 0xFF)!= 0xff){
			printk("bad block chip->badblockpos = 0x%x page =0x%x\n",chip->badblockpos,page);
			printk("ofs = 0x%08Lx data = 0x%x chip->page_shift =0x%x\n",(unsigned long long)ofs,buf[chip->badblockpos + mtd->writesize],chip->page_shift);
			res = 1;
		}
	}
	 /* STOP NFC && Clear NFC status register */
	 chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
	 mutex_unlock(&mv_nand_mutex);
	 if (getchip)
		mv_nand_release_device(mtd);
	 return res;
	
}


/**
 * mv_nand_block_bad_ab - [DEFAULT] Read bad block marker from the chip after probe
 * @mtd:	MTD device structure
 * @ofs:	offset from device start
 * @getchip:	0, if the chip is already selected
 *
 * Check, if the block is bad.
 */
static int mv_nand_block_bad_ab(struct mtd_info *mtd, loff_t ofs, int getchip)
{
	int pages_per_block ,page = 0, chipnr, res = 0;
	struct nand_chip *chip = mtd->priv;
	struct mv_nand_host *host = chip->priv;
	u16 bad;
	uint8_t *buf = host->buf;
	pages_per_block = 1 << (chip->phys_erase_shift - chip->page_shift);
	
	if (getchip) {
		page = (int)(ofs >> chip->page_shift);
		chipnr = (int)(ofs >> chip->chip_shift);

		mv_nand_get_device(chip, mtd, FL_READING);

		/* Select the NAND device */
		chip->select_chip(mtd, chipnr);
	} 
    
	else
		page = (int)((ofs >> chip->page_shift)& ((((unsigned long long)(1 << chip->chip_shift)) >> chip->page_shift) - 1));

#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	mutex_lock(&mv_nand_mutex);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
#endif
	if (chip->options & NAND_BUSWIDTH_16) {
		chip->cmdfunc(mtd, NAND_CMD_READOOB, chip->badblockpos & 0xFE,page & chip->pagemask);
		bad = cpu_to_le16(chip->read_word(mtd));
		if (chip->badblockpos & 0x1)
			bad >>= 8;
		if ((bad & 0xFF) != 0xff)
			res = 1;
	} else {
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
		chip->cmdfunc( mtd, NAND_CMD_READ0, 0, page);
		host->read_page(mtd, chip, buf);
		/*  PAGE DONE */
		chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
#else
		mv_nand_make_request(host,MV_DMA_CMD_READ,page,1,buf, NULL,1);
#endif
		if((buf[chip->badblockpos + mtd->writesize] & 0xFF)!= 0xff){
			printk("bad block chip->badblockpos = 0x%x page =0x%x\n",chip->badblockpos,page);
			printk("ofs = 0x%08Lx data = 0x%x chip->page_shift =0x%x\n",(unsigned long long)ofs,buf[chip->badblockpos + mtd->writesize],chip->page_shift);
			res = 1;
		}
	}
	 /* STOP NFC && Clear NFC status register */
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	 chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
	 mutex_unlock(&mv_nand_mutex);
#endif
	 if (getchip)
		mv_nand_release_device(mtd);
	 return res;
	
}
/**
 * mv_nand_wait - [DEFAULT]  wait until the command is done
 * @mtd:	MTD device structure
 * @chip:	NAND chip structure
 *
 * Wait for command done. This applies to erase and program only
 * Erase can take up to 400ms and program up to 20ms according to
 * general NAND and SmartMedia specs
 */
static int mv_nand_wait(struct mtd_info *mtd, struct nand_chip *chip)
{

	unsigned long timeo = jiffies;
	int status = 0,state;
	state = chip->state;
	if (state == FL_ERASING)
		timeo += (HZ * 400) / 1000;
	else
		timeo += (HZ * 20) / 1000;

	led_trigger_event(mv_nand_led_trigger, LED_FULL);

	/* Apply this short delay always to ensure that we do wait tWB in
	 * any case on any machine. */
	ndelay(100);

	while (time_before(jiffies, timeo)) {
		status = mv_nand_read_status_nolock(mtd,1);
		//printk("mv_nand0_wait status = 0x%x\n",status);
		/* if ready */
		//statusre = MV_NAND_REG_READ_DWORD(host->io_base, MV_NAND_NDSR_OFFSET); 
		//if((status & NAND_STATUS_READY)||(statusre &(host->board->ncs ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0)))
		if(((status & NAND_STATUS_READY)&&(status != 0xff)))
			break;
		cond_resched();
	}
	led_trigger_event(mv_nand_led_trigger, LED_OFF);


	return status;
}
/**
 * mv_nand_verify_buf - [DEFAULT] Verify chip data against buffer
 * @mtd:	MTD device structure
 * @buf:	buffer containing the data to compare
 * @len:	number of bytes to compare
 *
 * Default verify function for 8bit buswith
 */
static int mv_nand_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	int i;
	struct nand_chip *chip = mtd->priv;
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	uint8_t *tmpbuf = host->buf;
	for (i = 0; i < len; i++) {
		if(tmpbuf[i] != buf[i]){
			//printk("verify buffer start i = 0x%x error\n",i);
#if 0
			for(j = i;j < len;j++){
				if(j%8==0)
					printk("\n");
				if(tmpbuf[j] != buf[j])
				printk("j = 0x%x 0x%x 0x%x",j,tmpbuf[j],buf[j]);
			}
#endif
			return -EFAULT;
		}
	}
	
	return 0;
}
/**
 * mv_nand_select_chip - [DEFAULT] control CE line
 * @mtd:	MTD device structure
 * @chipnr:	chipnumber to select, -1 for deselect
 *
 * Default select function for 1 chip devices.
 */
static void mv_nand_select_chip(struct mtd_info *mtd, int chipnr)
{
	struct nand_chip *chip = mtd->priv;
    struct mv_nand_host *host = chip->priv;
	switch (chipnr) {
	case -1:
		//chip->cmd_ctrl(mtd, NAND_CMD_NONE, 0 | NAND_CTRL_CHANGE);
		break;
	case 0:
	case 1:
        host->board->ncs = chipnr? 1 : 0;
		//host->board->ncs = 1;
		break;
	default:
		BUG();
	}
}
/*
 * Set default functions
 */
static void mv_nand_set_defaults(struct nand_chip *chip, int busw)
{
	/* check for proper chip_delay setup, set 20us if not */
	if (!chip->chip_delay)
		chip->chip_delay = 20;

	/* check, if a user supplied command function given */
	//if (chip->cmdfunc == NULL)
	//	chip->cmdfunc = mv_nand_command;

	/* check, if a user supplied wait function given */
	if (chip->waitfunc == NULL)
		chip->waitfunc = mv_nand_wait;

	if (!chip->select_chip)
		chip->select_chip = mv_nand_select_chip;
	if (!chip->read_byte)
		chip->read_byte =  mv_nand_read_byte;
	if (!chip->read_word)
		chip->read_word = mv_nand_read_word;
	/* 20101013 After set dma mode ,it is necessary to avoid IO access 
	 */
	if (!chip->block_bad)
		chip->block_bad = mv_nand_block_bad_ab;
	if (!chip->block_markbad)
		chip->block_markbad = mv_nand_default_block_markbad;
	if (!chip->write_buf)
		chip->write_buf =  mv_nand_write_buf;
	if (!chip->read_buf)
		chip->read_buf =  mv_nand_read_buf;
	if (!chip->verify_buf)
		chip->verify_buf =  mv_nand_verify_buf;
	if (!chip->scan_bbt)
		chip->scan_bbt = nand_default_bbt;

	if (!chip->controller) {
		chip->controller = &chip->hwcontrol;
		spin_lock_init(&chip->controller->lock);
		init_waitqueue_head(&chip->controller->wq);
	}

}
/**
 * mv_multi_erease_cmd - [GENERIC] AND specific block erase command function
 * @mtd:	MTD device structure
 * @page:	the page address of the block which will be erased
 *
 * AND multi block erase command function
 * Erase 4 consecutive blocks
 */
static void mv_multi_erase_cmd(struct mtd_info *mtd, int page)
{
	//struct nand_chip *chip = mtd->priv;
	/* Send commands to erase a block */
	#if 0
	chip->cmdfunc(mtd, NAND_CMD_ERASE1, -1, page++);
	chip->cmdfunc(mtd, NAND_CMD_ERASE1, -1, page++);
	chip->cmdfunc(mtd, NAND_CMD_ERASE1, -1, page++);
	chip->cmdfunc(mtd, NAND_CMD_ERASE1, -1, page);
	chip->cmdfunc(mtd, NAND_CMD_ERASE2, -1, -1);
	#endif
}

/**
 * mv_single_erease_cmd - [GENERIC] NAND standard block erase command function
 * @mtd:	MTD device structure
 * @page:	the page address of the block which will be erased
 *
 * Standard erase command for NAND chips
 */
static void mv_single_erase_cmd(struct mtd_info *mtd, int page)
{
	struct nand_chip *chip = mtd->priv;
	/* Send commands to erase a block */
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	mutex_lock(&mv_nand_mutex);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
	chip->cmdfunc(mtd, NAND_CMD_ERASE1, -1, page);
	chip->waitfunc(mtd,chip);
	mutex_unlock(&mv_nand_mutex);
#else
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
#if 0
	mv_nand_dma_exrw_init(host);	
	mv_nand_erase_dma(host ,page);
	mv_nand_clear_nfcrun(host);
	
#endif
	mv_nand_make_request(host,MV_DMA_CMD_ERASE,page,0,NULL, NULL,0);
	chip->waitfunc(mtd,chip);
#endif
	
}
static int mv_set_oobsize(struct mtd_info *mtd,struct mv_nand_host *host)
{
	if(mtd->writesize == 2048) {
		/* SPARE_EN = 0 */
		if (!(host->board->conspare & 1))				
			host->oobsize  = MV_NAND_SPARE_2048_0xx;
		else if (!(host->board->conecc & 1))		
			/* SPARE_EN = 1 && ECC_EN = 0 */
			host->oobsize = MV_NAND_SPARE_2048_10x;
		else if (!(host->board->conbch & 1))
			/* SPARE_EN = 1 && ECC_EN = 1 && BCH_EN = 0 */
			host->oobsize = MV_NAND_SPARE_2048_110;
		else
			/* SPARE_EN = 1 && ECC_EN = 1 && BCH_EN = 1 */
			host->oobsize = MV_NAND_SPARE_2048_111;

		mtd->oobsize = MV_NAND_SPARE_2048_10x;
		
	} else if(mtd->writesize == 4096) {
		if (!(host->board->conspare & 1))
			host->oobsize  = MV_NAND_SPARE_4096_0xx;
		else if (!(host->board->conecc & 1)) {
			printk("don's support such ecc mode spare == 1 && ECC == 0\n");
			return -1;
		}
		else if (!(host->board->conbch & 1))
			host->oobsize = MV_NAND_SPARE_4096_110;
		else
			host->oobsize = MV_NAND_SPARE_4096_111;

		mtd->oobsize = MV_NAND_SPARE_4096_SIZE;
	
	} else if(mtd->writesize == 512) {
		if(host->board->conbch)
			host->board->conbch = 0;
		host->oobsize = 8;
		mtd->oobsize = 8;
	} else {
		printk("don't support writesize = 0x%x\n",mtd->writesize);
		return -1;
	}
	
	return 0;
}
/*
 * Get the flash and manufacturer id and lookup if the type is supported
 */
static struct nand_flash_dev *mv_nand_get_flash_type(struct mtd_info *mtd,
						  struct nand_chip *chip,
						  int busw, int *maf_id)
{
	struct nand_flash_dev *type = NULL;
	int i, dev_id, maf_idx;
	unsigned int tmp[2];
	struct mv_nand_host *host = chip->priv;

	/* Select the device */
	chip->select_chip(mtd, 0);

	
	//mutex_lock(&mv_nand_mutex);
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
	/* Send the command for reading device ID */
	chip->cmdfunc(mtd, NAND_CMD_READID, 0x00, -1);

	/* Read manufacturer and device IDs */
	chip->read_buf(mtd,(uint8_t *)&tmp[0],sizeof(unsigned int)*2);

	chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
#else
	
	mv_nand_dma_exrw_init(host);
	mv_nand_read_id_dma(host,(char *)&tmp[0]);
	//mv_nand_dma_wait_read_done(host);
	memcpy((char *)&tmp[0] , host->dmabuf , 8);
	for(i = 0 ; i < 8; i++) {
		printk("0x%x ",host->dmabuf[i]);
	}
#if 0
	mv_nand_dma_exrw_init(host);
	mv_nand_read_status_dma(host,(char *)&tmp[0]);
	mv_nand_dma_wait_read_done(host);
	for(i = 0 ; i < 8; i++) {
		printk("0x%x ",host->dmabuf[i]);
	}
#endif
#endif
	//dump_registers(&host->nand_chip);
	//mutex_unlock(&mv_nand_mutex); 
	*maf_id = tmp[0]&0xff;
	
	dev_id = (tmp[0]>>8)&0xff;
	galois_printk("NAND dev_id=%x,ID[0]=0x%08x,ID[1]=0x%08x\n",dev_id,tmp[0],tmp[1]);
	
	/* Lookup the flash id */
	for (i = 0; nand_flash_ids[i].name != NULL; i++) {
		if ((dev_id == nand_flash_ids[i].id)) {
			type =  &nand_flash_ids[i];
			break;
		}
	}

	if (!type)
		return ERR_PTR(-ENODEV);

	if (!mtd->name)
		mtd->name = type->name;

	chip->chipsize = type->chipsize << 20;

	/* Newer devices have all the information in additional id bytes */
	if (!type->pagesize) {
		int extid;
		/* it is SAMGSUNG chip and support cache program */
		if((NAND_MFR_SAMSUNG == *maf_id) && ((tmp[0] >> 16)&0x80)) {
			/* The 3rd id byte holds MLC / multichip data */
			chip->cellinfo = (tmp[0]>>16)&0xff;
			/* The 4th id byte is the important one */
			extid = (tmp[0]>>24)&0xff;
			/* Calc pagesize */
			mtd->writesize = 2048 << (extid & 0x3);
			extid >>= 2;
			
			mv_set_oobsize(mtd,host);
			/* Calc blocksize. Blocksize is multiples of 64KiB */
			mtd->erasesize = (128 * 1024) << (extid & 0x03);
			extid >>= 2;
			busw = 1;
		} else if ((NAND_MFR_HYNIX == *maf_id) && ((tmp[0]>>16)&0x80)) {
            /* The 3rd id byte holds MLC / multichip data */
            chip->cellinfo = (tmp[0]>>16)&0xff;
            /* The 4th id byte is the important one */
            extid = (tmp[0]>>24)&0xff;
            /* Calc pagesize */
            mtd->writesize = 2048 << (extid & 0x3);
			mv_set_oobsize(mtd,host);
            switch (extid & 0xB0) {
                case 0:
                    mtd->erasesize = 128 * 1024;
                    break;
                case 0x10:
                    mtd->erasesize = 2 * 128 * 1024;
                    break;
                case 0x20:
                    mtd->erasesize = 4 * 128 * 1024;
                    break;
                case 0x30:
                    mtd->erasesize = 6 * 128 * 1024;
                    break;
                case 0x80:
                    mtd->erasesize = 8 * 128 * 1024;
                    break;
            }
            busw = 1;
		} else {
			/* The 3rd id byte holds MLC / multichip data */
			chip->cellinfo = (tmp[0]>>16)&0xff;
			/* The 4th id byte is the important one */
			extid = (tmp[0]>>24)&0xff;
			/* Calc pagesize */
			mtd->writesize = 1024 << (extid & 0x3);
			extid >>= 2;
			/* Calc oobsize */
			mv_set_oobsize(mtd,host);
			extid >>= 2;
			/* Calc blocksize. Blocksize is multiples of 64KiB */
			mtd->erasesize = (64 * 1024) << (extid & 0x03);
			extid >>= 2;
			/* Get buswidth information */
			busw = (extid & 0x01) ? NAND_BUSWIDTH_16 : 0;
			if((NAND_MFR_SAMSUNG == *maf_id) && (dev_id == 0xd5)) {
				chip->chipsize = ((64 << ((tmp[1] >> 4) & 0x7)) << 17) * ( 1 << ((tmp[1] >> 2) & 0x3));
			}
		}

	} else {
		/*
		 * Old devices have chip data hardcoded in the device id table
		 */
		if((NAND_MFR_SAMSUNG == *maf_id) && (dev_id == 0xd5)) {
			int extid;	
			/* The 3rd id byte holds MLC / multichip data */
			chip->cellinfo = (tmp[0]>>16)&0xff;
			/* The 4th id byte is the important one */
			extid = (tmp[0]>>24)&0xff;
			/* Calc pagesize */
			mtd->writesize = 1024 << (extid & 0x3);
			extid >>= 2;
			/* Calc oobsize */
			mv_set_oobsize(mtd,host);
			extid >>= 2;
			/* Calc blocksize. Blocksize is multiples of 64KiB */
			mtd->erasesize = (64 * 1024) << (extid & 0x03);
			extid >>= 2;
			/* Get buswidth information */
			busw = (extid & 0x01) ? NAND_BUSWIDTH_16 : 0;
			chip->chipsize = ((64 << ((tmp[1] >> 4) & 0x7)) << 17) * ( 1 << ((tmp[1] >> 2) & 0x3));
		} else {
			mtd->erasesize = type->erasesize;
			mtd->writesize = type->pagesize;
			mtd->oobsize = mtd->writesize / 32;
			host->oobsize = 8;
			if(host->board->conbch) {
				host->board->conbch = 0;
				chip->ecc.mode = NAND_ECC_HW;
			}
		}
		//busw = type->options & NAND_BUSWIDTH_16;
	
	}
	printk("host->oobsize = 0x%x mtd->oobsize =0x%x\n",host->oobsize,mtd->oobsize);
  
	/* Try to identify manufacturer */
	for (maf_idx = 0; nand_manuf_ids[maf_idx].id != 0x0; maf_idx++) {
		if ((nand_manuf_ids[maf_idx].id == *maf_id))
			break;
	}
  
    #if 0
	/*
	 * Check, if buswidth is correct. Hardware drivers should set
	 * chip correct !
	 */
	if (busw != (chip->options & NAND_BUSWIDTH_16)) {
		printk(KERN_INFO "NAND device: Manufacturer ID:"
		       " 0x%02x, Chip ID: 0x%02x (%s %s)\n", *maf_id,
		       dev_id, nand_manuf_ids[maf_idx].name, mtd->name);
		printk(KERN_WARNING "NAND bus width %d instead %d bit\n",
		       (chip->options & NAND_BUSWIDTH_16) ? 16 : 8,
		       busw ? 16 : 8);
		return ERR_PTR(-EINVAL);
	}
    #endif

	/* Calculate the address shift from the page size */
	chip->page_shift = ffs(mtd->writesize) - 1;
  			
	/* Convert chipsize to number of pages per chip -1. */
	chip->pagemask = (chip->chipsize >> chip->page_shift) - 1;

	chip->bbt_erase_shift = chip->phys_erase_shift =
		ffs(mtd->erasesize) - 1;
	chip->chip_shift = ffs(chip->chipsize) - 1;

	/* Set the bad block position */
	chip->badblockpos = 0;//mtd->writesize > 512 ?NAND_LARGE_BADBLOCK_POS : NAND_SMALL_BADBLOCK_POS;

	/* Get chip options, preserve non chip based options */
	chip->options &= ~NAND_CHIPOPTIONS_MSK;
	chip->options |= type->options & NAND_CHIPOPTIONS_MSK;

	/*
	 * Set chip as a default. Board drivers can override it, if necessary
	 */
	chip->options |= NAND_NO_AUTOINCR;

	/* Check if chip is a not a samsung device. Do not clear the
	 * options for chips which are not having an extended id.
	 */
	if (*maf_id != NAND_MFR_SAMSUNG && !type->pagesize)
		chip->options &= ~NAND_SAMSUNG_LP_OPTIONS;

	/* Check for AND chips with 4 page planes */
	if (chip->options & NAND_4PAGE_ARRAY)
		chip->erase_cmd = mv_multi_erase_cmd;
	else
		chip->erase_cmd = mv_single_erase_cmd;

	/* Do not replace user supplied command function ! */

	galois_printk(KERN_INFO "NAND device: Manufacturer ID:"
	       " 0x%02x, Chip ID: 0x%02x (%s %s)\n", *maf_id, dev_id,
	       nand_manuf_ids[maf_idx].name, type->name);

	return type;
}

/**
 * mv_nand_scan_ident - [NAND Interface] Scan for the NAND device
 * @mtd:	     MTD device structure
 * @maxchips:	     Number of chips to scan for
 *
 * This is the first phase of the normal nand_scan() function. It
 * reads the flash ID and sets up MTD fields accordingly.
 *
 * The mtd->owner field must be set to the module of the caller.
 */
int mv_nand_scan_ident(struct mtd_info *mtd, int maxchips)
{
	int i, busw, nand_maf_id;
	struct nand_chip *chip = mtd->priv;
	struct nand_flash_dev *type;
	unsigned int tmp[2];
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	struct mv_nand_host *host = chip->priv;
#endif
	/* Get buswidth to select the correct functions */
	busw = chip->options & NAND_BUSWIDTH_16;
	/* Set the default functions */
	mv_nand_set_defaults(chip, busw);

	/* Read the flash type */
	type = mv_nand_get_flash_type(mtd, chip, busw, &nand_maf_id);

	if (IS_ERR(type)) {
		galois_printk(KERN_WARNING "No NAND device found!!!\n");
		chip->select_chip(mtd, -1);
		return PTR_ERR(type);
	}

	/* Check for a chip array */
	for (i = 1; i < maxchips; i++) {
		chip->select_chip(mtd, i);
		
		//mutex_lock(&mv_nand_mutex);
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
		chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
		/* Send the command for reading device ID */
		chip->cmdfunc(mtd, NAND_CMD_READID, 0x00, -1);
		/* Read manufacturer and device IDs */
		chip->read_buf(mtd,(uint8_t *)&tmp[0],sizeof(unsigned int)*2);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
		//mutex_unlock(&mv_nand_mutex);
#else
		mv_nand_dma_exrw_init(host);
		mv_nand_read_id_dma(host,(char *)&tmp[0]);
		//mv_nand_dma_wait_read_done();
		memcpy((char *)&tmp[0] , host->dmabuf , 8);
#endif
		if (nand_maf_id != (tmp[0]&0xff) ||
		    type->id != ((tmp[0]>>8)&0xff))
			break;
	}
	    
	
	if (i > 1)
		printk("%d NAND chips detected\n", i);
	/* Store the number of chips and calc total size for mtd */
	chip->numchips = i;
	mtd->size = i * chip->chipsize;
	return 0;
}
/**
 * mv_nand_get_device - [GENERIC] Get chip for selected access
 * @chip:	the nand chip descriptor
 * @mtd:	MTD device structure
 * @new_state:	the state which is requested
 *
 * Get the device and lock it for exclusive access
 */
static int
mv_nand_get_device(struct nand_chip *chip, struct mtd_info *mtd, int new_state)
{
	spinlock_t *lock = &chip->controller->lock;
	wait_queue_head_t *wq = &chip->controller->wq;
	DECLARE_WAITQUEUE(wait, current);
 retry:
	spin_lock(lock);

	/* Hardware controller shared among independend devices */
	/* Hardware controller shared among independend devices */
	if (!chip->controller->active)
		chip->controller->active = chip;

	if (chip->controller->active == chip && chip->state == FL_READY) {
		chip->state = new_state;
		spin_unlock(lock);
		return 0;
	}
	if (new_state == FL_PM_SUSPENDED) {
		spin_unlock(lock);
		return (chip->state == FL_PM_SUSPENDED) ? 0 : -EAGAIN;
	}
	set_current_state(TASK_UNINTERRUPTIBLE);
	add_wait_queue(wq, &wait);
	spin_unlock(lock);
	schedule();
	remove_wait_queue(wq, &wait);
	goto retry;
}
/**
 * mv_nand_check_wp - [GENERIC] check if the chip is write protected
 * @mtd:	MTD device structure
 * Check, if the device is write protected
 *
 * The function expects, that the device is already selected
 */
static int mv_nand_check_wp(struct mtd_info *mtd)
{
	int wp;

	//spin_lock_irqsave(&host->lock, flags);
	wp = mv_nand_read_status(mtd,0);
    //spin_unlock_irqrestore(&host->lock, flags);
	//printk("check wp wp = 0x%x\n",wp);	
	return (wp & NAND_STATUS_WP) ? 0 : 1;
}

/**
 * mv)nand_block_checkbad - [GENERIC] Check if a block is marked bad
 * @mtd:	MTD device structure
 * @ofs:	offset from device start
 * @getchip:	0, if the chip is already selected
 * @allowbbt:	1, if its allowed to access the bbt area
 *
 * Check, if the block is bad. Either by reading the bad block table or
 * calling of the scan function.
 */
static int mv_nand_block_checkbad(struct mtd_info *mtd, loff_t ofs, int getchip, int allowbbt)
{
	struct nand_chip *chip = mtd->priv;

#ifdef MV_NAND_IGNORE_BADBLOCK
	if(mv_nand_bignore_badblock)
		return 0;
#endif
    //return 0;
	if (!chip->bbt)
		return chip->block_bad(mtd, ofs, getchip);

	/* Return info from the table */
	return mv_nand_isbad_bbt(mtd, ofs, allowbbt);
}
/* mv_nand_cmd_request
 *
 * Wait for Write-Command-Request bit WRCMDREQ and clear it
 *
 */
static void mv_nand_cmd_request(struct nand_chip *chip)
{
	unsigned int read;

	/* Wait for Write-Command-Request bit WRCMDREQ */
	do
	{
		read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);	
		/* read status register */
		read &= MV_NAND_NDSR_WRCMDREQ;
		/* check WRCMDREQ bit */
	} while (!read);
	MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);
	/* write 1 to clear sticky bit (WRCMDREQ only)*/

	return;
}
static int mv_nand_read_status_nolock(struct mtd_info *mtd,int nc)
{
	 unsigned int cmd[2];
	 unsigned int status;
	 register struct nand_chip *chip = mtd->priv;
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	 chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
	 chip->cmdfunc(mtd, NAND_CMD_STATUS, nc, -1);
	 chip->read_buf(mtd,(uint8_t *)&cmd[0],sizeof(int));
	 chip->read_buf(mtd,(uint8_t *)&cmd[1],sizeof(int));
	 chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS,0);
#else
	 struct mv_nand_host *host = chip->priv;
	 do {
#if 0
	     mv_nand_dma_exrw_init(host);
	     mv_nand_read_status_dma(host ,NULL);
	     mv_nand_dma_wait_read_done(host);
#endif
		 mv_nand_make_request(host,MV_DMA_CMD_STATUS,0,0,(char *)&cmd[0], NULL,0);
	     //memcpy(&cmd[0],host->dmabuf,8);
	 }while((!(cmd[0] & NAND_STATUS_READY)) || ((cmd[0]&0xff) == 0xff));
#endif
	 status = cmd[0] & 0xFF;
	 return status;

}

/* mv_nand_read_status
 * Fetch the NAND device status
 * return: NAND status
 * 		I/O[0]: PASS/FAIL for Page Program or Block Erase (Pass: "0", Fail: "1")
 * 		I/O[5:1]: NC
 * 		I/O[6]: Ready/Busy (Busy: "0", Ready: "1")
 * 		I/O[7]: Write Protect (Protected: "0", Not Protected: "1")
 */
static int mv_nand_read_status(struct mtd_info *mtd,int nc)
{
	unsigned int cmd[2];
	unsigned int status;
	register struct nand_chip *chip = mtd->priv;
	//mutex_lock(&mv_nand_mutex);
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	mutex_lock(&mv_nand_mutex);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);	
	chip->cmdfunc(mtd, NAND_CMD_STATUS, nc, -1);
	chip->read_buf(mtd,(uint8_t *)&cmd[0],sizeof(int));
	/* dummy read because DFC alocates 1 buffer entry (8 bytes) to hold the read data for both read ID and read status command
     */ 
	chip->read_buf(mtd,(uint8_t *)&cmd[1],sizeof(int));
	/* command done */
	chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS,0);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
	mutex_unlock(&mv_nand_mutex);
#else
	
	struct mv_nand_host *host = chip->priv;
	do {
#if 0
	    mv_nand_dma_exrw_init(host);
	    mv_nand_read_status_dma(host ,NULL);
	    mv_nand_dma_wait_read_done(host);
#endif
		mv_nand_make_request(host,MV_DMA_CMD_STATUS,0,0,(void *)&cmd[0], NULL,0);
		//memcpy(&cmd[0],host->dmabuf,8);
	} while((!(cmd[0] & NAND_STATUS_READY)) ||((cmd[0]& 0xff) == 0xff));
#endif
	//mutex_unlock(&mv_nand_mutex);
	status = cmd[0] & 0xFF;

	return status;
}

/**
 * mv_nand_command - [DEFAULT] Send command to NAND device
 * @mtd:	MTD device structure
 * @command:	the command to be sent
 * @column:	the column address for this command, -1 if none
 * @page_addr:	the page address for this command, -1 if none
 *
 */
static void mv_nand_command(struct mtd_info *mtd, unsigned int command,
			 int column, int page_addr)
{
	register struct nand_chip *chip = mtd->priv;
	struct mv_nand_host *host = chip->priv;
	int ctrl ;
	unsigned int read;
	unsigned int dst_4321, dst_xxx5,dst,srcx;
	unsigned int row_addr_shift_left;
	unsigned int column_addr_mask;
	
	/*
	 * Write out the command to the device.
	 */
	if(!((command==NAND_CMD_CACHEDPROG)||(command == NAND_CMD_READSTART)||(command==NAND_CMD_PAGEPROG)||(command == NAND_CMD_READ1)||(command==NAND_CMD_ERASE1)||(command==NAND_CMD_ERASE2)||(command==NAND_CMD_SEQIN)||(command==NAND_CMD_RNDIN)||(command==NAND_CMD_STATUS)||(command==NAND_CMD_DEPLETE1)||(command==NAND_CMD_READ0)||(command==NAND_CMD_RESET)||(command==NAND_CMD_READID)||(command==NAND_CMD_READOOB)))
	    return;
	if((command == NAND_CMD_READ0)&&(mtd->writesize > 0x800))
		command = NAND_CMD_READSTART;
	if((command == NAND_CMD_SEQIN) && (mtd->writesize > 0x800))
		command = NAND_CMD_PAGEPROG;
#if 0
	if((command==NAND_CMD_RESET)||(command == NAND_CMD_PAGEPROG)) {
		// clear CSx_RDY bit
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, (host->board->ncs  ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0));		// write 1 to clear sticky bit (CSx_RDY only)
	}
		
	mv_nand_cmd_request(chip);
#endif
	if((command == NAND_CMD_SEQIN)||(command == NAND_CMD_READ0)||(command ==NAND_CMD_READSTART)||(command ==NAND_CMD_PAGEPROG)){
		if(host->board->conecc){
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET);
			read |= MV_NAND_NDCR_ECC_EN;
			MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET, read);
		}
		if(host->board->conspare) {
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET);
			read |= MV_NAND_NDCR_SPARE_EN;
			MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET, read);
		}
		if(host->board->conbch & 0x7F) {
			ctrl = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDECCCTRL_OFFSET);
			ctrl |= (host->board->conbch &0x7F);
			MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDECCCTRL_OFFSET, ctrl);
		}
	} else if((command == NAND_CMD_STATUS) || (command == NAND_CMD_READID)|| (command == NAND_CMD_ERASE1)) {
	    ctrl = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDECCCTRL_OFFSET);
		if(host->board->conbch & 0x7F) {
			ctrl &= ~1;
			MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDECCCTRL_OFFSET, ctrl);
		}
		if(host->board->conecc){
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET);
			read &= ~MV_NAND_NDCR_ECC_EN;
			MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET, read);
		}
		if(host->board->conspare) {
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET);
			read &= ~MV_NAND_NDCR_SPARE_EN;
			MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET, read);
		}
		if(command == NAND_CMD_READID) {
			if(host->board->conecc) {
				read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET);
				read &= ~MV_NAND_NDCR_ECC_EN;
				MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET, read);
			}
			if(host->board->conspare) {
				read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET);
				read &= ~MV_NAND_NDCR_SPARE_EN;
				MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET, read);
			}
		}	
	} 
	if((command == NAND_CMD_RESET) || (command == NAND_CMD_PAGEPROG) || (command == NAND_CMD_READ0)||(command == NAND_CMD_SEQIN) || (command == NAND_CMD_STATUS) || (command == NAND_CMD_READID)||(command == NAND_CMD_READSTART)||(command == NAND_CMD_ERASE1)) {
		mv_nand_disable(chip);
		mv_nand_start(chip);
	    	
	}	
	if((command==NAND_CMD_RESET)||(command == NAND_CMD_PAGEPROG)) {
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, (host->board->ncs  ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0));
	}
	mv_nand_cmd_request(chip);
	switch (command) {

	case NAND_CMD_READSTART:
		host->curcmd = NAND_CMD_READSTART;
		host->curpage = page_addr;
		/* for 4K large Page */
		ctrl = 0x00D5FF00 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
		/* CMD_TYPE(3b110, Naked Command Cycle), NC(1), DBC(0), ADDR_CYC(6)
		   CMD1(0x00, read cmd - 1st cycle), CMD2(NA)
		 */
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		/* address cycle 1:4*/
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		/* address cycle 5 and page count */
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		
		mv_nand_cmd_request(chip);
		
		ctrl = 0x00F5FFFF| (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);	
		
		dst = (page_addr << chip->page_shift) + column ;
		column_addr_mask =  (mtd->writesize - 1);
		row_addr_shift_left = 16 - chip->page_shift;
		dst_4321 = ((dst & column_addr_mask) | ((dst << row_addr_shift_left) & 0xFFFF0000));
		dst_xxx5 = ((dst >> (32 - row_addr_shift_left)) & ((1 << row_addr_shift_left) - 1));
		/* bytes needed to be read ,make use is multiple of 32 bytes if enabling ecc */
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		/* address cycle 1:4*/
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, dst_4321);
		/* address cycle 5 and page count */
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, dst_xxx5);
		mv_nand_cmd_request(chip);
		/* Naked Command 2 cycle */
		ctrl = 0x00D5FF30 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		mv_nand_cmd_request(chip);
		ctrl = 0xA015FF30 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		/* address cycle 1:4 */
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		/* address cycle 5 and page count */
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		do{
		    read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDSR_OFFSET);
			 
		} while (!(read & MV_NAND_NDSR_RDDREQ));
		 
		 //printk("NAND_CMD_READSTART 3 \n");
		 ctrl = MV_NAND_NDSR_RDDREQ;
		 if(read & MV_NAND_NDSR_CORERR) {
			 printk("correctable error detected\n");
			 ctrl |= MV_NAND_NDSR_CORERR;
			 mtd->ecc_stats.corrected += 1;
		 }
		 if(read & MV_NAND_NDSR_UNCERR) {
			 //printk("un-correctable error detected\n");
			 ctrl |= MV_NAND_NDSR_UNCERR;
			 //printk("uncrerr page = 0x%x\n ",host->curpage);
			 mtd->ecc_stats.failed++ ;
		 }
		 MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, ctrl);
		break;	
	case NAND_CMD_READ1:
		//printk("NAND_CMD_READ1 \n");
		host->curcmd = NAND_CMD_READ1;
		ctrl = 0xA015FF30 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		do{
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDSR_OFFSET);
			//read &= MV_NAND_NDSR_RDDREQ;
			//read &= (host->board->ncs ? ( MV_NAND_NDSR_CS1_PAGED): (MV_NAND_NDSR_CS0_PAGED));
		} while(!(read & MV_NAND_NDSR_RDDREQ));
		ctrl = MV_NAND_NDSR_RDDREQ;
		if(read & MV_NAND_NDSR_CORERR) {
			printk("correctable error detected\n");
			ctrl |= MV_NAND_NDSR_CORERR;
			mtd->ecc_stats.corrected += 1;
		}
		if(read & MV_NAND_NDSR_UNCERR) {
			mtd->ecc_stats.failed++ ;
			ctrl |= MV_NAND_NDSR_UNCERR;
		}
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, ctrl);
		break;
	case NAND_CMD_PAGEPROG:
		host->curcmd = NAND_CMD_PAGEPROG;
		host->curpage = page_addr;
		ctrl = 0x00D5FF80 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
		/* CMD_TYPE(3b110, Naked Command Cycle), NC(1), DBC(0), ADDR_CYC(6)
		 * CMD1(0x00, read cmd - 1st cycle), CMD2(NA)
		 */
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		/* address cycle 1:4 */
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		/* address cycle 5 and page count */
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		do {
		    read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);
		    /* check CSx_CMDD bit1247 */            
			read &= (host->board->ncs ? MV_NAND_NDSR_CS1_CMDD : MV_NAND_NDSR_CS0_CMDD     );
		 } while (!read);
		 
		 MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);

		 mv_nand_cmd_request(chip);
		 ctrl = 0x00F5FFFF | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);	
		 dst = (page_addr << chip->page_shift);
		 column_addr_mask =  (mtd->writesize - 1);
		 row_addr_shift_left = 16 - chip->page_shift;
		 dst_4321 = ((dst & column_addr_mask) | ((dst << row_addr_shift_left) & 0xFFFF0000));
		 dst_xxx5 = ((dst >> (32 - row_addr_shift_left)) & ((1 << row_addr_shift_left) - 1));
		 MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		 /* address cycle 1:4 */
		 MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, dst_4321);
         /* address cycle 5 and page count */
		 MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, dst_xxx5);
		 do {
			 /* read status register */
			 read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);
			 //printk("after reset check cmdd = %x\n",read);
			 read &= (host->board->ncs ? MV_NAND_NDSR_CS1_CMDD : MV_NAND_NDSR_CS0_CMDD);
		 } while (!read);
		 MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);
		 mv_nand_cmd_request(chip);
		 ctrl = 0xA0350000 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
		 MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		 /* address cycle 1:4 */
		 MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		 /* address cycle 5 and page count */
		 MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		 do{
			 /* read status register */
			 read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDSR_OFFSET);
			 /* check RDDREQ bit */
			 read &= MV_NAND_NDSR_WRDREQ;
			 
         } while (!read);
		 MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);
		break;
	case NAND_CMD_CACHEDPROG:
		//printk("NAND_CMD_CACHEDPROG \n");
		//for(tmpi = 0;tmpi <40000;tmpi++)
		host->curcmd = NAND_CMD_CACHEDPROG;
		ctrl = 0xA0350000 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		do{
			/* read status register */
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDSR_OFFSET);
			/* check RDDREQ bit */
			read &= MV_NAND_NDSR_WRDREQ;
		} while (!read);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);
		break;
	case NAND_CMD_DEPLETE1:
		host->curcmd = NAND_CMD_DEPLETE1;
		ctrl = 0x20D5FF10 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		do{
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDSR_OFFSET);
			read &= (host->board->ncs ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0);
		} while (!read);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);
		do {
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDSR_OFFSET);
			read &= (host->board->ncs ? (MV_NAND_NDSR_CS1_CMDD| MV_NAND_NDSR_CS1_PAGED): (MV_NAND_NDSR_CS0_CMDD| MV_NAND_NDSR_CS0_PAGED));
		
		}while(!read);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);
		break;
		
	case NAND_CMD_ERASE1:
		//printk("NAND_CMD_ERASE1 page = 0x%x\n",page_addr); 
		host->curcmd = NAND_CMD_ERASE1;
		host->status = 0;
		ctrl= 0x005BD060 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
		/*CMD_TYPE(0b10, erase), NC(1), DBC(1), ADDR_CYC(3)
		 *CMD1(0x60, erase cmd - 1st cycle), CMD2(0xD0, erase cmd - 2nd cycle)
		 *translate into row address format
		 */
		row_addr_shift_left = 16 - chip->page_shift;
		dst = page_addr << chip->page_shift;
		dst_4321 = ((dst >> chip->page_shift) & ~((mtd->erasesize/ mtd->writesize) - 1));
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, ctrl);
		/* address cycle 1:4 */
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, dst_4321);	
		/* address cycle 5 and page count */
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, 0);		
		/* Wait for Command-Done bit CSx_CMDD */
		do
		{
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);	
			read &= (host->board->ncs ? MV_NAND_NDSR_CS1_CMDD : MV_NAND_NDSR_CS0_CMDD);			
		} while (!read);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);

		break;
	case NAND_CMD_ERASE2:
	    break;
		
	case NAND_CMD_SEQIN:
		//printk("NAND_CMD_SEQIN page = 0x%x\n",page_addr);
		host->curcmd = NAND_CMD_SEQIN;
		/* write Page */
		//printk("NAND_CMD_SEQIN page =0x%x\n",page_addr);
		/* CMD_TYPE(0b01, program/write), NC(1), DBC(1), ADDR_CYC(5)
		 * CMD1(0x80, write cmd - 1st cycle), CMD2(0x10, write - 2nd cycle)
		 *translate into column-row address format
		 */
		 if(mtd->writesize == 2048) {
			ctrl = 0x022D1080 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
			column_addr_mask =  (mtd->writesize - 1);
			row_addr_shift_left = 16 - chip->page_shift;
			dst = page_addr << chip->page_shift;
			dst_4321 = ((dst & column_addr_mask) | ((dst << row_addr_shift_left) & 0xFFFF0000));
			dst_xxx5 = ((dst >> (32 - row_addr_shift_left)) & ((1 << row_addr_shift_left) - 1));
		} else if(mtd->writesize == 512) {
			/* ADDR_CYC(4) */
			ctrl = 0x022c1080 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
			dst = page_addr << chip->page_shift;
			dst_4321 = ((dst & 0xFF) | (dst>>(chip->page_shift-8)))  ;
			dst_xxx5 = 0;
		} else
			return ;
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		// address cycle 1:4
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, dst_4321);	
		// address cycle 5 and page count
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, dst_xxx5);	
		// Wait for Write-Data-Request bit WRDREQ
		do
		{
		    // read status register
		    read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDSR_OFFSET);
		    // check WRDREQ bit
		    read &= MV_NAND_NDSR_WRDREQ;
			
		} while (!read);
		// write 1 to clear sticky bit (WRDREQ only)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);		
	    break;
	case NAND_CMD_RNDIN:
	    break;
		
	case NAND_CMD_STATUS:
		//printk("NAND_CMD_STATUS page = 0x%x\n",page_addr);
		host->curcmd = NAND_CMD_STATUS;

		//printk("NAND_CMD_STATUS ncs = 0x%x\n",host->board->ncs);
		ctrl = 0x00800070 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0) | (column ? MV_NAND_NDCB_NC : 0);	
		// CMD_TYPE(0b100, read ID), DBC(0), ADDR_CYC(0)
		// CMD1(0x70, read status)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, ctrl);
		// address cycle 1:4
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, 0);
		// address cycle 5 and page count
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, 0);
		// Wait for Read-Data-Request bit RDDREQ
		do {
			// read status register
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDSR_OFFSET);	
			// check RDDREQ bit
			read &= MV_NAND_NDSR_RDDREQ;
		}while (!read);
		// write 1 to clear sticky bit (RDDREQ only)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);		
		break;
	
	case NAND_CMD_READ0:
		//printk("NAND_CMD_READ0 page = 0x%x\n",page_addr);
		host->curcmd = NAND_CMD_READ0;
		if(mtd->writesize == 2048) {
			ctrl = 0x001D3000 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);	
			// CMD_TYPE(0b0, read), NC(1), DBC(1), ADDR_CYC(5)
			// CMD1(0x00, read cmd - 1st cycle), CMD2(0x30, read cmd - 2nd cycle)
			srcx = page_addr << chip->page_shift;
			row_addr_shift_left = 16 - chip->page_shift;
			column_addr_mask =  (mtd->writesize - 1);
			dst_4321 = ((srcx & column_addr_mask) | ((srcx << row_addr_shift_left) & 0xFFFF0000));
			dst_xxx5 = ((srcx >> (32 - row_addr_shift_left)) & ((1 << row_addr_shift_left) - 1));
		} else if(mtd->writesize == 512){
			ctrl = 0x001c3000 | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
			srcx = page_addr << chip->page_shift;
			dst_4321 = ((srcx & 0xFF) | (srcx >> (chip->page_shift - 8)));
			dst_xxx5 = 0;
		} else
		    return ;
		
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, ctrl);
		// address cycle 1:4
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, dst_4321);	
		// address cycle 5 and page count
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, dst_xxx5);	
		// Wait for Write-Data-Request bit WRDREQ
		do{
			// read status register
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDSR_OFFSET);	
			// check RDDREQ bit
			
		} while (!(read & MV_NAND_NDSR_RDDREQ));

		ctrl = MV_NAND_NDSR_RDDREQ;
		if(read & MV_NAND_NDSR_CORERR) {
			printk("correctable error detected\n");
			ctrl |= MV_NAND_NDSR_CORERR;
			mtd->ecc_stats.corrected += 1;
		}
		if(read & MV_NAND_NDSR_UNCERR) {
			ctrl |= MV_NAND_NDSR_UNCERR;
		}
		// write 1 to clear sticky bit (WRDREQ only)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);		
		break;
		
	case NAND_CMD_RESET:
		// Write command and addresses
		host->curcmd = NAND_CMD_RESET;
		//printk("NAND_CMD_RESET ncs = 0x%x\n",host->board->ncs);
		ctrl = 0x00A000FF | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0) | (column ? MV_NAND_NDCB_NC : 0);
		// CMD_TYPE(0b101, reset), DBC(0), ADDR_CYC(0)
		// CMD1(0xFF, reset cmd)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, ctrl);
		// address cycle 1:4
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, 0);
		// address cycle 5 and page count
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, 0);
		
		// Wait for RDY bit
		do{
			// read status register
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);	
			// check RDYx bit
			read &= (host->board->ncs ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0);
            //printk("command reset NDSR read = %x\n",read);			
		} while (!read);
		// write 1 to clear sticky bit (RDYx only)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);		

		// Wait for Command-Done bit CSx_CMDD
		do
		{
		       // read status register
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);	
			//printk("After reset check cmdd = %x\n",read);
			// check CSx_CMDD bit
			read &= (host->board->ncs ? MV_NAND_NDSR_CS1_CMDD : MV_NAND_NDSR_CS0_CMDD);
		} while (!read);
		// write 1 to clear sticky bit (CS0_CMDD only)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);		
		
		break;
		
	case NAND_CMD_READID:
		host->curcmd = NAND_CMD_READID;
		// Write command and addresses
		//printk("NAND_CMD_READID,ncs = 0x%x\n",host->board->ncs);
		read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDCR_OFFSET);
		//printk("Command readid NDCR read = %x\n",read);
		ctrl = 0x00610090 | (host->board->ncs? MV_NAND_NDCB_CSEL : 0| (column ? MV_NAND_NDCB_NC : 0));
		// CMD_TYPE(0b011, read ID), DBC(0), ADDR_CYC(1)
		// CMD1(0x90, read ID)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, ctrl);
		// address cycle 1:4
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, 0);
		// address cycle 5 and page count
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, 0);
		// Wait for Read-Data-Request bit RDDREQ
		do  {
			// read status register
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDSR_OFFSET);	
	        //printk("readId status register read = %x\n",read);
			// check RDDREQ bi
			read &= MV_NAND_NDSR_RDDREQ;

		}while (!read);
		// write 1 to clear sticky bit (RDDREQ only)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);		
		break;
		
	default:
	    return;
	}
	
}

#define BBT_PAGE_MASK	0xffffff3f
/**
 * mv_nand_erase_nand - [Internal] erase block(s)
 * @mtd:	MTD device structure
 * @instr:	erase instruction
 * @allowbbt:	allow erasing the bbt area
 *
 * Erase one ore more blocks
 */
int mv_nand_erase_nand(struct mtd_info *mtd, struct erase_info *instr,
		    int allowbbt)
{
	int page, len, status, pages_per_block, ret, chipnr;
	struct nand_chip *chip = mtd->priv;
	struct mv_nand_host *host = chip->priv;
	int rewrite_bbt[NAND_MAX_CHIPS]={0};
	unsigned int bbt_masked_page = 0xffffffff;

    //printk("mv_nand_erase_nand: start = 0x%08x, len = %i\n",(unsigned int)instr->addr, (unsigned int)instr->len);

	/* Start address must align on block boundary */
	if (instr->addr & ((1 << chip->phys_erase_shift) - 1)) {
		printk("mv_nand_erase: Unaligned address\n");
		return -EINVAL;
	}

	/* Length must align on block boundary */
	if (instr->len & ((1 << chip->phys_erase_shift) - 1)) {
		DEBUG(MTD_DEBUG_LEVEL0,"mv_nand_erase: Length not block aligned\n");
		return -EINVAL;
	}

	/* Do not allow erase past end of device */
	if ((instr->len + instr->addr) > mtd->size) {
		DEBUG(MTD_DEBUG_LEVEL0, "mv_nand_erase: "
		      "Erase past end of device\n");
		return -EINVAL;
	}

	instr->fail_addr = 0xffffffff;

	/* Grab the lock and see if the device is available */
	mv_nand_get_device(chip, mtd, FL_ERASING);

	/* Shift to get first page */
	page = (int)(instr->addr >> chip->page_shift);
	chipnr = (int)(instr->addr >> chip->chip_shift);

	/* Calculate pages in each block */
	pages_per_block = 1 << (chip->phys_erase_shift - chip->page_shift);

	/* Select the NAND device */
	chip->select_chip(mtd, chipnr);

	//chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
	/* Check, if it is write protected */
	if (mv_nand_check_wp(mtd)) {
		DEBUG(MTD_DEBUG_LEVEL0, "mv_nand_erase: "
		      "Device is write protected!!!\n");
		instr->state = MTD_ERASE_FAILED;
		goto erase_exit;
	}
   // chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
	/*
	 * If BBT requires refresh, set the BBT page mask to see if the BBT
	 * should be rewritten. Otherwise the mask is set to 0xffffffff which
	 * can not be matched. This is also done when the bbt is actually
	 * erased to avoid recusrsive updates
	 */
	if (chip->options & BBT_AUTO_REFRESH && !allowbbt)
		bbt_masked_page = chip->bbt_td->pages[chipnr] & BBT_PAGE_MASK;

	/* Loop through the pages */
	len = instr->len;

	instr->state = MTD_ERASING;	

	while (len) {
		/*
		 * heck if we have a bad block, we do not erase bad blocks !
		 */
        #if 1
		if (mv_nand_block_checkbad(mtd, ((loff_t) (page)) << chip->page_shift, 0, allowbbt)) {
			printk(KERN_WARNING "mv_nand_erase: attempt to erase a "
			       "bad block at page 0x%08x\n", page);
			instr->state = MTD_ERASE_FAILED;
			goto erase_exit;
		}
        #endif
		/*
		 * Invalidate the page cache, if we erase the block which
		 * contains the current cached page
		 */
		if (page <= chip->pagebuf && chip->pagebuf <
		    (page + pages_per_block))
			chip->pagebuf = -1;
		/* start the chip */

		chip->erase_cmd(mtd, page & chip->pagemask);

		//status = chip->waitfunc(mtd, chip);
		status = host->status;

		/*
		 * See if operation failed and additional status checks are
		 * available
		 */
		if(status & NAND_STATUS_FAIL)
			printk(KERN_ERR"mv_nand_erase fail\n");
		if ((status & NAND_STATUS_FAIL) && (chip->errstat))
			status = chip->errstat(mtd, chip, FL_ERASING,status, page);
	    /* stop NFC */
              
		//chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
		

		/* See if block erase succeeded */
		if (status & NAND_STATUS_FAIL) {
			DEBUG(MTD_DEBUG_LEVEL0, "mv_nand_erase:Failed erase, page 0x%08x\n", page);
			instr->state = MTD_ERASE_FAILED;
			instr->fail_addr = (page << chip->page_shift);
			goto erase_exit;
		}

		/*
		 * If BBT requires refresh, set the BBT rewrite flag to the
		 * page being erased
		 */
		if (bbt_masked_page != 0xffffffff &&
		    (page & BBT_PAGE_MASK) == bbt_masked_page)
			    rewrite_bbt[chipnr] = (page << chip->page_shift);

		/* Increment page address and decrement length */
		len -= (1 << chip->phys_erase_shift);
		page += pages_per_block;

		/* Check, if we cross a chip boundary */
		if (len && !(page & chip->pagemask)) {
			chipnr++;
			chip->select_chip(mtd, -1);
			chip->select_chip(mtd, chipnr);

			/*
			 * If BBT requires refresh and BBT-PERCHIP, set the BBT
			 * page mask to see if this BBT should be rewritten
			 */
			if (bbt_masked_page != 0xffffffff &&
			    (chip->bbt_td->options & NAND_BBT_PERCHIP))
				bbt_masked_page = chip->bbt_td->pages[chipnr] &
					BBT_PAGE_MASK;
		}
	}
	instr->state = MTD_ERASE_DONE;

 erase_exit:

	ret = instr->state == MTD_ERASE_DONE ? 0 : -EIO;
	/* Do call back function */
	if (!ret)
		mtd_erase_callback(instr);

	/* Deselect and wake up anyone waiting on the device */
	mv_nand_release_device(mtd);

	/*
	 * If BBT requires refresh and erase was successful, rewrite any
	 * selected bad block tables
	 */
	if (bbt_masked_page == 0xffffffff || ret)
		return ret;

	for (chipnr = 0; chipnr < chip->numchips; chipnr++) {
		if (!rewrite_bbt[chipnr])
			continue;
		/* update the BBT for chip */
		DEBUG(MTD_DEBUG_LEVEL0, "mv_nand_erase_nand: mv_nand_update_bbt "
		      "(%d:0x%0x 0x%0x)\n", chipnr, rewrite_bbt[chipnr],
		      chip->bbt_td->pages[chipnr]);
		mv_nand_update_bbt(mtd, rewrite_bbt[chipnr]);
	}

	/* Return more or less happy */
	return ret;
}
/**
 * mv_nand_transfer_oob - [Internal] Transfer oob to client buffer
 * @chip:	nand chip structure
 * @oob:	oob destination address
 * @ops:	oob ops structure
 * @len:	size of oob to transfer
 */
static uint8_t *mv_nand_transfer_oob(struct nand_chip *chip, uint8_t *oob,
				  struct mtd_oob_ops *ops, size_t len)
{
	switch(ops->mode) {

	case MTD_OOB_PLACE:
	case MTD_OOB_RAW:
		memcpy(oob, chip->oob_poi + ops->ooboffs, len);
		return oob + len;

	case MTD_OOB_AUTO: {
		struct nand_oobfree *free = chip->ecc.layout->oobfree;
		uint32_t boffs = 0, roffs = ops->ooboffs;
		size_t bytes = 0;

		for(; free->length && len; free++, len -= bytes) {
			/* Read request not from offset 0 ? */
			if (unlikely(roffs)) {
				if (roffs >= free->length) {
					roffs -= free->length;
					continue;
				}
				boffs = free->offset + roffs;
				bytes = min_t(size_t, len,
					      (free->length - roffs));
				roffs = 0;
			} else {
				bytes = min_t(size_t, len, free->length);
				boffs = free->offset;
			}
			memcpy(oob, chip->oob_poi + boffs, bytes);
			oob += bytes;
		}
		return oob;
	}
	default:
		BUG();
	}
	return NULL;
}
/*
 * mv_nand_do_read_ops - [Internal] Read data with ECC
 *
 * @mtd:	MTD device structure
 * @from:	offset to read from
 * @ops:	oob ops structure
 *
 * Internal function. Called with chip held.
 */
static int mv_nand_do_read_ops(struct mtd_info *mtd, loff_t from,
			    struct mtd_oob_ops *ops)
{
	int chipnr, page, realpage, col, bytes, aligned;
	struct nand_chip *chip = mtd->priv;
#ifdef CONFIG_MTD_NAND_MARVELL_DMA 
	struct mv_nand_host *host = chip->priv;
#else
	int  sendcmd = 0;
#endif
	int sndcmd = 1;
	struct mtd_ecc_stats stats;
	int blkcheck = (1 << (chip->phys_erase_shift - chip->page_shift)) - 1;
	int ret = 0;
	uint32_t readlen = ops->len;
	uint32_t oobreadlen = ops->ooblen;
	uint8_t *bufpoi = NULL, *oob, *buf;
    //printk("mv_nand_do_read_ops from = 0x%08Lx readlen =0x%x oobreadlen = 0x%x\n",(unsigned long long )from,readlen,oobreadlen);
	stats = mtd->ecc_stats;

	chipnr = (int)(from >> chip->chip_shift);
	chip->select_chip(mtd, chipnr);

	realpage = (int)(from >> chip->page_shift);
	page = realpage & chip->pagemask;

	col = (int)(from & (mtd->writesize - 1));

	buf = ops->datbuf;
	oob = ops->oobbuf;
	
	//chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);

	while(1) {
		bytes = min(mtd->writesize - col, readlen);
		aligned = (bytes == mtd->writesize);

		/* Is the current page in the buffer ? */
		if (realpage != chip->pagebuf || oob) {
			bufpoi = aligned ? buf : chip->buffers->databuf;
			//mutex_lock(&mv_nand_mutex);
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
			mutex_lock(&mv_nand_mutex);
			if (likely(sndcmd)) {
				//printk("mv_nand_do_read_ops\n");
				chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
				chip->cmdfunc(mtd, NAND_CMD_READ0, 0x00, page);
				sndcmd = 0;
				sendcmd = 1;
			}

			/* Now read the page into the buffer */
			if (unlikely(ops->mode == MTD_OOB_RAW))
				ret = chip->ecc.read_page_raw(mtd, chip, bufpoi);
			else
				ret = chip->ecc.read_page(mtd, chip, bufpoi);
            if(sendcmd == 1) {
				/* PAGE DONE */
				chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
			    /*command done*/
				chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);
				chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
				sendcmd = 0;
			}
			mutex_unlock(&mv_nand_mutex);
#else
			ret = mv_nand_make_request(host,MV_DMA_CMD_READ,page,1,bufpoi, chip->oob_poi,0);
#endif      
			//mutex_unlock(&mv_nand_mutex);
			if(ret < 0)
				break;
			/* Transfer not aligned data */
			if (!aligned) {
				chip->pagebuf = realpage;
				memcpy(buf, chip->buffers->databuf + col, bytes);
			}

			buf += bytes;

			if (unlikely(oob)) {
				/* Raw mode does data:oob:data:oob */
				if (ops->mode != MTD_OOB_RAW) {
					int toread = min(oobreadlen,
						chip->ecc.layout->oobavail);
					if (toread) {
						oob = mv_nand_transfer_oob(chip,
							oob, ops, toread);
						oobreadlen -= toread;
					}
				} else
					buf = mv_nand_transfer_oob(chip,
						buf, ops, mtd->oobsize);
			}

			if (!(chip->options & NAND_NO_READRDY)) {
				/*
				 * Apply delay or wait for ready/busy pin. Do
				 * this before the AUTOINCR check, so no
				 * problems arise if a chip which does auto
				 * increment is marked as NOAUTOINCR by the
				 * board driver.
				 */
				if (!chip->dev_ready)
					udelay(chip->chip_delay);
				else
					nand_wait_ready(mtd);
			}
		} else {
			memcpy(buf, chip->buffers->databuf + col, bytes);
			buf += bytes;
		}
		readlen -= bytes;

		if (!readlen)
			break;

		/* For subsequent reads align to page boundary. */
		col = 0;
		/* Increment page address */
		realpage++;

		page = realpage & chip->pagemask;
		/* Check, if we cross a chip boundary */
		if (!page) {
			chipnr++;
			chip->select_chip(mtd, -1);
			chip->select_chip(mtd, chipnr);
		}

		/* Check, if the chip supports auto page increment
		 * or if we have hit a block boundary.
		 */
		if (!NAND_CANAUTOINCR(chip) || !(page & blkcheck)){
		  
			sndcmd = 1;
		}
	}
	

	ops->retlen = ops->len - (size_t) readlen;
	if (oob)
		ops->oobretlen = ops->ooblen - oobreadlen;

	if (ret)
		return ret;

	if (mtd->ecc_stats.failed - stats.failed)
		return -EBADMSG;

	return  mtd->ecc_stats.corrected - stats.corrected ? -EUCLEAN : 0;
}

/**
 * mv_nand_read - [MTD Interface] MTD compability function for nand_do_read_ecc
 * @mtd:	MTD device structure
 * @from:	offset to read from
 * @len:	number of bytes to read
 * @retlen:	pointer to variable to store the number of read bytes
 * @buf:	the databuffer to put data
 *
 * Get hold of the chip and call nand_do_read
 */
static int mv_nand_read(struct mtd_info *mtd, loff_t from, size_t len,
		     size_t *retlen, uint8_t *buf)
{
	struct nand_chip *chip = mtd->priv;
	int ret;
    //printk("mv_nand_read from =0x%08Lx len =0x%x\n",(unsigned long long)from,len);
	/* Do not allow reads past end of device */
	if ((from + len) > mtd->size)
		return -EINVAL;
	if (!len)
		return 0;

	//printk("mv_nand_read from = %x,len = %x\n",from,len);
	mv_nand_get_device(chip, mtd, FL_READING);

	chip->ops.len = len;
	chip->ops.datbuf = buf;
	chip->ops.oobbuf = NULL;

	ret = mv_nand_do_read_ops(mtd, from, &chip->ops);

	*retlen = chip->ops.retlen;

	mv_nand_release_device(mtd);

	return ret;
}
/**
 * mv_nand_fill_oob - [Internal] Transfer client buffer to oob
 * @chip:	nand chip structure
 * @oob:	oob data buffer
 * @ops:	oob ops structure
 */
static uint8_t *mv_nand_fill_oob(struct nand_chip *chip, uint8_t *oob,
				  struct mtd_oob_ops *ops)
{
	size_t len = ops->ooblen;
   // printk("mv_nand_fill_oob ops->ooblen = 0x%x mode = 0x%x offs = 0x%x",len,ops->mode,ops->ooboffs);
	switch(ops->mode) {

	case MTD_OOB_PLACE:
	case MTD_OOB_RAW:
		memcpy(chip->oob_poi + ops->ooboffs, oob, len);
		return oob + len;

	case MTD_OOB_AUTO: {
		struct nand_oobfree *free = chip->ecc.layout->oobfree;
		uint32_t boffs = 0, woffs = ops->ooboffs;
		size_t bytes = 0;

		for(; free->length && len; free++, len -= bytes) {
			/* Write request not from offset 0 ? */
			if (unlikely(woffs)) {
				if (woffs >= free->length) {
					woffs -= free->length;
					continue;
				}
				boffs = free->offset + woffs;
				bytes = min_t(size_t, len,
					      (free->length - woffs));
				woffs = 0;
			} else {
				bytes = min_t(size_t, len, free->length);
				boffs = free->offset;
			}
			memcpy(chip->oob_poi + boffs, oob, bytes);
			oob += bytes;
		}
		return oob;
	}
	default:
		BUG();
	}
	return NULL;
}
#define NOTALIGNED(x)	(x & (chip->subpagesize - 1)) != 0

/**
 * mv_nand_do_write_ops - [Internal] NAND write with ECC
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @ops:	oob operations description structure
 *
 * NAND write with ECC
 */
static int mv_nand_do_write_ops(struct mtd_info *mtd, loff_t to,
			     struct mtd_oob_ops *ops)
{
	int chipnr, realpage, page, blockmask, column;
	struct nand_chip *chip = mtd->priv;
	uint32_t writelen = ops->len;
	uint8_t *oob = ops->oobbuf;
	uint8_t *buf = ops->datbuf;
	int ret, subpage;

	//printk("mv_nand_do_write_ops to = %08Lx writelen = 0x%x\n",(unsigned long long)to,writelen);
	ops->retlen = 0;
	if (!writelen)
		return 0;

	/* reject writes, which are not page aligned */
	if (NOTALIGNED(to) || NOTALIGNED(ops->len)) {
		printk(KERN_NOTICE "nand_write: "
		       "Attempt to write not page aligned data\n");
		return -EINVAL;
	}

	column = to & (mtd->writesize - 1);
	subpage = column || (writelen & (mtd->writesize - 1));

	
	if (subpage && oob)
		return -EINVAL;

	chipnr = (int)(to >> chip->chip_shift);
	chip->select_chip(mtd, chipnr);
    //chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
	/* Check, if it is write protected */
	if (mv_nand_check_wp(mtd))
	{  
		printk("write protected\n");
		return -EIO;
    }
	realpage = (int)(to >> chip->page_shift);
	page = realpage & chip->pagemask;
	blockmask = (1 << (chip->phys_erase_shift - chip->page_shift)) - 1;

	/* Invalidate the page cache, when we write to the cached page */
	if (to <= (chip->pagebuf << chip->page_shift) &&
	    (chip->pagebuf << chip->page_shift) < (to + ops->len))
		chip->pagebuf = -1;

	/* If we're not given explicit OOB data, let it be 0xFF */
	if (likely(!oob)){
		//printk("fill oob 0xff\n");
		memset(chip->oob_poi, 0xff, mtd->oobsize);
	}
    
	//chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
	while(1) {
		int bytes = mtd->writesize;
		int cached = writelen > bytes && page != blockmask;
		uint8_t *wbuf = buf;

		/* Partial page write ? */
		if (unlikely(column || writelen < (mtd->writesize - 1))) {
			cached = 0;
			bytes = min_t(int, bytes - column, (int) writelen);
			chip->pagebuf = -1;
			memset(chip->buffers->databuf, 0xff, mtd->writesize);
			memcpy(&chip->buffers->databuf[column], buf, bytes);
			wbuf = chip->buffers->databuf;
		}

		if (unlikely(oob))
			oob = mv_nand_fill_oob(chip, oob, ops);
		//chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
		ret = chip->write_page(mtd, chip, wbuf, page, cached,
				       (ops->mode == MTD_OOB_RAW));
		if (ret)
		    break;
		writelen -= bytes;
		if (!writelen)
			break;
		column = 0;
		buf += bytes;
		realpage++;

		page = realpage & chip->pagemask;
		/* Check, if we cross a chip boundary */
		if (!page) {
			chipnr++;
			chip->select_chip(mtd, -1);
			chip->select_chip(mtd, chipnr);
		}
	}
    //chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
	
	ops->retlen = ops->len - writelen;
	if (unlikely(oob))
		ops->oobretlen = ops->ooblen;
	//printk("leaving mv_nand_do_write_ops\n");
	return ret;
}
/**
 * mv_nand_write - [MTD Interface] NAND write with ECC
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @len:	number of bytes to write
 * @retlen:	pointer to variable to store the number of written bytes
 * @buf:	the data to write
 *
 * NAND write with ECC
 */
static int mv_nand_write(struct mtd_info *mtd, loff_t to, size_t len,
			  size_t *retlen, const uint8_t *buf)
{
	struct nand_chip *chip = mtd->priv;
	int ret;

	/* Do not allow reads past end of device */
	if ((to + len) > mtd->size)
		return -EINVAL;
	if (!len)
		return 0;
    
	//printk("mv_nand_write to 0x%08Lx len = %x \n",(unsigned long long)to ,len);
	mv_nand_get_device(chip, mtd, FL_WRITING);

	chip->ops.len = len;
	chip->ops.datbuf = (uint8_t *)buf;
	chip->ops.oobbuf = NULL;

	ret = mv_nand_do_write_ops(mtd, to, &chip->ops);

	*retlen = chip->ops.retlen;

	mv_nand_release_device(mtd);

	return ret;
}
/**
 * mv_nand_do_read_oob - [Intern] NAND read out-of-band
 * @mtd:	MTD device structure
 * @from:	offset to read from
 * @ops:	oob operations description structure
 *
 * NAND read out-of-band data from the spare area
 */
static int mv_nand_do_read_oob(struct mtd_info *mtd, loff_t from,
			    struct mtd_oob_ops *ops)
{
	int page, realpage, chipnr, sndcmd = 1;
	struct nand_chip *chip = mtd->priv;
	int blkcheck = (1 << (chip->phys_erase_shift - chip->page_shift)) - 1;
	int readlen = ops->ooblen;
	int len;
	uint8_t *buf = ops->oobbuf;
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	struct mv_nand_host *host = chip->priv;
#endif
	if (ops->mode == MTD_OOB_AUTO)
		len = chip->ecc.layout->oobavail;
	else
		len = mtd->oobsize;

	if (unlikely(ops->ooboffs >= len)) {
		DEBUG(MTD_DEBUG_LEVEL0, "nand_read_oob: "
			"Attempt to start read outside oob\n");
		return -EINVAL;
	}

	/* Do not allow reads past end of device */
	if (unlikely(from >= mtd->size ||
		     ops->ooboffs + readlen > ((mtd->size >> chip->page_shift) -
					(from >> chip->page_shift)) * len)) {
		DEBUG(MTD_DEBUG_LEVEL0, "nand_read_oob: "
			"Attempt read beyond end of device\n");
		return -EINVAL;
	}

	chipnr = (int)(from >> chip->chip_shift);
	chip->select_chip(mtd, chipnr);

	/* Shift to get page */
	realpage = (int)(from >> chip->page_shift);
	page = realpage & chip->pagemask;

	while(1) {
		/* start the chip */
		//mutex_lock(&mv_nand_mutex);
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
		mutex_lock(&mv_nand_mutex);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
		
		sndcmd = chip->ecc.read_oob(mtd, chip, page, sndcmd);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);

		/* stop the chip */
		chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
		mutex_unlock(&mv_nand_mutex);
#else
		mv_nand_make_request(host,MV_DMA_CMD_READ,page,1,host->buf,NULL,1);
		memcpy(chip->oob_poi,host->buf + mtd->writesize ,host->oobsize);
		
#endif
		//mutex_unlock(&mv_nand_mutex);

		len = min(len, readlen);
		buf = mv_nand_transfer_oob(chip, buf, ops, len);

		if (!(chip->options & NAND_NO_READRDY)) {
			/*
			 * Apply delay or wait for ready/busy pin. Do this
			 * before the AUTOINCR check, so no problems arise if a
			 * chip which does auto increment is marked as
			 * NOAUTOINCR by the board driver.
			 */
			if (!chip->dev_ready)
				udelay(chip->chip_delay);
			else
				nand_wait_ready(mtd);
		}

		readlen -= len;
		if (!readlen)
			break;

		/* Increment page address */
		realpage++;

		page = realpage & chip->pagemask;
		/* Check, if we cross a chip boundary */
		if (!page) {
			chipnr++;
			chip->select_chip(mtd, -1);
			chip->select_chip(mtd, chipnr);
		}

		/* Check, if the chip supports auto page increment
		 * or if we have hit a block boundary.
		 */
		if (!NAND_CANAUTOINCR(chip) || !(page & blkcheck))
			sndcmd = 1;
	}

	ops->oobretlen = ops->ooblen;
	return 0;
}
/**
 * mv_nand_read_oob - [MTD Interface] NAND read data and/or out-of-band
 * @mtd:	MTD device structure
 * @from:	offset to read from
 * @ops:	oob operation description structure
 *
 * NAND read data and/or out-of-band data
 */
static int mv_nand_read_oob(struct mtd_info *mtd, loff_t from,
			 struct mtd_oob_ops *ops)
{
	struct nand_chip *chip = mtd->priv;
	int ret = -ENOTSUPP;

	//printk("mv_nand_read_oob from = 0x%08Lx\n",(unsigned long long)from);
	ops->retlen = 0;
	
	/* Do not allow reads past end of device */
	if (ops->datbuf && (from + ops->len) > mtd->size) {
		DEBUG(MTD_DEBUG_LEVEL0, "nand_read_oob: "
		      "Attempt read beyond end of device\n");
		return -EINVAL;
	}

	mv_nand_get_device(chip, mtd, FL_READING);

	switch(ops->mode) {
	case MTD_OOB_PLACE:
	case MTD_OOB_AUTO:
	case MTD_OOB_RAW:
		break;

	default:
		goto out;
	}

	if (!ops->datbuf) {
		ret = mv_nand_do_read_oob(mtd, from, ops);
	}
	else {
		ret = mv_nand_do_read_ops(mtd, from, ops);
	}

 out:
	mv_nand_release_device(mtd);
	return ret;
}
/**
 * mv_nand_do_write_oob - [MTD Interface] NAND write out-of-band
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @ops:	oob operation description structure
 *
 * NAND write out-of-band
 */
static int mv_nand_do_write_oob(struct mtd_info *mtd, loff_t to,
			     struct mtd_oob_ops *ops)
{
	int chipnr, page, status, len;
	struct nand_chip *chip = mtd->priv;
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	struct mv_nand_host *host = chip->priv;
#endif
	if (ops->mode == MTD_OOB_AUTO)
		len = chip->ecc.layout->oobavail;
	else
		len = mtd->oobsize;

	/* Do not allow write past end of page */
	if ((ops->ooboffs + ops->ooblen) > len) {
		DEBUG(MTD_DEBUG_LEVEL0, "nand_write_oob: "
		      "Attempt to write past end of page\n");
		return -EINVAL;
	}

	if (unlikely(ops->ooboffs >= len)) {
		DEBUG(MTD_DEBUG_LEVEL0, "nand_read_oob: "
			"Attempt to start write outside oob\n");
		return -EINVAL;
	}

	/* Do not allow reads past end of device */
	if (unlikely(to >= mtd->size ||
		     ops->ooboffs + ops->ooblen >
			((mtd->size >> chip->page_shift) -
			 (to >> chip->page_shift)) * len)) {
		DEBUG(MTD_DEBUG_LEVEL0, "nand_read_oob: "
			"Attempt write beyond end of device\n");
		return -EINVAL;
	}

	chipnr = (int)(to >> chip->chip_shift);
	chip->select_chip(mtd, chipnr);

	/* Shift to get page */
	page = (int)(to >> chip->page_shift);
	#if 0
	/*
	 * Reset the chip. Some chips (like the Toshiba TC5832DC found in one
	 * of my DiskOnChip 2000 test units) will clear the whole data page too
	 * if we don't do this. I have no clue why, but I seem to have 'fixed'
	 * it in the doc2000 driver in August 1999.  dwmw2.
	 */
	 
	chip->cmdfunc(mtd, NAND_CMD_RESET, -1, -1);
	#endif

	//chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
	/* Check, if it is write protected */
	if (mv_nand_check_wp(mtd)){
		//chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
		return -EROFS;
	}
	//chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);


	/* Invalidate the page cache, if we write to the cached page */
	if (page == chip->pagebuf)
		chip->pagebuf = -1;

	memset(chip->oob_poi, 0xff, mtd->oobsize);
	mv_nand_fill_oob(chip, ops->oobbuf, ops);
	//mutex_lock(&mv_nand_mutex);
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	mutex_lock(&mv_nand_mutex);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
	status = chip->ecc.write_oob(mtd, chip, page & chip->pagemask);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
	mutex_unlock(&mv_nand_mutex);
#else
	status = mv_nand_make_request(host,MV_DMA_CMD_READ,page & chip->pagemask,1,host->buf, NULL, 1);
	if(status)
		goto ERROR;
	memcpy(host->buf + mtd->writesize ,chip->oob_poi ,mtd->oobsize);
	status = mv_nand_make_request(host,MV_DMA_CMD_WRITE,page & chip->pagemask,1,host->buf, NULL,1);
    if(status)
		goto ERROR;
    status = chip->waitfunc(mtd, chip);
	if(status & NAND_STATUS_FAIL)  {
		printk("read status fail status =0x%x\n",status);
		status =  -EIO;
	}

    ERROR:
#endif
	//mutex_unlock(&mv_nand_mutex);
	memset(chip->oob_poi, 0xff, mtd->oobsize);

	if (status)
		return status;

	ops->oobretlen = ops->ooblen;

	return 0;
}
/**
 * mv_nand_write_oob - [MTD Interface] NAND write data and/or out-of-band
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @ops:	oob operation description structure
 */
static int mv_nand_write_oob(struct mtd_info *mtd, loff_t to,
			  struct mtd_oob_ops *ops)
{
	struct nand_chip *chip = mtd->priv;
	int ret = -ENOTSUPP;

	
	ops->retlen = 0;
    //printk("mv_nand_write_oob\n");
	/* Do not allow writes past end of device */
	if (ops->datbuf && (to + ops->len) > mtd->size) {
		DEBUG(MTD_DEBUG_LEVEL0, "nand_read_oob: "
		      "Attempt read beyond end of device\n");
		return -EINVAL;
	}

	mv_nand_get_device(chip, mtd, FL_WRITING);

	switch(ops->mode) {
	case MTD_OOB_PLACE:
	case MTD_OOB_AUTO:
	case MTD_OOB_RAW:
		break;

	default:
		goto out;
	}

	if (!ops->datbuf) {
		ret = mv_nand_do_write_oob(mtd, to, ops);
	}
	else
		ret = mv_nand_do_write_ops(mtd, to, ops);

 out:
	mv_nand_release_device(mtd);
	return ret;
}
/**
 * mv_nand_sync - [MTD Interface] sync
 * @mtd:	MTD device structure
 *
 * Sync is actually a wait for chip ready function
 */
static void mv_nand_sync(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;

	printk("mv_nand_sync: called\n");

	/* Grab the lock and see if the device is available */
	mv_nand_get_device(chip, mtd, FL_SYNCING);
	/* Release it and go back */
	mv_nand_release_device(mtd);
}

/**
 * mv_nand_suspend - [MTD Interface] Suspend the NAND flash
 * @mtd:	MTD device structure
 */
static int mv_nand_suspend(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;

	return mv_nand_get_device(chip, mtd, FL_PM_SUSPENDED);
}
/**
 * mv_nand_block_isbad - [MTD Interface] Check if block at offset is bad
 * @mtd:	MTD device structure
 * @offs:	offset relative to mtd start
 */
static int mv_nand_block_isbad(struct mtd_info *mtd, loff_t offs)
{
	/* Check for invalid offset */
	if (offs > mtd->size)
		return -EINVAL;

	return mv_nand_block_checkbad(mtd, offs, 1, 0);
}

/**
 * mv_nand_block_markbad - [MTD Interface] Mark block at the given offset as bad
 * @mtd:	MTD device structure
 * @ofs:	offset relative to mtd start
 */
static int mv_nand_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
	struct nand_chip *chip = mtd->priv;
	int ret;

	if ((ret = mv_nand_block_isbad(mtd, ofs))) {
		/* If it was bad already, return success and do nothing. */
		if (ret > 0)
			return 0;
		return ret;
	}

	return chip->block_markbad(mtd, ofs);
}
/**
 * mv_nand_read_page_bch- [Intern] read raw page data without ecc
 * @mtd:    mtd info structure
 * @chip:   nand chip info structure
 * @buf:    buffer to store read data
 * */
static int mv_nand_read_page_bch(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf)
{
	int i;
	struct mv_nand_host *host = chip->priv;
	//printk("read page syndrome\n");
	for(i = 0;i < mtd->writesize/32;i++) {
		chip->read_buf(mtd, buf + i*32, 32);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_ANR, 0);
	}
	chip->read_buf(mtd, chip->oob_poi, host->oobsize);
	return 0;
}

/**
 * mv_nand_read_page_raw - [Intern] read raw page data without ecc
 * @mtd:	mtd info structure
 * @chip:	nand chip info structure
 * @buf:	buffer to store read data
 */
static int mv_nand_read_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
			      uint8_t *buf)
{
	chip->read_buf(mtd, buf, mtd->writesize);
	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);
	return 0;
}
/**
 * mv_nand_write_page_lp
 * @mtd:    MTD device structure
 * @chip:   NAND chip descriptor
 * @buf:    the data to write
 * @page:   page number to write
 * @cached: cached programming
 * @raw:    use _raw version of write_page
 */
static int mv_nand_write_page_lp(struct mtd_info *mtd, struct nand_chip *chip,const uint8_t *buf, int page, int cached, int raw)
{
	int status;
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	//spin_lock_irq(&mv_work_lock);
	mutex_lock(&mv_nand_mutex);
#if 0
	chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
	chip->cmdfunc(mtd, NAND_CMD_READ0, 0, page);
	if(host->read_page(mtd, chip, tmpbuf)) {
		printk("read back error page =0x%x\n",page);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
	    //chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);	
		chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
	}
	chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
#endif
	chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);

	chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00, page);
	if(unlikely(raw))
		chip->ecc.write_page_raw(mtd, chip, buf);
	else
		chip->ecc.write_page(mtd, chip, buf);
	//chip->cmd_ctrl(mtd, MV_NAND_CMD_AUT,0);
	status = chip->waitfunc(mtd,chip);
	if(status & NAND_STATUS_FAIL)  {
		chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC,0);
		mutex_unlock(&mv_nand_mutex);
		//spin_unlock_irq(&mv_work_lock);
		printk("read status fail status =0x%x\n",status);
		return -EIO;
	}
	chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC,0);
	mutex_unlock(&mv_nand_mutex);
#else
	int ret;
	char *tmp ;
	struct mv_nand_host *host = chip->priv;
	//ret = mv_nand_make_request(host,MV_DMA_CMD_READ,page,1,host->buf, NULL,0);
	//if(ret)
		//return ret;
	tmp =(char *) buf;
	ret = mv_nand_make_request(host,MV_DMA_CMD_WRITE,page,1,tmp, chip->oob_poi,0);
	if(ret)
		return ret;
	status = chip->waitfunc(mtd, chip);
	if(status & NAND_STATUS_FAIL)  {
		printk("read status fail status =0x%x\n",status);
		return -EIO;
	}
#endif
	//spin_unlock_irq(&mv_work_lock);
	//spin_unlock_irq();
#ifdef CONFIG_MTD_NAND_VERIFY_WRITE
	//mutex_lock(&mv_nand_mutex);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_STC,0);
	chip->cmdfunc(mtd, NAND_CMD_READ0, 0, page);
		printk("read back error page =0x%x\n",page);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
	    //chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);	
		chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
		//mutex_unlock(&mv_nand_mutex);
		return -EIO;
	}
	chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
	//chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
	if (chip->verify_buf(mtd, buf, mtd->writesize)) {
		printk("verify buffer error page =0x%x\n",page);
		//mutex_unlock(&mv_nand_mutex);
		return -EIO;
	}
	//mutex_unlock(&mv_nand_mutex);
#endif
	return 0;



}
/**
 * mv_nand_write_page - [REPLACEABLE] write one page
 * @mtd:	MTD device structure
 * @chip:	NAND chip descriptor
 * @buf:	the data to write
 * @page:	page number to write
 * @cached:	cached programming
 * @raw:	use _raw version of write_page
 */
static int mv_nand_write_page(struct mtd_info *mtd, struct nand_chip *chip,
			   const uint8_t *buf, int page, int cached, int raw)
{
	int status;
	/* need to trace */
	//mutex_lock(&mv_nand_mutex);
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	mutex_lock(&mv_nand_mutex);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_STC, 0);
#if 0
	chip->cmdfunc(mtd, NAND_CMD_READ0, 0, page);

	if(host->read_page(mtd, chip, tmpbuf)) {
		printk("read back error page =0x%x\n",page);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
		//chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
		mutex_unlock(&mv_nand_mutex);
		return -EIO;
	}
	chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
	//chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);
#endif
	chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00, page);
	if(unlikely(raw))
		chip->ecc.write_page_raw(mtd, chip, buf);
	else
		chip->ecc.write_page(mtd, chip, buf);

	chip->cmd_ctrl(mtd, MV_NAND_CMD_AUT,0);
    //for (i = 0; i < 20000; i++);
	status = chip->waitfunc(mtd, chip);
#if 0
	/* handling auto-status-read after write result */
	status = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);		// read status register
	//printk("mv_nand_write_page status =0x%x\n",status);
	status &= (host->board->ncs ? MV_NAND_NDSR_CS1_BBD : MV_NAND_NDSR_CS0_BBD);
	//printk("mv_nand_write_page status = 0x%x\n",status);
#endif
	if(status & NAND_STATUS_FAIL)  {
		chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC,0);
		mutex_unlock(&mv_nand_mutex);
		printk("read status fail status =0x%x\n",status);
		return -EIO;
		
	}
    chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC,0);
	mutex_unlock(&mv_nand_mutex);
#else
	int ret;
	char *tmp ;
	struct mv_nand_host *host = chip->priv;
	//ret = mv_nand_make_request(host,MV_DMA_CMD_READ,page,1,host->buf, NULL,0);
	//if(ret)
	//	return ret;
	tmp =(char *) buf;
	ret = mv_nand_make_request(host,MV_DMA_CMD_WRITE,page,1,tmp, chip->oob_poi,0);
	if(ret)
		return ret;
	status = chip->waitfunc(mtd, chip);
	if(status & NAND_STATUS_FAIL)  {
		printk("read status fail status =0x%x\n",status);
		return -EIO;
	}

#endif
	//mutex_unlock(&mv_nand_mutex);
#ifdef CONFIG_MTD_NAND_VERIFY_WRITE
	//mutex_lock(&mv_nand_mutex);
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	chip->cmd_ctrl(mtd, MV_NAND_CMD_STC,0);
	/* Send command to read back the data */
	chip->cmdfunc(mtd, NAND_CMD_READ0, 0, page);
	if(host->read_page(mtd, chip, tmpbuf)) {
		printk("read back error page =0x%x\n",page);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
		//mutex_unlock(&mv_nand_mutex);
		return -EIO;
	}
	//printk("CONFIG_MTD_NAND_VERIFY_WRITE\n");
	chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
	//chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC, 0);
#else
	ret = mv_nand_make_request(host,MV_DMA_CMD_READ,page,1,tmp, NULL,1)
	if(ret)
		return ret;
#endif
	if (chip->verify_buf(mtd, buf, mtd->writesize)) {
		printk("verify buffer error page =0x%x\n",page);
		return -EIO;
	}

	//mutex_unlock(&mv_nand_mutex);
#endif
	return 0;
}
static void mv_nand_write_page_hwecc_lp(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf)
{
	struct mv_nand_host *host = chip->priv;
	//printk("mv_nand_write_page_hwecc_lp\n");
	chip->write_buf(mtd, buf, mtd->writesize/2);
	//printk("mtd->writesize/2 = %d host->oobsize/2 = %d",mtd->writesize/2, host->oobsize/2);
	chip->write_buf(mtd, chip->oob_poi , host->oobsize/2);
	//chip->cmd_ctrl(mtd , MV_NAND_CMD_ENP , 0);
	chip->cmdfunc( mtd, NAND_CMD_CACHEDPROG, 0, host->curpage);
	chip->write_buf(mtd, buf + mtd->writesize/2, mtd->writesize/2);
	chip->write_buf(mtd, chip->oob_poi + host->oobsize/2, host->oobsize/2);
	chip->cmdfunc( mtd, NAND_CMD_DEPLETE1, 0, host->curpage);


}
static void mv_nand_write_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf)
{
	struct mv_nand_host *host = chip->priv;
	chip->write_buf(mtd, buf, mtd->writesize);
	chip->write_buf(mtd, chip->oob_poi, host->oobsize);

}
/*
 * mv_nand_write_page_raw - [Intern] raw page write function
 * @mtd:	mtd info structure
 * @chip:	nand chip info structure
 * @buf:	data buffer
 */
static void mv_nand_write_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
				const uint8_t *buf)
{
    //printk("mv_nand_write_page_raw bufsize = 0x%x oobsize = 0x%x\n",mtd->writesize,mtd->oobsize);
	struct mv_nand_host *host = chip->priv;
	chip->write_buf(mtd, buf, mtd->writesize);
	chip->write_buf(mtd, host->buf + mtd->writesize, mtd->oobsize);

}

static int mv_nand_read_oob_hwecc(struct mtd_info *mtd, struct nand_chip *chip, int page, int sndcmd)
{
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	uint8_t *buf = host->buf;
	//printk("mv_nand_read_oob_hwecc\n");
	chip->cmdfunc( mtd, NAND_CMD_READ0, 0, page);
	host->read_page(mtd, chip, buf);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
	//chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);
	memcpy(chip->oob_poi,buf + mtd->writesize ,host->oobsize);
	return 0;
}
/**
 * mv_nand_read_oob_std - [REPLACABLE] the most common OOB data read function
 * @mtd:	mtd info structure
 * @chip:	nand chip info structure
 * @page:	page number to read
 * @sndcmd:	flag whether to issue read command or not
 */
static int mv_nand_read_oob_std(struct mtd_info *mtd, struct nand_chip *chip, int page, int sndcmd)
{   
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	uint8_t *buf = host->buf;

	//printk("mv_nand_read_oob_std page =0x%x sendcmd = %x\n",page,sndcmd);
	chip->cmdfunc( mtd, NAND_CMD_READ0, 0, page);
	//mv_nand_read_page_raw_random( mtd, chip, buf);
	host->read_page(mtd, chip, buf);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
	//chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);
	memcpy(chip->oob_poi,buf + mtd->writesize ,mtd->oobsize);

	return 0;
}
/**
 *  mv_nand_read_page_bch_random - [Intern] read raw page data without ecc
 *  @mtd:   mtd info structure
 *  @chip:  nand chip info structure
 *  @buf:   buffer to store read data
 */

int mv_nand_read_page_bch_random(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf)
{
	int i;
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	for(i = 0;i < mtd->writesize/32;i++) {
		chip->read_buf(mtd, buf + 32*i, 32);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_ANR, 0);
		//printk("bch random i = 0x%x\n",i);
	}
	memset(buf + mtd->writesize,0xFF,mtd->oobsize);
	chip->read_buf(mtd, buf + mtd->writesize, host->oobsize);
	//chip->cmd_ctrl(mtd, MV_NAND_CMD_ANR, 0);
	return 0;
}
int mv_nand_read_page_bch_random_lp(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf)
{
	int i,tmp;
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	tmp = mtd->writesize/2;
	tmp /= 32;
	for(i = 0;i < tmp; i++) {
		chip->read_buf(mtd, buf + i*32, 32);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_ANR, 0);
	}
	memset(buf + mtd->writesize,0xFF,mtd->oobsize);
	chip->read_buf(mtd, buf + mtd->writesize , 32);
    chip->cmdfunc(mtd, NAND_CMD_READ1, 0x00, host->curpage);
    tmp = mtd->writesize/2;
    tmp /= 32;
    for(i = 0;i < tmp; i++) {
	    chip->read_buf(mtd, buf + mtd->writesize/2 + i*32, 32);
	    chip->cmd_ctrl(mtd, MV_NAND_CMD_ANR, 0);
	}
	chip->read_buf(mtd, buf + mtd->writesize + 32, 32);
    return 0;	
}

int mv_nand_read_page_raw_hwecc_lp(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf)
{
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	//printk("mv_nand_read_page_raw_hwecc_lp mtd->writesize = 0x%x host->oobsize0x%x\n", mtd->writesize,host->oobsize);
	chip->read_buf(mtd, buf, mtd->writesize/2);
	memset(buf + mtd->writesize,0xFF,mtd->oobsize);
	chip->read_buf(mtd, buf + mtd->writesize, host->oobsize/2);
	chip->cmdfunc(mtd, NAND_CMD_READ1, 0x00, host->curpage);
	chip->read_buf(mtd, buf + mtd->writesize/2, mtd->writesize/2);
	chip->read_buf(mtd, buf + mtd->writesize + host->oobsize/2, host->oobsize/2);
	
	return 0;

}

int mv_nand_read_page_raw_hwecc(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf)
{
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	chip->read_buf(mtd, buf, mtd->writesize);
	memset(buf + mtd->writesize,0xFF,mtd->oobsize);
	chip->read_buf(mtd, buf + mtd->writesize, host->oobsize);
	return 0;
}
/**
 *  mv_nand_read_page_raw_random - [Intern] read raw page data without ecc
 *  @mtd:	mtd info structure
 *  @chip:	nand chip info structure
 *  @buf:	buffer to store read data
 */
int mv_nand_read_page_raw_random(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf)
{
	int eccbytes = chip->ecc.bytes;
    chip->read_buf(mtd, buf, mtd->writesize);
	chip->read_buf(mtd, buf + mtd->writesize, mtd->oobsize);
	if(eccbytes == 0)
		return 0;
	return 0;
}

static int mv_nand_write_page_raw_hwecc_lp(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf)
{
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	chip->write_buf(mtd, buf, mtd->writesize/2);
	chip->write_buf(mtd, buf +  mtd->writesize, host->oobsize/2);
	chip->cmdfunc(mtd, NAND_CMD_CACHEDPROG, 0x00, host->curpage);
	chip->write_buf(mtd, buf + mtd->writesize/2, mtd->writesize/2);
	chip->write_buf(mtd, buf +  mtd->writesize + host->oobsize/2, host->oobsize/2);
	chip->cmdfunc(mtd, NAND_CMD_DEPLETE1, 0x00, host->curpage);
	return 0;

}
static int mv_nand_write_page_raw_hwecc(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf)
{
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	chip->write_buf(mtd, buf, mtd->writesize);
	chip->write_buf(mtd, buf +  mtd->writesize, host->oobsize);
	return 0;
}
/**
 * mv_nand_write_page_raw_random - [Intern] raw page write function
 * @mtd:	mtd info structure
 * @chip:	nand chip info structure
 * @buf:	data buffer
 */
static int mv_nand_write_page_raw_random(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf)
{
	chip->write_buf(mtd, buf, mtd->writesize);
	/* the oobsize need to be justified in nand_get_flash_type() ,because it is different according to spare_en and ecc setting*/
	chip->write_buf(mtd, buf +  mtd->writesize, mtd->oobsize);
    return 0;
}

static int mv_nand_write_page_random_lp(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf,int page, int cached, int raw)
{
	int status;
	struct mv_nand_host *host = chip->priv;
	chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00, page);
	host->write_page(mtd, chip, buf);
	chip->waitfunc(mtd,chip);
	status = host->status & NAND_STATUS_FAIL;
	if(status) {
		chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC,0);
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		printk("read status fail\n");
		return -EIO;
	}
	
	return 0;

}


static int mv_nand_write_page_random(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf, int page, int cached, int raw)
{
    int status,i;
    struct mv_nand_host *host = chip->priv;
	chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00, page);
	//mv_nand_write_page_raw_random(mtd, chip, buf);
	host->write_page(mtd, chip, buf);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_AUT,0);
    
    for (i = 0; i < 20000; i++); 
	/* handling auto-status-read after write resul */
	status = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);      // read status register
	status &= (host->board->ncs ? MV_NAND_NDSR_CS1_BBD : MV_NAND_NDSR_CS0_BBD);
	if(status)  {
		chip->cmd_ctrl(mtd, MV_NAND_CMD_NFC,0);
	    MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W, MV_NAND_NDCB0_OFFSET, 0);
		printk("read status fail\n");
		return -EIO;
																		
	}
    return 0;
}
static int mv_nand_write_oob_hwecc_lp(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	int ret;
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	uint8_t *buf = host->buf;
	//printk("mv_nand_write_oob_hwecc_lp \n");
	chip->cmdfunc( mtd, NAND_CMD_READ0, 0, page);
	host->read_page(mtd, chip, buf);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);
	memcpy(buf + mtd->writesize, chip->oob_poi,host->oobsize);
	ret = mv_nand_write_page_random_lp( mtd, chip, buf,page, 0, 0);
	if(ret)
		return ret;
	return 0;
}
static int mv_nand_write_oob_hwecc(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	int ret;
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	uint8_t *buf = host->buf;
	//printk("mv_nand_write_oob_hwecc pae =0x%x\n",page);
	chip->cmdfunc( mtd, NAND_CMD_READ0, 0, page);
	host->read_page(mtd, chip, buf);
	chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
	//chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);
	memcpy(buf + mtd->writesize, chip->oob_poi,host->oobsize);
	ret = mv_nand_write_page_random( mtd, chip, buf,page, 0, 0);
	if(ret)
		return ret;
	return 0;
}
/**
 * mv_nand_write_oob_std - [REPLACABLE] the most common OOB data write function
 * @mtd:	mtd info structure
 * @chip:	nand chip info structure
 * @page:	page number to write
 */
static int mv_nand_write_oob_std(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	int ret;
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	uint8_t *buf = host->buf;
    //printk("mv_nand_write_oob_std page=0x%x\n",page);
	/* first read back all the block */
	chip->cmdfunc( mtd, NAND_CMD_READ0, 0, page);
	//mv_nand_read_page_raw_random( mtd, chip, buf);
	host->read_page(mtd, chip, buf);
	/*PAGE DONE*/
	chip->cmd_ctrl(mtd, MV_NAND_CMD_ENP, 0);
    //chip->cmd_ctrl(mtd, MV_NAND_CMD_RTS, 0);	
	//printk("after readpage raw random\n");
	/* modify the spare area */
	memcpy(buf + mtd->writesize, chip->oob_poi,mtd->oobsize);
	/*write the block*/
	ret = mv_nand_write_page_random( mtd, chip, buf,page, 0, 0);
    if(ret)
		return ret;
    return 0;
}
/**
 * mv_nand_resume - [MTD Interface] Resume the NAND flash
 * @mtd:	MTD device structure
 */
static void mv_nand_resume(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;

	if (chip->state == FL_PM_SUSPENDED)
		mv_nand_release_device(mtd);
	else
		printk(KERN_ERR "nand_resume() called for a chip which is not "
		       "in suspended state\n");
}

/**
 * mv_nand_erase - [MTD Interface] erase block(s)
 * @mtd:	MTD device structure
 * @instr:	erase instruction
 *
 * Erase one ore more blocks
 */
static int mv_nand_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	return mv_nand_erase_nand(mtd, instr, 0);
}
/**
 *mv_nand_read_page_bch_lp - [REPLACABLE] hardware ecc based page read function
 * @mtd:    mtd info structure
 * @chip:   nand chip info structure
 * @buf:    buffer to store read data
 */
static int mv_nand_read_page_bch_lp(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf)
{
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	int i,tmp;
	tmp = mtd->writesize/2;
	tmp /=32;
	for(i = 0; i <  tmp; i++) {
		chip->read_buf(mtd, buf + i*32, 32);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_ANR, 0);
	}
	chip->read_buf(mtd, chip->oob_poi, 32);
	chip->cmdfunc(mtd, NAND_CMD_READ1, 0x00, host->curpage);
	tmp = mtd->writesize/2;
	tmp /=32;
	for(i = 0; i <  tmp; i++) {
		chip->read_buf(mtd, buf + mtd->writesize/2 +i*32, 32);
		chip->cmd_ctrl(mtd, MV_NAND_CMD_ANR, 0);
	}
	chip->read_buf(mtd, chip->oob_poi + 32, 32);
	return 0;

}
/**
 * mv_nand_read_page_hwecc_lp - [REPLACABLE] hardware ecc based page read function
 * @mtd:    mtd info structure
 * @chip:   nand chip info structure
 * @buf:    buffer to store read data
 */
static int mv_nand_read_page_hwecc_lp(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf)
{
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	chip->read_buf(mtd, buf, mtd->writesize/2);
	chip->read_buf(mtd, chip->oob_poi, host->oobsize/2);
	chip->cmdfunc(mtd, NAND_CMD_READ1, 0x00, host->curpage);
	chip->read_buf(mtd, buf + mtd->writesize/2 , mtd->writesize/2);
	chip->read_buf(mtd, chip->oob_poi + host->oobsize/2, host->oobsize/2);

	return 0;
	

}

/**
 * mv_nand_read_page_hwecc - [REPLACABLE] hardware ecc based page read function
 * @mtd:	mtd info structure
 * @chip:	nand chip info structure
 * @buf:	buffer to store read data
 * 
 * Not for syndrome calculating ecc controllers which need a special oob layout
 */
static int mv_nand_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf)
{
	struct mv_nand_host *host = (struct mv_nand_host *)chip->priv;
	chip->read_buf(mtd, buf, mtd->writesize);
	chip->read_buf(mtd, chip->oob_poi, host->oobsize);
	return 0;

}


/**
 * mv_nand_scan_tail - [NAND Interface] Scan for the NAND device
 * @mtd:	    MTD device structure
 * @maxchips:	    Number of chips to scan for
 *
 * This is the second phase of the normal nand_scan() function. It
 * fills out all the uninitialized function pointers with the defaults
 * and scans for a bad block table if appropriate.
 */
int mv_nand_scan_tail(struct mtd_info *mtd)
{
	int i;
	struct nand_chip *chip = mtd->priv;
	struct mv_nand_host *host = chip->priv;

	if (!(chip->options & NAND_OWN_BUFFERS))
		chip->buffers = kmalloc(sizeof(*chip->buffers), GFP_KERNEL);
	if (!chip->buffers)
		return -ENOMEM;

	/* Set the internal oob buffer location, just after the page data */
	chip->oob_poi = chip->buffers->databuf + mtd->writesize;

	/*
	 * If no default placement scheme is given, select an appropriate one
	 */
	if (!chip->ecc.layout) {
		switch (mtd->oobsize) {
		case 8:
			chip->ecc.layout = &nand_oob_8;
			break;
		case 16:
			chip->ecc.layout = &nand_oob_16;
			break;
		case 64:
			chip->ecc.layout = &nand_oob_64;
			break;
		case 32:
			chip->ecc.layout = &nand_oob_32;
			break;
		case 128:
			if(host->oobsize == 80 ) {
				chip->ecc.layout = &nand_oob_80;

			} else if( host->oobsize == 64){
				chip->ecc.layout = &nand_oob_64_lp;
			} else {
				printk("error oob scheme for 128\n");
				BUG();
			}
			break;
		default:
			printk(KERN_WARNING "No oob scheme defined for "
			       "oobsize %d\n", mtd->oobsize);
			BUG();
		}
	}
	/*
	 * check ECC mode, default to software if 3byte/512byte hardware ECC is
	 * selected and we have 256 byte pagesize fallback to software ECC
	 */
	/*
	if (!chip->ecc.write_page_raw)
		chip->ecc.write_page_raw = mv_nand_write_page_raw;
    */
	switch (chip->ecc.mode) {
	case NAND_ECC_HW_SYNDROME:
		printk("NAND_ECC_BCH\n");
		if(mtd->writesize <= 2048) {
			if (!chip->write_page)
				chip->write_page = mv_nand_write_page;
			if (!chip->ecc.read_page_raw)
				chip->ecc.read_page_raw = mv_nand_read_page_bch;
			if (!chip->ecc.read_page)
				chip->ecc.read_page = mv_nand_read_page_bch;
			if (!chip->ecc.write_page)
				chip->ecc.write_page = mv_nand_write_page_hwecc;
			if (!chip->ecc.write_page_raw)
				chip->ecc.write_page_raw = mv_nand_write_page_hwecc;
			if (!chip->ecc.read_oob)
				chip->ecc.read_oob = mv_nand_read_oob_hwecc;
			if (!chip->ecc.write_oob)
				chip->ecc.write_oob = mv_nand_write_oob_hwecc;
			chip->ecc.size = mtd->writesize;
			chip->ecc.bytes = 0;
			if(!host->read_page)
				host->read_page = mv_nand_read_page_bch_random; 
			if(!host->write_page)
				host->write_page = mv_nand_write_page_raw_hwecc;
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
			if(!host->read_handler)
				host->read_handler = mv_nand_read_predma_2k_bch;
			if(!host->write_handler)
				host->write_handler = mv_nand_write_predma_2k_bch;
				
#endif
		} else {

			if (!chip->write_page)
				chip->write_page = mv_nand_write_page_lp;
			if (!chip->ecc.read_page_raw)
				chip->ecc.read_page_raw = mv_nand_read_page_bch_lp;
			if (!chip->ecc.read_page)
				chip->ecc.read_page = mv_nand_read_page_bch_lp;
			if (!chip->ecc.write_page)
				chip->ecc.write_page = mv_nand_write_page_hwecc_lp;
			if (!chip->ecc.write_page_raw)
				chip->ecc.write_page_raw = mv_nand_write_page_hwecc_lp;
			if (!chip->ecc.read_oob)
				chip->ecc.read_oob = mv_nand_read_oob_hwecc;
			if (!chip->ecc.write_oob)
				chip->ecc.write_oob = mv_nand_write_oob_hwecc_lp;
			chip->ecc.size = mtd->writesize;
			chip->ecc.bytes = 0;
			if(!host->read_page)
				host->read_page = mv_nand_read_page_bch_random_lp;
			if(!host->write_page)
				host->write_page = mv_nand_write_page_raw_hwecc_lp;

#ifdef CONFIG_MTD_NAND_MARVELL_DMA
			if(!host->read_handler)
				host->read_handler = mv_nand_read_predma_4k_bch;
			if(!host->write_handler)
				host->write_handler = mv_nand_write_predma_4k_bch;
#endif
		}

		break;
	case NAND_ECC_HW:
		printk("NAND_ECC_HW\n");
		/* Use standard hwecc read page function ? */
		if(mtd->writesize <= 2048) {
			if (!chip->write_page)
				chip->write_page = mv_nand_write_page;
		    if (!chip->ecc.read_page)
			    chip->ecc.read_page = mv_nand_read_page_hwecc;
		    if (!chip->ecc.write_page)
			    chip->ecc.write_page = mv_nand_write_page_hwecc;
		    if (!chip->ecc.read_oob)
			    chip->ecc.read_oob = mv_nand_read_oob_hwecc;
		    if (!chip->ecc.write_oob)
			    chip->ecc.write_oob =mv_nand_write_oob_hwecc;
		    if(!host->read_page)
			    host->read_page = mv_nand_read_page_raw_hwecc;
		    if(!host->write_page)
			    host->write_page = mv_nand_write_page_raw_hwecc;
		    if (!chip->ecc.read_page_raw)
			    chip->ecc.read_page_raw = mv_nand_read_page_hwecc;
		    if (!chip->ecc.write_page_raw)
			    chip->ecc.write_page_raw =  mv_nand_write_page_hwecc;
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
			if (mtd->writesize == 2048) {
				if(!host->read_handler)
					host->read_handler = mv_nand_read_predma_2k_eccnobch;
			    if(!host->write_handler)
					host->write_handler = mv_nand_write_predma_2k_bch;
			} else {
				if(!host->read_handler)
					host->read_handler = mv_nand_read_predma_512;
				if(!host->write_handler)
					host->write_handler = mv_nand_write_predma_512;
			}
#endif
		    chip->ecc.size = mtd->writesize;
		    chip->ecc.bytes = 0;
	    } else {
			printk("4 K pages\n");
			if (!chip->write_page)
				chip->write_page = mv_nand_write_page_lp;
			if (!chip->ecc.read_page)
				chip->ecc.read_page = mv_nand_read_page_hwecc_lp;
			if (!chip->ecc.write_page)
				chip->ecc.write_page = mv_nand_write_page_hwecc_lp;
			if (!chip->ecc.read_oob)
				chip->ecc.read_oob = mv_nand_read_oob_hwecc;
			if (!chip->ecc.write_oob)
				chip->ecc.write_oob =mv_nand_write_oob_hwecc_lp;
			if(!host->read_page)
				host->read_page = mv_nand_read_page_raw_hwecc_lp;
			if(!host->write_page)
				host->write_page = mv_nand_write_page_raw_hwecc_lp;
			if (!chip->ecc.read_page_raw)
				chip->ecc.read_page_raw = mv_nand_read_page_hwecc_lp;
			if (!chip->ecc.write_page_raw)
				chip->ecc.write_page_raw =  mv_nand_write_page_hwecc_lp;
			chip->ecc.size = mtd->writesize;
			chip->ecc.bytes = 0;
		}
	    
		break;
	
	case NAND_ECC_NONE:
		printk(KERN_WARNING "NAND_ECC_NONE selected by board driver. "
		       "This is not recommended !!\n");
		if (!chip->write_page)
			chip->write_page = mv_nand_write_page;
		chip->ecc.read_page = mv_nand_read_page_raw;
		chip->ecc.write_page = mv_nand_write_page_raw;
		chip->ecc.read_oob = mv_nand_read_oob_std;
		chip->ecc.write_oob = mv_nand_write_oob_std;
		chip->ecc.size = mtd->writesize;
		chip->ecc.bytes = 0;
		if (!chip->ecc.read_page_raw)
			chip->ecc.read_page_raw = mv_nand_read_page_raw;
		if (!chip->ecc.write_page_raw)
			chip->ecc.write_page_raw = mv_nand_write_page_raw;
		if(!host->read_page)
			host->read_page = mv_nand_read_page_raw_random;
		if(!host->write_page)
			host->write_page = mv_nand_write_page_raw_random;
		break;

	default:
		printk(KERN_WARNING "Invalid NAND_ECC_MODE %d\n",
		       chip->ecc.mode);
		BUG();
	}
  
	/*
	 * The number of bytes available for a client to place data into
	 * the out of band area
	 */
	chip->ecc.layout->oobavail = 0;
	for (i = 0; chip->ecc.layout->oobfree[i].length; i++)
		chip->ecc.layout->oobavail +=
			chip->ecc.layout->oobfree[i].length;
	mtd->oobavail = chip->ecc.layout->oobavail;

	/*
	 * Set the number of read / write steps for one page depending on ECC
	 * mode
	 */
	chip->ecc.steps = mtd->writesize / chip->ecc.size;
	if(chip->ecc.steps * chip->ecc.size != mtd->writesize) {
		printk(KERN_WARNING "Invalid ecc parameters\n");
		BUG();
	}
	chip->ecc.total = chip->ecc.steps * chip->ecc.bytes;

	/*
	 * Allow subpage writes up to ecc.steps. Not possible for MLC
	 * FLASH.
	 */
	if (!(chip->options & NAND_NO_SUBPAGE_WRITE) &&
	    !(chip->cellinfo & NAND_CI_CELLTYPE_MSK)) {
		switch(chip->ecc.steps) {
		case 2:
			mtd->subpage_sft = 1;
			break;
		case 4:
		case 8:
			mtd->subpage_sft = 2;
			break;
		}
	}
	chip->subpagesize = mtd->writesize >> mtd->subpage_sft;

	/* Initialize state */
	chip->state = FL_READY;

	/* De-select the device */
	chip->select_chip(mtd, -1);

	/* Invalidate the pagebuffer reference */
	chip->pagebuf = -1;

	/* Fill in remaining MTD driver data */
	mtd->type = MTD_NANDFLASH;
	mtd->flags = MTD_CAP_NANDFLASH;
	mtd->erase = mv_nand_erase;
	mtd->point = NULL;
	mtd->unpoint = NULL;
	mtd->read = mv_nand_read;
	mtd->write = mv_nand_write;
	mtd->read_oob = mv_nand_read_oob;
	mtd->write_oob = mv_nand_write_oob;
	mtd->sync = mv_nand_sync;
	mtd->lock = NULL;
	mtd->unlock = NULL;
	mtd->suspend = mv_nand_suspend;
	mtd->resume = mv_nand_resume;
	mtd->block_isbad = mv_nand_block_isbad;
	mtd->block_markbad = mv_nand_block_markbad;

	/* propagate ecc.layout to mtd_info */
	mtd->ecclayout = chip->ecc.layout;
    //printk("leaving scan tail\n");
	/* Check, if we should skip the bad block table scan */
	if (chip->options & NAND_SKIP_BBTSCAN)
		return 0;

	/* Build bad block table */
	return chip->scan_bbt(mtd);
}

/* module_text_address() isn't exported, and it's mostly a pointless
 *    test if this is a module _anyway_ -- they'd have to try _really_ hard
 *       to call us from in-kernel code if the core NAND support is modular. */
#ifdef MODULE
#define caller_is_module() (1)
#else
#define caller_is_module() \
		module_text_address((unsigned long)__builtin_return_address(0))
#endif

/**
 * mv_nand_scan - [NAND Interface] Scan for the NAND device
 * @mtd:	MTD device structure
 * @maxchips:	Number of chips to scan for
 *
 * This fills out all the uninitialized function pointers
 * with the defaults.
 * The flash ID is read and the mtd/chip structures are
 * filled with the appropriate values.
 * The mtd->owner field must be set to the module of the caller
 *
 */
int mv_nand_scan(struct mtd_info *mtd, int maxchips)
{
	int ret;

	/* Many callers got this wrong, so check for it for a while... */
	if (!mtd->owner && caller_is_module()) {
		printk(KERN_CRIT "nand_scan() called with NULL mtd->owner!\n");
		BUG();
	}

	ret = mv_nand_scan_ident(mtd, maxchips);
	if (!ret)
		ret = mv_nand_scan_tail(mtd);
	return ret;
}
/**
 * configure controller using default para 
 *
 */
static int mv_nand_chip_init(struct mv_nand_host *host)
{
   int bytes_per_page ,bytes_per_block,pages_per_block,i;
   unsigned int  ndcr,ndtr0cs0,ndtr1cs0 ,ndredel ,ndsr,ndeccctrl;
   int read;
   int galois_stat;

   ndcr = host->board->ndcr;
   ndtr0cs0 = host->board->ndtr0cs0;
   ndtr1cs0 = host->board->ndtr1cs0;
   ndredel = host->board->ndredel;
   ndsr = host->board->ndsr ;
   ndeccctrl = host->board->ndeccctrl;
   bytes_per_page = host->board->szofpg * 1024;
   bytes_per_block = host->board->szofblk * 1024;

   i = ffs(bytes_per_page) - 1;
   if (i >= 16) {
	   printk("invalid KB_per_page %d\n",i);
	   return -1;
   }
   printk("bytes_per_page =0x%x bytes_per_block = 0x%x\n",bytes_per_page,bytes_per_block);
   pages_per_block = bytes_per_block / bytes_per_page;
   ndcr &= ~(((1 << MV_NAND_NDCR_PG_PER_BLK_BITS) - 1) << MV_NAND_NDCR_PG_PER_BLK_LSB);
   if (pages_per_block == 32)
       ndcr|= (PAGES_PER_BLOCK_32 << MV_NAND_NDCR_PG_PER_BLK_LSB);
   else if (pages_per_block == 64)
       ndcr|= (PAGES_PER_BLOCK_64 << MV_NAND_NDCR_PG_PER_BLK_LSB);
   else if (pages_per_block == 128)
       ndcr|= (PAGES_PER_BLOCK_128 << MV_NAND_NDCR_PG_PER_BLK_LSB);
   else if (pages_per_block == 256)
       ndcr|= (PAGES_PER_BLOCK_256 << MV_NAND_NDCR_PG_PER_BLK_LSB);
   else {
	   printk("invalid Pages per Block ratio\n");
	   return -1;
   }
#if 0
   ndcr &= ~(((1 << MV_NAND_NDCR_PG_SIZE_BITS) - 1) << MV_NAND_NDCR_PG_SIZE_LSB);
   if(bytes_per_page == 0x1000) {
	   ndcr |= (MV_NAND_NDCR_PG4096) << MV_NAND_NDCR_PG_SIZE_LSB;
   } else if(bytes_per_page == 0x2000){
	   ndcr |= (MV_NAND_NDCR_PG8192) << MV_NAND_NDCR_PG_SIZE_LSB;
   } else if(bytes_per_page == 0x800) {
	   ndcr |= (MV_NAND_NDCR_PG2048) << MV_NAND_NDCR_PG_SIZE_LSB;
   } else if(bytes_per_page == 0x200) {
       ndcr |= (MV_NAND_NDCR_PG512) << MV_NAND_NDCR_PG_SIZE_LSB;
   } else {
	   printk("invalid Pages SIZE\n");
	   return -1;
   }

#endif
   if (host->board->conspare & 1)
       ndcr |= MV_NAND_NDCR_SPARE_EN;
   else
       ndcr &= ~MV_NAND_NDCR_SPARE_EN;
    			
   if (host->board->conecc & 1)
       ndcr |= MV_NAND_NDCR_ECC_EN;
   else
       ndcr &= ~MV_NAND_NDCR_ECC_EN;
#if 1	
   if (host->board->conbch & 0x7F)
	   ndeccctrl |= (host->board->conbch & 0x7F);
	   //ndeccctrl |= (host->board->ndeccctrl & 0x7F);
   else
	   ndeccctrl &= ~0x7F;
#endif
	
   if (host->board->constp & 1)
	   ndcr |= MV_NAND_NDCR_STOP_ON_UNCOR;
   else
	   ndcr &= ~MV_NAND_NDCR_STOP_ON_UNCOR;
   galois_stat = galois_printk_enable();
	/* Write control register */
	MV_NAND_REG_WRITE_DWORD(host->io_base, MV_NAND_NDCR_OFFSET, 0);						
	/* Read back for sanity check */
	read = MV_NAND_REG_READ_DWORD(host->io_base, MV_NAND_NDCR_OFFSET);					
	printk("NDCR expects 0x%X, reads 0x%X\n", 0, read);
	
	/* write timing register 0 value to controller */
	MV_NAND_REG_WRITE_DWORD(host->io_base, MV_NAND_NDTR0CS0_OFFSET,  MV_NAND_NDTR0CS0_VAL);
	/* Read back for sanity check */
	read = MV_NAND_REG_READ_DWORD(host->io_base,MV_NAND_NDTR0CS0_OFFSET);				
	printk("NDTR0CS0 expects 0x%X, reads 0x%X\n", MV_NAND_NDTR0CS0_VAL, read);
	
	/* write timing register 1 value to controller */
	MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDTR1CS0_OFFSET, MV_NAND_NDTR1CS0_VAL);
	/* Read back for sanity check */
	read = MV_NAND_REG_READ_DWORD(host->io_base,MV_NAND_NDTR1CS0_OFFSET);				
	printk("NDTR1CS0 expects 0x%X, reads 0x%X\n", MV_NAND_NDTR1CS0_VAL, read);
	
	/* write Read Enable Return Delay Register */
	MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDREDEL_OFFSET, ndredel);	
	/* Read back for sanity check */
	read = MV_NAND_REG_READ_DWORD(host->io_base, MV_NAND_NDREDEL_OFFSET);				
	printk("NDREDEL expects 0x%X, reads 0x%X\n", MV_NAND_NDREDEL_VAL, read);

	/* clear status register sticky bits on controller */
	MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDSR_OFFSET, ndsr);	
	/* Read back for sanity check */
	read = MV_NAND_REG_READ_DWORD(host->io_base, MV_NAND_NDSR_OFFSET);					
	printk("NDSR expects 0x%X(ndsr=0x%X) reads 0x%X\n", MV_NAND_NDSR_VAL, ndsr, read);
	/* set cctrl */
	MV_NAND_REG_WRITE_DWORD(host->io_base,MV_NAND_NDECCCTRL_OFFSET, ndeccctrl);	
	/* Read back for sanity check */
	read = MV_NAND_REG_READ_DWORD(host->io_base,MV_NAND_NDECCCTRL_OFFSET);					
	printk("NDECCCTRL expects 0x%X(ndecc=0x%X) reads 0x%X\n", MV_NAND_NDECCCTRL_VAL,
			ndeccctrl, read);

	/* Setup Control Register on controller */
	MV_NAND_REG_WRITE_DWORD(host->io_base, MV_NAND_NDCR_OFFSET, ndcr);	
	/* Read back for sanity check */
	read = MV_NAND_REG_READ_DWORD(host->io_base ,MV_NAND_NDCR_OFFSET);					
	printk("NDCR expects 0x%X, reads 0x%X\n", ndcr, read);
	
	/* Do some dummy reads (intent is to make sure the buffer is empty)*/
	for (i = 0; i < 10; i++)
		read = MV_NAND_REG_READ_DWORD(host->io_base,MV_NAND_NDDB_OFFSET); 
#if 0
	i = 0;
	cfgw.cfgw0.u_dat = 0;
	cfgw.cfgw1.u_devAdr = (int)(host->io_base + MV_NAND_NDCR_OFFSET) & 0x0FFFFFFF;
	cfgw.cfgw1.u_hdr = BCMINSFMT_hdr_CFGW;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw0.u32);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw1.u32);
	i += 4;


	cfgw.cfgw0.u_dat = ndtr0cs0;
	cfgw.cfgw1.u_devAdr = (int)(host->io_base + MV_NAND_NDTR0CS0_OFFSET) & 0x0FFFFFFF;
	cfgw.cfgw1.u_hdr = BCMINSFMT_hdr_CFGW;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw0.u32);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw1.u32);
	i += 4;


	cfgw.cfgw0.u_dat = ndtr1cs0;
	cfgw.cfgw1.u_devAdr = (int)(host->io_base + MV_NAND_NDTR1CS0_OFFSET) & 0x0FFFFFFF;
	cfgw.cfgw1.u_hdr = BCMINSFMT_hdr_CFGW;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw0.u32);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw1.u32);
	i += 4;


	cfgw.cfgw0.u_dat = ndredel;
	cfgw.cfgw1.u_devAdr = (int)(host->io_base + MV_NAND_NDREDEL_OFFSET) & 0x0FFFFFFF;
	cfgw.cfgw1.u_hdr = BCMINSFMT_hdr_CFGW;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw0.u32);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw1.u32);
	i += 4;

	cfgw.cfgw0.u_dat = ndeccctrl;
	cfgw.cfgw1.u_devAdr = (int)(host->io_base + MV_NAND_NDECCCTRL_OFFSET) & 0x0FFFFFFF;
	cfgw.cfgw1.u_hdr = BCMINSFMT_hdr_CFGW;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw0.u32);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw1.u32);
	i += 4;

	cfgw.cfgw0.u_dat = ndsr;
	cfgw.cfgw1.u_devAdr = (int)(host->io_base + MV_NAND_NDSR_OFFSET) & 0x0FFFFFFF;
	cfgw.cfgw1.u_hdr = BCMINSFMT_hdr_CFGW;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw0.u32);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw1.u32);
	i += 4;


	cfgw.cfgw0.u_dat = ndcr | MV_NAND_NDCR_ND_RUN | MV_NAND_NDCR_DMA_EN;
	cfgw.cfgw1.u_devAdr = (int)(host->io_base + MV_NAND_NDCR_OFFSET) & 0x0FFFFFFF;
	cfgw.cfgw1.u_hdr = BCMINSFMT_hdr_CFGW;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw0.u32);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw1.u32);
	i += 4;

	host->board->ndcr = ndcr | MV_NAND_NDCR_ND_RUN | MV_NAND_NDCR_DMA_EN;
	printk("chip init DMA\n");
	dhub_channel_write_cmd(
			&host->PB_dhubHandle,				/*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funbuf),
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			1,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or*/
			0							/*!	Pass in current cmdQ pointer (in 64b word), */
			);
	host->board->ndeccctrl = ndeccctrl;
#endif
	printk("ndcr1 = 0x%08X | 0x%08X\n", host->board->ndcr, MV_NAND_NDCR_ND_RUN);
	host->board->ndcr = ndcr | MV_NAND_NDCR_ND_RUN;
	printk("ndcr2 = 0x%08X\n", host->board->ndcr);
	host->board->ndeccctrl = ndeccctrl;
	galois_printk_restore(galois_stat);
	return 0; 
	
	 
}

/*
 * Read the Device Ready pin.
 */
static int mv_nand_device_ready(struct mtd_info *mtd)
{
	//struct nand_chip *nand_chip = mtd->priv;
	//struct mv_nand_host *host = nand_chip->priv;
	int ret;
	ret = 1;
	return ret;

}
int mv_nand_chip_reinit(struct mv_nand_host *host ,struct mtd_info *mtd)
{
	unsigned int  ndcr;
	int pages_per_block;
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	unsigned int i;
#endif
	if(mtd->writesize != (host->board->szofpg*1024)) {
		printk("detect page size = 0x%x\n",mtd->writesize);
	} else 
		return 0;
	if(mtd->erasesize == 0) {
		printk("error para mtd->erasesize = 0x%x\n",mtd->erasesize);
		return -1;
	}
	if(mtd->writesize == 0) {
		printk("error para mtd->writesize = 0x%x\n",mtd->writesize);
		return -1;
	}
	pages_per_block = mtd->erasesize/mtd->writesize;
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	ndcr = MV_NAND_REG_READ_DWORD(host->io_base ,MV_NAND_NDCR_OFFSET);
	ndcr &= ~(((1 << MV_NAND_NDCR_PG_PER_BLK_BITS) - 1) << MV_NAND_NDCR_PG_PER_BLK_LSB);
	if (pages_per_block == 32)
		ndcr|= (PAGES_PER_BLOCK_32 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else if (pages_per_block == 64)
		ndcr|= (PAGES_PER_BLOCK_64 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else if (pages_per_block == 128)
		ndcr|= (PAGES_PER_BLOCK_128 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else if (pages_per_block == 256)
		ndcr|= (PAGES_PER_BLOCK_256 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else {
		printk("invalid Pages per Block ratio\n");
		return -1;
	}
	if(mtd->writesize == 512) {
		ndcr &= ~(((1 << MV_NAND_NDCR_PG_SIZE_BITS) - 1) << MV_NAND_NDCR_PG_SIZE_LSB);
		
	}
	MV_NAND_REG_WRITE_DWORD(host->io_base, MV_NAND_NDCR_OFFSET, ndcr);
#else
	i = 0;
	ndcr = host->board->ndcr ;
	ndcr &= ~(((1 << MV_NAND_NDCR_PG_PER_BLK_BITS) - 1) << MV_NAND_NDCR_PG_PER_BLK_LSB);
	if (pages_per_block == 32)
		ndcr|= (PAGES_PER_BLOCK_32 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else if (pages_per_block == 64)
		ndcr|= (PAGES_PER_BLOCK_64 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else if (pages_per_block == 128)
		ndcr|= (PAGES_PER_BLOCK_128 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else if (pages_per_block == 256)
		ndcr|= (PAGES_PER_BLOCK_256 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else {
		printk("invalid Pages per Block ratio\n");
		return -1;
	}
	if(mtd->writesize == 512) {
		ndcr &= ~(((1 << MV_NAND_NDCR_PG_SIZE_BITS) - 1) << MV_NAND_NDCR_PG_SIZE_LSB);
		host->board->ndeccctrl = 0;
	}
	MV_NAND_REG_WRITE_DWORD(host->io_base, MV_NAND_NDCR_OFFSET, ndcr);
#if 0
	cfgw.cfgw0.u_dat = ndcr ;
	cfgw.cfgw1.u_devAdr = (int)(host->io_base + MV_NAND_NDCR_OFFSET) & 0x0FFFFFFF;
	cfgw.cfgw1.u_hdr = BCMINSFMT_hdr_CFGW;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw0.u32);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,cfgw.cfgw0.u32);
	i += 4;

	host->board->ndcr = ndcr | MV_NAND_NDCR_ND_RUN | MV_NAND_NDCR_DMA_EN;
	dhub_channel_write_cmd(
			&host->PB_dhubHandle,				/*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funaddr),
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			1,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or*/
			0							/*!	Pass in current cmdQ pointer (in 64b word), */
			);

#endif
#endif
    /* save it back*/
    host->board->ndcr = ndcr;	
	return 0;

}

#ifdef CONFIG_MTD_NAND_MARVELL_DMA
void mv_nand_dma_exrw_init(void *ht)
{
	unsigned int   i;
	SIE_BCMCFGW cfgw;
	unsigned int *p_u32;

	struct mv_nand_host *host = (struct mv_nand_host *)ht;
	unsigned int  ndcr,ndtr0cs0,ndtr1cs0 ,ndredel ,ndsr,ndeccctrl;
	ndcr = host->board->ndcr;
	ndtr0cs0 = host->board->ndtr0cs0;
	ndtr1cs0 = host->board->ndtr1cs0;
	ndredel = host->board->ndredel;
	ndsr = host->board->ndsr ;
	ndeccctrl = host->board->ndeccctrl;
	if (host->board->conspare & 1)
		ndcr &= ~MV_NAND_NDCR_SPARE_EN;
	if (host->board->conecc & 1)
		ndcr &= ~MV_NAND_NDCR_ECC_EN;
	if (host->board->conbch & 0x7F)
		ndeccctrl &= ~0x7F;

	i = 0;
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCR_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	cfgw.u_dat = ndtr0cs0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDTR0CS0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	cfgw.u_dat = ndtr1cs0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDTR1CS0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	cfgw.u_dat = ndredel;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDREDEL_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	cfgw.u_dat = ndeccctrl;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDECCCTRL_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	cfgw.u_dat = ndsr;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDSR_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	cfgw.u_dat = ndcr | MV_NAND_NDCR_ND_RUN;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCR_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

#if 0
	sema.u_pUpdId = 0;
	sema.u_pChkId = 0;
	sema.u_cUpdId = 0;
	sema.u_cChkId = PBSemaMap_dHubSemID_dHub_NFCDat;
	sema.u_hdr = BCMINSFMT_hdr_SEMA;
	p_u32 = (unsigned int *)&sema;

	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	wcmd.u_ddrAdr = (host->dmaaddr);
	wcmd.u_size = 40;
	wcmd.u_chkSemId = 0;
	wcmd.u_updSemId = PBSemaMap_dHubSemID_NFC_DATA_CP;
	wcmd.u_hdr = BCMINSFMT_hdr_WCMD;
	p_u32 = (unsigned int *)&wcmd;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	/* WDAT Read Status ID from dhub local to DDR */

	wdat.u_size = 40;
	wdat.u_mode = 2;
	wdat.u_endian = 0;
	wdat.u_last = 1;
	wdat.u_cUpdId = 0;
	wdat.u_devAdr = (int)(host->io_base + MV_NAND_NDDB_OFFSET) & 0x0FFFFFFF;
	wdat.u_hdr = BCMINSFMT_hdr_WDAT;
	p_u32 = (unsigned int *)&wdat;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;
    for(k = 0; k < i/4;k++)
	    printk("0x%x ",*((int *)(host->funbuf + k*4)));	
#endif
	dhub_channel_write_cmd(
			&host->PB_dhubHandle,				/*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funaddr),
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			1,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or*/
			0							/*!	Pass in current cmdQ pointer (in 64b word), */
			);
	//mv_nand_dma_wait_read_done(host);
	do {
		    i  = MV_NAND_REG_READ_DWORD(MEMMAP_PB_REG_BASE + RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub+ RA_SemaHub_full  ,0);
	}while(!(i & (1 << NFC_DEV_CTL_CHANNEL_ID)));
	MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE, RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_POP, (0x100 | NFC_DEV_CTL_CHANNEL_ID));
	MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE, RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_full,(1 << NFC_DEV_CTL_CHANNEL_ID));
}

void mv_nand_dma_rw_init(void *ht)
{
	unsigned int   i;
	SIE_BCMCFGW cfgw;
	unsigned int *p_u32;
	struct mv_nand_host *host = (struct mv_nand_host *)ht;
	
	unsigned int  ndcr,ndtr0cs0,ndtr1cs0 ,ndredel ,ndsr,ndeccctrl;
	ndcr = host->board->ndcr;
	ndtr0cs0 = host->board->ndtr0cs0;
	ndtr1cs0 = host->board->ndtr1cs0;
	ndredel = host->board->ndredel;
	ndsr = host->board->ndsr ;
	ndeccctrl = host->board->ndeccctrl;
	i = 0;
	cfgw.u_dat = 0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCR_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	cfgw.u_dat = ndtr0cs0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDTR0CS0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	cfgw.u_dat = ndtr1cs0;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDTR1CS0_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	cfgw.u_dat = ndredel;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDREDEL_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	cfgw.u_dat = ndeccctrl;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDECCCTRL_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;

	cfgw.u_dat = ndsr;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDSR_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;


	cfgw.u_dat = ndcr |  MV_NAND_NDCR_ND_RUN;
	cfgw.u_devAdr = (int)(host->io_base + MV_NAND_NDCR_OFFSET) & 0x0FFFFFFF;
	cfgw.u_hdr = BCMINSFMT_hdr_CFGW;
	p_u32 = (unsigned int *)&cfgw;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[0]);
	i += 4;
	MV_NAND_MEM_WRITE_DWORD(host->funbuf ,i ,p_u32[1]);
	i += 4;



	dhub_channel_write_cmd(
			&host->PB_dhubHandle,				/*!	Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,		/*!	Channel ID in $dHubReg !*/
			(host->funaddr),
			i,							/*!	CMD: number of bytes to transfer !*/
			0,							/*!	CMD: semaphore operation at CMD/MTU (0/1) !*/
			0,							/*!	CMD: non-zero to check semaphore !*/
			0,							/*!	CMD: non-zero to update semaphore !*/
			1,							/*!	CMD: raise interrupt at CMD finish !*/
			0,							/*!	Pass NULL to directly update dHub, or*/
			0							/*!	Pass in current cmdQ pointer (in 64b word), */
			);
	do {
		i  = MV_NAND_REG_READ_DWORD(MEMMAP_PB_REG_BASE + RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub+ RA_SemaHub_full  ,0);
	}while(!(i & (1 << NFC_DEV_CTL_CHANNEL_ID)));
	MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE, RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_POP, (0x100 | NFC_DEV_CTL_CHANNEL_ID));
	MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE, RA_pBridge_dHub + RA_dHubReg2D_dHub + RA_dHubReg_SemaHub + RA_SemaHub_full,(1 << NFC_DEV_CTL_CHANNEL_ID));
}



void pb_init(struct mv_nand_host *host)
{
	HDL_semaphore *pSemHandle;
	unsigned int cpuid;
	host->PB_mtu_size = MV_MTU_32_BYTE;
	/* Initialize HDL_dhub with a $dHub BIU instance. */
	dhub_hdl( MEMMAP_PB_REG_BASE + RA_pBridge_tcm,			/*!	Base address of DTCM !*/
			  MEMMAP_PB_REG_BASE + RA_pBridge_dHub,			/*!	Base address of a BIU instance of $dHub !*/
			  &host->PB_dhubHandle                          /*!	Handle to HDL_dhub !*/
			);
	/* Initialize FiFos */
	dhub_channel_cfg(
			&host->PB_dhubHandle , /*!	Handle to HDL_dhub !*/
			READ_DATA_CHANNEL_ID,			/*!	Channel ID in $dHubReg !*/
			READ_DATA_CHAN_CMD_BASE,		/*!	Channel FIFO base address (byte address) for cmdQ !*/
			READ_DATA_CHAN_DATA_BASE,		/*!	Channel FIFO base address (byte address) for dataQ !*/
			READ_DATA_CHAN_CMD_SIZE/8,		/*!	Channel FIFO depth for cmdQ, in 64b word !*/
			READ_DATA_CHAN_DATA_SIZE/8,		/*!	Channel FIFO depth for dataQ, in 64b word !*/
			host->PB_mtu_size,             /*!	See 'dHubChannel.CFG.MTU', 0/1/2 for 8/32/128 bytes !*/
			0,								/*!	See 'dHubChannel.CFG.QoS' !*/
			0,								/*!	See 'dHubChannel.CFG.selfLoop' !*/
			1,								/*!	0 to disable, 1 to enable !*/
			0								/*!	Pass NULL to directly init dHub, or Pass non-zero to receive programming sequence in (adr,data) pairs !*/
			);
	/* Write Data Channel */
	dhub_channel_cfg(
			&host->PB_dhubHandle ,/*!	Handle to HDL_dhub !*/
			WRITE_DATA_CHANNEL_ID,			/*!	Channel ID in $dHubReg !*/
			WRITE_DATA_CHAN_CMD_BASE,		/*!	Channel FIFO base address (byte address) for cmdQ !*/
			WRITE_DATA_CHAN_DATA_BASE,		/*!	Channel FIFO base address (byte address) for dataQ !*/
			WRITE_DATA_CHAN_CMD_SIZE/8,		/*!	Channel FIFO depth for cmdQ, in 64b word !*/
			WRITE_DATA_CHAN_DATA_SIZE/8,	/*!	Channel FIFO depth for dataQ, in 64b word !*/
			host->PB_mtu_size ,/*!	See 'dHubChannel.CFG.MTU', 0/1/2 for 8/32/128 bytes !*/
			0,								/*!	See 'dHubChannel.CFG.QoS' !*/
			0,								/*!	See 'dHubChannel.CFG.selfLoop' !*/
			1,                              /*! 0 to disable, 1 to enable !*/
			0								/*!	Pass NULL to directly init dHub */
			);
	/* Descriptor Channel */
	dhub_channel_cfg(
			&host->PB_dhubHandle ,/*!   Handle to HDL_dhub !*/
			DESCRIPTOR_CHANNEL_ID,			/*!	Channel ID in $dHubReg !*/
			DESCRIPTOR_CHAN_CMD_BASE,		//UNSG32		baseCmd,			/*!	Channel FIFO base address (byte address) for cmdQ !*/
			DESCRIPTOR_CHAN_DATA_BASE,		//UNSG32		baseData,			/*!	Channel FIFO base address (byte address) for dataQ !*/
			DESCRIPTOR_CHAN_CMD_SIZE/8,		//SIGN32		depthCmd,			/*!	Channel FIFO depth for cmdQ, in 64b word !*/
			DESCRIPTOR_CHAN_DATA_SIZE/8,	//SIGN32		depthData,			/*!	Channel FIFO depth for dataQ, in 64b word !*/
			host->PB_mtu_size ,/*!  See 'dHubChannel.CFG.MTU', 0/1/2 for 8/32/128 bytes !*/
			0,								/*!	See 'dHubChannel.CFG.QoS' !*/
			0,								/*!	See 'dHubChannel.CFG.selfLoop' !*/
			1,								/*!	0 to disable, 1 to enable !*/
			0								/*!	Pass NULL to directly init dHub, or */
			);
	/* NFC Device Control (function) Channel */
	dhub_channel_cfg(
			&host->PB_dhubHandle ,/*!   Handle to HDL_dhub !*/
			NFC_DEV_CTL_CHANNEL_ID,			/*!	Channel ID in $dHubReg !*/
			NFC_DEV_CTL_CHAN_CMD_BASE,		//UNSG32		baseCmd,			/*!	Channel FIFO base address (byte address) for cmdQ !*/
			NFC_DEV_CTL_CHAN_DATA_BASE,		//UNSG32		baseData,			/*!	Channel FIFO base address (byte address) for dataQ !*/
			NFC_DEV_CTL_CHAN_CMD_SIZE/8,	//SIGN32		depthCmd,			/*!	Channel FIFO depth for cmdQ, in 64b word !*/
			NFC_DEV_CTL_CHAN_DATA_SIZE/8,	//SIGN32		depthData,			/*!	Channel FIFO depth for dataQ, in 64b word !*/
		    host->PB_mtu_size ,/*!  See 'dHubChannel.CFG.MTU', 0/1/2 for 8/32/128 bytes !*/
			0,								/*!	See 'dHubChannel.CFG.QoS' !*/
			0,								/*!	See 'dHubChannel.CFG.selfLoop' !*/
			1,								/*!	0 to disable, 1 to enable !*/
			0								/*!	Pass NULL to directly init dHub, or */
			);
	pSemHandle = dhub_semaphore(&host->PB_dhubHandle);
	/*configure the semaphore depth to be 1 */
	semaphore_cfg(pSemHandle, READ_DATA_CHANNEL_ID, 1, 0);
	semaphore_cfg(pSemHandle, WRITE_DATA_CHANNEL_ID, 1, 0);
	semaphore_cfg(pSemHandle, DESCRIPTOR_CHANNEL_ID, 1, 0);
	semaphore_cfg(pSemHandle, NFC_DEV_CTL_CHANNEL_ID, 1, 0);

	semaphore_cfg(pSemHandle, PBSemaMap_dHubSemID_dHub_NFCCmd, 1, 0);
	semaphore_cfg(pSemHandle, PBSemaMap_dHubSemID_dHub_NFCDat, 1, 0);
	semaphore_cfg(pSemHandle, PBSemaMap_dHubSemID_NFC_DATA_CP, 1, 0);
	semaphore_cfg(pSemHandle, PBSemaMap_dHubSemID_dHub_APBRx2, 1, 0);
	semaphore_cfg(pSemHandle, PBSemaMap_dHubSemID_APB0_DATA_CP, 1, 0);
	GA_REG_WORD32_READ(MEMMAP_PROCESSOR_ID_REG_BASE, &cpuid);	
	semaphore_intr_enable (
			pSemHandle,
			PBSemaMap_dHubSemID_NFC_DATA_CP,
			0,
			1,
			0,
			0,
			cpuid);
			
}
#define MV_NAND_DMA_IRQ  18
void *mv_nand_dma_alloc(struct platform_device *pdev,
		size_t			size,
		gfp_t mem_flags,
		dma_addr_t		*dma)
{
	return dma_alloc_coherent(NULL, size, dma, GFP_KERNEL);
}

void mv_nand_free_dma(
		struct platform_device *pdev,
		size_t size,
		void 			*addr,
		dma_addr_t		dma
		)
{
	if (!addr)
		return;
	dma_free_coherent(NULL, size, addr, dma);
}

void fill_err_for_read_request(mv_nand_request_t *req,int ctrl)
{
	if(ctrl & MV_NAND_NDSR_UNCERR)
		req->corerr = nfc_uncorerr;
	else
		req->corerr = 0;
}

static void mv_nand_tasklet_finish(unsigned long param)
{
	struct mv_nand_host *host = (struct mv_nand_host*)param;
	unsigned long flags;
	struct mtd_info *mtd = &host->mtd;
	struct nand_chip *chip = mtd->priv;
	int read,ctrl,addidle;
	struct list_head *tmphead = NULL;
	mv_nand_request_t *tmpreq,*req = NULL;
	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	req = host->request_head.cur_request;
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	if(!req) {
		return;
	}
	addidle = 0;
	if( req->cmd_flags == MV_DMA_CMD_READ) {
		if((req->end_request_data) ||(list_empty(&host->request_head.done_head))) { 
			tmpreq = mv_nand_get_read_request(host,req->page + 1,0);
			if(tmpreq ) {
				fill_nand_request(tmpreq,req->page + 1,1,NULL,NULL,MV_DMA_CMD_READ,req->ctl);
				spin_lock_irqsave(&host->request_head.queue_lock, flags);
				tmphead = &host->request_head.queue_head;
				elevator_linus(tmpreq ,tmphead ,&host->request_head.queue_head);
				spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
			}	
		}
	} else {
		//flush_all_waiting_preread_request(host);
	}
	tmphead = NULL;
	tmpreq = NULL;
	if(req->cmd_flags == MV_DMA_CMD_READ) {
		read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W, MV_NAND_NDSR_OFFSET);
		ctrl = 0;
		if(read & MV_NAND_NDSR_UNCERR) {
			ctrl |= (0x1f0000 | MV_NAND_NDSR_UNCERR);
		}
		if(read & MV_NAND_NDSR_CORERR) {
			ctrl |= MV_NAND_NDSR_CORERR;
		}
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, ctrl);

		fill_err_for_read_request(req,ctrl);
		if(req->end_request_data) {
			if(req->singlelen && req->singledata && !req->oobbuffer) {
				dma_unmap_single(&host->pdev->dev,req->singledata,req->singlelen,DMA_FROM_DEVICE);
				req->singlelen = 0;
				req->singledata = 0;
				
			} else if(req->singlelen && req->singledata && req->oobbuffer) {
				dma_unmap_single(&host->pdev->dev,req->singledata,mtd->writesize,DMA_FROM_DEVICE);
				dma_unmap_single(&host->pdev->dev,req->singleoob,host->oobsize,DMA_FROM_DEVICE);
				req->singlelen = 0;
				req->singledata = 0;
				req->singleoob = 0;
			} else if((mtd->writesize == 512) && (!req->singlelen) ) {
				if(req->databuffer) {
					memcpy(req->databuffer , req->dataaddr ,mtd->writesize);
				}
				if(req->oobbuffer) {
					memcpy(req->oobbuffer , req->dataaddr + mtd->writesize ,host->oobsize);
				} else {
					memcpy(req->databuffer + mtd->writesize , req->dataaddr + mtd->writesize,host->oobsize);
				}
			}
		}
		else {
			spin_lock_irqsave(&host->request_head.queue_lock, flags);
			if(!list_empty(&host->request_head.queue_head)) {
				tmphead = host->request_head.queue_head.next;
				while(tmphead !=&host->request_head.queue_head) {
					tmpreq = list_to_request(tmphead);
					if((tmpreq->cmd_flags == MV_DMA_CMD_READ) && (tmpreq->end_request_data)&&(req->page == tmpreq->page)) {
						list_del_init(&tmpreq->queue);
						break;
					}
					tmphead = tmphead->next;
				}
			}
			spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
			if(tmpreq && (tmpreq->cmd_flags == MV_DMA_CMD_READ) && (tmpreq->end_request_data)&&(req->page == tmpreq->page)) {
				if(tmpreq->databuffer) {
					memcpy(tmpreq->databuffer , req->dataaddr ,mtd->writesize);
				}
				if(tmpreq->oobbuffer) {
					memcpy(tmpreq->oobbuffer , req->dataaddr + mtd->writesize ,host->oobsize);
				} else {
					memcpy(tmpreq->databuffer + mtd->writesize , req->dataaddr + mtd->writesize,host->oobsize);
				}
				fill_err_for_read_request(tmpreq,ctrl);
				mv_nand_release_request(tmpreq);
				add_request_to_fly_queue(host,tmpreq);
				done(host ,tmpreq ,0);
				add_request_to_idle_queue(host,req);
			} else 
				add_request_to_done_queue(host,req);
			
		}

	} else if(req->cmd_flags == MV_DMA_CMD_WRITE) { 
		mv_nand_clear_nfcrun(host);
		spin_lock_irqsave(&host->request_head.queue_lock, flags);
		flush_all_done_request(host);
		spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	} else if(req->cmd_flags == MV_DMA_CMD_STATUS) {
		memcpy(req->databuffer , host->dmabuf ,8);
		spin_lock_irqsave(&host->request_head.queue_lock, flags);
		flush_all_done_request(host);
		spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	}
	
	add_request_to_fly_queue(host,req);
	mv_nand_release_request(req);
	done(host ,req ,0);
	spin_lock_irqsave(&host->request_head.queue_lock, flags);
	host->request_head.cur_request = NULL;
	spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
	/*
	if(!list_empty(&host->request_head.queue_head)) {
		tmphead = &host->request_head.queue_head.next;
		tmpreq = list_to_request(tmphead);
		list_del_init(&tmpreq->queue);
		host->request_head.cur_request = tmpreq;
		host->request_head.issue_fn(host,tmpreq);

	}
	*/
#ifdef MV_NAND_USE_THREAD
	wake_up_process(host->request_head.thread);
#else
	//request_queue_t *mq = &host->request_head;

#endif
#if 0
	spin_lock_irqsave(&host->lock, flags);
	if(list_empty(&host->request_head.queue_head)) {
		q = mv_nand_get_queue(host);
		q->active = 0;
		spin_unlock_irqrestore(&host->lock, flags);
		return ;
	}
	req = list_to_request(host->request_head.queue_head.next);
	host->request_head.cur_request = req;
	mv_nand_dma_rw_init(host);
	if(req->cmd_flags == MV_DMA_CMD_READ)
		host->read_handler(host ,req );
	else
		host->write_handler(host ,req);
	spin_unlock_irqrestore(&host->lock, flags);
#endif
}


static int init_request_queue(struct mv_nand_host *host)
{
	int ret = 0,i;
	mv_nand_request_t *req = NULL;
	request_queue_t *q;
	
	struct list_head *head;
	struct mtd_info *mtd = &host->mtd;
	q = mv_nand_get_queue(host);
	INIT_LIST_HEAD(&host->request_head.queue_head);
	INIT_LIST_HEAD(&host->request_head.idle_head);
	INIT_LIST_HEAD(&host->request_head.done_head);
	INIT_LIST_HEAD(&host->request_head.fly_head);
	init_MUTEX(&host->request_head.thread_sem);
	tasklet_init(&host->request_head.finish_tasklet,mv_nand_tasklet_finish, (unsigned long)host);
	head = &q->idle_head;
	for(i = 1; i <= MV_NAND_READ_PAGE_NUM;i++) {
		req = mv_nand_alloc_request();
		if(req == NULL)
			return -ENOMEM;
		req = mv_nand_get_request(req);/*make sure is never released*/
		fill_read_request(req,host->dmabuf + i*(host->oobsize + mtd->writesize),host->dmaaddr +  i*(host->oobsize + mtd->writesize));
		elevator_linus(req ,head ,&q->idle_head);
	}

	return ret;
}
static void release_request_queue(struct mv_nand_host *host)
{
	mv_nand_request_t *req = NULL;
	tasklet_kill(&host->request_head.finish_tasklet);
	while(!list_empty(&host->request_head.idle_head)) {
		req = list_to_request(host->request_head.idle_head.next);
		list_del_init(&req->queue);
		kfree(req);
	}
}

#ifdef MV_NAND_USE_THREAD
static int nand_queue_thread(void *d)
{
	struct mv_nand_host *host = (struct mv_nand_host*)d;
	struct list_head *tmphead;
	mv_nand_request_t *tmpreq = NULL;
	struct mtd_info *mtd = &host->mtd;
	request_queue_t *mq = &host->request_head;

	current->flags |= PF_MEMALLOC;
	set_freezable();

	down(&mq->thread_sem);
	do {
		mv_nand_request_t *req = NULL;

		spin_lock_irq(&mq->queue_lock);

		if (host->request_head.cur_request == NULL) {
			spin_unlock_irq(&mq->queue_lock);
			try_to_freeze();
			spin_lock_irq(&mq->queue_lock);
//			if (suspend_up) {
//				galois_printk("bbb: mq->queue_lock=%d\n", mq->queue_lock.raw_lock);
//				debug_queue(host);
//			}
		}

		set_current_state(TASK_INTERRUPTIBLE);
		if((host->request_head.cur_request == NULL)&&(!list_empty(&host->request_head.queue_head))) {
			req = list_to_request(host->request_head.queue_head.next);
			if(req->cmd_flags == MV_DMA_CMD_DONE) {
				req = NULL;
				
			}
			else {
				list_del_init(&req->queue);
				host->request_head.cur_request = req;
			}
		}
		spin_unlock_irq(&mq->queue_lock);
		if (!req) {
			//printk("schdule\n");
			if (kthread_should_stop()) {
				set_current_state(TASK_RUNNING);
				break;
			}
			up(&mq->thread_sem);
			schedule();
			down(&mq->thread_sem);
			continue;
		}
		set_current_state(TASK_RUNNING);
		if((req->cmd_flags!=MV_DMA_CMD_READ)||!request_exist_in_done_queue(host,req)) {
			if(req->cmd_flags!=MV_DMA_CMD_READ || req->end_request_data) {
				spin_lock_irq(&mq->queue_lock);
				flush_all_done_request(host);
				spin_unlock_irq(&mq->queue_lock);		
			}
			mq->issue_fn((void *)host, req);
		}
		else {
			spin_lock_irq(&mq->queue_lock);
			host->request_head.cur_request = NULL;
			spin_unlock_irq(&mq->queue_lock);
			if(!req->end_request_data) {
				add_request_to_idle_queue(host,req);
			} else {
				tmphead = NULL;
				tmpreq = NULL;
				spin_lock_irq(&mq->queue_lock);
				if(!list_empty(&host->request_head.done_head)) {
					tmphead = host->request_head.done_head.next;
					while(tmphead !=&host->request_head.done_head) {
						tmpreq = list_to_request(tmphead);
						if((tmpreq->cmd_flags == MV_DMA_CMD_READ) && (!tmpreq->end_request_data)&&(req->page == tmpreq->page)) {
							list_del_init(&tmpreq->queue);
							break;
						}
						tmphead = tmphead->next;
					}
				}
				spin_unlock_irq(&mq->queue_lock);
				if(tmpreq && (tmpreq->page == req->page)&&(!tmpreq->end_request_data)) {
					if(req->databuffer) {
						memcpy(req->databuffer , tmpreq->dataaddr ,mtd->writesize);
					}
					if(req->oobbuffer) {
						memcpy(req->oobbuffer , tmpreq->dataaddr + mtd->writesize ,host->oobsize);
					} else {
						memcpy(req->databuffer + mtd->writesize , tmpreq->dataaddr + mtd->writesize,host->oobsize);
					}
					mv_nand_release_request(req);
					add_request_to_fly_queue(host,req);
					done(host ,req ,0);
					tmpreq->page++;
					add_request_to_work_queue(host,tmpreq);
				}
				
			}
		}
		
			
		//CleanEntireDCache();
		//printk("finish send\n");
		//schedule();
	}while(1);
	up(&mq->thread_sem);
	return 0;

}
#endif
#include <linux/kallsyms.h>
static int mv_nand_issure_fn(void *p,mv_nand_request_t * req)
{
	int ret = 0;
	unsigned long flags;
	struct mv_nand_host *host = (struct mv_nand_host*)p;
	//spin_lock_irqsave(&host->lock, flags);
	if(req->cmd_flags == MV_DMA_CMD_READ) {
//		if (suspend_up) {
//			print_fn_descriptor_symbol("bbb, %s is invoked\n",
//					host->read_handler);
//			printk("bbb, read_handler address=%p\n", host->read_handler);
//		}
		mv_nand_dma_rw_init(host);
		ret = host->read_handler(host ,req);
	}
	else if(req->cmd_flags == MV_DMA_CMD_WRITE) {
		mv_nand_dma_rw_init(host);
		ret = host->write_handler(host ,req);
	}
	else if (req->cmd_flags == MV_DMA_CMD_STATUS) {
		mv_nand_dma_exrw_init((void *)host);
		ret = mv_nand_read_status_dma(host,req);
	}
	else if (req->cmd_flags == MV_DMA_CMD_ERASE) {
		spin_lock_irqsave(&host->request_head.queue_lock, flags);
		host->request_head.cur_request = NULL;
		spin_unlock_irqrestore(&host->request_head.queue_lock, flags);
		mv_nand_dma_exrw_init((void *)host);
		mv_nand_erase_dma(host,req->page);
		mv_nand_clear_nfcrun(host);
		flush_all_done_request(host);
		
		mv_nand_release_request(req);
		done(host ,req ,0);


	}
	//spin_unlock_irqrestore(&host->lock, flags);
	return ret;
}
static int init_request_thread(struct mv_nand_host *host)
{
#ifdef MV_NAND_USE_THREAD 
	int ret;
	host->request_head.thread = kthread_run(nand_queue_thread, host, "mvnandqd");
	if(IS_ERR(host->request_head.thread)) {
		ret = PTR_ERR(host->request_head.thread);
		return ret;
	}
#endif
	init_MUTEX(&host->request_head.thread_sem);
	host->request_head.issue_fn = mv_nand_issure_fn; 
	spin_lock_init(&host->request_head.queue_lock);
	return 0;
}
#define MV_NAND_IRQ_NAME "mvnirq"
#endif

#ifdef CONFIG_MTD_PARTITIONS
static const char *mv_nand_part_probes[] = { "cmdlinepart", NULL };
#endif

static int has_video_logo = 0;
static int __init initcall_has_video_logo_setup(char *str)
{
	has_video_logo = 1;
	return 1;
}
__setup("has_video_logo", initcall_has_video_logo_setup);

/*
 * Probe for the NAND device.
 */
static int __init mv_nand_probe(struct platform_device *pdev)
{
	struct mv_nand_host *host;
	struct mtd_info *mtd;
	struct nand_chip *nand_chip;
	int res;

#ifdef CONFIG_MTD_PARTITIONS
	struct mtd_partition *partitions = NULL;
	int num_partitions = 0;
	int i;
#endif

	/* Allocate memory for the device structure (and zero it) */
	host = kzalloc(sizeof(struct mv_nand_host), GFP_KERNEL);
	if (!host) {
		printk(KERN_ERR "mv_nand: failed to allocate device structure.\n");
		return -ENOMEM;
	}
	mv_host = host;
	host->io_base = (void __iomem *)pdev->resource[0].start;
	mtd = &host->mtd;
	nand_chip = &host->nand_chip;
	host->board = pdev->dev.platform_data;
	/* link the private data structures */
	host->pdev = pdev;
	nand_chip->priv = host;		
	mtd->priv = nand_chip;
	mtd->owner = THIS_MODULE;
	host->board->ncs = 0;
	host->board->conecc = 1;
	host->board->conbch = 1;
	nand_chip->IO_ADDR_R = host->io_base;
	nand_chip->IO_ADDR_W = host->io_base;
	if (nand_chip->cmdfunc == NULL)
		nand_chip->cmdfunc = mv_nand_command;
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	//host->funbuf = (void *)kmalloc(MV_NAND_DMA_FUN_SIZE,GFP_KERNEL);
	host->funbuf = mv_nand_dma_alloc(pdev,MV_NAND_DMA_FUN_SIZE,0,(dma_addr_t *)&(host->funaddr));
	if(!host->funbuf){
		printk(KERN_ERR "mv_nand: failed to allocate device fun buffer.\n");
		res = -ENOMEM;
		goto out;
	}
	MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE,RA_pBridge_BCM + RA_BCM_base,(MEMMAP_NAND_FLASH_REG_BASE & 0xF0000000));
	pb_init(host);
	printk("pb_init\n");
#endif

	if(mv_nand_chip_init(host)) {
		printk(KERN_ERR "mv_nand: chip init failed\n");
		res = -EIO;
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
		goto outfun;
#else
		goto out;
#endif
	}

//	galois_printk("bbb,%s: after calling mv_nand_chip_init\n", __func__);
//	dump_registers(nand_chip);

	nand_chip->cmd_ctrl = mv_nand_cmd_ctrl;
	nand_chip->dev_ready = mv_nand_device_ready;

	/* return the status of extra status and ECC checks */
	//nand_chip->errstat = mv_nand_from4_errstat;
	/* check ECC mode*/
	if (!host->board->conecc)
		nand_chip->ecc.mode = NAND_ECC_NONE;
    else if((host->board->conecc) && (!host->board->conbch))
		nand_chip->ecc.mode = NAND_ECC_HW;
	else if(host->board->conbch && host->board->conecc)
		nand_chip->ecc.mode = NAND_ECC_HW_SYNDROME;
	printk("mode = 0x%x\n",nand_chip->ecc.mode); 	
	nand_chip->chip_delay = 0;		

	/* 16-bit bus width */
	if (host->board->bus_width_16)		
		nand_chip->options |= NAND_BUSWIDTH_16;
		
	/* skip bbtscan ?*/
	nand_chip->options   |= NAND_SKIP_BBTSCAN;

	//nand_chip->options   |=NAND_USE_FLASH_BBT;
	platform_set_drvdata(pdev, host);
	/* Scan to find existance of the device */
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	//host->dmabuf = (void *)kmalloc(mtd->writesize + 128,GFP_KERNEL);
	host->dmabuf = mv_nand_dma_alloc(pdev,MV_NAND_READ_BUF_SIZE,0,(dma_addr_t *)&(host->dmaaddr));
	if(!host->dmabuf){
		printk(KERN_ERR "mv_nand: failed to allocate device dma buffer.\n");
		res = -ENOMEM;
		goto outfun;
	}

#endif
	if (mv_nand_scan(mtd,1 )) {
		res = -ENXIO;
		goto outdma;
	}
	host->buf = (void *)kmalloc(mtd->writesize + 128,GFP_KERNEL);
	if(!host->buf){
		printk(KERN_ERR "mv_nand: failed to allocate device buffer.\n");
		res = -ENOMEM;
		goto release;
	}
#ifndef CONFIG_MTD_NAND_MARVELL_DMA
	nand_chip->cmdfunc(mtd, NAND_CMD_RESET, 0x00, 0x00);
#else
#if 1
	galois_printk("reset after probe\n");
	spin_lock_irq(&mv_work_lock);
	mv_nand_dma_exrw_init(host);
	mv_nand_reset_dma(host);
	spin_unlock_irq(&mv_work_lock);
#endif
#endif
	if(mv_nand_chip_reinit(host,mtd)) {
		printk(KERN_ERR "mv_nand: failed to reinit device\n");
		res = -ENXIO;
		goto freebuf;
	}
#ifdef CONFIG_MTD_NAND_MARVELL_DMA 
#ifdef MV_NAND_DMA_INT
	printk("init DMA queue\n");
	spin_lock_init(&host->lock);
#ifdef MV_NAND_USE_THREAD
	res = init_request_thread(host);
	if(res) {
		printk(KERN_ERR "mv_nand:failure init thread \n");
		goto freebuf;
	}
#endif
	res = init_request_queue(host);
	if(res) {
		printk(KERN_ERR "mv_nand:failure init queue \n");
		goto stopthread;
	}
	host->irq = MV_NAND_DMA_IRQ;
	res = request_irq(host->irq, mv_nand_dma_intr, IRQF_SHARED, MV_NAND_IRQ_NAME, host);
	if(res) {
		printk(KERN_ERR "mv_nand:failure requesting irq %i\n",host->irq);
		res = -EBUSY;
		goto releaseq;
	}
	//mv88de3010_open_irq(host->irq); /* enable irq in PIC */
#endif
#endif

	mtd->name = "mv_nand";
#ifdef CONFIG_MTD_PARTITIONS
#ifdef CONFIG_MTD_CMDLINE_PARTS
	num_partitions = parse_mtd_partitions(mtd, mv_nand_part_probes,
								&partitions, 0);
#endif
	if (num_partitions <= 0 && host->board->partition_info)
		partitions = host->board->partition_info(mtd->size, &num_partitions);

	if ((!partitions) || (num_partitions == 0)) {
		printk(KERN_ERR "mv_nand: No parititions defined, or unsupported device.\n");
		res = ENXIO;
		goto freeirq;
	}

	/* adjust the partition size according to practical flash chipsize */
	for (i = 0; i < num_partitions; i++) {
		if ((partitions[i].offset + partitions[i].size) >= nand_chip->chipsize) {
			partitions[i].size = nand_chip->chipsize - partitions[i].offset;
			printk(KERN_INFO "mv_nand: predefined:practical partition number is %d:%d\n", num_partitions, i + 1);
			num_partitions = i + 1;
			break;
		}
	}
	if (i == num_partitions) {
		partitions[i - 1].size = nand_chip->chipsize - partitions[i - 1].offset;
		printk(KERN_INFO "mv_nand: last partition size is enlarged to chip end.\n");
	}

	if (has_video_logo) {
		#define LOGO_PARTITION_SIZE (64*1024*1024)
		/* reserve 60M bytes at the tail of the last parition, add create a new parition */
		if (partitions[num_partitions-1].size <= LOGO_PARTITION_SIZE) {
			printk(KERN_INFO "mv_nand: last partition should be larger than 64M bytes");
			res = -ENOMEM;
		}
		if (!res) {
			partitions[num_partitions-1].size -= LOGO_PARTITION_SIZE;
			printk(KERN_INFO "mv_nand: partition %d resized to 0x%X(%d) bytes.\n",
				num_partitions-1, partitions[num_partitions-1].size,
				partitions[num_partitions-1].size);
			res = add_mtd_partitions(mtd, partitions, num_partitions);
		}
		if (!res) {
			struct mtd_partition logo_parition = {
				.name = "logo",
			};
			logo_parition.size = LOGO_PARTITION_SIZE;
			logo_parition.offset =
				partitions[num_partitions-1].offset +
				partitions[num_partitions-1].size;
			printk(KERN_INFO "mv_nand: add partition %d as logo partition.\n",
				num_partitions);
			res = add_mtd_partitions(mtd, &logo_parition, 1);
		}
	} else {
		res = add_mtd_partitions(mtd, partitions, num_partitions);
	}
	/* register the whole NAND as a single MTD device: */
	if (!res)
		res = add_mtd_device(mtd);
#else
	res = add_mtd_device(mtd);
#endif
	if (!res) {
		nand_chip->block_bad = mv_nand_block_bad_ab;
		return res;

	}
freeirq:
#ifdef MV_NAND_DMA_INT
	free_irq(host->irq,host);
releaseq:
	release_request_queue(host);
stopthread:
#ifdef MV_NAND_USE_THREAD
	kthread_stop(host->request_head.thread);
#endif
#endif
freebuf:
	kfree(host->buf);
release:
	nand_release(mtd);
outdma:
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	mv_nand_free_dma(pdev ,MV_NAND_DMA_FUN_SIZE ,(void *)host->dmabuf ,host->dmaaddr);
outfun:
	mv_nand_free_dma(pdev ,MV_NAND_DMA_FUN_SIZE ,(void *)host->funbuf ,host->funaddr);
#endif
out:
	platform_set_drvdata(pdev, NULL);
	kfree(host);
	return res;
}

/*
 * Remove a NAND device.
 */
static int __devexit mv_nand_remove(struct platform_device *pdev)
{
	struct mv_nand_host *host = platform_get_drvdata(pdev);
	struct mtd_info *mtd = &host->mtd;

#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	free_irq(host->irq, NULL);
	release_request_queue(host);
#ifdef MV_NAND_USE_THREAD
	kthread_stop(host->request_head.thread);
#endif
#endif
	nand_release(mtd);
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	mv_nand_free_dma(pdev ,MV_NAND_DMA_FUN_SIZE ,(void *)host->dmabuf ,host->dmaaddr);
	mv_nand_free_dma(pdev ,MV_NAND_DMA_FUN_SIZE ,(void *)host->funbuf ,host->funaddr);
#endif
	//iounmap(host->io_base);
	kfree(host->buf);
	kfree(host);

	return 0;
}

#ifdef CONFIG_PM
static int mv88de3010_nand_suspend(struct platform_device *pdev,
		pm_message_t state)
{
/*	struct mv_nand_host *host = platform_get_drvdata(pdev);

	if (nand->state & STATE_CMD_PREPARED) {
		dev_err(&pdev->dev, "driver busy, state = %d\n", nand->state);
		return -EAGAIN;
	}

	ndtr0cs0 = nand_readl(nand, NDTR0CS0);
	ndtr1cs0 = nand_readl(nand, NDTR1CS0);*/

	return 0;
}

static void reinit_mv_nand_data(struct mv_nand_host *host)
{
	host->board->ndcr = MV_NAND_NDCR_VAL;
	host->board->ndtr0cs0 = MV_NAND_NDTR0CS0_VAL;
	host->board->ndtr1cs0 = MV_NAND_NDTR1CS0_VAL;
	host->board->ndredel = MV_NAND_NDREDEL_VAL;
	host->board->ndsr = MV_NAND_NDSR_VAL;
	host->board->ndeccctrl = MV_NAND_NDECCCTRL_VAL;
}

#if 0
int nfc_resume_delay = 0;
static int __init setup_nfc_resume_delay(char *str)
{
	get_option(&str, &nfc_resume_delay);

	return 1;
}
__setup("nfc_resume_delay=", setup_nfc_resume_delay);
#endif

extern void mv88de3010_open_irq(unsigned int );
static int mv88de3010_nand_resume(struct platform_device *pdev)
{
	struct mv_nand_host *host = platform_get_drvdata(pdev);
	struct nand_chip *chip = &(host->nand_chip);
#if 0
	galois_printk("%s: wait %ds before resume/n", __func__, nfc_resume_delay);
	mdelay(1000*nfc_resume_delay);
#endif
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	reinit_mv_nand_data(host);
	MV_NAND_REG_WRITE_DWORD(MEMMAP_PB_REG_BASE,RA_pBridge_BCM + RA_BCM_base,
					(MEMMAP_NAND_FLASH_REG_BASE & 0xF0000000));
	pb_init(host);
#endif
	mv_nand_chip_init(host);

//	galois_printk("bbb: after calling mv_nand_chip_init\n");
//	dump_registers(chip);

//	mv_nand_scan(&(host->mtd), 1);

#ifdef CONFIG_MTD_NAND_MARVELL_DMA
	mv_nand_dma_exrw_init(host);
	mv_nand_reset_dma(host);
#else
	{ /* copied from nand_chip->cmdfunc(mtd, NAND_CMD_RESET, 0x00, 0x00);*/
		unsigned int read;
		int ctrl;
		mv_nand_disable(chip);
		mv_nand_start(chip);

		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET,
				(host->board->ncs  ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0));

		//printk("NAND_CMD_RESET ncs = 0x%x\n",host->board->ncs);
		ctrl = 0x00A000FF | (host->board->ncs ? MV_NAND_NDCB_CSEL : 0);
		// CMD_TYPE(0b101, reset), DBC(0), ADDR_CYC(0)
		// CMD1(0xFF, reset cmd)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, ctrl);
		// address cycle 1:4
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, 0);
		// address cycle 5 and page count
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDCB0_OFFSET, 0);
		
		// Wait for RDY bit
		do{
			// read status register
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);	
			// check RDYx bit
			read &= (host->board->ncs ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0);
			//printk("command reset NDSR read = %x\n",read);			
		} while (!read);
		// write 1 to clear sticky bit (RDYx only)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);		

		// Wait for Command-Done bit CSx_CMDD
		do
		{
			// read status register
			read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);	
			//printk("After reset check cmdd = %x\n",read);
			// check CSx_CMDD bit
			read &= (host->board->ncs ? MV_NAND_NDSR_CS1_CMDD : MV_NAND_NDSR_CS0_CMDD);
		} while (!read);
		// write 1 to clear sticky bit (CS0_CMDD only)
		MV_NAND_REG_WRITE_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET, read);		
	}
#endif
	{ /* copied from mv_nand_chip_reinit */
		int pages_per_block = 1 << (chip->phys_erase_shift - chip->page_shift);
		int writesize = 1 << chip->page_shift;
		unsigned int ndcr;
#ifdef CONFIG_MTD_NAND_MARVELL_DMA
		ndcr = host->board->ndcr ;
		ndcr &= ~(((1 << MV_NAND_NDCR_PG_PER_BLK_BITS) - 1) << MV_NAND_NDCR_PG_PER_BLK_LSB);
		if (pages_per_block == 32)
			ndcr|= (PAGES_PER_BLOCK_32 << MV_NAND_NDCR_PG_PER_BLK_LSB);
		else if (pages_per_block == 64)
			ndcr|= (PAGES_PER_BLOCK_64 << MV_NAND_NDCR_PG_PER_BLK_LSB);
		else if (pages_per_block == 128)
			ndcr|= (PAGES_PER_BLOCK_128 << MV_NAND_NDCR_PG_PER_BLK_LSB);
		else if (pages_per_block == 256)
			ndcr|= (PAGES_PER_BLOCK_256 << MV_NAND_NDCR_PG_PER_BLK_LSB);
		else {
			galois_printk("%s: invalid Pages per Block ratio\n", __func__);
			return -1;
		}
		if(writesize == 512) {
			ndcr &= ~(((1 << MV_NAND_NDCR_PG_SIZE_BITS) - 1) << MV_NAND_NDCR_PG_SIZE_LSB);
			host->board->ndeccctrl = 0;
		}
		MV_NAND_REG_WRITE_DWORD(host->io_base, MV_NAND_NDCR_OFFSET, ndcr);
		galois_printk("%s with DMA support finished!\n", __func__);
#else
		ndcr = host->board->ndcr ;
		ndcr &= ~(((1 << MV_NAND_NDCR_PG_PER_BLK_BITS) - 1) << MV_NAND_NDCR_PG_PER_BLK_LSB);
		if (pages_per_block == 32)
			ndcr|= (PAGES_PER_BLOCK_32 << MV_NAND_NDCR_PG_PER_BLK_LSB);
		else if (pages_per_block == 64)
			ndcr|= (PAGES_PER_BLOCK_64 << MV_NAND_NDCR_PG_PER_BLK_LSB);
		else if (pages_per_block == 128)
			ndcr|= (PAGES_PER_BLOCK_128 << MV_NAND_NDCR_PG_PER_BLK_LSB);
		else if (pages_per_block == 256)
			ndcr|= (PAGES_PER_BLOCK_256 << MV_NAND_NDCR_PG_PER_BLK_LSB);
		else {
			printk("invalid Pages per Block ratio\n");
			return -1;
		}
		if(writesize == 512)
			ndcr &= ~(((1 << MV_NAND_NDCR_PG_SIZE_BITS) - 1) << MV_NAND_NDCR_PG_SIZE_LSB);
		MV_NAND_REG_WRITE_DWORD(host->io_base, MV_NAND_NDCR_OFFSET, ndcr);
		galois_printk("%s with gpio support finished!\n", __func__);
#endif
	}
	mv88de3010_open_irq(host->irq); /* enable irq in PIC */
	return 0;
}
#else
#define mv88de3010_nand_suspend NULL
#define mv88de3010_nand_resume  NULL
#endif

static struct platform_driver mv_nand_driver = {
	.probe		= mv_nand_probe,
	.remove		= mv_nand_remove,
	.suspend	= mv88de3010_nand_suspend,
	.resume		= mv88de3010_nand_resume,
	.driver		= {
	    .name	= "mv_nand",
	    .owner	= THIS_MODULE,
	},
};

#ifndef CPU0_IS_NFC_OWNER
int nfc_is_shared = 0;
static int __init setup_shared_nfc(char *str)
{
	nfc_is_shared = 1;
	return 1;
}
__setup("nfc_shared", setup_shared_nfc);

/* 
 * Image-2 and Image-3 will set sync-up bit in RA_Gbl_sw_generic3
 */
#define CHIP_CTRL_SW_GENERIC3_REG	(RA_Gbl_sw_generic3 + MEMMAP_CHIP_CTRL_REG_BASE)
#define NFC_SYNC_REG				(CHIP_CTRL_SW_GENERIC3_REG)

#define NFC_SYNC_BIT				(0x0)
#define NFC_SYNC_DONE(value)		((value) & (1 << NFC_SYNC_BIT))
#define NFC_SYNC_CHECK_INTERVAL		(50)	/* in millisecond */
static void __init obtain_nfc_owner_token(void)
{
	unsigned int i = 0;
	unsigned int count = 500 / NFC_SYNC_CHECK_INTERVAL;
	unsigned int sync_value;
	
	if (nfc_is_shared) {
		galois_printk(KERN_WARNING "NFC sync: ");
		sync_value = *(unsigned int *)NFC_SYNC_REG;
		while (!NFC_SYNC_DONE(sync_value)) {
			if (i % count)
				galois_printk("*");
			msleep_interruptible(NFC_SYNC_CHECK_INTERVAL);
			sync_value = *(unsigned int *)NFC_SYNC_REG;
			i++;
		}
		galois_printk(" %ums elapsed\n", i * NFC_SYNC_CHECK_INTERVAL);
	}

	return;
}
#endif /* CPU0_IS_NFC_OWNER */

#ifdef MV_NAND_IGNORE_BADBLOCK
#include <linux/ctype.h>
#include <linux/proc_fs.h>

#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19))
#include <linux/config.h>
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26))
#define MV_PROC_ROOT  &proc_root
#else
#define MV_PROC_ROOT  NULL
#endif

static struct proc_dir_entry *mv_nand_proc_entry;

static int mv_nand_proc_write(struct file *file, const char *buf,
					 unsigned long count, void *data)
{
	int pos = 0;

	while ((pos < count) && isspace(buf[pos])) {
		pos++;
	}
	if(buf[pos] == 'n'){
		mv_nand_bignore_badblock = 0;
		printk("mv_nand : not erase bad blocks\n");
	}
	else if (buf[pos] == 'y'){
		mv_nand_bignore_badblock = 1;
		printk("mv_nand : erase bad blocks\n");
	}
#ifdef MV_NAND_TEST_TIME
	if(buf[pos] == '-'){
		mv_nand_page_reads = 0;
		mv_nand_page_read_cycles = 0;
		mv_nand_page_writes = 0;
		mv_nand_page_write_cycles = 0;
		mv_nand_page_readoobs = 0;
		mv_nand_page_readoob_cycles = 0;
		mv_nand_page_writeoobs = 0;
		mv_nand_page_writeoob_cycles = 0;
		mv_nand_erases = 0;
		mv_nand_erase_cycles = 0;
	}
#endif
	return count;
}

static void mv_print_status(char * buf, int * length)
{
	struct nand_chip * chip;
	unsigned int read;
	int len = *length;

	if(!mv_host) {
		printk("NO flash chip found through marvell NFC\n");
		return;
	}

	chip = &mv_host->nand_chip;

	// Read back for sanity check
	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDTR0CS0_OFFSET);				
	//len += sprintf(buf+000, "NDTR0CS0  reads 0x%08X\n", read);
	printk("NDTR0CS0  reads 0x%08X\n", read);

	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDTR1CS0_OFFSET);				
	//len += sprintf(buf+len, "NDTR1CS0  reads 0x%08X\n", read);
	printk("NDTR1CS0  reads 0x%08X\n", read);

	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDREDEL_OFFSET);				
	//len += sprintf(buf+len, "NDREDEL   reads 0x%08X\n", read);
	printk("NDREDEL   reads 0x%08X\n", read);

	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDSR_OFFSET);					
	//len += sprintf(buf+len, "NDSR      reads 0x%08X\n", read);
	printk("NDSR      reads 0x%08X\n", read);

	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDCR_OFFSET);					
	//len += sprintf(buf+len, "NDCR      reads 0x%08X\n", read);
	printk("NDCR      reads 0x%08X\n", read);

	read = MV_NAND_REG_READ_DWORD(chip->IO_ADDR_W,MV_NAND_NDECCCTRL_OFFSET);
	//len += sprintf(buf+len, "NDECCCTRL reads 0x%08X\n", read);
	printk("NDECCCTRL reads 0x%08X\n", read);

	*length = len;
}

static int mv_nand_proc_read(char *page,
			   char **start,
			   off_t offset, int count, int *eof, void *data)
{
	int len;
	off_t begin = 0;

	/* hold lock_kernel while traversing yaffs_dev_list */
	lock_kernel();

#ifdef MV_NAND_TEST_TIME
	len = sprintf(page, "mv_nand_page_write_cycles........ %Ld\n", mv_nand_page_write_cycles);
	len += sprintf(page+len, "mv_nand_page_writes.............. %Ld\n", mv_nand_page_writes);
	len += sprintf(page+len, "mv_nand_page_read_cycles......... %Ld\n", mv_nand_page_read_cycles);
	len += sprintf(page+len, "mv_nand_page_reads............... %Ld\n", mv_nand_page_reads);
	len += sprintf(page+len, "mv_nand_page_readoob_cycles...... %Ld\n", mv_nand_page_readoob_cycles);
	len += sprintf(page+len, "mv_nand_page_readoobs............ %Ld\n", mv_nand_page_readoobs);
	len += sprintf(page+len, "mv_nand_page_writeoob_cycles..... %Ld\n", mv_nand_page_writeoob_cycles);
	len += sprintf(page+len, "mv_nand_page_writeoobs........... %Ld\n", mv_nand_page_writeoobs);
	len += sprintf(page+len, "mv_nand_erase_cycles............. %Ld\n", mv_nand_erase_cycles);
	len += sprintf(page+len, "mv_nand_erases................... %Ld\n", mv_nand_erases);
#else
	len = sprintf(page, "mv_nand says hello\n");
#endif

	len += sprintf(page+len, "mv_nand_bignore_badblock......... %d\n", mv_nand_bignore_badblock);

	mv_print_status(page+len, &len);

	if(len+begin > offset+count)
		goto done;
	if(len+begin < offset){
		begin += len;
		len = 0;
	}
	*eof = 1;
done:
	unlock_kernel();

	if(offset>=len+begin)
		return 0;
	*start = page + (offset-begin);
	return ((count < begin+len-offset) ? count : begin+len-offset);
}

static void mv_nand_create_proc_entry(void)
{
	/* Install the proc_fs entry */
	printk("mv_nand creating procfs entry\n");
	mv_nand_proc_entry = create_proc_entry("mv_nand",
					       S_IRUGO | S_IFREG,
					       MV_PROC_ROOT);

	if (mv_nand_proc_entry) {
		mv_nand_proc_entry->write_proc = mv_nand_proc_write;
		mv_nand_proc_entry->read_proc = mv_nand_proc_read;
		mv_nand_proc_entry->data = NULL;
	} else {
		printk("out of memory mv_nand_proc_entry not created\n");
	}

}

#endif /* MV_NAND_IGNORE_BADBLOCK */

static int __init mv_nand_init(void)
{   
#ifndef CPU0_IS_NFC_OWNER
	obtain_nfc_owner_token();
#endif
	led_trigger_register_simple("mv-nand-disk", &mv_nand_led_trigger);
#ifdef MV_NAND_IGNORE_BADBLOCK
	mv_nand_create_proc_entry();
#endif
	return platform_driver_register(&mv_nand_driver);
}


static void __exit mv_nand_exit(void)
{
#ifdef MV_NAND_IGNORE_BADBLOCK
	remove_proc_entry("mv_nand", MV_PROC_ROOT);
#endif
	led_trigger_unregister_simple(mv_nand_led_trigger);
	platform_driver_unregister(&mv_nand_driver);
}


late_initcall(mv_nand_init);
module_exit(mv_nand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hz Chen");
MODULE_DESCRIPTION("NAND Controller driver for Marvell");
