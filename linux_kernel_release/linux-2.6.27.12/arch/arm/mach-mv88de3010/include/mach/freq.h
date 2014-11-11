#ifndef _FREQ_H_
#define _FREQ_H_

//#include "ctype.h"

typedef struct SOC_FREQ_VECTOR_T_LINUX{
    unsigned long ddr;
    unsigned long cpu0;
    unsigned long cpu1;
    unsigned long syspll;
    unsigned long sys;
    unsigned long vscope;
    unsigned long pcube;
    unsigned long vpp;
    unsigned long cfg;
    unsigned long adsp;
    unsigned long perif;
    unsigned long gfx;
    unsigned long drm;
    unsigned long nfcecc;
    unsigned long app;
}SOC_FREQ_VECTOR_Linux;

void query_board_freq(SOC_FREQ_VECTOR_Linux * freq);

#endif

