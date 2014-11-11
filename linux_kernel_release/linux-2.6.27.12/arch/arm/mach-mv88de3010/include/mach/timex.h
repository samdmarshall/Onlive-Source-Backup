
/*
 * On FPGA Galois SoC, the cpu-core clock is 9MHz.
 * the system clock is 6MHz.
 * #define CLOCK_TICK_RATE		9000000
 */

#include <mach/galois_platform.h>
#define CLOCK_TICK_RATE (GALOIS_TIMER_CLK)

extern struct sys_timer mv88de3010_timer;

