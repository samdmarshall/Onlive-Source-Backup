#include <linux/module.h>
#include "Galois_memmap.h"
#include "global.h"
#include "galois_io.h"
#include "mach/freq.h"
#include "galois_speed.h"

#define BFM_HOST_Bus_Read32(offset, holder)	((*(unsigned int*)(holder))=(*(unsigned int*)(offset)))

//#define	HW_show_speed(a)	galois_printk a

unsigned int init_cpu0_cpu1_clock_divider[2] ;


void __init query_board_freq(SOC_FREQ_VECTOR_Linux * freq){
	int i ;
	unsigned int * temp ;
	show_speed() ; 
	GaloisGetAllFreq((SOC_FREQ_VECTOR *)freq) ; 
	//	SOC_FREQ_VECTOR_Linux = SOC_FREQ_VECTOR * 1000000
	temp = (unsigned int *)freq ;
	for(i=0; i<(sizeof(SOC_FREQ_VECTOR_Linux)/4); i++){
		temp[i] *= 1000000 ;
	}
	GaloisGetCPUDivider(&(init_cpu0_cpu1_clock_divider[0]) , &(init_cpu0_cpu1_clock_divider[1])) ; 
}

#if 0
static const unsigned int clock_divider[] =
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
static unsigned int get_divider(unsigned int D3Switch, unsigned int Switch, unsigned int Select)
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

#ifdef CONFIG_MV88DE3010_BERLIN_B0
static float VcoDiv[] = 
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
static const char *AVPLL[] =
{
	"AVPllA5",
	"AVPllA6",
	"AVPllB6",
	"AVPllB7",
	"CPU1Pll"
};

#define MAKE_DEFAULT_CLK_600

void __init query_board_freq(SOC_FREQ_VECTOR_Linux * freq)
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
	HW_show_speed(("\n -------- Clock setting --------\n"));
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
		freq->ddr = memPll*1000000/2;
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
		freq->syspll = sysPll*1000000;
		HW_show_speed((" sysPLL       frequency %d\n", freq->syspll));
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
#if 1
		HW_show_speed((" cpu1PLL	frequency %d\n", cpu1Pll*1000000));
#endif
	}
	// DDR clk
	HW_show_speed((" ddrClk		frequency %d\n", freq->ddr));

	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch1.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &clkSelect2.u32);

#if 1
	// CPU1
	D3Switch = ClkSwitch.uClkSwitch_cpu1ClkD3Switch;
	Switch = ClkSwitch.uClkSwitch_cpu1ClkSwitch;
	Select = clkSelect.uclkSelect_cpu1ClkSel;
	divider = get_divider(D3Switch, Switch, Select);
	PllSel = clkSelect.uclkSelect_cpu1ClkPllSel;
	freq->cpu1 = cpu1Pll*1000000/divider;
	if (ClkSwitch.uClkSwitch_cpu1ClkPllSwitch) {		// AVPLL
		HW_show_speed((" cpu1Clk	from %s\n", AVPLL[PllSel]));
	} else {
		HW_show_speed((" cpu1Clk	frequency %d\n",freq->cpu1));
	}
	init_cpu0_cpu1_clock_divider[1] = divider ;
#endif
	// CPU0
	D3Switch = ClkSwitch.uClkSwitch_cpu0ClkD3Switch;
	Switch = ClkSwitch.uClkSwitch_cpu0ClkSwitch;
	Select = clkSelect.uclkSelect_cpu0ClkSel;
	divider = get_divider(D3Switch, Switch, Select);
	PllSel = clkSelect.uclkSelect_cpu0ClkPllSel;
	if (ClkSwitch.uClkSwitch_cpu0ClkPllSwitch) {		// AVPLL or cpu1Pll
		if (PllSel == 4) {
			freq->cpu0 = cpu1Pll*1000000/divider;
			HW_show_speed((" cpu0Clk	frequency %d (use cpu1Pll)\n", freq->cpu0));
		} else if (clkSelect.uclkSelect_cpu0ClkPllSel < 4) {
			HW_show_speed((" cpu0Clk	from %s\n", AVPLL[PllSel]));
#ifdef MAKE_DEFAULT_CLK_600
			freq->cpu0=600000000;
#endif
		} else {
			HW_show_speed((" cpu0Clk	invalid PllSel %d\n", PllSel));
#ifdef MAKE_DEFAULT_CLK_600
			freq->cpu0=600000000;
#endif
		}
	} else {	// sysPll
		freq->cpu0 = sysPll*1000000/divider;
		HW_show_speed((" cpu0Clk	frequency %d (use sysPll)\n", freq->cpu0));
	}
	init_cpu0_cpu1_clock_divider[0] = divider ;

	{
			// sysClk
		D3Switch = ClkSwitch.uClkSwitch_sysClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_sysClkSwitch;
		Select = clkSelect.uclkSelect_sysClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_sysClkPllSel;
		if (ClkSwitch.uClkSwitch_sysClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				freq->sys = cpu1Pll*1000000/divider;
				HW_show_speed((" sysClk		frequency %d (use cpu1Pll)\n", freq->sys));
			} else if (PllSel < 4) {
				HW_show_speed((" sysClk		from %s\n", AVPLL[PllSel]));
			} else {
				HW_show_speed((" sysClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			freq->sys = sysPll*1000000/divider;
			HW_show_speed((" sysClk		frequency %d (use sysPll)\n", freq->sys));
		}

		// drmFigoClk
		D3Switch = ClkSwitch.uClkSwitch_drmFigoClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_drmFigoClkSwitch;
		Select = clkSelect.uclkSelect_drmFigoClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_drmFigoClkPllSel;
		if (ClkSwitch.uClkSwitch_drmFigoClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				freq->drm = cpu1Pll*1000000/divider;
				HW_show_speed((" drmClk		frequency %d (use cpu1Pll)\n", freq->drm));
			} else if (PllSel < 4) {
				HW_show_speed((" drmClk		from %s\n", AVPLL[PllSel]));
			} else {
				HW_show_speed((" drmClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			freq->drm = sysPll*1000000/divider;
			HW_show_speed((" drmClk		frequency %d (use sysPll)\n", freq->drm));
		}

			// cfgClk
		D3Switch = ClkSwitch.uClkSwitch_cfgClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_cfgClkSwitch;
		Select = clkSelect.uclkSelect_cfgClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_cfgClkPllSel;
		if (ClkSwitch.uClkSwitch_cfgClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				freq->cfg = cpu1Pll*1000000/divider;
				HW_show_speed((" cfgClk		frequency %d (use cpu1Pll)\n", freq->cfg));
			} else if (PllSel < 4) {
				HW_show_speed((" cfgClk		from %s\n", AVPLL[PllSel]));
//				panic("cfgClk can't be accessed directly\n");
#ifdef MAKE_DEFAULT_CLK_600
				freq->cfg = 75000000;
#endif
			} else {
				HW_show_speed((" cfgClk		invalid PllSel %d\n", PllSel));
#ifdef MAKE_DEFAULT_CLK_600
				freq->cfg = 75000000;
#endif
			}
		} else {	// sysPll
			freq->cfg = sysPll*1000000/divider;
			HW_show_speed((" cfgClk		frequency %d (use sysPll)\n", freq->cfg));
		}

		// gfxClk
		D3Switch = ClkSwitch.uClkSwitch_gfxClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_gfxClkSwitch;
		Select = clkSelect1.uclkSelect_gfxClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_gfxClkPllSel;
		if (ClkSwitch.uClkSwitch_gfxClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				freq->gfx = cpu1Pll*1000000/divider;
				HW_show_speed((" gfxClk		frequency %d (use cpu1Pll)\n", freq->gfx));
			} else if (PllSel < 4) {
				HW_show_speed((" gfxClk		from %s\n", AVPLL[PllSel]));
			} else {
				HW_show_speed((" gfxClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			freq->gfx = sysPll*1000000/divider;
			HW_show_speed((" gfxClk		frequency %d (use sysPll)\n", freq->gfx));
		}

		// perifClk
		D3Switch = ClkSwitch.uClkSwitch_perifClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_perifClkSwitch;
		Select = clkSelect1.uclkSelect_perifClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_perifClkPllSel;
		if (ClkSwitch.uClkSwitch_perifClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				freq->perif = cpu1Pll*1000000/divider;
				HW_show_speed((" perifClk	frequency %d (use cpu1Pll)\n", freq->perif));
			} else if (PllSel < 4) {
				HW_show_speed((" perifClk	from %s\n", AVPLL[PllSel]));
			} else {
				HW_show_speed((" perifClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			freq->perif = sysPll*1000000/divider;
			HW_show_speed((" perifClk	frequency %d (use sysPll)\n", freq->perif));
		}

			// pCubeClk
		D3Switch = ClkSwitch.uClkSwitch_pCubeClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_pCubeClkSwitch;
		Select = clkSelect1.uclkSelect_pCubeClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_pCubeClkPllSel;
		if (ClkSwitch.uClkSwitch_pCubeClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				freq->pcube = cpu1Pll*1000000/divider;
				HW_show_speed((" pCubeClk	frequency %d (use cpu1Pll)\n", freq->pcube));
			} else if (PllSel < 4) {
				HW_show_speed((" pCubeClk	from %s\n", AVPLL[PllSel]));
			} else {
				HW_show_speed((" pCubeClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			freq->pcube = sysPll*1000000/divider;
			HW_show_speed((" pCubeClk	frequency %d (use sysPll)\n", freq->pcube));
		}

		// vScopeClk
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		D3Switch = ClkSwitch1.uClkSwitch_vScopeClkD3Switch;
#else
		D3Switch = ClkSwitch.uClkSwitch_vScopeClkD3Switch;
#endif
		Switch = ClkSwitch.uClkSwitch_vScopeClkSwitch;
		Select = clkSelect1.uclkSelect_vScopeClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_vScopeClkPllSel;
		if (ClkSwitch.uClkSwitch_vScopeClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				freq->vscope = cpu1Pll*1000000/divider;
				HW_show_speed((" vScopeClk	frequency %d (use cpu1Pll)\n", freq->vscope));
			} else if (PllSel < 4) {
				HW_show_speed((" vScopeClk	from %s\n", AVPLL[PllSel]));
			} else {
				HW_show_speed((" vScopeClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			freq->vscope = sysPll*1000000/divider;
			HW_show_speed((" vScopeClk	frequency %d (use sysPll)\n", freq->vscope));
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
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		nfccswitch = ClkSwitch1.uClkSwitch_nfcEccClkPllSwitch;
#else
		nfccswitch = ClkSwitch.uClkSwitch_nfcEccClkPllSwitch;
#endif
		if (nfccswitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				freq->nfcecc = cpu1Pll*1000000/divider;
				HW_show_speed((" nfcEccClk	frequency %d (use cpu1Pll)\n", freq->nfcecc));
			} else if (PllSel < 4) {
				HW_show_speed((" nfcEccClk	from %s\n", AVPLL[PllSel]));
			} else {
				HW_show_speed((" nfcEccClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			freq->nfcecc = sysPll*1000000/divider;
			HW_show_speed((" nfcEccClk	frequency %d (use sysPll)\n", freq->nfcecc));
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
		if (ClkSwitch1.uClkSwitch_vppSysClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				freq->vpp = cpu1Pll*1000000/divider;
				HW_show_speed((" vppSysClk	frequency %d (use cpu1Pll)\n", freq->vpp));
			} else if (PllSel < 4) {
				HW_show_speed((" vppSysClk	from %s\n", AVPLL[PllSel]));
			} else {
				HW_show_speed((" vppSysClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			freq->vpp = sysPll*1000000/divider;
			HW_show_speed((" vppSysClk	frequency %d (use sysPll)\n", freq->vpp));
		}

		// appClk
		D3Switch = ClkSwitch1.uClkSwitch_appClkD3Switch;
		Switch = ClkSwitch1.uClkSwitch_appClkSwitch;
		Select = clkSelect2.uclkSelect_appClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect2.uclkSelect_appClkPllSel;
		if (ClkSwitch1.uClkSwitch_appClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				freq->app = cpu1Pll*1000000/divider;
				HW_show_speed((" appClk		frequency %d (use cpu1Pll)\n", freq->app));
			} else if (PllSel < 4) {
				HW_show_speed((" appClk		from %s\n", AVPLL[PllSel]));
			} else {
				HW_show_speed(( " appClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			freq->app = sysPll*1000000/divider;
			HW_show_speed(( " appClk		frequency %d (use sysPll)\n", freq->app));
		}
	}

	HW_show_speed((" test show speed function \n")) ;
	show_speed() ;
}
#endif

#endif

