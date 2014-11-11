#include "ctypes.h"
#include "Galois_memmap.h"
#include "galois_io.h"
#include "MctrlDual.h"
#ifndef WIN32
#include "global.h"
#endif
#include "sys_memmap.h"
#include "GaloisTime.h"
#include "galois_speed.h"


#define BFM_HOST_Bus_Write32    GA_REG_WORD32_WRITE
#define BFM_HOST_Bus_Read32     GA_REG_WORD32_READ

#if defined(BERLIN)
#ifdef __ECOS__
#define	HW_show_speed(a)	printf a

#else
#ifdef	__LINUX_KERNEL__
#define	HW_show_speed(a)	
#endif

#ifdef	__UBOOT__
#define	HW_show_speed(a)	printf a
#endif

#ifdef BERLIN_SINGLE_CPU
#define HW_show_speed(a)	printf a
#endif

#endif

#define DYNAMIC_POWER_CONTROL




static SOC_FREQ_VECTOR mSocFreqVector={0};
const unsigned int clock_divider[] =
{
	1,
	2,
	4,
	6,
	8,
	12,
	1,
	1
};

static SOC_FREQ_VECTOR mSocOrigDivSaved = {0};
static SOC_FREQ_VECTOR mSocDivSaved = {0};

#if (BERLIN_CHIP_VERSION >= BERLIN_B_0)
float VcoDiv[] = 
{
	1,		/// * 0000: 1
	1.5,	/// * 0001: 1.5
	2,		/// * 0010: 2
	2.5,	/// * 0011: 2.5
	3,		/// * 0100: 3
	4,		/// * 0101: 4
	8,		/// * 0110: 8
	6,		/// * 0111: 6
	1,		/// * 1000: undefined
	10,		/// * 1001: 10
	12,		/// * 1010: 12
	14,		/// * 1011: 14
	1,		/// * 1100: undefined
	1,		/// * 1101: undefined
	1,		/// * 1110: undefined
	1		/// * 1111: undefined
};
const char *AVPLL[] =
{
	"AVPllA5",
	"AVPllA6",
	"AVPllB6",
	"AVPllB7",
	"CPU1Pll"
};
#endif


#if (BERLIN_CHIP_VERSION >= BERLIN_B_0)
static enum AVPLL_FREQ
{
	VCO_FREQ_1_258G=0,
	VCO_FREQ_1_350G,
	VCO_FREQ_1_510G,
	VCO_FREQ_1_620G,
	VCO_FREQ_1_854G,
	VCO_FREQ_2_225G,
};





int	get_avpll_B6_clock(unsigned int * avpll_b6_clock){
	
	T32Gbl_avPllCtl reg;
	T32Gbl_avPllCtl10 regData10;
	T32Gbl_avPllCtl16 regData16;
	T32Gbl_avPllCtl18 regData18;

	int	B6_Freq_Offset ; 
	int	B6_avPllDiv ; 
	int	B6_avPllDiv2 ;
	
	//	Normally, the steps to caculate the AVPLL_B6 are:
	//	1.	read register to get the reference clock
	//	2.	read register to get the avPllDiv2 and avPllDiv
	//	3.	read register to get the Freq_Offset and 1000PPM
	//	4.	caculate the B6 clock out
	
	//	Since a lot of constraints,	for Berlin, AVPLL_B's clock reference is always 1.620G
	//	To fit the clock constraints,	B6 clock will always 800M or 400M
	//	for 800M & 400M, here is the register value 

#define AVPLL_B6_DIV2_800                           (0x0000A000)
#define AVPLL_B6_ppmOffset_800                      (0x199A)

#define AVPLL_B6_DIV2_810                           (0x0000A000)
#define AVPLL_B6_ppmOffset_810                      (0)

#define AVPLL_B6_DIV2_405                           (0)
#define AVPLL_B6_ppmOffset_405                      (0)

#define AVPLL_B6_DIV2_400                           (0)
#define AVPLL_B6_ppmOffset_400                      (0x199A)
	
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), &regData10.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), &regData16.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), &regData18.u32);

	B6_Freq_Offset = regData10.uavPllCtl_avPllFreq_Offset_B6 ;
	B6_avPllDiv = regData16.uavPllCtl_avPllDiv_B6 ;	
	B6_avPllDiv2 = regData18.uavPllCtl_avPllDiv2_B6 ;

	HW_show_speed(("%s:	B6_Freq_Offset = %d, B6_avPllDiv = %d, B6_avPllDiv2 = %d\n" ,  \
					__FUNCTION__ , B6_Freq_Offset, B6_avPllDiv, B6_avPllDiv2 )) ;
	
	if(B6_avPllDiv2&0x8){
		//	avPllDiv2 is enabe and the div is 2
		*avpll_b6_clock = 800 ;
	}else{
		//	avPllDiv2 is disable, using avPllDiv, avPllDiv is 4
		*avpll_b6_clock = 400 ;
	}

	HW_show_speed(("%s, avpll_b6_clock is %d\n", __FUNCTION__, *avpll_b6_clock));
	return 0 ;
}

#endif


unsigned int get_divider(unsigned int D3Switch, unsigned int Switch, unsigned int Select)
{
	unsigned int divider;
	if (D3Switch)
		divider = 3;
	else
	{
		if (!Switch)
			divider = 1;
		else
			divider = clock_divider[Select];
	}
	return divider;
}
#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
void show_speed(void)
{
	unsigned int r14;		// SYSPLL
	unsigned int r20;		// MEMPLL
	unsigned int r84;		// SWITCH
	unsigned int r7c;		// SELECT 0
	unsigned int r80;		// SELECT 1
	unsigned int sysPll;
	unsigned int memPll;
	unsigned int FbDiv, RfDiv;
	unsigned int divider;
	unsigned int D3Switch;
	unsigned int Switch;
	unsigned int Select;

	// memPll
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_memPllCtl), &r20);
	FbDiv = (r20 >> LSb32Gbl_memPllCtl_memPllFbDiv) & ((1 << bGbl_memPllCtl_memPllFbDiv) - 1);
	RfDiv = (r20 >> LSb32Gbl_memPllCtl_memPllRfDiv) & ((1 << bGbl_memPllCtl_memPllRfDiv) - 1);
	memPll = (FbDiv + 2) * 25 / (RfDiv + 2);
		// DDR
        mSocFreqVector.ddr = memPll / 2;
	if(mSocDivSaved.ddr ==0) mSocDivSaved.ddr = 2;
	HW_show_speed((" DDR       frequency %d\n", memPll / 2));
	
	// sysPll
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl), &r14);
	FbDiv = (r14 >> LSb32Gbl_sysPllCtl_sysPllFbDiv) & ((1 << bGbl_sysPllCtl_sysPllFbDiv) - 1);
	RfDiv = (r14 >> LSb32Gbl_sysPllCtl_sysPllRfDiv) & ((1 << bGbl_sysPllCtl_sysPllRfDiv) - 1);
	sysPll = (FbDiv + 2) * 25 / (RfDiv + 2);

		// CPU
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &r84);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &r7c);
	{
		D3Switch = (r84 >> LSb32Gbl_ClkSwitch_cpu1ClkD3Switch) & 1;
		Switch = (r84 >> LSb32Gbl_ClkSwitch_cpu1ClkSwitch) & 1;
		Select = (r7c >> LSb32Gbl_clkSelect_cpu1ClkSel) & ((1 << bGbl_clkSelect_cpu1ClkSel) - 1);
		divider = get_divider(D3Switch, Switch, Select);
                mSocFreqVector.cpu1 = sysPll / divider;
		if(mSocDivSaved.cpu1 ==0) mSocDivSaved.cpu1 = divider;
		HW_show_speed((" CPU%d      frequency %d\n", 1, sysPll / divider));
	}
	{
		D3Switch = (r84 >> LSb32Gbl_ClkSwitch_cpu0ClkD3Switch) & 1;
		Switch = (r84 >> LSb32Gbl_ClkSwitch_cpu0ClkSwitch) & 1;
		Select = (r7c >> LSb32Gbl_clkSelect_cpu0ClkSel) & ((1 << bGbl_clkSelect_cpu0ClkSel) - 1);
		divider = get_divider(D3Switch, Switch, Select);
                mSocFreqVector.cpu0 = sysPll / divider;
		if(mSocDivSaved.cpu0 == 0) mSocDivSaved.cpu0 = divider;
		HW_show_speed((" CPU%d      frequency %d\n", 0, sysPll / divider));
	}
	
	{
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &r80);
		
		// sysPll
                mSocFreqVector.syspll = sysPll;
		if(mSocDivSaved.syspll ==0 ) mSocDivSaved.syspll = 1;
		HW_show_speed((" sysPll    frequency %d\n", sysPll));
		
		// sysCLK
		D3Switch = (r84 >> LSb32Gbl_ClkSwitch_sysClkD3Switch) & 1;
		Switch = (r84 >> LSb32Gbl_ClkSwitch_sysClkSwitch) & 1;
		Select = (r7c >> LSb32Gbl_clkSelect_sysClkSel) & ((1 << bGbl_clkSelect_sysClkSel) - 1);
		divider = get_divider(D3Switch, Switch, Select);
                mSocFreqVector.sys = sysPll / divider;
		if(mSocDivSaved.sys ==0) mSocDivSaved.sys = divider;
		HW_show_speed((" sysClk    frequency %d\n", sysPll / divider));
		
		// vScopeCLK
		D3Switch = (r84 >> LSb32Gbl_ClkSwitch_vScopeClkD3Switch) & 1;
		Switch = (r84 >> LSb32Gbl_ClkSwitch_vScopeClkSwitch) & 1;
		Select = (r7c >> LSb32Gbl_clkSelect_vScopeClkSel) & ((1 << bGbl_clkSelect_vScopeClkSel) - 1);
		divider = get_divider(D3Switch, Switch, Select);
                mSocFreqVector.vscope = sysPll / divider;
		if(mSocDivSaved.vscope ==0) mSocDivSaved.vscope = divider;
		HW_show_speed((" vScopeClk frequency %d\n", sysPll / divider));
		
		// pCubeCLK
		D3Switch = (r84 >> LSb32Gbl_ClkSwitch_pCubeClkD3Switch) & 1;
		Switch = (r84 >> LSb32Gbl_ClkSwitch_pCubeClkSwitch) & 1;
		Select = (r7c >> LSb32Gbl_clkSelect_pCubeClkSel) & ((1 << bGbl_clkSelect_pCubeClkSel) - 1);
		divider = get_divider(D3Switch, Switch, Select);
                mSocFreqVector.pcube = sysPll / divider;
		if(mSocDivSaved.pcube ==0) mSocDivSaved.pcube = divider;
		HW_show_speed((" pCubeClk  frequency %d\n", sysPll / divider));
		
		// vppCLK
		D3Switch = (r84 >> LSb32Gbl_ClkSwitch_vppSysClkD3Switch) & 1;
		Switch = (r84 >> LSb32Gbl_ClkSwitch_vppSysClkSwitch) & 1;
		Select = (r80 >> LSb32Gbl_clkSelect_vppSysClkSel) & ((1 << bGbl_clkSelect_vppSysClkSel) - 1);
		divider = get_divider(D3Switch, Switch, Select);
                mSocFreqVector.vpp = sysPll / divider;
		if(mSocDivSaved.vpp ==0) mSocDivSaved.vpp = divider;
		HW_show_speed((" vppClk    frequency %d\n", sysPll / divider));
		
		// cfgCLK
		D3Switch = (r84 >> LSb32Gbl_ClkSwitch_cfgClkD3Switch) & 1;
		Switch = (r84 >> LSb32Gbl_ClkSwitch_cfgClkSwitch) & 1;
		Select = (r7c >> LSb32Gbl_clkSelect_cfgClkSel) & ((1 << bGbl_clkSelect_cfgClkSel) - 1);
		divider = get_divider(D3Switch, Switch, Select);
                mSocFreqVector.cfg = sysPll / divider;
		if(mSocDivSaved.cfg ==0) mSocDivSaved.cfg = divider;
		HW_show_speed((" cfgClk    frequency %d\n", sysPll / divider));
#if !defined(BERLIN_SINGLE_CPU)
#ifdef __ECOS__
        _CYGNUM_HAL_RTC_PERIOD_value = (sysPll * 1000 * 1000 / divider) / MV_OS_TICKNUM_PER_SECOND - 1;	// it is used in galois_misc.c
#endif
#endif
		
		// perifCLK
		D3Switch = (r84 >> LSb32Gbl_ClkSwitch_perifClkD3Switch) & 1;
		Switch = (r84 >> LSb32Gbl_ClkSwitch_perifClkSwitch) & 1;
		Select = (r7c >> LSb32Gbl_clkSelect_perifClkSel) & ((1 << bGbl_clkSelect_perifClkSel) - 1);
		divider = get_divider(D3Switch, Switch, Select);
                mSocFreqVector.perif = sysPll / divider;
		if(mSocDivSaved.perif ==0) mSocDivSaved.perif = divider;
		HW_show_speed((" perifClk  frequency %d\n", sysPll / divider));
		
		// gfxCLK
		D3Switch = (r84 >> LSb32Gbl_ClkSwitch_gfxClkD3Switch) & 1;
		Switch = (r84 >> LSb32Gbl_ClkSwitch_gfxClkSwitch) & 1;
		Select = (r7c >> LSb32Gbl_clkSelect_gfxClkSel) & ((1 << bGbl_clkSelect_gfxClkSel) - 1);
		divider = get_divider(D3Switch, Switch, Select);
                mSocFreqVector.gfx = sysPll / divider;
		if(mSocDivSaved.gfx ==0) mSocDivSaved.gfx = divider;
		HW_show_speed((" gfxClk    frequency %d\n", sysPll / divider));
		
		// drmCLK
		D3Switch = (r84 >> LSb32Gbl_ClkSwitch_drmFigoClkD3Switch) & 1;
		Switch = (r84 >> LSb32Gbl_ClkSwitch_drmFigoClkSwitch) & 1;
		Select = (r7c >> LSb32Gbl_clkSelect_drmFigoClkSel) & ((1 << bGbl_clkSelect_drmFigoClkSel) - 1);
		divider = get_divider(D3Switch, Switch, Select);
        mSocFreqVector.drm = sysPll / divider;
		if(mSocDivSaved.drm ==0) mSocDivSaved.drm = divider;
		HW_show_speed((" drmClk    frequency %d\n", sysPll / divider));
		
		// nfcEccCLK
		D3Switch = (r84 >> LSb32Gbl_ClkSwitch_nfcEccClkD3Switch) & 1;
		Switch = (r84 >> LSb32Gbl_ClkSwitch_nfcEccClkSwitch) & 1;
		Select = (r7c >> LSb32Gbl_clkSelect_nfcEccClkSel) & ((1 << bGbl_clkSelect_nfcEccClkSel) - 1);
		divider = get_divider(D3Switch, Switch, Select);
                mSocFreqVector.nfcecc = sysPll / divider;
		if(mSocDivSaved.nfcecc ==0) mSocDivSaved.nfcecc = divider;
		HW_show_speed((" nfcEccClk frequency %d\n", sysPll / divider));
		
	}
}
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)
void show_speed(void)
{
	unsigned int FbDiv, RfDiv, VcoDivSel;
	unsigned int memPll;
	unsigned int sysPll;
	unsigned int cpu1Pll;
	unsigned int divider;
	unsigned int D3Switch, Switch, Select;
	int nfccswitch;
	T32Gbl_ClkSwitch	ClkSwitch;
	T32Gbl_ClkSwitch1	ClkSwitch1;
	T32Gbl_clkSelect	clkSelect;
	T32Gbl_clkSelect1	clkSelect1;
	T32Gbl_clkSelect2	clkSelect2;
	unsigned int PllSel;
	unsigned int avpll_b6_clock ; 
	HW_show_speed(("\n -------- Clock setting --------\n"));
	get_avpll_B6_clock(&avpll_b6_clock) ;
	{
		// memPll
		T32Gbl_memPllCtl	memPllCtl;
		T32Gbl_memPllCtl1	memPllCtl1;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_memPllCtl), &memPllCtl.u32);
		FbDiv = memPllCtl.umemPllCtl_memPllFbDiv;
		RfDiv = memPllCtl.umemPllCtl_memPllRfDiv;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_memPllCtl1), &memPllCtl1.u32);
		VcoDivSel = memPllCtl1.umemPllCtl_memPllVcoDivSel;
		memPll = (FbDiv + 2) * 25 / (RfDiv + 2) / (1 << VcoDivSel);
		mSocFreqVector.ddr = memPll/2;
		if(mSocDivSaved.ddr ==0) mSocDivSaved.ddr = 2;
		HW_show_speed((" memPLL       frequency %d\n", memPll));
	}
	{
		// sysPll
		T32Gbl_sysPllCtl	sysPllCtl;
		T32Gbl_sysPllCtl1	sysPllCtl1;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl), &sysPllCtl.u32);
		FbDiv = sysPllCtl.usysPllCtl_sysPllFbDiv;
		RfDiv = sysPllCtl.usysPllCtl_sysPllRfDiv;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl1), &sysPllCtl1.u32);
		VcoDivSel = sysPllCtl1.usysPllCtl_sysPllVcoDivSel;
		sysPll = (FbDiv + 2) * 25 / (RfDiv + 2) / (1 << VcoDivSel);
		mSocFreqVector.syspll = sysPll;
		if (mSocDivSaved.syspll ==0) mSocDivSaved.syspll = 1;
		HW_show_speed((" sysPLL       frequency %d\n", mSocFreqVector.syspll));
	}
	{
		// cpu1Pll
		T32Gbl_cpu1PllCtl	cpu1PllCtl;
		T32Gbl_cpu1PllCtl1	cpu1PllCtl1;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpu1PllCtl), &cpu1PllCtl.u32);
		FbDiv = cpu1PllCtl.ucpu1PllCtl_cpu1PllFbDiv;
		RfDiv = cpu1PllCtl.ucpu1PllCtl_cpu1PllRfDiv;
		if (RfDiv & 0xF)	// (M=REFDIV[3:0]+2, when REFDIV[3:0] not zero.)
			RfDiv = (RfDiv & 0xF) + 2;
		else				// (M=REFDIV[4]+1, when REFDIV[3:0] = zero.)
			RfDiv = (RfDiv >> 4) + 1;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpu1PllCtl1), &cpu1PllCtl1.u32);
		if (cpu1PllCtl1.ucpu1PllCtl_cpu1PllDiffClkEn)		// Differential clock output
			VcoDivSel = cpu1PllCtl1.ucpu1PllCtl_cpu1PllVcoDivSelDiff;
		else												// Single-ended clock output
			VcoDivSel = cpu1PllCtl1.ucpu1PllCtl_cpu1PllVcoDivSelSe;
		cpu1Pll = (FbDiv) * 25 / RfDiv;
		HW_show_speed((" cpu1PLL	frequency %d\n", cpu1Pll));
	}
	// DDR clk
	HW_show_speed((" ddrClk		frequency %d\n", mSocFreqVector.ddr));

	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch1.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &clkSelect2.u32);


	// CPU1
	D3Switch = ClkSwitch.uClkSwitch_cpu1ClkD3Switch;
	Switch = ClkSwitch.uClkSwitch_cpu1ClkSwitch;
	Select = clkSelect.uclkSelect_cpu1ClkSel;
	divider = get_divider(D3Switch, Switch, Select);
	PllSel = clkSelect.uclkSelect_cpu1ClkPllSel;
	if(mSocDivSaved.cpu1 ==0) mSocDivSaved.cpu1 = divider;
	if (ClkSwitch.uClkSwitch_cpu1ClkPllSwitch) {		// AVPLL
		mSocFreqVector.cpu1 = avpll_b6_clock/divider ;
		HW_show_speed((" cpu1Clk 	frequency %d (use %s)\n", mSocFreqVector.cpu1, AVPLL[PllSel] ));
	} else {
		mSocFreqVector.cpu1 = cpu1Pll/divider;
		HW_show_speed((" cpu1Clk	frequency %d (use cpu1pll)\n", mSocFreqVector.cpu1));
	}
	
	// CPU0
	D3Switch = ClkSwitch.uClkSwitch_cpu0ClkD3Switch;
	Switch = ClkSwitch.uClkSwitch_cpu0ClkSwitch;
	Select = clkSelect.uclkSelect_cpu0ClkSel;
	divider = get_divider(D3Switch, Switch, Select);
	PllSel = clkSelect.uclkSelect_cpu0ClkPllSel;
	if( mSocDivSaved.cpu0 ==0) mSocDivSaved.cpu0 = divider;
	if (ClkSwitch.uClkSwitch_cpu0ClkPllSwitch) {		// AVPLL or cpu1Pll
		if (PllSel == 4) {
			mSocFreqVector.cpu0 = cpu1Pll/divider;
			HW_show_speed((" cpu0Clk	frequency %d (use cpu1Pll)\n", mSocFreqVector.cpu0));
		} else if (clkSelect.uclkSelect_cpu0ClkPllSel < 4) {
			mSocFreqVector.cpu0 = avpll_b6_clock/divider ; 
			HW_show_speed((" cpu0Clk	frequency %d (use %s)\n", mSocFreqVector.cpu0, AVPLL[PllSel]));
		} else {
			HW_show_speed((" cpu0Clk	invalid PllSel %d\n", PllSel));
		}
	} else {	// sysPll
		mSocFreqVector.cpu0 = sysPll/divider;
		HW_show_speed((" cpu0Clk	frequency %d (use sysPll)\n", mSocFreqVector.cpu0));
	}

	{
			// sysClk
		D3Switch = ClkSwitch.uClkSwitch_sysClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_sysClkSwitch;
		Select = clkSelect.uclkSelect_sysClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_sysClkPllSel;
		if(mSocDivSaved.sys == 0) mSocDivSaved.sys = divider;
		if (ClkSwitch.uClkSwitch_sysClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.sys = cpu1Pll/divider;
				HW_show_speed((" sysClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.sys));
			} else if (PllSel < 4) {
				mSocFreqVector.sys = avpll_b6_clock/divider ;
				HW_show_speed((" sysClk		frequency %d (use  %s)\n", mSocFreqVector.sys, AVPLL[PllSel]));
			} else {
				HW_show_speed((" sysClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.sys = sysPll/divider;
			HW_show_speed((" sysClk		frequency %d (use sysPll)\n", mSocFreqVector.sys));
		}

		// drmFigoClk
		D3Switch = ClkSwitch.uClkSwitch_drmFigoClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_drmFigoClkSwitch;
		Select = clkSelect.uclkSelect_drmFigoClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_drmFigoClkPllSel;
		if(mSocDivSaved.drm ==0) mSocDivSaved.drm = divider;
		if (ClkSwitch.uClkSwitch_drmFigoClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.drm = cpu1Pll/divider;
				HW_show_speed((" drmClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.drm));
			} else if (PllSel < 4) {
				mSocFreqVector.drm = avpll_b6_clock/divider ;
				HW_show_speed((" drmClk		frequency %d (use %s)\n", mSocFreqVector.drm, AVPLL[PllSel]));
				mSocFreqVector.drm = avpll_b6_clock/divider ;
			} else {
				HW_show_speed((" drmClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.drm = sysPll/divider;
			HW_show_speed((" drmClk		frequency %d (use sysPll)\n", mSocFreqVector.drm));
		}

			// cfgClk
		D3Switch = ClkSwitch.uClkSwitch_cfgClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_cfgClkSwitch;
		Select = clkSelect.uclkSelect_cfgClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_cfgClkPllSel;
		if(mSocDivSaved.cfg ==0) mSocDivSaved.cfg = divider;
		if (ClkSwitch.uClkSwitch_cfgClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.cfg = cpu1Pll/divider;
				HW_show_speed((" cfgClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.cfg));
#if !defined(BERLIN_SINGLE_CPU)
#ifdef __ECOS__
#if	(CPUINDEX == 1)
       			_CYGNUM_HAL_RTC_PERIOD_value = (cpu1Pll * 1000 * 1000 / divider) / MV_OS_TICKNUM_PER_SECOND - 1;	// it is used in galois_misc.c
#endif
#endif
#endif
			} else if (PllSel < 4) {
				mSocFreqVector.cfg = avpll_b6_clock/divider ;
				HW_show_speed((" cfgClk		frequency %d (use %s)\n", mSocFreqVector.cfg, AVPLL[PllSel]));
#if !defined(BERLIN_SINGLE_CPU)
#ifdef __ECOS__
#if	(CPUINDEX == 1)
       			_CYGNUM_HAL_RTC_PERIOD_value = (avpll_b6_clock * 1000 * 1000 / divider) / MV_OS_TICKNUM_PER_SECOND - 1;	// it is used in galois_misc.c
#endif
#endif
#endif
			} else {
				HW_show_speed((" cfgClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.cfg = sysPll/divider;
			HW_show_speed((" cfgClk		frequency %d (use sysPll)\n", mSocFreqVector.cfg));
#if !defined(BERLIN_SINGLE_CPU)
#ifdef __ECOS__
#if	(CPUINDEX == 1)
        	_CYGNUM_HAL_RTC_PERIOD_value = (sysPll * 1000 * 1000 / divider) / MV_OS_TICKNUM_PER_SECOND - 1;	// it is used in galois_misc.c
#endif
#endif
#endif
		}

		// gfxClk
		D3Switch = ClkSwitch.uClkSwitch_gfxClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_gfxClkSwitch;
		Select = clkSelect1.uclkSelect_gfxClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_gfxClkPllSel;
		if(mSocDivSaved.gfx ==0) mSocDivSaved.gfx = divider;
		if (ClkSwitch.uClkSwitch_gfxClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.gfx = cpu1Pll/divider;
				HW_show_speed((" gfxClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.gfx));
			} else if (PllSel < 4) {
				mSocFreqVector.gfx = avpll_b6_clock/divider ;
				HW_show_speed((" gfxClk		frequency %d (use %s)\n", mSocFreqVector.gfx, AVPLL[PllSel]));
			} else {
				HW_show_speed((" gfxClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.gfx = sysPll/divider;
			HW_show_speed((" gfxClk		frequency %d (use sysPll)\n", mSocFreqVector.gfx));
		}
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
        // zspClk
		D3Switch = ClkSwitch.uClkSwitch_zspClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_zspClkSwitch;
		Select = clkSelect1.uclkSelect_zspClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_zspClkPllSel;
		if(mSocDivSaved.adsp ==0) mSocDivSaved.adsp = divider;
		if (ClkSwitch.uClkSwitch_zspClkPllSwitch) {       // AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.adsp = cpu1Pll/divider;
				HW_show_speed((" DSPClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.adsp));
			} else if (PllSel < 4) {
				mSocFreqVector.adsp = avpll_b6_clock/divider ;
				HW_show_speed((" DSPClk		frequency %d (use %s)\n", mSocFreqVector.adsp, AVPLL[PllSel]));
				mSocFreqVector.adsp = avpll_b6_clock/divider ;
			} else {
				HW_show_speed((" DSPClk		invalid PllSel %d\n", PllSel));
			}
		} else {    // sysPll
			mSocFreqVector.adsp = sysPll/divider;
			HW_show_speed((" DSPClk		frequency %d (use sysPll)\n", mSocFreqVector.adsp));
		}
#endif

		// perifClk
		D3Switch = ClkSwitch.uClkSwitch_perifClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_perifClkSwitch;
		Select = clkSelect1.uclkSelect_perifClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_perifClkPllSel;
		if(mSocDivSaved.perif ==0) mSocDivSaved.perif = divider;
		if (ClkSwitch.uClkSwitch_perifClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.perif = cpu1Pll/divider;
				HW_show_speed((" perifClk	frequency %d (use cpu1Pll)\n", mSocFreqVector.perif));
			} else if (PllSel < 4) {
				mSocFreqVector.perif = avpll_b6_clock/divider ;
				HW_show_speed((" perifClk	frequency %d (use %s)\n", mSocFreqVector.perif, AVPLL[PllSel]));
			} else {
				HW_show_speed((" perifClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.perif = sysPll/divider;
			HW_show_speed((" perifClk	frequency %d (use sysPll)\n", mSocFreqVector.perif));
		}

			// pCubeClk
		D3Switch = ClkSwitch.uClkSwitch_pCubeClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_pCubeClkSwitch;
		Select = clkSelect1.uclkSelect_pCubeClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_pCubeClkPllSel;
		if(mSocDivSaved.pcube ==0) mSocDivSaved.pcube = divider;
		if (ClkSwitch.uClkSwitch_pCubeClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.pcube = cpu1Pll/divider;
				HW_show_speed((" pCubeClk	frequency %d (use cpu1Pll)\n", mSocFreqVector.pcube));
			} else if (PllSel < 4) {
				mSocFreqVector.pcube = avpll_b6_clock ;
				HW_show_speed((" pCubeClk	frequency %d (use %s)\n", mSocFreqVector.pcube, AVPLL[PllSel]));
			} else {
				HW_show_speed((" pCubeClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.pcube = sysPll/divider;
			HW_show_speed((" pCubeClk	frequency %d (use sysPll)\n", mSocFreqVector.pcube));
		}

#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		D3Switch = ClkSwitch1.uClkSwitch_vScopeClkD3Switch;
#else
		D3Switch = ClkSwitch.uClkSwitch_vScopeClkD3Switch;
#endif
		// vScopeClk
		Switch = ClkSwitch.uClkSwitch_vScopeClkSwitch;
		Select = clkSelect1.uclkSelect_vScopeClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_vScopeClkPllSel;
		if(mSocDivSaved.vscope ==0) mSocDivSaved.vscope = divider;
		if (ClkSwitch.uClkSwitch_vScopeClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.vscope = cpu1Pll/divider;
				HW_show_speed((" vScopeClk	frequency %d (use cpu1Pll)\n", mSocFreqVector.vscope));
			} else if (PllSel < 4) {
				mSocFreqVector.vscope = avpll_b6_clock/divider ;
				HW_show_speed((" vScopeClk	frequency %d (use %s)\n",mSocFreqVector.vscope, AVPLL[PllSel]));

			} else {
				HW_show_speed((" vScopeClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.vscope = sysPll/divider;
			HW_show_speed((" vScopeClk	frequency %d (use sysPll)\n", mSocFreqVector.vscope));
		}

			// nfcEccClk
		D3Switch = ClkSwitch1.uClkSwitch_nfcEccClkD3Switch;
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		Switch = ClkSwitch1.uClkSwitch_nfcEccClkSwitch;
		Select = clkSelect2.uclkSelect_nfcEccClkSel;
#else
		Switch = ClkSwitch.uClkSwitch_nfcEccClkSwitch;
		Select = clkSelect1.uclkSelect_nfcEccClkSel;
#endif
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_nfcEccClkPllSel;
		if(mSocDivSaved.nfcecc ==0) mSocDivSaved.nfcecc = divider;
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		nfccswitch = ClkSwitch1.uClkSwitch_nfcEccClkPllSwitch;
#else
		nfccswitch = ClkSwitch.uClkSwitch_nfcEccClkPllSwitch;
#endif
		if (nfccswitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.nfcecc = cpu1Pll/divider;
				HW_show_speed((" nfcEccClk	frequency %d (use cpu1Pll)\n", mSocFreqVector.nfcecc));
			} else if (PllSel < 4) {
				mSocFreqVector.nfcecc = avpll_b6_clock/divider ;
				HW_show_speed((" nfcEccClk	frequency %d (use %s)\n", mSocFreqVector.nfcecc, AVPLL[PllSel]));

			} else {
				HW_show_speed((" nfcEccClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.nfcecc = sysPll/divider;
			HW_show_speed((" nfcEccClk	frequency %d (use sysPll)\n", mSocFreqVector.nfcecc));
		}

		// vppSysClk
		D3Switch = ClkSwitch1.uClkSwitch_vppSysClkD3Switch;
		Switch = ClkSwitch1.uClkSwitch_vppSysClkSwitch;
		Select = clkSelect2.uclkSelect_vppSysClkSel;
		divider = get_divider(D3Switch, Switch, Select);
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		PllSel = clkSelect2.uclkSelect_vppSysClkPllSel;
#else
		PllSel = clkSelect1.uclkSelect_vppSysClkPllSel;
#endif
		if(mSocDivSaved.vpp ==0) mSocDivSaved.vpp = divider;
		if (ClkSwitch1.uClkSwitch_vppSysClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.vpp = cpu1Pll/divider;
				HW_show_speed((" vppSysClk	frequency %d (use cpu1Pll)\n", mSocFreqVector.vpp));
			} else if (PllSel < 4) {
				mSocFreqVector.vpp = avpll_b6_clock/divider ;
				HW_show_speed((" vppSysClk	frequency %d (use %s)\n", mSocFreqVector.vpp, AVPLL[PllSel]));
			} else {
				HW_show_speed((" vppSysClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.vpp = sysPll/divider;
			HW_show_speed((" vppSysClk	frequency %d (use sysPll)\n", mSocFreqVector.vpp));
		}

		// appClk
		D3Switch = ClkSwitch1.uClkSwitch_appClkD3Switch;
		Switch = ClkSwitch1.uClkSwitch_appClkSwitch;
		Select = clkSelect2.uclkSelect_appClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect2.uclkSelect_appClkPllSel;
		if(mSocDivSaved.app ==0) mSocDivSaved.app = divider;
		if (ClkSwitch1.uClkSwitch_appClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.app = cpu1Pll/divider;
				HW_show_speed((" appClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.app));
			} else if (PllSel < 4) {
				mSocFreqVector.app = avpll_b6_clock/divider ;
				HW_show_speed((" appClk		frequency %d (use %s)\n", mSocFreqVector.app, AVPLL[PllSel]));

			} else {
				HW_show_speed(( " appClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.app = sysPll/divider;
			HW_show_speed(( " appClk		frequency %d (use sysPll)\n", mSocFreqVector.app));
		}
	}
}

int GaloisGetAllFreq(SOC_FREQ_VECTOR * freq){
	memcpy(freq, &mSocFreqVector, sizeof(mSocFreqVector)) ;
	return 0 ;
}

static void GetSettingFromDivider(unsigned int divider, unsigned int *D3Switch, unsigned int *Switch, unsigned int *Select)
{
	if(divider ==1)
	{
		*D3Switch = 0; 	// No use D3
		*Switch = 0;	// No Use divider
		*Select = 1;	// default 2
	}
	else if(divider ==2)
	{
		*D3Switch = 0; 	// No use D3
		*Switch = 1;	// Use divider
		*Select = 1;	// default 2
	}
	else if(divider ==3)
	{
		*D3Switch = 1; 	// Use D3
		*Switch = 1;	// Use divider
		*Select = 1;	// default 2
	} 
	else 
	{
		*D3Switch = 0; 	// No use D3
		*Switch = 1;	// Use divider
		if(divider >= 12 ) 
			*Select = 5; // div 12
		else 
			*Select  = divider/2;
	}
}


static int in_clock_changing =0; // for multi task protection.
UNSG32 Dynamic_Lower_vMeta_Speed(void)
{
#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)

#ifdef DYNAMIC_POWER_CONTROL
	unsigned int divider;
	unsigned int D3Switch, Switch, Select;
	unsigned int PllSel;
	T32Gbl_ClkSwitch	ClkSwitch;
	T32Gbl_ClkSwitch1	ClkSwitch1;
	T32Gbl_clkSelect1	clkSelect1;
	if(mSocOrigDivSaved.pcube == 0)
	{
		if(in_clock_changing) return -1;
		in_clock_changing = 1;
		mSocOrigDivSaved.vscope = mSocDivSaved.vscope;
	  	mSocOrigDivSaved.pcube = mSocDivSaved.pcube;
	  	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch1.u32);
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
		// pCubeClk
		if (ClkSwitch.uClkSwitch_pCubeClkPllSwitch && clkSelect1.uclkSelect_pCubeClkPllSel!= 4) 
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable
		}
		else
		{
			ClkSwitch.uClkSwitch_pCubeClkD3Switch = 0; 	// No use D3
			ClkSwitch.uClkSwitch_pCubeClkSwitch = 1;	// Use divider
			if(mSocDivSaved.pcube < 8)
			{
				clkSelect1.uclkSelect_pCubeClkSel = 4;  	// 4: 1/8; 5:1/12
				mSocDivSaved.pcube = 8;
			} else
			{
				clkSelect1.uclkSelect_pCubeClkSel = 5;  	// 5:1/12
				mSocDivSaved.pcube = 12;
			}
			mSocFreqVector.pcube = mSocFreqVector.pcube*mSocOrigDivSaved.pcube/mSocDivSaved.pcube;
		}
		
		// vScope clk
		if (ClkSwitch.uClkSwitch_vScopeClkPllSwitch && clkSelect1.uclkSelect_vScopeClkPllSel != 4) 
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable
		}
		else
		{
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
			ClkSwitch1.uClkSwitch_vScopeClkD3Switch =0;	// Not D3
#else
			ClkSwitch.uClkSwitch_vScopeClkD3Switch =0;	 //Not D3
#endif
			ClkSwitch.uClkSwitch_vScopeClkSwitch = 1;	// Use divider
			if(mSocDivSaved.vscope < 8)
			{
				clkSelect1.uclkSelect_vScopeClkSel = 4;  	// 4: 1/8; 5:1/12
				mSocDivSaved.vscope = 8;
			} else
			{
				clkSelect1.uclkSelect_vScopeClkSel = 5;  	// 5:1/12
				mSocDivSaved.vscope = 12;
			}
			mSocFreqVector.vscope = mSocFreqVector.vscope*mSocOrigDivSaved.vscope/mSocDivSaved.vscope;
		}
#if 0
printf("Low vMeta slect/D3/swith(%x-%x-%x) (%x-%x-%x) \n",
 clkSelect1.uclkSelect_vScopeClkSel, ClkSwitch1.uClkSwitch_vScopeClkD3Switch, ClkSwitch.uClkSwitch_vScopeClkSwitch,
 clkSelect1.uclkSelect_pCubeClkSel, ClkSwitch.uClkSwitch_pCubeClkD3Switch, ClkSwitch.uClkSwitch_pCubeClkSwitch);
#endif
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1),(clkSelect1.u32));
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1),(ClkSwitch1.u32));
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch),(ClkSwitch.u32));
		in_clock_changing = 0;
	}
#endif
	return 0;
#endif
}
UNSG32 Dynamic_Recover_vMeta_Speed(void)
{
#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)

#ifdef DYNAMIC_POWER_CONTROL
	unsigned int divider;
	unsigned int D3Switch, Switch, Select;
	unsigned int PllSel;
	T32Gbl_ClkSwitch	ClkSwitch;
	T32Gbl_ClkSwitch1	ClkSwitch1;
	T32Gbl_clkSelect1	clkSelect1;
	if(mSocOrigDivSaved.pcube)
	{
		if(in_clock_changing) return -1;
		in_clock_changing = 1;
	  	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch1.u32);
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
		// pCubeClk
		if (ClkSwitch.uClkSwitch_pCubeClkPllSwitch && clkSelect1.uclkSelect_pCubeClkPllSel!= 4)
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable	
		}
		else
		{
			GetSettingFromDivider(mSocOrigDivSaved.pcube, &D3Switch, &Switch, &Select);
			ClkSwitch.uClkSwitch_pCubeClkD3Switch = D3Switch; 	// D3
			ClkSwitch.uClkSwitch_pCubeClkSwitch = Switch;	//  divider
			clkSelect1.uclkSelect_pCubeClkSel = Select;		// default 2
			mSocFreqVector.pcube = mSocFreqVector.pcube*mSocDivSaved.pcube/mSocOrigDivSaved.pcube;
		}

		// vScope clk
		if (ClkSwitch.uClkSwitch_vScopeClkPllSwitch && clkSelect1.uclkSelect_vScopeClkPllSel != 4) 
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable
		}
		else
		{
			GetSettingFromDivider(mSocOrigDivSaved.vscope, &D3Switch, &Switch, &Select);
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
			ClkSwitch1.uClkSwitch_vScopeClkD3Switch = D3Switch;	// Not D3
#else
			ClkSwitch.uClkSwitch_vScopeClkD3Switch = D3Switch;	 //Not D3
#endif
			ClkSwitch.uClkSwitch_vScopeClkSwitch = Switch;	// Use divider
			clkSelect1.uclkSelect_vScopeClkSel = Select;  	// Select
			mSocFreqVector.vscope = mSocFreqVector.vscope*mSocDivSaved.vscope/mSocOrigDivSaved.vscope;
		}
#if 0
printf("Recover vMeta slect/D3/swith(%x-%x-%x) (%x-%x-%x) \n",
 clkSelect1.uclkSelect_vScopeClkSel, ClkSwitch1.uClkSwitch_vScopeClkD3Switch, ClkSwitch.uClkSwitch_vScopeClkSwitch,
 clkSelect1.uclkSelect_pCubeClkSel, ClkSwitch.uClkSwitch_pCubeClkD3Switch, ClkSwitch.uClkSwitch_pCubeClkSwitch);
#endif
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1),(clkSelect1.u32));
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1),(ClkSwitch1.u32));
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch),(ClkSwitch.u32));			
		mSocDivSaved.vscope = mSocOrigDivSaved.vscope;
	  	mSocDivSaved.pcube = mSocOrigDivSaved.pcube;
		mSocOrigDivSaved.vscope = 0;
	  	mSocOrigDivSaved.pcube = 0;
		in_clock_changing = 0;
	}
#endif
	return 0;
#endif
}

UNSG32  Dynamic_Lower_ZSP_Speed(void)
{
#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)

#ifdef DYNAMIC_POWER_CONTROL
	T32Gbl_ClkSwitch	ClkSwitch;
	T32Gbl_clkSelect1	clkSelect1;
	if(mSocOrigDivSaved.adsp == 0)
	{
		if(in_clock_changing) return 1;
		in_clock_changing = 1;
		mSocOrigDivSaved.adsp = mSocDivSaved.adsp;
	  	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
		if (ClkSwitch.uClkSwitch_zspClkPllSwitch && clkSelect1.uclkSelect_zspClkPllSel!= 4) 
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable
			
		}
		else
		{
			ClkSwitch.uClkSwitch_zspClkD3Switch = 0; 	// No use D3
			ClkSwitch.uClkSwitch_zspClkSwitch = 1;	// Use divider
			if(mSocDivSaved.adsp < 8)
			{
				clkSelect1.uclkSelect_zspClkSel = 4;  	// 4: 1/8; 5:1/12
				mSocDivSaved.adsp = 8;
			} else
			{
				clkSelect1.uclkSelect_zspClkSel = 5;  	// 5:1/12
				mSocDivSaved.adsp = 12;
			}
			mSocFreqVector.adsp = mSocFreqVector.adsp*mSocOrigDivSaved.adsp/mSocDivSaved.adsp;
		}
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1),(clkSelect1.u32));
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch),(ClkSwitch.u32));
		in_clock_changing = 0;
	}
#endif
	return 0;
#endif
#endif	
}
UNSG32 Dynamic_Recover_ZSP_Speed(void)
{
#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)

#ifdef DYNAMIC_POWER_CONTROL
	unsigned int divider;
	unsigned int D3Switch, Switch, Select;
	unsigned int PllSel;
	T32Gbl_ClkSwitch	ClkSwitch;
	T32Gbl_ClkSwitch1	ClkSwitch1;
	T32Gbl_clkSelect1	clkSelect1;
	if(mSocOrigDivSaved.adsp)
	{
		if(in_clock_changing) return -1;
		in_clock_changing = 1;
	  	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
		if (ClkSwitch.uClkSwitch_zspClkPllSwitch && clkSelect1.uclkSelect_zspClkPllSel!= 4)
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable			
		}
		else
		{
			GetSettingFromDivider(mSocOrigDivSaved.adsp, &D3Switch, &Switch, &Select);
			ClkSwitch.uClkSwitch_zspClkD3Switch = D3Switch; 	// D3
			ClkSwitch.uClkSwitch_zspClkSwitch = Switch;	//  divider
			clkSelect1.uclkSelect_zspClkSel = Select;		// default 2
			mSocFreqVector.adsp = mSocFreqVector.adsp*mSocDivSaved.adsp/mSocOrigDivSaved.adsp;
		}
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1),(clkSelect1.u32));
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch),(ClkSwitch.u32));
		mSocDivSaved.adsp = mSocOrigDivSaved.adsp;
		mSocOrigDivSaved.adsp = 0;
		in_clock_changing = 0;
	}
#endif
	return 0;
#endif
#endif

}

UNSG32 GaloisGetCPUDivider(unsigned int * cpu0_clock_divider,
						   unsigned int * cpu1_clock_divider)
{
#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)

	*cpu0_clock_divider = mSocDivSaved.cpu0 ; 
	*cpu1_clock_divider = mSocDivSaved.cpu1 ;
	return 0;
#endif
#endif

}

#else

void show_speed(void)
{
    return;
}


UNSG32 Dynamic_Lower_vMeta_Speed(void)
{
	    return (0);
}
UNSG32 Dynamic_Recover_vMeta_Speed(void)
{
	    return (0);
}
UNSG32  Dynamic_Lower_ZSP_Speed(void)
{
	    return (0);
}
UNSG32 Dynamic_Recover_ZSP_Speed(void)
{
	    return (0);
}

#endif

UNSG32 GaloisGetCfgFrequency(void)
{
    return (mSocFreqVector.cfg);
}

UNSG32 GaloisGetFrequency(int FreqID)
{
    switch (FreqID) {
        case SOC_FREQ_DDR:
             return (mSocFreqVector.ddr);
        case SOC_FREQ_CPU0:
             return (mSocFreqVector.cpu0);
        case SOC_FREQ_CPU1:
             return (mSocFreqVector.cpu1);
        case SOC_FREQ_SYSPLL:
             return (mSocFreqVector.syspll);
        case SOC_FREQ_SYS:
             return (mSocFreqVector.sys);
        case SOC_FREQ_VSCOPE:
             return (mSocFreqVector.vscope);
        case SOC_FREQ_PCUBE:
             return (mSocFreqVector.pcube);
        case SOC_FREQ_VPP:
             return (mSocFreqVector.vpp);
        case SOC_FREQ_CFG:
             return (mSocFreqVector.cfg);
        case SOC_FREQ_PERIF:
             return (mSocFreqVector.perif);
        case SOC_FREQ_GFX:
             return (mSocFreqVector.gfx);
        case SOC_FREQ_DRM:
             return (mSocFreqVector.drm);
        case SOC_FREQ_NFCECC:
             return (mSocFreqVector.nfcecc);
        default:
             return (0);
    }
}


#else
void show_speed(void)
{
}
UNSG32 GaloisGetFrequency(int FreqID)
{
     return (0);
}
#endif


