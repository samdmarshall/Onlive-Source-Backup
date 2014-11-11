#ifndef _PWM_DRIVER_H_
#define _PWM_DRIVER_H_

#include "com_type.h"

typedef struct pwm_settings_t {
	UINT32 En;
	UINT32 PreScale;
	UINT32 Polarity; /* 0/1 Active High/Low */
	UINT32 Duty;
	UINT32 Tcnt;
} PWM_SETTINGS;

int pwm_on_off(int index, PWM_SETTINGS * pwm_settings, int on_off);
int pwm_read_settings(int index, PWM_SETTINGS * pwm_settings);
int pwm_write_settings(int index, PWM_SETTINGS * pwm_settings);
#endif
