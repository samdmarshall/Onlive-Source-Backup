#define _PWM_DRIVER_C_

#include "pwm_driver.h"
#include "galois_io.h"

#define PWM_RegRead(a, pv)	GA_REG_WORD32_READ(base_addr+(a), pv) /*(*(volatile int *)(base_addr+(a)))*/
#define PWM_RegWrite(a, v)	GA_REG_WORD32_WRITE(base_addr+(a), v) /**(volatile int *)(base_addr+(a)) = (v)*/

static const int pwm_reg_offset[4] = {0, 0x10, 0x20, 0x30};

int pwm_on_off(int index, PWM_SETTINGS * pwm_settings, int on_off) {
	int base_addr = MEMMAP_PWM_REG_BASE + pwm_reg_offset[index];

	if(index < 0 || index > 3) {
		return -1;
	}

	on_off = on_off ? 1 : 0;
	PWM_RegWrite(0, on_off);

	//printf("<pwm_on_off> %d -- %d\n", index, on_off);

	pwm_settings->En = on_off;
	return 0;
}

int pwm_read_settings(int index, PWM_SETTINGS * pwm_settings) {
	int base_addr = MEMMAP_PWM_REG_BASE + pwm_reg_offset[index];
	UINT32 t[4];

	if(index < 0 || index > 3) {
		return -1;
	}

	PWM_RegRead(4, &t[1]);
	PWM_RegRead(8, &t[2]);
	PWM_RegRead(12, &t[3]);

	pwm_settings->PreScale = t[1] & 7;
	pwm_settings->Polarity = (t[1] >> 3) & 1;
	pwm_settings->Duty = t[2] & 0xffff;
	pwm_settings->Tcnt = t[3] & 0xffff;

	return 0;
} 

int pwm_write_settings(int index, PWM_SETTINGS * pwm_settings) {
	int base_addr = MEMMAP_PWM_REG_BASE + pwm_reg_offset[index];
	UINT32 t[4];

	if(index < 0 || index > 3) {
		return -1;
	}

	t[1] = (pwm_settings->PreScale & 7) | ((pwm_settings->Polarity & 1) << 3);
	t[2] = pwm_settings->Duty & 0xffff;
	t[3] = pwm_settings->Tcnt & 0xffff;

	PWM_RegWrite(4, t[1]);
	PWM_RegWrite(8, t[2]);
	PWM_RegWrite(12, t[3]);

	return 0;
}

