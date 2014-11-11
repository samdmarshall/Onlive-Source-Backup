#ifndef SysMgr_h
#define SysMgr_h (){}


#include "ctypes.h"

#pragma pack(1)
#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _DOCC_H_BITOPS_
#define _DOCC_H_BITOPS_ (){}

    #define _bSETMASK_(b)                                      ((b)<32 ? (1<<((b)&31)) : 0)
    #define _NSETMASK_(msb,lsb)                                (_bSETMASK_((msb)+1)-_bSETMASK_(lsb))
    #define _bCLRMASK_(b)                                      (~_bSETMASK_(b))
    #define _NCLRMASK_(msb,lsb)                                (~_NSETMASK_(msb,lsb))
    #define _BFGET_(r,msb,lsb)                                 (_NSETMASK_((msb)-(lsb),0)&((r)>>(lsb)))
    #define _BFSET_(r,msb,lsb,v)                               do{ (r)&=_NCLRMASK_(msb,lsb); (r)|=_NSETMASK_(msb,lsb)&((v)<<(lsb)); }while(0)

#endif



#ifndef h_smSysCtl
#define h_smSysCtl (){}

    #define     RA_smSysCtl_SM_ID                              0x0000

    #define     BA_smSysCtl_SM_ID_REV_ID                       0x0000
    #define     B16smSysCtl_SM_ID_REV_ID                       0x0000
    #define   LSb32smSysCtl_SM_ID_REV_ID                          0
    #define   LSb16smSysCtl_SM_ID_REV_ID                          0
    #define       bsmSysCtl_SM_ID_REV_ID                       4
    #define   MSK32smSysCtl_SM_ID_REV_ID                          0x0000000F

    #define     BA_smSysCtl_SM_ID_PART_ID                      0x0000
    #define     B16smSysCtl_SM_ID_PART_ID                      0x0000
    #define   LSb32smSysCtl_SM_ID_PART_ID                         4
    #define   LSb16smSysCtl_SM_ID_PART_ID                         4
    #define       bsmSysCtl_SM_ID_PART_ID                      16
    #define   MSK32smSysCtl_SM_ID_PART_ID                         0x000FFFF0

    #define     BA_smSysCtl_SM_ID_MFC_ID                       0x0002
    #define     B16smSysCtl_SM_ID_MFC_ID                       0x0002
    #define   LSb32smSysCtl_SM_ID_MFC_ID                          20
    #define   LSb16smSysCtl_SM_ID_MFC_ID                          4
    #define       bsmSysCtl_SM_ID_MFC_ID                       12
    #define   MSK32smSysCtl_SM_ID_MFC_ID                          0xFFF00000
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_CPU_CTRL                        0x0004

    #define     BA_smSysCtl_SM_CPU_CTRL_CPU_RST_GO             0x0004
    #define     B16smSysCtl_SM_CPU_CTRL_CPU_RST_GO             0x0004
    #define   LSb32smSysCtl_SM_CPU_CTRL_CPU_RST_GO                0
    #define   LSb16smSysCtl_SM_CPU_CTRL_CPU_RST_GO                0
    #define       bsmSysCtl_SM_CPU_CTRL_CPU_RST_GO             1
    #define   MSK32smSysCtl_SM_CPU_CTRL_CPU_RST_GO                0x00000001

    #define     BA_smSysCtl_SM_CPU_CTRL_CPU_VINITHI            0x0004
    #define     B16smSysCtl_SM_CPU_CTRL_CPU_VINITHI            0x0004
    #define   LSb32smSysCtl_SM_CPU_CTRL_CPU_VINITHI               1
    #define   LSb16smSysCtl_SM_CPU_CTRL_CPU_VINITHI               1
    #define       bsmSysCtl_SM_CPU_CTRL_CPU_VINITHI            1
    #define   MSK32smSysCtl_SM_CPU_CTRL_CPU_VINITHI               0x00000002

    #define     BA_smSysCtl_SM_CPU_CTRL_CPU_INITRAM            0x0004
    #define     B16smSysCtl_SM_CPU_CTRL_CPU_INITRAM            0x0004
    #define   LSb32smSysCtl_SM_CPU_CTRL_CPU_INITRAM               2
    #define   LSb16smSysCtl_SM_CPU_CTRL_CPU_INITRAM               2
    #define       bsmSysCtl_SM_CPU_CTRL_CPU_INITRAM            1
    #define   MSK32smSysCtl_SM_CPU_CTRL_CPU_INITRAM               0x00000004

    #define     BA_smSysCtl_SM_CPU_CTRL_CPU_BIGEND             0x0004
    #define     B16smSysCtl_SM_CPU_CTRL_CPU_BIGEND             0x0004
    #define   LSb32smSysCtl_SM_CPU_CTRL_CPU_BIGEND                3
    #define   LSb16smSysCtl_SM_CPU_CTRL_CPU_BIGEND                3
    #define       bsmSysCtl_SM_CPU_CTRL_CPU_BIGEND             1
    #define   MSK32smSysCtl_SM_CPU_CTRL_CPU_BIGEND                0x00000008
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_RST_CTRL                        0x0008

    #define     BA_smSysCtl_SM_RST_CTRL_SOC_RST_GO             0x0008
    #define     B16smSysCtl_SM_RST_CTRL_SOC_RST_GO             0x0008
    #define   LSb32smSysCtl_SM_RST_CTRL_SOC_RST_GO                0
    #define   LSb16smSysCtl_SM_RST_CTRL_SOC_RST_GO                0
    #define       bsmSysCtl_SM_RST_CTRL_SOC_RST_GO             1
    #define   MSK32smSysCtl_SM_RST_CTRL_SOC_RST_GO                0x00000001

    #define     BA_smSysCtl_SM_RST_CTRL_APB_RST_GO             0x0008
    #define     B16smSysCtl_SM_RST_CTRL_APB_RST_GO             0x0008
    #define   LSb32smSysCtl_SM_RST_CTRL_APB_RST_GO                1
    #define   LSb16smSysCtl_SM_RST_CTRL_APB_RST_GO                1
    #define       bsmSysCtl_SM_RST_CTRL_APB_RST_GO             1
    #define   MSK32smSysCtl_SM_RST_CTRL_APB_RST_GO                0x00000002

    #define     BA_smSysCtl_SM_RST_CTRL_SXBAR_RST_GO           0x0008
    #define     B16smSysCtl_SM_RST_CTRL_SXBAR_RST_GO           0x0008
    #define   LSb32smSysCtl_SM_RST_CTRL_SXBAR_RST_GO              2
    #define   LSb16smSysCtl_SM_RST_CTRL_SXBAR_RST_GO              2
    #define       bsmSysCtl_SM_RST_CTRL_SXBAR_RST_GO           1
    #define   MSK32smSysCtl_SM_RST_CTRL_SXBAR_RST_GO              0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_RST_STATUS                      0x000C

    #define     BA_smSysCtl_SM_RST_STATUS_RST_WD_0             0x000C
    #define     B16smSysCtl_SM_RST_STATUS_RST_WD_0             0x000C
    #define   LSb32smSysCtl_SM_RST_STATUS_RST_WD_0                0
    #define   LSb16smSysCtl_SM_RST_STATUS_RST_WD_0                0
    #define       bsmSysCtl_SM_RST_STATUS_RST_WD_0             1
    #define   MSK32smSysCtl_SM_RST_STATUS_RST_WD_0                0x00000001

    #define     BA_smSysCtl_SM_RST_STATUS_RST_WD_1             0x000C
    #define     B16smSysCtl_SM_RST_STATUS_RST_WD_1             0x000C
    #define   LSb32smSysCtl_SM_RST_STATUS_RST_WD_1                1
    #define   LSb16smSysCtl_SM_RST_STATUS_RST_WD_1                1
    #define       bsmSysCtl_SM_RST_STATUS_RST_WD_1             1
    #define   MSK32smSysCtl_SM_RST_STATUS_RST_WD_1                0x00000002

    #define     BA_smSysCtl_SM_RST_STATUS_RST_WD_2             0x000C
    #define     B16smSysCtl_SM_RST_STATUS_RST_WD_2             0x000C
    #define   LSb32smSysCtl_SM_RST_STATUS_RST_WD_2                2
    #define   LSb16smSysCtl_SM_RST_STATUS_RST_WD_2                2
    #define       bsmSysCtl_SM_RST_STATUS_RST_WD_2             1
    #define   MSK32smSysCtl_SM_RST_STATUS_RST_WD_2                0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_STRP_STATUS                     0x0010

    #define     BA_smSysCtl_SM_STRP_STATUS_BOOT_MODE           0x0010
    #define     B16smSysCtl_SM_STRP_STATUS_BOOT_MODE           0x0010
    #define   LSb32smSysCtl_SM_STRP_STATUS_BOOT_MODE              0
    #define   LSb16smSysCtl_SM_STRP_STATUS_BOOT_MODE              0
    #define       bsmSysCtl_SM_STRP_STATUS_BOOT_MODE           2
    #define   MSK32smSysCtl_SM_STRP_STATUS_BOOT_MODE              0x00000003

    #define     BA_smSysCtl_SM_STRP_STATUS_STRP_2              0x0010
    #define     B16smSysCtl_SM_STRP_STATUS_STRP_2              0x0010
    #define   LSb32smSysCtl_SM_STRP_STATUS_STRP_2                 2
    #define   LSb16smSysCtl_SM_STRP_STATUS_STRP_2                 2
    #define       bsmSysCtl_SM_STRP_STATUS_STRP_2              1
    #define   MSK32smSysCtl_SM_STRP_STATUS_STRP_2                 0x00000004

    #define     BA_smSysCtl_SM_STRP_STATUS_STRP_3              0x0010
    #define     B16smSysCtl_SM_STRP_STATUS_STRP_3              0x0010
    #define   LSb32smSysCtl_SM_STRP_STATUS_STRP_3                 3
    #define   LSb16smSysCtl_SM_STRP_STATUS_STRP_3                 3
    #define       bsmSysCtl_SM_STRP_STATUS_STRP_3              1
    #define   MSK32smSysCtl_SM_STRP_STATUS_STRP_3                 0x00000008
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_CTRL                            0x0014

    #define     BA_smSysCtl_SM_CTRL_ISO_EN                     0x0014
    #define     B16smSysCtl_SM_CTRL_ISO_EN                     0x0014
    #define   LSb32smSysCtl_SM_CTRL_ISO_EN                        0
    #define   LSb16smSysCtl_SM_CTRL_ISO_EN                        0
    #define       bsmSysCtl_SM_CTRL_ISO_EN                     1
    #define   MSK32smSysCtl_SM_CTRL_ISO_EN                        0x00000001

    #define     BA_smSysCtl_SM_CTRL_SM2SOC_SW_INTR             0x0014
    #define     B16smSysCtl_SM_CTRL_SM2SOC_SW_INTR             0x0014
    #define   LSb32smSysCtl_SM_CTRL_SM2SOC_SW_INTR                1
    #define   LSb16smSysCtl_SM_CTRL_SM2SOC_SW_INTR                1
    #define       bsmSysCtl_SM_CTRL_SM2SOC_SW_INTR             1
    #define   MSK32smSysCtl_SM_CTRL_SM2SOC_SW_INTR                0x00000002

    #define     BA_smSysCtl_SM_CTRL_SOC2SM_SW_INTR             0x0014
    #define     B16smSysCtl_SM_CTRL_SOC2SM_SW_INTR             0x0014
    #define   LSb32smSysCtl_SM_CTRL_SOC2SM_SW_INTR                2
    #define   LSb16smSysCtl_SM_CTRL_SOC2SM_SW_INTR                2
    #define       bsmSysCtl_SM_CTRL_SOC2SM_SW_INTR             1
    #define   MSK32smSysCtl_SM_CTRL_SOC2SM_SW_INTR                0x00000004

    #define     BA_smSysCtl_SM_CTRL_REV_0                      0x0014
    #define     B16smSysCtl_SM_CTRL_REV_0                      0x0014
    #define   LSb32smSysCtl_SM_CTRL_REV_0                         3
    #define   LSb16smSysCtl_SM_CTRL_REV_0                         3
    #define       bsmSysCtl_SM_CTRL_REV_0                      2
    #define   MSK32smSysCtl_SM_CTRL_REV_0                         0x00000018

    #define     BA_smSysCtl_SM_CTRL_ADC_SEL                    0x0014
    #define     B16smSysCtl_SM_CTRL_ADC_SEL                    0x0014
    #define   LSb32smSysCtl_SM_CTRL_ADC_SEL                       5
    #define   LSb16smSysCtl_SM_CTRL_ADC_SEL                       5
    #define       bsmSysCtl_SM_CTRL_ADC_SEL                    3
    #define   MSK32smSysCtl_SM_CTRL_ADC_SEL                       0x000000E0

    #define     BA_smSysCtl_SM_CTRL_ADC_PU                     0x0015
    #define     B16smSysCtl_SM_CTRL_ADC_PU                     0x0014
    #define   LSb32smSysCtl_SM_CTRL_ADC_PU                        8
    #define   LSb16smSysCtl_SM_CTRL_ADC_PU                        8
    #define       bsmSysCtl_SM_CTRL_ADC_PU                     1
    #define   MSK32smSysCtl_SM_CTRL_ADC_PU                        0x00000100

    #define     BA_smSysCtl_SM_CTRL_ADC_CKSEL                  0x0015
    #define     B16smSysCtl_SM_CTRL_ADC_CKSEL                  0x0014
    #define   LSb32smSysCtl_SM_CTRL_ADC_CKSEL                     9
    #define   LSb16smSysCtl_SM_CTRL_ADC_CKSEL                     9
    #define       bsmSysCtl_SM_CTRL_ADC_CKSEL                  2
    #define   MSK32smSysCtl_SM_CTRL_ADC_CKSEL                     0x00000600

    #define     BA_smSysCtl_SM_CTRL_ADC_START                  0x0015
    #define     B16smSysCtl_SM_CTRL_ADC_START                  0x0014
    #define   LSb32smSysCtl_SM_CTRL_ADC_START                     11
    #define   LSb16smSysCtl_SM_CTRL_ADC_START                     11
    #define       bsmSysCtl_SM_CTRL_ADC_START                  1
    #define   MSK32smSysCtl_SM_CTRL_ADC_START                     0x00000800

    #define     BA_smSysCtl_SM_CTRL_ADC_RESET                  0x0015
    #define     B16smSysCtl_SM_CTRL_ADC_RESET                  0x0014
    #define   LSb32smSysCtl_SM_CTRL_ADC_RESET                     12
    #define   LSb16smSysCtl_SM_CTRL_ADC_RESET                     12
    #define       bsmSysCtl_SM_CTRL_ADC_RESET                  1
    #define   MSK32smSysCtl_SM_CTRL_ADC_RESET                     0x00001000

    #define     BA_smSysCtl_SM_CTRL_TEMP_SNR_PU                0x0015
    #define     B16smSysCtl_SM_CTRL_TEMP_SNR_PU                0x0014
    #define   LSb32smSysCtl_SM_CTRL_TEMP_SNR_PU                   13
    #define   LSb16smSysCtl_SM_CTRL_TEMP_SNR_PU                   13
    #define       bsmSysCtl_SM_CTRL_TEMP_SNR_PU                1
    #define   MSK32smSysCtl_SM_CTRL_TEMP_SNR_PU                   0x00002000
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_ADC_STATUS                      0x0018

    #define     BA_smSysCtl_SM_ADC_STATUS_CH0_DATA_RDY         0x0018
    #define     B16smSysCtl_SM_ADC_STATUS_CH0_DATA_RDY         0x0018
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH0_DATA_RDY            0
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH0_DATA_RDY            0
    #define       bsmSysCtl_SM_ADC_STATUS_CH0_DATA_RDY         1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH0_DATA_RDY            0x00000001

    #define     BA_smSysCtl_SM_ADC_STATUS_CH1_DATA_RDY         0x0018
    #define     B16smSysCtl_SM_ADC_STATUS_CH1_DATA_RDY         0x0018
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH1_DATA_RDY            1
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH1_DATA_RDY            1
    #define       bsmSysCtl_SM_ADC_STATUS_CH1_DATA_RDY         1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH1_DATA_RDY            0x00000002

    #define     BA_smSysCtl_SM_ADC_STATUS_CH2_DATA_RDY         0x0018
    #define     B16smSysCtl_SM_ADC_STATUS_CH2_DATA_RDY         0x0018
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH2_DATA_RDY            2
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH2_DATA_RDY            2
    #define       bsmSysCtl_SM_ADC_STATUS_CH2_DATA_RDY         1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH2_DATA_RDY            0x00000004

    #define     BA_smSysCtl_SM_ADC_STATUS_CH3_DATA_RDY         0x0018
    #define     B16smSysCtl_SM_ADC_STATUS_CH3_DATA_RDY         0x0018
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH3_DATA_RDY            3
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH3_DATA_RDY            3
    #define       bsmSysCtl_SM_ADC_STATUS_CH3_DATA_RDY         1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH3_DATA_RDY            0x00000008

    #define     BA_smSysCtl_SM_ADC_STATUS_CH4_DATA_RDY         0x0018
    #define     B16smSysCtl_SM_ADC_STATUS_CH4_DATA_RDY         0x0018
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH4_DATA_RDY            4
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH4_DATA_RDY            4
    #define       bsmSysCtl_SM_ADC_STATUS_CH4_DATA_RDY         1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH4_DATA_RDY            0x00000010

    #define     BA_smSysCtl_SM_ADC_STATUS_CH5_DATA_RDY         0x0018
    #define     B16smSysCtl_SM_ADC_STATUS_CH5_DATA_RDY         0x0018
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH5_DATA_RDY            5
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH5_DATA_RDY            5
    #define       bsmSysCtl_SM_ADC_STATUS_CH5_DATA_RDY         1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH5_DATA_RDY            0x00000020

    #define     BA_smSysCtl_SM_ADC_STATUS_CH6_DATA_RDY         0x0018
    #define     B16smSysCtl_SM_ADC_STATUS_CH6_DATA_RDY         0x0018
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH6_DATA_RDY            6
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH6_DATA_RDY            6
    #define       bsmSysCtl_SM_ADC_STATUS_CH6_DATA_RDY         1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH6_DATA_RDY            0x00000040

    #define     BA_smSysCtl_SM_ADC_STATUS_CH7_DATA_RDY         0x0018
    #define     B16smSysCtl_SM_ADC_STATUS_CH7_DATA_RDY         0x0018
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH7_DATA_RDY            7
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH7_DATA_RDY            7
    #define       bsmSysCtl_SM_ADC_STATUS_CH7_DATA_RDY         1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH7_DATA_RDY            0x00000080

    #define     BA_smSysCtl_SM_ADC_STATUS_REV_0                0x0019
    #define     B16smSysCtl_SM_ADC_STATUS_REV_0                0x0018
    #define   LSb32smSysCtl_SM_ADC_STATUS_REV_0                   8
    #define   LSb16smSysCtl_SM_ADC_STATUS_REV_0                   8
    #define       bsmSysCtl_SM_ADC_STATUS_REV_0                8
    #define   MSK32smSysCtl_SM_ADC_STATUS_REV_0                   0x0000FF00

    #define     BA_smSysCtl_SM_ADC_STATUS_CH0_INT_EN           0x001A
    #define     B16smSysCtl_SM_ADC_STATUS_CH0_INT_EN           0x001A
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH0_INT_EN              16
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH0_INT_EN              0
    #define       bsmSysCtl_SM_ADC_STATUS_CH0_INT_EN           1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH0_INT_EN              0x00010000

    #define     BA_smSysCtl_SM_ADC_STATUS_CH1_INT_EN           0x001A
    #define     B16smSysCtl_SM_ADC_STATUS_CH1_INT_EN           0x001A
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH1_INT_EN              17
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH1_INT_EN              1
    #define       bsmSysCtl_SM_ADC_STATUS_CH1_INT_EN           1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH1_INT_EN              0x00020000

    #define     BA_smSysCtl_SM_ADC_STATUS_CH2_INT_EN           0x001A
    #define     B16smSysCtl_SM_ADC_STATUS_CH2_INT_EN           0x001A
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH2_INT_EN              18
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH2_INT_EN              2
    #define       bsmSysCtl_SM_ADC_STATUS_CH2_INT_EN           1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH2_INT_EN              0x00040000

    #define     BA_smSysCtl_SM_ADC_STATUS_CH3_INT_EN           0x001A
    #define     B16smSysCtl_SM_ADC_STATUS_CH3_INT_EN           0x001A
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH3_INT_EN              19
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH3_INT_EN              3
    #define       bsmSysCtl_SM_ADC_STATUS_CH3_INT_EN           1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH3_INT_EN              0x00080000

    #define     BA_smSysCtl_SM_ADC_STATUS_CH4_INT_EN           0x001A
    #define     B16smSysCtl_SM_ADC_STATUS_CH4_INT_EN           0x001A
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH4_INT_EN              20
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH4_INT_EN              4
    #define       bsmSysCtl_SM_ADC_STATUS_CH4_INT_EN           1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH4_INT_EN              0x00100000

    #define     BA_smSysCtl_SM_ADC_STATUS_CH5_INT_EN           0x001A
    #define     B16smSysCtl_SM_ADC_STATUS_CH5_INT_EN           0x001A
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH5_INT_EN              21
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH5_INT_EN              5
    #define       bsmSysCtl_SM_ADC_STATUS_CH5_INT_EN           1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH5_INT_EN              0x00200000

    #define     BA_smSysCtl_SM_ADC_STATUS_CH6_INT_EN           0x001A
    #define     B16smSysCtl_SM_ADC_STATUS_CH6_INT_EN           0x001A
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH6_INT_EN              22
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH6_INT_EN              6
    #define       bsmSysCtl_SM_ADC_STATUS_CH6_INT_EN           1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH6_INT_EN              0x00400000

    #define     BA_smSysCtl_SM_ADC_STATUS_CH7_INT_EN           0x001A
    #define     B16smSysCtl_SM_ADC_STATUS_CH7_INT_EN           0x001A
    #define   LSb32smSysCtl_SM_ADC_STATUS_CH7_INT_EN              23
    #define   LSb16smSysCtl_SM_ADC_STATUS_CH7_INT_EN              7
    #define       bsmSysCtl_SM_ADC_STATUS_CH7_INT_EN           1
    #define   MSK32smSysCtl_SM_ADC_STATUS_CH7_INT_EN              0x00800000
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_ADC_DATA                        0x001C

    #define     BA_smSysCtl_SM_ADC_DATA_ADC_DATA               0x001C
    #define     B16smSysCtl_SM_ADC_DATA_ADC_DATA               0x001C
    #define   LSb32smSysCtl_SM_ADC_DATA_ADC_DATA                  0
    #define   LSb16smSysCtl_SM_ADC_DATA_ADC_DATA                  0
    #define       bsmSysCtl_SM_ADC_DATA_ADC_DATA               10
    #define   MSK32smSysCtl_SM_ADC_DATA_ADC_DATA                  0x000003FF
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_BOOT_STATUS                     0x0020

    #define     BA_smSysCtl_SM_BOOT_STATUS_MODE                0x0020
    #define     B16smSysCtl_SM_BOOT_STATUS_MODE                0x0020
    #define   LSb32smSysCtl_SM_BOOT_STATUS_MODE                   0
    #define   LSb16smSysCtl_SM_BOOT_STATUS_MODE                   0
    #define       bsmSysCtl_SM_BOOT_STATUS_MODE                32
    #define   MSK32smSysCtl_SM_BOOT_STATUS_MODE                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_WDT_MASK                        0x0024

    #define     BA_smSysCtl_SM_WDT_MASK_SM_RST                 0x0024
    #define     B16smSysCtl_SM_WDT_MASK_SM_RST                 0x0024
    #define   LSb32smSysCtl_SM_WDT_MASK_SM_RST                    0
    #define   LSb16smSysCtl_SM_WDT_MASK_SM_RST                    0
    #define       bsmSysCtl_SM_WDT_MASK_SM_RST                 3
    #define   MSK32smSysCtl_SM_WDT_MASK_SM_RST                    0x00000007

    #define     BA_smSysCtl_SM_WDT_MASK_SOC_RST                0x0024
    #define     B16smSysCtl_SM_WDT_MASK_SOC_RST                0x0024
    #define   LSb32smSysCtl_SM_WDT_MASK_SOC_RST                   3
    #define   LSb16smSysCtl_SM_WDT_MASK_SOC_RST                   3
    #define       bsmSysCtl_SM_WDT_MASK_SOC_RST                3
    #define   MSK32smSysCtl_SM_WDT_MASK_SOC_RST                   0x00000038
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_smAnaGrpCtl                        0x0028

    #define     BA_smSysCtl_smAnaGrpCtl_puIVref                0x0028
    #define     B16smSysCtl_smAnaGrpCtl_puIVref                0x0028
    #define   LSb32smSysCtl_smAnaGrpCtl_puIVref                   0
    #define   LSb16smSysCtl_smAnaGrpCtl_puIVref                   0
    #define       bsmSysCtl_smAnaGrpCtl_puIVref                1
    #define   MSK32smSysCtl_smAnaGrpCtl_puIVref                   0x00000001

    #define     BA_smSysCtl_smAnaGrpCtl_bgrSel                 0x0028
    #define     B16smSysCtl_smAnaGrpCtl_bgrSel                 0x0028
    #define   LSb32smSysCtl_smAnaGrpCtl_bgrSel                    1
    #define   LSb16smSysCtl_smAnaGrpCtl_bgrSel                    1
    #define       bsmSysCtl_smAnaGrpCtl_bgrSel                 1
    #define   MSK32smSysCtl_smAnaGrpCtl_bgrSel                    0x00000002

    #define     BA_smSysCtl_smAnaGrpCtl_puXtl                  0x0028
    #define     B16smSysCtl_smAnaGrpCtl_puXtl                  0x0028
    #define   LSb32smSysCtl_smAnaGrpCtl_puXtl                     2
    #define   LSb16smSysCtl_smAnaGrpCtl_puXtl                     2
    #define       bsmSysCtl_smAnaGrpCtl_puXtl                  1
    #define   MSK32smSysCtl_smAnaGrpCtl_puXtl                     0x00000004

    #define     BA_smSysCtl_smAnaGrpCtl_bypass                 0x0028
    #define     B16smSysCtl_smAnaGrpCtl_bypass                 0x0028
    #define   LSb32smSysCtl_smAnaGrpCtl_bypass                    3
    #define   LSb16smSysCtl_smAnaGrpCtl_bypass                    3
    #define       bsmSysCtl_smAnaGrpCtl_bypass                 1
    #define   MSK32smSysCtl_smAnaGrpCtl_bypass                    0x00000008

    #define     BA_smSysCtl_smAnaGrpCtl_gainX2                 0x0028
    #define     B16smSysCtl_smAnaGrpCtl_gainX2                 0x0028
    #define   LSb32smSysCtl_smAnaGrpCtl_gainX2                    4
    #define   LSb16smSysCtl_smAnaGrpCtl_gainX2                    4
    #define       bsmSysCtl_smAnaGrpCtl_gainX2                 1
    #define   MSK32smSysCtl_smAnaGrpCtl_gainX2                    0x00000010

    #define     BA_smSysCtl_smAnaGrpCtl_testAna                0x0028
    #define     B16smSysCtl_smAnaGrpCtl_testAna                0x0028
    #define   LSb32smSysCtl_smAnaGrpCtl_testAna                   5
    #define   LSb16smSysCtl_smAnaGrpCtl_testAna                   5
    #define       bsmSysCtl_smAnaGrpCtl_testAna                3
    #define   MSK32smSysCtl_smAnaGrpCtl_testAna                   0x000000E0
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_GSM_SEL                         0x002C

    #define     BA_smSysCtl_SM_GSM_SEL_GSM0_SEL                0x002C
    #define     B16smSysCtl_SM_GSM_SEL_GSM0_SEL                0x002C
    #define   LSb32smSysCtl_SM_GSM_SEL_GSM0_SEL                   0
    #define   LSb16smSysCtl_SM_GSM_SEL_GSM0_SEL                   0
    #define       bsmSysCtl_SM_GSM_SEL_GSM0_SEL                2
    #define   MSK32smSysCtl_SM_GSM_SEL_GSM0_SEL                   0x00000003

    #define     BA_smSysCtl_SM_GSM_SEL_GSM1_SEL                0x002C
    #define     B16smSysCtl_SM_GSM_SEL_GSM1_SEL                0x002C
    #define   LSb32smSysCtl_SM_GSM_SEL_GSM1_SEL                   2
    #define   LSb16smSysCtl_SM_GSM_SEL_GSM1_SEL                   2
    #define       bsmSysCtl_SM_GSM_SEL_GSM1_SEL                2
    #define   MSK32smSysCtl_SM_GSM_SEL_GSM1_SEL                   0x0000000C

    #define     BA_smSysCtl_SM_GSM_SEL_GSM2_SEL                0x002C
    #define     B16smSysCtl_SM_GSM_SEL_GSM2_SEL                0x002C
    #define   LSb32smSysCtl_SM_GSM_SEL_GSM2_SEL                   4
    #define   LSb16smSysCtl_SM_GSM_SEL_GSM2_SEL                   4
    #define       bsmSysCtl_SM_GSM_SEL_GSM2_SEL                2
    #define   MSK32smSysCtl_SM_GSM_SEL_GSM2_SEL                   0x00000030

    #define     BA_smSysCtl_SM_GSM_SEL_GSM3_SEL                0x002C
    #define     B16smSysCtl_SM_GSM_SEL_GSM3_SEL                0x002C
    #define   LSb32smSysCtl_SM_GSM_SEL_GSM3_SEL                   6
    #define   LSb16smSysCtl_SM_GSM_SEL_GSM3_SEL                   6
    #define       bsmSysCtl_SM_GSM_SEL_GSM3_SEL                2
    #define   MSK32smSysCtl_SM_GSM_SEL_GSM3_SEL                   0x000000C0

    #define     BA_smSysCtl_SM_GSM_SEL_GSM4_SEL                0x002D
    #define     B16smSysCtl_SM_GSM_SEL_GSM4_SEL                0x002C
    #define   LSb32smSysCtl_SM_GSM_SEL_GSM4_SEL                   8
    #define   LSb16smSysCtl_SM_GSM_SEL_GSM4_SEL                   8
    #define       bsmSysCtl_SM_GSM_SEL_GSM4_SEL                2
    #define   MSK32smSysCtl_SM_GSM_SEL_GSM4_SEL                   0x00000300

    #define     BA_smSysCtl_SM_GSM_SEL_GSM5_SEL                0x002D
    #define     B16smSysCtl_SM_GSM_SEL_GSM5_SEL                0x002C
    #define   LSb32smSysCtl_SM_GSM_SEL_GSM5_SEL                   10
    #define   LSb16smSysCtl_SM_GSM_SEL_GSM5_SEL                   10
    #define       bsmSysCtl_SM_GSM_SEL_GSM5_SEL                2
    #define   MSK32smSysCtl_SM_GSM_SEL_GSM5_SEL                   0x00000C00

    #define     BA_smSysCtl_SM_GSM_SEL_GSM6_SEL                0x002D
    #define     B16smSysCtl_SM_GSM_SEL_GSM6_SEL                0x002C
    #define   LSb32smSysCtl_SM_GSM_SEL_GSM6_SEL                   12
    #define   LSb16smSysCtl_SM_GSM_SEL_GSM6_SEL                   12
    #define       bsmSysCtl_SM_GSM_SEL_GSM6_SEL                2
    #define   MSK32smSysCtl_SM_GSM_SEL_GSM6_SEL                   0x00003000

    #define     BA_smSysCtl_SM_GSM_SEL_GSM7_SEL                0x002D
    #define     B16smSysCtl_SM_GSM_SEL_GSM7_SEL                0x002C
    #define   LSb32smSysCtl_SM_GSM_SEL_GSM7_SEL                   14
    #define   LSb16smSysCtl_SM_GSM_SEL_GSM7_SEL                   14
    #define       bsmSysCtl_SM_GSM_SEL_GSM7_SEL                1
    #define   MSK32smSysCtl_SM_GSM_SEL_GSM7_SEL                   0x00004000

    #define     BA_smSysCtl_SM_GSM_SEL_GSM8_SEL                0x002D
    #define     B16smSysCtl_SM_GSM_SEL_GSM8_SEL                0x002C
    #define   LSb32smSysCtl_SM_GSM_SEL_GSM8_SEL                   15
    #define   LSb16smSysCtl_SM_GSM_SEL_GSM8_SEL                   15
    #define       bsmSysCtl_SM_GSM_SEL_GSM8_SEL                1
    #define   MSK32smSysCtl_SM_GSM_SEL_GSM8_SEL                   0x00008000
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_CORE_CTRL                       0x0030

    #define     BA_smSysCtl_SM_CORE_CTRL_DVDDO_DI_SMANA0_V25EN_CORE 0x0030
    #define     B16smSysCtl_SM_CORE_CTRL_DVDDO_DI_SMANA0_V25EN_CORE 0x0030
    #define   LSb32smSysCtl_SM_CORE_CTRL_DVDDO_DI_SMANA0_V25EN_CORE    0
    #define   LSb16smSysCtl_SM_CORE_CTRL_DVDDO_DI_SMANA0_V25EN_CORE    0
    #define       bsmSysCtl_SM_CORE_CTRL_DVDDO_DI_SMANA0_V25EN_CORE 1
    #define   MSK32smSysCtl_SM_CORE_CTRL_DVDDO_DI_SMANA0_V25EN_CORE    0x00000001

    #define     BA_smSysCtl_SM_CORE_CTRL_DVDD_IO1_SMGPIO2_V25EN_CORE 0x0030
    #define     B16smSysCtl_SM_CORE_CTRL_DVDD_IO1_SMGPIO2_V25EN_CORE 0x0030
    #define   LSb32smSysCtl_SM_CORE_CTRL_DVDD_IO1_SMGPIO2_V25EN_CORE    1
    #define   LSb16smSysCtl_SM_CORE_CTRL_DVDD_IO1_SMGPIO2_V25EN_CORE    1
    #define       bsmSysCtl_SM_CORE_CTRL_DVDD_IO1_SMGPIO2_V25EN_CORE 1
    #define   MSK32smSysCtl_SM_CORE_CTRL_DVDD_IO1_SMGPIO2_V25EN_CORE    0x00000002

    #define     BA_smSysCtl_SM_CORE_CTRL_DVDD_IO2_SMGPIO0_V25EN_CORE 0x0030
    #define     B16smSysCtl_SM_CORE_CTRL_DVDD_IO2_SMGPIO0_V25EN_CORE 0x0030
    #define   LSb32smSysCtl_SM_CORE_CTRL_DVDD_IO2_SMGPIO0_V25EN_CORE    2
    #define   LSb16smSysCtl_SM_CORE_CTRL_DVDD_IO2_SMGPIO0_V25EN_CORE    2
    #define       bsmSysCtl_SM_CORE_CTRL_DVDD_IO2_SMGPIO0_V25EN_CORE 1
    #define   MSK32smSysCtl_SM_CORE_CTRL_DVDD_IO2_SMGPIO0_V25EN_CORE    0x00000004

    #define     BA_smSysCtl_SM_CORE_CTRL_V18_SELECT_SMANA0_V18EN_CORE 0x0030
    #define     B16smSysCtl_SM_CORE_CTRL_V18_SELECT_SMANA0_V18EN_CORE 0x0030
    #define   LSb32smSysCtl_SM_CORE_CTRL_V18_SELECT_SMANA0_V18EN_CORE    3
    #define   LSb16smSysCtl_SM_CORE_CTRL_V18_SELECT_SMANA0_V18EN_CORE    3
    #define       bsmSysCtl_SM_CORE_CTRL_V18_SELECT_SMANA0_V18EN_CORE 1
    #define   MSK32smSysCtl_SM_CORE_CTRL_V18_SELECT_SMANA0_V18EN_CORE    0x00000008

    #define     BA_smSysCtl_SM_CORE_CTRL_V18_SELECT_SMGBL0_V18EN_CORE 0x0030
    #define     B16smSysCtl_SM_CORE_CTRL_V18_SELECT_SMGBL0_V18EN_CORE 0x0030
    #define   LSb32smSysCtl_SM_CORE_CTRL_V18_SELECT_SMGBL0_V18EN_CORE    4
    #define   LSb16smSysCtl_SM_CORE_CTRL_V18_SELECT_SMGBL0_V18EN_CORE    4
    #define       bsmSysCtl_SM_CORE_CTRL_V18_SELECT_SMGBL0_V18EN_CORE 1
    #define   MSK32smSysCtl_SM_CORE_CTRL_V18_SELECT_SMGBL0_V18EN_CORE    0x00000010

    #define     BA_smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZN     0x0030
    #define     B16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZN     0x0030
    #define   LSb32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZN        5
    #define   LSb16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZN        5
    #define       bsmSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZN     3
    #define   MSK32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZN        0x000000E0

    #define     BA_smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZP     0x0031
    #define     B16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZP     0x0030
    #define   LSb32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZP        8
    #define   LSb16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZP        8
    #define       bsmSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZP     3
    #define   MSK32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZP        0x00000700

    #define     BA_smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZN     0x0031
    #define     B16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZN     0x0030
    #define   LSb32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZN        11
    #define   LSb16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZN        11
    #define       bsmSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZN     3
    #define   MSK32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZN        0x00003800

    #define     BA_smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZP     0x0031
    #define     B16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZP     0x0030
    #define   LSb32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZP        14
    #define   LSb16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZP        14
    #define       bsmSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZP     3
    #define   MSK32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZP        0x0001C000

    #define     BA_smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZN    0x0032
    #define     B16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZN    0x0032
    #define   LSb32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZN       17
    #define   LSb16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZN       1
    #define       bsmSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZN    3
    #define   MSK32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZN       0x000E0000

    #define     BA_smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZP    0x0032
    #define     B16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZP    0x0032
    #define   LSb32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZP       20
    #define   LSb16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZP       4
    #define       bsmSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZP    3
    #define   MSK32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZP       0x00700000
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_PAD_CTRL_0                      0x0034

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_ROSC          0x0034
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_ROSC          0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_ROSC             0
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_ROSC             0
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_ROSC          1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_ROSC             0x00000001

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTIn         0x0034
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTIn         0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTIn            1
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTIn            1
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTIn         1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTIn            0x00000002

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTOn         0x0034
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTOn         0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTOn            2
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTOn            2
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTOn         1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTOn            0x00000004

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_TCK           0x0034
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TCK           0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TCK              3
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TCK              3
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_TCK           1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TCK              0x00000008

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_TRSTn         0x0034
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TRSTn         0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TRSTn            4
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TRSTn            4
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_TRSTn         1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TRSTn            0x00000010

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_TMS           0x0034
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TMS           0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TMS              5
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TMS              5
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_TMS           1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TMS              0x00000020

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDI           0x0034
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDI           0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDI              6
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDI              6
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_TDI           1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDI              0x00000040

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDO           0x0034
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDO           0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDO              7
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDO              7
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_TDO           1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDO              0x00000080

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_SOC_RSTOn     0x0035
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SOC_RSTOn     0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SOC_RSTOn        8
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SOC_RSTOn        8
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_SOC_RSTOn     1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SOC_RSTOn        0x00000100

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_TEST_EN       0x0035
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TEST_EN       0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TEST_EN          9
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TEST_EN          9
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_TEST_EN       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TEST_EN          0x00000200

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_0        0x0035
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_0        0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_0           10
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_0           10
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_0        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_0           0x00000400

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_1        0x0035
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_1        0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_1           11
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_1           11
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_1        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_1           0x00000800

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_0        0x0035
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_0        0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_0           12
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_0           12
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_0        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_0           0x00001000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_0         0x0035
    #define     B16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_0         0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_0            13
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_0            13
    #define       bsmSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_0         1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_0            0x00002000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_0         0x0035
    #define     B16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_0         0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_0            14
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_0            14
    #define       bsmSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_0         1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_0            0x00004000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_1        0x0035
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_1        0x0034
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_1           15
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_1           15
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_1        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_1           0x00008000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_1         0x0036
    #define     B16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_1         0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_1            16
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_1            0
    #define       bsmSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_1         1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_1            0x00010000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_1         0x0036
    #define     B16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_1         0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_1            17
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_1            1
    #define       bsmSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_1         1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_1            0x00020000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_2        0x0036
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_2        0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_2           18
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_2           2
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_2        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_2           0x00040000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_2         0x0036
    #define     B16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_2         0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_2            19
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_2            3
    #define       bsmSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_2         1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_2            0x00080000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_2         0x0036
    #define     B16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_2         0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_2            20
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_2            4
    #define       bsmSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_2         1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_2            0x00100000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_3        0x0036
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_3        0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_3           21
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_3           5
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_3        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_3           0x00200000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_3         0x0036
    #define     B16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_3         0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_3            22
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_3            6
    #define       bsmSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_3         1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_3            0x00400000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_3         0x0036
    #define     B16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_3         0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_3            23
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_3            7
    #define       bsmSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_3         1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_3            0x00800000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S0n      0x0037
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S0n      0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S0n         24
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S0n         8
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S0n      1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S0n         0x01000000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S0n       0x0037
    #define     B16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S0n       0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S0n          25
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S0n          9
    #define       bsmSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S0n       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S0n          0x02000000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S0n       0x0037
    #define     B16smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S0n       0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S0n          26
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S0n          10
    #define       bsmSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S0n       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S0n          0x04000000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S1n      0x0037
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S1n      0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S1n         27
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S1n         11
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S1n      1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S1n         0x08000000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S1n       0x0037
    #define     B16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S1n       0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S1n          28
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S1n          12
    #define       bsmSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S1n       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S1n          0x10000000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S1n       0x0037
    #define     B16smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S1n       0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S1n          29
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S1n          13
    #define       bsmSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S1n       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S1n          0x20000000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S2n      0x0037
    #define     B16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S2n      0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S2n         30
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S2n         14
    #define       bsmSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S2n      1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S2n         0x40000000

    #define     BA_smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S2n       0x0037
    #define     B16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S2n       0x0036
    #define   LSb32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S2n          31
    #define   LSb16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S2n          15
    #define       bsmSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S2n       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S2n          0x80000000
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_PAD_CTRL_1                      0x0038

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S2n       0x0038
    #define     B16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S2n       0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S2n          0
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S2n          0
    #define       bsmSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S2n       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S2n          0x00000001

    #define     BA_smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_S3n      0x0038
    #define     B16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_S3n      0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_S3n         1
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_S3n         1
    #define       bsmSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_S3n      1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_S3n         0x00000002

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_S3n       0x0038
    #define     B16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_S3n       0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_S3n          2
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_S3n          2
    #define       bsmSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_S3n       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_S3n          0x00000004

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S3n       0x0038
    #define     B16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S3n       0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S3n          3
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S3n          3
    #define       bsmSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S3n       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S3n          0x00000008

    #define     BA_smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SCK      0x0038
    #define     B16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SCK      0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SCK         4
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SCK         4
    #define       bsmSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SCK      1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SCK         0x00000010

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SCK       0x0038
    #define     B16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SCK       0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SCK          5
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SCK          5
    #define       bsmSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SCK       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SCK          0x00000020

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SCK       0x0038
    #define     B16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SCK       0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SCK          6
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SCK          6
    #define       bsmSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SCK       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SCK          0x00000040

    #define     BA_smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDO      0x0038
    #define     B16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDO      0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDO         7
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDO         7
    #define       bsmSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDO      1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDO         0x00000080

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDO       0x0039
    #define     B16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDO       0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDO          8
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDO          8
    #define       bsmSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDO       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDO          0x00000100

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDO       0x0039
    #define     B16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDO       0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDO          9
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDO          9
    #define       bsmSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDO       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDO          0x00000200

    #define     BA_smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDI      0x0039
    #define     B16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDI      0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDI         10
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDI         10
    #define       bsmSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDI      1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDI         0x00000400

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDI       0x0039
    #define     B16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDI       0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDI          11
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDI          11
    #define       bsmSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDI       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDI          0x00000800

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDI       0x0039
    #define     B16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDI       0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDI          12
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDI          12
    #define       bsmSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDI       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDI          0x00001000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RXD       0x0039
    #define     B16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RXD       0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RXD          13
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RXD          13
    #define       bsmSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RXD       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RXD          0x00002000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RXD        0x0039
    #define     B16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RXD        0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RXD           14
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RXD           14
    #define       bsmSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RXD        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RXD           0x00004000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RXD        0x0039
    #define     B16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RXD        0x0038
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RXD           15
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RXD           15
    #define       bsmSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RXD        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RXD           0x00008000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_TXD       0x003A
    #define     B16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_TXD       0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_TXD          16
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_TXD          0
    #define       bsmSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_TXD       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_TXD          0x00010000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_TXD        0x003A
    #define     B16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_TXD        0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_TXD           17
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_TXD           1
    #define       bsmSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_TXD        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_TXD           0x00020000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_TXD        0x003A
    #define     B16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_TXD        0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_TXD           18
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_TXD           2
    #define       bsmSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_TXD        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_TXD           0x00040000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_CTSn      0x003A
    #define     B16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_CTSn      0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_CTSn         19
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_CTSn         3
    #define       bsmSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_CTSn      1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_CTSn         0x00080000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_CTSn       0x003A
    #define     B16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_CTSn       0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_CTSn          20
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_CTSn          4
    #define       bsmSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_CTSn       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_CTSn          0x00100000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_CTSn       0x003A
    #define     B16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_CTSn       0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_CTSn          21
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_CTSn          5
    #define       bsmSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_CTSn       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_CTSn          0x00200000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RTSn      0x003A
    #define     B16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RTSn      0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RTSn         22
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RTSn         6
    #define       bsmSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RTSn      1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RTSn         0x00400000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RTSn       0x003A
    #define     B16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RTSn       0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RTSn          23
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RTSn          7
    #define       bsmSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RTSn       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RTSn          0x00800000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RTSn       0x003B
    #define     B16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RTSn       0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RTSn          24
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RTSn          8
    #define       bsmSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RTSn       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RTSn          0x01000000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_RXD       0x003B
    #define     B16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_RXD       0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_RXD          25
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_RXD          9
    #define       bsmSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_RXD       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_RXD          0x02000000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_RXD        0x003B
    #define     B16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_RXD        0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_RXD           26
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_RXD           10
    #define       bsmSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_RXD        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_RXD           0x04000000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_RXD        0x003B
    #define     B16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_RXD        0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_RXD           27
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_RXD           11
    #define       bsmSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_RXD        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_RXD           0x08000000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_TXD       0x003B
    #define     B16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_TXD       0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_TXD          28
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_TXD          12
    #define       bsmSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_TXD       1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_TXD          0x10000000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_TXD        0x003B
    #define     B16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_TXD        0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_TXD           29
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_TXD           13
    #define       bsmSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_TXD        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_TXD           0x20000000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_TXD        0x003B
    #define     B16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_TXD        0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_TXD           30
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_TXD           14
    #define       bsmSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_TXD        1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_TXD           0x40000000

    #define     BA_smSysCtl_SM_PAD_CTRL_1_SLP_DI_CLKI          0x003B
    #define     B16smSysCtl_SM_PAD_CTRL_1_SLP_DI_CLKI          0x003A
    #define   LSb32smSysCtl_SM_PAD_CTRL_1_SLP_DI_CLKI             31
    #define   LSb16smSysCtl_SM_PAD_CTRL_1_SLP_DI_CLKI             15
    #define       bsmSysCtl_SM_PAD_CTRL_1_SLP_DI_CLKI          1
    #define   MSK32smSysCtl_SM_PAD_CTRL_1_SLP_DI_CLKI             0x80000000
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_PAD_CTRL_2                      0x003C

    #define     BA_smSysCtl_SM_PAD_CTRL_2_PU_EN_CLKI           0x003C
    #define     B16smSysCtl_SM_PAD_CTRL_2_PU_EN_CLKI           0x003C
    #define   LSb32smSysCtl_SM_PAD_CTRL_2_PU_EN_CLKI              0
    #define   LSb16smSysCtl_SM_PAD_CTRL_2_PU_EN_CLKI              0
    #define       bsmSysCtl_SM_PAD_CTRL_2_PU_EN_CLKI           1
    #define   MSK32smSysCtl_SM_PAD_CTRL_2_PU_EN_CLKI              0x00000001

    #define     BA_smSysCtl_SM_PAD_CTRL_2_PD_EN_CLKI           0x003C
    #define     B16smSysCtl_SM_PAD_CTRL_2_PD_EN_CLKI           0x003C
    #define   LSb32smSysCtl_SM_PAD_CTRL_2_PD_EN_CLKI              1
    #define   LSb16smSysCtl_SM_PAD_CTRL_2_PD_EN_CLKI              1
    #define       bsmSysCtl_SM_PAD_CTRL_2_PD_EN_CLKI           1
    #define   MSK32smSysCtl_SM_PAD_CTRL_2_PD_EN_CLKI              0x00000002
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_TEST                            0x0040

    #define     BA_smSysCtl_SM_TEST_ADC_TEST_FAIL              0x0040
    #define     B16smSysCtl_SM_TEST_ADC_TEST_FAIL              0x0040
    #define   LSb32smSysCtl_SM_TEST_ADC_TEST_FAIL                 0
    #define   LSb16smSysCtl_SM_TEST_ADC_TEST_FAIL                 0
    #define       bsmSysCtl_SM_TEST_ADC_TEST_FAIL              1
    #define   MSK32smSysCtl_SM_TEST_ADC_TEST_FAIL                 0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_TEST_DATA0                      0x0044

    #define     BA_smSysCtl_SM_TEST_DATA0_ADC_DATA_HIGH        0x0044
    #define     B16smSysCtl_SM_TEST_DATA0_ADC_DATA_HIGH        0x0044
    #define   LSb32smSysCtl_SM_TEST_DATA0_ADC_DATA_HIGH           0
    #define   LSb16smSysCtl_SM_TEST_DATA0_ADC_DATA_HIGH           0
    #define       bsmSysCtl_SM_TEST_DATA0_ADC_DATA_HIGH        10
    #define   MSK32smSysCtl_SM_TEST_DATA0_ADC_DATA_HIGH           0x000003FF
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_TEST_DATA1                      0x0048

    #define     BA_smSysCtl_SM_TEST_DATA1_ADC_DATA_LOW         0x0048
    #define     B16smSysCtl_SM_TEST_DATA1_ADC_DATA_LOW         0x0048
    #define   LSb32smSysCtl_SM_TEST_DATA1_ADC_DATA_LOW            0
    #define   LSb16smSysCtl_SM_TEST_DATA1_ADC_DATA_LOW            0
    #define       bsmSysCtl_SM_TEST_DATA1_ADC_DATA_LOW         10
    #define   MSK32smSysCtl_SM_TEST_DATA1_ADC_DATA_LOW            0x000003FF
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_RWTC_CTRL_0                     0x004C

    #define     BA_smSysCtl_SM_RWTC_CTRL_0_RF1P_LOW            0x004C
    #define     B16smSysCtl_SM_RWTC_CTRL_0_RF1P_LOW            0x004C
    #define   LSb32smSysCtl_SM_RWTC_CTRL_0_RF1P_LOW               0
    #define   LSb16smSysCtl_SM_RWTC_CTRL_0_RF1P_LOW               0
    #define       bsmSysCtl_SM_RWTC_CTRL_0_RF1P_LOW            4
    #define   MSK32smSysCtl_SM_RWTC_CTRL_0_RF1P_LOW               0x0000000F

    #define     BA_smSysCtl_SM_RWTC_CTRL_0_RF1P_HIGH           0x004C
    #define     B16smSysCtl_SM_RWTC_CTRL_0_RF1P_HIGH           0x004C
    #define   LSb32smSysCtl_SM_RWTC_CTRL_0_RF1P_HIGH              4
    #define   LSb16smSysCtl_SM_RWTC_CTRL_0_RF1P_HIGH              4
    #define       bsmSysCtl_SM_RWTC_CTRL_0_RF1P_HIGH           4
    #define   MSK32smSysCtl_SM_RWTC_CTRL_0_RF1P_HIGH              0x000000F0

    #define     BA_smSysCtl_SM_RWTC_CTRL_0_RF2P_LOW            0x004D
    #define     B16smSysCtl_SM_RWTC_CTRL_0_RF2P_LOW            0x004C
    #define   LSb32smSysCtl_SM_RWTC_CTRL_0_RF2P_LOW               8
    #define   LSb16smSysCtl_SM_RWTC_CTRL_0_RF2P_LOW               8
    #define       bsmSysCtl_SM_RWTC_CTRL_0_RF2P_LOW            4
    #define   MSK32smSysCtl_SM_RWTC_CTRL_0_RF2P_LOW               0x00000F00

    #define     BA_smSysCtl_SM_RWTC_CTRL_0_RF2P_HIGH           0x004D
    #define     B16smSysCtl_SM_RWTC_CTRL_0_RF2P_HIGH           0x004C
    #define   LSb32smSysCtl_SM_RWTC_CTRL_0_RF2P_HIGH              12
    #define   LSb16smSysCtl_SM_RWTC_CTRL_0_RF2P_HIGH              12
    #define       bsmSysCtl_SM_RWTC_CTRL_0_RF2P_HIGH           4
    #define   MSK32smSysCtl_SM_RWTC_CTRL_0_RF2P_HIGH              0x0000F000

    #define     BA_smSysCtl_SM_RWTC_CTRL_0_SR1P_LOW            0x004E
    #define     B16smSysCtl_SM_RWTC_CTRL_0_SR1P_LOW            0x004E
    #define   LSb32smSysCtl_SM_RWTC_CTRL_0_SR1P_LOW               16
    #define   LSb16smSysCtl_SM_RWTC_CTRL_0_SR1P_LOW               0
    #define       bsmSysCtl_SM_RWTC_CTRL_0_SR1P_LOW            4
    #define   MSK32smSysCtl_SM_RWTC_CTRL_0_SR1P_LOW               0x000F0000

    #define     BA_smSysCtl_SM_RWTC_CTRL_0_SR1P_HIGH           0x004E
    #define     B16smSysCtl_SM_RWTC_CTRL_0_SR1P_HIGH           0x004E
    #define   LSb32smSysCtl_SM_RWTC_CTRL_0_SR1P_HIGH              20
    #define   LSb16smSysCtl_SM_RWTC_CTRL_0_SR1P_HIGH              4
    #define       bsmSysCtl_SM_RWTC_CTRL_0_SR1P_HIGH           4
    #define   MSK32smSysCtl_SM_RWTC_CTRL_0_SR1P_HIGH              0x00F00000

    #define     BA_smSysCtl_SM_RWTC_CTRL_0_SR2P_LOW            0x004F
    #define     B16smSysCtl_SM_RWTC_CTRL_0_SR2P_LOW            0x004E
    #define   LSb32smSysCtl_SM_RWTC_CTRL_0_SR2P_LOW               24
    #define   LSb16smSysCtl_SM_RWTC_CTRL_0_SR2P_LOW               8
    #define       bsmSysCtl_SM_RWTC_CTRL_0_SR2P_LOW            4
    #define   MSK32smSysCtl_SM_RWTC_CTRL_0_SR2P_LOW               0x0F000000

    #define     BA_smSysCtl_SM_RWTC_CTRL_0_SR2P_HIGH           0x004F
    #define     B16smSysCtl_SM_RWTC_CTRL_0_SR2P_HIGH           0x004E
    #define   LSb32smSysCtl_SM_RWTC_CTRL_0_SR2P_HIGH              28
    #define   LSb16smSysCtl_SM_RWTC_CTRL_0_SR2P_HIGH              12
    #define       bsmSysCtl_SM_RWTC_CTRL_0_SR2P_HIGH           4
    #define   MSK32smSysCtl_SM_RWTC_CTRL_0_SR2P_HIGH              0xF0000000
    ///////////////////////////////////////////////////////////
    #define     RA_smSysCtl_SM_RWTC_CTRL_1                     0x0050

    #define     BA_smSysCtl_SM_RWTC_CTRL_1_ROM_LOW             0x0050
    #define     B16smSysCtl_SM_RWTC_CTRL_1_ROM_LOW             0x0050
    #define   LSb32smSysCtl_SM_RWTC_CTRL_1_ROM_LOW                0
    #define   LSb16smSysCtl_SM_RWTC_CTRL_1_ROM_LOW                0
    #define       bsmSysCtl_SM_RWTC_CTRL_1_ROM_LOW             4
    #define   MSK32smSysCtl_SM_RWTC_CTRL_1_ROM_LOW                0x0000000F

    #define     BA_smSysCtl_SM_RWTC_CTRL_1_ROM_HIGH            0x0050
    #define     B16smSysCtl_SM_RWTC_CTRL_1_ROM_HIGH            0x0050
    #define   LSb32smSysCtl_SM_RWTC_CTRL_1_ROM_HIGH               4
    #define   LSb16smSysCtl_SM_RWTC_CTRL_1_ROM_HIGH               4
    #define       bsmSysCtl_SM_RWTC_CTRL_1_ROM_HIGH            4
    #define   MSK32smSysCtl_SM_RWTC_CTRL_1_ROM_HIGH               0x000000F0

    #define     BA_smSysCtl_SM_RWTC_CTRL_1_ACMEM               0x0051
    #define     B16smSysCtl_SM_RWTC_CTRL_1_ACMEM               0x0050
    #define   LSb32smSysCtl_SM_RWTC_CTRL_1_ACMEM                  8
    #define   LSb16smSysCtl_SM_RWTC_CTRL_1_ACMEM                  8
    #define       bsmSysCtl_SM_RWTC_CTRL_1_ACMEM               4
    #define   MSK32smSysCtl_SM_RWTC_CTRL_1_ACMEM                  0x00000F00
    ///////////////////////////////////////////////////////////

    typedef struct SIE_smSysCtl {
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_ID_REV_ID(r32)                  _BFGET_(r32, 3, 0)
    #define   SET32smSysCtl_SM_ID_REV_ID(r32,v)                _BFSET_(r32, 3, 0,v)
    #define   GET16smSysCtl_SM_ID_REV_ID(r16)                  _BFGET_(r16, 3, 0)
    #define   SET16smSysCtl_SM_ID_REV_ID(r16,v)                _BFSET_(r16, 3, 0,v)

    #define   GET32smSysCtl_SM_ID_PART_ID(r32)                 _BFGET_(r32,19, 4)
    #define   SET32smSysCtl_SM_ID_PART_ID(r32,v)               _BFSET_(r32,19, 4,v)

    #define   GET32smSysCtl_SM_ID_MFC_ID(r32)                  _BFGET_(r32,31,20)
    #define   SET32smSysCtl_SM_ID_MFC_ID(r32,v)                _BFSET_(r32,31,20,v)
    #define   GET16smSysCtl_SM_ID_MFC_ID(r16)                  _BFGET_(r16,15, 4)
    #define   SET16smSysCtl_SM_ID_MFC_ID(r16,v)                _BFSET_(r16,15, 4,v)

    #define     w32smSysCtl_SM_ID                              {\
            UNSG32 uSM_ID_REV_ID                               :  4;\
            UNSG32 uSM_ID_PART_ID                              : 16;\
            UNSG32 uSM_ID_MFC_ID                               : 12;\
          }
    union { UNSG32 u32smSysCtl_SM_ID;
            struct w32smSysCtl_SM_ID;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_CPU_CTRL_CPU_RST_GO(r32)        _BFGET_(r32, 0, 0)
    #define   SET32smSysCtl_SM_CPU_CTRL_CPU_RST_GO(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   GET16smSysCtl_SM_CPU_CTRL_CPU_RST_GO(r16)        _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_CPU_CTRL_CPU_RST_GO(r16,v)      _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_CPU_CTRL_CPU_VINITHI(r32)       _BFGET_(r32, 1, 1)
    #define   SET32smSysCtl_SM_CPU_CTRL_CPU_VINITHI(r32,v)     _BFSET_(r32, 1, 1,v)
    #define   GET16smSysCtl_SM_CPU_CTRL_CPU_VINITHI(r16)       _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_CPU_CTRL_CPU_VINITHI(r16,v)     _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_SM_CPU_CTRL_CPU_INITRAM(r32)       _BFGET_(r32, 2, 2)
    #define   SET32smSysCtl_SM_CPU_CTRL_CPU_INITRAM(r32,v)     _BFSET_(r32, 2, 2,v)
    #define   GET16smSysCtl_SM_CPU_CTRL_CPU_INITRAM(r16)       _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_CPU_CTRL_CPU_INITRAM(r16,v)     _BFSET_(r16, 2, 2,v)

    #define   GET32smSysCtl_SM_CPU_CTRL_CPU_BIGEND(r32)        _BFGET_(r32, 3, 3)
    #define   SET32smSysCtl_SM_CPU_CTRL_CPU_BIGEND(r32,v)      _BFSET_(r32, 3, 3,v)
    #define   GET16smSysCtl_SM_CPU_CTRL_CPU_BIGEND(r16)        _BFGET_(r16, 3, 3)
    #define   SET16smSysCtl_SM_CPU_CTRL_CPU_BIGEND(r16,v)      _BFSET_(r16, 3, 3,v)

    #define     w32smSysCtl_SM_CPU_CTRL                        {\
            UNSG32 uSM_CPU_CTRL_CPU_RST_GO                     :  1;\
            UNSG32 uSM_CPU_CTRL_CPU_VINITHI                    :  1;\
            UNSG32 uSM_CPU_CTRL_CPU_INITRAM                    :  1;\
            UNSG32 uSM_CPU_CTRL_CPU_BIGEND                     :  1;\
            UNSG32 RSVDx4_b4                                   : 28;\
          }
    union { UNSG32 u32smSysCtl_SM_CPU_CTRL;
            struct w32smSysCtl_SM_CPU_CTRL;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_RST_CTRL_SOC_RST_GO(r32)        _BFGET_(r32, 0, 0)
    #define   SET32smSysCtl_SM_RST_CTRL_SOC_RST_GO(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   GET16smSysCtl_SM_RST_CTRL_SOC_RST_GO(r16)        _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_RST_CTRL_SOC_RST_GO(r16,v)      _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_RST_CTRL_APB_RST_GO(r32)        _BFGET_(r32, 1, 1)
    #define   SET32smSysCtl_SM_RST_CTRL_APB_RST_GO(r32,v)      _BFSET_(r32, 1, 1,v)
    #define   GET16smSysCtl_SM_RST_CTRL_APB_RST_GO(r16)        _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_RST_CTRL_APB_RST_GO(r16,v)      _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_SM_RST_CTRL_SXBAR_RST_GO(r32)      _BFGET_(r32, 2, 2)
    #define   SET32smSysCtl_SM_RST_CTRL_SXBAR_RST_GO(r32,v)    _BFSET_(r32, 2, 2,v)
    #define   GET16smSysCtl_SM_RST_CTRL_SXBAR_RST_GO(r16)      _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_RST_CTRL_SXBAR_RST_GO(r16,v)    _BFSET_(r16, 2, 2,v)

    #define     w32smSysCtl_SM_RST_CTRL                        {\
            UNSG32 uSM_RST_CTRL_SOC_RST_GO                     :  1;\
            UNSG32 uSM_RST_CTRL_APB_RST_GO                     :  1;\
            UNSG32 uSM_RST_CTRL_SXBAR_RST_GO                   :  1;\
            UNSG32 RSVDx8_b3                                   : 29;\
          }
    union { UNSG32 u32smSysCtl_SM_RST_CTRL;
            struct w32smSysCtl_SM_RST_CTRL;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_RST_STATUS_RST_WD_0(r32)        _BFGET_(r32, 0, 0)
    #define   SET32smSysCtl_SM_RST_STATUS_RST_WD_0(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   GET16smSysCtl_SM_RST_STATUS_RST_WD_0(r16)        _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_RST_STATUS_RST_WD_0(r16,v)      _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_RST_STATUS_RST_WD_1(r32)        _BFGET_(r32, 1, 1)
    #define   SET32smSysCtl_SM_RST_STATUS_RST_WD_1(r32,v)      _BFSET_(r32, 1, 1,v)
    #define   GET16smSysCtl_SM_RST_STATUS_RST_WD_1(r16)        _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_RST_STATUS_RST_WD_1(r16,v)      _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_SM_RST_STATUS_RST_WD_2(r32)        _BFGET_(r32, 2, 2)
    #define   SET32smSysCtl_SM_RST_STATUS_RST_WD_2(r32,v)      _BFSET_(r32, 2, 2,v)
    #define   GET16smSysCtl_SM_RST_STATUS_RST_WD_2(r16)        _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_RST_STATUS_RST_WD_2(r16,v)      _BFSET_(r16, 2, 2,v)

    #define     w32smSysCtl_SM_RST_STATUS                      {\
            UNSG32 uSM_RST_STATUS_RST_WD_0                     :  1;\
            UNSG32 uSM_RST_STATUS_RST_WD_1                     :  1;\
            UNSG32 uSM_RST_STATUS_RST_WD_2                     :  1;\
            UNSG32 RSVDxC_b3                                   : 29;\
          }
    union { UNSG32 u32smSysCtl_SM_RST_STATUS;
            struct w32smSysCtl_SM_RST_STATUS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_STRP_STATUS_BOOT_MODE(r32)      _BFGET_(r32, 1, 0)
    #define   SET32smSysCtl_SM_STRP_STATUS_BOOT_MODE(r32,v)    _BFSET_(r32, 1, 0,v)
    #define   GET16smSysCtl_SM_STRP_STATUS_BOOT_MODE(r16)      _BFGET_(r16, 1, 0)
    #define   SET16smSysCtl_SM_STRP_STATUS_BOOT_MODE(r16,v)    _BFSET_(r16, 1, 0,v)

    #define   GET32smSysCtl_SM_STRP_STATUS_STRP_2(r32)         _BFGET_(r32, 2, 2)
    #define   SET32smSysCtl_SM_STRP_STATUS_STRP_2(r32,v)       _BFSET_(r32, 2, 2,v)
    #define   GET16smSysCtl_SM_STRP_STATUS_STRP_2(r16)         _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_STRP_STATUS_STRP_2(r16,v)       _BFSET_(r16, 2, 2,v)

    #define   GET32smSysCtl_SM_STRP_STATUS_STRP_3(r32)         _BFGET_(r32, 3, 3)
    #define   SET32smSysCtl_SM_STRP_STATUS_STRP_3(r32,v)       _BFSET_(r32, 3, 3,v)
    #define   GET16smSysCtl_SM_STRP_STATUS_STRP_3(r16)         _BFGET_(r16, 3, 3)
    #define   SET16smSysCtl_SM_STRP_STATUS_STRP_3(r16,v)       _BFSET_(r16, 3, 3,v)

    #define     w32smSysCtl_SM_STRP_STATUS                     {\
            UNSG32 uSM_STRP_STATUS_BOOT_MODE                   :  2;\
            UNSG32 uSM_STRP_STATUS_STRP_2                      :  1;\
            UNSG32 uSM_STRP_STATUS_STRP_3                      :  1;\
            UNSG32 RSVDx10_b4                                  : 28;\
          }
    union { UNSG32 u32smSysCtl_SM_STRP_STATUS;
            struct w32smSysCtl_SM_STRP_STATUS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_CTRL_ISO_EN(r32)                _BFGET_(r32, 0, 0)
    #define   SET32smSysCtl_SM_CTRL_ISO_EN(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16smSysCtl_SM_CTRL_ISO_EN(r16)                _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_CTRL_ISO_EN(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_CTRL_SM2SOC_SW_INTR(r32)        _BFGET_(r32, 1, 1)
    #define   SET32smSysCtl_SM_CTRL_SM2SOC_SW_INTR(r32,v)      _BFSET_(r32, 1, 1,v)
    #define   GET16smSysCtl_SM_CTRL_SM2SOC_SW_INTR(r16)        _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_CTRL_SM2SOC_SW_INTR(r16,v)      _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_SM_CTRL_SOC2SM_SW_INTR(r32)        _BFGET_(r32, 2, 2)
    #define   SET32smSysCtl_SM_CTRL_SOC2SM_SW_INTR(r32,v)      _BFSET_(r32, 2, 2,v)
    #define   GET16smSysCtl_SM_CTRL_SOC2SM_SW_INTR(r16)        _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_CTRL_SOC2SM_SW_INTR(r16,v)      _BFSET_(r16, 2, 2,v)

    #define   GET32smSysCtl_SM_CTRL_REV_0(r32)                 _BFGET_(r32, 4, 3)
    #define   SET32smSysCtl_SM_CTRL_REV_0(r32,v)               _BFSET_(r32, 4, 3,v)
    #define   GET16smSysCtl_SM_CTRL_REV_0(r16)                 _BFGET_(r16, 4, 3)
    #define   SET16smSysCtl_SM_CTRL_REV_0(r16,v)               _BFSET_(r16, 4, 3,v)

    #define   GET32smSysCtl_SM_CTRL_ADC_SEL(r32)               _BFGET_(r32, 7, 5)
    #define   SET32smSysCtl_SM_CTRL_ADC_SEL(r32,v)             _BFSET_(r32, 7, 5,v)
    #define   GET16smSysCtl_SM_CTRL_ADC_SEL(r16)               _BFGET_(r16, 7, 5)
    #define   SET16smSysCtl_SM_CTRL_ADC_SEL(r16,v)             _BFSET_(r16, 7, 5,v)

    #define   GET32smSysCtl_SM_CTRL_ADC_PU(r32)                _BFGET_(r32, 8, 8)
    #define   SET32smSysCtl_SM_CTRL_ADC_PU(r32,v)              _BFSET_(r32, 8, 8,v)
    #define   GET16smSysCtl_SM_CTRL_ADC_PU(r16)                _BFGET_(r16, 8, 8)
    #define   SET16smSysCtl_SM_CTRL_ADC_PU(r16,v)              _BFSET_(r16, 8, 8,v)

    #define   GET32smSysCtl_SM_CTRL_ADC_CKSEL(r32)             _BFGET_(r32,10, 9)
    #define   SET32smSysCtl_SM_CTRL_ADC_CKSEL(r32,v)           _BFSET_(r32,10, 9,v)
    #define   GET16smSysCtl_SM_CTRL_ADC_CKSEL(r16)             _BFGET_(r16,10, 9)
    #define   SET16smSysCtl_SM_CTRL_ADC_CKSEL(r16,v)           _BFSET_(r16,10, 9,v)

    #define   GET32smSysCtl_SM_CTRL_ADC_START(r32)             _BFGET_(r32,11,11)
    #define   SET32smSysCtl_SM_CTRL_ADC_START(r32,v)           _BFSET_(r32,11,11,v)
    #define   GET16smSysCtl_SM_CTRL_ADC_START(r16)             _BFGET_(r16,11,11)
    #define   SET16smSysCtl_SM_CTRL_ADC_START(r16,v)           _BFSET_(r16,11,11,v)

    #define   GET32smSysCtl_SM_CTRL_ADC_RESET(r32)             _BFGET_(r32,12,12)
    #define   SET32smSysCtl_SM_CTRL_ADC_RESET(r32,v)           _BFSET_(r32,12,12,v)
    #define   GET16smSysCtl_SM_CTRL_ADC_RESET(r16)             _BFGET_(r16,12,12)
    #define   SET16smSysCtl_SM_CTRL_ADC_RESET(r16,v)           _BFSET_(r16,12,12,v)

    #define   GET32smSysCtl_SM_CTRL_TEMP_SNR_PU(r32)           _BFGET_(r32,13,13)
    #define   SET32smSysCtl_SM_CTRL_TEMP_SNR_PU(r32,v)         _BFSET_(r32,13,13,v)
    #define   GET16smSysCtl_SM_CTRL_TEMP_SNR_PU(r16)           _BFGET_(r16,13,13)
    #define   SET16smSysCtl_SM_CTRL_TEMP_SNR_PU(r16,v)         _BFSET_(r16,13,13,v)

    #define     w32smSysCtl_SM_CTRL                            {\
            UNSG32 uSM_CTRL_ISO_EN                             :  1;\
            UNSG32 uSM_CTRL_SM2SOC_SW_INTR                     :  1;\
            UNSG32 uSM_CTRL_SOC2SM_SW_INTR                     :  1;\
            UNSG32 uSM_CTRL_REV_0                              :  2;\
            UNSG32 uSM_CTRL_ADC_SEL                            :  3;\
            UNSG32 uSM_CTRL_ADC_PU                             :  1;\
            UNSG32 uSM_CTRL_ADC_CKSEL                          :  2;\
            UNSG32 uSM_CTRL_ADC_START                          :  1;\
            UNSG32 uSM_CTRL_ADC_RESET                          :  1;\
            UNSG32 uSM_CTRL_TEMP_SNR_PU                        :  1;\
            UNSG32 RSVDx14_b14                                 : 18;\
          }
    union { UNSG32 u32smSysCtl_SM_CTRL;
            struct w32smSysCtl_SM_CTRL;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_ADC_STATUS_CH0_DATA_RDY(r32)    _BFGET_(r32, 0, 0)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH0_DATA_RDY(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH0_DATA_RDY(r16)    _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH0_DATA_RDY(r16,v)  _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH1_DATA_RDY(r32)    _BFGET_(r32, 1, 1)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH1_DATA_RDY(r32,v)  _BFSET_(r32, 1, 1,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH1_DATA_RDY(r16)    _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH1_DATA_RDY(r16,v)  _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH2_DATA_RDY(r32)    _BFGET_(r32, 2, 2)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH2_DATA_RDY(r32,v)  _BFSET_(r32, 2, 2,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH2_DATA_RDY(r16)    _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH2_DATA_RDY(r16,v)  _BFSET_(r16, 2, 2,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH3_DATA_RDY(r32)    _BFGET_(r32, 3, 3)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH3_DATA_RDY(r32,v)  _BFSET_(r32, 3, 3,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH3_DATA_RDY(r16)    _BFGET_(r16, 3, 3)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH3_DATA_RDY(r16,v)  _BFSET_(r16, 3, 3,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH4_DATA_RDY(r32)    _BFGET_(r32, 4, 4)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH4_DATA_RDY(r32,v)  _BFSET_(r32, 4, 4,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH4_DATA_RDY(r16)    _BFGET_(r16, 4, 4)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH4_DATA_RDY(r16,v)  _BFSET_(r16, 4, 4,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH5_DATA_RDY(r32)    _BFGET_(r32, 5, 5)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH5_DATA_RDY(r32,v)  _BFSET_(r32, 5, 5,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH5_DATA_RDY(r16)    _BFGET_(r16, 5, 5)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH5_DATA_RDY(r16,v)  _BFSET_(r16, 5, 5,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH6_DATA_RDY(r32)    _BFGET_(r32, 6, 6)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH6_DATA_RDY(r32,v)  _BFSET_(r32, 6, 6,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH6_DATA_RDY(r16)    _BFGET_(r16, 6, 6)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH6_DATA_RDY(r16,v)  _BFSET_(r16, 6, 6,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH7_DATA_RDY(r32)    _BFGET_(r32, 7, 7)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH7_DATA_RDY(r32,v)  _BFSET_(r32, 7, 7,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH7_DATA_RDY(r16)    _BFGET_(r16, 7, 7)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH7_DATA_RDY(r16,v)  _BFSET_(r16, 7, 7,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_REV_0(r32)           _BFGET_(r32,15, 8)
    #define   SET32smSysCtl_SM_ADC_STATUS_REV_0(r32,v)         _BFSET_(r32,15, 8,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_REV_0(r16)           _BFGET_(r16,15, 8)
    #define   SET16smSysCtl_SM_ADC_STATUS_REV_0(r16,v)         _BFSET_(r16,15, 8,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH0_INT_EN(r32)      _BFGET_(r32,16,16)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH0_INT_EN(r32,v)    _BFSET_(r32,16,16,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH0_INT_EN(r16)      _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH0_INT_EN(r16,v)    _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH1_INT_EN(r32)      _BFGET_(r32,17,17)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH1_INT_EN(r32,v)    _BFSET_(r32,17,17,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH1_INT_EN(r16)      _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH1_INT_EN(r16,v)    _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH2_INT_EN(r32)      _BFGET_(r32,18,18)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH2_INT_EN(r32,v)    _BFSET_(r32,18,18,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH2_INT_EN(r16)      _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH2_INT_EN(r16,v)    _BFSET_(r16, 2, 2,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH3_INT_EN(r32)      _BFGET_(r32,19,19)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH3_INT_EN(r32,v)    _BFSET_(r32,19,19,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH3_INT_EN(r16)      _BFGET_(r16, 3, 3)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH3_INT_EN(r16,v)    _BFSET_(r16, 3, 3,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH4_INT_EN(r32)      _BFGET_(r32,20,20)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH4_INT_EN(r32,v)    _BFSET_(r32,20,20,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH4_INT_EN(r16)      _BFGET_(r16, 4, 4)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH4_INT_EN(r16,v)    _BFSET_(r16, 4, 4,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH5_INT_EN(r32)      _BFGET_(r32,21,21)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH5_INT_EN(r32,v)    _BFSET_(r32,21,21,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH5_INT_EN(r16)      _BFGET_(r16, 5, 5)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH5_INT_EN(r16,v)    _BFSET_(r16, 5, 5,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH6_INT_EN(r32)      _BFGET_(r32,22,22)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH6_INT_EN(r32,v)    _BFSET_(r32,22,22,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH6_INT_EN(r16)      _BFGET_(r16, 6, 6)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH6_INT_EN(r16,v)    _BFSET_(r16, 6, 6,v)

    #define   GET32smSysCtl_SM_ADC_STATUS_CH7_INT_EN(r32)      _BFGET_(r32,23,23)
    #define   SET32smSysCtl_SM_ADC_STATUS_CH7_INT_EN(r32,v)    _BFSET_(r32,23,23,v)
    #define   GET16smSysCtl_SM_ADC_STATUS_CH7_INT_EN(r16)      _BFGET_(r16, 7, 7)
    #define   SET16smSysCtl_SM_ADC_STATUS_CH7_INT_EN(r16,v)    _BFSET_(r16, 7, 7,v)

    #define     w32smSysCtl_SM_ADC_STATUS                      {\
            UNSG32 uSM_ADC_STATUS_CH0_DATA_RDY                 :  1;\
            UNSG32 uSM_ADC_STATUS_CH1_DATA_RDY                 :  1;\
            UNSG32 uSM_ADC_STATUS_CH2_DATA_RDY                 :  1;\
            UNSG32 uSM_ADC_STATUS_CH3_DATA_RDY                 :  1;\
            UNSG32 uSM_ADC_STATUS_CH4_DATA_RDY                 :  1;\
            UNSG32 uSM_ADC_STATUS_CH5_DATA_RDY                 :  1;\
            UNSG32 uSM_ADC_STATUS_CH6_DATA_RDY                 :  1;\
            UNSG32 uSM_ADC_STATUS_CH7_DATA_RDY                 :  1;\
            UNSG32 uSM_ADC_STATUS_REV_0                        :  8;\
            UNSG32 uSM_ADC_STATUS_CH0_INT_EN                   :  1;\
            UNSG32 uSM_ADC_STATUS_CH1_INT_EN                   :  1;\
            UNSG32 uSM_ADC_STATUS_CH2_INT_EN                   :  1;\
            UNSG32 uSM_ADC_STATUS_CH3_INT_EN                   :  1;\
            UNSG32 uSM_ADC_STATUS_CH4_INT_EN                   :  1;\
            UNSG32 uSM_ADC_STATUS_CH5_INT_EN                   :  1;\
            UNSG32 uSM_ADC_STATUS_CH6_INT_EN                   :  1;\
            UNSG32 uSM_ADC_STATUS_CH7_INT_EN                   :  1;\
            UNSG32 RSVDx18_b24                                 :  8;\
          }
    union { UNSG32 u32smSysCtl_SM_ADC_STATUS;
            struct w32smSysCtl_SM_ADC_STATUS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_ADC_DATA_ADC_DATA(r32)          _BFGET_(r32, 9, 0)
    #define   SET32smSysCtl_SM_ADC_DATA_ADC_DATA(r32,v)        _BFSET_(r32, 9, 0,v)
    #define   GET16smSysCtl_SM_ADC_DATA_ADC_DATA(r16)          _BFGET_(r16, 9, 0)
    #define   SET16smSysCtl_SM_ADC_DATA_ADC_DATA(r16,v)        _BFSET_(r16, 9, 0,v)

    #define     w32smSysCtl_SM_ADC_DATA                        {\
            UNSG32 uSM_ADC_DATA_ADC_DATA                       : 10;\
            UNSG32 RSVDx1C_b10                                 : 22;\
          }
    union { UNSG32 u32smSysCtl_SM_ADC_DATA;
            struct w32smSysCtl_SM_ADC_DATA;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_BOOT_STATUS_MODE(r32)           _BFGET_(r32,31, 0)
    #define   SET32smSysCtl_SM_BOOT_STATUS_MODE(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32smSysCtl_SM_BOOT_STATUS                     {\
            UNSG32 uSM_BOOT_STATUS_MODE                        : 32;\
          }
    union { UNSG32 u32smSysCtl_SM_BOOT_STATUS;
            struct w32smSysCtl_SM_BOOT_STATUS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_WDT_MASK_SM_RST(r32)            _BFGET_(r32, 2, 0)
    #define   SET32smSysCtl_SM_WDT_MASK_SM_RST(r32,v)          _BFSET_(r32, 2, 0,v)
    #define   GET16smSysCtl_SM_WDT_MASK_SM_RST(r16)            _BFGET_(r16, 2, 0)
    #define   SET16smSysCtl_SM_WDT_MASK_SM_RST(r16,v)          _BFSET_(r16, 2, 0,v)

    #define   GET32smSysCtl_SM_WDT_MASK_SOC_RST(r32)           _BFGET_(r32, 5, 3)
    #define   SET32smSysCtl_SM_WDT_MASK_SOC_RST(r32,v)         _BFSET_(r32, 5, 3,v)
    #define   GET16smSysCtl_SM_WDT_MASK_SOC_RST(r16)           _BFGET_(r16, 5, 3)
    #define   SET16smSysCtl_SM_WDT_MASK_SOC_RST(r16,v)         _BFSET_(r16, 5, 3,v)

    #define     w32smSysCtl_SM_WDT_MASK                        {\
            UNSG32 uSM_WDT_MASK_SM_RST                         :  3;\
            UNSG32 uSM_WDT_MASK_SOC_RST                        :  3;\
            UNSG32 RSVDx24_b6                                  : 26;\
          }
    union { UNSG32 u32smSysCtl_SM_WDT_MASK;
            struct w32smSysCtl_SM_WDT_MASK;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_smAnaGrpCtl_puIVref(r32)           _BFGET_(r32, 0, 0)
    #define   SET32smSysCtl_smAnaGrpCtl_puIVref(r32,v)         _BFSET_(r32, 0, 0,v)
    #define   GET16smSysCtl_smAnaGrpCtl_puIVref(r16)           _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_smAnaGrpCtl_puIVref(r16,v)         _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_smAnaGrpCtl_bgrSel(r32)            _BFGET_(r32, 1, 1)
    #define   SET32smSysCtl_smAnaGrpCtl_bgrSel(r32,v)          _BFSET_(r32, 1, 1,v)
    #define   GET16smSysCtl_smAnaGrpCtl_bgrSel(r16)            _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_smAnaGrpCtl_bgrSel(r16,v)          _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_smAnaGrpCtl_puXtl(r32)             _BFGET_(r32, 2, 2)
    #define   SET32smSysCtl_smAnaGrpCtl_puXtl(r32,v)           _BFSET_(r32, 2, 2,v)
    #define   GET16smSysCtl_smAnaGrpCtl_puXtl(r16)             _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_smAnaGrpCtl_puXtl(r16,v)           _BFSET_(r16, 2, 2,v)

    #define   GET32smSysCtl_smAnaGrpCtl_bypass(r32)            _BFGET_(r32, 3, 3)
    #define   SET32smSysCtl_smAnaGrpCtl_bypass(r32,v)          _BFSET_(r32, 3, 3,v)
    #define   GET16smSysCtl_smAnaGrpCtl_bypass(r16)            _BFGET_(r16, 3, 3)
    #define   SET16smSysCtl_smAnaGrpCtl_bypass(r16,v)          _BFSET_(r16, 3, 3,v)

    #define   GET32smSysCtl_smAnaGrpCtl_gainX2(r32)            _BFGET_(r32, 4, 4)
    #define   SET32smSysCtl_smAnaGrpCtl_gainX2(r32,v)          _BFSET_(r32, 4, 4,v)
    #define   GET16smSysCtl_smAnaGrpCtl_gainX2(r16)            _BFGET_(r16, 4, 4)
    #define   SET16smSysCtl_smAnaGrpCtl_gainX2(r16,v)          _BFSET_(r16, 4, 4,v)

    #define   GET32smSysCtl_smAnaGrpCtl_testAna(r32)           _BFGET_(r32, 7, 5)
    #define   SET32smSysCtl_smAnaGrpCtl_testAna(r32,v)         _BFSET_(r32, 7, 5,v)
    #define   GET16smSysCtl_smAnaGrpCtl_testAna(r16)           _BFGET_(r16, 7, 5)
    #define   SET16smSysCtl_smAnaGrpCtl_testAna(r16,v)         _BFSET_(r16, 7, 5,v)

    #define     w32smSysCtl_smAnaGrpCtl                        {\
            UNSG32 usmAnaGrpCtl_puIVref                        :  1;\
            UNSG32 usmAnaGrpCtl_bgrSel                         :  1;\
            UNSG32 usmAnaGrpCtl_puXtl                          :  1;\
            UNSG32 usmAnaGrpCtl_bypass                         :  1;\
            UNSG32 usmAnaGrpCtl_gainX2                         :  1;\
            UNSG32 usmAnaGrpCtl_testAna                        :  3;\
            UNSG32 RSVDx28_b8                                  : 24;\
          }
    union { UNSG32 u32smSysCtl_smAnaGrpCtl;
            struct w32smSysCtl_smAnaGrpCtl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_GSM_SEL_GSM0_SEL(r32)           _BFGET_(r32, 1, 0)
    #define   SET32smSysCtl_SM_GSM_SEL_GSM0_SEL(r32,v)         _BFSET_(r32, 1, 0,v)
    #define   GET16smSysCtl_SM_GSM_SEL_GSM0_SEL(r16)           _BFGET_(r16, 1, 0)
    #define   SET16smSysCtl_SM_GSM_SEL_GSM0_SEL(r16,v)         _BFSET_(r16, 1, 0,v)

    #define   GET32smSysCtl_SM_GSM_SEL_GSM1_SEL(r32)           _BFGET_(r32, 3, 2)
    #define   SET32smSysCtl_SM_GSM_SEL_GSM1_SEL(r32,v)         _BFSET_(r32, 3, 2,v)
    #define   GET16smSysCtl_SM_GSM_SEL_GSM1_SEL(r16)           _BFGET_(r16, 3, 2)
    #define   SET16smSysCtl_SM_GSM_SEL_GSM1_SEL(r16,v)         _BFSET_(r16, 3, 2,v)

    #define   GET32smSysCtl_SM_GSM_SEL_GSM2_SEL(r32)           _BFGET_(r32, 5, 4)
    #define   SET32smSysCtl_SM_GSM_SEL_GSM2_SEL(r32,v)         _BFSET_(r32, 5, 4,v)
    #define   GET16smSysCtl_SM_GSM_SEL_GSM2_SEL(r16)           _BFGET_(r16, 5, 4)
    #define   SET16smSysCtl_SM_GSM_SEL_GSM2_SEL(r16,v)         _BFSET_(r16, 5, 4,v)

    #define   GET32smSysCtl_SM_GSM_SEL_GSM3_SEL(r32)           _BFGET_(r32, 7, 6)
    #define   SET32smSysCtl_SM_GSM_SEL_GSM3_SEL(r32,v)         _BFSET_(r32, 7, 6,v)
    #define   GET16smSysCtl_SM_GSM_SEL_GSM3_SEL(r16)           _BFGET_(r16, 7, 6)
    #define   SET16smSysCtl_SM_GSM_SEL_GSM3_SEL(r16,v)         _BFSET_(r16, 7, 6,v)

    #define   GET32smSysCtl_SM_GSM_SEL_GSM4_SEL(r32)           _BFGET_(r32, 9, 8)
    #define   SET32smSysCtl_SM_GSM_SEL_GSM4_SEL(r32,v)         _BFSET_(r32, 9, 8,v)
    #define   GET16smSysCtl_SM_GSM_SEL_GSM4_SEL(r16)           _BFGET_(r16, 9, 8)
    #define   SET16smSysCtl_SM_GSM_SEL_GSM4_SEL(r16,v)         _BFSET_(r16, 9, 8,v)

    #define   GET32smSysCtl_SM_GSM_SEL_GSM5_SEL(r32)           _BFGET_(r32,11,10)
    #define   SET32smSysCtl_SM_GSM_SEL_GSM5_SEL(r32,v)         _BFSET_(r32,11,10,v)
    #define   GET16smSysCtl_SM_GSM_SEL_GSM5_SEL(r16)           _BFGET_(r16,11,10)
    #define   SET16smSysCtl_SM_GSM_SEL_GSM5_SEL(r16,v)         _BFSET_(r16,11,10,v)

    #define   GET32smSysCtl_SM_GSM_SEL_GSM6_SEL(r32)           _BFGET_(r32,13,12)
    #define   SET32smSysCtl_SM_GSM_SEL_GSM6_SEL(r32,v)         _BFSET_(r32,13,12,v)
    #define   GET16smSysCtl_SM_GSM_SEL_GSM6_SEL(r16)           _BFGET_(r16,13,12)
    #define   SET16smSysCtl_SM_GSM_SEL_GSM6_SEL(r16,v)         _BFSET_(r16,13,12,v)

    #define   GET32smSysCtl_SM_GSM_SEL_GSM7_SEL(r32)           _BFGET_(r32,14,14)
    #define   SET32smSysCtl_SM_GSM_SEL_GSM7_SEL(r32,v)         _BFSET_(r32,14,14,v)
    #define   GET16smSysCtl_SM_GSM_SEL_GSM7_SEL(r16)           _BFGET_(r16,14,14)
    #define   SET16smSysCtl_SM_GSM_SEL_GSM7_SEL(r16,v)         _BFSET_(r16,14,14,v)

    #define   GET32smSysCtl_SM_GSM_SEL_GSM8_SEL(r32)           _BFGET_(r32,15,15)
    #define   SET32smSysCtl_SM_GSM_SEL_GSM8_SEL(r32,v)         _BFSET_(r32,15,15,v)
    #define   GET16smSysCtl_SM_GSM_SEL_GSM8_SEL(r16)           _BFGET_(r16,15,15)
    #define   SET16smSysCtl_SM_GSM_SEL_GSM8_SEL(r16,v)         _BFSET_(r16,15,15,v)

    #define     w32smSysCtl_SM_GSM_SEL                         {\
            UNSG32 uSM_GSM_SEL_GSM0_SEL                        :  2;\
            UNSG32 uSM_GSM_SEL_GSM1_SEL                        :  2;\
            UNSG32 uSM_GSM_SEL_GSM2_SEL                        :  2;\
            UNSG32 uSM_GSM_SEL_GSM3_SEL                        :  2;\
            UNSG32 uSM_GSM_SEL_GSM4_SEL                        :  2;\
            UNSG32 uSM_GSM_SEL_GSM5_SEL                        :  2;\
            UNSG32 uSM_GSM_SEL_GSM6_SEL                        :  2;\
            UNSG32 uSM_GSM_SEL_GSM7_SEL                        :  1;\
            UNSG32 uSM_GSM_SEL_GSM8_SEL                        :  1;\
            UNSG32 RSVDx2C_b16                                 : 16;\
          }
    union { UNSG32 u32smSysCtl_SM_GSM_SEL;
            struct w32smSysCtl_SM_GSM_SEL;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_CORE_CTRL_DVDDO_DI_SMANA0_V25EN_CORE(r32) _BFGET_(r32, 0, 0)
    #define   SET32smSysCtl_SM_CORE_CTRL_DVDDO_DI_SMANA0_V25EN_CORE(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16smSysCtl_SM_CORE_CTRL_DVDDO_DI_SMANA0_V25EN_CORE(r16) _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_CORE_CTRL_DVDDO_DI_SMANA0_V25EN_CORE(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_CORE_CTRL_DVDD_IO1_SMGPIO2_V25EN_CORE(r32) _BFGET_(r32, 1, 1)
    #define   SET32smSysCtl_SM_CORE_CTRL_DVDD_IO1_SMGPIO2_V25EN_CORE(r32,v) _BFSET_(r32, 1, 1,v)
    #define   GET16smSysCtl_SM_CORE_CTRL_DVDD_IO1_SMGPIO2_V25EN_CORE(r16) _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_CORE_CTRL_DVDD_IO1_SMGPIO2_V25EN_CORE(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_SM_CORE_CTRL_DVDD_IO2_SMGPIO0_V25EN_CORE(r32) _BFGET_(r32, 2, 2)
    #define   SET32smSysCtl_SM_CORE_CTRL_DVDD_IO2_SMGPIO0_V25EN_CORE(r32,v) _BFSET_(r32, 2, 2,v)
    #define   GET16smSysCtl_SM_CORE_CTRL_DVDD_IO2_SMGPIO0_V25EN_CORE(r16) _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_CORE_CTRL_DVDD_IO2_SMGPIO0_V25EN_CORE(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32smSysCtl_SM_CORE_CTRL_V18_SELECT_SMANA0_V18EN_CORE(r32) _BFGET_(r32, 3, 3)
    #define   SET32smSysCtl_SM_CORE_CTRL_V18_SELECT_SMANA0_V18EN_CORE(r32,v) _BFSET_(r32, 3, 3,v)
    #define   GET16smSysCtl_SM_CORE_CTRL_V18_SELECT_SMANA0_V18EN_CORE(r16) _BFGET_(r16, 3, 3)
    #define   SET16smSysCtl_SM_CORE_CTRL_V18_SELECT_SMANA0_V18EN_CORE(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32smSysCtl_SM_CORE_CTRL_V18_SELECT_SMGBL0_V18EN_CORE(r32) _BFGET_(r32, 4, 4)
    #define   SET32smSysCtl_SM_CORE_CTRL_V18_SELECT_SMGBL0_V18EN_CORE(r32,v) _BFSET_(r32, 4, 4,v)
    #define   GET16smSysCtl_SM_CORE_CTRL_V18_SELECT_SMGBL0_V18EN_CORE(r16) _BFGET_(r16, 4, 4)
    #define   SET16smSysCtl_SM_CORE_CTRL_V18_SELECT_SMGBL0_V18EN_CORE(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZN(r32) _BFGET_(r32, 7, 5)
    #define   SET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZN(r32,v) _BFSET_(r32, 7, 5,v)
    #define   GET16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZN(r16) _BFGET_(r16, 7, 5)
    #define   SET16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZN(r16,v) _BFSET_(r16, 7, 5,v)

    #define   GET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZP(r32) _BFGET_(r32,10, 8)
    #define   SET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZP(r32,v) _BFSET_(r32,10, 8,v)
    #define   GET16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZP(r16) _BFGET_(r16,10, 8)
    #define   SET16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMANA0_ZP(r16,v) _BFSET_(r16,10, 8,v)

    #define   GET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZN(r32) _BFGET_(r32,13,11)
    #define   SET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZN(r32,v) _BFSET_(r32,13,11,v)
    #define   GET16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZN(r16) _BFGET_(r16,13,11)
    #define   SET16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZN(r16,v) _BFSET_(r16,13,11,v)

    #define   GET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZP(r32) _BFGET_(r32,16,14)
    #define   SET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZP(r32,v) _BFSET_(r32,16,14,v)

    #define   GET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZN(r32) _BFGET_(r32,19,17)
    #define   SET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZN(r32,v) _BFSET_(r32,19,17,v)
    #define   GET16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZN(r16) _BFGET_(r16, 3, 1)
    #define   SET16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZN(r16,v) _BFSET_(r16, 3, 1,v)

    #define   GET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZP(r32) _BFGET_(r32,22,20)
    #define   SET32smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZP(r32,v) _BFSET_(r32,22,20,v)
    #define   GET16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZP(r16) _BFGET_(r16, 6, 4)
    #define   SET16smSysCtl_SM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZP(r16,v) _BFSET_(r16, 6, 4,v)

    #define     w32smSysCtl_SM_CORE_CTRL                       {\
            UNSG32 uSM_CORE_CTRL_DVDDO_DI_SMANA0_V25EN_CORE    :  1;\
            UNSG32 uSM_CORE_CTRL_DVDD_IO1_SMGPIO2_V25EN_CORE   :  1;\
            UNSG32 uSM_CORE_CTRL_DVDD_IO2_SMGPIO0_V25EN_CORE   :  1;\
            UNSG32 uSM_CORE_CTRL_V18_SELECT_SMANA0_V18EN_CORE  :  1;\
            UNSG32 uSM_CORE_CTRL_V18_SELECT_SMGBL0_V18EN_CORE  :  1;\
            UNSG32 uSM_CORE_CTRL_ZNR_ZPR_SMANA0_ZN             :  3;\
            UNSG32 uSM_CORE_CTRL_ZNR_ZPR_SMANA0_ZP             :  3;\
            UNSG32 uSM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZN             :  3;\
            UNSG32 uSM_CORE_CTRL_ZNR_ZPR_SMGBL0_ZP             :  3;\
            UNSG32 uSM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZN            :  3;\
            UNSG32 uSM_CORE_CTRL_ZNR_ZPR_SMGPIO0_ZP            :  3;\
            UNSG32 RSVDx30_b23                                 :  9;\
          }
    union { UNSG32 u32smSysCtl_SM_CORE_CTRL;
            struct w32smSysCtl_SM_CORE_CTRL;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_ROSC(r32)     _BFGET_(r32, 0, 0)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_ROSC(r32,v)   _BFSET_(r32, 0, 0,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_ROSC(r16)     _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_ROSC(r16,v)   _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTIn(r32)    _BFGET_(r32, 1, 1)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTIn(r32,v)  _BFSET_(r32, 1, 1,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTIn(r16)    _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTIn(r16,v)  _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTOn(r32)    _BFGET_(r32, 2, 2)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTOn(r32,v)  _BFSET_(r32, 2, 2,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTOn(r16)    _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_RSTOn(r16,v)  _BFSET_(r16, 2, 2,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TCK(r32)      _BFGET_(r32, 3, 3)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TCK(r32,v)    _BFSET_(r32, 3, 3,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TCK(r16)      _BFGET_(r16, 3, 3)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TCK(r16,v)    _BFSET_(r16, 3, 3,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TRSTn(r32)    _BFGET_(r32, 4, 4)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TRSTn(r32,v)  _BFSET_(r32, 4, 4,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TRSTn(r16)    _BFGET_(r16, 4, 4)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TRSTn(r16,v)  _BFSET_(r16, 4, 4,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TMS(r32)      _BFGET_(r32, 5, 5)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TMS(r32,v)    _BFSET_(r32, 5, 5,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TMS(r16)      _BFGET_(r16, 5, 5)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TMS(r16,v)    _BFSET_(r16, 5, 5,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDI(r32)      _BFGET_(r32, 6, 6)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDI(r32,v)    _BFSET_(r32, 6, 6,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDI(r16)      _BFGET_(r16, 6, 6)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDI(r16,v)    _BFSET_(r16, 6, 6,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDO(r32)      _BFGET_(r32, 7, 7)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDO(r32,v)    _BFSET_(r32, 7, 7,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDO(r16)      _BFGET_(r16, 7, 7)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TDO(r16,v)    _BFSET_(r16, 7, 7,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SOC_RSTOn(r32) _BFGET_(r32, 8, 8)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SOC_RSTOn(r32,v) _BFSET_(r32, 8, 8,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SOC_RSTOn(r16) _BFGET_(r16, 8, 8)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SOC_RSTOn(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TEST_EN(r32)  _BFGET_(r32, 9, 9)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_TEST_EN(r32,v) _BFSET_(r32, 9, 9,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TEST_EN(r16)  _BFGET_(r16, 9, 9)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_TEST_EN(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_0(r32)   _BFGET_(r32,10,10)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_0(r32,v) _BFSET_(r32,10,10,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_0(r16)   _BFGET_(r16,10,10)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_0(r16,v) _BFSET_(r16,10,10,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_1(r32)   _BFGET_(r32,11,11)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_1(r32,v) _BFSET_(r32,11,11,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_1(r16)   _BFGET_(r16,11,11)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_JSEL_1(r16,v) _BFSET_(r16,11,11,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_0(r32)   _BFGET_(r32,12,12)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_0(r32,v) _BFSET_(r32,12,12,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_0(r16)   _BFGET_(r16,12,12)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_0(r16,v) _BFSET_(r16,12,12,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_0(r32)    _BFGET_(r32,13,13)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_0(r32,v)  _BFSET_(r32,13,13,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_0(r16)    _BFGET_(r16,13,13)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_0(r16,v)  _BFSET_(r16,13,13,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_0(r32)    _BFGET_(r32,14,14)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_0(r32,v)  _BFSET_(r32,14,14,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_0(r16)    _BFGET_(r16,14,14)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_0(r16,v)  _BFSET_(r16,14,14,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_1(r32)   _BFGET_(r32,15,15)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_1(r32,v) _BFSET_(r32,15,15,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_1(r16)   _BFGET_(r16,15,15)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_1(r16,v) _BFSET_(r16,15,15,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_1(r32)    _BFGET_(r32,16,16)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_1(r32,v)  _BFSET_(r32,16,16,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_1(r16)    _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_1(r16,v)  _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_1(r32)    _BFGET_(r32,17,17)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_1(r32,v)  _BFSET_(r32,17,17,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_1(r16)    _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_1(r16,v)  _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_2(r32)   _BFGET_(r32,18,18)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_2(r32,v) _BFSET_(r32,18,18,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_2(r16)   _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_2(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_2(r32)    _BFGET_(r32,19,19)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_2(r32,v)  _BFSET_(r32,19,19,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_2(r16)    _BFGET_(r16, 3, 3)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_2(r16,v)  _BFSET_(r16, 3, 3,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_2(r32)    _BFGET_(r32,20,20)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_2(r32,v)  _BFSET_(r32,20,20,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_2(r16)    _BFGET_(r16, 4, 4)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_2(r16,v)  _BFSET_(r16, 4, 4,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_3(r32)   _BFGET_(r32,21,21)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_3(r32,v) _BFSET_(r32,21,21,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_3(r16)   _BFGET_(r16, 5, 5)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_GPIO_3(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_3(r32)    _BFGET_(r32,22,22)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_3(r32,v)  _BFSET_(r32,22,22,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_3(r16)    _BFGET_(r16, 6, 6)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PU_EN_GPIO_3(r16,v)  _BFSET_(r16, 6, 6,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_3(r32)    _BFGET_(r32,23,23)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_3(r32,v)  _BFSET_(r32,23,23,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_3(r16)    _BFGET_(r16, 7, 7)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PD_EN_GPIO_3(r16,v)  _BFSET_(r16, 7, 7,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S0n(r32) _BFGET_(r32,24,24)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S0n(r32,v) _BFSET_(r32,24,24,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S0n(r16) _BFGET_(r16, 8, 8)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S0n(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S0n(r32)  _BFGET_(r32,25,25)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S0n(r32,v) _BFSET_(r32,25,25,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S0n(r16)  _BFGET_(r16, 9, 9)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S0n(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S0n(r32)  _BFGET_(r32,26,26)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S0n(r32,v) _BFSET_(r32,26,26,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S0n(r16)  _BFGET_(r16,10,10)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S0n(r16,v) _BFSET_(r16,10,10,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S1n(r32) _BFGET_(r32,27,27)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S1n(r32,v) _BFSET_(r32,27,27,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S1n(r16) _BFGET_(r16,11,11)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S1n(r16,v) _BFSET_(r16,11,11,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S1n(r32)  _BFGET_(r32,28,28)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S1n(r32,v) _BFSET_(r32,28,28,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S1n(r16)  _BFGET_(r16,12,12)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S1n(r16,v) _BFSET_(r16,12,12,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S1n(r32)  _BFGET_(r32,29,29)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S1n(r32,v) _BFSET_(r32,29,29,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S1n(r16)  _BFGET_(r16,13,13)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PD_EN_SPI2_S1n(r16,v) _BFSET_(r16,13,13,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S2n(r32) _BFGET_(r32,30,30)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S2n(r32,v) _BFSET_(r32,30,30,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S2n(r16) _BFGET_(r16,14,14)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_SLP_DI_SPI2_S2n(r16,v) _BFSET_(r16,14,14,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S2n(r32)  _BFGET_(r32,31,31)
    #define   SET32smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S2n(r32,v) _BFSET_(r32,31,31,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S2n(r16)  _BFGET_(r16,15,15)
    #define   SET16smSysCtl_SM_PAD_CTRL_0_PU_EN_SPI2_S2n(r16,v) _BFSET_(r16,15,15,v)

    #define     w32smSysCtl_SM_PAD_CTRL_0                      {\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_ROSC                  :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_RSTIn                 :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_RSTOn                 :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_TCK                   :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_TRSTn                 :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_TMS                   :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_TDI                   :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_TDO                   :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_SOC_RSTOn             :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_TEST_EN               :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_JSEL_0                :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_JSEL_1                :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_GPIO_0                :  1;\
            UNSG32 uSM_PAD_CTRL_0_PU_EN_GPIO_0                 :  1;\
            UNSG32 uSM_PAD_CTRL_0_PD_EN_GPIO_0                 :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_GPIO_1                :  1;\
            UNSG32 uSM_PAD_CTRL_0_PU_EN_GPIO_1                 :  1;\
            UNSG32 uSM_PAD_CTRL_0_PD_EN_GPIO_1                 :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_GPIO_2                :  1;\
            UNSG32 uSM_PAD_CTRL_0_PU_EN_GPIO_2                 :  1;\
            UNSG32 uSM_PAD_CTRL_0_PD_EN_GPIO_2                 :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_GPIO_3                :  1;\
            UNSG32 uSM_PAD_CTRL_0_PU_EN_GPIO_3                 :  1;\
            UNSG32 uSM_PAD_CTRL_0_PD_EN_GPIO_3                 :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_SPI2_S0n              :  1;\
            UNSG32 uSM_PAD_CTRL_0_PU_EN_SPI2_S0n               :  1;\
            UNSG32 uSM_PAD_CTRL_0_PD_EN_SPI2_S0n               :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_SPI2_S1n              :  1;\
            UNSG32 uSM_PAD_CTRL_0_PU_EN_SPI2_S1n               :  1;\
            UNSG32 uSM_PAD_CTRL_0_PD_EN_SPI2_S1n               :  1;\
            UNSG32 uSM_PAD_CTRL_0_SLP_DI_SPI2_S2n              :  1;\
            UNSG32 uSM_PAD_CTRL_0_PU_EN_SPI2_S2n               :  1;\
          }
    union { UNSG32 u32smSysCtl_SM_PAD_CTRL_0;
            struct w32smSysCtl_SM_PAD_CTRL_0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S2n(r32)  _BFGET_(r32, 0, 0)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S2n(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S2n(r16)  _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S2n(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_S3n(r32) _BFGET_(r32, 1, 1)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_S3n(r32,v) _BFSET_(r32, 1, 1,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_S3n(r16) _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_S3n(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_S3n(r32)  _BFGET_(r32, 2, 2)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_S3n(r32,v) _BFSET_(r32, 2, 2,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_S3n(r16)  _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_S3n(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S3n(r32)  _BFGET_(r32, 3, 3)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S3n(r32,v) _BFSET_(r32, 3, 3,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S3n(r16)  _BFGET_(r16, 3, 3)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_S3n(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SCK(r32) _BFGET_(r32, 4, 4)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SCK(r32,v) _BFSET_(r32, 4, 4,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SCK(r16) _BFGET_(r16, 4, 4)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SCK(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SCK(r32)  _BFGET_(r32, 5, 5)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SCK(r32,v) _BFSET_(r32, 5, 5,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SCK(r16)  _BFGET_(r16, 5, 5)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SCK(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SCK(r32)  _BFGET_(r32, 6, 6)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SCK(r32,v) _BFSET_(r32, 6, 6,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SCK(r16)  _BFGET_(r16, 6, 6)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SCK(r16,v) _BFSET_(r16, 6, 6,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDO(r32) _BFGET_(r32, 7, 7)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDO(r32,v) _BFSET_(r32, 7, 7,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDO(r16) _BFGET_(r16, 7, 7)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDO(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDO(r32)  _BFGET_(r32, 8, 8)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDO(r32,v) _BFSET_(r32, 8, 8,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDO(r16)  _BFGET_(r16, 8, 8)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDO(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDO(r32)  _BFGET_(r32, 9, 9)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDO(r32,v) _BFSET_(r32, 9, 9,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDO(r16)  _BFGET_(r16, 9, 9)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDO(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDI(r32) _BFGET_(r32,10,10)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDI(r32,v) _BFSET_(r32,10,10,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDI(r16) _BFGET_(r16,10,10)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_SPI2_SDI(r16,v) _BFSET_(r16,10,10,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDI(r32)  _BFGET_(r32,11,11)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDI(r32,v) _BFSET_(r32,11,11,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDI(r16)  _BFGET_(r16,11,11)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PU_EN_SPI2_SDI(r16,v) _BFSET_(r16,11,11,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDI(r32)  _BFGET_(r32,12,12)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDI(r32,v) _BFSET_(r32,12,12,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDI(r16)  _BFGET_(r16,12,12)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PD_EN_SPI2_SDI(r16,v) _BFSET_(r16,12,12,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RXD(r32)  _BFGET_(r32,13,13)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RXD(r32,v) _BFSET_(r32,13,13,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RXD(r16)  _BFGET_(r16,13,13)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RXD(r16,v) _BFSET_(r16,13,13,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RXD(r32)   _BFGET_(r32,14,14)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RXD(r32,v) _BFSET_(r32,14,14,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RXD(r16)   _BFGET_(r16,14,14)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RXD(r16,v) _BFSET_(r16,14,14,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RXD(r32)   _BFGET_(r32,15,15)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RXD(r32,v) _BFSET_(r32,15,15,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RXD(r16)   _BFGET_(r16,15,15)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RXD(r16,v) _BFSET_(r16,15,15,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_TXD(r32)  _BFGET_(r32,16,16)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_TXD(r32,v) _BFSET_(r32,16,16,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_TXD(r16)  _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_TXD(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_TXD(r32)   _BFGET_(r32,17,17)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_TXD(r32,v) _BFSET_(r32,17,17,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_TXD(r16)   _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_TXD(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_TXD(r32)   _BFGET_(r32,18,18)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_TXD(r32,v) _BFSET_(r32,18,18,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_TXD(r16)   _BFGET_(r16, 2, 2)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_TXD(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_CTSn(r32) _BFGET_(r32,19,19)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_CTSn(r32,v) _BFSET_(r32,19,19,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_CTSn(r16) _BFGET_(r16, 3, 3)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_CTSn(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_CTSn(r32)  _BFGET_(r32,20,20)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_CTSn(r32,v) _BFSET_(r32,20,20,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_CTSn(r16)  _BFGET_(r16, 4, 4)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_CTSn(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_CTSn(r32)  _BFGET_(r32,21,21)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_CTSn(r32,v) _BFSET_(r32,21,21,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_CTSn(r16)  _BFGET_(r16, 5, 5)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_CTSn(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RTSn(r32) _BFGET_(r32,22,22)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RTSn(r32,v) _BFSET_(r32,22,22,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RTSn(r16) _BFGET_(r16, 6, 6)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR0_RTSn(r16,v) _BFSET_(r16, 6, 6,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RTSn(r32)  _BFGET_(r32,23,23)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RTSn(r32,v) _BFSET_(r32,23,23,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RTSn(r16)  _BFGET_(r16, 7, 7)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR0_RTSn(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RTSn(r32)  _BFGET_(r32,24,24)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RTSn(r32,v) _BFSET_(r32,24,24,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RTSn(r16)  _BFGET_(r16, 8, 8)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR0_RTSn(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_RXD(r32)  _BFGET_(r32,25,25)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_RXD(r32,v) _BFSET_(r32,25,25,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_RXD(r16)  _BFGET_(r16, 9, 9)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_RXD(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_RXD(r32)   _BFGET_(r32,26,26)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_RXD(r32,v) _BFSET_(r32,26,26,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_RXD(r16)   _BFGET_(r16,10,10)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_RXD(r16,v) _BFSET_(r16,10,10,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_RXD(r32)   _BFGET_(r32,27,27)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_RXD(r32,v) _BFSET_(r32,27,27,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_RXD(r16)   _BFGET_(r16,11,11)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_RXD(r16,v) _BFSET_(r16,11,11,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_TXD(r32)  _BFGET_(r32,28,28)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_TXD(r32,v) _BFSET_(r32,28,28,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_TXD(r16)  _BFGET_(r16,12,12)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_UR1_TXD(r16,v) _BFSET_(r16,12,12,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_TXD(r32)   _BFGET_(r32,29,29)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_TXD(r32,v) _BFSET_(r32,29,29,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_TXD(r16)   _BFGET_(r16,13,13)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PU_EN_UR1_TXD(r16,v) _BFSET_(r16,13,13,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_TXD(r32)   _BFGET_(r32,30,30)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_TXD(r32,v) _BFSET_(r32,30,30,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_TXD(r16)   _BFGET_(r16,14,14)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_PD_EN_UR1_TXD(r16,v) _BFSET_(r16,14,14,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_CLKI(r32)     _BFGET_(r32,31,31)
    #define   SET32smSysCtl_SM_PAD_CTRL_1_SLP_DI_CLKI(r32,v)   _BFSET_(r32,31,31,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_CLKI(r16)     _BFGET_(r16,15,15)
    #define   SET16smSysCtl_SM_PAD_CTRL_1_SLP_DI_CLKI(r16,v)   _BFSET_(r16,15,15,v)

    #define     w32smSysCtl_SM_PAD_CTRL_1                      {\
            UNSG32 uSM_PAD_CTRL_1_PD_EN_SPI2_S2n               :  1;\
            UNSG32 uSM_PAD_CTRL_1_SLP_DI_SPI2_S3n              :  1;\
            UNSG32 uSM_PAD_CTRL_1_PU_EN_SPI2_S3n               :  1;\
            UNSG32 uSM_PAD_CTRL_1_PD_EN_SPI2_S3n               :  1;\
            UNSG32 uSM_PAD_CTRL_1_SLP_DI_SPI2_SCK              :  1;\
            UNSG32 uSM_PAD_CTRL_1_PU_EN_SPI2_SCK               :  1;\
            UNSG32 uSM_PAD_CTRL_1_PD_EN_SPI2_SCK               :  1;\
            UNSG32 uSM_PAD_CTRL_1_SLP_DI_SPI2_SDO              :  1;\
            UNSG32 uSM_PAD_CTRL_1_PU_EN_SPI2_SDO               :  1;\
            UNSG32 uSM_PAD_CTRL_1_PD_EN_SPI2_SDO               :  1;\
            UNSG32 uSM_PAD_CTRL_1_SLP_DI_SPI2_SDI              :  1;\
            UNSG32 uSM_PAD_CTRL_1_PU_EN_SPI2_SDI               :  1;\
            UNSG32 uSM_PAD_CTRL_1_PD_EN_SPI2_SDI               :  1;\
            UNSG32 uSM_PAD_CTRL_1_SLP_DI_UR0_RXD               :  1;\
            UNSG32 uSM_PAD_CTRL_1_PU_EN_UR0_RXD                :  1;\
            UNSG32 uSM_PAD_CTRL_1_PD_EN_UR0_RXD                :  1;\
            UNSG32 uSM_PAD_CTRL_1_SLP_DI_UR0_TXD               :  1;\
            UNSG32 uSM_PAD_CTRL_1_PU_EN_UR0_TXD                :  1;\
            UNSG32 uSM_PAD_CTRL_1_PD_EN_UR0_TXD                :  1;\
            UNSG32 uSM_PAD_CTRL_1_SLP_DI_UR0_CTSn              :  1;\
            UNSG32 uSM_PAD_CTRL_1_PU_EN_UR0_CTSn               :  1;\
            UNSG32 uSM_PAD_CTRL_1_PD_EN_UR0_CTSn               :  1;\
            UNSG32 uSM_PAD_CTRL_1_SLP_DI_UR0_RTSn              :  1;\
            UNSG32 uSM_PAD_CTRL_1_PU_EN_UR0_RTSn               :  1;\
            UNSG32 uSM_PAD_CTRL_1_PD_EN_UR0_RTSn               :  1;\
            UNSG32 uSM_PAD_CTRL_1_SLP_DI_UR1_RXD               :  1;\
            UNSG32 uSM_PAD_CTRL_1_PU_EN_UR1_RXD                :  1;\
            UNSG32 uSM_PAD_CTRL_1_PD_EN_UR1_RXD                :  1;\
            UNSG32 uSM_PAD_CTRL_1_SLP_DI_UR1_TXD               :  1;\
            UNSG32 uSM_PAD_CTRL_1_PU_EN_UR1_TXD                :  1;\
            UNSG32 uSM_PAD_CTRL_1_PD_EN_UR1_TXD                :  1;\
            UNSG32 uSM_PAD_CTRL_1_SLP_DI_CLKI                  :  1;\
          }
    union { UNSG32 u32smSysCtl_SM_PAD_CTRL_1;
            struct w32smSysCtl_SM_PAD_CTRL_1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_PAD_CTRL_2_PU_EN_CLKI(r32)      _BFGET_(r32, 0, 0)
    #define   SET32smSysCtl_SM_PAD_CTRL_2_PU_EN_CLKI(r32,v)    _BFSET_(r32, 0, 0,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_2_PU_EN_CLKI(r16)      _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_PAD_CTRL_2_PU_EN_CLKI(r16,v)    _BFSET_(r16, 0, 0,v)

    #define   GET32smSysCtl_SM_PAD_CTRL_2_PD_EN_CLKI(r32)      _BFGET_(r32, 1, 1)
    #define   SET32smSysCtl_SM_PAD_CTRL_2_PD_EN_CLKI(r32,v)    _BFSET_(r32, 1, 1,v)
    #define   GET16smSysCtl_SM_PAD_CTRL_2_PD_EN_CLKI(r16)      _BFGET_(r16, 1, 1)
    #define   SET16smSysCtl_SM_PAD_CTRL_2_PD_EN_CLKI(r16,v)    _BFSET_(r16, 1, 1,v)

    #define     w32smSysCtl_SM_PAD_CTRL_2                      {\
            UNSG32 uSM_PAD_CTRL_2_PU_EN_CLKI                   :  1;\
            UNSG32 uSM_PAD_CTRL_2_PD_EN_CLKI                   :  1;\
            UNSG32 RSVDx3C_b2                                  : 30;\
          }
    union { UNSG32 u32smSysCtl_SM_PAD_CTRL_2;
            struct w32smSysCtl_SM_PAD_CTRL_2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_TEST_ADC_TEST_FAIL(r32)         _BFGET_(r32, 0, 0)
    #define   SET32smSysCtl_SM_TEST_ADC_TEST_FAIL(r32,v)       _BFSET_(r32, 0, 0,v)
    #define   GET16smSysCtl_SM_TEST_ADC_TEST_FAIL(r16)         _BFGET_(r16, 0, 0)
    #define   SET16smSysCtl_SM_TEST_ADC_TEST_FAIL(r16,v)       _BFSET_(r16, 0, 0,v)

    #define     w32smSysCtl_SM_TEST                            {\
            UNSG32 uSM_TEST_ADC_TEST_FAIL                      :  1;\
            UNSG32 RSVDx40_b1                                  : 31;\
          }
    union { UNSG32 u32smSysCtl_SM_TEST;
            struct w32smSysCtl_SM_TEST;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_TEST_DATA0_ADC_DATA_HIGH(r32)   _BFGET_(r32, 9, 0)
    #define   SET32smSysCtl_SM_TEST_DATA0_ADC_DATA_HIGH(r32,v) _BFSET_(r32, 9, 0,v)
    #define   GET16smSysCtl_SM_TEST_DATA0_ADC_DATA_HIGH(r16)   _BFGET_(r16, 9, 0)
    #define   SET16smSysCtl_SM_TEST_DATA0_ADC_DATA_HIGH(r16,v) _BFSET_(r16, 9, 0,v)

    #define     w32smSysCtl_SM_TEST_DATA0                      {\
            UNSG32 uSM_TEST_DATA0_ADC_DATA_HIGH                : 10;\
            UNSG32 RSVDx44_b10                                 : 22;\
          }
    union { UNSG32 u32smSysCtl_SM_TEST_DATA0;
            struct w32smSysCtl_SM_TEST_DATA0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_TEST_DATA1_ADC_DATA_LOW(r32)    _BFGET_(r32, 9, 0)
    #define   SET32smSysCtl_SM_TEST_DATA1_ADC_DATA_LOW(r32,v)  _BFSET_(r32, 9, 0,v)
    #define   GET16smSysCtl_SM_TEST_DATA1_ADC_DATA_LOW(r16)    _BFGET_(r16, 9, 0)
    #define   SET16smSysCtl_SM_TEST_DATA1_ADC_DATA_LOW(r16,v)  _BFSET_(r16, 9, 0,v)

    #define     w32smSysCtl_SM_TEST_DATA1                      {\
            UNSG32 uSM_TEST_DATA1_ADC_DATA_LOW                 : 10;\
            UNSG32 RSVDx48_b10                                 : 22;\
          }
    union { UNSG32 u32smSysCtl_SM_TEST_DATA1;
            struct w32smSysCtl_SM_TEST_DATA1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_RWTC_CTRL_0_RF1P_LOW(r32)       _BFGET_(r32, 3, 0)
    #define   SET32smSysCtl_SM_RWTC_CTRL_0_RF1P_LOW(r32,v)     _BFSET_(r32, 3, 0,v)
    #define   GET16smSysCtl_SM_RWTC_CTRL_0_RF1P_LOW(r16)       _BFGET_(r16, 3, 0)
    #define   SET16smSysCtl_SM_RWTC_CTRL_0_RF1P_LOW(r16,v)     _BFSET_(r16, 3, 0,v)

    #define   GET32smSysCtl_SM_RWTC_CTRL_0_RF1P_HIGH(r32)      _BFGET_(r32, 7, 4)
    #define   SET32smSysCtl_SM_RWTC_CTRL_0_RF1P_HIGH(r32,v)    _BFSET_(r32, 7, 4,v)
    #define   GET16smSysCtl_SM_RWTC_CTRL_0_RF1P_HIGH(r16)      _BFGET_(r16, 7, 4)
    #define   SET16smSysCtl_SM_RWTC_CTRL_0_RF1P_HIGH(r16,v)    _BFSET_(r16, 7, 4,v)

    #define   GET32smSysCtl_SM_RWTC_CTRL_0_RF2P_LOW(r32)       _BFGET_(r32,11, 8)
    #define   SET32smSysCtl_SM_RWTC_CTRL_0_RF2P_LOW(r32,v)     _BFSET_(r32,11, 8,v)
    #define   GET16smSysCtl_SM_RWTC_CTRL_0_RF2P_LOW(r16)       _BFGET_(r16,11, 8)
    #define   SET16smSysCtl_SM_RWTC_CTRL_0_RF2P_LOW(r16,v)     _BFSET_(r16,11, 8,v)

    #define   GET32smSysCtl_SM_RWTC_CTRL_0_RF2P_HIGH(r32)      _BFGET_(r32,15,12)
    #define   SET32smSysCtl_SM_RWTC_CTRL_0_RF2P_HIGH(r32,v)    _BFSET_(r32,15,12,v)
    #define   GET16smSysCtl_SM_RWTC_CTRL_0_RF2P_HIGH(r16)      _BFGET_(r16,15,12)
    #define   SET16smSysCtl_SM_RWTC_CTRL_0_RF2P_HIGH(r16,v)    _BFSET_(r16,15,12,v)

    #define   GET32smSysCtl_SM_RWTC_CTRL_0_SR1P_LOW(r32)       _BFGET_(r32,19,16)
    #define   SET32smSysCtl_SM_RWTC_CTRL_0_SR1P_LOW(r32,v)     _BFSET_(r32,19,16,v)
    #define   GET16smSysCtl_SM_RWTC_CTRL_0_SR1P_LOW(r16)       _BFGET_(r16, 3, 0)
    #define   SET16smSysCtl_SM_RWTC_CTRL_0_SR1P_LOW(r16,v)     _BFSET_(r16, 3, 0,v)

    #define   GET32smSysCtl_SM_RWTC_CTRL_0_SR1P_HIGH(r32)      _BFGET_(r32,23,20)
    #define   SET32smSysCtl_SM_RWTC_CTRL_0_SR1P_HIGH(r32,v)    _BFSET_(r32,23,20,v)
    #define   GET16smSysCtl_SM_RWTC_CTRL_0_SR1P_HIGH(r16)      _BFGET_(r16, 7, 4)
    #define   SET16smSysCtl_SM_RWTC_CTRL_0_SR1P_HIGH(r16,v)    _BFSET_(r16, 7, 4,v)

    #define   GET32smSysCtl_SM_RWTC_CTRL_0_SR2P_LOW(r32)       _BFGET_(r32,27,24)
    #define   SET32smSysCtl_SM_RWTC_CTRL_0_SR2P_LOW(r32,v)     _BFSET_(r32,27,24,v)
    #define   GET16smSysCtl_SM_RWTC_CTRL_0_SR2P_LOW(r16)       _BFGET_(r16,11, 8)
    #define   SET16smSysCtl_SM_RWTC_CTRL_0_SR2P_LOW(r16,v)     _BFSET_(r16,11, 8,v)

    #define   GET32smSysCtl_SM_RWTC_CTRL_0_SR2P_HIGH(r32)      _BFGET_(r32,31,28)
    #define   SET32smSysCtl_SM_RWTC_CTRL_0_SR2P_HIGH(r32,v)    _BFSET_(r32,31,28,v)
    #define   GET16smSysCtl_SM_RWTC_CTRL_0_SR2P_HIGH(r16)      _BFGET_(r16,15,12)
    #define   SET16smSysCtl_SM_RWTC_CTRL_0_SR2P_HIGH(r16,v)    _BFSET_(r16,15,12,v)

    #define     w32smSysCtl_SM_RWTC_CTRL_0                     {\
            UNSG32 uSM_RWTC_CTRL_0_RF1P_LOW                    :  4;\
            UNSG32 uSM_RWTC_CTRL_0_RF1P_HIGH                   :  4;\
            UNSG32 uSM_RWTC_CTRL_0_RF2P_LOW                    :  4;\
            UNSG32 uSM_RWTC_CTRL_0_RF2P_HIGH                   :  4;\
            UNSG32 uSM_RWTC_CTRL_0_SR1P_LOW                    :  4;\
            UNSG32 uSM_RWTC_CTRL_0_SR1P_HIGH                   :  4;\
            UNSG32 uSM_RWTC_CTRL_0_SR2P_LOW                    :  4;\
            UNSG32 uSM_RWTC_CTRL_0_SR2P_HIGH                   :  4;\
          }
    union { UNSG32 u32smSysCtl_SM_RWTC_CTRL_0;
            struct w32smSysCtl_SM_RWTC_CTRL_0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32smSysCtl_SM_RWTC_CTRL_1_ROM_LOW(r32)        _BFGET_(r32, 3, 0)
    #define   SET32smSysCtl_SM_RWTC_CTRL_1_ROM_LOW(r32,v)      _BFSET_(r32, 3, 0,v)
    #define   GET16smSysCtl_SM_RWTC_CTRL_1_ROM_LOW(r16)        _BFGET_(r16, 3, 0)
    #define   SET16smSysCtl_SM_RWTC_CTRL_1_ROM_LOW(r16,v)      _BFSET_(r16, 3, 0,v)

    #define   GET32smSysCtl_SM_RWTC_CTRL_1_ROM_HIGH(r32)       _BFGET_(r32, 7, 4)
    #define   SET32smSysCtl_SM_RWTC_CTRL_1_ROM_HIGH(r32,v)     _BFSET_(r32, 7, 4,v)
    #define   GET16smSysCtl_SM_RWTC_CTRL_1_ROM_HIGH(r16)       _BFGET_(r16, 7, 4)
    #define   SET16smSysCtl_SM_RWTC_CTRL_1_ROM_HIGH(r16,v)     _BFSET_(r16, 7, 4,v)

    #define   GET32smSysCtl_SM_RWTC_CTRL_1_ACMEM(r32)          _BFGET_(r32,11, 8)
    #define   SET32smSysCtl_SM_RWTC_CTRL_1_ACMEM(r32,v)        _BFSET_(r32,11, 8,v)
    #define   GET16smSysCtl_SM_RWTC_CTRL_1_ACMEM(r16)          _BFGET_(r16,11, 8)
    #define   SET16smSysCtl_SM_RWTC_CTRL_1_ACMEM(r16,v)        _BFSET_(r16,11, 8,v)

    #define     w32smSysCtl_SM_RWTC_CTRL_1                     {\
            UNSG32 uSM_RWTC_CTRL_1_ROM_LOW                     :  4;\
            UNSG32 uSM_RWTC_CTRL_1_ROM_HIGH                    :  4;\
            UNSG32 uSM_RWTC_CTRL_1_ACMEM                       :  4;\
            UNSG32 RSVDx50_b12                                 : 20;\
          }
    union { UNSG32 u32smSysCtl_SM_RWTC_CTRL_1;
            struct w32smSysCtl_SM_RWTC_CTRL_1;
          };
    ///////////////////////////////////////////////////////////
    } SIE_smSysCtl;

    typedef union  T32smSysCtl_SM_ID
          { UNSG32 u32;
            struct w32smSysCtl_SM_ID;
                 } T32smSysCtl_SM_ID;
    typedef union  T32smSysCtl_SM_CPU_CTRL
          { UNSG32 u32;
            struct w32smSysCtl_SM_CPU_CTRL;
                 } T32smSysCtl_SM_CPU_CTRL;
    typedef union  T32smSysCtl_SM_RST_CTRL
          { UNSG32 u32;
            struct w32smSysCtl_SM_RST_CTRL;
                 } T32smSysCtl_SM_RST_CTRL;
    typedef union  T32smSysCtl_SM_RST_STATUS
          { UNSG32 u32;
            struct w32smSysCtl_SM_RST_STATUS;
                 } T32smSysCtl_SM_RST_STATUS;
    typedef union  T32smSysCtl_SM_STRP_STATUS
          { UNSG32 u32;
            struct w32smSysCtl_SM_STRP_STATUS;
                 } T32smSysCtl_SM_STRP_STATUS;
    typedef union  T32smSysCtl_SM_CTRL
          { UNSG32 u32;
            struct w32smSysCtl_SM_CTRL;
                 } T32smSysCtl_SM_CTRL;
    typedef union  T32smSysCtl_SM_ADC_STATUS
          { UNSG32 u32;
            struct w32smSysCtl_SM_ADC_STATUS;
                 } T32smSysCtl_SM_ADC_STATUS;
    typedef union  T32smSysCtl_SM_ADC_DATA
          { UNSG32 u32;
            struct w32smSysCtl_SM_ADC_DATA;
                 } T32smSysCtl_SM_ADC_DATA;
    typedef union  T32smSysCtl_SM_BOOT_STATUS
          { UNSG32 u32;
            struct w32smSysCtl_SM_BOOT_STATUS;
                 } T32smSysCtl_SM_BOOT_STATUS;
    typedef union  T32smSysCtl_SM_WDT_MASK
          { UNSG32 u32;
            struct w32smSysCtl_SM_WDT_MASK;
                 } T32smSysCtl_SM_WDT_MASK;
    typedef union  T32smSysCtl_smAnaGrpCtl
          { UNSG32 u32;
            struct w32smSysCtl_smAnaGrpCtl;
                 } T32smSysCtl_smAnaGrpCtl;
    typedef union  T32smSysCtl_SM_GSM_SEL
          { UNSG32 u32;
            struct w32smSysCtl_SM_GSM_SEL;
                 } T32smSysCtl_SM_GSM_SEL;
    typedef union  T32smSysCtl_SM_CORE_CTRL
          { UNSG32 u32;
            struct w32smSysCtl_SM_CORE_CTRL;
                 } T32smSysCtl_SM_CORE_CTRL;
    typedef union  T32smSysCtl_SM_PAD_CTRL_0
          { UNSG32 u32;
            struct w32smSysCtl_SM_PAD_CTRL_0;
                 } T32smSysCtl_SM_PAD_CTRL_0;
    typedef union  T32smSysCtl_SM_PAD_CTRL_1
          { UNSG32 u32;
            struct w32smSysCtl_SM_PAD_CTRL_1;
                 } T32smSysCtl_SM_PAD_CTRL_1;
    typedef union  T32smSysCtl_SM_PAD_CTRL_2
          { UNSG32 u32;
            struct w32smSysCtl_SM_PAD_CTRL_2;
                 } T32smSysCtl_SM_PAD_CTRL_2;
    typedef union  T32smSysCtl_SM_TEST
          { UNSG32 u32;
            struct w32smSysCtl_SM_TEST;
                 } T32smSysCtl_SM_TEST;
    typedef union  T32smSysCtl_SM_TEST_DATA0
          { UNSG32 u32;
            struct w32smSysCtl_SM_TEST_DATA0;
                 } T32smSysCtl_SM_TEST_DATA0;
    typedef union  T32smSysCtl_SM_TEST_DATA1
          { UNSG32 u32;
            struct w32smSysCtl_SM_TEST_DATA1;
                 } T32smSysCtl_SM_TEST_DATA1;
    typedef union  T32smSysCtl_SM_RWTC_CTRL_0
          { UNSG32 u32;
            struct w32smSysCtl_SM_RWTC_CTRL_0;
                 } T32smSysCtl_SM_RWTC_CTRL_0;
    typedef union  T32smSysCtl_SM_RWTC_CTRL_1
          { UNSG32 u32;
            struct w32smSysCtl_SM_RWTC_CTRL_1;
                 } T32smSysCtl_SM_RWTC_CTRL_1;
    ///////////////////////////////////////////////////////////

    typedef union  TsmSysCtl_SM_ID
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_ID;
                   };
                 } TsmSysCtl_SM_ID;
    typedef union  TsmSysCtl_SM_CPU_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_CPU_CTRL;
                   };
                 } TsmSysCtl_SM_CPU_CTRL;
    typedef union  TsmSysCtl_SM_RST_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_RST_CTRL;
                   };
                 } TsmSysCtl_SM_RST_CTRL;
    typedef union  TsmSysCtl_SM_RST_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_RST_STATUS;
                   };
                 } TsmSysCtl_SM_RST_STATUS;
    typedef union  TsmSysCtl_SM_STRP_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_STRP_STATUS;
                   };
                 } TsmSysCtl_SM_STRP_STATUS;
    typedef union  TsmSysCtl_SM_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_CTRL;
                   };
                 } TsmSysCtl_SM_CTRL;
    typedef union  TsmSysCtl_SM_ADC_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_ADC_STATUS;
                   };
                 } TsmSysCtl_SM_ADC_STATUS;
    typedef union  TsmSysCtl_SM_ADC_DATA
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_ADC_DATA;
                   };
                 } TsmSysCtl_SM_ADC_DATA;
    typedef union  TsmSysCtl_SM_BOOT_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_BOOT_STATUS;
                   };
                 } TsmSysCtl_SM_BOOT_STATUS;
    typedef union  TsmSysCtl_SM_WDT_MASK
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_WDT_MASK;
                   };
                 } TsmSysCtl_SM_WDT_MASK;
    typedef union  TsmSysCtl_smAnaGrpCtl
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_smAnaGrpCtl;
                   };
                 } TsmSysCtl_smAnaGrpCtl;
    typedef union  TsmSysCtl_SM_GSM_SEL
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_GSM_SEL;
                   };
                 } TsmSysCtl_SM_GSM_SEL;
    typedef union  TsmSysCtl_SM_CORE_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_CORE_CTRL;
                   };
                 } TsmSysCtl_SM_CORE_CTRL;
    typedef union  TsmSysCtl_SM_PAD_CTRL_0
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_PAD_CTRL_0;
                   };
                 } TsmSysCtl_SM_PAD_CTRL_0;
    typedef union  TsmSysCtl_SM_PAD_CTRL_1
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_PAD_CTRL_1;
                   };
                 } TsmSysCtl_SM_PAD_CTRL_1;
    typedef union  TsmSysCtl_SM_PAD_CTRL_2
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_PAD_CTRL_2;
                   };
                 } TsmSysCtl_SM_PAD_CTRL_2;
    typedef union  TsmSysCtl_SM_TEST
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_TEST;
                   };
                 } TsmSysCtl_SM_TEST;
    typedef union  TsmSysCtl_SM_TEST_DATA0
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_TEST_DATA0;
                   };
                 } TsmSysCtl_SM_TEST_DATA0;
    typedef union  TsmSysCtl_SM_TEST_DATA1
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_TEST_DATA1;
                   };
                 } TsmSysCtl_SM_TEST_DATA1;
    typedef union  TsmSysCtl_SM_RWTC_CTRL_0
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_RWTC_CTRL_0;
                   };
                 } TsmSysCtl_SM_RWTC_CTRL_0;
    typedef union  TsmSysCtl_SM_RWTC_CTRL_1
          { UNSG32 u32[1];
            struct {
            struct w32smSysCtl_SM_RWTC_CTRL_1;
                   };
                 } TsmSysCtl_SM_RWTC_CTRL_1;

    ///////////////////////////////////////////////////////////
     SIGN32 smSysCtl_drvrd(SIE_smSysCtl *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 smSysCtl_drvwr(SIE_smSysCtl *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void smSysCtl_reset(SIE_smSysCtl *p);
     SIGN32 smSysCtl_cmp  (SIE_smSysCtl *p, SIE_smSysCtl *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define smSysCtl_check(p,pie,pfx,hLOG) smSysCtl_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define smSysCtl_print(p,    pfx,hLOG) smSysCtl_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif

