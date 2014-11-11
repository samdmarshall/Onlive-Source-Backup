#ifndef sataWrapper_h
#define sataWrapper_h (){}


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



#ifndef h_SataRegs
#define h_SataRegs (){}

    #define     RA_SataRegs_IPPorts                            0x8000

    #define     BA_SataRegs_IPPorts_unit_mbus_id               0x8000
    #define     B16SataRegs_IPPorts_unit_mbus_id               0x8000
    #define   LSb32SataRegs_IPPorts_unit_mbus_id                  0
    #define   LSb16SataRegs_IPPorts_unit_mbus_id                  0
    #define       bSataRegs_IPPorts_unit_mbus_id               4
    #define   MSK32SataRegs_IPPorts_unit_mbus_id                  0x0000000F

    #define     BA_SataRegs_IPPorts_REFCLK_CNFG                0x8000
    #define     B16SataRegs_IPPorts_REFCLK_CNFG                0x8000
    #define   LSb32SataRegs_IPPorts_REFCLK_CNFG                   4
    #define   LSb16SataRegs_IPPorts_REFCLK_CNFG                   4
    #define       bSataRegs_IPPorts_REFCLK_CNFG                3
    #define   MSK32SataRegs_IPPorts_REFCLK_CNFG                   0x00000070

    #define     BA_SataRegs_IPPorts_SATA_GEN2_EN_0             0x8000
    #define     B16SataRegs_IPPorts_SATA_GEN2_EN_0             0x8000
    #define   LSb32SataRegs_IPPorts_SATA_GEN2_EN_0                7
    #define   LSb16SataRegs_IPPorts_SATA_GEN2_EN_0                7
    #define       bSataRegs_IPPorts_SATA_GEN2_EN_0             1
    #define   MSK32SataRegs_IPPorts_SATA_GEN2_EN_0                0x00000080

    #define     BA_SataRegs_IPPorts_SATA_COMM_EN_0             0x8001
    #define     B16SataRegs_IPPorts_SATA_COMM_EN_0             0x8000
    #define   LSb32SataRegs_IPPorts_SATA_COMM_EN_0                8
    #define   LSb16SataRegs_IPPorts_SATA_COMM_EN_0                8
    #define       bSataRegs_IPPorts_SATA_COMM_EN_0             1
    #define   MSK32SataRegs_IPPorts_SATA_COMM_EN_0                0x00000100

    #define     BA_SataRegs_IPPorts_SATA_PHY_OFF_0             0x8001
    #define     B16SataRegs_IPPorts_SATA_PHY_OFF_0             0x8000
    #define   LSb32SataRegs_IPPorts_SATA_PHY_OFF_0                9
    #define   LSb16SataRegs_IPPorts_SATA_PHY_OFF_0                9
    #define       bSataRegs_IPPorts_SATA_PHY_OFF_0             1
    #define   MSK32SataRegs_IPPorts_SATA_PHY_OFF_0                0x00000200

    #define     BA_SataRegs_IPPorts_SATA_GEN2_EN_1             0x8001
    #define     B16SataRegs_IPPorts_SATA_GEN2_EN_1             0x8000
    #define   LSb32SataRegs_IPPorts_SATA_GEN2_EN_1                10
    #define   LSb16SataRegs_IPPorts_SATA_GEN2_EN_1                10
    #define       bSataRegs_IPPorts_SATA_GEN2_EN_1             1
    #define   MSK32SataRegs_IPPorts_SATA_GEN2_EN_1                0x00000400

    #define     BA_SataRegs_IPPorts_SATA_COMM_EN_1             0x8001
    #define     B16SataRegs_IPPorts_SATA_COMM_EN_1             0x8000
    #define   LSb32SataRegs_IPPorts_SATA_COMM_EN_1                11
    #define   LSb16SataRegs_IPPorts_SATA_COMM_EN_1                11
    #define       bSataRegs_IPPorts_SATA_COMM_EN_1             1
    #define   MSK32SataRegs_IPPorts_SATA_COMM_EN_1                0x00000800

    #define     BA_SataRegs_IPPorts_SATA_PHY_OFF_1             0x8001
    #define     B16SataRegs_IPPorts_SATA_PHY_OFF_1             0x8000
    #define   LSb32SataRegs_IPPorts_SATA_PHY_OFF_1                12
    #define   LSb16SataRegs_IPPorts_SATA_PHY_OFF_1                12
    #define       bSataRegs_IPPorts_SATA_PHY_OFF_1             1
    #define   MSK32SataRegs_IPPorts_SATA_PHY_OFF_1                0x00001000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_SataRegs {
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx0                                      [32768];
    ///////////////////////////////////////////////////////////
    #define   GET32SataRegs_IPPorts_unit_mbus_id(r32)          _BFGET_(r32, 3, 0)
    #define   SET32SataRegs_IPPorts_unit_mbus_id(r32,v)        _BFSET_(r32, 3, 0,v)
    #define   GET16SataRegs_IPPorts_unit_mbus_id(r16)          _BFGET_(r16, 3, 0)
    #define   SET16SataRegs_IPPorts_unit_mbus_id(r16,v)        _BFSET_(r16, 3, 0,v)

    #define   GET32SataRegs_IPPorts_REFCLK_CNFG(r32)           _BFGET_(r32, 6, 4)
    #define   SET32SataRegs_IPPorts_REFCLK_CNFG(r32,v)         _BFSET_(r32, 6, 4,v)
    #define   GET16SataRegs_IPPorts_REFCLK_CNFG(r16)           _BFGET_(r16, 6, 4)
    #define   SET16SataRegs_IPPorts_REFCLK_CNFG(r16,v)         _BFSET_(r16, 6, 4,v)

    #define   GET32SataRegs_IPPorts_SATA_GEN2_EN_0(r32)        _BFGET_(r32, 7, 7)
    #define   SET32SataRegs_IPPorts_SATA_GEN2_EN_0(r32,v)      _BFSET_(r32, 7, 7,v)
    #define   GET16SataRegs_IPPorts_SATA_GEN2_EN_0(r16)        _BFGET_(r16, 7, 7)
    #define   SET16SataRegs_IPPorts_SATA_GEN2_EN_0(r16,v)      _BFSET_(r16, 7, 7,v)

    #define   GET32SataRegs_IPPorts_SATA_COMM_EN_0(r32)        _BFGET_(r32, 8, 8)
    #define   SET32SataRegs_IPPorts_SATA_COMM_EN_0(r32,v)      _BFSET_(r32, 8, 8,v)
    #define   GET16SataRegs_IPPorts_SATA_COMM_EN_0(r16)        _BFGET_(r16, 8, 8)
    #define   SET16SataRegs_IPPorts_SATA_COMM_EN_0(r16,v)      _BFSET_(r16, 8, 8,v)

    #define   GET32SataRegs_IPPorts_SATA_PHY_OFF_0(r32)        _BFGET_(r32, 9, 9)
    #define   SET32SataRegs_IPPorts_SATA_PHY_OFF_0(r32,v)      _BFSET_(r32, 9, 9,v)
    #define   GET16SataRegs_IPPorts_SATA_PHY_OFF_0(r16)        _BFGET_(r16, 9, 9)
    #define   SET16SataRegs_IPPorts_SATA_PHY_OFF_0(r16,v)      _BFSET_(r16, 9, 9,v)

    #define   GET32SataRegs_IPPorts_SATA_GEN2_EN_1(r32)        _BFGET_(r32,10,10)
    #define   SET32SataRegs_IPPorts_SATA_GEN2_EN_1(r32,v)      _BFSET_(r32,10,10,v)
    #define   GET16SataRegs_IPPorts_SATA_GEN2_EN_1(r16)        _BFGET_(r16,10,10)
    #define   SET16SataRegs_IPPorts_SATA_GEN2_EN_1(r16,v)      _BFSET_(r16,10,10,v)

    #define   GET32SataRegs_IPPorts_SATA_COMM_EN_1(r32)        _BFGET_(r32,11,11)
    #define   SET32SataRegs_IPPorts_SATA_COMM_EN_1(r32,v)      _BFSET_(r32,11,11,v)
    #define   GET16SataRegs_IPPorts_SATA_COMM_EN_1(r16)        _BFGET_(r16,11,11)
    #define   SET16SataRegs_IPPorts_SATA_COMM_EN_1(r16,v)      _BFSET_(r16,11,11,v)

    #define   GET32SataRegs_IPPorts_SATA_PHY_OFF_1(r32)        _BFGET_(r32,12,12)
    #define   SET32SataRegs_IPPorts_SATA_PHY_OFF_1(r32,v)      _BFSET_(r32,12,12,v)
    #define   GET16SataRegs_IPPorts_SATA_PHY_OFF_1(r16)        _BFGET_(r16,12,12)
    #define   SET16SataRegs_IPPorts_SATA_PHY_OFF_1(r16,v)      _BFSET_(r16,12,12,v)

    #define     w32SataRegs_IPPorts                            {\
            UNSG32 uIPPorts_unit_mbus_id                       :  4;\
            UNSG32 uIPPorts_REFCLK_CNFG                        :  3;\
            UNSG32 uIPPorts_SATA_GEN2_EN_0                     :  1;\
            UNSG32 uIPPorts_SATA_COMM_EN_0                     :  1;\
            UNSG32 uIPPorts_SATA_PHY_OFF_0                     :  1;\
            UNSG32 uIPPorts_SATA_GEN2_EN_1                     :  1;\
            UNSG32 uIPPorts_SATA_COMM_EN_1                     :  1;\
            UNSG32 uIPPorts_SATA_PHY_OFF_1                     :  1;\
            UNSG32 RSVDx8000_b13                               : 19;\
          }
    union { UNSG32 u32SataRegs_IPPorts;
            struct w32SataRegs_IPPorts;
          };
    ///////////////////////////////////////////////////////////
    } SIE_SataRegs;

    typedef union  T32SataRegs_IPPorts
          { UNSG32 u32;
            struct w32SataRegs_IPPorts;
                 } T32SataRegs_IPPorts;
    ///////////////////////////////////////////////////////////

    typedef union  TSataRegs_IPPorts
          { UNSG32 u32[1];
            struct {
            struct w32SataRegs_IPPorts;
                   };
                 } TSataRegs_IPPorts;

    ///////////////////////////////////////////////////////////
     SIGN32 SataRegs_drvrd(SIE_SataRegs *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 SataRegs_drvwr(SIE_SataRegs *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void SataRegs_reset(SIE_SataRegs *p);
     SIGN32 SataRegs_cmp  (SIE_SataRegs *p, SIE_SataRegs *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define SataRegs_check(p,pie,pfx,hLOG) SataRegs_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define SataRegs_print(p,    pfx,hLOG) SataRegs_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif

