#ifndef CYGONCE_KERNEL_MON_CPUSWITCH_INL
#define CYGONCE_KERNEL_MON_CPUSWITCH_INL


// stched.cxx
#ifdef CYGFUN_KERNEL_THREADS_CPUSWITCH

#define SYSTEM_TIMER_VALUE						(0xFFFFFFFF)		/* defined in com_config.h */
#define MV_CC_HAL_MEMBOUNDARY					(64)				/* defined in com_config.h */
#define MV_REG_TIMER_6							(6)					/* defined in com_config.h */

typedef unsigned int        UINT32; 								/* defined in com_type.h */



__externC int  MV_CC_ICCFIFOInit(void *pHandle, unsigned int FrameNMax, unsigned int FrameSize);
__externC int  MV_CC_ICCFIFOWrite(void *pHandle, unsigned char *pSrcBuf);
__externC unsigned int   TIMERx_GetCurrentValue(unsigned int  timer_N);

#define LOG_SWITCH_TASK					(1)
#define LOG_SWITCH_INT_START			(2)
#define LOG_SWITCH_INT_END				(3)
#define LOG_SWITCH_DSR_START			(4)
#define LOG_SWITCH_DSR_END				(5)
#define LOG_SWITCH_CLOCK				(6)
#define LOG_SWITCH_TIMESLICE			(7)
#define LOG_SWITCH_BUF_ABV				(99)

typedef struct _data_task_switch {
	
	unsigned short m_out_task_id;
	unsigned short m_out_task_state;

	unsigned short m_in_task_id;
	unsigned short m_in_task_state;
	
} data_task_switch_t;

typedef struct _data_interrupt_switch {
	
	unsigned int m_vector;
	unsigned int m_count;
	
} data_interrupt_switch_t;

typedef struct _data_system_clock {
	
	unsigned int m_current_system_clock;
	unsigned int m_last_system_clock;
	
} data_system_clock_t;

typedef struct _data_timeslice {
	
	unsigned int m_last_timeslice_count;
	unsigned int m_next_timeslice_count;
	
} data_timeslice_t;

typedef struct _data_common_switch {
	
	unsigned int m_data1;
	unsigned int m_data2;
	
} data_common_switch_t;

typedef struct _MV_MON_LogType_CPUSwitch {
	
	UINT32			m_type;							// data type = Task Switch
													// data type = Interrupt Switch START/END
	UINT32			m_index;						
	UINT32			m_timestamp;
	
	union			{
		
		data_task_switch_t		m_task;				// data type = Task Switch
		data_interrupt_switch_t	m_interrupt;		// data type = Interrupt Switch START/END
		data_system_clock_t		m_clock;			// data type = LOG_SWITCH_CLOCK
		data_timeslice_t		m_timeslice;		// data type = LOG_SWITCH_TIMESLICE
		data_common_switch_t	m_common;			// data type = others
		
	};
	
} MV_MON_LogType_CPUSwitch_t, *pMV_MON_LogType_CPUSwitch_t;


#define TEST_CPU_SWITCH_TIMER					(MV_REG_TIMER_6)
#define TEST_CPU_SWITCH_LIST_ELEMENT_NUMBER		(4096 * 10)
#define TEST_CPU_SWITCH_LIST_ELEMENT_SIZE		(sizeof(MV_MON_LogType_CPUSwitch_t))

unsigned char CPUSwtichList[TEST_CPU_SWITCH_LIST_ELEMENT_NUMBER * TEST_CPU_SWITCH_LIST_ELEMENT_SIZE + MV_CC_HAL_MEMBOUNDARY];
void *pCPUSwtichList = CPUSwtichList;
unsigned int	flag_cpu_switch = 0;


////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_TEST_CPU_SWITCH_TASK_Add
//!
//! Description: Add a task switch record into CPU Switch List
//! 
//!	Warning:	It is an inline function only for eCos OS
//!
//! \param task_switches 	(IN): -- the number of the task switch
//! \param out_task_id 		(IN): -- the id of the task switched out
//! \param out_task_state 	(IN): -- the state of the task switched out
//! \param in_task_id 		(IN): -- the id of the task switched in
//! \param in_task_state 	(IN): -- the state of the task switched in
//!
//! \return Return:		
//! 
////////////////////////////////////////////////////////////////////////////////
inline void MV_TEST_CPU_SWITCH_TASK_Add( unsigned int task_switches,
										unsigned short out_task_id,
										unsigned short out_task_state,
										unsigned short in_task_id,
										unsigned short in_task_state)
{
	MV_MON_LogType_CPUSwitch_t LogData;

    cyg_uint32 old_intr;
    HAL_DISABLE_INTERRUPTS(old_intr);
    	
	if (flag_cpu_switch)	
	{
		LogData.m_type = LOG_SWITCH_TASK;
		LogData.m_index = task_switches;
		LogData.m_timestamp = SYSTEM_TIMER_VALUE - TIMERx_GetCurrentValue(TEST_CPU_SWITCH_TIMER);
		LogData.m_task.m_out_task_id = out_task_id;
		LogData.m_task.m_out_task_state = out_task_state;
		LogData.m_task.m_in_task_id = in_task_id;
		LogData.m_task.m_in_task_state = in_task_state;	
	
		MV_CC_ICCFIFOWrite(pCPUSwtichList, (unsigned char *)&LogData);
	}

	HAL_RESTORE_INTERRUPTS(old_intr);
}

inline void MV_TEST_CPU_SWITCH_CLOCK_Add( 	unsigned int current_system_clock,
											unsigned int last_system_clock)
{
	MV_MON_LogType_CPUSwitch_t LogData;

    cyg_uint32 old_intr;
    HAL_DISABLE_INTERRUPTS(old_intr);
    	
	if (flag_cpu_switch)	
	{
		LogData.m_type = LOG_SWITCH_CLOCK;
		LogData.m_index = 0;
		LogData.m_timestamp = SYSTEM_TIMER_VALUE - TIMERx_GetCurrentValue(TEST_CPU_SWITCH_TIMER);
		LogData.m_clock.m_current_system_clock = current_system_clock;
		LogData.m_clock.m_last_system_clock = last_system_clock;
	
		MV_CC_ICCFIFOWrite(pCPUSwtichList, (unsigned char *)&LogData);
	}

	HAL_RESTORE_INTERRUPTS(old_intr);
}

inline void MV_TEST_CPU_SWITCH_TIMESLICE_Add( unsigned int last_timeslice_count,
											  unsigned int next_timeslice_count)
{
	MV_MON_LogType_CPUSwitch_t LogData;

    cyg_uint32 old_intr;
    HAL_DISABLE_INTERRUPTS(old_intr);
    	
	if (flag_cpu_switch)	
	{
		LogData.m_type = LOG_SWITCH_TIMESLICE;
		LogData.m_index = 0;
		LogData.m_timestamp = SYSTEM_TIMER_VALUE - TIMERx_GetCurrentValue(TEST_CPU_SWITCH_TIMER);
		LogData.m_timeslice.m_last_timeslice_count = last_timeslice_count;
		LogData.m_timeslice.m_next_timeslice_count = next_timeslice_count;
	
		MV_CC_ICCFIFOWrite(pCPUSwtichList, (unsigned char *)&LogData);
	}

	HAL_RESTORE_INTERRUPTS(old_intr);
}


#endif



#endif // ifndef CYGONCE_KERNEL_MON_CPUSWITCH_INL
// EOF mon_copuswitch.inl
