
#include "Galois_memmap.h"
#include "galois_io.h"
#include "global.h"
#include "pinmux.h"
#if defined(BERLIN)
#include "SysMgr.h"
#include "apb_perf_base.h"
#endif
#include "pinmux_config.h"

#if !defined(BERLIN)
/*	pin mux group tables arrange as
 *	grpMask, grpLsb, grpModeBits
 */
#define NUM_PINMUX_GRPS			38
#define FirstGrpLastID			20
static UNSG32 pinmuxtab[NUM_PINMUX_GRPS][3] = { 
	{MSK32Gbl_pinMux_gp0, LSb32Gbl_pinMux_gp0, bGbl_pinMux_gp0},
	{MSK32Gbl_pinMux_gp1, LSb32Gbl_pinMux_gp1, bGbl_pinMux_gp1},
	{MSK32Gbl_pinMux_gp2, LSb32Gbl_pinMux_gp2, bGbl_pinMux_gp2},
	{MSK32Gbl_pinMux_gp3, LSb32Gbl_pinMux_gp3, bGbl_pinMux_gp3},
	{MSK32Gbl_pinMux_gp4, LSb32Gbl_pinMux_gp4, bGbl_pinMux_gp4},
	{MSK32Gbl_pinMux_gp5, LSb32Gbl_pinMux_gp5, bGbl_pinMux_gp5},
	{MSK32Gbl_pinMux_gp6, LSb32Gbl_pinMux_gp6, bGbl_pinMux_gp6},
	{MSK32Gbl_pinMux_gp7, LSb32Gbl_pinMux_gp7, bGbl_pinMux_gp7},
	{MSK32Gbl_pinMux_gp8, LSb32Gbl_pinMux_gp8, bGbl_pinMux_gp8},
	{MSK32Gbl_pinMux_gp9, LSb32Gbl_pinMux_gp9, bGbl_pinMux_gp9},
	{MSK32Gbl_pinMux_gp10, LSb32Gbl_pinMux_gp10, bGbl_pinMux_gp10},
	{MSK32Gbl_pinMux_gp11, LSb32Gbl_pinMux_gp11, bGbl_pinMux_gp11},	
	{MSK32Gbl_pinMux_gp12, LSb32Gbl_pinMux_gp12, bGbl_pinMux_gp12},
	{MSK32Gbl_pinMux_gp13, LSb32Gbl_pinMux_gp13, bGbl_pinMux_gp13},
	{MSK32Gbl_pinMux_gp14, LSb32Gbl_pinMux_gp14, bGbl_pinMux_gp14},
	{MSK32Gbl_pinMux_gp15, LSb32Gbl_pinMux_gp15, bGbl_pinMux_gp15},
	{MSK32Gbl_pinMux_gp16, LSb32Gbl_pinMux_gp16, bGbl_pinMux_gp16},
	{MSK32Gbl_pinMux_gp17, LSb32Gbl_pinMux_gp17, bGbl_pinMux_gp17},
	{MSK32Gbl_pinMux_gp18, LSb32Gbl_pinMux_gp18, bGbl_pinMux_gp18},
	{MSK32Gbl_pinMux_gp19, LSb32Gbl_pinMux_gp19, bGbl_pinMux_gp19},
	{MSK32Gbl_pinMux_gp20, LSb32Gbl_pinMux_gp20, bGbl_pinMux_gp20},
	{MSK32Gbl_pinMux_gp21, LSb32Gbl_pinMux_gp21, bGbl_pinMux_gp21},
	{MSK32Gbl_pinMux_gp22, LSb32Gbl_pinMux_gp22, bGbl_pinMux_gp22},
	{MSK32Gbl_pinMux_gp23, LSb32Gbl_pinMux_gp23, bGbl_pinMux_gp23},
	{MSK32Gbl_pinMux_gp24, LSb32Gbl_pinMux_gp24, bGbl_pinMux_gp24},
	{MSK32Gbl_pinMux_gp25, LSb32Gbl_pinMux_gp25, bGbl_pinMux_gp25},
	{MSK32Gbl_pinMux_gp26, LSb32Gbl_pinMux_gp26, bGbl_pinMux_gp26},
	{MSK32Gbl_pinMux_gp27, LSb32Gbl_pinMux_gp27, bGbl_pinMux_gp27},	
	{MSK32Gbl_pinMux_gp28, LSb32Gbl_pinMux_gp28, bGbl_pinMux_gp28},
	{MSK32Gbl_pinMux_gp29, LSb32Gbl_pinMux_gp29, bGbl_pinMux_gp29},
	{MSK32Gbl_pinMux_gp30, LSb32Gbl_pinMux_gp30, bGbl_pinMux_gp30},
	{MSK32Gbl_pinMux_gp31, LSb32Gbl_pinMux_gp31, bGbl_pinMux_gp31},
	{MSK32Gbl_pinMux_gp32, LSb32Gbl_pinMux_gp32, bGbl_pinMux_gp32},
	{MSK32Gbl_pinMux_gp33, LSb32Gbl_pinMux_gp33, bGbl_pinMux_gp33},
	{MSK32Gbl_pinMux_gp34, LSb32Gbl_pinMux_gp34, bGbl_pinMux_gp34},
	{MSK32Gbl_pinMux_gp35, LSb32Gbl_pinMux_gp35, bGbl_pinMux_gp35},	
	{MSK32Gbl_pinMux_gp36, LSb32Gbl_pinMux_gp36, bGbl_pinMux_gp36},
	{MSK32Gbl_pinMux_gp37, LSb32Gbl_pinMux_gp37, bGbl_pinMux_gp37}
};
#endif
	
#if !defined(BERLIN)
/*******************************************************************************
* Function:    PinMuxSetGrp
*
* Description:  set output pin mux group mode
*
* Inputs:      	grpID 	--	PinMux group ID 
*				mode	-- 	group output mode
*       
* Outputs:     none
* Return:      none
*******************************************************************************/
void PinMuxSetGrp(UNSG32 grpID, UNSG32 mode)
{
	UNSG32 pin_mux, mask, shift, bits, offset;
	if(grpID >= NUM_PINMUX_GRPS) return; // void groups
	mask = pinmuxtab[grpID][0];
	shift = pinmuxtab[grpID][1];
	bits = pinmuxtab[grpID][2];
	if(mode >= (0x1<<bits)) return; // invalid mode
	if(grpID <= FirstGrpLastID)
		offset = RA_Gbl_pinMux;	//in first grp
	else
		offset = RA_Gbl_pinMux1; //in second grp
	
	GA_REG_WORD32_READ(MEMMAP_CHIP_CTRL_REG_BASE + offset, &pin_mux);
    pin_mux &= (~(mask));
    pin_mux |= (mode << shift);
    GA_REG_WORD32_WRITE(MEMMAP_CHIP_CTRL_REG_BASE+ offset, pin_mux);
}

/******************************************************************************
* Function:    PinMuxGetGrpMode
*
* Description:  Get pin mux group mode
*
* Inputs:      	grpID 	--	PinMux group ID 
*       
* Outputs:     mode 	-- the pointer the mode filled;
* Return:      0, success
*			   others: Fail
*******************************************************************************/
UNSG32 PinMuxGetGrpMode(UNSG32 grpID, UNSG32 *mode)
{
	UNSG32 pin_mux, mask, shift, bits, offset;
	if(grpID >= NUM_PINMUX_GRPS) return 1; // void groups
	if(mode == NULL) return 1;
	if(grpID <= FirstGrpLastID)
		offset = RA_Gbl_pinMux;	//in first grp
	else
		offset = RA_Gbl_pinMux1; //in second grp
	GA_REG_WORD32_READ(MEMMAP_CHIP_CTRL_REG_BASE + offset, &pin_mux);
	mask = pinmuxtab[grpID][0];
	shift = pinmuxtab[grpID][1];
	bits = pinmuxtab[grpID][2];
    pin_mux &= mask;
    pin_mux = (pin_mux >> shift);
    pin_mux &= ((0x1 <<bits) -1);
    *mode = pin_mux;
    return 0;
}
#endif

#define PWM_2_ENABLE

/******************************************************************************
* Function:    PinMuxInitSetting
*
* Description:  Init PinMux setting for Galois
*
* Inputs:      none
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PinMuxInitSetting(void)
{
#if (BERLIN_CHIP_VERSION >= BERLIN_B_0)
	{
		T32Gbl_pinMux reg;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(reg.u32)); // read current pinmux

		// enable I2S2 MCLK
		reg.upinMux_gp7 = Gbl_pinMux_gp7_MODE_3;
        //for hualu, gp3 is used to control mute/unmute, default unmute 
#if defined(BERLIN_MK1)||defined(BERLIN_MK3) 
		reg.upinMux_gp3 = Gbl_pinMux_gp3_MODE_2;
#endif		
		// gp10, gp11, I2C
		reg.upinMux_gp10 = Gbl_pinMux_gp10_MODE_1;
		reg.upinMux_gp11 = Gbl_pinMux_gp11_MODE_1;
		//gp12, default I2S_LRCLK, BCLK is on with mode_0
		//reg.pinMux_gp12 = Gbl_pinMux_gp12_MODE_0;
		// gp14-- gp20 : DVIO[]
		reg.upinMux_gp14 = Gbl_pinMux_gp14_MODE_1;
		reg.upinMux_gp15 = Gbl_pinMux_gp15_MODE_1;
		reg.upinMux_gp16 = Gbl_pinMux_gp16_MODE_1;
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(reg.u32));
	}

	{
		T32Gbl_pinMux1 reg;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),&(reg.u32)); // read current pinmux

		reg.upinMux_gp17 = Gbl_pinMux_gp17_MODE_1;
		reg.upinMux_gp18 = Gbl_pinMux_gp18_MODE_1;
		// gp14-- gp20 : DVIO[]
		reg.upinMux_gp19 = Gbl_pinMux_gp19_MODE_1;
		// gp14-- gp20 : DVIO[]
		reg.upinMux_gp20 = Gbl_pinMux_gp20_MODE_1;

		// gp25: DV0 H_syc, Vsync, FID
		reg.upinMux_gp25 = Gbl_pinMux_gp25_MODE_1;
		// gp22. gp23: DV1 H_syc, Vsync, FID
		// I2S0 MCLK
		reg.upinMux_gp21 = Gbl_pinMux_gp21_MODE_2;
		// I2S1 MCLK
		reg.upinMux_gp23 = Gbl_pinMux_gp23_MODE_2;
		// gp22. : HPD to HDMI TX hardware block
		reg.upinMux_gp22 = Gbl_pinMux_gp21_MODE_3;

		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),(reg.u32));
	}
	{
		T32Gbl_PadSelect reg;
		GA_REG_WORD32_READ(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect, &(reg.u32));
		reg.uPadSelect_DVIO_OEN = Gbl_PadSelect_DVIO_OEN_Enable; 
		reg.uPadSelect_DVIO0_V18EN = 0;
		reg.uPadSelect_CLK0_ZN = 7;
		reg.uPadSelect_CLK0_ZP = 7;
		reg.uPadSelect_DVIO0_ZN = 7;
		reg.uPadSelect_DVIO0_ZP = 7;
		GA_REG_WORD32_WRITE(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect, (reg.u32));
	}
	{
		T32smSysCtl_SM_GSM_SEL reg;
		GA_REG_WORD32_READ(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, &(reg.u32));
		// ENABLE SPI2
		// SM_SPI2_SS0N
		reg.uSM_GSM_SEL_GSM7_SEL = 1; 
		// SM_SPI2_SS1N
		reg.uSM_GSM_SEL_GSM8_SEL = 0; 
		// SM_SPI2_SS2N
		reg.uSM_GSM_SEL_GSM0_SEL = 1; 
		// SM_SPI2_SS3N
		// Please set uSM_GSM_SEL_GSM1_SEL carefully, it's possibly used by Power_OK -Yufeng
		//reg.uSM_GSM_SEL_GSM1_SEL = 1; 
		// SM_SPI2_SCLK, SM_SPI2_SDO
		reg.uSM_GSM_SEL_GSM6_SEL = 1; 

		// ENABLE UART0 and UART1
		reg.uSM_GSM_SEL_GSM2_SEL = 1; 
		reg.uSM_GSM_SEL_GSM3_SEL = /*1*/0; 
		reg.uSM_GSM_SEL_GSM4_SEL = 0; 
		reg.uSM_GSM_SEL_GSM5_SEL = 1; 
		GA_REG_WORD32_WRITE(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, (reg.u32));
	}
#endif

}
/******************************************************************************
* Function:    simple_PinMuxInitSetting
*
* Description:  Partially init PinMux setting for Galois
*
* Inputs:      none
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void simple_PinMuxInitSetting(void)
{
#if 0	/* Plese reserve gp1, gp4, gp9 for SD/SDIO. see simple_PinMuxInitSetting */
	{
		T32Gbl_pinMux reg;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(reg.u32)); // read current pinmux
	
		// gp1: SD_DAT3
		reg.upinMux_gp1 = Gbl_pinMux_gp1_MODE_2;
		// gp4: SD_DAT0, SD_CMD
		reg.upinMux_gp4 = Gbl_pinMux_gp4_MODE_2;
		// gp9: SD_CLK, SD_DAT1, SD_DAT2. SD_CDn, and SD_WP
		reg.upinMux_gp9 = Gbl_pinMux_gp9_MODE_2;

		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(reg.u32));
	}
#endif
	{
		T32Gbl_pinMux1 reg;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),&(reg.u32)); // read current pinmux

		// gp24: ETH MDC, MDIO
		reg.upinMux_gp24 = Gbl_pinMux_gp24_MODE_1;

		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),(reg.u32));
	}
	{
		T32smSysCtl_SM_GSM_SEL reg;
		int do_init = 0;
		GA_REG_WORD32_READ(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, &(reg.u32));
		// ENABLE UART0 and UART1
		if (reg.uSM_GSM_SEL_GSM2_SEL != 1) {
			reg.uSM_GSM_SEL_GSM2_SEL = 1; 
			do_init = 1;
		}
		if ( reg.uSM_GSM_SEL_GSM3_SEL == 1) { 
			reg.uSM_GSM_SEL_GSM3_SEL = 0; 
			do_init = 1;
		}
		if (reg.uSM_GSM_SEL_GSM4_SEL == 1) {
			reg.uSM_GSM_SEL_GSM4_SEL = 0; 
			do_init = 1;
		}
		if (reg.uSM_GSM_SEL_GSM5_SEL != 1) {
			reg.uSM_GSM_SEL_GSM5_SEL = 1; 
			do_init = 1;
		}
		if (do_init) {
			GA_REG_WORD32_WRITE(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, (reg.u32));
		}
	}

}

/******************************************************************************
* Function:    Customization_PinMuxInitSetting
*
* Description:  Partially init PinMux setting for Galois using customization setting
*
* Inputs:      pinmux value address
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void Customization_PinMuxInitSetting(unsigned int * puiSystem_Customization_Pinmux){

#if (BERLIN_CHIP_VERSION >= BERLIN_B_0)

	T32Gbl_pinMux	SoC_Pinmux0_Value ; 
	T32Gbl_pinMux1 	SoC_Pinmux1_Value;  
	T32smSysCtl_SM_GSM_SEL	SM_Pinmux_Value ; 

	//	RA_Gbl_pinMux has 17 gps
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(SoC_Pinmux0_Value.u32)); 
	
	if( *(puiSystem_Customization_Pinmux) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp0 = *(puiSystem_Customization_Pinmux) ; 

	if( *(puiSystem_Customization_Pinmux + 1) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp1 = *(puiSystem_Customization_Pinmux + 1) ;

	if( *(puiSystem_Customization_Pinmux + 2) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp2 = *(puiSystem_Customization_Pinmux + 2) ;

	if( *(puiSystem_Customization_Pinmux + 3) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp3 = *(puiSystem_Customization_Pinmux + 3) ;
	
	if( *(puiSystem_Customization_Pinmux + 4) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp4 = *(puiSystem_Customization_Pinmux + 4) ;

	if( *(puiSystem_Customization_Pinmux + 5) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp5 = *(puiSystem_Customization_Pinmux + 5) ;

	if( *(puiSystem_Customization_Pinmux + 6) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp6 = *(puiSystem_Customization_Pinmux + 6) ;

	if( *(puiSystem_Customization_Pinmux + 7) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp7 = *(puiSystem_Customization_Pinmux + 7) ;

	if( *(puiSystem_Customization_Pinmux + 8) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp8 = *(puiSystem_Customization_Pinmux + 8) ;

	if( *(puiSystem_Customization_Pinmux + 9) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp9 = *(puiSystem_Customization_Pinmux + 9) ;

	if( *(puiSystem_Customization_Pinmux + 10) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp10 = *(puiSystem_Customization_Pinmux + 10) ;

	if( *(puiSystem_Customization_Pinmux + 11) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp11 = *(puiSystem_Customization_Pinmux + 11) ;

	if( *(puiSystem_Customization_Pinmux + 12) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp12 = *(puiSystem_Customization_Pinmux + 12) ;

	if( *(puiSystem_Customization_Pinmux + 13) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp13 = *(puiSystem_Customization_Pinmux + 13) ;
	
	if( *(puiSystem_Customization_Pinmux + 14) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp14 = *(puiSystem_Customization_Pinmux + 14) ;

	if( *(puiSystem_Customization_Pinmux + 15) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp15 = *(puiSystem_Customization_Pinmux + 15) ;

	if( *(puiSystem_Customization_Pinmux + 16) != 0xFF)
		SoC_Pinmux0_Value.upinMux_gp16 = *(puiSystem_Customization_Pinmux + 16) ;
	//for hualu, gp3 is used to control mute/unmute, default unmute 
#if defined(BERLIN_MK1)||defined(BERLIN_MK3) 
	SoC_Pinmux0_Value.upinMux_gp3 = Gbl_pinMux_gp3_MODE_2;
#endif	
	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(SoC_Pinmux0_Value.u32));

	//	pin mux reg 1

	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),&(SoC_Pinmux1_Value.u32)); // read current pinmux

	if(*(puiSystem_Customization_Pinmux + 17) != 0xFF)
		SoC_Pinmux1_Value.upinMux_gp17 = *(puiSystem_Customization_Pinmux + 17) ;

	if(*(puiSystem_Customization_Pinmux + 18) != 0xFF)
		SoC_Pinmux1_Value.upinMux_gp18 = *(puiSystem_Customization_Pinmux + 18) ;

	if(*(puiSystem_Customization_Pinmux + 19) != 0xFF)
		SoC_Pinmux1_Value.upinMux_gp19 = *(puiSystem_Customization_Pinmux + 19) ;
	
	if( *(puiSystem_Customization_Pinmux + 20) != 0xFF)
		SoC_Pinmux1_Value.upinMux_gp20 = *(puiSystem_Customization_Pinmux + 20) ;

	if( *(puiSystem_Customization_Pinmux + 21) != 0xFF)
		SoC_Pinmux1_Value.upinMux_gp21 = *(puiSystem_Customization_Pinmux + 21) ;

	if( *(puiSystem_Customization_Pinmux + 22) != 0xFF)
		SoC_Pinmux1_Value.upinMux_gp22 = *(puiSystem_Customization_Pinmux + 22) ;

	if( *(puiSystem_Customization_Pinmux + 23) != 0xFF)
		SoC_Pinmux1_Value.upinMux_gp23 = *(puiSystem_Customization_Pinmux + 23) ;
	
	if( *(puiSystem_Customization_Pinmux + 24) != 0xFF)
		SoC_Pinmux1_Value.upinMux_gp24 = *(puiSystem_Customization_Pinmux + 24) ;

	if( *(puiSystem_Customization_Pinmux + 25) != 0xFF)
		SoC_Pinmux1_Value.upinMux_gp25 = *(puiSystem_Customization_Pinmux + 25) ;

	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),(SoC_Pinmux1_Value.u32));


	{
		T32Gbl_PadSelect reg;
		GA_REG_WORD32_READ(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect, &(reg.u32));
		reg.uPadSelect_DVIO_OEN = Gbl_PadSelect_DVIO_OEN_Enable; 
		reg.uPadSelect_DVIO0_V18EN = 0;
		reg.uPadSelect_CLK0_ZN = 7;
		reg.uPadSelect_CLK0_ZP = 7;
		reg.uPadSelect_DVIO0_ZN = 7;
		reg.uPadSelect_DVIO0_ZP = 7;
		GA_REG_WORD32_WRITE(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect, (reg.u32));
	}


	GA_REG_WORD32_READ(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, &(SM_Pinmux_Value.u32));
	
	if( *(puiSystem_Customization_Pinmux + 26) != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM0_SEL = *(puiSystem_Customization_Pinmux + 26) ;

	if( *(puiSystem_Customization_Pinmux + 27) != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM1_SEL = *(puiSystem_Customization_Pinmux + 27) ;

	if( *(puiSystem_Customization_Pinmux + 28) != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM2_SEL = *(puiSystem_Customization_Pinmux + 28) ;

	if( *(puiSystem_Customization_Pinmux + 29) != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM3_SEL = *(puiSystem_Customization_Pinmux + 29) ;

	if( *(puiSystem_Customization_Pinmux + 30) != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM4_SEL = *(puiSystem_Customization_Pinmux + 30) ;

	if( *(puiSystem_Customization_Pinmux + 31) != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM5_SEL = *(puiSystem_Customization_Pinmux + 31) ;

	if( *(puiSystem_Customization_Pinmux + 32) != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM6_SEL = *(puiSystem_Customization_Pinmux + 32) ;

	if( *(puiSystem_Customization_Pinmux + 33) != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM7_SEL = *(puiSystem_Customization_Pinmux + 33) ;

	if( *(puiSystem_Customization_Pinmux + 34) != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM8_SEL = *(puiSystem_Customization_Pinmux + 34) ;

	GA_REG_WORD32_WRITE(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, (SM_Pinmux_Value.u32));
#endif
}

/******************************************************************************
* Function:    Customization_Simple_PinMuxInitSetting
*
* Description:  Partially init PinMux setting for Galois using customization setting
*
* Inputs:      none
* Outputs:     none
*
* Return:      none
*******************************************************************************/
#if 1
void Customization_Simple_PinMuxInitSetting(void){

#if (BERLIN_CHIP_VERSION >= BERLIN_B_0)
	unsigned int uiSoC_Pinmux[] = SOC_PINMUX ;
	unsigned int uiSM_Pinmux[] = SM_PINMUX ;
	T32Gbl_pinMux1 	SoC_Pinmux1_Value;  
	T32smSysCtl_SM_GSM_SEL	SM_Pinmux_Value ; 

	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),&(SoC_Pinmux1_Value.u32)); // read current pinmux
	if(uiSoC_Pinmux[24] != 0xFF)
	// gp24: ETH MDC, MDIO
		SoC_Pinmux1_Value.upinMux_gp24 = uiSoC_Pinmux[24];

	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),(SoC_Pinmux1_Value.u32));

	GA_REG_WORD32_READ(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, &(SM_Pinmux_Value.u32));
	
	if( uiSM_Pinmux[2] != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM2_SEL = uiSM_Pinmux[2] ; 

	if( uiSM_Pinmux[3] != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM3_SEL = uiSM_Pinmux[3] ;

	if( uiSM_Pinmux[4] != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM4_SEL = uiSM_Pinmux[4] ;

	if( uiSM_Pinmux[5] != 0xFF)
		SM_Pinmux_Value.uSM_GSM_SEL_GSM5_SEL = uiSM_Pinmux[5] ;

	GA_REG_WORD32_WRITE(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, (SM_Pinmux_Value.u32));
#endif
}
#endif
/**************** END of Pin mux Driver ***********************************************/
