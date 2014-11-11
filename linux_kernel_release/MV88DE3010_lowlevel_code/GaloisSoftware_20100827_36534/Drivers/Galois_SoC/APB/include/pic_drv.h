#include "pic.h"
#include "Galois_memmap.h"
#include "galois_io.h"
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
void PIC_SetSource(UNSG32 PIC_num, UNSG32 fsrcPolSel, UNSG32 fsrcSensSel, UNSG32 nsrcPolSel, UNSG32 nsrcSensSel);

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
void PIC_Get_nIntSts(UNSG32 PIC_num, UNSG32 *pnIntSts);

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
void PIC_Get_fIntSts(UNSG32 PIC_num, UNSG32 *pfIntSts);

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
void PIC_GetIntSts(UNSG32 PIC_num, UNSG32 *pfIntSts, UNSG32 *pnIntSts);

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
void PIC_SetSource(UNSG32 fsrcPolSel, UNSG32 fsrcSensSel, UNSG32 nsrcPolSel, UNSG32 nsrcSensSel);
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
void PIC_SetSourcePerVector(UNSG32 vec_num, UNSG32 fsrcPolSel, UNSG32 fsrcSensSel, UNSG32 nsrcPolSel, UNSG32 nsrcSensSel);
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
void PIC_Get_nIntSts(UNSG32 *pnIntSts);
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
void PIC_Get_fIntSts(UNSG32 *pfIntSts);
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
void PIC_GetIntSts(UNSG32 *pfIntSts, UNSG32 *pnIntSts);
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
void PIC_ClrIntSts(UNSG32 fIntSts, UNSG32 nIntSts);
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
void PIC_ClrIntStsPerVector(UNSG32 vec_num, UNSG32 fIntSts, UNSG32 nIntSts);
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
void PIC_SetPerPIC(UNSG32 PIC_num, UNSG32 fTgtPolSel, UNSG32 fTgtSensSel, UNSG32 fIntE, UNSG32 fGIntE, UNSG32 nTgtPolSel, UNSG32 nTgtSensSel, UNSG32 nIntE, UNSG32 nGIntE);

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
void PIC_SetPerPIC_PerVector(UNSG32 PIC_num, UNSG32 vec_num, UNSG32 fIntE, UNSG32 nIntE);
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
void PIC_GetPIC_fIntE(UNSG32 PIC_num, UNSG32 *pfIntE);
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
void PIC_GetPIC_nIntE(UNSG32 PIC_num, UNSG32 *pnIntE);
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
void PIC_SetPerPIC_TgtSel(UNSG32 PIC_num, UNSG32 fTgtPolSel, UNSG32 fTgtSensSel, UNSG32 nTgtPolSel, UNSG32 nTgtSensSel);
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
void PIC_SetPerPIC_TgtGIntE(UNSG32 PIC_num, UNSG32 fGIntE, UNSG32 nGIntE);
void PIC_Get_TgtGIntE(UNSG32 PIC_num, UNSG32 *fGIntE, UNSG32 *nGIntE);

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
void PIC_SRCHOSTtoDSTHOST_write_int(UNSG32 src_host, UNSG32 dst_host,  UNSG32 enable);

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
UNSG32 PIC_SRCHOSTtoDSTHOST_read_int(UNSG32 src_host, UNSG32 dst_host);
#endif

/******************************************************************************
* Function:    PIC_SRCHOSTtoDSTHOST_write_data
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
void PIC_SRCHOSTtoDSTHOST_write_data(UNSG32 src_host, UNSG32 dst_host, UNSG32 data);

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
UNSG32 PIC_SRCHOSTtoDSTHOST_read_data(UNSG32 src_host, UNSG32 dst_host);

