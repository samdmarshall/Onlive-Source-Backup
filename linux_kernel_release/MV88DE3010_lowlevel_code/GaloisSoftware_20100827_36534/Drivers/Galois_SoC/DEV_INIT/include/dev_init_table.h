#include "dev_init_def.h"
#ifdef  MAIN_DEV_INIT_TBL
#include "pic.h"
#include "apb_ictl.h"
#include "pic_cpu_intr.h"
#include "GaloisTime.h"
#if defined(BERLIN)
DEV_INIT_T  dev_init_tbl[] = {
   // PIC to CPU0, CPU1, CPU2
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_dHubIntrAvio0,              DEV_TYPE_DHUB_AVIO0,    GRP_DEV_VEC_ENABLE,    0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_dHubIntrAvio0,              DEV_TYPE_DHUB_AVIO0,    GRP_DEV_VEC_DISABLE, 0   },
#else
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_dHubIntrAvio0,              DEV_TYPE_DHUB_AVIO0,    GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_dHubIntrAvio0,              DEV_TYPE_DHUB_AVIO0,    GRP_DEV_VEC_ENABLE, 0   },
#endif
   {GRP_PIC_CPU2,   G_DEV_ID0,  IRQ_dHubIntrAvio0,              DEV_TYPE_DHUB_AVIO0,    GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_dHubIntrAvio1,              DEV_TYPE_DHUB_AVIO1,    GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_dHubIntrAvio1,              DEV_TYPE_DHUB_AVIO1,    GRP_DEV_VEC_ENABLE, 0   },
   {GRP_PIC_CPU2,   G_DEV_ID0,  IRQ_dHubIntrAvio1,              DEV_TYPE_DHUB_AVIO1,    GRP_DEV_VEC_DISABLE,    0   },

   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_dHubIntrVpro,               DEV_TYPE_DMA_VPRO,  GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_dHubIntrVpro,               DEV_TYPE_DMA_VPRO,  GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU2,   G_DEV_ID0,  IRQ_dHubIntrVpro,               DEV_TYPE_DMA_VPRO,  GRP_DEV_VEC_DISABLE,    0   },

   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_ictlInstCPUIrq,             DEV_TYPE_ICTL,      GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_ictlInstCPUIrq,             DEV_TYPE_ICTL,      GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU2,   G_DEV_ID0,  IRQ_ictlInstCPUIrq,             DEV_TYPE_ICTL,      GRP_DEV_VEC_DISABLE,    0   },


#if (BERLIN_CHIP_VERSION < BERLIN_BG2)
   {GRP_PIC_CPU2,   G_DEV_ID0,  IRQ_pex2intr_int,               DEV_TYPE_PEX_INT,   GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU2,   G_DEV_ID0,  IRQ_pex2intr_err,               DEV_TYPE_PEX_ERR,   GRP_DEV_VEC_DISABLE,    0   },
#endif

   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_intrGfxM0,                  DEV_TYPE_GFX,       GRP_DEV_VEC_ENABLE,     0   },

   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_SATAintr,                   DEV_TYPE_SATA,      GRP_DEV_VEC_ENABLE, 0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_ethIrq,                     DEV_TYPE_ETH,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_drmIntr,                    DEV_TYPE_DRM,       GRP_DEV_VEC_DISABLE,    0   },

   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_drmFigoIntr,                DEV_TYPE_DRM_FIGO,  GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_usb0Interrupt,              DEV_TYPE_USB0,      GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_usb1Interrupt,              DEV_TYPE_USB1,      GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_sm2socInt,                  DEV_TYPE_SM2SOC,    GRP_DEV_VEC_ENABLE,    0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_sm2socInt,                  DEV_TYPE_SM2SOC,    GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_ptpIrq,                     DEV_TYPE_PTP,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_ptpIrq,                     DEV_TYPE_PTP,       GRP_DEV_VEC_DISABLE,    0   },

   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_inter_PCIEtoCPU0,           DEV_TYPE_INTER_HOST_H,  GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_inter_CPU1toCPU0,           DEV_TYPE_INTER_HOST_L,  GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_inter_PCIEtoCPU1,           DEV_TYPE_INTER_HOST_H,  GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_inter_CPU0toCPU1,           DEV_TYPE_INTER_HOST_L,  GRP_DEV_VEC_ENABLE,     0   },

   // ICTL
#if defined(ENABLE_GPIO_IR)
   {GRP_ICTL_DEV0,  G_DEV_ID0,  LSbGpioInst0,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_ENABLE,     0   },
#elif defined(SPDIF_RX)
   {GRP_ICTL_DEV1,  G_DEV_ID0,  LSbGpioInst0,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_ENABLE,    0   },   
#else
   {GRP_ICTL_DEV0,  G_DEV_ID0,  LSbGpioInst0,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_DISABLE,    0   },
#endif
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbGpioInst1,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV0,  G_DEV_ID2,  LSbGpioInst2,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV0,  G_DEV_ID3,  LSbGpioInst3,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_DISABLE,    0   },

   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbSsiInst1,                    DEV_TYPE_SSI,       GRP_DEV_VEC_DISABLE,    0   },

   {GRP_ICTL_DEV1,  G_DEV_ID0,  LSbWdtInst0,                    DEV_TYPE_WDT,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbWdtInst1,                    DEV_TYPE_WDT,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID2,  LSbWdtInst2,                    DEV_TYPE_WDT,       GRP_DEV_VEC_DISABLE,    0   },
#ifdef __ECOS__
   {GRP_ICTL_DEV0,  G_DEV_ID0,  LSbTimerInst1_0,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE,     CYGNUM_HAL_RTC_PERIOD},
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbTimerInst1_1,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE,     CYGNUM_HAL_RTC_PERIOD},
#else
   {GRP_ICTL_DEV0,  G_DEV_ID0,  LSbTimerInst1_0,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE, 0},
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbTimerInst1_1,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE, 0},
#endif
   {GRP_ICTL_DEV1,  G_DEV_ID2,  LSbTimerInst1_2,                DEV_TYPE_TIMER,     GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID3,  LSbTimerInst1_3,                DEV_TYPE_TIMER,     GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID4,  LSbTimerInst1_4,                DEV_TYPE_TIMER,     GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID5,  LSbTimerInst1_5,                DEV_TYPE_TIMER,     GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID6,  LSbTimerInst1_6,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE,         SYSTEM_TIMER_VALUE},
   {GRP_ICTL_DEV0,  G_DEV_ID7,  LSbTimerInst1_7,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE_ONLY,    DEBUG_TIMER_VALUE},
   {GRP_ICTL_DEV1,  G_DEV_ID7,  LSbTimerInst1_7,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE_ONLY,    DEBUG_TIMER_VALUE},

#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
   {GRP_ICTL_DEV0,  G_DEV_ID0,  LSbI2cInst0,                    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },

   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_gen_call_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_rx_under_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_rx_over_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_rx_full_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_tx_over_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_tx_empty_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_rd_req_intr,     DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_tx_abrt_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_rx_done_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_activity_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_stop_det_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV0,  G_DEV_ID1,  LSbI2cInst1_ic_start_det_intr,  DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
#else
   {GRP_ICTL_DEV1,  G_DEV_ID0,  LSbI2cInst0,                    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },

   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_gen_call_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_rx_under_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_rx_over_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_rx_full_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_tx_over_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_tx_empty_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_rd_req_intr,     DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_tx_abrt_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_rx_done_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_activity_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_stop_det_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_start_det_intr,  DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
#endif

   // DMA

   // VOP

   // GFX

   // SATA

   // ETHERNET

   // DRM

   // I2S

   // PCIE
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
   // Audio DSP
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_zspInt,              DEV_TYPE_ADSP,    GRP_DEV_VEC_ENABLE,    0   },
#endif

};
#else
DEV_INIT_T  dev_init_tbl[] = {
   // PIC to CPU0, CPU1, CPU2
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_dmaIntr0Avio,               DEV_TYPE_DMA_AVIO,  GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU1,   G_DEV_ID1,  IRQ_dmaIntr1Avio,               DEV_TYPE_DMA_AVIO,  GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU2,   G_DEV_ID2,  IRQ_dmaIntr2Avio,               DEV_TYPE_DMA_AVIO,  GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_vopInt,                     DEV_TYPE_VOP,       GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_semIntr0Vpro,               DEV_TYPE_SEM_VPRO,  GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU1,   G_DEV_ID1,  IRQ_semIntr1Vpro,               DEV_TYPE_SEM_VPRO,  GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU2,   G_DEV_ID2,  IRQ_semIntr2Vpro,               DEV_TYPE_SEM_VPRO,  GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_dmaIntr0Vpro,               DEV_TYPE_DMA_VPRO,  GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU1,   G_DEV_ID1,  IRQ_dmaIntr1Vpro,               DEV_TYPE_DMA_VPRO,  GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU2,   G_DEV_ID2,  IRQ_dmaIntr2Vpro,               DEV_TYPE_DMA_VPRO,  GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_ictlInst0CPUIrq,            DEV_TYPE_ICTL,      GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU1,   G_DEV_ID1,  IRQ_ictlInst1CPUIrq,            DEV_TYPE_ICTL,      GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU2,   G_DEV_ID2,  IRQ_ictlInst2CPUIrq,            DEV_TYPE_ICTL,      GRP_DEV_VEC_DISABLE,    0   },
#if (BERLIN_CHIP_VERSION < BERLIN_BG2)
   {GRP_PIC_CPU2,   G_DEV_ID0,  IRQ_pex2intr_int,               DEV_TYPE_PEX_INT,   GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU2,   G_DEV_ID0,  IRQ_pex2intr_err,               DEV_TYPE_PEX_ERR,   GRP_DEV_VEC_DISABLE,    0   },
#endif
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_intrGfxM0,                  DEV_TYPE_GFX,       GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_SATAintr,                   DEV_TYPE_SATA,      GRP_DEV_VEC_ENABLE, 0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_ethIrq,                     DEV_TYPE_ETH,       GRP_DEV_VEC_ENABLE, 0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_drmIntr,                    DEV_TYPE_DRM,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_i2sInt,                     DEV_TYPE_I2S,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_inter_PCIEtoCPU0,           DEV_TYPE_INTER_HOST_H,  GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU0,   G_DEV_ID0,  IRQ_inter_CPU1toCPU0,           DEV_TYPE_INTER_HOST_L,  GRP_DEV_VEC_ENABLE,     0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_inter_PCIEtoCPU1,           DEV_TYPE_INTER_HOST_H,  GRP_DEV_VEC_DISABLE,    0   },
   {GRP_PIC_CPU1,   G_DEV_ID0,  IRQ_inter_CPU0toCPU1,           DEV_TYPE_INTER_HOST_L,  GRP_DEV_VEC_ENABLE,     0   },

   // ICTL
   {GRP_ICTL_DEV1,  G_DEV_ID2,  LSbI2cInst2,                    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE,     0   },
#ifdef ENABLE_GPIO_IR   /* GPIO[0] for FPUI.IR.RxD, to CPU#1 */
   {GRP_ICTL_DEV1,  G_DEV_ID0,  LSbGpioInst0,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_ENABLE,     0   },
#else
   {GRP_ICTL_DEV1,  G_DEV_ID0,  LSbGpioInst0,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_DISABLE,    0   },
#endif
   {GRP_ICTL_DEV1,  G_DEV_ID0,  LSbUartInst0,                   DEV_TYPE_UART,      GRP_DEV_VEC_ENABLE,     0   },
   {GRP_ICTL_DEV1,  G_DEV_ID0,  LSbI2cInst0,                    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE,     0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbUartInst1,                   DEV_TYPE_UART,      GRP_DEV_VEC_ENABLE,     0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbSsiInst1,                    DEV_TYPE_SSI,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID0,  LSbSsiInst0,                    DEV_TYPE_SSI,       GRP_DEV_VEC_ENABLE,     0   },
   {GRP_ICTL_DEV1,  G_DEV_ID2,  LSbSsiInst2,                    DEV_TYPE_SSI,       GRP_DEV_VEC_ENABLE,     0   },
   {GRP_ICTL_DEV1,  G_DEV_ID0,  LSbWdtInst0,                    DEV_TYPE_WDT,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbWdtInst1,                    DEV_TYPE_WDT,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID2,  LSbWdtInst2,                    DEV_TYPE_WDT,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_gen_call_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_rx_under_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_rx_over_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_rx_full_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_tx_over_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_tx_empty_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_rd_req_intr,     DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_tx_abrt_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_rx_done_intr,    DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_activity_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_stop_det_intr,   DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbI2cInst1_ic_start_det_intr,  DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
#ifdef __ECOS__
   {GRP_ICTL_DEV0,  G_DEV_ID0,  LSbTimerInst1_0,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE,     CYGNUM_HAL_RTC_PERIOD},
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbTimerInst1_1,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE,     CYGNUM_HAL_RTC_PERIOD},
#else
   {GRP_ICTL_DEV0,  G_DEV_ID0,  LSbTimerInst1_0,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE, 0},
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbTimerInst1_1,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE, 0},
#endif
   {GRP_ICTL_DEV1,  G_DEV_ID2,  LSbTimerInst1_2,                DEV_TYPE_TIMER,     GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID3,  LSbTimerInst1_3,                DEV_TYPE_TIMER,     GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID4,  LSbTimerInst1_4,                DEV_TYPE_TIMER,     GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID5,  LSbTimerInst1_5,                DEV_TYPE_TIMER,     GRP_DEV_VEC_DISABLE,    0   },
   {GRP_ICTL_DEV1,  G_DEV_ID6,  LSbTimerInst1_6,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE,         SYSTEM_TIMER_VALUE},
   {GRP_ICTL_DEV0,  G_DEV_ID7,  LSbTimerInst1_7,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE_ONLY,    DEBUG_TIMER_VALUE},
   {GRP_ICTL_DEV1,  G_DEV_ID7,  LSbTimerInst1_7,    DEV_TYPE_TIMER,     GRP_DEV_VEC_ENABLE_ONLY,    DEBUG_TIMER_VALUE},
#ifdef I2C_3_ENABLE
   {GRP_ICTL_DEV0,  G_DEV_ID3,  LSbI2cInst3,                    DEV_TYPE_I2C,       GRP_DEV_VEC_ENABLE, 0   },
#else
   {GRP_ICTL_DEV1,  G_DEV_ID3,  LSbI2cInst3,                    DEV_TYPE_I2C,       GRP_DEV_VEC_DISABLE,    0   },
#endif
   {GRP_ICTL_DEV1,  G_DEV_ID1,  LSbGpioInst1,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_DISABLE,    0   },
#ifdef ENABLE_GPIO_IR   /* GPIO[16] for IR.RxD, to CPU#1 */
   {GRP_ICTL_DEV1,  G_DEV_ID2,  LSbGpioInst2,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_ENABLE,     0   },
#else
   {GRP_ICTL_DEV1,  G_DEV_ID2,  LSbGpioInst2,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_DISABLE,    0   },
#endif
   {GRP_ICTL_DEV1,  G_DEV_ID3,  LSbGpioInst3,                   DEV_TYPE_GPIO,      GRP_DEV_VEC_DISABLE,    0   },

   // DMA

   // VOP

   // GFX

   // SATA

   // ETHERNET

   // DRM

   // I2S

   // PCIE
};
#endif
#else
extern DEV_INIT_T  dev_init_tbl[];
#endif
