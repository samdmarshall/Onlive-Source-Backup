#ifndef __SOC_WATCHDOG_HEADER__
#define __SOC_WATCHDOG_HEADER__

#include "com_type.h"
#include "GaloisTime.h"

#define MaxNameLen	20


#ifdef WD_DEBUG
#define WDLOGPRINTF(x) 	printf x
#else
#define WDLOGPRINTF(x) 	do ;while(0)
#endif

typedef struct WD_MOD{
	int status;//1:module is normal, 0: module timeout
	int enable;
	MV_TimeSpec_t	period;
	char name[MaxNameLen];//module name
	struct WD_MOD * pnext;
	MV_TimeSpec_t lastkickofftime;
}Watchdog_Module;

typedef struct WD_Center{
	int num_module;//number of register watchdog module
	char to_mod_name[MaxNameLen];//module name that timeout
	char name[MaxNameLen];
	Watchdog_Module * plisthead;
}Watchdog_Center;

Watchdog_Module * WatchdogModule_Register(Watchdog_Center * pwdc,char * pname,int msperiod);
HRESULT WatchdogModule_Unregister(Watchdog_Center * pwdc,Watchdog_Module * pmodule);
HRESULT WatchdogModule_Kickoff(Watchdog_Module * pmodule);

#endif /* __SOC_WATCHDOG_HEADER__ */

