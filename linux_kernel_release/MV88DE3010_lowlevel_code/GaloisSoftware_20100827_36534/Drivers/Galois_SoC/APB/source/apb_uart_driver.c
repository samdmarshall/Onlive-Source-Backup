#include "com_type.h" /* include <common.h> for __UBOOT__ vsprintf */
#include "galois_common.h"
#include "galois_io.h"
#include "apb_perf_base.h"
#include "apb_uart.h"
#include "apb_uart_drv.h"
#ifndef WIN32
#include "global.h"
#endif

#define __UART_OUTPUT_BUF_SIZE 80
static SIGN8 apb_print_buf[__UART_OUTPUT_BUF_SIZE];

/*******************************************************************************
* Function:    APB_UART_init
*
* Description: initialize UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   tclk-- UART source clock
*			   baud-- UART baud rate
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void APB_UART_init(UNSG32 channel, UNSG32 tclk, UNSG32 baud)
{

    UNSG32 base = APB_UART_INST0_BASE;
	UNSG32 ulDivisor;

	if (channel == 0) base = APB_UART_INST0_BASE;
	else if (channel == 1) base = APB_UART_INST1_BASE;
    //UART FIFO control register:FCR(Enable FIFO mode)
    GA_REG_WORD32_WRITE(base+APB_UART_FCR, (3<<6) | (3<<4) | (1<<2) | (1<<1) | (1<<0));

    //UART modem control register:MCR
    GA_REG_WORD32_WRITE(base+APB_UART_MCR, 0);

    //UART line control register: Normal,No parity,1 stop,8 bits and enable baud divider register
    GA_REG_WORD32_WRITE(base+APB_UART_LCR, 0x3 | BIT7);

    //UART control register
    //GA_REG_WORD32_WRITE(base+OFS_UCON, 0x245);
	
    //UART baud divider register
    ulDivisor = (tclk/baud+8)/16;
    GA_REG_WORD32_WRITE(base+APB_UART_DLL, ulDivisor & 0xff);
	GA_REG_WORD32_WRITE(base+APB_UART_DLH, (ulDivisor >> 8) & 0xff);
}
/*******************************************************************************
* Function:    APB_UART_putc
*
* Description: initialize UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   c-- output character
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void APB_UART_putc(UNSG32 channel, SIGN8 ch)
{
    UNSG32 base = APB_UART_INST0_BASE;
    UNSG32 status, ulReg;

	if (channel == 0) base = APB_UART_INST0_BASE;
	else if (channel == 1) base = APB_UART_INST1_BASE;

    //Enable thr for write? LCR
	GA_REG_WORD32_READ(base+APB_UART_LCR, &ulReg);
    GA_REG_WORD32_WRITE(base+APB_UART_LCR, ulReg & ~(BIT7));
	
    // Wait for Tx FIFO not full(Check THRE bit)
    do
    {
        GA_REG_WORD32_READ(base+APB_UART_LSR, &status);
    }
    while (!(status & BIT5)) ;

    //UART TX data register
    GA_REG_WORD32_WRITE(base+APB_UART_THR, ch);
}

/*******************************************************************************
* Function: APB_UART_tstc
*
* Description: Test if there's input character from UART.
*
* Inputs: channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*
* Outputs: none
*
* Return: 0-- there's no char input
* 		  1-- there's char input 
*******************************************************************************/
UNSG8 APB_UART_tstc(UNSG32 channel)
{
	UNSG32 base = APB_UART_INST0_BASE;
	UNSG32 status;

	if (channel == 0) base = APB_UART_INST0_BASE;
	else if (channel == 1) base = APB_UART_INST1_BASE;
	GA_REG_WORD32_READ(base+APB_UART_LSR, &status);
	if (status & BIT0)
		return 1;
	else
		return 0;
}
/*******************************************************************************
* Function:    APB_UART_getc_nonblock
*
* Description: non blocking mode read charactor from UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   c-- output character
* Outputs:     none
*
* Return:      0-- didn't get any character
*			   1-- get one character
*******************************************************************************/
UNSG8 APB_UART_getc_nonblock(UNSG32 channel, UNSG8 *ch)
{
    UNSG32 base = APB_UART_INST0_BASE;
    UNSG32 status, ulReg;
	UNSG32 data;

	if (channel == 0) base = APB_UART_INST0_BASE;
	else if (channel == 1) base = APB_UART_INST1_BASE;    
	GA_REG_WORD32_READ(base+APB_UART_LSR, &status);
    if (status & BIT0) /* If there's char[s] in RBR or FIFO */
    {
    	//Enable rbr for read
		GA_REG_WORD32_READ(base+APB_UART_LCR, &ulReg);
    	GA_REG_WORD32_WRITE(base+APB_UART_LCR, ulReg & ~(BIT7));
		
        GA_REG_WORD32_READ(base+APB_UART_RBR, &data);
		*ch = data & 0xFF;
        return 1;
    }

    return 0;
}
/*******************************************************************************
* Function:    APB_UART_getc
*
* Description: blocking mode read charactor from UART. Wait until get something
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
* Outputs:     none
*
* Return:      return UIN8 value.
*******************************************************************************/
UNSG8 APB_UART_getc(UNSG32 channel)
{
    UNSG8 ch;

    while(!APB_UART_getc_nonblock(channel, &ch));
	return ch;
}
/*******************************************************************************
* Function:    APB_UART_write
*
* Description: Write the buffer into UART.
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   buf-- pointer point to the string what will be written to UART
*			   len-- length of the string
* Outputs:     none
*
* Return:      none.
*******************************************************************************/
void APB_UART_write(UNSG32 channel, SIGN8 *buf)
{
	SIGN8 ch;
	UNSG32 len;
	len=strlen(buf);
    while(len-- > 0) 
    {
		ch = *buf++;
		APB_UART_putc(channel, ch );
		if (ch=='\n')
		{
			ch='\r';
			APB_UART_putc(channel, ch );		
		}	
    }
        
}

/*******************************************************************************
* Function:    APB_UART_read
*
* Description: read the string from UART.
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   buf-- pointer point to the string what will be written to UART
*			   len-- length of the string
* Outputs:     none
*
* Return:      none.
*******************************************************************************/
void APB_UART_read(UNSG32 channel, SIGN8 *buf, UNSG32 len)
{
    while(len-- > 0)
        *buf++ = APB_UART_getc(channel);
}
/*******************************************************************************
* Function:    APB_UART_printf
*
* Description: printf sytle output.print the output to UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   szFormat-- print format
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void APB_UART_printf(UNSG32 channel, UNSG8* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);
	/*
	 * The uboot has no vsnprintf() implementation itself.
	 * uboot doesn't call APB_UART_printf().
	 */
#if defined(__UBOOT__)
	vsprintf(apb_print_buf, szFormat, args);
#else
	vsnprintf(apb_print_buf, __UART_OUTPUT_BUF_SIZE, szFormat, args);
#endif

	APB_UART_write(channel, apb_print_buf);
	va_end(args);
}

