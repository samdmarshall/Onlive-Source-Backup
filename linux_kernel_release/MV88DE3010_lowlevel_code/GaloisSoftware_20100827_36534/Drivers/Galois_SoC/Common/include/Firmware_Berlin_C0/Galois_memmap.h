#ifndef Galois_memmap_h
#define Galois_memmap_h (){}


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



#ifndef h_MEMMAP
#define h_MEMMAP (){}

    #define        MEMMAP_DRAM_CACHE_BASE                      0x0
    #define        MEMMAP_DRAM_CACHE_SIZE                      0x40000000
    #define        MEMMAP_DRAM_CACHE_DEC_BIT                   0x1E
    #define        MEMMAP_DRAM_UNCACHE_BASE                    0x40000000
    #define        MEMMAP_DRAM_UNCACHE_SIZE                    0x40000000
    #define        MEMMAP_DRAM_UNCACHE_DEC_BIT                 0x1E
    #define        MEMMAP_PCIE_UPSTREAM_BASE                   0x80000000
    #define        MEMMAP_PCIE_UPSTREAM_SIZE                   0x20000000
    #define        MEMMAP_PCIE_UPSTREAM_DEC_BIT                0x1D
    #define        MEMMAP_DXBAR_REG_BASE                       0xA0000000
    #define        MEMMAP_DXBAR_REG_SIZE                       0x10000
    #define        MEMMAP_DXBAR_REG_DEC_BIT                    0x10
    #define        MEMMAP_PXBAR_REG_BASE                       0xA0010000
    #define        MEMMAP_PXBAR_REG_SIZE                       0x10000
    #define        MEMMAP_PXBAR_REG_DEC_BIT                    0x10
    #define        MEMMAP_CXBAR_REG_BASE                       0xA0020000
    #define        MEMMAP_CXBAR_REG_SIZE                       0x10000
    #define        MEMMAP_CXBAR_REG_DEC_BIT                    0x10
    #define        MEMMAP_RESERVED_BASE                        0xA0030000
    #define        MEMMAP_PCIE_DOWNSTREAM_BASE                 0xE0000000
    #define        MEMMAP_PCIE_DOWNSTREAM_SIZE                 0x10000000
    #define        MEMMAP_PCIE_DOWNSTREAM_DEC_BIT              0x1C
    #define        MEMMAP_SPI_FLASH_BASE                       0xF0000000
    #define        MEMMAP_SPI_FLASH_SIZE                       0x4000000
    #define        MEMMAP_SPI_FLASH_DEC_BIT                    0x1A
    #define        MEMMAP_SUNOL_FLASH_BASE                     0xF4000000
    #define        MEMMAP_SUNOL_FLASH_SIZE                     0x2000000
    #define        MEMMAP_SUNOL_FLASH_DEC_BIT                  0x19
    #define        MEMMAP_SLOW_REG_BASE                        0xF7BC0000
    #define        MEMMAP_AVIO_RESR0_BASE                      0xF7BC0000
    #define        MEMMAP_AVIO_RESR0_SIZE                      0x40000
    #define        MEMMAP_AVIO_RESR0_DEC_BIT                   0x12
    #define        MEMMAP_ZSP_REG_BASE                         0xF7C00000
    #define        MEMMAP_ZSP_REG_SIZE                         0x80000
    #define        MEMMAP_ZSP_REG_DEC_BIT                      0x13
    #define        MEMMAP_AVIO_RESR1_BASE                      0xF7C80000
    #define        MEMMAP_AVIO_RESR1_SIZE_                     0x20000
    #define        MEMMAP_AVIO_RESR1_DEC_BIT                   0x11
    #define        MEMMAP_SOC_REG_BASE                         0xF7CA0000
    #define        MEMMAP_SOC_REG_SIZE                         0x10000
    #define        MEMMAP_SOC_REG_DEC_BIT                      0x10
    #define        MEMMAP_DDR_REG_BASE                         0xF7CB0000
    #define        MEMMAP_DDR_REG_SIZE                         0x10000
    #define        MEMMAP_DDR_REG_DEC_BIT                      0x10
    #define        MEMMAP_TSI_REG_BASE                         0xF7CC0000
    #define        MEMMAP_TSI_REG_SIZE                         0x40000
    #define        MEMMAP_TSI_REG_DEC_BIT                      0x12
    #define        MEMMAP_AG_DHUB_REG_BASE                     0xF7D00000
    #define        MEMMAP_AG_DHUB_REG_SIZE                     0x20000
    #define        MEMMAP_AG_DHUB_REG_DEC_BIT                  0x11
    #define        MEMMAP_APP_REG_BASE                         0xF7D30000
    #define        MEMMAP_APP_REG_SIZE                         0x10000
    #define        MEMMAP_APP_REG_DEC_BIT                      0x10
    #define        MEMMAP_SPU_REG_BASE                         0xF7D40000
    #define        MEMMAP_SPU_REG_SIZE                         0x10000
    #define        MEMMAP_SPU_REG_DEC_BIT                      0x10
    #define        MEMMAP_SDIO_REG_BASE                        0xF7D50000
    #define        MEMMAP_SDIO_REG_SIZE                        0x10000
    #define        MEMMAP_SDIO_REG_DEC_BIT                     0x10
    #define        MEMMAP_PTP_REG_BASE                         0xF7D60000
    #define        MEMMAP_PTP_REG_SIZE                         0x10000
    #define        MEMMAP_PTP_REG_DEC_BIT                      0x10
    #define        MEMMAP_PB_REG_BASE                          0xF7D70000
    #define        MEMMAP_PB_REG_SIZE                          0x10000
    #define        MEMMAP_PB_REG_DEC_BIT                       0x10
    #define        MEMMAP_SX_REG_BASE                          0xF7D80000
    #define        MEMMAP_SX_REG_SIZE                          0x10000
    #define        MEMMAP_SX_REG_DEC_BIT                       0x10
    #define        MEMMAP_MX_REG_BASE                          0xF7D90000
    #define        MEMMAP_MX_REG_SIZE                          0x10000
    #define        MEMMAP_MX_REG_DEC_BIT                       0x10
    #define        MEMMAP_NX_REG_BASE                          0xF7DA0000
    #define        MEMMAP_NX_REG_SIZE                          0x10000
    #define        MEMMAP_NX_REG_DEC_BIT                       0x10
    #define        MEMMAP_VOP_PG_REG_BASE                      0xF7DB0000
    #define        MEMMAP_VOP_PG_REG_SIZE                      0x10000
    #define        MEMMAP_VOP_PG_REG_DEC_BIT                   0x10
    #define        MEMMAP_CMD_PACK_REG_BASE                    0xF7DC0000
    #define        MEMMAP_CMD_PACK_REG_SIZE                    0x10000
    #define        MEMMAP_CMD_PACK_REG_DEC_BIT                 0x10
    #define        MEMMAP_VOP_MIXER_REG_BASE                   0xF7DD0000
    #define        MEMMAP_VOP_MIXER_REG_SIZE                   0x10000
    #define        MEMMAP_VOP_MIXER_REG_DEC_BIT                0x10
    #define        MEMMAP_VOP_VIDEO_REG_BASE                   0xF7DE0000
    #define        MEMMAP_VOP_VIDEO_REG_SIZE                   0x10000
    #define        MEMMAP_VOP_VIDEO_REG_DEC_BIT                0x10
    #define        MEMMAP_VOP_PIP_REG_BASE                     0xF7DF0000
    #define        MEMMAP_VOP_PIP_REG_SIZE                     0x10000
    #define        MEMMAP_VOP_PIP_REG_DEC_BIT                  0x10
    #define        MEMMAP_VOP_OSD_REG_BASE                     0xF7E00000
    #define        MEMMAP_VOP_OSD_REG_SIZE                     0x10000
    #define        MEMMAP_VOP_OSD_REG_DEC_BIT                  0x10
    #define        MEMMAP_AVIO_RESR2_BASE                      0xF7E10000
    #define        MEMMAP_AVIO_RESR2_SIZE                      0x10000
    #define        MEMMAP_AVIO_RESR2_DEC_BIT                   0x10
    #define        MEMMAP_AVIO_RESR3_BASE                      0xF7E20000
    #define        MEMMAP_AVIO_RESR3_SIZE                      0x10000
    #define        MEMMAP_AVIO_RESR3_DEC_BIT                   0x10
    #define        MEMMAP_PCI_REG_BASE                         0xF7E30000
    #define        MEMMAP_PCI_REG_SIZE                         0x10000
    #define        MEMMAP_PCI_REG_DEC_BIT                      0x10
    #define        MEMMAP_PCIE_REG_BASE                        0xF7E40000
    #define        MEMMAP_PCIE_REG_SIZE                        0x10000
    #define        MEMMAP_PCIE_REG_DEC_BIT                     0x10
    #define        MEMMAP_ETHERNET_REG_BASE                    0xF7E50000
    #define        MEMMAP_ETHERNET_REG_SIZE                    0x10000
    #define        MEMMAP_ETHERNET_REG_DEC_BIT                 0x10
    #define        MEMMAP_SPDIF_REG_BASE                       0xF7E60000
    #define        MEMMAP_SPDIF_REG_SIZE                       0x10000
    #define        MEMMAP_SPDIF_REG_DEC_BIT                    0x10
    #define        MEMMAP_I2S_REG_BASE                         0xF7E70000
    #define        MEMMAP_I2S_REG_SIZE                         0x10000
    #define        MEMMAP_I2S_REG_DEC_BIT                      0x10
    #define        MEMMAP_APBPERIF_REG_BASE                    0xF7E80000
    #define        MEMMAP_APBPERIF_REG_SIZE                    0x10000
    #define        MEMMAP_APBPERIF_REG_DEC_BIT                 0x10
    #define        MEMMAP_SATA_REG_BASE                        0xF7E90000
    #define        MEMMAP_SATA_REG_SIZE                        0x10000
    #define        MEMMAP_SATA_REG_DEC_BIT                     0x10
    #define        MEMMAP_CHIP_CTRL_REG_BASE                   0xF7EA0000
    #define        MEMMAP_CHIP_CTRL_REG_SIZE                   0x10000
    #define        MEMMAP_CHIP_CTRL_REG_DEC_BIT                0x10
    #define        MEMMAP_MPP_REG_BASE                         0xF7EB0000
    #define        MEMMAP_MPP_REG_SIZE                         0x10000
    #define        MEMMAP_MPP_REG_DEC_BIT                      0x10
    #define        MEMMAP_AVIO_SEMAPHORE_REG_BASE              0xF7EC0000
    #define        MEMMAP_AVIO_SEMAPHORE_REG_SIZE              0x10000
    #define        MEMMAP_AVIO_SEMAPHORE_DEC_BIT               0x10
    #define        MEMMAP_USB0_REG_BASE                        0xF7ED0000
    #define        MEMMAP_USB0_REG_SIZE                        0x10000
    #define        MEMMAP_USB0_REG_DEC_BIT                     0x10
    #define        MEMMAP_USB1_REG_BASE                        0xF7EE0000
    #define        MEMMAP_USB1_REG_SIZE                        0x10000
    #define        MEMMAP_USB1_REG_DEC_BIT                     0x10
    #define        MEMMAP_GFX_REG_BASE                         0xF7EF0000
    #define        MEMMAP_GFX_REG_SIZE                         0x10000
    #define        MEMMAP_GFX_REG_DEC_BIT                      0x10
    #define        MEMMAP_NAND_FLASH_REG_BASE                  0xF7F00000
    #define        MEMMAP_NAND_FLASH_REG_SIZE                  0x10000
    #define        MEMMAP_NAND_FLASH_DEC_BIT                   0x10
    #define        MEMMAP_LBC_REG_BASE                         0xF7F10000
    #define        MEMMAP_LBC_REG_SIZE                         0x10000
    #define        MEMMAP_LBC_REG_DEC_BIT                      0x10
    #define        MEMMAP_PWM_REG_BASE                         0xF7F20000
    #define        MEMMAP_PWM_REG_SIZE                         0x10000
    #define        MEMMAP_PWM_REG_DEC_BIT                      0x10
    #define        MEMMAP_VPP_REG_BASE                         0xF7F30000
    #define        MEMMAP_VPP_REG_SIZE                         0x10000
    #define        MEMMAP_VPP_REG_DEC_BIT                      0x10
    #define        MEMMAP_DMA_AVIO_REG_BASE                    0xF7F40000
    #define        MEMMAP_DMA_AVIO_REG_SIZE                    0x10000
    #define        MEMMAP_DMA_AVIO_REG_DEC_BIT                 0x10
    #define        MEMMAP_VPP_DHUB_REG_BASE                    0xF7F40000
    #define        MEMMAP_VPP_DHUB_REG_SIZE                    0x20000
    #define        MEMMAP_VPP_DHUB_REG_DEC_BIT                 0x11
    #define        MEMMAP_SM_REG_BASE                          0xF7F80000
    #define        MEMMAP_SM_REG_SIZE                          0x80000
    #define        MEMMAP_SM_REG_DEC_BIT                       0x13
    #define        MEMMAP_FAST_REG_BASE                        0xF8000000
    #define        MEMMAP_VPRO_REG_BASE                        0xF8000000
    #define        MEMMAP_VPRO_REG_SIZE                        0x400000
    #define        MEMMAP_VPRO_REG_DEC_BIT                     0x16
    #define        MEMMAP_UFAHB_REG_BASE                       0xF8400000
    #define        MEMMAP_UFAHB_REG_SIZE                       0x20000
    #define        MEMMAP_UFAHB_REG_DEC_BIT                    0x11
    #define        MEMMAP_PIC_REG_BASE                         0xF8420000
    #define        MEMMAP_PIC_REG_SIZE                         0x10000
    #define        MEMMAP_PIC_REG_DEC_BIT                      0x10
    #define        MEMMAP_PROCESSOR_ID_REG_BASE                0xF8430000
    #define        MEMMAP_PROCESSOR_ID_REG_SIZE                0x20
    #define        MEMMAP_PROCESSOR_ID_REG_DEC_BIT             0x5
    #define        MEMMAP_ONCHIP_LOW_ROM_BASE                  0xFF800000
    #define        MEMMAP_ONCHIP_LOW_ROM_SIZE                  0x10000
    #define        MEMMAP_ONCHIP_LOW_ROM_DEC_BIT               0x10
    #define        MEMMAP_HIGH_ROM_VECTOR_BASE                 0xFFFF0000
    #define        MEMMAP_HIGH_ROM_VECTOR_SIZE                 0x10000
    #define        MEMMAP_HIGH_ROM_VECTOR_DEC_BIT              0x10
    #define        MEMMAP_VECTOR_BASE                          0xFFFF0000
    #define        MEMMAP_VECTOR_SIZE                          0x20
    #define        MEMMAP_VECTOR_DEC_BIT                       0x5
    #define        MEMMAP_HIGH_EXCEPT_RESET_REG                0xFFFF0000
    #define        MEMMAP_HIGH_EXCEPT_UNDEF_REG                0xFFFF0004
    #define        MEMMAP_HIGH_EXCEPT_SOFT_REG                 0xFFFF0008
    #define        MEMMAP_HIGH_EXCEPT_PREFETCH_REG             0xFFFF000C
    #define        MEMMAP_HIGH_EXCEPT_DATA_REG                 0xFFFF0010
    #define        MEMMAP_HIGH_EXCEPT_UNUSED_REG               0xFFFF0014
    #define        MEMMAP_HIGH_EXCEPT_IRQ_REG                  0xFFFF0018
    #define        MEMMAP_HIGH_EXCEPT_FIQ_REG                  0xFFFF001C
    #define        MEMMAP_ADDR_DATA_REG_BASE                   0xFFFF0020
    #define        MEMMAP_ADDR_DATA_REG_SIZE                   0x80
    #define        MEMMAP_VECTOR_ADDR_0_REG                    0xFFFF0020
    #define        MEMMAP_VECTOR_ADDR_1_REG                    0xFFFF0024
    #define        MEMMAP_VECTOR_ADDR_2_REG                    0xFFFF0028
    #define        MEMMAP_VECTOR_ADDR_3_REG                    0xFFFF002C
    #define        MEMMAP_VECTOR_ADDR_4_REG                    0xFFFF0030
    #define        MEMMAP_VECTOR_ADDR_5_REG                    0xFFFF0034
    #define        MEMMAP_VECTOR_ADDR_6_REG                    0xFFFF0038
    #define        MEMMAP_VECTOR_ADDR_7_REG                    0xFFFF003C
    #define        MEMMAP_BRCH_ADDR_1_0_REG                    0xFFFF0040
    #define        MEMMAP_BRCH_ADDR_1_1_REG                    0xFFFF0044
    #define        MEMMAP_BRCH_ADDR_1_2_REG                    0xFFFF0048
    #define        MEMMAP_BRCH_ADDR_2_0_REG                    0xFFFF004C
    #define        MEMMAP_BRCH_ADDR_2_1_REG                    0xFFFF0050
    #define        MEMMAP_BRCH_ADDR_2_2_REG                    0xFFFF0054
    #define        MEMMAP_BRCH_ADDR_3_0_REG                    0xFFFF0058
    #define        MEMMAP_BRCH_ADDR_3_1_REG                    0xFFFF005C
    #define        MEMMAP_BRCH_ADDR_3_2_REG                    0xFFFF0060
    #define        MEMMAP_BRCH_ADDR_4_0_REG                    0xFFFF0064
    #define        MEMMAP_BRCH_ADDR_4_1_REG                    0xFFFF0068
    #define        MEMMAP_BRCH_ADDR_4_2_REG                    0xFFFF006C
    #define        MEMMAP_BRCH_ADDR_5_0_REG                    0xFFFF0070
    #define        MEMMAP_BRCH_ADDR_5_1_REG                    0xFFFF0074
    #define        MEMMAP_BRCH_ADDR_5_2_REG                    0xFFFF0078
    #define        MEMMAP_BRCH_ADDR_6_0_REG                    0xFFFF007C
    #define        MEMMAP_BRCH_ADDR_6_1_REG                    0xFFFF0080
    #define        MEMMAP_BRCH_ADDR_6_2_REG                    0xFFFF0084
    #define        MEMMAP_BRCH_ADDR_7_0_REG                    0xFFFF0088
    #define        MEMMAP_BRCH_ADDR_7_1_REG                    0xFFFF008C
    #define        MEMMAP_BRCH_ADDR_7_2_REG                    0xFFFF0090
    #define        MEMMAP_SOFTWARE_RESET_ADDR_REG              0xFFFF0094
    #define        MEMMAP_ONCHIP_HIGH_ROM_BASE                 0xFFFFC000
    #define        MEMMAP_ONCHIP_HIGH_ROM_SIZE                 0x4000
    #define        MEMMAP_ONCHIP_HIGH_ROM_DEC_BIT              0xE
    ///////////////////////////////////////////////////////////
    #define     RA_MEMMAP_memmap                               0x0000

    #define     BA_MEMMAP_memmap_dummy                         0x0000
    #define     B16MEMMAP_memmap_dummy                         0x0000
    #define   LSb32MEMMAP_memmap_dummy                            0
    #define   LSb16MEMMAP_memmap_dummy                            0
    #define       bMEMMAP_memmap_dummy                         32
    #define   MSK32MEMMAP_memmap_dummy                            0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_MEMMAP {
    ///////////////////////////////////////////////////////////
    #define   GET32MEMMAP_memmap_dummy(r32)                    _BFGET_(r32,31, 0)
    #define   SET32MEMMAP_memmap_dummy(r32,v)                  _BFSET_(r32,31, 0,v)

    #define     w32MEMMAP_memmap                               {\
            UNSG32 smemmap_dummy                               : 32;\
          }
    union { UNSG32 u32MEMMAP_memmap;
            struct w32MEMMAP_memmap;
          };
    ///////////////////////////////////////////////////////////
    } SIE_MEMMAP;

    typedef union  T32MEMMAP_memmap
          { UNSG32 u32;
            struct w32MEMMAP_memmap;
                 } T32MEMMAP_memmap;
    ///////////////////////////////////////////////////////////

    typedef union  TMEMMAP_memmap
          { UNSG32 u32[1];
            struct {
            struct w32MEMMAP_memmap;
                   };
                 } TMEMMAP_memmap;

    ///////////////////////////////////////////////////////////
     SIGN32 MEMMAP_drvrd(SIE_MEMMAP *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 MEMMAP_drvwr(SIE_MEMMAP *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void MEMMAP_reset(SIE_MEMMAP *p);
     SIGN32 MEMMAP_cmp  (SIE_MEMMAP *p, SIE_MEMMAP *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define MEMMAP_check(p,pie,pfx,hLOG) MEMMAP_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define MEMMAP_print(p,    pfx,hLOG) MEMMAP_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif

