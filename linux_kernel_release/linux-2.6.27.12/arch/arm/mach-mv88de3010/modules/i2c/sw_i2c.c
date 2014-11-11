#include "Galois_memmap.h"
#include "galois_io.h"
#include "galois_hal_com.h"
#include "pic_drv.h"
#include "global.h"
#include "SysMgr.h"
#include "apb_perf_base.h"
#include "gpio.h"
#include "i2c_master.h"

#include <linux/delay.h>
#include <mach/twsi_bus.h>
#include <mach/galois_generic.h>

#ifdef CONFIG_MV88DE3010_DYNAMIC_FREQ
#include <mach/freq.h>
extern SOC_FREQ_VECTOR_Linux socFreqVec;
#endif
static unsigned int cpu_mhz = 600;

#define WORD_DELAY 5 // 5us

#ifndef MIN
#define MIN(A, B)		((A) < (B) ? (A) : (B))
#endif
#ifndef MAX
#define MAX(A, B)		((A) > (B) ? (A) : (B))
#endif

typedef struct twsidev_bus_timing_T{
	int start_su;	// START setup time, in unit of nano-second
	int start_hd;	// START hold time
	int stop_su;	// STOP setup time
	int stop_hd;	// STOP hold time
	int data_su;	// data setup time
	int data_hd;	// data hold time
	int clk_low;	// clock low period
	int clk_high;	// clock high period
	int timeout;	// timeout value in unit of nano-second
}twsidev_bus_timing_t;

// 100k bus default timing
const twsidev_bus_timing_t twsi_bus_100k_timing={
	4700/*start_su*/, 4000/*start_hd*/, 4000/*stop_su*/, 4700/*stop_hd*/, 250/*data_su*/, 300/*data_hd*/,
	4700/*clk_low*/, 4000/*clk_high*/, 4000000/*timtout*/
};

// 400k bus default timing
const twsidev_bus_timing_t twsi_bus_400k_timing={
	600/*start_su*/, 600/*start_hd*/, 600/*stop_su*/, 1300/*stop_hd*/, 100/*data_su*/, 300/*data_hd*/,
	1300/*clk_low*/, 600/*clk_high*/, 1000000/*timtout*/
};

static inline twsidev_bus_timing_t *twsidev_bus_timing(int speed)
{
	if (speed == TWSI_SPEED_400)
		return (twsidev_bus_timing_t *)&twsi_bus_400k_timing;
	else
		return (twsidev_bus_timing_t *)&twsi_bus_100k_timing;
}

#define TWSI0_SDA_PIN 25
#define TWSI0_SCL_PIN 24
#define TWSI1_SDA_PIN 27
#define TWSI1_SCL_PIN 26
#define TWSI2_SDA_PIN 5  /* SM GPIO */
#define TWSI2_SCL_PIN 4  /* SM GPIO */
#define TWSI3_SDA_PIN 11  /* SM GPIO */
#define TWSI3_SCL_PIN 10  /* SM GPIO */

static void sda_set(int bus) 
{ 
	switch (bus) {
		case 0:
			GPIO_PortSetInOut(TWSI0_SDA_PIN, 1); 
			break;
		case 1:
			GPIO_PortSetInOut(TWSI1_SDA_PIN, 1); 
			break;
		case 2:
			SM_GPIO_PortSetInOut(TWSI2_SDA_PIN, 1); 
			break;
		case 3:
			SM_GPIO_PortSetInOut(TWSI3_SDA_PIN, 1); 
			break;
		default:
			break;
	}
}

static void sda_clr(int bus) 
{
	switch (bus) {
		case 0:
			GPIO_PortWrite(TWSI0_SDA_PIN, 0); 
			break;
		case 1:
			GPIO_PortWrite(TWSI1_SDA_PIN, 0); 
			break;
		case 2:
			SM_GPIO_PortWrite(TWSI2_SDA_PIN, 0); 
			break;
		case 3:
			SM_GPIO_PortWrite(TWSI3_SDA_PIN, 0); 
			break;
		default:
			break;
	}
}

static int sda_rd(int bus) 
{ 
	int value; 
	
	switch (bus) {
		case 0:
			GPIO_PortRead(TWSI0_SDA_PIN, &value); 
			break;
		case 1:
			GPIO_PortRead(TWSI1_SDA_PIN, &value); 
			break;
		case 2:
			SM_GPIO_PortRead(TWSI2_SDA_PIN, &value); 
			break;
		case 3:
			SM_GPIO_PortRead(TWSI3_SDA_PIN, &value); 
			break;
		default:
			break;
	}

	return value; 
}

static void sda_tristate(int bus) 
{
	switch (bus) {
		case 0:
			GPIO_PortSetInOut(TWSI0_SDA_PIN, 1); 
			break;
		case 1:
			GPIO_PortSetInOut(TWSI1_SDA_PIN, 1); 
			break;
		case 2:
			SM_GPIO_PortSetInOut(TWSI2_SDA_PIN, 1); 
			break;
		case 3:
			SM_GPIO_PortSetInOut(TWSI3_SDA_PIN, 1); 
			break;
		default:
			break;
	}
}

static void scl_set(int bus) 
{
	switch (bus) {
		case 0:
			GPIO_PortSetInOut(TWSI0_SCL_PIN, 1); 
			break;
		case 1:
			GPIO_PortSetInOut(TWSI1_SCL_PIN, 1); 
			break;
		case 2:
			SM_GPIO_PortSetInOut(TWSI2_SCL_PIN, 1); 
			break;
		case 3:
			SM_GPIO_PortSetInOut(TWSI3_SCL_PIN, 1); 
			break;
		default:
			break;
	}
}

static void scl_clr(int bus) 
{  
	switch (bus) {
		case 0:
			GPIO_PortWrite(TWSI0_SCL_PIN, 0); 
			break;
		case 1:
			GPIO_PortWrite(TWSI1_SCL_PIN, 0); 
			break;
		case 2:
			SM_GPIO_PortWrite(TWSI2_SCL_PIN, 0); 
			break;
		case 3:
			SM_GPIO_PortWrite(TWSI3_SCL_PIN, 0); 
			break;
		default:
			break;
	}
}

static int scl_rd(int bus) 
{ 
	int value; 
	switch (bus) {
		case 0:
			GPIO_PortRead(TWSI0_SCL_PIN, &value); 
			break;
		case 1:
			GPIO_PortRead(TWSI1_SCL_PIN, &value); 
			break;
		case 2:
			SM_GPIO_PortRead(TWSI2_SCL_PIN, &value); 
			break;
		case 3:
			SM_GPIO_PortRead(TWSI3_SCL_PIN, &value); 
			break;
		default:
			break;
	}
	return value; 
}

/*
 * set interrupt disable/enable
 */
static void Set_PIC_GInt(unsigned int cpu_id, unsigned int enable) 
{
	// enable PIC G enable bits. disable FIQ and enable IRQ now.
	PIC_SetPerPIC_TgtGIntE(cpu_id, INT_DISABLE_BIT, enable);
}

static void local_delay(int ns)
{
    int nop_cycles = cpu_mhz * ns / 32000;

    while (nop_cycles--) {
        __asm__ __volatile__("mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             "mov r0, r0" "\n\t" \
                             );
    }
}

/////////////////////////////////////////////////////////////////
// The following functions are used internally for type-1 bus
// 1. start(int)
// 2. stop(int)
// 3. rack(int)
// 4. wack(int,int)
// 5. rbyte(int,uchar *)
// 6. wbyte(int,uchar)
/////////////////////////////////////////////////////////////////
static void _start(int bus, int speed)
{
	twsidev_bus_timing_t *ptiming = twsidev_bus_timing(speed);

	if (!scl_rd(bus)) {
		if (!sda_rd(bus))
			sda_set(bus); // pull SDA up 

		local_delay(MAX(ptiming->data_su, (ptiming->clk_low-ptiming->data_hd)));
		scl_set(bus); // pull SCL up
	}

	local_delay(ptiming->start_su);
	sda_clr(bus); // pull SDA down

	local_delay(ptiming->start_hd);
	scl_clr(bus); // pull SCL down

	return;
}

static void _stop(int bus, int speed)
{
	twsidev_bus_timing_t *ptiming = twsidev_bus_timing(speed);
	
	if (sda_rd(bus))
		sda_clr(bus); // pull SDA down
	
	local_delay(MAX(ptiming->data_su, (ptiming->clk_low-ptiming->data_hd)));
	scl_set(bus); // pull SCL up

	local_delay(ptiming->stop_su);
	sda_set(bus); // pull SDA up
	
	local_delay(ptiming->stop_hd);

	return;
}

//////////////////////////////////
// return: 0 - ack
//      1 - nack
//////////////////////////////////
static int _rack(int bus, int speed)
{
	twsidev_bus_timing_t *ptiming = twsidev_bus_timing(speed);
	int value;

	sda_tristate(bus); // release SDA for input
	local_delay(MAX(ptiming->data_su, (ptiming->clk_low-ptiming->data_hd)));

	scl_set(bus); // pull SCL up
	value = sda_rd(bus);

	local_delay(ptiming->clk_high);
	scl_clr(bus); // pull SCL down

	local_delay(ptiming->data_hd);

	return (value);
}

//////////////////////////////////
// ack: 0 - ack
//      1 - nack
//////////////////////////////////
static void _wack(int bus, int speed, int ack)
{
	twsidev_bus_timing_t *ptiming = twsidev_bus_timing(speed);

	if(ack) 
		sda_set(bus); // pull SDA up
	else 
		sda_clr(bus); // or pull SDA down

	local_delay(MAX(ptiming->data_su, (ptiming->clk_low-ptiming->data_hd)));
	scl_set(bus); // pull SCL up

	local_delay(ptiming->clk_high);
	scl_clr(bus); // pull SCL down

	local_delay(ptiming->data_hd);

	return;
}

static void _rbyte(int bus, int speed, unsigned char *pvalue)
{
	twsidev_bus_timing_t *ptiming = twsidev_bus_timing(speed);
	unsigned char mask=0x80;
	int value;

	*pvalue = 0;
	while(mask){
		sda_tristate(bus); // release SDA for input

		local_delay(MAX(ptiming->data_su, (ptiming->clk_low-ptiming->data_hd)));
		scl_set(bus); // pull SCL up

		value = sda_rd(bus);

		local_delay(ptiming->clk_high);
		scl_clr(bus); // pull SCL down

	    local_delay(ptiming->data_hd);

	    if( value )
			*pvalue |= mask;
	    mask >>= 1;
	}
	return;
}

static void _wbyte(int bus, int speed, unsigned char value)
{
	twsidev_bus_timing_t *ptiming = twsidev_bus_timing(speed);
	unsigned char mask=0x80;

	while(mask){
		if(value&mask)
			sda_set(bus); // pull SDA up
		else
			sda_clr(bus); // pull SDA down

		local_delay(MAX(ptiming->data_su, (ptiming->clk_low-ptiming->data_hd)));
		scl_set(bus); // pull SCL up

		local_delay(ptiming->clk_high);
		scl_clr(bus); // pull SCL down

		local_delay(ptiming->data_hd);

		mask >>= 1;
	}
	return;
}

/*
 * main function
 * @speed: TWSI_SPEED_100 or TWSI_SPEED_400
 * @slave_addr: TWSI slave device's address
 * @addr_type: TWSI_7BIT_SLAVE_ADDR or TWSI_10BIT_SLAVE_ADDR
 */
int bus_writeread_bytes(int bus, int speed, int slave_addr, int addr_type, unsigned char *pwbuf, int wnum, unsigned char *prbuf, int rnum)
{
	int i;
	unsigned char cmd;
	int ret = I2C_OK;

		/* setup pinmux */
		set_sw_twsi_pinmux(bus);

		/* disable CPU-0 interrupts */
		Set_PIC_GInt(0/*CPU-0*/, 0/*disable*/);

		// START condition
		_start(bus, speed);

		// write slave address
		if (addr_type==TWSI_7BIT_SLAVE_ADDR){ // 7-bit slave address
			// slave address byte
			cmd = (unsigned char)(slave_addr<<1);
			
			// if there is something to write
			if (pwbuf){
				// write command
				_wbyte(bus, speed, cmd);
				// Ack
				if(_rack(bus, speed)){ 
					ret = I2C_EBUSRW; 
					goto DONE; 
				}
				/* enable CPU-0 interrupts */
				Set_PIC_GInt(0/*CPU-0*/, 1/*enable*/);

				udelay(WORD_DELAY);

				// write data
				for (i=0; i<wnum; i++){
					/* disable CPU-0 interrupts */
					Set_PIC_GInt(0/*CPU-0*/, 0/*disable*/);
					
					_wbyte(bus, speed,pwbuf[i]);
					if(_rack(bus, speed)){ 
						ret = I2C_EBUSRW; 
						goto DONE; 
					}
					/* enable CPU-0 interrupts */
					Set_PIC_GInt(0/*CPU-0*/, 1/*enable*/);
  
					udelay(WORD_DELAY);	
				}
			}
			
			// if there is something to read
			if (prbuf){
				/* disable CPU-0 interrupts */
				Set_PIC_GInt(0/*CPU-0*/, 0/*disable*/);

				// if this is a write & read combined transaction
				if (pwbuf){
					// repeat START
					_start(bus, speed);
				}
				// read command
				_wbyte(bus, speed, cmd|0x01);
				// Ack
				if(_rack(bus, speed)){ 
					ret = I2C_EBUSRW; 
					goto DONE; 
				}

				/* enable CPU-0 interrupts */
				Set_PIC_GInt(0/*CPU-0*/, 1/*enable*/);

				udelay(WORD_DELAY);

				// read data
				for (i=0; i<rnum; i++){
					/* disable CPU-0 interrupts */
					Set_PIC_GInt(0/*CPU-0*/, 0/*disable*/);

					_rbyte(bus, speed,prbuf+i);
					if( i!=rnum-1 ) 
						_wack(bus, speed,0); // Ack
					else 
						_wack(bus, speed,1); // Nack

					/* enable CPU-0 interrupts */
					Set_PIC_GInt(0/*CPU-0*/, 1/*enable*/);

					udelay(WORD_DELAY);
				}
			}
		} else { // 10-bit slave address
			// slave address 1st byte
			cmd = (unsigned char)(slave_addr>>7);
			cmd &= 0x06;
			cmd |= 0xf0;
			_wbyte(bus, speed, cmd);
			// Ack
			if( _rack(bus, speed) ){ 
				ret = I2C_EBUSRW; 
				goto DONE; 
			}
			
			// slave address 2nd byte
			_wbyte(bus, speed, (unsigned char)(slave_addr));
			// Ack
			if( _rack(bus, speed) ){ 
				ret = I2C_EBUSRW; 
				goto DONE; 
			}
			
			/* enable CPU-0 interrupts */
			Set_PIC_GInt(0/*CPU-0*/, 1/*enable*/);

			udelay(WORD_DELAY);

			// if there is something to write
			if (pwbuf){
				// write data
				for (i=0; i<wnum; i++){
					/* disable CPU-0 interrupts */
					Set_PIC_GInt(0/*CPU-0*/, 0/*disable*/);

					_wbyte(bus, speed,pwbuf[i]);
					if( _rack(bus, speed) ){ 
						ret = I2C_EBUSRW; 
						goto DONE; 
					}

					/* enable CPU-0 interrupts */
					Set_PIC_GInt(0/*CPU-0*/, 1/*enable*/);
 
					udelay(WORD_DELAY);
				}
			}
			
			// if there is something to read
			if (prbuf){
				/* disable CPU-0 interrupts */
				Set_PIC_GInt(0/*CPU-0*/, 0/*disable*/);

				// repeat START
				_start(bus, speed);
				// read command
				_wbyte(bus, speed, cmd|0x01);
				// Ack
				if( _rack(bus, speed) ){ 
					ret = I2C_EBUSRW; 
					goto DONE; 
				}

				/* enable CPU-0 interrupts */
				Set_PIC_GInt(0/*CPU-0*/, 1/*enable*/);
 
				udelay(WORD_DELAY);

				// read data
				for (i=0; i<rnum; i++){
					/* disable CPU-0 interrupts */
					Set_PIC_GInt(0/*CPU-0*/, 0/*disable*/);

					_rbyte(bus, speed,prbuf+i);
					if( i!=rnum-1 ) 
						_wack(bus, speed,0); // Ack
					else 
						_wack(bus, speed,1); // Nack

					/* enable CPU-0 interrupts */
					Set_PIC_GInt(0/*CPU-0*/, 1/*enable*/);
  
					udelay(WORD_DELAY);	
				}
			}
		}
DONE:
		/* disable CPU-0 interrupts */
		Set_PIC_GInt(0/*CPU-0*/, 0/*disable*/);

		// STOP condition
		_stop(bus, speed);

		/* enable CPU-0 interrupts */
		Set_PIC_GInt(0/*CPU-0*/, 1/*enable*/);

		return (ret);
}

void init_sw_twsi(void)
{
#ifdef CONFIG_MV88DE3010_DYNAMIC_FREQ
	cpu_mhz = (socFreqVec.cpu0/1000000);
#else
	cpu_mhz = (GALOIS_CPU_CLK/1000000);
#endif
	printk("s/w TWSI used, cpu clock: %u\n", cpu_mhz);
	return;
}
