/*
 * HBA house-keeper timer used to listen hotplug message and process it.
 * Timer module?
 * and generic req->eh_timer functions
 */
#include "mv_include.h"

/* how long a time between which should each keeper work be done */
#define KEEPER_SHIFT HZ/2

static hba_msg_queue mv_msg_queue;
static struct workqueue_struct *mv_msg_workqueue;
static struct timer_list mv_keeper_timer;

static int __msg_queue_state;

/* non-zero value will make keeper_timer stop re-registrating itself */
static unsigned int mv_keeper_exit_flag;


static inline int queue_state_get(void)
{
	return __msg_queue_state;
}

static inline void queue_state_set(int state)
{
	__msg_queue_state = state;
}

static void hba_proc_msg(hba_msg *pmsg)
{
	PHBA_Extension phba;
	struct scsi_device *psdev;

	/* we don't do things without pmsg->data */
	if ( NULL == pmsg->data )
		return;

	phba = (PHBA_Extension) Module_GetHBAExtension(pmsg->data);

	
	MV_DBG(DMSG_HBA, "__MV__ In hba_proc_msg.\n");

	MV_ASSERT(pmsg);

	switch (pmsg->msg) {
	case EVENT_DEVICE_ARRIVAL:
		if ( scsi_add_device(phba->host, 0, pmsg->param, 0) )
			MV_DBG(DMSG_SCSI, 
			       "__MV__ add scsi disk %d-%d-%d failed.\n",
			       0, pmsg->param, 0);
		else
			MV_DBG(DMSG_SCSI,
			       "__MV__ add scsi disk %d-%d-%d.\n",
			       0, pmsg->param, 0);
		break;
	case EVENT_DEVICE_REMOVAL:
		psdev = scsi_device_lookup( phba->host, 0, pmsg->param, 0);

		if ( NULL != psdev ) {
			MV_DBG(DMSG_SCSI, 
			       "__MV__ remove scsi disk %d-%d-%d.\n",
			       0, pmsg->param, 0);
			scsi_remove_device(psdev);
			scsi_device_put(psdev);
		} else {
			MV_DBG(DMSG_SCSI,
			       "__MV__ no disk to remove %d-%d-%d\n",
			       0, pmsg->param, 0);
		}
		break;
	default:
		break;
	}
}

/* a work queue func */
static void mv_proc_queue(struct work_struct *data)
{
	hba_msg *pmsg;
	
	/* work on queue non-stop, pre-empty me! */
	queue_state_set(MSG_QUEUE_PROC);

	while (1) {
		MV_DBG(DMSG_HBA, "__MV__ process queue starts.\n");
		MV_LOCK_IRQ(&mv_msg_queue.lock);
		if ( list_empty(&mv_msg_queue.tasks) ) {
			/* it's important we put queue_state_set here. */
			queue_state_set(MSG_QUEUE_IDLE);
			MV_UNLOCK_IRQ(&mv_msg_queue.lock);
			MV_DBG(DMSG_HBA, "__MV__ process queue ends.\n");
			break;
		}
		pmsg = list_entry(mv_msg_queue.tasks.next, hba_msg, msg_list);
		MV_UNLOCK_IRQ(&mv_msg_queue.lock);

		hba_proc_msg(pmsg);
		
		/* clean the pmsg before returning it to free?*/
		pmsg->data = NULL;
		MV_LOCK_IRQ(&mv_msg_queue.lock);
		list_move_tail(&pmsg->msg_list, &(mv_msg_queue.free));
		MV_UNLOCK_IRQ(&mv_msg_queue.lock);
		MV_DBG(DMSG_HBA, "__MV__ process queue ends.\n");
	}

}
static DECLARE_WORK(mv_msg_work, mv_proc_queue);

static inline MV_U32 hba_msg_queue_empty(void)
{
	return list_empty(&(mv_msg_queue.tasks));
}


static void hba_house_keeper(unsigned long data)
{
	/* test to see if it's neccessary, when you have time - A.C. */
	if ( mv_keeper_exit_flag ) {
		MV_DBG(DMSG_HBA, "__MV__ Mom calls me home.\n");
		return;
	}

	if (!hba_msg_queue_empty() && MSG_QUEUE_IDLE == queue_state_get()) {
		if ( !queue_work(mv_msg_workqueue, &mv_msg_work))
			MV_DBG(DMSG_HBA, "__MV__ work queue insert error.\n");
	}

	mv_keeper_timer.expires = jiffies + KEEPER_SHIFT;
	add_timer(&mv_keeper_timer);
}


static void hba_msg_queue_init(void)
{
	int i;
	
	spin_lock_init(&mv_msg_queue.lock);

	/* as we're in init, there should be no need to hold the spinlock*/
	INIT_LIST_HEAD(&(mv_msg_queue.free));
	INIT_LIST_HEAD(&(mv_msg_queue.tasks));

	for (i=0; i<MSG_QUEUE_DEPTH; i++) {
		list_add_tail(&mv_msg_queue.msgs[i].msg_list, &mv_msg_queue.free);
	}
	
	mv_msg_workqueue = create_singlethread_workqueue("MV_RAID");
}

void hba_house_keeper_init(void)
{
	hba_msg_queue_init();

	queue_state_set(MSG_QUEUE_IDLE);

	init_timer(&mv_keeper_timer);
}

void hba_house_keeper_run(void)
{
	mv_keeper_timer.function = hba_house_keeper;
	mv_keeper_timer.data     = 0;
	mv_keeper_timer.expires  = jiffies+KEEPER_SHIFT;

	add_timer(&mv_keeper_timer);
}

void hba_house_keeper_exit(void)
{
	
	mv_keeper_exit_flag = 1; /* stop its re-registration */

	/* stop timer before workqueue as work is scheduled by timer */
	del_timer_sync(&mv_keeper_timer);

	flush_workqueue(mv_msg_workqueue);

	/* in fact, destroy_workqueue does flush_workqueue ...  */
	destroy_workqueue(mv_msg_workqueue);
}

void hba_msg_insert(void *data, unsigned int msg, unsigned int param)
{
	hba_msg *pmsg;
	unsigned long flags;

	MV_DBG(DMSG_HBA, "__MV__ msg insert  %d.\n", msg);

	spin_lock_irqsave(&mv_msg_queue.lock, flags);
	if ( list_empty(&mv_msg_queue.free) ) {
		/* should wreck some havoc ...*/
		MV_DBG(DMSG_HBA, "-- MV -- Message queue is full.\n");
		spin_unlock_irqrestore(&mv_msg_queue.lock, flags);
		return;
	}

	pmsg = list_entry(mv_msg_queue.free.next, hba_msg, msg_list);
	pmsg->data = data;
	pmsg->msg  = msg;

	switch (msg) {
	case EVENT_DEVICE_REMOVAL:
	case EVENT_DEVICE_ARRIVAL:
		pmsg->param = param;
		break;
	default:
		pmsg->param = param;
                /*(NULL==param)?0:*((unsigned int*) param);*/
		break;
	}

	list_move_tail(&pmsg->msg_list, &mv_msg_queue.tasks);
	spin_unlock_irqrestore(&mv_msg_queue.lock, flags);
}

/*****************************************
 * HBA TIMER module?
 *****************************************/
MV_U32 Timer_GetResourceQuota(MV_U16 max_io)
{
	return 0;
}

void Timer_Initialize(
	IN OUT PTimer_Module This,
	IN MV_PU8 pool)
{
	;
}

void Timer_Stop(PTimer_Module This)
{
}

/*****************************************
 * Generic timer processing in PMV_Request.eh_timeout
 *****************************************/
#if defined(SUPPORT_ERROR_HANDLING)
/* for req */
void hba_add_timer(PMV_Request req, int timeout,
		   MV_VOID (*function)(MV_PVOID data))
{
	req->eh_timeout.data = (unsigned long)req;
	/* timeout is in unit of second */
	req->eh_timeout.expires = jiffies + timeout*HZ;
	req->eh_timeout.function = (void (*)(unsigned long)) function;

	add_timer(&req->eh_timeout);
	return;
}
	
void hba_remove_timer(PMV_Request req)
{
	/* should be using del_timer_sync, but ... HBA->lock ... */
	if ( req->eh_timeout.function ) {
		del_timer(&req->eh_timeout);
		req->eh_timeout.function = NULL;
	}
}

void hba_init_timer(PMV_Request req)
{
	/*
	 * as we have no init routine for req, we'll do init_timer every 
	 * time it is used until we could uniformly init. all reqs
	 */
	req->eh_timeout.function = NULL;
	init_timer(&req->eh_timeout);
}
#endif
