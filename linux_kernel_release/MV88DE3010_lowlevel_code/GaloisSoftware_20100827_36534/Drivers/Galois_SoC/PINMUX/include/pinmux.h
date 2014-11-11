

#ifndef __PINMUX_H__
#define __PINMUX_H__

//  Output pin mux group define here

/*!
******************************************************************************
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
void PinMuxSetGrp(UNSG32 grpID, UNSG32 mode);

/*!
******************************************************************************
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
UNSG32 PinMuxGetGrpMode(UNSG32 grpID, UNSG32 *mode);

/*!
******************************************************************************
* Function:    PinMuxInitSetting
*
* Description:  Init PinMux setting for Galois
*
* Inputs:      none
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void PinMuxInitSetting(void);

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
void simple_PinMuxInitSetting(void);

/******************************************************************************
* Function:    Customization_PinMuxInitSetting
*
* Description:  Partially init PinMux setting for Galois using customization setting
*
* Inputs:      address of pinmux value
* Outputs:     none
*
* Return:      none
*******************************************************************************/

void Customization_PinMuxInitSetting(unsigned int * puiSystem_Customization_Pinmux) ;

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
void Customization_Simple_PinMuxInitSetting(void) ;

#endif
/**************** END of PIN MUX Driver ***********************************************/
