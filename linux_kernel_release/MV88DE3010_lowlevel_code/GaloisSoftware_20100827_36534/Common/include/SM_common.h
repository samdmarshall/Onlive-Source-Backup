#ifndef _SM_H_
#define _SM_H_

#include "com_type.h"

#define SM_ITCM_BASE	0x00000000
#define SM_ITCM_SIZE	0x00008000	// 32k
#define SM_ITCM_END		0x00008000
#define SM_ITCM_ALIAS_SIZE	0x00020000	// 128k

#define SM_DTCM_BASE	0x04000000
#define SM_DTCM_SIZE	0x00004000	// 16k
#define SM_DTCM_END		0x04004000
#define SM_DTCM_ALIAS_SIZE	0x00020000	// 128k

#define SM_APBC_BASE	0x10000000
#define SM_APBC_SIZE	0x00010000	// 64k
#define SM_APBC_END		0x10010000
#define SM_APBC_ALIAS_SIZE	0x00010000	// 64k

#define SM_SECM_BASE	0x40000000
#define SM_SECM_SIZE	0x00010000	//64k
#define SM_SECM_END		0x40010000
#define SM_SECM_ALIAS_SIZE	0x00010000	// 64k

#define SOC_ITCM_BASE	0xf7f80000
#define SOC_DTCM_BASE	0xf7fa0000
#define SOC_APBC_BASE	0xf7fc0000
#define SOC_SECM_BASE	0xf7fd0000

#define SOC_ITCM( addr )	( (INT)addr - SM_ITCM_BASE + SOC_ITCM_BASE )
#define SOC_DTCM( addr )	( (INT)addr - SM_DTCM_BASE + SOC_DTCM_BASE )
#define SOC_APBC( addr )	( (INT)addr - SM_APBC_BASE + SOC_APBC_BASE )
#define SOC_SECM( addr )	( (INT)addr - SM_SECM_BASE + SOC_SECM_BASE )

#if ( CPUINDEX == 2 )
#define SM_APBC( reg )         ( (INT32)(reg) )
#else
#define SM_APBC( reg )         ( (INT32)(reg) - SM_APBC_BASE + SOC_APBC_BASE )
#endif

#define MV_SM_READ_REG32( reg )		( *( (volatile  UINT32*)SM_APBC( reg ) ) )
#define MV_SM_READ_REG8( reg )		( *( (volatile UCHAR*)SM_APBC( reg ) ) )
#define MV_SM_WRITE_REG32( reg, value )	( *( (volatile UINT32*)SM_APBC( reg ) ) = (UINT32)( value ) )
#define MV_SM_WRITE_REG8( reg, value )	( *( (volatile UCHAR*)SM_APBC( reg ) ) = (UCHAR)( value ) )

#define SM_SM_APB_ICTL_BASE		(SM_APBC_BASE)
#define SM_SM_APB_WDT0_BASE		(SM_APBC_BASE + 0x1000)
#define SM_SM_APB_WDT1_BASE		(SM_APBC_BASE + 0x2000)
#define SM_SM_APB_WDT2_BASE		(SM_APBC_BASE + 0x3000)
#define SM_SM_APB_TIMERS_BASE	(SM_APBC_BASE + 0x4000)
#define SM_SM_APB_COUNTER_BASE	(SM_APBC_BASE + 0x5000)
#define SM_SM_APB_SPI_BASE		(SM_APBC_BASE + 0x6000)
#define SM_SM_APB_I2C0_BASE		(SM_APBC_BASE + 0x7000)
#define SM_SM_APB_I2C1_BASE		(SM_APBC_BASE + 0x8000)
#define SM_SM_APB_UART0_BASE	(SM_APBC_BASE + 0x9000)
#define SM_SM_APB_UART1_BASE	(SM_APBC_BASE + 0xA000)
#define SM_SM_APB_UART2_BASE	(SM_APBC_BASE + 0xB000)
#define SM_SM_APB_GPIO_BASE		(SM_APBC_BASE + 0xC000)
#define SM_SM_SYS_CTRL_REG_BASE	(SM_APBC_BASE + 0xD000)

#define	LSb32SS_SM_ICTL_IRQ_SM_WDT0				0
#define	LSb32SS_SM_ICTL_IRQ_SM_WDT1				1
#define	LSb32SS_SM_ICTL_IRQ_SM_WDT2				2
#define	LSb32SS_SM_ICTL_IRQ_SM_TIMERS			3
#if	(BERLIN_CHIP_VERSION >= BERLIN_C_0) 
#define	LSb32SS_SM_ICTL_IRQ_SM_GPIO_B			4
#else
#define	LSb32SS_SM_ICTL_IRQ_RTC		 			4
#endif
#define	LSb32SS_SM_ICTL_IRQ_SM_SPI				5
#define	LSb32SS_SM_ICTL_IRQ_SM_I2C0				6
#define	LSb32SS_SM_ICTL_IRQ_SM_I2C1				7
#define	LSb32SS_SM_ICTL_IRQ_SM_UART0			8
#define	LSb32SS_SM_ICTL_IRQ_SM_UART1			9
#define	LSb32SS_SM_ICTL_IRQ_SM_UART2			10
#define	LSb32SS_SM_ICTL_IRQ_SM_GPIO_A			11
#define	LSb32SS_SM_ICTL_IRQ_ADC		 			12

#define SM_MSG_SIZE			32
#define SM_MSG_BODY_SIZE	( SM_MSG_SIZE - sizeof(INT16) * 2 )
#define SM_MSGQ_TOTAL_SIZE	512
#define SM_MSGQ_HEADER_SIZE	SM_MSG_SIZE
#define SM_MSGQ_SIZE		( SM_MSGQ_TOTAL_SIZE - SM_MSGQ_HEADER_SIZE )
#define SM_MSGQ_MSG_COUNT	( SM_MSGQ_SIZE / SM_MSG_SIZE )

#define MAX_MSG_TYPE 12

typedef struct
{
	INT16	m_iModuleID;
	INT16	m_iMsgLen;
	CHAR	m_pucMsgBody[SM_MSG_BODY_SIZE];
} MV_SM_Message;

typedef struct
{
	INT32	m_iWrite;
	INT32	m_iRead;
	INT32	m_iWriteTotal;
	INT32	m_iReadTotal;
	CHAR	m_Padding[ SM_MSGQ_HEADER_SIZE - sizeof(INT32) * 4 ];
	CHAR	m_Queue[ SM_MSGQ_SIZE ];
} MV_SM_MsgQ;

#define SM_CPU0_INPUT_QUEUE_ADDR	( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 4 )
#define SM_CPU1_INPUT_QUEUE_ADDR	( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 3 )
#define SM_CPU0_OUTPUT_QUEUE_ADDR	( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 2 )
#define SM_CPU1_OUTPUT_QUEUE_ADDR	( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 1 )

#define MV_SM_POWER_WARMDOWN_REQUEST	1//from Linux
#define	MV_SM_POWER_WARMDOWN_REQUEST_2	2	
#define MV_SM_POWER_WARMDOWN_TIME		11//from Linux
#define MV_SM_POWER_WARMUP_TIME			12//from Linux

#define MV_SM_POWER_WARMUP_0_RESP		(MV_SM_STATE_WAIT_WARMUP_0_RESP)
#define MV_SM_POWER_WARMDOWN_1_RESP		(MV_SM_STATE_WAIT_WARMDOWN_RESP)
#define MV_SM_POWER_WARMUP_1_RESP		(MV_SM_STATE_WAIT_WARMUP_RESP)
#define MV_SM_POWER_WARMDOWN_2_RESP		(MV_SM_STATE_WAIT_WARMDOWN_2_RESP)
#define MV_SM_POWER_WARMUP_2_RESP		(MV_SM_STATE_WAIT_WARMUP_2_RESP)
#define MV_SM_POWER_SYS_RESET		0xFF
#define	MV_SoC_STATE_COLDBOOT		0x21

#define MV_SM_IR_Linuxready     30
#define MV_SM_WD_EN				0x12
#define MV_SM_WD_Kickoff		0x24
#define MV_SM_TEMP_EN			0x03
#define MV_SM_TEMP_DISEN		0x0C
#define MV_SM_TEMP_SAMPLE		0xF0
#define MV_SM_TEMP_SAMPLE_ORG	0xF3//get original data
#define MV_SM_TEMP_SAMPLE_CES	0xFC//get Celsius value
#define MV_SM_CEC_APPREADY		0x1
#define MV_SM_CEC_INFO			0x2
#define MV_SM_CEC_TASKSTOP		0x3//SM send to app, shows SM stops CEC task already

#define MV_SM_ID_SYS	1
#define MV_SM_ID_COMM	2
#define MV_SM_ID_IR		3
#define MV_SM_ID_KEY	4
#define MV_SM_ID_POWER	5
#define MV_SM_ID_WD		6
#define MV_SM_ID_TEMP	7
#define MV_SM_ID_VFD	8
#define MV_SM_ID_SPI	9
#define MV_SM_ID_I2C	10
#define MV_SM_ID_UART	11
#define MV_SM_ID_CEC	12

#define SM_Enable_WaitQueue	0x1234
#define SM_Disable_WaitQueue	0x3456
#define		CMD_BASE				0x00//send direct cmd
#define		CMD_PARA_BASE			0x10//send parameter to PowerManagement Module

#define		OFFSET_WARMDOWN			0
#define		OFFSET_WARMDOWN_ONTIME		1
#define		OFFSET_WARMUP_ONTIME		2
#define		OFFSET_WARMDOWN_2		3
#define		OFFSET_CPUBusyLimit 	0
#define		OFFSET_InputIdleLimit 	1
#define 	OFFSET_CPUTimerInv 		2
#define 	OFFSET_DetectTimerInv 	3
#define		OFFSET_ENTER_LOW_SPEED_MODE	(0x100+1)
#define 	OFFSET_ENTER_SLEEP_MODE		(0x100+2)
#define		OFFSET_PRINT_CLOCK_INFO		(0x100+3)
#define     OFFSET_ENTER_NORMAL_SPEED_MODE  (0x100+4)

#define 	CMD_WARMDOWN			(CMD_BASE+OFFSET_WARMDOWN)
#define		CMD_WARMDOWN_2			(CMD_BASE+OFFSET_WARMDOWN_2)
#define 	CMD_WARMDOWN_ONTIME		(CMD_BASE+OFFSET_WARMDOWN_ONTIME)
#define 	CMD_WARMUP_ONTIME		(CMD_BASE+OFFSET_WARMUP_ONTIME)
#define		CMD_SET_CPUBusyLimit 	(CMD_PARA_BASE+OFFSET_CPUBusyLimit)//when cpu usage > (CPUBusyLimit)%, the cpu is defined as busy and Berlin can not enter standby
#define 	CMD_SET_InputIdleLimit	(CMD_PARA_BASE+OFFSET_InputIdleLimit)//If the input has no event in (InputIdleLimit)seconds,Berlin may enter standby, also, the cpu usage should be considered.
#define 	CMD_SET_CPUTimerInv 	(CMD_PARA_BASE+OFFSET_CPUTimerInv)
#define 	CMD_SET_DetectTimerInv 	(CMD_PARA_BASE+OFFSET_DetectTimerInv)
#define		CMD_ENTER_LOW_SPEED_MODE	(CMD_PARA_BASE+OFFSET_ENTER_LOW_SPEED_MODE)
#define		CMD_ENTER_SLEEP_MODE		(CMD_PARA_BASE+OFFSET_ENTER_SLEEP_MODE)
#define		CMD_PRINT_CLOCK_INFO		(CMD_PARA_BASE+OFFSET_PRINT_CLOCK_INFO)
#define     CMD_ENTER_NORMAL_SPEED_MODE (CMD_PARA_BASE+OFFSET_ENTER_NORMAL_SPEED_MODE)

#endif // #ifndef _SM_H_
