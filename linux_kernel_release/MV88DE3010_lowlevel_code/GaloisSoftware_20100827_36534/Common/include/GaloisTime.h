/*******************************************************************************
*******************************************************************************/



#ifndef _GALOISTIME_H_
#define _GALOISTIME_H_

#if defined( __LINUX__ )
#include "time.h"
#elif defined( __ECOS__ )
#include <sys/time.h>
#endif

#include "com_type.h"

#include "ErrorCode.h"

#ifdef __cplusplus
extern "C"  {
#endif

#if !defined(WIN32) && !defined(SoC_PCSim)

extern unsigned long *ptr_system_timer_jiffies;
extern unsigned long *ptr_system_clock_timer;
extern unsigned long *ptr_system_clock_second;
extern unsigned int MV_Time_System_Timer_Clock;			// GaloisTime.c
extern unsigned long _CYGNUM_HAL_RTC_PERIOD_value;		// galois_misc.c

#define CMD_PFTIMER_GET_VALUE 				(0x1000)

#define CMD_ICCEXTRA_GET_SYSTIME			(0x1F07)
#define CMD_ICCEXTRA_TEST_01				(0x1F08)
#define CMD_ICCEXTRA_GET_CLOCK				(0x1F09)
#define CMD_ICCEXTRA_GET_CLOCK_ALL			(0x1F10)

#define MV_Time_FLOAT_TO_FIXED(x, fract_bits)	((INT32)((x) * (0x01LL << fract_bits)))
#define MV_Time_TIMER_Diff(x_new, y_old)	( (x_new < y_old) ? (y_old - x_new): ~(x_new - y_old))

typedef struct _system_clock_data {
         unsigned long   	m_Clock_Second;
         unsigned long     	m_Clock_Timer;
         unsigned long		m_Hardware_Timer;
} system_clock_data_t;

#endif

#if defined(WIN32) || defined(__UBOOT__)
struct timespec {
         long   	tv_sec;         		/* seconds */
         long     	tv_nsec;        		/* nanoseconds */
};

#endif


typedef struct timespec MV_TimeSpec_t, *pMV_TimeSpec_t;
typedef struct timeval 	MV_TimeVal_t, *pMV_TimeVal_t;


typedef PVOID 	MV_HANDLE_Time_t;
typedef PVOID 	*pMV_HANDLE_Time_t;


typedef PVOID 	MV_HANDLE_TimePlay_t;
typedef PVOID 	*pMV_HANDLE_TimePlay_t;

typedef struct {
            UINT16 is_pi;	    /* if 1: pi_id is playitem id; don¡¯t care sub_pi_id and sync_pi_id 
							    if 0: sub_pi_id is sub playitem id; if sync_pi_id == 0xffff, sub playitem is 
							    not synced with playitem; otherwise sync_pi_id is the playitem id synced with the sub playitem id */
            UINT16 pi_id;       /* 0xffff: invalid; otherwise playitem id */
            UINT16 sub_pi_id;   /* 0xffff: invalid; otherwise sub playitem id */
            UINT16 sync_pi_id;  /* 0xffff: invalid; otherwise playitem id synced with the sub playitem id */
} MV_PlayItem_ID_t;

typedef struct _MV_TimePTS
{						// PTS is a 33bit unsigned integer
	UINT32 m_high;		// Only the lowest bit is available for calculating, MSB has some special meanings
	UINT32 m_low;		// 32bit is available
  MV_PlayItem_ID_t PI;
} MV_TimePTS_t, *pMV_TimePTS_t;

#define MV_TIMEPTS_FREQUENCY		(90000)
#define MV_TIMEPTS_HIGHMASK			(0x00000001)
#define MV_TIMEPTS_VALIDMASK		(0x80000000)
#define MV_TIMEPTS_PTSPERHOUR		(MV_TIMEPTS_FREQUENCY * 60 * 60)
#define MV_TIMEPTS_PTSPERMINUTE		(MV_TIMEPTS_FREQUENCY * 60)
#define MV_TIMEPTS_PTSPERSEC		(MV_TIMEPTS_FREQUENCY)

#define	RESET_PTS(x)		        do{(x).m_high = 0; (x).m_low=0;}while(0)
#define	IS_PTS_VALID(x)		        ((x).m_high & MV_TIMEPTS_VALIDMASK)
#define	SET_PTS_VALID(x)	        do{(x).m_high |= MV_TIMEPTS_VALIDMASK;}while(0)


typedef struct _MV_TimePTSCounter
{	
	MV_TimePTS_t	m_Counter;
	INT				m_Counter_Residua_Numerator;

	INT				m_Counter_Step_Integer;
	INT				m_Counter_Step_Numerator;
	UINT			m_Counter_Step_Denominator;

} MV_TimePTSCounter_t, *pMV_TimePTSCounter_t;










typedef enum _MV_TimePlay_FrameType
{
	MV_TimePlay_FrameType_50 = 50,
	MV_TimePlay_FrameType_60 = 60,
	MV_TimePlay_FrameType_75 = 75

} MV_TimePlay_FrameType_t;


typedef enum _MV_TimePlay_TimeType
{
	MV_TimePlay_TimeType_Str = 0,
	MV_TimePlay_TimeType_Num = 1,
	MV_TimePlay_TimeType_BCD = 2

} MV_TimePlay_TimeType_t;


typedef struct _MV_TimePlay_TimeCommon
{
	MV_TimePlay_TimeType_t m_TimeType;
	MV_TimePlay_FrameType_t m_FrameType;
	
} MV_TimePlay_TimeCommon_t, *pMV_TimePlay_TimeCommon_t;


typedef struct _MV_TimePlay_TimeStr
{
	MV_TimePlay_TimeType_t m_TimeType;
	MV_TimePlay_FrameType_t m_FrameType;	
	
	CHAR m_Time[12];		// HH:MM:SS:FF\0
	
} MV_TimePlay_TimeStr_t, *pMV_TimePlay_TimeStr_t;


typedef struct _MV_TimePlay_TimeNum
{
	MV_TimePlay_TimeType_t m_TimeType;
	MV_TimePlay_FrameType_t m_FrameType;	
	
	UCHAR m_HH;				// HH
	UCHAR m_MM;				// MM
	UCHAR m_SS;				// SS
	UCHAR m_FF;				// FF
	
} MV_TimePlay_TimeNum_t, *pMV_TimePlay_TimeNum_t;


typedef struct _MV_TimePlay_TimeBCD		// BCD = 8421
{
	MV_TimePlay_TimeType_t m_TimeType;
	MV_TimePlay_FrameType_t m_FrameType;
	
	UCHAR m_HH_BCD;			// HH = 4:4
	UCHAR m_MM_BCD;			// HH = 3:4		1bit pading
	UCHAR m_SS_BCD;			// HH = 3:4
	UCHAR m_FF_BCD;			// HH = 3:4
	
} MV_TimePlay_TimeBCD_t, *pMV_TimePlay_TimeBCD_t;






HRESULT  MV_TimePTS_Equal(MV_TimePTS_t *a, MV_TimePTS_t *b);


HRESULT MV_TimePTS_Compare(MV_TimePTS_t *a, MV_TimePTS_t *b);



HRESULT  MV_TimePTS_Set(MV_TimePTS_t *a, UINT64 b);


UINT64  MV_TimePTS_Get(MV_TimePTS_t *a);


MV_TimePTS_t MV_TimePTS_Add(MV_TimePTS_t *a, MV_TimePTS_t *b);



INT64  MV_TimePTS_Sub(MV_TimePTS_t *a, MV_TimePTS_t *b);

INT32  MV_TimePTS_Diff(MV_TimePTS_t *a, MV_TimePTS_t *b, UINT32 *pts_diff);


MV_TimePTS_t  MV_TimePTS_SetbyPTS(MV_TimePTS_t *a);



MV_TimePTS_t MV_TimePTS_AddbyUINT64(MV_TimePTS_t *a, UINT64 b);







MV_TimeVal_t  MV_TimePTS_ToTimeVal(MV_TimePTS_t *a);


MV_TimeSpec_t  MV_TimePTS_ToTimeSpec(MV_TimePTS_t *a);




HRESULT MV_TimePTS_ToTimePlay(MV_TimePTS_t *a, MV_HANDLE_TimePlay_t b);






HRESULT  MV_TimePTSCounter_Set(pMV_TimePTSCounter_t pCounter, MV_TimePTS_t *a);


HRESULT  MV_TimePTSCounter_SetStep(pMV_TimePTSCounter_t pCounter,
									INT 				Integer, 
									INT 				Numerator, 
									UINT 				Denominator);


HRESULT  MV_TimePTSCounter_Get(pMV_TimePTSCounter_t pCounter, MV_TimePTS_t *a);



HRESULT  MV_TimePTSCounter_GetStep(pMV_TimePTSCounter_t pCounter,
									INT 				*Integer, 
									INT 				*Numerator, 
									UINT 				*Denominator);


HRESULT  MV_TimePTSCounter_Step(pMV_TimePTSCounter_t pCounter);



HRESULT  MV_TimePTSCounter_MultiStep(pMV_TimePTSCounter_t pCounter, UINT Multiple);



HRESULT  MV_TimePTSCounter_Add(pMV_TimePTSCounter_t pCounter, MV_TimePTS_t *a);


HRESULT  MV_TimePTSCounter_Sub(pMV_TimePTSCounter_t pCounter, MV_TimePTS_t *a);


HRESULT  MV_TimePTSCounter_Equal(pMV_TimePTSCounter_t pCounter, MV_TimePTS_t *a);


HRESULT  MV_TimePTSCounter_Compare(pMV_TimePTSCounter_t pCounter, MV_TimePTS_t *a);









HRESULT  MV_TimeSpec_Equal(MV_TimeSpec_t *a, MV_TimeSpec_t *b);


HRESULT  MV_TimeSpec_Compare(MV_TimeSpec_t *a, MV_TimeSpec_t *b);





MV_TimeSpec_t  MV_TimeSpec_Add(MV_TimeSpec_t *a, MV_TimeSpec_t *b);



MV_TimeSpec_t  MV_TimeSpec_Sub(MV_TimeSpec_t *a, MV_TimeSpec_t *b);


MV_TimePTS_t  MV_TimeSpec_ToTimePTS(MV_TimeSpec_t *a);


HRESULT  MV_TimeSpec_ToTimePlay(MV_TimeSpec_t *a, MV_HANDLE_TimePlay_t b);







HRESULT  MV_TimeVal_Equal(MV_TimeVal_t *a, MV_TimeVal_t *b);


HRESULT  MV_TimeVal_Compare(MV_TimeVal_t *a, MV_TimeVal_t *b);



MV_TimeVal_t  MV_TimeVal_Add(MV_TimeVal_t *a, MV_TimeVal_t *b);



MV_TimeVal_t  MV_TimeVal_Sub(MV_TimeVal_t *a, MV_TimeVal_t *b);


MV_TimePTS_t  MV_TimeVal_ToTimePTS(MV_TimeVal_t *a);


HRESULT  MV_TimeVal_ToTimePlay(MV_TimeVal_t *a, MV_HANDLE_TimePlay_t b);






MV_TimePTS_t MV_TimePlay_ToTimePTS(MV_HANDLE_TimePlay_t a);



HRESULT MV_TimePlay_SetByPTS(MV_HANDLE_TimePlay_t a, MV_TimePlay_FrameType_t FrameType, MV_TimePlay_TimeType_t TimeType, MV_TimePTS_t *b);


HRESULT  MV_TimePlay_Equal(MV_HANDLE_TimePlay_t a, MV_HANDLE_TimePlay_t b);


HRESULT  MV_TimePlay_Compare(MV_HANDLE_TimePlay_t a, MV_HANDLE_TimePlay_t b);



HRESULT  MV_TimePlay_Add(MV_HANDLE_TimePlay_t a, MV_HANDLE_TimePlay_t b);



HRESULT MV_TimePlay_Sub(MV_HANDLE_TimePlay_t a, MV_HANDLE_TimePlay_t b);



MV_TimeVal_t  MV_TimePlay_ToTimeVal(MV_HANDLE_TimePlay_t a);



MV_TimeSpec_t  MV_TimePlay_ToTimeSpec(MV_HANDLE_TimePlay_t a);






HRESULT MV_TimeSpec_Delay(MV_TimeSpec_t *pTime);


HRESULT MV_TimePTS_Delay(MV_TimePTS_t *pTime);


UINT32 MV_Time_GetOSTimeMS( void );


HRESULT MV_Time_Init(void);


HRESULT MV_Time_Exit(void);


UINT32 MV_Time_GetPFTIMER( void );


HRESULT MV_Time_GetSysTime(MV_TimeSpec_t *pTimeSpec);


UINT32 MV_Time_GetSysTimeMS( void );


HRESULT Output_MV_TimePTS(MV_TimePTS_t *a);

#ifndef BERLIN_SINGLE_CPU
UINT32 MV_Time_GetTIMER6(void);
#else
#define MV_Time_GetTIMER6   MV_Time_GetTIMER7
#endif

UINT32 MV_Time_GetTIMER7(void);

INT64 MV_Get_System_Time_Tick( void );

#ifdef __cplusplus
}
#endif


#endif
