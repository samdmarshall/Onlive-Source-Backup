/*******************************************************************************
*******************************************************************************/

#ifndef __GALOIS_PERFORMANCE_COUNTER_H__
#define __GALOIS_PERFORMANCE_COUNTER_H__
void ResetCycleCounter();

void EnableCycleCounter(int on_off);

void ReadCycleCounter(int *hi, int *lo);

void ResetDCacheReadMiss();

void EnableDCacheReadMiss(int on_off);

void ReadDCacheReadMiss(int *lo, int *hi);

void ResetDCacheWriteMiss();

void EnableDCacheWriteMiss(int on_off);

void ReadDCacheWriteMiss(int *lo, int *hi);

void ResetICacheReadMiss();

void EnableICacheReadMiss(int on_off);

void ReadICacheReadMiss(int *lo, int *hi);

void StartPerformanceMeasure(void);

void StopPerformanceMeasure(unsigned int *D_read_hi, unsigned int *D_read_lo,
                            unsigned int *D_write_hi, unsigned int *D_write_lo,
                            unsigned int *I_read_hi, unsigned int *I_read_lo,
                            unsigned int *cycle_hi, unsigned int *cycle_lo);

#endif
