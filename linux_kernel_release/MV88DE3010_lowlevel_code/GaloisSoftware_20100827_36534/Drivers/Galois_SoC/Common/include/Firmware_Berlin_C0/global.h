#ifndef global_h
#define global_h (){}


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



#ifndef h_Gbl
#define h_Gbl (){}

    #define     RA_Gbl_pinMux                                  0x0000

    #define     BA_Gbl_pinMux_gp0                              0x0000
    #define     B16Gbl_pinMux_gp0                              0x0000
    #define   LSb32Gbl_pinMux_gp0                                 0
    #define   LSb16Gbl_pinMux_gp0                                 0
    #define       bGbl_pinMux_gp0                              1
    #define   MSK32Gbl_pinMux_gp0                                 0x00000001
    #define        Gbl_pinMux_gp0_MODE_0                                    0x0
    #define        Gbl_pinMux_gp0_MODE_1                                    0x1

    #define     BA_Gbl_pinMux_gp1                              0x0000
    #define     B16Gbl_pinMux_gp1                              0x0000
    #define   LSb32Gbl_pinMux_gp1                                 1
    #define   LSb16Gbl_pinMux_gp1                                 1
    #define       bGbl_pinMux_gp1                              2
    #define   MSK32Gbl_pinMux_gp1                                 0x00000006
    #define        Gbl_pinMux_gp1_MODE_0                                    0x0
    #define        Gbl_pinMux_gp1_MODE_1                                    0x1
    #define        Gbl_pinMux_gp1_MODE_2                                    0x2

    #define     BA_Gbl_pinMux_gp2                              0x0000
    #define     B16Gbl_pinMux_gp2                              0x0000
    #define   LSb32Gbl_pinMux_gp2                                 3
    #define   LSb16Gbl_pinMux_gp2                                 3
    #define       bGbl_pinMux_gp2                              2
    #define   MSK32Gbl_pinMux_gp2                                 0x00000018
    #define        Gbl_pinMux_gp2_MODE_0                                    0x0
    #define        Gbl_pinMux_gp2_MODE_1                                    0x1
    #define        Gbl_pinMux_gp2_MODE_2                                    0x2
    #define        Gbl_pinMux_gp2_MODE_3                                    0x3

    #define     BA_Gbl_pinMux_gp3                              0x0000
    #define     B16Gbl_pinMux_gp3                              0x0000
    #define   LSb32Gbl_pinMux_gp3                                 5
    #define   LSb16Gbl_pinMux_gp3                                 5
    #define       bGbl_pinMux_gp3                              2
    #define   MSK32Gbl_pinMux_gp3                                 0x00000060
    #define        Gbl_pinMux_gp3_MODE_0                                    0x0
    #define        Gbl_pinMux_gp3_MODE_1                                    0x1
    #define        Gbl_pinMux_gp3_MODE_2                                    0x2
    #define        Gbl_pinMux_gp3_MODE_3                                    0x3

    #define     BA_Gbl_pinMux_gp4                              0x0000
    #define     B16Gbl_pinMux_gp4                              0x0000
    #define   LSb32Gbl_pinMux_gp4                                 7
    #define   LSb16Gbl_pinMux_gp4                                 7
    #define       bGbl_pinMux_gp4                              2
    #define   MSK32Gbl_pinMux_gp4                                 0x00000180
    #define        Gbl_pinMux_gp4_MODE_0                                    0x0
    #define        Gbl_pinMux_gp4_MODE_1                                    0x1
    #define        Gbl_pinMux_gp4_MODE_2                                    0x2

    #define     BA_Gbl_pinMux_gp5                              0x0001
    #define     B16Gbl_pinMux_gp5                              0x0000
    #define   LSb32Gbl_pinMux_gp5                                 9
    #define   LSb16Gbl_pinMux_gp5                                 9
    #define       bGbl_pinMux_gp5                              1
    #define   MSK32Gbl_pinMux_gp5                                 0x00000200
    #define        Gbl_pinMux_gp5_MODE_0                                    0x0
    #define        Gbl_pinMux_gp5_MODE_1                                    0x1

    #define     BA_Gbl_pinMux_gp6                              0x0001
    #define     B16Gbl_pinMux_gp6                              0x0000
    #define   LSb32Gbl_pinMux_gp6                                 10
    #define   LSb16Gbl_pinMux_gp6                                 10
    #define       bGbl_pinMux_gp6                              2
    #define   MSK32Gbl_pinMux_gp6                                 0x00000C00
    #define        Gbl_pinMux_gp6_MODE_0                                    0x0
    #define        Gbl_pinMux_gp6_MODE_1                                    0x1
    #define        Gbl_pinMux_gp6_MODE_2                                    0x2

    #define     BA_Gbl_pinMux_gp7                              0x0001
    #define     B16Gbl_pinMux_gp7                              0x0000
    #define   LSb32Gbl_pinMux_gp7                                 12
    #define   LSb16Gbl_pinMux_gp7                                 12
    #define       bGbl_pinMux_gp7                              2
    #define   MSK32Gbl_pinMux_gp7                                 0x00003000
    #define        Gbl_pinMux_gp7_MODE_0                                    0x0
    #define        Gbl_pinMux_gp7_MODE_1                                    0x1
    #define        Gbl_pinMux_gp7_MODE_2                                    0x2
    #define        Gbl_pinMux_gp7_MODE_3                                    0x3

    #define     BA_Gbl_pinMux_gp8                              0x0001
    #define     B16Gbl_pinMux_gp8                              0x0000
    #define   LSb32Gbl_pinMux_gp8                                 14
    #define   LSb16Gbl_pinMux_gp8                                 14
    #define       bGbl_pinMux_gp8                              2
    #define   MSK32Gbl_pinMux_gp8                                 0x0000C000
    #define        Gbl_pinMux_gp8_MODE_0                                    0x0
    #define        Gbl_pinMux_gp8_MODE_1                                    0x1
    #define        Gbl_pinMux_gp8_MODE_2                                    0x2
    #define        Gbl_pinMux_gp8_MODE_3                                    0x3

    #define     BA_Gbl_pinMux_gp9                              0x0002
    #define     B16Gbl_pinMux_gp9                              0x0002
    #define   LSb32Gbl_pinMux_gp9                                 16
    #define   LSb16Gbl_pinMux_gp9                                 0
    #define       bGbl_pinMux_gp9                              2
    #define   MSK32Gbl_pinMux_gp9                                 0x00030000
    #define        Gbl_pinMux_gp9_MODE_0                                    0x0
    #define        Gbl_pinMux_gp9_MODE_1                                    0x1
    #define        Gbl_pinMux_gp9_MODE_2                                    0x2

    #define     BA_Gbl_pinMux_gp10                             0x0002
    #define     B16Gbl_pinMux_gp10                             0x0002
    #define   LSb32Gbl_pinMux_gp10                                18
    #define   LSb16Gbl_pinMux_gp10                                2
    #define       bGbl_pinMux_gp10                             2
    #define   MSK32Gbl_pinMux_gp10                                0x000C0000
    #define        Gbl_pinMux_gp10_MODE_0                                   0x0
    #define        Gbl_pinMux_gp10_MODE_1                                   0x1
    #define        Gbl_pinMux_gp10_MODE_2                                   0x2

    #define     BA_Gbl_pinMux_gp11                             0x0002
    #define     B16Gbl_pinMux_gp11                             0x0002
    #define   LSb32Gbl_pinMux_gp11                                20
    #define   LSb16Gbl_pinMux_gp11                                4
    #define       bGbl_pinMux_gp11                             2
    #define   MSK32Gbl_pinMux_gp11                                0x00300000
    #define        Gbl_pinMux_gp11_MODE_0                                   0x0
    #define        Gbl_pinMux_gp11_MODE_1                                   0x1
    #define        Gbl_pinMux_gp11_MODE_2                                   0x2

    #define     BA_Gbl_pinMux_gp12                             0x0002
    #define     B16Gbl_pinMux_gp12                             0x0002
    #define   LSb32Gbl_pinMux_gp12                                22
    #define   LSb16Gbl_pinMux_gp12                                6
    #define       bGbl_pinMux_gp12                             2
    #define   MSK32Gbl_pinMux_gp12                                0x00C00000
    #define        Gbl_pinMux_gp12_MODE_0                                   0x0
    #define        Gbl_pinMux_gp12_MODE_2                                   0x2

    #define     BA_Gbl_pinMux_gp13                             0x0003
    #define     B16Gbl_pinMux_gp13                             0x0002
    #define   LSb32Gbl_pinMux_gp13                                24
    #define   LSb16Gbl_pinMux_gp13                                8
    #define       bGbl_pinMux_gp13                             2
    #define   MSK32Gbl_pinMux_gp13                                0x03000000
    #define        Gbl_pinMux_gp13_MODE_0                                   0x0
    #define        Gbl_pinMux_gp13_MODE_2                                   0x2

    #define     BA_Gbl_pinMux_gp14                             0x0003
    #define     B16Gbl_pinMux_gp14                             0x0002
    #define   LSb32Gbl_pinMux_gp14                                26
    #define   LSb16Gbl_pinMux_gp14                                10
    #define       bGbl_pinMux_gp14                             1
    #define   MSK32Gbl_pinMux_gp14                                0x04000000
    #define        Gbl_pinMux_gp14_MODE_0                                   0x0
    #define        Gbl_pinMux_gp14_MODE_1                                   0x1

    #define     BA_Gbl_pinMux_gp15                             0x0003
    #define     B16Gbl_pinMux_gp15                             0x0002
    #define   LSb32Gbl_pinMux_gp15                                27
    #define   LSb16Gbl_pinMux_gp15                                11
    #define       bGbl_pinMux_gp15                             1
    #define   MSK32Gbl_pinMux_gp15                                0x08000000
    #define        Gbl_pinMux_gp15_MODE_0                                   0x0
    #define        Gbl_pinMux_gp15_MODE_1                                   0x1

    #define     BA_Gbl_pinMux_gp16                             0x0003
    #define     B16Gbl_pinMux_gp16                             0x0002
    #define   LSb32Gbl_pinMux_gp16                                28
    #define   LSb16Gbl_pinMux_gp16                                12
    #define       bGbl_pinMux_gp16                             3
    #define   MSK32Gbl_pinMux_gp16                                0x70000000
    #define        Gbl_pinMux_gp16_MODE_0                                   0x0
    #define        Gbl_pinMux_gp16_MODE_1                                   0x1
    #define        Gbl_pinMux_gp16_MODE_7                                   0x7

    #define     RA_Gbl_pinMux1                                 0x0004

    #define     BA_Gbl_pinMux_gp17                             0x0004
    #define     B16Gbl_pinMux_gp17                             0x0004
    #define   LSb32Gbl_pinMux_gp17                                0
    #define   LSb16Gbl_pinMux_gp17                                0
    #define       bGbl_pinMux_gp17                             3
    #define   MSK32Gbl_pinMux_gp17                                0x00000007
    #define        Gbl_pinMux_gp17_MODE_0                                   0x0
    #define        Gbl_pinMux_gp17_MODE_1                                   0x1
    #define        Gbl_pinMux_gp17_MODE_7                                   0x7

    #define     BA_Gbl_pinMux_gp18                             0x0004
    #define     B16Gbl_pinMux_gp18                             0x0004
    #define   LSb32Gbl_pinMux_gp18                                3
    #define   LSb16Gbl_pinMux_gp18                                3
    #define       bGbl_pinMux_gp18                             1
    #define   MSK32Gbl_pinMux_gp18                                0x00000008
    #define        Gbl_pinMux_gp18_MODE_0                                   0x0
    #define        Gbl_pinMux_gp18_MODE_1                                   0x1

    #define     BA_Gbl_pinMux_gp19                             0x0004
    #define     B16Gbl_pinMux_gp19                             0x0004
    #define   LSb32Gbl_pinMux_gp19                                4
    #define   LSb16Gbl_pinMux_gp19                                4
    #define       bGbl_pinMux_gp19                             1
    #define   MSK32Gbl_pinMux_gp19                                0x00000010
    #define        Gbl_pinMux_gp19_MODE_0                                   0x0
    #define        Gbl_pinMux_gp19_MODE_1                                   0x1

    #define     BA_Gbl_pinMux_gp20                             0x0004
    #define     B16Gbl_pinMux_gp20                             0x0004
    #define   LSb32Gbl_pinMux_gp20                                5
    #define   LSb16Gbl_pinMux_gp20                                5
    #define       bGbl_pinMux_gp20                             1
    #define   MSK32Gbl_pinMux_gp20                                0x00000020
    #define        Gbl_pinMux_gp20_MODE_0                                   0x0
    #define        Gbl_pinMux_gp20_MODE_1                                   0x1

    #define     BA_Gbl_pinMux_gp21                             0x0004
    #define     B16Gbl_pinMux_gp21                             0x0004
    #define   LSb32Gbl_pinMux_gp21                                6
    #define   LSb16Gbl_pinMux_gp21                                6
    #define       bGbl_pinMux_gp21                             3
    #define   MSK32Gbl_pinMux_gp21                                0x000001C0
    #define        Gbl_pinMux_gp21_MODE_0                                   0x0
    #define        Gbl_pinMux_gp21_MODE_1                                   0x1
    #define        Gbl_pinMux_gp21_MODE_2                                   0x2
    #define        Gbl_pinMux_gp21_MODE_3                                   0x3
    #define        Gbl_pinMux_gp21_MODE_7                                   0x7

    #define     BA_Gbl_pinMux_gp22                             0x0005
    #define     B16Gbl_pinMux_gp22                             0x0004
    #define   LSb32Gbl_pinMux_gp22                                9
    #define   LSb16Gbl_pinMux_gp22                                9
    #define       bGbl_pinMux_gp22                             3
    #define   MSK32Gbl_pinMux_gp22                                0x00000E00
    #define        Gbl_pinMux_gp22_MODE_0                                   0x0
    #define        Gbl_pinMux_gp22_MODE_1                                   0x1
    #define        Gbl_pinMux_gp22_MODE_2                                   0x2
    #define        Gbl_pinMux_gp22_MODE_7                                   0x7

    #define     BA_Gbl_pinMux_gp23                             0x0005
    #define     B16Gbl_pinMux_gp23                             0x0004
    #define   LSb32Gbl_pinMux_gp23                                12
    #define   LSb16Gbl_pinMux_gp23                                12
    #define       bGbl_pinMux_gp23                             3
    #define   MSK32Gbl_pinMux_gp23                                0x00007000
    #define        Gbl_pinMux_gp23_MODE_0                                   0x0
    #define        Gbl_pinMux_gp23_MODE_1                                   0x1
    #define        Gbl_pinMux_gp23_MODE_2                                   0x2
    #define        Gbl_pinMux_gp23_MODE_3                                   0x3
    #define        Gbl_pinMux_gp23_MODE_7                                   0x7

    #define     BA_Gbl_pinMux_gp24                             0x0005
    #define     B16Gbl_pinMux_gp24                             0x0004
    #define   LSb32Gbl_pinMux_gp24                                15
    #define   LSb16Gbl_pinMux_gp24                                15
    #define       bGbl_pinMux_gp24                             2
    #define   MSK32Gbl_pinMux_gp24                                0x00018000
    #define        Gbl_pinMux_gp24_MODE_0                                   0x0
    #define        Gbl_pinMux_gp24_MODE_1                                   0x1
    #define        Gbl_pinMux_gp24_MODE_2                                   0x2

    #define     BA_Gbl_pinMux_gp25                             0x0006
    #define     B16Gbl_pinMux_gp25                             0x0006
    #define   LSb32Gbl_pinMux_gp25                                17
    #define   LSb16Gbl_pinMux_gp25                                1
    #define       bGbl_pinMux_gp25                             2
    #define   MSK32Gbl_pinMux_gp25                                0x00060000
    #define        Gbl_pinMux_gp25_MODE_0                                   0x0
    #define        Gbl_pinMux_gp25_MODE_1                                   0x1
    #define        Gbl_pinMux_gp25_MODE_2                                   0x2
    #define        Gbl_pinMux_gp25_MODE_3                                   0x3
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_bootStrap                               0x0008

    #define     BA_Gbl_bootStrap_softwareStrap                 0x0008
    #define     B16Gbl_bootStrap_softwareStrap                 0x0008
    #define   LSb32Gbl_bootStrap_softwareStrap                    0
    #define   LSb16Gbl_bootStrap_softwareStrap                    0
    #define       bGbl_bootStrap_softwareStrap                 7
    #define   MSK32Gbl_bootStrap_softwareStrap                    0x0000007F

    #define     BA_Gbl_bootStrap_bootSrc                       0x0008
    #define     B16Gbl_bootStrap_bootSrc                       0x0008
    #define   LSb32Gbl_bootStrap_bootSrc                          7
    #define   LSb16Gbl_bootStrap_bootSrc                          7
    #define       bGbl_bootStrap_bootSrc                       2
    #define   MSK32Gbl_bootStrap_bootSrc                          0x00000180

    #define     BA_Gbl_bootStrap_sysPllByps                    0x0009
    #define     B16Gbl_bootStrap_sysPllByps                    0x0008
    #define   LSb32Gbl_bootStrap_sysPllByps                       9
    #define   LSb16Gbl_bootStrap_sysPllByps                       9
    #define       bGbl_bootStrap_sysPllByps                    1
    #define   MSK32Gbl_bootStrap_sysPllByps                       0x00000200
    #define        Gbl_bootStrap_sysPllByps_PLL_OUT                         0x0
    #define        Gbl_bootStrap_sysPllByps_BYPS                            0x1

    #define     BA_Gbl_bootStrap_memPllByps                    0x0009
    #define     B16Gbl_bootStrap_memPllByps                    0x0008
    #define   LSb32Gbl_bootStrap_memPllByps                       10
    #define   LSb16Gbl_bootStrap_memPllByps                       10
    #define       bGbl_bootStrap_memPllByps                    1
    #define   MSK32Gbl_bootStrap_memPllByps                       0x00000400
    #define        Gbl_bootStrap_memPllByps_PLL_OUT                         0x0
    #define        Gbl_bootStrap_memPllByps_BYPS                            0x1

    #define     BA_Gbl_bootStrap_socJtagFromSM                 0x0009
    #define     B16Gbl_bootStrap_socJtagFromSM                 0x0008
    #define   LSb32Gbl_bootStrap_socJtagFromSM                    11
    #define   LSb16Gbl_bootStrap_socJtagFromSM                    11
    #define       bGbl_bootStrap_socJtagFromSM                 1
    #define   MSK32Gbl_bootStrap_socJtagFromSM                    0x00000800
    #define        Gbl_bootStrap_socJtagFromSM_fromPinMux                   0x0
    #define        Gbl_bootStrap_socJtagFromSM_fromSM                       0x1

    #define     BA_Gbl_bootStrap_nandV18Enable                 0x0009
    #define     B16Gbl_bootStrap_nandV18Enable                 0x0008
    #define   LSb32Gbl_bootStrap_nandV18Enable                    12
    #define   LSb16Gbl_bootStrap_nandV18Enable                    12
    #define       bGbl_bootStrap_nandV18Enable                 1
    #define   MSK32Gbl_bootStrap_nandV18Enable                    0x00001000
    #define        Gbl_bootStrap_nandV18Enable_V1R8                         0x0
    #define        Gbl_bootStrap_nandV18Enable_V3R3                         0x1

    #define     BA_Gbl_bootStrap_spi1V18Enable                 0x0009
    #define     B16Gbl_bootStrap_spi1V18Enable                 0x0008
    #define   LSb32Gbl_bootStrap_spi1V18Enable                    13
    #define   LSb16Gbl_bootStrap_spi1V18Enable                    13
    #define       bGbl_bootStrap_spi1V18Enable                 1
    #define   MSK32Gbl_bootStrap_spi1V18Enable                    0x00002000
    #define        Gbl_bootStrap_spi1V18Enable_V1R8                         0x0
    #define        Gbl_bootStrap_spi1V18Enable_V3R3                         0x1

    #define     BA_Gbl_bootStrap_nandV25Enable                 0x0009
    #define     B16Gbl_bootStrap_nandV25Enable                 0x0008
    #define   LSb32Gbl_bootStrap_nandV25Enable                    14
    #define   LSb16Gbl_bootStrap_nandV25Enable                    14
    #define       bGbl_bootStrap_nandV25Enable                 1
    #define   MSK32Gbl_bootStrap_nandV25Enable                    0x00004000

    #define     BA_Gbl_bootStrap_spi1V25Enable                 0x0009
    #define     B16Gbl_bootStrap_spi1V25Enable                 0x0008
    #define   LSb32Gbl_bootStrap_spi1V25Enable                    15
    #define   LSb16Gbl_bootStrap_spi1V25Enable                    15
    #define       bGbl_bootStrap_spi1V25Enable                 1
    #define   MSK32Gbl_bootStrap_spi1V25Enable                    0x00008000

    #define     BA_Gbl_bootStrap_ENG_EN                        0x000A
    #define     B16Gbl_bootStrap_ENG_EN                        0x000A
    #define   LSb32Gbl_bootStrap_ENG_EN                           16
    #define   LSb16Gbl_bootStrap_ENG_EN                           0
    #define       bGbl_bootStrap_ENG_EN                        1
    #define   MSK32Gbl_bootStrap_ENG_EN                           0x00010000
    #define        Gbl_bootStrap_ENG_EN_PRODUCTION_MODE                     0x0
    #define        Gbl_bootStrap_ENG_EN_DEVELOPE_MODE                       0x1
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_chipCntl                                0x000C

    #define     BA_Gbl_chipCntl_pcieDownMap                    0x000C
    #define     B16Gbl_chipCntl_pcieDownMap                    0x000C
    #define   LSb32Gbl_chipCntl_pcieDownMap                       0
    #define   LSb16Gbl_chipCntl_pcieDownMap                       0
    #define       bGbl_chipCntl_pcieDownMap                    1
    #define   MSK32Gbl_chipCntl_pcieDownMap                       0x00000001
    #define        Gbl_chipCntl_pcieDownMap_M512                            0x0
    #define        Gbl_chipCntl_pcieDownMap_M256                            0x1

    #define     BA_Gbl_chipCntl_pcie_rc                        0x000C
    #define     B16Gbl_chipCntl_pcie_rc                        0x000C
    #define   LSb32Gbl_chipCntl_pcie_rc                           1
    #define   LSb16Gbl_chipCntl_pcie_rc                           1
    #define       bGbl_chipCntl_pcie_rc                        1
    #define   MSK32Gbl_chipCntl_pcie_rc                           0x00000002
    #define        Gbl_chipCntl_pcie_rc_END_POINT                           0x0
    #define        Gbl_chipCntl_pcie_rc_ROOT_COMPLEX                        0x1
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_anaGrpCtl                               0x0010

    #define     BA_Gbl_anaGrpCtl_anaGrpPuIvref                 0x0010
    #define     B16Gbl_anaGrpCtl_anaGrpPuIvref                 0x0010
    #define   LSb32Gbl_anaGrpCtl_anaGrpPuIvref                    0
    #define   LSb16Gbl_anaGrpCtl_anaGrpPuIvref                    0
    #define       bGbl_anaGrpCtl_anaGrpPuIvref                 1
    #define   MSK32Gbl_anaGrpCtl_anaGrpPuIvref                    0x00000001

    #define     BA_Gbl_anaGrpCtl_anaGrpBgrSel                  0x0010
    #define     B16Gbl_anaGrpCtl_anaGrpBgrSel                  0x0010
    #define   LSb32Gbl_anaGrpCtl_anaGrpBgrSel                     1
    #define   LSb16Gbl_anaGrpCtl_anaGrpBgrSel                     1
    #define       bGbl_anaGrpCtl_anaGrpBgrSel                  1
    #define   MSK32Gbl_anaGrpCtl_anaGrpBgrSel                     0x00000002

    #define     BA_Gbl_anaGrpCtl_anaGrpPuXtl                   0x0010
    #define     B16Gbl_anaGrpCtl_anaGrpPuXtl                   0x0010
    #define   LSb32Gbl_anaGrpCtl_anaGrpPuXtl                      2
    #define   LSb16Gbl_anaGrpCtl_anaGrpPuXtl                      2
    #define       bGbl_anaGrpCtl_anaGrpPuXtl                   1
    #define   MSK32Gbl_anaGrpCtl_anaGrpPuXtl                      0x00000004

    #define     BA_Gbl_anaGrpCtl_anaGrpGainX2                  0x0010
    #define     B16Gbl_anaGrpCtl_anaGrpGainX2                  0x0010
    #define   LSb32Gbl_anaGrpCtl_anaGrpGainX2                     3
    #define   LSb16Gbl_anaGrpCtl_anaGrpGainX2                     3
    #define       bGbl_anaGrpCtl_anaGrpGainX2                  1
    #define   MSK32Gbl_anaGrpCtl_anaGrpGainX2                     0x00000008

    #define     BA_Gbl_anaGrpCtl_anaGrpTestAna                 0x0010
    #define     B16Gbl_anaGrpCtl_anaGrpTestAna                 0x0010
    #define   LSb32Gbl_anaGrpCtl_anaGrpTestAna                    4
    #define   LSb16Gbl_anaGrpCtl_anaGrpTestAna                    4
    #define       bGbl_anaGrpCtl_anaGrpTestAna                 3
    #define   MSK32Gbl_anaGrpCtl_anaGrpTestAna                    0x00000070
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_sysPllCtl                               0x0014

    #define     BA_Gbl_sysPllCtl_sysPllPu                      0x0014
    #define     B16Gbl_sysPllCtl_sysPllPu                      0x0014
    #define   LSb32Gbl_sysPllCtl_sysPllPu                         0
    #define   LSb16Gbl_sysPllCtl_sysPllPu                         0
    #define       bGbl_sysPllCtl_sysPllPu                      1
    #define   MSK32Gbl_sysPllCtl_sysPllPu                         0x00000001

    #define     BA_Gbl_sysPllCtl_sysPllRfDiv                   0x0014
    #define     B16Gbl_sysPllCtl_sysPllRfDiv                   0x0014
    #define   LSb32Gbl_sysPllCtl_sysPllRfDiv                      1
    #define   LSb16Gbl_sysPllCtl_sysPllRfDiv                      1
    #define       bGbl_sysPllCtl_sysPllRfDiv                   5
    #define   MSK32Gbl_sysPllCtl_sysPllRfDiv                      0x0000003E

    #define     BA_Gbl_sysPllCtl_sysPllFbDiv                   0x0014
    #define     B16Gbl_sysPllCtl_sysPllFbDiv                   0x0014
    #define   LSb32Gbl_sysPllCtl_sysPllFbDiv                      6
    #define   LSb16Gbl_sysPllCtl_sysPllFbDiv                      6
    #define       bGbl_sysPllCtl_sysPllFbDiv                   9
    #define   MSK32Gbl_sysPllCtl_sysPllFbDiv                      0x00007FC0

    #define     BA_Gbl_sysPllCtl_sysPllVddm                    0x0015
    #define     B16Gbl_sysPllCtl_sysPllVddm                    0x0014
    #define   LSb32Gbl_sysPllCtl_sysPllVddm                       15
    #define   LSb16Gbl_sysPllCtl_sysPllVddm                       15
    #define       bGbl_sysPllCtl_sysPllVddm                    2
    #define   MSK32Gbl_sysPllCtl_sysPllVddm                       0x00018000
    #define        Gbl_sysPllCtl_sysPllVddm_1R10v                           0x0
    #define        Gbl_sysPllCtl_sysPllVddm_1R15v                           0x1
    #define        Gbl_sysPllCtl_sysPllVddm_1R2v                            0x2
    #define        Gbl_sysPllCtl_sysPllVddm_1R25v                           0x3

    #define     BA_Gbl_sysPllCtl_sysPllVddl                    0x0016
    #define     B16Gbl_sysPllCtl_sysPllVddl                    0x0016
    #define   LSb32Gbl_sysPllCtl_sysPllVddl                       17
    #define   LSb16Gbl_sysPllCtl_sysPllVddl                       1
    #define       bGbl_sysPllCtl_sysPllVddl                    2
    #define   MSK32Gbl_sysPllCtl_sysPllVddl                       0x00060000
    #define        Gbl_sysPllCtl_sysPllVddl_0R90v                           0x0
    #define        Gbl_sysPllCtl_sysPllVddl_1R00v                           0x1
    #define        Gbl_sysPllCtl_sysPllVddl_1R05v                           0x2
    #define        Gbl_sysPllCtl_sysPllVddl_1R10v                           0x3

    #define     BA_Gbl_sysPllCtl_sysPllVR1P8V                  0x0016
    #define     B16Gbl_sysPllCtl_sysPllVR1P8V                  0x0016
    #define   LSb32Gbl_sysPllCtl_sysPllVR1P8V                     19
    #define   LSb16Gbl_sysPllCtl_sysPllVR1P8V                     3
    #define       bGbl_sysPllCtl_sysPllVR1P8V                  2
    #define   MSK32Gbl_sysPllCtl_sysPllVR1P8V                     0x00180000

    #define     BA_Gbl_sysPllCtl_sysPllVcoFbst                 0x0016
    #define     B16Gbl_sysPllCtl_sysPllVcoFbst                 0x0016
    #define   LSb32Gbl_sysPllCtl_sysPllVcoFbst                    21
    #define   LSb16Gbl_sysPllCtl_sysPllVcoFbst                    5
    #define       bGbl_sysPllCtl_sysPllVcoFbst                 1
    #define   MSK32Gbl_sysPllCtl_sysPllVcoFbst                    0x00200000

    #define     BA_Gbl_sysPllCtl_sysPllVcoRng                  0x0016
    #define     B16Gbl_sysPllCtl_sysPllVcoRng                  0x0016
    #define   LSb32Gbl_sysPllCtl_sysPllVcoRng                     22
    #define   LSb16Gbl_sysPllCtl_sysPllVcoRng                     6
    #define       bGbl_sysPllCtl_sysPllVcoRng                  2
    #define   MSK32Gbl_sysPllCtl_sysPllVcoRng                     0x00C00000
    #define        Gbl_sysPllCtl_sysPllVcoRng_Min                           0x0
    #define        Gbl_sysPllCtl_sysPllVcoRng_Max                           0x3

    #define     BA_Gbl_sysPllCtl_sysPllIcp                     0x0017
    #define     B16Gbl_sysPllCtl_sysPllIcp                     0x0016
    #define   LSb32Gbl_sysPllCtl_sysPllIcp                        24
    #define   LSb16Gbl_sysPllCtl_sysPllIcp                        8
    #define       bGbl_sysPllCtl_sysPllIcp                     3
    #define   MSK32Gbl_sysPllCtl_sysPllIcp                        0x07000000
    #define        Gbl_sysPllCtl_sysPllIcp_2R5uA                            0x0
    #define        Gbl_sysPllCtl_sysPllIcp_5R0uA                            0x1
    #define        Gbl_sysPllCtl_sysPllIcp_7R5uA                            0x2
    #define        Gbl_sysPllCtl_sysPllIcp_10R0uA                           0x3
    #define        Gbl_sysPllCtl_sysPllIcp_12R5uA                           0x4
    #define        Gbl_sysPllCtl_sysPllIcp_15R0uA                           0x5
    #define        Gbl_sysPllCtl_sysPllIcp_17R5uA                           0x6
    #define        Gbl_sysPllCtl_sysPllIcp_20R0uA                           0x7

    #define     BA_Gbl_sysPllCtl_sysPllKvco                    0x0017
    #define     B16Gbl_sysPllCtl_sysPllKvco                    0x0016
    #define   LSb32Gbl_sysPllCtl_sysPllKvco                       27
    #define   LSb16Gbl_sysPllCtl_sysPllKvco                       11
    #define       bGbl_sysPllCtl_sysPllKvco                    4
    #define   MSK32Gbl_sysPllCtl_sysPllKvco                       0x78000000
    #define        Gbl_sysPllCtl_sysPllKvco_500to700Mhz                     0x3
    #define        Gbl_sysPllCtl_sysPllKvco_700to1Ghz                       0x4

    #define     RA_Gbl_sysPllCtl1                              0x0018

    #define     BA_Gbl_sysPllCtl_sysPllVcoDivSel               0x0018
    #define     B16Gbl_sysPllCtl_sysPllVcoDivSel               0x0018
    #define   LSb32Gbl_sysPllCtl_sysPllVcoDivSel                  0
    #define   LSb16Gbl_sysPllCtl_sysPllVcoDivSel                  0
    #define       bGbl_sysPllCtl_sysPllVcoDivSel               2
    #define   MSK32Gbl_sysPllCtl_sysPllVcoDivSel                  0x00000003
    #define        Gbl_sysPllCtl_sysPllVcoDivSel_1                          0x0
    #define        Gbl_sysPllCtl_sysPllVcoDivSel_2                          0x1
    #define        Gbl_sysPllCtl_sysPllVcoDivSel_4                          0x2
    #define        Gbl_sysPllCtl_sysPllVcoDivSel_8                          0x3

    #define     BA_Gbl_sysPllCtl_sysPllIntpi                   0x0018
    #define     B16Gbl_sysPllCtl_sysPllIntpi                   0x0018
    #define   LSb32Gbl_sysPllCtl_sysPllIntpi                      2
    #define   LSb16Gbl_sysPllCtl_sysPllIntpi                      2
    #define       bGbl_sysPllCtl_sysPllIntpi                   2
    #define   MSK32Gbl_sysPllCtl_sysPllIntpi                      0x0000000C
    #define        Gbl_sysPllCtl_sysPllIntpi_10uA                           0x0
    #define        Gbl_sysPllCtl_sysPllIntpi_15uA                           0x1
    #define        Gbl_sysPllCtl_sysPllIntpi_20uA                           0x2
    #define        Gbl_sysPllCtl_sysPllIntpi_25uA                           0x3

    #define     BA_Gbl_sysPllCtl_sysPllCapSel                  0x0018
    #define     B16Gbl_sysPllCtl_sysPllCapSel                  0x0018
    #define   LSb32Gbl_sysPllCtl_sysPllCapSel                     4
    #define   LSb16Gbl_sysPllCtl_sysPllCapSel                     4
    #define       bGbl_sysPllCtl_sysPllCapSel                  3
    #define   MSK32Gbl_sysPllCtl_sysPllCapSel                     0x00000070
    #define        Gbl_sysPllCtl_sysPllCapSel_Small                         0x0
    #define        Gbl_sysPllCtl_sysPllCapSel_Large                         0x7

    #define     BA_Gbl_sysPllCtl_sysPllSscEn                   0x0018
    #define     B16Gbl_sysPllCtl_sysPllSscEn                   0x0018
    #define   LSb32Gbl_sysPllCtl_sysPllSscEn                      7
    #define   LSb16Gbl_sysPllCtl_sysPllSscEn                      7
    #define       bGbl_sysPllCtl_sysPllSscEn                   1
    #define   MSK32Gbl_sysPllCtl_sysPllSscEn                      0x00000080
    #define        Gbl_sysPllCtl_sysPllSscEn_Disable                        0x0
    #define        Gbl_sysPllCtl_sysPllSscEn_Enable                         0x1

    #define     BA_Gbl_sysPllCtl_sysPllPuSsc                   0x0019
    #define     B16Gbl_sysPllCtl_sysPllPuSsc                   0x0018
    #define   LSb32Gbl_sysPllCtl_sysPllPuSsc                      8
    #define   LSb16Gbl_sysPllCtl_sysPllPuSsc                      8
    #define       bGbl_sysPllCtl_sysPllPuSsc                   1
    #define   MSK32Gbl_sysPllCtl_sysPllPuSsc                      0x00000100
    #define        Gbl_sysPllCtl_sysPllPuSsc_Disable                        0x0
    #define        Gbl_sysPllCtl_sysPllPuSsc_Enable                         0x1

    #define     BA_Gbl_sysPllCtl_sysPllSscFdiv                 0x0019
    #define     B16Gbl_sysPllCtl_sysPllSscFdiv                 0x0018
    #define   LSb32Gbl_sysPllCtl_sysPllSscFdiv                    9
    #define   LSb16Gbl_sysPllCtl_sysPllSscFdiv                    9
    #define       bGbl_sysPllCtl_sysPllSscFdiv                 15
    #define   MSK32Gbl_sysPllCtl_sysPllSscFdiv                    0x00FFFE00

    #define     RA_Gbl_sysPllCtl2                              0x001C

    #define     BA_Gbl_sysPllCtl_sysPllSscRnge                 0x001C
    #define     B16Gbl_sysPllCtl_sysPllSscRnge                 0x001C
    #define   LSb32Gbl_sysPllCtl_sysPllSscRnge                    0
    #define   LSb16Gbl_sysPllCtl_sysPllSscRnge                    0
    #define       bGbl_sysPllCtl_sysPllSscRnge                 14
    #define   MSK32Gbl_sysPllCtl_sysPllSscRnge                    0x00003FFF

    #define     BA_Gbl_sysPllCtl_sysPllSscGain2x               0x001D
    #define     B16Gbl_sysPllCtl_sysPllSscGain2x               0x001C
    #define   LSb32Gbl_sysPllCtl_sysPllSscGain2x                  14
    #define   LSb16Gbl_sysPllCtl_sysPllSscGain2x                  14
    #define       bGbl_sysPllCtl_sysPllSscGain2x               1
    #define   MSK32Gbl_sysPllCtl_sysPllSscGain2x                  0x00004000

    #define     BA_Gbl_sysPllCtl_sysPllSscmode                 0x001D
    #define     B16Gbl_sysPllCtl_sysPllSscmode                 0x001C
    #define   LSb32Gbl_sysPllCtl_sysPllSscmode                    15
    #define   LSb16Gbl_sysPllCtl_sysPllSscmode                    15
    #define       bGbl_sysPllCtl_sysPllSscmode                 1
    #define   MSK32Gbl_sysPllCtl_sysPllSscmode                    0x00008000
    #define        Gbl_sysPllCtl_sysPllSscmode_Centre                       0x0
    #define        Gbl_sysPllCtl_sysPllSscmode_Down                         0x1

    #define     BA_Gbl_sysPllCtl_sysPllSscResetExt             0x001E
    #define     B16Gbl_sysPllCtl_sysPllSscResetExt             0x001E
    #define   LSb32Gbl_sysPllCtl_sysPllSscResetExt                16
    #define   LSb16Gbl_sysPllCtl_sysPllSscResetExt                0
    #define       bGbl_sysPllCtl_sysPllSscResetExt             1
    #define   MSK32Gbl_sysPllCtl_sysPllSscResetExt                0x00010000
    #define        Gbl_sysPllCtl_sysPllSscResetExt_Int                      0x0
    #define        Gbl_sysPllCtl_sysPllSscResetExt_Ext                      0x1

    #define     BA_Gbl_sysPllCtl_sysPllTestMon                 0x001E
    #define     B16Gbl_sysPllCtl_sysPllTestMon                 0x001E
    #define   LSb32Gbl_sysPllCtl_sysPllTestMon                    17
    #define   LSb16Gbl_sysPllCtl_sysPllTestMon                    1
    #define       bGbl_sysPllCtl_sysPllTestMon                 4
    #define   MSK32Gbl_sysPllCtl_sysPllTestMon                    0x001E0000
    #define        Gbl_sysPllCtl_sysPllTestMon_Tri_state                    0x0
    #define        Gbl_sysPllCtl_sysPllTestMon_VDD_VCO                      0x1
    #define        Gbl_sysPllCtl_sysPllTestMon_VDD_DIV                      0x2
    #define        Gbl_sysPllCtl_sysPllTestMon_VDD_LAT                      0x3
    #define        Gbl_sysPllCtl_sysPllTestMon_AVDDR                        0x4
    #define        Gbl_sysPllCtl_sysPllTestMon_VDD_DIV1                     0x5
    #define        Gbl_sysPllCtl_sysPllTestMon_VDDR1P0                      0x6
    #define        Gbl_sysPllCtl_sysPllTestMon_VCOVDDF                      0x7

    #define     BA_Gbl_sysPllCtl_sysPllRsvd                    0x001E
    #define     B16Gbl_sysPllCtl_sysPllRsvd                    0x001E
    #define   LSb32Gbl_sysPllCtl_sysPllRsvd                       21
    #define   LSb16Gbl_sysPllCtl_sysPllRsvd                       5
    #define       bGbl_sysPllCtl_sysPllRsvd                    2
    #define   MSK32Gbl_sysPllCtl_sysPllRsvd                       0x00600000
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_memPllCtl                               0x0020

    #define     BA_Gbl_memPllCtl_memPllPu                      0x0020
    #define     B16Gbl_memPllCtl_memPllPu                      0x0020
    #define   LSb32Gbl_memPllCtl_memPllPu                         0
    #define   LSb16Gbl_memPllCtl_memPllPu                         0
    #define       bGbl_memPllCtl_memPllPu                      1
    #define   MSK32Gbl_memPllCtl_memPllPu                         0x00000001

    #define     BA_Gbl_memPllCtl_memPllRfDiv                   0x0020
    #define     B16Gbl_memPllCtl_memPllRfDiv                   0x0020
    #define   LSb32Gbl_memPllCtl_memPllRfDiv                      1
    #define   LSb16Gbl_memPllCtl_memPllRfDiv                      1
    #define       bGbl_memPllCtl_memPllRfDiv                   5
    #define   MSK32Gbl_memPllCtl_memPllRfDiv                      0x0000003E

    #define     BA_Gbl_memPllCtl_memPllFbDiv                   0x0020
    #define     B16Gbl_memPllCtl_memPllFbDiv                   0x0020
    #define   LSb32Gbl_memPllCtl_memPllFbDiv                      6
    #define   LSb16Gbl_memPllCtl_memPllFbDiv                      6
    #define       bGbl_memPllCtl_memPllFbDiv                   9
    #define   MSK32Gbl_memPllCtl_memPllFbDiv                      0x00007FC0

    #define     BA_Gbl_memPllCtl_memPllVddm                    0x0021
    #define     B16Gbl_memPllCtl_memPllVddm                    0x0020
    #define   LSb32Gbl_memPllCtl_memPllVddm                       15
    #define   LSb16Gbl_memPllCtl_memPllVddm                       15
    #define       bGbl_memPllCtl_memPllVddm                    2
    #define   MSK32Gbl_memPllCtl_memPllVddm                       0x00018000
    #define        Gbl_memPllCtl_memPllVddm_1R10v                           0x0
    #define        Gbl_memPllCtl_memPllVddm_1R15v                           0x1
    #define        Gbl_memPllCtl_memPllVddm_1R2v                            0x2
    #define        Gbl_memPllCtl_memPllVddm_1R25v                           0x3

    #define     BA_Gbl_memPllCtl_memPllVddl                    0x0022
    #define     B16Gbl_memPllCtl_memPllVddl                    0x0022
    #define   LSb32Gbl_memPllCtl_memPllVddl                       17
    #define   LSb16Gbl_memPllCtl_memPllVddl                       1
    #define       bGbl_memPllCtl_memPllVddl                    2
    #define   MSK32Gbl_memPllCtl_memPllVddl                       0x00060000
    #define        Gbl_memPllCtl_memPllVddl_0R90v                           0x0
    #define        Gbl_memPllCtl_memPllVddl_1R00v                           0x1
    #define        Gbl_memPllCtl_memPllVddl_1R05v                           0x2
    #define        Gbl_memPllCtl_memPllVddl_1R10v                           0x3

    #define     BA_Gbl_memPllCtl_memPllVR1P8V                  0x0022
    #define     B16Gbl_memPllCtl_memPllVR1P8V                  0x0022
    #define   LSb32Gbl_memPllCtl_memPllVR1P8V                     19
    #define   LSb16Gbl_memPllCtl_memPllVR1P8V                     3
    #define       bGbl_memPllCtl_memPllVR1P8V                  2
    #define   MSK32Gbl_memPllCtl_memPllVR1P8V                     0x00180000

    #define     BA_Gbl_memPllCtl_memPllVcoFbst                 0x0022
    #define     B16Gbl_memPllCtl_memPllVcoFbst                 0x0022
    #define   LSb32Gbl_memPllCtl_memPllVcoFbst                    21
    #define   LSb16Gbl_memPllCtl_memPllVcoFbst                    5
    #define       bGbl_memPllCtl_memPllVcoFbst                 1
    #define   MSK32Gbl_memPllCtl_memPllVcoFbst                    0x00200000

    #define     BA_Gbl_memPllCtl_memPllVcoRng                  0x0022
    #define     B16Gbl_memPllCtl_memPllVcoRng                  0x0022
    #define   LSb32Gbl_memPllCtl_memPllVcoRng                     22
    #define   LSb16Gbl_memPllCtl_memPllVcoRng                     6
    #define       bGbl_memPllCtl_memPllVcoRng                  2
    #define   MSK32Gbl_memPllCtl_memPllVcoRng                     0x00C00000
    #define        Gbl_memPllCtl_memPllVcoRng_Min                           0x0
    #define        Gbl_memPllCtl_memPllVcoRng_Max                           0x3

    #define     BA_Gbl_memPllCtl_memPllIcp                     0x0023
    #define     B16Gbl_memPllCtl_memPllIcp                     0x0022
    #define   LSb32Gbl_memPllCtl_memPllIcp                        24
    #define   LSb16Gbl_memPllCtl_memPllIcp                        8
    #define       bGbl_memPllCtl_memPllIcp                     3
    #define   MSK32Gbl_memPllCtl_memPllIcp                        0x07000000
    #define        Gbl_memPllCtl_memPllIcp_2R5uA                            0x0
    #define        Gbl_memPllCtl_memPllIcp_5R0uA                            0x1
    #define        Gbl_memPllCtl_memPllIcp_7R5uA                            0x2
    #define        Gbl_memPllCtl_memPllIcp_10R0uA                           0x3
    #define        Gbl_memPllCtl_memPllIcp_12R5uA                           0x4
    #define        Gbl_memPllCtl_memPllIcp_15R0uA                           0x5
    #define        Gbl_memPllCtl_memPllIcp_17R5uA                           0x6
    #define        Gbl_memPllCtl_memPllIcp_20R0uA                           0x7

    #define     BA_Gbl_memPllCtl_memPllKvco                    0x0023
    #define     B16Gbl_memPllCtl_memPllKvco                    0x0022
    #define   LSb32Gbl_memPllCtl_memPllKvco                       27
    #define   LSb16Gbl_memPllCtl_memPllKvco                       11
    #define       bGbl_memPllCtl_memPllKvco                    4
    #define   MSK32Gbl_memPllCtl_memPllKvco                       0x78000000
    #define        Gbl_memPllCtl_memPllKvco_500to700Mhz                     0x3
    #define        Gbl_memPllCtl_memPllKvco_700to1Ghz                       0x4

    #define     RA_Gbl_memPllCtl1                              0x0024

    #define     BA_Gbl_memPllCtl_memPllVcoDivSel               0x0024
    #define     B16Gbl_memPllCtl_memPllVcoDivSel               0x0024
    #define   LSb32Gbl_memPllCtl_memPllVcoDivSel                  0
    #define   LSb16Gbl_memPllCtl_memPllVcoDivSel                  0
    #define       bGbl_memPllCtl_memPllVcoDivSel               2
    #define   MSK32Gbl_memPllCtl_memPllVcoDivSel                  0x00000003
    #define        Gbl_memPllCtl_memPllVcoDivSel_1                          0x0
    #define        Gbl_memPllCtl_memPllVcoDivSel_2                          0x1
    #define        Gbl_memPllCtl_memPllVcoDivSel_4                          0x2
    #define        Gbl_memPllCtl_memPllVcoDivSel_8                          0x3

    #define     BA_Gbl_memPllCtl_memPllIntpi                   0x0024
    #define     B16Gbl_memPllCtl_memPllIntpi                   0x0024
    #define   LSb32Gbl_memPllCtl_memPllIntpi                      2
    #define   LSb16Gbl_memPllCtl_memPllIntpi                      2
    #define       bGbl_memPllCtl_memPllIntpi                   2
    #define   MSK32Gbl_memPllCtl_memPllIntpi                      0x0000000C
    #define        Gbl_memPllCtl_memPllIntpi_10uA                           0x0
    #define        Gbl_memPllCtl_memPllIntpi_15uA                           0x1
    #define        Gbl_memPllCtl_memPllIntpi_20uA                           0x2
    #define        Gbl_memPllCtl_memPllIntpi_25uA                           0x3

    #define     BA_Gbl_memPllCtl_memPllCapSel                  0x0024
    #define     B16Gbl_memPllCtl_memPllCapSel                  0x0024
    #define   LSb32Gbl_memPllCtl_memPllCapSel                     4
    #define   LSb16Gbl_memPllCtl_memPllCapSel                     4
    #define       bGbl_memPllCtl_memPllCapSel                  3
    #define   MSK32Gbl_memPllCtl_memPllCapSel                     0x00000070
    #define        Gbl_memPllCtl_memPllCapSel_Small                         0x0
    #define        Gbl_memPllCtl_memPllCapSel_Large                         0x7

    #define     BA_Gbl_memPllCtl_memPllSscEn                   0x0024
    #define     B16Gbl_memPllCtl_memPllSscEn                   0x0024
    #define   LSb32Gbl_memPllCtl_memPllSscEn                      7
    #define   LSb16Gbl_memPllCtl_memPllSscEn                      7
    #define       bGbl_memPllCtl_memPllSscEn                   1
    #define   MSK32Gbl_memPllCtl_memPllSscEn                      0x00000080
    #define        Gbl_memPllCtl_memPllSscEn_Disable                        0x0
    #define        Gbl_memPllCtl_memPllSscEn_Enable                         0x1

    #define     BA_Gbl_memPllCtl_memPllPuSsc                   0x0025
    #define     B16Gbl_memPllCtl_memPllPuSsc                   0x0024
    #define   LSb32Gbl_memPllCtl_memPllPuSsc                      8
    #define   LSb16Gbl_memPllCtl_memPllPuSsc                      8
    #define       bGbl_memPllCtl_memPllPuSsc                   1
    #define   MSK32Gbl_memPllCtl_memPllPuSsc                      0x00000100
    #define        Gbl_memPllCtl_memPllPuSsc_Disable                        0x0
    #define        Gbl_memPllCtl_memPllPuSsc_Enable                         0x1

    #define     BA_Gbl_memPllCtl_memPllSscFdiv                 0x0025
    #define     B16Gbl_memPllCtl_memPllSscFdiv                 0x0024
    #define   LSb32Gbl_memPllCtl_memPllSscFdiv                    9
    #define   LSb16Gbl_memPllCtl_memPllSscFdiv                    9
    #define       bGbl_memPllCtl_memPllSscFdiv                 15
    #define   MSK32Gbl_memPllCtl_memPllSscFdiv                    0x00FFFE00

    #define     RA_Gbl_memPllCtl2                              0x0028

    #define     BA_Gbl_memPllCtl_memPllSscRnge                 0x0028
    #define     B16Gbl_memPllCtl_memPllSscRnge                 0x0028
    #define   LSb32Gbl_memPllCtl_memPllSscRnge                    0
    #define   LSb16Gbl_memPllCtl_memPllSscRnge                    0
    #define       bGbl_memPllCtl_memPllSscRnge                 14
    #define   MSK32Gbl_memPllCtl_memPllSscRnge                    0x00003FFF

    #define     BA_Gbl_memPllCtl_memPllSscGain2x               0x0029
    #define     B16Gbl_memPllCtl_memPllSscGain2x               0x0028
    #define   LSb32Gbl_memPllCtl_memPllSscGain2x                  14
    #define   LSb16Gbl_memPllCtl_memPllSscGain2x                  14
    #define       bGbl_memPllCtl_memPllSscGain2x               1
    #define   MSK32Gbl_memPllCtl_memPllSscGain2x                  0x00004000

    #define     BA_Gbl_memPllCtl_memPllSscmode                 0x0029
    #define     B16Gbl_memPllCtl_memPllSscmode                 0x0028
    #define   LSb32Gbl_memPllCtl_memPllSscmode                    15
    #define   LSb16Gbl_memPllCtl_memPllSscmode                    15
    #define       bGbl_memPllCtl_memPllSscmode                 1
    #define   MSK32Gbl_memPllCtl_memPllSscmode                    0x00008000
    #define        Gbl_memPllCtl_memPllSscmode_Centre                       0x0
    #define        Gbl_memPllCtl_memPllSscmode_Down                         0x1

    #define     BA_Gbl_memPllCtl_memPllSscResetExt             0x002A
    #define     B16Gbl_memPllCtl_memPllSscResetExt             0x002A
    #define   LSb32Gbl_memPllCtl_memPllSscResetExt                16
    #define   LSb16Gbl_memPllCtl_memPllSscResetExt                0
    #define       bGbl_memPllCtl_memPllSscResetExt             1
    #define   MSK32Gbl_memPllCtl_memPllSscResetExt                0x00010000
    #define        Gbl_memPllCtl_memPllSscResetExt_Int                      0x0
    #define        Gbl_memPllCtl_memPllSscResetExt_Ext                      0x1

    #define     BA_Gbl_memPllCtl_memPllTestMon                 0x002A
    #define     B16Gbl_memPllCtl_memPllTestMon                 0x002A
    #define   LSb32Gbl_memPllCtl_memPllTestMon                    17
    #define   LSb16Gbl_memPllCtl_memPllTestMon                    1
    #define       bGbl_memPllCtl_memPllTestMon                 4
    #define   MSK32Gbl_memPllCtl_memPllTestMon                    0x001E0000
    #define        Gbl_memPllCtl_memPllTestMon_Tri_state                    0x0
    #define        Gbl_memPllCtl_memPllTestMon_VDD_VCO                      0x1
    #define        Gbl_memPllCtl_memPllTestMon_VDD_DIV                      0x2
    #define        Gbl_memPllCtl_memPllTestMon_VDD_LAT                      0x3
    #define        Gbl_memPllCtl_memPllTestMon_AVDDR                        0x4
    #define        Gbl_memPllCtl_memPllTestMon_VDD_DIV1                     0x5
    #define        Gbl_memPllCtl_memPllTestMon_VDDR1P0                      0x6
    #define        Gbl_memPllCtl_memPllTestMon_VCOVDDF                      0x7

    #define     BA_Gbl_memPllCtl_memPllRsvd                    0x002A
    #define     B16Gbl_memPllCtl_memPllRsvd                    0x002A
    #define   LSb32Gbl_memPllCtl_memPllRsvd                       21
    #define   LSb16Gbl_memPllCtl_memPllRsvd                       5
    #define       bGbl_memPllCtl_memPllRsvd                    2
    #define   MSK32Gbl_memPllCtl_memPllRsvd                       0x00600000
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_cpu1PllCtl                              0x002C

    #define     BA_Gbl_cpu1PllCtl_cpu1PllPu                    0x002C
    #define     B16Gbl_cpu1PllCtl_cpu1PllPu                    0x002C
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllPu                       0
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllPu                       0
    #define       bGbl_cpu1PllCtl_cpu1PllPu                    1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllPu                       0x00000001

    #define     BA_Gbl_cpu1PllCtl_cpu1PllRfDiv                 0x002C
    #define     B16Gbl_cpu1PllCtl_cpu1PllRfDiv                 0x002C
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllRfDiv                    1
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllRfDiv                    1
    #define       bGbl_cpu1PllCtl_cpu1PllRfDiv                 5
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllRfDiv                    0x0000003E

    #define     BA_Gbl_cpu1PllCtl_cpu1PllFbDiv                 0x002C
    #define     B16Gbl_cpu1PllCtl_cpu1PllFbDiv                 0x002C
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllFbDiv                    6
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllFbDiv                    6
    #define       bGbl_cpu1PllCtl_cpu1PllFbDiv                 9
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllFbDiv                    0x00007FC0

    #define     BA_Gbl_cpu1PllCtl_cpu1PllVddm                  0x002D
    #define     B16Gbl_cpu1PllCtl_cpu1PllVddm                  0x002C
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllVddm                     15
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllVddm                     15
    #define       bGbl_cpu1PllCtl_cpu1PllVddm                  2
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllVddm                     0x00018000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllVddl                  0x002E
    #define     B16Gbl_cpu1PllCtl_cpu1PllVddl                  0x002E
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllVddl                     17
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllVddl                     1
    #define       bGbl_cpu1PllCtl_cpu1PllVddl                  2
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllVddl                     0x00060000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllVcoFbst               0x002E
    #define     B16Gbl_cpu1PllCtl_cpu1PllVcoFbst               0x002E
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllVcoFbst                  19
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllVcoFbst                  3
    #define       bGbl_cpu1PllCtl_cpu1PllVcoFbst               1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllVcoFbst                  0x00080000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllIcp                   0x002E
    #define     B16Gbl_cpu1PllCtl_cpu1PllIcp                   0x002E
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllIcp                      20
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllIcp                      4
    #define       bGbl_cpu1PllCtl_cpu1PllIcp                   3
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllIcp                      0x00700000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllKvco                  0x002E
    #define     B16Gbl_cpu1PllCtl_cpu1PllKvco                  0x002E
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllKvco                     23
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllKvco                     7
    #define       bGbl_cpu1PllCtl_cpu1PllKvco                  4
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllKvco                     0x07800000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllVcoRng                0x002F
    #define     B16Gbl_cpu1PllCtl_cpu1PllVcoRng                0x002E
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllVcoRng                   27
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllVcoRng                   11
    #define       bGbl_cpu1PllCtl_cpu1PllVcoRng                3
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllVcoRng                   0x38000000

    #define     RA_Gbl_cpu1PllCtl1                             0x0030

    #define     BA_Gbl_cpu1PllCtl_cpu1PllVcoDivSelDiff         0x0030
    #define     B16Gbl_cpu1PllCtl_cpu1PllVcoDivSelDiff         0x0030
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllVcoDivSelDiff            0
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllVcoDivSelDiff            0
    #define       bGbl_cpu1PllCtl_cpu1PllVcoDivSelDiff         4
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllVcoDivSelDiff            0x0000000F

    #define     BA_Gbl_cpu1PllCtl_cpu1PllVcoDivSelSe           0x0030
    #define     B16Gbl_cpu1PllCtl_cpu1PllVcoDivSelSe           0x0030
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllVcoDivSelSe              4
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllVcoDivSelSe              4
    #define       bGbl_cpu1PllCtl_cpu1PllVcoDivSelSe           4
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllVcoDivSelSe              0x000000F0

    #define     BA_Gbl_cpu1PllCtl_cpu1PllDiffClkEn             0x0031
    #define     B16Gbl_cpu1PllCtl_cpu1PllDiffClkEn             0x0030
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllDiffClkEn                8
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllDiffClkEn                8
    #define       bGbl_cpu1PllCtl_cpu1PllDiffClkEn             1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllDiffClkEn                0x00000100

    #define     BA_Gbl_cpu1PllCtl_cpu1PllSelVcoDiff            0x0031
    #define     B16Gbl_cpu1PllCtl_cpu1PllSelVcoDiff            0x0030
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllSelVcoDiff               9
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllSelVcoDiff               9
    #define       bGbl_cpu1PllCtl_cpu1PllSelVcoDiff            1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllSelVcoDiff               0x00000200

    #define     BA_Gbl_cpu1PllCtl_cpu1PllSelVcoClkSe           0x0031
    #define     B16Gbl_cpu1PllCtl_cpu1PllSelVcoClkSe           0x0030
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllSelVcoClkSe              10
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllSelVcoClkSe              10
    #define       bGbl_cpu1PllCtl_cpu1PllSelVcoClkSe           1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllSelVcoClkSe              0x00000400

    #define     BA_Gbl_cpu1PllCtl_cpu1PllBypassFbDiv           0x0031
    #define     B16Gbl_cpu1PllCtl_cpu1PllBypassFbDiv           0x0030
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllBypassFbDiv              11
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllBypassFbDiv              11
    #define       bGbl_cpu1PllCtl_cpu1PllBypassFbDiv           1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllBypassFbDiv              0x00000800

    #define     BA_Gbl_cpu1PllCtl_cpu1PllGateClkCntrl          0x0031
    #define     B16Gbl_cpu1PllCtl_cpu1PllGateClkCntrl          0x0030
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllGateClkCntrl             12
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllGateClkCntrl             12
    #define       bGbl_cpu1PllCtl_cpu1PllGateClkCntrl          1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllGateClkCntrl             0x00001000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllIntpi                 0x0031
    #define     B16Gbl_cpu1PllCtl_cpu1PllIntpi                 0x0030
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllIntpi                    13
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllIntpi                    13
    #define       bGbl_cpu1PllCtl_cpu1PllIntpi                 3
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllIntpi                    0x0000E000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllFreqOffsetEn          0x0032
    #define     B16Gbl_cpu1PllCtl_cpu1PllFreqOffsetEn          0x0032
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllFreqOffsetEn             16
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllFreqOffsetEn             0
    #define       bGbl_cpu1PllCtl_cpu1PllFreqOffsetEn          1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllFreqOffsetEn             0x00010000

    #define     RA_Gbl_cpu1PllCtl2                             0x0034

    #define     BA_Gbl_cpu1PllCtl_cpu1PllFreqOffset            0x0034
    #define     B16Gbl_cpu1PllCtl_cpu1PllFreqOffset            0x0034
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllFreqOffset               0
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllFreqOffset               0
    #define       bGbl_cpu1PllCtl_cpu1PllFreqOffset            17
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllFreqOffset               0x0001FFFF

    #define     BA_Gbl_cpu1PllCtl_cpu1PllFreqOffsetValid       0x0036
    #define     B16Gbl_cpu1PllCtl_cpu1PllFreqOffsetValid       0x0036
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllFreqOffsetValid          17
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllFreqOffsetValid          1
    #define       bGbl_cpu1PllCtl_cpu1PllFreqOffsetValid       1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllFreqOffsetValid          0x00020000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllClkDetEn              0x0036
    #define     B16Gbl_cpu1PllCtl_cpu1PllClkDetEn              0x0036
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllClkDetEn                 18
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllClkDetEn                 2
    #define       bGbl_cpu1PllCtl_cpu1PllClkDetEn              1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllClkDetEn                 0x00040000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllExtRst                0x0036
    #define     B16Gbl_cpu1PllCtl_cpu1PllExtRst                0x0036
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllExtRst                   19
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllExtRst                   3
    #define       bGbl_cpu1PllCtl_cpu1PllExtRst                1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllExtRst                   0x00080000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllFreqOffsetModeSel     0x0036
    #define     B16Gbl_cpu1PllCtl_cpu1PllFreqOffsetModeSel     0x0036
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllFreqOffsetModeSel        20
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllFreqOffsetModeSel        4
    #define       bGbl_cpu1PllCtl_cpu1PllFreqOffsetModeSel     1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllFreqOffsetModeSel        0x00100000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllSscEn                 0x0036
    #define     B16Gbl_cpu1PllCtl_cpu1PllSscEn                 0x0036
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllSscEn                    21
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllSscEn                    5
    #define       bGbl_cpu1PllCtl_cpu1PllSscEn                 1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllSscEn                    0x00200000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllSscMode               0x0036
    #define     B16Gbl_cpu1PllCtl_cpu1PllSscMode               0x0036
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllSscMode                  22
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllSscMode                  6
    #define       bGbl_cpu1PllCtl_cpu1PllSscMode               1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllSscMode                  0x00400000

    #define     RA_Gbl_cpu1PllCtl3                             0x0038

    #define     BA_Gbl_cpu1PllCtl_cpu1PllSscFreqDiv            0x0038
    #define     B16Gbl_cpu1PllCtl_cpu1PllSscFreqDiv            0x0038
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllSscFreqDiv               0
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllSscFreqDiv               0
    #define       bGbl_cpu1PllCtl_cpu1PllSscFreqDiv            15
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllSscFreqDiv               0x00007FFF

    #define     BA_Gbl_cpu1PllCtl_cpu1PllSscRnge               0x0039
    #define     B16Gbl_cpu1PllCtl_cpu1PllSscRnge               0x0038
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllSscRnge                  15
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllSscRnge                  15
    #define       bGbl_cpu1PllCtl_cpu1PllSscRnge               11
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllSscRnge                  0x03FF8000

    #define     BA_Gbl_cpu1PllCtl_cpu1PllSscExtRst             0x003B
    #define     B16Gbl_cpu1PllCtl_cpu1PllSscExtRst             0x003A
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllSscExtRst                26
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllSscExtRst                10
    #define       bGbl_cpu1PllCtl_cpu1PllSscExtRst             1
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllSscExtRst                0x04000000

    #define     RA_Gbl_cpu1PllCtl4                             0x003C

    #define     BA_Gbl_cpu1PllCtl_cpu1PllTestMon               0x003C
    #define     B16Gbl_cpu1PllCtl_cpu1PllTestMon               0x003C
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllTestMon                  0
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllTestMon                  0
    #define       bGbl_cpu1PllCtl_cpu1PllTestMon               6
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllTestMon                  0x0000003F

    #define     BA_Gbl_cpu1PllCtl_cpu1PllReserve               0x003C
    #define     B16Gbl_cpu1PllCtl_cpu1PllReserve               0x003C
    #define   LSb32Gbl_cpu1PllCtl_cpu1PllReserve                  6
    #define   LSb16Gbl_cpu1PllCtl_cpu1PllReserve                  6
    #define       bGbl_cpu1PllCtl_cpu1PllReserve               2
    #define   MSK32Gbl_cpu1PllCtl_cpu1PllReserve                  0x000000C0
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_avPllCtl                                0x0040

    #define     BA_Gbl_avPllCtl_avPllReset_Pll_A               0x0040
    #define     B16Gbl_avPllCtl_avPllReset_Pll_A               0x0040
    #define   LSb32Gbl_avPllCtl_avPllReset_Pll_A                  0
    #define   LSb16Gbl_avPllCtl_avPllReset_Pll_A                  0
    #define       bGbl_avPllCtl_avPllReset_Pll_A               1
    #define   MSK32Gbl_avPllCtl_avPllReset_Pll_A                  0x00000001

    #define     BA_Gbl_avPllCtl_avPllReset_Pll_B               0x0040
    #define     B16Gbl_avPllCtl_avPllReset_Pll_B               0x0040
    #define   LSb32Gbl_avPllCtl_avPllReset_Pll_B                  1
    #define   LSb16Gbl_avPllCtl_avPllReset_Pll_B                  1
    #define       bGbl_avPllCtl_avPllReset_Pll_B               1
    #define   MSK32Gbl_avPllCtl_avPllReset_Pll_B                  0x00000002

    #define     BA_Gbl_avPllCtl_avPllPu                        0x0040
    #define     B16Gbl_avPllCtl_avPllPu                        0x0040
    #define   LSb32Gbl_avPllCtl_avPllPu                           2
    #define   LSb16Gbl_avPllCtl_avPllPu                           2
    #define       bGbl_avPllCtl_avPllPu                        1
    #define   MSK32Gbl_avPllCtl_avPllPu                           0x00000004

    #define     BA_Gbl_avPllCtl_avPllPu_Pll_B                  0x0040
    #define     B16Gbl_avPllCtl_avPllPu_Pll_B                  0x0040
    #define   LSb32Gbl_avPllCtl_avPllPu_Pll_B                     3
    #define   LSb16Gbl_avPllCtl_avPllPu_Pll_B                     3
    #define       bGbl_avPllCtl_avPllPu_Pll_B                  1
    #define   MSK32Gbl_avPllCtl_avPllPu_Pll_B                     0x00000008

    #define     BA_Gbl_avPllCtl_avPllPll_RegPump               0x0040
    #define     B16Gbl_avPllCtl_avPllPll_RegPump               0x0040
    #define   LSb32Gbl_avPllCtl_avPllPll_RegPump                  4
    #define   LSb16Gbl_avPllCtl_avPllPll_RegPump                  4
    #define       bGbl_avPllCtl_avPllPll_RegPump               2
    #define   MSK32Gbl_avPllCtl_avPllPll_RegPump                  0x00000030

    #define     BA_Gbl_avPllCtl_avPllVDDR20_VCO                0x0040
    #define     B16Gbl_avPllCtl_avPllVDDR20_VCO                0x0040
    #define   LSb32Gbl_avPllCtl_avPllVDDR20_VCO                   6
    #define   LSb16Gbl_avPllCtl_avPllVDDR20_VCO                   6
    #define       bGbl_avPllCtl_avPllVDDR20_VCO                2
    #define   MSK32Gbl_avPllCtl_avPllVDDR20_VCO                   0x000000C0

    #define     BA_Gbl_avPllCtl_avPllVDDR17_VCO                0x0041
    #define     B16Gbl_avPllCtl_avPllVDDR17_VCO                0x0040
    #define   LSb32Gbl_avPllCtl_avPllVDDR17_VCO                   8
    #define   LSb16Gbl_avPllCtl_avPllVDDR17_VCO                   8
    #define       bGbl_avPllCtl_avPllVDDR17_VCO                2
    #define   MSK32Gbl_avPllCtl_avPllVDDR17_VCO                   0x00000300

    #define     BA_Gbl_avPllCtl_avPllVDDR10_VCO                0x0041
    #define     B16Gbl_avPllCtl_avPllVDDR10_VCO                0x0040
    #define   LSb32Gbl_avPllCtl_avPllVDDR10_VCO                   10
    #define   LSb16Gbl_avPllCtl_avPllVDDR10_VCO                   10
    #define       bGbl_avPllCtl_avPllVDDR10_VCO                2
    #define   MSK32Gbl_avPllCtl_avPllVDDR10_VCO                   0x00000C00

    #define     BA_Gbl_avPllCtl_avPllVTHVOCal                  0x0041
    #define     B16Gbl_avPllCtl_avPllVTHVOCal                  0x0040
    #define   LSb32Gbl_avPllCtl_avPllVTHVOCal                     12
    #define   LSb16Gbl_avPllCtl_avPllVTHVOCal                     12
    #define       bGbl_avPllCtl_avPllVTHVOCal                  2
    #define   MSK32Gbl_avPllCtl_avPllVTHVOCal                     0x00003000

    #define     BA_Gbl_avPllCtl_avPllSpeed_A                   0x0041
    #define     B16Gbl_avPllCtl_avPllSpeed_A                   0x0040
    #define   LSb32Gbl_avPllCtl_avPllSpeed_A                      14
    #define   LSb16Gbl_avPllCtl_avPllSpeed_A                      14
    #define       bGbl_avPllCtl_avPllSpeed_A                   2
    #define   MSK32Gbl_avPllCtl_avPllSpeed_A                      0x0000C000

    #define     BA_Gbl_avPllCtl_avPllSpeed_B                   0x0042
    #define     B16Gbl_avPllCtl_avPllSpeed_B                   0x0042
    #define   LSb32Gbl_avPllCtl_avPllSpeed_B                      16
    #define   LSb16Gbl_avPllCtl_avPllSpeed_B                      0
    #define       bGbl_avPllCtl_avPllSpeed_B                   2
    #define   MSK32Gbl_avPllCtl_avPllSpeed_B                      0x00030000

    #define     BA_Gbl_avPllCtl_avPllClkDetEn                  0x0042
    #define     B16Gbl_avPllCtl_avPllClkDetEn                  0x0042
    #define   LSb32Gbl_avPllCtl_avPllClkDetEn                     18
    #define   LSb16Gbl_avPllCtl_avPllClkDetEn                     2
    #define       bGbl_avPllCtl_avPllClkDetEn                  1
    #define   MSK32Gbl_avPllCtl_avPllClkDetEn                     0x00040000

    #define     BA_Gbl_avPllCtl_avPllIcp_A                     0x0042
    #define     B16Gbl_avPllCtl_avPllIcp_A                     0x0042
    #define   LSb32Gbl_avPllCtl_avPllIcp_A                        19
    #define   LSb16Gbl_avPllCtl_avPllIcp_A                        3
    #define       bGbl_avPllCtl_avPllIcp_A                     4
    #define   MSK32Gbl_avPllCtl_avPllIcp_A                        0x00780000

    #define     BA_Gbl_avPllCtl_avPllIcp_B                     0x0042
    #define     B16Gbl_avPllCtl_avPllIcp_B                     0x0042
    #define   LSb32Gbl_avPllCtl_avPllIcp_B                        23
    #define   LSb16Gbl_avPllCtl_avPllIcp_B                        7
    #define       bGbl_avPllCtl_avPllIcp_B                     4
    #define   MSK32Gbl_avPllCtl_avPllIcp_B                        0x07800000

    #define     BA_Gbl_avPllCtl_avPllRfDiv                     0x0043
    #define     B16Gbl_avPllCtl_avPllRfDiv                     0x0042
    #define   LSb32Gbl_avPllCtl_avPllRfDiv                        27
    #define   LSb16Gbl_avPllCtl_avPllRfDiv                        11
    #define       bGbl_avPllCtl_avPllRfDiv                     2
    #define   MSK32Gbl_avPllCtl_avPllRfDiv                        0x18000000

    #define     RA_Gbl_avPllCtl1                               0x0044

    #define     BA_Gbl_avPllCtl_avPllFbDiv_A                   0x0044
    #define     B16Gbl_avPllCtl_avPllFbDiv_A                   0x0044
    #define   LSb32Gbl_avPllCtl_avPllFbDiv_A                      0
    #define   LSb16Gbl_avPllCtl_avPllFbDiv_A                      0
    #define       bGbl_avPllCtl_avPllFbDiv_A                   8
    #define   MSK32Gbl_avPllCtl_avPllFbDiv_A                      0x000000FF

    #define     BA_Gbl_avPllCtl_avPllFbDiv_B                   0x0045
    #define     B16Gbl_avPllCtl_avPllFbDiv_B                   0x0044
    #define   LSb32Gbl_avPllCtl_avPllFbDiv_B                      8
    #define   LSb16Gbl_avPllCtl_avPllFbDiv_B                      8
    #define       bGbl_avPllCtl_avPllFbDiv_B                   8
    #define   MSK32Gbl_avPllCtl_avPllFbDiv_B                      0x0000FF00

    #define     BA_Gbl_avPllCtl_avPllSelV2i_A                  0x0046
    #define     B16Gbl_avPllCtl_avPllSelV2i_A                  0x0046
    #define   LSb32Gbl_avPllCtl_avPllSelV2i_A                     16
    #define   LSb16Gbl_avPllCtl_avPllSelV2i_A                     0
    #define       bGbl_avPllCtl_avPllSelV2i_A                  3
    #define   MSK32Gbl_avPllCtl_avPllSelV2i_A                     0x00070000

    #define     BA_Gbl_avPllCtl_avPllSelV2i_B                  0x0046
    #define     B16Gbl_avPllCtl_avPllSelV2i_B                  0x0046
    #define   LSb32Gbl_avPllCtl_avPllSelV2i_B                     19
    #define   LSb16Gbl_avPllCtl_avPllSelV2i_B                     3
    #define       bGbl_avPllCtl_avPllSelV2i_B                  3
    #define   MSK32Gbl_avPllCtl_avPllSelV2i_B                     0x00380000

    #define     BA_Gbl_avPllCtl_avPllSel_LpFRes_A              0x0046
    #define     B16Gbl_avPllCtl_avPllSel_LpFRes_A              0x0046
    #define   LSb32Gbl_avPllCtl_avPllSel_LpFRes_A                 22
    #define   LSb16Gbl_avPllCtl_avPllSel_LpFRes_A                 6
    #define       bGbl_avPllCtl_avPllSel_LpFRes_A              1
    #define   MSK32Gbl_avPllCtl_avPllSel_LpFRes_A                 0x00400000

    #define     BA_Gbl_avPllCtl_avPllSel_LpFRes_B              0x0046
    #define     B16Gbl_avPllCtl_avPllSel_LpFRes_B              0x0046
    #define   LSb32Gbl_avPllCtl_avPllSel_LpFRes_B                 23
    #define   LSb16Gbl_avPllCtl_avPllSel_LpFRes_B                 7
    #define       bGbl_avPllCtl_avPllSel_LpFRes_B              1
    #define   MSK32Gbl_avPllCtl_avPllSel_LpFRes_B                 0x00800000

    #define     BA_Gbl_avPllCtl_avPllCalStart                  0x0047
    #define     B16Gbl_avPllCtl_avPllCalStart                  0x0046
    #define   LSb32Gbl_avPllCtl_avPllCalStart                     24
    #define   LSb16Gbl_avPllCtl_avPllCalStart                     8
    #define       bGbl_avPllCtl_avPllCalStart                  1
    #define   MSK32Gbl_avPllCtl_avPllCalStart                     0x01000000

    #define     BA_Gbl_avPllCtl_avPllKVCO_Ext_En               0x0047
    #define     B16Gbl_avPllCtl_avPllKVCO_Ext_En               0x0046
    #define   LSb32Gbl_avPllCtl_avPllKVCO_Ext_En                  25
    #define   LSb16Gbl_avPllCtl_avPllKVCO_Ext_En                  9
    #define       bGbl_avPllCtl_avPllKVCO_Ext_En               1
    #define   MSK32Gbl_avPllCtl_avPllKVCO_Ext_En                  0x02000000

    #define     BA_Gbl_avPllCtl_avPllKVCO_A                    0x0047
    #define     B16Gbl_avPllCtl_avPllKVCO_A                    0x0046
    #define   LSb32Gbl_avPllCtl_avPllKVCO_A                       26
    #define   LSb16Gbl_avPllCtl_avPllKVCO_A                       10
    #define       bGbl_avPllCtl_avPllKVCO_A                    3
    #define   MSK32Gbl_avPllCtl_avPllKVCO_A                       0x1C000000

    #define     BA_Gbl_avPllCtl_avPllKVCO_B                    0x0047
    #define     B16Gbl_avPllCtl_avPllKVCO_B                    0x0046
    #define   LSb32Gbl_avPllCtl_avPllKVCO_B                       29
    #define   LSb16Gbl_avPllCtl_avPllKVCO_B                       13
    #define       bGbl_avPllCtl_avPllKVCO_B                    3
    #define   MSK32Gbl_avPllCtl_avPllKVCO_B                       0xE0000000

    #define     RA_Gbl_avPllCtl2                               0x0048

    #define     BA_Gbl_avPllCtl_avPllReset_Ext_A               0x0048
    #define     B16Gbl_avPllCtl_avPllReset_Ext_A               0x0048
    #define   LSb32Gbl_avPllCtl_avPllReset_Ext_A                  0
    #define   LSb16Gbl_avPllCtl_avPllReset_Ext_A                  0
    #define       bGbl_avPllCtl_avPllReset_Ext_A               8
    #define   MSK32Gbl_avPllCtl_avPllReset_Ext_A                  0x000000FF

    #define     BA_Gbl_avPllCtl_avPllReset_Ext_B               0x0049
    #define     B16Gbl_avPllCtl_avPllReset_Ext_B               0x0048
    #define   LSb32Gbl_avPllCtl_avPllReset_Ext_B                  8
    #define   LSb16Gbl_avPllCtl_avPllReset_Ext_B                  8
    #define       bGbl_avPllCtl_avPllReset_Ext_B               8
    #define   MSK32Gbl_avPllCtl_avPllReset_Ext_B                  0x0000FF00

    #define     BA_Gbl_avPllCtl_avPllIntpi_A                   0x004A
    #define     B16Gbl_avPllCtl_avPllIntpi_A                   0x004A
    #define   LSb32Gbl_avPllCtl_avPllIntpi_A                      16
    #define   LSb16Gbl_avPllCtl_avPllIntpi_A                      0
    #define       bGbl_avPllCtl_avPllIntpi_A                   3
    #define   MSK32Gbl_avPllCtl_avPllIntpi_A                      0x00070000

    #define     BA_Gbl_avPllCtl_avPllIntpi_B                   0x004A
    #define     B16Gbl_avPllCtl_avPllIntpi_B                   0x004A
    #define   LSb32Gbl_avPllCtl_avPllIntpi_B                      19
    #define   LSb16Gbl_avPllCtl_avPllIntpi_B                      3
    #define       bGbl_avPllCtl_avPllIntpi_B                   3
    #define   MSK32Gbl_avPllCtl_avPllIntpi_B                      0x00380000

    #define     BA_Gbl_avPllCtl_avPll1000PPM_En_A              0x004A
    #define     B16Gbl_avPllCtl_avPll1000PPM_En_A              0x004A
    #define   LSb32Gbl_avPllCtl_avPll1000PPM_En_A                 22
    #define   LSb16Gbl_avPllCtl_avPll1000PPM_En_A                 6
    #define       bGbl_avPllCtl_avPll1000PPM_En_A              7
    #define   MSK32Gbl_avPllCtl_avPll1000PPM_En_A                 0x1FC00000

    #define     RA_Gbl_avPllCtl3                               0x004C

    #define     BA_Gbl_avPllCtl_avPll1000PPM_En_B              0x004C
    #define     B16Gbl_avPllCtl_avPll1000PPM_En_B              0x004C
    #define   LSb32Gbl_avPllCtl_avPll1000PPM_En_B                 0
    #define   LSb16Gbl_avPllCtl_avPll1000PPM_En_B                 0
    #define       bGbl_avPllCtl_avPll1000PPM_En_B              7
    #define   MSK32Gbl_avPllCtl_avPll1000PPM_En_B                 0x0000007F

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_A1            0x004C
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_A1            0x004C
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_A1               7
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_A1               7
    #define       bGbl_avPllCtl_avPllFreq_Offset_A1            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_A1               0x007FFF80

    #define     RA_Gbl_avPllCtl4                               0x0050

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_A2            0x0050
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_A2            0x0050
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_A2               0
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_A2               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_A2            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_A2               0x0000FFFF

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_A3            0x0052
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_A3            0x0052
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_A3               16
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_A3               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_A3            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_A3               0xFFFF0000

    #define     RA_Gbl_avPllCtl5                               0x0054

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_A4            0x0054
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_A4            0x0054
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_A4               0
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_A4               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_A4            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_A4               0x0000FFFF

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_A5            0x0056
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_A5            0x0056
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_A5               16
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_A5               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_A5            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_A5               0xFFFF0000

    #define     RA_Gbl_avPllCtl6                               0x0058

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_A6            0x0058
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_A6            0x0058
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_A6               0
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_A6               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_A6            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_A6               0x0000FFFF

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_A7            0x005A
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_A7            0x005A
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_A7               16
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_A7               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_A7            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_A7               0xFFFF0000

    #define     RA_Gbl_avPllCtl7                               0x005C

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_A8            0x005C
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_A8            0x005C
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_A8               0
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_A8               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_A8            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_A8               0x0000FFFF

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_B1            0x005E
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_B1            0x005E
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_B1               16
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_B1               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_B1            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_B1               0xFFFF0000

    #define     RA_Gbl_avPllCtl8                               0x0060

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_B2            0x0060
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_B2            0x0060
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_B2               0
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_B2               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_B2            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_B2               0x0000FFFF

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_B3            0x0062
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_B3            0x0062
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_B3               16
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_B3               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_B3            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_B3               0xFFFF0000

    #define     RA_Gbl_avPllCtl9                               0x0064

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_B4            0x0064
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_B4            0x0064
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_B4               0
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_B4               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_B4            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_B4               0x0000FFFF

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_B5            0x0066
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_B5            0x0066
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_B5               16
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_B5               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_B5            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_B5               0xFFFF0000

    #define     RA_Gbl_avPllCtl10                              0x0068

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_B6            0x0068
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_B6            0x0068
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_B6               0
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_B6               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_B6            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_B6               0x0000FFFF

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_B7            0x006A
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_B7            0x006A
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_B7               16
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_B7               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_B7            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_B7               0xFFFF0000

    #define     RA_Gbl_avPllCtl11                              0x006C

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_B8            0x006C
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_B8            0x006C
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_B8               0
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_B8               0
    #define       bGbl_avPllCtl_avPllFreq_Offset_B8            16
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_B8               0x0000FFFF

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_Rdy_A         0x006E
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_Rdy_A         0x006E
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_Rdy_A            16
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_Rdy_A            0
    #define       bGbl_avPllCtl_avPllFreq_Offset_Rdy_A         7
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_Rdy_A            0x007F0000

    #define     BA_Gbl_avPllCtl_avPllFreq_Offset_Rdy_B         0x006E
    #define     B16Gbl_avPllCtl_avPllFreq_Offset_Rdy_B         0x006E
    #define   LSb32Gbl_avPllCtl_avPllFreq_Offset_Rdy_B            23
    #define   LSb16Gbl_avPllCtl_avPllFreq_Offset_Rdy_B            7
    #define       bGbl_avPllCtl_avPllFreq_Offset_Rdy_B         7
    #define   MSK32Gbl_avPllCtl_avPllFreq_Offset_Rdy_B            0x3F800000

    #define     RA_Gbl_avPllCtl12                              0x0070

    #define     BA_Gbl_avPllCtl_avPllPU_A                      0x0070
    #define     B16Gbl_avPllCtl_avPllPU_A                      0x0070
    #define   LSb32Gbl_avPllCtl_avPllPU_A                         0
    #define   LSb16Gbl_avPllCtl_avPllPU_A                         0
    #define       bGbl_avPllCtl_avPllPU_A                      7
    #define   MSK32Gbl_avPllCtl_avPllPU_A                         0x0000007F

    #define     BA_Gbl_avPllCtl_avPllPU_B                      0x0070
    #define     B16Gbl_avPllCtl_avPllPU_B                      0x0070
    #define   LSb32Gbl_avPllCtl_avPllPU_B                         7
    #define   LSb16Gbl_avPllCtl_avPllPU_B                         7
    #define       bGbl_avPllCtl_avPllPU_B                      7
    #define   MSK32Gbl_avPllCtl_avPllPU_B                         0x00003F80

    #define     BA_Gbl_avPllCtl_avPllDiv_A1                    0x0071
    #define     B16Gbl_avPllCtl_avPllDiv_A1                    0x0070
    #define   LSb32Gbl_avPllCtl_avPllDiv_A1                       14
    #define   LSb16Gbl_avPllCtl_avPllDiv_A1                       14
    #define       bGbl_avPllCtl_avPllDiv_A1                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_A1                       0x007FC000

    #define     BA_Gbl_avPllCtl_avPllDiv_A2                    0x0072
    #define     B16Gbl_avPllCtl_avPllDiv_A2                    0x0072
    #define   LSb32Gbl_avPllCtl_avPllDiv_A2                       23
    #define   LSb16Gbl_avPllCtl_avPllDiv_A2                       7
    #define       bGbl_avPllCtl_avPllDiv_A2                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_A2                       0xFF800000

    #define     RA_Gbl_avPllCtl13                              0x0074

    #define     BA_Gbl_avPllCtl_avPllDiv_A3                    0x0074
    #define     B16Gbl_avPllCtl_avPllDiv_A3                    0x0074
    #define   LSb32Gbl_avPllCtl_avPllDiv_A3                       0
    #define   LSb16Gbl_avPllCtl_avPllDiv_A3                       0
    #define       bGbl_avPllCtl_avPllDiv_A3                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_A3                       0x000001FF

    #define     BA_Gbl_avPllCtl_avPllDiv_A4                    0x0075
    #define     B16Gbl_avPllCtl_avPllDiv_A4                    0x0074
    #define   LSb32Gbl_avPllCtl_avPllDiv_A4                       9
    #define   LSb16Gbl_avPllCtl_avPllDiv_A4                       9
    #define       bGbl_avPllCtl_avPllDiv_A4                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_A4                       0x0003FE00

    #define     BA_Gbl_avPllCtl_avPllDiv_A5                    0x0076
    #define     B16Gbl_avPllCtl_avPllDiv_A5                    0x0076
    #define   LSb32Gbl_avPllCtl_avPllDiv_A5                       18
    #define   LSb16Gbl_avPllCtl_avPllDiv_A5                       2
    #define       bGbl_avPllCtl_avPllDiv_A5                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_A5                       0x07FC0000

    #define     RA_Gbl_avPllCtl14                              0x0078

    #define     BA_Gbl_avPllCtl_avPllDiv_A6                    0x0078
    #define     B16Gbl_avPllCtl_avPllDiv_A6                    0x0078
    #define   LSb32Gbl_avPllCtl_avPllDiv_A6                       0
    #define   LSb16Gbl_avPllCtl_avPllDiv_A6                       0
    #define       bGbl_avPllCtl_avPllDiv_A6                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_A6                       0x000001FF

    #define     BA_Gbl_avPllCtl_avPllDiv_A7                    0x0079
    #define     B16Gbl_avPllCtl_avPllDiv_A7                    0x0078
    #define   LSb32Gbl_avPllCtl_avPllDiv_A7                       9
    #define   LSb16Gbl_avPllCtl_avPllDiv_A7                       9
    #define       bGbl_avPllCtl_avPllDiv_A7                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_A7                       0x0003FE00

    #define     BA_Gbl_avPllCtl_avPllDiv_B1                    0x007A
    #define     B16Gbl_avPllCtl_avPllDiv_B1                    0x007A
    #define   LSb32Gbl_avPllCtl_avPllDiv_B1                       18
    #define   LSb16Gbl_avPllCtl_avPllDiv_B1                       2
    #define       bGbl_avPllCtl_avPllDiv_B1                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_B1                       0x07FC0000

    #define     RA_Gbl_avPllCtl15                              0x007C

    #define     BA_Gbl_avPllCtl_avPllDiv_B2                    0x007C
    #define     B16Gbl_avPllCtl_avPllDiv_B2                    0x007C
    #define   LSb32Gbl_avPllCtl_avPllDiv_B2                       0
    #define   LSb16Gbl_avPllCtl_avPllDiv_B2                       0
    #define       bGbl_avPllCtl_avPllDiv_B2                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_B2                       0x000001FF

    #define     BA_Gbl_avPllCtl_avPllDiv_B3                    0x007D
    #define     B16Gbl_avPllCtl_avPllDiv_B3                    0x007C
    #define   LSb32Gbl_avPllCtl_avPllDiv_B3                       9
    #define   LSb16Gbl_avPllCtl_avPllDiv_B3                       9
    #define       bGbl_avPllCtl_avPllDiv_B3                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_B3                       0x0003FE00

    #define     BA_Gbl_avPllCtl_avPllDiv_B4                    0x007E
    #define     B16Gbl_avPllCtl_avPllDiv_B4                    0x007E
    #define   LSb32Gbl_avPllCtl_avPllDiv_B4                       18
    #define   LSb16Gbl_avPllCtl_avPllDiv_B4                       2
    #define       bGbl_avPllCtl_avPllDiv_B4                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_B4                       0x07FC0000

    #define     RA_Gbl_avPllCtl16                              0x0080

    #define     BA_Gbl_avPllCtl_avPllDiv_B5                    0x0080
    #define     B16Gbl_avPllCtl_avPllDiv_B5                    0x0080
    #define   LSb32Gbl_avPllCtl_avPllDiv_B5                       0
    #define   LSb16Gbl_avPllCtl_avPllDiv_B5                       0
    #define       bGbl_avPllCtl_avPllDiv_B5                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_B5                       0x000001FF

    #define     BA_Gbl_avPllCtl_avPllDiv_B6                    0x0081
    #define     B16Gbl_avPllCtl_avPllDiv_B6                    0x0080
    #define   LSb32Gbl_avPllCtl_avPllDiv_B6                       9
    #define   LSb16Gbl_avPllCtl_avPllDiv_B6                       9
    #define       bGbl_avPllCtl_avPllDiv_B6                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_B6                       0x0003FE00

    #define     BA_Gbl_avPllCtl_avPllDiv_B7                    0x0082
    #define     B16Gbl_avPllCtl_avPllDiv_B7                    0x0082
    #define   LSb32Gbl_avPllCtl_avPllDiv_B7                       18
    #define   LSb16Gbl_avPllCtl_avPllDiv_B7                       2
    #define       bGbl_avPllCtl_avPllDiv_B7                    9
    #define   MSK32Gbl_avPllCtl_avPllDiv_B7                       0x07FC0000

    #define     BA_Gbl_avPllCtl_avPllDiv2_A1                   0x0083
    #define     B16Gbl_avPllCtl_avPllDiv2_A1                   0x0082
    #define   LSb32Gbl_avPllCtl_avPllDiv2_A1                      27
    #define   LSb16Gbl_avPllCtl_avPllDiv2_A1                      11
    #define       bGbl_avPllCtl_avPllDiv2_A1                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_A1                      0x78000000

    #define     RA_Gbl_avPllCtl17                              0x0084

    #define     BA_Gbl_avPllCtl_avPllDiv2_A2                   0x0084
    #define     B16Gbl_avPllCtl_avPllDiv2_A2                   0x0084
    #define   LSb32Gbl_avPllCtl_avPllDiv2_A2                      0
    #define   LSb16Gbl_avPllCtl_avPllDiv2_A2                      0
    #define       bGbl_avPllCtl_avPllDiv2_A2                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_A2                      0x0000000F

    #define     BA_Gbl_avPllCtl_avPllDiv2_A3                   0x0084
    #define     B16Gbl_avPllCtl_avPllDiv2_A3                   0x0084
    #define   LSb32Gbl_avPllCtl_avPllDiv2_A3                      4
    #define   LSb16Gbl_avPllCtl_avPllDiv2_A3                      4
    #define       bGbl_avPllCtl_avPllDiv2_A3                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_A3                      0x000000F0

    #define     BA_Gbl_avPllCtl_avPllDiv2_A4                   0x0085
    #define     B16Gbl_avPllCtl_avPllDiv2_A4                   0x0084
    #define   LSb32Gbl_avPllCtl_avPllDiv2_A4                      8
    #define   LSb16Gbl_avPllCtl_avPllDiv2_A4                      8
    #define       bGbl_avPllCtl_avPllDiv2_A4                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_A4                      0x00000F00

    #define     BA_Gbl_avPllCtl_avPllDiv2_A5                   0x0085
    #define     B16Gbl_avPllCtl_avPllDiv2_A5                   0x0084
    #define   LSb32Gbl_avPllCtl_avPllDiv2_A5                      12
    #define   LSb16Gbl_avPllCtl_avPllDiv2_A5                      12
    #define       bGbl_avPllCtl_avPllDiv2_A5                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_A5                      0x0000F000

    #define     BA_Gbl_avPllCtl_avPllDiv2_A6                   0x0086
    #define     B16Gbl_avPllCtl_avPllDiv2_A6                   0x0086
    #define   LSb32Gbl_avPllCtl_avPllDiv2_A6                      16
    #define   LSb16Gbl_avPllCtl_avPllDiv2_A6                      0
    #define       bGbl_avPllCtl_avPllDiv2_A6                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_A6                      0x000F0000

    #define     BA_Gbl_avPllCtl_avPllDiv2_A7                   0x0086
    #define     B16Gbl_avPllCtl_avPllDiv2_A7                   0x0086
    #define   LSb32Gbl_avPllCtl_avPllDiv2_A7                      20
    #define   LSb16Gbl_avPllCtl_avPllDiv2_A7                      4
    #define       bGbl_avPllCtl_avPllDiv2_A7                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_A7                      0x00F00000

    #define     BA_Gbl_avPllCtl_avPllDiv2_B1                   0x0087
    #define     B16Gbl_avPllCtl_avPllDiv2_B1                   0x0086
    #define   LSb32Gbl_avPllCtl_avPllDiv2_B1                      24
    #define   LSb16Gbl_avPllCtl_avPllDiv2_B1                      8
    #define       bGbl_avPllCtl_avPllDiv2_B1                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_B1                      0x0F000000

    #define     BA_Gbl_avPllCtl_avPllDiv2_B2                   0x0087
    #define     B16Gbl_avPllCtl_avPllDiv2_B2                   0x0086
    #define   LSb32Gbl_avPllCtl_avPllDiv2_B2                      28
    #define   LSb16Gbl_avPllCtl_avPllDiv2_B2                      12
    #define       bGbl_avPllCtl_avPllDiv2_B2                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_B2                      0xF0000000

    #define     RA_Gbl_avPllCtl18                              0x0088

    #define     BA_Gbl_avPllCtl_avPllDiv2_B3                   0x0088
    #define     B16Gbl_avPllCtl_avPllDiv2_B3                   0x0088
    #define   LSb32Gbl_avPllCtl_avPllDiv2_B3                      0
    #define   LSb16Gbl_avPllCtl_avPllDiv2_B3                      0
    #define       bGbl_avPllCtl_avPllDiv2_B3                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_B3                      0x0000000F

    #define     BA_Gbl_avPllCtl_avPllDiv2_B4                   0x0088
    #define     B16Gbl_avPllCtl_avPllDiv2_B4                   0x0088
    #define   LSb32Gbl_avPllCtl_avPllDiv2_B4                      4
    #define   LSb16Gbl_avPllCtl_avPllDiv2_B4                      4
    #define       bGbl_avPllCtl_avPllDiv2_B4                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_B4                      0x000000F0

    #define     BA_Gbl_avPllCtl_avPllDiv2_B5                   0x0089
    #define     B16Gbl_avPllCtl_avPllDiv2_B5                   0x0088
    #define   LSb32Gbl_avPllCtl_avPllDiv2_B5                      8
    #define   LSb16Gbl_avPllCtl_avPllDiv2_B5                      8
    #define       bGbl_avPllCtl_avPllDiv2_B5                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_B5                      0x00000F00

    #define     BA_Gbl_avPllCtl_avPllDiv2_B6                   0x0089
    #define     B16Gbl_avPllCtl_avPllDiv2_B6                   0x0088
    #define   LSb32Gbl_avPllCtl_avPllDiv2_B6                      12
    #define   LSb16Gbl_avPllCtl_avPllDiv2_B6                      12
    #define       bGbl_avPllCtl_avPllDiv2_B6                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_B6                      0x0000F000

    #define     BA_Gbl_avPllCtl_avPllDiv2_B7                   0x008A
    #define     B16Gbl_avPllCtl_avPllDiv2_B7                   0x008A
    #define   LSb32Gbl_avPllCtl_avPllDiv2_B7                      16
    #define   LSb16Gbl_avPllCtl_avPllDiv2_B7                      0
    #define       bGbl_avPllCtl_avPllDiv2_B7                   4
    #define   MSK32Gbl_avPllCtl_avPllDiv2_B7                      0x000F0000

    #define     BA_Gbl_avPllCtl_avPllTestAna                   0x008A
    #define     B16Gbl_avPllCtl_avPllTestAna                   0x008A
    #define   LSb32Gbl_avPllCtl_avPllTestAna                      20
    #define   LSb16Gbl_avPllCtl_avPllTestAna                      4
    #define       bGbl_avPllCtl_avPllTestAna                   6
    #define   MSK32Gbl_avPllCtl_avPllTestAna                      0x03F00000
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_pllStatus                               0x008C

    #define     BA_Gbl_pllStatus_sysPllLock                    0x008C
    #define     B16Gbl_pllStatus_sysPllLock                    0x008C
    #define   LSb32Gbl_pllStatus_sysPllLock                       0
    #define   LSb16Gbl_pllStatus_sysPllLock                       0
    #define       bGbl_pllStatus_sysPllLock                    1
    #define   MSK32Gbl_pllStatus_sysPllLock                       0x00000001

    #define     BA_Gbl_pllStatus_memPllLock                    0x008C
    #define     B16Gbl_pllStatus_memPllLock                    0x008C
    #define   LSb32Gbl_pllStatus_memPllLock                       1
    #define   LSb16Gbl_pllStatus_memPllLock                       1
    #define       bGbl_pllStatus_memPllLock                    1
    #define   MSK32Gbl_pllStatus_memPllLock                       0x00000002

    #define     BA_Gbl_pllStatus_cpu1PllLock                   0x008C
    #define     B16Gbl_pllStatus_cpu1PllLock                   0x008C
    #define   LSb32Gbl_pllStatus_cpu1PllLock                      2
    #define   LSb16Gbl_pllStatus_cpu1PllLock                      2
    #define       bGbl_pllStatus_cpu1PllLock                   1
    #define   MSK32Gbl_pllStatus_cpu1PllLock                      0x00000004

    #define     BA_Gbl_pllStatus_avPllALock                    0x008C
    #define     B16Gbl_pllStatus_avPllALock                    0x008C
    #define   LSb32Gbl_pllStatus_avPllALock                       3
    #define   LSb16Gbl_pllStatus_avPllALock                       3
    #define       bGbl_pllStatus_avPllALock                    1
    #define   MSK32Gbl_pllStatus_avPllALock                       0x00000008

    #define     BA_Gbl_pllStatus_avPllBLock                    0x008C
    #define     B16Gbl_pllStatus_avPllBLock                    0x008C
    #define   LSb32Gbl_pllStatus_avPllBLock                       4
    #define   LSb16Gbl_pllStatus_avPllBLock                       4
    #define       bGbl_pllStatus_avPllBLock                    1
    #define   MSK32Gbl_pllStatus_avPllBLock                       0x00000010

    #define     BA_Gbl_pllStatus_avPllKVCOAInt                 0x008C
    #define     B16Gbl_pllStatus_avPllKVCOAInt                 0x008C
    #define   LSb32Gbl_pllStatus_avPllKVCOAInt                    5
    #define   LSb16Gbl_pllStatus_avPllKVCOAInt                    5
    #define       bGbl_pllStatus_avPllKVCOAInt                 3
    #define   MSK32Gbl_pllStatus_avPllKVCOAInt                    0x000000E0

    #define     BA_Gbl_pllStatus_avPllCaliDone                 0x008D
    #define     B16Gbl_pllStatus_avPllCaliDone                 0x008C
    #define   LSb32Gbl_pllStatus_avPllCaliDone                    8
    #define   LSb16Gbl_pllStatus_avPllCaliDone                    8
    #define       bGbl_pllStatus_avPllCaliDone                 1
    #define   MSK32Gbl_pllStatus_avPllCaliDone                    0x00000100
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_clkEnable                               0x0090

    #define     BA_Gbl_clkEnable_sysClkEn                      0x0090
    #define     B16Gbl_clkEnable_sysClkEn                      0x0090
    #define   LSb32Gbl_clkEnable_sysClkEn                         0
    #define   LSb16Gbl_clkEnable_sysClkEn                         0
    #define       bGbl_clkEnable_sysClkEn                      1
    #define   MSK32Gbl_clkEnable_sysClkEn                         0x00000001
    #define        Gbl_clkEnable_sysClkEn_enable                            0x1
    #define        Gbl_clkEnable_sysClkEn_disable                           0x0

    #define     BA_Gbl_clkEnable_cfgClkEn                      0x0090
    #define     B16Gbl_clkEnable_cfgClkEn                      0x0090
    #define   LSb32Gbl_clkEnable_cfgClkEn                         1
    #define   LSb16Gbl_clkEnable_cfgClkEn                         1
    #define       bGbl_clkEnable_cfgClkEn                      1
    #define   MSK32Gbl_clkEnable_cfgClkEn                         0x00000002
    #define        Gbl_clkEnable_cfgClkEn_enable                            0x1
    #define        Gbl_clkEnable_cfgClkEn_disable                           0x0

    #define     BA_Gbl_clkEnable_pCubeClkEn                    0x0090
    #define     B16Gbl_clkEnable_pCubeClkEn                    0x0090
    #define   LSb32Gbl_clkEnable_pCubeClkEn                       2
    #define   LSb16Gbl_clkEnable_pCubeClkEn                       2
    #define       bGbl_clkEnable_pCubeClkEn                    1
    #define   MSK32Gbl_clkEnable_pCubeClkEn                       0x00000004
    #define        Gbl_clkEnable_pCubeClkEn_enable                          0x1
    #define        Gbl_clkEnable_pCubeClkEn_disable                         0x0

    #define     BA_Gbl_clkEnable_vScopeClkEn                   0x0090
    #define     B16Gbl_clkEnable_vScopeClkEn                   0x0090
    #define   LSb32Gbl_clkEnable_vScopeClkEn                      3
    #define   LSb16Gbl_clkEnable_vScopeClkEn                      3
    #define       bGbl_clkEnable_vScopeClkEn                   1
    #define   MSK32Gbl_clkEnable_vScopeClkEn                      0x00000008
    #define        Gbl_clkEnable_vScopeClkEn_enable                         0x1
    #define        Gbl_clkEnable_vScopeClkEn_disable                        0x0

    #define     BA_Gbl_clkEnable_gfxClkEn                      0x0090
    #define     B16Gbl_clkEnable_gfxClkEn                      0x0090
    #define   LSb32Gbl_clkEnable_gfxClkEn                         4
    #define   LSb16Gbl_clkEnable_gfxClkEn                         4
    #define       bGbl_clkEnable_gfxClkEn                      1
    #define   MSK32Gbl_clkEnable_gfxClkEn                         0x00000010
    #define        Gbl_clkEnable_gfxClkEn_enable                            0x1
    #define        Gbl_clkEnable_gfxClkEn_disable                           0x0

    #define     BA_Gbl_clkEnable_zspClkEn                      0x0090
    #define     B16Gbl_clkEnable_zspClkEn                      0x0090
    #define   LSb32Gbl_clkEnable_zspClkEn                         5
    #define   LSb16Gbl_clkEnable_zspClkEn                         5
    #define       bGbl_clkEnable_zspClkEn                      1
    #define   MSK32Gbl_clkEnable_zspClkEn                         0x00000020
    #define        Gbl_clkEnable_zspClkEn_enable                            0x1
    #define        Gbl_clkEnable_zspClkEn_disable                           0x0

    #define     BA_Gbl_clkEnable_perifClkEn                    0x0090
    #define     B16Gbl_clkEnable_perifClkEn                    0x0090
    #define   LSb32Gbl_clkEnable_perifClkEn                       6
    #define   LSb16Gbl_clkEnable_perifClkEn                       6
    #define       bGbl_clkEnable_perifClkEn                    1
    #define   MSK32Gbl_clkEnable_perifClkEn                       0x00000040
    #define        Gbl_clkEnable_perifClkEn_enable                          0x1
    #define        Gbl_clkEnable_perifClkEn_disable                         0x0

    #define     BA_Gbl_clkEnable_gethCoreClkEn                 0x0090
    #define     B16Gbl_clkEnable_gethCoreClkEn                 0x0090
    #define   LSb32Gbl_clkEnable_gethCoreClkEn                    7
    #define   LSb16Gbl_clkEnable_gethCoreClkEn                    7
    #define       bGbl_clkEnable_gethCoreClkEn                 1
    #define   MSK32Gbl_clkEnable_gethCoreClkEn                    0x00000080
    #define        Gbl_clkEnable_gethCoreClkEn_enable                       0x1
    #define        Gbl_clkEnable_gethCoreClkEn_disable                      0x0

    #define     BA_Gbl_clkEnable_pexCoreClkEn                  0x0091
    #define     B16Gbl_clkEnable_pexCoreClkEn                  0x0090
    #define   LSb32Gbl_clkEnable_pexCoreClkEn                     8
    #define   LSb16Gbl_clkEnable_pexCoreClkEn                     8
    #define       bGbl_clkEnable_pexCoreClkEn                  1
    #define   MSK32Gbl_clkEnable_pexCoreClkEn                     0x00000100
    #define        Gbl_clkEnable_pexCoreClkEn_enable                        0x1
    #define        Gbl_clkEnable_pexCoreClkEn_disable                       0x0

    #define     BA_Gbl_clkEnable_sataCoreClkEn                 0x0091
    #define     B16Gbl_clkEnable_sataCoreClkEn                 0x0090
    #define   LSb32Gbl_clkEnable_sataCoreClkEn                    9
    #define   LSb16Gbl_clkEnable_sataCoreClkEn                    9
    #define       bGbl_clkEnable_sataCoreClkEn                 1
    #define   MSK32Gbl_clkEnable_sataCoreClkEn                    0x00000200
    #define        Gbl_clkEnable_sataCoreClkEn_enable                       0x1
    #define        Gbl_clkEnable_sataCoreClkEn_disable                      0x0

    #define     BA_Gbl_clkEnable_ahbApbCoreClkEn               0x0091
    #define     B16Gbl_clkEnable_ahbApbCoreClkEn               0x0090
    #define   LSb32Gbl_clkEnable_ahbApbCoreClkEn                  10
    #define   LSb16Gbl_clkEnable_ahbApbCoreClkEn                  10
    #define       bGbl_clkEnable_ahbApbCoreClkEn               1
    #define   MSK32Gbl_clkEnable_ahbApbCoreClkEn                  0x00000400
    #define        Gbl_clkEnable_ahbApbCoreClkEn_enable                     0x1
    #define        Gbl_clkEnable_ahbApbCoreClkEn_disable                    0x0

    #define     BA_Gbl_clkEnable_usb0CoreClkEn                 0x0091
    #define     B16Gbl_clkEnable_usb0CoreClkEn                 0x0090
    #define   LSb32Gbl_clkEnable_usb0CoreClkEn                    11
    #define   LSb16Gbl_clkEnable_usb0CoreClkEn                    11
    #define       bGbl_clkEnable_usb0CoreClkEn                 1
    #define   MSK32Gbl_clkEnable_usb0CoreClkEn                    0x00000800
    #define        Gbl_clkEnable_usb0CoreClkEn_enable                       0x1
    #define        Gbl_clkEnable_usb0CoreClkEn_disable                      0x0

    #define     BA_Gbl_clkEnable_usb1CoreClkEn                 0x0091
    #define     B16Gbl_clkEnable_usb1CoreClkEn                 0x0090
    #define   LSb32Gbl_clkEnable_usb1CoreClkEn                    12
    #define   LSb16Gbl_clkEnable_usb1CoreClkEn                    12
    #define       bGbl_clkEnable_usb1CoreClkEn                 1
    #define   MSK32Gbl_clkEnable_usb1CoreClkEn                    0x00001000
    #define        Gbl_clkEnable_usb1CoreClkEn_enable                       0x1
    #define        Gbl_clkEnable_usb1CoreClkEn_disable                      0x0

    #define     BA_Gbl_clkEnable_pBridgeCoreClkEn              0x0091
    #define     B16Gbl_clkEnable_pBridgeCoreClkEn              0x0090
    #define   LSb32Gbl_clkEnable_pBridgeCoreClkEn                 13
    #define   LSb16Gbl_clkEnable_pBridgeCoreClkEn                 13
    #define       bGbl_clkEnable_pBridgeCoreClkEn              1
    #define   MSK32Gbl_clkEnable_pBridgeCoreClkEn                 0x00002000
    #define        Gbl_clkEnable_pBridgeCoreClkEn_enable                    0x1
    #define        Gbl_clkEnable_pBridgeCoreClkEn_disable                   0x0

    #define     BA_Gbl_clkEnable_sdioCoreClkEn                 0x0091
    #define     B16Gbl_clkEnable_sdioCoreClkEn                 0x0090
    #define   LSb32Gbl_clkEnable_sdioCoreClkEn                    14
    #define   LSb16Gbl_clkEnable_sdioCoreClkEn                    14
    #define       bGbl_clkEnable_sdioCoreClkEn                 1
    #define   MSK32Gbl_clkEnable_sdioCoreClkEn                    0x00004000
    #define        Gbl_clkEnable_sdioCoreClkEn_enable                       0x1
    #define        Gbl_clkEnable_sdioCoreClkEn_disable                      0x0

    #define     BA_Gbl_clkEnable_drmFigoClkEn                  0x0091
    #define     B16Gbl_clkEnable_drmFigoClkEn                  0x0090
    #define   LSb32Gbl_clkEnable_drmFigoClkEn                     15
    #define   LSb16Gbl_clkEnable_drmFigoClkEn                     15
    #define       bGbl_clkEnable_drmFigoClkEn                  1
    #define   MSK32Gbl_clkEnable_drmFigoClkEn                     0x00008000
    #define        Gbl_clkEnable_drmFigoClkEn_enable                        0x1
    #define        Gbl_clkEnable_drmFigoClkEn_disable                       0x0

    #define     BA_Gbl_clkEnable_nfcCoreClkEn                  0x0092
    #define     B16Gbl_clkEnable_nfcCoreClkEn                  0x0092
    #define   LSb32Gbl_clkEnable_nfcCoreClkEn                     16
    #define   LSb16Gbl_clkEnable_nfcCoreClkEn                     0
    #define       bGbl_clkEnable_nfcCoreClkEn                  1
    #define   MSK32Gbl_clkEnable_nfcCoreClkEn                     0x00010000
    #define        Gbl_clkEnable_nfcCoreClkEn_enable                        0x1
    #define        Gbl_clkEnable_nfcCoreClkEn_disable                       0x0

    #define     BA_Gbl_clkEnable_nfcEccClkEn                   0x0092
    #define     B16Gbl_clkEnable_nfcEccClkEn                   0x0092
    #define   LSb32Gbl_clkEnable_nfcEccClkEn                      17
    #define   LSb16Gbl_clkEnable_nfcEccClkEn                      1
    #define       bGbl_clkEnable_nfcEccClkEn                   1
    #define   MSK32Gbl_clkEnable_nfcEccClkEn                      0x00020000
    #define        Gbl_clkEnable_nfcEccClkEn_enable                         0x1
    #define        Gbl_clkEnable_nfcEccClkEn_disable                        0x0

    #define     BA_Gbl_clkEnable_smemcCoreClkEn                0x0092
    #define     B16Gbl_clkEnable_smemcCoreClkEn                0x0092
    #define   LSb32Gbl_clkEnable_smemcCoreClkEn                   18
    #define   LSb16Gbl_clkEnable_smemcCoreClkEn                   2
    #define       bGbl_clkEnable_smemcCoreClkEn                1
    #define   MSK32Gbl_clkEnable_smemcCoreClkEn                   0x00040000
    #define        Gbl_clkEnable_smemcCoreClkEn_enable                      0x1
    #define        Gbl_clkEnable_smemcCoreClkEn_disable                     0x0

    #define     BA_Gbl_clkEnable_appClkEn                      0x0092
    #define     B16Gbl_clkEnable_appClkEn                      0x0092
    #define   LSb32Gbl_clkEnable_appClkEn                         19
    #define   LSb16Gbl_clkEnable_appClkEn                         3
    #define       bGbl_clkEnable_appClkEn                      1
    #define   MSK32Gbl_clkEnable_appClkEn                         0x00080000
    #define        Gbl_clkEnable_appClkEn_enable                            0x1
    #define        Gbl_clkEnable_appClkEn_disable                           0x0

    #define     BA_Gbl_clkEnable_vppSysClkEn                   0x0092
    #define     B16Gbl_clkEnable_vppSysClkEn                   0x0092
    #define   LSb32Gbl_clkEnable_vppSysClkEn                      20
    #define   LSb16Gbl_clkEnable_vppSysClkEn                      4
    #define       bGbl_clkEnable_vppSysClkEn                   1
    #define   MSK32Gbl_clkEnable_vppSysClkEn                      0x00100000
    #define        Gbl_clkEnable_vppSysClkEn_enable                         0x1
    #define        Gbl_clkEnable_vppSysClkEn_disable                        0x0

    #define     BA_Gbl_clkEnable_audio0ClkEn                   0x0092
    #define     B16Gbl_clkEnable_audio0ClkEn                   0x0092
    #define   LSb32Gbl_clkEnable_audio0ClkEn                      21
    #define   LSb16Gbl_clkEnable_audio0ClkEn                      5
    #define       bGbl_clkEnable_audio0ClkEn                   1
    #define   MSK32Gbl_clkEnable_audio0ClkEn                      0x00200000
    #define        Gbl_clkEnable_audio0ClkEn_enable                         0x1
    #define        Gbl_clkEnable_audio0ClkEn_disable                        0x0

    #define     BA_Gbl_clkEnable_audio1ClkEn                   0x0092
    #define     B16Gbl_clkEnable_audio1ClkEn                   0x0092
    #define   LSb32Gbl_clkEnable_audio1ClkEn                      22
    #define   LSb16Gbl_clkEnable_audio1ClkEn                      6
    #define       bGbl_clkEnable_audio1ClkEn                   1
    #define   MSK32Gbl_clkEnable_audio1ClkEn                      0x00400000
    #define        Gbl_clkEnable_audio1ClkEn_enable                         0x1
    #define        Gbl_clkEnable_audio1ClkEn_disable                        0x0

    #define     BA_Gbl_clkEnable_audio2ClkEn                   0x0092
    #define     B16Gbl_clkEnable_audio2ClkEn                   0x0092
    #define   LSb32Gbl_clkEnable_audio2ClkEn                      23
    #define   LSb16Gbl_clkEnable_audio2ClkEn                      7
    #define       bGbl_clkEnable_audio2ClkEn                   1
    #define   MSK32Gbl_clkEnable_audio2ClkEn                      0x00800000
    #define        Gbl_clkEnable_audio2ClkEn_enable                         0x1
    #define        Gbl_clkEnable_audio2ClkEn_disable                        0x0

    #define     BA_Gbl_clkEnable_audio3ClkEn                   0x0093
    #define     B16Gbl_clkEnable_audio3ClkEn                   0x0092
    #define   LSb32Gbl_clkEnable_audio3ClkEn                      24
    #define   LSb16Gbl_clkEnable_audio3ClkEn                      8
    #define       bGbl_clkEnable_audio3ClkEn                   1
    #define   MSK32Gbl_clkEnable_audio3ClkEn                      0x01000000
    #define        Gbl_clkEnable_audio3ClkEn_enable                         0x1
    #define        Gbl_clkEnable_audio3ClkEn_disable                        0x0

    #define     BA_Gbl_clkEnable_audioHdClkEn                  0x0093
    #define     B16Gbl_clkEnable_audioHdClkEn                  0x0092
    #define   LSb32Gbl_clkEnable_audioHdClkEn                     25
    #define   LSb16Gbl_clkEnable_audioHdClkEn                     9
    #define       bGbl_clkEnable_audioHdClkEn                  1
    #define   MSK32Gbl_clkEnable_audioHdClkEn                     0x02000000
    #define        Gbl_clkEnable_audioHdClkEn_enable                        0x1
    #define        Gbl_clkEnable_audioHdClkEn_disable                       0x0

    #define     BA_Gbl_clkEnable_video0ClkEn                   0x0093
    #define     B16Gbl_clkEnable_video0ClkEn                   0x0092
    #define   LSb32Gbl_clkEnable_video0ClkEn                      26
    #define   LSb16Gbl_clkEnable_video0ClkEn                      10
    #define       bGbl_clkEnable_video0ClkEn                   1
    #define   MSK32Gbl_clkEnable_video0ClkEn                      0x04000000
    #define        Gbl_clkEnable_video0ClkEn_enable                         0x1
    #define        Gbl_clkEnable_video0ClkEn_disable                        0x0

    #define     BA_Gbl_clkEnable_video1ClkEn                   0x0093
    #define     B16Gbl_clkEnable_video1ClkEn                   0x0092
    #define   LSb32Gbl_clkEnable_video1ClkEn                      27
    #define   LSb16Gbl_clkEnable_video1ClkEn                      11
    #define       bGbl_clkEnable_video1ClkEn                   1
    #define   MSK32Gbl_clkEnable_video1ClkEn                      0x08000000
    #define        Gbl_clkEnable_video1ClkEn_enable                         0x1
    #define        Gbl_clkEnable_video1ClkEn_disable                        0x0

    #define     BA_Gbl_clkEnable_video2ClkEn                   0x0093
    #define     B16Gbl_clkEnable_video2ClkEn                   0x0092
    #define   LSb32Gbl_clkEnable_video2ClkEn                      28
    #define   LSb16Gbl_clkEnable_video2ClkEn                      12
    #define       bGbl_clkEnable_video2ClkEn                   1
    #define   MSK32Gbl_clkEnable_video2ClkEn                      0x10000000
    #define        Gbl_clkEnable_video2ClkEn_enable                         0x1
    #define        Gbl_clkEnable_video2ClkEn_disable                        0x0

    #define     BA_Gbl_clkEnable_PEX_mx_refclk_off             0x0093
    #define     B16Gbl_clkEnable_PEX_mx_refclk_off             0x0092
    #define   LSb32Gbl_clkEnable_PEX_mx_refclk_off                29
    #define   LSb16Gbl_clkEnable_PEX_mx_refclk_off                13
    #define       bGbl_clkEnable_PEX_mx_refclk_off             1
    #define   MSK32Gbl_clkEnable_PEX_mx_refclk_off                0x20000000
    #define        Gbl_clkEnable_PEX_mx_refclk_off_ON                       0x0
    #define        Gbl_clkEnable_PEX_mx_refclk_off_OFF                      0x1
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_clkSelect                               0x0094

    #define     BA_Gbl_clkSelect_sysClkPllSel                  0x0094
    #define     B16Gbl_clkSelect_sysClkPllSel                  0x0094
    #define   LSb32Gbl_clkSelect_sysClkPllSel                     0
    #define   LSb16Gbl_clkSelect_sysClkPllSel                     0
    #define       bGbl_clkSelect_sysClkPllSel                  3
    #define   MSK32Gbl_clkSelect_sysClkPllSel                     0x00000007
    #define        Gbl_clkSelect_sysClkPllSel_AVPllA5                       0x0
    #define        Gbl_clkSelect_sysClkPllSel_AVPllA6                       0x1
    #define        Gbl_clkSelect_sysClkPllSel_AVPllB6                       0x2
    #define        Gbl_clkSelect_sysClkPllSel_AVPllB7                       0x3
    #define        Gbl_clkSelect_sysClkPllSel_CPU1Pll                       0x4

    #define     BA_Gbl_clkSelect_sysClkSel                     0x0094
    #define     B16Gbl_clkSelect_sysClkSel                     0x0094
    #define   LSb32Gbl_clkSelect_sysClkSel                        3
    #define   LSb16Gbl_clkSelect_sysClkSel                        3
    #define       bGbl_clkSelect_sysClkSel                     3
    #define   MSK32Gbl_clkSelect_sysClkSel                        0x00000038
    #define        Gbl_clkSelect_sysClkSel_d2                               0x1
    #define        Gbl_clkSelect_sysClkSel_d4                               0x2
    #define        Gbl_clkSelect_sysClkSel_d6                               0x3
    #define        Gbl_clkSelect_sysClkSel_d8                               0x4
    #define        Gbl_clkSelect_sysClkSel_d12                              0x5

    #define     BA_Gbl_clkSelect_cpu0ClkPllSel                 0x0094
    #define     B16Gbl_clkSelect_cpu0ClkPllSel                 0x0094
    #define   LSb32Gbl_clkSelect_cpu0ClkPllSel                    6
    #define   LSb16Gbl_clkSelect_cpu0ClkPllSel                    6
    #define       bGbl_clkSelect_cpu0ClkPllSel                 3
    #define   MSK32Gbl_clkSelect_cpu0ClkPllSel                    0x000001C0
    #define        Gbl_clkSelect_cpu0ClkPllSel_AVPllA5                      0x0
    #define        Gbl_clkSelect_cpu0ClkPllSel_AVPllA6                      0x1
    #define        Gbl_clkSelect_cpu0ClkPllSel_AVPllB6                      0x2
    #define        Gbl_clkSelect_cpu0ClkPllSel_AVPllB7                      0x3
    #define        Gbl_clkSelect_cpu0ClkPllSel_CPU1Pll                      0x4

    #define     BA_Gbl_clkSelect_cpu0ClkSel                    0x0095
    #define     B16Gbl_clkSelect_cpu0ClkSel                    0x0094
    #define   LSb32Gbl_clkSelect_cpu0ClkSel                       9
    #define   LSb16Gbl_clkSelect_cpu0ClkSel                       9
    #define       bGbl_clkSelect_cpu0ClkSel                    3
    #define   MSK32Gbl_clkSelect_cpu0ClkSel                       0x00000E00
    #define        Gbl_clkSelect_cpu0ClkSel_d2                              0x1
    #define        Gbl_clkSelect_cpu0ClkSel_d4                              0x2
    #define        Gbl_clkSelect_cpu0ClkSel_d6                              0x3
    #define        Gbl_clkSelect_cpu0ClkSel_d8                              0x4
    #define        Gbl_clkSelect_cpu0ClkSel_d12                             0x5

    #define     BA_Gbl_clkSelect_cpu1ClkPllSel                 0x0095
    #define     B16Gbl_clkSelect_cpu1ClkPllSel                 0x0094
    #define   LSb32Gbl_clkSelect_cpu1ClkPllSel                    12
    #define   LSb16Gbl_clkSelect_cpu1ClkPllSel                    12
    #define       bGbl_clkSelect_cpu1ClkPllSel                 2
    #define   MSK32Gbl_clkSelect_cpu1ClkPllSel                    0x00003000
    #define        Gbl_clkSelect_cpu1ClkPllSel_AVPllA5                      0x0
    #define        Gbl_clkSelect_cpu1ClkPllSel_AVPllA6                      0x1
    #define        Gbl_clkSelect_cpu1ClkPllSel_AVPllB6                      0x2
    #define        Gbl_clkSelect_cpu1ClkPllSel_AVPllB7                      0x3

    #define     BA_Gbl_clkSelect_cpu1ClkSel                    0x0095
    #define     B16Gbl_clkSelect_cpu1ClkSel                    0x0094
    #define   LSb32Gbl_clkSelect_cpu1ClkSel                       14
    #define   LSb16Gbl_clkSelect_cpu1ClkSel                       14
    #define       bGbl_clkSelect_cpu1ClkSel                    3
    #define   MSK32Gbl_clkSelect_cpu1ClkSel                       0x0001C000
    #define        Gbl_clkSelect_cpu1ClkSel_d2                              0x1
    #define        Gbl_clkSelect_cpu1ClkSel_d4                              0x2
    #define        Gbl_clkSelect_cpu1ClkSel_d6                              0x3
    #define        Gbl_clkSelect_cpu1ClkSel_d8                              0x4
    #define        Gbl_clkSelect_cpu1ClkSel_d12                             0x5

    #define     BA_Gbl_clkSelect_drmFigoClkPllSel              0x0096
    #define     B16Gbl_clkSelect_drmFigoClkPllSel              0x0096
    #define   LSb32Gbl_clkSelect_drmFigoClkPllSel                 17
    #define   LSb16Gbl_clkSelect_drmFigoClkPllSel                 1
    #define       bGbl_clkSelect_drmFigoClkPllSel              3
    #define   MSK32Gbl_clkSelect_drmFigoClkPllSel                 0x000E0000
    #define        Gbl_clkSelect_drmFigoClkPllSel_AVPllA5                   0x0
    #define        Gbl_clkSelect_drmFigoClkPllSel_AVPllA6                   0x1
    #define        Gbl_clkSelect_drmFigoClkPllSel_AVPllB6                   0x2
    #define        Gbl_clkSelect_drmFigoClkPllSel_AVPllB7                   0x3
    #define        Gbl_clkSelect_drmFigoClkPllSel_CPU1Pll                   0x4

    #define     BA_Gbl_clkSelect_drmFigoClkSel                 0x0096
    #define     B16Gbl_clkSelect_drmFigoClkSel                 0x0096
    #define   LSb32Gbl_clkSelect_drmFigoClkSel                    20
    #define   LSb16Gbl_clkSelect_drmFigoClkSel                    4
    #define       bGbl_clkSelect_drmFigoClkSel                 3
    #define   MSK32Gbl_clkSelect_drmFigoClkSel                    0x00700000
    #define        Gbl_clkSelect_drmFigoClkSel_d2                           0x1
    #define        Gbl_clkSelect_drmFigoClkSel_d4                           0x2
    #define        Gbl_clkSelect_drmFigoClkSel_d6                           0x3
    #define        Gbl_clkSelect_drmFigoClkSel_d8                           0x4
    #define        Gbl_clkSelect_drmFigoClkSel_d12                          0x5

    #define     BA_Gbl_clkSelect_cfgClkPllSel                  0x0096
    #define     B16Gbl_clkSelect_cfgClkPllSel                  0x0096
    #define   LSb32Gbl_clkSelect_cfgClkPllSel                     23
    #define   LSb16Gbl_clkSelect_cfgClkPllSel                     7
    #define       bGbl_clkSelect_cfgClkPllSel                  3
    #define   MSK32Gbl_clkSelect_cfgClkPllSel                     0x03800000
    #define        Gbl_clkSelect_cfgClkPllSel_AVPllA5                       0x0
    #define        Gbl_clkSelect_cfgClkPllSel_AVPllA6                       0x1
    #define        Gbl_clkSelect_cfgClkPllSel_AVPllB6                       0x2
    #define        Gbl_clkSelect_cfgClkPllSel_AVPllB7                       0x3
    #define        Gbl_clkSelect_cfgClkPllSel_CPU1Pll                       0x4

    #define     BA_Gbl_clkSelect_cfgClkSel                     0x0097
    #define     B16Gbl_clkSelect_cfgClkSel                     0x0096
    #define   LSb32Gbl_clkSelect_cfgClkSel                        26
    #define   LSb16Gbl_clkSelect_cfgClkSel                        10
    #define       bGbl_clkSelect_cfgClkSel                     3
    #define   MSK32Gbl_clkSelect_cfgClkSel                        0x1C000000
    #define        Gbl_clkSelect_cfgClkSel_d2                               0x1
    #define        Gbl_clkSelect_cfgClkSel_d4                               0x2
    #define        Gbl_clkSelect_cfgClkSel_d6                               0x3
    #define        Gbl_clkSelect_cfgClkSel_d8                               0x4
    #define        Gbl_clkSelect_cfgClkSel_d12                              0x5

    #define     BA_Gbl_clkSelect_gfxClkPllSel                  0x0097
    #define     B16Gbl_clkSelect_gfxClkPllSel                  0x0096
    #define   LSb32Gbl_clkSelect_gfxClkPllSel                     29
    #define   LSb16Gbl_clkSelect_gfxClkPllSel                     13
    #define       bGbl_clkSelect_gfxClkPllSel                  3
    #define   MSK32Gbl_clkSelect_gfxClkPllSel                     0xE0000000
    #define        Gbl_clkSelect_gfxClkPllSel_AVPllA5                       0x0
    #define        Gbl_clkSelect_gfxClkPllSel_AVPllA6                       0x1
    #define        Gbl_clkSelect_gfxClkPllSel_AVPllB6                       0x2
    #define        Gbl_clkSelect_gfxClkPllSel_AVPllB7                       0x3
    #define        Gbl_clkSelect_gfxClkPllSel_CPU1Pll                       0x4

    #define     RA_Gbl_clkSelect1                              0x0098

    #define     BA_Gbl_clkSelect_gfxClkSel                     0x0098
    #define     B16Gbl_clkSelect_gfxClkSel                     0x0098
    #define   LSb32Gbl_clkSelect_gfxClkSel                        0
    #define   LSb16Gbl_clkSelect_gfxClkSel                        0
    #define       bGbl_clkSelect_gfxClkSel                     3
    #define   MSK32Gbl_clkSelect_gfxClkSel                        0x00000007
    #define        Gbl_clkSelect_gfxClkSel_d2                               0x1
    #define        Gbl_clkSelect_gfxClkSel_d4                               0x2
    #define        Gbl_clkSelect_gfxClkSel_d6                               0x3
    #define        Gbl_clkSelect_gfxClkSel_d8                               0x4
    #define        Gbl_clkSelect_gfxClkSel_d12                              0x5

    #define     BA_Gbl_clkSelect_zspClkPllSel                  0x0098
    #define     B16Gbl_clkSelect_zspClkPllSel                  0x0098
    #define   LSb32Gbl_clkSelect_zspClkPllSel                     3
    #define   LSb16Gbl_clkSelect_zspClkPllSel                     3
    #define       bGbl_clkSelect_zspClkPllSel                  3
    #define   MSK32Gbl_clkSelect_zspClkPllSel                     0x00000038
    #define        Gbl_clkSelect_zspClkPllSel_AVPllA5                       0x0
    #define        Gbl_clkSelect_zspClkPllSel_AVPllA6                       0x1
    #define        Gbl_clkSelect_zspClkPllSel_AVPllB6                       0x2
    #define        Gbl_clkSelect_zspClkPllSel_AVPllB7                       0x3
    #define        Gbl_clkSelect_zspClkPllSel_CPU1Pll                       0x4

    #define     BA_Gbl_clkSelect_zspClkSel                     0x0098
    #define     B16Gbl_clkSelect_zspClkSel                     0x0098
    #define   LSb32Gbl_clkSelect_zspClkSel                        6
    #define   LSb16Gbl_clkSelect_zspClkSel                        6
    #define       bGbl_clkSelect_zspClkSel                     3
    #define   MSK32Gbl_clkSelect_zspClkSel                        0x000001C0
    #define        Gbl_clkSelect_zspClkSel_d2                               0x1
    #define        Gbl_clkSelect_zspClkSel_d4                               0x2
    #define        Gbl_clkSelect_zspClkSel_d6                               0x3
    #define        Gbl_clkSelect_zspClkSel_d8                               0x4
    #define        Gbl_clkSelect_zspClkSel_d12                              0x5

    #define     BA_Gbl_clkSelect_perifClkPllSel                0x0099
    #define     B16Gbl_clkSelect_perifClkPllSel                0x0098
    #define   LSb32Gbl_clkSelect_perifClkPllSel                   9
    #define   LSb16Gbl_clkSelect_perifClkPllSel                   9
    #define       bGbl_clkSelect_perifClkPllSel                3
    #define   MSK32Gbl_clkSelect_perifClkPllSel                   0x00000E00
    #define        Gbl_clkSelect_perifClkPllSel_AVPllA5                     0x0
    #define        Gbl_clkSelect_perifClkPllSel_AVPllA6                     0x1
    #define        Gbl_clkSelect_perifClkPllSel_AVPllB6                     0x2
    #define        Gbl_clkSelect_perifClkPllSel_AVPllB7                     0x3
    #define        Gbl_clkSelect_perifClkPllSel_CPU1Pll                     0x4

    #define     BA_Gbl_clkSelect_perifClkSel                   0x0099
    #define     B16Gbl_clkSelect_perifClkSel                   0x0098
    #define   LSb32Gbl_clkSelect_perifClkSel                      12
    #define   LSb16Gbl_clkSelect_perifClkSel                      12
    #define       bGbl_clkSelect_perifClkSel                   3
    #define   MSK32Gbl_clkSelect_perifClkSel                      0x00007000
    #define        Gbl_clkSelect_perifClkSel_d2                             0x1
    #define        Gbl_clkSelect_perifClkSel_d4                             0x2
    #define        Gbl_clkSelect_perifClkSel_d6                             0x3
    #define        Gbl_clkSelect_perifClkSel_d8                             0x4
    #define        Gbl_clkSelect_perifClkSel_d12                            0x5

    #define     BA_Gbl_clkSelect_pCubeClkPllSel                0x0099
    #define     B16Gbl_clkSelect_pCubeClkPllSel                0x0098
    #define   LSb32Gbl_clkSelect_pCubeClkPllSel                   15
    #define   LSb16Gbl_clkSelect_pCubeClkPllSel                   15
    #define       bGbl_clkSelect_pCubeClkPllSel                3
    #define   MSK32Gbl_clkSelect_pCubeClkPllSel                   0x00038000
    #define        Gbl_clkSelect_pCubeClkPllSel_AVPllA5                     0x0
    #define        Gbl_clkSelect_pCubeClkPllSel_AVPllA6                     0x1
    #define        Gbl_clkSelect_pCubeClkPllSel_AVPllB6                     0x2
    #define        Gbl_clkSelect_pCubeClkPllSel_AVPllB7                     0x3
    #define        Gbl_clkSelect_pCubeClkPllSel_CPU1Pll                     0x4

    #define     BA_Gbl_clkSelect_pCubeClkSel                   0x009A
    #define     B16Gbl_clkSelect_pCubeClkSel                   0x009A
    #define   LSb32Gbl_clkSelect_pCubeClkSel                      18
    #define   LSb16Gbl_clkSelect_pCubeClkSel                      2
    #define       bGbl_clkSelect_pCubeClkSel                   3
    #define   MSK32Gbl_clkSelect_pCubeClkSel                      0x001C0000
    #define        Gbl_clkSelect_pCubeClkSel_d2                             0x1
    #define        Gbl_clkSelect_pCubeClkSel_d4                             0x2
    #define        Gbl_clkSelect_pCubeClkSel_d6                             0x3
    #define        Gbl_clkSelect_pCubeClkSel_d8                             0x4
    #define        Gbl_clkSelect_pCubeClkSel_d12                            0x5

    #define     BA_Gbl_clkSelect_vScopeClkPllSel               0x009A
    #define     B16Gbl_clkSelect_vScopeClkPllSel               0x009A
    #define   LSb32Gbl_clkSelect_vScopeClkPllSel                  21
    #define   LSb16Gbl_clkSelect_vScopeClkPllSel                  5
    #define       bGbl_clkSelect_vScopeClkPllSel               3
    #define   MSK32Gbl_clkSelect_vScopeClkPllSel                  0x00E00000
    #define        Gbl_clkSelect_vScopeClkPllSel_AVPllA5                    0x0
    #define        Gbl_clkSelect_vScopeClkPllSel_AVPllA6                    0x1
    #define        Gbl_clkSelect_vScopeClkPllSel_AVPllB6                    0x2
    #define        Gbl_clkSelect_vScopeClkPllSel_AVPllB7                    0x3
    #define        Gbl_clkSelect_vScopeClkPllSel_CPU1Pll                    0x4

    #define     BA_Gbl_clkSelect_vScopeClkSel                  0x009B
    #define     B16Gbl_clkSelect_vScopeClkSel                  0x009A
    #define   LSb32Gbl_clkSelect_vScopeClkSel                     24
    #define   LSb16Gbl_clkSelect_vScopeClkSel                     8
    #define       bGbl_clkSelect_vScopeClkSel                  3
    #define   MSK32Gbl_clkSelect_vScopeClkSel                     0x07000000
    #define        Gbl_clkSelect_vScopeClkSel_d2                            0x1
    #define        Gbl_clkSelect_vScopeClkSel_d4                            0x2
    #define        Gbl_clkSelect_vScopeClkSel_d6                            0x3
    #define        Gbl_clkSelect_vScopeClkSel_d8                            0x4
    #define        Gbl_clkSelect_vScopeClkSel_d12                           0x5

    #define     BA_Gbl_clkSelect_nfcEccClkPllSel               0x009B
    #define     B16Gbl_clkSelect_nfcEccClkPllSel               0x009A
    #define   LSb32Gbl_clkSelect_nfcEccClkPllSel                  27
    #define   LSb16Gbl_clkSelect_nfcEccClkPllSel                  11
    #define       bGbl_clkSelect_nfcEccClkPllSel               3
    #define   MSK32Gbl_clkSelect_nfcEccClkPllSel                  0x38000000
    #define        Gbl_clkSelect_nfcEccClkPllSel_AVPllA5                    0x0
    #define        Gbl_clkSelect_nfcEccClkPllSel_AVPllA6                    0x1
    #define        Gbl_clkSelect_nfcEccClkPllSel_AVPllB6                    0x2
    #define        Gbl_clkSelect_nfcEccClkPllSel_AVPllB7                    0x3
    #define        Gbl_clkSelect_nfcEccClkPllSel_CPU1Pll                    0x4

    #define     RA_Gbl_clkSelect2                              0x009C

    #define     BA_Gbl_clkSelect_nfcEccClkSel                  0x009C
    #define     B16Gbl_clkSelect_nfcEccClkSel                  0x009C
    #define   LSb32Gbl_clkSelect_nfcEccClkSel                     0
    #define   LSb16Gbl_clkSelect_nfcEccClkSel                     0
    #define       bGbl_clkSelect_nfcEccClkSel                  3
    #define   MSK32Gbl_clkSelect_nfcEccClkSel                     0x00000007
    #define        Gbl_clkSelect_nfcEccClkSel_d2                            0x1
    #define        Gbl_clkSelect_nfcEccClkSel_d4                            0x2
    #define        Gbl_clkSelect_nfcEccClkSel_d6                            0x3
    #define        Gbl_clkSelect_nfcEccClkSel_d8                            0x4
    #define        Gbl_clkSelect_nfcEccClkSel_d12                           0x5

    #define     BA_Gbl_clkSelect_vppSysClkPllSel               0x009C
    #define     B16Gbl_clkSelect_vppSysClkPllSel               0x009C
    #define   LSb32Gbl_clkSelect_vppSysClkPllSel                  3
    #define   LSb16Gbl_clkSelect_vppSysClkPllSel                  3
    #define       bGbl_clkSelect_vppSysClkPllSel               3
    #define   MSK32Gbl_clkSelect_vppSysClkPllSel                  0x00000038
    #define        Gbl_clkSelect_vppSysClkPllSel_AVPllA5                    0x0
    #define        Gbl_clkSelect_vppSysClkPllSel_AVPllA6                    0x1
    #define        Gbl_clkSelect_vppSysClkPllSel_AVPllB6                    0x2
    #define        Gbl_clkSelect_vppSysClkPllSel_AVPllB7                    0x3
    #define        Gbl_clkSelect_vppSysClkPllSel_CPU1Pll                    0x4

    #define     BA_Gbl_clkSelect_vppSysClkSel                  0x009C
    #define     B16Gbl_clkSelect_vppSysClkSel                  0x009C
    #define   LSb32Gbl_clkSelect_vppSysClkSel                     6
    #define   LSb16Gbl_clkSelect_vppSysClkSel                     6
    #define       bGbl_clkSelect_vppSysClkSel                  3
    #define   MSK32Gbl_clkSelect_vppSysClkSel                     0x000001C0
    #define        Gbl_clkSelect_vppSysClkSel_d2                            0x1
    #define        Gbl_clkSelect_vppSysClkSel_d4                            0x2
    #define        Gbl_clkSelect_vppSysClkSel_d6                            0x3
    #define        Gbl_clkSelect_vppSysClkSel_d8                            0x4
    #define        Gbl_clkSelect_vppSysClkSel_d12                           0x5

    #define     BA_Gbl_clkSelect_appClkPllSel                  0x009D
    #define     B16Gbl_clkSelect_appClkPllSel                  0x009C
    #define   LSb32Gbl_clkSelect_appClkPllSel                     9
    #define   LSb16Gbl_clkSelect_appClkPllSel                     9
    #define       bGbl_clkSelect_appClkPllSel                  3
    #define   MSK32Gbl_clkSelect_appClkPllSel                     0x00000E00
    #define        Gbl_clkSelect_appClkPllSel_AVPllA5                       0x0
    #define        Gbl_clkSelect_appClkPllSel_AVPllA6                       0x1
    #define        Gbl_clkSelect_appClkPllSel_AVPllB6                       0x2
    #define        Gbl_clkSelect_appClkPllSel_AVPllB7                       0x3
    #define        Gbl_clkSelect_appClkPllSel_CPU1Pll                       0x4

    #define     BA_Gbl_clkSelect_appClkSel                     0x009D
    #define     B16Gbl_clkSelect_appClkSel                     0x009C
    #define   LSb32Gbl_clkSelect_appClkSel                        12
    #define   LSb16Gbl_clkSelect_appClkSel                        12
    #define       bGbl_clkSelect_appClkSel                     3
    #define   MSK32Gbl_clkSelect_appClkSel                        0x00007000
    #define        Gbl_clkSelect_appClkSel_d2                               0x1
    #define        Gbl_clkSelect_appClkSel_d4                               0x2
    #define        Gbl_clkSelect_appClkSel_d6                               0x3
    #define        Gbl_clkSelect_appClkSel_d8                               0x4
    #define        Gbl_clkSelect_appClkSel_d12                              0x5

    #define     BA_Gbl_clkSelect_audioFastExtClkSel            0x009D
    #define     B16Gbl_clkSelect_audioFastExtClkSel            0x009C
    #define   LSb32Gbl_clkSelect_audioFastExtClkSel               15
    #define   LSb16Gbl_clkSelect_audioFastExtClkSel               15
    #define       bGbl_clkSelect_audioFastExtClkSel            1
    #define   MSK32Gbl_clkSelect_audioFastExtClkSel               0x00008000

    #define     BA_Gbl_clkSelect_audioFastClkSel               0x009E
    #define     B16Gbl_clkSelect_audioFastClkSel               0x009E
    #define   LSb32Gbl_clkSelect_audioFastClkSel                  16
    #define   LSb16Gbl_clkSelect_audioFastClkSel                  0
    #define       bGbl_clkSelect_audioFastClkSel               1
    #define   MSK32Gbl_clkSelect_audioFastClkSel                  0x00010000
    #define        Gbl_clkSelect_audioFastClkSel_pllClk                     0x0
    #define        Gbl_clkSelect_audioFastClkSel_extClk                     0x1

    #define     BA_Gbl_clkSelect_audio0ClkSel                  0x009E
    #define     B16Gbl_clkSelect_audio0ClkSel                  0x009E
    #define   LSb32Gbl_clkSelect_audio0ClkSel                     17
    #define   LSb16Gbl_clkSelect_audio0ClkSel                     1
    #define       bGbl_clkSelect_audio0ClkSel                  3
    #define   MSK32Gbl_clkSelect_audio0ClkSel                     0x000E0000
    #define        Gbl_clkSelect_audio0ClkSel_d2                            0x1
    #define        Gbl_clkSelect_audio0ClkSel_d4                            0x2
    #define        Gbl_clkSelect_audio0ClkSel_d6                            0x3
    #define        Gbl_clkSelect_audio0ClkSel_d8                            0x4
    #define        Gbl_clkSelect_audio0ClkSel_d12                           0x5

    #define     BA_Gbl_clkSelect_audio2ClkSel                  0x009E
    #define     B16Gbl_clkSelect_audio2ClkSel                  0x009E
    #define   LSb32Gbl_clkSelect_audio2ClkSel                     20
    #define   LSb16Gbl_clkSelect_audio2ClkSel                     4
    #define       bGbl_clkSelect_audio2ClkSel                  3
    #define   MSK32Gbl_clkSelect_audio2ClkSel                     0x00700000
    #define        Gbl_clkSelect_audio2ClkSel_d2                            0x1
    #define        Gbl_clkSelect_audio2ClkSel_d4                            0x2
    #define        Gbl_clkSelect_audio2ClkSel_d6                            0x3
    #define        Gbl_clkSelect_audio2ClkSel_d8                            0x4
    #define        Gbl_clkSelect_audio2ClkSel_d12                           0x5

    #define     BA_Gbl_clkSelect_audio3ClkSel                  0x009E
    #define     B16Gbl_clkSelect_audio3ClkSel                  0x009E
    #define   LSb32Gbl_clkSelect_audio3ClkSel                     23
    #define   LSb16Gbl_clkSelect_audio3ClkSel                     7
    #define       bGbl_clkSelect_audio3ClkSel                  3
    #define   MSK32Gbl_clkSelect_audio3ClkSel                     0x03800000
    #define        Gbl_clkSelect_audio3ClkSel_d2                            0x1
    #define        Gbl_clkSelect_audio3ClkSel_d4                            0x2
    #define        Gbl_clkSelect_audio3ClkSel_d6                            0x3
    #define        Gbl_clkSelect_audio3ClkSel_d8                            0x4
    #define        Gbl_clkSelect_audio3ClkSel_d12                           0x5

    #define     BA_Gbl_clkSelect_audioHdExtClkSel              0x009F
    #define     B16Gbl_clkSelect_audioHdExtClkSel              0x009E
    #define   LSb32Gbl_clkSelect_audioHdExtClkSel                 26
    #define   LSb16Gbl_clkSelect_audioHdExtClkSel                 10
    #define       bGbl_clkSelect_audioHdExtClkSel              1
    #define   MSK32Gbl_clkSelect_audioHdExtClkSel                 0x04000000

    #define     BA_Gbl_clkSelect_audioHdClkSel                 0x009F
    #define     B16Gbl_clkSelect_audioHdClkSel                 0x009E
    #define   LSb32Gbl_clkSelect_audioHdClkSel                    27
    #define   LSb16Gbl_clkSelect_audioHdClkSel                    11
    #define       bGbl_clkSelect_audioHdClkSel                 1
    #define   MSK32Gbl_clkSelect_audioHdClkSel                    0x08000000
    #define        Gbl_clkSelect_audioHdClkSel_pllClk                       0x0
    #define        Gbl_clkSelect_audioHdClkSel_extClk                       0x1

    #define     BA_Gbl_clkSelect_audio1ExtClkSel               0x009F
    #define     B16Gbl_clkSelect_audio1ExtClkSel               0x009E
    #define   LSb32Gbl_clkSelect_audio1ExtClkSel                  28
    #define   LSb16Gbl_clkSelect_audio1ExtClkSel                  12
    #define       bGbl_clkSelect_audio1ExtClkSel               1
    #define   MSK32Gbl_clkSelect_audio1ExtClkSel                  0x10000000

    #define     BA_Gbl_clkSelect_audio1ClkPllSel               0x009F
    #define     B16Gbl_clkSelect_audio1ClkPllSel               0x009E
    #define   LSb32Gbl_clkSelect_audio1ClkPllSel                  29
    #define   LSb16Gbl_clkSelect_audio1ClkPllSel                  13
    #define       bGbl_clkSelect_audio1ClkPllSel               1
    #define   MSK32Gbl_clkSelect_audio1ClkPllSel                  0x20000000
    #define        Gbl_clkSelect_audio1ClkPllSel_AVPllB3                    0x0
    #define        Gbl_clkSelect_audio1ClkPllSel_AVPllA3                    0x1

    #define     BA_Gbl_clkSelect_audio1SrcClkSel               0x009F
    #define     B16Gbl_clkSelect_audio1SrcClkSel               0x009E
    #define   LSb32Gbl_clkSelect_audio1SrcClkSel                  30
    #define   LSb16Gbl_clkSelect_audio1SrcClkSel                  14
    #define       bGbl_clkSelect_audio1SrcClkSel               1
    #define   MSK32Gbl_clkSelect_audio1SrcClkSel                  0x40000000
    #define        Gbl_clkSelect_audio1SrcClkSel_pllClk                     0x0
    #define        Gbl_clkSelect_audio1SrcClkSel_extClk                     0x1

    #define     RA_Gbl_clkSelect3                              0x00A0

    #define     BA_Gbl_clkSelect_audio1ClkSel                  0x00A0
    #define     B16Gbl_clkSelect_audio1ClkSel                  0x00A0
    #define   LSb32Gbl_clkSelect_audio1ClkSel                     0
    #define   LSb16Gbl_clkSelect_audio1ClkSel                     0
    #define       bGbl_clkSelect_audio1ClkSel                  3
    #define   MSK32Gbl_clkSelect_audio1ClkSel                     0x00000007
    #define        Gbl_clkSelect_audio1ClkSel_d2                            0x1
    #define        Gbl_clkSelect_audio1ClkSel_d4                            0x2
    #define        Gbl_clkSelect_audio1ClkSel_d6                            0x3
    #define        Gbl_clkSelect_audio1ClkSel_d8                            0x4
    #define        Gbl_clkSelect_audio1ClkSel_d12                           0x5

    #define     BA_Gbl_clkSelect_video0ExtClkSel               0x00A0
    #define     B16Gbl_clkSelect_video0ExtClkSel               0x00A0
    #define   LSb32Gbl_clkSelect_video0ExtClkSel                  3
    #define   LSb16Gbl_clkSelect_video0ExtClkSel                  3
    #define       bGbl_clkSelect_video0ExtClkSel               1
    #define   MSK32Gbl_clkSelect_video0ExtClkSel                  0x00000008

    #define     BA_Gbl_clkSelect_video0ClkSel                  0x00A0
    #define     B16Gbl_clkSelect_video0ClkSel                  0x00A0
    #define   LSb32Gbl_clkSelect_video0ClkSel                     4
    #define   LSb16Gbl_clkSelect_video0ClkSel                     4
    #define       bGbl_clkSelect_video0ClkSel                  1
    #define   MSK32Gbl_clkSelect_video0ClkSel                     0x00000010
    #define        Gbl_clkSelect_video0ClkSel_pllClk                        0x0
    #define        Gbl_clkSelect_video0ClkSel_extClk                        0x1

    #define     BA_Gbl_clkSelect_video1ExtClkSel               0x00A0
    #define     B16Gbl_clkSelect_video1ExtClkSel               0x00A0
    #define   LSb32Gbl_clkSelect_video1ExtClkSel                  5
    #define   LSb16Gbl_clkSelect_video1ExtClkSel                  5
    #define       bGbl_clkSelect_video1ExtClkSel               1
    #define   MSK32Gbl_clkSelect_video1ExtClkSel                  0x00000020

    #define     BA_Gbl_clkSelect_video1ClkSel                  0x00A0
    #define     B16Gbl_clkSelect_video1ClkSel                  0x00A0
    #define   LSb32Gbl_clkSelect_video1ClkSel                     6
    #define   LSb16Gbl_clkSelect_video1ClkSel                     6
    #define       bGbl_clkSelect_video1ClkSel                  1
    #define   MSK32Gbl_clkSelect_video1ClkSel                     0x00000040
    #define        Gbl_clkSelect_video1ClkSel_pllClk                        0x0
    #define        Gbl_clkSelect_video1ClkSel_extClk                        0x1

    #define     BA_Gbl_clkSelect_video1ClkPllSel               0x00A0
    #define     B16Gbl_clkSelect_video1ClkPllSel               0x00A0
    #define   LSb32Gbl_clkSelect_video1ClkPllSel                  7
    #define   LSb16Gbl_clkSelect_video1ClkPllSel                  7
    #define       bGbl_clkSelect_video1ClkPllSel               1
    #define   MSK32Gbl_clkSelect_video1ClkPllSel                  0x00000080
    #define        Gbl_clkSelect_video1ClkPllSel_AVPllA2                    0x0
    #define        Gbl_clkSelect_video1ClkPllSel_AVPllB2                    0x1

    #define     BA_Gbl_clkSelect_video2ExtClkSel               0x00A1
    #define     B16Gbl_clkSelect_video2ExtClkSel               0x00A0
    #define   LSb32Gbl_clkSelect_video2ExtClkSel                  8
    #define   LSb16Gbl_clkSelect_video2ExtClkSel                  8
    #define       bGbl_clkSelect_video2ExtClkSel               1
    #define   MSK32Gbl_clkSelect_video2ExtClkSel                  0x00000100

    #define     BA_Gbl_clkSelect_video2ClkSel                  0x00A1
    #define     B16Gbl_clkSelect_video2ClkSel                  0x00A0
    #define   LSb32Gbl_clkSelect_video2ClkSel                     9
    #define   LSb16Gbl_clkSelect_video2ClkSel                     9
    #define       bGbl_clkSelect_video2ClkSel                  1
    #define   MSK32Gbl_clkSelect_video2ClkSel                     0x00000200
    #define        Gbl_clkSelect_video2ClkSel_pllClk                        0x0
    #define        Gbl_clkSelect_video2ClkSel_extClk                        0x1

    #define     BA_Gbl_clkSelect_video2ClkPllSel               0x00A1
    #define     B16Gbl_clkSelect_video2ClkPllSel               0x00A0
    #define   LSb32Gbl_clkSelect_video2ClkPllSel                  10
    #define   LSb16Gbl_clkSelect_video2ClkPllSel                  10
    #define       bGbl_clkSelect_video2ClkPllSel               1
    #define   MSK32Gbl_clkSelect_video2ClkPllSel                  0x00000400
    #define        Gbl_clkSelect_video2ClkPllSel_AVPllB1                    0x0
    #define        Gbl_clkSelect_video2ClkPllSel_AVPllA5                    0x1
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ClkSwitch                               0x00A4

    #define     BA_Gbl_ClkSwitch_sysPLLSWBypass                0x00A4
    #define     B16Gbl_ClkSwitch_sysPLLSWBypass                0x00A4
    #define   LSb32Gbl_ClkSwitch_sysPLLSWBypass                   0
    #define   LSb16Gbl_ClkSwitch_sysPLLSWBypass                   0
    #define       bGbl_ClkSwitch_sysPLLSWBypass                1
    #define   MSK32Gbl_ClkSwitch_sysPLLSWBypass                   0x00000001
    #define        Gbl_ClkSwitch_sysPLLSWBypass_refClk                      0x1
    #define        Gbl_ClkSwitch_sysPLLSWBypass_pllClk                      0x0

    #define     BA_Gbl_ClkSwitch_memPLLSWBypass                0x00A4
    #define     B16Gbl_ClkSwitch_memPLLSWBypass                0x00A4
    #define   LSb32Gbl_ClkSwitch_memPLLSWBypass                   1
    #define   LSb16Gbl_ClkSwitch_memPLLSWBypass                   1
    #define       bGbl_ClkSwitch_memPLLSWBypass                1
    #define   MSK32Gbl_ClkSwitch_memPLLSWBypass                   0x00000002
    #define        Gbl_ClkSwitch_memPLLSWBypass_refClk                      0x1
    #define        Gbl_ClkSwitch_memPLLSWBypass_pllClk                      0x0

    #define     BA_Gbl_ClkSwitch_cpu1PLLSWBypass               0x00A4
    #define     B16Gbl_ClkSwitch_cpu1PLLSWBypass               0x00A4
    #define   LSb32Gbl_ClkSwitch_cpu1PLLSWBypass                  2
    #define   LSb16Gbl_ClkSwitch_cpu1PLLSWBypass                  2
    #define       bGbl_ClkSwitch_cpu1PLLSWBypass               1
    #define   MSK32Gbl_ClkSwitch_cpu1PLLSWBypass                  0x00000004
    #define        Gbl_ClkSwitch_cpu1PLLSWBypass_refClk                     0x1
    #define        Gbl_ClkSwitch_cpu1PLLSWBypass_pllClk                     0x0

    #define     BA_Gbl_ClkSwitch_sysClkPllSwitch               0x00A4
    #define     B16Gbl_ClkSwitch_sysClkPllSwitch               0x00A4
    #define   LSb32Gbl_ClkSwitch_sysClkPllSwitch                  3
    #define   LSb16Gbl_ClkSwitch_sysClkPllSwitch                  3
    #define       bGbl_ClkSwitch_sysClkPllSwitch               1
    #define   MSK32Gbl_ClkSwitch_sysClkPllSwitch                  0x00000008
    #define        Gbl_ClkSwitch_sysClkPllSwitch_SYSPLL                     0x0
    #define        Gbl_ClkSwitch_sysClkPllSwitch_AVPLL                      0x1

    #define     BA_Gbl_ClkSwitch_sysClkSwitch                  0x00A4
    #define     B16Gbl_ClkSwitch_sysClkSwitch                  0x00A4
    #define   LSb32Gbl_ClkSwitch_sysClkSwitch                     4
    #define   LSb16Gbl_ClkSwitch_sysClkSwitch                     4
    #define       bGbl_ClkSwitch_sysClkSwitch                  1
    #define   MSK32Gbl_ClkSwitch_sysClkSwitch                     0x00000010
    #define        Gbl_ClkSwitch_sysClkSwitch_SrcClk                        0x0
    #define        Gbl_ClkSwitch_sysClkSwitch_DivClk                        0x1

    #define     BA_Gbl_ClkSwitch_sysClkD3Switch                0x00A4
    #define     B16Gbl_ClkSwitch_sysClkD3Switch                0x00A4
    #define   LSb32Gbl_ClkSwitch_sysClkD3Switch                   5
    #define   LSb16Gbl_ClkSwitch_sysClkD3Switch                   5
    #define       bGbl_ClkSwitch_sysClkD3Switch                1
    #define   MSK32Gbl_ClkSwitch_sysClkD3Switch                   0x00000020
    #define        Gbl_ClkSwitch_sysClkD3Switch_NonDiv3Clk                  0x0
    #define        Gbl_ClkSwitch_sysClkD3Switch_Div3Clk                     0x1

    #define     BA_Gbl_ClkSwitch_cpu0ClkPllSwitch              0x00A4
    #define     B16Gbl_ClkSwitch_cpu0ClkPllSwitch              0x00A4
    #define   LSb32Gbl_ClkSwitch_cpu0ClkPllSwitch                 6
    #define   LSb16Gbl_ClkSwitch_cpu0ClkPllSwitch                 6
    #define       bGbl_ClkSwitch_cpu0ClkPllSwitch              1
    #define   MSK32Gbl_ClkSwitch_cpu0ClkPllSwitch                 0x00000040
    #define        Gbl_ClkSwitch_cpu0ClkPllSwitch_SYSPLL                    0x0
    #define        Gbl_ClkSwitch_cpu0ClkPllSwitch_AVPLL                     0x1

    #define     BA_Gbl_ClkSwitch_cpu0ClkSwitch                 0x00A4
    #define     B16Gbl_ClkSwitch_cpu0ClkSwitch                 0x00A4
    #define   LSb32Gbl_ClkSwitch_cpu0ClkSwitch                    7
    #define   LSb16Gbl_ClkSwitch_cpu0ClkSwitch                    7
    #define       bGbl_ClkSwitch_cpu0ClkSwitch                 1
    #define   MSK32Gbl_ClkSwitch_cpu0ClkSwitch                    0x00000080
    #define        Gbl_ClkSwitch_cpu0ClkSwitch_SrcClk                       0x0
    #define        Gbl_ClkSwitch_cpu0ClkSwitch_DivClk                       0x1

    #define     BA_Gbl_ClkSwitch_cpu0ClkD3Switch               0x00A5
    #define     B16Gbl_ClkSwitch_cpu0ClkD3Switch               0x00A4
    #define   LSb32Gbl_ClkSwitch_cpu0ClkD3Switch                  8
    #define   LSb16Gbl_ClkSwitch_cpu0ClkD3Switch                  8
    #define       bGbl_ClkSwitch_cpu0ClkD3Switch               1
    #define   MSK32Gbl_ClkSwitch_cpu0ClkD3Switch                  0x00000100
    #define        Gbl_ClkSwitch_cpu0ClkD3Switch_NonDiv3Clk                 0x0
    #define        Gbl_ClkSwitch_cpu0ClkD3Switch_Div3Clk                    0x1

    #define     BA_Gbl_ClkSwitch_cpu1ClkPllSwitch              0x00A5
    #define     B16Gbl_ClkSwitch_cpu1ClkPllSwitch              0x00A4
    #define   LSb32Gbl_ClkSwitch_cpu1ClkPllSwitch                 9
    #define   LSb16Gbl_ClkSwitch_cpu1ClkPllSwitch                 9
    #define       bGbl_ClkSwitch_cpu1ClkPllSwitch              1
    #define   MSK32Gbl_ClkSwitch_cpu1ClkPllSwitch                 0x00000200
    #define        Gbl_ClkSwitch_cpu1ClkPllSwitch_CPU1PLL                   0x0
    #define        Gbl_ClkSwitch_cpu1ClkPllSwitch_AVPLL                     0x1

    #define     BA_Gbl_ClkSwitch_cpu1ClkSwitch                 0x00A5
    #define     B16Gbl_ClkSwitch_cpu1ClkSwitch                 0x00A4
    #define   LSb32Gbl_ClkSwitch_cpu1ClkSwitch                    10
    #define   LSb16Gbl_ClkSwitch_cpu1ClkSwitch                    10
    #define       bGbl_ClkSwitch_cpu1ClkSwitch                 1
    #define   MSK32Gbl_ClkSwitch_cpu1ClkSwitch                    0x00000400
    #define        Gbl_ClkSwitch_cpu1ClkSwitch_SrcClk                       0x0
    #define        Gbl_ClkSwitch_cpu1ClkSwitch_DivClk                       0x1

    #define     BA_Gbl_ClkSwitch_cpu1ClkD3Switch               0x00A5
    #define     B16Gbl_ClkSwitch_cpu1ClkD3Switch               0x00A4
    #define   LSb32Gbl_ClkSwitch_cpu1ClkD3Switch                  11
    #define   LSb16Gbl_ClkSwitch_cpu1ClkD3Switch                  11
    #define       bGbl_ClkSwitch_cpu1ClkD3Switch               1
    #define   MSK32Gbl_ClkSwitch_cpu1ClkD3Switch                  0x00000800
    #define        Gbl_ClkSwitch_cpu1ClkD3Switch_NonDiv3Clk                 0x0
    #define        Gbl_ClkSwitch_cpu1ClkD3Switch_Div3Clk                    0x1

    #define     BA_Gbl_ClkSwitch_drmFigoClkPllSwitch           0x00A5
    #define     B16Gbl_ClkSwitch_drmFigoClkPllSwitch           0x00A4
    #define   LSb32Gbl_ClkSwitch_drmFigoClkPllSwitch              12
    #define   LSb16Gbl_ClkSwitch_drmFigoClkPllSwitch              12
    #define       bGbl_ClkSwitch_drmFigoClkPllSwitch           1
    #define   MSK32Gbl_ClkSwitch_drmFigoClkPllSwitch              0x00001000
    #define        Gbl_ClkSwitch_drmFigoClkPllSwitch_SYSPLL                 0x0
    #define        Gbl_ClkSwitch_drmFigoClkPllSwitch_AVPLL                  0x1

    #define     BA_Gbl_ClkSwitch_drmFigoClkSwitch              0x00A5
    #define     B16Gbl_ClkSwitch_drmFigoClkSwitch              0x00A4
    #define   LSb32Gbl_ClkSwitch_drmFigoClkSwitch                 13
    #define   LSb16Gbl_ClkSwitch_drmFigoClkSwitch                 13
    #define       bGbl_ClkSwitch_drmFigoClkSwitch              1
    #define   MSK32Gbl_ClkSwitch_drmFigoClkSwitch                 0x00002000
    #define        Gbl_ClkSwitch_drmFigoClkSwitch_SrcClk                    0x0
    #define        Gbl_ClkSwitch_drmFigoClkSwitch_DivClk                    0x1

    #define     BA_Gbl_ClkSwitch_drmFigoClkD3Switch            0x00A5
    #define     B16Gbl_ClkSwitch_drmFigoClkD3Switch            0x00A4
    #define   LSb32Gbl_ClkSwitch_drmFigoClkD3Switch               14
    #define   LSb16Gbl_ClkSwitch_drmFigoClkD3Switch               14
    #define       bGbl_ClkSwitch_drmFigoClkD3Switch            1
    #define   MSK32Gbl_ClkSwitch_drmFigoClkD3Switch               0x00004000
    #define        Gbl_ClkSwitch_drmFigoClkD3Switch_NonDiv3Clk              0x0
    #define        Gbl_ClkSwitch_drmFigoClkD3Switch_Div3Clk                 0x1

    #define     BA_Gbl_ClkSwitch_cfgClkPllSwitch               0x00A5
    #define     B16Gbl_ClkSwitch_cfgClkPllSwitch               0x00A4
    #define   LSb32Gbl_ClkSwitch_cfgClkPllSwitch                  15
    #define   LSb16Gbl_ClkSwitch_cfgClkPllSwitch                  15
    #define       bGbl_ClkSwitch_cfgClkPllSwitch               1
    #define   MSK32Gbl_ClkSwitch_cfgClkPllSwitch                  0x00008000
    #define        Gbl_ClkSwitch_cfgClkPllSwitch_SYSPLL                     0x0
    #define        Gbl_ClkSwitch_cfgClkPllSwitch_AVPLL                      0x1

    #define     BA_Gbl_ClkSwitch_cfgClkSwitch                  0x00A6
    #define     B16Gbl_ClkSwitch_cfgClkSwitch                  0x00A6
    #define   LSb32Gbl_ClkSwitch_cfgClkSwitch                     16
    #define   LSb16Gbl_ClkSwitch_cfgClkSwitch                     0
    #define       bGbl_ClkSwitch_cfgClkSwitch                  1
    #define   MSK32Gbl_ClkSwitch_cfgClkSwitch                     0x00010000
    #define        Gbl_ClkSwitch_cfgClkSwitch_SrcClk                        0x0
    #define        Gbl_ClkSwitch_cfgClkSwitch_DivClk                        0x1

    #define     BA_Gbl_ClkSwitch_cfgClkD3Switch                0x00A6
    #define     B16Gbl_ClkSwitch_cfgClkD3Switch                0x00A6
    #define   LSb32Gbl_ClkSwitch_cfgClkD3Switch                   17
    #define   LSb16Gbl_ClkSwitch_cfgClkD3Switch                   1
    #define       bGbl_ClkSwitch_cfgClkD3Switch                1
    #define   MSK32Gbl_ClkSwitch_cfgClkD3Switch                   0x00020000
    #define        Gbl_ClkSwitch_cfgClkD3Switch_NonDiv3Clk                  0x0
    #define        Gbl_ClkSwitch_cfgClkD3Switch_Div3Clk                     0x1

    #define     BA_Gbl_ClkSwitch_gfxClkPllSwitch               0x00A6
    #define     B16Gbl_ClkSwitch_gfxClkPllSwitch               0x00A6
    #define   LSb32Gbl_ClkSwitch_gfxClkPllSwitch                  18
    #define   LSb16Gbl_ClkSwitch_gfxClkPllSwitch                  2
    #define       bGbl_ClkSwitch_gfxClkPllSwitch               1
    #define   MSK32Gbl_ClkSwitch_gfxClkPllSwitch                  0x00040000
    #define        Gbl_ClkSwitch_gfxClkPllSwitch_SYSPLL                     0x0
    #define        Gbl_ClkSwitch_gfxClkPllSwitch_AVPLL                      0x1

    #define     BA_Gbl_ClkSwitch_gfxClkSwitch                  0x00A6
    #define     B16Gbl_ClkSwitch_gfxClkSwitch                  0x00A6
    #define   LSb32Gbl_ClkSwitch_gfxClkSwitch                     19
    #define   LSb16Gbl_ClkSwitch_gfxClkSwitch                     3
    #define       bGbl_ClkSwitch_gfxClkSwitch                  1
    #define   MSK32Gbl_ClkSwitch_gfxClkSwitch                     0x00080000
    #define        Gbl_ClkSwitch_gfxClkSwitch_SrcClk                        0x0
    #define        Gbl_ClkSwitch_gfxClkSwitch_DivClk                        0x1

    #define     BA_Gbl_ClkSwitch_gfxClkD3Switch                0x00A6
    #define     B16Gbl_ClkSwitch_gfxClkD3Switch                0x00A6
    #define   LSb32Gbl_ClkSwitch_gfxClkD3Switch                   20
    #define   LSb16Gbl_ClkSwitch_gfxClkD3Switch                   4
    #define       bGbl_ClkSwitch_gfxClkD3Switch                1
    #define   MSK32Gbl_ClkSwitch_gfxClkD3Switch                   0x00100000
    #define        Gbl_ClkSwitch_gfxClkD3Switch_NonDiv3Clk                  0x0
    #define        Gbl_ClkSwitch_gfxClkD3Switch_Div3Clk                     0x1

    #define     BA_Gbl_ClkSwitch_zspClkPllSwitch               0x00A6
    #define     B16Gbl_ClkSwitch_zspClkPllSwitch               0x00A6
    #define   LSb32Gbl_ClkSwitch_zspClkPllSwitch                  21
    #define   LSb16Gbl_ClkSwitch_zspClkPllSwitch                  5
    #define       bGbl_ClkSwitch_zspClkPllSwitch               1
    #define   MSK32Gbl_ClkSwitch_zspClkPllSwitch                  0x00200000
    #define        Gbl_ClkSwitch_zspClkPllSwitch_SYSPLL                     0x0
    #define        Gbl_ClkSwitch_zspClkPllSwitch_AVPLL                      0x1

    #define     BA_Gbl_ClkSwitch_zspClkSwitch                  0x00A6
    #define     B16Gbl_ClkSwitch_zspClkSwitch                  0x00A6
    #define   LSb32Gbl_ClkSwitch_zspClkSwitch                     22
    #define   LSb16Gbl_ClkSwitch_zspClkSwitch                     6
    #define       bGbl_ClkSwitch_zspClkSwitch                  1
    #define   MSK32Gbl_ClkSwitch_zspClkSwitch                     0x00400000
    #define        Gbl_ClkSwitch_zspClkSwitch_SrcClk                        0x0
    #define        Gbl_ClkSwitch_zspClkSwitch_DivClk                        0x1

    #define     BA_Gbl_ClkSwitch_zspClkD3Switch                0x00A6
    #define     B16Gbl_ClkSwitch_zspClkD3Switch                0x00A6
    #define   LSb32Gbl_ClkSwitch_zspClkD3Switch                   23
    #define   LSb16Gbl_ClkSwitch_zspClkD3Switch                   7
    #define       bGbl_ClkSwitch_zspClkD3Switch                1
    #define   MSK32Gbl_ClkSwitch_zspClkD3Switch                   0x00800000
    #define        Gbl_ClkSwitch_zspClkD3Switch_NonDiv3Clk                  0x0
    #define        Gbl_ClkSwitch_zspClkD3Switch_Div3Clk                     0x1

    #define     BA_Gbl_ClkSwitch_perifClkPllSwitch             0x00A7
    #define     B16Gbl_ClkSwitch_perifClkPllSwitch             0x00A6
    #define   LSb32Gbl_ClkSwitch_perifClkPllSwitch                24
    #define   LSb16Gbl_ClkSwitch_perifClkPllSwitch                8
    #define       bGbl_ClkSwitch_perifClkPllSwitch             1
    #define   MSK32Gbl_ClkSwitch_perifClkPllSwitch                0x01000000
    #define        Gbl_ClkSwitch_perifClkPllSwitch_SYSPLL                   0x0
    #define        Gbl_ClkSwitch_perifClkPllSwitch_AVPLL                    0x1

    #define     BA_Gbl_ClkSwitch_perifClkSwitch                0x00A7
    #define     B16Gbl_ClkSwitch_perifClkSwitch                0x00A6
    #define   LSb32Gbl_ClkSwitch_perifClkSwitch                   25
    #define   LSb16Gbl_ClkSwitch_perifClkSwitch                   9
    #define       bGbl_ClkSwitch_perifClkSwitch                1
    #define   MSK32Gbl_ClkSwitch_perifClkSwitch                   0x02000000
    #define        Gbl_ClkSwitch_perifClkSwitch_SrcClk                      0x0
    #define        Gbl_ClkSwitch_perifClkSwitch_DivClk                      0x1

    #define     BA_Gbl_ClkSwitch_perifClkD3Switch              0x00A7
    #define     B16Gbl_ClkSwitch_perifClkD3Switch              0x00A6
    #define   LSb32Gbl_ClkSwitch_perifClkD3Switch                 26
    #define   LSb16Gbl_ClkSwitch_perifClkD3Switch                 10
    #define       bGbl_ClkSwitch_perifClkD3Switch              1
    #define   MSK32Gbl_ClkSwitch_perifClkD3Switch                 0x04000000
    #define        Gbl_ClkSwitch_perifClkD3Switch_NonDiv3Clk                0x0
    #define        Gbl_ClkSwitch_perifClkD3Switch_Div3Clk                   0x1

    #define     BA_Gbl_ClkSwitch_pCubeClkPllSwitch             0x00A7
    #define     B16Gbl_ClkSwitch_pCubeClkPllSwitch             0x00A6
    #define   LSb32Gbl_ClkSwitch_pCubeClkPllSwitch                27
    #define   LSb16Gbl_ClkSwitch_pCubeClkPllSwitch                11
    #define       bGbl_ClkSwitch_pCubeClkPllSwitch             1
    #define   MSK32Gbl_ClkSwitch_pCubeClkPllSwitch                0x08000000
    #define        Gbl_ClkSwitch_pCubeClkPllSwitch_SYSPLL                   0x0
    #define        Gbl_ClkSwitch_pCubeClkPllSwitch_AVPLL                    0x1

    #define     BA_Gbl_ClkSwitch_pCubeClkSwitch                0x00A7
    #define     B16Gbl_ClkSwitch_pCubeClkSwitch                0x00A6
    #define   LSb32Gbl_ClkSwitch_pCubeClkSwitch                   28
    #define   LSb16Gbl_ClkSwitch_pCubeClkSwitch                   12
    #define       bGbl_ClkSwitch_pCubeClkSwitch                1
    #define   MSK32Gbl_ClkSwitch_pCubeClkSwitch                   0x10000000
    #define        Gbl_ClkSwitch_pCubeClkSwitch_SrcClk                      0x0
    #define        Gbl_ClkSwitch_pCubeClkSwitch_DivClk                      0x1

    #define     BA_Gbl_ClkSwitch_pCubeClkD3Switch              0x00A7
    #define     B16Gbl_ClkSwitch_pCubeClkD3Switch              0x00A6
    #define   LSb32Gbl_ClkSwitch_pCubeClkD3Switch                 29
    #define   LSb16Gbl_ClkSwitch_pCubeClkD3Switch                 13
    #define       bGbl_ClkSwitch_pCubeClkD3Switch              1
    #define   MSK32Gbl_ClkSwitch_pCubeClkD3Switch                 0x20000000
    #define        Gbl_ClkSwitch_pCubeClkD3Switch_NonDiv3Clk                0x0
    #define        Gbl_ClkSwitch_pCubeClkD3Switch_Div3Clk                   0x1

    #define     BA_Gbl_ClkSwitch_vScopeClkPllSwitch            0x00A7
    #define     B16Gbl_ClkSwitch_vScopeClkPllSwitch            0x00A6
    #define   LSb32Gbl_ClkSwitch_vScopeClkPllSwitch               30
    #define   LSb16Gbl_ClkSwitch_vScopeClkPllSwitch               14
    #define       bGbl_ClkSwitch_vScopeClkPllSwitch            1
    #define   MSK32Gbl_ClkSwitch_vScopeClkPllSwitch               0x40000000
    #define        Gbl_ClkSwitch_vScopeClkPllSwitch_SYSPLL                  0x0
    #define        Gbl_ClkSwitch_vScopeClkPllSwitch_AVPLL                   0x1

    #define     BA_Gbl_ClkSwitch_vScopeClkSwitch               0x00A7
    #define     B16Gbl_ClkSwitch_vScopeClkSwitch               0x00A6
    #define   LSb32Gbl_ClkSwitch_vScopeClkSwitch                  31
    #define   LSb16Gbl_ClkSwitch_vScopeClkSwitch                  15
    #define       bGbl_ClkSwitch_vScopeClkSwitch               1
    #define   MSK32Gbl_ClkSwitch_vScopeClkSwitch                  0x80000000
    #define        Gbl_ClkSwitch_vScopeClkSwitch_SrcClk                     0x0
    #define        Gbl_ClkSwitch_vScopeClkSwitch_DivClk                     0x1

    #define     RA_Gbl_ClkSwitch1                              0x00A8

    #define     BA_Gbl_ClkSwitch_vScopeClkD3Switch             0x00A8
    #define     B16Gbl_ClkSwitch_vScopeClkD3Switch             0x00A8
    #define   LSb32Gbl_ClkSwitch_vScopeClkD3Switch                0
    #define   LSb16Gbl_ClkSwitch_vScopeClkD3Switch                0
    #define       bGbl_ClkSwitch_vScopeClkD3Switch             1
    #define   MSK32Gbl_ClkSwitch_vScopeClkD3Switch                0x00000001
    #define        Gbl_ClkSwitch_vScopeClkD3Switch_NonDiv3Clk               0x0
    #define        Gbl_ClkSwitch_vScopeClkD3Switch_Div3Clk                  0x1

    #define     BA_Gbl_ClkSwitch_nfcEccClkPllSwitch            0x00A8
    #define     B16Gbl_ClkSwitch_nfcEccClkPllSwitch            0x00A8
    #define   LSb32Gbl_ClkSwitch_nfcEccClkPllSwitch               1
    #define   LSb16Gbl_ClkSwitch_nfcEccClkPllSwitch               1
    #define       bGbl_ClkSwitch_nfcEccClkPllSwitch            1
    #define   MSK32Gbl_ClkSwitch_nfcEccClkPllSwitch               0x00000002
    #define        Gbl_ClkSwitch_nfcEccClkPllSwitch_SYSPLL                  0x0
    #define        Gbl_ClkSwitch_nfcEccClkPllSwitch_AVPLL                   0x1

    #define     BA_Gbl_ClkSwitch_nfcEccClkSwitch               0x00A8
    #define     B16Gbl_ClkSwitch_nfcEccClkSwitch               0x00A8
    #define   LSb32Gbl_ClkSwitch_nfcEccClkSwitch                  2
    #define   LSb16Gbl_ClkSwitch_nfcEccClkSwitch                  2
    #define       bGbl_ClkSwitch_nfcEccClkSwitch               1
    #define   MSK32Gbl_ClkSwitch_nfcEccClkSwitch                  0x00000004
    #define        Gbl_ClkSwitch_nfcEccClkSwitch_SrcClk                     0x0
    #define        Gbl_ClkSwitch_nfcEccClkSwitch_DivClk                     0x1

    #define     BA_Gbl_ClkSwitch_nfcEccClkD3Switch             0x00A8
    #define     B16Gbl_ClkSwitch_nfcEccClkD3Switch             0x00A8
    #define   LSb32Gbl_ClkSwitch_nfcEccClkD3Switch                3
    #define   LSb16Gbl_ClkSwitch_nfcEccClkD3Switch                3
    #define       bGbl_ClkSwitch_nfcEccClkD3Switch             1
    #define   MSK32Gbl_ClkSwitch_nfcEccClkD3Switch                0x00000008
    #define        Gbl_ClkSwitch_nfcEccClkD3Switch_NonDiv3Clk               0x0
    #define        Gbl_ClkSwitch_nfcEccClkD3Switch_Div3Clk                  0x1

    #define     BA_Gbl_ClkSwitch_vppSysClkPllSwitch            0x00A8
    #define     B16Gbl_ClkSwitch_vppSysClkPllSwitch            0x00A8
    #define   LSb32Gbl_ClkSwitch_vppSysClkPllSwitch               4
    #define   LSb16Gbl_ClkSwitch_vppSysClkPllSwitch               4
    #define       bGbl_ClkSwitch_vppSysClkPllSwitch            1
    #define   MSK32Gbl_ClkSwitch_vppSysClkPllSwitch               0x00000010
    #define        Gbl_ClkSwitch_vppSysClkPllSwitch_SYSPLL                  0x0
    #define        Gbl_ClkSwitch_vppSysClkPllSwitch_AVPLL                   0x1

    #define     BA_Gbl_ClkSwitch_vppSysClkSwitch               0x00A8
    #define     B16Gbl_ClkSwitch_vppSysClkSwitch               0x00A8
    #define   LSb32Gbl_ClkSwitch_vppSysClkSwitch                  5
    #define   LSb16Gbl_ClkSwitch_vppSysClkSwitch                  5
    #define       bGbl_ClkSwitch_vppSysClkSwitch               1
    #define   MSK32Gbl_ClkSwitch_vppSysClkSwitch                  0x00000020
    #define        Gbl_ClkSwitch_vppSysClkSwitch_SrcClk                     0x0
    #define        Gbl_ClkSwitch_vppSysClkSwitch_DivClk                     0x1

    #define     BA_Gbl_ClkSwitch_vppSysClkD3Switch             0x00A8
    #define     B16Gbl_ClkSwitch_vppSysClkD3Switch             0x00A8
    #define   LSb32Gbl_ClkSwitch_vppSysClkD3Switch                6
    #define   LSb16Gbl_ClkSwitch_vppSysClkD3Switch                6
    #define       bGbl_ClkSwitch_vppSysClkD3Switch             1
    #define   MSK32Gbl_ClkSwitch_vppSysClkD3Switch                0x00000040
    #define        Gbl_ClkSwitch_vppSysClkD3Switch_NonDiv3Clk               0x0
    #define        Gbl_ClkSwitch_vppSysClkD3Switch_Div3Clk                  0x1

    #define     BA_Gbl_ClkSwitch_appClkPllSwitch               0x00A8
    #define     B16Gbl_ClkSwitch_appClkPllSwitch               0x00A8
    #define   LSb32Gbl_ClkSwitch_appClkPllSwitch                  7
    #define   LSb16Gbl_ClkSwitch_appClkPllSwitch                  7
    #define       bGbl_ClkSwitch_appClkPllSwitch               1
    #define   MSK32Gbl_ClkSwitch_appClkPllSwitch                  0x00000080
    #define        Gbl_ClkSwitch_appClkPllSwitch_SYSPLL                     0x0
    #define        Gbl_ClkSwitch_appClkPllSwitch_AVPLL                      0x1

    #define     BA_Gbl_ClkSwitch_appClkSwitch                  0x00A9
    #define     B16Gbl_ClkSwitch_appClkSwitch                  0x00A8
    #define   LSb32Gbl_ClkSwitch_appClkSwitch                     8
    #define   LSb16Gbl_ClkSwitch_appClkSwitch                     8
    #define       bGbl_ClkSwitch_appClkSwitch                  1
    #define   MSK32Gbl_ClkSwitch_appClkSwitch                     0x00000100
    #define        Gbl_ClkSwitch_appClkSwitch_SrcClk                        0x0
    #define        Gbl_ClkSwitch_appClkSwitch_DivClk                        0x1

    #define     BA_Gbl_ClkSwitch_appClkD3Switch                0x00A9
    #define     B16Gbl_ClkSwitch_appClkD3Switch                0x00A8
    #define   LSb32Gbl_ClkSwitch_appClkD3Switch                   9
    #define   LSb16Gbl_ClkSwitch_appClkD3Switch                   9
    #define       bGbl_ClkSwitch_appClkD3Switch                1
    #define   MSK32Gbl_ClkSwitch_appClkD3Switch                   0x00000200
    #define        Gbl_ClkSwitch_appClkD3Switch_NonDiv3Clk                  0x0
    #define        Gbl_ClkSwitch_appClkD3Switch_Div3Clk                     0x1

    #define     BA_Gbl_ClkSwitch_audio0ClkSwitch               0x00A9
    #define     B16Gbl_ClkSwitch_audio0ClkSwitch               0x00A8
    #define   LSb32Gbl_ClkSwitch_audio0ClkSwitch                  10
    #define   LSb16Gbl_ClkSwitch_audio0ClkSwitch                  10
    #define       bGbl_ClkSwitch_audio0ClkSwitch               1
    #define   MSK32Gbl_ClkSwitch_audio0ClkSwitch                  0x00000400
    #define        Gbl_ClkSwitch_audio0ClkSwitch_SrcClk                     0x0
    #define        Gbl_ClkSwitch_audio0ClkSwitch_DivClk                     0x1

    #define     BA_Gbl_ClkSwitch_audio0ClkD3Switch             0x00A9
    #define     B16Gbl_ClkSwitch_audio0ClkD3Switch             0x00A8
    #define   LSb32Gbl_ClkSwitch_audio0ClkD3Switch                11
    #define   LSb16Gbl_ClkSwitch_audio0ClkD3Switch                11
    #define       bGbl_ClkSwitch_audio0ClkD3Switch             1
    #define   MSK32Gbl_ClkSwitch_audio0ClkD3Switch                0x00000800
    #define        Gbl_ClkSwitch_audio0ClkD3Switch_NonDiv3Clk               0x0
    #define        Gbl_ClkSwitch_audio0ClkD3Switch_Div3Clk                  0x1

    #define     BA_Gbl_ClkSwitch_audio1ClkSwitch               0x00A9
    #define     B16Gbl_ClkSwitch_audio1ClkSwitch               0x00A8
    #define   LSb32Gbl_ClkSwitch_audio1ClkSwitch                  12
    #define   LSb16Gbl_ClkSwitch_audio1ClkSwitch                  12
    #define       bGbl_ClkSwitch_audio1ClkSwitch               1
    #define   MSK32Gbl_ClkSwitch_audio1ClkSwitch                  0x00001000
    #define        Gbl_ClkSwitch_audio1ClkSwitch_SrcClk                     0x0
    #define        Gbl_ClkSwitch_audio1ClkSwitch_DivClk                     0x1

    #define     BA_Gbl_ClkSwitch_audio1ClkD3Switch             0x00A9
    #define     B16Gbl_ClkSwitch_audio1ClkD3Switch             0x00A8
    #define   LSb32Gbl_ClkSwitch_audio1ClkD3Switch                13
    #define   LSb16Gbl_ClkSwitch_audio1ClkD3Switch                13
    #define       bGbl_ClkSwitch_audio1ClkD3Switch             1
    #define   MSK32Gbl_ClkSwitch_audio1ClkD3Switch                0x00002000
    #define        Gbl_ClkSwitch_audio1ClkD3Switch_NonDiv3Clk               0x0
    #define        Gbl_ClkSwitch_audio1ClkD3Switch_Div3Clk                  0x1

    #define     BA_Gbl_ClkSwitch_audio2ClkSwitch               0x00A9
    #define     B16Gbl_ClkSwitch_audio2ClkSwitch               0x00A8
    #define   LSb32Gbl_ClkSwitch_audio2ClkSwitch                  14
    #define   LSb16Gbl_ClkSwitch_audio2ClkSwitch                  14
    #define       bGbl_ClkSwitch_audio2ClkSwitch               1
    #define   MSK32Gbl_ClkSwitch_audio2ClkSwitch                  0x00004000
    #define        Gbl_ClkSwitch_audio2ClkSwitch_SrcClk                     0x0
    #define        Gbl_ClkSwitch_audio2ClkSwitch_DivClk                     0x1

    #define     BA_Gbl_ClkSwitch_audio2ClkD3Switch             0x00A9
    #define     B16Gbl_ClkSwitch_audio2ClkD3Switch             0x00A8
    #define   LSb32Gbl_ClkSwitch_audio2ClkD3Switch                15
    #define   LSb16Gbl_ClkSwitch_audio2ClkD3Switch                15
    #define       bGbl_ClkSwitch_audio2ClkD3Switch             1
    #define   MSK32Gbl_ClkSwitch_audio2ClkD3Switch                0x00008000
    #define        Gbl_ClkSwitch_audio2ClkD3Switch_NonDiv3Clk               0x0
    #define        Gbl_ClkSwitch_audio2ClkD3Switch_Div3Clk                  0x1

    #define     BA_Gbl_ClkSwitch_audio3ClkSwitch               0x00AA
    #define     B16Gbl_ClkSwitch_audio3ClkSwitch               0x00AA
    #define   LSb32Gbl_ClkSwitch_audio3ClkSwitch                  16
    #define   LSb16Gbl_ClkSwitch_audio3ClkSwitch                  0
    #define       bGbl_ClkSwitch_audio3ClkSwitch               1
    #define   MSK32Gbl_ClkSwitch_audio3ClkSwitch                  0x00010000
    #define        Gbl_ClkSwitch_audio3ClkSwitch_SrcClk                     0x0
    #define        Gbl_ClkSwitch_audio3ClkSwitch_DivClk                     0x1

    #define     BA_Gbl_ClkSwitch_audio3ClkD3Switch             0x00AA
    #define     B16Gbl_ClkSwitch_audio3ClkD3Switch             0x00AA
    #define   LSb32Gbl_ClkSwitch_audio3ClkD3Switch                17
    #define   LSb16Gbl_ClkSwitch_audio3ClkD3Switch                1
    #define       bGbl_ClkSwitch_audio3ClkD3Switch             1
    #define   MSK32Gbl_ClkSwitch_audio3ClkD3Switch                0x00020000
    #define        Gbl_ClkSwitch_audio3ClkD3Switch_NonDiv3Clk               0x0
    #define        Gbl_ClkSwitch_audio3ClkD3Switch_Div3Clk                  0x1
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_PadSelect                               0x00AC

    #define     BA_Gbl_PadSelect_CLK0_V18EN                    0x00AC
    #define     B16Gbl_PadSelect_CLK0_V18EN                    0x00AC
    #define   LSb32Gbl_PadSelect_CLK0_V18EN                       0
    #define   LSb16Gbl_PadSelect_CLK0_V18EN                       0
    #define       bGbl_PadSelect_CLK0_V18EN                    1
    #define   MSK32Gbl_PadSelect_CLK0_V18EN                       0x00000001
    #define        Gbl_PadSelect_CLK0_V18EN_V1P8                            0x1
    #define        Gbl_PadSelect_CLK0_V18EN_V3R3                            0x0

    #define     BA_Gbl_PadSelect_DVIO0_V18EN                   0x00AC
    #define     B16Gbl_PadSelect_DVIO0_V18EN                   0x00AC
    #define   LSb32Gbl_PadSelect_DVIO0_V18EN                      1
    #define   LSb16Gbl_PadSelect_DVIO0_V18EN                      1
    #define       bGbl_PadSelect_DVIO0_V18EN                   1
    #define   MSK32Gbl_PadSelect_DVIO0_V18EN                      0x00000002
    #define        Gbl_PadSelect_DVIO0_V18EN_V1P8                           0x1
    #define        Gbl_PadSelect_DVIO0_V18EN_V3R3                           0x0

    #define     BA_Gbl_PadSelect_SOC0_V18EN                    0x00AC
    #define     B16Gbl_PadSelect_SOC0_V18EN                    0x00AC
    #define   LSb32Gbl_PadSelect_SOC0_V18EN                       2
    #define   LSb16Gbl_PadSelect_SOC0_V18EN                       2
    #define       bGbl_PadSelect_SOC0_V18EN                    1
    #define   MSK32Gbl_PadSelect_SOC0_V18EN                       0x00000004
    #define        Gbl_PadSelect_SOC0_V18EN_V1P8                            0x1
    #define        Gbl_PadSelect_SOC0_V18EN_V3R3                            0x0

    #define     BA_Gbl_PadSelect_SPI0_V18EN                    0x00AC
    #define     B16Gbl_PadSelect_SPI0_V18EN                    0x00AC
    #define   LSb32Gbl_PadSelect_SPI0_V18EN                       3
    #define   LSb16Gbl_PadSelect_SPI0_V18EN                       3
    #define       bGbl_PadSelect_SPI0_V18EN                    1
    #define   MSK32Gbl_PadSelect_SPI0_V18EN                       0x00000008
    #define        Gbl_PadSelect_SPI0_V18EN_V1P8                            0x1
    #define        Gbl_PadSelect_SPI0_V18EN_V3R3                            0x0

    #define     BA_Gbl_PadSelect_CLK0_V25EN                    0x00AC
    #define     B16Gbl_PadSelect_CLK0_V25EN                    0x00AC
    #define   LSb32Gbl_PadSelect_CLK0_V25EN                       4
    #define   LSb16Gbl_PadSelect_CLK0_V25EN                       4
    #define       bGbl_PadSelect_CLK0_V25EN                    1
    #define   MSK32Gbl_PadSelect_CLK0_V25EN                       0x00000010
    #define        Gbl_PadSelect_CLK0_V25EN_Enable                          0x1
    #define        Gbl_PadSelect_CLK0_V25EN_Disable                         0x0

    #define     BA_Gbl_PadSelect_DVIO0_V25EN                   0x00AC
    #define     B16Gbl_PadSelect_DVIO0_V25EN                   0x00AC
    #define   LSb32Gbl_PadSelect_DVIO0_V25EN                      5
    #define   LSb16Gbl_PadSelect_DVIO0_V25EN                      5
    #define       bGbl_PadSelect_DVIO0_V25EN                   1
    #define   MSK32Gbl_PadSelect_DVIO0_V25EN                      0x00000020
    #define        Gbl_PadSelect_DVIO0_V25EN_Enable                         0x1
    #define        Gbl_PadSelect_DVIO0_V25EN_Disable                        0x0

    #define     BA_Gbl_PadSelect_TS0_V25EN                     0x00AC
    #define     B16Gbl_PadSelect_TS0_V25EN                     0x00AC
    #define   LSb32Gbl_PadSelect_TS0_V25EN                        6
    #define   LSb16Gbl_PadSelect_TS0_V25EN                        6
    #define       bGbl_PadSelect_TS0_V25EN                     1
    #define   MSK32Gbl_PadSelect_TS0_V25EN                        0x00000040
    #define        Gbl_PadSelect_TS0_V25EN_Enable                           0x1
    #define        Gbl_PadSelect_TS0_V25EN_Disable                          0x0

    #define     BA_Gbl_PadSelect_SPI0_V25EN                    0x00AC
    #define     B16Gbl_PadSelect_SPI0_V25EN                    0x00AC
    #define   LSb32Gbl_PadSelect_SPI0_V25EN                       7
    #define   LSb16Gbl_PadSelect_SPI0_V25EN                       7
    #define       bGbl_PadSelect_SPI0_V25EN                    1
    #define   MSK32Gbl_PadSelect_SPI0_V25EN                       0x00000080
    #define        Gbl_PadSelect_SPI0_V25EN_Enable                          0x1
    #define        Gbl_PadSelect_SPI0_V25EN_Disable                         0x0

    #define     BA_Gbl_PadSelect_U6_V25EN                      0x00AD
    #define     B16Gbl_PadSelect_U6_V25EN                      0x00AC
    #define   LSb32Gbl_PadSelect_U6_V25EN                         8
    #define   LSb16Gbl_PadSelect_U6_V25EN                         8
    #define       bGbl_PadSelect_U6_V25EN                      1
    #define   MSK32Gbl_PadSelect_U6_V25EN                         0x00000100
    #define        Gbl_PadSelect_U6_V25EN_Enable                            0x1
    #define        Gbl_PadSelect_U6_V25EN_Disable                           0x0

    #define     BA_Gbl_PadSelect_W14_V25EN                     0x00AD
    #define     B16Gbl_PadSelect_W14_V25EN                     0x00AC
    #define   LSb32Gbl_PadSelect_W14_V25EN                        9
    #define   LSb16Gbl_PadSelect_W14_V25EN                        9
    #define       bGbl_PadSelect_W14_V25EN                     1
    #define   MSK32Gbl_PadSelect_W14_V25EN                        0x00000200
    #define        Gbl_PadSelect_W14_V25EN_Enable                           0x1
    #define        Gbl_PadSelect_W14_V25EN_Disable                          0x0

    #define     BA_Gbl_PadSelect_DVU0_V25EN                    0x00AD
    #define     B16Gbl_PadSelect_DVU0_V25EN                    0x00AC
    #define   LSb32Gbl_PadSelect_DVU0_V25EN                       10
    #define   LSb16Gbl_PadSelect_DVU0_V25EN                       10
    #define       bGbl_PadSelect_DVU0_V25EN                    1
    #define   MSK32Gbl_PadSelect_DVU0_V25EN                       0x00000400
    #define        Gbl_PadSelect_DVU0_V25EN_Enable                          0x1
    #define        Gbl_PadSelect_DVU0_V25EN_Disable                         0x0

    #define     BA_Gbl_PadSelect_SOC0_V25EN                    0x00AD
    #define     B16Gbl_PadSelect_SOC0_V25EN                    0x00AC
    #define   LSb32Gbl_PadSelect_SOC0_V25EN                       11
    #define   LSb16Gbl_PadSelect_SOC0_V25EN                       11
    #define       bGbl_PadSelect_SOC0_V25EN                    1
    #define   MSK32Gbl_PadSelect_SOC0_V25EN                       0x00000800
    #define        Gbl_PadSelect_SOC0_V25EN_Enable                          0x1
    #define        Gbl_PadSelect_SOC0_V25EN_Disable                         0x0

    #define     BA_Gbl_PadSelect_ETU1_V25EN                    0x00AD
    #define     B16Gbl_PadSelect_ETU1_V25EN                    0x00AC
    #define   LSb32Gbl_PadSelect_ETU1_V25EN                       12
    #define   LSb16Gbl_PadSelect_ETU1_V25EN                       12
    #define       bGbl_PadSelect_ETU1_V25EN                    1
    #define   MSK32Gbl_PadSelect_ETU1_V25EN                       0x00001000
    #define        Gbl_PadSelect_ETU1_V25EN_Enable                          0x1
    #define        Gbl_PadSelect_ETU1_V25EN_Disable                         0x0

    #define     BA_Gbl_PadSelect_DVIO_OEN                      0x00AD
    #define     B16Gbl_PadSelect_DVIO_OEN                      0x00AC
    #define   LSb32Gbl_PadSelect_DVIO_OEN                         13
    #define   LSb16Gbl_PadSelect_DVIO_OEN                         13
    #define       bGbl_PadSelect_DVIO_OEN                      1
    #define   MSK32Gbl_PadSelect_DVIO_OEN                         0x00002000
    #define        Gbl_PadSelect_DVIO_OEN_Enable                            0x1
    #define        Gbl_PadSelect_DVIO_OEN_Disable                           0x0

    #define     BA_Gbl_PadSelect_CLK0_ZN                       0x00AD
    #define     B16Gbl_PadSelect_CLK0_ZN                       0x00AC
    #define   LSb32Gbl_PadSelect_CLK0_ZN                          14
    #define   LSb16Gbl_PadSelect_CLK0_ZN                          14
    #define       bGbl_PadSelect_CLK0_ZN                       3
    #define   MSK32Gbl_PadSelect_CLK0_ZN                          0x0001C000

    #define     BA_Gbl_PadSelect_CLK0_ZP                       0x00AE
    #define     B16Gbl_PadSelect_CLK0_ZP                       0x00AE
    #define   LSb32Gbl_PadSelect_CLK0_ZP                          17
    #define   LSb16Gbl_PadSelect_CLK0_ZP                          1
    #define       bGbl_PadSelect_CLK0_ZP                       3
    #define   MSK32Gbl_PadSelect_CLK0_ZP                          0x000E0000

    #define     BA_Gbl_PadSelect_DVIO0_ZN                      0x00AE
    #define     B16Gbl_PadSelect_DVIO0_ZN                      0x00AE
    #define   LSb32Gbl_PadSelect_DVIO0_ZN                         20
    #define   LSb16Gbl_PadSelect_DVIO0_ZN                         4
    #define       bGbl_PadSelect_DVIO0_ZN                      3
    #define   MSK32Gbl_PadSelect_DVIO0_ZN                         0x00700000

    #define     BA_Gbl_PadSelect_DVIO0_ZP                      0x00AE
    #define     B16Gbl_PadSelect_DVIO0_ZP                      0x00AE
    #define   LSb32Gbl_PadSelect_DVIO0_ZP                         23
    #define   LSb16Gbl_PadSelect_DVIO0_ZP                         7
    #define       bGbl_PadSelect_DVIO0_ZP                      3
    #define   MSK32Gbl_PadSelect_DVIO0_ZP                         0x03800000

    #define     BA_Gbl_PadSelect_ND0_ZN                        0x00AF
    #define     B16Gbl_PadSelect_ND0_ZN                        0x00AE
    #define   LSb32Gbl_PadSelect_ND0_ZN                           26
    #define   LSb16Gbl_PadSelect_ND0_ZN                           10
    #define       bGbl_PadSelect_ND0_ZN                        3
    #define   MSK32Gbl_PadSelect_ND0_ZN                           0x1C000000

    #define     BA_Gbl_PadSelect_ND0_ZP                        0x00AF
    #define     B16Gbl_PadSelect_ND0_ZP                        0x00AE
    #define   LSb32Gbl_PadSelect_ND0_ZP                           29
    #define   LSb16Gbl_PadSelect_ND0_ZP                           13
    #define       bGbl_PadSelect_ND0_ZP                        3
    #define   MSK32Gbl_PadSelect_ND0_ZP                           0xE0000000

    #define     RA_Gbl_PadSelect1                              0x00B0

    #define     BA_Gbl_PadSelect_SOC0_ZN                       0x00B0
    #define     B16Gbl_PadSelect_SOC0_ZN                       0x00B0
    #define   LSb32Gbl_PadSelect_SOC0_ZN                          0
    #define   LSb16Gbl_PadSelect_SOC0_ZN                          0
    #define       bGbl_PadSelect_SOC0_ZN                       3
    #define   MSK32Gbl_PadSelect_SOC0_ZN                          0x00000007

    #define     BA_Gbl_PadSelect_SOC0_ZP                       0x00B0
    #define     B16Gbl_PadSelect_SOC0_ZP                       0x00B0
    #define   LSb32Gbl_PadSelect_SOC0_ZP                          3
    #define   LSb16Gbl_PadSelect_SOC0_ZP                          3
    #define       bGbl_PadSelect_SOC0_ZP                       3
    #define   MSK32Gbl_PadSelect_SOC0_ZP                          0x00000038

    #define     BA_Gbl_PadSelect_SPI0_ZN                       0x00B0
    #define     B16Gbl_PadSelect_SPI0_ZN                       0x00B0
    #define   LSb32Gbl_PadSelect_SPI0_ZN                          6
    #define   LSb16Gbl_PadSelect_SPI0_ZN                          6
    #define       bGbl_PadSelect_SPI0_ZN                       3
    #define   MSK32Gbl_PadSelect_SPI0_ZN                          0x000001C0

    #define     BA_Gbl_PadSelect_SPI0_ZP                       0x00B1
    #define     B16Gbl_PadSelect_SPI0_ZP                       0x00B0
    #define   LSb32Gbl_PadSelect_SPI0_ZP                          9
    #define   LSb16Gbl_PadSelect_SPI0_ZP                          9
    #define       bGbl_PadSelect_SPI0_ZP                       3
    #define   MSK32Gbl_PadSelect_SPI0_ZP                          0x00000E00
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ResetTrigger                            0x00B4

    #define     BA_Gbl_ResetTrigger_chipReset                  0x00B4
    #define     B16Gbl_ResetTrigger_chipReset                  0x00B4
    #define   LSb32Gbl_ResetTrigger_chipReset                     0
    #define   LSb16Gbl_ResetTrigger_chipReset                     0
    #define       bGbl_ResetTrigger_chipReset                  1
    #define   MSK32Gbl_ResetTrigger_chipReset                     0x00000001
    #define        Gbl_ResetTrigger_chipReset_assert                        0x1
    #define        Gbl_ResetTrigger_chipReset_deassert                      0x0

    #define     BA_Gbl_ResetTrigger_audio0ClkReset             0x00B4
    #define     B16Gbl_ResetTrigger_audio0ClkReset             0x00B4
    #define   LSb32Gbl_ResetTrigger_audio0ClkReset                1
    #define   LSb16Gbl_ResetTrigger_audio0ClkReset                1
    #define       bGbl_ResetTrigger_audio0ClkReset             1
    #define   MSK32Gbl_ResetTrigger_audio0ClkReset                0x00000002
    #define        Gbl_ResetTrigger_audio0ClkReset_assert                   0x1
    #define        Gbl_ResetTrigger_audio0ClkReset_deassert                 0x0

    #define     BA_Gbl_ResetTrigger_audio1ClkReset             0x00B4
    #define     B16Gbl_ResetTrigger_audio1ClkReset             0x00B4
    #define   LSb32Gbl_ResetTrigger_audio1ClkReset                2
    #define   LSb16Gbl_ResetTrigger_audio1ClkReset                2
    #define       bGbl_ResetTrigger_audio1ClkReset             1
    #define   MSK32Gbl_ResetTrigger_audio1ClkReset                0x00000004
    #define        Gbl_ResetTrigger_audio1ClkReset_assert                   0x1
    #define        Gbl_ResetTrigger_audio1ClkReset_deassert                 0x0

    #define     BA_Gbl_ResetTrigger_audio2ClkReset             0x00B4
    #define     B16Gbl_ResetTrigger_audio2ClkReset             0x00B4
    #define   LSb32Gbl_ResetTrigger_audio2ClkReset                3
    #define   LSb16Gbl_ResetTrigger_audio2ClkReset                3
    #define       bGbl_ResetTrigger_audio2ClkReset             1
    #define   MSK32Gbl_ResetTrigger_audio2ClkReset                0x00000008
    #define        Gbl_ResetTrigger_audio2ClkReset_assert                   0x1
    #define        Gbl_ResetTrigger_audio2ClkReset_deassert                 0x0

    #define     BA_Gbl_ResetTrigger_audio3ClkReset             0x00B4
    #define     B16Gbl_ResetTrigger_audio3ClkReset             0x00B4
    #define   LSb32Gbl_ResetTrigger_audio3ClkReset                4
    #define   LSb16Gbl_ResetTrigger_audio3ClkReset                4
    #define       bGbl_ResetTrigger_audio3ClkReset             1
    #define   MSK32Gbl_ResetTrigger_audio3ClkReset                0x00000010
    #define        Gbl_ResetTrigger_audio3ClkReset_assert                   0x1
    #define        Gbl_ResetTrigger_audio3ClkReset_deassert                 0x0

    #define     BA_Gbl_ResetTrigger_audioHdClkReset            0x00B4
    #define     B16Gbl_ResetTrigger_audioHdClkReset            0x00B4
    #define   LSb32Gbl_ResetTrigger_audioHdClkReset               5
    #define   LSb16Gbl_ResetTrigger_audioHdClkReset               5
    #define       bGbl_ResetTrigger_audioHdClkReset            1
    #define   MSK32Gbl_ResetTrigger_audioHdClkReset               0x00000020
    #define        Gbl_ResetTrigger_audioHdClkReset_assert                  0x1
    #define        Gbl_ResetTrigger_audioHdClkReset_deassert                0x0

    #define     BA_Gbl_ResetTrigger_sysPllSyncReset            0x00B4
    #define     B16Gbl_ResetTrigger_sysPllSyncReset            0x00B4
    #define   LSb32Gbl_ResetTrigger_sysPllSyncReset               6
    #define   LSb16Gbl_ResetTrigger_sysPllSyncReset               6
    #define       bGbl_ResetTrigger_sysPllSyncReset            1
    #define   MSK32Gbl_ResetTrigger_sysPllSyncReset               0x00000040
    #define        Gbl_ResetTrigger_sysPllSyncReset_assert                  0x1
    #define        Gbl_ResetTrigger_sysPllSyncReset_deassert                0x0

    #define     BA_Gbl_ResetTrigger_memPllSyncReset            0x00B4
    #define     B16Gbl_ResetTrigger_memPllSyncReset            0x00B4
    #define   LSb32Gbl_ResetTrigger_memPllSyncReset               7
    #define   LSb16Gbl_ResetTrigger_memPllSyncReset               7
    #define       bGbl_ResetTrigger_memPllSyncReset            1
    #define   MSK32Gbl_ResetTrigger_memPllSyncReset               0x00000080
    #define        Gbl_ResetTrigger_memPllSyncReset_assert                  0x1
    #define        Gbl_ResetTrigger_memPllSyncReset_deassert                0x0

    #define     BA_Gbl_ResetTrigger_cpu1PllSyncReset           0x00B5
    #define     B16Gbl_ResetTrigger_cpu1PllSyncReset           0x00B4
    #define   LSb32Gbl_ResetTrigger_cpu1PllSyncReset              8
    #define   LSb16Gbl_ResetTrigger_cpu1PllSyncReset              8
    #define       bGbl_ResetTrigger_cpu1PllSyncReset           1
    #define   MSK32Gbl_ResetTrigger_cpu1PllSyncReset              0x00000100
    #define        Gbl_ResetTrigger_cpu1PllSyncReset_assert                 0x1
    #define        Gbl_ResetTrigger_cpu1PllSyncReset_deassert               0x0

    #define     BA_Gbl_ResetTrigger_cpu0SyncReset              0x00B5
    #define     B16Gbl_ResetTrigger_cpu0SyncReset              0x00B4
    #define   LSb32Gbl_ResetTrigger_cpu0SyncReset                 9
    #define   LSb16Gbl_ResetTrigger_cpu0SyncReset                 9
    #define       bGbl_ResetTrigger_cpu0SyncReset              1
    #define   MSK32Gbl_ResetTrigger_cpu0SyncReset                 0x00000200
    #define        Gbl_ResetTrigger_cpu0SyncReset_assert                    0x1
    #define        Gbl_ResetTrigger_cpu0SyncReset_deassert                  0x0

    #define     BA_Gbl_ResetTrigger_cpu1SyncReset              0x00B5
    #define     B16Gbl_ResetTrigger_cpu1SyncReset              0x00B4
    #define   LSb32Gbl_ResetTrigger_cpu1SyncReset                 10
    #define   LSb16Gbl_ResetTrigger_cpu1SyncReset                 10
    #define       bGbl_ResetTrigger_cpu1SyncReset              1
    #define   MSK32Gbl_ResetTrigger_cpu1SyncReset                 0x00000400
    #define        Gbl_ResetTrigger_cpu1SyncReset_assert                    0x1
    #define        Gbl_ResetTrigger_cpu1SyncReset_deassert                  0x0

    #define     BA_Gbl_ResetTrigger_socDdrSyncReset            0x00B5
    #define     B16Gbl_ResetTrigger_socDdrSyncReset            0x00B4
    #define   LSb32Gbl_ResetTrigger_socDdrSyncReset               11
    #define   LSb16Gbl_ResetTrigger_socDdrSyncReset               11
    #define       bGbl_ResetTrigger_socDdrSyncReset            1
    #define   MSK32Gbl_ResetTrigger_socDdrSyncReset               0x00000800
    #define        Gbl_ResetTrigger_socDdrSyncReset_assert                  0x1
    #define        Gbl_ResetTrigger_socDdrSyncReset_deassert                0x0

    #define     BA_Gbl_ResetTrigger_gfxSyncReset               0x00B5
    #define     B16Gbl_ResetTrigger_gfxSyncReset               0x00B4
    #define   LSb32Gbl_ResetTrigger_gfxSyncReset                  12
    #define   LSb16Gbl_ResetTrigger_gfxSyncReset                  12
    #define       bGbl_ResetTrigger_gfxSyncReset               1
    #define   MSK32Gbl_ResetTrigger_gfxSyncReset                  0x00001000
    #define        Gbl_ResetTrigger_gfxSyncReset_assert                     0x1
    #define        Gbl_ResetTrigger_gfxSyncReset_deassert                   0x0

    #define     BA_Gbl_ResetTrigger_zspSyncReset               0x00B5
    #define     B16Gbl_ResetTrigger_zspSyncReset               0x00B4
    #define   LSb32Gbl_ResetTrigger_zspSyncReset                  13
    #define   LSb16Gbl_ResetTrigger_zspSyncReset                  13
    #define       bGbl_ResetTrigger_zspSyncReset               1
    #define   MSK32Gbl_ResetTrigger_zspSyncReset                  0x00002000
    #define        Gbl_ResetTrigger_zspSyncReset_assert                     0x1
    #define        Gbl_ResetTrigger_zspSyncReset_deassert                   0x0

    #define     BA_Gbl_ResetTrigger_avioSyncReset              0x00B5
    #define     B16Gbl_ResetTrigger_avioSyncReset              0x00B4
    #define   LSb32Gbl_ResetTrigger_avioSyncReset                 14
    #define   LSb16Gbl_ResetTrigger_avioSyncReset                 14
    #define       bGbl_ResetTrigger_avioSyncReset              1
    #define   MSK32Gbl_ResetTrigger_avioSyncReset                 0x00004000
    #define        Gbl_ResetTrigger_avioSyncReset_assert                    0x1
    #define        Gbl_ResetTrigger_avioSyncReset_deassert                  0x0

    #define     BA_Gbl_ResetTrigger_perifSyncReset             0x00B5
    #define     B16Gbl_ResetTrigger_perifSyncReset             0x00B4
    #define   LSb32Gbl_ResetTrigger_perifSyncReset                15
    #define   LSb16Gbl_ResetTrigger_perifSyncReset                15
    #define       bGbl_ResetTrigger_perifSyncReset             1
    #define   MSK32Gbl_ResetTrigger_perifSyncReset                0x00008000
    #define        Gbl_ResetTrigger_perifSyncReset_assert                   0x1
    #define        Gbl_ResetTrigger_perifSyncReset_deassert                 0x0

    #define     BA_Gbl_ResetTrigger_gethSyncReset              0x00B6
    #define     B16Gbl_ResetTrigger_gethSyncReset              0x00B6
    #define   LSb32Gbl_ResetTrigger_gethSyncReset                 16
    #define   LSb16Gbl_ResetTrigger_gethSyncReset                 0
    #define       bGbl_ResetTrigger_gethSyncReset              1
    #define   MSK32Gbl_ResetTrigger_gethSyncReset                 0x00010000
    #define        Gbl_ResetTrigger_gethSyncReset_assert                    0x1
    #define        Gbl_ResetTrigger_gethSyncReset_deassert                  0x0

    #define     BA_Gbl_ResetTrigger_sataSyncReset              0x00B6
    #define     B16Gbl_ResetTrigger_sataSyncReset              0x00B6
    #define   LSb32Gbl_ResetTrigger_sataSyncReset                 17
    #define   LSb16Gbl_ResetTrigger_sataSyncReset                 1
    #define       bGbl_ResetTrigger_sataSyncReset              1
    #define   MSK32Gbl_ResetTrigger_sataSyncReset                 0x00020000
    #define        Gbl_ResetTrigger_sataSyncReset_assert                    0x1
    #define        Gbl_ResetTrigger_sataSyncReset_deassert                  0x0

    #define     BA_Gbl_ResetTrigger_pexSyncReset               0x00B6
    #define     B16Gbl_ResetTrigger_pexSyncReset               0x00B6
    #define   LSb32Gbl_ResetTrigger_pexSyncReset                  18
    #define   LSb16Gbl_ResetTrigger_pexSyncReset                  2
    #define       bGbl_ResetTrigger_pexSyncReset               1
    #define   MSK32Gbl_ResetTrigger_pexSyncReset                  0x00040000
    #define        Gbl_ResetTrigger_pexSyncReset_assert                     0x1
    #define        Gbl_ResetTrigger_pexSyncReset_deassert                   0x0

    #define     BA_Gbl_ResetTrigger_ahbApbSyncReset            0x00B6
    #define     B16Gbl_ResetTrigger_ahbApbSyncReset            0x00B6
    #define   LSb32Gbl_ResetTrigger_ahbApbSyncReset               19
    #define   LSb16Gbl_ResetTrigger_ahbApbSyncReset               3
    #define       bGbl_ResetTrigger_ahbApbSyncReset            1
    #define   MSK32Gbl_ResetTrigger_ahbApbSyncReset               0x00080000
    #define        Gbl_ResetTrigger_ahbApbSyncReset_assert                  0x1
    #define        Gbl_ResetTrigger_ahbApbSyncReset_deassert                0x0

    #define     BA_Gbl_ResetTrigger_nanfSyncReset              0x00B6
    #define     B16Gbl_ResetTrigger_nanfSyncReset              0x00B6
    #define   LSb32Gbl_ResetTrigger_nanfSyncReset                 20
    #define   LSb16Gbl_ResetTrigger_nanfSyncReset                 4
    #define       bGbl_ResetTrigger_nanfSyncReset              1
    #define   MSK32Gbl_ResetTrigger_nanfSyncReset                 0x00100000
    #define        Gbl_ResetTrigger_nanfSyncReset_assert                    0x1
    #define        Gbl_ResetTrigger_nanfSyncReset_deassert                  0x0

    #define     BA_Gbl_ResetTrigger_smemcSyncReset             0x00B6
    #define     B16Gbl_ResetTrigger_smemcSyncReset             0x00B6
    #define   LSb32Gbl_ResetTrigger_smemcSyncReset                21
    #define   LSb16Gbl_ResetTrigger_smemcSyncReset                5
    #define       bGbl_ResetTrigger_smemcSyncReset             1
    #define   MSK32Gbl_ResetTrigger_smemcSyncReset                0x00200000
    #define        Gbl_ResetTrigger_smemcSyncReset_assert                   0x1
    #define        Gbl_ResetTrigger_smemcSyncReset_deassert                 0x0

    #define     BA_Gbl_ResetTrigger_drmSyncReset               0x00B6
    #define     B16Gbl_ResetTrigger_drmSyncReset               0x00B6
    #define   LSb32Gbl_ResetTrigger_drmSyncReset                  22
    #define   LSb16Gbl_ResetTrigger_drmSyncReset                  6
    #define       bGbl_ResetTrigger_drmSyncReset               1
    #define   MSK32Gbl_ResetTrigger_drmSyncReset                  0x00400000
    #define        Gbl_ResetTrigger_drmSyncReset_assert                     0x1
    #define        Gbl_ResetTrigger_drmSyncReset_deassert                   0x0

    #define     BA_Gbl_ResetTrigger_usb0SyncReset              0x00B6
    #define     B16Gbl_ResetTrigger_usb0SyncReset              0x00B6
    #define   LSb32Gbl_ResetTrigger_usb0SyncReset                 23
    #define   LSb16Gbl_ResetTrigger_usb0SyncReset                 7
    #define       bGbl_ResetTrigger_usb0SyncReset              1
    #define   MSK32Gbl_ResetTrigger_usb0SyncReset                 0x00800000
    #define        Gbl_ResetTrigger_usb0SyncReset_assert                    0x1
    #define        Gbl_ResetTrigger_usb0SyncReset_deassert                  0x0

    #define     BA_Gbl_ResetTrigger_usb1SyncReset              0x00B7
    #define     B16Gbl_ResetTrigger_usb1SyncReset              0x00B6
    #define   LSb32Gbl_ResetTrigger_usb1SyncReset                 24
    #define   LSb16Gbl_ResetTrigger_usb1SyncReset                 8
    #define       bGbl_ResetTrigger_usb1SyncReset              1
    #define   MSK32Gbl_ResetTrigger_usb1SyncReset                 0x01000000
    #define        Gbl_ResetTrigger_usb1SyncReset_assert                    0x1
    #define        Gbl_ResetTrigger_usb1SyncReset_deassert                  0x0

    #define     BA_Gbl_ResetTrigger_pBridgeSyncReset           0x00B7
    #define     B16Gbl_ResetTrigger_pBridgeSyncReset           0x00B6
    #define   LSb32Gbl_ResetTrigger_pBridgeSyncReset              25
    #define   LSb16Gbl_ResetTrigger_pBridgeSyncReset              9
    #define       bGbl_ResetTrigger_pBridgeSyncReset           1
    #define   MSK32Gbl_ResetTrigger_pBridgeSyncReset              0x02000000
    #define        Gbl_ResetTrigger_pBridgeSyncReset_assert                 0x1
    #define        Gbl_ResetTrigger_pBridgeSyncReset_deassert               0x0

    #define     BA_Gbl_ResetTrigger_vppSyncReset               0x00B7
    #define     B16Gbl_ResetTrigger_vppSyncReset               0x00B6
    #define   LSb32Gbl_ResetTrigger_vppSyncReset                  26
    #define   LSb16Gbl_ResetTrigger_vppSyncReset                  10
    #define       bGbl_ResetTrigger_vppSyncReset               1
    #define   MSK32Gbl_ResetTrigger_vppSyncReset                  0x04000000
    #define        Gbl_ResetTrigger_vppSyncReset_assert                     0x1
    #define        Gbl_ResetTrigger_vppSyncReset_deassert                   0x0

    #define     BA_Gbl_ResetTrigger_appSyncReset               0x00B7
    #define     B16Gbl_ResetTrigger_appSyncReset               0x00B6
    #define   LSb32Gbl_ResetTrigger_appSyncReset                  27
    #define   LSb16Gbl_ResetTrigger_appSyncReset                  11
    #define       bGbl_ResetTrigger_appSyncReset               1
    #define   MSK32Gbl_ResetTrigger_appSyncReset                  0x08000000
    #define        Gbl_ResetTrigger_appSyncReset_assert                     0x1
    #define        Gbl_ResetTrigger_appSyncReset_deassert                   0x0

    #define     BA_Gbl_ResetTrigger_sdioSyncReset              0x00B7
    #define     B16Gbl_ResetTrigger_sdioSyncReset              0x00B6
    #define   LSb32Gbl_ResetTrigger_sdioSyncReset                 28
    #define   LSb16Gbl_ResetTrigger_sdioSyncReset                 12
    #define       bGbl_ResetTrigger_sdioSyncReset              1
    #define   MSK32Gbl_ResetTrigger_sdioSyncReset                 0x10000000
    #define        Gbl_ResetTrigger_sdioSyncReset_assert                    0x1
    #define        Gbl_ResetTrigger_sdioSyncReset_deassert                  0x0
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ResetStatus                             0x00B8

    #define     BA_Gbl_ResetStatus_ChipResetStatus             0x00B8
    #define     B16Gbl_ResetStatus_ChipResetStatus             0x00B8
    #define   LSb32Gbl_ResetStatus_ChipResetStatus                0
    #define   LSb16Gbl_ResetStatus_ChipResetStatus                0
    #define       bGbl_ResetStatus_ChipResetStatus             1
    #define   MSK32Gbl_ResetStatus_ChipResetStatus                0x00000001
    #define        Gbl_ResetStatus_ChipResetStatus_asserted                 0x1
    #define        Gbl_ResetStatus_ChipResetStatus_deasserted               0x0

    #define     BA_Gbl_ResetStatus_wd0Status                   0x00B8
    #define     B16Gbl_ResetStatus_wd0Status                   0x00B8
    #define   LSb32Gbl_ResetStatus_wd0Status                      1
    #define   LSb16Gbl_ResetStatus_wd0Status                      1
    #define       bGbl_ResetStatus_wd0Status                   1
    #define   MSK32Gbl_ResetStatus_wd0Status                      0x00000002
    #define        Gbl_ResetStatus_wd0Status_asserted                       0x1
    #define        Gbl_ResetStatus_wd0Status_deasserted                     0x0

    #define     BA_Gbl_ResetStatus_wd1Status                   0x00B8
    #define     B16Gbl_ResetStatus_wd1Status                   0x00B8
    #define   LSb32Gbl_ResetStatus_wd1Status                      2
    #define   LSb16Gbl_ResetStatus_wd1Status                      2
    #define       bGbl_ResetStatus_wd1Status                   1
    #define   MSK32Gbl_ResetStatus_wd1Status                      0x00000004
    #define        Gbl_ResetStatus_wd1Status_asserted                       0x1
    #define        Gbl_ResetStatus_wd1Status_deasserted                     0x0

    #define     BA_Gbl_ResetStatus_wd2Status                   0x00B8
    #define     B16Gbl_ResetStatus_wd2Status                   0x00B8
    #define   LSb32Gbl_ResetStatus_wd2Status                      3
    #define   LSb16Gbl_ResetStatus_wd2Status                      3
    #define       bGbl_ResetStatus_wd2Status                   1
    #define   MSK32Gbl_ResetStatus_wd2Status                      0x00000008
    #define        Gbl_ResetStatus_wd2Status_asserted                       0x1
    #define        Gbl_ResetStatus_wd2Status_deasserted                     0x0

    #define     BA_Gbl_ResetStatus_audio0ClkResetStatus        0x00B8
    #define     B16Gbl_ResetStatus_audio0ClkResetStatus        0x00B8
    #define   LSb32Gbl_ResetStatus_audio0ClkResetStatus           4
    #define   LSb16Gbl_ResetStatus_audio0ClkResetStatus           4
    #define       bGbl_ResetStatus_audio0ClkResetStatus        1
    #define   MSK32Gbl_ResetStatus_audio0ClkResetStatus           0x00000010
    #define        Gbl_ResetStatus_audio0ClkResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_audio0ClkResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_audio1ClkResetStatus        0x00B8
    #define     B16Gbl_ResetStatus_audio1ClkResetStatus        0x00B8
    #define   LSb32Gbl_ResetStatus_audio1ClkResetStatus           5
    #define   LSb16Gbl_ResetStatus_audio1ClkResetStatus           5
    #define       bGbl_ResetStatus_audio1ClkResetStatus        1
    #define   MSK32Gbl_ResetStatus_audio1ClkResetStatus           0x00000020
    #define        Gbl_ResetStatus_audio1ClkResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_audio1ClkResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_audio2ClkResetStatus        0x00B8
    #define     B16Gbl_ResetStatus_audio2ClkResetStatus        0x00B8
    #define   LSb32Gbl_ResetStatus_audio2ClkResetStatus           6
    #define   LSb16Gbl_ResetStatus_audio2ClkResetStatus           6
    #define       bGbl_ResetStatus_audio2ClkResetStatus        1
    #define   MSK32Gbl_ResetStatus_audio2ClkResetStatus           0x00000040
    #define        Gbl_ResetStatus_audio2ClkResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_audio2ClkResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_audio3ClkResetStatus        0x00B8
    #define     B16Gbl_ResetStatus_audio3ClkResetStatus        0x00B8
    #define   LSb32Gbl_ResetStatus_audio3ClkResetStatus           7
    #define   LSb16Gbl_ResetStatus_audio3ClkResetStatus           7
    #define       bGbl_ResetStatus_audio3ClkResetStatus        1
    #define   MSK32Gbl_ResetStatus_audio3ClkResetStatus           0x00000080
    #define        Gbl_ResetStatus_audio3ClkResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_audio3ClkResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_audioHdClkResetStatus       0x00B9
    #define     B16Gbl_ResetStatus_audioHdClkResetStatus       0x00B8
    #define   LSb32Gbl_ResetStatus_audioHdClkResetStatus          8
    #define   LSb16Gbl_ResetStatus_audioHdClkResetStatus          8
    #define       bGbl_ResetStatus_audioHdClkResetStatus       1
    #define   MSK32Gbl_ResetStatus_audioHdClkResetStatus          0x00000100
    #define        Gbl_ResetStatus_audioHdClkResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_audioHdClkResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_sysPllSyncResetStatus       0x00B9
    #define     B16Gbl_ResetStatus_sysPllSyncResetStatus       0x00B8
    #define   LSb32Gbl_ResetStatus_sysPllSyncResetStatus          9
    #define   LSb16Gbl_ResetStatus_sysPllSyncResetStatus          9
    #define       bGbl_ResetStatus_sysPllSyncResetStatus       1
    #define   MSK32Gbl_ResetStatus_sysPllSyncResetStatus          0x00000200
    #define        Gbl_ResetStatus_sysPllSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_sysPllSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_memPllSyncResetStatus       0x00B9
    #define     B16Gbl_ResetStatus_memPllSyncResetStatus       0x00B8
    #define   LSb32Gbl_ResetStatus_memPllSyncResetStatus          10
    #define   LSb16Gbl_ResetStatus_memPllSyncResetStatus          10
    #define       bGbl_ResetStatus_memPllSyncResetStatus       1
    #define   MSK32Gbl_ResetStatus_memPllSyncResetStatus          0x00000400
    #define        Gbl_ResetStatus_memPllSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_memPllSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_cpu1PllSyncResetStatus      0x00B9
    #define     B16Gbl_ResetStatus_cpu1PllSyncResetStatus      0x00B8
    #define   LSb32Gbl_ResetStatus_cpu1PllSyncResetStatus         11
    #define   LSb16Gbl_ResetStatus_cpu1PllSyncResetStatus         11
    #define       bGbl_ResetStatus_cpu1PllSyncResetStatus      1
    #define   MSK32Gbl_ResetStatus_cpu1PllSyncResetStatus         0x00000800
    #define        Gbl_ResetStatus_cpu1PllSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_cpu1PllSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_avPllASyncResetStatus       0x00B9
    #define     B16Gbl_ResetStatus_avPllASyncResetStatus       0x00B8
    #define   LSb32Gbl_ResetStatus_avPllASyncResetStatus          12
    #define   LSb16Gbl_ResetStatus_avPllASyncResetStatus          12
    #define       bGbl_ResetStatus_avPllASyncResetStatus       1
    #define   MSK32Gbl_ResetStatus_avPllASyncResetStatus          0x00001000
    #define        Gbl_ResetStatus_avPllASyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_avPllASyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_avPllBSyncResetStatus       0x00B9
    #define     B16Gbl_ResetStatus_avPllBSyncResetStatus       0x00B8
    #define   LSb32Gbl_ResetStatus_avPllBSyncResetStatus          13
    #define   LSb16Gbl_ResetStatus_avPllBSyncResetStatus          13
    #define       bGbl_ResetStatus_avPllBSyncResetStatus       1
    #define   MSK32Gbl_ResetStatus_avPllBSyncResetStatus          0x00002000
    #define        Gbl_ResetStatus_avPllBSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_avPllBSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_cpu0SyncResetStatus         0x00B9
    #define     B16Gbl_ResetStatus_cpu0SyncResetStatus         0x00B8
    #define   LSb32Gbl_ResetStatus_cpu0SyncResetStatus            14
    #define   LSb16Gbl_ResetStatus_cpu0SyncResetStatus            14
    #define       bGbl_ResetStatus_cpu0SyncResetStatus         1
    #define   MSK32Gbl_ResetStatus_cpu0SyncResetStatus            0x00004000
    #define        Gbl_ResetStatus_cpu0SyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_cpu0SyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_cpu1SyncResetStatus         0x00B9
    #define     B16Gbl_ResetStatus_cpu1SyncResetStatus         0x00B8
    #define   LSb32Gbl_ResetStatus_cpu1SyncResetStatus            15
    #define   LSb16Gbl_ResetStatus_cpu1SyncResetStatus            15
    #define       bGbl_ResetStatus_cpu1SyncResetStatus         1
    #define   MSK32Gbl_ResetStatus_cpu1SyncResetStatus            0x00008000
    #define        Gbl_ResetStatus_cpu1SyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_cpu1SyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_socDdrSyncResetStatus       0x00BA
    #define     B16Gbl_ResetStatus_socDdrSyncResetStatus       0x00BA
    #define   LSb32Gbl_ResetStatus_socDdrSyncResetStatus          16
    #define   LSb16Gbl_ResetStatus_socDdrSyncResetStatus          0
    #define       bGbl_ResetStatus_socDdrSyncResetStatus       1
    #define   MSK32Gbl_ResetStatus_socDdrSyncResetStatus          0x00010000
    #define        Gbl_ResetStatus_socDdrSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_socDdrSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_gfxSyncResetStatus          0x00BA
    #define     B16Gbl_ResetStatus_gfxSyncResetStatus          0x00BA
    #define   LSb32Gbl_ResetStatus_gfxSyncResetStatus             17
    #define   LSb16Gbl_ResetStatus_gfxSyncResetStatus             1
    #define       bGbl_ResetStatus_gfxSyncResetStatus          1
    #define   MSK32Gbl_ResetStatus_gfxSyncResetStatus             0x00020000
    #define        Gbl_ResetStatus_gfxSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_gfxSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_zspSyncResetStatus          0x00BA
    #define     B16Gbl_ResetStatus_zspSyncResetStatus          0x00BA
    #define   LSb32Gbl_ResetStatus_zspSyncResetStatus             18
    #define   LSb16Gbl_ResetStatus_zspSyncResetStatus             2
    #define       bGbl_ResetStatus_zspSyncResetStatus          1
    #define   MSK32Gbl_ResetStatus_zspSyncResetStatus             0x00040000
    #define        Gbl_ResetStatus_zspSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_zspSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_avioSyncResetStatus         0x00BA
    #define     B16Gbl_ResetStatus_avioSyncResetStatus         0x00BA
    #define   LSb32Gbl_ResetStatus_avioSyncResetStatus            19
    #define   LSb16Gbl_ResetStatus_avioSyncResetStatus            3
    #define       bGbl_ResetStatus_avioSyncResetStatus         1
    #define   MSK32Gbl_ResetStatus_avioSyncResetStatus            0x00080000
    #define        Gbl_ResetStatus_avioSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_avioSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_perifSyncResetStatus        0x00BA
    #define     B16Gbl_ResetStatus_perifSyncResetStatus        0x00BA
    #define   LSb32Gbl_ResetStatus_perifSyncResetStatus           20
    #define   LSb16Gbl_ResetStatus_perifSyncResetStatus           4
    #define       bGbl_ResetStatus_perifSyncResetStatus        1
    #define   MSK32Gbl_ResetStatus_perifSyncResetStatus           0x00100000
    #define        Gbl_ResetStatus_perifSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_perifSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_gethSyncResetStatus         0x00BA
    #define     B16Gbl_ResetStatus_gethSyncResetStatus         0x00BA
    #define   LSb32Gbl_ResetStatus_gethSyncResetStatus            21
    #define   LSb16Gbl_ResetStatus_gethSyncResetStatus            5
    #define       bGbl_ResetStatus_gethSyncResetStatus         1
    #define   MSK32Gbl_ResetStatus_gethSyncResetStatus            0x00200000
    #define        Gbl_ResetStatus_gethSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_gethSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_sataSyncResetStatus         0x00BA
    #define     B16Gbl_ResetStatus_sataSyncResetStatus         0x00BA
    #define   LSb32Gbl_ResetStatus_sataSyncResetStatus            22
    #define   LSb16Gbl_ResetStatus_sataSyncResetStatus            6
    #define       bGbl_ResetStatus_sataSyncResetStatus         1
    #define   MSK32Gbl_ResetStatus_sataSyncResetStatus            0x00400000
    #define        Gbl_ResetStatus_sataSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_sataSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_ahbApbSyncResetStatus       0x00BA
    #define     B16Gbl_ResetStatus_ahbApbSyncResetStatus       0x00BA
    #define   LSb32Gbl_ResetStatus_ahbApbSyncResetStatus          23
    #define   LSb16Gbl_ResetStatus_ahbApbSyncResetStatus          7
    #define       bGbl_ResetStatus_ahbApbSyncResetStatus       1
    #define   MSK32Gbl_ResetStatus_ahbApbSyncResetStatus          0x00800000
    #define        Gbl_ResetStatus_ahbApbSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_ahbApbSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_nanfSyncResetStatus         0x00BB
    #define     B16Gbl_ResetStatus_nanfSyncResetStatus         0x00BA
    #define   LSb32Gbl_ResetStatus_nanfSyncResetStatus            24
    #define   LSb16Gbl_ResetStatus_nanfSyncResetStatus            8
    #define       bGbl_ResetStatus_nanfSyncResetStatus         1
    #define   MSK32Gbl_ResetStatus_nanfSyncResetStatus            0x01000000
    #define        Gbl_ResetStatus_nanfSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_nanfSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_smemcSyncResetStatus        0x00BB
    #define     B16Gbl_ResetStatus_smemcSyncResetStatus        0x00BA
    #define   LSb32Gbl_ResetStatus_smemcSyncResetStatus           25
    #define   LSb16Gbl_ResetStatus_smemcSyncResetStatus           9
    #define       bGbl_ResetStatus_smemcSyncResetStatus        1
    #define   MSK32Gbl_ResetStatus_smemcSyncResetStatus           0x02000000
    #define        Gbl_ResetStatus_smemcSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_smemcSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_drmSyncResetStatus          0x00BB
    #define     B16Gbl_ResetStatus_drmSyncResetStatus          0x00BA
    #define   LSb32Gbl_ResetStatus_drmSyncResetStatus             26
    #define   LSb16Gbl_ResetStatus_drmSyncResetStatus             10
    #define       bGbl_ResetStatus_drmSyncResetStatus          1
    #define   MSK32Gbl_ResetStatus_drmSyncResetStatus             0x04000000
    #define        Gbl_ResetStatus_drmSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_drmSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_usb0SyncResetStatus         0x00BB
    #define     B16Gbl_ResetStatus_usb0SyncResetStatus         0x00BA
    #define   LSb32Gbl_ResetStatus_usb0SyncResetStatus            27
    #define   LSb16Gbl_ResetStatus_usb0SyncResetStatus            11
    #define       bGbl_ResetStatus_usb0SyncResetStatus         1
    #define   MSK32Gbl_ResetStatus_usb0SyncResetStatus            0x08000000
    #define        Gbl_ResetStatus_usb0SyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_usb0SyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_usb1SyncResetStatus         0x00BB
    #define     B16Gbl_ResetStatus_usb1SyncResetStatus         0x00BA
    #define   LSb32Gbl_ResetStatus_usb1SyncResetStatus            28
    #define   LSb16Gbl_ResetStatus_usb1SyncResetStatus            12
    #define       bGbl_ResetStatus_usb1SyncResetStatus         1
    #define   MSK32Gbl_ResetStatus_usb1SyncResetStatus            0x10000000
    #define        Gbl_ResetStatus_usb1SyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_usb1SyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_pBridgeSyncResetStatus      0x00BB
    #define     B16Gbl_ResetStatus_pBridgeSyncResetStatus      0x00BA
    #define   LSb32Gbl_ResetStatus_pBridgeSyncResetStatus         29
    #define   LSb16Gbl_ResetStatus_pBridgeSyncResetStatus         13
    #define       bGbl_ResetStatus_pBridgeSyncResetStatus      1
    #define   MSK32Gbl_ResetStatus_pBridgeSyncResetStatus         0x20000000
    #define        Gbl_ResetStatus_pBridgeSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_pBridgeSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_vppSyncResetStatus          0x00BB
    #define     B16Gbl_ResetStatus_vppSyncResetStatus          0x00BA
    #define   LSb32Gbl_ResetStatus_vppSyncResetStatus             30
    #define   LSb16Gbl_ResetStatus_vppSyncResetStatus             14
    #define       bGbl_ResetStatus_vppSyncResetStatus          1
    #define   MSK32Gbl_ResetStatus_vppSyncResetStatus             0x40000000
    #define        Gbl_ResetStatus_vppSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_vppSyncResetStatus_deasserted              0x0

    #define     BA_Gbl_ResetStatus_appSyncResetStatus          0x00BB
    #define     B16Gbl_ResetStatus_appSyncResetStatus          0x00BA
    #define   LSb32Gbl_ResetStatus_appSyncResetStatus             31
    #define   LSb16Gbl_ResetStatus_appSyncResetStatus             15
    #define       bGbl_ResetStatus_appSyncResetStatus          1
    #define   MSK32Gbl_ResetStatus_appSyncResetStatus             0x80000000
    #define        Gbl_ResetStatus_appSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_appSyncResetStatus_deasserted              0x0

    #define     RA_Gbl_ResetStatus1                            0x00BC

    #define     BA_Gbl_ResetStatus_sdioSyncResetStatus         0x00BC
    #define     B16Gbl_ResetStatus_sdioSyncResetStatus         0x00BC
    #define   LSb32Gbl_ResetStatus_sdioSyncResetStatus            0
    #define   LSb16Gbl_ResetStatus_sdioSyncResetStatus            0
    #define       bGbl_ResetStatus_sdioSyncResetStatus         1
    #define   MSK32Gbl_ResetStatus_sdioSyncResetStatus            0x00000001
    #define        Gbl_ResetStatus_sdioSyncResetStatus_asserted              0x1
    #define        Gbl_ResetStatus_sdioSyncResetStatus_deasserted              0x0
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_sw_generic0                             0x00C0

    #define     BA_Gbl_sw_generic0_swReg0                      0x00C0
    #define     B16Gbl_sw_generic0_swReg0                      0x00C0
    #define   LSb32Gbl_sw_generic0_swReg0                         0
    #define   LSb16Gbl_sw_generic0_swReg0                         0
    #define       bGbl_sw_generic0_swReg0                      32
    #define   MSK32Gbl_sw_generic0_swReg0                         0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_sw_generic1                             0x00C4

    #define     BA_Gbl_sw_generic1_swReg1                      0x00C4
    #define     B16Gbl_sw_generic1_swReg1                      0x00C4
    #define   LSb32Gbl_sw_generic1_swReg1                         0
    #define   LSb16Gbl_sw_generic1_swReg1                         0
    #define       bGbl_sw_generic1_swReg1                      32
    #define   MSK32Gbl_sw_generic1_swReg1                         0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_sw_generic2                             0x00C8

    #define     BA_Gbl_sw_generic2_swReg2                      0x00C8
    #define     B16Gbl_sw_generic2_swReg2                      0x00C8
    #define   LSb32Gbl_sw_generic2_swReg2                         0
    #define   LSb16Gbl_sw_generic2_swReg2                         0
    #define       bGbl_sw_generic2_swReg2                      32
    #define   MSK32Gbl_sw_generic2_swReg2                         0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_sw_generic3                             0x00CC

    #define     BA_Gbl_sw_generic3_swReg3                      0x00CC
    #define     B16Gbl_sw_generic3_swReg3                      0x00CC
    #define   LSb32Gbl_sw_generic3_swReg3                         0
    #define   LSb16Gbl_sw_generic3_swReg3                         0
    #define       bGbl_sw_generic3_swReg3                      32
    #define   MSK32Gbl_sw_generic3_swReg3                         0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ChipCntl0                               0x00D0

    #define     BA_Gbl_ChipCntl0_dCacheCfgCPU0                 0x00D0
    #define     B16Gbl_ChipCntl0_dCacheCfgCPU0                 0x00D0
    #define   LSb32Gbl_ChipCntl0_dCacheCfgCPU0                    0
    #define   LSb16Gbl_ChipCntl0_dCacheCfgCPU0                    0
    #define       bGbl_ChipCntl0_dCacheCfgCPU0                 2
    #define   MSK32Gbl_ChipCntl0_dCacheCfgCPU0                    0x00000003

    #define     BA_Gbl_ChipCntl0_iCacheCfgCPU0                 0x00D0
    #define     B16Gbl_ChipCntl0_iCacheCfgCPU0                 0x00D0
    #define   LSb32Gbl_ChipCntl0_iCacheCfgCPU0                    2
    #define   LSb16Gbl_ChipCntl0_iCacheCfgCPU0                    2
    #define       bGbl_ChipCntl0_iCacheCfgCPU0                 2
    #define   MSK32Gbl_ChipCntl0_iCacheCfgCPU0                    0x0000000C

    #define     BA_Gbl_ChipCntl0_dCacheCfgCPU1                 0x00D0
    #define     B16Gbl_ChipCntl0_dCacheCfgCPU1                 0x00D0
    #define   LSb32Gbl_ChipCntl0_dCacheCfgCPU1                    4
    #define   LSb16Gbl_ChipCntl0_dCacheCfgCPU1                    4
    #define       bGbl_ChipCntl0_dCacheCfgCPU1                 2
    #define   MSK32Gbl_ChipCntl0_dCacheCfgCPU1                    0x00000030

    #define     BA_Gbl_ChipCntl0_iCacheCfgCPU1                 0x00D0
    #define     B16Gbl_ChipCntl0_iCacheCfgCPU1                 0x00D0
    #define   LSb32Gbl_ChipCntl0_iCacheCfgCPU1                    6
    #define   LSb16Gbl_ChipCntl0_iCacheCfgCPU1                    6
    #define       bGbl_ChipCntl0_iCacheCfgCPU1                 2
    #define   MSK32Gbl_ChipCntl0_iCacheCfgCPU1                    0x000000C0

    #define     BA_Gbl_ChipCntl0_cg_bpass0Cpu0                 0x00D1
    #define     B16Gbl_ChipCntl0_cg_bpass0Cpu0                 0x00D0
    #define   LSb32Gbl_ChipCntl0_cg_bpass0Cpu0                    8
    #define   LSb16Gbl_ChipCntl0_cg_bpass0Cpu0                    8
    #define       bGbl_ChipCntl0_cg_bpass0Cpu0                 1
    #define   MSK32Gbl_ChipCntl0_cg_bpass0Cpu0                    0x00000100

    #define     BA_Gbl_ChipCntl0_cg_bpass0Cpu1                 0x00D1
    #define     B16Gbl_ChipCntl0_cg_bpass0Cpu1                 0x00D0
    #define   LSb32Gbl_ChipCntl0_cg_bpass0Cpu1                    9
    #define   LSb16Gbl_ChipCntl0_cg_bpass0Cpu1                    9
    #define       bGbl_ChipCntl0_cg_bpass0Cpu1                 1
    #define   MSK32Gbl_ChipCntl0_cg_bpass0Cpu1                    0x00000200

    #define     BA_Gbl_ChipCntl0_wfiJtagClkEn                  0x00D1
    #define     B16Gbl_ChipCntl0_wfiJtagClkEn                  0x00D0
    #define   LSb32Gbl_ChipCntl0_wfiJtagClkEn                     10
    #define   LSb16Gbl_ChipCntl0_wfiJtagClkEn                     10
    #define       bGbl_ChipCntl0_wfiJtagClkEn                  1
    #define   MSK32Gbl_ChipCntl0_wfiJtagClkEn                     0x00000400
    #define        Gbl_ChipCntl0_wfiJtagClkEn_RUNNING                       0x1
    #define        Gbl_ChipCntl0_wfiJtagClkEn_FREEZE                        0x0
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ChipCntl1                               0x00D4

    #define     BA_Gbl_ChipCntl1_rtcIcDataCpu0                 0x00D4
    #define     B16Gbl_ChipCntl1_rtcIcDataCpu0                 0x00D4
    #define   LSb32Gbl_ChipCntl1_rtcIcDataCpu0                    0
    #define   LSb16Gbl_ChipCntl1_rtcIcDataCpu0                    0
    #define       bGbl_ChipCntl1_rtcIcDataCpu0                 3
    #define   MSK32Gbl_ChipCntl1_rtcIcDataCpu0                    0x00000007

    #define     BA_Gbl_ChipCntl1_rtcIcTagCpu0                  0x00D4
    #define     B16Gbl_ChipCntl1_rtcIcTagCpu0                  0x00D4
    #define   LSb32Gbl_ChipCntl1_rtcIcTagCpu0                     3
    #define   LSb16Gbl_ChipCntl1_rtcIcTagCpu0                     3
    #define       bGbl_ChipCntl1_rtcIcTagCpu0                  3
    #define   MSK32Gbl_ChipCntl1_rtcIcTagCpu0                     0x00000038

    #define     BA_Gbl_ChipCntl1_rtcDcDataCpu0                 0x00D4
    #define     B16Gbl_ChipCntl1_rtcDcDataCpu0                 0x00D4
    #define   LSb32Gbl_ChipCntl1_rtcDcDataCpu0                    6
    #define   LSb16Gbl_ChipCntl1_rtcDcDataCpu0                    6
    #define       bGbl_ChipCntl1_rtcDcDataCpu0                 3
    #define   MSK32Gbl_ChipCntl1_rtcDcDataCpu0                    0x000001C0

    #define     BA_Gbl_ChipCntl1_rtcDcTagCpu0                  0x00D5
    #define     B16Gbl_ChipCntl1_rtcDcTagCpu0                  0x00D4
    #define   LSb32Gbl_ChipCntl1_rtcDcTagCpu0                     9
    #define   LSb16Gbl_ChipCntl1_rtcDcTagCpu0                     9
    #define       bGbl_ChipCntl1_rtcDcTagCpu0                  2
    #define   MSK32Gbl_ChipCntl1_rtcDcTagCpu0                     0x00000600

    #define     BA_Gbl_ChipCntl1_wtcDcTagCpu0                  0x00D5
    #define     B16Gbl_ChipCntl1_wtcDcTagCpu0                  0x00D4
    #define   LSb32Gbl_ChipCntl1_wtcDcTagCpu0                     11
    #define   LSb16Gbl_ChipCntl1_wtcDcTagCpu0                     11
    #define       bGbl_ChipCntl1_wtcDcTagCpu0                  3
    #define   MSK32Gbl_ChipCntl1_wtcDcTagCpu0                     0x00003800

    #define     BA_Gbl_ChipCntl1_rtcDcDirtyCpu0                0x00D5
    #define     B16Gbl_ChipCntl1_rtcDcDirtyCpu0                0x00D4
    #define   LSb32Gbl_ChipCntl1_rtcDcDirtyCpu0                   14
    #define   LSb16Gbl_ChipCntl1_rtcDcDirtyCpu0                   14
    #define       bGbl_ChipCntl1_rtcDcDirtyCpu0                2
    #define   MSK32Gbl_ChipCntl1_rtcDcDirtyCpu0                   0x0000C000

    #define     BA_Gbl_ChipCntl1_wtcDcDirtyCpu0                0x00D6
    #define     B16Gbl_ChipCntl1_wtcDcDirtyCpu0                0x00D6
    #define   LSb32Gbl_ChipCntl1_wtcDcDirtyCpu0                   16
    #define   LSb16Gbl_ChipCntl1_wtcDcDirtyCpu0                   0
    #define       bGbl_ChipCntl1_wtcDcDirtyCpu0                3
    #define   MSK32Gbl_ChipCntl1_wtcDcDirtyCpu0                   0x00070000

    #define     BA_Gbl_ChipCntl1_rtcBpmCpu0                    0x00D6
    #define     B16Gbl_ChipCntl1_rtcBpmCpu0                    0x00D6
    #define   LSb32Gbl_ChipCntl1_rtcBpmCpu0                       19
    #define   LSb16Gbl_ChipCntl1_rtcBpmCpu0                       3
    #define       bGbl_ChipCntl1_rtcBpmCpu0                    2
    #define   MSK32Gbl_ChipCntl1_rtcBpmCpu0                       0x00180000

    #define     BA_Gbl_ChipCntl1_wtcBpmCpu0                    0x00D6
    #define     B16Gbl_ChipCntl1_wtcBpmCpu0                    0x00D6
    #define   LSb32Gbl_ChipCntl1_wtcBpmCpu0                       21
    #define   LSb16Gbl_ChipCntl1_wtcBpmCpu0                       5
    #define       bGbl_ChipCntl1_wtcBpmCpu0                    3
    #define   MSK32Gbl_ChipCntl1_wtcBpmCpu0                       0x00E00000

    #define     BA_Gbl_ChipCntl1_rtcTlbCpu0                    0x00D7
    #define     B16Gbl_ChipCntl1_rtcTlbCpu0                    0x00D6
    #define   LSb32Gbl_ChipCntl1_rtcTlbCpu0                       24
    #define   LSb16Gbl_ChipCntl1_rtcTlbCpu0                       8
    #define       bGbl_ChipCntl1_rtcTlbCpu0                    2
    #define   MSK32Gbl_ChipCntl1_rtcTlbCpu0                       0x03000000

    #define     BA_Gbl_ChipCntl1_wtcTlbCpu0                    0x00D7
    #define     B16Gbl_ChipCntl1_wtcTlbCpu0                    0x00D6
    #define   LSb32Gbl_ChipCntl1_wtcTlbCpu0                       26
    #define   LSb16Gbl_ChipCntl1_wtcTlbCpu0                       10
    #define       bGbl_ChipCntl1_wtcTlbCpu0                    3
    #define   MSK32Gbl_ChipCntl1_wtcTlbCpu0                       0x1C000000

    #define     BA_Gbl_ChipCntl1_rtcIcDataCpu1                 0x00D7
    #define     B16Gbl_ChipCntl1_rtcIcDataCpu1                 0x00D6
    #define   LSb32Gbl_ChipCntl1_rtcIcDataCpu1                    29
    #define   LSb16Gbl_ChipCntl1_rtcIcDataCpu1                    13
    #define       bGbl_ChipCntl1_rtcIcDataCpu1                 3
    #define   MSK32Gbl_ChipCntl1_rtcIcDataCpu1                    0xE0000000

    #define     RA_Gbl_ChipCntl11                              0x00D8

    #define     BA_Gbl_ChipCntl1_rtcIcTagCpu1                  0x00D8
    #define     B16Gbl_ChipCntl1_rtcIcTagCpu1                  0x00D8
    #define   LSb32Gbl_ChipCntl1_rtcIcTagCpu1                     0
    #define   LSb16Gbl_ChipCntl1_rtcIcTagCpu1                     0
    #define       bGbl_ChipCntl1_rtcIcTagCpu1                  3
    #define   MSK32Gbl_ChipCntl1_rtcIcTagCpu1                     0x00000007

    #define     BA_Gbl_ChipCntl1_rtcDcDataCpu1                 0x00D8
    #define     B16Gbl_ChipCntl1_rtcDcDataCpu1                 0x00D8
    #define   LSb32Gbl_ChipCntl1_rtcDcDataCpu1                    3
    #define   LSb16Gbl_ChipCntl1_rtcDcDataCpu1                    3
    #define       bGbl_ChipCntl1_rtcDcDataCpu1                 3
    #define   MSK32Gbl_ChipCntl1_rtcDcDataCpu1                    0x00000038

    #define     BA_Gbl_ChipCntl1_rtcDcTagCpu1                  0x00D8
    #define     B16Gbl_ChipCntl1_rtcDcTagCpu1                  0x00D8
    #define   LSb32Gbl_ChipCntl1_rtcDcTagCpu1                     6
    #define   LSb16Gbl_ChipCntl1_rtcDcTagCpu1                     6
    #define       bGbl_ChipCntl1_rtcDcTagCpu1                  2
    #define   MSK32Gbl_ChipCntl1_rtcDcTagCpu1                     0x000000C0

    #define     BA_Gbl_ChipCntl1_wtcDcTagCpu1                  0x00D9
    #define     B16Gbl_ChipCntl1_wtcDcTagCpu1                  0x00D8
    #define   LSb32Gbl_ChipCntl1_wtcDcTagCpu1                     8
    #define   LSb16Gbl_ChipCntl1_wtcDcTagCpu1                     8
    #define       bGbl_ChipCntl1_wtcDcTagCpu1                  3
    #define   MSK32Gbl_ChipCntl1_wtcDcTagCpu1                     0x00000700

    #define     BA_Gbl_ChipCntl1_rtcDcDirtyCpu1                0x00D9
    #define     B16Gbl_ChipCntl1_rtcDcDirtyCpu1                0x00D8
    #define   LSb32Gbl_ChipCntl1_rtcDcDirtyCpu1                   11
    #define   LSb16Gbl_ChipCntl1_rtcDcDirtyCpu1                   11
    #define       bGbl_ChipCntl1_rtcDcDirtyCpu1                2
    #define   MSK32Gbl_ChipCntl1_rtcDcDirtyCpu1                   0x00001800

    #define     BA_Gbl_ChipCntl1_wtcDcDirtyCpu1                0x00D9
    #define     B16Gbl_ChipCntl1_wtcDcDirtyCpu1                0x00D8
    #define   LSb32Gbl_ChipCntl1_wtcDcDirtyCpu1                   13
    #define   LSb16Gbl_ChipCntl1_wtcDcDirtyCpu1                   13
    #define       bGbl_ChipCntl1_wtcDcDirtyCpu1                3
    #define   MSK32Gbl_ChipCntl1_wtcDcDirtyCpu1                   0x0000E000

    #define     BA_Gbl_ChipCntl1_rtcBpmCpu1                    0x00DA
    #define     B16Gbl_ChipCntl1_rtcBpmCpu1                    0x00DA
    #define   LSb32Gbl_ChipCntl1_rtcBpmCpu1                       16
    #define   LSb16Gbl_ChipCntl1_rtcBpmCpu1                       0
    #define       bGbl_ChipCntl1_rtcBpmCpu1                    2
    #define   MSK32Gbl_ChipCntl1_rtcBpmCpu1                       0x00030000

    #define     BA_Gbl_ChipCntl1_wtcBpmCpu1                    0x00DA
    #define     B16Gbl_ChipCntl1_wtcBpmCpu1                    0x00DA
    #define   LSb32Gbl_ChipCntl1_wtcBpmCpu1                       18
    #define   LSb16Gbl_ChipCntl1_wtcBpmCpu1                       2
    #define       bGbl_ChipCntl1_wtcBpmCpu1                    3
    #define   MSK32Gbl_ChipCntl1_wtcBpmCpu1                       0x001C0000

    #define     BA_Gbl_ChipCntl1_rtcTlbCpu1                    0x00DA
    #define     B16Gbl_ChipCntl1_rtcTlbCpu1                    0x00DA
    #define   LSb32Gbl_ChipCntl1_rtcTlbCpu1                       21
    #define   LSb16Gbl_ChipCntl1_rtcTlbCpu1                       5
    #define       bGbl_ChipCntl1_rtcTlbCpu1                    2
    #define   MSK32Gbl_ChipCntl1_rtcTlbCpu1                       0x00600000

    #define     BA_Gbl_ChipCntl1_wtcTlbCpu1                    0x00DA
    #define     B16Gbl_ChipCntl1_wtcTlbCpu1                    0x00DA
    #define   LSb32Gbl_ChipCntl1_wtcTlbCpu1                       23
    #define   LSb16Gbl_ChipCntl1_wtcTlbCpu1                       7
    #define       bGbl_ChipCntl1_wtcTlbCpu1                    3
    #define   MSK32Gbl_ChipCntl1_wtcTlbCpu1                       0x03800000
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RWTC_avio31to0                          0x00DC

    #define     BA_Gbl_RWTC_avio31to0_value                    0x00DC
    #define     B16Gbl_RWTC_avio31to0_value                    0x00DC
    #define   LSb32Gbl_RWTC_avio31to0_value                       0
    #define   LSb16Gbl_RWTC_avio31to0_value                       0
    #define       bGbl_RWTC_avio31to0_value                    32
    #define   MSK32Gbl_RWTC_avio31to0_value                       0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RWTC_avio43to32                         0x00E0

    #define     BA_Gbl_RWTC_avio43to32_value                   0x00E0
    #define     B16Gbl_RWTC_avio43to32_value                   0x00E0
    #define   LSb32Gbl_RWTC_avio43to32_value                      0
    #define   LSb16Gbl_RWTC_avio43to32_value                      0
    #define       bGbl_RWTC_avio43to32_value                   12
    #define   MSK32Gbl_RWTC_avio43to32_value                      0x00000FFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RWTC_gfx2D31to0                         0x00E4

    #define     BA_Gbl_RWTC_gfx2D31to0_value                   0x00E4
    #define     B16Gbl_RWTC_gfx2D31to0_value                   0x00E4
    #define   LSb32Gbl_RWTC_gfx2D31to0_value                      0
    #define   LSb16Gbl_RWTC_gfx2D31to0_value                      0
    #define       bGbl_RWTC_gfx2D31to0_value                   32
    #define   MSK32Gbl_RWTC_gfx2D31to0_value                      0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RWTC_gfx2D43to32                        0x00E8

    #define     BA_Gbl_RWTC_gfx2D43to32_value                  0x00E8
    #define     B16Gbl_RWTC_gfx2D43to32_value                  0x00E8
    #define   LSb32Gbl_RWTC_gfx2D43to32_value                     0
    #define   LSb16Gbl_RWTC_gfx2D43to32_value                     0
    #define       bGbl_RWTC_gfx2D43to32_value                  12
    #define   MSK32Gbl_RWTC_gfx2D43to32_value                     0x00000FFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RWTC_perif31to0                         0x00EC

    #define     BA_Gbl_RWTC_perif31to0_value                   0x00EC
    #define     B16Gbl_RWTC_perif31to0_value                   0x00EC
    #define   LSb32Gbl_RWTC_perif31to0_value                      0
    #define   LSb16Gbl_RWTC_perif31to0_value                      0
    #define       bGbl_RWTC_perif31to0_value                   32
    #define   MSK32Gbl_RWTC_perif31to0_value                      0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RWTC_perif43to32                        0x00F0

    #define     BA_Gbl_RWTC_perif43to32_value                  0x00F0
    #define     B16Gbl_RWTC_perif43to32_value                  0x00F0
    #define   LSb32Gbl_RWTC_perif43to32_value                     0
    #define   LSb16Gbl_RWTC_perif43to32_value                     0
    #define       bGbl_RWTC_perif43to32_value                  12
    #define   MSK32Gbl_RWTC_perif43to32_value                     0x00000FFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RWTC_soc31to0                           0x00F4

    #define     BA_Gbl_RWTC_soc31to0_value                     0x00F4
    #define     B16Gbl_RWTC_soc31to0_value                     0x00F4
    #define   LSb32Gbl_RWTC_soc31to0_value                        0
    #define   LSb16Gbl_RWTC_soc31to0_value                        0
    #define       bGbl_RWTC_soc31to0_value                     32
    #define   MSK32Gbl_RWTC_soc31to0_value                        0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RWTC_soc43to32                          0x00F8

    #define     BA_Gbl_RWTC_soc43to32_value                    0x00F8
    #define     B16Gbl_RWTC_soc43to32_value                    0x00F8
    #define   LSb32Gbl_RWTC_soc43to32_value                       0
    #define   LSb16Gbl_RWTC_soc43to32_value                       0
    #define       bGbl_RWTC_soc43to32_value                    12
    #define   MSK32Gbl_RWTC_soc43to32_value                       0x00000FFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RWTC_vPro31to0                          0x00FC

    #define     BA_Gbl_RWTC_vPro31to0_value                    0x00FC
    #define     B16Gbl_RWTC_vPro31to0_value                    0x00FC
    #define   LSb32Gbl_RWTC_vPro31to0_value                       0
    #define   LSb16Gbl_RWTC_vPro31to0_value                       0
    #define       bGbl_RWTC_vPro31to0_value                    32
    #define   MSK32Gbl_RWTC_vPro31to0_value                       0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RWTC_vPro43to32                         0x0100

    #define     BA_Gbl_RWTC_vPro43to32_value                   0x0100
    #define     B16Gbl_RWTC_vPro43to32_value                   0x0100
    #define   LSb32Gbl_RWTC_vPro43to32_value                      0
    #define   LSb16Gbl_RWTC_vPro43to32_value                      0
    #define       bGbl_RWTC_vPro43to32_value                   12
    #define   MSK32Gbl_RWTC_vPro43to32_value                      0x00000FFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ProductId                               0x0104

    #define     BA_Gbl_ProductId_Id                            0x0104
    #define     B16Gbl_ProductId_Id                            0x0104
    #define   LSb32Gbl_ProductId_Id                               0
    #define   LSb16Gbl_ProductId_Id                               0
    #define       bGbl_ProductId_Id                            32
    #define   MSK32Gbl_ProductId_Id                               0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ProductId_ext                           0x0108

    #define     BA_Gbl_ProductId_ext_ID_EXT                    0x0108
    #define     B16Gbl_ProductId_ext_ID_EXT                    0x0108
    #define   LSb32Gbl_ProductId_ext_ID_EXT                       0
    #define   LSb16Gbl_ProductId_ext_ID_EXT                       0
    #define       bGbl_ProductId_ext_ID_EXT                    8
    #define   MSK32Gbl_ProductId_ext_ID_EXT                       0x000000FF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_FPGAR                                   0x010C

    #define     BA_Gbl_FPGAR_FPGAR                             0x010C
    #define     B16Gbl_FPGAR_FPGAR                             0x010C
    #define   LSb32Gbl_FPGAR_FPGAR                                0
    #define   LSb16Gbl_FPGAR_FPGAR                                0
    #define       bGbl_FPGAR_FPGAR                             32
    #define   MSK32Gbl_FPGAR_FPGAR                                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_FPGARW                                  0x0110

    #define     BA_Gbl_FPGARW_FPGARW                           0x0110
    #define     B16Gbl_FPGARW_FPGARW                           0x0110
    #define   LSb32Gbl_FPGARW_FPGARW                              0
    #define   LSb16Gbl_FPGARW_FPGARW                              0
    #define       bGbl_FPGARW_FPGARW                           32
    #define   MSK32Gbl_FPGARW_FPGARW                              0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RingOscCtl                              0x0114

    #define     BA_Gbl_RingOscCtl_centerInit                   0x0114
    #define     B16Gbl_RingOscCtl_centerInit                   0x0114
    #define   LSb32Gbl_RingOscCtl_centerInit                      0
    #define   LSb16Gbl_RingOscCtl_centerInit                      0
    #define       bGbl_RingOscCtl_centerInit                   1
    #define   MSK32Gbl_RingOscCtl_centerInit                      0x00000001
    #define        Gbl_RingOscCtl_centerInit_OFF                            0x0
    #define        Gbl_RingOscCtl_centerInit_ON                             0x1

    #define     BA_Gbl_RingOscCtl_avioTopInit                  0x0114
    #define     B16Gbl_RingOscCtl_avioTopInit                  0x0114
    #define   LSb32Gbl_RingOscCtl_avioTopInit                     1
    #define   LSb16Gbl_RingOscCtl_avioTopInit                     1
    #define       bGbl_RingOscCtl_avioTopInit                  1
    #define   MSK32Gbl_RingOscCtl_avioTopInit                     0x00000002
    #define        Gbl_RingOscCtl_avioTopInit_OFF                           0x0
    #define        Gbl_RingOscCtl_avioTopInit_ON                            0x1

    #define     BA_Gbl_RingOscCtl_avioBotInit                  0x0114
    #define     B16Gbl_RingOscCtl_avioBotInit                  0x0114
    #define   LSb32Gbl_RingOscCtl_avioBotInit                     2
    #define   LSb16Gbl_RingOscCtl_avioBotInit                     2
    #define       bGbl_RingOscCtl_avioBotInit                  1
    #define   MSK32Gbl_RingOscCtl_avioBotInit                     0x00000004
    #define        Gbl_RingOscCtl_avioBotInit_OFF                           0x0
    #define        Gbl_RingOscCtl_avioBotInit_ON                            0x1

    #define     BA_Gbl_RingOscCtl_usbTopInit                   0x0114
    #define     B16Gbl_RingOscCtl_usbTopInit                   0x0114
    #define   LSb32Gbl_RingOscCtl_usbTopInit                      3
    #define   LSb16Gbl_RingOscCtl_usbTopInit                      3
    #define       bGbl_RingOscCtl_usbTopInit                   1
    #define   MSK32Gbl_RingOscCtl_usbTopInit                      0x00000008
    #define        Gbl_RingOscCtl_usbTopInit_OFF                            0x0
    #define        Gbl_RingOscCtl_usbTopInit_ON                             0x1

    #define     BA_Gbl_RingOscCtl_cpu1TopInit                  0x0114
    #define     B16Gbl_RingOscCtl_cpu1TopInit                  0x0114
    #define   LSb32Gbl_RingOscCtl_cpu1TopInit                     4
    #define   LSb16Gbl_RingOscCtl_cpu1TopInit                     4
    #define       bGbl_RingOscCtl_cpu1TopInit                  1
    #define   MSK32Gbl_RingOscCtl_cpu1TopInit                     0x00000010
    #define        Gbl_RingOscCtl_cpu1TopInit_OFF                           0x0
    #define        Gbl_RingOscCtl_cpu1TopInit_ON                            0x1

    #define     BA_Gbl_RingOscCtl_cpu0TopInit                  0x0114
    #define     B16Gbl_RingOscCtl_cpu0TopInit                  0x0114
    #define   LSb32Gbl_RingOscCtl_cpu0TopInit                     5
    #define   LSb16Gbl_RingOscCtl_cpu0TopInit                     5
    #define       bGbl_RingOscCtl_cpu0TopInit                  1
    #define   MSK32Gbl_RingOscCtl_cpu0TopInit                     0x00000020
    #define        Gbl_RingOscCtl_cpu0TopInit_OFF                           0x0
    #define        Gbl_RingOscCtl_cpu0TopInit_ON                            0x1

    #define     BA_Gbl_RingOscCtl_cpuBotInit                   0x0114
    #define     B16Gbl_RingOscCtl_cpuBotInit                   0x0114
    #define   LSb32Gbl_RingOscCtl_cpuBotInit                      6
    #define   LSb16Gbl_RingOscCtl_cpuBotInit                      6
    #define       bGbl_RingOscCtl_cpuBotInit                   1
    #define   MSK32Gbl_RingOscCtl_cpuBotInit                      0x00000040
    #define        Gbl_RingOscCtl_cpuBotInit_OFF                            0x0
    #define        Gbl_RingOscCtl_cpuBotInit_ON                             0x1

    #define     BA_Gbl_RingOscCtl_topRiteInit                  0x0114
    #define     B16Gbl_RingOscCtl_topRiteInit                  0x0114
    #define   LSb32Gbl_RingOscCtl_topRiteInit                     7
    #define   LSb16Gbl_RingOscCtl_topRiteInit                     7
    #define       bGbl_RingOscCtl_topRiteInit                  1
    #define   MSK32Gbl_RingOscCtl_topRiteInit                     0x00000080
    #define        Gbl_RingOscCtl_topRiteInit_OFF                           0x0
    #define        Gbl_RingOscCtl_topRiteInit_ON                            0x1

    #define     BA_Gbl_RingOscCtl_midRiteInit                  0x0115
    #define     B16Gbl_RingOscCtl_midRiteInit                  0x0114
    #define   LSb32Gbl_RingOscCtl_midRiteInit                     8
    #define   LSb16Gbl_RingOscCtl_midRiteInit                     8
    #define       bGbl_RingOscCtl_midRiteInit                  1
    #define   MSK32Gbl_RingOscCtl_midRiteInit                     0x00000100
    #define        Gbl_RingOscCtl_midRiteInit_OFF                           0x0
    #define        Gbl_RingOscCtl_midRiteInit_ON                            0x1

    #define     BA_Gbl_RingOscCtl_botRiteInit                  0x0115
    #define     B16Gbl_RingOscCtl_botRiteInit                  0x0114
    #define   LSb32Gbl_RingOscCtl_botRiteInit                     9
    #define   LSb16Gbl_RingOscCtl_botRiteInit                     9
    #define       bGbl_RingOscCtl_botRiteInit                  1
    #define   MSK32Gbl_RingOscCtl_botRiteInit                     0x00000200
    #define        Gbl_RingOscCtl_botRiteInit_OFF                           0x0
    #define        Gbl_RingOscCtl_botRiteInit_ON                            0x1
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_PLLBypsBootStrapEn                      0x0118

    #define     BA_Gbl_PLLBypsBootStrapEn_sysPLLBypsEn         0x0118
    #define     B16Gbl_PLLBypsBootStrapEn_sysPLLBypsEn         0x0118
    #define   LSb32Gbl_PLLBypsBootStrapEn_sysPLLBypsEn            0
    #define   LSb16Gbl_PLLBypsBootStrapEn_sysPLLBypsEn            0
    #define       bGbl_PLLBypsBootStrapEn_sysPLLBypsEn         1
    #define   MSK32Gbl_PLLBypsBootStrapEn_sysPLLBypsEn            0x00000001
    #define        Gbl_PLLBypsBootStrapEn_sysPLLBypsEn_Disable              0x0
    #define        Gbl_PLLBypsBootStrapEn_sysPLLBypsEn_Enable               0x1

    #define     BA_Gbl_PLLBypsBootStrapEn_memPLLBypsEn         0x0118
    #define     B16Gbl_PLLBypsBootStrapEn_memPLLBypsEn         0x0118
    #define   LSb32Gbl_PLLBypsBootStrapEn_memPLLBypsEn            1
    #define   LSb16Gbl_PLLBypsBootStrapEn_memPLLBypsEn            1
    #define       bGbl_PLLBypsBootStrapEn_memPLLBypsEn         1
    #define   MSK32Gbl_PLLBypsBootStrapEn_memPLLBypsEn            0x00000002
    #define        Gbl_PLLBypsBootStrapEn_memPLLBypsEn_Disable              0x0
    #define        Gbl_PLLBypsBootStrapEn_memPLLBypsEn_Enable               0x1

    #define     BA_Gbl_PLLBypsBootStrapEn_cpu1PLLBypsEn        0x0118
    #define     B16Gbl_PLLBypsBootStrapEn_cpu1PLLBypsEn        0x0118
    #define   LSb32Gbl_PLLBypsBootStrapEn_cpu1PLLBypsEn           2
    #define   LSb16Gbl_PLLBypsBootStrapEn_cpu1PLLBypsEn           2
    #define       bGbl_PLLBypsBootStrapEn_cpu1PLLBypsEn        1
    #define   MSK32Gbl_PLLBypsBootStrapEn_cpu1PLLBypsEn           0x00000004
    #define        Gbl_PLLBypsBootStrapEn_cpu1PLLBypsEn_Disable              0x0
    #define        Gbl_PLLBypsBootStrapEn_cpu1PLLBypsEn_Enable              0x1
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_IO_CALIBRATE_IN                         0x011C

    #define     BA_Gbl_IO_CALIBRATE_IN_DO                      0x011C
    #define     B16Gbl_IO_CALIBRATE_IN_DO                      0x011C
    #define   LSb32Gbl_IO_CALIBRATE_IN_DO                         0
    #define   LSb16Gbl_IO_CALIBRATE_IN_DO                         0
    #define       bGbl_IO_CALIBRATE_IN_DO                      1
    #define   MSK32Gbl_IO_CALIBRATE_IN_DO                         0x00000001
    #define        Gbl_IO_CALIBRATE_IN_DO_NMOS                              0x0
    #define        Gbl_IO_CALIBRATE_IN_DO_PMOS                              0x1

    #define     BA_Gbl_IO_CALIBRATE_IN_CAL_EN                  0x011C
    #define     B16Gbl_IO_CALIBRATE_IN_CAL_EN                  0x011C
    #define   LSb32Gbl_IO_CALIBRATE_IN_CAL_EN                     1
    #define   LSb16Gbl_IO_CALIBRATE_IN_CAL_EN                     1
    #define       bGbl_IO_CALIBRATE_IN_CAL_EN                  1
    #define   MSK32Gbl_IO_CALIBRATE_IN_CAL_EN                     0x00000002
    #define        Gbl_IO_CALIBRATE_IN_CAL_EN_DISABLE                       0x0
    #define        Gbl_IO_CALIBRATE_IN_CAL_EN_ENABLE                        0x1

    #define     BA_Gbl_IO_CALIBRATE_IN_ZP_CAL                  0x011C
    #define     B16Gbl_IO_CALIBRATE_IN_ZP_CAL                  0x011C
    #define   LSb32Gbl_IO_CALIBRATE_IN_ZP_CAL                     2
    #define   LSb16Gbl_IO_CALIBRATE_IN_ZP_CAL                     2
    #define       bGbl_IO_CALIBRATE_IN_ZP_CAL                  3
    #define   MSK32Gbl_IO_CALIBRATE_IN_ZP_CAL                     0x0000001C

    #define     BA_Gbl_IO_CALIBRATE_IN_ZN_CAL                  0x011C
    #define     B16Gbl_IO_CALIBRATE_IN_ZN_CAL                  0x011C
    #define   LSb32Gbl_IO_CALIBRATE_IN_ZN_CAL                     5
    #define   LSb16Gbl_IO_CALIBRATE_IN_ZN_CAL                     5
    #define       bGbl_IO_CALIBRATE_IN_ZN_CAL                  3
    #define   MSK32Gbl_IO_CALIBRATE_IN_ZN_CAL                     0x000000E0

    #define     BA_Gbl_IO_CALIBRATE_IN_RON_ADJ                 0x011D
    #define     B16Gbl_IO_CALIBRATE_IN_RON_ADJ                 0x011C
    #define   LSb32Gbl_IO_CALIBRATE_IN_RON_ADJ                    8
    #define   LSb16Gbl_IO_CALIBRATE_IN_RON_ADJ                    8
    #define       bGbl_IO_CALIBRATE_IN_RON_ADJ                 3
    #define   MSK32Gbl_IO_CALIBRATE_IN_RON_ADJ                    0x00000700
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_IO_CALBRATE_OUT                         0x0120

    #define     BA_Gbl_IO_CALBRATE_OUT_INC                     0x0120
    #define     B16Gbl_IO_CALBRATE_OUT_INC                     0x0120
    #define   LSb32Gbl_IO_CALBRATE_OUT_INC                        0
    #define   LSb16Gbl_IO_CALBRATE_OUT_INC                        0
    #define       bGbl_IO_CALBRATE_OUT_INC                     1
    #define   MSK32Gbl_IO_CALBRATE_OUT_INC                        0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_SECURE_SCAN_EN                          0x0124

    #define     BA_Gbl_SECURE_SCAN_EN_SET                      0x0124
    #define     B16Gbl_SECURE_SCAN_EN_SET                      0x0124
    #define   LSb32Gbl_SECURE_SCAN_EN_SET                         0
    #define   LSb16Gbl_SECURE_SCAN_EN_SET                         0
    #define       bGbl_SECURE_SCAN_EN_SET                      1
    #define   MSK32Gbl_SECURE_SCAN_EN_SET                         0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NandWPn                                 0x0128

    #define     BA_Gbl_NandWPn_Sel                             0x0128
    #define     B16Gbl_NandWPn_Sel                             0x0128
    #define   LSb32Gbl_NandWPn_Sel                                0
    #define   LSb16Gbl_NandWPn_Sel                                0
    #define       bGbl_NandWPn_Sel                             1
    #define   MSK32Gbl_NandWPn_Sel                                0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RingCntCntl                             0x012C

    #define     BA_Gbl_RingCntCntl_centerStart                 0x012C
    #define     B16Gbl_RingCntCntl_centerStart                 0x012C
    #define   LSb32Gbl_RingCntCntl_centerStart                    0
    #define   LSb16Gbl_RingCntCntl_centerStart                    0
    #define       bGbl_RingCntCntl_centerStart                 1
    #define   MSK32Gbl_RingCntCntl_centerStart                    0x00000001

    #define     BA_Gbl_RingCntCntl_centerStop                  0x012C
    #define     B16Gbl_RingCntCntl_centerStop                  0x012C
    #define   LSb32Gbl_RingCntCntl_centerStop                     1
    #define   LSb16Gbl_RingCntCntl_centerStop                     1
    #define       bGbl_RingCntCntl_centerStop                  1
    #define   MSK32Gbl_RingCntCntl_centerStop                     0x00000002

    #define     BA_Gbl_RingCntCntl_avioTopStart                0x012C
    #define     B16Gbl_RingCntCntl_avioTopStart                0x012C
    #define   LSb32Gbl_RingCntCntl_avioTopStart                   2
    #define   LSb16Gbl_RingCntCntl_avioTopStart                   2
    #define       bGbl_RingCntCntl_avioTopStart                1
    #define   MSK32Gbl_RingCntCntl_avioTopStart                   0x00000004

    #define     BA_Gbl_RingCntCntl_avioTopStop                 0x012C
    #define     B16Gbl_RingCntCntl_avioTopStop                 0x012C
    #define   LSb32Gbl_RingCntCntl_avioTopStop                    3
    #define   LSb16Gbl_RingCntCntl_avioTopStop                    3
    #define       bGbl_RingCntCntl_avioTopStop                 1
    #define   MSK32Gbl_RingCntCntl_avioTopStop                    0x00000008

    #define     BA_Gbl_RingCntCntl_avioBotStart                0x012C
    #define     B16Gbl_RingCntCntl_avioBotStart                0x012C
    #define   LSb32Gbl_RingCntCntl_avioBotStart                   4
    #define   LSb16Gbl_RingCntCntl_avioBotStart                   4
    #define       bGbl_RingCntCntl_avioBotStart                1
    #define   MSK32Gbl_RingCntCntl_avioBotStart                   0x00000010

    #define     BA_Gbl_RingCntCntl_avioBotStop                 0x012C
    #define     B16Gbl_RingCntCntl_avioBotStop                 0x012C
    #define   LSb32Gbl_RingCntCntl_avioBotStop                    5
    #define   LSb16Gbl_RingCntCntl_avioBotStop                    5
    #define       bGbl_RingCntCntl_avioBotStop                 1
    #define   MSK32Gbl_RingCntCntl_avioBotStop                    0x00000020

    #define     BA_Gbl_RingCntCntl_usbTopStart                 0x012C
    #define     B16Gbl_RingCntCntl_usbTopStart                 0x012C
    #define   LSb32Gbl_RingCntCntl_usbTopStart                    6
    #define   LSb16Gbl_RingCntCntl_usbTopStart                    6
    #define       bGbl_RingCntCntl_usbTopStart                 1
    #define   MSK32Gbl_RingCntCntl_usbTopStart                    0x00000040

    #define     BA_Gbl_RingCntCntl_usbTopStop                  0x012C
    #define     B16Gbl_RingCntCntl_usbTopStop                  0x012C
    #define   LSb32Gbl_RingCntCntl_usbTopStop                     7
    #define   LSb16Gbl_RingCntCntl_usbTopStop                     7
    #define       bGbl_RingCntCntl_usbTopStop                  1
    #define   MSK32Gbl_RingCntCntl_usbTopStop                     0x00000080

    #define     BA_Gbl_RingCntCntl_cpu1TopStart                0x012D
    #define     B16Gbl_RingCntCntl_cpu1TopStart                0x012C
    #define   LSb32Gbl_RingCntCntl_cpu1TopStart                   8
    #define   LSb16Gbl_RingCntCntl_cpu1TopStart                   8
    #define       bGbl_RingCntCntl_cpu1TopStart                1
    #define   MSK32Gbl_RingCntCntl_cpu1TopStart                   0x00000100

    #define     BA_Gbl_RingCntCntl_cpu1TopStop                 0x012D
    #define     B16Gbl_RingCntCntl_cpu1TopStop                 0x012C
    #define   LSb32Gbl_RingCntCntl_cpu1TopStop                    9
    #define   LSb16Gbl_RingCntCntl_cpu1TopStop                    9
    #define       bGbl_RingCntCntl_cpu1TopStop                 1
    #define   MSK32Gbl_RingCntCntl_cpu1TopStop                    0x00000200

    #define     BA_Gbl_RingCntCntl_cpu0TopStart                0x012D
    #define     B16Gbl_RingCntCntl_cpu0TopStart                0x012C
    #define   LSb32Gbl_RingCntCntl_cpu0TopStart                   10
    #define   LSb16Gbl_RingCntCntl_cpu0TopStart                   10
    #define       bGbl_RingCntCntl_cpu0TopStart                1
    #define   MSK32Gbl_RingCntCntl_cpu0TopStart                   0x00000400

    #define     BA_Gbl_RingCntCntl_cpu0TopStop                 0x012D
    #define     B16Gbl_RingCntCntl_cpu0TopStop                 0x012C
    #define   LSb32Gbl_RingCntCntl_cpu0TopStop                    11
    #define   LSb16Gbl_RingCntCntl_cpu0TopStop                    11
    #define       bGbl_RingCntCntl_cpu0TopStop                 1
    #define   MSK32Gbl_RingCntCntl_cpu0TopStop                    0x00000800

    #define     BA_Gbl_RingCntCntl_cpuBotStart                 0x012D
    #define     B16Gbl_RingCntCntl_cpuBotStart                 0x012C
    #define   LSb32Gbl_RingCntCntl_cpuBotStart                    12
    #define   LSb16Gbl_RingCntCntl_cpuBotStart                    12
    #define       bGbl_RingCntCntl_cpuBotStart                 1
    #define   MSK32Gbl_RingCntCntl_cpuBotStart                    0x00001000

    #define     BA_Gbl_RingCntCntl_cpuBotStop                  0x012D
    #define     B16Gbl_RingCntCntl_cpuBotStop                  0x012C
    #define   LSb32Gbl_RingCntCntl_cpuBotStop                     13
    #define   LSb16Gbl_RingCntCntl_cpuBotStop                     13
    #define       bGbl_RingCntCntl_cpuBotStop                  1
    #define   MSK32Gbl_RingCntCntl_cpuBotStop                     0x00002000

    #define     BA_Gbl_RingCntCntl_topRiteStart                0x012D
    #define     B16Gbl_RingCntCntl_topRiteStart                0x012C
    #define   LSb32Gbl_RingCntCntl_topRiteStart                   14
    #define   LSb16Gbl_RingCntCntl_topRiteStart                   14
    #define       bGbl_RingCntCntl_topRiteStart                1
    #define   MSK32Gbl_RingCntCntl_topRiteStart                   0x00004000

    #define     BA_Gbl_RingCntCntl_topRiteStop                 0x012D
    #define     B16Gbl_RingCntCntl_topRiteStop                 0x012C
    #define   LSb32Gbl_RingCntCntl_topRiteStop                    15
    #define   LSb16Gbl_RingCntCntl_topRiteStop                    15
    #define       bGbl_RingCntCntl_topRiteStop                 1
    #define   MSK32Gbl_RingCntCntl_topRiteStop                    0x00008000

    #define     BA_Gbl_RingCntCntl_midRiteStart                0x012E
    #define     B16Gbl_RingCntCntl_midRiteStart                0x012E
    #define   LSb32Gbl_RingCntCntl_midRiteStart                   16
    #define   LSb16Gbl_RingCntCntl_midRiteStart                   0
    #define       bGbl_RingCntCntl_midRiteStart                1
    #define   MSK32Gbl_RingCntCntl_midRiteStart                   0x00010000

    #define     BA_Gbl_RingCntCntl_midRiteStop                 0x012E
    #define     B16Gbl_RingCntCntl_midRiteStop                 0x012E
    #define   LSb32Gbl_RingCntCntl_midRiteStop                    17
    #define   LSb16Gbl_RingCntCntl_midRiteStop                    1
    #define       bGbl_RingCntCntl_midRiteStop                 1
    #define   MSK32Gbl_RingCntCntl_midRiteStop                    0x00020000

    #define     BA_Gbl_RingCntCntl_botRiteStart                0x012E
    #define     B16Gbl_RingCntCntl_botRiteStart                0x012E
    #define   LSb32Gbl_RingCntCntl_botRiteStart                   18
    #define   LSb16Gbl_RingCntCntl_botRiteStart                   2
    #define       bGbl_RingCntCntl_botRiteStart                1
    #define   MSK32Gbl_RingCntCntl_botRiteStart                   0x00040000

    #define     BA_Gbl_RingCntCntl_botRiteStop                 0x012E
    #define     B16Gbl_RingCntCntl_botRiteStop                 0x012E
    #define   LSb32Gbl_RingCntCntl_botRiteStop                    19
    #define   LSb16Gbl_RingCntCntl_botRiteStop                    3
    #define       bGbl_RingCntCntl_botRiteStop                 1
    #define   MSK32Gbl_RingCntCntl_botRiteStop                    0x00080000
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_CenterCnt                               0x0130

    #define     BA_Gbl_CenterCnt_CNTVALUE                      0x0130
    #define     B16Gbl_CenterCnt_CNTVALUE                      0x0130
    #define   LSb32Gbl_CenterCnt_CNTVALUE                         0
    #define   LSb16Gbl_CenterCnt_CNTVALUE                         0
    #define       bGbl_CenterCnt_CNTVALUE                      16
    #define   MSK32Gbl_CenterCnt_CNTVALUE                         0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_avioTopCnt                              0x0134

    #define     BA_Gbl_avioTopCnt_CNTVALUE                     0x0134
    #define     B16Gbl_avioTopCnt_CNTVALUE                     0x0134
    #define   LSb32Gbl_avioTopCnt_CNTVALUE                        0
    #define   LSb16Gbl_avioTopCnt_CNTVALUE                        0
    #define       bGbl_avioTopCnt_CNTVALUE                     16
    #define   MSK32Gbl_avioTopCnt_CNTVALUE                        0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_avioBotCnt                              0x0138

    #define     BA_Gbl_avioBotCnt_CNTVALUE                     0x0138
    #define     B16Gbl_avioBotCnt_CNTVALUE                     0x0138
    #define   LSb32Gbl_avioBotCnt_CNTVALUE                        0
    #define   LSb16Gbl_avioBotCnt_CNTVALUE                        0
    #define       bGbl_avioBotCnt_CNTVALUE                     16
    #define   MSK32Gbl_avioBotCnt_CNTVALUE                        0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_usbTopCnt                               0x013C

    #define     BA_Gbl_usbTopCnt_CNTVALUE                      0x013C
    #define     B16Gbl_usbTopCnt_CNTVALUE                      0x013C
    #define   LSb32Gbl_usbTopCnt_CNTVALUE                         0
    #define   LSb16Gbl_usbTopCnt_CNTVALUE                         0
    #define       bGbl_usbTopCnt_CNTVALUE                      16
    #define   MSK32Gbl_usbTopCnt_CNTVALUE                         0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_cpu1TopCnt                              0x0140

    #define     BA_Gbl_cpu1TopCnt_CNTVALUE                     0x0140
    #define     B16Gbl_cpu1TopCnt_CNTVALUE                     0x0140
    #define   LSb32Gbl_cpu1TopCnt_CNTVALUE                        0
    #define   LSb16Gbl_cpu1TopCnt_CNTVALUE                        0
    #define       bGbl_cpu1TopCnt_CNTVALUE                     16
    #define   MSK32Gbl_cpu1TopCnt_CNTVALUE                        0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_cpu0TopCnt                              0x0144

    #define     BA_Gbl_cpu0TopCnt_CNTVALUE                     0x0144
    #define     B16Gbl_cpu0TopCnt_CNTVALUE                     0x0144
    #define   LSb32Gbl_cpu0TopCnt_CNTVALUE                        0
    #define   LSb16Gbl_cpu0TopCnt_CNTVALUE                        0
    #define       bGbl_cpu0TopCnt_CNTVALUE                     16
    #define   MSK32Gbl_cpu0TopCnt_CNTVALUE                        0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_cpuBotCnt                               0x0148

    #define     BA_Gbl_cpuBotCnt_CNTVALUE                      0x0148
    #define     B16Gbl_cpuBotCnt_CNTVALUE                      0x0148
    #define   LSb32Gbl_cpuBotCnt_CNTVALUE                         0
    #define   LSb16Gbl_cpuBotCnt_CNTVALUE                         0
    #define       bGbl_cpuBotCnt_CNTVALUE                      16
    #define   MSK32Gbl_cpuBotCnt_CNTVALUE                         0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_topRiteCnt                              0x014C

    #define     BA_Gbl_topRiteCnt_CNTVALUE                     0x014C
    #define     B16Gbl_topRiteCnt_CNTVALUE                     0x014C
    #define   LSb32Gbl_topRiteCnt_CNTVALUE                        0
    #define   LSb16Gbl_topRiteCnt_CNTVALUE                        0
    #define       bGbl_topRiteCnt_CNTVALUE                     16
    #define   MSK32Gbl_topRiteCnt_CNTVALUE                        0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_midRiteCnt                              0x0150

    #define     BA_Gbl_midRiteCnt_CNTVALUE                     0x0150
    #define     B16Gbl_midRiteCnt_CNTVALUE                     0x0150
    #define   LSb32Gbl_midRiteCnt_CNTVALUE                        0
    #define   LSb16Gbl_midRiteCnt_CNTVALUE                        0
    #define       bGbl_midRiteCnt_CNTVALUE                     16
    #define   MSK32Gbl_midRiteCnt_CNTVALUE                        0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_botRiteCnt                              0x0154

    #define     BA_Gbl_botRiteCnt_CNTVALUE                     0x0154
    #define     B16Gbl_botRiteCnt_CNTVALUE                     0x0154
    #define   LSb32Gbl_botRiteCnt_CNTVALUE                        0
    #define   LSb16Gbl_botRiteCnt_CNTVALUE                        0
    #define       bGbl_botRiteCnt_CNTVALUE                     16
    #define   MSK32Gbl_botRiteCnt_CNTVALUE                        0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_IO0Cntl                            0x0158

    #define     BA_Gbl_NAND_IO0Cntl_SLP_DI                     0x0158
    #define     B16Gbl_NAND_IO0Cntl_SLP_DI                     0x0158
    #define   LSb32Gbl_NAND_IO0Cntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_IO0Cntl_SLP_DI                        0
    #define       bGbl_NAND_IO0Cntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_IO0Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_IO0Cntl_PU_EN                      0x0158
    #define     B16Gbl_NAND_IO0Cntl_PU_EN                      0x0158
    #define   LSb32Gbl_NAND_IO0Cntl_PU_EN                         1
    #define   LSb16Gbl_NAND_IO0Cntl_PU_EN                         1
    #define       bGbl_NAND_IO0Cntl_PU_EN                      1
    #define   MSK32Gbl_NAND_IO0Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_IO0Cntl_PD_EN                      0x0158
    #define     B16Gbl_NAND_IO0Cntl_PD_EN                      0x0158
    #define   LSb32Gbl_NAND_IO0Cntl_PD_EN                         2
    #define   LSb16Gbl_NAND_IO0Cntl_PD_EN                         2
    #define       bGbl_NAND_IO0Cntl_PD_EN                      1
    #define   MSK32Gbl_NAND_IO0Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_IO1Cntl                            0x015C

    #define     BA_Gbl_NAND_IO1Cntl_SLP_DI                     0x015C
    #define     B16Gbl_NAND_IO1Cntl_SLP_DI                     0x015C
    #define   LSb32Gbl_NAND_IO1Cntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_IO1Cntl_SLP_DI                        0
    #define       bGbl_NAND_IO1Cntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_IO1Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_IO1Cntl_PU_EN                      0x015C
    #define     B16Gbl_NAND_IO1Cntl_PU_EN                      0x015C
    #define   LSb32Gbl_NAND_IO1Cntl_PU_EN                         1
    #define   LSb16Gbl_NAND_IO1Cntl_PU_EN                         1
    #define       bGbl_NAND_IO1Cntl_PU_EN                      1
    #define   MSK32Gbl_NAND_IO1Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_IO1Cntl_PD_EN                      0x015C
    #define     B16Gbl_NAND_IO1Cntl_PD_EN                      0x015C
    #define   LSb32Gbl_NAND_IO1Cntl_PD_EN                         2
    #define   LSb16Gbl_NAND_IO1Cntl_PD_EN                         2
    #define       bGbl_NAND_IO1Cntl_PD_EN                      1
    #define   MSK32Gbl_NAND_IO1Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_IO2Cntl                            0x0160

    #define     BA_Gbl_NAND_IO2Cntl_SLP_DI                     0x0160
    #define     B16Gbl_NAND_IO2Cntl_SLP_DI                     0x0160
    #define   LSb32Gbl_NAND_IO2Cntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_IO2Cntl_SLP_DI                        0
    #define       bGbl_NAND_IO2Cntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_IO2Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_IO2Cntl_PU_EN                      0x0160
    #define     B16Gbl_NAND_IO2Cntl_PU_EN                      0x0160
    #define   LSb32Gbl_NAND_IO2Cntl_PU_EN                         1
    #define   LSb16Gbl_NAND_IO2Cntl_PU_EN                         1
    #define       bGbl_NAND_IO2Cntl_PU_EN                      1
    #define   MSK32Gbl_NAND_IO2Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_IO2Cntl_PD_EN                      0x0160
    #define     B16Gbl_NAND_IO2Cntl_PD_EN                      0x0160
    #define   LSb32Gbl_NAND_IO2Cntl_PD_EN                         2
    #define   LSb16Gbl_NAND_IO2Cntl_PD_EN                         2
    #define       bGbl_NAND_IO2Cntl_PD_EN                      1
    #define   MSK32Gbl_NAND_IO2Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_IO3Cntl                            0x0164

    #define     BA_Gbl_NAND_IO3Cntl_SLP_DI                     0x0164
    #define     B16Gbl_NAND_IO3Cntl_SLP_DI                     0x0164
    #define   LSb32Gbl_NAND_IO3Cntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_IO3Cntl_SLP_DI                        0
    #define       bGbl_NAND_IO3Cntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_IO3Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_IO3Cntl_PU_EN                      0x0164
    #define     B16Gbl_NAND_IO3Cntl_PU_EN                      0x0164
    #define   LSb32Gbl_NAND_IO3Cntl_PU_EN                         1
    #define   LSb16Gbl_NAND_IO3Cntl_PU_EN                         1
    #define       bGbl_NAND_IO3Cntl_PU_EN                      1
    #define   MSK32Gbl_NAND_IO3Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_IO3Cntl_PD_EN                      0x0164
    #define     B16Gbl_NAND_IO3Cntl_PD_EN                      0x0164
    #define   LSb32Gbl_NAND_IO3Cntl_PD_EN                         2
    #define   LSb16Gbl_NAND_IO3Cntl_PD_EN                         2
    #define       bGbl_NAND_IO3Cntl_PD_EN                      1
    #define   MSK32Gbl_NAND_IO3Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_IO4Cntl                            0x0168

    #define     BA_Gbl_NAND_IO4Cntl_SLP_DI                     0x0168
    #define     B16Gbl_NAND_IO4Cntl_SLP_DI                     0x0168
    #define   LSb32Gbl_NAND_IO4Cntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_IO4Cntl_SLP_DI                        0
    #define       bGbl_NAND_IO4Cntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_IO4Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_IO4Cntl_PU_EN                      0x0168
    #define     B16Gbl_NAND_IO4Cntl_PU_EN                      0x0168
    #define   LSb32Gbl_NAND_IO4Cntl_PU_EN                         1
    #define   LSb16Gbl_NAND_IO4Cntl_PU_EN                         1
    #define       bGbl_NAND_IO4Cntl_PU_EN                      1
    #define   MSK32Gbl_NAND_IO4Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_IO4Cntl_PD_EN                      0x0168
    #define     B16Gbl_NAND_IO4Cntl_PD_EN                      0x0168
    #define   LSb32Gbl_NAND_IO4Cntl_PD_EN                         2
    #define   LSb16Gbl_NAND_IO4Cntl_PD_EN                         2
    #define       bGbl_NAND_IO4Cntl_PD_EN                      1
    #define   MSK32Gbl_NAND_IO4Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_IO5Cntl                            0x016C

    #define     BA_Gbl_NAND_IO5Cntl_SLP_DI                     0x016C
    #define     B16Gbl_NAND_IO5Cntl_SLP_DI                     0x016C
    #define   LSb32Gbl_NAND_IO5Cntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_IO5Cntl_SLP_DI                        0
    #define       bGbl_NAND_IO5Cntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_IO5Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_IO5Cntl_PU_EN                      0x016C
    #define     B16Gbl_NAND_IO5Cntl_PU_EN                      0x016C
    #define   LSb32Gbl_NAND_IO5Cntl_PU_EN                         1
    #define   LSb16Gbl_NAND_IO5Cntl_PU_EN                         1
    #define       bGbl_NAND_IO5Cntl_PU_EN                      1
    #define   MSK32Gbl_NAND_IO5Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_IO5Cntl_PD_EN                      0x016C
    #define     B16Gbl_NAND_IO5Cntl_PD_EN                      0x016C
    #define   LSb32Gbl_NAND_IO5Cntl_PD_EN                         2
    #define   LSb16Gbl_NAND_IO5Cntl_PD_EN                         2
    #define       bGbl_NAND_IO5Cntl_PD_EN                      1
    #define   MSK32Gbl_NAND_IO5Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_IO6Cntl                            0x0170

    #define     BA_Gbl_NAND_IO6Cntl_SLP_DI                     0x0170
    #define     B16Gbl_NAND_IO6Cntl_SLP_DI                     0x0170
    #define   LSb32Gbl_NAND_IO6Cntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_IO6Cntl_SLP_DI                        0
    #define       bGbl_NAND_IO6Cntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_IO6Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_IO6Cntl_PU_EN                      0x0170
    #define     B16Gbl_NAND_IO6Cntl_PU_EN                      0x0170
    #define   LSb32Gbl_NAND_IO6Cntl_PU_EN                         1
    #define   LSb16Gbl_NAND_IO6Cntl_PU_EN                         1
    #define       bGbl_NAND_IO6Cntl_PU_EN                      1
    #define   MSK32Gbl_NAND_IO6Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_IO6Cntl_PD_EN                      0x0170
    #define     B16Gbl_NAND_IO6Cntl_PD_EN                      0x0170
    #define   LSb32Gbl_NAND_IO6Cntl_PD_EN                         2
    #define   LSb16Gbl_NAND_IO6Cntl_PD_EN                         2
    #define       bGbl_NAND_IO6Cntl_PD_EN                      1
    #define   MSK32Gbl_NAND_IO6Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_IO7Cntl                            0x0174

    #define     BA_Gbl_NAND_IO7Cntl_SLP_DI                     0x0174
    #define     B16Gbl_NAND_IO7Cntl_SLP_DI                     0x0174
    #define   LSb32Gbl_NAND_IO7Cntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_IO7Cntl_SLP_DI                        0
    #define       bGbl_NAND_IO7Cntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_IO7Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_IO7Cntl_PU_EN                      0x0174
    #define     B16Gbl_NAND_IO7Cntl_PU_EN                      0x0174
    #define   LSb32Gbl_NAND_IO7Cntl_PU_EN                         1
    #define   LSb16Gbl_NAND_IO7Cntl_PU_EN                         1
    #define       bGbl_NAND_IO7Cntl_PU_EN                      1
    #define   MSK32Gbl_NAND_IO7Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_IO7Cntl_PD_EN                      0x0174
    #define     B16Gbl_NAND_IO7Cntl_PD_EN                      0x0174
    #define   LSb32Gbl_NAND_IO7Cntl_PD_EN                         2
    #define   LSb16Gbl_NAND_IO7Cntl_PD_EN                         2
    #define       bGbl_NAND_IO7Cntl_PD_EN                      1
    #define   MSK32Gbl_NAND_IO7Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_ALECntl                            0x0178

    #define     BA_Gbl_NAND_ALECntl_SLP_DI                     0x0178
    #define     B16Gbl_NAND_ALECntl_SLP_DI                     0x0178
    #define   LSb32Gbl_NAND_ALECntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_ALECntl_SLP_DI                        0
    #define       bGbl_NAND_ALECntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_ALECntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_ALECntl_PU_EN                      0x0178
    #define     B16Gbl_NAND_ALECntl_PU_EN                      0x0178
    #define   LSb32Gbl_NAND_ALECntl_PU_EN                         1
    #define   LSb16Gbl_NAND_ALECntl_PU_EN                         1
    #define       bGbl_NAND_ALECntl_PU_EN                      1
    #define   MSK32Gbl_NAND_ALECntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_ALECntl_PD_EN                      0x0178
    #define     B16Gbl_NAND_ALECntl_PD_EN                      0x0178
    #define   LSb32Gbl_NAND_ALECntl_PD_EN                         2
    #define   LSb16Gbl_NAND_ALECntl_PD_EN                         2
    #define       bGbl_NAND_ALECntl_PD_EN                      1
    #define   MSK32Gbl_NAND_ALECntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_CLECntl                            0x017C

    #define     BA_Gbl_NAND_CLECntl_SLP_DI                     0x017C
    #define     B16Gbl_NAND_CLECntl_SLP_DI                     0x017C
    #define   LSb32Gbl_NAND_CLECntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_CLECntl_SLP_DI                        0
    #define       bGbl_NAND_CLECntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_CLECntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_CLECntl_PU_EN                      0x017C
    #define     B16Gbl_NAND_CLECntl_PU_EN                      0x017C
    #define   LSb32Gbl_NAND_CLECntl_PU_EN                         1
    #define   LSb16Gbl_NAND_CLECntl_PU_EN                         1
    #define       bGbl_NAND_CLECntl_PU_EN                      1
    #define   MSK32Gbl_NAND_CLECntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_CLECntl_PD_EN                      0x017C
    #define     B16Gbl_NAND_CLECntl_PD_EN                      0x017C
    #define   LSb32Gbl_NAND_CLECntl_PD_EN                         2
    #define   LSb16Gbl_NAND_CLECntl_PD_EN                         2
    #define       bGbl_NAND_CLECntl_PD_EN                      1
    #define   MSK32Gbl_NAND_CLECntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_WEnCntl                            0x0180

    #define     BA_Gbl_NAND_WEnCntl_SLP_DI                     0x0180
    #define     B16Gbl_NAND_WEnCntl_SLP_DI                     0x0180
    #define   LSb32Gbl_NAND_WEnCntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_WEnCntl_SLP_DI                        0
    #define       bGbl_NAND_WEnCntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_WEnCntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_WEnCntl_PU_EN                      0x0180
    #define     B16Gbl_NAND_WEnCntl_PU_EN                      0x0180
    #define   LSb32Gbl_NAND_WEnCntl_PU_EN                         1
    #define   LSb16Gbl_NAND_WEnCntl_PU_EN                         1
    #define       bGbl_NAND_WEnCntl_PU_EN                      1
    #define   MSK32Gbl_NAND_WEnCntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_WEnCntl_PD_EN                      0x0180
    #define     B16Gbl_NAND_WEnCntl_PD_EN                      0x0180
    #define   LSb32Gbl_NAND_WEnCntl_PD_EN                         2
    #define   LSb16Gbl_NAND_WEnCntl_PD_EN                         2
    #define       bGbl_NAND_WEnCntl_PD_EN                      1
    #define   MSK32Gbl_NAND_WEnCntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_REnCntl                            0x0184

    #define     BA_Gbl_NAND_REnCntl_SLP_DI                     0x0184
    #define     B16Gbl_NAND_REnCntl_SLP_DI                     0x0184
    #define   LSb32Gbl_NAND_REnCntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_REnCntl_SLP_DI                        0
    #define       bGbl_NAND_REnCntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_REnCntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_REnCntl_PU_EN                      0x0184
    #define     B16Gbl_NAND_REnCntl_PU_EN                      0x0184
    #define   LSb32Gbl_NAND_REnCntl_PU_EN                         1
    #define   LSb16Gbl_NAND_REnCntl_PU_EN                         1
    #define       bGbl_NAND_REnCntl_PU_EN                      1
    #define   MSK32Gbl_NAND_REnCntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_REnCntl_PD_EN                      0x0184
    #define     B16Gbl_NAND_REnCntl_PD_EN                      0x0184
    #define   LSb32Gbl_NAND_REnCntl_PD_EN                         2
    #define   LSb16Gbl_NAND_REnCntl_PD_EN                         2
    #define       bGbl_NAND_REnCntl_PD_EN                      1
    #define   MSK32Gbl_NAND_REnCntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_WPnCntl                            0x0188

    #define     BA_Gbl_NAND_WPnCntl_SLP_DI                     0x0188
    #define     B16Gbl_NAND_WPnCntl_SLP_DI                     0x0188
    #define   LSb32Gbl_NAND_WPnCntl_SLP_DI                        0
    #define   LSb16Gbl_NAND_WPnCntl_SLP_DI                        0
    #define       bGbl_NAND_WPnCntl_SLP_DI                     1
    #define   MSK32Gbl_NAND_WPnCntl_SLP_DI                        0x00000001

    #define     BA_Gbl_NAND_WPnCntl_PU_EN                      0x0188
    #define     B16Gbl_NAND_WPnCntl_PU_EN                      0x0188
    #define   LSb32Gbl_NAND_WPnCntl_PU_EN                         1
    #define   LSb16Gbl_NAND_WPnCntl_PU_EN                         1
    #define       bGbl_NAND_WPnCntl_PU_EN                      1
    #define   MSK32Gbl_NAND_WPnCntl_PU_EN                         0x00000002

    #define     BA_Gbl_NAND_WPnCntl_PD_EN                      0x0188
    #define     B16Gbl_NAND_WPnCntl_PD_EN                      0x0188
    #define   LSb32Gbl_NAND_WPnCntl_PD_EN                         2
    #define   LSb16Gbl_NAND_WPnCntl_PD_EN                         2
    #define       bGbl_NAND_WPnCntl_PD_EN                      1
    #define   MSK32Gbl_NAND_WPnCntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_CEn0Cntl                           0x018C

    #define     BA_Gbl_NAND_CEn0Cntl_SLP_DI                    0x018C
    #define     B16Gbl_NAND_CEn0Cntl_SLP_DI                    0x018C
    #define   LSb32Gbl_NAND_CEn0Cntl_SLP_DI                       0
    #define   LSb16Gbl_NAND_CEn0Cntl_SLP_DI                       0
    #define       bGbl_NAND_CEn0Cntl_SLP_DI                    1
    #define   MSK32Gbl_NAND_CEn0Cntl_SLP_DI                       0x00000001

    #define     BA_Gbl_NAND_CEn0Cntl_PU_EN                     0x018C
    #define     B16Gbl_NAND_CEn0Cntl_PU_EN                     0x018C
    #define   LSb32Gbl_NAND_CEn0Cntl_PU_EN                        1
    #define   LSb16Gbl_NAND_CEn0Cntl_PU_EN                        1
    #define       bGbl_NAND_CEn0Cntl_PU_EN                     1
    #define   MSK32Gbl_NAND_CEn0Cntl_PU_EN                        0x00000002

    #define     BA_Gbl_NAND_CEn0Cntl_PD_EN                     0x018C
    #define     B16Gbl_NAND_CEn0Cntl_PD_EN                     0x018C
    #define   LSb32Gbl_NAND_CEn0Cntl_PD_EN                        2
    #define   LSb16Gbl_NAND_CEn0Cntl_PD_EN                        2
    #define       bGbl_NAND_CEn0Cntl_PD_EN                     1
    #define   MSK32Gbl_NAND_CEn0Cntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_CEn1Cntl                           0x0190

    #define     BA_Gbl_NAND_CEn1Cntl_SLP_DI                    0x0190
    #define     B16Gbl_NAND_CEn1Cntl_SLP_DI                    0x0190
    #define   LSb32Gbl_NAND_CEn1Cntl_SLP_DI                       0
    #define   LSb16Gbl_NAND_CEn1Cntl_SLP_DI                       0
    #define       bGbl_NAND_CEn1Cntl_SLP_DI                    1
    #define   MSK32Gbl_NAND_CEn1Cntl_SLP_DI                       0x00000001

    #define     BA_Gbl_NAND_CEn1Cntl_PU_EN                     0x0190
    #define     B16Gbl_NAND_CEn1Cntl_PU_EN                     0x0190
    #define   LSb32Gbl_NAND_CEn1Cntl_PU_EN                        1
    #define   LSb16Gbl_NAND_CEn1Cntl_PU_EN                        1
    #define       bGbl_NAND_CEn1Cntl_PU_EN                     1
    #define   MSK32Gbl_NAND_CEn1Cntl_PU_EN                        0x00000002

    #define     BA_Gbl_NAND_CEn1Cntl_PD_EN                     0x0190
    #define     B16Gbl_NAND_CEn1Cntl_PD_EN                     0x0190
    #define   LSb32Gbl_NAND_CEn1Cntl_PD_EN                        2
    #define   LSb16Gbl_NAND_CEn1Cntl_PD_EN                        2
    #define       bGbl_NAND_CEn1Cntl_PD_EN                     1
    #define   MSK32Gbl_NAND_CEn1Cntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_RDY0Cntl                           0x0194

    #define     BA_Gbl_NAND_RDY0Cntl_SLP_DI                    0x0194
    #define     B16Gbl_NAND_RDY0Cntl_SLP_DI                    0x0194
    #define   LSb32Gbl_NAND_RDY0Cntl_SLP_DI                       0
    #define   LSb16Gbl_NAND_RDY0Cntl_SLP_DI                       0
    #define       bGbl_NAND_RDY0Cntl_SLP_DI                    1
    #define   MSK32Gbl_NAND_RDY0Cntl_SLP_DI                       0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_NAND_RDY1Cntl                           0x0198

    #define     BA_Gbl_NAND_RDY1Cntl_SLP_DI                    0x0198
    #define     B16Gbl_NAND_RDY1Cntl_SLP_DI                    0x0198
    #define   LSb32Gbl_NAND_RDY1Cntl_SLP_DI                       0
    #define   LSb16Gbl_NAND_RDY1Cntl_SLP_DI                       0
    #define       bGbl_NAND_RDY1Cntl_SLP_DI                    1
    #define   MSK32Gbl_NAND_RDY1Cntl_SLP_DI                       0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_TXCLKIOCntl                          0x019C

    #define     BA_Gbl_ET_TXCLKIOCntl_SLP_DI                   0x019C
    #define     B16Gbl_ET_TXCLKIOCntl_SLP_DI                   0x019C
    #define   LSb32Gbl_ET_TXCLKIOCntl_SLP_DI                      0
    #define   LSb16Gbl_ET_TXCLKIOCntl_SLP_DI                      0
    #define       bGbl_ET_TXCLKIOCntl_SLP_DI                   1
    #define   MSK32Gbl_ET_TXCLKIOCntl_SLP_DI                      0x00000001

    #define     BA_Gbl_ET_TXCLKIOCntl_PU_EN                    0x019C
    #define     B16Gbl_ET_TXCLKIOCntl_PU_EN                    0x019C
    #define   LSb32Gbl_ET_TXCLKIOCntl_PU_EN                       1
    #define   LSb16Gbl_ET_TXCLKIOCntl_PU_EN                       1
    #define       bGbl_ET_TXCLKIOCntl_PU_EN                    1
    #define   MSK32Gbl_ET_TXCLKIOCntl_PU_EN                       0x00000002

    #define     BA_Gbl_ET_TXCLKIOCntl_PD_EN                    0x019C
    #define     B16Gbl_ET_TXCLKIOCntl_PD_EN                    0x019C
    #define   LSb32Gbl_ET_TXCLKIOCntl_PD_EN                       2
    #define   LSb16Gbl_ET_TXCLKIOCntl_PD_EN                       2
    #define       bGbl_ET_TXCLKIOCntl_PD_EN                    1
    #define   MSK32Gbl_ET_TXCLKIOCntl_PD_EN                       0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_TXD0Cntl                             0x01A0

    #define     BA_Gbl_ET_TXD0Cntl_SLP_DI                      0x01A0
    #define     B16Gbl_ET_TXD0Cntl_SLP_DI                      0x01A0
    #define   LSb32Gbl_ET_TXD0Cntl_SLP_DI                         0
    #define   LSb16Gbl_ET_TXD0Cntl_SLP_DI                         0
    #define       bGbl_ET_TXD0Cntl_SLP_DI                      1
    #define   MSK32Gbl_ET_TXD0Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_TXD0Cntl_PU_EN                       0x01A0
    #define     B16Gbl_ET_TXD0Cntl_PU_EN                       0x01A0
    #define   LSb32Gbl_ET_TXD0Cntl_PU_EN                          1
    #define   LSb16Gbl_ET_TXD0Cntl_PU_EN                          1
    #define       bGbl_ET_TXD0Cntl_PU_EN                       1
    #define   MSK32Gbl_ET_TXD0Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_TXD0Cntl_PD_EN                       0x01A0
    #define     B16Gbl_ET_TXD0Cntl_PD_EN                       0x01A0
    #define   LSb32Gbl_ET_TXD0Cntl_PD_EN                          2
    #define   LSb16Gbl_ET_TXD0Cntl_PD_EN                          2
    #define       bGbl_ET_TXD0Cntl_PD_EN                       1
    #define   MSK32Gbl_ET_TXD0Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_TXD1Cntl                             0x01A4

    #define     BA_Gbl_ET_TXD1Cntl_SLP_DI                      0x01A4
    #define     B16Gbl_ET_TXD1Cntl_SLP_DI                      0x01A4
    #define   LSb32Gbl_ET_TXD1Cntl_SLP_DI                         0
    #define   LSb16Gbl_ET_TXD1Cntl_SLP_DI                         0
    #define       bGbl_ET_TXD1Cntl_SLP_DI                      1
    #define   MSK32Gbl_ET_TXD1Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_TXD1Cntl_PU_EN                       0x01A4
    #define     B16Gbl_ET_TXD1Cntl_PU_EN                       0x01A4
    #define   LSb32Gbl_ET_TXD1Cntl_PU_EN                          1
    #define   LSb16Gbl_ET_TXD1Cntl_PU_EN                          1
    #define       bGbl_ET_TXD1Cntl_PU_EN                       1
    #define   MSK32Gbl_ET_TXD1Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_TXD1Cntl_PD_EN                       0x01A4
    #define     B16Gbl_ET_TXD1Cntl_PD_EN                       0x01A4
    #define   LSb32Gbl_ET_TXD1Cntl_PD_EN                          2
    #define   LSb16Gbl_ET_TXD1Cntl_PD_EN                          2
    #define       bGbl_ET_TXD1Cntl_PD_EN                       1
    #define   MSK32Gbl_ET_TXD1Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_TXD2Cntl                             0x01A8

    #define     BA_Gbl_ET_TXD2Cntl_SLP_DI                      0x01A8
    #define     B16Gbl_ET_TXD2Cntl_SLP_DI                      0x01A8
    #define   LSb32Gbl_ET_TXD2Cntl_SLP_DI                         0
    #define   LSb16Gbl_ET_TXD2Cntl_SLP_DI                         0
    #define       bGbl_ET_TXD2Cntl_SLP_DI                      1
    #define   MSK32Gbl_ET_TXD2Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_TXD2Cntl_PU_EN                       0x01A8
    #define     B16Gbl_ET_TXD2Cntl_PU_EN                       0x01A8
    #define   LSb32Gbl_ET_TXD2Cntl_PU_EN                          1
    #define   LSb16Gbl_ET_TXD2Cntl_PU_EN                          1
    #define       bGbl_ET_TXD2Cntl_PU_EN                       1
    #define   MSK32Gbl_ET_TXD2Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_TXD2Cntl_PD_EN                       0x01A8
    #define     B16Gbl_ET_TXD2Cntl_PD_EN                       0x01A8
    #define   LSb32Gbl_ET_TXD2Cntl_PD_EN                          2
    #define   LSb16Gbl_ET_TXD2Cntl_PD_EN                          2
    #define       bGbl_ET_TXD2Cntl_PD_EN                       1
    #define   MSK32Gbl_ET_TXD2Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_TXD3Cntl                             0x01AC

    #define     BA_Gbl_ET_TXD3Cntl_SLP_DI                      0x01AC
    #define     B16Gbl_ET_TXD3Cntl_SLP_DI                      0x01AC
    #define   LSb32Gbl_ET_TXD3Cntl_SLP_DI                         0
    #define   LSb16Gbl_ET_TXD3Cntl_SLP_DI                         0
    #define       bGbl_ET_TXD3Cntl_SLP_DI                      1
    #define   MSK32Gbl_ET_TXD3Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_TXD3Cntl_PU_EN                       0x01AC
    #define     B16Gbl_ET_TXD3Cntl_PU_EN                       0x01AC
    #define   LSb32Gbl_ET_TXD3Cntl_PU_EN                          1
    #define   LSb16Gbl_ET_TXD3Cntl_PU_EN                          1
    #define       bGbl_ET_TXD3Cntl_PU_EN                       1
    #define   MSK32Gbl_ET_TXD3Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_TXD3Cntl_PD_EN                       0x01AC
    #define     B16Gbl_ET_TXD3Cntl_PD_EN                       0x01AC
    #define   LSb32Gbl_ET_TXD3Cntl_PD_EN                          2
    #define   LSb16Gbl_ET_TXD3Cntl_PD_EN                          2
    #define       bGbl_ET_TXD3Cntl_PD_EN                       1
    #define   MSK32Gbl_ET_TXD3Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_TXENCntl                             0x01B0

    #define     BA_Gbl_ET_TXENCntl_SLP_DI                      0x01B0
    #define     B16Gbl_ET_TXENCntl_SLP_DI                      0x01B0
    #define   LSb32Gbl_ET_TXENCntl_SLP_DI                         0
    #define   LSb16Gbl_ET_TXENCntl_SLP_DI                         0
    #define       bGbl_ET_TXENCntl_SLP_DI                      1
    #define   MSK32Gbl_ET_TXENCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_TXENCntl_PU_EN                       0x01B0
    #define     B16Gbl_ET_TXENCntl_PU_EN                       0x01B0
    #define   LSb32Gbl_ET_TXENCntl_PU_EN                          1
    #define   LSb16Gbl_ET_TXENCntl_PU_EN                          1
    #define       bGbl_ET_TXENCntl_PU_EN                       1
    #define   MSK32Gbl_ET_TXENCntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_TXENCntl_PD_EN                       0x01B0
    #define     B16Gbl_ET_TXENCntl_PD_EN                       0x01B0
    #define   LSb32Gbl_ET_TXENCntl_PD_EN                          2
    #define   LSb16Gbl_ET_TXENCntl_PD_EN                          2
    #define       bGbl_ET_TXENCntl_PD_EN                       1
    #define   MSK32Gbl_ET_TXENCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_TXERCntl                             0x01B4

    #define     BA_Gbl_ET_TXERCntl_SLP_DI                      0x01B4
    #define     B16Gbl_ET_TXERCntl_SLP_DI                      0x01B4
    #define   LSb32Gbl_ET_TXERCntl_SLP_DI                         0
    #define   LSb16Gbl_ET_TXERCntl_SLP_DI                         0
    #define       bGbl_ET_TXERCntl_SLP_DI                      1
    #define   MSK32Gbl_ET_TXERCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_TXERCntl_PU_EN                       0x01B4
    #define     B16Gbl_ET_TXERCntl_PU_EN                       0x01B4
    #define   LSb32Gbl_ET_TXERCntl_PU_EN                          1
    #define   LSb16Gbl_ET_TXERCntl_PU_EN                          1
    #define       bGbl_ET_TXERCntl_PU_EN                       1
    #define   MSK32Gbl_ET_TXERCntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_TXERCntl_PD_EN                       0x01B4
    #define     B16Gbl_ET_TXERCntl_PD_EN                       0x01B4
    #define   LSb32Gbl_ET_TXERCntl_PD_EN                          2
    #define   LSb16Gbl_ET_TXERCntl_PD_EN                          2
    #define       bGbl_ET_TXERCntl_PD_EN                       1
    #define   MSK32Gbl_ET_TXERCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_RXCLKICntl                           0x01B8

    #define     BA_Gbl_ET_RXCLKICntl_SLP_DI                    0x01B8
    #define     B16Gbl_ET_RXCLKICntl_SLP_DI                    0x01B8
    #define   LSb32Gbl_ET_RXCLKICntl_SLP_DI                       0
    #define   LSb16Gbl_ET_RXCLKICntl_SLP_DI                       0
    #define       bGbl_ET_RXCLKICntl_SLP_DI                    1
    #define   MSK32Gbl_ET_RXCLKICntl_SLP_DI                       0x00000001

    #define     BA_Gbl_ET_RXCLKICntl_PU_EN                     0x01B8
    #define     B16Gbl_ET_RXCLKICntl_PU_EN                     0x01B8
    #define   LSb32Gbl_ET_RXCLKICntl_PU_EN                        1
    #define   LSb16Gbl_ET_RXCLKICntl_PU_EN                        1
    #define       bGbl_ET_RXCLKICntl_PU_EN                     1
    #define   MSK32Gbl_ET_RXCLKICntl_PU_EN                        0x00000002

    #define     BA_Gbl_ET_RXCLKICntl_PD_EN                     0x01B8
    #define     B16Gbl_ET_RXCLKICntl_PD_EN                     0x01B8
    #define   LSb32Gbl_ET_RXCLKICntl_PD_EN                        2
    #define   LSb16Gbl_ET_RXCLKICntl_PD_EN                        2
    #define       bGbl_ET_RXCLKICntl_PD_EN                     1
    #define   MSK32Gbl_ET_RXCLKICntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_RXD0Cntl                             0x01BC

    #define     BA_Gbl_ET_RXD0Cntl_SLP_DI                      0x01BC
    #define     B16Gbl_ET_RXD0Cntl_SLP_DI                      0x01BC
    #define   LSb32Gbl_ET_RXD0Cntl_SLP_DI                         0
    #define   LSb16Gbl_ET_RXD0Cntl_SLP_DI                         0
    #define       bGbl_ET_RXD0Cntl_SLP_DI                      1
    #define   MSK32Gbl_ET_RXD0Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_RXD0Cntl_PU_EN                       0x01BC
    #define     B16Gbl_ET_RXD0Cntl_PU_EN                       0x01BC
    #define   LSb32Gbl_ET_RXD0Cntl_PU_EN                          1
    #define   LSb16Gbl_ET_RXD0Cntl_PU_EN                          1
    #define       bGbl_ET_RXD0Cntl_PU_EN                       1
    #define   MSK32Gbl_ET_RXD0Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_RXD0Cntl_PD_EN                       0x01BC
    #define     B16Gbl_ET_RXD0Cntl_PD_EN                       0x01BC
    #define   LSb32Gbl_ET_RXD0Cntl_PD_EN                          2
    #define   LSb16Gbl_ET_RXD0Cntl_PD_EN                          2
    #define       bGbl_ET_RXD0Cntl_PD_EN                       1
    #define   MSK32Gbl_ET_RXD0Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_RXD1Cntl                             0x01C0

    #define     BA_Gbl_ET_RXD1Cntl_SLP_DI                      0x01C0
    #define     B16Gbl_ET_RXD1Cntl_SLP_DI                      0x01C0
    #define   LSb32Gbl_ET_RXD1Cntl_SLP_DI                         0
    #define   LSb16Gbl_ET_RXD1Cntl_SLP_DI                         0
    #define       bGbl_ET_RXD1Cntl_SLP_DI                      1
    #define   MSK32Gbl_ET_RXD1Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_RXD1Cntl_PU_EN                       0x01C0
    #define     B16Gbl_ET_RXD1Cntl_PU_EN                       0x01C0
    #define   LSb32Gbl_ET_RXD1Cntl_PU_EN                          1
    #define   LSb16Gbl_ET_RXD1Cntl_PU_EN                          1
    #define       bGbl_ET_RXD1Cntl_PU_EN                       1
    #define   MSK32Gbl_ET_RXD1Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_RXD1Cntl_PD_EN                       0x01C0
    #define     B16Gbl_ET_RXD1Cntl_PD_EN                       0x01C0
    #define   LSb32Gbl_ET_RXD1Cntl_PD_EN                          2
    #define   LSb16Gbl_ET_RXD1Cntl_PD_EN                          2
    #define       bGbl_ET_RXD1Cntl_PD_EN                       1
    #define   MSK32Gbl_ET_RXD1Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_RXD2Cntl                             0x01C4

    #define     BA_Gbl_ET_RXD2Cntl_SLP_DI                      0x01C4
    #define     B16Gbl_ET_RXD2Cntl_SLP_DI                      0x01C4
    #define   LSb32Gbl_ET_RXD2Cntl_SLP_DI                         0
    #define   LSb16Gbl_ET_RXD2Cntl_SLP_DI                         0
    #define       bGbl_ET_RXD2Cntl_SLP_DI                      1
    #define   MSK32Gbl_ET_RXD2Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_RXD2Cntl_PU_EN                       0x01C4
    #define     B16Gbl_ET_RXD2Cntl_PU_EN                       0x01C4
    #define   LSb32Gbl_ET_RXD2Cntl_PU_EN                          1
    #define   LSb16Gbl_ET_RXD2Cntl_PU_EN                          1
    #define       bGbl_ET_RXD2Cntl_PU_EN                       1
    #define   MSK32Gbl_ET_RXD2Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_RXD2Cntl_PD_EN                       0x01C4
    #define     B16Gbl_ET_RXD2Cntl_PD_EN                       0x01C4
    #define   LSb32Gbl_ET_RXD2Cntl_PD_EN                          2
    #define   LSb16Gbl_ET_RXD2Cntl_PD_EN                          2
    #define       bGbl_ET_RXD2Cntl_PD_EN                       1
    #define   MSK32Gbl_ET_RXD2Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_RXD3Cntl                             0x01C8

    #define     BA_Gbl_ET_RXD3Cntl_SLP_DI                      0x01C8
    #define     B16Gbl_ET_RXD3Cntl_SLP_DI                      0x01C8
    #define   LSb32Gbl_ET_RXD3Cntl_SLP_DI                         0
    #define   LSb16Gbl_ET_RXD3Cntl_SLP_DI                         0
    #define       bGbl_ET_RXD3Cntl_SLP_DI                      1
    #define   MSK32Gbl_ET_RXD3Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_RXD3Cntl_PU_EN                       0x01C8
    #define     B16Gbl_ET_RXD3Cntl_PU_EN                       0x01C8
    #define   LSb32Gbl_ET_RXD3Cntl_PU_EN                          1
    #define   LSb16Gbl_ET_RXD3Cntl_PU_EN                          1
    #define       bGbl_ET_RXD3Cntl_PU_EN                       1
    #define   MSK32Gbl_ET_RXD3Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_RXD3Cntl_PD_EN                       0x01C8
    #define     B16Gbl_ET_RXD3Cntl_PD_EN                       0x01C8
    #define   LSb32Gbl_ET_RXD3Cntl_PD_EN                          2
    #define   LSb16Gbl_ET_RXD3Cntl_PD_EN                          2
    #define       bGbl_ET_RXD3Cntl_PD_EN                       1
    #define   MSK32Gbl_ET_RXD3Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_CRSCntl                              0x01CC

    #define     BA_Gbl_ET_CRSCntl_SLP_DI                       0x01CC
    #define     B16Gbl_ET_CRSCntl_SLP_DI                       0x01CC
    #define   LSb32Gbl_ET_CRSCntl_SLP_DI                          0
    #define   LSb16Gbl_ET_CRSCntl_SLP_DI                          0
    #define       bGbl_ET_CRSCntl_SLP_DI                       1
    #define   MSK32Gbl_ET_CRSCntl_SLP_DI                          0x00000001

    #define     BA_Gbl_ET_CRSCntl_PU_EN                        0x01CC
    #define     B16Gbl_ET_CRSCntl_PU_EN                        0x01CC
    #define   LSb32Gbl_ET_CRSCntl_PU_EN                           1
    #define   LSb16Gbl_ET_CRSCntl_PU_EN                           1
    #define       bGbl_ET_CRSCntl_PU_EN                        1
    #define   MSK32Gbl_ET_CRSCntl_PU_EN                           0x00000002

    #define     BA_Gbl_ET_CRSCntl_PD_EN                        0x01CC
    #define     B16Gbl_ET_CRSCntl_PD_EN                        0x01CC
    #define   LSb32Gbl_ET_CRSCntl_PD_EN                           2
    #define   LSb16Gbl_ET_CRSCntl_PD_EN                           2
    #define       bGbl_ET_CRSCntl_PD_EN                        1
    #define   MSK32Gbl_ET_CRSCntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_COLCntl                              0x01D0

    #define     BA_Gbl_ET_COLCntl_SLP_DI                       0x01D0
    #define     B16Gbl_ET_COLCntl_SLP_DI                       0x01D0
    #define   LSb32Gbl_ET_COLCntl_SLP_DI                          0
    #define   LSb16Gbl_ET_COLCntl_SLP_DI                          0
    #define       bGbl_ET_COLCntl_SLP_DI                       1
    #define   MSK32Gbl_ET_COLCntl_SLP_DI                          0x00000001

    #define     BA_Gbl_ET_COLCntl_PU_EN                        0x01D0
    #define     B16Gbl_ET_COLCntl_PU_EN                        0x01D0
    #define   LSb32Gbl_ET_COLCntl_PU_EN                           1
    #define   LSb16Gbl_ET_COLCntl_PU_EN                           1
    #define       bGbl_ET_COLCntl_PU_EN                        1
    #define   MSK32Gbl_ET_COLCntl_PU_EN                           0x00000002

    #define     BA_Gbl_ET_COLCntl_PD_EN                        0x01D0
    #define     B16Gbl_ET_COLCntl_PD_EN                        0x01D0
    #define   LSb32Gbl_ET_COLCntl_PD_EN                           2
    #define   LSb16Gbl_ET_COLCntl_PD_EN                           2
    #define       bGbl_ET_COLCntl_PD_EN                        1
    #define   MSK32Gbl_ET_COLCntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_RXDVCntl                             0x01D4

    #define     BA_Gbl_ET_RXDVCntl_SLP_DI                      0x01D4
    #define     B16Gbl_ET_RXDVCntl_SLP_DI                      0x01D4
    #define   LSb32Gbl_ET_RXDVCntl_SLP_DI                         0
    #define   LSb16Gbl_ET_RXDVCntl_SLP_DI                         0
    #define       bGbl_ET_RXDVCntl_SLP_DI                      1
    #define   MSK32Gbl_ET_RXDVCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_RXDVCntl_PU_EN                       0x01D4
    #define     B16Gbl_ET_RXDVCntl_PU_EN                       0x01D4
    #define   LSb32Gbl_ET_RXDVCntl_PU_EN                          1
    #define   LSb16Gbl_ET_RXDVCntl_PU_EN                          1
    #define       bGbl_ET_RXDVCntl_PU_EN                       1
    #define   MSK32Gbl_ET_RXDVCntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_RXDVCntl_PD_EN                       0x01D4
    #define     B16Gbl_ET_RXDVCntl_PD_EN                       0x01D4
    #define   LSb32Gbl_ET_RXDVCntl_PD_EN                          2
    #define   LSb16Gbl_ET_RXDVCntl_PD_EN                          2
    #define       bGbl_ET_RXDVCntl_PD_EN                       1
    #define   MSK32Gbl_ET_RXDVCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_RXERCntl                             0x01D8

    #define     BA_Gbl_ET_RXERCntl_SLP_DI                      0x01D8
    #define     B16Gbl_ET_RXERCntl_SLP_DI                      0x01D8
    #define   LSb32Gbl_ET_RXERCntl_SLP_DI                         0
    #define   LSb16Gbl_ET_RXERCntl_SLP_DI                         0
    #define       bGbl_ET_RXERCntl_SLP_DI                      1
    #define   MSK32Gbl_ET_RXERCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_RXERCntl_PU_EN                       0x01D8
    #define     B16Gbl_ET_RXERCntl_PU_EN                       0x01D8
    #define   LSb32Gbl_ET_RXERCntl_PU_EN                          1
    #define   LSb16Gbl_ET_RXERCntl_PU_EN                          1
    #define       bGbl_ET_RXERCntl_PU_EN                       1
    #define   MSK32Gbl_ET_RXERCntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_RXERCntl_PD_EN                       0x01D8
    #define     B16Gbl_ET_RXERCntl_PD_EN                       0x01D8
    #define   LSb32Gbl_ET_RXERCntl_PD_EN                          2
    #define   LSb16Gbl_ET_RXERCntl_PD_EN                          2
    #define       bGbl_ET_RXERCntl_PD_EN                       1
    #define   MSK32Gbl_ET_RXERCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_MDCCntl                              0x01DC

    #define     BA_Gbl_ET_MDCCntl_SLP_DI                       0x01DC
    #define     B16Gbl_ET_MDCCntl_SLP_DI                       0x01DC
    #define   LSb32Gbl_ET_MDCCntl_SLP_DI                          0
    #define   LSb16Gbl_ET_MDCCntl_SLP_DI                          0
    #define       bGbl_ET_MDCCntl_SLP_DI                       1
    #define   MSK32Gbl_ET_MDCCntl_SLP_DI                          0x00000001

    #define     BA_Gbl_ET_MDCCntl_PU_EN                        0x01DC
    #define     B16Gbl_ET_MDCCntl_PU_EN                        0x01DC
    #define   LSb32Gbl_ET_MDCCntl_PU_EN                           1
    #define   LSb16Gbl_ET_MDCCntl_PU_EN                           1
    #define       bGbl_ET_MDCCntl_PU_EN                        1
    #define   MSK32Gbl_ET_MDCCntl_PU_EN                           0x00000002

    #define     BA_Gbl_ET_MDCCntl_PD_EN                        0x01DC
    #define     B16Gbl_ET_MDCCntl_PD_EN                        0x01DC
    #define   LSb32Gbl_ET_MDCCntl_PD_EN                           2
    #define   LSb16Gbl_ET_MDCCntl_PD_EN                           2
    #define       bGbl_ET_MDCCntl_PD_EN                        1
    #define   MSK32Gbl_ET_MDCCntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_MDIOCntl                             0x01E0

    #define     BA_Gbl_ET_MDIOCntl_SLP_DI                      0x01E0
    #define     B16Gbl_ET_MDIOCntl_SLP_DI                      0x01E0
    #define   LSb32Gbl_ET_MDIOCntl_SLP_DI                         0
    #define   LSb16Gbl_ET_MDIOCntl_SLP_DI                         0
    #define       bGbl_ET_MDIOCntl_SLP_DI                      1
    #define   MSK32Gbl_ET_MDIOCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_ET_MDIOCntl_PU_EN                       0x01E0
    #define     B16Gbl_ET_MDIOCntl_PU_EN                       0x01E0
    #define   LSb32Gbl_ET_MDIOCntl_PU_EN                          1
    #define   LSb16Gbl_ET_MDIOCntl_PU_EN                          1
    #define       bGbl_ET_MDIOCntl_PU_EN                       1
    #define   MSK32Gbl_ET_MDIOCntl_PU_EN                          0x00000002

    #define     BA_Gbl_ET_MDIOCntl_PD_EN                       0x01E0
    #define     B16Gbl_ET_MDIOCntl_PD_EN                       0x01E0
    #define   LSb32Gbl_ET_MDIOCntl_PD_EN                          2
    #define   LSb16Gbl_ET_MDIOCntl_PD_EN                          2
    #define       bGbl_ET_MDIOCntl_PD_EN                       1
    #define   MSK32Gbl_ET_MDIOCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ET_RCLKOCntl                            0x01E4

    #define     BA_Gbl_ET_RCLKOCntl_SLP_DI                     0x01E4
    #define     B16Gbl_ET_RCLKOCntl_SLP_DI                     0x01E4
    #define   LSb32Gbl_ET_RCLKOCntl_SLP_DI                        0
    #define   LSb16Gbl_ET_RCLKOCntl_SLP_DI                        0
    #define       bGbl_ET_RCLKOCntl_SLP_DI                     1
    #define   MSK32Gbl_ET_RCLKOCntl_SLP_DI                        0x00000001

    #define     BA_Gbl_ET_RCLKOCntl_PU_EN                      0x01E4
    #define     B16Gbl_ET_RCLKOCntl_PU_EN                      0x01E4
    #define   LSb32Gbl_ET_RCLKOCntl_PU_EN                         1
    #define   LSb16Gbl_ET_RCLKOCntl_PU_EN                         1
    #define       bGbl_ET_RCLKOCntl_PU_EN                      1
    #define   MSK32Gbl_ET_RCLKOCntl_PU_EN                         0x00000002

    #define     BA_Gbl_ET_RCLKOCntl_PD_EN                      0x01E4
    #define     B16Gbl_ET_RCLKOCntl_PD_EN                      0x01E4
    #define   LSb32Gbl_ET_RCLKOCntl_PD_EN                         2
    #define   LSb16Gbl_ET_RCLKOCntl_PD_EN                         2
    #define       bGbl_ET_RCLKOCntl_PD_EN                      1
    #define   MSK32Gbl_ET_RCLKOCntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TW0_SCLCntl                             0x01E8

    #define     BA_Gbl_TW0_SCLCntl_SLP_DI                      0x01E8
    #define     B16Gbl_TW0_SCLCntl_SLP_DI                      0x01E8
    #define   LSb32Gbl_TW0_SCLCntl_SLP_DI                         0
    #define   LSb16Gbl_TW0_SCLCntl_SLP_DI                         0
    #define       bGbl_TW0_SCLCntl_SLP_DI                      1
    #define   MSK32Gbl_TW0_SCLCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TW0_SCLCntl_PU_EN                       0x01E8
    #define     B16Gbl_TW0_SCLCntl_PU_EN                       0x01E8
    #define   LSb32Gbl_TW0_SCLCntl_PU_EN                          1
    #define   LSb16Gbl_TW0_SCLCntl_PU_EN                          1
    #define       bGbl_TW0_SCLCntl_PU_EN                       1
    #define   MSK32Gbl_TW0_SCLCntl_PU_EN                          0x00000002

    #define     BA_Gbl_TW0_SCLCntl_PD_EN                       0x01E8
    #define     B16Gbl_TW0_SCLCntl_PD_EN                       0x01E8
    #define   LSb32Gbl_TW0_SCLCntl_PD_EN                          2
    #define   LSb16Gbl_TW0_SCLCntl_PD_EN                          2
    #define       bGbl_TW0_SCLCntl_PD_EN                       1
    #define   MSK32Gbl_TW0_SCLCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TW0_SDACntl                             0x01EC

    #define     BA_Gbl_TW0_SDACntl_SLP_DI                      0x01EC
    #define     B16Gbl_TW0_SDACntl_SLP_DI                      0x01EC
    #define   LSb32Gbl_TW0_SDACntl_SLP_DI                         0
    #define   LSb16Gbl_TW0_SDACntl_SLP_DI                         0
    #define       bGbl_TW0_SDACntl_SLP_DI                      1
    #define   MSK32Gbl_TW0_SDACntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TW0_SDACntl_PU_EN                       0x01EC
    #define     B16Gbl_TW0_SDACntl_PU_EN                       0x01EC
    #define   LSb32Gbl_TW0_SDACntl_PU_EN                          1
    #define   LSb16Gbl_TW0_SDACntl_PU_EN                          1
    #define       bGbl_TW0_SDACntl_PU_EN                       1
    #define   MSK32Gbl_TW0_SDACntl_PU_EN                          0x00000002

    #define     BA_Gbl_TW0_SDACntl_PD_EN                       0x01EC
    #define     B16Gbl_TW0_SDACntl_PD_EN                       0x01EC
    #define   LSb32Gbl_TW0_SDACntl_PD_EN                          2
    #define   LSb16Gbl_TW0_SDACntl_PD_EN                          2
    #define       bGbl_TW0_SDACntl_PD_EN                       1
    #define   MSK32Gbl_TW0_SDACntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TW1_SCLCntl                             0x01F0

    #define     BA_Gbl_TW1_SCLCntl_SLP_DI                      0x01F0
    #define     B16Gbl_TW1_SCLCntl_SLP_DI                      0x01F0
    #define   LSb32Gbl_TW1_SCLCntl_SLP_DI                         0
    #define   LSb16Gbl_TW1_SCLCntl_SLP_DI                         0
    #define       bGbl_TW1_SCLCntl_SLP_DI                      1
    #define   MSK32Gbl_TW1_SCLCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TW1_SCLCntl_PU_EN                       0x01F0
    #define     B16Gbl_TW1_SCLCntl_PU_EN                       0x01F0
    #define   LSb32Gbl_TW1_SCLCntl_PU_EN                          1
    #define   LSb16Gbl_TW1_SCLCntl_PU_EN                          1
    #define       bGbl_TW1_SCLCntl_PU_EN                       1
    #define   MSK32Gbl_TW1_SCLCntl_PU_EN                          0x00000002

    #define     BA_Gbl_TW1_SCLCntl_PD_EN                       0x01F0
    #define     B16Gbl_TW1_SCLCntl_PD_EN                       0x01F0
    #define   LSb32Gbl_TW1_SCLCntl_PD_EN                          2
    #define   LSb16Gbl_TW1_SCLCntl_PD_EN                          2
    #define       bGbl_TW1_SCLCntl_PD_EN                       1
    #define   MSK32Gbl_TW1_SCLCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TW1_SDACntl                             0x01F4

    #define     BA_Gbl_TW1_SDACntl_SLP_DI                      0x01F4
    #define     B16Gbl_TW1_SDACntl_SLP_DI                      0x01F4
    #define   LSb32Gbl_TW1_SDACntl_SLP_DI                         0
    #define   LSb16Gbl_TW1_SDACntl_SLP_DI                         0
    #define       bGbl_TW1_SDACntl_SLP_DI                      1
    #define   MSK32Gbl_TW1_SDACntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TW1_SDACntl_PU_EN                       0x01F4
    #define     B16Gbl_TW1_SDACntl_PU_EN                       0x01F4
    #define   LSb32Gbl_TW1_SDACntl_PU_EN                          1
    #define   LSb16Gbl_TW1_SDACntl_PU_EN                          1
    #define       bGbl_TW1_SDACntl_PU_EN                       1
    #define   MSK32Gbl_TW1_SDACntl_PU_EN                          0x00000002

    #define     BA_Gbl_TW1_SDACntl_PD_EN                       0x01F4
    #define     B16Gbl_TW1_SDACntl_PD_EN                       0x01F4
    #define   LSb32Gbl_TW1_SDACntl_PD_EN                          2
    #define   LSb16Gbl_TW1_SDACntl_PD_EN                          2
    #define       bGbl_TW1_SDACntl_PD_EN                       1
    #define   MSK32Gbl_TW1_SDACntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_SPI1_SS0nCntl                           0x01F8

    #define     BA_Gbl_SPI1_SS0nCntl_SLP_DI                    0x01F8
    #define     B16Gbl_SPI1_SS0nCntl_SLP_DI                    0x01F8
    #define   LSb32Gbl_SPI1_SS0nCntl_SLP_DI                       0
    #define   LSb16Gbl_SPI1_SS0nCntl_SLP_DI                       0
    #define       bGbl_SPI1_SS0nCntl_SLP_DI                    1
    #define   MSK32Gbl_SPI1_SS0nCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_SPI1_SS0nCntl_PU_EN                     0x01F8
    #define     B16Gbl_SPI1_SS0nCntl_PU_EN                     0x01F8
    #define   LSb32Gbl_SPI1_SS0nCntl_PU_EN                        1
    #define   LSb16Gbl_SPI1_SS0nCntl_PU_EN                        1
    #define       bGbl_SPI1_SS0nCntl_PU_EN                     1
    #define   MSK32Gbl_SPI1_SS0nCntl_PU_EN                        0x00000002

    #define     BA_Gbl_SPI1_SS0nCntl_PD_EN                     0x01F8
    #define     B16Gbl_SPI1_SS0nCntl_PD_EN                     0x01F8
    #define   LSb32Gbl_SPI1_SS0nCntl_PD_EN                        2
    #define   LSb16Gbl_SPI1_SS0nCntl_PD_EN                        2
    #define       bGbl_SPI1_SS0nCntl_PD_EN                     1
    #define   MSK32Gbl_SPI1_SS0nCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_SPI1_SS1nCntl                           0x01FC

    #define     BA_Gbl_SPI1_SS1nCntl_SLP_DI                    0x01FC
    #define     B16Gbl_SPI1_SS1nCntl_SLP_DI                    0x01FC
    #define   LSb32Gbl_SPI1_SS1nCntl_SLP_DI                       0
    #define   LSb16Gbl_SPI1_SS1nCntl_SLP_DI                       0
    #define       bGbl_SPI1_SS1nCntl_SLP_DI                    1
    #define   MSK32Gbl_SPI1_SS1nCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_SPI1_SS1nCntl_PU_EN                     0x01FC
    #define     B16Gbl_SPI1_SS1nCntl_PU_EN                     0x01FC
    #define   LSb32Gbl_SPI1_SS1nCntl_PU_EN                        1
    #define   LSb16Gbl_SPI1_SS1nCntl_PU_EN                        1
    #define       bGbl_SPI1_SS1nCntl_PU_EN                     1
    #define   MSK32Gbl_SPI1_SS1nCntl_PU_EN                        0x00000002

    #define     BA_Gbl_SPI1_SS1nCntl_PD_EN                     0x01FC
    #define     B16Gbl_SPI1_SS1nCntl_PD_EN                     0x01FC
    #define   LSb32Gbl_SPI1_SS1nCntl_PD_EN                        2
    #define   LSb16Gbl_SPI1_SS1nCntl_PD_EN                        2
    #define       bGbl_SPI1_SS1nCntl_PD_EN                     1
    #define   MSK32Gbl_SPI1_SS1nCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_SPI1_SS2nCntl                           0x0200

    #define     BA_Gbl_SPI1_SS2nCntl_SLP_DI                    0x0200
    #define     B16Gbl_SPI1_SS2nCntl_SLP_DI                    0x0200
    #define   LSb32Gbl_SPI1_SS2nCntl_SLP_DI                       0
    #define   LSb16Gbl_SPI1_SS2nCntl_SLP_DI                       0
    #define       bGbl_SPI1_SS2nCntl_SLP_DI                    1
    #define   MSK32Gbl_SPI1_SS2nCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_SPI1_SS2nCntl_PU_EN                     0x0200
    #define     B16Gbl_SPI1_SS2nCntl_PU_EN                     0x0200
    #define   LSb32Gbl_SPI1_SS2nCntl_PU_EN                        1
    #define   LSb16Gbl_SPI1_SS2nCntl_PU_EN                        1
    #define       bGbl_SPI1_SS2nCntl_PU_EN                     1
    #define   MSK32Gbl_SPI1_SS2nCntl_PU_EN                        0x00000002

    #define     BA_Gbl_SPI1_SS2nCntl_PD_EN                     0x0200
    #define     B16Gbl_SPI1_SS2nCntl_PD_EN                     0x0200
    #define   LSb32Gbl_SPI1_SS2nCntl_PD_EN                        2
    #define   LSb16Gbl_SPI1_SS2nCntl_PD_EN                        2
    #define       bGbl_SPI1_SS2nCntl_PD_EN                     1
    #define   MSK32Gbl_SPI1_SS2nCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_SPI1_SS3nCntl                           0x0204

    #define     BA_Gbl_SPI1_SS3nCntl_SLP_DI                    0x0204
    #define     B16Gbl_SPI1_SS3nCntl_SLP_DI                    0x0204
    #define   LSb32Gbl_SPI1_SS3nCntl_SLP_DI                       0
    #define   LSb16Gbl_SPI1_SS3nCntl_SLP_DI                       0
    #define       bGbl_SPI1_SS3nCntl_SLP_DI                    1
    #define   MSK32Gbl_SPI1_SS3nCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_SPI1_SS3nCntl_PU_EN                     0x0204
    #define     B16Gbl_SPI1_SS3nCntl_PU_EN                     0x0204
    #define   LSb32Gbl_SPI1_SS3nCntl_PU_EN                        1
    #define   LSb16Gbl_SPI1_SS3nCntl_PU_EN                        1
    #define       bGbl_SPI1_SS3nCntl_PU_EN                     1
    #define   MSK32Gbl_SPI1_SS3nCntl_PU_EN                        0x00000002

    #define     BA_Gbl_SPI1_SS3nCntl_PD_EN                     0x0204
    #define     B16Gbl_SPI1_SS3nCntl_PD_EN                     0x0204
    #define   LSb32Gbl_SPI1_SS3nCntl_PD_EN                        2
    #define   LSb16Gbl_SPI1_SS3nCntl_PD_EN                        2
    #define       bGbl_SPI1_SS3nCntl_PD_EN                     1
    #define   MSK32Gbl_SPI1_SS3nCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_SPI1_SCLKCntl                           0x0208

    #define     BA_Gbl_SPI1_SCLKCntl_SLP_DI                    0x0208
    #define     B16Gbl_SPI1_SCLKCntl_SLP_DI                    0x0208
    #define   LSb32Gbl_SPI1_SCLKCntl_SLP_DI                       0
    #define   LSb16Gbl_SPI1_SCLKCntl_SLP_DI                       0
    #define       bGbl_SPI1_SCLKCntl_SLP_DI                    1
    #define   MSK32Gbl_SPI1_SCLKCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_SPI1_SCLKCntl_PU_EN                     0x0208
    #define     B16Gbl_SPI1_SCLKCntl_PU_EN                     0x0208
    #define   LSb32Gbl_SPI1_SCLKCntl_PU_EN                        1
    #define   LSb16Gbl_SPI1_SCLKCntl_PU_EN                        1
    #define       bGbl_SPI1_SCLKCntl_PU_EN                     1
    #define   MSK32Gbl_SPI1_SCLKCntl_PU_EN                        0x00000002

    #define     BA_Gbl_SPI1_SCLKCntl_PD_EN                     0x0208
    #define     B16Gbl_SPI1_SCLKCntl_PD_EN                     0x0208
    #define   LSb32Gbl_SPI1_SCLKCntl_PD_EN                        2
    #define   LSb16Gbl_SPI1_SCLKCntl_PD_EN                        2
    #define       bGbl_SPI1_SCLKCntl_PD_EN                     1
    #define   MSK32Gbl_SPI1_SCLKCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_SPI1_SDOCntl                            0x020C

    #define     BA_Gbl_SPI1_SDOCntl_SLP_DI                     0x020C
    #define     B16Gbl_SPI1_SDOCntl_SLP_DI                     0x020C
    #define   LSb32Gbl_SPI1_SDOCntl_SLP_DI                        0
    #define   LSb16Gbl_SPI1_SDOCntl_SLP_DI                        0
    #define       bGbl_SPI1_SDOCntl_SLP_DI                     1
    #define   MSK32Gbl_SPI1_SDOCntl_SLP_DI                        0x00000001

    #define     BA_Gbl_SPI1_SDOCntl_PU_EN                      0x020C
    #define     B16Gbl_SPI1_SDOCntl_PU_EN                      0x020C
    #define   LSb32Gbl_SPI1_SDOCntl_PU_EN                         1
    #define   LSb16Gbl_SPI1_SDOCntl_PU_EN                         1
    #define       bGbl_SPI1_SDOCntl_PU_EN                      1
    #define   MSK32Gbl_SPI1_SDOCntl_PU_EN                         0x00000002

    #define     BA_Gbl_SPI1_SDOCntl_PD_EN                      0x020C
    #define     B16Gbl_SPI1_SDOCntl_PD_EN                      0x020C
    #define   LSb32Gbl_SPI1_SDOCntl_PD_EN                         2
    #define   LSb16Gbl_SPI1_SDOCntl_PD_EN                         2
    #define       bGbl_SPI1_SDOCntl_PD_EN                      1
    #define   MSK32Gbl_SPI1_SDOCntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_SPI1_SDICntl                            0x0210

    #define     BA_Gbl_SPI1_SDICntl_SLP_DI                     0x0210
    #define     B16Gbl_SPI1_SDICntl_SLP_DI                     0x0210
    #define   LSb32Gbl_SPI1_SDICntl_SLP_DI                        0
    #define   LSb16Gbl_SPI1_SDICntl_SLP_DI                        0
    #define       bGbl_SPI1_SDICntl_SLP_DI                     1
    #define   MSK32Gbl_SPI1_SDICntl_SLP_DI                        0x00000001

    #define     BA_Gbl_SPI1_SDICntl_PU_EN                      0x0210
    #define     B16Gbl_SPI1_SDICntl_PU_EN                      0x0210
    #define   LSb32Gbl_SPI1_SDICntl_PU_EN                         1
    #define   LSb16Gbl_SPI1_SDICntl_PU_EN                         1
    #define       bGbl_SPI1_SDICntl_PU_EN                      1
    #define   MSK32Gbl_SPI1_SDICntl_PU_EN                         0x00000002

    #define     BA_Gbl_SPI1_SDICntl_PD_EN                      0x0210
    #define     B16Gbl_SPI1_SDICntl_PD_EN                      0x0210
    #define   LSb32Gbl_SPI1_SDICntl_PD_EN                         2
    #define   LSb16Gbl_SPI1_SDICntl_PD_EN                         2
    #define       bGbl_SPI1_SDICntl_PD_EN                      1
    #define   MSK32Gbl_SPI1_SDICntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_CLKCntl                             0x0214

    #define     BA_Gbl_TS0_CLKCntl_SLP_DI                      0x0214
    #define     B16Gbl_TS0_CLKCntl_SLP_DI                      0x0214
    #define   LSb32Gbl_TS0_CLKCntl_SLP_DI                         0
    #define   LSb16Gbl_TS0_CLKCntl_SLP_DI                         0
    #define       bGbl_TS0_CLKCntl_SLP_DI                      1
    #define   MSK32Gbl_TS0_CLKCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TS0_CLKCntl_PU_EN                       0x0214
    #define     B16Gbl_TS0_CLKCntl_PU_EN                       0x0214
    #define   LSb32Gbl_TS0_CLKCntl_PU_EN                          1
    #define   LSb16Gbl_TS0_CLKCntl_PU_EN                          1
    #define       bGbl_TS0_CLKCntl_PU_EN                       1
    #define   MSK32Gbl_TS0_CLKCntl_PU_EN                          0x00000002

    #define     BA_Gbl_TS0_CLKCntl_PD_EN                       0x0214
    #define     B16Gbl_TS0_CLKCntl_PD_EN                       0x0214
    #define   LSb32Gbl_TS0_CLKCntl_PD_EN                          2
    #define   LSb16Gbl_TS0_CLKCntl_PD_EN                          2
    #define       bGbl_TS0_CLKCntl_PD_EN                       1
    #define   MSK32Gbl_TS0_CLKCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_SOPCntl                             0x0218

    #define     BA_Gbl_TS0_SOPCntl_SLP_DI                      0x0218
    #define     B16Gbl_TS0_SOPCntl_SLP_DI                      0x0218
    #define   LSb32Gbl_TS0_SOPCntl_SLP_DI                         0
    #define   LSb16Gbl_TS0_SOPCntl_SLP_DI                         0
    #define       bGbl_TS0_SOPCntl_SLP_DI                      1
    #define   MSK32Gbl_TS0_SOPCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TS0_SOPCntl_PU_EN                       0x0218
    #define     B16Gbl_TS0_SOPCntl_PU_EN                       0x0218
    #define   LSb32Gbl_TS0_SOPCntl_PU_EN                          1
    #define   LSb16Gbl_TS0_SOPCntl_PU_EN                          1
    #define       bGbl_TS0_SOPCntl_PU_EN                       1
    #define   MSK32Gbl_TS0_SOPCntl_PU_EN                          0x00000002

    #define     BA_Gbl_TS0_SOPCntl_PD_EN                       0x0218
    #define     B16Gbl_TS0_SOPCntl_PD_EN                       0x0218
    #define   LSb32Gbl_TS0_SOPCntl_PD_EN                          2
    #define   LSb16Gbl_TS0_SOPCntl_PD_EN                          2
    #define       bGbl_TS0_SOPCntl_PD_EN                       1
    #define   MSK32Gbl_TS0_SOPCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_ERRORCntl                           0x021C

    #define     BA_Gbl_TS0_ERRORCntl_SLP_DI                    0x021C
    #define     B16Gbl_TS0_ERRORCntl_SLP_DI                    0x021C
    #define   LSb32Gbl_TS0_ERRORCntl_SLP_DI                       0
    #define   LSb16Gbl_TS0_ERRORCntl_SLP_DI                       0
    #define       bGbl_TS0_ERRORCntl_SLP_DI                    1
    #define   MSK32Gbl_TS0_ERRORCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_TS0_ERRORCntl_PU_EN                     0x021C
    #define     B16Gbl_TS0_ERRORCntl_PU_EN                     0x021C
    #define   LSb32Gbl_TS0_ERRORCntl_PU_EN                        1
    #define   LSb16Gbl_TS0_ERRORCntl_PU_EN                        1
    #define       bGbl_TS0_ERRORCntl_PU_EN                     1
    #define   MSK32Gbl_TS0_ERRORCntl_PU_EN                        0x00000002

    #define     BA_Gbl_TS0_ERRORCntl_PD_EN                     0x021C
    #define     B16Gbl_TS0_ERRORCntl_PD_EN                     0x021C
    #define   LSb32Gbl_TS0_ERRORCntl_PD_EN                        2
    #define   LSb16Gbl_TS0_ERRORCntl_PD_EN                        2
    #define       bGbl_TS0_ERRORCntl_PD_EN                     1
    #define   MSK32Gbl_TS0_ERRORCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_VALDCntl                            0x0220

    #define     BA_Gbl_TS0_VALDCntl_SLP_DI                     0x0220
    #define     B16Gbl_TS0_VALDCntl_SLP_DI                     0x0220
    #define   LSb32Gbl_TS0_VALDCntl_SLP_DI                        0
    #define   LSb16Gbl_TS0_VALDCntl_SLP_DI                        0
    #define       bGbl_TS0_VALDCntl_SLP_DI                     1
    #define   MSK32Gbl_TS0_VALDCntl_SLP_DI                        0x00000001

    #define     BA_Gbl_TS0_VALDCntl_PU_EN                      0x0220
    #define     B16Gbl_TS0_VALDCntl_PU_EN                      0x0220
    #define   LSb32Gbl_TS0_VALDCntl_PU_EN                         1
    #define   LSb16Gbl_TS0_VALDCntl_PU_EN                         1
    #define       bGbl_TS0_VALDCntl_PU_EN                      1
    #define   MSK32Gbl_TS0_VALDCntl_PU_EN                         0x00000002

    #define     BA_Gbl_TS0_VALDCntl_PD_EN                      0x0220
    #define     B16Gbl_TS0_VALDCntl_PD_EN                      0x0220
    #define   LSb32Gbl_TS0_VALDCntl_PD_EN                         2
    #define   LSb16Gbl_TS0_VALDCntl_PD_EN                         2
    #define       bGbl_TS0_VALDCntl_PD_EN                      1
    #define   MSK32Gbl_TS0_VALDCntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_PD0Cntl                             0x0224

    #define     BA_Gbl_TS0_PD0Cntl_SLP_DI                      0x0224
    #define     B16Gbl_TS0_PD0Cntl_SLP_DI                      0x0224
    #define   LSb32Gbl_TS0_PD0Cntl_SLP_DI                         0
    #define   LSb16Gbl_TS0_PD0Cntl_SLP_DI                         0
    #define       bGbl_TS0_PD0Cntl_SLP_DI                      1
    #define   MSK32Gbl_TS0_PD0Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TS0_PD0Cntl_PU_EN                       0x0224
    #define     B16Gbl_TS0_PD0Cntl_PU_EN                       0x0224
    #define   LSb32Gbl_TS0_PD0Cntl_PU_EN                          1
    #define   LSb16Gbl_TS0_PD0Cntl_PU_EN                          1
    #define       bGbl_TS0_PD0Cntl_PU_EN                       1
    #define   MSK32Gbl_TS0_PD0Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_TS0_PD0Cntl_PD_EN                       0x0224
    #define     B16Gbl_TS0_PD0Cntl_PD_EN                       0x0224
    #define   LSb32Gbl_TS0_PD0Cntl_PD_EN                          2
    #define   LSb16Gbl_TS0_PD0Cntl_PD_EN                          2
    #define       bGbl_TS0_PD0Cntl_PD_EN                       1
    #define   MSK32Gbl_TS0_PD0Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_PD1Cntl                             0x0228

    #define     BA_Gbl_TS0_PD1Cntl_SLP_DI                      0x0228
    #define     B16Gbl_TS0_PD1Cntl_SLP_DI                      0x0228
    #define   LSb32Gbl_TS0_PD1Cntl_SLP_DI                         0
    #define   LSb16Gbl_TS0_PD1Cntl_SLP_DI                         0
    #define       bGbl_TS0_PD1Cntl_SLP_DI                      1
    #define   MSK32Gbl_TS0_PD1Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TS0_PD1Cntl_PU_EN                       0x0228
    #define     B16Gbl_TS0_PD1Cntl_PU_EN                       0x0228
    #define   LSb32Gbl_TS0_PD1Cntl_PU_EN                          1
    #define   LSb16Gbl_TS0_PD1Cntl_PU_EN                          1
    #define       bGbl_TS0_PD1Cntl_PU_EN                       1
    #define   MSK32Gbl_TS0_PD1Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_TS0_PD1Cntl_PD_EN                       0x0228
    #define     B16Gbl_TS0_PD1Cntl_PD_EN                       0x0228
    #define   LSb32Gbl_TS0_PD1Cntl_PD_EN                          2
    #define   LSb16Gbl_TS0_PD1Cntl_PD_EN                          2
    #define       bGbl_TS0_PD1Cntl_PD_EN                       1
    #define   MSK32Gbl_TS0_PD1Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_PD2Cntl                             0x022C

    #define     BA_Gbl_TS0_PD2Cntl_SLP_DI                      0x022C
    #define     B16Gbl_TS0_PD2Cntl_SLP_DI                      0x022C
    #define   LSb32Gbl_TS0_PD2Cntl_SLP_DI                         0
    #define   LSb16Gbl_TS0_PD2Cntl_SLP_DI                         0
    #define       bGbl_TS0_PD2Cntl_SLP_DI                      1
    #define   MSK32Gbl_TS0_PD2Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TS0_PD2Cntl_PU_EN                       0x022C
    #define     B16Gbl_TS0_PD2Cntl_PU_EN                       0x022C
    #define   LSb32Gbl_TS0_PD2Cntl_PU_EN                          1
    #define   LSb16Gbl_TS0_PD2Cntl_PU_EN                          1
    #define       bGbl_TS0_PD2Cntl_PU_EN                       1
    #define   MSK32Gbl_TS0_PD2Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_TS0_PD2Cntl_PD_EN                       0x022C
    #define     B16Gbl_TS0_PD2Cntl_PD_EN                       0x022C
    #define   LSb32Gbl_TS0_PD2Cntl_PD_EN                          2
    #define   LSb16Gbl_TS0_PD2Cntl_PD_EN                          2
    #define       bGbl_TS0_PD2Cntl_PD_EN                       1
    #define   MSK32Gbl_TS0_PD2Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_PD3Cntl                             0x0230

    #define     BA_Gbl_TS0_PD3Cntl_SLP_DI                      0x0230
    #define     B16Gbl_TS0_PD3Cntl_SLP_DI                      0x0230
    #define   LSb32Gbl_TS0_PD3Cntl_SLP_DI                         0
    #define   LSb16Gbl_TS0_PD3Cntl_SLP_DI                         0
    #define       bGbl_TS0_PD3Cntl_SLP_DI                      1
    #define   MSK32Gbl_TS0_PD3Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TS0_PD3Cntl_PU_EN                       0x0230
    #define     B16Gbl_TS0_PD3Cntl_PU_EN                       0x0230
    #define   LSb32Gbl_TS0_PD3Cntl_PU_EN                          1
    #define   LSb16Gbl_TS0_PD3Cntl_PU_EN                          1
    #define       bGbl_TS0_PD3Cntl_PU_EN                       1
    #define   MSK32Gbl_TS0_PD3Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_TS0_PD3Cntl_PD_EN                       0x0230
    #define     B16Gbl_TS0_PD3Cntl_PD_EN                       0x0230
    #define   LSb32Gbl_TS0_PD3Cntl_PD_EN                          2
    #define   LSb16Gbl_TS0_PD3Cntl_PD_EN                          2
    #define       bGbl_TS0_PD3Cntl_PD_EN                       1
    #define   MSK32Gbl_TS0_PD3Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_PD4Cntl                             0x0234

    #define     BA_Gbl_TS0_PD4Cntl_SLP_DI                      0x0234
    #define     B16Gbl_TS0_PD4Cntl_SLP_DI                      0x0234
    #define   LSb32Gbl_TS0_PD4Cntl_SLP_DI                         0
    #define   LSb16Gbl_TS0_PD4Cntl_SLP_DI                         0
    #define       bGbl_TS0_PD4Cntl_SLP_DI                      1
    #define   MSK32Gbl_TS0_PD4Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TS0_PD4Cntl_PU_EN                       0x0234
    #define     B16Gbl_TS0_PD4Cntl_PU_EN                       0x0234
    #define   LSb32Gbl_TS0_PD4Cntl_PU_EN                          1
    #define   LSb16Gbl_TS0_PD4Cntl_PU_EN                          1
    #define       bGbl_TS0_PD4Cntl_PU_EN                       1
    #define   MSK32Gbl_TS0_PD4Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_TS0_PD4Cntl_PD_EN                       0x0234
    #define     B16Gbl_TS0_PD4Cntl_PD_EN                       0x0234
    #define   LSb32Gbl_TS0_PD4Cntl_PD_EN                          2
    #define   LSb16Gbl_TS0_PD4Cntl_PD_EN                          2
    #define       bGbl_TS0_PD4Cntl_PD_EN                       1
    #define   MSK32Gbl_TS0_PD4Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_PD5Cntl                             0x0238

    #define     BA_Gbl_TS0_PD5Cntl_SLP_DI                      0x0238
    #define     B16Gbl_TS0_PD5Cntl_SLP_DI                      0x0238
    #define   LSb32Gbl_TS0_PD5Cntl_SLP_DI                         0
    #define   LSb16Gbl_TS0_PD5Cntl_SLP_DI                         0
    #define       bGbl_TS0_PD5Cntl_SLP_DI                      1
    #define   MSK32Gbl_TS0_PD5Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TS0_PD5Cntl_PU_EN                       0x0238
    #define     B16Gbl_TS0_PD5Cntl_PU_EN                       0x0238
    #define   LSb32Gbl_TS0_PD5Cntl_PU_EN                          1
    #define   LSb16Gbl_TS0_PD5Cntl_PU_EN                          1
    #define       bGbl_TS0_PD5Cntl_PU_EN                       1
    #define   MSK32Gbl_TS0_PD5Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_TS0_PD5Cntl_PD_EN                       0x0238
    #define     B16Gbl_TS0_PD5Cntl_PD_EN                       0x0238
    #define   LSb32Gbl_TS0_PD5Cntl_PD_EN                          2
    #define   LSb16Gbl_TS0_PD5Cntl_PD_EN                          2
    #define       bGbl_TS0_PD5Cntl_PD_EN                       1
    #define   MSK32Gbl_TS0_PD5Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_PD6Cntl                             0x023C

    #define     BA_Gbl_TS0_PD6Cntl_SLP_DI                      0x023C
    #define     B16Gbl_TS0_PD6Cntl_SLP_DI                      0x023C
    #define   LSb32Gbl_TS0_PD6Cntl_SLP_DI                         0
    #define   LSb16Gbl_TS0_PD6Cntl_SLP_DI                         0
    #define       bGbl_TS0_PD6Cntl_SLP_DI                      1
    #define   MSK32Gbl_TS0_PD6Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TS0_PD6Cntl_PU_EN                       0x023C
    #define     B16Gbl_TS0_PD6Cntl_PU_EN                       0x023C
    #define   LSb32Gbl_TS0_PD6Cntl_PU_EN                          1
    #define   LSb16Gbl_TS0_PD6Cntl_PU_EN                          1
    #define       bGbl_TS0_PD6Cntl_PU_EN                       1
    #define   MSK32Gbl_TS0_PD6Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_TS0_PD6Cntl_PD_EN                       0x023C
    #define     B16Gbl_TS0_PD6Cntl_PD_EN                       0x023C
    #define   LSb32Gbl_TS0_PD6Cntl_PD_EN                          2
    #define   LSb16Gbl_TS0_PD6Cntl_PD_EN                          2
    #define       bGbl_TS0_PD6Cntl_PD_EN                       1
    #define   MSK32Gbl_TS0_PD6Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_TS0_PD7Cntl                             0x0240

    #define     BA_Gbl_TS0_PD7Cntl_SLP_DI                      0x0240
    #define     B16Gbl_TS0_PD7Cntl_SLP_DI                      0x0240
    #define   LSb32Gbl_TS0_PD7Cntl_SLP_DI                         0
    #define   LSb16Gbl_TS0_PD7Cntl_SLP_DI                         0
    #define       bGbl_TS0_PD7Cntl_SLP_DI                      1
    #define   MSK32Gbl_TS0_PD7Cntl_SLP_DI                         0x00000001

    #define     BA_Gbl_TS0_PD7Cntl_PU_EN                       0x0240
    #define     B16Gbl_TS0_PD7Cntl_PU_EN                       0x0240
    #define   LSb32Gbl_TS0_PD7Cntl_PU_EN                          1
    #define   LSb16Gbl_TS0_PD7Cntl_PU_EN                          1
    #define       bGbl_TS0_PD7Cntl_PU_EN                       1
    #define   MSK32Gbl_TS0_PD7Cntl_PU_EN                          0x00000002

    #define     BA_Gbl_TS0_PD7Cntl_PD_EN                       0x0240
    #define     B16Gbl_TS0_PD7Cntl_PD_EN                       0x0240
    #define   LSb32Gbl_TS0_PD7Cntl_PD_EN                          2
    #define   LSb16Gbl_TS0_PD7Cntl_PD_EN                          2
    #define       bGbl_TS0_PD7Cntl_PD_EN                       1
    #define   MSK32Gbl_TS0_PD7Cntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_STS2_CLKCntl                            0x0244

    #define     BA_Gbl_STS2_CLKCntl_SLP_DI                     0x0244
    #define     B16Gbl_STS2_CLKCntl_SLP_DI                     0x0244
    #define   LSb32Gbl_STS2_CLKCntl_SLP_DI                        0
    #define   LSb16Gbl_STS2_CLKCntl_SLP_DI                        0
    #define       bGbl_STS2_CLKCntl_SLP_DI                     1
    #define   MSK32Gbl_STS2_CLKCntl_SLP_DI                        0x00000001

    #define     BA_Gbl_STS2_CLKCntl_PU_EN                      0x0244
    #define     B16Gbl_STS2_CLKCntl_PU_EN                      0x0244
    #define   LSb32Gbl_STS2_CLKCntl_PU_EN                         1
    #define   LSb16Gbl_STS2_CLKCntl_PU_EN                         1
    #define       bGbl_STS2_CLKCntl_PU_EN                      1
    #define   MSK32Gbl_STS2_CLKCntl_PU_EN                         0x00000002

    #define     BA_Gbl_STS2_CLKCntl_PD_EN                      0x0244
    #define     B16Gbl_STS2_CLKCntl_PD_EN                      0x0244
    #define   LSb32Gbl_STS2_CLKCntl_PD_EN                         2
    #define   LSb16Gbl_STS2_CLKCntl_PD_EN                         2
    #define       bGbl_STS2_CLKCntl_PD_EN                      1
    #define   MSK32Gbl_STS2_CLKCntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_STS2_ERRORCntl                          0x0248

    #define     BA_Gbl_STS2_ERRORCntl_SLP_DI                   0x0248
    #define     B16Gbl_STS2_ERRORCntl_SLP_DI                   0x0248
    #define   LSb32Gbl_STS2_ERRORCntl_SLP_DI                      0
    #define   LSb16Gbl_STS2_ERRORCntl_SLP_DI                      0
    #define       bGbl_STS2_ERRORCntl_SLP_DI                   1
    #define   MSK32Gbl_STS2_ERRORCntl_SLP_DI                      0x00000001

    #define     BA_Gbl_STS2_ERRORCntl_PU_EN                    0x0248
    #define     B16Gbl_STS2_ERRORCntl_PU_EN                    0x0248
    #define   LSb32Gbl_STS2_ERRORCntl_PU_EN                       1
    #define   LSb16Gbl_STS2_ERRORCntl_PU_EN                       1
    #define       bGbl_STS2_ERRORCntl_PU_EN                    1
    #define   MSK32Gbl_STS2_ERRORCntl_PU_EN                       0x00000002

    #define     BA_Gbl_STS2_ERRORCntl_PD_EN                    0x0248
    #define     B16Gbl_STS2_ERRORCntl_PD_EN                    0x0248
    #define   LSb32Gbl_STS2_ERRORCntl_PD_EN                       2
    #define   LSb16Gbl_STS2_ERRORCntl_PD_EN                       2
    #define       bGbl_STS2_ERRORCntl_PD_EN                    1
    #define   MSK32Gbl_STS2_ERRORCntl_PD_EN                       0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_STS2_VALDCntl                           0x024C

    #define     BA_Gbl_STS2_VALDCntl_SLP_DI                    0x024C
    #define     B16Gbl_STS2_VALDCntl_SLP_DI                    0x024C
    #define   LSb32Gbl_STS2_VALDCntl_SLP_DI                       0
    #define   LSb16Gbl_STS2_VALDCntl_SLP_DI                       0
    #define       bGbl_STS2_VALDCntl_SLP_DI                    1
    #define   MSK32Gbl_STS2_VALDCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_STS2_VALDCntl_PU_EN                     0x024C
    #define     B16Gbl_STS2_VALDCntl_PU_EN                     0x024C
    #define   LSb32Gbl_STS2_VALDCntl_PU_EN                        1
    #define   LSb16Gbl_STS2_VALDCntl_PU_EN                        1
    #define       bGbl_STS2_VALDCntl_PU_EN                     1
    #define   MSK32Gbl_STS2_VALDCntl_PU_EN                        0x00000002

    #define     BA_Gbl_STS2_VALDCntl_PD_EN                     0x024C
    #define     B16Gbl_STS2_VALDCntl_PD_EN                     0x024C
    #define   LSb32Gbl_STS2_VALDCntl_PD_EN                        2
    #define   LSb16Gbl_STS2_VALDCntl_PD_EN                        2
    #define       bGbl_STS2_VALDCntl_PD_EN                     1
    #define   MSK32Gbl_STS2_VALDCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_STS2_SOPCntl                            0x0250

    #define     BA_Gbl_STS2_SOPCntl_SLP_DI                     0x0250
    #define     B16Gbl_STS2_SOPCntl_SLP_DI                     0x0250
    #define   LSb32Gbl_STS2_SOPCntl_SLP_DI                        0
    #define   LSb16Gbl_STS2_SOPCntl_SLP_DI                        0
    #define       bGbl_STS2_SOPCntl_SLP_DI                     1
    #define   MSK32Gbl_STS2_SOPCntl_SLP_DI                        0x00000001

    #define     BA_Gbl_STS2_SOPCntl_PU_EN                      0x0250
    #define     B16Gbl_STS2_SOPCntl_PU_EN                      0x0250
    #define   LSb32Gbl_STS2_SOPCntl_PU_EN                         1
    #define   LSb16Gbl_STS2_SOPCntl_PU_EN                         1
    #define       bGbl_STS2_SOPCntl_PU_EN                      1
    #define   MSK32Gbl_STS2_SOPCntl_PU_EN                         0x00000002

    #define     BA_Gbl_STS2_SOPCntl_PD_EN                      0x0250
    #define     B16Gbl_STS2_SOPCntl_PD_EN                      0x0250
    #define   LSb32Gbl_STS2_SOPCntl_PD_EN                         2
    #define   LSb16Gbl_STS2_SOPCntl_PD_EN                         2
    #define       bGbl_STS2_SOPCntl_PD_EN                      1
    #define   MSK32Gbl_STS2_SOPCntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_STS2_SDCntl                             0x0254

    #define     BA_Gbl_STS2_SDCntl_SLP_DI                      0x0254
    #define     B16Gbl_STS2_SDCntl_SLP_DI                      0x0254
    #define   LSb32Gbl_STS2_SDCntl_SLP_DI                         0
    #define   LSb16Gbl_STS2_SDCntl_SLP_DI                         0
    #define       bGbl_STS2_SDCntl_SLP_DI                      1
    #define   MSK32Gbl_STS2_SDCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_STS2_SDCntl_PU_EN                       0x0254
    #define     B16Gbl_STS2_SDCntl_PU_EN                       0x0254
    #define   LSb32Gbl_STS2_SDCntl_PU_EN                          1
    #define   LSb16Gbl_STS2_SDCntl_PU_EN                          1
    #define       bGbl_STS2_SDCntl_PU_EN                       1
    #define   MSK32Gbl_STS2_SDCntl_PU_EN                          0x00000002

    #define     BA_Gbl_STS2_SDCntl_PD_EN                       0x0254
    #define     B16Gbl_STS2_SDCntl_PD_EN                       0x0254
    #define   LSb32Gbl_STS2_SDCntl_PD_EN                          2
    #define   LSb16Gbl_STS2_SDCntl_PD_EN                          2
    #define       bGbl_STS2_SDCntl_PD_EN                       1
    #define   MSK32Gbl_STS2_SDCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_RSTInCntl                               0x0258

    #define     BA_Gbl_RSTInCntl_SLP_DI                        0x0258
    #define     B16Gbl_RSTInCntl_SLP_DI                        0x0258
    #define   LSb32Gbl_RSTInCntl_SLP_DI                           0
    #define   LSb16Gbl_RSTInCntl_SLP_DI                           0
    #define       bGbl_RSTInCntl_SLP_DI                        1
    #define   MSK32Gbl_RSTInCntl_SLP_DI                           0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_BYPS_OSCCntl                            0x025C

    #define     BA_Gbl_BYPS_OSCCntl_SLP_DI                     0x025C
    #define     B16Gbl_BYPS_OSCCntl_SLP_DI                     0x025C
    #define   LSb32Gbl_BYPS_OSCCntl_SLP_DI                        0
    #define   LSb16Gbl_BYPS_OSCCntl_SLP_DI                        0
    #define       bGbl_BYPS_OSCCntl_SLP_DI                     1
    #define   MSK32Gbl_BYPS_OSCCntl_SLP_DI                        0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_ENG_ENCntl                              0x0260

    #define     BA_Gbl_ENG_ENCntl_SLP_DI                       0x0260
    #define     B16Gbl_ENG_ENCntl_SLP_DI                       0x0260
    #define   LSb32Gbl_ENG_ENCntl_SLP_DI                          0
    #define   LSb16Gbl_ENG_ENCntl_SLP_DI                          0
    #define       bGbl_ENG_ENCntl_SLP_DI                       1
    #define   MSK32Gbl_ENG_ENCntl_SLP_DI                          0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO0Cntl                               0x0264

    #define     BA_Gbl_DVIO0Cntl_SLP_DI                        0x0264
    #define     B16Gbl_DVIO0Cntl_SLP_DI                        0x0264
    #define   LSb32Gbl_DVIO0Cntl_SLP_DI                           0
    #define   LSb16Gbl_DVIO0Cntl_SLP_DI                           0
    #define       bGbl_DVIO0Cntl_SLP_DI                        1
    #define   MSK32Gbl_DVIO0Cntl_SLP_DI                           0x00000001

    #define     BA_Gbl_DVIO0Cntl_PU_EN                         0x0264
    #define     B16Gbl_DVIO0Cntl_PU_EN                         0x0264
    #define   LSb32Gbl_DVIO0Cntl_PU_EN                            1
    #define   LSb16Gbl_DVIO0Cntl_PU_EN                            1
    #define       bGbl_DVIO0Cntl_PU_EN                         1
    #define   MSK32Gbl_DVIO0Cntl_PU_EN                            0x00000002

    #define     BA_Gbl_DVIO0Cntl_PD_EN                         0x0264
    #define     B16Gbl_DVIO0Cntl_PD_EN                         0x0264
    #define   LSb32Gbl_DVIO0Cntl_PD_EN                            2
    #define   LSb16Gbl_DVIO0Cntl_PD_EN                            2
    #define       bGbl_DVIO0Cntl_PD_EN                         1
    #define   MSK32Gbl_DVIO0Cntl_PD_EN                            0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO1Cntl                               0x0268

    #define     BA_Gbl_DVIO1Cntl_SLP_DI                        0x0268
    #define     B16Gbl_DVIO1Cntl_SLP_DI                        0x0268
    #define   LSb32Gbl_DVIO1Cntl_SLP_DI                           0
    #define   LSb16Gbl_DVIO1Cntl_SLP_DI                           0
    #define       bGbl_DVIO1Cntl_SLP_DI                        1
    #define   MSK32Gbl_DVIO1Cntl_SLP_DI                           0x00000001

    #define     BA_Gbl_DVIO1Cntl_PU_EN                         0x0268
    #define     B16Gbl_DVIO1Cntl_PU_EN                         0x0268
    #define   LSb32Gbl_DVIO1Cntl_PU_EN                            1
    #define   LSb16Gbl_DVIO1Cntl_PU_EN                            1
    #define       bGbl_DVIO1Cntl_PU_EN                         1
    #define   MSK32Gbl_DVIO1Cntl_PU_EN                            0x00000002

    #define     BA_Gbl_DVIO1Cntl_PD_EN                         0x0268
    #define     B16Gbl_DVIO1Cntl_PD_EN                         0x0268
    #define   LSb32Gbl_DVIO1Cntl_PD_EN                            2
    #define   LSb16Gbl_DVIO1Cntl_PD_EN                            2
    #define       bGbl_DVIO1Cntl_PD_EN                         1
    #define   MSK32Gbl_DVIO1Cntl_PD_EN                            0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO2Cntl                               0x026C

    #define     BA_Gbl_DVIO2Cntl_SLP_DI                        0x026C
    #define     B16Gbl_DVIO2Cntl_SLP_DI                        0x026C
    #define   LSb32Gbl_DVIO2Cntl_SLP_DI                           0
    #define   LSb16Gbl_DVIO2Cntl_SLP_DI                           0
    #define       bGbl_DVIO2Cntl_SLP_DI                        1
    #define   MSK32Gbl_DVIO2Cntl_SLP_DI                           0x00000001

    #define     BA_Gbl_DVIO2Cntl_PU_EN                         0x026C
    #define     B16Gbl_DVIO2Cntl_PU_EN                         0x026C
    #define   LSb32Gbl_DVIO2Cntl_PU_EN                            1
    #define   LSb16Gbl_DVIO2Cntl_PU_EN                            1
    #define       bGbl_DVIO2Cntl_PU_EN                         1
    #define   MSK32Gbl_DVIO2Cntl_PU_EN                            0x00000002

    #define     BA_Gbl_DVIO2Cntl_PD_EN                         0x026C
    #define     B16Gbl_DVIO2Cntl_PD_EN                         0x026C
    #define   LSb32Gbl_DVIO2Cntl_PD_EN                            2
    #define   LSb16Gbl_DVIO2Cntl_PD_EN                            2
    #define       bGbl_DVIO2Cntl_PD_EN                         1
    #define   MSK32Gbl_DVIO2Cntl_PD_EN                            0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO3Cntl                               0x0270

    #define     BA_Gbl_DVIO3Cntl_SLP_DI                        0x0270
    #define     B16Gbl_DVIO3Cntl_SLP_DI                        0x0270
    #define   LSb32Gbl_DVIO3Cntl_SLP_DI                           0
    #define   LSb16Gbl_DVIO3Cntl_SLP_DI                           0
    #define       bGbl_DVIO3Cntl_SLP_DI                        1
    #define   MSK32Gbl_DVIO3Cntl_SLP_DI                           0x00000001

    #define     BA_Gbl_DVIO3Cntl_PU_EN                         0x0270
    #define     B16Gbl_DVIO3Cntl_PU_EN                         0x0270
    #define   LSb32Gbl_DVIO3Cntl_PU_EN                            1
    #define   LSb16Gbl_DVIO3Cntl_PU_EN                            1
    #define       bGbl_DVIO3Cntl_PU_EN                         1
    #define   MSK32Gbl_DVIO3Cntl_PU_EN                            0x00000002

    #define     BA_Gbl_DVIO3Cntl_PD_EN                         0x0270
    #define     B16Gbl_DVIO3Cntl_PD_EN                         0x0270
    #define   LSb32Gbl_DVIO3Cntl_PD_EN                            2
    #define   LSb16Gbl_DVIO3Cntl_PD_EN                            2
    #define       bGbl_DVIO3Cntl_PD_EN                         1
    #define   MSK32Gbl_DVIO3Cntl_PD_EN                            0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO4Cntl                               0x0274

    #define     BA_Gbl_DVIO4Cntl_SLP_DI                        0x0274
    #define     B16Gbl_DVIO4Cntl_SLP_DI                        0x0274
    #define   LSb32Gbl_DVIO4Cntl_SLP_DI                           0
    #define   LSb16Gbl_DVIO4Cntl_SLP_DI                           0
    #define       bGbl_DVIO4Cntl_SLP_DI                        1
    #define   MSK32Gbl_DVIO4Cntl_SLP_DI                           0x00000001

    #define     BA_Gbl_DVIO4Cntl_PU_EN                         0x0274
    #define     B16Gbl_DVIO4Cntl_PU_EN                         0x0274
    #define   LSb32Gbl_DVIO4Cntl_PU_EN                            1
    #define   LSb16Gbl_DVIO4Cntl_PU_EN                            1
    #define       bGbl_DVIO4Cntl_PU_EN                         1
    #define   MSK32Gbl_DVIO4Cntl_PU_EN                            0x00000002

    #define     BA_Gbl_DVIO4Cntl_PD_EN                         0x0274
    #define     B16Gbl_DVIO4Cntl_PD_EN                         0x0274
    #define   LSb32Gbl_DVIO4Cntl_PD_EN                            2
    #define   LSb16Gbl_DVIO4Cntl_PD_EN                            2
    #define       bGbl_DVIO4Cntl_PD_EN                         1
    #define   MSK32Gbl_DVIO4Cntl_PD_EN                            0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO5Cntl                               0x0278

    #define     BA_Gbl_DVIO5Cntl_SLP_DI                        0x0278
    #define     B16Gbl_DVIO5Cntl_SLP_DI                        0x0278
    #define   LSb32Gbl_DVIO5Cntl_SLP_DI                           0
    #define   LSb16Gbl_DVIO5Cntl_SLP_DI                           0
    #define       bGbl_DVIO5Cntl_SLP_DI                        1
    #define   MSK32Gbl_DVIO5Cntl_SLP_DI                           0x00000001

    #define     BA_Gbl_DVIO5Cntl_PU_EN                         0x0278
    #define     B16Gbl_DVIO5Cntl_PU_EN                         0x0278
    #define   LSb32Gbl_DVIO5Cntl_PU_EN                            1
    #define   LSb16Gbl_DVIO5Cntl_PU_EN                            1
    #define       bGbl_DVIO5Cntl_PU_EN                         1
    #define   MSK32Gbl_DVIO5Cntl_PU_EN                            0x00000002

    #define     BA_Gbl_DVIO5Cntl_PD_EN                         0x0278
    #define     B16Gbl_DVIO5Cntl_PD_EN                         0x0278
    #define   LSb32Gbl_DVIO5Cntl_PD_EN                            2
    #define   LSb16Gbl_DVIO5Cntl_PD_EN                            2
    #define       bGbl_DVIO5Cntl_PD_EN                         1
    #define   MSK32Gbl_DVIO5Cntl_PD_EN                            0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO6Cntl                               0x027C

    #define     BA_Gbl_DVIO6Cntl_SLP_DI                        0x027C
    #define     B16Gbl_DVIO6Cntl_SLP_DI                        0x027C
    #define   LSb32Gbl_DVIO6Cntl_SLP_DI                           0
    #define   LSb16Gbl_DVIO6Cntl_SLP_DI                           0
    #define       bGbl_DVIO6Cntl_SLP_DI                        1
    #define   MSK32Gbl_DVIO6Cntl_SLP_DI                           0x00000001

    #define     BA_Gbl_DVIO6Cntl_PU_EN                         0x027C
    #define     B16Gbl_DVIO6Cntl_PU_EN                         0x027C
    #define   LSb32Gbl_DVIO6Cntl_PU_EN                            1
    #define   LSb16Gbl_DVIO6Cntl_PU_EN                            1
    #define       bGbl_DVIO6Cntl_PU_EN                         1
    #define   MSK32Gbl_DVIO6Cntl_PU_EN                            0x00000002

    #define     BA_Gbl_DVIO6Cntl_PD_EN                         0x027C
    #define     B16Gbl_DVIO6Cntl_PD_EN                         0x027C
    #define   LSb32Gbl_DVIO6Cntl_PD_EN                            2
    #define   LSb16Gbl_DVIO6Cntl_PD_EN                            2
    #define       bGbl_DVIO6Cntl_PD_EN                         1
    #define   MSK32Gbl_DVIO6Cntl_PD_EN                            0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO7Cntl                               0x0280

    #define     BA_Gbl_DVIO7Cntl_SLP_DI                        0x0280
    #define     B16Gbl_DVIO7Cntl_SLP_DI                        0x0280
    #define   LSb32Gbl_DVIO7Cntl_SLP_DI                           0
    #define   LSb16Gbl_DVIO7Cntl_SLP_DI                           0
    #define       bGbl_DVIO7Cntl_SLP_DI                        1
    #define   MSK32Gbl_DVIO7Cntl_SLP_DI                           0x00000001

    #define     BA_Gbl_DVIO7Cntl_PU_EN                         0x0280
    #define     B16Gbl_DVIO7Cntl_PU_EN                         0x0280
    #define   LSb32Gbl_DVIO7Cntl_PU_EN                            1
    #define   LSb16Gbl_DVIO7Cntl_PU_EN                            1
    #define       bGbl_DVIO7Cntl_PU_EN                         1
    #define   MSK32Gbl_DVIO7Cntl_PU_EN                            0x00000002

    #define     BA_Gbl_DVIO7Cntl_PD_EN                         0x0280
    #define     B16Gbl_DVIO7Cntl_PD_EN                         0x0280
    #define   LSb32Gbl_DVIO7Cntl_PD_EN                            2
    #define   LSb16Gbl_DVIO7Cntl_PD_EN                            2
    #define       bGbl_DVIO7Cntl_PD_EN                         1
    #define   MSK32Gbl_DVIO7Cntl_PD_EN                            0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO8Cntl                               0x0284

    #define     BA_Gbl_DVIO8Cntl_SLP_DI                        0x0284
    #define     B16Gbl_DVIO8Cntl_SLP_DI                        0x0284
    #define   LSb32Gbl_DVIO8Cntl_SLP_DI                           0
    #define   LSb16Gbl_DVIO8Cntl_SLP_DI                           0
    #define       bGbl_DVIO8Cntl_SLP_DI                        1
    #define   MSK32Gbl_DVIO8Cntl_SLP_DI                           0x00000001

    #define     BA_Gbl_DVIO8Cntl_PU_EN                         0x0284
    #define     B16Gbl_DVIO8Cntl_PU_EN                         0x0284
    #define   LSb32Gbl_DVIO8Cntl_PU_EN                            1
    #define   LSb16Gbl_DVIO8Cntl_PU_EN                            1
    #define       bGbl_DVIO8Cntl_PU_EN                         1
    #define   MSK32Gbl_DVIO8Cntl_PU_EN                            0x00000002

    #define     BA_Gbl_DVIO8Cntl_PD_EN                         0x0284
    #define     B16Gbl_DVIO8Cntl_PD_EN                         0x0284
    #define   LSb32Gbl_DVIO8Cntl_PD_EN                            2
    #define   LSb16Gbl_DVIO8Cntl_PD_EN                            2
    #define       bGbl_DVIO8Cntl_PD_EN                         1
    #define   MSK32Gbl_DVIO8Cntl_PD_EN                            0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO9Cntl                               0x0288

    #define     BA_Gbl_DVIO9Cntl_SLP_DI                        0x0288
    #define     B16Gbl_DVIO9Cntl_SLP_DI                        0x0288
    #define   LSb32Gbl_DVIO9Cntl_SLP_DI                           0
    #define   LSb16Gbl_DVIO9Cntl_SLP_DI                           0
    #define       bGbl_DVIO9Cntl_SLP_DI                        1
    #define   MSK32Gbl_DVIO9Cntl_SLP_DI                           0x00000001

    #define     BA_Gbl_DVIO9Cntl_PU_EN                         0x0288
    #define     B16Gbl_DVIO9Cntl_PU_EN                         0x0288
    #define   LSb32Gbl_DVIO9Cntl_PU_EN                            1
    #define   LSb16Gbl_DVIO9Cntl_PU_EN                            1
    #define       bGbl_DVIO9Cntl_PU_EN                         1
    #define   MSK32Gbl_DVIO9Cntl_PU_EN                            0x00000002

    #define     BA_Gbl_DVIO9Cntl_PD_EN                         0x0288
    #define     B16Gbl_DVIO9Cntl_PD_EN                         0x0288
    #define   LSb32Gbl_DVIO9Cntl_PD_EN                            2
    #define   LSb16Gbl_DVIO9Cntl_PD_EN                            2
    #define       bGbl_DVIO9Cntl_PD_EN                         1
    #define   MSK32Gbl_DVIO9Cntl_PD_EN                            0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO10Cntl                              0x028C

    #define     BA_Gbl_DVIO10Cntl_SLP_DI                       0x028C
    #define     B16Gbl_DVIO10Cntl_SLP_DI                       0x028C
    #define   LSb32Gbl_DVIO10Cntl_SLP_DI                          0
    #define   LSb16Gbl_DVIO10Cntl_SLP_DI                          0
    #define       bGbl_DVIO10Cntl_SLP_DI                       1
    #define   MSK32Gbl_DVIO10Cntl_SLP_DI                          0x00000001

    #define     BA_Gbl_DVIO10Cntl_PU_EN                        0x028C
    #define     B16Gbl_DVIO10Cntl_PU_EN                        0x028C
    #define   LSb32Gbl_DVIO10Cntl_PU_EN                           1
    #define   LSb16Gbl_DVIO10Cntl_PU_EN                           1
    #define       bGbl_DVIO10Cntl_PU_EN                        1
    #define   MSK32Gbl_DVIO10Cntl_PU_EN                           0x00000002

    #define     BA_Gbl_DVIO10Cntl_PD_EN                        0x028C
    #define     B16Gbl_DVIO10Cntl_PD_EN                        0x028C
    #define   LSb32Gbl_DVIO10Cntl_PD_EN                           2
    #define   LSb16Gbl_DVIO10Cntl_PD_EN                           2
    #define       bGbl_DVIO10Cntl_PD_EN                        1
    #define   MSK32Gbl_DVIO10Cntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO11Cntl                              0x0290

    #define     BA_Gbl_DVIO11Cntl_SLP_DI                       0x0290
    #define     B16Gbl_DVIO11Cntl_SLP_DI                       0x0290
    #define   LSb32Gbl_DVIO11Cntl_SLP_DI                          0
    #define   LSb16Gbl_DVIO11Cntl_SLP_DI                          0
    #define       bGbl_DVIO11Cntl_SLP_DI                       1
    #define   MSK32Gbl_DVIO11Cntl_SLP_DI                          0x00000001

    #define     BA_Gbl_DVIO11Cntl_PU_EN                        0x0290
    #define     B16Gbl_DVIO11Cntl_PU_EN                        0x0290
    #define   LSb32Gbl_DVIO11Cntl_PU_EN                           1
    #define   LSb16Gbl_DVIO11Cntl_PU_EN                           1
    #define       bGbl_DVIO11Cntl_PU_EN                        1
    #define   MSK32Gbl_DVIO11Cntl_PU_EN                           0x00000002

    #define     BA_Gbl_DVIO11Cntl_PD_EN                        0x0290
    #define     B16Gbl_DVIO11Cntl_PD_EN                        0x0290
    #define   LSb32Gbl_DVIO11Cntl_PD_EN                           2
    #define   LSb16Gbl_DVIO11Cntl_PD_EN                           2
    #define       bGbl_DVIO11Cntl_PD_EN                        1
    #define   MSK32Gbl_DVIO11Cntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO12Cntl                              0x0294

    #define     BA_Gbl_DVIO12Cntl_SLP_DI                       0x0294
    #define     B16Gbl_DVIO12Cntl_SLP_DI                       0x0294
    #define   LSb32Gbl_DVIO12Cntl_SLP_DI                          0
    #define   LSb16Gbl_DVIO12Cntl_SLP_DI                          0
    #define       bGbl_DVIO12Cntl_SLP_DI                       1
    #define   MSK32Gbl_DVIO12Cntl_SLP_DI                          0x00000001

    #define     BA_Gbl_DVIO12Cntl_PU_EN                        0x0294
    #define     B16Gbl_DVIO12Cntl_PU_EN                        0x0294
    #define   LSb32Gbl_DVIO12Cntl_PU_EN                           1
    #define   LSb16Gbl_DVIO12Cntl_PU_EN                           1
    #define       bGbl_DVIO12Cntl_PU_EN                        1
    #define   MSK32Gbl_DVIO12Cntl_PU_EN                           0x00000002

    #define     BA_Gbl_DVIO12Cntl_PD_EN                        0x0294
    #define     B16Gbl_DVIO12Cntl_PD_EN                        0x0294
    #define   LSb32Gbl_DVIO12Cntl_PD_EN                           2
    #define   LSb16Gbl_DVIO12Cntl_PD_EN                           2
    #define       bGbl_DVIO12Cntl_PD_EN                        1
    #define   MSK32Gbl_DVIO12Cntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO13Cntl                              0x0298

    #define     BA_Gbl_DVIO13Cntl_SLP_DI                       0x0298
    #define     B16Gbl_DVIO13Cntl_SLP_DI                       0x0298
    #define   LSb32Gbl_DVIO13Cntl_SLP_DI                          0
    #define   LSb16Gbl_DVIO13Cntl_SLP_DI                          0
    #define       bGbl_DVIO13Cntl_SLP_DI                       1
    #define   MSK32Gbl_DVIO13Cntl_SLP_DI                          0x00000001

    #define     BA_Gbl_DVIO13Cntl_PU_EN                        0x0298
    #define     B16Gbl_DVIO13Cntl_PU_EN                        0x0298
    #define   LSb32Gbl_DVIO13Cntl_PU_EN                           1
    #define   LSb16Gbl_DVIO13Cntl_PU_EN                           1
    #define       bGbl_DVIO13Cntl_PU_EN                        1
    #define   MSK32Gbl_DVIO13Cntl_PU_EN                           0x00000002

    #define     BA_Gbl_DVIO13Cntl_PD_EN                        0x0298
    #define     B16Gbl_DVIO13Cntl_PD_EN                        0x0298
    #define   LSb32Gbl_DVIO13Cntl_PD_EN                           2
    #define   LSb16Gbl_DVIO13Cntl_PD_EN                           2
    #define       bGbl_DVIO13Cntl_PD_EN                        1
    #define   MSK32Gbl_DVIO13Cntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO14Cntl                              0x029C

    #define     BA_Gbl_DVIO14Cntl_SLP_DI                       0x029C
    #define     B16Gbl_DVIO14Cntl_SLP_DI                       0x029C
    #define   LSb32Gbl_DVIO14Cntl_SLP_DI                          0
    #define   LSb16Gbl_DVIO14Cntl_SLP_DI                          0
    #define       bGbl_DVIO14Cntl_SLP_DI                       1
    #define   MSK32Gbl_DVIO14Cntl_SLP_DI                          0x00000001

    #define     BA_Gbl_DVIO14Cntl_PU_EN                        0x029C
    #define     B16Gbl_DVIO14Cntl_PU_EN                        0x029C
    #define   LSb32Gbl_DVIO14Cntl_PU_EN                           1
    #define   LSb16Gbl_DVIO14Cntl_PU_EN                           1
    #define       bGbl_DVIO14Cntl_PU_EN                        1
    #define   MSK32Gbl_DVIO14Cntl_PU_EN                           0x00000002

    #define     BA_Gbl_DVIO14Cntl_PD_EN                        0x029C
    #define     B16Gbl_DVIO14Cntl_PD_EN                        0x029C
    #define   LSb32Gbl_DVIO14Cntl_PD_EN                           2
    #define   LSb16Gbl_DVIO14Cntl_PD_EN                           2
    #define       bGbl_DVIO14Cntl_PD_EN                        1
    #define   MSK32Gbl_DVIO14Cntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DVIO15Cntl                              0x02A0

    #define     BA_Gbl_DVIO15Cntl_SLP_DI                       0x02A0
    #define     B16Gbl_DVIO15Cntl_SLP_DI                       0x02A0
    #define   LSb32Gbl_DVIO15Cntl_SLP_DI                          0
    #define   LSb16Gbl_DVIO15Cntl_SLP_DI                          0
    #define       bGbl_DVIO15Cntl_SLP_DI                       1
    #define   MSK32Gbl_DVIO15Cntl_SLP_DI                          0x00000001

    #define     BA_Gbl_DVIO15Cntl_PU_EN                        0x02A0
    #define     B16Gbl_DVIO15Cntl_PU_EN                        0x02A0
    #define   LSb32Gbl_DVIO15Cntl_PU_EN                           1
    #define   LSb16Gbl_DVIO15Cntl_PU_EN                           1
    #define       bGbl_DVIO15Cntl_PU_EN                        1
    #define   MSK32Gbl_DVIO15Cntl_PU_EN                           0x00000002

    #define     BA_Gbl_DVIO15Cntl_PD_EN                        0x02A0
    #define     B16Gbl_DVIO15Cntl_PD_EN                        0x02A0
    #define   LSb32Gbl_DVIO15Cntl_PD_EN                           2
    #define   LSb16Gbl_DVIO15Cntl_PD_EN                           2
    #define       bGbl_DVIO15Cntl_PD_EN                        1
    #define   MSK32Gbl_DVIO15Cntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DV0_CLKCntl                             0x02A4

    #define     BA_Gbl_DV0_CLKCntl_SLP_DI                      0x02A4
    #define     B16Gbl_DV0_CLKCntl_SLP_DI                      0x02A4
    #define   LSb32Gbl_DV0_CLKCntl_SLP_DI                         0
    #define   LSb16Gbl_DV0_CLKCntl_SLP_DI                         0
    #define       bGbl_DV0_CLKCntl_SLP_DI                      1
    #define   MSK32Gbl_DV0_CLKCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_DV0_CLKCntl_PU_EN                       0x02A4
    #define     B16Gbl_DV0_CLKCntl_PU_EN                       0x02A4
    #define   LSb32Gbl_DV0_CLKCntl_PU_EN                          1
    #define   LSb16Gbl_DV0_CLKCntl_PU_EN                          1
    #define       bGbl_DV0_CLKCntl_PU_EN                       1
    #define   MSK32Gbl_DV0_CLKCntl_PU_EN                          0x00000002

    #define     BA_Gbl_DV0_CLKCntl_PD_EN                       0x02A4
    #define     B16Gbl_DV0_CLKCntl_PD_EN                       0x02A4
    #define   LSb32Gbl_DV0_CLKCntl_PD_EN                          2
    #define   LSb16Gbl_DV0_CLKCntl_PD_EN                          2
    #define       bGbl_DV0_CLKCntl_PD_EN                       1
    #define   MSK32Gbl_DV0_CLKCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DV0_HSCntl                              0x02A8

    #define     BA_Gbl_DV0_HSCntl_SLP_DI                       0x02A8
    #define     B16Gbl_DV0_HSCntl_SLP_DI                       0x02A8
    #define   LSb32Gbl_DV0_HSCntl_SLP_DI                          0
    #define   LSb16Gbl_DV0_HSCntl_SLP_DI                          0
    #define       bGbl_DV0_HSCntl_SLP_DI                       1
    #define   MSK32Gbl_DV0_HSCntl_SLP_DI                          0x00000001

    #define     BA_Gbl_DV0_HSCntl_PU_EN                        0x02A8
    #define     B16Gbl_DV0_HSCntl_PU_EN                        0x02A8
    #define   LSb32Gbl_DV0_HSCntl_PU_EN                           1
    #define   LSb16Gbl_DV0_HSCntl_PU_EN                           1
    #define       bGbl_DV0_HSCntl_PU_EN                        1
    #define   MSK32Gbl_DV0_HSCntl_PU_EN                           0x00000002

    #define     BA_Gbl_DV0_HSCntl_PD_EN                        0x02A8
    #define     B16Gbl_DV0_HSCntl_PD_EN                        0x02A8
    #define   LSb32Gbl_DV0_HSCntl_PD_EN                           2
    #define   LSb16Gbl_DV0_HSCntl_PD_EN                           2
    #define       bGbl_DV0_HSCntl_PD_EN                        1
    #define   MSK32Gbl_DV0_HSCntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DV0_VSCntl                              0x02AC

    #define     BA_Gbl_DV0_VSCntl_SLP_DI                       0x02AC
    #define     B16Gbl_DV0_VSCntl_SLP_DI                       0x02AC
    #define   LSb32Gbl_DV0_VSCntl_SLP_DI                          0
    #define   LSb16Gbl_DV0_VSCntl_SLP_DI                          0
    #define       bGbl_DV0_VSCntl_SLP_DI                       1
    #define   MSK32Gbl_DV0_VSCntl_SLP_DI                          0x00000001

    #define     BA_Gbl_DV0_VSCntl_PU_EN                        0x02AC
    #define     B16Gbl_DV0_VSCntl_PU_EN                        0x02AC
    #define   LSb32Gbl_DV0_VSCntl_PU_EN                           1
    #define   LSb16Gbl_DV0_VSCntl_PU_EN                           1
    #define       bGbl_DV0_VSCntl_PU_EN                        1
    #define   MSK32Gbl_DV0_VSCntl_PU_EN                           0x00000002

    #define     BA_Gbl_DV0_VSCntl_PD_EN                        0x02AC
    #define     B16Gbl_DV0_VSCntl_PD_EN                        0x02AC
    #define   LSb32Gbl_DV0_VSCntl_PD_EN                           2
    #define   LSb16Gbl_DV0_VSCntl_PD_EN                           2
    #define       bGbl_DV0_VSCntl_PD_EN                        1
    #define   MSK32Gbl_DV0_VSCntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DV0_FIDCntl                             0x02B0

    #define     BA_Gbl_DV0_FIDCntl_SLP_DI                      0x02B0
    #define     B16Gbl_DV0_FIDCntl_SLP_DI                      0x02B0
    #define   LSb32Gbl_DV0_FIDCntl_SLP_DI                         0
    #define   LSb16Gbl_DV0_FIDCntl_SLP_DI                         0
    #define       bGbl_DV0_FIDCntl_SLP_DI                      1
    #define   MSK32Gbl_DV0_FIDCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_DV0_FIDCntl_PU_EN                       0x02B0
    #define     B16Gbl_DV0_FIDCntl_PU_EN                       0x02B0
    #define   LSb32Gbl_DV0_FIDCntl_PU_EN                          1
    #define   LSb16Gbl_DV0_FIDCntl_PU_EN                          1
    #define       bGbl_DV0_FIDCntl_PU_EN                       1
    #define   MSK32Gbl_DV0_FIDCntl_PU_EN                          0x00000002

    #define     BA_Gbl_DV0_FIDCntl_PD_EN                       0x02B0
    #define     B16Gbl_DV0_FIDCntl_PD_EN                       0x02B0
    #define   LSb32Gbl_DV0_FIDCntl_PD_EN                          2
    #define   LSb16Gbl_DV0_FIDCntl_PD_EN                          2
    #define       bGbl_DV0_FIDCntl_PD_EN                       1
    #define   MSK32Gbl_DV0_FIDCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DV1_CLKCntl                             0x02B4

    #define     BA_Gbl_DV1_CLKCntl_SLP_DI                      0x02B4
    #define     B16Gbl_DV1_CLKCntl_SLP_DI                      0x02B4
    #define   LSb32Gbl_DV1_CLKCntl_SLP_DI                         0
    #define   LSb16Gbl_DV1_CLKCntl_SLP_DI                         0
    #define       bGbl_DV1_CLKCntl_SLP_DI                      1
    #define   MSK32Gbl_DV1_CLKCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_DV1_CLKCntl_PU_EN                       0x02B4
    #define     B16Gbl_DV1_CLKCntl_PU_EN                       0x02B4
    #define   LSb32Gbl_DV1_CLKCntl_PU_EN                          1
    #define   LSb16Gbl_DV1_CLKCntl_PU_EN                          1
    #define       bGbl_DV1_CLKCntl_PU_EN                       1
    #define   MSK32Gbl_DV1_CLKCntl_PU_EN                          0x00000002

    #define     BA_Gbl_DV1_CLKCntl_PD_EN                       0x02B4
    #define     B16Gbl_DV1_CLKCntl_PD_EN                       0x02B4
    #define   LSb32Gbl_DV1_CLKCntl_PD_EN                          2
    #define   LSb16Gbl_DV1_CLKCntl_PD_EN                          2
    #define       bGbl_DV1_CLKCntl_PD_EN                       1
    #define   MSK32Gbl_DV1_CLKCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DV1_HSCntl                              0x02B8

    #define     BA_Gbl_DV1_HSCntl_SLP_DI                       0x02B8
    #define     B16Gbl_DV1_HSCntl_SLP_DI                       0x02B8
    #define   LSb32Gbl_DV1_HSCntl_SLP_DI                          0
    #define   LSb16Gbl_DV1_HSCntl_SLP_DI                          0
    #define       bGbl_DV1_HSCntl_SLP_DI                       1
    #define   MSK32Gbl_DV1_HSCntl_SLP_DI                          0x00000001

    #define     BA_Gbl_DV1_HSCntl_PU_EN                        0x02B8
    #define     B16Gbl_DV1_HSCntl_PU_EN                        0x02B8
    #define   LSb32Gbl_DV1_HSCntl_PU_EN                           1
    #define   LSb16Gbl_DV1_HSCntl_PU_EN                           1
    #define       bGbl_DV1_HSCntl_PU_EN                        1
    #define   MSK32Gbl_DV1_HSCntl_PU_EN                           0x00000002

    #define     BA_Gbl_DV1_HSCntl_PD_EN                        0x02B8
    #define     B16Gbl_DV1_HSCntl_PD_EN                        0x02B8
    #define   LSb32Gbl_DV1_HSCntl_PD_EN                           2
    #define   LSb16Gbl_DV1_HSCntl_PD_EN                           2
    #define       bGbl_DV1_HSCntl_PD_EN                        1
    #define   MSK32Gbl_DV1_HSCntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DV1_VSCntl                              0x02BC

    #define     BA_Gbl_DV1_VSCntl_SLP_DI                       0x02BC
    #define     B16Gbl_DV1_VSCntl_SLP_DI                       0x02BC
    #define   LSb32Gbl_DV1_VSCntl_SLP_DI                          0
    #define   LSb16Gbl_DV1_VSCntl_SLP_DI                          0
    #define       bGbl_DV1_VSCntl_SLP_DI                       1
    #define   MSK32Gbl_DV1_VSCntl_SLP_DI                          0x00000001

    #define     BA_Gbl_DV1_VSCntl_PU_EN                        0x02BC
    #define     B16Gbl_DV1_VSCntl_PU_EN                        0x02BC
    #define   LSb32Gbl_DV1_VSCntl_PU_EN                           1
    #define   LSb16Gbl_DV1_VSCntl_PU_EN                           1
    #define       bGbl_DV1_VSCntl_PU_EN                        1
    #define   MSK32Gbl_DV1_VSCntl_PU_EN                           0x00000002

    #define     BA_Gbl_DV1_VSCntl_PD_EN                        0x02BC
    #define     B16Gbl_DV1_VSCntl_PD_EN                        0x02BC
    #define   LSb32Gbl_DV1_VSCntl_PD_EN                           2
    #define   LSb16Gbl_DV1_VSCntl_PD_EN                           2
    #define       bGbl_DV1_VSCntl_PD_EN                        1
    #define   MSK32Gbl_DV1_VSCntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_DV1_FIDCntl                             0x02C0

    #define     BA_Gbl_DV1_FIDCntl_SLP_DI                      0x02C0
    #define     B16Gbl_DV1_FIDCntl_SLP_DI                      0x02C0
    #define   LSb32Gbl_DV1_FIDCntl_SLP_DI                         0
    #define   LSb16Gbl_DV1_FIDCntl_SLP_DI                         0
    #define       bGbl_DV1_FIDCntl_SLP_DI                      1
    #define   MSK32Gbl_DV1_FIDCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_DV1_FIDCntl_PU_EN                       0x02C0
    #define     B16Gbl_DV1_FIDCntl_PU_EN                       0x02C0
    #define   LSb32Gbl_DV1_FIDCntl_PU_EN                          1
    #define   LSb16Gbl_DV1_FIDCntl_PU_EN                          1
    #define       bGbl_DV1_FIDCntl_PU_EN                       1
    #define   MSK32Gbl_DV1_FIDCntl_PU_EN                          0x00000002

    #define     BA_Gbl_DV1_FIDCntl_PD_EN                       0x02C0
    #define     B16Gbl_DV1_FIDCntl_PD_EN                       0x02C0
    #define   LSb32Gbl_DV1_FIDCntl_PD_EN                          2
    #define   LSb16Gbl_DV1_FIDCntl_PD_EN                          2
    #define       bGbl_DV1_FIDCntl_PD_EN                       1
    #define   MSK32Gbl_DV1_FIDCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S0_BCLKCntl                           0x02C4

    #define     BA_Gbl_I2S0_BCLKCntl_SLP_DI                    0x02C4
    #define     B16Gbl_I2S0_BCLKCntl_SLP_DI                    0x02C4
    #define   LSb32Gbl_I2S0_BCLKCntl_SLP_DI                       0
    #define   LSb16Gbl_I2S0_BCLKCntl_SLP_DI                       0
    #define       bGbl_I2S0_BCLKCntl_SLP_DI                    1
    #define   MSK32Gbl_I2S0_BCLKCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_I2S0_BCLKCntl_PU_EN                     0x02C4
    #define     B16Gbl_I2S0_BCLKCntl_PU_EN                     0x02C4
    #define   LSb32Gbl_I2S0_BCLKCntl_PU_EN                        1
    #define   LSb16Gbl_I2S0_BCLKCntl_PU_EN                        1
    #define       bGbl_I2S0_BCLKCntl_PU_EN                     1
    #define   MSK32Gbl_I2S0_BCLKCntl_PU_EN                        0x00000002

    #define     BA_Gbl_I2S0_BCLKCntl_PD_EN                     0x02C4
    #define     B16Gbl_I2S0_BCLKCntl_PD_EN                     0x02C4
    #define   LSb32Gbl_I2S0_BCLKCntl_PD_EN                        2
    #define   LSb16Gbl_I2S0_BCLKCntl_PD_EN                        2
    #define       bGbl_I2S0_BCLKCntl_PD_EN                     1
    #define   MSK32Gbl_I2S0_BCLKCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S0_LRCKCntl                           0x02C8

    #define     BA_Gbl_I2S0_LRCKCntl_SLP_DI                    0x02C8
    #define     B16Gbl_I2S0_LRCKCntl_SLP_DI                    0x02C8
    #define   LSb32Gbl_I2S0_LRCKCntl_SLP_DI                       0
    #define   LSb16Gbl_I2S0_LRCKCntl_SLP_DI                       0
    #define       bGbl_I2S0_LRCKCntl_SLP_DI                    1
    #define   MSK32Gbl_I2S0_LRCKCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_I2S0_LRCKCntl_PU_EN                     0x02C8
    #define     B16Gbl_I2S0_LRCKCntl_PU_EN                     0x02C8
    #define   LSb32Gbl_I2S0_LRCKCntl_PU_EN                        1
    #define   LSb16Gbl_I2S0_LRCKCntl_PU_EN                        1
    #define       bGbl_I2S0_LRCKCntl_PU_EN                     1
    #define   MSK32Gbl_I2S0_LRCKCntl_PU_EN                        0x00000002

    #define     BA_Gbl_I2S0_LRCKCntl_PD_EN                     0x02C8
    #define     B16Gbl_I2S0_LRCKCntl_PD_EN                     0x02C8
    #define   LSb32Gbl_I2S0_LRCKCntl_PD_EN                        2
    #define   LSb16Gbl_I2S0_LRCKCntl_PD_EN                        2
    #define       bGbl_I2S0_LRCKCntl_PD_EN                     1
    #define   MSK32Gbl_I2S0_LRCKCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S0_DO0Cntl                            0x02CC

    #define     BA_Gbl_I2S0_DO0Cntl_SLP_DI                     0x02CC
    #define     B16Gbl_I2S0_DO0Cntl_SLP_DI                     0x02CC
    #define   LSb32Gbl_I2S0_DO0Cntl_SLP_DI                        0
    #define   LSb16Gbl_I2S0_DO0Cntl_SLP_DI                        0
    #define       bGbl_I2S0_DO0Cntl_SLP_DI                     1
    #define   MSK32Gbl_I2S0_DO0Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_I2S0_DO0Cntl_PU_EN                      0x02CC
    #define     B16Gbl_I2S0_DO0Cntl_PU_EN                      0x02CC
    #define   LSb32Gbl_I2S0_DO0Cntl_PU_EN                         1
    #define   LSb16Gbl_I2S0_DO0Cntl_PU_EN                         1
    #define       bGbl_I2S0_DO0Cntl_PU_EN                      1
    #define   MSK32Gbl_I2S0_DO0Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_I2S0_DO0Cntl_PD_EN                      0x02CC
    #define     B16Gbl_I2S0_DO0Cntl_PD_EN                      0x02CC
    #define   LSb32Gbl_I2S0_DO0Cntl_PD_EN                         2
    #define   LSb16Gbl_I2S0_DO0Cntl_PD_EN                         2
    #define       bGbl_I2S0_DO0Cntl_PD_EN                      1
    #define   MSK32Gbl_I2S0_DO0Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S0_DO1Cntl                            0x02D0

    #define     BA_Gbl_I2S0_DO1Cntl_SLP_DI                     0x02D0
    #define     B16Gbl_I2S0_DO1Cntl_SLP_DI                     0x02D0
    #define   LSb32Gbl_I2S0_DO1Cntl_SLP_DI                        0
    #define   LSb16Gbl_I2S0_DO1Cntl_SLP_DI                        0
    #define       bGbl_I2S0_DO1Cntl_SLP_DI                     1
    #define   MSK32Gbl_I2S0_DO1Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_I2S0_DO1Cntl_PU_EN                      0x02D0
    #define     B16Gbl_I2S0_DO1Cntl_PU_EN                      0x02D0
    #define   LSb32Gbl_I2S0_DO1Cntl_PU_EN                         1
    #define   LSb16Gbl_I2S0_DO1Cntl_PU_EN                         1
    #define       bGbl_I2S0_DO1Cntl_PU_EN                      1
    #define   MSK32Gbl_I2S0_DO1Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_I2S0_DO1Cntl_PD_EN                      0x02D0
    #define     B16Gbl_I2S0_DO1Cntl_PD_EN                      0x02D0
    #define   LSb32Gbl_I2S0_DO1Cntl_PD_EN                         2
    #define   LSb16Gbl_I2S0_DO1Cntl_PD_EN                         2
    #define       bGbl_I2S0_DO1Cntl_PD_EN                      1
    #define   MSK32Gbl_I2S0_DO1Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S0_DO2Cntl                            0x02D4

    #define     BA_Gbl_I2S0_DO2Cntl_SLP_DI                     0x02D4
    #define     B16Gbl_I2S0_DO2Cntl_SLP_DI                     0x02D4
    #define   LSb32Gbl_I2S0_DO2Cntl_SLP_DI                        0
    #define   LSb16Gbl_I2S0_DO2Cntl_SLP_DI                        0
    #define       bGbl_I2S0_DO2Cntl_SLP_DI                     1
    #define   MSK32Gbl_I2S0_DO2Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_I2S0_DO2Cntl_PU_EN                      0x02D4
    #define     B16Gbl_I2S0_DO2Cntl_PU_EN                      0x02D4
    #define   LSb32Gbl_I2S0_DO2Cntl_PU_EN                         1
    #define   LSb16Gbl_I2S0_DO2Cntl_PU_EN                         1
    #define       bGbl_I2S0_DO2Cntl_PU_EN                      1
    #define   MSK32Gbl_I2S0_DO2Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_I2S0_DO2Cntl_PD_EN                      0x02D4
    #define     B16Gbl_I2S0_DO2Cntl_PD_EN                      0x02D4
    #define   LSb32Gbl_I2S0_DO2Cntl_PD_EN                         2
    #define   LSb16Gbl_I2S0_DO2Cntl_PD_EN                         2
    #define       bGbl_I2S0_DO2Cntl_PD_EN                      1
    #define   MSK32Gbl_I2S0_DO2Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S0_DO3Cntl                            0x02D8

    #define     BA_Gbl_I2S0_DO3Cntl_SLP_DI                     0x02D8
    #define     B16Gbl_I2S0_DO3Cntl_SLP_DI                     0x02D8
    #define   LSb32Gbl_I2S0_DO3Cntl_SLP_DI                        0
    #define   LSb16Gbl_I2S0_DO3Cntl_SLP_DI                        0
    #define       bGbl_I2S0_DO3Cntl_SLP_DI                     1
    #define   MSK32Gbl_I2S0_DO3Cntl_SLP_DI                        0x00000001

    #define     BA_Gbl_I2S0_DO3Cntl_PU_EN                      0x02D8
    #define     B16Gbl_I2S0_DO3Cntl_PU_EN                      0x02D8
    #define   LSb32Gbl_I2S0_DO3Cntl_PU_EN                         1
    #define   LSb16Gbl_I2S0_DO3Cntl_PU_EN                         1
    #define       bGbl_I2S0_DO3Cntl_PU_EN                      1
    #define   MSK32Gbl_I2S0_DO3Cntl_PU_EN                         0x00000002

    #define     BA_Gbl_I2S0_DO3Cntl_PD_EN                      0x02D8
    #define     B16Gbl_I2S0_DO3Cntl_PD_EN                      0x02D8
    #define   LSb32Gbl_I2S0_DO3Cntl_PD_EN                         2
    #define   LSb16Gbl_I2S0_DO3Cntl_PD_EN                         2
    #define       bGbl_I2S0_DO3Cntl_PD_EN                      1
    #define   MSK32Gbl_I2S0_DO3Cntl_PD_EN                         0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S1_LRCKCntl                           0x02DC

    #define     BA_Gbl_I2S1_LRCKCntl_SLP_DI                    0x02DC
    #define     B16Gbl_I2S1_LRCKCntl_SLP_DI                    0x02DC
    #define   LSb32Gbl_I2S1_LRCKCntl_SLP_DI                       0
    #define   LSb16Gbl_I2S1_LRCKCntl_SLP_DI                       0
    #define       bGbl_I2S1_LRCKCntl_SLP_DI                    1
    #define   MSK32Gbl_I2S1_LRCKCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_I2S1_LRCKCntl_PU_EN                     0x02DC
    #define     B16Gbl_I2S1_LRCKCntl_PU_EN                     0x02DC
    #define   LSb32Gbl_I2S1_LRCKCntl_PU_EN                        1
    #define   LSb16Gbl_I2S1_LRCKCntl_PU_EN                        1
    #define       bGbl_I2S1_LRCKCntl_PU_EN                     1
    #define   MSK32Gbl_I2S1_LRCKCntl_PU_EN                        0x00000002

    #define     BA_Gbl_I2S1_LRCKCntl_PD_EN                     0x02DC
    #define     B16Gbl_I2S1_LRCKCntl_PD_EN                     0x02DC
    #define   LSb32Gbl_I2S1_LRCKCntl_PD_EN                        2
    #define   LSb16Gbl_I2S1_LRCKCntl_PD_EN                        2
    #define       bGbl_I2S1_LRCKCntl_PD_EN                     1
    #define   MSK32Gbl_I2S1_LRCKCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S1_BCLKCntl                           0x02E0

    #define     BA_Gbl_I2S1_BCLKCntl_SLP_DI                    0x02E0
    #define     B16Gbl_I2S1_BCLKCntl_SLP_DI                    0x02E0
    #define   LSb32Gbl_I2S1_BCLKCntl_SLP_DI                       0
    #define   LSb16Gbl_I2S1_BCLKCntl_SLP_DI                       0
    #define       bGbl_I2S1_BCLKCntl_SLP_DI                    1
    #define   MSK32Gbl_I2S1_BCLKCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_I2S1_BCLKCntl_PU_EN                     0x02E0
    #define     B16Gbl_I2S1_BCLKCntl_PU_EN                     0x02E0
    #define   LSb32Gbl_I2S1_BCLKCntl_PU_EN                        1
    #define   LSb16Gbl_I2S1_BCLKCntl_PU_EN                        1
    #define       bGbl_I2S1_BCLKCntl_PU_EN                     1
    #define   MSK32Gbl_I2S1_BCLKCntl_PU_EN                        0x00000002

    #define     BA_Gbl_I2S1_BCLKCntl_PD_EN                     0x02E0
    #define     B16Gbl_I2S1_BCLKCntl_PD_EN                     0x02E0
    #define   LSb32Gbl_I2S1_BCLKCntl_PD_EN                        2
    #define   LSb16Gbl_I2S1_BCLKCntl_PD_EN                        2
    #define       bGbl_I2S1_BCLKCntl_PD_EN                     1
    #define   MSK32Gbl_I2S1_BCLKCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S1_DOCntl                             0x02E4

    #define     BA_Gbl_I2S1_DOCntl_SLP_DI                      0x02E4
    #define     B16Gbl_I2S1_DOCntl_SLP_DI                      0x02E4
    #define   LSb32Gbl_I2S1_DOCntl_SLP_DI                         0
    #define   LSb16Gbl_I2S1_DOCntl_SLP_DI                         0
    #define       bGbl_I2S1_DOCntl_SLP_DI                      1
    #define   MSK32Gbl_I2S1_DOCntl_SLP_DI                         0x00000001

    #define     BA_Gbl_I2S1_DOCntl_PU_EN                       0x02E4
    #define     B16Gbl_I2S1_DOCntl_PU_EN                       0x02E4
    #define   LSb32Gbl_I2S1_DOCntl_PU_EN                          1
    #define   LSb16Gbl_I2S1_DOCntl_PU_EN                          1
    #define       bGbl_I2S1_DOCntl_PU_EN                       1
    #define   MSK32Gbl_I2S1_DOCntl_PU_EN                          0x00000002

    #define     BA_Gbl_I2S1_DOCntl_PD_EN                       0x02E4
    #define     B16Gbl_I2S1_DOCntl_PD_EN                       0x02E4
    #define   LSb32Gbl_I2S1_DOCntl_PD_EN                          2
    #define   LSb16Gbl_I2S1_DOCntl_PD_EN                          2
    #define       bGbl_I2S1_DOCntl_PD_EN                       1
    #define   MSK32Gbl_I2S1_DOCntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S2_LRCKCntl                           0x02E8

    #define     BA_Gbl_I2S2_LRCKCntl_SLP_DI                    0x02E8
    #define     B16Gbl_I2S2_LRCKCntl_SLP_DI                    0x02E8
    #define   LSb32Gbl_I2S2_LRCKCntl_SLP_DI                       0
    #define   LSb16Gbl_I2S2_LRCKCntl_SLP_DI                       0
    #define       bGbl_I2S2_LRCKCntl_SLP_DI                    1
    #define   MSK32Gbl_I2S2_LRCKCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_I2S2_LRCKCntl_PU_EN                     0x02E8
    #define     B16Gbl_I2S2_LRCKCntl_PU_EN                     0x02E8
    #define   LSb32Gbl_I2S2_LRCKCntl_PU_EN                        1
    #define   LSb16Gbl_I2S2_LRCKCntl_PU_EN                        1
    #define       bGbl_I2S2_LRCKCntl_PU_EN                     1
    #define   MSK32Gbl_I2S2_LRCKCntl_PU_EN                        0x00000002

    #define     BA_Gbl_I2S2_LRCKCntl_PD_EN                     0x02E8
    #define     B16Gbl_I2S2_LRCKCntl_PD_EN                     0x02E8
    #define   LSb32Gbl_I2S2_LRCKCntl_PD_EN                        2
    #define   LSb16Gbl_I2S2_LRCKCntl_PD_EN                        2
    #define       bGbl_I2S2_LRCKCntl_PD_EN                     1
    #define   MSK32Gbl_I2S2_LRCKCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S2_BCLKCntl                           0x02EC

    #define     BA_Gbl_I2S2_BCLKCntl_SLP_DI                    0x02EC
    #define     B16Gbl_I2S2_BCLKCntl_SLP_DI                    0x02EC
    #define   LSb32Gbl_I2S2_BCLKCntl_SLP_DI                       0
    #define   LSb16Gbl_I2S2_BCLKCntl_SLP_DI                       0
    #define       bGbl_I2S2_BCLKCntl_SLP_DI                    1
    #define   MSK32Gbl_I2S2_BCLKCntl_SLP_DI                       0x00000001

    #define     BA_Gbl_I2S2_BCLKCntl_PU_EN                     0x02EC
    #define     B16Gbl_I2S2_BCLKCntl_PU_EN                     0x02EC
    #define   LSb32Gbl_I2S2_BCLKCntl_PU_EN                        1
    #define   LSb16Gbl_I2S2_BCLKCntl_PU_EN                        1
    #define       bGbl_I2S2_BCLKCntl_PU_EN                     1
    #define   MSK32Gbl_I2S2_BCLKCntl_PU_EN                        0x00000002

    #define     BA_Gbl_I2S2_BCLKCntl_PD_EN                     0x02EC
    #define     B16Gbl_I2S2_BCLKCntl_PD_EN                     0x02EC
    #define   LSb32Gbl_I2S2_BCLKCntl_PD_EN                        2
    #define   LSb16Gbl_I2S2_BCLKCntl_PD_EN                        2
    #define       bGbl_I2S2_BCLKCntl_PD_EN                     1
    #define   MSK32Gbl_I2S2_BCLKCntl_PD_EN                        0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_I2S2_DICntl                             0x02F0

    #define     BA_Gbl_I2S2_DICntl_SLP_DI                      0x02F0
    #define     B16Gbl_I2S2_DICntl_SLP_DI                      0x02F0
    #define   LSb32Gbl_I2S2_DICntl_SLP_DI                         0
    #define   LSb16Gbl_I2S2_DICntl_SLP_DI                         0
    #define       bGbl_I2S2_DICntl_SLP_DI                      1
    #define   MSK32Gbl_I2S2_DICntl_SLP_DI                         0x00000001

    #define     BA_Gbl_I2S2_DICntl_PU_EN                       0x02F0
    #define     B16Gbl_I2S2_DICntl_PU_EN                       0x02F0
    #define   LSb32Gbl_I2S2_DICntl_PU_EN                          1
    #define   LSb16Gbl_I2S2_DICntl_PU_EN                          1
    #define       bGbl_I2S2_DICntl_PU_EN                       1
    #define   MSK32Gbl_I2S2_DICntl_PU_EN                          0x00000002

    #define     BA_Gbl_I2S2_DICntl_PD_EN                       0x02F0
    #define     B16Gbl_I2S2_DICntl_PD_EN                       0x02F0
    #define   LSb32Gbl_I2S2_DICntl_PD_EN                          2
    #define   LSb16Gbl_I2S2_DICntl_PD_EN                          2
    #define       bGbl_I2S2_DICntl_PD_EN                       1
    #define   MSK32Gbl_I2S2_DICntl_PD_EN                          0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_SPDIFOCntl                              0x02F4

    #define     BA_Gbl_SPDIFOCntl_SLP_DI                       0x02F4
    #define     B16Gbl_SPDIFOCntl_SLP_DI                       0x02F4
    #define   LSb32Gbl_SPDIFOCntl_SLP_DI                          0
    #define   LSb16Gbl_SPDIFOCntl_SLP_DI                          0
    #define       bGbl_SPDIFOCntl_SLP_DI                       1
    #define   MSK32Gbl_SPDIFOCntl_SLP_DI                          0x00000001

    #define     BA_Gbl_SPDIFOCntl_PU_EN                        0x02F4
    #define     B16Gbl_SPDIFOCntl_PU_EN                        0x02F4
    #define   LSb32Gbl_SPDIFOCntl_PU_EN                           1
    #define   LSb16Gbl_SPDIFOCntl_PU_EN                           1
    #define       bGbl_SPDIFOCntl_PU_EN                        1
    #define   MSK32Gbl_SPDIFOCntl_PU_EN                           0x00000002

    #define     BA_Gbl_SPDIFOCntl_PD_EN                        0x02F4
    #define     B16Gbl_SPDIFOCntl_PD_EN                        0x02F4
    #define   LSb32Gbl_SPDIFOCntl_PD_EN                           2
    #define   LSb16Gbl_SPDIFOCntl_PD_EN                           2
    #define       bGbl_SPDIFOCntl_PD_EN                        1
    #define   MSK32Gbl_SPDIFOCntl_PD_EN                           0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_Gbl_HDMI_CECCntl                            0x02F8

    #define     BA_Gbl_HDMI_CECCntl_SLP_DI                     0x02F8
    #define     B16Gbl_HDMI_CECCntl_SLP_DI                     0x02F8
    #define   LSb32Gbl_HDMI_CECCntl_SLP_DI                        0
    #define   LSb16Gbl_HDMI_CECCntl_SLP_DI                        0
    #define       bGbl_HDMI_CECCntl_SLP_DI                     1
    #define   MSK32Gbl_HDMI_CECCntl_SLP_DI                        0x00000001
    ///////////////////////////////////////////////////////////

    typedef struct SIE_Gbl {
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_pinMux_gp0(r32)                         _BFGET_(r32, 0, 0)
    #define   SET32Gbl_pinMux_gp0(r32,v)                       _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_pinMux_gp0(r16)                         _BFGET_(r16, 0, 0)
    #define   SET16Gbl_pinMux_gp0(r16,v)                       _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_pinMux_gp1(r32)                         _BFGET_(r32, 2, 1)
    #define   SET32Gbl_pinMux_gp1(r32,v)                       _BFSET_(r32, 2, 1,v)
    #define   GET16Gbl_pinMux_gp1(r16)                         _BFGET_(r16, 2, 1)
    #define   SET16Gbl_pinMux_gp1(r16,v)                       _BFSET_(r16, 2, 1,v)

    #define   GET32Gbl_pinMux_gp2(r32)                         _BFGET_(r32, 4, 3)
    #define   SET32Gbl_pinMux_gp2(r32,v)                       _BFSET_(r32, 4, 3,v)
    #define   GET16Gbl_pinMux_gp2(r16)                         _BFGET_(r16, 4, 3)
    #define   SET16Gbl_pinMux_gp2(r16,v)                       _BFSET_(r16, 4, 3,v)

    #define   GET32Gbl_pinMux_gp3(r32)                         _BFGET_(r32, 6, 5)
    #define   SET32Gbl_pinMux_gp3(r32,v)                       _BFSET_(r32, 6, 5,v)
    #define   GET16Gbl_pinMux_gp3(r16)                         _BFGET_(r16, 6, 5)
    #define   SET16Gbl_pinMux_gp3(r16,v)                       _BFSET_(r16, 6, 5,v)

    #define   GET32Gbl_pinMux_gp4(r32)                         _BFGET_(r32, 8, 7)
    #define   SET32Gbl_pinMux_gp4(r32,v)                       _BFSET_(r32, 8, 7,v)
    #define   GET16Gbl_pinMux_gp4(r16)                         _BFGET_(r16, 8, 7)
    #define   SET16Gbl_pinMux_gp4(r16,v)                       _BFSET_(r16, 8, 7,v)

    #define   GET32Gbl_pinMux_gp5(r32)                         _BFGET_(r32, 9, 9)
    #define   SET32Gbl_pinMux_gp5(r32,v)                       _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_pinMux_gp5(r16)                         _BFGET_(r16, 9, 9)
    #define   SET16Gbl_pinMux_gp5(r16,v)                       _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_pinMux_gp6(r32)                         _BFGET_(r32,11,10)
    #define   SET32Gbl_pinMux_gp6(r32,v)                       _BFSET_(r32,11,10,v)
    #define   GET16Gbl_pinMux_gp6(r16)                         _BFGET_(r16,11,10)
    #define   SET16Gbl_pinMux_gp6(r16,v)                       _BFSET_(r16,11,10,v)

    #define   GET32Gbl_pinMux_gp7(r32)                         _BFGET_(r32,13,12)
    #define   SET32Gbl_pinMux_gp7(r32,v)                       _BFSET_(r32,13,12,v)
    #define   GET16Gbl_pinMux_gp7(r16)                         _BFGET_(r16,13,12)
    #define   SET16Gbl_pinMux_gp7(r16,v)                       _BFSET_(r16,13,12,v)

    #define   GET32Gbl_pinMux_gp8(r32)                         _BFGET_(r32,15,14)
    #define   SET32Gbl_pinMux_gp8(r32,v)                       _BFSET_(r32,15,14,v)
    #define   GET16Gbl_pinMux_gp8(r16)                         _BFGET_(r16,15,14)
    #define   SET16Gbl_pinMux_gp8(r16,v)                       _BFSET_(r16,15,14,v)

    #define   GET32Gbl_pinMux_gp9(r32)                         _BFGET_(r32,17,16)
    #define   SET32Gbl_pinMux_gp9(r32,v)                       _BFSET_(r32,17,16,v)
    #define   GET16Gbl_pinMux_gp9(r16)                         _BFGET_(r16, 1, 0)
    #define   SET16Gbl_pinMux_gp9(r16,v)                       _BFSET_(r16, 1, 0,v)

    #define   GET32Gbl_pinMux_gp10(r32)                        _BFGET_(r32,19,18)
    #define   SET32Gbl_pinMux_gp10(r32,v)                      _BFSET_(r32,19,18,v)
    #define   GET16Gbl_pinMux_gp10(r16)                        _BFGET_(r16, 3, 2)
    #define   SET16Gbl_pinMux_gp10(r16,v)                      _BFSET_(r16, 3, 2,v)

    #define   GET32Gbl_pinMux_gp11(r32)                        _BFGET_(r32,21,20)
    #define   SET32Gbl_pinMux_gp11(r32,v)                      _BFSET_(r32,21,20,v)
    #define   GET16Gbl_pinMux_gp11(r16)                        _BFGET_(r16, 5, 4)
    #define   SET16Gbl_pinMux_gp11(r16,v)                      _BFSET_(r16, 5, 4,v)

    #define   GET32Gbl_pinMux_gp12(r32)                        _BFGET_(r32,23,22)
    #define   SET32Gbl_pinMux_gp12(r32,v)                      _BFSET_(r32,23,22,v)
    #define   GET16Gbl_pinMux_gp12(r16)                        _BFGET_(r16, 7, 6)
    #define   SET16Gbl_pinMux_gp12(r16,v)                      _BFSET_(r16, 7, 6,v)

    #define   GET32Gbl_pinMux_gp13(r32)                        _BFGET_(r32,25,24)
    #define   SET32Gbl_pinMux_gp13(r32,v)                      _BFSET_(r32,25,24,v)
    #define   GET16Gbl_pinMux_gp13(r16)                        _BFGET_(r16, 9, 8)
    #define   SET16Gbl_pinMux_gp13(r16,v)                      _BFSET_(r16, 9, 8,v)

    #define   GET32Gbl_pinMux_gp14(r32)                        _BFGET_(r32,26,26)
    #define   SET32Gbl_pinMux_gp14(r32,v)                      _BFSET_(r32,26,26,v)
    #define   GET16Gbl_pinMux_gp14(r16)                        _BFGET_(r16,10,10)
    #define   SET16Gbl_pinMux_gp14(r16,v)                      _BFSET_(r16,10,10,v)

    #define   GET32Gbl_pinMux_gp15(r32)                        _BFGET_(r32,27,27)
    #define   SET32Gbl_pinMux_gp15(r32,v)                      _BFSET_(r32,27,27,v)
    #define   GET16Gbl_pinMux_gp15(r16)                        _BFGET_(r16,11,11)
    #define   SET16Gbl_pinMux_gp15(r16,v)                      _BFSET_(r16,11,11,v)

    #define   GET32Gbl_pinMux_gp16(r32)                        _BFGET_(r32,30,28)
    #define   SET32Gbl_pinMux_gp16(r32,v)                      _BFSET_(r32,30,28,v)
    #define   GET16Gbl_pinMux_gp16(r16)                        _BFGET_(r16,14,12)
    #define   SET16Gbl_pinMux_gp16(r16,v)                      _BFSET_(r16,14,12,v)

    #define     w32Gbl_pinMux                                  {\
            UNSG32 upinMux_gp0                                 :  1;\
            UNSG32 upinMux_gp1                                 :  2;\
            UNSG32 upinMux_gp2                                 :  2;\
            UNSG32 upinMux_gp3                                 :  2;\
            UNSG32 upinMux_gp4                                 :  2;\
            UNSG32 upinMux_gp5                                 :  1;\
            UNSG32 upinMux_gp6                                 :  2;\
            UNSG32 upinMux_gp7                                 :  2;\
            UNSG32 upinMux_gp8                                 :  2;\
            UNSG32 upinMux_gp9                                 :  2;\
            UNSG32 upinMux_gp10                                :  2;\
            UNSG32 upinMux_gp11                                :  2;\
            UNSG32 upinMux_gp12                                :  2;\
            UNSG32 upinMux_gp13                                :  2;\
            UNSG32 upinMux_gp14                                :  1;\
            UNSG32 upinMux_gp15                                :  1;\
            UNSG32 upinMux_gp16                                :  3;\
            UNSG32 RSVDx0_b31                                  :  1;\
          }
    union { UNSG32 u32Gbl_pinMux;
            struct w32Gbl_pinMux;
          };
    #define   GET32Gbl_pinMux_gp17(r32)                        _BFGET_(r32, 2, 0)
    #define   SET32Gbl_pinMux_gp17(r32,v)                      _BFSET_(r32, 2, 0,v)
    #define   GET16Gbl_pinMux_gp17(r16)                        _BFGET_(r16, 2, 0)
    #define   SET16Gbl_pinMux_gp17(r16,v)                      _BFSET_(r16, 2, 0,v)

    #define   GET32Gbl_pinMux_gp18(r32)                        _BFGET_(r32, 3, 3)
    #define   SET32Gbl_pinMux_gp18(r32,v)                      _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_pinMux_gp18(r16)                        _BFGET_(r16, 3, 3)
    #define   SET16Gbl_pinMux_gp18(r16,v)                      _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_pinMux_gp19(r32)                        _BFGET_(r32, 4, 4)
    #define   SET32Gbl_pinMux_gp19(r32,v)                      _BFSET_(r32, 4, 4,v)
    #define   GET16Gbl_pinMux_gp19(r16)                        _BFGET_(r16, 4, 4)
    #define   SET16Gbl_pinMux_gp19(r16,v)                      _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_pinMux_gp20(r32)                        _BFGET_(r32, 5, 5)
    #define   SET32Gbl_pinMux_gp20(r32,v)                      _BFSET_(r32, 5, 5,v)
    #define   GET16Gbl_pinMux_gp20(r16)                        _BFGET_(r16, 5, 5)
    #define   SET16Gbl_pinMux_gp20(r16,v)                      _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_pinMux_gp21(r32)                        _BFGET_(r32, 8, 6)
    #define   SET32Gbl_pinMux_gp21(r32,v)                      _BFSET_(r32, 8, 6,v)
    #define   GET16Gbl_pinMux_gp21(r16)                        _BFGET_(r16, 8, 6)
    #define   SET16Gbl_pinMux_gp21(r16,v)                      _BFSET_(r16, 8, 6,v)

    #define   GET32Gbl_pinMux_gp22(r32)                        _BFGET_(r32,11, 9)
    #define   SET32Gbl_pinMux_gp22(r32,v)                      _BFSET_(r32,11, 9,v)
    #define   GET16Gbl_pinMux_gp22(r16)                        _BFGET_(r16,11, 9)
    #define   SET16Gbl_pinMux_gp22(r16,v)                      _BFSET_(r16,11, 9,v)

    #define   GET32Gbl_pinMux_gp23(r32)                        _BFGET_(r32,14,12)
    #define   SET32Gbl_pinMux_gp23(r32,v)                      _BFSET_(r32,14,12,v)
    #define   GET16Gbl_pinMux_gp23(r16)                        _BFGET_(r16,14,12)
    #define   SET16Gbl_pinMux_gp23(r16,v)                      _BFSET_(r16,14,12,v)

    #define   GET32Gbl_pinMux_gp24(r32)                        _BFGET_(r32,16,15)
    #define   SET32Gbl_pinMux_gp24(r32,v)                      _BFSET_(r32,16,15,v)

    #define   GET32Gbl_pinMux_gp25(r32)                        _BFGET_(r32,18,17)
    #define   SET32Gbl_pinMux_gp25(r32,v)                      _BFSET_(r32,18,17,v)
    #define   GET16Gbl_pinMux_gp25(r16)                        _BFGET_(r16, 2, 1)
    #define   SET16Gbl_pinMux_gp25(r16,v)                      _BFSET_(r16, 2, 1,v)

    #define     w32Gbl_pinMux1                                 {\
            UNSG32 upinMux_gp17                                :  3;\
            UNSG32 upinMux_gp18                                :  1;\
            UNSG32 upinMux_gp19                                :  1;\
            UNSG32 upinMux_gp20                                :  1;\
            UNSG32 upinMux_gp21                                :  3;\
            UNSG32 upinMux_gp22                                :  3;\
            UNSG32 upinMux_gp23                                :  3;\
            UNSG32 upinMux_gp24                                :  2;\
            UNSG32 upinMux_gp25                                :  2;\
            UNSG32 RSVDx4_b19                                  : 13;\
          }
    union { UNSG32 u32Gbl_pinMux1;
            struct w32Gbl_pinMux1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_bootStrap_softwareStrap(r32)            _BFGET_(r32, 6, 0)
    #define   SET32Gbl_bootStrap_softwareStrap(r32,v)          _BFSET_(r32, 6, 0,v)
    #define   GET16Gbl_bootStrap_softwareStrap(r16)            _BFGET_(r16, 6, 0)
    #define   SET16Gbl_bootStrap_softwareStrap(r16,v)          _BFSET_(r16, 6, 0,v)

    #define   GET32Gbl_bootStrap_bootSrc(r32)                  _BFGET_(r32, 8, 7)
    #define   SET32Gbl_bootStrap_bootSrc(r32,v)                _BFSET_(r32, 8, 7,v)
    #define   GET16Gbl_bootStrap_bootSrc(r16)                  _BFGET_(r16, 8, 7)
    #define   SET16Gbl_bootStrap_bootSrc(r16,v)                _BFSET_(r16, 8, 7,v)

    #define   GET32Gbl_bootStrap_sysPllByps(r32)               _BFGET_(r32, 9, 9)
    #define   SET32Gbl_bootStrap_sysPllByps(r32,v)             _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_bootStrap_sysPllByps(r16)               _BFGET_(r16, 9, 9)
    #define   SET16Gbl_bootStrap_sysPllByps(r16,v)             _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_bootStrap_memPllByps(r32)               _BFGET_(r32,10,10)
    #define   SET32Gbl_bootStrap_memPllByps(r32,v)             _BFSET_(r32,10,10,v)
    #define   GET16Gbl_bootStrap_memPllByps(r16)               _BFGET_(r16,10,10)
    #define   SET16Gbl_bootStrap_memPllByps(r16,v)             _BFSET_(r16,10,10,v)

    #define   GET32Gbl_bootStrap_socJtagFromSM(r32)            _BFGET_(r32,11,11)
    #define   SET32Gbl_bootStrap_socJtagFromSM(r32,v)          _BFSET_(r32,11,11,v)
    #define   GET16Gbl_bootStrap_socJtagFromSM(r16)            _BFGET_(r16,11,11)
    #define   SET16Gbl_bootStrap_socJtagFromSM(r16,v)          _BFSET_(r16,11,11,v)

    #define   GET32Gbl_bootStrap_nandV18Enable(r32)            _BFGET_(r32,12,12)
    #define   SET32Gbl_bootStrap_nandV18Enable(r32,v)          _BFSET_(r32,12,12,v)
    #define   GET16Gbl_bootStrap_nandV18Enable(r16)            _BFGET_(r16,12,12)
    #define   SET16Gbl_bootStrap_nandV18Enable(r16,v)          _BFSET_(r16,12,12,v)

    #define   GET32Gbl_bootStrap_spi1V18Enable(r32)            _BFGET_(r32,13,13)
    #define   SET32Gbl_bootStrap_spi1V18Enable(r32,v)          _BFSET_(r32,13,13,v)
    #define   GET16Gbl_bootStrap_spi1V18Enable(r16)            _BFGET_(r16,13,13)
    #define   SET16Gbl_bootStrap_spi1V18Enable(r16,v)          _BFSET_(r16,13,13,v)

    #define   GET32Gbl_bootStrap_nandV25Enable(r32)            _BFGET_(r32,14,14)
    #define   SET32Gbl_bootStrap_nandV25Enable(r32,v)          _BFSET_(r32,14,14,v)
    #define   GET16Gbl_bootStrap_nandV25Enable(r16)            _BFGET_(r16,14,14)
    #define   SET16Gbl_bootStrap_nandV25Enable(r16,v)          _BFSET_(r16,14,14,v)

    #define   GET32Gbl_bootStrap_spi1V25Enable(r32)            _BFGET_(r32,15,15)
    #define   SET32Gbl_bootStrap_spi1V25Enable(r32,v)          _BFSET_(r32,15,15,v)
    #define   GET16Gbl_bootStrap_spi1V25Enable(r16)            _BFGET_(r16,15,15)
    #define   SET16Gbl_bootStrap_spi1V25Enable(r16,v)          _BFSET_(r16,15,15,v)

    #define   GET32Gbl_bootStrap_ENG_EN(r32)                   _BFGET_(r32,16,16)
    #define   SET32Gbl_bootStrap_ENG_EN(r32,v)                 _BFSET_(r32,16,16,v)
    #define   GET16Gbl_bootStrap_ENG_EN(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_bootStrap_ENG_EN(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_bootStrap                               {\
            UNSG32 ubootStrap_softwareStrap                    :  7;\
            UNSG32 ubootStrap_bootSrc                          :  2;\
            UNSG32 ubootStrap_sysPllByps                       :  1;\
            UNSG32 ubootStrap_memPllByps                       :  1;\
            UNSG32 ubootStrap_socJtagFromSM                    :  1;\
            UNSG32 ubootStrap_nandV18Enable                    :  1;\
            UNSG32 ubootStrap_spi1V18Enable                    :  1;\
            UNSG32 ubootStrap_nandV25Enable                    :  1;\
            UNSG32 ubootStrap_spi1V25Enable                    :  1;\
            UNSG32 ubootStrap_ENG_EN                           :  1;\
            UNSG32 RSVDx8_b17                                  : 15;\
          }
    union { UNSG32 u32Gbl_bootStrap;
            struct w32Gbl_bootStrap;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_chipCntl_pcieDownMap(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_chipCntl_pcieDownMap(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_chipCntl_pcieDownMap(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_chipCntl_pcieDownMap(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_chipCntl_pcie_rc(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_chipCntl_pcie_rc(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_chipCntl_pcie_rc(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_chipCntl_pcie_rc(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define     w32Gbl_chipCntl                                {\
            UNSG32 uchipCntl_pcieDownMap                       :  1;\
            UNSG32 uchipCntl_pcie_rc                           :  1;\
            UNSG32 RSVDxC_b2                                   : 30;\
          }
    union { UNSG32 u32Gbl_chipCntl;
            struct w32Gbl_chipCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_anaGrpCtl_anaGrpPuIvref(r32)            _BFGET_(r32, 0, 0)
    #define   SET32Gbl_anaGrpCtl_anaGrpPuIvref(r32,v)          _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_anaGrpCtl_anaGrpPuIvref(r16)            _BFGET_(r16, 0, 0)
    #define   SET16Gbl_anaGrpCtl_anaGrpPuIvref(r16,v)          _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_anaGrpCtl_anaGrpBgrSel(r32)             _BFGET_(r32, 1, 1)
    #define   SET32Gbl_anaGrpCtl_anaGrpBgrSel(r32,v)           _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_anaGrpCtl_anaGrpBgrSel(r16)             _BFGET_(r16, 1, 1)
    #define   SET16Gbl_anaGrpCtl_anaGrpBgrSel(r16,v)           _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_anaGrpCtl_anaGrpPuXtl(r32)              _BFGET_(r32, 2, 2)
    #define   SET32Gbl_anaGrpCtl_anaGrpPuXtl(r32,v)            _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_anaGrpCtl_anaGrpPuXtl(r16)              _BFGET_(r16, 2, 2)
    #define   SET16Gbl_anaGrpCtl_anaGrpPuXtl(r16,v)            _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_anaGrpCtl_anaGrpGainX2(r32)             _BFGET_(r32, 3, 3)
    #define   SET32Gbl_anaGrpCtl_anaGrpGainX2(r32,v)           _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_anaGrpCtl_anaGrpGainX2(r16)             _BFGET_(r16, 3, 3)
    #define   SET16Gbl_anaGrpCtl_anaGrpGainX2(r16,v)           _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_anaGrpCtl_anaGrpTestAna(r32)            _BFGET_(r32, 6, 4)
    #define   SET32Gbl_anaGrpCtl_anaGrpTestAna(r32,v)          _BFSET_(r32, 6, 4,v)
    #define   GET16Gbl_anaGrpCtl_anaGrpTestAna(r16)            _BFGET_(r16, 6, 4)
    #define   SET16Gbl_anaGrpCtl_anaGrpTestAna(r16,v)          _BFSET_(r16, 6, 4,v)

    #define     w32Gbl_anaGrpCtl                               {\
            UNSG32 uanaGrpCtl_anaGrpPuIvref                    :  1;\
            UNSG32 uanaGrpCtl_anaGrpBgrSel                     :  1;\
            UNSG32 uanaGrpCtl_anaGrpPuXtl                      :  1;\
            UNSG32 uanaGrpCtl_anaGrpGainX2                     :  1;\
            UNSG32 uanaGrpCtl_anaGrpTestAna                    :  3;\
            UNSG32 RSVDx10_b7                                  : 25;\
          }
    union { UNSG32 u32Gbl_anaGrpCtl;
            struct w32Gbl_anaGrpCtl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_sysPllCtl_sysPllPu(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_sysPllCtl_sysPllPu(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_sysPllCtl_sysPllPu(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_sysPllCtl_sysPllPu(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_sysPllCtl_sysPllRfDiv(r32)              _BFGET_(r32, 5, 1)
    #define   SET32Gbl_sysPllCtl_sysPllRfDiv(r32,v)            _BFSET_(r32, 5, 1,v)
    #define   GET16Gbl_sysPllCtl_sysPllRfDiv(r16)              _BFGET_(r16, 5, 1)
    #define   SET16Gbl_sysPllCtl_sysPllRfDiv(r16,v)            _BFSET_(r16, 5, 1,v)

    #define   GET32Gbl_sysPllCtl_sysPllFbDiv(r32)              _BFGET_(r32,14, 6)
    #define   SET32Gbl_sysPllCtl_sysPllFbDiv(r32,v)            _BFSET_(r32,14, 6,v)
    #define   GET16Gbl_sysPllCtl_sysPllFbDiv(r16)              _BFGET_(r16,14, 6)
    #define   SET16Gbl_sysPllCtl_sysPllFbDiv(r16,v)            _BFSET_(r16,14, 6,v)

    #define   GET32Gbl_sysPllCtl_sysPllVddm(r32)               _BFGET_(r32,16,15)
    #define   SET32Gbl_sysPllCtl_sysPllVddm(r32,v)             _BFSET_(r32,16,15,v)

    #define   GET32Gbl_sysPllCtl_sysPllVddl(r32)               _BFGET_(r32,18,17)
    #define   SET32Gbl_sysPllCtl_sysPllVddl(r32,v)             _BFSET_(r32,18,17,v)
    #define   GET16Gbl_sysPllCtl_sysPllVddl(r16)               _BFGET_(r16, 2, 1)
    #define   SET16Gbl_sysPllCtl_sysPllVddl(r16,v)             _BFSET_(r16, 2, 1,v)

    #define   GET32Gbl_sysPllCtl_sysPllVR1P8V(r32)             _BFGET_(r32,20,19)
    #define   SET32Gbl_sysPllCtl_sysPllVR1P8V(r32,v)           _BFSET_(r32,20,19,v)
    #define   GET16Gbl_sysPllCtl_sysPllVR1P8V(r16)             _BFGET_(r16, 4, 3)
    #define   SET16Gbl_sysPllCtl_sysPllVR1P8V(r16,v)           _BFSET_(r16, 4, 3,v)

    #define   GET32Gbl_sysPllCtl_sysPllVcoFbst(r32)            _BFGET_(r32,21,21)
    #define   SET32Gbl_sysPllCtl_sysPllVcoFbst(r32,v)          _BFSET_(r32,21,21,v)
    #define   GET16Gbl_sysPllCtl_sysPllVcoFbst(r16)            _BFGET_(r16, 5, 5)
    #define   SET16Gbl_sysPllCtl_sysPllVcoFbst(r16,v)          _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_sysPllCtl_sysPllVcoRng(r32)             _BFGET_(r32,23,22)
    #define   SET32Gbl_sysPllCtl_sysPllVcoRng(r32,v)           _BFSET_(r32,23,22,v)
    #define   GET16Gbl_sysPllCtl_sysPllVcoRng(r16)             _BFGET_(r16, 7, 6)
    #define   SET16Gbl_sysPllCtl_sysPllVcoRng(r16,v)           _BFSET_(r16, 7, 6,v)

    #define   GET32Gbl_sysPllCtl_sysPllIcp(r32)                _BFGET_(r32,26,24)
    #define   SET32Gbl_sysPllCtl_sysPllIcp(r32,v)              _BFSET_(r32,26,24,v)
    #define   GET16Gbl_sysPllCtl_sysPllIcp(r16)                _BFGET_(r16,10, 8)
    #define   SET16Gbl_sysPllCtl_sysPllIcp(r16,v)              _BFSET_(r16,10, 8,v)

    #define   GET32Gbl_sysPllCtl_sysPllKvco(r32)               _BFGET_(r32,30,27)
    #define   SET32Gbl_sysPllCtl_sysPllKvco(r32,v)             _BFSET_(r32,30,27,v)
    #define   GET16Gbl_sysPllCtl_sysPllKvco(r16)               _BFGET_(r16,14,11)
    #define   SET16Gbl_sysPllCtl_sysPllKvco(r16,v)             _BFSET_(r16,14,11,v)

    #define     w32Gbl_sysPllCtl                               {\
            UNSG32 usysPllCtl_sysPllPu                         :  1;\
            UNSG32 usysPllCtl_sysPllRfDiv                      :  5;\
            UNSG32 usysPllCtl_sysPllFbDiv                      :  9;\
            UNSG32 usysPllCtl_sysPllVddm                       :  2;\
            UNSG32 usysPllCtl_sysPllVddl                       :  2;\
            UNSG32 usysPllCtl_sysPllVR1P8V                     :  2;\
            UNSG32 usysPllCtl_sysPllVcoFbst                    :  1;\
            UNSG32 usysPllCtl_sysPllVcoRng                     :  2;\
            UNSG32 usysPllCtl_sysPllIcp                        :  3;\
            UNSG32 usysPllCtl_sysPllKvco                       :  4;\
            UNSG32 RSVDx14_b31                                 :  1;\
          }
    union { UNSG32 u32Gbl_sysPllCtl;
            struct w32Gbl_sysPllCtl;
          };
    #define   GET32Gbl_sysPllCtl_sysPllVcoDivSel(r32)          _BFGET_(r32, 1, 0)
    #define   SET32Gbl_sysPllCtl_sysPllVcoDivSel(r32,v)        _BFSET_(r32, 1, 0,v)
    #define   GET16Gbl_sysPllCtl_sysPllVcoDivSel(r16)          _BFGET_(r16, 1, 0)
    #define   SET16Gbl_sysPllCtl_sysPllVcoDivSel(r16,v)        _BFSET_(r16, 1, 0,v)

    #define   GET32Gbl_sysPllCtl_sysPllIntpi(r32)              _BFGET_(r32, 3, 2)
    #define   SET32Gbl_sysPllCtl_sysPllIntpi(r32,v)            _BFSET_(r32, 3, 2,v)
    #define   GET16Gbl_sysPllCtl_sysPllIntpi(r16)              _BFGET_(r16, 3, 2)
    #define   SET16Gbl_sysPllCtl_sysPllIntpi(r16,v)            _BFSET_(r16, 3, 2,v)

    #define   GET32Gbl_sysPllCtl_sysPllCapSel(r32)             _BFGET_(r32, 6, 4)
    #define   SET32Gbl_sysPllCtl_sysPllCapSel(r32,v)           _BFSET_(r32, 6, 4,v)
    #define   GET16Gbl_sysPllCtl_sysPllCapSel(r16)             _BFGET_(r16, 6, 4)
    #define   SET16Gbl_sysPllCtl_sysPllCapSel(r16,v)           _BFSET_(r16, 6, 4,v)

    #define   GET32Gbl_sysPllCtl_sysPllSscEn(r32)              _BFGET_(r32, 7, 7)
    #define   SET32Gbl_sysPllCtl_sysPllSscEn(r32,v)            _BFSET_(r32, 7, 7,v)
    #define   GET16Gbl_sysPllCtl_sysPllSscEn(r16)              _BFGET_(r16, 7, 7)
    #define   SET16Gbl_sysPllCtl_sysPllSscEn(r16,v)            _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_sysPllCtl_sysPllPuSsc(r32)              _BFGET_(r32, 8, 8)
    #define   SET32Gbl_sysPllCtl_sysPllPuSsc(r32,v)            _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_sysPllCtl_sysPllPuSsc(r16)              _BFGET_(r16, 8, 8)
    #define   SET16Gbl_sysPllCtl_sysPllPuSsc(r16,v)            _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_sysPllCtl_sysPllSscFdiv(r32)            _BFGET_(r32,23, 9)
    #define   SET32Gbl_sysPllCtl_sysPllSscFdiv(r32,v)          _BFSET_(r32,23, 9,v)

    #define     w32Gbl_sysPllCtl1                              {\
            UNSG32 usysPllCtl_sysPllVcoDivSel                  :  2;\
            UNSG32 usysPllCtl_sysPllIntpi                      :  2;\
            UNSG32 usysPllCtl_sysPllCapSel                     :  3;\
            UNSG32 usysPllCtl_sysPllSscEn                      :  1;\
            UNSG32 usysPllCtl_sysPllPuSsc                      :  1;\
            UNSG32 usysPllCtl_sysPllSscFdiv                    : 15;\
            UNSG32 RSVDx18_b24                                 :  8;\
          }
    union { UNSG32 u32Gbl_sysPllCtl1;
            struct w32Gbl_sysPllCtl1;
          };
    #define   GET32Gbl_sysPllCtl_sysPllSscRnge(r32)            _BFGET_(r32,13, 0)
    #define   SET32Gbl_sysPllCtl_sysPllSscRnge(r32,v)          _BFSET_(r32,13, 0,v)
    #define   GET16Gbl_sysPllCtl_sysPllSscRnge(r16)            _BFGET_(r16,13, 0)
    #define   SET16Gbl_sysPllCtl_sysPllSscRnge(r16,v)          _BFSET_(r16,13, 0,v)

    #define   GET32Gbl_sysPllCtl_sysPllSscGain2x(r32)          _BFGET_(r32,14,14)
    #define   SET32Gbl_sysPllCtl_sysPllSscGain2x(r32,v)        _BFSET_(r32,14,14,v)
    #define   GET16Gbl_sysPllCtl_sysPllSscGain2x(r16)          _BFGET_(r16,14,14)
    #define   SET16Gbl_sysPllCtl_sysPllSscGain2x(r16,v)        _BFSET_(r16,14,14,v)

    #define   GET32Gbl_sysPllCtl_sysPllSscmode(r32)            _BFGET_(r32,15,15)
    #define   SET32Gbl_sysPllCtl_sysPllSscmode(r32,v)          _BFSET_(r32,15,15,v)
    #define   GET16Gbl_sysPllCtl_sysPllSscmode(r16)            _BFGET_(r16,15,15)
    #define   SET16Gbl_sysPllCtl_sysPllSscmode(r16,v)          _BFSET_(r16,15,15,v)

    #define   GET32Gbl_sysPllCtl_sysPllSscResetExt(r32)        _BFGET_(r32,16,16)
    #define   SET32Gbl_sysPllCtl_sysPllSscResetExt(r32,v)      _BFSET_(r32,16,16,v)
    #define   GET16Gbl_sysPllCtl_sysPllSscResetExt(r16)        _BFGET_(r16, 0, 0)
    #define   SET16Gbl_sysPllCtl_sysPllSscResetExt(r16,v)      _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_sysPllCtl_sysPllTestMon(r32)            _BFGET_(r32,20,17)
    #define   SET32Gbl_sysPllCtl_sysPllTestMon(r32,v)          _BFSET_(r32,20,17,v)
    #define   GET16Gbl_sysPllCtl_sysPllTestMon(r16)            _BFGET_(r16, 4, 1)
    #define   SET16Gbl_sysPllCtl_sysPllTestMon(r16,v)          _BFSET_(r16, 4, 1,v)

    #define   GET32Gbl_sysPllCtl_sysPllRsvd(r32)               _BFGET_(r32,22,21)
    #define   SET32Gbl_sysPllCtl_sysPllRsvd(r32,v)             _BFSET_(r32,22,21,v)
    #define   GET16Gbl_sysPllCtl_sysPllRsvd(r16)               _BFGET_(r16, 6, 5)
    #define   SET16Gbl_sysPllCtl_sysPllRsvd(r16,v)             _BFSET_(r16, 6, 5,v)

    #define     w32Gbl_sysPllCtl2                              {\
            UNSG32 usysPllCtl_sysPllSscRnge                    : 14;\
            UNSG32 usysPllCtl_sysPllSscGain2x                  :  1;\
            UNSG32 usysPllCtl_sysPllSscmode                    :  1;\
            UNSG32 usysPllCtl_sysPllSscResetExt                :  1;\
            UNSG32 usysPllCtl_sysPllTestMon                    :  4;\
            UNSG32 usysPllCtl_sysPllRsvd                       :  2;\
            UNSG32 RSVDx1C_b23                                 :  9;\
          }
    union { UNSG32 u32Gbl_sysPllCtl2;
            struct w32Gbl_sysPllCtl2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_memPllCtl_memPllPu(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_memPllCtl_memPllPu(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_memPllCtl_memPllPu(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_memPllCtl_memPllPu(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_memPllCtl_memPllRfDiv(r32)              _BFGET_(r32, 5, 1)
    #define   SET32Gbl_memPllCtl_memPllRfDiv(r32,v)            _BFSET_(r32, 5, 1,v)
    #define   GET16Gbl_memPllCtl_memPllRfDiv(r16)              _BFGET_(r16, 5, 1)
    #define   SET16Gbl_memPllCtl_memPllRfDiv(r16,v)            _BFSET_(r16, 5, 1,v)

    #define   GET32Gbl_memPllCtl_memPllFbDiv(r32)              _BFGET_(r32,14, 6)
    #define   SET32Gbl_memPllCtl_memPllFbDiv(r32,v)            _BFSET_(r32,14, 6,v)
    #define   GET16Gbl_memPllCtl_memPllFbDiv(r16)              _BFGET_(r16,14, 6)
    #define   SET16Gbl_memPllCtl_memPllFbDiv(r16,v)            _BFSET_(r16,14, 6,v)

    #define   GET32Gbl_memPllCtl_memPllVddm(r32)               _BFGET_(r32,16,15)
    #define   SET32Gbl_memPllCtl_memPllVddm(r32,v)             _BFSET_(r32,16,15,v)

    #define   GET32Gbl_memPllCtl_memPllVddl(r32)               _BFGET_(r32,18,17)
    #define   SET32Gbl_memPllCtl_memPllVddl(r32,v)             _BFSET_(r32,18,17,v)
    #define   GET16Gbl_memPllCtl_memPllVddl(r16)               _BFGET_(r16, 2, 1)
    #define   SET16Gbl_memPllCtl_memPllVddl(r16,v)             _BFSET_(r16, 2, 1,v)

    #define   GET32Gbl_memPllCtl_memPllVR1P8V(r32)             _BFGET_(r32,20,19)
    #define   SET32Gbl_memPllCtl_memPllVR1P8V(r32,v)           _BFSET_(r32,20,19,v)
    #define   GET16Gbl_memPllCtl_memPllVR1P8V(r16)             _BFGET_(r16, 4, 3)
    #define   SET16Gbl_memPllCtl_memPllVR1P8V(r16,v)           _BFSET_(r16, 4, 3,v)

    #define   GET32Gbl_memPllCtl_memPllVcoFbst(r32)            _BFGET_(r32,21,21)
    #define   SET32Gbl_memPllCtl_memPllVcoFbst(r32,v)          _BFSET_(r32,21,21,v)
    #define   GET16Gbl_memPllCtl_memPllVcoFbst(r16)            _BFGET_(r16, 5, 5)
    #define   SET16Gbl_memPllCtl_memPllVcoFbst(r16,v)          _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_memPllCtl_memPllVcoRng(r32)             _BFGET_(r32,23,22)
    #define   SET32Gbl_memPllCtl_memPllVcoRng(r32,v)           _BFSET_(r32,23,22,v)
    #define   GET16Gbl_memPllCtl_memPllVcoRng(r16)             _BFGET_(r16, 7, 6)
    #define   SET16Gbl_memPllCtl_memPllVcoRng(r16,v)           _BFSET_(r16, 7, 6,v)

    #define   GET32Gbl_memPllCtl_memPllIcp(r32)                _BFGET_(r32,26,24)
    #define   SET32Gbl_memPllCtl_memPllIcp(r32,v)              _BFSET_(r32,26,24,v)
    #define   GET16Gbl_memPllCtl_memPllIcp(r16)                _BFGET_(r16,10, 8)
    #define   SET16Gbl_memPllCtl_memPllIcp(r16,v)              _BFSET_(r16,10, 8,v)

    #define   GET32Gbl_memPllCtl_memPllKvco(r32)               _BFGET_(r32,30,27)
    #define   SET32Gbl_memPllCtl_memPllKvco(r32,v)             _BFSET_(r32,30,27,v)
    #define   GET16Gbl_memPllCtl_memPllKvco(r16)               _BFGET_(r16,14,11)
    #define   SET16Gbl_memPllCtl_memPllKvco(r16,v)             _BFSET_(r16,14,11,v)

    #define     w32Gbl_memPllCtl                               {\
            UNSG32 umemPllCtl_memPllPu                         :  1;\
            UNSG32 umemPllCtl_memPllRfDiv                      :  5;\
            UNSG32 umemPllCtl_memPllFbDiv                      :  9;\
            UNSG32 umemPllCtl_memPllVddm                       :  2;\
            UNSG32 umemPllCtl_memPllVddl                       :  2;\
            UNSG32 umemPllCtl_memPllVR1P8V                     :  2;\
            UNSG32 umemPllCtl_memPllVcoFbst                    :  1;\
            UNSG32 umemPllCtl_memPllVcoRng                     :  2;\
            UNSG32 umemPllCtl_memPllIcp                        :  3;\
            UNSG32 umemPllCtl_memPllKvco                       :  4;\
            UNSG32 RSVDx20_b31                                 :  1;\
          }
    union { UNSG32 u32Gbl_memPllCtl;
            struct w32Gbl_memPllCtl;
          };
    #define   GET32Gbl_memPllCtl_memPllVcoDivSel(r32)          _BFGET_(r32, 1, 0)
    #define   SET32Gbl_memPllCtl_memPllVcoDivSel(r32,v)        _BFSET_(r32, 1, 0,v)
    #define   GET16Gbl_memPllCtl_memPllVcoDivSel(r16)          _BFGET_(r16, 1, 0)
    #define   SET16Gbl_memPllCtl_memPllVcoDivSel(r16,v)        _BFSET_(r16, 1, 0,v)

    #define   GET32Gbl_memPllCtl_memPllIntpi(r32)              _BFGET_(r32, 3, 2)
    #define   SET32Gbl_memPllCtl_memPllIntpi(r32,v)            _BFSET_(r32, 3, 2,v)
    #define   GET16Gbl_memPllCtl_memPllIntpi(r16)              _BFGET_(r16, 3, 2)
    #define   SET16Gbl_memPllCtl_memPllIntpi(r16,v)            _BFSET_(r16, 3, 2,v)

    #define   GET32Gbl_memPllCtl_memPllCapSel(r32)             _BFGET_(r32, 6, 4)
    #define   SET32Gbl_memPllCtl_memPllCapSel(r32,v)           _BFSET_(r32, 6, 4,v)
    #define   GET16Gbl_memPllCtl_memPllCapSel(r16)             _BFGET_(r16, 6, 4)
    #define   SET16Gbl_memPllCtl_memPllCapSel(r16,v)           _BFSET_(r16, 6, 4,v)

    #define   GET32Gbl_memPllCtl_memPllSscEn(r32)              _BFGET_(r32, 7, 7)
    #define   SET32Gbl_memPllCtl_memPllSscEn(r32,v)            _BFSET_(r32, 7, 7,v)
    #define   GET16Gbl_memPllCtl_memPllSscEn(r16)              _BFGET_(r16, 7, 7)
    #define   SET16Gbl_memPllCtl_memPllSscEn(r16,v)            _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_memPllCtl_memPllPuSsc(r32)              _BFGET_(r32, 8, 8)
    #define   SET32Gbl_memPllCtl_memPllPuSsc(r32,v)            _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_memPllCtl_memPllPuSsc(r16)              _BFGET_(r16, 8, 8)
    #define   SET16Gbl_memPllCtl_memPllPuSsc(r16,v)            _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_memPllCtl_memPllSscFdiv(r32)            _BFGET_(r32,23, 9)
    #define   SET32Gbl_memPllCtl_memPllSscFdiv(r32,v)          _BFSET_(r32,23, 9,v)

    #define     w32Gbl_memPllCtl1                              {\
            UNSG32 umemPllCtl_memPllVcoDivSel                  :  2;\
            UNSG32 umemPllCtl_memPllIntpi                      :  2;\
            UNSG32 umemPllCtl_memPllCapSel                     :  3;\
            UNSG32 umemPllCtl_memPllSscEn                      :  1;\
            UNSG32 umemPllCtl_memPllPuSsc                      :  1;\
            UNSG32 umemPllCtl_memPllSscFdiv                    : 15;\
            UNSG32 RSVDx24_b24                                 :  8;\
          }
    union { UNSG32 u32Gbl_memPllCtl1;
            struct w32Gbl_memPllCtl1;
          };
    #define   GET32Gbl_memPllCtl_memPllSscRnge(r32)            _BFGET_(r32,13, 0)
    #define   SET32Gbl_memPllCtl_memPllSscRnge(r32,v)          _BFSET_(r32,13, 0,v)
    #define   GET16Gbl_memPllCtl_memPllSscRnge(r16)            _BFGET_(r16,13, 0)
    #define   SET16Gbl_memPllCtl_memPllSscRnge(r16,v)          _BFSET_(r16,13, 0,v)

    #define   GET32Gbl_memPllCtl_memPllSscGain2x(r32)          _BFGET_(r32,14,14)
    #define   SET32Gbl_memPllCtl_memPllSscGain2x(r32,v)        _BFSET_(r32,14,14,v)
    #define   GET16Gbl_memPllCtl_memPllSscGain2x(r16)          _BFGET_(r16,14,14)
    #define   SET16Gbl_memPllCtl_memPllSscGain2x(r16,v)        _BFSET_(r16,14,14,v)

    #define   GET32Gbl_memPllCtl_memPllSscmode(r32)            _BFGET_(r32,15,15)
    #define   SET32Gbl_memPllCtl_memPllSscmode(r32,v)          _BFSET_(r32,15,15,v)
    #define   GET16Gbl_memPllCtl_memPllSscmode(r16)            _BFGET_(r16,15,15)
    #define   SET16Gbl_memPllCtl_memPllSscmode(r16,v)          _BFSET_(r16,15,15,v)

    #define   GET32Gbl_memPllCtl_memPllSscResetExt(r32)        _BFGET_(r32,16,16)
    #define   SET32Gbl_memPllCtl_memPllSscResetExt(r32,v)      _BFSET_(r32,16,16,v)
    #define   GET16Gbl_memPllCtl_memPllSscResetExt(r16)        _BFGET_(r16, 0, 0)
    #define   SET16Gbl_memPllCtl_memPllSscResetExt(r16,v)      _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_memPllCtl_memPllTestMon(r32)            _BFGET_(r32,20,17)
    #define   SET32Gbl_memPllCtl_memPllTestMon(r32,v)          _BFSET_(r32,20,17,v)
    #define   GET16Gbl_memPllCtl_memPllTestMon(r16)            _BFGET_(r16, 4, 1)
    #define   SET16Gbl_memPllCtl_memPllTestMon(r16,v)          _BFSET_(r16, 4, 1,v)

    #define   GET32Gbl_memPllCtl_memPllRsvd(r32)               _BFGET_(r32,22,21)
    #define   SET32Gbl_memPllCtl_memPllRsvd(r32,v)             _BFSET_(r32,22,21,v)
    #define   GET16Gbl_memPllCtl_memPllRsvd(r16)               _BFGET_(r16, 6, 5)
    #define   SET16Gbl_memPllCtl_memPllRsvd(r16,v)             _BFSET_(r16, 6, 5,v)

    #define     w32Gbl_memPllCtl2                              {\
            UNSG32 umemPllCtl_memPllSscRnge                    : 14;\
            UNSG32 umemPllCtl_memPllSscGain2x                  :  1;\
            UNSG32 umemPllCtl_memPllSscmode                    :  1;\
            UNSG32 umemPllCtl_memPllSscResetExt                :  1;\
            UNSG32 umemPllCtl_memPllTestMon                    :  4;\
            UNSG32 umemPllCtl_memPllRsvd                       :  2;\
            UNSG32 RSVDx28_b23                                 :  9;\
          }
    union { UNSG32 u32Gbl_memPllCtl2;
            struct w32Gbl_memPllCtl2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_cpu1PllCtl_cpu1PllPu(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllPu(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllPu(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllPu(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllRfDiv(r32)            _BFGET_(r32, 5, 1)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllRfDiv(r32,v)          _BFSET_(r32, 5, 1,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllRfDiv(r16)            _BFGET_(r16, 5, 1)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllRfDiv(r16,v)          _BFSET_(r16, 5, 1,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllFbDiv(r32)            _BFGET_(r32,14, 6)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllFbDiv(r32,v)          _BFSET_(r32,14, 6,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllFbDiv(r16)            _BFGET_(r16,14, 6)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllFbDiv(r16,v)          _BFSET_(r16,14, 6,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllVddm(r32)             _BFGET_(r32,16,15)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllVddm(r32,v)           _BFSET_(r32,16,15,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllVddl(r32)             _BFGET_(r32,18,17)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllVddl(r32,v)           _BFSET_(r32,18,17,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllVddl(r16)             _BFGET_(r16, 2, 1)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllVddl(r16,v)           _BFSET_(r16, 2, 1,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllVcoFbst(r32)          _BFGET_(r32,19,19)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllVcoFbst(r32,v)        _BFSET_(r32,19,19,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllVcoFbst(r16)          _BFGET_(r16, 3, 3)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllVcoFbst(r16,v)        _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllIcp(r32)              _BFGET_(r32,22,20)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllIcp(r32,v)            _BFSET_(r32,22,20,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllIcp(r16)              _BFGET_(r16, 6, 4)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllIcp(r16,v)            _BFSET_(r16, 6, 4,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllKvco(r32)             _BFGET_(r32,26,23)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllKvco(r32,v)           _BFSET_(r32,26,23,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllKvco(r16)             _BFGET_(r16,10, 7)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllKvco(r16,v)           _BFSET_(r16,10, 7,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllVcoRng(r32)           _BFGET_(r32,29,27)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllVcoRng(r32,v)         _BFSET_(r32,29,27,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllVcoRng(r16)           _BFGET_(r16,13,11)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllVcoRng(r16,v)         _BFSET_(r16,13,11,v)

    #define     w32Gbl_cpu1PllCtl                              {\
            UNSG32 ucpu1PllCtl_cpu1PllPu                       :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllRfDiv                    :  5;\
            UNSG32 ucpu1PllCtl_cpu1PllFbDiv                    :  9;\
            UNSG32 ucpu1PllCtl_cpu1PllVddm                     :  2;\
            UNSG32 ucpu1PllCtl_cpu1PllVddl                     :  2;\
            UNSG32 ucpu1PllCtl_cpu1PllVcoFbst                  :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllIcp                      :  3;\
            UNSG32 ucpu1PllCtl_cpu1PllKvco                     :  4;\
            UNSG32 ucpu1PllCtl_cpu1PllVcoRng                   :  3;\
            UNSG32 RSVDx2C_b30                                 :  2;\
          }
    union { UNSG32 u32Gbl_cpu1PllCtl;
            struct w32Gbl_cpu1PllCtl;
          };
    #define   GET32Gbl_cpu1PllCtl_cpu1PllVcoDivSelDiff(r32)    _BFGET_(r32, 3, 0)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllVcoDivSelDiff(r32,v)  _BFSET_(r32, 3, 0,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllVcoDivSelDiff(r16)    _BFGET_(r16, 3, 0)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllVcoDivSelDiff(r16,v)  _BFSET_(r16, 3, 0,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllVcoDivSelSe(r32)      _BFGET_(r32, 7, 4)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllVcoDivSelSe(r32,v)    _BFSET_(r32, 7, 4,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllVcoDivSelSe(r16)      _BFGET_(r16, 7, 4)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllVcoDivSelSe(r16,v)    _BFSET_(r16, 7, 4,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllDiffClkEn(r32)        _BFGET_(r32, 8, 8)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllDiffClkEn(r32,v)      _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllDiffClkEn(r16)        _BFGET_(r16, 8, 8)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllDiffClkEn(r16,v)      _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllSelVcoDiff(r32)       _BFGET_(r32, 9, 9)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllSelVcoDiff(r32,v)     _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllSelVcoDiff(r16)       _BFGET_(r16, 9, 9)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllSelVcoDiff(r16,v)     _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllSelVcoClkSe(r32)      _BFGET_(r32,10,10)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllSelVcoClkSe(r32,v)    _BFSET_(r32,10,10,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllSelVcoClkSe(r16)      _BFGET_(r16,10,10)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllSelVcoClkSe(r16,v)    _BFSET_(r16,10,10,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllBypassFbDiv(r32)      _BFGET_(r32,11,11)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllBypassFbDiv(r32,v)    _BFSET_(r32,11,11,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllBypassFbDiv(r16)      _BFGET_(r16,11,11)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllBypassFbDiv(r16,v)    _BFSET_(r16,11,11,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllGateClkCntrl(r32)     _BFGET_(r32,12,12)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllGateClkCntrl(r32,v)   _BFSET_(r32,12,12,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllGateClkCntrl(r16)     _BFGET_(r16,12,12)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllGateClkCntrl(r16,v)   _BFSET_(r16,12,12,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllIntpi(r32)            _BFGET_(r32,15,13)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllIntpi(r32,v)          _BFSET_(r32,15,13,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllIntpi(r16)            _BFGET_(r16,15,13)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllIntpi(r16,v)          _BFSET_(r16,15,13,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllFreqOffsetEn(r32)     _BFGET_(r32,16,16)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllFreqOffsetEn(r32,v)   _BFSET_(r32,16,16,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllFreqOffsetEn(r16)     _BFGET_(r16, 0, 0)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllFreqOffsetEn(r16,v)   _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_cpu1PllCtl1                             {\
            UNSG32 ucpu1PllCtl_cpu1PllVcoDivSelDiff            :  4;\
            UNSG32 ucpu1PllCtl_cpu1PllVcoDivSelSe              :  4;\
            UNSG32 ucpu1PllCtl_cpu1PllDiffClkEn                :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllSelVcoDiff               :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllSelVcoClkSe              :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllBypassFbDiv              :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllGateClkCntrl             :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllIntpi                    :  3;\
            UNSG32 ucpu1PllCtl_cpu1PllFreqOffsetEn             :  1;\
            UNSG32 RSVDx30_b17                                 : 15;\
          }
    union { UNSG32 u32Gbl_cpu1PllCtl1;
            struct w32Gbl_cpu1PllCtl1;
          };
    #define   GET32Gbl_cpu1PllCtl_cpu1PllFreqOffset(r32)       _BFGET_(r32,16, 0)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllFreqOffset(r32,v)     _BFSET_(r32,16, 0,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllFreqOffsetValid(r32)  _BFGET_(r32,17,17)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllFreqOffsetValid(r32,v) _BFSET_(r32,17,17,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllFreqOffsetValid(r16)  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllFreqOffsetValid(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllClkDetEn(r32)         _BFGET_(r32,18,18)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllClkDetEn(r32,v)       _BFSET_(r32,18,18,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllClkDetEn(r16)         _BFGET_(r16, 2, 2)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllClkDetEn(r16,v)       _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllExtRst(r32)           _BFGET_(r32,19,19)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllExtRst(r32,v)         _BFSET_(r32,19,19,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllExtRst(r16)           _BFGET_(r16, 3, 3)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllExtRst(r16,v)         _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllFreqOffsetModeSel(r32) _BFGET_(r32,20,20)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllFreqOffsetModeSel(r32,v) _BFSET_(r32,20,20,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllFreqOffsetModeSel(r16) _BFGET_(r16, 4, 4)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllFreqOffsetModeSel(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllSscEn(r32)            _BFGET_(r32,21,21)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllSscEn(r32,v)          _BFSET_(r32,21,21,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllSscEn(r16)            _BFGET_(r16, 5, 5)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllSscEn(r16,v)          _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllSscMode(r32)          _BFGET_(r32,22,22)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllSscMode(r32,v)        _BFSET_(r32,22,22,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllSscMode(r16)          _BFGET_(r16, 6, 6)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllSscMode(r16,v)        _BFSET_(r16, 6, 6,v)

    #define     w32Gbl_cpu1PllCtl2                             {\
            UNSG32 ucpu1PllCtl_cpu1PllFreqOffset               : 17;\
            UNSG32 ucpu1PllCtl_cpu1PllFreqOffsetValid          :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllClkDetEn                 :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllExtRst                   :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllFreqOffsetModeSel        :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllSscEn                    :  1;\
            UNSG32 ucpu1PllCtl_cpu1PllSscMode                  :  1;\
            UNSG32 RSVDx34_b23                                 :  9;\
          }
    union { UNSG32 u32Gbl_cpu1PllCtl2;
            struct w32Gbl_cpu1PllCtl2;
          };
    #define   GET32Gbl_cpu1PllCtl_cpu1PllSscFreqDiv(r32)       _BFGET_(r32,14, 0)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllSscFreqDiv(r32,v)     _BFSET_(r32,14, 0,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllSscFreqDiv(r16)       _BFGET_(r16,14, 0)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllSscFreqDiv(r16,v)     _BFSET_(r16,14, 0,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllSscRnge(r32)          _BFGET_(r32,25,15)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllSscRnge(r32,v)        _BFSET_(r32,25,15,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllSscExtRst(r32)        _BFGET_(r32,26,26)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllSscExtRst(r32,v)      _BFSET_(r32,26,26,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllSscExtRst(r16)        _BFGET_(r16,10,10)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllSscExtRst(r16,v)      _BFSET_(r16,10,10,v)

    #define     w32Gbl_cpu1PllCtl3                             {\
            UNSG32 ucpu1PllCtl_cpu1PllSscFreqDiv               : 15;\
            UNSG32 ucpu1PllCtl_cpu1PllSscRnge                  : 11;\
            UNSG32 ucpu1PllCtl_cpu1PllSscExtRst                :  1;\
            UNSG32 RSVDx38_b27                                 :  5;\
          }
    union { UNSG32 u32Gbl_cpu1PllCtl3;
            struct w32Gbl_cpu1PllCtl3;
          };
    #define   GET32Gbl_cpu1PllCtl_cpu1PllTestMon(r32)          _BFGET_(r32, 5, 0)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllTestMon(r32,v)        _BFSET_(r32, 5, 0,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllTestMon(r16)          _BFGET_(r16, 5, 0)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllTestMon(r16,v)        _BFSET_(r16, 5, 0,v)

    #define   GET32Gbl_cpu1PllCtl_cpu1PllReserve(r32)          _BFGET_(r32, 7, 6)
    #define   SET32Gbl_cpu1PllCtl_cpu1PllReserve(r32,v)        _BFSET_(r32, 7, 6,v)
    #define   GET16Gbl_cpu1PllCtl_cpu1PllReserve(r16)          _BFGET_(r16, 7, 6)
    #define   SET16Gbl_cpu1PllCtl_cpu1PllReserve(r16,v)        _BFSET_(r16, 7, 6,v)

    #define     w32Gbl_cpu1PllCtl4                             {\
            UNSG32 ucpu1PllCtl_cpu1PllTestMon                  :  6;\
            UNSG32 ucpu1PllCtl_cpu1PllReserve                  :  2;\
            UNSG32 RSVDx3C_b8                                  : 24;\
          }
    union { UNSG32 u32Gbl_cpu1PllCtl4;
            struct w32Gbl_cpu1PllCtl4;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_avPllCtl_avPllReset_Pll_A(r32)          _BFGET_(r32, 0, 0)
    #define   SET32Gbl_avPllCtl_avPllReset_Pll_A(r32,v)        _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_avPllCtl_avPllReset_Pll_A(r16)          _BFGET_(r16, 0, 0)
    #define   SET16Gbl_avPllCtl_avPllReset_Pll_A(r16,v)        _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_avPllCtl_avPllReset_Pll_B(r32)          _BFGET_(r32, 1, 1)
    #define   SET32Gbl_avPllCtl_avPllReset_Pll_B(r32,v)        _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_avPllCtl_avPllReset_Pll_B(r16)          _BFGET_(r16, 1, 1)
    #define   SET16Gbl_avPllCtl_avPllReset_Pll_B(r16,v)        _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_avPllCtl_avPllPu(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_avPllCtl_avPllPu(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_avPllCtl_avPllPu(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_avPllCtl_avPllPu(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_avPllCtl_avPllPu_Pll_B(r32)             _BFGET_(r32, 3, 3)
    #define   SET32Gbl_avPllCtl_avPllPu_Pll_B(r32,v)           _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_avPllCtl_avPllPu_Pll_B(r16)             _BFGET_(r16, 3, 3)
    #define   SET16Gbl_avPllCtl_avPllPu_Pll_B(r16,v)           _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_avPllCtl_avPllPll_RegPump(r32)          _BFGET_(r32, 5, 4)
    #define   SET32Gbl_avPllCtl_avPllPll_RegPump(r32,v)        _BFSET_(r32, 5, 4,v)
    #define   GET16Gbl_avPllCtl_avPllPll_RegPump(r16)          _BFGET_(r16, 5, 4)
    #define   SET16Gbl_avPllCtl_avPllPll_RegPump(r16,v)        _BFSET_(r16, 5, 4,v)

    #define   GET32Gbl_avPllCtl_avPllVDDR20_VCO(r32)           _BFGET_(r32, 7, 6)
    #define   SET32Gbl_avPllCtl_avPllVDDR20_VCO(r32,v)         _BFSET_(r32, 7, 6,v)
    #define   GET16Gbl_avPllCtl_avPllVDDR20_VCO(r16)           _BFGET_(r16, 7, 6)
    #define   SET16Gbl_avPllCtl_avPllVDDR20_VCO(r16,v)         _BFSET_(r16, 7, 6,v)

    #define   GET32Gbl_avPllCtl_avPllVDDR17_VCO(r32)           _BFGET_(r32, 9, 8)
    #define   SET32Gbl_avPllCtl_avPllVDDR17_VCO(r32,v)         _BFSET_(r32, 9, 8,v)
    #define   GET16Gbl_avPllCtl_avPllVDDR17_VCO(r16)           _BFGET_(r16, 9, 8)
    #define   SET16Gbl_avPllCtl_avPllVDDR17_VCO(r16,v)         _BFSET_(r16, 9, 8,v)

    #define   GET32Gbl_avPllCtl_avPllVDDR10_VCO(r32)           _BFGET_(r32,11,10)
    #define   SET32Gbl_avPllCtl_avPllVDDR10_VCO(r32,v)         _BFSET_(r32,11,10,v)
    #define   GET16Gbl_avPllCtl_avPllVDDR10_VCO(r16)           _BFGET_(r16,11,10)
    #define   SET16Gbl_avPllCtl_avPllVDDR10_VCO(r16,v)         _BFSET_(r16,11,10,v)

    #define   GET32Gbl_avPllCtl_avPllVTHVOCal(r32)             _BFGET_(r32,13,12)
    #define   SET32Gbl_avPllCtl_avPllVTHVOCal(r32,v)           _BFSET_(r32,13,12,v)
    #define   GET16Gbl_avPllCtl_avPllVTHVOCal(r16)             _BFGET_(r16,13,12)
    #define   SET16Gbl_avPllCtl_avPllVTHVOCal(r16,v)           _BFSET_(r16,13,12,v)

    #define   GET32Gbl_avPllCtl_avPllSpeed_A(r32)              _BFGET_(r32,15,14)
    #define   SET32Gbl_avPllCtl_avPllSpeed_A(r32,v)            _BFSET_(r32,15,14,v)
    #define   GET16Gbl_avPllCtl_avPllSpeed_A(r16)              _BFGET_(r16,15,14)
    #define   SET16Gbl_avPllCtl_avPllSpeed_A(r16,v)            _BFSET_(r16,15,14,v)

    #define   GET32Gbl_avPllCtl_avPllSpeed_B(r32)              _BFGET_(r32,17,16)
    #define   SET32Gbl_avPllCtl_avPllSpeed_B(r32,v)            _BFSET_(r32,17,16,v)
    #define   GET16Gbl_avPllCtl_avPllSpeed_B(r16)              _BFGET_(r16, 1, 0)
    #define   SET16Gbl_avPllCtl_avPllSpeed_B(r16,v)            _BFSET_(r16, 1, 0,v)

    #define   GET32Gbl_avPllCtl_avPllClkDetEn(r32)             _BFGET_(r32,18,18)
    #define   SET32Gbl_avPllCtl_avPllClkDetEn(r32,v)           _BFSET_(r32,18,18,v)
    #define   GET16Gbl_avPllCtl_avPllClkDetEn(r16)             _BFGET_(r16, 2, 2)
    #define   SET16Gbl_avPllCtl_avPllClkDetEn(r16,v)           _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_avPllCtl_avPllIcp_A(r32)                _BFGET_(r32,22,19)
    #define   SET32Gbl_avPllCtl_avPllIcp_A(r32,v)              _BFSET_(r32,22,19,v)
    #define   GET16Gbl_avPllCtl_avPllIcp_A(r16)                _BFGET_(r16, 6, 3)
    #define   SET16Gbl_avPllCtl_avPllIcp_A(r16,v)              _BFSET_(r16, 6, 3,v)

    #define   GET32Gbl_avPllCtl_avPllIcp_B(r32)                _BFGET_(r32,26,23)
    #define   SET32Gbl_avPllCtl_avPllIcp_B(r32,v)              _BFSET_(r32,26,23,v)
    #define   GET16Gbl_avPllCtl_avPllIcp_B(r16)                _BFGET_(r16,10, 7)
    #define   SET16Gbl_avPllCtl_avPllIcp_B(r16,v)              _BFSET_(r16,10, 7,v)

    #define   GET32Gbl_avPllCtl_avPllRfDiv(r32)                _BFGET_(r32,28,27)
    #define   SET32Gbl_avPllCtl_avPllRfDiv(r32,v)              _BFSET_(r32,28,27,v)
    #define   GET16Gbl_avPllCtl_avPllRfDiv(r16)                _BFGET_(r16,12,11)
    #define   SET16Gbl_avPllCtl_avPllRfDiv(r16,v)              _BFSET_(r16,12,11,v)

    #define     w32Gbl_avPllCtl                                {\
            UNSG32 uavPllCtl_avPllReset_Pll_A                  :  1;\
            UNSG32 uavPllCtl_avPllReset_Pll_B                  :  1;\
            UNSG32 uavPllCtl_avPllPu                           :  1;\
            UNSG32 uavPllCtl_avPllPu_Pll_B                     :  1;\
            UNSG32 uavPllCtl_avPllPll_RegPump                  :  2;\
            UNSG32 uavPllCtl_avPllVDDR20_VCO                   :  2;\
            UNSG32 uavPllCtl_avPllVDDR17_VCO                   :  2;\
            UNSG32 uavPllCtl_avPllVDDR10_VCO                   :  2;\
            UNSG32 uavPllCtl_avPllVTHVOCal                     :  2;\
            UNSG32 uavPllCtl_avPllSpeed_A                      :  2;\
            UNSG32 uavPllCtl_avPllSpeed_B                      :  2;\
            UNSG32 uavPllCtl_avPllClkDetEn                     :  1;\
            UNSG32 uavPllCtl_avPllIcp_A                        :  4;\
            UNSG32 uavPllCtl_avPllIcp_B                        :  4;\
            UNSG32 uavPllCtl_avPllRfDiv                        :  2;\
            UNSG32 RSVDx40_b29                                 :  3;\
          }
    union { UNSG32 u32Gbl_avPllCtl;
            struct w32Gbl_avPllCtl;
          };
    #define   GET32Gbl_avPllCtl_avPllFbDiv_A(r32)              _BFGET_(r32, 7, 0)
    #define   SET32Gbl_avPllCtl_avPllFbDiv_A(r32,v)            _BFSET_(r32, 7, 0,v)
    #define   GET16Gbl_avPllCtl_avPllFbDiv_A(r16)              _BFGET_(r16, 7, 0)
    #define   SET16Gbl_avPllCtl_avPllFbDiv_A(r16,v)            _BFSET_(r16, 7, 0,v)

    #define   GET32Gbl_avPllCtl_avPllFbDiv_B(r32)              _BFGET_(r32,15, 8)
    #define   SET32Gbl_avPllCtl_avPllFbDiv_B(r32,v)            _BFSET_(r32,15, 8,v)
    #define   GET16Gbl_avPllCtl_avPllFbDiv_B(r16)              _BFGET_(r16,15, 8)
    #define   SET16Gbl_avPllCtl_avPllFbDiv_B(r16,v)            _BFSET_(r16,15, 8,v)

    #define   GET32Gbl_avPllCtl_avPllSelV2i_A(r32)             _BFGET_(r32,18,16)
    #define   SET32Gbl_avPllCtl_avPllSelV2i_A(r32,v)           _BFSET_(r32,18,16,v)
    #define   GET16Gbl_avPllCtl_avPllSelV2i_A(r16)             _BFGET_(r16, 2, 0)
    #define   SET16Gbl_avPllCtl_avPllSelV2i_A(r16,v)           _BFSET_(r16, 2, 0,v)

    #define   GET32Gbl_avPllCtl_avPllSelV2i_B(r32)             _BFGET_(r32,21,19)
    #define   SET32Gbl_avPllCtl_avPllSelV2i_B(r32,v)           _BFSET_(r32,21,19,v)
    #define   GET16Gbl_avPllCtl_avPllSelV2i_B(r16)             _BFGET_(r16, 5, 3)
    #define   SET16Gbl_avPllCtl_avPllSelV2i_B(r16,v)           _BFSET_(r16, 5, 3,v)

    #define   GET32Gbl_avPllCtl_avPllSel_LpFRes_A(r32)         _BFGET_(r32,22,22)
    #define   SET32Gbl_avPllCtl_avPllSel_LpFRes_A(r32,v)       _BFSET_(r32,22,22,v)
    #define   GET16Gbl_avPllCtl_avPllSel_LpFRes_A(r16)         _BFGET_(r16, 6, 6)
    #define   SET16Gbl_avPllCtl_avPllSel_LpFRes_A(r16,v)       _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_avPllCtl_avPllSel_LpFRes_B(r32)         _BFGET_(r32,23,23)
    #define   SET32Gbl_avPllCtl_avPllSel_LpFRes_B(r32,v)       _BFSET_(r32,23,23,v)
    #define   GET16Gbl_avPllCtl_avPllSel_LpFRes_B(r16)         _BFGET_(r16, 7, 7)
    #define   SET16Gbl_avPllCtl_avPllSel_LpFRes_B(r16,v)       _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_avPllCtl_avPllCalStart(r32)             _BFGET_(r32,24,24)
    #define   SET32Gbl_avPllCtl_avPllCalStart(r32,v)           _BFSET_(r32,24,24,v)
    #define   GET16Gbl_avPllCtl_avPllCalStart(r16)             _BFGET_(r16, 8, 8)
    #define   SET16Gbl_avPllCtl_avPllCalStart(r16,v)           _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_avPllCtl_avPllKVCO_Ext_En(r32)          _BFGET_(r32,25,25)
    #define   SET32Gbl_avPllCtl_avPllKVCO_Ext_En(r32,v)        _BFSET_(r32,25,25,v)
    #define   GET16Gbl_avPllCtl_avPllKVCO_Ext_En(r16)          _BFGET_(r16, 9, 9)
    #define   SET16Gbl_avPllCtl_avPllKVCO_Ext_En(r16,v)        _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_avPllCtl_avPllKVCO_A(r32)               _BFGET_(r32,28,26)
    #define   SET32Gbl_avPllCtl_avPllKVCO_A(r32,v)             _BFSET_(r32,28,26,v)
    #define   GET16Gbl_avPllCtl_avPllKVCO_A(r16)               _BFGET_(r16,12,10)
    #define   SET16Gbl_avPllCtl_avPllKVCO_A(r16,v)             _BFSET_(r16,12,10,v)

    #define   GET32Gbl_avPllCtl_avPllKVCO_B(r32)               _BFGET_(r32,31,29)
    #define   SET32Gbl_avPllCtl_avPllKVCO_B(r32,v)             _BFSET_(r32,31,29,v)
    #define   GET16Gbl_avPllCtl_avPllKVCO_B(r16)               _BFGET_(r16,15,13)
    #define   SET16Gbl_avPllCtl_avPllKVCO_B(r16,v)             _BFSET_(r16,15,13,v)

    #define     w32Gbl_avPllCtl1                               {\
            UNSG32 uavPllCtl_avPllFbDiv_A                      :  8;\
            UNSG32 uavPllCtl_avPllFbDiv_B                      :  8;\
            UNSG32 uavPllCtl_avPllSelV2i_A                     :  3;\
            UNSG32 uavPllCtl_avPllSelV2i_B                     :  3;\
            UNSG32 uavPllCtl_avPllSel_LpFRes_A                 :  1;\
            UNSG32 uavPllCtl_avPllSel_LpFRes_B                 :  1;\
            UNSG32 uavPllCtl_avPllCalStart                     :  1;\
            UNSG32 uavPllCtl_avPllKVCO_Ext_En                  :  1;\
            UNSG32 uavPllCtl_avPllKVCO_A                       :  3;\
            UNSG32 uavPllCtl_avPllKVCO_B                       :  3;\
          }
    union { UNSG32 u32Gbl_avPllCtl1;
            struct w32Gbl_avPllCtl1;
          };
    #define   GET32Gbl_avPllCtl_avPllReset_Ext_A(r32)          _BFGET_(r32, 7, 0)
    #define   SET32Gbl_avPllCtl_avPllReset_Ext_A(r32,v)        _BFSET_(r32, 7, 0,v)
    #define   GET16Gbl_avPllCtl_avPllReset_Ext_A(r16)          _BFGET_(r16, 7, 0)
    #define   SET16Gbl_avPllCtl_avPllReset_Ext_A(r16,v)        _BFSET_(r16, 7, 0,v)

    #define   GET32Gbl_avPllCtl_avPllReset_Ext_B(r32)          _BFGET_(r32,15, 8)
    #define   SET32Gbl_avPllCtl_avPllReset_Ext_B(r32,v)        _BFSET_(r32,15, 8,v)
    #define   GET16Gbl_avPllCtl_avPllReset_Ext_B(r16)          _BFGET_(r16,15, 8)
    #define   SET16Gbl_avPllCtl_avPllReset_Ext_B(r16,v)        _BFSET_(r16,15, 8,v)

    #define   GET32Gbl_avPllCtl_avPllIntpi_A(r32)              _BFGET_(r32,18,16)
    #define   SET32Gbl_avPllCtl_avPllIntpi_A(r32,v)            _BFSET_(r32,18,16,v)
    #define   GET16Gbl_avPllCtl_avPllIntpi_A(r16)              _BFGET_(r16, 2, 0)
    #define   SET16Gbl_avPllCtl_avPllIntpi_A(r16,v)            _BFSET_(r16, 2, 0,v)

    #define   GET32Gbl_avPllCtl_avPllIntpi_B(r32)              _BFGET_(r32,21,19)
    #define   SET32Gbl_avPllCtl_avPllIntpi_B(r32,v)            _BFSET_(r32,21,19,v)
    #define   GET16Gbl_avPllCtl_avPllIntpi_B(r16)              _BFGET_(r16, 5, 3)
    #define   SET16Gbl_avPllCtl_avPllIntpi_B(r16,v)            _BFSET_(r16, 5, 3,v)

    #define   GET32Gbl_avPllCtl_avPll1000PPM_En_A(r32)         _BFGET_(r32,28,22)
    #define   SET32Gbl_avPllCtl_avPll1000PPM_En_A(r32,v)       _BFSET_(r32,28,22,v)
    #define   GET16Gbl_avPllCtl_avPll1000PPM_En_A(r16)         _BFGET_(r16,12, 6)
    #define   SET16Gbl_avPllCtl_avPll1000PPM_En_A(r16,v)       _BFSET_(r16,12, 6,v)

    #define     w32Gbl_avPllCtl2                               {\
            UNSG32 uavPllCtl_avPllReset_Ext_A                  :  8;\
            UNSG32 uavPllCtl_avPllReset_Ext_B                  :  8;\
            UNSG32 uavPllCtl_avPllIntpi_A                      :  3;\
            UNSG32 uavPllCtl_avPllIntpi_B                      :  3;\
            UNSG32 uavPllCtl_avPll1000PPM_En_A                 :  7;\
            UNSG32 RSVDx48_b29                                 :  3;\
          }
    union { UNSG32 u32Gbl_avPllCtl2;
            struct w32Gbl_avPllCtl2;
          };
    #define   GET32Gbl_avPllCtl_avPll1000PPM_En_B(r32)         _BFGET_(r32, 6, 0)
    #define   SET32Gbl_avPllCtl_avPll1000PPM_En_B(r32,v)       _BFSET_(r32, 6, 0,v)
    #define   GET16Gbl_avPllCtl_avPll1000PPM_En_B(r16)         _BFGET_(r16, 6, 0)
    #define   SET16Gbl_avPllCtl_avPll1000PPM_En_B(r16,v)       _BFSET_(r16, 6, 0,v)

    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_A1(r32)       _BFGET_(r32,22, 7)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_A1(r32,v)     _BFSET_(r32,22, 7,v)

    #define     w32Gbl_avPllCtl3                               {\
            UNSG32 uavPllCtl_avPll1000PPM_En_B                 :  7;\
            UNSG32 uavPllCtl_avPllFreq_Offset_A1               : 16;\
            UNSG32 RSVDx4C_b23                                 :  9;\
          }
    union { UNSG32 u32Gbl_avPllCtl3;
            struct w32Gbl_avPllCtl3;
          };
    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_A2(r32)       _BFGET_(r32,15, 0)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_A2(r32,v)     _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_A2(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_A2(r16,v)     _BFSET_(r16,15, 0,v)

    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_A3(r32)       _BFGET_(r32,31,16)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_A3(r32,v)     _BFSET_(r32,31,16,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_A3(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_A3(r16,v)     _BFSET_(r16,15, 0,v)

    #define     w32Gbl_avPllCtl4                               {\
            UNSG32 uavPllCtl_avPllFreq_Offset_A2               : 16;\
            UNSG32 uavPllCtl_avPllFreq_Offset_A3               : 16;\
          }
    union { UNSG32 u32Gbl_avPllCtl4;
            struct w32Gbl_avPllCtl4;
          };
    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_A4(r32)       _BFGET_(r32,15, 0)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_A4(r32,v)     _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_A4(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_A4(r16,v)     _BFSET_(r16,15, 0,v)

    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_A5(r32)       _BFGET_(r32,31,16)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_A5(r32,v)     _BFSET_(r32,31,16,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_A5(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_A5(r16,v)     _BFSET_(r16,15, 0,v)

    #define     w32Gbl_avPllCtl5                               {\
            UNSG32 uavPllCtl_avPllFreq_Offset_A4               : 16;\
            UNSG32 uavPllCtl_avPllFreq_Offset_A5               : 16;\
          }
    union { UNSG32 u32Gbl_avPllCtl5;
            struct w32Gbl_avPllCtl5;
          };
    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_A6(r32)       _BFGET_(r32,15, 0)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_A6(r32,v)     _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_A6(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_A6(r16,v)     _BFSET_(r16,15, 0,v)

    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_A7(r32)       _BFGET_(r32,31,16)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_A7(r32,v)     _BFSET_(r32,31,16,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_A7(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_A7(r16,v)     _BFSET_(r16,15, 0,v)

    #define     w32Gbl_avPllCtl6                               {\
            UNSG32 uavPllCtl_avPllFreq_Offset_A6               : 16;\
            UNSG32 uavPllCtl_avPllFreq_Offset_A7               : 16;\
          }
    union { UNSG32 u32Gbl_avPllCtl6;
            struct w32Gbl_avPllCtl6;
          };
    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_A8(r32)       _BFGET_(r32,15, 0)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_A8(r32,v)     _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_A8(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_A8(r16,v)     _BFSET_(r16,15, 0,v)

    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_B1(r32)       _BFGET_(r32,31,16)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_B1(r32,v)     _BFSET_(r32,31,16,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_B1(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_B1(r16,v)     _BFSET_(r16,15, 0,v)

    #define     w32Gbl_avPllCtl7                               {\
            UNSG32 uavPllCtl_avPllFreq_Offset_A8               : 16;\
            UNSG32 uavPllCtl_avPllFreq_Offset_B1               : 16;\
          }
    union { UNSG32 u32Gbl_avPllCtl7;
            struct w32Gbl_avPllCtl7;
          };
    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_B2(r32)       _BFGET_(r32,15, 0)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_B2(r32,v)     _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_B2(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_B2(r16,v)     _BFSET_(r16,15, 0,v)

    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_B3(r32)       _BFGET_(r32,31,16)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_B3(r32,v)     _BFSET_(r32,31,16,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_B3(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_B3(r16,v)     _BFSET_(r16,15, 0,v)

    #define     w32Gbl_avPllCtl8                               {\
            UNSG32 uavPllCtl_avPllFreq_Offset_B2               : 16;\
            UNSG32 uavPllCtl_avPllFreq_Offset_B3               : 16;\
          }
    union { UNSG32 u32Gbl_avPllCtl8;
            struct w32Gbl_avPllCtl8;
          };
    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_B4(r32)       _BFGET_(r32,15, 0)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_B4(r32,v)     _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_B4(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_B4(r16,v)     _BFSET_(r16,15, 0,v)

    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_B5(r32)       _BFGET_(r32,31,16)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_B5(r32,v)     _BFSET_(r32,31,16,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_B5(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_B5(r16,v)     _BFSET_(r16,15, 0,v)

    #define     w32Gbl_avPllCtl9                               {\
            UNSG32 uavPllCtl_avPllFreq_Offset_B4               : 16;\
            UNSG32 uavPllCtl_avPllFreq_Offset_B5               : 16;\
          }
    union { UNSG32 u32Gbl_avPllCtl9;
            struct w32Gbl_avPllCtl9;
          };
    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_B6(r32)       _BFGET_(r32,15, 0)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_B6(r32,v)     _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_B6(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_B6(r16,v)     _BFSET_(r16,15, 0,v)

    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_B7(r32)       _BFGET_(r32,31,16)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_B7(r32,v)     _BFSET_(r32,31,16,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_B7(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_B7(r16,v)     _BFSET_(r16,15, 0,v)

    #define     w32Gbl_avPllCtl10                              {\
            UNSG32 uavPllCtl_avPllFreq_Offset_B6               : 16;\
            UNSG32 uavPllCtl_avPllFreq_Offset_B7               : 16;\
          }
    union { UNSG32 u32Gbl_avPllCtl10;
            struct w32Gbl_avPllCtl10;
          };
    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_B8(r32)       _BFGET_(r32,15, 0)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_B8(r32,v)     _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_B8(r16)       _BFGET_(r16,15, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_B8(r16,v)     _BFSET_(r16,15, 0,v)

    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_Rdy_A(r32)    _BFGET_(r32,22,16)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_Rdy_A(r32,v)  _BFSET_(r32,22,16,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_Rdy_A(r16)    _BFGET_(r16, 6, 0)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_Rdy_A(r16,v)  _BFSET_(r16, 6, 0,v)

    #define   GET32Gbl_avPllCtl_avPllFreq_Offset_Rdy_B(r32)    _BFGET_(r32,29,23)
    #define   SET32Gbl_avPllCtl_avPllFreq_Offset_Rdy_B(r32,v)  _BFSET_(r32,29,23,v)
    #define   GET16Gbl_avPllCtl_avPllFreq_Offset_Rdy_B(r16)    _BFGET_(r16,13, 7)
    #define   SET16Gbl_avPllCtl_avPllFreq_Offset_Rdy_B(r16,v)  _BFSET_(r16,13, 7,v)

    #define     w32Gbl_avPllCtl11                              {\
            UNSG32 uavPllCtl_avPllFreq_Offset_B8               : 16;\
            UNSG32 uavPllCtl_avPllFreq_Offset_Rdy_A            :  7;\
            UNSG32 uavPllCtl_avPllFreq_Offset_Rdy_B            :  7;\
            UNSG32 RSVDx6C_b30                                 :  2;\
          }
    union { UNSG32 u32Gbl_avPllCtl11;
            struct w32Gbl_avPllCtl11;
          };
    #define   GET32Gbl_avPllCtl_avPllPU_A(r32)                 _BFGET_(r32, 6, 0)
    #define   SET32Gbl_avPllCtl_avPllPU_A(r32,v)               _BFSET_(r32, 6, 0,v)
    #define   GET16Gbl_avPllCtl_avPllPU_A(r16)                 _BFGET_(r16, 6, 0)
    #define   SET16Gbl_avPllCtl_avPllPU_A(r16,v)               _BFSET_(r16, 6, 0,v)

    #define   GET32Gbl_avPllCtl_avPllPU_B(r32)                 _BFGET_(r32,13, 7)
    #define   SET32Gbl_avPllCtl_avPllPU_B(r32,v)               _BFSET_(r32,13, 7,v)
    #define   GET16Gbl_avPllCtl_avPllPU_B(r16)                 _BFGET_(r16,13, 7)
    #define   SET16Gbl_avPllCtl_avPllPU_B(r16,v)               _BFSET_(r16,13, 7,v)

    #define   GET32Gbl_avPllCtl_avPllDiv_A1(r32)               _BFGET_(r32,22,14)
    #define   SET32Gbl_avPllCtl_avPllDiv_A1(r32,v)             _BFSET_(r32,22,14,v)

    #define   GET32Gbl_avPllCtl_avPllDiv_A2(r32)               _BFGET_(r32,31,23)
    #define   SET32Gbl_avPllCtl_avPllDiv_A2(r32,v)             _BFSET_(r32,31,23,v)
    #define   GET16Gbl_avPllCtl_avPllDiv_A2(r16)               _BFGET_(r16,15, 7)
    #define   SET16Gbl_avPllCtl_avPllDiv_A2(r16,v)             _BFSET_(r16,15, 7,v)

    #define     w32Gbl_avPllCtl12                              {\
            UNSG32 uavPllCtl_avPllPU_A                         :  7;\
            UNSG32 uavPllCtl_avPllPU_B                         :  7;\
            UNSG32 uavPllCtl_avPllDiv_A1                       :  9;\
            UNSG32 uavPllCtl_avPllDiv_A2                       :  9;\
          }
    union { UNSG32 u32Gbl_avPllCtl12;
            struct w32Gbl_avPllCtl12;
          };
    #define   GET32Gbl_avPllCtl_avPllDiv_A3(r32)               _BFGET_(r32, 8, 0)
    #define   SET32Gbl_avPllCtl_avPllDiv_A3(r32,v)             _BFSET_(r32, 8, 0,v)
    #define   GET16Gbl_avPllCtl_avPllDiv_A3(r16)               _BFGET_(r16, 8, 0)
    #define   SET16Gbl_avPllCtl_avPllDiv_A3(r16,v)             _BFSET_(r16, 8, 0,v)

    #define   GET32Gbl_avPllCtl_avPllDiv_A4(r32)               _BFGET_(r32,17, 9)
    #define   SET32Gbl_avPllCtl_avPllDiv_A4(r32,v)             _BFSET_(r32,17, 9,v)

    #define   GET32Gbl_avPllCtl_avPllDiv_A5(r32)               _BFGET_(r32,26,18)
    #define   SET32Gbl_avPllCtl_avPllDiv_A5(r32,v)             _BFSET_(r32,26,18,v)
    #define   GET16Gbl_avPllCtl_avPllDiv_A5(r16)               _BFGET_(r16,10, 2)
    #define   SET16Gbl_avPllCtl_avPllDiv_A5(r16,v)             _BFSET_(r16,10, 2,v)

    #define     w32Gbl_avPllCtl13                              {\
            UNSG32 uavPllCtl_avPllDiv_A3                       :  9;\
            UNSG32 uavPllCtl_avPllDiv_A4                       :  9;\
            UNSG32 uavPllCtl_avPllDiv_A5                       :  9;\
            UNSG32 RSVDx74_b27                                 :  5;\
          }
    union { UNSG32 u32Gbl_avPllCtl13;
            struct w32Gbl_avPllCtl13;
          };
    #define   GET32Gbl_avPllCtl_avPllDiv_A6(r32)               _BFGET_(r32, 8, 0)
    #define   SET32Gbl_avPllCtl_avPllDiv_A6(r32,v)             _BFSET_(r32, 8, 0,v)
    #define   GET16Gbl_avPllCtl_avPllDiv_A6(r16)               _BFGET_(r16, 8, 0)
    #define   SET16Gbl_avPllCtl_avPllDiv_A6(r16,v)             _BFSET_(r16, 8, 0,v)

    #define   GET32Gbl_avPllCtl_avPllDiv_A7(r32)               _BFGET_(r32,17, 9)
    #define   SET32Gbl_avPllCtl_avPllDiv_A7(r32,v)             _BFSET_(r32,17, 9,v)

    #define   GET32Gbl_avPllCtl_avPllDiv_B1(r32)               _BFGET_(r32,26,18)
    #define   SET32Gbl_avPllCtl_avPllDiv_B1(r32,v)             _BFSET_(r32,26,18,v)
    #define   GET16Gbl_avPllCtl_avPllDiv_B1(r16)               _BFGET_(r16,10, 2)
    #define   SET16Gbl_avPllCtl_avPllDiv_B1(r16,v)             _BFSET_(r16,10, 2,v)

    #define     w32Gbl_avPllCtl14                              {\
            UNSG32 uavPllCtl_avPllDiv_A6                       :  9;\
            UNSG32 uavPllCtl_avPllDiv_A7                       :  9;\
            UNSG32 uavPllCtl_avPllDiv_B1                       :  9;\
            UNSG32 RSVDx78_b27                                 :  5;\
          }
    union { UNSG32 u32Gbl_avPllCtl14;
            struct w32Gbl_avPllCtl14;
          };
    #define   GET32Gbl_avPllCtl_avPllDiv_B2(r32)               _BFGET_(r32, 8, 0)
    #define   SET32Gbl_avPllCtl_avPllDiv_B2(r32,v)             _BFSET_(r32, 8, 0,v)
    #define   GET16Gbl_avPllCtl_avPllDiv_B2(r16)               _BFGET_(r16, 8, 0)
    #define   SET16Gbl_avPllCtl_avPllDiv_B2(r16,v)             _BFSET_(r16, 8, 0,v)

    #define   GET32Gbl_avPllCtl_avPllDiv_B3(r32)               _BFGET_(r32,17, 9)
    #define   SET32Gbl_avPllCtl_avPllDiv_B3(r32,v)             _BFSET_(r32,17, 9,v)

    #define   GET32Gbl_avPllCtl_avPllDiv_B4(r32)               _BFGET_(r32,26,18)
    #define   SET32Gbl_avPllCtl_avPllDiv_B4(r32,v)             _BFSET_(r32,26,18,v)
    #define   GET16Gbl_avPllCtl_avPllDiv_B4(r16)               _BFGET_(r16,10, 2)
    #define   SET16Gbl_avPllCtl_avPllDiv_B4(r16,v)             _BFSET_(r16,10, 2,v)

    #define     w32Gbl_avPllCtl15                              {\
            UNSG32 uavPllCtl_avPllDiv_B2                       :  9;\
            UNSG32 uavPllCtl_avPllDiv_B3                       :  9;\
            UNSG32 uavPllCtl_avPllDiv_B4                       :  9;\
            UNSG32 RSVDx7C_b27                                 :  5;\
          }
    union { UNSG32 u32Gbl_avPllCtl15;
            struct w32Gbl_avPllCtl15;
          };
    #define   GET32Gbl_avPllCtl_avPllDiv_B5(r32)               _BFGET_(r32, 8, 0)
    #define   SET32Gbl_avPllCtl_avPllDiv_B5(r32,v)             _BFSET_(r32, 8, 0,v)
    #define   GET16Gbl_avPllCtl_avPllDiv_B5(r16)               _BFGET_(r16, 8, 0)
    #define   SET16Gbl_avPllCtl_avPllDiv_B5(r16,v)             _BFSET_(r16, 8, 0,v)

    #define   GET32Gbl_avPllCtl_avPllDiv_B6(r32)               _BFGET_(r32,17, 9)
    #define   SET32Gbl_avPllCtl_avPllDiv_B6(r32,v)             _BFSET_(r32,17, 9,v)

    #define   GET32Gbl_avPllCtl_avPllDiv_B7(r32)               _BFGET_(r32,26,18)
    #define   SET32Gbl_avPllCtl_avPllDiv_B7(r32,v)             _BFSET_(r32,26,18,v)
    #define   GET16Gbl_avPllCtl_avPllDiv_B7(r16)               _BFGET_(r16,10, 2)
    #define   SET16Gbl_avPllCtl_avPllDiv_B7(r16,v)             _BFSET_(r16,10, 2,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_A1(r32)              _BFGET_(r32,30,27)
    #define   SET32Gbl_avPllCtl_avPllDiv2_A1(r32,v)            _BFSET_(r32,30,27,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_A1(r16)              _BFGET_(r16,14,11)
    #define   SET16Gbl_avPllCtl_avPllDiv2_A1(r16,v)            _BFSET_(r16,14,11,v)

    #define     w32Gbl_avPllCtl16                              {\
            UNSG32 uavPllCtl_avPllDiv_B5                       :  9;\
            UNSG32 uavPllCtl_avPllDiv_B6                       :  9;\
            UNSG32 uavPllCtl_avPllDiv_B7                       :  9;\
            UNSG32 uavPllCtl_avPllDiv2_A1                      :  4;\
            UNSG32 RSVDx80_b31                                 :  1;\
          }
    union { UNSG32 u32Gbl_avPllCtl16;
            struct w32Gbl_avPllCtl16;
          };
    #define   GET32Gbl_avPllCtl_avPllDiv2_A2(r32)              _BFGET_(r32, 3, 0)
    #define   SET32Gbl_avPllCtl_avPllDiv2_A2(r32,v)            _BFSET_(r32, 3, 0,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_A2(r16)              _BFGET_(r16, 3, 0)
    #define   SET16Gbl_avPllCtl_avPllDiv2_A2(r16,v)            _BFSET_(r16, 3, 0,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_A3(r32)              _BFGET_(r32, 7, 4)
    #define   SET32Gbl_avPllCtl_avPllDiv2_A3(r32,v)            _BFSET_(r32, 7, 4,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_A3(r16)              _BFGET_(r16, 7, 4)
    #define   SET16Gbl_avPllCtl_avPllDiv2_A3(r16,v)            _BFSET_(r16, 7, 4,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_A4(r32)              _BFGET_(r32,11, 8)
    #define   SET32Gbl_avPllCtl_avPllDiv2_A4(r32,v)            _BFSET_(r32,11, 8,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_A4(r16)              _BFGET_(r16,11, 8)
    #define   SET16Gbl_avPllCtl_avPllDiv2_A4(r16,v)            _BFSET_(r16,11, 8,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_A5(r32)              _BFGET_(r32,15,12)
    #define   SET32Gbl_avPllCtl_avPllDiv2_A5(r32,v)            _BFSET_(r32,15,12,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_A5(r16)              _BFGET_(r16,15,12)
    #define   SET16Gbl_avPllCtl_avPllDiv2_A5(r16,v)            _BFSET_(r16,15,12,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_A6(r32)              _BFGET_(r32,19,16)
    #define   SET32Gbl_avPllCtl_avPllDiv2_A6(r32,v)            _BFSET_(r32,19,16,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_A6(r16)              _BFGET_(r16, 3, 0)
    #define   SET16Gbl_avPllCtl_avPllDiv2_A6(r16,v)            _BFSET_(r16, 3, 0,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_A7(r32)              _BFGET_(r32,23,20)
    #define   SET32Gbl_avPllCtl_avPllDiv2_A7(r32,v)            _BFSET_(r32,23,20,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_A7(r16)              _BFGET_(r16, 7, 4)
    #define   SET16Gbl_avPllCtl_avPllDiv2_A7(r16,v)            _BFSET_(r16, 7, 4,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_B1(r32)              _BFGET_(r32,27,24)
    #define   SET32Gbl_avPllCtl_avPllDiv2_B1(r32,v)            _BFSET_(r32,27,24,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_B1(r16)              _BFGET_(r16,11, 8)
    #define   SET16Gbl_avPllCtl_avPllDiv2_B1(r16,v)            _BFSET_(r16,11, 8,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_B2(r32)              _BFGET_(r32,31,28)
    #define   SET32Gbl_avPllCtl_avPllDiv2_B2(r32,v)            _BFSET_(r32,31,28,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_B2(r16)              _BFGET_(r16,15,12)
    #define   SET16Gbl_avPllCtl_avPllDiv2_B2(r16,v)            _BFSET_(r16,15,12,v)

    #define     w32Gbl_avPllCtl17                              {\
            UNSG32 uavPllCtl_avPllDiv2_A2                      :  4;\
            UNSG32 uavPllCtl_avPllDiv2_A3                      :  4;\
            UNSG32 uavPllCtl_avPllDiv2_A4                      :  4;\
            UNSG32 uavPllCtl_avPllDiv2_A5                      :  4;\
            UNSG32 uavPllCtl_avPllDiv2_A6                      :  4;\
            UNSG32 uavPllCtl_avPllDiv2_A7                      :  4;\
            UNSG32 uavPllCtl_avPllDiv2_B1                      :  4;\
            UNSG32 uavPllCtl_avPllDiv2_B2                      :  4;\
          }
    union { UNSG32 u32Gbl_avPllCtl17;
            struct w32Gbl_avPllCtl17;
          };
    #define   GET32Gbl_avPllCtl_avPllDiv2_B3(r32)              _BFGET_(r32, 3, 0)
    #define   SET32Gbl_avPllCtl_avPllDiv2_B3(r32,v)            _BFSET_(r32, 3, 0,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_B3(r16)              _BFGET_(r16, 3, 0)
    #define   SET16Gbl_avPllCtl_avPllDiv2_B3(r16,v)            _BFSET_(r16, 3, 0,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_B4(r32)              _BFGET_(r32, 7, 4)
    #define   SET32Gbl_avPllCtl_avPllDiv2_B4(r32,v)            _BFSET_(r32, 7, 4,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_B4(r16)              _BFGET_(r16, 7, 4)
    #define   SET16Gbl_avPllCtl_avPllDiv2_B4(r16,v)            _BFSET_(r16, 7, 4,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_B5(r32)              _BFGET_(r32,11, 8)
    #define   SET32Gbl_avPllCtl_avPllDiv2_B5(r32,v)            _BFSET_(r32,11, 8,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_B5(r16)              _BFGET_(r16,11, 8)
    #define   SET16Gbl_avPllCtl_avPllDiv2_B5(r16,v)            _BFSET_(r16,11, 8,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_B6(r32)              _BFGET_(r32,15,12)
    #define   SET32Gbl_avPllCtl_avPllDiv2_B6(r32,v)            _BFSET_(r32,15,12,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_B6(r16)              _BFGET_(r16,15,12)
    #define   SET16Gbl_avPllCtl_avPllDiv2_B6(r16,v)            _BFSET_(r16,15,12,v)

    #define   GET32Gbl_avPllCtl_avPllDiv2_B7(r32)              _BFGET_(r32,19,16)
    #define   SET32Gbl_avPllCtl_avPllDiv2_B7(r32,v)            _BFSET_(r32,19,16,v)
    #define   GET16Gbl_avPllCtl_avPllDiv2_B7(r16)              _BFGET_(r16, 3, 0)
    #define   SET16Gbl_avPllCtl_avPllDiv2_B7(r16,v)            _BFSET_(r16, 3, 0,v)

    #define   GET32Gbl_avPllCtl_avPllTestAna(r32)              _BFGET_(r32,25,20)
    #define   SET32Gbl_avPllCtl_avPllTestAna(r32,v)            _BFSET_(r32,25,20,v)
    #define   GET16Gbl_avPllCtl_avPllTestAna(r16)              _BFGET_(r16, 9, 4)
    #define   SET16Gbl_avPllCtl_avPllTestAna(r16,v)            _BFSET_(r16, 9, 4,v)

    #define     w32Gbl_avPllCtl18                              {\
            UNSG32 uavPllCtl_avPllDiv2_B3                      :  4;\
            UNSG32 uavPllCtl_avPllDiv2_B4                      :  4;\
            UNSG32 uavPllCtl_avPllDiv2_B5                      :  4;\
            UNSG32 uavPllCtl_avPllDiv2_B6                      :  4;\
            UNSG32 uavPllCtl_avPllDiv2_B7                      :  4;\
            UNSG32 uavPllCtl_avPllTestAna                      :  6;\
            UNSG32 RSVDx88_b26                                 :  6;\
          }
    union { UNSG32 u32Gbl_avPllCtl18;
            struct w32Gbl_avPllCtl18;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_pllStatus_sysPllLock(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_pllStatus_sysPllLock(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_pllStatus_sysPllLock(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_pllStatus_sysPllLock(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_pllStatus_memPllLock(r32)               _BFGET_(r32, 1, 1)
    #define   SET32Gbl_pllStatus_memPllLock(r32,v)             _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_pllStatus_memPllLock(r16)               _BFGET_(r16, 1, 1)
    #define   SET16Gbl_pllStatus_memPllLock(r16,v)             _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_pllStatus_cpu1PllLock(r32)              _BFGET_(r32, 2, 2)
    #define   SET32Gbl_pllStatus_cpu1PllLock(r32,v)            _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_pllStatus_cpu1PllLock(r16)              _BFGET_(r16, 2, 2)
    #define   SET16Gbl_pllStatus_cpu1PllLock(r16,v)            _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_pllStatus_avPllALock(r32)               _BFGET_(r32, 3, 3)
    #define   SET32Gbl_pllStatus_avPllALock(r32,v)             _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_pllStatus_avPllALock(r16)               _BFGET_(r16, 3, 3)
    #define   SET16Gbl_pllStatus_avPllALock(r16,v)             _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_pllStatus_avPllBLock(r32)               _BFGET_(r32, 4, 4)
    #define   SET32Gbl_pllStatus_avPllBLock(r32,v)             _BFSET_(r32, 4, 4,v)
    #define   GET16Gbl_pllStatus_avPllBLock(r16)               _BFGET_(r16, 4, 4)
    #define   SET16Gbl_pllStatus_avPllBLock(r16,v)             _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_pllStatus_avPllKVCOAInt(r32)            _BFGET_(r32, 7, 5)
    #define   SET32Gbl_pllStatus_avPllKVCOAInt(r32,v)          _BFSET_(r32, 7, 5,v)
    #define   GET16Gbl_pllStatus_avPllKVCOAInt(r16)            _BFGET_(r16, 7, 5)
    #define   SET16Gbl_pllStatus_avPllKVCOAInt(r16,v)          _BFSET_(r16, 7, 5,v)

    #define   GET32Gbl_pllStatus_avPllCaliDone(r32)            _BFGET_(r32, 8, 8)
    #define   SET32Gbl_pllStatus_avPllCaliDone(r32,v)          _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_pllStatus_avPllCaliDone(r16)            _BFGET_(r16, 8, 8)
    #define   SET16Gbl_pllStatus_avPllCaliDone(r16,v)          _BFSET_(r16, 8, 8,v)

    #define     w32Gbl_pllStatus                               {\
            UNSG32 upllStatus_sysPllLock                       :  1;\
            UNSG32 upllStatus_memPllLock                       :  1;\
            UNSG32 upllStatus_cpu1PllLock                      :  1;\
            UNSG32 upllStatus_avPllALock                       :  1;\
            UNSG32 upllStatus_avPllBLock                       :  1;\
            UNSG32 upllStatus_avPllKVCOAInt                    :  3;\
            UNSG32 upllStatus_avPllCaliDone                    :  1;\
            UNSG32 RSVDx8C_b9                                  : 23;\
          }
    union { UNSG32 u32Gbl_pllStatus;
            struct w32Gbl_pllStatus;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_clkEnable_sysClkEn(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_clkEnable_sysClkEn(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_clkEnable_sysClkEn(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_clkEnable_sysClkEn(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_clkEnable_cfgClkEn(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_clkEnable_cfgClkEn(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_clkEnable_cfgClkEn(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_clkEnable_cfgClkEn(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_clkEnable_pCubeClkEn(r32)               _BFGET_(r32, 2, 2)
    #define   SET32Gbl_clkEnable_pCubeClkEn(r32,v)             _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_clkEnable_pCubeClkEn(r16)               _BFGET_(r16, 2, 2)
    #define   SET16Gbl_clkEnable_pCubeClkEn(r16,v)             _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_clkEnable_vScopeClkEn(r32)              _BFGET_(r32, 3, 3)
    #define   SET32Gbl_clkEnable_vScopeClkEn(r32,v)            _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_clkEnable_vScopeClkEn(r16)              _BFGET_(r16, 3, 3)
    #define   SET16Gbl_clkEnable_vScopeClkEn(r16,v)            _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_clkEnable_gfxClkEn(r32)                 _BFGET_(r32, 4, 4)
    #define   SET32Gbl_clkEnable_gfxClkEn(r32,v)               _BFSET_(r32, 4, 4,v)
    #define   GET16Gbl_clkEnable_gfxClkEn(r16)                 _BFGET_(r16, 4, 4)
    #define   SET16Gbl_clkEnable_gfxClkEn(r16,v)               _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_clkEnable_zspClkEn(r32)                 _BFGET_(r32, 5, 5)
    #define   SET32Gbl_clkEnable_zspClkEn(r32,v)               _BFSET_(r32, 5, 5,v)
    #define   GET16Gbl_clkEnable_zspClkEn(r16)                 _BFGET_(r16, 5, 5)
    #define   SET16Gbl_clkEnable_zspClkEn(r16,v)               _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_clkEnable_perifClkEn(r32)               _BFGET_(r32, 6, 6)
    #define   SET32Gbl_clkEnable_perifClkEn(r32,v)             _BFSET_(r32, 6, 6,v)
    #define   GET16Gbl_clkEnable_perifClkEn(r16)               _BFGET_(r16, 6, 6)
    #define   SET16Gbl_clkEnable_perifClkEn(r16,v)             _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_clkEnable_gethCoreClkEn(r32)            _BFGET_(r32, 7, 7)
    #define   SET32Gbl_clkEnable_gethCoreClkEn(r32,v)          _BFSET_(r32, 7, 7,v)
    #define   GET16Gbl_clkEnable_gethCoreClkEn(r16)            _BFGET_(r16, 7, 7)
    #define   SET16Gbl_clkEnable_gethCoreClkEn(r16,v)          _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_clkEnable_pexCoreClkEn(r32)             _BFGET_(r32, 8, 8)
    #define   SET32Gbl_clkEnable_pexCoreClkEn(r32,v)           _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_clkEnable_pexCoreClkEn(r16)             _BFGET_(r16, 8, 8)
    #define   SET16Gbl_clkEnable_pexCoreClkEn(r16,v)           _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_clkEnable_sataCoreClkEn(r32)            _BFGET_(r32, 9, 9)
    #define   SET32Gbl_clkEnable_sataCoreClkEn(r32,v)          _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_clkEnable_sataCoreClkEn(r16)            _BFGET_(r16, 9, 9)
    #define   SET16Gbl_clkEnable_sataCoreClkEn(r16,v)          _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_clkEnable_ahbApbCoreClkEn(r32)          _BFGET_(r32,10,10)
    #define   SET32Gbl_clkEnable_ahbApbCoreClkEn(r32,v)        _BFSET_(r32,10,10,v)
    #define   GET16Gbl_clkEnable_ahbApbCoreClkEn(r16)          _BFGET_(r16,10,10)
    #define   SET16Gbl_clkEnable_ahbApbCoreClkEn(r16,v)        _BFSET_(r16,10,10,v)

    #define   GET32Gbl_clkEnable_usb0CoreClkEn(r32)            _BFGET_(r32,11,11)
    #define   SET32Gbl_clkEnable_usb0CoreClkEn(r32,v)          _BFSET_(r32,11,11,v)
    #define   GET16Gbl_clkEnable_usb0CoreClkEn(r16)            _BFGET_(r16,11,11)
    #define   SET16Gbl_clkEnable_usb0CoreClkEn(r16,v)          _BFSET_(r16,11,11,v)

    #define   GET32Gbl_clkEnable_usb1CoreClkEn(r32)            _BFGET_(r32,12,12)
    #define   SET32Gbl_clkEnable_usb1CoreClkEn(r32,v)          _BFSET_(r32,12,12,v)
    #define   GET16Gbl_clkEnable_usb1CoreClkEn(r16)            _BFGET_(r16,12,12)
    #define   SET16Gbl_clkEnable_usb1CoreClkEn(r16,v)          _BFSET_(r16,12,12,v)

    #define   GET32Gbl_clkEnable_pBridgeCoreClkEn(r32)         _BFGET_(r32,13,13)
    #define   SET32Gbl_clkEnable_pBridgeCoreClkEn(r32,v)       _BFSET_(r32,13,13,v)
    #define   GET16Gbl_clkEnable_pBridgeCoreClkEn(r16)         _BFGET_(r16,13,13)
    #define   SET16Gbl_clkEnable_pBridgeCoreClkEn(r16,v)       _BFSET_(r16,13,13,v)

    #define   GET32Gbl_clkEnable_sdioCoreClkEn(r32)            _BFGET_(r32,14,14)
    #define   SET32Gbl_clkEnable_sdioCoreClkEn(r32,v)          _BFSET_(r32,14,14,v)
    #define   GET16Gbl_clkEnable_sdioCoreClkEn(r16)            _BFGET_(r16,14,14)
    #define   SET16Gbl_clkEnable_sdioCoreClkEn(r16,v)          _BFSET_(r16,14,14,v)

    #define   GET32Gbl_clkEnable_drmFigoClkEn(r32)             _BFGET_(r32,15,15)
    #define   SET32Gbl_clkEnable_drmFigoClkEn(r32,v)           _BFSET_(r32,15,15,v)
    #define   GET16Gbl_clkEnable_drmFigoClkEn(r16)             _BFGET_(r16,15,15)
    #define   SET16Gbl_clkEnable_drmFigoClkEn(r16,v)           _BFSET_(r16,15,15,v)

    #define   GET32Gbl_clkEnable_nfcCoreClkEn(r32)             _BFGET_(r32,16,16)
    #define   SET32Gbl_clkEnable_nfcCoreClkEn(r32,v)           _BFSET_(r32,16,16,v)
    #define   GET16Gbl_clkEnable_nfcCoreClkEn(r16)             _BFGET_(r16, 0, 0)
    #define   SET16Gbl_clkEnable_nfcCoreClkEn(r16,v)           _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_clkEnable_nfcEccClkEn(r32)              _BFGET_(r32,17,17)
    #define   SET32Gbl_clkEnable_nfcEccClkEn(r32,v)            _BFSET_(r32,17,17,v)
    #define   GET16Gbl_clkEnable_nfcEccClkEn(r16)              _BFGET_(r16, 1, 1)
    #define   SET16Gbl_clkEnable_nfcEccClkEn(r16,v)            _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_clkEnable_smemcCoreClkEn(r32)           _BFGET_(r32,18,18)
    #define   SET32Gbl_clkEnable_smemcCoreClkEn(r32,v)         _BFSET_(r32,18,18,v)
    #define   GET16Gbl_clkEnable_smemcCoreClkEn(r16)           _BFGET_(r16, 2, 2)
    #define   SET16Gbl_clkEnable_smemcCoreClkEn(r16,v)         _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_clkEnable_appClkEn(r32)                 _BFGET_(r32,19,19)
    #define   SET32Gbl_clkEnable_appClkEn(r32,v)               _BFSET_(r32,19,19,v)
    #define   GET16Gbl_clkEnable_appClkEn(r16)                 _BFGET_(r16, 3, 3)
    #define   SET16Gbl_clkEnable_appClkEn(r16,v)               _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_clkEnable_vppSysClkEn(r32)              _BFGET_(r32,20,20)
    #define   SET32Gbl_clkEnable_vppSysClkEn(r32,v)            _BFSET_(r32,20,20,v)
    #define   GET16Gbl_clkEnable_vppSysClkEn(r16)              _BFGET_(r16, 4, 4)
    #define   SET16Gbl_clkEnable_vppSysClkEn(r16,v)            _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_clkEnable_audio0ClkEn(r32)              _BFGET_(r32,21,21)
    #define   SET32Gbl_clkEnable_audio0ClkEn(r32,v)            _BFSET_(r32,21,21,v)
    #define   GET16Gbl_clkEnable_audio0ClkEn(r16)              _BFGET_(r16, 5, 5)
    #define   SET16Gbl_clkEnable_audio0ClkEn(r16,v)            _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_clkEnable_audio1ClkEn(r32)              _BFGET_(r32,22,22)
    #define   SET32Gbl_clkEnable_audio1ClkEn(r32,v)            _BFSET_(r32,22,22,v)
    #define   GET16Gbl_clkEnable_audio1ClkEn(r16)              _BFGET_(r16, 6, 6)
    #define   SET16Gbl_clkEnable_audio1ClkEn(r16,v)            _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_clkEnable_audio2ClkEn(r32)              _BFGET_(r32,23,23)
    #define   SET32Gbl_clkEnable_audio2ClkEn(r32,v)            _BFSET_(r32,23,23,v)
    #define   GET16Gbl_clkEnable_audio2ClkEn(r16)              _BFGET_(r16, 7, 7)
    #define   SET16Gbl_clkEnable_audio2ClkEn(r16,v)            _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_clkEnable_audio3ClkEn(r32)              _BFGET_(r32,24,24)
    #define   SET32Gbl_clkEnable_audio3ClkEn(r32,v)            _BFSET_(r32,24,24,v)
    #define   GET16Gbl_clkEnable_audio3ClkEn(r16)              _BFGET_(r16, 8, 8)
    #define   SET16Gbl_clkEnable_audio3ClkEn(r16,v)            _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_clkEnable_audioHdClkEn(r32)             _BFGET_(r32,25,25)
    #define   SET32Gbl_clkEnable_audioHdClkEn(r32,v)           _BFSET_(r32,25,25,v)
    #define   GET16Gbl_clkEnable_audioHdClkEn(r16)             _BFGET_(r16, 9, 9)
    #define   SET16Gbl_clkEnable_audioHdClkEn(r16,v)           _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_clkEnable_video0ClkEn(r32)              _BFGET_(r32,26,26)
    #define   SET32Gbl_clkEnable_video0ClkEn(r32,v)            _BFSET_(r32,26,26,v)
    #define   GET16Gbl_clkEnable_video0ClkEn(r16)              _BFGET_(r16,10,10)
    #define   SET16Gbl_clkEnable_video0ClkEn(r16,v)            _BFSET_(r16,10,10,v)

    #define   GET32Gbl_clkEnable_video1ClkEn(r32)              _BFGET_(r32,27,27)
    #define   SET32Gbl_clkEnable_video1ClkEn(r32,v)            _BFSET_(r32,27,27,v)
    #define   GET16Gbl_clkEnable_video1ClkEn(r16)              _BFGET_(r16,11,11)
    #define   SET16Gbl_clkEnable_video1ClkEn(r16,v)            _BFSET_(r16,11,11,v)

    #define   GET32Gbl_clkEnable_video2ClkEn(r32)              _BFGET_(r32,28,28)
    #define   SET32Gbl_clkEnable_video2ClkEn(r32,v)            _BFSET_(r32,28,28,v)
    #define   GET16Gbl_clkEnable_video2ClkEn(r16)              _BFGET_(r16,12,12)
    #define   SET16Gbl_clkEnable_video2ClkEn(r16,v)            _BFSET_(r16,12,12,v)

    #define   GET32Gbl_clkEnable_PEX_mx_refclk_off(r32)        _BFGET_(r32,29,29)
    #define   SET32Gbl_clkEnable_PEX_mx_refclk_off(r32,v)      _BFSET_(r32,29,29,v)
    #define   GET16Gbl_clkEnable_PEX_mx_refclk_off(r16)        _BFGET_(r16,13,13)
    #define   SET16Gbl_clkEnable_PEX_mx_refclk_off(r16,v)      _BFSET_(r16,13,13,v)

    #define     w32Gbl_clkEnable                               {\
            UNSG32 uclkEnable_sysClkEn                         :  1;\
            UNSG32 uclkEnable_cfgClkEn                         :  1;\
            UNSG32 uclkEnable_pCubeClkEn                       :  1;\
            UNSG32 uclkEnable_vScopeClkEn                      :  1;\
            UNSG32 uclkEnable_gfxClkEn                         :  1;\
            UNSG32 uclkEnable_zspClkEn                         :  1;\
            UNSG32 uclkEnable_perifClkEn                       :  1;\
            UNSG32 uclkEnable_gethCoreClkEn                    :  1;\
            UNSG32 uclkEnable_pexCoreClkEn                     :  1;\
            UNSG32 uclkEnable_sataCoreClkEn                    :  1;\
            UNSG32 uclkEnable_ahbApbCoreClkEn                  :  1;\
            UNSG32 uclkEnable_usb0CoreClkEn                    :  1;\
            UNSG32 uclkEnable_usb1CoreClkEn                    :  1;\
            UNSG32 uclkEnable_pBridgeCoreClkEn                 :  1;\
            UNSG32 uclkEnable_sdioCoreClkEn                    :  1;\
            UNSG32 uclkEnable_drmFigoClkEn                     :  1;\
            UNSG32 uclkEnable_nfcCoreClkEn                     :  1;\
            UNSG32 uclkEnable_nfcEccClkEn                      :  1;\
            UNSG32 uclkEnable_smemcCoreClkEn                   :  1;\
            UNSG32 uclkEnable_appClkEn                         :  1;\
            UNSG32 uclkEnable_vppSysClkEn                      :  1;\
            UNSG32 uclkEnable_audio0ClkEn                      :  1;\
            UNSG32 uclkEnable_audio1ClkEn                      :  1;\
            UNSG32 uclkEnable_audio2ClkEn                      :  1;\
            UNSG32 uclkEnable_audio3ClkEn                      :  1;\
            UNSG32 uclkEnable_audioHdClkEn                     :  1;\
            UNSG32 uclkEnable_video0ClkEn                      :  1;\
            UNSG32 uclkEnable_video1ClkEn                      :  1;\
            UNSG32 uclkEnable_video2ClkEn                      :  1;\
            UNSG32 uclkEnable_PEX_mx_refclk_off                :  1;\
            UNSG32 RSVDx90_b30                                 :  2;\
          }
    union { UNSG32 u32Gbl_clkEnable;
            struct w32Gbl_clkEnable;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_clkSelect_sysClkPllSel(r32)             _BFGET_(r32, 2, 0)
    #define   SET32Gbl_clkSelect_sysClkPllSel(r32,v)           _BFSET_(r32, 2, 0,v)
    #define   GET16Gbl_clkSelect_sysClkPllSel(r16)             _BFGET_(r16, 2, 0)
    #define   SET16Gbl_clkSelect_sysClkPllSel(r16,v)           _BFSET_(r16, 2, 0,v)

    #define   GET32Gbl_clkSelect_sysClkSel(r32)                _BFGET_(r32, 5, 3)
    #define   SET32Gbl_clkSelect_sysClkSel(r32,v)              _BFSET_(r32, 5, 3,v)
    #define   GET16Gbl_clkSelect_sysClkSel(r16)                _BFGET_(r16, 5, 3)
    #define   SET16Gbl_clkSelect_sysClkSel(r16,v)              _BFSET_(r16, 5, 3,v)

    #define   GET32Gbl_clkSelect_cpu0ClkPllSel(r32)            _BFGET_(r32, 8, 6)
    #define   SET32Gbl_clkSelect_cpu0ClkPllSel(r32,v)          _BFSET_(r32, 8, 6,v)
    #define   GET16Gbl_clkSelect_cpu0ClkPllSel(r16)            _BFGET_(r16, 8, 6)
    #define   SET16Gbl_clkSelect_cpu0ClkPllSel(r16,v)          _BFSET_(r16, 8, 6,v)

    #define   GET32Gbl_clkSelect_cpu0ClkSel(r32)               _BFGET_(r32,11, 9)
    #define   SET32Gbl_clkSelect_cpu0ClkSel(r32,v)             _BFSET_(r32,11, 9,v)
    #define   GET16Gbl_clkSelect_cpu0ClkSel(r16)               _BFGET_(r16,11, 9)
    #define   SET16Gbl_clkSelect_cpu0ClkSel(r16,v)             _BFSET_(r16,11, 9,v)

    #define   GET32Gbl_clkSelect_cpu1ClkPllSel(r32)            _BFGET_(r32,13,12)
    #define   SET32Gbl_clkSelect_cpu1ClkPllSel(r32,v)          _BFSET_(r32,13,12,v)
    #define   GET16Gbl_clkSelect_cpu1ClkPllSel(r16)            _BFGET_(r16,13,12)
    #define   SET16Gbl_clkSelect_cpu1ClkPllSel(r16,v)          _BFSET_(r16,13,12,v)

    #define   GET32Gbl_clkSelect_cpu1ClkSel(r32)               _BFGET_(r32,16,14)
    #define   SET32Gbl_clkSelect_cpu1ClkSel(r32,v)             _BFSET_(r32,16,14,v)

    #define   GET32Gbl_clkSelect_drmFigoClkPllSel(r32)         _BFGET_(r32,19,17)
    #define   SET32Gbl_clkSelect_drmFigoClkPllSel(r32,v)       _BFSET_(r32,19,17,v)
    #define   GET16Gbl_clkSelect_drmFigoClkPllSel(r16)         _BFGET_(r16, 3, 1)
    #define   SET16Gbl_clkSelect_drmFigoClkPllSel(r16,v)       _BFSET_(r16, 3, 1,v)

    #define   GET32Gbl_clkSelect_drmFigoClkSel(r32)            _BFGET_(r32,22,20)
    #define   SET32Gbl_clkSelect_drmFigoClkSel(r32,v)          _BFSET_(r32,22,20,v)
    #define   GET16Gbl_clkSelect_drmFigoClkSel(r16)            _BFGET_(r16, 6, 4)
    #define   SET16Gbl_clkSelect_drmFigoClkSel(r16,v)          _BFSET_(r16, 6, 4,v)

    #define   GET32Gbl_clkSelect_cfgClkPllSel(r32)             _BFGET_(r32,25,23)
    #define   SET32Gbl_clkSelect_cfgClkPllSel(r32,v)           _BFSET_(r32,25,23,v)
    #define   GET16Gbl_clkSelect_cfgClkPllSel(r16)             _BFGET_(r16, 9, 7)
    #define   SET16Gbl_clkSelect_cfgClkPllSel(r16,v)           _BFSET_(r16, 9, 7,v)

    #define   GET32Gbl_clkSelect_cfgClkSel(r32)                _BFGET_(r32,28,26)
    #define   SET32Gbl_clkSelect_cfgClkSel(r32,v)              _BFSET_(r32,28,26,v)
    #define   GET16Gbl_clkSelect_cfgClkSel(r16)                _BFGET_(r16,12,10)
    #define   SET16Gbl_clkSelect_cfgClkSel(r16,v)              _BFSET_(r16,12,10,v)

    #define   GET32Gbl_clkSelect_gfxClkPllSel(r32)             _BFGET_(r32,31,29)
    #define   SET32Gbl_clkSelect_gfxClkPllSel(r32,v)           _BFSET_(r32,31,29,v)
    #define   GET16Gbl_clkSelect_gfxClkPllSel(r16)             _BFGET_(r16,15,13)
    #define   SET16Gbl_clkSelect_gfxClkPllSel(r16,v)           _BFSET_(r16,15,13,v)

    #define     w32Gbl_clkSelect                               {\
            UNSG32 uclkSelect_sysClkPllSel                     :  3;\
            UNSG32 uclkSelect_sysClkSel                        :  3;\
            UNSG32 uclkSelect_cpu0ClkPllSel                    :  3;\
            UNSG32 uclkSelect_cpu0ClkSel                       :  3;\
            UNSG32 uclkSelect_cpu1ClkPllSel                    :  2;\
            UNSG32 uclkSelect_cpu1ClkSel                       :  3;\
            UNSG32 uclkSelect_drmFigoClkPllSel                 :  3;\
            UNSG32 uclkSelect_drmFigoClkSel                    :  3;\
            UNSG32 uclkSelect_cfgClkPllSel                     :  3;\
            UNSG32 uclkSelect_cfgClkSel                        :  3;\
            UNSG32 uclkSelect_gfxClkPllSel                     :  3;\
          }
    union { UNSG32 u32Gbl_clkSelect;
            struct w32Gbl_clkSelect;
          };
    #define   GET32Gbl_clkSelect_gfxClkSel(r32)                _BFGET_(r32, 2, 0)
    #define   SET32Gbl_clkSelect_gfxClkSel(r32,v)              _BFSET_(r32, 2, 0,v)
    #define   GET16Gbl_clkSelect_gfxClkSel(r16)                _BFGET_(r16, 2, 0)
    #define   SET16Gbl_clkSelect_gfxClkSel(r16,v)              _BFSET_(r16, 2, 0,v)

    #define   GET32Gbl_clkSelect_zspClkPllSel(r32)             _BFGET_(r32, 5, 3)
    #define   SET32Gbl_clkSelect_zspClkPllSel(r32,v)           _BFSET_(r32, 5, 3,v)
    #define   GET16Gbl_clkSelect_zspClkPllSel(r16)             _BFGET_(r16, 5, 3)
    #define   SET16Gbl_clkSelect_zspClkPllSel(r16,v)           _BFSET_(r16, 5, 3,v)

    #define   GET32Gbl_clkSelect_zspClkSel(r32)                _BFGET_(r32, 8, 6)
    #define   SET32Gbl_clkSelect_zspClkSel(r32,v)              _BFSET_(r32, 8, 6,v)
    #define   GET16Gbl_clkSelect_zspClkSel(r16)                _BFGET_(r16, 8, 6)
    #define   SET16Gbl_clkSelect_zspClkSel(r16,v)              _BFSET_(r16, 8, 6,v)

    #define   GET32Gbl_clkSelect_perifClkPllSel(r32)           _BFGET_(r32,11, 9)
    #define   SET32Gbl_clkSelect_perifClkPllSel(r32,v)         _BFSET_(r32,11, 9,v)
    #define   GET16Gbl_clkSelect_perifClkPllSel(r16)           _BFGET_(r16,11, 9)
    #define   SET16Gbl_clkSelect_perifClkPllSel(r16,v)         _BFSET_(r16,11, 9,v)

    #define   GET32Gbl_clkSelect_perifClkSel(r32)              _BFGET_(r32,14,12)
    #define   SET32Gbl_clkSelect_perifClkSel(r32,v)            _BFSET_(r32,14,12,v)
    #define   GET16Gbl_clkSelect_perifClkSel(r16)              _BFGET_(r16,14,12)
    #define   SET16Gbl_clkSelect_perifClkSel(r16,v)            _BFSET_(r16,14,12,v)

    #define   GET32Gbl_clkSelect_pCubeClkPllSel(r32)           _BFGET_(r32,17,15)
    #define   SET32Gbl_clkSelect_pCubeClkPllSel(r32,v)         _BFSET_(r32,17,15,v)

    #define   GET32Gbl_clkSelect_pCubeClkSel(r32)              _BFGET_(r32,20,18)
    #define   SET32Gbl_clkSelect_pCubeClkSel(r32,v)            _BFSET_(r32,20,18,v)
    #define   GET16Gbl_clkSelect_pCubeClkSel(r16)              _BFGET_(r16, 4, 2)
    #define   SET16Gbl_clkSelect_pCubeClkSel(r16,v)            _BFSET_(r16, 4, 2,v)

    #define   GET32Gbl_clkSelect_vScopeClkPllSel(r32)          _BFGET_(r32,23,21)
    #define   SET32Gbl_clkSelect_vScopeClkPllSel(r32,v)        _BFSET_(r32,23,21,v)
    #define   GET16Gbl_clkSelect_vScopeClkPllSel(r16)          _BFGET_(r16, 7, 5)
    #define   SET16Gbl_clkSelect_vScopeClkPllSel(r16,v)        _BFSET_(r16, 7, 5,v)

    #define   GET32Gbl_clkSelect_vScopeClkSel(r32)             _BFGET_(r32,26,24)
    #define   SET32Gbl_clkSelect_vScopeClkSel(r32,v)           _BFSET_(r32,26,24,v)
    #define   GET16Gbl_clkSelect_vScopeClkSel(r16)             _BFGET_(r16,10, 8)
    #define   SET16Gbl_clkSelect_vScopeClkSel(r16,v)           _BFSET_(r16,10, 8,v)

    #define   GET32Gbl_clkSelect_nfcEccClkPllSel(r32)          _BFGET_(r32,29,27)
    #define   SET32Gbl_clkSelect_nfcEccClkPllSel(r32,v)        _BFSET_(r32,29,27,v)
    #define   GET16Gbl_clkSelect_nfcEccClkPllSel(r16)          _BFGET_(r16,13,11)
    #define   SET16Gbl_clkSelect_nfcEccClkPllSel(r16,v)        _BFSET_(r16,13,11,v)

    #define     w32Gbl_clkSelect1                              {\
            UNSG32 uclkSelect_gfxClkSel                        :  3;\
            UNSG32 uclkSelect_zspClkPllSel                     :  3;\
            UNSG32 uclkSelect_zspClkSel                        :  3;\
            UNSG32 uclkSelect_perifClkPllSel                   :  3;\
            UNSG32 uclkSelect_perifClkSel                      :  3;\
            UNSG32 uclkSelect_pCubeClkPllSel                   :  3;\
            UNSG32 uclkSelect_pCubeClkSel                      :  3;\
            UNSG32 uclkSelect_vScopeClkPllSel                  :  3;\
            UNSG32 uclkSelect_vScopeClkSel                     :  3;\
            UNSG32 uclkSelect_nfcEccClkPllSel                  :  3;\
            UNSG32 RSVDx98_b30                                 :  2;\
          }
    union { UNSG32 u32Gbl_clkSelect1;
            struct w32Gbl_clkSelect1;
          };
    #define   GET32Gbl_clkSelect_nfcEccClkSel(r32)             _BFGET_(r32, 2, 0)
    #define   SET32Gbl_clkSelect_nfcEccClkSel(r32,v)           _BFSET_(r32, 2, 0,v)
    #define   GET16Gbl_clkSelect_nfcEccClkSel(r16)             _BFGET_(r16, 2, 0)
    #define   SET16Gbl_clkSelect_nfcEccClkSel(r16,v)           _BFSET_(r16, 2, 0,v)

    #define   GET32Gbl_clkSelect_vppSysClkPllSel(r32)          _BFGET_(r32, 5, 3)
    #define   SET32Gbl_clkSelect_vppSysClkPllSel(r32,v)        _BFSET_(r32, 5, 3,v)
    #define   GET16Gbl_clkSelect_vppSysClkPllSel(r16)          _BFGET_(r16, 5, 3)
    #define   SET16Gbl_clkSelect_vppSysClkPllSel(r16,v)        _BFSET_(r16, 5, 3,v)

    #define   GET32Gbl_clkSelect_vppSysClkSel(r32)             _BFGET_(r32, 8, 6)
    #define   SET32Gbl_clkSelect_vppSysClkSel(r32,v)           _BFSET_(r32, 8, 6,v)
    #define   GET16Gbl_clkSelect_vppSysClkSel(r16)             _BFGET_(r16, 8, 6)
    #define   SET16Gbl_clkSelect_vppSysClkSel(r16,v)           _BFSET_(r16, 8, 6,v)

    #define   GET32Gbl_clkSelect_appClkPllSel(r32)             _BFGET_(r32,11, 9)
    #define   SET32Gbl_clkSelect_appClkPllSel(r32,v)           _BFSET_(r32,11, 9,v)
    #define   GET16Gbl_clkSelect_appClkPllSel(r16)             _BFGET_(r16,11, 9)
    #define   SET16Gbl_clkSelect_appClkPllSel(r16,v)           _BFSET_(r16,11, 9,v)

    #define   GET32Gbl_clkSelect_appClkSel(r32)                _BFGET_(r32,14,12)
    #define   SET32Gbl_clkSelect_appClkSel(r32,v)              _BFSET_(r32,14,12,v)
    #define   GET16Gbl_clkSelect_appClkSel(r16)                _BFGET_(r16,14,12)
    #define   SET16Gbl_clkSelect_appClkSel(r16,v)              _BFSET_(r16,14,12,v)

    #define   GET32Gbl_clkSelect_audioFastExtClkSel(r32)       _BFGET_(r32,15,15)
    #define   SET32Gbl_clkSelect_audioFastExtClkSel(r32,v)     _BFSET_(r32,15,15,v)
    #define   GET16Gbl_clkSelect_audioFastExtClkSel(r16)       _BFGET_(r16,15,15)
    #define   SET16Gbl_clkSelect_audioFastExtClkSel(r16,v)     _BFSET_(r16,15,15,v)

    #define   GET32Gbl_clkSelect_audioFastClkSel(r32)          _BFGET_(r32,16,16)
    #define   SET32Gbl_clkSelect_audioFastClkSel(r32,v)        _BFSET_(r32,16,16,v)
    #define   GET16Gbl_clkSelect_audioFastClkSel(r16)          _BFGET_(r16, 0, 0)
    #define   SET16Gbl_clkSelect_audioFastClkSel(r16,v)        _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_clkSelect_audio0ClkSel(r32)             _BFGET_(r32,19,17)
    #define   SET32Gbl_clkSelect_audio0ClkSel(r32,v)           _BFSET_(r32,19,17,v)
    #define   GET16Gbl_clkSelect_audio0ClkSel(r16)             _BFGET_(r16, 3, 1)
    #define   SET16Gbl_clkSelect_audio0ClkSel(r16,v)           _BFSET_(r16, 3, 1,v)

    #define   GET32Gbl_clkSelect_audio2ClkSel(r32)             _BFGET_(r32,22,20)
    #define   SET32Gbl_clkSelect_audio2ClkSel(r32,v)           _BFSET_(r32,22,20,v)
    #define   GET16Gbl_clkSelect_audio2ClkSel(r16)             _BFGET_(r16, 6, 4)
    #define   SET16Gbl_clkSelect_audio2ClkSel(r16,v)           _BFSET_(r16, 6, 4,v)

    #define   GET32Gbl_clkSelect_audio3ClkSel(r32)             _BFGET_(r32,25,23)
    #define   SET32Gbl_clkSelect_audio3ClkSel(r32,v)           _BFSET_(r32,25,23,v)
    #define   GET16Gbl_clkSelect_audio3ClkSel(r16)             _BFGET_(r16, 9, 7)
    #define   SET16Gbl_clkSelect_audio3ClkSel(r16,v)           _BFSET_(r16, 9, 7,v)

    #define   GET32Gbl_clkSelect_audioHdExtClkSel(r32)         _BFGET_(r32,26,26)
    #define   SET32Gbl_clkSelect_audioHdExtClkSel(r32,v)       _BFSET_(r32,26,26,v)
    #define   GET16Gbl_clkSelect_audioHdExtClkSel(r16)         _BFGET_(r16,10,10)
    #define   SET16Gbl_clkSelect_audioHdExtClkSel(r16,v)       _BFSET_(r16,10,10,v)

    #define   GET32Gbl_clkSelect_audioHdClkSel(r32)            _BFGET_(r32,27,27)
    #define   SET32Gbl_clkSelect_audioHdClkSel(r32,v)          _BFSET_(r32,27,27,v)
    #define   GET16Gbl_clkSelect_audioHdClkSel(r16)            _BFGET_(r16,11,11)
    #define   SET16Gbl_clkSelect_audioHdClkSel(r16,v)          _BFSET_(r16,11,11,v)

    #define   GET32Gbl_clkSelect_audio1ExtClkSel(r32)          _BFGET_(r32,28,28)
    #define   SET32Gbl_clkSelect_audio1ExtClkSel(r32,v)        _BFSET_(r32,28,28,v)
    #define   GET16Gbl_clkSelect_audio1ExtClkSel(r16)          _BFGET_(r16,12,12)
    #define   SET16Gbl_clkSelect_audio1ExtClkSel(r16,v)        _BFSET_(r16,12,12,v)

    #define   GET32Gbl_clkSelect_audio1ClkPllSel(r32)          _BFGET_(r32,29,29)
    #define   SET32Gbl_clkSelect_audio1ClkPllSel(r32,v)        _BFSET_(r32,29,29,v)
    #define   GET16Gbl_clkSelect_audio1ClkPllSel(r16)          _BFGET_(r16,13,13)
    #define   SET16Gbl_clkSelect_audio1ClkPllSel(r16,v)        _BFSET_(r16,13,13,v)

    #define   GET32Gbl_clkSelect_audio1SrcClkSel(r32)          _BFGET_(r32,30,30)
    #define   SET32Gbl_clkSelect_audio1SrcClkSel(r32,v)        _BFSET_(r32,30,30,v)
    #define   GET16Gbl_clkSelect_audio1SrcClkSel(r16)          _BFGET_(r16,14,14)
    #define   SET16Gbl_clkSelect_audio1SrcClkSel(r16,v)        _BFSET_(r16,14,14,v)

    #define     w32Gbl_clkSelect2                              {\
            UNSG32 uclkSelect_nfcEccClkSel                     :  3;\
            UNSG32 uclkSelect_vppSysClkPllSel                  :  3;\
            UNSG32 uclkSelect_vppSysClkSel                     :  3;\
            UNSG32 uclkSelect_appClkPllSel                     :  3;\
            UNSG32 uclkSelect_appClkSel                        :  3;\
            UNSG32 uclkSelect_audioFastExtClkSel               :  1;\
            UNSG32 uclkSelect_audioFastClkSel                  :  1;\
            UNSG32 uclkSelect_audio0ClkSel                     :  3;\
            UNSG32 uclkSelect_audio2ClkSel                     :  3;\
            UNSG32 uclkSelect_audio3ClkSel                     :  3;\
            UNSG32 uclkSelect_audioHdExtClkSel                 :  1;\
            UNSG32 uclkSelect_audioHdClkSel                    :  1;\
            UNSG32 uclkSelect_audio1ExtClkSel                  :  1;\
            UNSG32 uclkSelect_audio1ClkPllSel                  :  1;\
            UNSG32 uclkSelect_audio1SrcClkSel                  :  1;\
            UNSG32 RSVDx9C_b31                                 :  1;\
          }
    union { UNSG32 u32Gbl_clkSelect2;
            struct w32Gbl_clkSelect2;
          };
    #define   GET32Gbl_clkSelect_audio1ClkSel(r32)             _BFGET_(r32, 2, 0)
    #define   SET32Gbl_clkSelect_audio1ClkSel(r32,v)           _BFSET_(r32, 2, 0,v)
    #define   GET16Gbl_clkSelect_audio1ClkSel(r16)             _BFGET_(r16, 2, 0)
    #define   SET16Gbl_clkSelect_audio1ClkSel(r16,v)           _BFSET_(r16, 2, 0,v)

    #define   GET32Gbl_clkSelect_video0ExtClkSel(r32)          _BFGET_(r32, 3, 3)
    #define   SET32Gbl_clkSelect_video0ExtClkSel(r32,v)        _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_clkSelect_video0ExtClkSel(r16)          _BFGET_(r16, 3, 3)
    #define   SET16Gbl_clkSelect_video0ExtClkSel(r16,v)        _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_clkSelect_video0ClkSel(r32)             _BFGET_(r32, 4, 4)
    #define   SET32Gbl_clkSelect_video0ClkSel(r32,v)           _BFSET_(r32, 4, 4,v)
    #define   GET16Gbl_clkSelect_video0ClkSel(r16)             _BFGET_(r16, 4, 4)
    #define   SET16Gbl_clkSelect_video0ClkSel(r16,v)           _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_clkSelect_video1ExtClkSel(r32)          _BFGET_(r32, 5, 5)
    #define   SET32Gbl_clkSelect_video1ExtClkSel(r32,v)        _BFSET_(r32, 5, 5,v)
    #define   GET16Gbl_clkSelect_video1ExtClkSel(r16)          _BFGET_(r16, 5, 5)
    #define   SET16Gbl_clkSelect_video1ExtClkSel(r16,v)        _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_clkSelect_video1ClkSel(r32)             _BFGET_(r32, 6, 6)
    #define   SET32Gbl_clkSelect_video1ClkSel(r32,v)           _BFSET_(r32, 6, 6,v)
    #define   GET16Gbl_clkSelect_video1ClkSel(r16)             _BFGET_(r16, 6, 6)
    #define   SET16Gbl_clkSelect_video1ClkSel(r16,v)           _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_clkSelect_video1ClkPllSel(r32)          _BFGET_(r32, 7, 7)
    #define   SET32Gbl_clkSelect_video1ClkPllSel(r32,v)        _BFSET_(r32, 7, 7,v)
    #define   GET16Gbl_clkSelect_video1ClkPllSel(r16)          _BFGET_(r16, 7, 7)
    #define   SET16Gbl_clkSelect_video1ClkPllSel(r16,v)        _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_clkSelect_video2ExtClkSel(r32)          _BFGET_(r32, 8, 8)
    #define   SET32Gbl_clkSelect_video2ExtClkSel(r32,v)        _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_clkSelect_video2ExtClkSel(r16)          _BFGET_(r16, 8, 8)
    #define   SET16Gbl_clkSelect_video2ExtClkSel(r16,v)        _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_clkSelect_video2ClkSel(r32)             _BFGET_(r32, 9, 9)
    #define   SET32Gbl_clkSelect_video2ClkSel(r32,v)           _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_clkSelect_video2ClkSel(r16)             _BFGET_(r16, 9, 9)
    #define   SET16Gbl_clkSelect_video2ClkSel(r16,v)           _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_clkSelect_video2ClkPllSel(r32)          _BFGET_(r32,10,10)
    #define   SET32Gbl_clkSelect_video2ClkPllSel(r32,v)        _BFSET_(r32,10,10,v)
    #define   GET16Gbl_clkSelect_video2ClkPllSel(r16)          _BFGET_(r16,10,10)
    #define   SET16Gbl_clkSelect_video2ClkPllSel(r16,v)        _BFSET_(r16,10,10,v)

    #define     w32Gbl_clkSelect3                              {\
            UNSG32 uclkSelect_audio1ClkSel                     :  3;\
            UNSG32 uclkSelect_video0ExtClkSel                  :  1;\
            UNSG32 uclkSelect_video0ClkSel                     :  1;\
            UNSG32 uclkSelect_video1ExtClkSel                  :  1;\
            UNSG32 uclkSelect_video1ClkSel                     :  1;\
            UNSG32 uclkSelect_video1ClkPllSel                  :  1;\
            UNSG32 uclkSelect_video2ExtClkSel                  :  1;\
            UNSG32 uclkSelect_video2ClkSel                     :  1;\
            UNSG32 uclkSelect_video2ClkPllSel                  :  1;\
            UNSG32 RSVDxA0_b11                                 : 21;\
          }
    union { UNSG32 u32Gbl_clkSelect3;
            struct w32Gbl_clkSelect3;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ClkSwitch_sysPLLSWBypass(r32)           _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ClkSwitch_sysPLLSWBypass(r32,v)         _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ClkSwitch_sysPLLSWBypass(r16)           _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ClkSwitch_sysPLLSWBypass(r16,v)         _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ClkSwitch_memPLLSWBypass(r32)           _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ClkSwitch_memPLLSWBypass(r32,v)         _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ClkSwitch_memPLLSWBypass(r16)           _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ClkSwitch_memPLLSWBypass(r16,v)         _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ClkSwitch_cpu1PLLSWBypass(r32)          _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ClkSwitch_cpu1PLLSWBypass(r32,v)        _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ClkSwitch_cpu1PLLSWBypass(r16)          _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ClkSwitch_cpu1PLLSWBypass(r16,v)        _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_ClkSwitch_sysClkPllSwitch(r32)          _BFGET_(r32, 3, 3)
    #define   SET32Gbl_ClkSwitch_sysClkPllSwitch(r32,v)        _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_ClkSwitch_sysClkPllSwitch(r16)          _BFGET_(r16, 3, 3)
    #define   SET16Gbl_ClkSwitch_sysClkPllSwitch(r16,v)        _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_ClkSwitch_sysClkSwitch(r32)             _BFGET_(r32, 4, 4)
    #define   SET32Gbl_ClkSwitch_sysClkSwitch(r32,v)           _BFSET_(r32, 4, 4,v)
    #define   GET16Gbl_ClkSwitch_sysClkSwitch(r16)             _BFGET_(r16, 4, 4)
    #define   SET16Gbl_ClkSwitch_sysClkSwitch(r16,v)           _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_ClkSwitch_sysClkD3Switch(r32)           _BFGET_(r32, 5, 5)
    #define   SET32Gbl_ClkSwitch_sysClkD3Switch(r32,v)         _BFSET_(r32, 5, 5,v)
    #define   GET16Gbl_ClkSwitch_sysClkD3Switch(r16)           _BFGET_(r16, 5, 5)
    #define   SET16Gbl_ClkSwitch_sysClkD3Switch(r16,v)         _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_ClkSwitch_cpu0ClkPllSwitch(r32)         _BFGET_(r32, 6, 6)
    #define   SET32Gbl_ClkSwitch_cpu0ClkPllSwitch(r32,v)       _BFSET_(r32, 6, 6,v)
    #define   GET16Gbl_ClkSwitch_cpu0ClkPllSwitch(r16)         _BFGET_(r16, 6, 6)
    #define   SET16Gbl_ClkSwitch_cpu0ClkPllSwitch(r16,v)       _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_ClkSwitch_cpu0ClkSwitch(r32)            _BFGET_(r32, 7, 7)
    #define   SET32Gbl_ClkSwitch_cpu0ClkSwitch(r32,v)          _BFSET_(r32, 7, 7,v)
    #define   GET16Gbl_ClkSwitch_cpu0ClkSwitch(r16)            _BFGET_(r16, 7, 7)
    #define   SET16Gbl_ClkSwitch_cpu0ClkSwitch(r16,v)          _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_ClkSwitch_cpu0ClkD3Switch(r32)          _BFGET_(r32, 8, 8)
    #define   SET32Gbl_ClkSwitch_cpu0ClkD3Switch(r32,v)        _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_ClkSwitch_cpu0ClkD3Switch(r16)          _BFGET_(r16, 8, 8)
    #define   SET16Gbl_ClkSwitch_cpu0ClkD3Switch(r16,v)        _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_ClkSwitch_cpu1ClkPllSwitch(r32)         _BFGET_(r32, 9, 9)
    #define   SET32Gbl_ClkSwitch_cpu1ClkPllSwitch(r32,v)       _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_ClkSwitch_cpu1ClkPllSwitch(r16)         _BFGET_(r16, 9, 9)
    #define   SET16Gbl_ClkSwitch_cpu1ClkPllSwitch(r16,v)       _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_ClkSwitch_cpu1ClkSwitch(r32)            _BFGET_(r32,10,10)
    #define   SET32Gbl_ClkSwitch_cpu1ClkSwitch(r32,v)          _BFSET_(r32,10,10,v)
    #define   GET16Gbl_ClkSwitch_cpu1ClkSwitch(r16)            _BFGET_(r16,10,10)
    #define   SET16Gbl_ClkSwitch_cpu1ClkSwitch(r16,v)          _BFSET_(r16,10,10,v)

    #define   GET32Gbl_ClkSwitch_cpu1ClkD3Switch(r32)          _BFGET_(r32,11,11)
    #define   SET32Gbl_ClkSwitch_cpu1ClkD3Switch(r32,v)        _BFSET_(r32,11,11,v)
    #define   GET16Gbl_ClkSwitch_cpu1ClkD3Switch(r16)          _BFGET_(r16,11,11)
    #define   SET16Gbl_ClkSwitch_cpu1ClkD3Switch(r16,v)        _BFSET_(r16,11,11,v)

    #define   GET32Gbl_ClkSwitch_drmFigoClkPllSwitch(r32)      _BFGET_(r32,12,12)
    #define   SET32Gbl_ClkSwitch_drmFigoClkPllSwitch(r32,v)    _BFSET_(r32,12,12,v)
    #define   GET16Gbl_ClkSwitch_drmFigoClkPllSwitch(r16)      _BFGET_(r16,12,12)
    #define   SET16Gbl_ClkSwitch_drmFigoClkPllSwitch(r16,v)    _BFSET_(r16,12,12,v)

    #define   GET32Gbl_ClkSwitch_drmFigoClkSwitch(r32)         _BFGET_(r32,13,13)
    #define   SET32Gbl_ClkSwitch_drmFigoClkSwitch(r32,v)       _BFSET_(r32,13,13,v)
    #define   GET16Gbl_ClkSwitch_drmFigoClkSwitch(r16)         _BFGET_(r16,13,13)
    #define   SET16Gbl_ClkSwitch_drmFigoClkSwitch(r16,v)       _BFSET_(r16,13,13,v)

    #define   GET32Gbl_ClkSwitch_drmFigoClkD3Switch(r32)       _BFGET_(r32,14,14)
    #define   SET32Gbl_ClkSwitch_drmFigoClkD3Switch(r32,v)     _BFSET_(r32,14,14,v)
    #define   GET16Gbl_ClkSwitch_drmFigoClkD3Switch(r16)       _BFGET_(r16,14,14)
    #define   SET16Gbl_ClkSwitch_drmFigoClkD3Switch(r16,v)     _BFSET_(r16,14,14,v)

    #define   GET32Gbl_ClkSwitch_cfgClkPllSwitch(r32)          _BFGET_(r32,15,15)
    #define   SET32Gbl_ClkSwitch_cfgClkPllSwitch(r32,v)        _BFSET_(r32,15,15,v)
    #define   GET16Gbl_ClkSwitch_cfgClkPllSwitch(r16)          _BFGET_(r16,15,15)
    #define   SET16Gbl_ClkSwitch_cfgClkPllSwitch(r16,v)        _BFSET_(r16,15,15,v)

    #define   GET32Gbl_ClkSwitch_cfgClkSwitch(r32)             _BFGET_(r32,16,16)
    #define   SET32Gbl_ClkSwitch_cfgClkSwitch(r32,v)           _BFSET_(r32,16,16,v)
    #define   GET16Gbl_ClkSwitch_cfgClkSwitch(r16)             _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ClkSwitch_cfgClkSwitch(r16,v)           _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ClkSwitch_cfgClkD3Switch(r32)           _BFGET_(r32,17,17)
    #define   SET32Gbl_ClkSwitch_cfgClkD3Switch(r32,v)         _BFSET_(r32,17,17,v)
    #define   GET16Gbl_ClkSwitch_cfgClkD3Switch(r16)           _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ClkSwitch_cfgClkD3Switch(r16,v)         _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ClkSwitch_gfxClkPllSwitch(r32)          _BFGET_(r32,18,18)
    #define   SET32Gbl_ClkSwitch_gfxClkPllSwitch(r32,v)        _BFSET_(r32,18,18,v)
    #define   GET16Gbl_ClkSwitch_gfxClkPllSwitch(r16)          _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ClkSwitch_gfxClkPllSwitch(r16,v)        _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_ClkSwitch_gfxClkSwitch(r32)             _BFGET_(r32,19,19)
    #define   SET32Gbl_ClkSwitch_gfxClkSwitch(r32,v)           _BFSET_(r32,19,19,v)
    #define   GET16Gbl_ClkSwitch_gfxClkSwitch(r16)             _BFGET_(r16, 3, 3)
    #define   SET16Gbl_ClkSwitch_gfxClkSwitch(r16,v)           _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_ClkSwitch_gfxClkD3Switch(r32)           _BFGET_(r32,20,20)
    #define   SET32Gbl_ClkSwitch_gfxClkD3Switch(r32,v)         _BFSET_(r32,20,20,v)
    #define   GET16Gbl_ClkSwitch_gfxClkD3Switch(r16)           _BFGET_(r16, 4, 4)
    #define   SET16Gbl_ClkSwitch_gfxClkD3Switch(r16,v)         _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_ClkSwitch_zspClkPllSwitch(r32)          _BFGET_(r32,21,21)
    #define   SET32Gbl_ClkSwitch_zspClkPllSwitch(r32,v)        _BFSET_(r32,21,21,v)
    #define   GET16Gbl_ClkSwitch_zspClkPllSwitch(r16)          _BFGET_(r16, 5, 5)
    #define   SET16Gbl_ClkSwitch_zspClkPllSwitch(r16,v)        _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_ClkSwitch_zspClkSwitch(r32)             _BFGET_(r32,22,22)
    #define   SET32Gbl_ClkSwitch_zspClkSwitch(r32,v)           _BFSET_(r32,22,22,v)
    #define   GET16Gbl_ClkSwitch_zspClkSwitch(r16)             _BFGET_(r16, 6, 6)
    #define   SET16Gbl_ClkSwitch_zspClkSwitch(r16,v)           _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_ClkSwitch_zspClkD3Switch(r32)           _BFGET_(r32,23,23)
    #define   SET32Gbl_ClkSwitch_zspClkD3Switch(r32,v)         _BFSET_(r32,23,23,v)
    #define   GET16Gbl_ClkSwitch_zspClkD3Switch(r16)           _BFGET_(r16, 7, 7)
    #define   SET16Gbl_ClkSwitch_zspClkD3Switch(r16,v)         _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_ClkSwitch_perifClkPllSwitch(r32)        _BFGET_(r32,24,24)
    #define   SET32Gbl_ClkSwitch_perifClkPllSwitch(r32,v)      _BFSET_(r32,24,24,v)
    #define   GET16Gbl_ClkSwitch_perifClkPllSwitch(r16)        _BFGET_(r16, 8, 8)
    #define   SET16Gbl_ClkSwitch_perifClkPllSwitch(r16,v)      _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_ClkSwitch_perifClkSwitch(r32)           _BFGET_(r32,25,25)
    #define   SET32Gbl_ClkSwitch_perifClkSwitch(r32,v)         _BFSET_(r32,25,25,v)
    #define   GET16Gbl_ClkSwitch_perifClkSwitch(r16)           _BFGET_(r16, 9, 9)
    #define   SET16Gbl_ClkSwitch_perifClkSwitch(r16,v)         _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_ClkSwitch_perifClkD3Switch(r32)         _BFGET_(r32,26,26)
    #define   SET32Gbl_ClkSwitch_perifClkD3Switch(r32,v)       _BFSET_(r32,26,26,v)
    #define   GET16Gbl_ClkSwitch_perifClkD3Switch(r16)         _BFGET_(r16,10,10)
    #define   SET16Gbl_ClkSwitch_perifClkD3Switch(r16,v)       _BFSET_(r16,10,10,v)

    #define   GET32Gbl_ClkSwitch_pCubeClkPllSwitch(r32)        _BFGET_(r32,27,27)
    #define   SET32Gbl_ClkSwitch_pCubeClkPllSwitch(r32,v)      _BFSET_(r32,27,27,v)
    #define   GET16Gbl_ClkSwitch_pCubeClkPllSwitch(r16)        _BFGET_(r16,11,11)
    #define   SET16Gbl_ClkSwitch_pCubeClkPllSwitch(r16,v)      _BFSET_(r16,11,11,v)

    #define   GET32Gbl_ClkSwitch_pCubeClkSwitch(r32)           _BFGET_(r32,28,28)
    #define   SET32Gbl_ClkSwitch_pCubeClkSwitch(r32,v)         _BFSET_(r32,28,28,v)
    #define   GET16Gbl_ClkSwitch_pCubeClkSwitch(r16)           _BFGET_(r16,12,12)
    #define   SET16Gbl_ClkSwitch_pCubeClkSwitch(r16,v)         _BFSET_(r16,12,12,v)

    #define   GET32Gbl_ClkSwitch_pCubeClkD3Switch(r32)         _BFGET_(r32,29,29)
    #define   SET32Gbl_ClkSwitch_pCubeClkD3Switch(r32,v)       _BFSET_(r32,29,29,v)
    #define   GET16Gbl_ClkSwitch_pCubeClkD3Switch(r16)         _BFGET_(r16,13,13)
    #define   SET16Gbl_ClkSwitch_pCubeClkD3Switch(r16,v)       _BFSET_(r16,13,13,v)

    #define   GET32Gbl_ClkSwitch_vScopeClkPllSwitch(r32)       _BFGET_(r32,30,30)
    #define   SET32Gbl_ClkSwitch_vScopeClkPllSwitch(r32,v)     _BFSET_(r32,30,30,v)
    #define   GET16Gbl_ClkSwitch_vScopeClkPllSwitch(r16)       _BFGET_(r16,14,14)
    #define   SET16Gbl_ClkSwitch_vScopeClkPllSwitch(r16,v)     _BFSET_(r16,14,14,v)

    #define   GET32Gbl_ClkSwitch_vScopeClkSwitch(r32)          _BFGET_(r32,31,31)
    #define   SET32Gbl_ClkSwitch_vScopeClkSwitch(r32,v)        _BFSET_(r32,31,31,v)
    #define   GET16Gbl_ClkSwitch_vScopeClkSwitch(r16)          _BFGET_(r16,15,15)
    #define   SET16Gbl_ClkSwitch_vScopeClkSwitch(r16,v)        _BFSET_(r16,15,15,v)

    #define     w32Gbl_ClkSwitch                               {\
            UNSG32 uClkSwitch_sysPLLSWBypass                   :  1;\
            UNSG32 uClkSwitch_memPLLSWBypass                   :  1;\
            UNSG32 uClkSwitch_cpu1PLLSWBypass                  :  1;\
            UNSG32 uClkSwitch_sysClkPllSwitch                  :  1;\
            UNSG32 uClkSwitch_sysClkSwitch                     :  1;\
            UNSG32 uClkSwitch_sysClkD3Switch                   :  1;\
            UNSG32 uClkSwitch_cpu0ClkPllSwitch                 :  1;\
            UNSG32 uClkSwitch_cpu0ClkSwitch                    :  1;\
            UNSG32 uClkSwitch_cpu0ClkD3Switch                  :  1;\
            UNSG32 uClkSwitch_cpu1ClkPllSwitch                 :  1;\
            UNSG32 uClkSwitch_cpu1ClkSwitch                    :  1;\
            UNSG32 uClkSwitch_cpu1ClkD3Switch                  :  1;\
            UNSG32 uClkSwitch_drmFigoClkPllSwitch              :  1;\
            UNSG32 uClkSwitch_drmFigoClkSwitch                 :  1;\
            UNSG32 uClkSwitch_drmFigoClkD3Switch               :  1;\
            UNSG32 uClkSwitch_cfgClkPllSwitch                  :  1;\
            UNSG32 uClkSwitch_cfgClkSwitch                     :  1;\
            UNSG32 uClkSwitch_cfgClkD3Switch                   :  1;\
            UNSG32 uClkSwitch_gfxClkPllSwitch                  :  1;\
            UNSG32 uClkSwitch_gfxClkSwitch                     :  1;\
            UNSG32 uClkSwitch_gfxClkD3Switch                   :  1;\
            UNSG32 uClkSwitch_zspClkPllSwitch                  :  1;\
            UNSG32 uClkSwitch_zspClkSwitch                     :  1;\
            UNSG32 uClkSwitch_zspClkD3Switch                   :  1;\
            UNSG32 uClkSwitch_perifClkPllSwitch                :  1;\
            UNSG32 uClkSwitch_perifClkSwitch                   :  1;\
            UNSG32 uClkSwitch_perifClkD3Switch                 :  1;\
            UNSG32 uClkSwitch_pCubeClkPllSwitch                :  1;\
            UNSG32 uClkSwitch_pCubeClkSwitch                   :  1;\
            UNSG32 uClkSwitch_pCubeClkD3Switch                 :  1;\
            UNSG32 uClkSwitch_vScopeClkPllSwitch               :  1;\
            UNSG32 uClkSwitch_vScopeClkSwitch                  :  1;\
          }
    union { UNSG32 u32Gbl_ClkSwitch;
            struct w32Gbl_ClkSwitch;
          };
    #define   GET32Gbl_ClkSwitch_vScopeClkD3Switch(r32)        _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ClkSwitch_vScopeClkD3Switch(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ClkSwitch_vScopeClkD3Switch(r16)        _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ClkSwitch_vScopeClkD3Switch(r16,v)      _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ClkSwitch_nfcEccClkPllSwitch(r32)       _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ClkSwitch_nfcEccClkPllSwitch(r32,v)     _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ClkSwitch_nfcEccClkPllSwitch(r16)       _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ClkSwitch_nfcEccClkPllSwitch(r16,v)     _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ClkSwitch_nfcEccClkSwitch(r32)          _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ClkSwitch_nfcEccClkSwitch(r32,v)        _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ClkSwitch_nfcEccClkSwitch(r16)          _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ClkSwitch_nfcEccClkSwitch(r16,v)        _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_ClkSwitch_nfcEccClkD3Switch(r32)        _BFGET_(r32, 3, 3)
    #define   SET32Gbl_ClkSwitch_nfcEccClkD3Switch(r32,v)      _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_ClkSwitch_nfcEccClkD3Switch(r16)        _BFGET_(r16, 3, 3)
    #define   SET16Gbl_ClkSwitch_nfcEccClkD3Switch(r16,v)      _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_ClkSwitch_vppSysClkPllSwitch(r32)       _BFGET_(r32, 4, 4)
    #define   SET32Gbl_ClkSwitch_vppSysClkPllSwitch(r32,v)     _BFSET_(r32, 4, 4,v)
    #define   GET16Gbl_ClkSwitch_vppSysClkPllSwitch(r16)       _BFGET_(r16, 4, 4)
    #define   SET16Gbl_ClkSwitch_vppSysClkPllSwitch(r16,v)     _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_ClkSwitch_vppSysClkSwitch(r32)          _BFGET_(r32, 5, 5)
    #define   SET32Gbl_ClkSwitch_vppSysClkSwitch(r32,v)        _BFSET_(r32, 5, 5,v)
    #define   GET16Gbl_ClkSwitch_vppSysClkSwitch(r16)          _BFGET_(r16, 5, 5)
    #define   SET16Gbl_ClkSwitch_vppSysClkSwitch(r16,v)        _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_ClkSwitch_vppSysClkD3Switch(r32)        _BFGET_(r32, 6, 6)
    #define   SET32Gbl_ClkSwitch_vppSysClkD3Switch(r32,v)      _BFSET_(r32, 6, 6,v)
    #define   GET16Gbl_ClkSwitch_vppSysClkD3Switch(r16)        _BFGET_(r16, 6, 6)
    #define   SET16Gbl_ClkSwitch_vppSysClkD3Switch(r16,v)      _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_ClkSwitch_appClkPllSwitch(r32)          _BFGET_(r32, 7, 7)
    #define   SET32Gbl_ClkSwitch_appClkPllSwitch(r32,v)        _BFSET_(r32, 7, 7,v)
    #define   GET16Gbl_ClkSwitch_appClkPllSwitch(r16)          _BFGET_(r16, 7, 7)
    #define   SET16Gbl_ClkSwitch_appClkPllSwitch(r16,v)        _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_ClkSwitch_appClkSwitch(r32)             _BFGET_(r32, 8, 8)
    #define   SET32Gbl_ClkSwitch_appClkSwitch(r32,v)           _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_ClkSwitch_appClkSwitch(r16)             _BFGET_(r16, 8, 8)
    #define   SET16Gbl_ClkSwitch_appClkSwitch(r16,v)           _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_ClkSwitch_appClkD3Switch(r32)           _BFGET_(r32, 9, 9)
    #define   SET32Gbl_ClkSwitch_appClkD3Switch(r32,v)         _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_ClkSwitch_appClkD3Switch(r16)           _BFGET_(r16, 9, 9)
    #define   SET16Gbl_ClkSwitch_appClkD3Switch(r16,v)         _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_ClkSwitch_audio0ClkSwitch(r32)          _BFGET_(r32,10,10)
    #define   SET32Gbl_ClkSwitch_audio0ClkSwitch(r32,v)        _BFSET_(r32,10,10,v)
    #define   GET16Gbl_ClkSwitch_audio0ClkSwitch(r16)          _BFGET_(r16,10,10)
    #define   SET16Gbl_ClkSwitch_audio0ClkSwitch(r16,v)        _BFSET_(r16,10,10,v)

    #define   GET32Gbl_ClkSwitch_audio0ClkD3Switch(r32)        _BFGET_(r32,11,11)
    #define   SET32Gbl_ClkSwitch_audio0ClkD3Switch(r32,v)      _BFSET_(r32,11,11,v)
    #define   GET16Gbl_ClkSwitch_audio0ClkD3Switch(r16)        _BFGET_(r16,11,11)
    #define   SET16Gbl_ClkSwitch_audio0ClkD3Switch(r16,v)      _BFSET_(r16,11,11,v)

    #define   GET32Gbl_ClkSwitch_audio1ClkSwitch(r32)          _BFGET_(r32,12,12)
    #define   SET32Gbl_ClkSwitch_audio1ClkSwitch(r32,v)        _BFSET_(r32,12,12,v)
    #define   GET16Gbl_ClkSwitch_audio1ClkSwitch(r16)          _BFGET_(r16,12,12)
    #define   SET16Gbl_ClkSwitch_audio1ClkSwitch(r16,v)        _BFSET_(r16,12,12,v)

    #define   GET32Gbl_ClkSwitch_audio1ClkD3Switch(r32)        _BFGET_(r32,13,13)
    #define   SET32Gbl_ClkSwitch_audio1ClkD3Switch(r32,v)      _BFSET_(r32,13,13,v)
    #define   GET16Gbl_ClkSwitch_audio1ClkD3Switch(r16)        _BFGET_(r16,13,13)
    #define   SET16Gbl_ClkSwitch_audio1ClkD3Switch(r16,v)      _BFSET_(r16,13,13,v)

    #define   GET32Gbl_ClkSwitch_audio2ClkSwitch(r32)          _BFGET_(r32,14,14)
    #define   SET32Gbl_ClkSwitch_audio2ClkSwitch(r32,v)        _BFSET_(r32,14,14,v)
    #define   GET16Gbl_ClkSwitch_audio2ClkSwitch(r16)          _BFGET_(r16,14,14)
    #define   SET16Gbl_ClkSwitch_audio2ClkSwitch(r16,v)        _BFSET_(r16,14,14,v)

    #define   GET32Gbl_ClkSwitch_audio2ClkD3Switch(r32)        _BFGET_(r32,15,15)
    #define   SET32Gbl_ClkSwitch_audio2ClkD3Switch(r32,v)      _BFSET_(r32,15,15,v)
    #define   GET16Gbl_ClkSwitch_audio2ClkD3Switch(r16)        _BFGET_(r16,15,15)
    #define   SET16Gbl_ClkSwitch_audio2ClkD3Switch(r16,v)      _BFSET_(r16,15,15,v)

    #define   GET32Gbl_ClkSwitch_audio3ClkSwitch(r32)          _BFGET_(r32,16,16)
    #define   SET32Gbl_ClkSwitch_audio3ClkSwitch(r32,v)        _BFSET_(r32,16,16,v)
    #define   GET16Gbl_ClkSwitch_audio3ClkSwitch(r16)          _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ClkSwitch_audio3ClkSwitch(r16,v)        _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ClkSwitch_audio3ClkD3Switch(r32)        _BFGET_(r32,17,17)
    #define   SET32Gbl_ClkSwitch_audio3ClkD3Switch(r32,v)      _BFSET_(r32,17,17,v)
    #define   GET16Gbl_ClkSwitch_audio3ClkD3Switch(r16)        _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ClkSwitch_audio3ClkD3Switch(r16,v)      _BFSET_(r16, 1, 1,v)

    #define     w32Gbl_ClkSwitch1                              {\
            UNSG32 uClkSwitch_vScopeClkD3Switch                :  1;\
            UNSG32 uClkSwitch_nfcEccClkPllSwitch               :  1;\
            UNSG32 uClkSwitch_nfcEccClkSwitch                  :  1;\
            UNSG32 uClkSwitch_nfcEccClkD3Switch                :  1;\
            UNSG32 uClkSwitch_vppSysClkPllSwitch               :  1;\
            UNSG32 uClkSwitch_vppSysClkSwitch                  :  1;\
            UNSG32 uClkSwitch_vppSysClkD3Switch                :  1;\
            UNSG32 uClkSwitch_appClkPllSwitch                  :  1;\
            UNSG32 uClkSwitch_appClkSwitch                     :  1;\
            UNSG32 uClkSwitch_appClkD3Switch                   :  1;\
            UNSG32 uClkSwitch_audio0ClkSwitch                  :  1;\
            UNSG32 uClkSwitch_audio0ClkD3Switch                :  1;\
            UNSG32 uClkSwitch_audio1ClkSwitch                  :  1;\
            UNSG32 uClkSwitch_audio1ClkD3Switch                :  1;\
            UNSG32 uClkSwitch_audio2ClkSwitch                  :  1;\
            UNSG32 uClkSwitch_audio2ClkD3Switch                :  1;\
            UNSG32 uClkSwitch_audio3ClkSwitch                  :  1;\
            UNSG32 uClkSwitch_audio3ClkD3Switch                :  1;\
            UNSG32 RSVDxA8_b18                                 : 14;\
          }
    union { UNSG32 u32Gbl_ClkSwitch1;
            struct w32Gbl_ClkSwitch1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_PadSelect_CLK0_V18EN(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_PadSelect_CLK0_V18EN(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_PadSelect_CLK0_V18EN(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_PadSelect_CLK0_V18EN(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_PadSelect_DVIO0_V18EN(r32)              _BFGET_(r32, 1, 1)
    #define   SET32Gbl_PadSelect_DVIO0_V18EN(r32,v)            _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_PadSelect_DVIO0_V18EN(r16)              _BFGET_(r16, 1, 1)
    #define   SET16Gbl_PadSelect_DVIO0_V18EN(r16,v)            _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_PadSelect_SOC0_V18EN(r32)               _BFGET_(r32, 2, 2)
    #define   SET32Gbl_PadSelect_SOC0_V18EN(r32,v)             _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_PadSelect_SOC0_V18EN(r16)               _BFGET_(r16, 2, 2)
    #define   SET16Gbl_PadSelect_SOC0_V18EN(r16,v)             _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_PadSelect_SPI0_V18EN(r32)               _BFGET_(r32, 3, 3)
    #define   SET32Gbl_PadSelect_SPI0_V18EN(r32,v)             _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_PadSelect_SPI0_V18EN(r16)               _BFGET_(r16, 3, 3)
    #define   SET16Gbl_PadSelect_SPI0_V18EN(r16,v)             _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_PadSelect_CLK0_V25EN(r32)               _BFGET_(r32, 4, 4)
    #define   SET32Gbl_PadSelect_CLK0_V25EN(r32,v)             _BFSET_(r32, 4, 4,v)
    #define   GET16Gbl_PadSelect_CLK0_V25EN(r16)               _BFGET_(r16, 4, 4)
    #define   SET16Gbl_PadSelect_CLK0_V25EN(r16,v)             _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_PadSelect_DVIO0_V25EN(r32)              _BFGET_(r32, 5, 5)
    #define   SET32Gbl_PadSelect_DVIO0_V25EN(r32,v)            _BFSET_(r32, 5, 5,v)
    #define   GET16Gbl_PadSelect_DVIO0_V25EN(r16)              _BFGET_(r16, 5, 5)
    #define   SET16Gbl_PadSelect_DVIO0_V25EN(r16,v)            _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_PadSelect_TS0_V25EN(r32)                _BFGET_(r32, 6, 6)
    #define   SET32Gbl_PadSelect_TS0_V25EN(r32,v)              _BFSET_(r32, 6, 6,v)
    #define   GET16Gbl_PadSelect_TS0_V25EN(r16)                _BFGET_(r16, 6, 6)
    #define   SET16Gbl_PadSelect_TS0_V25EN(r16,v)              _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_PadSelect_SPI0_V25EN(r32)               _BFGET_(r32, 7, 7)
    #define   SET32Gbl_PadSelect_SPI0_V25EN(r32,v)             _BFSET_(r32, 7, 7,v)
    #define   GET16Gbl_PadSelect_SPI0_V25EN(r16)               _BFGET_(r16, 7, 7)
    #define   SET16Gbl_PadSelect_SPI0_V25EN(r16,v)             _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_PadSelect_U6_V25EN(r32)                 _BFGET_(r32, 8, 8)
    #define   SET32Gbl_PadSelect_U6_V25EN(r32,v)               _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_PadSelect_U6_V25EN(r16)                 _BFGET_(r16, 8, 8)
    #define   SET16Gbl_PadSelect_U6_V25EN(r16,v)               _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_PadSelect_W14_V25EN(r32)                _BFGET_(r32, 9, 9)
    #define   SET32Gbl_PadSelect_W14_V25EN(r32,v)              _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_PadSelect_W14_V25EN(r16)                _BFGET_(r16, 9, 9)
    #define   SET16Gbl_PadSelect_W14_V25EN(r16,v)              _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_PadSelect_DVU0_V25EN(r32)               _BFGET_(r32,10,10)
    #define   SET32Gbl_PadSelect_DVU0_V25EN(r32,v)             _BFSET_(r32,10,10,v)
    #define   GET16Gbl_PadSelect_DVU0_V25EN(r16)               _BFGET_(r16,10,10)
    #define   SET16Gbl_PadSelect_DVU0_V25EN(r16,v)             _BFSET_(r16,10,10,v)

    #define   GET32Gbl_PadSelect_SOC0_V25EN(r32)               _BFGET_(r32,11,11)
    #define   SET32Gbl_PadSelect_SOC0_V25EN(r32,v)             _BFSET_(r32,11,11,v)
    #define   GET16Gbl_PadSelect_SOC0_V25EN(r16)               _BFGET_(r16,11,11)
    #define   SET16Gbl_PadSelect_SOC0_V25EN(r16,v)             _BFSET_(r16,11,11,v)

    #define   GET32Gbl_PadSelect_ETU1_V25EN(r32)               _BFGET_(r32,12,12)
    #define   SET32Gbl_PadSelect_ETU1_V25EN(r32,v)             _BFSET_(r32,12,12,v)
    #define   GET16Gbl_PadSelect_ETU1_V25EN(r16)               _BFGET_(r16,12,12)
    #define   SET16Gbl_PadSelect_ETU1_V25EN(r16,v)             _BFSET_(r16,12,12,v)

    #define   GET32Gbl_PadSelect_DVIO_OEN(r32)                 _BFGET_(r32,13,13)
    #define   SET32Gbl_PadSelect_DVIO_OEN(r32,v)               _BFSET_(r32,13,13,v)
    #define   GET16Gbl_PadSelect_DVIO_OEN(r16)                 _BFGET_(r16,13,13)
    #define   SET16Gbl_PadSelect_DVIO_OEN(r16,v)               _BFSET_(r16,13,13,v)

    #define   GET32Gbl_PadSelect_CLK0_ZN(r32)                  _BFGET_(r32,16,14)
    #define   SET32Gbl_PadSelect_CLK0_ZN(r32,v)                _BFSET_(r32,16,14,v)

    #define   GET32Gbl_PadSelect_CLK0_ZP(r32)                  _BFGET_(r32,19,17)
    #define   SET32Gbl_PadSelect_CLK0_ZP(r32,v)                _BFSET_(r32,19,17,v)
    #define   GET16Gbl_PadSelect_CLK0_ZP(r16)                  _BFGET_(r16, 3, 1)
    #define   SET16Gbl_PadSelect_CLK0_ZP(r16,v)                _BFSET_(r16, 3, 1,v)

    #define   GET32Gbl_PadSelect_DVIO0_ZN(r32)                 _BFGET_(r32,22,20)
    #define   SET32Gbl_PadSelect_DVIO0_ZN(r32,v)               _BFSET_(r32,22,20,v)
    #define   GET16Gbl_PadSelect_DVIO0_ZN(r16)                 _BFGET_(r16, 6, 4)
    #define   SET16Gbl_PadSelect_DVIO0_ZN(r16,v)               _BFSET_(r16, 6, 4,v)

    #define   GET32Gbl_PadSelect_DVIO0_ZP(r32)                 _BFGET_(r32,25,23)
    #define   SET32Gbl_PadSelect_DVIO0_ZP(r32,v)               _BFSET_(r32,25,23,v)
    #define   GET16Gbl_PadSelect_DVIO0_ZP(r16)                 _BFGET_(r16, 9, 7)
    #define   SET16Gbl_PadSelect_DVIO0_ZP(r16,v)               _BFSET_(r16, 9, 7,v)

    #define   GET32Gbl_PadSelect_ND0_ZN(r32)                   _BFGET_(r32,28,26)
    #define   SET32Gbl_PadSelect_ND0_ZN(r32,v)                 _BFSET_(r32,28,26,v)
    #define   GET16Gbl_PadSelect_ND0_ZN(r16)                   _BFGET_(r16,12,10)
    #define   SET16Gbl_PadSelect_ND0_ZN(r16,v)                 _BFSET_(r16,12,10,v)

    #define   GET32Gbl_PadSelect_ND0_ZP(r32)                   _BFGET_(r32,31,29)
    #define   SET32Gbl_PadSelect_ND0_ZP(r32,v)                 _BFSET_(r32,31,29,v)
    #define   GET16Gbl_PadSelect_ND0_ZP(r16)                   _BFGET_(r16,15,13)
    #define   SET16Gbl_PadSelect_ND0_ZP(r16,v)                 _BFSET_(r16,15,13,v)

    #define     w32Gbl_PadSelect                               {\
            UNSG32 uPadSelect_CLK0_V18EN                       :  1;\
            UNSG32 uPadSelect_DVIO0_V18EN                      :  1;\
            UNSG32 uPadSelect_SOC0_V18EN                       :  1;\
            UNSG32 uPadSelect_SPI0_V18EN                       :  1;\
            UNSG32 uPadSelect_CLK0_V25EN                       :  1;\
            UNSG32 uPadSelect_DVIO0_V25EN                      :  1;\
            UNSG32 uPadSelect_TS0_V25EN                        :  1;\
            UNSG32 uPadSelect_SPI0_V25EN                       :  1;\
            UNSG32 uPadSelect_U6_V25EN                         :  1;\
            UNSG32 uPadSelect_W14_V25EN                        :  1;\
            UNSG32 uPadSelect_DVU0_V25EN                       :  1;\
            UNSG32 uPadSelect_SOC0_V25EN                       :  1;\
            UNSG32 uPadSelect_ETU1_V25EN                       :  1;\
            UNSG32 uPadSelect_DVIO_OEN                         :  1;\
            UNSG32 uPadSelect_CLK0_ZN                          :  3;\
            UNSG32 uPadSelect_CLK0_ZP                          :  3;\
            UNSG32 uPadSelect_DVIO0_ZN                         :  3;\
            UNSG32 uPadSelect_DVIO0_ZP                         :  3;\
            UNSG32 uPadSelect_ND0_ZN                           :  3;\
            UNSG32 uPadSelect_ND0_ZP                           :  3;\
          }
    union { UNSG32 u32Gbl_PadSelect;
            struct w32Gbl_PadSelect;
          };
    #define   GET32Gbl_PadSelect_SOC0_ZN(r32)                  _BFGET_(r32, 2, 0)
    #define   SET32Gbl_PadSelect_SOC0_ZN(r32,v)                _BFSET_(r32, 2, 0,v)
    #define   GET16Gbl_PadSelect_SOC0_ZN(r16)                  _BFGET_(r16, 2, 0)
    #define   SET16Gbl_PadSelect_SOC0_ZN(r16,v)                _BFSET_(r16, 2, 0,v)

    #define   GET32Gbl_PadSelect_SOC0_ZP(r32)                  _BFGET_(r32, 5, 3)
    #define   SET32Gbl_PadSelect_SOC0_ZP(r32,v)                _BFSET_(r32, 5, 3,v)
    #define   GET16Gbl_PadSelect_SOC0_ZP(r16)                  _BFGET_(r16, 5, 3)
    #define   SET16Gbl_PadSelect_SOC0_ZP(r16,v)                _BFSET_(r16, 5, 3,v)

    #define   GET32Gbl_PadSelect_SPI0_ZN(r32)                  _BFGET_(r32, 8, 6)
    #define   SET32Gbl_PadSelect_SPI0_ZN(r32,v)                _BFSET_(r32, 8, 6,v)
    #define   GET16Gbl_PadSelect_SPI0_ZN(r16)                  _BFGET_(r16, 8, 6)
    #define   SET16Gbl_PadSelect_SPI0_ZN(r16,v)                _BFSET_(r16, 8, 6,v)

    #define   GET32Gbl_PadSelect_SPI0_ZP(r32)                  _BFGET_(r32,11, 9)
    #define   SET32Gbl_PadSelect_SPI0_ZP(r32,v)                _BFSET_(r32,11, 9,v)
    #define   GET16Gbl_PadSelect_SPI0_ZP(r16)                  _BFGET_(r16,11, 9)
    #define   SET16Gbl_PadSelect_SPI0_ZP(r16,v)                _BFSET_(r16,11, 9,v)

    #define     w32Gbl_PadSelect1                              {\
            UNSG32 uPadSelect_SOC0_ZN                          :  3;\
            UNSG32 uPadSelect_SOC0_ZP                          :  3;\
            UNSG32 uPadSelect_SPI0_ZN                          :  3;\
            UNSG32 uPadSelect_SPI0_ZP                          :  3;\
            UNSG32 RSVDxB0_b12                                 : 20;\
          }
    union { UNSG32 u32Gbl_PadSelect1;
            struct w32Gbl_PadSelect1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ResetTrigger_chipReset(r32)             _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ResetTrigger_chipReset(r32,v)           _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ResetTrigger_chipReset(r16)             _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ResetTrigger_chipReset(r16,v)           _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ResetTrigger_audio0ClkReset(r32)        _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ResetTrigger_audio0ClkReset(r32,v)      _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ResetTrigger_audio0ClkReset(r16)        _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ResetTrigger_audio0ClkReset(r16,v)      _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ResetTrigger_audio1ClkReset(r32)        _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ResetTrigger_audio1ClkReset(r32,v)      _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ResetTrigger_audio1ClkReset(r16)        _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ResetTrigger_audio1ClkReset(r16,v)      _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_ResetTrigger_audio2ClkReset(r32)        _BFGET_(r32, 3, 3)
    #define   SET32Gbl_ResetTrigger_audio2ClkReset(r32,v)      _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_ResetTrigger_audio2ClkReset(r16)        _BFGET_(r16, 3, 3)
    #define   SET16Gbl_ResetTrigger_audio2ClkReset(r16,v)      _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_ResetTrigger_audio3ClkReset(r32)        _BFGET_(r32, 4, 4)
    #define   SET32Gbl_ResetTrigger_audio3ClkReset(r32,v)      _BFSET_(r32, 4, 4,v)
    #define   GET16Gbl_ResetTrigger_audio3ClkReset(r16)        _BFGET_(r16, 4, 4)
    #define   SET16Gbl_ResetTrigger_audio3ClkReset(r16,v)      _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_ResetTrigger_audioHdClkReset(r32)       _BFGET_(r32, 5, 5)
    #define   SET32Gbl_ResetTrigger_audioHdClkReset(r32,v)     _BFSET_(r32, 5, 5,v)
    #define   GET16Gbl_ResetTrigger_audioHdClkReset(r16)       _BFGET_(r16, 5, 5)
    #define   SET16Gbl_ResetTrigger_audioHdClkReset(r16,v)     _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_ResetTrigger_sysPllSyncReset(r32)       _BFGET_(r32, 6, 6)
    #define   SET32Gbl_ResetTrigger_sysPllSyncReset(r32,v)     _BFSET_(r32, 6, 6,v)
    #define   GET16Gbl_ResetTrigger_sysPllSyncReset(r16)       _BFGET_(r16, 6, 6)
    #define   SET16Gbl_ResetTrigger_sysPllSyncReset(r16,v)     _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_ResetTrigger_memPllSyncReset(r32)       _BFGET_(r32, 7, 7)
    #define   SET32Gbl_ResetTrigger_memPllSyncReset(r32,v)     _BFSET_(r32, 7, 7,v)
    #define   GET16Gbl_ResetTrigger_memPllSyncReset(r16)       _BFGET_(r16, 7, 7)
    #define   SET16Gbl_ResetTrigger_memPllSyncReset(r16,v)     _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_ResetTrigger_cpu1PllSyncReset(r32)      _BFGET_(r32, 8, 8)
    #define   SET32Gbl_ResetTrigger_cpu1PllSyncReset(r32,v)    _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_ResetTrigger_cpu1PllSyncReset(r16)      _BFGET_(r16, 8, 8)
    #define   SET16Gbl_ResetTrigger_cpu1PllSyncReset(r16,v)    _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_ResetTrigger_cpu0SyncReset(r32)         _BFGET_(r32, 9, 9)
    #define   SET32Gbl_ResetTrigger_cpu0SyncReset(r32,v)       _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_ResetTrigger_cpu0SyncReset(r16)         _BFGET_(r16, 9, 9)
    #define   SET16Gbl_ResetTrigger_cpu0SyncReset(r16,v)       _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_ResetTrigger_cpu1SyncReset(r32)         _BFGET_(r32,10,10)
    #define   SET32Gbl_ResetTrigger_cpu1SyncReset(r32,v)       _BFSET_(r32,10,10,v)
    #define   GET16Gbl_ResetTrigger_cpu1SyncReset(r16)         _BFGET_(r16,10,10)
    #define   SET16Gbl_ResetTrigger_cpu1SyncReset(r16,v)       _BFSET_(r16,10,10,v)

    #define   GET32Gbl_ResetTrigger_socDdrSyncReset(r32)       _BFGET_(r32,11,11)
    #define   SET32Gbl_ResetTrigger_socDdrSyncReset(r32,v)     _BFSET_(r32,11,11,v)
    #define   GET16Gbl_ResetTrigger_socDdrSyncReset(r16)       _BFGET_(r16,11,11)
    #define   SET16Gbl_ResetTrigger_socDdrSyncReset(r16,v)     _BFSET_(r16,11,11,v)

    #define   GET32Gbl_ResetTrigger_gfxSyncReset(r32)          _BFGET_(r32,12,12)
    #define   SET32Gbl_ResetTrigger_gfxSyncReset(r32,v)        _BFSET_(r32,12,12,v)
    #define   GET16Gbl_ResetTrigger_gfxSyncReset(r16)          _BFGET_(r16,12,12)
    #define   SET16Gbl_ResetTrigger_gfxSyncReset(r16,v)        _BFSET_(r16,12,12,v)

    #define   GET32Gbl_ResetTrigger_zspSyncReset(r32)          _BFGET_(r32,13,13)
    #define   SET32Gbl_ResetTrigger_zspSyncReset(r32,v)        _BFSET_(r32,13,13,v)
    #define   GET16Gbl_ResetTrigger_zspSyncReset(r16)          _BFGET_(r16,13,13)
    #define   SET16Gbl_ResetTrigger_zspSyncReset(r16,v)        _BFSET_(r16,13,13,v)

    #define   GET32Gbl_ResetTrigger_avioSyncReset(r32)         _BFGET_(r32,14,14)
    #define   SET32Gbl_ResetTrigger_avioSyncReset(r32,v)       _BFSET_(r32,14,14,v)
    #define   GET16Gbl_ResetTrigger_avioSyncReset(r16)         _BFGET_(r16,14,14)
    #define   SET16Gbl_ResetTrigger_avioSyncReset(r16,v)       _BFSET_(r16,14,14,v)

    #define   GET32Gbl_ResetTrigger_perifSyncReset(r32)        _BFGET_(r32,15,15)
    #define   SET32Gbl_ResetTrigger_perifSyncReset(r32,v)      _BFSET_(r32,15,15,v)
    #define   GET16Gbl_ResetTrigger_perifSyncReset(r16)        _BFGET_(r16,15,15)
    #define   SET16Gbl_ResetTrigger_perifSyncReset(r16,v)      _BFSET_(r16,15,15,v)

    #define   GET32Gbl_ResetTrigger_gethSyncReset(r32)         _BFGET_(r32,16,16)
    #define   SET32Gbl_ResetTrigger_gethSyncReset(r32,v)       _BFSET_(r32,16,16,v)
    #define   GET16Gbl_ResetTrigger_gethSyncReset(r16)         _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ResetTrigger_gethSyncReset(r16,v)       _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ResetTrigger_sataSyncReset(r32)         _BFGET_(r32,17,17)
    #define   SET32Gbl_ResetTrigger_sataSyncReset(r32,v)       _BFSET_(r32,17,17,v)
    #define   GET16Gbl_ResetTrigger_sataSyncReset(r16)         _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ResetTrigger_sataSyncReset(r16,v)       _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ResetTrigger_pexSyncReset(r32)          _BFGET_(r32,18,18)
    #define   SET32Gbl_ResetTrigger_pexSyncReset(r32,v)        _BFSET_(r32,18,18,v)
    #define   GET16Gbl_ResetTrigger_pexSyncReset(r16)          _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ResetTrigger_pexSyncReset(r16,v)        _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_ResetTrigger_ahbApbSyncReset(r32)       _BFGET_(r32,19,19)
    #define   SET32Gbl_ResetTrigger_ahbApbSyncReset(r32,v)     _BFSET_(r32,19,19,v)
    #define   GET16Gbl_ResetTrigger_ahbApbSyncReset(r16)       _BFGET_(r16, 3, 3)
    #define   SET16Gbl_ResetTrigger_ahbApbSyncReset(r16,v)     _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_ResetTrigger_nanfSyncReset(r32)         _BFGET_(r32,20,20)
    #define   SET32Gbl_ResetTrigger_nanfSyncReset(r32,v)       _BFSET_(r32,20,20,v)
    #define   GET16Gbl_ResetTrigger_nanfSyncReset(r16)         _BFGET_(r16, 4, 4)
    #define   SET16Gbl_ResetTrigger_nanfSyncReset(r16,v)       _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_ResetTrigger_smemcSyncReset(r32)        _BFGET_(r32,21,21)
    #define   SET32Gbl_ResetTrigger_smemcSyncReset(r32,v)      _BFSET_(r32,21,21,v)
    #define   GET16Gbl_ResetTrigger_smemcSyncReset(r16)        _BFGET_(r16, 5, 5)
    #define   SET16Gbl_ResetTrigger_smemcSyncReset(r16,v)      _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_ResetTrigger_drmSyncReset(r32)          _BFGET_(r32,22,22)
    #define   SET32Gbl_ResetTrigger_drmSyncReset(r32,v)        _BFSET_(r32,22,22,v)
    #define   GET16Gbl_ResetTrigger_drmSyncReset(r16)          _BFGET_(r16, 6, 6)
    #define   SET16Gbl_ResetTrigger_drmSyncReset(r16,v)        _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_ResetTrigger_usb0SyncReset(r32)         _BFGET_(r32,23,23)
    #define   SET32Gbl_ResetTrigger_usb0SyncReset(r32,v)       _BFSET_(r32,23,23,v)
    #define   GET16Gbl_ResetTrigger_usb0SyncReset(r16)         _BFGET_(r16, 7, 7)
    #define   SET16Gbl_ResetTrigger_usb0SyncReset(r16,v)       _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_ResetTrigger_usb1SyncReset(r32)         _BFGET_(r32,24,24)
    #define   SET32Gbl_ResetTrigger_usb1SyncReset(r32,v)       _BFSET_(r32,24,24,v)
    #define   GET16Gbl_ResetTrigger_usb1SyncReset(r16)         _BFGET_(r16, 8, 8)
    #define   SET16Gbl_ResetTrigger_usb1SyncReset(r16,v)       _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_ResetTrigger_pBridgeSyncReset(r32)      _BFGET_(r32,25,25)
    #define   SET32Gbl_ResetTrigger_pBridgeSyncReset(r32,v)    _BFSET_(r32,25,25,v)
    #define   GET16Gbl_ResetTrigger_pBridgeSyncReset(r16)      _BFGET_(r16, 9, 9)
    #define   SET16Gbl_ResetTrigger_pBridgeSyncReset(r16,v)    _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_ResetTrigger_vppSyncReset(r32)          _BFGET_(r32,26,26)
    #define   SET32Gbl_ResetTrigger_vppSyncReset(r32,v)        _BFSET_(r32,26,26,v)
    #define   GET16Gbl_ResetTrigger_vppSyncReset(r16)          _BFGET_(r16,10,10)
    #define   SET16Gbl_ResetTrigger_vppSyncReset(r16,v)        _BFSET_(r16,10,10,v)

    #define   GET32Gbl_ResetTrigger_appSyncReset(r32)          _BFGET_(r32,27,27)
    #define   SET32Gbl_ResetTrigger_appSyncReset(r32,v)        _BFSET_(r32,27,27,v)
    #define   GET16Gbl_ResetTrigger_appSyncReset(r16)          _BFGET_(r16,11,11)
    #define   SET16Gbl_ResetTrigger_appSyncReset(r16,v)        _BFSET_(r16,11,11,v)

    #define   GET32Gbl_ResetTrigger_sdioSyncReset(r32)         _BFGET_(r32,28,28)
    #define   SET32Gbl_ResetTrigger_sdioSyncReset(r32,v)       _BFSET_(r32,28,28,v)
    #define   GET16Gbl_ResetTrigger_sdioSyncReset(r16)         _BFGET_(r16,12,12)
    #define   SET16Gbl_ResetTrigger_sdioSyncReset(r16,v)       _BFSET_(r16,12,12,v)

    #define     w32Gbl_ResetTrigger                            {\
            UNSG32 uResetTrigger_chipReset                     :  1;\
            UNSG32 uResetTrigger_audio0ClkReset                :  1;\
            UNSG32 uResetTrigger_audio1ClkReset                :  1;\
            UNSG32 uResetTrigger_audio2ClkReset                :  1;\
            UNSG32 uResetTrigger_audio3ClkReset                :  1;\
            UNSG32 uResetTrigger_audioHdClkReset               :  1;\
            UNSG32 uResetTrigger_sysPllSyncReset               :  1;\
            UNSG32 uResetTrigger_memPllSyncReset               :  1;\
            UNSG32 uResetTrigger_cpu1PllSyncReset              :  1;\
            UNSG32 uResetTrigger_cpu0SyncReset                 :  1;\
            UNSG32 uResetTrigger_cpu1SyncReset                 :  1;\
            UNSG32 uResetTrigger_socDdrSyncReset               :  1;\
            UNSG32 uResetTrigger_gfxSyncReset                  :  1;\
            UNSG32 uResetTrigger_zspSyncReset                  :  1;\
            UNSG32 uResetTrigger_avioSyncReset                 :  1;\
            UNSG32 uResetTrigger_perifSyncReset                :  1;\
            UNSG32 uResetTrigger_gethSyncReset                 :  1;\
            UNSG32 uResetTrigger_sataSyncReset                 :  1;\
            UNSG32 uResetTrigger_pexSyncReset                  :  1;\
            UNSG32 uResetTrigger_ahbApbSyncReset               :  1;\
            UNSG32 uResetTrigger_nanfSyncReset                 :  1;\
            UNSG32 uResetTrigger_smemcSyncReset                :  1;\
            UNSG32 uResetTrigger_drmSyncReset                  :  1;\
            UNSG32 uResetTrigger_usb0SyncReset                 :  1;\
            UNSG32 uResetTrigger_usb1SyncReset                 :  1;\
            UNSG32 uResetTrigger_pBridgeSyncReset              :  1;\
            UNSG32 uResetTrigger_vppSyncReset                  :  1;\
            UNSG32 uResetTrigger_appSyncReset                  :  1;\
            UNSG32 uResetTrigger_sdioSyncReset                 :  1;\
            UNSG32 RSVDxB4_b29                                 :  3;\
          }
    union { UNSG32 u32Gbl_ResetTrigger;
            struct w32Gbl_ResetTrigger;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ResetStatus_ChipResetStatus(r32)        _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ResetStatus_ChipResetStatus(r32,v)      _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ResetStatus_ChipResetStatus(r16)        _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ResetStatus_ChipResetStatus(r16,v)      _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ResetStatus_wd0Status(r32)              _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ResetStatus_wd0Status(r32,v)            _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ResetStatus_wd0Status(r16)              _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ResetStatus_wd0Status(r16,v)            _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ResetStatus_wd1Status(r32)              _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ResetStatus_wd1Status(r32,v)            _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ResetStatus_wd1Status(r16)              _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ResetStatus_wd1Status(r16,v)            _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_ResetStatus_wd2Status(r32)              _BFGET_(r32, 3, 3)
    #define   SET32Gbl_ResetStatus_wd2Status(r32,v)            _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_ResetStatus_wd2Status(r16)              _BFGET_(r16, 3, 3)
    #define   SET16Gbl_ResetStatus_wd2Status(r16,v)            _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_ResetStatus_audio0ClkResetStatus(r32)   _BFGET_(r32, 4, 4)
    #define   SET32Gbl_ResetStatus_audio0ClkResetStatus(r32,v) _BFSET_(r32, 4, 4,v)
    #define   GET16Gbl_ResetStatus_audio0ClkResetStatus(r16)   _BFGET_(r16, 4, 4)
    #define   SET16Gbl_ResetStatus_audio0ClkResetStatus(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_ResetStatus_audio1ClkResetStatus(r32)   _BFGET_(r32, 5, 5)
    #define   SET32Gbl_ResetStatus_audio1ClkResetStatus(r32,v) _BFSET_(r32, 5, 5,v)
    #define   GET16Gbl_ResetStatus_audio1ClkResetStatus(r16)   _BFGET_(r16, 5, 5)
    #define   SET16Gbl_ResetStatus_audio1ClkResetStatus(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_ResetStatus_audio2ClkResetStatus(r32)   _BFGET_(r32, 6, 6)
    #define   SET32Gbl_ResetStatus_audio2ClkResetStatus(r32,v) _BFSET_(r32, 6, 6,v)
    #define   GET16Gbl_ResetStatus_audio2ClkResetStatus(r16)   _BFGET_(r16, 6, 6)
    #define   SET16Gbl_ResetStatus_audio2ClkResetStatus(r16,v) _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_ResetStatus_audio3ClkResetStatus(r32)   _BFGET_(r32, 7, 7)
    #define   SET32Gbl_ResetStatus_audio3ClkResetStatus(r32,v) _BFSET_(r32, 7, 7,v)
    #define   GET16Gbl_ResetStatus_audio3ClkResetStatus(r16)   _BFGET_(r16, 7, 7)
    #define   SET16Gbl_ResetStatus_audio3ClkResetStatus(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_ResetStatus_audioHdClkResetStatus(r32)  _BFGET_(r32, 8, 8)
    #define   SET32Gbl_ResetStatus_audioHdClkResetStatus(r32,v) _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_ResetStatus_audioHdClkResetStatus(r16)  _BFGET_(r16, 8, 8)
    #define   SET16Gbl_ResetStatus_audioHdClkResetStatus(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_ResetStatus_sysPllSyncResetStatus(r32)  _BFGET_(r32, 9, 9)
    #define   SET32Gbl_ResetStatus_sysPllSyncResetStatus(r32,v) _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_ResetStatus_sysPllSyncResetStatus(r16)  _BFGET_(r16, 9, 9)
    #define   SET16Gbl_ResetStatus_sysPllSyncResetStatus(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_ResetStatus_memPllSyncResetStatus(r32)  _BFGET_(r32,10,10)
    #define   SET32Gbl_ResetStatus_memPllSyncResetStatus(r32,v) _BFSET_(r32,10,10,v)
    #define   GET16Gbl_ResetStatus_memPllSyncResetStatus(r16)  _BFGET_(r16,10,10)
    #define   SET16Gbl_ResetStatus_memPllSyncResetStatus(r16,v) _BFSET_(r16,10,10,v)

    #define   GET32Gbl_ResetStatus_cpu1PllSyncResetStatus(r32) _BFGET_(r32,11,11)
    #define   SET32Gbl_ResetStatus_cpu1PllSyncResetStatus(r32,v) _BFSET_(r32,11,11,v)
    #define   GET16Gbl_ResetStatus_cpu1PllSyncResetStatus(r16) _BFGET_(r16,11,11)
    #define   SET16Gbl_ResetStatus_cpu1PllSyncResetStatus(r16,v) _BFSET_(r16,11,11,v)

    #define   GET32Gbl_ResetStatus_avPllASyncResetStatus(r32)  _BFGET_(r32,12,12)
    #define   SET32Gbl_ResetStatus_avPllASyncResetStatus(r32,v) _BFSET_(r32,12,12,v)
    #define   GET16Gbl_ResetStatus_avPllASyncResetStatus(r16)  _BFGET_(r16,12,12)
    #define   SET16Gbl_ResetStatus_avPllASyncResetStatus(r16,v) _BFSET_(r16,12,12,v)

    #define   GET32Gbl_ResetStatus_avPllBSyncResetStatus(r32)  _BFGET_(r32,13,13)
    #define   SET32Gbl_ResetStatus_avPllBSyncResetStatus(r32,v) _BFSET_(r32,13,13,v)
    #define   GET16Gbl_ResetStatus_avPllBSyncResetStatus(r16)  _BFGET_(r16,13,13)
    #define   SET16Gbl_ResetStatus_avPllBSyncResetStatus(r16,v) _BFSET_(r16,13,13,v)

    #define   GET32Gbl_ResetStatus_cpu0SyncResetStatus(r32)    _BFGET_(r32,14,14)
    #define   SET32Gbl_ResetStatus_cpu0SyncResetStatus(r32,v)  _BFSET_(r32,14,14,v)
    #define   GET16Gbl_ResetStatus_cpu0SyncResetStatus(r16)    _BFGET_(r16,14,14)
    #define   SET16Gbl_ResetStatus_cpu0SyncResetStatus(r16,v)  _BFSET_(r16,14,14,v)

    #define   GET32Gbl_ResetStatus_cpu1SyncResetStatus(r32)    _BFGET_(r32,15,15)
    #define   SET32Gbl_ResetStatus_cpu1SyncResetStatus(r32,v)  _BFSET_(r32,15,15,v)
    #define   GET16Gbl_ResetStatus_cpu1SyncResetStatus(r16)    _BFGET_(r16,15,15)
    #define   SET16Gbl_ResetStatus_cpu1SyncResetStatus(r16,v)  _BFSET_(r16,15,15,v)

    #define   GET32Gbl_ResetStatus_socDdrSyncResetStatus(r32)  _BFGET_(r32,16,16)
    #define   SET32Gbl_ResetStatus_socDdrSyncResetStatus(r32,v) _BFSET_(r32,16,16,v)
    #define   GET16Gbl_ResetStatus_socDdrSyncResetStatus(r16)  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ResetStatus_socDdrSyncResetStatus(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ResetStatus_gfxSyncResetStatus(r32)     _BFGET_(r32,17,17)
    #define   SET32Gbl_ResetStatus_gfxSyncResetStatus(r32,v)   _BFSET_(r32,17,17,v)
    #define   GET16Gbl_ResetStatus_gfxSyncResetStatus(r16)     _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ResetStatus_gfxSyncResetStatus(r16,v)   _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ResetStatus_zspSyncResetStatus(r32)     _BFGET_(r32,18,18)
    #define   SET32Gbl_ResetStatus_zspSyncResetStatus(r32,v)   _BFSET_(r32,18,18,v)
    #define   GET16Gbl_ResetStatus_zspSyncResetStatus(r16)     _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ResetStatus_zspSyncResetStatus(r16,v)   _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_ResetStatus_avioSyncResetStatus(r32)    _BFGET_(r32,19,19)
    #define   SET32Gbl_ResetStatus_avioSyncResetStatus(r32,v)  _BFSET_(r32,19,19,v)
    #define   GET16Gbl_ResetStatus_avioSyncResetStatus(r16)    _BFGET_(r16, 3, 3)
    #define   SET16Gbl_ResetStatus_avioSyncResetStatus(r16,v)  _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_ResetStatus_perifSyncResetStatus(r32)   _BFGET_(r32,20,20)
    #define   SET32Gbl_ResetStatus_perifSyncResetStatus(r32,v) _BFSET_(r32,20,20,v)
    #define   GET16Gbl_ResetStatus_perifSyncResetStatus(r16)   _BFGET_(r16, 4, 4)
    #define   SET16Gbl_ResetStatus_perifSyncResetStatus(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_ResetStatus_gethSyncResetStatus(r32)    _BFGET_(r32,21,21)
    #define   SET32Gbl_ResetStatus_gethSyncResetStatus(r32,v)  _BFSET_(r32,21,21,v)
    #define   GET16Gbl_ResetStatus_gethSyncResetStatus(r16)    _BFGET_(r16, 5, 5)
    #define   SET16Gbl_ResetStatus_gethSyncResetStatus(r16,v)  _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_ResetStatus_sataSyncResetStatus(r32)    _BFGET_(r32,22,22)
    #define   SET32Gbl_ResetStatus_sataSyncResetStatus(r32,v)  _BFSET_(r32,22,22,v)
    #define   GET16Gbl_ResetStatus_sataSyncResetStatus(r16)    _BFGET_(r16, 6, 6)
    #define   SET16Gbl_ResetStatus_sataSyncResetStatus(r16,v)  _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_ResetStatus_ahbApbSyncResetStatus(r32)  _BFGET_(r32,23,23)
    #define   SET32Gbl_ResetStatus_ahbApbSyncResetStatus(r32,v) _BFSET_(r32,23,23,v)
    #define   GET16Gbl_ResetStatus_ahbApbSyncResetStatus(r16)  _BFGET_(r16, 7, 7)
    #define   SET16Gbl_ResetStatus_ahbApbSyncResetStatus(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_ResetStatus_nanfSyncResetStatus(r32)    _BFGET_(r32,24,24)
    #define   SET32Gbl_ResetStatus_nanfSyncResetStatus(r32,v)  _BFSET_(r32,24,24,v)
    #define   GET16Gbl_ResetStatus_nanfSyncResetStatus(r16)    _BFGET_(r16, 8, 8)
    #define   SET16Gbl_ResetStatus_nanfSyncResetStatus(r16,v)  _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_ResetStatus_smemcSyncResetStatus(r32)   _BFGET_(r32,25,25)
    #define   SET32Gbl_ResetStatus_smemcSyncResetStatus(r32,v) _BFSET_(r32,25,25,v)
    #define   GET16Gbl_ResetStatus_smemcSyncResetStatus(r16)   _BFGET_(r16, 9, 9)
    #define   SET16Gbl_ResetStatus_smemcSyncResetStatus(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_ResetStatus_drmSyncResetStatus(r32)     _BFGET_(r32,26,26)
    #define   SET32Gbl_ResetStatus_drmSyncResetStatus(r32,v)   _BFSET_(r32,26,26,v)
    #define   GET16Gbl_ResetStatus_drmSyncResetStatus(r16)     _BFGET_(r16,10,10)
    #define   SET16Gbl_ResetStatus_drmSyncResetStatus(r16,v)   _BFSET_(r16,10,10,v)

    #define   GET32Gbl_ResetStatus_usb0SyncResetStatus(r32)    _BFGET_(r32,27,27)
    #define   SET32Gbl_ResetStatus_usb0SyncResetStatus(r32,v)  _BFSET_(r32,27,27,v)
    #define   GET16Gbl_ResetStatus_usb0SyncResetStatus(r16)    _BFGET_(r16,11,11)
    #define   SET16Gbl_ResetStatus_usb0SyncResetStatus(r16,v)  _BFSET_(r16,11,11,v)

    #define   GET32Gbl_ResetStatus_usb1SyncResetStatus(r32)    _BFGET_(r32,28,28)
    #define   SET32Gbl_ResetStatus_usb1SyncResetStatus(r32,v)  _BFSET_(r32,28,28,v)
    #define   GET16Gbl_ResetStatus_usb1SyncResetStatus(r16)    _BFGET_(r16,12,12)
    #define   SET16Gbl_ResetStatus_usb1SyncResetStatus(r16,v)  _BFSET_(r16,12,12,v)

    #define   GET32Gbl_ResetStatus_pBridgeSyncResetStatus(r32) _BFGET_(r32,29,29)
    #define   SET32Gbl_ResetStatus_pBridgeSyncResetStatus(r32,v) _BFSET_(r32,29,29,v)
    #define   GET16Gbl_ResetStatus_pBridgeSyncResetStatus(r16) _BFGET_(r16,13,13)
    #define   SET16Gbl_ResetStatus_pBridgeSyncResetStatus(r16,v) _BFSET_(r16,13,13,v)

    #define   GET32Gbl_ResetStatus_vppSyncResetStatus(r32)     _BFGET_(r32,30,30)
    #define   SET32Gbl_ResetStatus_vppSyncResetStatus(r32,v)   _BFSET_(r32,30,30,v)
    #define   GET16Gbl_ResetStatus_vppSyncResetStatus(r16)     _BFGET_(r16,14,14)
    #define   SET16Gbl_ResetStatus_vppSyncResetStatus(r16,v)   _BFSET_(r16,14,14,v)

    #define   GET32Gbl_ResetStatus_appSyncResetStatus(r32)     _BFGET_(r32,31,31)
    #define   SET32Gbl_ResetStatus_appSyncResetStatus(r32,v)   _BFSET_(r32,31,31,v)
    #define   GET16Gbl_ResetStatus_appSyncResetStatus(r16)     _BFGET_(r16,15,15)
    #define   SET16Gbl_ResetStatus_appSyncResetStatus(r16,v)   _BFSET_(r16,15,15,v)

    #define     w32Gbl_ResetStatus                             {\
            UNSG32 uResetStatus_ChipResetStatus                :  1;\
            UNSG32 uResetStatus_wd0Status                      :  1;\
            UNSG32 uResetStatus_wd1Status                      :  1;\
            UNSG32 uResetStatus_wd2Status                      :  1;\
            UNSG32 uResetStatus_audio0ClkResetStatus           :  1;\
            UNSG32 uResetStatus_audio1ClkResetStatus           :  1;\
            UNSG32 uResetStatus_audio2ClkResetStatus           :  1;\
            UNSG32 uResetStatus_audio3ClkResetStatus           :  1;\
            UNSG32 uResetStatus_audioHdClkResetStatus          :  1;\
            UNSG32 uResetStatus_sysPllSyncResetStatus          :  1;\
            UNSG32 uResetStatus_memPllSyncResetStatus          :  1;\
            UNSG32 uResetStatus_cpu1PllSyncResetStatus         :  1;\
            UNSG32 uResetStatus_avPllASyncResetStatus          :  1;\
            UNSG32 uResetStatus_avPllBSyncResetStatus          :  1;\
            UNSG32 uResetStatus_cpu0SyncResetStatus            :  1;\
            UNSG32 uResetStatus_cpu1SyncResetStatus            :  1;\
            UNSG32 uResetStatus_socDdrSyncResetStatus          :  1;\
            UNSG32 uResetStatus_gfxSyncResetStatus             :  1;\
            UNSG32 uResetStatus_zspSyncResetStatus             :  1;\
            UNSG32 uResetStatus_avioSyncResetStatus            :  1;\
            UNSG32 uResetStatus_perifSyncResetStatus           :  1;\
            UNSG32 uResetStatus_gethSyncResetStatus            :  1;\
            UNSG32 uResetStatus_sataSyncResetStatus            :  1;\
            UNSG32 uResetStatus_ahbApbSyncResetStatus          :  1;\
            UNSG32 uResetStatus_nanfSyncResetStatus            :  1;\
            UNSG32 uResetStatus_smemcSyncResetStatus           :  1;\
            UNSG32 uResetStatus_drmSyncResetStatus             :  1;\
            UNSG32 uResetStatus_usb0SyncResetStatus            :  1;\
            UNSG32 uResetStatus_usb1SyncResetStatus            :  1;\
            UNSG32 uResetStatus_pBridgeSyncResetStatus         :  1;\
            UNSG32 uResetStatus_vppSyncResetStatus             :  1;\
            UNSG32 uResetStatus_appSyncResetStatus             :  1;\
          }
    union { UNSG32 u32Gbl_ResetStatus;
            struct w32Gbl_ResetStatus;
          };
    #define   GET32Gbl_ResetStatus_sdioSyncResetStatus(r32)    _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ResetStatus_sdioSyncResetStatus(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ResetStatus_sdioSyncResetStatus(r16)    _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ResetStatus_sdioSyncResetStatus(r16,v)  _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_ResetStatus1                            {\
            UNSG32 uResetStatus_sdioSyncResetStatus            :  1;\
            UNSG32 RSVDxBC_b1                                  : 31;\
          }
    union { UNSG32 u32Gbl_ResetStatus1;
            struct w32Gbl_ResetStatus1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_sw_generic0_swReg0(r32)                 _BFGET_(r32,31, 0)
    #define   SET32Gbl_sw_generic0_swReg0(r32,v)               _BFSET_(r32,31, 0,v)

    #define     w32Gbl_sw_generic0                             {\
            UNSG32 usw_generic0_swReg0                         : 32;\
          }
    union { UNSG32 u32Gbl_sw_generic0;
            struct w32Gbl_sw_generic0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_sw_generic1_swReg1(r32)                 _BFGET_(r32,31, 0)
    #define   SET32Gbl_sw_generic1_swReg1(r32,v)               _BFSET_(r32,31, 0,v)

    #define     w32Gbl_sw_generic1                             {\
            UNSG32 usw_generic1_swReg1                         : 32;\
          }
    union { UNSG32 u32Gbl_sw_generic1;
            struct w32Gbl_sw_generic1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_sw_generic2_swReg2(r32)                 _BFGET_(r32,31, 0)
    #define   SET32Gbl_sw_generic2_swReg2(r32,v)               _BFSET_(r32,31, 0,v)

    #define     w32Gbl_sw_generic2                             {\
            UNSG32 usw_generic2_swReg2                         : 32;\
          }
    union { UNSG32 u32Gbl_sw_generic2;
            struct w32Gbl_sw_generic2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_sw_generic3_swReg3(r32)                 _BFGET_(r32,31, 0)
    #define   SET32Gbl_sw_generic3_swReg3(r32,v)               _BFSET_(r32,31, 0,v)

    #define     w32Gbl_sw_generic3                             {\
            UNSG32 usw_generic3_swReg3                         : 32;\
          }
    union { UNSG32 u32Gbl_sw_generic3;
            struct w32Gbl_sw_generic3;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ChipCntl0_dCacheCfgCPU0(r32)            _BFGET_(r32, 1, 0)
    #define   SET32Gbl_ChipCntl0_dCacheCfgCPU0(r32,v)          _BFSET_(r32, 1, 0,v)
    #define   GET16Gbl_ChipCntl0_dCacheCfgCPU0(r16)            _BFGET_(r16, 1, 0)
    #define   SET16Gbl_ChipCntl0_dCacheCfgCPU0(r16,v)          _BFSET_(r16, 1, 0,v)

    #define   GET32Gbl_ChipCntl0_iCacheCfgCPU0(r32)            _BFGET_(r32, 3, 2)
    #define   SET32Gbl_ChipCntl0_iCacheCfgCPU0(r32,v)          _BFSET_(r32, 3, 2,v)
    #define   GET16Gbl_ChipCntl0_iCacheCfgCPU0(r16)            _BFGET_(r16, 3, 2)
    #define   SET16Gbl_ChipCntl0_iCacheCfgCPU0(r16,v)          _BFSET_(r16, 3, 2,v)

    #define   GET32Gbl_ChipCntl0_dCacheCfgCPU1(r32)            _BFGET_(r32, 5, 4)
    #define   SET32Gbl_ChipCntl0_dCacheCfgCPU1(r32,v)          _BFSET_(r32, 5, 4,v)
    #define   GET16Gbl_ChipCntl0_dCacheCfgCPU1(r16)            _BFGET_(r16, 5, 4)
    #define   SET16Gbl_ChipCntl0_dCacheCfgCPU1(r16,v)          _BFSET_(r16, 5, 4,v)

    #define   GET32Gbl_ChipCntl0_iCacheCfgCPU1(r32)            _BFGET_(r32, 7, 6)
    #define   SET32Gbl_ChipCntl0_iCacheCfgCPU1(r32,v)          _BFSET_(r32, 7, 6,v)
    #define   GET16Gbl_ChipCntl0_iCacheCfgCPU1(r16)            _BFGET_(r16, 7, 6)
    #define   SET16Gbl_ChipCntl0_iCacheCfgCPU1(r16,v)          _BFSET_(r16, 7, 6,v)

    #define   GET32Gbl_ChipCntl0_cg_bpass0Cpu0(r32)            _BFGET_(r32, 8, 8)
    #define   SET32Gbl_ChipCntl0_cg_bpass0Cpu0(r32,v)          _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_ChipCntl0_cg_bpass0Cpu0(r16)            _BFGET_(r16, 8, 8)
    #define   SET16Gbl_ChipCntl0_cg_bpass0Cpu0(r16,v)          _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_ChipCntl0_cg_bpass0Cpu1(r32)            _BFGET_(r32, 9, 9)
    #define   SET32Gbl_ChipCntl0_cg_bpass0Cpu1(r32,v)          _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_ChipCntl0_cg_bpass0Cpu1(r16)            _BFGET_(r16, 9, 9)
    #define   SET16Gbl_ChipCntl0_cg_bpass0Cpu1(r16,v)          _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_ChipCntl0_wfiJtagClkEn(r32)             _BFGET_(r32,10,10)
    #define   SET32Gbl_ChipCntl0_wfiJtagClkEn(r32,v)           _BFSET_(r32,10,10,v)
    #define   GET16Gbl_ChipCntl0_wfiJtagClkEn(r16)             _BFGET_(r16,10,10)
    #define   SET16Gbl_ChipCntl0_wfiJtagClkEn(r16,v)           _BFSET_(r16,10,10,v)

    #define     w32Gbl_ChipCntl0                               {\
            UNSG32 uChipCntl0_dCacheCfgCPU0                    :  2;\
            UNSG32 uChipCntl0_iCacheCfgCPU0                    :  2;\
            UNSG32 uChipCntl0_dCacheCfgCPU1                    :  2;\
            UNSG32 uChipCntl0_iCacheCfgCPU1                    :  2;\
            UNSG32 uChipCntl0_cg_bpass0Cpu0                    :  1;\
            UNSG32 uChipCntl0_cg_bpass0Cpu1                    :  1;\
            UNSG32 uChipCntl0_wfiJtagClkEn                     :  1;\
            UNSG32 RSVDxD0_b11                                 : 21;\
          }
    union { UNSG32 u32Gbl_ChipCntl0;
            struct w32Gbl_ChipCntl0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ChipCntl1_rtcIcDataCpu0(r32)            _BFGET_(r32, 2, 0)
    #define   SET32Gbl_ChipCntl1_rtcIcDataCpu0(r32,v)          _BFSET_(r32, 2, 0,v)
    #define   GET16Gbl_ChipCntl1_rtcIcDataCpu0(r16)            _BFGET_(r16, 2, 0)
    #define   SET16Gbl_ChipCntl1_rtcIcDataCpu0(r16,v)          _BFSET_(r16, 2, 0,v)

    #define   GET32Gbl_ChipCntl1_rtcIcTagCpu0(r32)             _BFGET_(r32, 5, 3)
    #define   SET32Gbl_ChipCntl1_rtcIcTagCpu0(r32,v)           _BFSET_(r32, 5, 3,v)
    #define   GET16Gbl_ChipCntl1_rtcIcTagCpu0(r16)             _BFGET_(r16, 5, 3)
    #define   SET16Gbl_ChipCntl1_rtcIcTagCpu0(r16,v)           _BFSET_(r16, 5, 3,v)

    #define   GET32Gbl_ChipCntl1_rtcDcDataCpu0(r32)            _BFGET_(r32, 8, 6)
    #define   SET32Gbl_ChipCntl1_rtcDcDataCpu0(r32,v)          _BFSET_(r32, 8, 6,v)
    #define   GET16Gbl_ChipCntl1_rtcDcDataCpu0(r16)            _BFGET_(r16, 8, 6)
    #define   SET16Gbl_ChipCntl1_rtcDcDataCpu0(r16,v)          _BFSET_(r16, 8, 6,v)

    #define   GET32Gbl_ChipCntl1_rtcDcTagCpu0(r32)             _BFGET_(r32,10, 9)
    #define   SET32Gbl_ChipCntl1_rtcDcTagCpu0(r32,v)           _BFSET_(r32,10, 9,v)
    #define   GET16Gbl_ChipCntl1_rtcDcTagCpu0(r16)             _BFGET_(r16,10, 9)
    #define   SET16Gbl_ChipCntl1_rtcDcTagCpu0(r16,v)           _BFSET_(r16,10, 9,v)

    #define   GET32Gbl_ChipCntl1_wtcDcTagCpu0(r32)             _BFGET_(r32,13,11)
    #define   SET32Gbl_ChipCntl1_wtcDcTagCpu0(r32,v)           _BFSET_(r32,13,11,v)
    #define   GET16Gbl_ChipCntl1_wtcDcTagCpu0(r16)             _BFGET_(r16,13,11)
    #define   SET16Gbl_ChipCntl1_wtcDcTagCpu0(r16,v)           _BFSET_(r16,13,11,v)

    #define   GET32Gbl_ChipCntl1_rtcDcDirtyCpu0(r32)           _BFGET_(r32,15,14)
    #define   SET32Gbl_ChipCntl1_rtcDcDirtyCpu0(r32,v)         _BFSET_(r32,15,14,v)
    #define   GET16Gbl_ChipCntl1_rtcDcDirtyCpu0(r16)           _BFGET_(r16,15,14)
    #define   SET16Gbl_ChipCntl1_rtcDcDirtyCpu0(r16,v)         _BFSET_(r16,15,14,v)

    #define   GET32Gbl_ChipCntl1_wtcDcDirtyCpu0(r32)           _BFGET_(r32,18,16)
    #define   SET32Gbl_ChipCntl1_wtcDcDirtyCpu0(r32,v)         _BFSET_(r32,18,16,v)
    #define   GET16Gbl_ChipCntl1_wtcDcDirtyCpu0(r16)           _BFGET_(r16, 2, 0)
    #define   SET16Gbl_ChipCntl1_wtcDcDirtyCpu0(r16,v)         _BFSET_(r16, 2, 0,v)

    #define   GET32Gbl_ChipCntl1_rtcBpmCpu0(r32)               _BFGET_(r32,20,19)
    #define   SET32Gbl_ChipCntl1_rtcBpmCpu0(r32,v)             _BFSET_(r32,20,19,v)
    #define   GET16Gbl_ChipCntl1_rtcBpmCpu0(r16)               _BFGET_(r16, 4, 3)
    #define   SET16Gbl_ChipCntl1_rtcBpmCpu0(r16,v)             _BFSET_(r16, 4, 3,v)

    #define   GET32Gbl_ChipCntl1_wtcBpmCpu0(r32)               _BFGET_(r32,23,21)
    #define   SET32Gbl_ChipCntl1_wtcBpmCpu0(r32,v)             _BFSET_(r32,23,21,v)
    #define   GET16Gbl_ChipCntl1_wtcBpmCpu0(r16)               _BFGET_(r16, 7, 5)
    #define   SET16Gbl_ChipCntl1_wtcBpmCpu0(r16,v)             _BFSET_(r16, 7, 5,v)

    #define   GET32Gbl_ChipCntl1_rtcTlbCpu0(r32)               _BFGET_(r32,25,24)
    #define   SET32Gbl_ChipCntl1_rtcTlbCpu0(r32,v)             _BFSET_(r32,25,24,v)
    #define   GET16Gbl_ChipCntl1_rtcTlbCpu0(r16)               _BFGET_(r16, 9, 8)
    #define   SET16Gbl_ChipCntl1_rtcTlbCpu0(r16,v)             _BFSET_(r16, 9, 8,v)

    #define   GET32Gbl_ChipCntl1_wtcTlbCpu0(r32)               _BFGET_(r32,28,26)
    #define   SET32Gbl_ChipCntl1_wtcTlbCpu0(r32,v)             _BFSET_(r32,28,26,v)
    #define   GET16Gbl_ChipCntl1_wtcTlbCpu0(r16)               _BFGET_(r16,12,10)
    #define   SET16Gbl_ChipCntl1_wtcTlbCpu0(r16,v)             _BFSET_(r16,12,10,v)

    #define   GET32Gbl_ChipCntl1_rtcIcDataCpu1(r32)            _BFGET_(r32,31,29)
    #define   SET32Gbl_ChipCntl1_rtcIcDataCpu1(r32,v)          _BFSET_(r32,31,29,v)
    #define   GET16Gbl_ChipCntl1_rtcIcDataCpu1(r16)            _BFGET_(r16,15,13)
    #define   SET16Gbl_ChipCntl1_rtcIcDataCpu1(r16,v)          _BFSET_(r16,15,13,v)

    #define     w32Gbl_ChipCntl1                               {\
            UNSG32 uChipCntl1_rtcIcDataCpu0                    :  3;\
            UNSG32 uChipCntl1_rtcIcTagCpu0                     :  3;\
            UNSG32 uChipCntl1_rtcDcDataCpu0                    :  3;\
            UNSG32 uChipCntl1_rtcDcTagCpu0                     :  2;\
            UNSG32 uChipCntl1_wtcDcTagCpu0                     :  3;\
            UNSG32 uChipCntl1_rtcDcDirtyCpu0                   :  2;\
            UNSG32 uChipCntl1_wtcDcDirtyCpu0                   :  3;\
            UNSG32 uChipCntl1_rtcBpmCpu0                       :  2;\
            UNSG32 uChipCntl1_wtcBpmCpu0                       :  3;\
            UNSG32 uChipCntl1_rtcTlbCpu0                       :  2;\
            UNSG32 uChipCntl1_wtcTlbCpu0                       :  3;\
            UNSG32 uChipCntl1_rtcIcDataCpu1                    :  3;\
          }
    union { UNSG32 u32Gbl_ChipCntl1;
            struct w32Gbl_ChipCntl1;
          };
    #define   GET32Gbl_ChipCntl1_rtcIcTagCpu1(r32)             _BFGET_(r32, 2, 0)
    #define   SET32Gbl_ChipCntl1_rtcIcTagCpu1(r32,v)           _BFSET_(r32, 2, 0,v)
    #define   GET16Gbl_ChipCntl1_rtcIcTagCpu1(r16)             _BFGET_(r16, 2, 0)
    #define   SET16Gbl_ChipCntl1_rtcIcTagCpu1(r16,v)           _BFSET_(r16, 2, 0,v)

    #define   GET32Gbl_ChipCntl1_rtcDcDataCpu1(r32)            _BFGET_(r32, 5, 3)
    #define   SET32Gbl_ChipCntl1_rtcDcDataCpu1(r32,v)          _BFSET_(r32, 5, 3,v)
    #define   GET16Gbl_ChipCntl1_rtcDcDataCpu1(r16)            _BFGET_(r16, 5, 3)
    #define   SET16Gbl_ChipCntl1_rtcDcDataCpu1(r16,v)          _BFSET_(r16, 5, 3,v)

    #define   GET32Gbl_ChipCntl1_rtcDcTagCpu1(r32)             _BFGET_(r32, 7, 6)
    #define   SET32Gbl_ChipCntl1_rtcDcTagCpu1(r32,v)           _BFSET_(r32, 7, 6,v)
    #define   GET16Gbl_ChipCntl1_rtcDcTagCpu1(r16)             _BFGET_(r16, 7, 6)
    #define   SET16Gbl_ChipCntl1_rtcDcTagCpu1(r16,v)           _BFSET_(r16, 7, 6,v)

    #define   GET32Gbl_ChipCntl1_wtcDcTagCpu1(r32)             _BFGET_(r32,10, 8)
    #define   SET32Gbl_ChipCntl1_wtcDcTagCpu1(r32,v)           _BFSET_(r32,10, 8,v)
    #define   GET16Gbl_ChipCntl1_wtcDcTagCpu1(r16)             _BFGET_(r16,10, 8)
    #define   SET16Gbl_ChipCntl1_wtcDcTagCpu1(r16,v)           _BFSET_(r16,10, 8,v)

    #define   GET32Gbl_ChipCntl1_rtcDcDirtyCpu1(r32)           _BFGET_(r32,12,11)
    #define   SET32Gbl_ChipCntl1_rtcDcDirtyCpu1(r32,v)         _BFSET_(r32,12,11,v)
    #define   GET16Gbl_ChipCntl1_rtcDcDirtyCpu1(r16)           _BFGET_(r16,12,11)
    #define   SET16Gbl_ChipCntl1_rtcDcDirtyCpu1(r16,v)         _BFSET_(r16,12,11,v)

    #define   GET32Gbl_ChipCntl1_wtcDcDirtyCpu1(r32)           _BFGET_(r32,15,13)
    #define   SET32Gbl_ChipCntl1_wtcDcDirtyCpu1(r32,v)         _BFSET_(r32,15,13,v)
    #define   GET16Gbl_ChipCntl1_wtcDcDirtyCpu1(r16)           _BFGET_(r16,15,13)
    #define   SET16Gbl_ChipCntl1_wtcDcDirtyCpu1(r16,v)         _BFSET_(r16,15,13,v)

    #define   GET32Gbl_ChipCntl1_rtcBpmCpu1(r32)               _BFGET_(r32,17,16)
    #define   SET32Gbl_ChipCntl1_rtcBpmCpu1(r32,v)             _BFSET_(r32,17,16,v)
    #define   GET16Gbl_ChipCntl1_rtcBpmCpu1(r16)               _BFGET_(r16, 1, 0)
    #define   SET16Gbl_ChipCntl1_rtcBpmCpu1(r16,v)             _BFSET_(r16, 1, 0,v)

    #define   GET32Gbl_ChipCntl1_wtcBpmCpu1(r32)               _BFGET_(r32,20,18)
    #define   SET32Gbl_ChipCntl1_wtcBpmCpu1(r32,v)             _BFSET_(r32,20,18,v)
    #define   GET16Gbl_ChipCntl1_wtcBpmCpu1(r16)               _BFGET_(r16, 4, 2)
    #define   SET16Gbl_ChipCntl1_wtcBpmCpu1(r16,v)             _BFSET_(r16, 4, 2,v)

    #define   GET32Gbl_ChipCntl1_rtcTlbCpu1(r32)               _BFGET_(r32,22,21)
    #define   SET32Gbl_ChipCntl1_rtcTlbCpu1(r32,v)             _BFSET_(r32,22,21,v)
    #define   GET16Gbl_ChipCntl1_rtcTlbCpu1(r16)               _BFGET_(r16, 6, 5)
    #define   SET16Gbl_ChipCntl1_rtcTlbCpu1(r16,v)             _BFSET_(r16, 6, 5,v)

    #define   GET32Gbl_ChipCntl1_wtcTlbCpu1(r32)               _BFGET_(r32,25,23)
    #define   SET32Gbl_ChipCntl1_wtcTlbCpu1(r32,v)             _BFSET_(r32,25,23,v)
    #define   GET16Gbl_ChipCntl1_wtcTlbCpu1(r16)               _BFGET_(r16, 9, 7)
    #define   SET16Gbl_ChipCntl1_wtcTlbCpu1(r16,v)             _BFSET_(r16, 9, 7,v)

    #define     w32Gbl_ChipCntl11                              {\
            UNSG32 uChipCntl1_rtcIcTagCpu1                     :  3;\
            UNSG32 uChipCntl1_rtcDcDataCpu1                    :  3;\
            UNSG32 uChipCntl1_rtcDcTagCpu1                     :  2;\
            UNSG32 uChipCntl1_wtcDcTagCpu1                     :  3;\
            UNSG32 uChipCntl1_rtcDcDirtyCpu1                   :  2;\
            UNSG32 uChipCntl1_wtcDcDirtyCpu1                   :  3;\
            UNSG32 uChipCntl1_rtcBpmCpu1                       :  2;\
            UNSG32 uChipCntl1_wtcBpmCpu1                       :  3;\
            UNSG32 uChipCntl1_rtcTlbCpu1                       :  2;\
            UNSG32 uChipCntl1_wtcTlbCpu1                       :  3;\
            UNSG32 RSVDxD8_b26                                 :  6;\
          }
    union { UNSG32 u32Gbl_ChipCntl11;
            struct w32Gbl_ChipCntl11;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RWTC_avio31to0_value(r32)               _BFGET_(r32,31, 0)
    #define   SET32Gbl_RWTC_avio31to0_value(r32,v)             _BFSET_(r32,31, 0,v)

    #define     w32Gbl_RWTC_avio31to0                          {\
            UNSG32 uRWTC_avio31to0_value                       : 32;\
          }
    union { UNSG32 u32Gbl_RWTC_avio31to0;
            struct w32Gbl_RWTC_avio31to0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RWTC_avio43to32_value(r32)              _BFGET_(r32,11, 0)
    #define   SET32Gbl_RWTC_avio43to32_value(r32,v)            _BFSET_(r32,11, 0,v)
    #define   GET16Gbl_RWTC_avio43to32_value(r16)              _BFGET_(r16,11, 0)
    #define   SET16Gbl_RWTC_avio43to32_value(r16,v)            _BFSET_(r16,11, 0,v)

    #define     w32Gbl_RWTC_avio43to32                         {\
            UNSG32 uRWTC_avio43to32_value                      : 12;\
            UNSG32 RSVDxE0_b12                                 : 20;\
          }
    union { UNSG32 u32Gbl_RWTC_avio43to32;
            struct w32Gbl_RWTC_avio43to32;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RWTC_gfx2D31to0_value(r32)              _BFGET_(r32,31, 0)
    #define   SET32Gbl_RWTC_gfx2D31to0_value(r32,v)            _BFSET_(r32,31, 0,v)

    #define     w32Gbl_RWTC_gfx2D31to0                         {\
            UNSG32 uRWTC_gfx2D31to0_value                      : 32;\
          }
    union { UNSG32 u32Gbl_RWTC_gfx2D31to0;
            struct w32Gbl_RWTC_gfx2D31to0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RWTC_gfx2D43to32_value(r32)             _BFGET_(r32,11, 0)
    #define   SET32Gbl_RWTC_gfx2D43to32_value(r32,v)           _BFSET_(r32,11, 0,v)
    #define   GET16Gbl_RWTC_gfx2D43to32_value(r16)             _BFGET_(r16,11, 0)
    #define   SET16Gbl_RWTC_gfx2D43to32_value(r16,v)           _BFSET_(r16,11, 0,v)

    #define     w32Gbl_RWTC_gfx2D43to32                        {\
            UNSG32 uRWTC_gfx2D43to32_value                     : 12;\
            UNSG32 RSVDxE8_b12                                 : 20;\
          }
    union { UNSG32 u32Gbl_RWTC_gfx2D43to32;
            struct w32Gbl_RWTC_gfx2D43to32;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RWTC_perif31to0_value(r32)              _BFGET_(r32,31, 0)
    #define   SET32Gbl_RWTC_perif31to0_value(r32,v)            _BFSET_(r32,31, 0,v)

    #define     w32Gbl_RWTC_perif31to0                         {\
            UNSG32 uRWTC_perif31to0_value                      : 32;\
          }
    union { UNSG32 u32Gbl_RWTC_perif31to0;
            struct w32Gbl_RWTC_perif31to0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RWTC_perif43to32_value(r32)             _BFGET_(r32,11, 0)
    #define   SET32Gbl_RWTC_perif43to32_value(r32,v)           _BFSET_(r32,11, 0,v)
    #define   GET16Gbl_RWTC_perif43to32_value(r16)             _BFGET_(r16,11, 0)
    #define   SET16Gbl_RWTC_perif43to32_value(r16,v)           _BFSET_(r16,11, 0,v)

    #define     w32Gbl_RWTC_perif43to32                        {\
            UNSG32 uRWTC_perif43to32_value                     : 12;\
            UNSG32 RSVDxF0_b12                                 : 20;\
          }
    union { UNSG32 u32Gbl_RWTC_perif43to32;
            struct w32Gbl_RWTC_perif43to32;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RWTC_soc31to0_value(r32)                _BFGET_(r32,31, 0)
    #define   SET32Gbl_RWTC_soc31to0_value(r32,v)              _BFSET_(r32,31, 0,v)

    #define     w32Gbl_RWTC_soc31to0                           {\
            UNSG32 uRWTC_soc31to0_value                        : 32;\
          }
    union { UNSG32 u32Gbl_RWTC_soc31to0;
            struct w32Gbl_RWTC_soc31to0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RWTC_soc43to32_value(r32)               _BFGET_(r32,11, 0)
    #define   SET32Gbl_RWTC_soc43to32_value(r32,v)             _BFSET_(r32,11, 0,v)
    #define   GET16Gbl_RWTC_soc43to32_value(r16)               _BFGET_(r16,11, 0)
    #define   SET16Gbl_RWTC_soc43to32_value(r16,v)             _BFSET_(r16,11, 0,v)

    #define     w32Gbl_RWTC_soc43to32                          {\
            UNSG32 uRWTC_soc43to32_value                       : 12;\
            UNSG32 RSVDxF8_b12                                 : 20;\
          }
    union { UNSG32 u32Gbl_RWTC_soc43to32;
            struct w32Gbl_RWTC_soc43to32;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RWTC_vPro31to0_value(r32)               _BFGET_(r32,31, 0)
    #define   SET32Gbl_RWTC_vPro31to0_value(r32,v)             _BFSET_(r32,31, 0,v)

    #define     w32Gbl_RWTC_vPro31to0                          {\
            UNSG32 uRWTC_vPro31to0_value                       : 32;\
          }
    union { UNSG32 u32Gbl_RWTC_vPro31to0;
            struct w32Gbl_RWTC_vPro31to0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RWTC_vPro43to32_value(r32)              _BFGET_(r32,11, 0)
    #define   SET32Gbl_RWTC_vPro43to32_value(r32,v)            _BFSET_(r32,11, 0,v)
    #define   GET16Gbl_RWTC_vPro43to32_value(r16)              _BFGET_(r16,11, 0)
    #define   SET16Gbl_RWTC_vPro43to32_value(r16,v)            _BFSET_(r16,11, 0,v)

    #define     w32Gbl_RWTC_vPro43to32                         {\
            UNSG32 uRWTC_vPro43to32_value                      : 12;\
            UNSG32 RSVDx100_b12                                : 20;\
          }
    union { UNSG32 u32Gbl_RWTC_vPro43to32;
            struct w32Gbl_RWTC_vPro43to32;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ProductId_Id(r32)                       _BFGET_(r32,31, 0)
    #define   SET32Gbl_ProductId_Id(r32,v)                     _BFSET_(r32,31, 0,v)

    #define     w32Gbl_ProductId                               {\
            UNSG32 uProductId_Id                               : 32;\
          }
    union { UNSG32 u32Gbl_ProductId;
            struct w32Gbl_ProductId;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ProductId_ext_ID_EXT(r32)               _BFGET_(r32, 7, 0)
    #define   SET32Gbl_ProductId_ext_ID_EXT(r32,v)             _BFSET_(r32, 7, 0,v)
    #define   GET16Gbl_ProductId_ext_ID_EXT(r16)               _BFGET_(r16, 7, 0)
    #define   SET16Gbl_ProductId_ext_ID_EXT(r16,v)             _BFSET_(r16, 7, 0,v)

    #define     w32Gbl_ProductId_ext                           {\
            UNSG32 uProductId_ext_ID_EXT                       :  8;\
            UNSG32 RSVDx108_b8                                 : 24;\
          }
    union { UNSG32 u32Gbl_ProductId_ext;
            struct w32Gbl_ProductId_ext;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_FPGAR_FPGAR(r32)                        _BFGET_(r32,31, 0)
    #define   SET32Gbl_FPGAR_FPGAR(r32,v)                      _BFSET_(r32,31, 0,v)

    #define     w32Gbl_FPGAR                                   {\
            UNSG32 uFPGAR_FPGAR                                : 32;\
          }
    union { UNSG32 u32Gbl_FPGAR;
            struct w32Gbl_FPGAR;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_FPGARW_FPGARW(r32)                      _BFGET_(r32,31, 0)
    #define   SET32Gbl_FPGARW_FPGARW(r32,v)                    _BFSET_(r32,31, 0,v)

    #define     w32Gbl_FPGARW                                  {\
            UNSG32 uFPGARW_FPGARW                              : 32;\
          }
    union { UNSG32 u32Gbl_FPGARW;
            struct w32Gbl_FPGARW;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RingOscCtl_centerInit(r32)              _BFGET_(r32, 0, 0)
    #define   SET32Gbl_RingOscCtl_centerInit(r32,v)            _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_RingOscCtl_centerInit(r16)              _BFGET_(r16, 0, 0)
    #define   SET16Gbl_RingOscCtl_centerInit(r16,v)            _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_RingOscCtl_avioTopInit(r32)             _BFGET_(r32, 1, 1)
    #define   SET32Gbl_RingOscCtl_avioTopInit(r32,v)           _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_RingOscCtl_avioTopInit(r16)             _BFGET_(r16, 1, 1)
    #define   SET16Gbl_RingOscCtl_avioTopInit(r16,v)           _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_RingOscCtl_avioBotInit(r32)             _BFGET_(r32, 2, 2)
    #define   SET32Gbl_RingOscCtl_avioBotInit(r32,v)           _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_RingOscCtl_avioBotInit(r16)             _BFGET_(r16, 2, 2)
    #define   SET16Gbl_RingOscCtl_avioBotInit(r16,v)           _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_RingOscCtl_usbTopInit(r32)              _BFGET_(r32, 3, 3)
    #define   SET32Gbl_RingOscCtl_usbTopInit(r32,v)            _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_RingOscCtl_usbTopInit(r16)              _BFGET_(r16, 3, 3)
    #define   SET16Gbl_RingOscCtl_usbTopInit(r16,v)            _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_RingOscCtl_cpu1TopInit(r32)             _BFGET_(r32, 4, 4)
    #define   SET32Gbl_RingOscCtl_cpu1TopInit(r32,v)           _BFSET_(r32, 4, 4,v)
    #define   GET16Gbl_RingOscCtl_cpu1TopInit(r16)             _BFGET_(r16, 4, 4)
    #define   SET16Gbl_RingOscCtl_cpu1TopInit(r16,v)           _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_RingOscCtl_cpu0TopInit(r32)             _BFGET_(r32, 5, 5)
    #define   SET32Gbl_RingOscCtl_cpu0TopInit(r32,v)           _BFSET_(r32, 5, 5,v)
    #define   GET16Gbl_RingOscCtl_cpu0TopInit(r16)             _BFGET_(r16, 5, 5)
    #define   SET16Gbl_RingOscCtl_cpu0TopInit(r16,v)           _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_RingOscCtl_cpuBotInit(r32)              _BFGET_(r32, 6, 6)
    #define   SET32Gbl_RingOscCtl_cpuBotInit(r32,v)            _BFSET_(r32, 6, 6,v)
    #define   GET16Gbl_RingOscCtl_cpuBotInit(r16)              _BFGET_(r16, 6, 6)
    #define   SET16Gbl_RingOscCtl_cpuBotInit(r16,v)            _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_RingOscCtl_topRiteInit(r32)             _BFGET_(r32, 7, 7)
    #define   SET32Gbl_RingOscCtl_topRiteInit(r32,v)           _BFSET_(r32, 7, 7,v)
    #define   GET16Gbl_RingOscCtl_topRiteInit(r16)             _BFGET_(r16, 7, 7)
    #define   SET16Gbl_RingOscCtl_topRiteInit(r16,v)           _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_RingOscCtl_midRiteInit(r32)             _BFGET_(r32, 8, 8)
    #define   SET32Gbl_RingOscCtl_midRiteInit(r32,v)           _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_RingOscCtl_midRiteInit(r16)             _BFGET_(r16, 8, 8)
    #define   SET16Gbl_RingOscCtl_midRiteInit(r16,v)           _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_RingOscCtl_botRiteInit(r32)             _BFGET_(r32, 9, 9)
    #define   SET32Gbl_RingOscCtl_botRiteInit(r32,v)           _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_RingOscCtl_botRiteInit(r16)             _BFGET_(r16, 9, 9)
    #define   SET16Gbl_RingOscCtl_botRiteInit(r16,v)           _BFSET_(r16, 9, 9,v)

    #define     w32Gbl_RingOscCtl                              {\
            UNSG32 uRingOscCtl_centerInit                      :  1;\
            UNSG32 uRingOscCtl_avioTopInit                     :  1;\
            UNSG32 uRingOscCtl_avioBotInit                     :  1;\
            UNSG32 uRingOscCtl_usbTopInit                      :  1;\
            UNSG32 uRingOscCtl_cpu1TopInit                     :  1;\
            UNSG32 uRingOscCtl_cpu0TopInit                     :  1;\
            UNSG32 uRingOscCtl_cpuBotInit                      :  1;\
            UNSG32 uRingOscCtl_topRiteInit                     :  1;\
            UNSG32 uRingOscCtl_midRiteInit                     :  1;\
            UNSG32 uRingOscCtl_botRiteInit                     :  1;\
            UNSG32 RSVDx114_b10                                : 22;\
          }
    union { UNSG32 u32Gbl_RingOscCtl;
            struct w32Gbl_RingOscCtl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_PLLBypsBootStrapEn_sysPLLBypsEn(r32)    _BFGET_(r32, 0, 0)
    #define   SET32Gbl_PLLBypsBootStrapEn_sysPLLBypsEn(r32,v)  _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_PLLBypsBootStrapEn_sysPLLBypsEn(r16)    _BFGET_(r16, 0, 0)
    #define   SET16Gbl_PLLBypsBootStrapEn_sysPLLBypsEn(r16,v)  _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_PLLBypsBootStrapEn_memPLLBypsEn(r32)    _BFGET_(r32, 1, 1)
    #define   SET32Gbl_PLLBypsBootStrapEn_memPLLBypsEn(r32,v)  _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_PLLBypsBootStrapEn_memPLLBypsEn(r16)    _BFGET_(r16, 1, 1)
    #define   SET16Gbl_PLLBypsBootStrapEn_memPLLBypsEn(r16,v)  _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_PLLBypsBootStrapEn_cpu1PLLBypsEn(r32)   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_PLLBypsBootStrapEn_cpu1PLLBypsEn(r32,v) _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_PLLBypsBootStrapEn_cpu1PLLBypsEn(r16)   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_PLLBypsBootStrapEn_cpu1PLLBypsEn(r16,v) _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_PLLBypsBootStrapEn                      {\
            UNSG32 uPLLBypsBootStrapEn_sysPLLBypsEn            :  1;\
            UNSG32 uPLLBypsBootStrapEn_memPLLBypsEn            :  1;\
            UNSG32 uPLLBypsBootStrapEn_cpu1PLLBypsEn           :  1;\
            UNSG32 RSVDx118_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_PLLBypsBootStrapEn;
            struct w32Gbl_PLLBypsBootStrapEn;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_IO_CALIBRATE_IN_DO(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_IO_CALIBRATE_IN_DO(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_IO_CALIBRATE_IN_DO(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_IO_CALIBRATE_IN_DO(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_IO_CALIBRATE_IN_CAL_EN(r32)             _BFGET_(r32, 1, 1)
    #define   SET32Gbl_IO_CALIBRATE_IN_CAL_EN(r32,v)           _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_IO_CALIBRATE_IN_CAL_EN(r16)             _BFGET_(r16, 1, 1)
    #define   SET16Gbl_IO_CALIBRATE_IN_CAL_EN(r16,v)           _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_IO_CALIBRATE_IN_ZP_CAL(r32)             _BFGET_(r32, 4, 2)
    #define   SET32Gbl_IO_CALIBRATE_IN_ZP_CAL(r32,v)           _BFSET_(r32, 4, 2,v)
    #define   GET16Gbl_IO_CALIBRATE_IN_ZP_CAL(r16)             _BFGET_(r16, 4, 2)
    #define   SET16Gbl_IO_CALIBRATE_IN_ZP_CAL(r16,v)           _BFSET_(r16, 4, 2,v)

    #define   GET32Gbl_IO_CALIBRATE_IN_ZN_CAL(r32)             _BFGET_(r32, 7, 5)
    #define   SET32Gbl_IO_CALIBRATE_IN_ZN_CAL(r32,v)           _BFSET_(r32, 7, 5,v)
    #define   GET16Gbl_IO_CALIBRATE_IN_ZN_CAL(r16)             _BFGET_(r16, 7, 5)
    #define   SET16Gbl_IO_CALIBRATE_IN_ZN_CAL(r16,v)           _BFSET_(r16, 7, 5,v)

    #define   GET32Gbl_IO_CALIBRATE_IN_RON_ADJ(r32)            _BFGET_(r32,10, 8)
    #define   SET32Gbl_IO_CALIBRATE_IN_RON_ADJ(r32,v)          _BFSET_(r32,10, 8,v)
    #define   GET16Gbl_IO_CALIBRATE_IN_RON_ADJ(r16)            _BFGET_(r16,10, 8)
    #define   SET16Gbl_IO_CALIBRATE_IN_RON_ADJ(r16,v)          _BFSET_(r16,10, 8,v)

    #define     w32Gbl_IO_CALIBRATE_IN                         {\
            UNSG32 uIO_CALIBRATE_IN_DO                         :  1;\
            UNSG32 uIO_CALIBRATE_IN_CAL_EN                     :  1;\
            UNSG32 uIO_CALIBRATE_IN_ZP_CAL                     :  3;\
            UNSG32 uIO_CALIBRATE_IN_ZN_CAL                     :  3;\
            UNSG32 uIO_CALIBRATE_IN_RON_ADJ                    :  3;\
            UNSG32 RSVDx11C_b11                                : 21;\
          }
    union { UNSG32 u32Gbl_IO_CALIBRATE_IN;
            struct w32Gbl_IO_CALIBRATE_IN;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_IO_CALBRATE_OUT_INC(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_IO_CALBRATE_OUT_INC(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_IO_CALBRATE_OUT_INC(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_IO_CALBRATE_OUT_INC(r16,v)              _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_IO_CALBRATE_OUT                         {\
            UNSG32 uIO_CALBRATE_OUT_INC                        :  1;\
            UNSG32 RSVDx120_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_IO_CALBRATE_OUT;
            struct w32Gbl_IO_CALBRATE_OUT;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_SECURE_SCAN_EN_SET(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_SECURE_SCAN_EN_SET(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_SECURE_SCAN_EN_SET(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_SECURE_SCAN_EN_SET(r16,v)               _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_SECURE_SCAN_EN                          {\
            UNSG32 uSECURE_SCAN_EN_SET                         :  1;\
            UNSG32 RSVDx124_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_SECURE_SCAN_EN;
            struct w32Gbl_SECURE_SCAN_EN;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NandWPn_Sel(r32)                        _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NandWPn_Sel(r32,v)                      _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NandWPn_Sel(r16)                        _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NandWPn_Sel(r16,v)                      _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_NandWPn                                 {\
            UNSG32 uNandWPn_Sel                                :  1;\
            UNSG32 RSVDx128_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_NandWPn;
            struct w32Gbl_NandWPn;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RingCntCntl_centerStart(r32)            _BFGET_(r32, 0, 0)
    #define   SET32Gbl_RingCntCntl_centerStart(r32,v)          _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_RingCntCntl_centerStart(r16)            _BFGET_(r16, 0, 0)
    #define   SET16Gbl_RingCntCntl_centerStart(r16,v)          _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_RingCntCntl_centerStop(r32)             _BFGET_(r32, 1, 1)
    #define   SET32Gbl_RingCntCntl_centerStop(r32,v)           _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_RingCntCntl_centerStop(r16)             _BFGET_(r16, 1, 1)
    #define   SET16Gbl_RingCntCntl_centerStop(r16,v)           _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_RingCntCntl_avioTopStart(r32)           _BFGET_(r32, 2, 2)
    #define   SET32Gbl_RingCntCntl_avioTopStart(r32,v)         _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_RingCntCntl_avioTopStart(r16)           _BFGET_(r16, 2, 2)
    #define   SET16Gbl_RingCntCntl_avioTopStart(r16,v)         _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_RingCntCntl_avioTopStop(r32)            _BFGET_(r32, 3, 3)
    #define   SET32Gbl_RingCntCntl_avioTopStop(r32,v)          _BFSET_(r32, 3, 3,v)
    #define   GET16Gbl_RingCntCntl_avioTopStop(r16)            _BFGET_(r16, 3, 3)
    #define   SET16Gbl_RingCntCntl_avioTopStop(r16,v)          _BFSET_(r16, 3, 3,v)

    #define   GET32Gbl_RingCntCntl_avioBotStart(r32)           _BFGET_(r32, 4, 4)
    #define   SET32Gbl_RingCntCntl_avioBotStart(r32,v)         _BFSET_(r32, 4, 4,v)
    #define   GET16Gbl_RingCntCntl_avioBotStart(r16)           _BFGET_(r16, 4, 4)
    #define   SET16Gbl_RingCntCntl_avioBotStart(r16,v)         _BFSET_(r16, 4, 4,v)

    #define   GET32Gbl_RingCntCntl_avioBotStop(r32)            _BFGET_(r32, 5, 5)
    #define   SET32Gbl_RingCntCntl_avioBotStop(r32,v)          _BFSET_(r32, 5, 5,v)
    #define   GET16Gbl_RingCntCntl_avioBotStop(r16)            _BFGET_(r16, 5, 5)
    #define   SET16Gbl_RingCntCntl_avioBotStop(r16,v)          _BFSET_(r16, 5, 5,v)

    #define   GET32Gbl_RingCntCntl_usbTopStart(r32)            _BFGET_(r32, 6, 6)
    #define   SET32Gbl_RingCntCntl_usbTopStart(r32,v)          _BFSET_(r32, 6, 6,v)
    #define   GET16Gbl_RingCntCntl_usbTopStart(r16)            _BFGET_(r16, 6, 6)
    #define   SET16Gbl_RingCntCntl_usbTopStart(r16,v)          _BFSET_(r16, 6, 6,v)

    #define   GET32Gbl_RingCntCntl_usbTopStop(r32)             _BFGET_(r32, 7, 7)
    #define   SET32Gbl_RingCntCntl_usbTopStop(r32,v)           _BFSET_(r32, 7, 7,v)
    #define   GET16Gbl_RingCntCntl_usbTopStop(r16)             _BFGET_(r16, 7, 7)
    #define   SET16Gbl_RingCntCntl_usbTopStop(r16,v)           _BFSET_(r16, 7, 7,v)

    #define   GET32Gbl_RingCntCntl_cpu1TopStart(r32)           _BFGET_(r32, 8, 8)
    #define   SET32Gbl_RingCntCntl_cpu1TopStart(r32,v)         _BFSET_(r32, 8, 8,v)
    #define   GET16Gbl_RingCntCntl_cpu1TopStart(r16)           _BFGET_(r16, 8, 8)
    #define   SET16Gbl_RingCntCntl_cpu1TopStart(r16,v)         _BFSET_(r16, 8, 8,v)

    #define   GET32Gbl_RingCntCntl_cpu1TopStop(r32)            _BFGET_(r32, 9, 9)
    #define   SET32Gbl_RingCntCntl_cpu1TopStop(r32,v)          _BFSET_(r32, 9, 9,v)
    #define   GET16Gbl_RingCntCntl_cpu1TopStop(r16)            _BFGET_(r16, 9, 9)
    #define   SET16Gbl_RingCntCntl_cpu1TopStop(r16,v)          _BFSET_(r16, 9, 9,v)

    #define   GET32Gbl_RingCntCntl_cpu0TopStart(r32)           _BFGET_(r32,10,10)
    #define   SET32Gbl_RingCntCntl_cpu0TopStart(r32,v)         _BFSET_(r32,10,10,v)
    #define   GET16Gbl_RingCntCntl_cpu0TopStart(r16)           _BFGET_(r16,10,10)
    #define   SET16Gbl_RingCntCntl_cpu0TopStart(r16,v)         _BFSET_(r16,10,10,v)

    #define   GET32Gbl_RingCntCntl_cpu0TopStop(r32)            _BFGET_(r32,11,11)
    #define   SET32Gbl_RingCntCntl_cpu0TopStop(r32,v)          _BFSET_(r32,11,11,v)
    #define   GET16Gbl_RingCntCntl_cpu0TopStop(r16)            _BFGET_(r16,11,11)
    #define   SET16Gbl_RingCntCntl_cpu0TopStop(r16,v)          _BFSET_(r16,11,11,v)

    #define   GET32Gbl_RingCntCntl_cpuBotStart(r32)            _BFGET_(r32,12,12)
    #define   SET32Gbl_RingCntCntl_cpuBotStart(r32,v)          _BFSET_(r32,12,12,v)
    #define   GET16Gbl_RingCntCntl_cpuBotStart(r16)            _BFGET_(r16,12,12)
    #define   SET16Gbl_RingCntCntl_cpuBotStart(r16,v)          _BFSET_(r16,12,12,v)

    #define   GET32Gbl_RingCntCntl_cpuBotStop(r32)             _BFGET_(r32,13,13)
    #define   SET32Gbl_RingCntCntl_cpuBotStop(r32,v)           _BFSET_(r32,13,13,v)
    #define   GET16Gbl_RingCntCntl_cpuBotStop(r16)             _BFGET_(r16,13,13)
    #define   SET16Gbl_RingCntCntl_cpuBotStop(r16,v)           _BFSET_(r16,13,13,v)

    #define   GET32Gbl_RingCntCntl_topRiteStart(r32)           _BFGET_(r32,14,14)
    #define   SET32Gbl_RingCntCntl_topRiteStart(r32,v)         _BFSET_(r32,14,14,v)
    #define   GET16Gbl_RingCntCntl_topRiteStart(r16)           _BFGET_(r16,14,14)
    #define   SET16Gbl_RingCntCntl_topRiteStart(r16,v)         _BFSET_(r16,14,14,v)

    #define   GET32Gbl_RingCntCntl_topRiteStop(r32)            _BFGET_(r32,15,15)
    #define   SET32Gbl_RingCntCntl_topRiteStop(r32,v)          _BFSET_(r32,15,15,v)
    #define   GET16Gbl_RingCntCntl_topRiteStop(r16)            _BFGET_(r16,15,15)
    #define   SET16Gbl_RingCntCntl_topRiteStop(r16,v)          _BFSET_(r16,15,15,v)

    #define   GET32Gbl_RingCntCntl_midRiteStart(r32)           _BFGET_(r32,16,16)
    #define   SET32Gbl_RingCntCntl_midRiteStart(r32,v)         _BFSET_(r32,16,16,v)
    #define   GET16Gbl_RingCntCntl_midRiteStart(r16)           _BFGET_(r16, 0, 0)
    #define   SET16Gbl_RingCntCntl_midRiteStart(r16,v)         _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_RingCntCntl_midRiteStop(r32)            _BFGET_(r32,17,17)
    #define   SET32Gbl_RingCntCntl_midRiteStop(r32,v)          _BFSET_(r32,17,17,v)
    #define   GET16Gbl_RingCntCntl_midRiteStop(r16)            _BFGET_(r16, 1, 1)
    #define   SET16Gbl_RingCntCntl_midRiteStop(r16,v)          _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_RingCntCntl_botRiteStart(r32)           _BFGET_(r32,18,18)
    #define   SET32Gbl_RingCntCntl_botRiteStart(r32,v)         _BFSET_(r32,18,18,v)
    #define   GET16Gbl_RingCntCntl_botRiteStart(r16)           _BFGET_(r16, 2, 2)
    #define   SET16Gbl_RingCntCntl_botRiteStart(r16,v)         _BFSET_(r16, 2, 2,v)

    #define   GET32Gbl_RingCntCntl_botRiteStop(r32)            _BFGET_(r32,19,19)
    #define   SET32Gbl_RingCntCntl_botRiteStop(r32,v)          _BFSET_(r32,19,19,v)
    #define   GET16Gbl_RingCntCntl_botRiteStop(r16)            _BFGET_(r16, 3, 3)
    #define   SET16Gbl_RingCntCntl_botRiteStop(r16,v)          _BFSET_(r16, 3, 3,v)

    #define     w32Gbl_RingCntCntl                             {\
            UNSG32 uRingCntCntl_centerStart                    :  1;\
            UNSG32 uRingCntCntl_centerStop                     :  1;\
            UNSG32 uRingCntCntl_avioTopStart                   :  1;\
            UNSG32 uRingCntCntl_avioTopStop                    :  1;\
            UNSG32 uRingCntCntl_avioBotStart                   :  1;\
            UNSG32 uRingCntCntl_avioBotStop                    :  1;\
            UNSG32 uRingCntCntl_usbTopStart                    :  1;\
            UNSG32 uRingCntCntl_usbTopStop                     :  1;\
            UNSG32 uRingCntCntl_cpu1TopStart                   :  1;\
            UNSG32 uRingCntCntl_cpu1TopStop                    :  1;\
            UNSG32 uRingCntCntl_cpu0TopStart                   :  1;\
            UNSG32 uRingCntCntl_cpu0TopStop                    :  1;\
            UNSG32 uRingCntCntl_cpuBotStart                    :  1;\
            UNSG32 uRingCntCntl_cpuBotStop                     :  1;\
            UNSG32 uRingCntCntl_topRiteStart                   :  1;\
            UNSG32 uRingCntCntl_topRiteStop                    :  1;\
            UNSG32 uRingCntCntl_midRiteStart                   :  1;\
            UNSG32 uRingCntCntl_midRiteStop                    :  1;\
            UNSG32 uRingCntCntl_botRiteStart                   :  1;\
            UNSG32 uRingCntCntl_botRiteStop                    :  1;\
            UNSG32 RSVDx12C_b20                                : 12;\
          }
    union { UNSG32 u32Gbl_RingCntCntl;
            struct w32Gbl_RingCntCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_CenterCnt_CNTVALUE(r32)                 _BFGET_(r32,15, 0)
    #define   SET32Gbl_CenterCnt_CNTVALUE(r32,v)               _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_CenterCnt_CNTVALUE(r16)                 _BFGET_(r16,15, 0)
    #define   SET16Gbl_CenterCnt_CNTVALUE(r16,v)               _BFSET_(r16,15, 0,v)

    #define     w32Gbl_CenterCnt                               {\
            UNSG32 uCenterCnt_CNTVALUE                         : 16;\
            UNSG32 RSVDx130_b16                                : 16;\
          }
    union { UNSG32 u32Gbl_CenterCnt;
            struct w32Gbl_CenterCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_avioTopCnt_CNTVALUE(r32)                _BFGET_(r32,15, 0)
    #define   SET32Gbl_avioTopCnt_CNTVALUE(r32,v)              _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_avioTopCnt_CNTVALUE(r16)                _BFGET_(r16,15, 0)
    #define   SET16Gbl_avioTopCnt_CNTVALUE(r16,v)              _BFSET_(r16,15, 0,v)

    #define     w32Gbl_avioTopCnt                              {\
            UNSG32 uavioTopCnt_CNTVALUE                        : 16;\
            UNSG32 RSVDx134_b16                                : 16;\
          }
    union { UNSG32 u32Gbl_avioTopCnt;
            struct w32Gbl_avioTopCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_avioBotCnt_CNTVALUE(r32)                _BFGET_(r32,15, 0)
    #define   SET32Gbl_avioBotCnt_CNTVALUE(r32,v)              _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_avioBotCnt_CNTVALUE(r16)                _BFGET_(r16,15, 0)
    #define   SET16Gbl_avioBotCnt_CNTVALUE(r16,v)              _BFSET_(r16,15, 0,v)

    #define     w32Gbl_avioBotCnt                              {\
            UNSG32 uavioBotCnt_CNTVALUE                        : 16;\
            UNSG32 RSVDx138_b16                                : 16;\
          }
    union { UNSG32 u32Gbl_avioBotCnt;
            struct w32Gbl_avioBotCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_usbTopCnt_CNTVALUE(r32)                 _BFGET_(r32,15, 0)
    #define   SET32Gbl_usbTopCnt_CNTVALUE(r32,v)               _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_usbTopCnt_CNTVALUE(r16)                 _BFGET_(r16,15, 0)
    #define   SET16Gbl_usbTopCnt_CNTVALUE(r16,v)               _BFSET_(r16,15, 0,v)

    #define     w32Gbl_usbTopCnt                               {\
            UNSG32 uusbTopCnt_CNTVALUE                         : 16;\
            UNSG32 RSVDx13C_b16                                : 16;\
          }
    union { UNSG32 u32Gbl_usbTopCnt;
            struct w32Gbl_usbTopCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_cpu1TopCnt_CNTVALUE(r32)                _BFGET_(r32,15, 0)
    #define   SET32Gbl_cpu1TopCnt_CNTVALUE(r32,v)              _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_cpu1TopCnt_CNTVALUE(r16)                _BFGET_(r16,15, 0)
    #define   SET16Gbl_cpu1TopCnt_CNTVALUE(r16,v)              _BFSET_(r16,15, 0,v)

    #define     w32Gbl_cpu1TopCnt                              {\
            UNSG32 ucpu1TopCnt_CNTVALUE                        : 16;\
            UNSG32 RSVDx140_b16                                : 16;\
          }
    union { UNSG32 u32Gbl_cpu1TopCnt;
            struct w32Gbl_cpu1TopCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_cpu0TopCnt_CNTVALUE(r32)                _BFGET_(r32,15, 0)
    #define   SET32Gbl_cpu0TopCnt_CNTVALUE(r32,v)              _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_cpu0TopCnt_CNTVALUE(r16)                _BFGET_(r16,15, 0)
    #define   SET16Gbl_cpu0TopCnt_CNTVALUE(r16,v)              _BFSET_(r16,15, 0,v)

    #define     w32Gbl_cpu0TopCnt                              {\
            UNSG32 ucpu0TopCnt_CNTVALUE                        : 16;\
            UNSG32 RSVDx144_b16                                : 16;\
          }
    union { UNSG32 u32Gbl_cpu0TopCnt;
            struct w32Gbl_cpu0TopCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_cpuBotCnt_CNTVALUE(r32)                 _BFGET_(r32,15, 0)
    #define   SET32Gbl_cpuBotCnt_CNTVALUE(r32,v)               _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_cpuBotCnt_CNTVALUE(r16)                 _BFGET_(r16,15, 0)
    #define   SET16Gbl_cpuBotCnt_CNTVALUE(r16,v)               _BFSET_(r16,15, 0,v)

    #define     w32Gbl_cpuBotCnt                               {\
            UNSG32 ucpuBotCnt_CNTVALUE                         : 16;\
            UNSG32 RSVDx148_b16                                : 16;\
          }
    union { UNSG32 u32Gbl_cpuBotCnt;
            struct w32Gbl_cpuBotCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_topRiteCnt_CNTVALUE(r32)                _BFGET_(r32,15, 0)
    #define   SET32Gbl_topRiteCnt_CNTVALUE(r32,v)              _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_topRiteCnt_CNTVALUE(r16)                _BFGET_(r16,15, 0)
    #define   SET16Gbl_topRiteCnt_CNTVALUE(r16,v)              _BFSET_(r16,15, 0,v)

    #define     w32Gbl_topRiteCnt                              {\
            UNSG32 utopRiteCnt_CNTVALUE                        : 16;\
            UNSG32 RSVDx14C_b16                                : 16;\
          }
    union { UNSG32 u32Gbl_topRiteCnt;
            struct w32Gbl_topRiteCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_midRiteCnt_CNTVALUE(r32)                _BFGET_(r32,15, 0)
    #define   SET32Gbl_midRiteCnt_CNTVALUE(r32,v)              _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_midRiteCnt_CNTVALUE(r16)                _BFGET_(r16,15, 0)
    #define   SET16Gbl_midRiteCnt_CNTVALUE(r16,v)              _BFSET_(r16,15, 0,v)

    #define     w32Gbl_midRiteCnt                              {\
            UNSG32 umidRiteCnt_CNTVALUE                        : 16;\
            UNSG32 RSVDx150_b16                                : 16;\
          }
    union { UNSG32 u32Gbl_midRiteCnt;
            struct w32Gbl_midRiteCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_botRiteCnt_CNTVALUE(r32)                _BFGET_(r32,15, 0)
    #define   SET32Gbl_botRiteCnt_CNTVALUE(r32,v)              _BFSET_(r32,15, 0,v)
    #define   GET16Gbl_botRiteCnt_CNTVALUE(r16)                _BFGET_(r16,15, 0)
    #define   SET16Gbl_botRiteCnt_CNTVALUE(r16,v)              _BFSET_(r16,15, 0,v)

    #define     w32Gbl_botRiteCnt                              {\
            UNSG32 ubotRiteCnt_CNTVALUE                        : 16;\
            UNSG32 RSVDx154_b16                                : 16;\
          }
    union { UNSG32 u32Gbl_botRiteCnt;
            struct w32Gbl_botRiteCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_IO0Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_IO0Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_IO0Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_IO0Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_IO0Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_IO0Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_IO0Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_IO0Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_IO0Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_IO0Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_IO0Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_IO0Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_IO0Cntl                            {\
            UNSG32 uNAND_IO0Cntl_SLP_DI                        :  1;\
            UNSG32 uNAND_IO0Cntl_PU_EN                         :  1;\
            UNSG32 uNAND_IO0Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx158_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_IO0Cntl;
            struct w32Gbl_NAND_IO0Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_IO1Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_IO1Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_IO1Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_IO1Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_IO1Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_IO1Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_IO1Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_IO1Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_IO1Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_IO1Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_IO1Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_IO1Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_IO1Cntl                            {\
            UNSG32 uNAND_IO1Cntl_SLP_DI                        :  1;\
            UNSG32 uNAND_IO1Cntl_PU_EN                         :  1;\
            UNSG32 uNAND_IO1Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx15C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_IO1Cntl;
            struct w32Gbl_NAND_IO1Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_IO2Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_IO2Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_IO2Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_IO2Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_IO2Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_IO2Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_IO2Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_IO2Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_IO2Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_IO2Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_IO2Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_IO2Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_IO2Cntl                            {\
            UNSG32 uNAND_IO2Cntl_SLP_DI                        :  1;\
            UNSG32 uNAND_IO2Cntl_PU_EN                         :  1;\
            UNSG32 uNAND_IO2Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx160_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_IO2Cntl;
            struct w32Gbl_NAND_IO2Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_IO3Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_IO3Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_IO3Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_IO3Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_IO3Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_IO3Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_IO3Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_IO3Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_IO3Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_IO3Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_IO3Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_IO3Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_IO3Cntl                            {\
            UNSG32 uNAND_IO3Cntl_SLP_DI                        :  1;\
            UNSG32 uNAND_IO3Cntl_PU_EN                         :  1;\
            UNSG32 uNAND_IO3Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx164_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_IO3Cntl;
            struct w32Gbl_NAND_IO3Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_IO4Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_IO4Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_IO4Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_IO4Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_IO4Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_IO4Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_IO4Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_IO4Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_IO4Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_IO4Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_IO4Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_IO4Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_IO4Cntl                            {\
            UNSG32 uNAND_IO4Cntl_SLP_DI                        :  1;\
            UNSG32 uNAND_IO4Cntl_PU_EN                         :  1;\
            UNSG32 uNAND_IO4Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx168_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_IO4Cntl;
            struct w32Gbl_NAND_IO4Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_IO5Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_IO5Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_IO5Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_IO5Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_IO5Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_IO5Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_IO5Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_IO5Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_IO5Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_IO5Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_IO5Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_IO5Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_IO5Cntl                            {\
            UNSG32 uNAND_IO5Cntl_SLP_DI                        :  1;\
            UNSG32 uNAND_IO5Cntl_PU_EN                         :  1;\
            UNSG32 uNAND_IO5Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx16C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_IO5Cntl;
            struct w32Gbl_NAND_IO5Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_IO6Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_IO6Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_IO6Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_IO6Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_IO6Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_IO6Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_IO6Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_IO6Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_IO6Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_IO6Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_IO6Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_IO6Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_IO6Cntl                            {\
            UNSG32 uNAND_IO6Cntl_SLP_DI                        :  1;\
            UNSG32 uNAND_IO6Cntl_PU_EN                         :  1;\
            UNSG32 uNAND_IO6Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx170_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_IO6Cntl;
            struct w32Gbl_NAND_IO6Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_IO7Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_IO7Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_IO7Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_IO7Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_IO7Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_IO7Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_IO7Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_IO7Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_IO7Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_IO7Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_IO7Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_IO7Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_IO7Cntl                            {\
            UNSG32 uNAND_IO7Cntl_SLP_DI                        :  1;\
            UNSG32 uNAND_IO7Cntl_PU_EN                         :  1;\
            UNSG32 uNAND_IO7Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx174_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_IO7Cntl;
            struct w32Gbl_NAND_IO7Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_ALECntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_ALECntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_ALECntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_ALECntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_ALECntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_ALECntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_ALECntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_ALECntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_ALECntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_ALECntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_ALECntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_ALECntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_ALECntl                            {\
            UNSG32 uNAND_ALECntl_SLP_DI                        :  1;\
            UNSG32 uNAND_ALECntl_PU_EN                         :  1;\
            UNSG32 uNAND_ALECntl_PD_EN                         :  1;\
            UNSG32 RSVDx178_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_ALECntl;
            struct w32Gbl_NAND_ALECntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_CLECntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_CLECntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_CLECntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_CLECntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_CLECntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_CLECntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_CLECntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_CLECntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_CLECntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_CLECntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_CLECntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_CLECntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_CLECntl                            {\
            UNSG32 uNAND_CLECntl_SLP_DI                        :  1;\
            UNSG32 uNAND_CLECntl_PU_EN                         :  1;\
            UNSG32 uNAND_CLECntl_PD_EN                         :  1;\
            UNSG32 RSVDx17C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_CLECntl;
            struct w32Gbl_NAND_CLECntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_WEnCntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_WEnCntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_WEnCntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_WEnCntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_WEnCntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_WEnCntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_WEnCntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_WEnCntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_WEnCntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_WEnCntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_WEnCntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_WEnCntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_WEnCntl                            {\
            UNSG32 uNAND_WEnCntl_SLP_DI                        :  1;\
            UNSG32 uNAND_WEnCntl_PU_EN                         :  1;\
            UNSG32 uNAND_WEnCntl_PD_EN                         :  1;\
            UNSG32 RSVDx180_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_WEnCntl;
            struct w32Gbl_NAND_WEnCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_REnCntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_REnCntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_REnCntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_REnCntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_REnCntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_REnCntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_REnCntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_REnCntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_REnCntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_REnCntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_REnCntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_REnCntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_REnCntl                            {\
            UNSG32 uNAND_REnCntl_SLP_DI                        :  1;\
            UNSG32 uNAND_REnCntl_PU_EN                         :  1;\
            UNSG32 uNAND_REnCntl_PD_EN                         :  1;\
            UNSG32 RSVDx184_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_REnCntl;
            struct w32Gbl_NAND_REnCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_WPnCntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_WPnCntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_WPnCntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_WPnCntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_WPnCntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_WPnCntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_WPnCntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_WPnCntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_WPnCntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_WPnCntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_WPnCntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_WPnCntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_WPnCntl                            {\
            UNSG32 uNAND_WPnCntl_SLP_DI                        :  1;\
            UNSG32 uNAND_WPnCntl_PU_EN                         :  1;\
            UNSG32 uNAND_WPnCntl_PD_EN                         :  1;\
            UNSG32 RSVDx188_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_WPnCntl;
            struct w32Gbl_NAND_WPnCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_CEn0Cntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_CEn0Cntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_CEn0Cntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_CEn0Cntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_CEn0Cntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_CEn0Cntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_CEn0Cntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_CEn0Cntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_CEn0Cntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_CEn0Cntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_CEn0Cntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_CEn0Cntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_CEn0Cntl                           {\
            UNSG32 uNAND_CEn0Cntl_SLP_DI                       :  1;\
            UNSG32 uNAND_CEn0Cntl_PU_EN                        :  1;\
            UNSG32 uNAND_CEn0Cntl_PD_EN                        :  1;\
            UNSG32 RSVDx18C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_CEn0Cntl;
            struct w32Gbl_NAND_CEn0Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_CEn1Cntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_CEn1Cntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_CEn1Cntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_CEn1Cntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_NAND_CEn1Cntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_NAND_CEn1Cntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_NAND_CEn1Cntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_NAND_CEn1Cntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_NAND_CEn1Cntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_NAND_CEn1Cntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_NAND_CEn1Cntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_NAND_CEn1Cntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_NAND_CEn1Cntl                           {\
            UNSG32 uNAND_CEn1Cntl_SLP_DI                       :  1;\
            UNSG32 uNAND_CEn1Cntl_PU_EN                        :  1;\
            UNSG32 uNAND_CEn1Cntl_PD_EN                        :  1;\
            UNSG32 RSVDx190_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_NAND_CEn1Cntl;
            struct w32Gbl_NAND_CEn1Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_RDY0Cntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_RDY0Cntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_RDY0Cntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_RDY0Cntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_NAND_RDY0Cntl                           {\
            UNSG32 uNAND_RDY0Cntl_SLP_DI                       :  1;\
            UNSG32 RSVDx194_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_NAND_RDY0Cntl;
            struct w32Gbl_NAND_RDY0Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_NAND_RDY1Cntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_NAND_RDY1Cntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_NAND_RDY1Cntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_NAND_RDY1Cntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_NAND_RDY1Cntl                           {\
            UNSG32 uNAND_RDY1Cntl_SLP_DI                       :  1;\
            UNSG32 RSVDx198_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_NAND_RDY1Cntl;
            struct w32Gbl_NAND_RDY1Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_TXCLKIOCntl_SLP_DI(r32)              _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_TXCLKIOCntl_SLP_DI(r32,v)            _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_TXCLKIOCntl_SLP_DI(r16)              _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_TXCLKIOCntl_SLP_DI(r16,v)            _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_TXCLKIOCntl_PU_EN(r32)               _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_TXCLKIOCntl_PU_EN(r32,v)             _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_TXCLKIOCntl_PU_EN(r16)               _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_TXCLKIOCntl_PU_EN(r16,v)             _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_TXCLKIOCntl_PD_EN(r32)               _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_TXCLKIOCntl_PD_EN(r32,v)             _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_TXCLKIOCntl_PD_EN(r16)               _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_TXCLKIOCntl_PD_EN(r16,v)             _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_TXCLKIOCntl                          {\
            UNSG32 uET_TXCLKIOCntl_SLP_DI                      :  1;\
            UNSG32 uET_TXCLKIOCntl_PU_EN                       :  1;\
            UNSG32 uET_TXCLKIOCntl_PD_EN                       :  1;\
            UNSG32 RSVDx19C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_TXCLKIOCntl;
            struct w32Gbl_ET_TXCLKIOCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_TXD0Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_TXD0Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_TXD0Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_TXD0Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_TXD0Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_TXD0Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_TXD0Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_TXD0Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_TXD0Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_TXD0Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_TXD0Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_TXD0Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_TXD0Cntl                             {\
            UNSG32 uET_TXD0Cntl_SLP_DI                         :  1;\
            UNSG32 uET_TXD0Cntl_PU_EN                          :  1;\
            UNSG32 uET_TXD0Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx1A0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_TXD0Cntl;
            struct w32Gbl_ET_TXD0Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_TXD1Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_TXD1Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_TXD1Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_TXD1Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_TXD1Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_TXD1Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_TXD1Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_TXD1Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_TXD1Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_TXD1Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_TXD1Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_TXD1Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_TXD1Cntl                             {\
            UNSG32 uET_TXD1Cntl_SLP_DI                         :  1;\
            UNSG32 uET_TXD1Cntl_PU_EN                          :  1;\
            UNSG32 uET_TXD1Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx1A4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_TXD1Cntl;
            struct w32Gbl_ET_TXD1Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_TXD2Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_TXD2Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_TXD2Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_TXD2Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_TXD2Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_TXD2Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_TXD2Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_TXD2Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_TXD2Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_TXD2Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_TXD2Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_TXD2Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_TXD2Cntl                             {\
            UNSG32 uET_TXD2Cntl_SLP_DI                         :  1;\
            UNSG32 uET_TXD2Cntl_PU_EN                          :  1;\
            UNSG32 uET_TXD2Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx1A8_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_TXD2Cntl;
            struct w32Gbl_ET_TXD2Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_TXD3Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_TXD3Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_TXD3Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_TXD3Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_TXD3Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_TXD3Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_TXD3Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_TXD3Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_TXD3Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_TXD3Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_TXD3Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_TXD3Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_TXD3Cntl                             {\
            UNSG32 uET_TXD3Cntl_SLP_DI                         :  1;\
            UNSG32 uET_TXD3Cntl_PU_EN                          :  1;\
            UNSG32 uET_TXD3Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx1AC_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_TXD3Cntl;
            struct w32Gbl_ET_TXD3Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_TXENCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_TXENCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_TXENCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_TXENCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_TXENCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_TXENCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_TXENCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_TXENCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_TXENCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_TXENCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_TXENCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_TXENCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_TXENCntl                             {\
            UNSG32 uET_TXENCntl_SLP_DI                         :  1;\
            UNSG32 uET_TXENCntl_PU_EN                          :  1;\
            UNSG32 uET_TXENCntl_PD_EN                          :  1;\
            UNSG32 RSVDx1B0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_TXENCntl;
            struct w32Gbl_ET_TXENCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_TXERCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_TXERCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_TXERCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_TXERCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_TXERCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_TXERCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_TXERCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_TXERCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_TXERCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_TXERCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_TXERCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_TXERCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_TXERCntl                             {\
            UNSG32 uET_TXERCntl_SLP_DI                         :  1;\
            UNSG32 uET_TXERCntl_PU_EN                          :  1;\
            UNSG32 uET_TXERCntl_PD_EN                          :  1;\
            UNSG32 RSVDx1B4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_TXERCntl;
            struct w32Gbl_ET_TXERCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_RXCLKICntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_RXCLKICntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_RXCLKICntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_RXCLKICntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_RXCLKICntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_RXCLKICntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_RXCLKICntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_RXCLKICntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_RXCLKICntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_RXCLKICntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_RXCLKICntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_RXCLKICntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_RXCLKICntl                           {\
            UNSG32 uET_RXCLKICntl_SLP_DI                       :  1;\
            UNSG32 uET_RXCLKICntl_PU_EN                        :  1;\
            UNSG32 uET_RXCLKICntl_PD_EN                        :  1;\
            UNSG32 RSVDx1B8_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_RXCLKICntl;
            struct w32Gbl_ET_RXCLKICntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_RXD0Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_RXD0Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_RXD0Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_RXD0Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_RXD0Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_RXD0Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_RXD0Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_RXD0Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_RXD0Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_RXD0Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_RXD0Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_RXD0Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_RXD0Cntl                             {\
            UNSG32 uET_RXD0Cntl_SLP_DI                         :  1;\
            UNSG32 uET_RXD0Cntl_PU_EN                          :  1;\
            UNSG32 uET_RXD0Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx1BC_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_RXD0Cntl;
            struct w32Gbl_ET_RXD0Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_RXD1Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_RXD1Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_RXD1Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_RXD1Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_RXD1Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_RXD1Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_RXD1Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_RXD1Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_RXD1Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_RXD1Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_RXD1Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_RXD1Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_RXD1Cntl                             {\
            UNSG32 uET_RXD1Cntl_SLP_DI                         :  1;\
            UNSG32 uET_RXD1Cntl_PU_EN                          :  1;\
            UNSG32 uET_RXD1Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx1C0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_RXD1Cntl;
            struct w32Gbl_ET_RXD1Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_RXD2Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_RXD2Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_RXD2Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_RXD2Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_RXD2Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_RXD2Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_RXD2Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_RXD2Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_RXD2Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_RXD2Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_RXD2Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_RXD2Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_RXD2Cntl                             {\
            UNSG32 uET_RXD2Cntl_SLP_DI                         :  1;\
            UNSG32 uET_RXD2Cntl_PU_EN                          :  1;\
            UNSG32 uET_RXD2Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx1C4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_RXD2Cntl;
            struct w32Gbl_ET_RXD2Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_RXD3Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_RXD3Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_RXD3Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_RXD3Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_RXD3Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_RXD3Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_RXD3Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_RXD3Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_RXD3Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_RXD3Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_RXD3Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_RXD3Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_RXD3Cntl                             {\
            UNSG32 uET_RXD3Cntl_SLP_DI                         :  1;\
            UNSG32 uET_RXD3Cntl_PU_EN                          :  1;\
            UNSG32 uET_RXD3Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx1C8_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_RXD3Cntl;
            struct w32Gbl_ET_RXD3Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_CRSCntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_CRSCntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_CRSCntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_CRSCntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_CRSCntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_CRSCntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_CRSCntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_CRSCntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_CRSCntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_CRSCntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_CRSCntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_CRSCntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_CRSCntl                              {\
            UNSG32 uET_CRSCntl_SLP_DI                          :  1;\
            UNSG32 uET_CRSCntl_PU_EN                           :  1;\
            UNSG32 uET_CRSCntl_PD_EN                           :  1;\
            UNSG32 RSVDx1CC_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_CRSCntl;
            struct w32Gbl_ET_CRSCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_COLCntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_COLCntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_COLCntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_COLCntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_COLCntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_COLCntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_COLCntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_COLCntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_COLCntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_COLCntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_COLCntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_COLCntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_COLCntl                              {\
            UNSG32 uET_COLCntl_SLP_DI                          :  1;\
            UNSG32 uET_COLCntl_PU_EN                           :  1;\
            UNSG32 uET_COLCntl_PD_EN                           :  1;\
            UNSG32 RSVDx1D0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_COLCntl;
            struct w32Gbl_ET_COLCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_RXDVCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_RXDVCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_RXDVCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_RXDVCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_RXDVCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_RXDVCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_RXDVCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_RXDVCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_RXDVCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_RXDVCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_RXDVCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_RXDVCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_RXDVCntl                             {\
            UNSG32 uET_RXDVCntl_SLP_DI                         :  1;\
            UNSG32 uET_RXDVCntl_PU_EN                          :  1;\
            UNSG32 uET_RXDVCntl_PD_EN                          :  1;\
            UNSG32 RSVDx1D4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_RXDVCntl;
            struct w32Gbl_ET_RXDVCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_RXERCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_RXERCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_RXERCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_RXERCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_RXERCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_RXERCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_RXERCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_RXERCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_RXERCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_RXERCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_RXERCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_RXERCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_RXERCntl                             {\
            UNSG32 uET_RXERCntl_SLP_DI                         :  1;\
            UNSG32 uET_RXERCntl_PU_EN                          :  1;\
            UNSG32 uET_RXERCntl_PD_EN                          :  1;\
            UNSG32 RSVDx1D8_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_RXERCntl;
            struct w32Gbl_ET_RXERCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_MDCCntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_MDCCntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_MDCCntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_MDCCntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_MDCCntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_MDCCntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_MDCCntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_MDCCntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_MDCCntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_MDCCntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_MDCCntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_MDCCntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_MDCCntl                              {\
            UNSG32 uET_MDCCntl_SLP_DI                          :  1;\
            UNSG32 uET_MDCCntl_PU_EN                           :  1;\
            UNSG32 uET_MDCCntl_PD_EN                           :  1;\
            UNSG32 RSVDx1DC_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_MDCCntl;
            struct w32Gbl_ET_MDCCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_MDIOCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_MDIOCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_MDIOCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_MDIOCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_MDIOCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_MDIOCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_MDIOCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_MDIOCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_MDIOCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_MDIOCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_MDIOCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_MDIOCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_MDIOCntl                             {\
            UNSG32 uET_MDIOCntl_SLP_DI                         :  1;\
            UNSG32 uET_MDIOCntl_PU_EN                          :  1;\
            UNSG32 uET_MDIOCntl_PD_EN                          :  1;\
            UNSG32 RSVDx1E0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_MDIOCntl;
            struct w32Gbl_ET_MDIOCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ET_RCLKOCntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ET_RCLKOCntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ET_RCLKOCntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ET_RCLKOCntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_ET_RCLKOCntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_ET_RCLKOCntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_ET_RCLKOCntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_ET_RCLKOCntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_ET_RCLKOCntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_ET_RCLKOCntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_ET_RCLKOCntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_ET_RCLKOCntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_ET_RCLKOCntl                            {\
            UNSG32 uET_RCLKOCntl_SLP_DI                        :  1;\
            UNSG32 uET_RCLKOCntl_PU_EN                         :  1;\
            UNSG32 uET_RCLKOCntl_PD_EN                         :  1;\
            UNSG32 RSVDx1E4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_ET_RCLKOCntl;
            struct w32Gbl_ET_RCLKOCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TW0_SCLCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TW0_SCLCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TW0_SCLCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TW0_SCLCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TW0_SCLCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TW0_SCLCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TW0_SCLCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TW0_SCLCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TW0_SCLCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TW0_SCLCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TW0_SCLCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TW0_SCLCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TW0_SCLCntl                             {\
            UNSG32 uTW0_SCLCntl_SLP_DI                         :  1;\
            UNSG32 uTW0_SCLCntl_PU_EN                          :  1;\
            UNSG32 uTW0_SCLCntl_PD_EN                          :  1;\
            UNSG32 RSVDx1E8_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TW0_SCLCntl;
            struct w32Gbl_TW0_SCLCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TW0_SDACntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TW0_SDACntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TW0_SDACntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TW0_SDACntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TW0_SDACntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TW0_SDACntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TW0_SDACntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TW0_SDACntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TW0_SDACntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TW0_SDACntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TW0_SDACntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TW0_SDACntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TW0_SDACntl                             {\
            UNSG32 uTW0_SDACntl_SLP_DI                         :  1;\
            UNSG32 uTW0_SDACntl_PU_EN                          :  1;\
            UNSG32 uTW0_SDACntl_PD_EN                          :  1;\
            UNSG32 RSVDx1EC_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TW0_SDACntl;
            struct w32Gbl_TW0_SDACntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TW1_SCLCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TW1_SCLCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TW1_SCLCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TW1_SCLCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TW1_SCLCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TW1_SCLCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TW1_SCLCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TW1_SCLCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TW1_SCLCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TW1_SCLCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TW1_SCLCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TW1_SCLCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TW1_SCLCntl                             {\
            UNSG32 uTW1_SCLCntl_SLP_DI                         :  1;\
            UNSG32 uTW1_SCLCntl_PU_EN                          :  1;\
            UNSG32 uTW1_SCLCntl_PD_EN                          :  1;\
            UNSG32 RSVDx1F0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TW1_SCLCntl;
            struct w32Gbl_TW1_SCLCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TW1_SDACntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TW1_SDACntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TW1_SDACntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TW1_SDACntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TW1_SDACntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TW1_SDACntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TW1_SDACntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TW1_SDACntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TW1_SDACntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TW1_SDACntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TW1_SDACntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TW1_SDACntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TW1_SDACntl                             {\
            UNSG32 uTW1_SDACntl_SLP_DI                         :  1;\
            UNSG32 uTW1_SDACntl_PU_EN                          :  1;\
            UNSG32 uTW1_SDACntl_PD_EN                          :  1;\
            UNSG32 RSVDx1F4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TW1_SDACntl;
            struct w32Gbl_TW1_SDACntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_SPI1_SS0nCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_SPI1_SS0nCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_SPI1_SS0nCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_SPI1_SS0nCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_SPI1_SS0nCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_SPI1_SS0nCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_SPI1_SS0nCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_SPI1_SS0nCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_SPI1_SS0nCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_SPI1_SS0nCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_SPI1_SS0nCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_SPI1_SS0nCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_SPI1_SS0nCntl                           {\
            UNSG32 uSPI1_SS0nCntl_SLP_DI                       :  1;\
            UNSG32 uSPI1_SS0nCntl_PU_EN                        :  1;\
            UNSG32 uSPI1_SS0nCntl_PD_EN                        :  1;\
            UNSG32 RSVDx1F8_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_SPI1_SS0nCntl;
            struct w32Gbl_SPI1_SS0nCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_SPI1_SS1nCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_SPI1_SS1nCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_SPI1_SS1nCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_SPI1_SS1nCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_SPI1_SS1nCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_SPI1_SS1nCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_SPI1_SS1nCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_SPI1_SS1nCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_SPI1_SS1nCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_SPI1_SS1nCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_SPI1_SS1nCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_SPI1_SS1nCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_SPI1_SS1nCntl                           {\
            UNSG32 uSPI1_SS1nCntl_SLP_DI                       :  1;\
            UNSG32 uSPI1_SS1nCntl_PU_EN                        :  1;\
            UNSG32 uSPI1_SS1nCntl_PD_EN                        :  1;\
            UNSG32 RSVDx1FC_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_SPI1_SS1nCntl;
            struct w32Gbl_SPI1_SS1nCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_SPI1_SS2nCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_SPI1_SS2nCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_SPI1_SS2nCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_SPI1_SS2nCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_SPI1_SS2nCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_SPI1_SS2nCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_SPI1_SS2nCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_SPI1_SS2nCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_SPI1_SS2nCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_SPI1_SS2nCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_SPI1_SS2nCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_SPI1_SS2nCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_SPI1_SS2nCntl                           {\
            UNSG32 uSPI1_SS2nCntl_SLP_DI                       :  1;\
            UNSG32 uSPI1_SS2nCntl_PU_EN                        :  1;\
            UNSG32 uSPI1_SS2nCntl_PD_EN                        :  1;\
            UNSG32 RSVDx200_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_SPI1_SS2nCntl;
            struct w32Gbl_SPI1_SS2nCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_SPI1_SS3nCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_SPI1_SS3nCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_SPI1_SS3nCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_SPI1_SS3nCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_SPI1_SS3nCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_SPI1_SS3nCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_SPI1_SS3nCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_SPI1_SS3nCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_SPI1_SS3nCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_SPI1_SS3nCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_SPI1_SS3nCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_SPI1_SS3nCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_SPI1_SS3nCntl                           {\
            UNSG32 uSPI1_SS3nCntl_SLP_DI                       :  1;\
            UNSG32 uSPI1_SS3nCntl_PU_EN                        :  1;\
            UNSG32 uSPI1_SS3nCntl_PD_EN                        :  1;\
            UNSG32 RSVDx204_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_SPI1_SS3nCntl;
            struct w32Gbl_SPI1_SS3nCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_SPI1_SCLKCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_SPI1_SCLKCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_SPI1_SCLKCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_SPI1_SCLKCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_SPI1_SCLKCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_SPI1_SCLKCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_SPI1_SCLKCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_SPI1_SCLKCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_SPI1_SCLKCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_SPI1_SCLKCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_SPI1_SCLKCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_SPI1_SCLKCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_SPI1_SCLKCntl                           {\
            UNSG32 uSPI1_SCLKCntl_SLP_DI                       :  1;\
            UNSG32 uSPI1_SCLKCntl_PU_EN                        :  1;\
            UNSG32 uSPI1_SCLKCntl_PD_EN                        :  1;\
            UNSG32 RSVDx208_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_SPI1_SCLKCntl;
            struct w32Gbl_SPI1_SCLKCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_SPI1_SDOCntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_SPI1_SDOCntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_SPI1_SDOCntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_SPI1_SDOCntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_SPI1_SDOCntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_SPI1_SDOCntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_SPI1_SDOCntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_SPI1_SDOCntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_SPI1_SDOCntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_SPI1_SDOCntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_SPI1_SDOCntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_SPI1_SDOCntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_SPI1_SDOCntl                            {\
            UNSG32 uSPI1_SDOCntl_SLP_DI                        :  1;\
            UNSG32 uSPI1_SDOCntl_PU_EN                         :  1;\
            UNSG32 uSPI1_SDOCntl_PD_EN                         :  1;\
            UNSG32 RSVDx20C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_SPI1_SDOCntl;
            struct w32Gbl_SPI1_SDOCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_SPI1_SDICntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_SPI1_SDICntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_SPI1_SDICntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_SPI1_SDICntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_SPI1_SDICntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_SPI1_SDICntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_SPI1_SDICntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_SPI1_SDICntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_SPI1_SDICntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_SPI1_SDICntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_SPI1_SDICntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_SPI1_SDICntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_SPI1_SDICntl                            {\
            UNSG32 uSPI1_SDICntl_SLP_DI                        :  1;\
            UNSG32 uSPI1_SDICntl_PU_EN                         :  1;\
            UNSG32 uSPI1_SDICntl_PD_EN                         :  1;\
            UNSG32 RSVDx210_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_SPI1_SDICntl;
            struct w32Gbl_SPI1_SDICntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_CLKCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_CLKCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_CLKCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_CLKCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_CLKCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_CLKCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_CLKCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_CLKCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_CLKCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_CLKCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_CLKCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_CLKCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_CLKCntl                             {\
            UNSG32 uTS0_CLKCntl_SLP_DI                         :  1;\
            UNSG32 uTS0_CLKCntl_PU_EN                          :  1;\
            UNSG32 uTS0_CLKCntl_PD_EN                          :  1;\
            UNSG32 RSVDx214_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_CLKCntl;
            struct w32Gbl_TS0_CLKCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_SOPCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_SOPCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_SOPCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_SOPCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_SOPCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_SOPCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_SOPCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_SOPCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_SOPCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_SOPCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_SOPCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_SOPCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_SOPCntl                             {\
            UNSG32 uTS0_SOPCntl_SLP_DI                         :  1;\
            UNSG32 uTS0_SOPCntl_PU_EN                          :  1;\
            UNSG32 uTS0_SOPCntl_PD_EN                          :  1;\
            UNSG32 RSVDx218_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_SOPCntl;
            struct w32Gbl_TS0_SOPCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_ERRORCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_ERRORCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_ERRORCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_ERRORCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_ERRORCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_ERRORCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_ERRORCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_ERRORCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_ERRORCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_ERRORCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_ERRORCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_ERRORCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_ERRORCntl                           {\
            UNSG32 uTS0_ERRORCntl_SLP_DI                       :  1;\
            UNSG32 uTS0_ERRORCntl_PU_EN                        :  1;\
            UNSG32 uTS0_ERRORCntl_PD_EN                        :  1;\
            UNSG32 RSVDx21C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_ERRORCntl;
            struct w32Gbl_TS0_ERRORCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_VALDCntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_VALDCntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_VALDCntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_VALDCntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_VALDCntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_VALDCntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_VALDCntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_VALDCntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_VALDCntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_VALDCntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_VALDCntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_VALDCntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_VALDCntl                            {\
            UNSG32 uTS0_VALDCntl_SLP_DI                        :  1;\
            UNSG32 uTS0_VALDCntl_PU_EN                         :  1;\
            UNSG32 uTS0_VALDCntl_PD_EN                         :  1;\
            UNSG32 RSVDx220_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_VALDCntl;
            struct w32Gbl_TS0_VALDCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_PD0Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_PD0Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_PD0Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_PD0Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_PD0Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_PD0Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_PD0Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_PD0Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_PD0Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_PD0Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_PD0Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_PD0Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_PD0Cntl                             {\
            UNSG32 uTS0_PD0Cntl_SLP_DI                         :  1;\
            UNSG32 uTS0_PD0Cntl_PU_EN                          :  1;\
            UNSG32 uTS0_PD0Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx224_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_PD0Cntl;
            struct w32Gbl_TS0_PD0Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_PD1Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_PD1Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_PD1Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_PD1Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_PD1Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_PD1Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_PD1Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_PD1Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_PD1Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_PD1Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_PD1Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_PD1Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_PD1Cntl                             {\
            UNSG32 uTS0_PD1Cntl_SLP_DI                         :  1;\
            UNSG32 uTS0_PD1Cntl_PU_EN                          :  1;\
            UNSG32 uTS0_PD1Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx228_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_PD1Cntl;
            struct w32Gbl_TS0_PD1Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_PD2Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_PD2Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_PD2Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_PD2Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_PD2Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_PD2Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_PD2Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_PD2Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_PD2Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_PD2Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_PD2Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_PD2Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_PD2Cntl                             {\
            UNSG32 uTS0_PD2Cntl_SLP_DI                         :  1;\
            UNSG32 uTS0_PD2Cntl_PU_EN                          :  1;\
            UNSG32 uTS0_PD2Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx22C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_PD2Cntl;
            struct w32Gbl_TS0_PD2Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_PD3Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_PD3Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_PD3Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_PD3Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_PD3Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_PD3Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_PD3Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_PD3Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_PD3Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_PD3Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_PD3Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_PD3Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_PD3Cntl                             {\
            UNSG32 uTS0_PD3Cntl_SLP_DI                         :  1;\
            UNSG32 uTS0_PD3Cntl_PU_EN                          :  1;\
            UNSG32 uTS0_PD3Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx230_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_PD3Cntl;
            struct w32Gbl_TS0_PD3Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_PD4Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_PD4Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_PD4Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_PD4Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_PD4Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_PD4Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_PD4Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_PD4Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_PD4Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_PD4Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_PD4Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_PD4Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_PD4Cntl                             {\
            UNSG32 uTS0_PD4Cntl_SLP_DI                         :  1;\
            UNSG32 uTS0_PD4Cntl_PU_EN                          :  1;\
            UNSG32 uTS0_PD4Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx234_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_PD4Cntl;
            struct w32Gbl_TS0_PD4Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_PD5Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_PD5Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_PD5Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_PD5Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_PD5Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_PD5Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_PD5Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_PD5Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_PD5Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_PD5Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_PD5Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_PD5Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_PD5Cntl                             {\
            UNSG32 uTS0_PD5Cntl_SLP_DI                         :  1;\
            UNSG32 uTS0_PD5Cntl_PU_EN                          :  1;\
            UNSG32 uTS0_PD5Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx238_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_PD5Cntl;
            struct w32Gbl_TS0_PD5Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_PD6Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_PD6Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_PD6Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_PD6Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_PD6Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_PD6Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_PD6Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_PD6Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_PD6Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_PD6Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_PD6Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_PD6Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_PD6Cntl                             {\
            UNSG32 uTS0_PD6Cntl_SLP_DI                         :  1;\
            UNSG32 uTS0_PD6Cntl_PU_EN                          :  1;\
            UNSG32 uTS0_PD6Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx23C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_PD6Cntl;
            struct w32Gbl_TS0_PD6Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_TS0_PD7Cntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_TS0_PD7Cntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_TS0_PD7Cntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_TS0_PD7Cntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_TS0_PD7Cntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_TS0_PD7Cntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_TS0_PD7Cntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_TS0_PD7Cntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_TS0_PD7Cntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_TS0_PD7Cntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_TS0_PD7Cntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_TS0_PD7Cntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_TS0_PD7Cntl                             {\
            UNSG32 uTS0_PD7Cntl_SLP_DI                         :  1;\
            UNSG32 uTS0_PD7Cntl_PU_EN                          :  1;\
            UNSG32 uTS0_PD7Cntl_PD_EN                          :  1;\
            UNSG32 RSVDx240_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_TS0_PD7Cntl;
            struct w32Gbl_TS0_PD7Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_STS2_CLKCntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_STS2_CLKCntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_STS2_CLKCntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_STS2_CLKCntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_STS2_CLKCntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_STS2_CLKCntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_STS2_CLKCntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_STS2_CLKCntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_STS2_CLKCntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_STS2_CLKCntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_STS2_CLKCntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_STS2_CLKCntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_STS2_CLKCntl                            {\
            UNSG32 uSTS2_CLKCntl_SLP_DI                        :  1;\
            UNSG32 uSTS2_CLKCntl_PU_EN                         :  1;\
            UNSG32 uSTS2_CLKCntl_PD_EN                         :  1;\
            UNSG32 RSVDx244_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_STS2_CLKCntl;
            struct w32Gbl_STS2_CLKCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_STS2_ERRORCntl_SLP_DI(r32)              _BFGET_(r32, 0, 0)
    #define   SET32Gbl_STS2_ERRORCntl_SLP_DI(r32,v)            _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_STS2_ERRORCntl_SLP_DI(r16)              _BFGET_(r16, 0, 0)
    #define   SET16Gbl_STS2_ERRORCntl_SLP_DI(r16,v)            _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_STS2_ERRORCntl_PU_EN(r32)               _BFGET_(r32, 1, 1)
    #define   SET32Gbl_STS2_ERRORCntl_PU_EN(r32,v)             _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_STS2_ERRORCntl_PU_EN(r16)               _BFGET_(r16, 1, 1)
    #define   SET16Gbl_STS2_ERRORCntl_PU_EN(r16,v)             _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_STS2_ERRORCntl_PD_EN(r32)               _BFGET_(r32, 2, 2)
    #define   SET32Gbl_STS2_ERRORCntl_PD_EN(r32,v)             _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_STS2_ERRORCntl_PD_EN(r16)               _BFGET_(r16, 2, 2)
    #define   SET16Gbl_STS2_ERRORCntl_PD_EN(r16,v)             _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_STS2_ERRORCntl                          {\
            UNSG32 uSTS2_ERRORCntl_SLP_DI                      :  1;\
            UNSG32 uSTS2_ERRORCntl_PU_EN                       :  1;\
            UNSG32 uSTS2_ERRORCntl_PD_EN                       :  1;\
            UNSG32 RSVDx248_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_STS2_ERRORCntl;
            struct w32Gbl_STS2_ERRORCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_STS2_VALDCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_STS2_VALDCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_STS2_VALDCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_STS2_VALDCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_STS2_VALDCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_STS2_VALDCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_STS2_VALDCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_STS2_VALDCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_STS2_VALDCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_STS2_VALDCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_STS2_VALDCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_STS2_VALDCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_STS2_VALDCntl                           {\
            UNSG32 uSTS2_VALDCntl_SLP_DI                       :  1;\
            UNSG32 uSTS2_VALDCntl_PU_EN                        :  1;\
            UNSG32 uSTS2_VALDCntl_PD_EN                        :  1;\
            UNSG32 RSVDx24C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_STS2_VALDCntl;
            struct w32Gbl_STS2_VALDCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_STS2_SOPCntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_STS2_SOPCntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_STS2_SOPCntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_STS2_SOPCntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_STS2_SOPCntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_STS2_SOPCntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_STS2_SOPCntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_STS2_SOPCntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_STS2_SOPCntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_STS2_SOPCntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_STS2_SOPCntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_STS2_SOPCntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_STS2_SOPCntl                            {\
            UNSG32 uSTS2_SOPCntl_SLP_DI                        :  1;\
            UNSG32 uSTS2_SOPCntl_PU_EN                         :  1;\
            UNSG32 uSTS2_SOPCntl_PD_EN                         :  1;\
            UNSG32 RSVDx250_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_STS2_SOPCntl;
            struct w32Gbl_STS2_SOPCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_STS2_SDCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_STS2_SDCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_STS2_SDCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_STS2_SDCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_STS2_SDCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_STS2_SDCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_STS2_SDCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_STS2_SDCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_STS2_SDCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_STS2_SDCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_STS2_SDCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_STS2_SDCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_STS2_SDCntl                             {\
            UNSG32 uSTS2_SDCntl_SLP_DI                         :  1;\
            UNSG32 uSTS2_SDCntl_PU_EN                          :  1;\
            UNSG32 uSTS2_SDCntl_PD_EN                          :  1;\
            UNSG32 RSVDx254_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_STS2_SDCntl;
            struct w32Gbl_STS2_SDCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_RSTInCntl_SLP_DI(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32Gbl_RSTInCntl_SLP_DI(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_RSTInCntl_SLP_DI(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_RSTInCntl_SLP_DI(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_RSTInCntl                               {\
            UNSG32 uRSTInCntl_SLP_DI                           :  1;\
            UNSG32 RSVDx258_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_RSTInCntl;
            struct w32Gbl_RSTInCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_BYPS_OSCCntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_BYPS_OSCCntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_BYPS_OSCCntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_BYPS_OSCCntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_BYPS_OSCCntl                            {\
            UNSG32 uBYPS_OSCCntl_SLP_DI                        :  1;\
            UNSG32 RSVDx25C_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_BYPS_OSCCntl;
            struct w32Gbl_BYPS_OSCCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_ENG_ENCntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_ENG_ENCntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_ENG_ENCntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_ENG_ENCntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_ENG_ENCntl                              {\
            UNSG32 uENG_ENCntl_SLP_DI                          :  1;\
            UNSG32 RSVDx260_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_ENG_ENCntl;
            struct w32Gbl_ENG_ENCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO0Cntl_SLP_DI(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO0Cntl_SLP_DI(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO0Cntl_SLP_DI(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO0Cntl_SLP_DI(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO0Cntl_PU_EN(r32)                    _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO0Cntl_PU_EN(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO0Cntl_PU_EN(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO0Cntl_PU_EN(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO0Cntl_PD_EN(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO0Cntl_PD_EN(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO0Cntl_PD_EN(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO0Cntl_PD_EN(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO0Cntl                               {\
            UNSG32 uDVIO0Cntl_SLP_DI                           :  1;\
            UNSG32 uDVIO0Cntl_PU_EN                            :  1;\
            UNSG32 uDVIO0Cntl_PD_EN                            :  1;\
            UNSG32 RSVDx264_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO0Cntl;
            struct w32Gbl_DVIO0Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO1Cntl_SLP_DI(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO1Cntl_SLP_DI(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO1Cntl_SLP_DI(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO1Cntl_SLP_DI(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO1Cntl_PU_EN(r32)                    _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO1Cntl_PU_EN(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO1Cntl_PU_EN(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO1Cntl_PU_EN(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO1Cntl_PD_EN(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO1Cntl_PD_EN(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO1Cntl_PD_EN(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO1Cntl_PD_EN(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO1Cntl                               {\
            UNSG32 uDVIO1Cntl_SLP_DI                           :  1;\
            UNSG32 uDVIO1Cntl_PU_EN                            :  1;\
            UNSG32 uDVIO1Cntl_PD_EN                            :  1;\
            UNSG32 RSVDx268_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO1Cntl;
            struct w32Gbl_DVIO1Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO2Cntl_SLP_DI(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO2Cntl_SLP_DI(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO2Cntl_SLP_DI(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO2Cntl_SLP_DI(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO2Cntl_PU_EN(r32)                    _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO2Cntl_PU_EN(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO2Cntl_PU_EN(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO2Cntl_PU_EN(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO2Cntl_PD_EN(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO2Cntl_PD_EN(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO2Cntl_PD_EN(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO2Cntl_PD_EN(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO2Cntl                               {\
            UNSG32 uDVIO2Cntl_SLP_DI                           :  1;\
            UNSG32 uDVIO2Cntl_PU_EN                            :  1;\
            UNSG32 uDVIO2Cntl_PD_EN                            :  1;\
            UNSG32 RSVDx26C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO2Cntl;
            struct w32Gbl_DVIO2Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO3Cntl_SLP_DI(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO3Cntl_SLP_DI(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO3Cntl_SLP_DI(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO3Cntl_SLP_DI(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO3Cntl_PU_EN(r32)                    _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO3Cntl_PU_EN(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO3Cntl_PU_EN(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO3Cntl_PU_EN(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO3Cntl_PD_EN(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO3Cntl_PD_EN(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO3Cntl_PD_EN(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO3Cntl_PD_EN(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO3Cntl                               {\
            UNSG32 uDVIO3Cntl_SLP_DI                           :  1;\
            UNSG32 uDVIO3Cntl_PU_EN                            :  1;\
            UNSG32 uDVIO3Cntl_PD_EN                            :  1;\
            UNSG32 RSVDx270_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO3Cntl;
            struct w32Gbl_DVIO3Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO4Cntl_SLP_DI(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO4Cntl_SLP_DI(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO4Cntl_SLP_DI(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO4Cntl_SLP_DI(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO4Cntl_PU_EN(r32)                    _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO4Cntl_PU_EN(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO4Cntl_PU_EN(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO4Cntl_PU_EN(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO4Cntl_PD_EN(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO4Cntl_PD_EN(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO4Cntl_PD_EN(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO4Cntl_PD_EN(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO4Cntl                               {\
            UNSG32 uDVIO4Cntl_SLP_DI                           :  1;\
            UNSG32 uDVIO4Cntl_PU_EN                            :  1;\
            UNSG32 uDVIO4Cntl_PD_EN                            :  1;\
            UNSG32 RSVDx274_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO4Cntl;
            struct w32Gbl_DVIO4Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO5Cntl_SLP_DI(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO5Cntl_SLP_DI(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO5Cntl_SLP_DI(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO5Cntl_SLP_DI(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO5Cntl_PU_EN(r32)                    _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO5Cntl_PU_EN(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO5Cntl_PU_EN(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO5Cntl_PU_EN(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO5Cntl_PD_EN(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO5Cntl_PD_EN(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO5Cntl_PD_EN(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO5Cntl_PD_EN(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO5Cntl                               {\
            UNSG32 uDVIO5Cntl_SLP_DI                           :  1;\
            UNSG32 uDVIO5Cntl_PU_EN                            :  1;\
            UNSG32 uDVIO5Cntl_PD_EN                            :  1;\
            UNSG32 RSVDx278_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO5Cntl;
            struct w32Gbl_DVIO5Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO6Cntl_SLP_DI(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO6Cntl_SLP_DI(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO6Cntl_SLP_DI(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO6Cntl_SLP_DI(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO6Cntl_PU_EN(r32)                    _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO6Cntl_PU_EN(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO6Cntl_PU_EN(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO6Cntl_PU_EN(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO6Cntl_PD_EN(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO6Cntl_PD_EN(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO6Cntl_PD_EN(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO6Cntl_PD_EN(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO6Cntl                               {\
            UNSG32 uDVIO6Cntl_SLP_DI                           :  1;\
            UNSG32 uDVIO6Cntl_PU_EN                            :  1;\
            UNSG32 uDVIO6Cntl_PD_EN                            :  1;\
            UNSG32 RSVDx27C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO6Cntl;
            struct w32Gbl_DVIO6Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO7Cntl_SLP_DI(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO7Cntl_SLP_DI(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO7Cntl_SLP_DI(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO7Cntl_SLP_DI(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO7Cntl_PU_EN(r32)                    _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO7Cntl_PU_EN(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO7Cntl_PU_EN(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO7Cntl_PU_EN(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO7Cntl_PD_EN(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO7Cntl_PD_EN(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO7Cntl_PD_EN(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO7Cntl_PD_EN(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO7Cntl                               {\
            UNSG32 uDVIO7Cntl_SLP_DI                           :  1;\
            UNSG32 uDVIO7Cntl_PU_EN                            :  1;\
            UNSG32 uDVIO7Cntl_PD_EN                            :  1;\
            UNSG32 RSVDx280_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO7Cntl;
            struct w32Gbl_DVIO7Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO8Cntl_SLP_DI(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO8Cntl_SLP_DI(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO8Cntl_SLP_DI(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO8Cntl_SLP_DI(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO8Cntl_PU_EN(r32)                    _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO8Cntl_PU_EN(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO8Cntl_PU_EN(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO8Cntl_PU_EN(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO8Cntl_PD_EN(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO8Cntl_PD_EN(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO8Cntl_PD_EN(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO8Cntl_PD_EN(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO8Cntl                               {\
            UNSG32 uDVIO8Cntl_SLP_DI                           :  1;\
            UNSG32 uDVIO8Cntl_PU_EN                            :  1;\
            UNSG32 uDVIO8Cntl_PD_EN                            :  1;\
            UNSG32 RSVDx284_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO8Cntl;
            struct w32Gbl_DVIO8Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO9Cntl_SLP_DI(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO9Cntl_SLP_DI(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO9Cntl_SLP_DI(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO9Cntl_SLP_DI(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO9Cntl_PU_EN(r32)                    _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO9Cntl_PU_EN(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO9Cntl_PU_EN(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO9Cntl_PU_EN(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO9Cntl_PD_EN(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO9Cntl_PD_EN(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO9Cntl_PD_EN(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO9Cntl_PD_EN(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO9Cntl                               {\
            UNSG32 uDVIO9Cntl_SLP_DI                           :  1;\
            UNSG32 uDVIO9Cntl_PU_EN                            :  1;\
            UNSG32 uDVIO9Cntl_PD_EN                            :  1;\
            UNSG32 RSVDx288_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO9Cntl;
            struct w32Gbl_DVIO9Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO10Cntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO10Cntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO10Cntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO10Cntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO10Cntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO10Cntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO10Cntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO10Cntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO10Cntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO10Cntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO10Cntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO10Cntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO10Cntl                              {\
            UNSG32 uDVIO10Cntl_SLP_DI                          :  1;\
            UNSG32 uDVIO10Cntl_PU_EN                           :  1;\
            UNSG32 uDVIO10Cntl_PD_EN                           :  1;\
            UNSG32 RSVDx28C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO10Cntl;
            struct w32Gbl_DVIO10Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO11Cntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO11Cntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO11Cntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO11Cntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO11Cntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO11Cntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO11Cntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO11Cntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO11Cntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO11Cntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO11Cntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO11Cntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO11Cntl                              {\
            UNSG32 uDVIO11Cntl_SLP_DI                          :  1;\
            UNSG32 uDVIO11Cntl_PU_EN                           :  1;\
            UNSG32 uDVIO11Cntl_PD_EN                           :  1;\
            UNSG32 RSVDx290_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO11Cntl;
            struct w32Gbl_DVIO11Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO12Cntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO12Cntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO12Cntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO12Cntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO12Cntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO12Cntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO12Cntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO12Cntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO12Cntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO12Cntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO12Cntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO12Cntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO12Cntl                              {\
            UNSG32 uDVIO12Cntl_SLP_DI                          :  1;\
            UNSG32 uDVIO12Cntl_PU_EN                           :  1;\
            UNSG32 uDVIO12Cntl_PD_EN                           :  1;\
            UNSG32 RSVDx294_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO12Cntl;
            struct w32Gbl_DVIO12Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO13Cntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO13Cntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO13Cntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO13Cntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO13Cntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO13Cntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO13Cntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO13Cntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO13Cntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO13Cntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO13Cntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO13Cntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO13Cntl                              {\
            UNSG32 uDVIO13Cntl_SLP_DI                          :  1;\
            UNSG32 uDVIO13Cntl_PU_EN                           :  1;\
            UNSG32 uDVIO13Cntl_PD_EN                           :  1;\
            UNSG32 RSVDx298_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO13Cntl;
            struct w32Gbl_DVIO13Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO14Cntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO14Cntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO14Cntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO14Cntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO14Cntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO14Cntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO14Cntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO14Cntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO14Cntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO14Cntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO14Cntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO14Cntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO14Cntl                              {\
            UNSG32 uDVIO14Cntl_SLP_DI                          :  1;\
            UNSG32 uDVIO14Cntl_PU_EN                           :  1;\
            UNSG32 uDVIO14Cntl_PD_EN                           :  1;\
            UNSG32 RSVDx29C_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO14Cntl;
            struct w32Gbl_DVIO14Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DVIO15Cntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DVIO15Cntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DVIO15Cntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DVIO15Cntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DVIO15Cntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DVIO15Cntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DVIO15Cntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DVIO15Cntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DVIO15Cntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DVIO15Cntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DVIO15Cntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DVIO15Cntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DVIO15Cntl                              {\
            UNSG32 uDVIO15Cntl_SLP_DI                          :  1;\
            UNSG32 uDVIO15Cntl_PU_EN                           :  1;\
            UNSG32 uDVIO15Cntl_PD_EN                           :  1;\
            UNSG32 RSVDx2A0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DVIO15Cntl;
            struct w32Gbl_DVIO15Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DV0_CLKCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DV0_CLKCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DV0_CLKCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DV0_CLKCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DV0_CLKCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DV0_CLKCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DV0_CLKCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DV0_CLKCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DV0_CLKCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DV0_CLKCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DV0_CLKCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DV0_CLKCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DV0_CLKCntl                             {\
            UNSG32 uDV0_CLKCntl_SLP_DI                         :  1;\
            UNSG32 uDV0_CLKCntl_PU_EN                          :  1;\
            UNSG32 uDV0_CLKCntl_PD_EN                          :  1;\
            UNSG32 RSVDx2A4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DV0_CLKCntl;
            struct w32Gbl_DV0_CLKCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DV0_HSCntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DV0_HSCntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DV0_HSCntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DV0_HSCntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DV0_HSCntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DV0_HSCntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DV0_HSCntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DV0_HSCntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DV0_HSCntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DV0_HSCntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DV0_HSCntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DV0_HSCntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DV0_HSCntl                              {\
            UNSG32 uDV0_HSCntl_SLP_DI                          :  1;\
            UNSG32 uDV0_HSCntl_PU_EN                           :  1;\
            UNSG32 uDV0_HSCntl_PD_EN                           :  1;\
            UNSG32 RSVDx2A8_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DV0_HSCntl;
            struct w32Gbl_DV0_HSCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DV0_VSCntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DV0_VSCntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DV0_VSCntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DV0_VSCntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DV0_VSCntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DV0_VSCntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DV0_VSCntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DV0_VSCntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DV0_VSCntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DV0_VSCntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DV0_VSCntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DV0_VSCntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DV0_VSCntl                              {\
            UNSG32 uDV0_VSCntl_SLP_DI                          :  1;\
            UNSG32 uDV0_VSCntl_PU_EN                           :  1;\
            UNSG32 uDV0_VSCntl_PD_EN                           :  1;\
            UNSG32 RSVDx2AC_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DV0_VSCntl;
            struct w32Gbl_DV0_VSCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DV0_FIDCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DV0_FIDCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DV0_FIDCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DV0_FIDCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DV0_FIDCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DV0_FIDCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DV0_FIDCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DV0_FIDCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DV0_FIDCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DV0_FIDCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DV0_FIDCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DV0_FIDCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DV0_FIDCntl                             {\
            UNSG32 uDV0_FIDCntl_SLP_DI                         :  1;\
            UNSG32 uDV0_FIDCntl_PU_EN                          :  1;\
            UNSG32 uDV0_FIDCntl_PD_EN                          :  1;\
            UNSG32 RSVDx2B0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DV0_FIDCntl;
            struct w32Gbl_DV0_FIDCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DV1_CLKCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DV1_CLKCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DV1_CLKCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DV1_CLKCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DV1_CLKCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DV1_CLKCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DV1_CLKCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DV1_CLKCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DV1_CLKCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DV1_CLKCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DV1_CLKCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DV1_CLKCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DV1_CLKCntl                             {\
            UNSG32 uDV1_CLKCntl_SLP_DI                         :  1;\
            UNSG32 uDV1_CLKCntl_PU_EN                          :  1;\
            UNSG32 uDV1_CLKCntl_PD_EN                          :  1;\
            UNSG32 RSVDx2B4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DV1_CLKCntl;
            struct w32Gbl_DV1_CLKCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DV1_HSCntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DV1_HSCntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DV1_HSCntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DV1_HSCntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DV1_HSCntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DV1_HSCntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DV1_HSCntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DV1_HSCntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DV1_HSCntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DV1_HSCntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DV1_HSCntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DV1_HSCntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DV1_HSCntl                              {\
            UNSG32 uDV1_HSCntl_SLP_DI                          :  1;\
            UNSG32 uDV1_HSCntl_PU_EN                           :  1;\
            UNSG32 uDV1_HSCntl_PD_EN                           :  1;\
            UNSG32 RSVDx2B8_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DV1_HSCntl;
            struct w32Gbl_DV1_HSCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DV1_VSCntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DV1_VSCntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DV1_VSCntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DV1_VSCntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DV1_VSCntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DV1_VSCntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DV1_VSCntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DV1_VSCntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DV1_VSCntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DV1_VSCntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DV1_VSCntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DV1_VSCntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DV1_VSCntl                              {\
            UNSG32 uDV1_VSCntl_SLP_DI                          :  1;\
            UNSG32 uDV1_VSCntl_PU_EN                           :  1;\
            UNSG32 uDV1_VSCntl_PD_EN                           :  1;\
            UNSG32 RSVDx2BC_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DV1_VSCntl;
            struct w32Gbl_DV1_VSCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_DV1_FIDCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_DV1_FIDCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_DV1_FIDCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_DV1_FIDCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_DV1_FIDCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_DV1_FIDCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_DV1_FIDCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_DV1_FIDCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_DV1_FIDCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_DV1_FIDCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_DV1_FIDCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_DV1_FIDCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_DV1_FIDCntl                             {\
            UNSG32 uDV1_FIDCntl_SLP_DI                         :  1;\
            UNSG32 uDV1_FIDCntl_PU_EN                          :  1;\
            UNSG32 uDV1_FIDCntl_PD_EN                          :  1;\
            UNSG32 RSVDx2C0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_DV1_FIDCntl;
            struct w32Gbl_DV1_FIDCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S0_BCLKCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S0_BCLKCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S0_BCLKCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S0_BCLKCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S0_BCLKCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S0_BCLKCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S0_BCLKCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S0_BCLKCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S0_BCLKCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S0_BCLKCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S0_BCLKCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S0_BCLKCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S0_BCLKCntl                           {\
            UNSG32 uI2S0_BCLKCntl_SLP_DI                       :  1;\
            UNSG32 uI2S0_BCLKCntl_PU_EN                        :  1;\
            UNSG32 uI2S0_BCLKCntl_PD_EN                        :  1;\
            UNSG32 RSVDx2C4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S0_BCLKCntl;
            struct w32Gbl_I2S0_BCLKCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S0_LRCKCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S0_LRCKCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S0_LRCKCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S0_LRCKCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S0_LRCKCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S0_LRCKCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S0_LRCKCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S0_LRCKCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S0_LRCKCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S0_LRCKCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S0_LRCKCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S0_LRCKCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S0_LRCKCntl                           {\
            UNSG32 uI2S0_LRCKCntl_SLP_DI                       :  1;\
            UNSG32 uI2S0_LRCKCntl_PU_EN                        :  1;\
            UNSG32 uI2S0_LRCKCntl_PD_EN                        :  1;\
            UNSG32 RSVDx2C8_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S0_LRCKCntl;
            struct w32Gbl_I2S0_LRCKCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S0_DO0Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S0_DO0Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S0_DO0Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S0_DO0Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S0_DO0Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S0_DO0Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S0_DO0Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S0_DO0Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S0_DO0Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S0_DO0Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S0_DO0Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S0_DO0Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S0_DO0Cntl                            {\
            UNSG32 uI2S0_DO0Cntl_SLP_DI                        :  1;\
            UNSG32 uI2S0_DO0Cntl_PU_EN                         :  1;\
            UNSG32 uI2S0_DO0Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx2CC_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S0_DO0Cntl;
            struct w32Gbl_I2S0_DO0Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S0_DO1Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S0_DO1Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S0_DO1Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S0_DO1Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S0_DO1Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S0_DO1Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S0_DO1Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S0_DO1Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S0_DO1Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S0_DO1Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S0_DO1Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S0_DO1Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S0_DO1Cntl                            {\
            UNSG32 uI2S0_DO1Cntl_SLP_DI                        :  1;\
            UNSG32 uI2S0_DO1Cntl_PU_EN                         :  1;\
            UNSG32 uI2S0_DO1Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx2D0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S0_DO1Cntl;
            struct w32Gbl_I2S0_DO1Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S0_DO2Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S0_DO2Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S0_DO2Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S0_DO2Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S0_DO2Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S0_DO2Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S0_DO2Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S0_DO2Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S0_DO2Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S0_DO2Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S0_DO2Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S0_DO2Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S0_DO2Cntl                            {\
            UNSG32 uI2S0_DO2Cntl_SLP_DI                        :  1;\
            UNSG32 uI2S0_DO2Cntl_PU_EN                         :  1;\
            UNSG32 uI2S0_DO2Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx2D4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S0_DO2Cntl;
            struct w32Gbl_I2S0_DO2Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S0_DO3Cntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S0_DO3Cntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S0_DO3Cntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S0_DO3Cntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S0_DO3Cntl_PU_EN(r32)                 _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S0_DO3Cntl_PU_EN(r32,v)               _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S0_DO3Cntl_PU_EN(r16)                 _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S0_DO3Cntl_PU_EN(r16,v)               _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S0_DO3Cntl_PD_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S0_DO3Cntl_PD_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S0_DO3Cntl_PD_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S0_DO3Cntl_PD_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S0_DO3Cntl                            {\
            UNSG32 uI2S0_DO3Cntl_SLP_DI                        :  1;\
            UNSG32 uI2S0_DO3Cntl_PU_EN                         :  1;\
            UNSG32 uI2S0_DO3Cntl_PD_EN                         :  1;\
            UNSG32 RSVDx2D8_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S0_DO3Cntl;
            struct w32Gbl_I2S0_DO3Cntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S1_LRCKCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S1_LRCKCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S1_LRCKCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S1_LRCKCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S1_LRCKCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S1_LRCKCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S1_LRCKCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S1_LRCKCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S1_LRCKCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S1_LRCKCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S1_LRCKCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S1_LRCKCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S1_LRCKCntl                           {\
            UNSG32 uI2S1_LRCKCntl_SLP_DI                       :  1;\
            UNSG32 uI2S1_LRCKCntl_PU_EN                        :  1;\
            UNSG32 uI2S1_LRCKCntl_PD_EN                        :  1;\
            UNSG32 RSVDx2DC_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S1_LRCKCntl;
            struct w32Gbl_I2S1_LRCKCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S1_BCLKCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S1_BCLKCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S1_BCLKCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S1_BCLKCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S1_BCLKCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S1_BCLKCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S1_BCLKCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S1_BCLKCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S1_BCLKCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S1_BCLKCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S1_BCLKCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S1_BCLKCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S1_BCLKCntl                           {\
            UNSG32 uI2S1_BCLKCntl_SLP_DI                       :  1;\
            UNSG32 uI2S1_BCLKCntl_PU_EN                        :  1;\
            UNSG32 uI2S1_BCLKCntl_PD_EN                        :  1;\
            UNSG32 RSVDx2E0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S1_BCLKCntl;
            struct w32Gbl_I2S1_BCLKCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S1_DOCntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S1_DOCntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S1_DOCntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S1_DOCntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S1_DOCntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S1_DOCntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S1_DOCntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S1_DOCntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S1_DOCntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S1_DOCntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S1_DOCntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S1_DOCntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S1_DOCntl                             {\
            UNSG32 uI2S1_DOCntl_SLP_DI                         :  1;\
            UNSG32 uI2S1_DOCntl_PU_EN                          :  1;\
            UNSG32 uI2S1_DOCntl_PD_EN                          :  1;\
            UNSG32 RSVDx2E4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S1_DOCntl;
            struct w32Gbl_I2S1_DOCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S2_LRCKCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S2_LRCKCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S2_LRCKCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S2_LRCKCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S2_LRCKCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S2_LRCKCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S2_LRCKCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S2_LRCKCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S2_LRCKCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S2_LRCKCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S2_LRCKCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S2_LRCKCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S2_LRCKCntl                           {\
            UNSG32 uI2S2_LRCKCntl_SLP_DI                       :  1;\
            UNSG32 uI2S2_LRCKCntl_PU_EN                        :  1;\
            UNSG32 uI2S2_LRCKCntl_PD_EN                        :  1;\
            UNSG32 RSVDx2E8_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S2_LRCKCntl;
            struct w32Gbl_I2S2_LRCKCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S2_BCLKCntl_SLP_DI(r32)               _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S2_BCLKCntl_SLP_DI(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S2_BCLKCntl_SLP_DI(r16)               _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S2_BCLKCntl_SLP_DI(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S2_BCLKCntl_PU_EN(r32)                _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S2_BCLKCntl_PU_EN(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S2_BCLKCntl_PU_EN(r16)                _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S2_BCLKCntl_PU_EN(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S2_BCLKCntl_PD_EN(r32)                _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S2_BCLKCntl_PD_EN(r32,v)              _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S2_BCLKCntl_PD_EN(r16)                _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S2_BCLKCntl_PD_EN(r16,v)              _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S2_BCLKCntl                           {\
            UNSG32 uI2S2_BCLKCntl_SLP_DI                       :  1;\
            UNSG32 uI2S2_BCLKCntl_PU_EN                        :  1;\
            UNSG32 uI2S2_BCLKCntl_PD_EN                        :  1;\
            UNSG32 RSVDx2EC_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S2_BCLKCntl;
            struct w32Gbl_I2S2_BCLKCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_I2S2_DICntl_SLP_DI(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32Gbl_I2S2_DICntl_SLP_DI(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_I2S2_DICntl_SLP_DI(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16Gbl_I2S2_DICntl_SLP_DI(r16,v)               _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_I2S2_DICntl_PU_EN(r32)                  _BFGET_(r32, 1, 1)
    #define   SET32Gbl_I2S2_DICntl_PU_EN(r32,v)                _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_I2S2_DICntl_PU_EN(r16)                  _BFGET_(r16, 1, 1)
    #define   SET16Gbl_I2S2_DICntl_PU_EN(r16,v)                _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_I2S2_DICntl_PD_EN(r32)                  _BFGET_(r32, 2, 2)
    #define   SET32Gbl_I2S2_DICntl_PD_EN(r32,v)                _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_I2S2_DICntl_PD_EN(r16)                  _BFGET_(r16, 2, 2)
    #define   SET16Gbl_I2S2_DICntl_PD_EN(r16,v)                _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_I2S2_DICntl                             {\
            UNSG32 uI2S2_DICntl_SLP_DI                         :  1;\
            UNSG32 uI2S2_DICntl_PU_EN                          :  1;\
            UNSG32 uI2S2_DICntl_PD_EN                          :  1;\
            UNSG32 RSVDx2F0_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_I2S2_DICntl;
            struct w32Gbl_I2S2_DICntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_SPDIFOCntl_SLP_DI(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32Gbl_SPDIFOCntl_SLP_DI(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_SPDIFOCntl_SLP_DI(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16Gbl_SPDIFOCntl_SLP_DI(r16,v)                _BFSET_(r16, 0, 0,v)

    #define   GET32Gbl_SPDIFOCntl_PU_EN(r32)                   _BFGET_(r32, 1, 1)
    #define   SET32Gbl_SPDIFOCntl_PU_EN(r32,v)                 _BFSET_(r32, 1, 1,v)
    #define   GET16Gbl_SPDIFOCntl_PU_EN(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16Gbl_SPDIFOCntl_PU_EN(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32Gbl_SPDIFOCntl_PD_EN(r32)                   _BFGET_(r32, 2, 2)
    #define   SET32Gbl_SPDIFOCntl_PD_EN(r32,v)                 _BFSET_(r32, 2, 2,v)
    #define   GET16Gbl_SPDIFOCntl_PD_EN(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16Gbl_SPDIFOCntl_PD_EN(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define     w32Gbl_SPDIFOCntl                              {\
            UNSG32 uSPDIFOCntl_SLP_DI                          :  1;\
            UNSG32 uSPDIFOCntl_PU_EN                           :  1;\
            UNSG32 uSPDIFOCntl_PD_EN                           :  1;\
            UNSG32 RSVDx2F4_b3                                 : 29;\
          }
    union { UNSG32 u32Gbl_SPDIFOCntl;
            struct w32Gbl_SPDIFOCntl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32Gbl_HDMI_CECCntl_SLP_DI(r32)                _BFGET_(r32, 0, 0)
    #define   SET32Gbl_HDMI_CECCntl_SLP_DI(r32,v)              _BFSET_(r32, 0, 0,v)
    #define   GET16Gbl_HDMI_CECCntl_SLP_DI(r16)                _BFGET_(r16, 0, 0)
    #define   SET16Gbl_HDMI_CECCntl_SLP_DI(r16,v)              _BFSET_(r16, 0, 0,v)

    #define     w32Gbl_HDMI_CECCntl                            {\
            UNSG32 uHDMI_CECCntl_SLP_DI                        :  1;\
            UNSG32 RSVDx2F8_b1                                 : 31;\
          }
    union { UNSG32 u32Gbl_HDMI_CECCntl;
            struct w32Gbl_HDMI_CECCntl;
          };
    ///////////////////////////////////////////////////////////
    } SIE_Gbl;

    typedef union  T32Gbl_pinMux
          { UNSG32 u32;
            struct w32Gbl_pinMux;
                 } T32Gbl_pinMux;
    typedef union  T32Gbl_pinMux1
          { UNSG32 u32;
            struct w32Gbl_pinMux1;
                 } T32Gbl_pinMux1;
    typedef union  T32Gbl_bootStrap
          { UNSG32 u32;
            struct w32Gbl_bootStrap;
                 } T32Gbl_bootStrap;
    typedef union  T32Gbl_chipCntl
          { UNSG32 u32;
            struct w32Gbl_chipCntl;
                 } T32Gbl_chipCntl;
    typedef union  T32Gbl_anaGrpCtl
          { UNSG32 u32;
            struct w32Gbl_anaGrpCtl;
                 } T32Gbl_anaGrpCtl;
    typedef union  T32Gbl_sysPllCtl
          { UNSG32 u32;
            struct w32Gbl_sysPllCtl;
                 } T32Gbl_sysPllCtl;
    typedef union  T32Gbl_sysPllCtl1
          { UNSG32 u32;
            struct w32Gbl_sysPllCtl1;
                 } T32Gbl_sysPllCtl1;
    typedef union  T32Gbl_sysPllCtl2
          { UNSG32 u32;
            struct w32Gbl_sysPllCtl2;
                 } T32Gbl_sysPllCtl2;
    typedef union  T32Gbl_memPllCtl
          { UNSG32 u32;
            struct w32Gbl_memPllCtl;
                 } T32Gbl_memPllCtl;
    typedef union  T32Gbl_memPllCtl1
          { UNSG32 u32;
            struct w32Gbl_memPllCtl1;
                 } T32Gbl_memPllCtl1;
    typedef union  T32Gbl_memPllCtl2
          { UNSG32 u32;
            struct w32Gbl_memPllCtl2;
                 } T32Gbl_memPllCtl2;
    typedef union  T32Gbl_cpu1PllCtl
          { UNSG32 u32;
            struct w32Gbl_cpu1PllCtl;
                 } T32Gbl_cpu1PllCtl;
    typedef union  T32Gbl_cpu1PllCtl1
          { UNSG32 u32;
            struct w32Gbl_cpu1PllCtl1;
                 } T32Gbl_cpu1PllCtl1;
    typedef union  T32Gbl_cpu1PllCtl2
          { UNSG32 u32;
            struct w32Gbl_cpu1PllCtl2;
                 } T32Gbl_cpu1PllCtl2;
    typedef union  T32Gbl_cpu1PllCtl3
          { UNSG32 u32;
            struct w32Gbl_cpu1PllCtl3;
                 } T32Gbl_cpu1PllCtl3;
    typedef union  T32Gbl_cpu1PllCtl4
          { UNSG32 u32;
            struct w32Gbl_cpu1PllCtl4;
                 } T32Gbl_cpu1PllCtl4;
    typedef union  T32Gbl_avPllCtl
          { UNSG32 u32;
            struct w32Gbl_avPllCtl;
                 } T32Gbl_avPllCtl;
    typedef union  T32Gbl_avPllCtl1
          { UNSG32 u32;
            struct w32Gbl_avPllCtl1;
                 } T32Gbl_avPllCtl1;
    typedef union  T32Gbl_avPllCtl2
          { UNSG32 u32;
            struct w32Gbl_avPllCtl2;
                 } T32Gbl_avPllCtl2;
    typedef union  T32Gbl_avPllCtl3
          { UNSG32 u32;
            struct w32Gbl_avPllCtl3;
                 } T32Gbl_avPllCtl3;
    typedef union  T32Gbl_avPllCtl4
          { UNSG32 u32;
            struct w32Gbl_avPllCtl4;
                 } T32Gbl_avPllCtl4;
    typedef union  T32Gbl_avPllCtl5
          { UNSG32 u32;
            struct w32Gbl_avPllCtl5;
                 } T32Gbl_avPllCtl5;
    typedef union  T32Gbl_avPllCtl6
          { UNSG32 u32;
            struct w32Gbl_avPllCtl6;
                 } T32Gbl_avPllCtl6;
    typedef union  T32Gbl_avPllCtl7
          { UNSG32 u32;
            struct w32Gbl_avPllCtl7;
                 } T32Gbl_avPllCtl7;
    typedef union  T32Gbl_avPllCtl8
          { UNSG32 u32;
            struct w32Gbl_avPllCtl8;
                 } T32Gbl_avPllCtl8;
    typedef union  T32Gbl_avPllCtl9
          { UNSG32 u32;
            struct w32Gbl_avPllCtl9;
                 } T32Gbl_avPllCtl9;
    typedef union  T32Gbl_avPllCtl10
          { UNSG32 u32;
            struct w32Gbl_avPllCtl10;
                 } T32Gbl_avPllCtl10;
    typedef union  T32Gbl_avPllCtl11
          { UNSG32 u32;
            struct w32Gbl_avPllCtl11;
                 } T32Gbl_avPllCtl11;
    typedef union  T32Gbl_avPllCtl12
          { UNSG32 u32;
            struct w32Gbl_avPllCtl12;
                 } T32Gbl_avPllCtl12;
    typedef union  T32Gbl_avPllCtl13
          { UNSG32 u32;
            struct w32Gbl_avPllCtl13;
                 } T32Gbl_avPllCtl13;
    typedef union  T32Gbl_avPllCtl14
          { UNSG32 u32;
            struct w32Gbl_avPllCtl14;
                 } T32Gbl_avPllCtl14;
    typedef union  T32Gbl_avPllCtl15
          { UNSG32 u32;
            struct w32Gbl_avPllCtl15;
                 } T32Gbl_avPllCtl15;
    typedef union  T32Gbl_avPllCtl16
          { UNSG32 u32;
            struct w32Gbl_avPllCtl16;
                 } T32Gbl_avPllCtl16;
    typedef union  T32Gbl_avPllCtl17
          { UNSG32 u32;
            struct w32Gbl_avPllCtl17;
                 } T32Gbl_avPllCtl17;
    typedef union  T32Gbl_avPllCtl18
          { UNSG32 u32;
            struct w32Gbl_avPllCtl18;
                 } T32Gbl_avPllCtl18;
    typedef union  T32Gbl_pllStatus
          { UNSG32 u32;
            struct w32Gbl_pllStatus;
                 } T32Gbl_pllStatus;
    typedef union  T32Gbl_clkEnable
          { UNSG32 u32;
            struct w32Gbl_clkEnable;
                 } T32Gbl_clkEnable;
    typedef union  T32Gbl_clkSelect
          { UNSG32 u32;
            struct w32Gbl_clkSelect;
                 } T32Gbl_clkSelect;
    typedef union  T32Gbl_clkSelect1
          { UNSG32 u32;
            struct w32Gbl_clkSelect1;
                 } T32Gbl_clkSelect1;
    typedef union  T32Gbl_clkSelect2
          { UNSG32 u32;
            struct w32Gbl_clkSelect2;
                 } T32Gbl_clkSelect2;
    typedef union  T32Gbl_clkSelect3
          { UNSG32 u32;
            struct w32Gbl_clkSelect3;
                 } T32Gbl_clkSelect3;
    typedef union  T32Gbl_ClkSwitch
          { UNSG32 u32;
            struct w32Gbl_ClkSwitch;
                 } T32Gbl_ClkSwitch;
    typedef union  T32Gbl_ClkSwitch1
          { UNSG32 u32;
            struct w32Gbl_ClkSwitch1;
                 } T32Gbl_ClkSwitch1;
    typedef union  T32Gbl_PadSelect
          { UNSG32 u32;
            struct w32Gbl_PadSelect;
                 } T32Gbl_PadSelect;
    typedef union  T32Gbl_PadSelect1
          { UNSG32 u32;
            struct w32Gbl_PadSelect1;
                 } T32Gbl_PadSelect1;
    typedef union  T32Gbl_ResetTrigger
          { UNSG32 u32;
            struct w32Gbl_ResetTrigger;
                 } T32Gbl_ResetTrigger;
    typedef union  T32Gbl_ResetStatus
          { UNSG32 u32;
            struct w32Gbl_ResetStatus;
                 } T32Gbl_ResetStatus;
    typedef union  T32Gbl_ResetStatus1
          { UNSG32 u32;
            struct w32Gbl_ResetStatus1;
                 } T32Gbl_ResetStatus1;
    typedef union  T32Gbl_sw_generic0
          { UNSG32 u32;
            struct w32Gbl_sw_generic0;
                 } T32Gbl_sw_generic0;
    typedef union  T32Gbl_sw_generic1
          { UNSG32 u32;
            struct w32Gbl_sw_generic1;
                 } T32Gbl_sw_generic1;
    typedef union  T32Gbl_sw_generic2
          { UNSG32 u32;
            struct w32Gbl_sw_generic2;
                 } T32Gbl_sw_generic2;
    typedef union  T32Gbl_sw_generic3
          { UNSG32 u32;
            struct w32Gbl_sw_generic3;
                 } T32Gbl_sw_generic3;
    typedef union  T32Gbl_ChipCntl0
          { UNSG32 u32;
            struct w32Gbl_ChipCntl0;
                 } T32Gbl_ChipCntl0;
    typedef union  T32Gbl_ChipCntl1
          { UNSG32 u32;
            struct w32Gbl_ChipCntl1;
                 } T32Gbl_ChipCntl1;
    typedef union  T32Gbl_ChipCntl11
          { UNSG32 u32;
            struct w32Gbl_ChipCntl11;
                 } T32Gbl_ChipCntl11;
    typedef union  T32Gbl_RWTC_avio31to0
          { UNSG32 u32;
            struct w32Gbl_RWTC_avio31to0;
                 } T32Gbl_RWTC_avio31to0;
    typedef union  T32Gbl_RWTC_avio43to32
          { UNSG32 u32;
            struct w32Gbl_RWTC_avio43to32;
                 } T32Gbl_RWTC_avio43to32;
    typedef union  T32Gbl_RWTC_gfx2D31to0
          { UNSG32 u32;
            struct w32Gbl_RWTC_gfx2D31to0;
                 } T32Gbl_RWTC_gfx2D31to0;
    typedef union  T32Gbl_RWTC_gfx2D43to32
          { UNSG32 u32;
            struct w32Gbl_RWTC_gfx2D43to32;
                 } T32Gbl_RWTC_gfx2D43to32;
    typedef union  T32Gbl_RWTC_perif31to0
          { UNSG32 u32;
            struct w32Gbl_RWTC_perif31to0;
                 } T32Gbl_RWTC_perif31to0;
    typedef union  T32Gbl_RWTC_perif43to32
          { UNSG32 u32;
            struct w32Gbl_RWTC_perif43to32;
                 } T32Gbl_RWTC_perif43to32;
    typedef union  T32Gbl_RWTC_soc31to0
          { UNSG32 u32;
            struct w32Gbl_RWTC_soc31to0;
                 } T32Gbl_RWTC_soc31to0;
    typedef union  T32Gbl_RWTC_soc43to32
          { UNSG32 u32;
            struct w32Gbl_RWTC_soc43to32;
                 } T32Gbl_RWTC_soc43to32;
    typedef union  T32Gbl_RWTC_vPro31to0
          { UNSG32 u32;
            struct w32Gbl_RWTC_vPro31to0;
                 } T32Gbl_RWTC_vPro31to0;
    typedef union  T32Gbl_RWTC_vPro43to32
          { UNSG32 u32;
            struct w32Gbl_RWTC_vPro43to32;
                 } T32Gbl_RWTC_vPro43to32;
    typedef union  T32Gbl_ProductId
          { UNSG32 u32;
            struct w32Gbl_ProductId;
                 } T32Gbl_ProductId;
    typedef union  T32Gbl_ProductId_ext
          { UNSG32 u32;
            struct w32Gbl_ProductId_ext;
                 } T32Gbl_ProductId_ext;
    typedef union  T32Gbl_FPGAR
          { UNSG32 u32;
            struct w32Gbl_FPGAR;
                 } T32Gbl_FPGAR;
    typedef union  T32Gbl_FPGARW
          { UNSG32 u32;
            struct w32Gbl_FPGARW;
                 } T32Gbl_FPGARW;
    typedef union  T32Gbl_RingOscCtl
          { UNSG32 u32;
            struct w32Gbl_RingOscCtl;
                 } T32Gbl_RingOscCtl;
    typedef union  T32Gbl_PLLBypsBootStrapEn
          { UNSG32 u32;
            struct w32Gbl_PLLBypsBootStrapEn;
                 } T32Gbl_PLLBypsBootStrapEn;
    typedef union  T32Gbl_IO_CALIBRATE_IN
          { UNSG32 u32;
            struct w32Gbl_IO_CALIBRATE_IN;
                 } T32Gbl_IO_CALIBRATE_IN;
    typedef union  T32Gbl_IO_CALBRATE_OUT
          { UNSG32 u32;
            struct w32Gbl_IO_CALBRATE_OUT;
                 } T32Gbl_IO_CALBRATE_OUT;
    typedef union  T32Gbl_SECURE_SCAN_EN
          { UNSG32 u32;
            struct w32Gbl_SECURE_SCAN_EN;
                 } T32Gbl_SECURE_SCAN_EN;
    typedef union  T32Gbl_NandWPn
          { UNSG32 u32;
            struct w32Gbl_NandWPn;
                 } T32Gbl_NandWPn;
    typedef union  T32Gbl_RingCntCntl
          { UNSG32 u32;
            struct w32Gbl_RingCntCntl;
                 } T32Gbl_RingCntCntl;
    typedef union  T32Gbl_CenterCnt
          { UNSG32 u32;
            struct w32Gbl_CenterCnt;
                 } T32Gbl_CenterCnt;
    typedef union  T32Gbl_avioTopCnt
          { UNSG32 u32;
            struct w32Gbl_avioTopCnt;
                 } T32Gbl_avioTopCnt;
    typedef union  T32Gbl_avioBotCnt
          { UNSG32 u32;
            struct w32Gbl_avioBotCnt;
                 } T32Gbl_avioBotCnt;
    typedef union  T32Gbl_usbTopCnt
          { UNSG32 u32;
            struct w32Gbl_usbTopCnt;
                 } T32Gbl_usbTopCnt;
    typedef union  T32Gbl_cpu1TopCnt
          { UNSG32 u32;
            struct w32Gbl_cpu1TopCnt;
                 } T32Gbl_cpu1TopCnt;
    typedef union  T32Gbl_cpu0TopCnt
          { UNSG32 u32;
            struct w32Gbl_cpu0TopCnt;
                 } T32Gbl_cpu0TopCnt;
    typedef union  T32Gbl_cpuBotCnt
          { UNSG32 u32;
            struct w32Gbl_cpuBotCnt;
                 } T32Gbl_cpuBotCnt;
    typedef union  T32Gbl_topRiteCnt
          { UNSG32 u32;
            struct w32Gbl_topRiteCnt;
                 } T32Gbl_topRiteCnt;
    typedef union  T32Gbl_midRiteCnt
          { UNSG32 u32;
            struct w32Gbl_midRiteCnt;
                 } T32Gbl_midRiteCnt;
    typedef union  T32Gbl_botRiteCnt
          { UNSG32 u32;
            struct w32Gbl_botRiteCnt;
                 } T32Gbl_botRiteCnt;
    typedef union  T32Gbl_NAND_IO0Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_IO0Cntl;
                 } T32Gbl_NAND_IO0Cntl;
    typedef union  T32Gbl_NAND_IO1Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_IO1Cntl;
                 } T32Gbl_NAND_IO1Cntl;
    typedef union  T32Gbl_NAND_IO2Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_IO2Cntl;
                 } T32Gbl_NAND_IO2Cntl;
    typedef union  T32Gbl_NAND_IO3Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_IO3Cntl;
                 } T32Gbl_NAND_IO3Cntl;
    typedef union  T32Gbl_NAND_IO4Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_IO4Cntl;
                 } T32Gbl_NAND_IO4Cntl;
    typedef union  T32Gbl_NAND_IO5Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_IO5Cntl;
                 } T32Gbl_NAND_IO5Cntl;
    typedef union  T32Gbl_NAND_IO6Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_IO6Cntl;
                 } T32Gbl_NAND_IO6Cntl;
    typedef union  T32Gbl_NAND_IO7Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_IO7Cntl;
                 } T32Gbl_NAND_IO7Cntl;
    typedef union  T32Gbl_NAND_ALECntl
          { UNSG32 u32;
            struct w32Gbl_NAND_ALECntl;
                 } T32Gbl_NAND_ALECntl;
    typedef union  T32Gbl_NAND_CLECntl
          { UNSG32 u32;
            struct w32Gbl_NAND_CLECntl;
                 } T32Gbl_NAND_CLECntl;
    typedef union  T32Gbl_NAND_WEnCntl
          { UNSG32 u32;
            struct w32Gbl_NAND_WEnCntl;
                 } T32Gbl_NAND_WEnCntl;
    typedef union  T32Gbl_NAND_REnCntl
          { UNSG32 u32;
            struct w32Gbl_NAND_REnCntl;
                 } T32Gbl_NAND_REnCntl;
    typedef union  T32Gbl_NAND_WPnCntl
          { UNSG32 u32;
            struct w32Gbl_NAND_WPnCntl;
                 } T32Gbl_NAND_WPnCntl;
    typedef union  T32Gbl_NAND_CEn0Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_CEn0Cntl;
                 } T32Gbl_NAND_CEn0Cntl;
    typedef union  T32Gbl_NAND_CEn1Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_CEn1Cntl;
                 } T32Gbl_NAND_CEn1Cntl;
    typedef union  T32Gbl_NAND_RDY0Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_RDY0Cntl;
                 } T32Gbl_NAND_RDY0Cntl;
    typedef union  T32Gbl_NAND_RDY1Cntl
          { UNSG32 u32;
            struct w32Gbl_NAND_RDY1Cntl;
                 } T32Gbl_NAND_RDY1Cntl;
    typedef union  T32Gbl_ET_TXCLKIOCntl
          { UNSG32 u32;
            struct w32Gbl_ET_TXCLKIOCntl;
                 } T32Gbl_ET_TXCLKIOCntl;
    typedef union  T32Gbl_ET_TXD0Cntl
          { UNSG32 u32;
            struct w32Gbl_ET_TXD0Cntl;
                 } T32Gbl_ET_TXD0Cntl;
    typedef union  T32Gbl_ET_TXD1Cntl
          { UNSG32 u32;
            struct w32Gbl_ET_TXD1Cntl;
                 } T32Gbl_ET_TXD1Cntl;
    typedef union  T32Gbl_ET_TXD2Cntl
          { UNSG32 u32;
            struct w32Gbl_ET_TXD2Cntl;
                 } T32Gbl_ET_TXD2Cntl;
    typedef union  T32Gbl_ET_TXD3Cntl
          { UNSG32 u32;
            struct w32Gbl_ET_TXD3Cntl;
                 } T32Gbl_ET_TXD3Cntl;
    typedef union  T32Gbl_ET_TXENCntl
          { UNSG32 u32;
            struct w32Gbl_ET_TXENCntl;
                 } T32Gbl_ET_TXENCntl;
    typedef union  T32Gbl_ET_TXERCntl
          { UNSG32 u32;
            struct w32Gbl_ET_TXERCntl;
                 } T32Gbl_ET_TXERCntl;
    typedef union  T32Gbl_ET_RXCLKICntl
          { UNSG32 u32;
            struct w32Gbl_ET_RXCLKICntl;
                 } T32Gbl_ET_RXCLKICntl;
    typedef union  T32Gbl_ET_RXD0Cntl
          { UNSG32 u32;
            struct w32Gbl_ET_RXD0Cntl;
                 } T32Gbl_ET_RXD0Cntl;
    typedef union  T32Gbl_ET_RXD1Cntl
          { UNSG32 u32;
            struct w32Gbl_ET_RXD1Cntl;
                 } T32Gbl_ET_RXD1Cntl;
    typedef union  T32Gbl_ET_RXD2Cntl
          { UNSG32 u32;
            struct w32Gbl_ET_RXD2Cntl;
                 } T32Gbl_ET_RXD2Cntl;
    typedef union  T32Gbl_ET_RXD3Cntl
          { UNSG32 u32;
            struct w32Gbl_ET_RXD3Cntl;
                 } T32Gbl_ET_RXD3Cntl;
    typedef union  T32Gbl_ET_CRSCntl
          { UNSG32 u32;
            struct w32Gbl_ET_CRSCntl;
                 } T32Gbl_ET_CRSCntl;
    typedef union  T32Gbl_ET_COLCntl
          { UNSG32 u32;
            struct w32Gbl_ET_COLCntl;
                 } T32Gbl_ET_COLCntl;
    typedef union  T32Gbl_ET_RXDVCntl
          { UNSG32 u32;
            struct w32Gbl_ET_RXDVCntl;
                 } T32Gbl_ET_RXDVCntl;
    typedef union  T32Gbl_ET_RXERCntl
          { UNSG32 u32;
            struct w32Gbl_ET_RXERCntl;
                 } T32Gbl_ET_RXERCntl;
    typedef union  T32Gbl_ET_MDCCntl
          { UNSG32 u32;
            struct w32Gbl_ET_MDCCntl;
                 } T32Gbl_ET_MDCCntl;
    typedef union  T32Gbl_ET_MDIOCntl
          { UNSG32 u32;
            struct w32Gbl_ET_MDIOCntl;
                 } T32Gbl_ET_MDIOCntl;
    typedef union  T32Gbl_ET_RCLKOCntl
          { UNSG32 u32;
            struct w32Gbl_ET_RCLKOCntl;
                 } T32Gbl_ET_RCLKOCntl;
    typedef union  T32Gbl_TW0_SCLCntl
          { UNSG32 u32;
            struct w32Gbl_TW0_SCLCntl;
                 } T32Gbl_TW0_SCLCntl;
    typedef union  T32Gbl_TW0_SDACntl
          { UNSG32 u32;
            struct w32Gbl_TW0_SDACntl;
                 } T32Gbl_TW0_SDACntl;
    typedef union  T32Gbl_TW1_SCLCntl
          { UNSG32 u32;
            struct w32Gbl_TW1_SCLCntl;
                 } T32Gbl_TW1_SCLCntl;
    typedef union  T32Gbl_TW1_SDACntl
          { UNSG32 u32;
            struct w32Gbl_TW1_SDACntl;
                 } T32Gbl_TW1_SDACntl;
    typedef union  T32Gbl_SPI1_SS0nCntl
          { UNSG32 u32;
            struct w32Gbl_SPI1_SS0nCntl;
                 } T32Gbl_SPI1_SS0nCntl;
    typedef union  T32Gbl_SPI1_SS1nCntl
          { UNSG32 u32;
            struct w32Gbl_SPI1_SS1nCntl;
                 } T32Gbl_SPI1_SS1nCntl;
    typedef union  T32Gbl_SPI1_SS2nCntl
          { UNSG32 u32;
            struct w32Gbl_SPI1_SS2nCntl;
                 } T32Gbl_SPI1_SS2nCntl;
    typedef union  T32Gbl_SPI1_SS3nCntl
          { UNSG32 u32;
            struct w32Gbl_SPI1_SS3nCntl;
                 } T32Gbl_SPI1_SS3nCntl;
    typedef union  T32Gbl_SPI1_SCLKCntl
          { UNSG32 u32;
            struct w32Gbl_SPI1_SCLKCntl;
                 } T32Gbl_SPI1_SCLKCntl;
    typedef union  T32Gbl_SPI1_SDOCntl
          { UNSG32 u32;
            struct w32Gbl_SPI1_SDOCntl;
                 } T32Gbl_SPI1_SDOCntl;
    typedef union  T32Gbl_SPI1_SDICntl
          { UNSG32 u32;
            struct w32Gbl_SPI1_SDICntl;
                 } T32Gbl_SPI1_SDICntl;
    typedef union  T32Gbl_TS0_CLKCntl
          { UNSG32 u32;
            struct w32Gbl_TS0_CLKCntl;
                 } T32Gbl_TS0_CLKCntl;
    typedef union  T32Gbl_TS0_SOPCntl
          { UNSG32 u32;
            struct w32Gbl_TS0_SOPCntl;
                 } T32Gbl_TS0_SOPCntl;
    typedef union  T32Gbl_TS0_ERRORCntl
          { UNSG32 u32;
            struct w32Gbl_TS0_ERRORCntl;
                 } T32Gbl_TS0_ERRORCntl;
    typedef union  T32Gbl_TS0_VALDCntl
          { UNSG32 u32;
            struct w32Gbl_TS0_VALDCntl;
                 } T32Gbl_TS0_VALDCntl;
    typedef union  T32Gbl_TS0_PD0Cntl
          { UNSG32 u32;
            struct w32Gbl_TS0_PD0Cntl;
                 } T32Gbl_TS0_PD0Cntl;
    typedef union  T32Gbl_TS0_PD1Cntl
          { UNSG32 u32;
            struct w32Gbl_TS0_PD1Cntl;
                 } T32Gbl_TS0_PD1Cntl;
    typedef union  T32Gbl_TS0_PD2Cntl
          { UNSG32 u32;
            struct w32Gbl_TS0_PD2Cntl;
                 } T32Gbl_TS0_PD2Cntl;
    typedef union  T32Gbl_TS0_PD3Cntl
          { UNSG32 u32;
            struct w32Gbl_TS0_PD3Cntl;
                 } T32Gbl_TS0_PD3Cntl;
    typedef union  T32Gbl_TS0_PD4Cntl
          { UNSG32 u32;
            struct w32Gbl_TS0_PD4Cntl;
                 } T32Gbl_TS0_PD4Cntl;
    typedef union  T32Gbl_TS0_PD5Cntl
          { UNSG32 u32;
            struct w32Gbl_TS0_PD5Cntl;
                 } T32Gbl_TS0_PD5Cntl;
    typedef union  T32Gbl_TS0_PD6Cntl
          { UNSG32 u32;
            struct w32Gbl_TS0_PD6Cntl;
                 } T32Gbl_TS0_PD6Cntl;
    typedef union  T32Gbl_TS0_PD7Cntl
          { UNSG32 u32;
            struct w32Gbl_TS0_PD7Cntl;
                 } T32Gbl_TS0_PD7Cntl;
    typedef union  T32Gbl_STS2_CLKCntl
          { UNSG32 u32;
            struct w32Gbl_STS2_CLKCntl;
                 } T32Gbl_STS2_CLKCntl;
    typedef union  T32Gbl_STS2_ERRORCntl
          { UNSG32 u32;
            struct w32Gbl_STS2_ERRORCntl;
                 } T32Gbl_STS2_ERRORCntl;
    typedef union  T32Gbl_STS2_VALDCntl
          { UNSG32 u32;
            struct w32Gbl_STS2_VALDCntl;
                 } T32Gbl_STS2_VALDCntl;
    typedef union  T32Gbl_STS2_SOPCntl
          { UNSG32 u32;
            struct w32Gbl_STS2_SOPCntl;
                 } T32Gbl_STS2_SOPCntl;
    typedef union  T32Gbl_STS2_SDCntl
          { UNSG32 u32;
            struct w32Gbl_STS2_SDCntl;
                 } T32Gbl_STS2_SDCntl;
    typedef union  T32Gbl_RSTInCntl
          { UNSG32 u32;
            struct w32Gbl_RSTInCntl;
                 } T32Gbl_RSTInCntl;
    typedef union  T32Gbl_BYPS_OSCCntl
          { UNSG32 u32;
            struct w32Gbl_BYPS_OSCCntl;
                 } T32Gbl_BYPS_OSCCntl;
    typedef union  T32Gbl_ENG_ENCntl
          { UNSG32 u32;
            struct w32Gbl_ENG_ENCntl;
                 } T32Gbl_ENG_ENCntl;
    typedef union  T32Gbl_DVIO0Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO0Cntl;
                 } T32Gbl_DVIO0Cntl;
    typedef union  T32Gbl_DVIO1Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO1Cntl;
                 } T32Gbl_DVIO1Cntl;
    typedef union  T32Gbl_DVIO2Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO2Cntl;
                 } T32Gbl_DVIO2Cntl;
    typedef union  T32Gbl_DVIO3Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO3Cntl;
                 } T32Gbl_DVIO3Cntl;
    typedef union  T32Gbl_DVIO4Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO4Cntl;
                 } T32Gbl_DVIO4Cntl;
    typedef union  T32Gbl_DVIO5Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO5Cntl;
                 } T32Gbl_DVIO5Cntl;
    typedef union  T32Gbl_DVIO6Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO6Cntl;
                 } T32Gbl_DVIO6Cntl;
    typedef union  T32Gbl_DVIO7Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO7Cntl;
                 } T32Gbl_DVIO7Cntl;
    typedef union  T32Gbl_DVIO8Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO8Cntl;
                 } T32Gbl_DVIO8Cntl;
    typedef union  T32Gbl_DVIO9Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO9Cntl;
                 } T32Gbl_DVIO9Cntl;
    typedef union  T32Gbl_DVIO10Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO10Cntl;
                 } T32Gbl_DVIO10Cntl;
    typedef union  T32Gbl_DVIO11Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO11Cntl;
                 } T32Gbl_DVIO11Cntl;
    typedef union  T32Gbl_DVIO12Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO12Cntl;
                 } T32Gbl_DVIO12Cntl;
    typedef union  T32Gbl_DVIO13Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO13Cntl;
                 } T32Gbl_DVIO13Cntl;
    typedef union  T32Gbl_DVIO14Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO14Cntl;
                 } T32Gbl_DVIO14Cntl;
    typedef union  T32Gbl_DVIO15Cntl
          { UNSG32 u32;
            struct w32Gbl_DVIO15Cntl;
                 } T32Gbl_DVIO15Cntl;
    typedef union  T32Gbl_DV0_CLKCntl
          { UNSG32 u32;
            struct w32Gbl_DV0_CLKCntl;
                 } T32Gbl_DV0_CLKCntl;
    typedef union  T32Gbl_DV0_HSCntl
          { UNSG32 u32;
            struct w32Gbl_DV0_HSCntl;
                 } T32Gbl_DV0_HSCntl;
    typedef union  T32Gbl_DV0_VSCntl
          { UNSG32 u32;
            struct w32Gbl_DV0_VSCntl;
                 } T32Gbl_DV0_VSCntl;
    typedef union  T32Gbl_DV0_FIDCntl
          { UNSG32 u32;
            struct w32Gbl_DV0_FIDCntl;
                 } T32Gbl_DV0_FIDCntl;
    typedef union  T32Gbl_DV1_CLKCntl
          { UNSG32 u32;
            struct w32Gbl_DV1_CLKCntl;
                 } T32Gbl_DV1_CLKCntl;
    typedef union  T32Gbl_DV1_HSCntl
          { UNSG32 u32;
            struct w32Gbl_DV1_HSCntl;
                 } T32Gbl_DV1_HSCntl;
    typedef union  T32Gbl_DV1_VSCntl
          { UNSG32 u32;
            struct w32Gbl_DV1_VSCntl;
                 } T32Gbl_DV1_VSCntl;
    typedef union  T32Gbl_DV1_FIDCntl
          { UNSG32 u32;
            struct w32Gbl_DV1_FIDCntl;
                 } T32Gbl_DV1_FIDCntl;
    typedef union  T32Gbl_I2S0_BCLKCntl
          { UNSG32 u32;
            struct w32Gbl_I2S0_BCLKCntl;
                 } T32Gbl_I2S0_BCLKCntl;
    typedef union  T32Gbl_I2S0_LRCKCntl
          { UNSG32 u32;
            struct w32Gbl_I2S0_LRCKCntl;
                 } T32Gbl_I2S0_LRCKCntl;
    typedef union  T32Gbl_I2S0_DO0Cntl
          { UNSG32 u32;
            struct w32Gbl_I2S0_DO0Cntl;
                 } T32Gbl_I2S0_DO0Cntl;
    typedef union  T32Gbl_I2S0_DO1Cntl
          { UNSG32 u32;
            struct w32Gbl_I2S0_DO1Cntl;
                 } T32Gbl_I2S0_DO1Cntl;
    typedef union  T32Gbl_I2S0_DO2Cntl
          { UNSG32 u32;
            struct w32Gbl_I2S0_DO2Cntl;
                 } T32Gbl_I2S0_DO2Cntl;
    typedef union  T32Gbl_I2S0_DO3Cntl
          { UNSG32 u32;
            struct w32Gbl_I2S0_DO3Cntl;
                 } T32Gbl_I2S0_DO3Cntl;
    typedef union  T32Gbl_I2S1_LRCKCntl
          { UNSG32 u32;
            struct w32Gbl_I2S1_LRCKCntl;
                 } T32Gbl_I2S1_LRCKCntl;
    typedef union  T32Gbl_I2S1_BCLKCntl
          { UNSG32 u32;
            struct w32Gbl_I2S1_BCLKCntl;
                 } T32Gbl_I2S1_BCLKCntl;
    typedef union  T32Gbl_I2S1_DOCntl
          { UNSG32 u32;
            struct w32Gbl_I2S1_DOCntl;
                 } T32Gbl_I2S1_DOCntl;
    typedef union  T32Gbl_I2S2_LRCKCntl
          { UNSG32 u32;
            struct w32Gbl_I2S2_LRCKCntl;
                 } T32Gbl_I2S2_LRCKCntl;
    typedef union  T32Gbl_I2S2_BCLKCntl
          { UNSG32 u32;
            struct w32Gbl_I2S2_BCLKCntl;
                 } T32Gbl_I2S2_BCLKCntl;
    typedef union  T32Gbl_I2S2_DICntl
          { UNSG32 u32;
            struct w32Gbl_I2S2_DICntl;
                 } T32Gbl_I2S2_DICntl;
    typedef union  T32Gbl_SPDIFOCntl
          { UNSG32 u32;
            struct w32Gbl_SPDIFOCntl;
                 } T32Gbl_SPDIFOCntl;
    typedef union  T32Gbl_HDMI_CECCntl
          { UNSG32 u32;
            struct w32Gbl_HDMI_CECCntl;
                 } T32Gbl_HDMI_CECCntl;
    ///////////////////////////////////////////////////////////

    typedef union  TGbl_pinMux
          { UNSG32 u32[2];
            struct {
            struct w32Gbl_pinMux;
            struct w32Gbl_pinMux1;
                   };
                 } TGbl_pinMux;
    typedef union  TGbl_bootStrap
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_bootStrap;
                   };
                 } TGbl_bootStrap;
    typedef union  TGbl_chipCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_chipCntl;
                   };
                 } TGbl_chipCntl;
    typedef union  TGbl_anaGrpCtl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_anaGrpCtl;
                   };
                 } TGbl_anaGrpCtl;
    typedef union  TGbl_sysPllCtl
          { UNSG32 u32[3];
            struct {
            struct w32Gbl_sysPllCtl;
            struct w32Gbl_sysPllCtl1;
            struct w32Gbl_sysPllCtl2;
                   };
                 } TGbl_sysPllCtl;
    typedef union  TGbl_memPllCtl
          { UNSG32 u32[3];
            struct {
            struct w32Gbl_memPllCtl;
            struct w32Gbl_memPllCtl1;
            struct w32Gbl_memPllCtl2;
                   };
                 } TGbl_memPllCtl;
    typedef union  TGbl_cpu1PllCtl
          { UNSG32 u32[5];
            struct {
            struct w32Gbl_cpu1PllCtl;
            struct w32Gbl_cpu1PllCtl1;
            struct w32Gbl_cpu1PllCtl2;
            struct w32Gbl_cpu1PllCtl3;
            struct w32Gbl_cpu1PllCtl4;
                   };
                 } TGbl_cpu1PllCtl;
    typedef union  TGbl_avPllCtl
          { UNSG32 u32[19];
            struct {
            struct w32Gbl_avPllCtl;
            struct w32Gbl_avPllCtl1;
            struct w32Gbl_avPllCtl2;
            struct w32Gbl_avPllCtl3;
            struct w32Gbl_avPllCtl4;
            struct w32Gbl_avPllCtl5;
            struct w32Gbl_avPllCtl6;
            struct w32Gbl_avPllCtl7;
            struct w32Gbl_avPllCtl8;
            struct w32Gbl_avPllCtl9;
            struct w32Gbl_avPllCtl10;
            struct w32Gbl_avPllCtl11;
            struct w32Gbl_avPllCtl12;
            struct w32Gbl_avPllCtl13;
            struct w32Gbl_avPllCtl14;
            struct w32Gbl_avPllCtl15;
            struct w32Gbl_avPllCtl16;
            struct w32Gbl_avPllCtl17;
            struct w32Gbl_avPllCtl18;
                   };
                 } TGbl_avPllCtl;
    typedef union  TGbl_pllStatus
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_pllStatus;
                   };
                 } TGbl_pllStatus;
    typedef union  TGbl_clkEnable
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_clkEnable;
                   };
                 } TGbl_clkEnable;
    typedef union  TGbl_clkSelect
          { UNSG32 u32[4];
            struct {
            struct w32Gbl_clkSelect;
            struct w32Gbl_clkSelect1;
            struct w32Gbl_clkSelect2;
            struct w32Gbl_clkSelect3;
                   };
                 } TGbl_clkSelect;
    typedef union  TGbl_ClkSwitch
          { UNSG32 u32[2];
            struct {
            struct w32Gbl_ClkSwitch;
            struct w32Gbl_ClkSwitch1;
                   };
                 } TGbl_ClkSwitch;
    typedef union  TGbl_PadSelect
          { UNSG32 u32[2];
            struct {
            struct w32Gbl_PadSelect;
            struct w32Gbl_PadSelect1;
                   };
                 } TGbl_PadSelect;
    typedef union  TGbl_ResetTrigger
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ResetTrigger;
                   };
                 } TGbl_ResetTrigger;
    typedef union  TGbl_ResetStatus
          { UNSG32 u32[2];
            struct {
            struct w32Gbl_ResetStatus;
            struct w32Gbl_ResetStatus1;
                   };
                 } TGbl_ResetStatus;
    typedef union  TGbl_sw_generic0
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_sw_generic0;
                   };
                 } TGbl_sw_generic0;
    typedef union  TGbl_sw_generic1
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_sw_generic1;
                   };
                 } TGbl_sw_generic1;
    typedef union  TGbl_sw_generic2
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_sw_generic2;
                   };
                 } TGbl_sw_generic2;
    typedef union  TGbl_sw_generic3
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_sw_generic3;
                   };
                 } TGbl_sw_generic3;
    typedef union  TGbl_ChipCntl0
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ChipCntl0;
                   };
                 } TGbl_ChipCntl0;
    typedef union  TGbl_ChipCntl1
          { UNSG32 u32[2];
            struct {
            struct w32Gbl_ChipCntl1;
            struct w32Gbl_ChipCntl11;
                   };
                 } TGbl_ChipCntl1;
    typedef union  TGbl_RWTC_avio31to0
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RWTC_avio31to0;
                   };
                 } TGbl_RWTC_avio31to0;
    typedef union  TGbl_RWTC_avio43to32
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RWTC_avio43to32;
                   };
                 } TGbl_RWTC_avio43to32;
    typedef union  TGbl_RWTC_gfx2D31to0
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RWTC_gfx2D31to0;
                   };
                 } TGbl_RWTC_gfx2D31to0;
    typedef union  TGbl_RWTC_gfx2D43to32
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RWTC_gfx2D43to32;
                   };
                 } TGbl_RWTC_gfx2D43to32;
    typedef union  TGbl_RWTC_perif31to0
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RWTC_perif31to0;
                   };
                 } TGbl_RWTC_perif31to0;
    typedef union  TGbl_RWTC_perif43to32
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RWTC_perif43to32;
                   };
                 } TGbl_RWTC_perif43to32;
    typedef union  TGbl_RWTC_soc31to0
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RWTC_soc31to0;
                   };
                 } TGbl_RWTC_soc31to0;
    typedef union  TGbl_RWTC_soc43to32
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RWTC_soc43to32;
                   };
                 } TGbl_RWTC_soc43to32;
    typedef union  TGbl_RWTC_vPro31to0
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RWTC_vPro31to0;
                   };
                 } TGbl_RWTC_vPro31to0;
    typedef union  TGbl_RWTC_vPro43to32
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RWTC_vPro43to32;
                   };
                 } TGbl_RWTC_vPro43to32;
    typedef union  TGbl_ProductId
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ProductId;
                   };
                 } TGbl_ProductId;
    typedef union  TGbl_ProductId_ext
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ProductId_ext;
                   };
                 } TGbl_ProductId_ext;
    typedef union  TGbl_FPGAR
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_FPGAR;
                   };
                 } TGbl_FPGAR;
    typedef union  TGbl_FPGARW
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_FPGARW;
                   };
                 } TGbl_FPGARW;
    typedef union  TGbl_RingOscCtl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RingOscCtl;
                   };
                 } TGbl_RingOscCtl;
    typedef union  TGbl_PLLBypsBootStrapEn
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_PLLBypsBootStrapEn;
                   };
                 } TGbl_PLLBypsBootStrapEn;
    typedef union  TGbl_IO_CALIBRATE_IN
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_IO_CALIBRATE_IN;
                   };
                 } TGbl_IO_CALIBRATE_IN;
    typedef union  TGbl_IO_CALBRATE_OUT
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_IO_CALBRATE_OUT;
                   };
                 } TGbl_IO_CALBRATE_OUT;
    typedef union  TGbl_SECURE_SCAN_EN
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_SECURE_SCAN_EN;
                   };
                 } TGbl_SECURE_SCAN_EN;
    typedef union  TGbl_NandWPn
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NandWPn;
                   };
                 } TGbl_NandWPn;
    typedef union  TGbl_RingCntCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RingCntCntl;
                   };
                 } TGbl_RingCntCntl;
    typedef union  TGbl_CenterCnt
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_CenterCnt;
                   };
                 } TGbl_CenterCnt;
    typedef union  TGbl_avioTopCnt
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_avioTopCnt;
                   };
                 } TGbl_avioTopCnt;
    typedef union  TGbl_avioBotCnt
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_avioBotCnt;
                   };
                 } TGbl_avioBotCnt;
    typedef union  TGbl_usbTopCnt
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_usbTopCnt;
                   };
                 } TGbl_usbTopCnt;
    typedef union  TGbl_cpu1TopCnt
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_cpu1TopCnt;
                   };
                 } TGbl_cpu1TopCnt;
    typedef union  TGbl_cpu0TopCnt
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_cpu0TopCnt;
                   };
                 } TGbl_cpu0TopCnt;
    typedef union  TGbl_cpuBotCnt
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_cpuBotCnt;
                   };
                 } TGbl_cpuBotCnt;
    typedef union  TGbl_topRiteCnt
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_topRiteCnt;
                   };
                 } TGbl_topRiteCnt;
    typedef union  TGbl_midRiteCnt
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_midRiteCnt;
                   };
                 } TGbl_midRiteCnt;
    typedef union  TGbl_botRiteCnt
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_botRiteCnt;
                   };
                 } TGbl_botRiteCnt;
    typedef union  TGbl_NAND_IO0Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_IO0Cntl;
                   };
                 } TGbl_NAND_IO0Cntl;
    typedef union  TGbl_NAND_IO1Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_IO1Cntl;
                   };
                 } TGbl_NAND_IO1Cntl;
    typedef union  TGbl_NAND_IO2Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_IO2Cntl;
                   };
                 } TGbl_NAND_IO2Cntl;
    typedef union  TGbl_NAND_IO3Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_IO3Cntl;
                   };
                 } TGbl_NAND_IO3Cntl;
    typedef union  TGbl_NAND_IO4Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_IO4Cntl;
                   };
                 } TGbl_NAND_IO4Cntl;
    typedef union  TGbl_NAND_IO5Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_IO5Cntl;
                   };
                 } TGbl_NAND_IO5Cntl;
    typedef union  TGbl_NAND_IO6Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_IO6Cntl;
                   };
                 } TGbl_NAND_IO6Cntl;
    typedef union  TGbl_NAND_IO7Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_IO7Cntl;
                   };
                 } TGbl_NAND_IO7Cntl;
    typedef union  TGbl_NAND_ALECntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_ALECntl;
                   };
                 } TGbl_NAND_ALECntl;
    typedef union  TGbl_NAND_CLECntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_CLECntl;
                   };
                 } TGbl_NAND_CLECntl;
    typedef union  TGbl_NAND_WEnCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_WEnCntl;
                   };
                 } TGbl_NAND_WEnCntl;
    typedef union  TGbl_NAND_REnCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_REnCntl;
                   };
                 } TGbl_NAND_REnCntl;
    typedef union  TGbl_NAND_WPnCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_WPnCntl;
                   };
                 } TGbl_NAND_WPnCntl;
    typedef union  TGbl_NAND_CEn0Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_CEn0Cntl;
                   };
                 } TGbl_NAND_CEn0Cntl;
    typedef union  TGbl_NAND_CEn1Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_CEn1Cntl;
                   };
                 } TGbl_NAND_CEn1Cntl;
    typedef union  TGbl_NAND_RDY0Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_RDY0Cntl;
                   };
                 } TGbl_NAND_RDY0Cntl;
    typedef union  TGbl_NAND_RDY1Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_NAND_RDY1Cntl;
                   };
                 } TGbl_NAND_RDY1Cntl;
    typedef union  TGbl_ET_TXCLKIOCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_TXCLKIOCntl;
                   };
                 } TGbl_ET_TXCLKIOCntl;
    typedef union  TGbl_ET_TXD0Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_TXD0Cntl;
                   };
                 } TGbl_ET_TXD0Cntl;
    typedef union  TGbl_ET_TXD1Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_TXD1Cntl;
                   };
                 } TGbl_ET_TXD1Cntl;
    typedef union  TGbl_ET_TXD2Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_TXD2Cntl;
                   };
                 } TGbl_ET_TXD2Cntl;
    typedef union  TGbl_ET_TXD3Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_TXD3Cntl;
                   };
                 } TGbl_ET_TXD3Cntl;
    typedef union  TGbl_ET_TXENCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_TXENCntl;
                   };
                 } TGbl_ET_TXENCntl;
    typedef union  TGbl_ET_TXERCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_TXERCntl;
                   };
                 } TGbl_ET_TXERCntl;
    typedef union  TGbl_ET_RXCLKICntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_RXCLKICntl;
                   };
                 } TGbl_ET_RXCLKICntl;
    typedef union  TGbl_ET_RXD0Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_RXD0Cntl;
                   };
                 } TGbl_ET_RXD0Cntl;
    typedef union  TGbl_ET_RXD1Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_RXD1Cntl;
                   };
                 } TGbl_ET_RXD1Cntl;
    typedef union  TGbl_ET_RXD2Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_RXD2Cntl;
                   };
                 } TGbl_ET_RXD2Cntl;
    typedef union  TGbl_ET_RXD3Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_RXD3Cntl;
                   };
                 } TGbl_ET_RXD3Cntl;
    typedef union  TGbl_ET_CRSCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_CRSCntl;
                   };
                 } TGbl_ET_CRSCntl;
    typedef union  TGbl_ET_COLCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_COLCntl;
                   };
                 } TGbl_ET_COLCntl;
    typedef union  TGbl_ET_RXDVCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_RXDVCntl;
                   };
                 } TGbl_ET_RXDVCntl;
    typedef union  TGbl_ET_RXERCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_RXERCntl;
                   };
                 } TGbl_ET_RXERCntl;
    typedef union  TGbl_ET_MDCCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_MDCCntl;
                   };
                 } TGbl_ET_MDCCntl;
    typedef union  TGbl_ET_MDIOCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_MDIOCntl;
                   };
                 } TGbl_ET_MDIOCntl;
    typedef union  TGbl_ET_RCLKOCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ET_RCLKOCntl;
                   };
                 } TGbl_ET_RCLKOCntl;
    typedef union  TGbl_TW0_SCLCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TW0_SCLCntl;
                   };
                 } TGbl_TW0_SCLCntl;
    typedef union  TGbl_TW0_SDACntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TW0_SDACntl;
                   };
                 } TGbl_TW0_SDACntl;
    typedef union  TGbl_TW1_SCLCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TW1_SCLCntl;
                   };
                 } TGbl_TW1_SCLCntl;
    typedef union  TGbl_TW1_SDACntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TW1_SDACntl;
                   };
                 } TGbl_TW1_SDACntl;
    typedef union  TGbl_SPI1_SS0nCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_SPI1_SS0nCntl;
                   };
                 } TGbl_SPI1_SS0nCntl;
    typedef union  TGbl_SPI1_SS1nCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_SPI1_SS1nCntl;
                   };
                 } TGbl_SPI1_SS1nCntl;
    typedef union  TGbl_SPI1_SS2nCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_SPI1_SS2nCntl;
                   };
                 } TGbl_SPI1_SS2nCntl;
    typedef union  TGbl_SPI1_SS3nCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_SPI1_SS3nCntl;
                   };
                 } TGbl_SPI1_SS3nCntl;
    typedef union  TGbl_SPI1_SCLKCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_SPI1_SCLKCntl;
                   };
                 } TGbl_SPI1_SCLKCntl;
    typedef union  TGbl_SPI1_SDOCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_SPI1_SDOCntl;
                   };
                 } TGbl_SPI1_SDOCntl;
    typedef union  TGbl_SPI1_SDICntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_SPI1_SDICntl;
                   };
                 } TGbl_SPI1_SDICntl;
    typedef union  TGbl_TS0_CLKCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_CLKCntl;
                   };
                 } TGbl_TS0_CLKCntl;
    typedef union  TGbl_TS0_SOPCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_SOPCntl;
                   };
                 } TGbl_TS0_SOPCntl;
    typedef union  TGbl_TS0_ERRORCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_ERRORCntl;
                   };
                 } TGbl_TS0_ERRORCntl;
    typedef union  TGbl_TS0_VALDCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_VALDCntl;
                   };
                 } TGbl_TS0_VALDCntl;
    typedef union  TGbl_TS0_PD0Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_PD0Cntl;
                   };
                 } TGbl_TS0_PD0Cntl;
    typedef union  TGbl_TS0_PD1Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_PD1Cntl;
                   };
                 } TGbl_TS0_PD1Cntl;
    typedef union  TGbl_TS0_PD2Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_PD2Cntl;
                   };
                 } TGbl_TS0_PD2Cntl;
    typedef union  TGbl_TS0_PD3Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_PD3Cntl;
                   };
                 } TGbl_TS0_PD3Cntl;
    typedef union  TGbl_TS0_PD4Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_PD4Cntl;
                   };
                 } TGbl_TS0_PD4Cntl;
    typedef union  TGbl_TS0_PD5Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_PD5Cntl;
                   };
                 } TGbl_TS0_PD5Cntl;
    typedef union  TGbl_TS0_PD6Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_PD6Cntl;
                   };
                 } TGbl_TS0_PD6Cntl;
    typedef union  TGbl_TS0_PD7Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_TS0_PD7Cntl;
                   };
                 } TGbl_TS0_PD7Cntl;
    typedef union  TGbl_STS2_CLKCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_STS2_CLKCntl;
                   };
                 } TGbl_STS2_CLKCntl;
    typedef union  TGbl_STS2_ERRORCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_STS2_ERRORCntl;
                   };
                 } TGbl_STS2_ERRORCntl;
    typedef union  TGbl_STS2_VALDCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_STS2_VALDCntl;
                   };
                 } TGbl_STS2_VALDCntl;
    typedef union  TGbl_STS2_SOPCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_STS2_SOPCntl;
                   };
                 } TGbl_STS2_SOPCntl;
    typedef union  TGbl_STS2_SDCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_STS2_SDCntl;
                   };
                 } TGbl_STS2_SDCntl;
    typedef union  TGbl_RSTInCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_RSTInCntl;
                   };
                 } TGbl_RSTInCntl;
    typedef union  TGbl_BYPS_OSCCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_BYPS_OSCCntl;
                   };
                 } TGbl_BYPS_OSCCntl;
    typedef union  TGbl_ENG_ENCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_ENG_ENCntl;
                   };
                 } TGbl_ENG_ENCntl;
    typedef union  TGbl_DVIO0Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO0Cntl;
                   };
                 } TGbl_DVIO0Cntl;
    typedef union  TGbl_DVIO1Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO1Cntl;
                   };
                 } TGbl_DVIO1Cntl;
    typedef union  TGbl_DVIO2Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO2Cntl;
                   };
                 } TGbl_DVIO2Cntl;
    typedef union  TGbl_DVIO3Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO3Cntl;
                   };
                 } TGbl_DVIO3Cntl;
    typedef union  TGbl_DVIO4Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO4Cntl;
                   };
                 } TGbl_DVIO4Cntl;
    typedef union  TGbl_DVIO5Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO5Cntl;
                   };
                 } TGbl_DVIO5Cntl;
    typedef union  TGbl_DVIO6Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO6Cntl;
                   };
                 } TGbl_DVIO6Cntl;
    typedef union  TGbl_DVIO7Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO7Cntl;
                   };
                 } TGbl_DVIO7Cntl;
    typedef union  TGbl_DVIO8Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO8Cntl;
                   };
                 } TGbl_DVIO8Cntl;
    typedef union  TGbl_DVIO9Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO9Cntl;
                   };
                 } TGbl_DVIO9Cntl;
    typedef union  TGbl_DVIO10Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO10Cntl;
                   };
                 } TGbl_DVIO10Cntl;
    typedef union  TGbl_DVIO11Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO11Cntl;
                   };
                 } TGbl_DVIO11Cntl;
    typedef union  TGbl_DVIO12Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO12Cntl;
                   };
                 } TGbl_DVIO12Cntl;
    typedef union  TGbl_DVIO13Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO13Cntl;
                   };
                 } TGbl_DVIO13Cntl;
    typedef union  TGbl_DVIO14Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO14Cntl;
                   };
                 } TGbl_DVIO14Cntl;
    typedef union  TGbl_DVIO15Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DVIO15Cntl;
                   };
                 } TGbl_DVIO15Cntl;
    typedef union  TGbl_DV0_CLKCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DV0_CLKCntl;
                   };
                 } TGbl_DV0_CLKCntl;
    typedef union  TGbl_DV0_HSCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DV0_HSCntl;
                   };
                 } TGbl_DV0_HSCntl;
    typedef union  TGbl_DV0_VSCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DV0_VSCntl;
                   };
                 } TGbl_DV0_VSCntl;
    typedef union  TGbl_DV0_FIDCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DV0_FIDCntl;
                   };
                 } TGbl_DV0_FIDCntl;
    typedef union  TGbl_DV1_CLKCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DV1_CLKCntl;
                   };
                 } TGbl_DV1_CLKCntl;
    typedef union  TGbl_DV1_HSCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DV1_HSCntl;
                   };
                 } TGbl_DV1_HSCntl;
    typedef union  TGbl_DV1_VSCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DV1_VSCntl;
                   };
                 } TGbl_DV1_VSCntl;
    typedef union  TGbl_DV1_FIDCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_DV1_FIDCntl;
                   };
                 } TGbl_DV1_FIDCntl;
    typedef union  TGbl_I2S0_BCLKCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S0_BCLKCntl;
                   };
                 } TGbl_I2S0_BCLKCntl;
    typedef union  TGbl_I2S0_LRCKCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S0_LRCKCntl;
                   };
                 } TGbl_I2S0_LRCKCntl;
    typedef union  TGbl_I2S0_DO0Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S0_DO0Cntl;
                   };
                 } TGbl_I2S0_DO0Cntl;
    typedef union  TGbl_I2S0_DO1Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S0_DO1Cntl;
                   };
                 } TGbl_I2S0_DO1Cntl;
    typedef union  TGbl_I2S0_DO2Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S0_DO2Cntl;
                   };
                 } TGbl_I2S0_DO2Cntl;
    typedef union  TGbl_I2S0_DO3Cntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S0_DO3Cntl;
                   };
                 } TGbl_I2S0_DO3Cntl;
    typedef union  TGbl_I2S1_LRCKCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S1_LRCKCntl;
                   };
                 } TGbl_I2S1_LRCKCntl;
    typedef union  TGbl_I2S1_BCLKCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S1_BCLKCntl;
                   };
                 } TGbl_I2S1_BCLKCntl;
    typedef union  TGbl_I2S1_DOCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S1_DOCntl;
                   };
                 } TGbl_I2S1_DOCntl;
    typedef union  TGbl_I2S2_LRCKCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S2_LRCKCntl;
                   };
                 } TGbl_I2S2_LRCKCntl;
    typedef union  TGbl_I2S2_BCLKCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S2_BCLKCntl;
                   };
                 } TGbl_I2S2_BCLKCntl;
    typedef union  TGbl_I2S2_DICntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_I2S2_DICntl;
                   };
                 } TGbl_I2S2_DICntl;
    typedef union  TGbl_SPDIFOCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_SPDIFOCntl;
                   };
                 } TGbl_SPDIFOCntl;
    typedef union  TGbl_HDMI_CECCntl
          { UNSG32 u32[1];
            struct {
            struct w32Gbl_HDMI_CECCntl;
                   };
                 } TGbl_HDMI_CECCntl;

    ///////////////////////////////////////////////////////////
     SIGN32 Gbl_drvrd(SIE_Gbl *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 Gbl_drvwr(SIE_Gbl *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void Gbl_reset(SIE_Gbl *p);
     SIGN32 Gbl_cmp  (SIE_Gbl *p, SIE_Gbl *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define Gbl_check(p,pie,pfx,hLOG) Gbl_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define Gbl_print(p,    pfx,hLOG) Gbl_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif

