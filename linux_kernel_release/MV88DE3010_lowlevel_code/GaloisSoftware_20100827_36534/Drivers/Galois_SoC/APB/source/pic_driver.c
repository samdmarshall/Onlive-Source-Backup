#include "pic.h"
#include "Galois_memmap.h"
#include "galois_io.h"
#include "cinclude.h"
#if defined(BERLIN)
/*******************************************************************************
* Function:    PIC_SetSource
*
* Description: Set PIC common registers. (Source input)
*
* Inputs:      PIC_num-- PIC number.
*              fsrcPolSel-- FIQ src Polarity, 32bits. each bit 1-active high, 0- active low
*              fsrcSensSel--FIQ sensitivity. 32bits. each bits1- Edge triggered, 0- level triggered
*              nsrcPolSel-- IRQ src Polarity, 32bits. each bit 1-active high, 0- active low
*              nsrcSensSel-- IRQ sensitivity. 32bits. each bit 1- Edge triggered, 0- level triggered
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_SetSource(UNSG32 PIC_num, UNSG32 fsrcPolSel, UNSG32 fsrcSensSel, UNSG32 nsrcPolSel, UNSG32 nsrcSensSel) 
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+RA_PIC_cmn;
	UNSG32 fSrcPolSel_offset;
	UNSG32 fSrcSensSel_offset;
	UNSG32 nSrcPolSel_offset;
	UNSG32 nSrcSensSel_offset;
	switch (PIC_num) {
		case 1: //
			fSrcPolSel_offset = RA_cmn_fSrcPolSel1;
			fSrcSensSel_offset = RA_cmn_fSrcSensSel1;
			nSrcPolSel_offset = RA_cmn_nSrcPolSel1;
			nSrcSensSel_offset = RA_cmn_nSrcSensSel1;
			break;
		case 2: //
			fSrcPolSel_offset = RA_cmn_fSrcPolSel2;
			fSrcSensSel_offset = RA_cmn_fSrcSensSel2;
			nSrcPolSel_offset = RA_cmn_nSrcPolSel2;
			nSrcSensSel_offset = RA_cmn_nSrcSensSel2;
			break;
		case 0: //
		default : //
			fSrcPolSel_offset = RA_cmn_fSrcPolSel0;
			fSrcSensSel_offset = RA_cmn_fSrcSensSel0;
			nSrcPolSel_offset = RA_cmn_nSrcPolSel0;
			nSrcSensSel_offset = RA_cmn_nSrcSensSel0;
			break;
	}
	GA_REG_WORD32_WRITE(base_addr+fSrcPolSel_offset, fsrcPolSel);
	GA_REG_WORD32_WRITE(base_addr+fSrcSensSel_offset, fsrcSensSel);
	GA_REG_WORD32_WRITE(base_addr+nSrcPolSel_offset, nsrcPolSel);
	GA_REG_WORD32_WRITE(base_addr+nSrcSensSel_offset, nsrcSensSel);
}
/*******************************************************************************
* Function:    PIC_Get_nIntSts
*
* Description: Get interrupt IRQ status. (Source input)
*
* Inputs:      PIC_num-- PIC number.
* Outputs:     pnIntSts-- IRQ Interrupt status
*
* Return:      none
*******************************************************************************/
void PIC_Get_nIntSts(UNSG32 PIC_num, UNSG32 *pnIntSts)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+RA_PIC_cmn;
	UNSG32 nIntSts_offset;
	switch (PIC_num) {
		case 1: //
			nIntSts_offset = RA_cmn_nIntSts1;
			break;
		case 2: //
			nIntSts_offset = RA_cmn_nIntSts2;
			break;
		case 0: //
		default:
			nIntSts_offset = RA_cmn_nIntSts0;
			break;
	}

	GA_REG_WORD32_READ(base_addr+nIntSts_offset, pnIntSts);
}
/*******************************************************************************
* Function:    PIC_Get_fIntSts
*
* Description: Get interrupt FIQ status. (Source input)
*
* Inputs:      PIC_num-- PIC number.
* Outputs:     pfIntSts-- FIQ Interrupt status
*
* Return:      none
*******************************************************************************/
void PIC_Get_fIntSts(UNSG32 PIC_num, UNSG32 *pfIntSts)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+RA_PIC_cmn;
	UNSG32 fIntSts_offset;
	switch (PIC_num) {
		case 1: //
			fIntSts_offset = RA_cmn_fIntSts1;
			break;
		case 2: //
			fIntSts_offset = RA_cmn_fIntSts2;
			break;
		case 0: //
		default:
			fIntSts_offset = RA_cmn_fIntSts0;
			break;
	}
	GA_REG_WORD32_READ(base_addr+fIntSts_offset, pfIntSts);
}
/*******************************************************************************
* Function:    PIC_GetIntSts
*
* Description: Get interrupt status. (Source input)
*
* Inputs:      PIC_num-- PIC number.
* Outputs:     pfIntSts-- FIQ Interrupt status
*              pnIntSts-- IRQ Interrupt status
*
* Return:      none
*******************************************************************************/
void PIC_GetIntSts(UNSG32 PIC_num, UNSG32 *pfIntSts, UNSG32 *pnIntSts)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+RA_PIC_cmn;
	UNSG32 nIntSts_offset;
	UNSG32 fIntSts_offset;
	switch (PIC_num) {
		case 1: //
			nIntSts_offset = RA_cmn_nIntSts1;
			fIntSts_offset = RA_cmn_fIntSts1;
			break;
		case 2: //
			nIntSts_offset = RA_cmn_nIntSts2;
			fIntSts_offset = RA_cmn_fIntSts2;
			break;
		case 0: //
		default:
			nIntSts_offset = RA_cmn_nIntSts0;
			fIntSts_offset = RA_cmn_fIntSts0;
			break;
	}
	GA_REG_WORD32_READ(base_addr+fIntSts_offset, pfIntSts);
	GA_REG_WORD32_READ(base_addr+nIntSts_offset, pnIntSts);
}
#else
/*******************************************************************************
* Function:    PIC_SetSource
*
* Description: Set PIC common registers. (Source input)
*
* Inputs:      fsrcPolSel-- FIQ src Polarity, 32bits. each bit 1-active high, 0- active low
*              fsrcSensSel--FIQ sensitivity. 32bits. each bits1- Edge triggered, 0- level triggered
*              nsrcPolSel-- IRQ src Polarity, 32bits. each bit 1-active high, 0- active low
*              nsrcSensSel-- IRQ sensitivity. 32bits. each bit 1- Edge triggered, 0- level triggered
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_SetSource(UNSG32 fsrcPolSel, UNSG32 fsrcSensSel, UNSG32 nsrcPolSel, UNSG32 nsrcSensSel) 
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+RA_PIC_cmn;

	GA_REG_WORD32_WRITE(base_addr+RA_cmn_fSrcPolSel, fsrcPolSel);
	GA_REG_WORD32_WRITE(base_addr+RA_cmn_fSrcSensSel, fsrcSensSel);
	GA_REG_WORD32_WRITE(base_addr+RA_cmn_nSrcPolSel, nsrcPolSel);
	GA_REG_WORD32_WRITE(base_addr+RA_cmn_nSrcSensSel, nsrcSensSel);
}
/*******************************************************************************
* Function:    PIC_SetSourcePerVector
*
* Description: Set PIC common registers for specific vector. (Source input)
*
* Inputs:      vec_num-- vector number. (corresponding to the bit number)     
*              fsrcPolSel-- FIQ src Polarity, 1-active high, 0- active low
*              fsrcSensSel--FIQ sensitivity. 1- Edge triggered, 0- level triggered
*              nsrcPolSel-- IRQ src Polarity, 1-active high, 0- active low
*              nsrcSensSel-- IRQ sensitivity. 1- Edge triggered, 0- level triggered
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_SetSourcePerVector(UNSG32 vec_num, UNSG32 fsrcPolSel, UNSG32 fsrcSensSel, UNSG32 nsrcPolSel, UNSG32 nsrcSensSel) 
{
	UNSG32 temp;
	UNSG32 mask;
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+RA_PIC_cmn;
	mask = ~(1<<vec_num);
	// set fsrcPolsel
	GA_REG_WORD32_READ(base_addr+RA_cmn_fSrcPolSel, &temp);
	temp &= mask;
	temp |= fsrcPolSel << vec_num;
	GA_REG_WORD32_WRITE(base_addr+RA_cmn_fSrcPolSel, temp);

	// set fsrcSensSel
	GA_REG_WORD32_READ(base_addr+RA_cmn_fSrcSensSel, &temp);
	temp &= mask;
	temp |= fsrcSensSel << vec_num;
	GA_REG_WORD32_WRITE(base_addr+RA_cmn_fSrcSensSel, temp);

	// set nSrcPolSel
	GA_REG_WORD32_READ(base_addr+RA_cmn_nSrcPolSel, &temp);
	temp &= mask;
	temp |= nsrcPolSel << vec_num;
	GA_REG_WORD32_WRITE(base_addr+RA_cmn_nSrcPolSel, temp);

	//set nSrcSensSel
	GA_REG_WORD32_READ(base_addr+RA_cmn_nSrcSensSel, &temp);
	temp &= mask;
	temp |= nsrcSensSel << vec_num;
	GA_REG_WORD32_WRITE(base_addr+RA_cmn_nSrcSensSel, temp);
}
/*******************************************************************************
* Function:    PIC_Get_nIntSts
*
* Description: Get interrupt IRQ status. (Source input)
*
* Inputs:      none
* Outputs:     pnIntSts-- IRQ Interrupt status
*
* Return:      none
*******************************************************************************/
void PIC_Get_nIntSts(UNSG32 *pnIntSts)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+RA_PIC_cmn;
	GA_REG_WORD32_READ(base_addr+RA_cmn_nIntSts, pnIntSts);
}
/*******************************************************************************
* Function:    PIC_Get_fIntSts
*
* Description: Get interrupt FIQ status. (Source input)
*
* Inputs:      none
* Outputs:     pfIntSts-- FIQ Interrupt status
*
* Return:      none
*******************************************************************************/
void PIC_Get_fIntSts(UNSG32 *pfIntSts)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+RA_PIC_cmn;
	GA_REG_WORD32_READ(base_addr+RA_cmn_fIntSts, pfIntSts);
}
/*******************************************************************************
* Function:    PIC_GetIntSts
*
* Description: Get interrupt status. (Source input)
*
* Inputs:      none
* Outputs:     pfIntSts-- FIQ Interrupt status
*              pnIntSts-- IRQ Interrupt status
*
* Return:      none
*******************************************************************************/
void PIC_GetIntSts(UNSG32 *pfIntSts, UNSG32 *pnIntSts)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+RA_PIC_cmn;
	GA_REG_WORD32_READ(base_addr+RA_cmn_fIntSts, pfIntSts);
	GA_REG_WORD32_READ(base_addr+RA_cmn_nIntSts, pnIntSts);
}
/*******************************************************************************
* Function:    PIC_ClrIntSts
*
* Description: Clear interrupt status at once. (set the desire vectors bits to 1.)
*
* Inputs:      fIntSts-- FIQ Interrupt status
*              nIntSts-- IRQ Interrupt status
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_ClrIntSts(UNSG32 fIntSts, UNSG32 nIntSts)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+RA_PIC_cmn;
	GA_REG_WORD32_WRITE(base_addr+RA_cmn_fIntSts, fIntSts);
	GA_REG_WORD32_WRITE(base_addr+RA_cmn_nIntSts, nIntSts);
}
/*******************************************************************************
* Function:    PIC_ClrIntStsPerVector
*
* Description: Clear interrupt status for specific vector.
*
* Inputs:      fIntSts-- FIQ Interrupt status. (1--- clear, 0-- do nothing)
*              nIntSts-- IRQ Interrupt status. (1--- clear, 0-- do nothing)
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_ClrIntStsPerVector(UNSG32 vec_num, UNSG32 fIntSts, UNSG32 nIntSts)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+RA_PIC_cmn;
	UNSG32 temp;
	if (fIntSts) {
		temp = fIntSts << vec_num;
		GA_REG_WORD32_WRITE(base_addr+RA_cmn_fIntSts, temp);
	}
	if (nIntSts) {
		temp = nIntSts << vec_num;
		GA_REG_WORD32_WRITE(base_addr+RA_cmn_nIntSts, temp);
	}
}
#endif
/*******************************************************************************
* Function:    PIC_SetPerPIC
*
* Description: Set all of perPIC registers at once. (Target)
*
* Inputs:      PIC_num-- PIC number.
*              fsrcPolSel-- FIQ src Polarity, 1-active high, 0- active low
*              fsrcSensSel--FIQ sensitivity. 1- Edge triggered, 0- level triggered
*              fIntE--------FIQ interrupt enable bit, 32bits. each bit 1-- enable, 0 -- disable
*              fGIntE--------FIQ globale interrupt enable bit. 1-- enable, 0 -- disable
*              nsrcPolSel-- IRQ src Polarity, 1-active high, 0- active low
*              nsrcSensSel-- IRQ sensitivity. 1- Edge triggered, 0- level triggered
*              nIntE--------IRQ interrupt enable bit, 32bits. each bit 1-- enable, 0 -- disable
*              nGIntE--------IRQ globale interrupt enable bit. 1-- enable, 0 -- disable
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_SetPerPIC(UNSG32 PIC_num, UNSG32 fTgtPolSel, UNSG32 fTgtSensSel, UNSG32 fIntE, UNSG32 fGIntE,
								   UNSG32 nTgtPolSel, UNSG32 nTgtSensSel, UNSG32 nIntE, UNSG32 nGIntE)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_num*sizeof(SIE_perPIC);

	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_fTgtPolSel, fTgtPolSel);
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_fTgtSensSel, fTgtSensSel);
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_fIntE, fIntE);
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_fGIntE, fGIntE);
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_nTgtPolSel, nTgtPolSel);
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_nTgtSensSel, nTgtSensSel);
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_nIntE, nIntE);
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_nGIntE, nGIntE);
}

/*******************************************************************************
* Function:    PIC_SetPerPIC_PerVector
*
* Description: Set perPIC registers for sepcific vector. (Target)
*
* Inputs:      PIC_num-- PIC number.
*              vec_num--- Vector number.
*              fIntE--------FIQ interrupt enable bit, 32bits. each bit 1-- enable, 0 -- disable
*              nIntE--------IRQ interrupt enable bit, 32bits. each bit 1-- enable, 0 -- disable
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_SetPerPIC_PerVector(UNSG32 PIC_num, UNSG32 vec_num, UNSG32 fIntE, UNSG32 nIntE)
{
	UNSG32 temp;
	UNSG32 mask;
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_num*sizeof(SIE_perPIC);
	mask = ~(1<<vec_num);

	// set fIntE
	GA_REG_WORD32_READ(base_addr+RA_perPIC_fIntE, &temp);
	temp &= mask;
	temp |= fIntE << vec_num;
		
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_fIntE, temp);

	// set nIntE
	GA_REG_WORD32_READ(base_addr+RA_perPIC_nIntE, &temp);
	temp &= mask;
	temp |= nIntE << vec_num;
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_nIntE, temp);
}
/*******************************************************************************
* Function:    PIC_GetPIC_fIntE
*
* Description: Get perPIC fIntE. (Target)
*
* Inputs:      PIC_num-- PIC number.
*              fIntE--------FIQ interrupt enable bit, 32bits. each bit 1-- enable, 0 -- disable
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_GetPIC_fIntE(UNSG32 PIC_num, UNSG32 *pfIntE)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_num*sizeof(SIE_perPIC);
	GA_REG_WORD32_READ(base_addr+RA_perPIC_fIntE, pfIntE);
}
/*******************************************************************************
* Function:    PIC_GetPIC_nIntE
*
* Description: Get perPIC nIntE. (Target)
*
* Inputs:      PIC_num-- PIC number.
*              nIntE--------IRQ interrupt enable bit, 32bits. each bit 1-- enable, 0 -- disable
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_GetPIC_nIntE(UNSG32 PIC_num, UNSG32 *pnIntE)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_num*sizeof(SIE_perPIC);
	GA_REG_WORD32_READ(base_addr+RA_perPIC_nIntE, pnIntE);
}
/*******************************************************************************
* Function:    PIC_SetPerPIC_TgtSel
*
* Description: Set target select register. (Target)
*
* Inputs:      PIC_num-- PIC number.
*              fsrcPolSel-- FIQ src Polarity, 1-active high, 0- active low
*              fsrcSensSel--FIQ sensitivity. 1- Edge triggered, 0- level triggered
*              nsrcPolSel-- IRQ src Polarity, 1-active high, 0- active low
*              nsrcSensSel-- IRQ sensitivity. 1- Edge triggered, 0- level triggered
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_SetPerPIC_TgtSel(UNSG32 PIC_num, UNSG32 fTgtPolSel, UNSG32 fTgtSensSel,
										 UNSG32 nTgtPolSel, UNSG32 nTgtSensSel)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_num*sizeof(SIE_perPIC);

	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_fTgtPolSel, fTgtPolSel);
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_fTgtSensSel, fTgtSensSel);

	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_nTgtPolSel, nTgtPolSel);
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_nTgtSensSel, nTgtSensSel);
}
/*******************************************************************************
* Function:    PIC_SetPerPIC_TgtGIntE
*
* Description: Set all of perPIC registers at once. (Target)
*
* Inputs:      PIC_num-- PIC number.
*              fGIntE--------FIQ globale interrupt enable bit. 1-- enable, 0 -- disable
*              nGIntE--------IRQ globale interrupt enable bit. 1-- enable, 0 -- disable
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_SetPerPIC_TgtGIntE(UNSG32 PIC_num, UNSG32 fGIntE, UNSG32 nGIntE)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_num*sizeof(SIE_perPIC);

	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_fGIntE, fGIntE);
	GA_REG_WORD32_WRITE(base_addr+RA_perPIC_nGIntE, nGIntE);
}
void PIC_Get_TgtGIntE(UNSG32 PIC_num, UNSG32 *fGIntE, UNSG32 *nGIntE)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_num*sizeof(SIE_perPIC);
	GA_REG_WORD32_READ(base_addr+RA_perPIC_fGIntE, fGIntE);
	GA_REG_WORD32_READ(base_addr+RA_perPIC_nGIntE, nGIntE);
}
/******************************************************************************
* Function:    PIC_SRCHOSTtoDSTHOST_write_int
*
* Description: Set or clear SRC_HOST to DST_HOST interrupt.
*
* Inputs:      src_host-- source host (PIC_CPU0== 0, PIC_CPU1 == 1, PIC_PCIE== 2) 
*              dst_host-- destination host 
*              enable -- set/clear interrupt. (1--- set, 0-- clear)
* Outputs:     none
*
* Return:      none
*******************************************************************************/

static UNSG32 PIC_int_interhost[3][3] = {
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
			0, 0, 0, // destination CPU0
			0, 0, 0, // destination CPU1
			0, 0, 0, /// destination PCIE
#else
/*source	CPU0					CPU1					PCIE	*/
			0,						RA_PIC_int_CPU1toCPU0,	RA_PIC_int_PCIEtoCPU0, // destination CPU0
			RA_PIC_int_CPU0toCPU1,	0,						RA_PIC_int_PCIEtoCPU1, // destination CPU1
			RA_PIC_int_CPU0toPCIE,	RA_PIC_int_CPU1toPCIE,	0,				       // destination PCIE
#endif
};
/******************************************************************************
* Function:    PIC_SRCHOSTtoDSTHOST_write_int
*
* Description: Set or clear SRC_HOST to DST_HOST interrupt.
*
* Inputs:      src_host-- source host (PIC_CPU0== 0, PIC_CPU1 == 1, PIC_PCIE== 2) 
*              dst_host-- destination host 
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PIC_SRCHOSTtoDSTHOST_write_int(UNSG32 src_host, UNSG32 dst_host,  UNSG32 enable)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_int_interhost[dst_host][src_host];
	if (enable) {
		enable = 1;
	}
	GA_REG_WORD32_WRITE(base_addr, enable);
}
#if (BERLIN_CHIP_VERSION < BERLIN_BG2)
/******************************************************************************
* Function:    PIC_SRCHOSTtoDSTHOST_read_int
*
* Description: Read SRC_HOST to DST_HOST interrupt bit.
*
* Inputs:      src_host-- source host (PIC_CPU0== 0, PIC_CPU1 == 1, PIC_PCIE== 2) 
*              dst_host-- destination host 
* Outputs:     none
*
* Return:      interrupt status bit
*******************************************************************************/
UNSG32 PIC_SRCHOSTtoDSTHOST_read_int(UNSG32 src_host, UNSG32 dst_host)
{
	UNSG32 status;
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_int_interhost[dst_host][src_host];

	GA_REG_WORD32_READ(base_addr, &status);
	status = CutTo(status, bPIC_int_CPU1toCPU0_int);

	return status;
}
#endif
/******************************************************************************
* Function:    PIC_SRCHOSTtoDSTHOST_writedata
*
* Description: write data for SRC_HOST to DST_HOST interrupt.
*
* Inputs:      src_host-- source host (CPU0== 0, CPU1 == 1, PCIE== 2) 
*              dst_host-- destination host 
*              data -- data to be written into the register.
* Outputs:     none
*
* Return:      none
*******************************************************************************/
static UNSG32 PIC_data_interhost[3][3] = {
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
			0, 0, 0, // destination CPU0
			0, 0, 0, // destination CPU1
			0, 0, 0, /// destination PCIE
#else
/*source	CPU0					CPU1					PCIE	*/
			0,						RA_PIC_data_CPU1toCPU0,	RA_PIC_data_PCIEtoCPU0, // destination CPU0
			RA_PIC_data_CPU0toCPU1, 0,						RA_PIC_data_PCIEtoCPU1, // destination CPU1
			RA_PIC_data_CPU0toPCIE,	RA_PIC_data_CPU1toPCIE,	0,				        // destination PCIE
#endif
};
void PIC_SRCHOSTtoDSTHOST_write_data(UNSG32 src_host, UNSG32 dst_host, UNSG32 data)
{
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_data_interhost[dst_host][src_host];
	GA_REG_WORD32_WRITE(base_addr, data);
}
/******************************************************************************
* Function:    PIC_SRCHOSTtoDSTHOST_read_data
*
* Description: read data for SRC_HOST to DST_HOST interrupt.
*
* Inputs:      src_host-- source host (CPU0== 0, CPU1 == 1, PCIE== 2) 
*              dst_host-- destination host 
* Outputs:     none
*
* Return:      data -- data read from the register.
*******************************************************************************/

UNSG32 PIC_SRCHOSTtoDSTHOST_read_data(UNSG32 src_host, UNSG32 dst_host)
{
	UNSG32 data;
	UNSG32 base_addr=MEMMAP_PIC_REG_BASE+PIC_data_interhost[dst_host][src_host];
	GA_REG_WORD32_READ(base_addr, &data);
	return (data);
}
