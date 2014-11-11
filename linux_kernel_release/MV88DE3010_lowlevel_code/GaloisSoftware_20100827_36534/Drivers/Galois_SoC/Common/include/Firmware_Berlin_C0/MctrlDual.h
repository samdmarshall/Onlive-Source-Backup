#ifndef MctrlDual_h
#define MctrlDual_h (){}


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



#ifndef h_seSetting
#define h_seSetting (){}

    #define     RA_seSetting_setting                           0x0000

    #define     BA_seSetting_setting_startAddr3116             0x0000
    #define     B16seSetting_setting_startAddr3116             0x0000
    #define   LSb32seSetting_setting_startAddr3116                0
    #define   LSb16seSetting_setting_startAddr3116                0
    #define       bseSetting_setting_startAddr3116             16
    #define   MSK32seSetting_setting_startAddr3116                0x0000FFFF

    #define     BA_seSetting_setting_regionSize                0x0002
    #define     B16seSetting_setting_regionSize                0x0002
    #define   LSb32seSetting_setting_regionSize                   16
    #define   LSb16seSetting_setting_regionSize                   0
    #define       bseSetting_setting_regionSize                4
    #define   MSK32seSetting_setting_regionSize                   0x000F0000
    #define        seSetting_setting_regionSize_size64KB                    0x0
    #define        seSetting_setting_regionSize_size128KB                   0x1
    #define        seSetting_setting_regionSize_size256KB                   0x2
    #define        seSetting_setting_regionSize_size512KB                   0x3
    #define        seSetting_setting_regionSize_size1MB                     0x4
    #define        seSetting_setting_regionSize_size2MB                     0x5
    #define        seSetting_setting_regionSize_size4MB                     0x6
    #define        seSetting_setting_regionSize_size8MB                     0x7
    #define        seSetting_setting_regionSize_size16MB                    0x8
    #define        seSetting_setting_regionSize_size32MB                    0x9
    #define        seSetting_setting_regionSize_size64MB                    0xA
    #define        seSetting_setting_regionSize_size128MB                   0xB
    #define        seSetting_setting_regionSize_size256MB                   0xC
    #define        seSetting_setting_regionSize_size512MB                   0xD

    #define     BA_seSetting_setting_enable                    0x0002
    #define     B16seSetting_setting_enable                    0x0002
    #define   LSb32seSetting_setting_enable                       20
    #define   LSb16seSetting_setting_enable                       4
    #define       bseSetting_setting_enable                    1
    #define   MSK32seSetting_setting_enable                       0x00100000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_seSetting {
    ///////////////////////////////////////////////////////////
    #define   GET32seSetting_setting_startAddr3116(r32)        _BFGET_(r32,15, 0)
    #define   SET32seSetting_setting_startAddr3116(r32,v)      _BFSET_(r32,15, 0,v)
    #define   GET16seSetting_setting_startAddr3116(r16)        _BFGET_(r16,15, 0)
    #define   SET16seSetting_setting_startAddr3116(r16,v)      _BFSET_(r16,15, 0,v)

    #define   GET32seSetting_setting_regionSize(r32)           _BFGET_(r32,19,16)
    #define   SET32seSetting_setting_regionSize(r32,v)         _BFSET_(r32,19,16,v)
    #define   GET16seSetting_setting_regionSize(r16)           _BFGET_(r16, 3, 0)
    #define   SET16seSetting_setting_regionSize(r16,v)         _BFSET_(r16, 3, 0,v)

    #define   GET32seSetting_setting_enable(r32)               _BFGET_(r32,20,20)
    #define   SET32seSetting_setting_enable(r32,v)             _BFSET_(r32,20,20,v)
    #define   GET16seSetting_setting_enable(r16)               _BFGET_(r16, 4, 4)
    #define   SET16seSetting_setting_enable(r16,v)             _BFSET_(r16, 4, 4,v)

    #define     w32seSetting_setting                           {\
            UNSG32 usetting_startAddr3116                      : 16;\
            UNSG32 usetting_regionSize                         :  4;\
            UNSG32 usetting_enable                             :  1;\
            UNSG32 RSVDx0_b21                                  : 11;\
          }
    union { UNSG32 u32seSetting_setting;
            struct w32seSetting_setting;
          };
    ///////////////////////////////////////////////////////////
    } SIE_seSetting;

    typedef union  T32seSetting_setting
          { UNSG32 u32;
            struct w32seSetting_setting;
                 } T32seSetting_setting;
    ///////////////////////////////////////////////////////////

    typedef union  TseSetting_setting
          { UNSG32 u32[1];
            struct {
            struct w32seSetting_setting;
                   };
                 } TseSetting_setting;

    ///////////////////////////////////////////////////////////
     SIGN32 seSetting_drvrd(SIE_seSetting *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 seSetting_drvwr(SIE_seSetting *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void seSetting_reset(SIE_seSetting *p);
     SIGN32 seSetting_cmp  (SIE_seSetting *p, SIE_seSetting *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define seSetting_check(p,pie,pfx,hLOG) seSetting_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define seSetting_print(p,    pfx,hLOG) seSetting_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_seRegion
#define h_seRegion (){}

    #define     RA_seRegion_CTRL0                              0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_seRegion_CTRL1                              0x0004
    ///////////////////////////////////////////////////////////
    #define     RA_seRegion_CTRL2                              0x0008
    ///////////////////////////////////////////////////////////
    #define     RA_seRegion_CTRL3                              0x000C
    ///////////////////////////////////////////////////////////
    #define     RA_seRegion_CTRL4                              0x0010
    ///////////////////////////////////////////////////////////
    #define     RA_seRegion_CTRL5                              0x0014
    ///////////////////////////////////////////////////////////
    #define     RA_seRegion_CTRL6                              0x0018
    ///////////////////////////////////////////////////////////
    #define     RA_seRegion_CTRL7                              0x001C
    ///////////////////////////////////////////////////////////

    typedef struct SIE_seRegion {
    ///////////////////////////////////////////////////////////
              SIE_seSetting                                    ie_CTRL0;
    ///////////////////////////////////////////////////////////
              SIE_seSetting                                    ie_CTRL1;
    ///////////////////////////////////////////////////////////
              SIE_seSetting                                    ie_CTRL2;
    ///////////////////////////////////////////////////////////
              SIE_seSetting                                    ie_CTRL3;
    ///////////////////////////////////////////////////////////
              SIE_seSetting                                    ie_CTRL4;
    ///////////////////////////////////////////////////////////
              SIE_seSetting                                    ie_CTRL5;
    ///////////////////////////////////////////////////////////
              SIE_seSetting                                    ie_CTRL6;
    ///////////////////////////////////////////////////////////
              SIE_seSetting                                    ie_CTRL7;
    ///////////////////////////////////////////////////////////
    } SIE_seRegion;

    ///////////////////////////////////////////////////////////
     SIGN32 seRegion_drvrd(SIE_seRegion *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 seRegion_drvwr(SIE_seRegion *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void seRegion_reset(SIE_seRegion *p);
     SIGN32 seRegion_cmp  (SIE_seRegion *p, SIE_seRegion *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define seRegion_check(p,pie,pfx,hLOG) seRegion_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define seRegion_print(p,    pfx,hLOG) seRegion_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_ACCTX
#define h_ACCTX (){}

    #define     RA_ACCTX_CPU0WMEM                              0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_ACCTX_CPU0RMEM                              0x0020
    ///////////////////////////////////////////////////////////
    #define     RA_ACCTX_PERIFMSTWMEM                          0x0040
    ///////////////////////////////////////////////////////////
    #define     RA_ACCTX_PERIFMSTRMEM                          0x0060
    ///////////////////////////////////////////////////////////

    typedef struct SIE_ACCTX {
    ///////////////////////////////////////////////////////////
              SIE_seRegion                                     ie_CPU0WMEM;
    ///////////////////////////////////////////////////////////
              SIE_seRegion                                     ie_CPU0RMEM;
    ///////////////////////////////////////////////////////////
              SIE_seRegion                                     ie_PERIFMSTWMEM;
    ///////////////////////////////////////////////////////////
              SIE_seRegion                                     ie_PERIFMSTRMEM;
    ///////////////////////////////////////////////////////////
    } SIE_ACCTX;

    ///////////////////////////////////////////////////////////
     SIGN32 ACCTX_drvrd(SIE_ACCTX *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 ACCTX_drvwr(SIE_ACCTX *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void ACCTX_reset(SIE_ACCTX *p);
     SIGN32 ACCTX_cmp  (SIE_ACCTX *p, SIE_ACCTX *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define ACCTX_check(p,pie,pfx,hLOG) ACCTX_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define ACCTX_print(p,    pfx,hLOG) ACCTX_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_DRV_TERM
#define h_DRV_TERM (){}

    #define     RA_DRV_TERM_Ctrl                               0x0000

    #define     BA_DRV_TERM_Ctrl_WCK_DLY                       0x0000
    #define     B16DRV_TERM_Ctrl_WCK_DLY                       0x0000
    #define   LSb32DRV_TERM_Ctrl_WCK_DLY                          0
    #define   LSb16DRV_TERM_Ctrl_WCK_DLY                          0
    #define       bDRV_TERM_Ctrl_WCK_DLY                       3
    #define   MSK32DRV_TERM_Ctrl_WCK_DLY                          0x00000007

    #define     BA_DRV_TERM_Ctrl_ZPDRV                         0x0000
    #define     B16DRV_TERM_Ctrl_ZPDRV                         0x0000
    #define   LSb32DRV_TERM_Ctrl_ZPDRV                            3
    #define   LSb16DRV_TERM_Ctrl_ZPDRV                            3
    #define       bDRV_TERM_Ctrl_ZPDRV                         4
    #define   MSK32DRV_TERM_Ctrl_ZPDRV                            0x00000078

    #define     BA_DRV_TERM_Ctrl_ZNDRV                         0x0000
    #define     B16DRV_TERM_Ctrl_ZNDRV                         0x0000
    #define   LSb32DRV_TERM_Ctrl_ZNDRV                            7
    #define   LSb16DRV_TERM_Ctrl_ZNDRV                            7
    #define       bDRV_TERM_Ctrl_ZNDRV                         4
    #define   MSK32DRV_TERM_Ctrl_ZNDRV                            0x00000780

    #define     BA_DRV_TERM_Ctrl_ZPTRM                         0x0001
    #define     B16DRV_TERM_Ctrl_ZPTRM                         0x0000
    #define   LSb32DRV_TERM_Ctrl_ZPTRM                            11
    #define   LSb16DRV_TERM_Ctrl_ZPTRM                            11
    #define       bDRV_TERM_Ctrl_ZPTRM                         4
    #define   MSK32DRV_TERM_Ctrl_ZPTRM                            0x00007800

    #define     BA_DRV_TERM_Ctrl_ZNTRM                         0x0001
    #define     B16DRV_TERM_Ctrl_ZNTRM                         0x0000
    #define   LSb32DRV_TERM_Ctrl_ZNTRM                            15
    #define   LSb16DRV_TERM_Ctrl_ZNTRM                            15
    #define       bDRV_TERM_Ctrl_ZNTRM                         4
    #define   MSK32DRV_TERM_Ctrl_ZNTRM                            0x00078000

    #define     BA_DRV_TERM_Ctrl_VREF_SEL                      0x0002
    #define     B16DRV_TERM_Ctrl_VREF_SEL                      0x0002
    #define   LSb32DRV_TERM_Ctrl_VREF_SEL                         19
    #define   LSb16DRV_TERM_Ctrl_VREF_SEL                         3
    #define       bDRV_TERM_Ctrl_VREF_SEL                      2
    #define   MSK32DRV_TERM_Ctrl_VREF_SEL                         0x00180000

    #define     BA_DRV_TERM_Ctrl_ZPR                           0x0002
    #define     B16DRV_TERM_Ctrl_ZPR                           0x0002
    #define   LSb32DRV_TERM_Ctrl_ZPR                              21
    #define   LSb16DRV_TERM_Ctrl_ZPR                              5
    #define       bDRV_TERM_Ctrl_ZPR                           4
    #define   MSK32DRV_TERM_Ctrl_ZPR                              0x01E00000

    #define     BA_DRV_TERM_Ctrl_ZNR                           0x0003
    #define     B16DRV_TERM_Ctrl_ZNR                           0x0002
    #define   LSb32DRV_TERM_Ctrl_ZNR                              25
    #define   LSb16DRV_TERM_Ctrl_ZNR                              9
    #define       bDRV_TERM_Ctrl_ZNR                           4
    #define   MSK32DRV_TERM_Ctrl_ZNR                              0x1E000000

    #define     BA_DRV_TERM_Ctrl_ZD                            0x0003
    #define     B16DRV_TERM_Ctrl_ZD                            0x0002
    #define   LSb32DRV_TERM_Ctrl_ZD                               29
    #define   LSb16DRV_TERM_Ctrl_ZD                               13
    #define       bDRV_TERM_Ctrl_ZD                            1
    #define   MSK32DRV_TERM_Ctrl_ZD                               0x20000000

    #define     BA_DRV_TERM_Ctrl_MODE                          0x0003
    #define     B16DRV_TERM_Ctrl_MODE                          0x0002
    #define   LSb32DRV_TERM_Ctrl_MODE                             30
    #define   LSb16DRV_TERM_Ctrl_MODE                             14
    #define       bDRV_TERM_Ctrl_MODE                          1
    #define   MSK32DRV_TERM_Ctrl_MODE                             0x40000000

    #define     BA_DRV_TERM_Ctrl_PD                            0x0003
    #define     B16DRV_TERM_Ctrl_PD                            0x0002
    #define   LSb32DRV_TERM_Ctrl_PD                               31
    #define   LSb16DRV_TERM_Ctrl_PD                               15
    #define       bDRV_TERM_Ctrl_PD                            1
    #define   MSK32DRV_TERM_Ctrl_PD                               0x80000000
    ///////////////////////////////////////////////////////////
    #define     RA_DRV_TERM_rcvCtrl                            0x0004

    #define     BA_DRV_TERM_rcvCtrl_RCVTYPE                    0x0004
    #define     B16DRV_TERM_rcvCtrl_RCVTYPE                    0x0004
    #define   LSb32DRV_TERM_rcvCtrl_RCVTYPE                       0
    #define   LSb16DRV_TERM_rcvCtrl_RCVTYPE                       0
    #define       bDRV_TERM_rcvCtrl_RCVTYPE                    1
    #define   MSK32DRV_TERM_rcvCtrl_RCVTYPE                       0x00000001

    #define     BA_DRV_TERM_rcvCtrl_RCVEN                      0x0004
    #define     B16DRV_TERM_rcvCtrl_RCVEN                      0x0004
    #define   LSb32DRV_TERM_rcvCtrl_RCVEN                         1
    #define   LSb16DRV_TERM_rcvCtrl_RCVEN                         1
    #define       bDRV_TERM_rcvCtrl_RCVEN                      3
    #define   MSK32DRV_TERM_rcvCtrl_RCVEN                         0x0000000E

    #define     BA_DRV_TERM_rcvCtrl_RCVEP                      0x0004
    #define     B16DRV_TERM_rcvCtrl_RCVEP                      0x0004
    #define   LSb32DRV_TERM_rcvCtrl_RCVEP                         4
    #define   LSb16DRV_TERM_rcvCtrl_RCVEP                         4
    #define       bDRV_TERM_rcvCtrl_RCVEP                      3
    #define   MSK32DRV_TERM_rcvCtrl_RCVEP                         0x00000070
    ///////////////////////////////////////////////////////////

    typedef struct SIE_DRV_TERM {
    ///////////////////////////////////////////////////////////
    #define   GET32DRV_TERM_Ctrl_WCK_DLY(r32)                  _BFGET_(r32, 2, 0)
    #define   SET32DRV_TERM_Ctrl_WCK_DLY(r32,v)                _BFSET_(r32, 2, 0,v)
    #define   GET16DRV_TERM_Ctrl_WCK_DLY(r16)                  _BFGET_(r16, 2, 0)
    #define   SET16DRV_TERM_Ctrl_WCK_DLY(r16,v)                _BFSET_(r16, 2, 0,v)

    #define   GET32DRV_TERM_Ctrl_ZPDRV(r32)                    _BFGET_(r32, 6, 3)
    #define   SET32DRV_TERM_Ctrl_ZPDRV(r32,v)                  _BFSET_(r32, 6, 3,v)
    #define   GET16DRV_TERM_Ctrl_ZPDRV(r16)                    _BFGET_(r16, 6, 3)
    #define   SET16DRV_TERM_Ctrl_ZPDRV(r16,v)                  _BFSET_(r16, 6, 3,v)

    #define   GET32DRV_TERM_Ctrl_ZNDRV(r32)                    _BFGET_(r32,10, 7)
    #define   SET32DRV_TERM_Ctrl_ZNDRV(r32,v)                  _BFSET_(r32,10, 7,v)
    #define   GET16DRV_TERM_Ctrl_ZNDRV(r16)                    _BFGET_(r16,10, 7)
    #define   SET16DRV_TERM_Ctrl_ZNDRV(r16,v)                  _BFSET_(r16,10, 7,v)

    #define   GET32DRV_TERM_Ctrl_ZPTRM(r32)                    _BFGET_(r32,14,11)
    #define   SET32DRV_TERM_Ctrl_ZPTRM(r32,v)                  _BFSET_(r32,14,11,v)
    #define   GET16DRV_TERM_Ctrl_ZPTRM(r16)                    _BFGET_(r16,14,11)
    #define   SET16DRV_TERM_Ctrl_ZPTRM(r16,v)                  _BFSET_(r16,14,11,v)

    #define   GET32DRV_TERM_Ctrl_ZNTRM(r32)                    _BFGET_(r32,18,15)
    #define   SET32DRV_TERM_Ctrl_ZNTRM(r32,v)                  _BFSET_(r32,18,15,v)

    #define   GET32DRV_TERM_Ctrl_VREF_SEL(r32)                 _BFGET_(r32,20,19)
    #define   SET32DRV_TERM_Ctrl_VREF_SEL(r32,v)               _BFSET_(r32,20,19,v)
    #define   GET16DRV_TERM_Ctrl_VREF_SEL(r16)                 _BFGET_(r16, 4, 3)
    #define   SET16DRV_TERM_Ctrl_VREF_SEL(r16,v)               _BFSET_(r16, 4, 3,v)

    #define   GET32DRV_TERM_Ctrl_ZPR(r32)                      _BFGET_(r32,24,21)
    #define   SET32DRV_TERM_Ctrl_ZPR(r32,v)                    _BFSET_(r32,24,21,v)
    #define   GET16DRV_TERM_Ctrl_ZPR(r16)                      _BFGET_(r16, 8, 5)
    #define   SET16DRV_TERM_Ctrl_ZPR(r16,v)                    _BFSET_(r16, 8, 5,v)

    #define   GET32DRV_TERM_Ctrl_ZNR(r32)                      _BFGET_(r32,28,25)
    #define   SET32DRV_TERM_Ctrl_ZNR(r32,v)                    _BFSET_(r32,28,25,v)
    #define   GET16DRV_TERM_Ctrl_ZNR(r16)                      _BFGET_(r16,12, 9)
    #define   SET16DRV_TERM_Ctrl_ZNR(r16,v)                    _BFSET_(r16,12, 9,v)

    #define   GET32DRV_TERM_Ctrl_ZD(r32)                       _BFGET_(r32,29,29)
    #define   SET32DRV_TERM_Ctrl_ZD(r32,v)                     _BFSET_(r32,29,29,v)
    #define   GET16DRV_TERM_Ctrl_ZD(r16)                       _BFGET_(r16,13,13)
    #define   SET16DRV_TERM_Ctrl_ZD(r16,v)                     _BFSET_(r16,13,13,v)

    #define   GET32DRV_TERM_Ctrl_MODE(r32)                     _BFGET_(r32,30,30)
    #define   SET32DRV_TERM_Ctrl_MODE(r32,v)                   _BFSET_(r32,30,30,v)
    #define   GET16DRV_TERM_Ctrl_MODE(r16)                     _BFGET_(r16,14,14)
    #define   SET16DRV_TERM_Ctrl_MODE(r16,v)                   _BFSET_(r16,14,14,v)

    #define   GET32DRV_TERM_Ctrl_PD(r32)                       _BFGET_(r32,31,31)
    #define   SET32DRV_TERM_Ctrl_PD(r32,v)                     _BFSET_(r32,31,31,v)
    #define   GET16DRV_TERM_Ctrl_PD(r16)                       _BFGET_(r16,15,15)
    #define   SET16DRV_TERM_Ctrl_PD(r16,v)                     _BFSET_(r16,15,15,v)

    #define     w32DRV_TERM_Ctrl                               {\
            UNSG32 uCtrl_WCK_DLY                               :  3;\
            UNSG32 uCtrl_ZPDRV                                 :  4;\
            UNSG32 uCtrl_ZNDRV                                 :  4;\
            UNSG32 uCtrl_ZPTRM                                 :  4;\
            UNSG32 uCtrl_ZNTRM                                 :  4;\
            UNSG32 uCtrl_VREF_SEL                              :  2;\
            UNSG32 uCtrl_ZPR                                   :  4;\
            UNSG32 uCtrl_ZNR                                   :  4;\
            UNSG32 uCtrl_ZD                                    :  1;\
            UNSG32 uCtrl_MODE                                  :  1;\
            UNSG32 uCtrl_PD                                    :  1;\
          }
    union { UNSG32 u32DRV_TERM_Ctrl;
            struct w32DRV_TERM_Ctrl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32DRV_TERM_rcvCtrl_RCVTYPE(r32)               _BFGET_(r32, 0, 0)
    #define   SET32DRV_TERM_rcvCtrl_RCVTYPE(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16DRV_TERM_rcvCtrl_RCVTYPE(r16)               _BFGET_(r16, 0, 0)
    #define   SET16DRV_TERM_rcvCtrl_RCVTYPE(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32DRV_TERM_rcvCtrl_RCVEN(r32)                 _BFGET_(r32, 3, 1)
    #define   SET32DRV_TERM_rcvCtrl_RCVEN(r32,v)               _BFSET_(r32, 3, 1,v)
    #define   GET16DRV_TERM_rcvCtrl_RCVEN(r16)                 _BFGET_(r16, 3, 1)
    #define   SET16DRV_TERM_rcvCtrl_RCVEN(r16,v)               _BFSET_(r16, 3, 1,v)

    #define   GET32DRV_TERM_rcvCtrl_RCVEP(r32)                 _BFGET_(r32, 6, 4)
    #define   SET32DRV_TERM_rcvCtrl_RCVEP(r32,v)               _BFSET_(r32, 6, 4,v)
    #define   GET16DRV_TERM_rcvCtrl_RCVEP(r16)                 _BFGET_(r16, 6, 4)
    #define   SET16DRV_TERM_rcvCtrl_RCVEP(r16,v)               _BFSET_(r16, 6, 4,v)

    #define     w32DRV_TERM_rcvCtrl                            {\
            UNSG32 urcvCtrl_RCVTYPE                            :  1;\
            UNSG32 urcvCtrl_RCVEN                              :  3;\
            UNSG32 urcvCtrl_RCVEP                              :  3;\
            UNSG32 RSVDx4_b7                                   : 25;\
          }
    union { UNSG32 u32DRV_TERM_rcvCtrl;
            struct w32DRV_TERM_rcvCtrl;
          };
    ///////////////////////////////////////////////////////////
    } SIE_DRV_TERM;

    typedef union  T32DRV_TERM_Ctrl
          { UNSG32 u32;
            struct w32DRV_TERM_Ctrl;
                 } T32DRV_TERM_Ctrl;
    typedef union  T32DRV_TERM_rcvCtrl
          { UNSG32 u32;
            struct w32DRV_TERM_rcvCtrl;
                 } T32DRV_TERM_rcvCtrl;
    ///////////////////////////////////////////////////////////

    typedef union  TDRV_TERM_Ctrl
          { UNSG32 u32[1];
            struct {
            struct w32DRV_TERM_Ctrl;
                   };
                 } TDRV_TERM_Ctrl;
    typedef union  TDRV_TERM_rcvCtrl
          { UNSG32 u32[1];
            struct {
            struct w32DRV_TERM_rcvCtrl;
                   };
                 } TDRV_TERM_rcvCtrl;

    ///////////////////////////////////////////////////////////
     SIGN32 DRV_TERM_drvrd(SIE_DRV_TERM *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 DRV_TERM_drvwr(SIE_DRV_TERM *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void DRV_TERM_reset(SIE_DRV_TERM *p);
     SIGN32 DRV_TERM_cmp  (SIE_DRV_TERM *p, SIE_DRV_TERM *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define DRV_TERM_check(p,pie,pfx,hLOG) DRV_TERM_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define DRV_TERM_print(p,    pfx,hLOG) DRV_TERM_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_DLL_DELAY
#define h_DLL_DELAY (){}

    #define     BA_DLL_DELAY_DELAY_TEST_VAL                    0x0000
    #define     B16DLL_DELAY_DELAY_TEST_VAL                    0x0000
    #define   LSb32DLL_DELAY_DELAY_TEST_VAL                       0
    #define   LSb16DLL_DELAY_DELAY_TEST_VAL                       0
    #define       bDLL_DELAY_DELAY_TEST_VAL                    9
    #define   MSK32DLL_DELAY_DELAY_TEST_VAL                       0x000001FF

    #define     BA_DLL_DELAY_PHASE_SEL_VAL                     0x0001
    #define     B16DLL_DELAY_PHASE_SEL_VAL                     0x0000
    #define   LSb32DLL_DELAY_PHASE_SEL_VAL                        9
    #define   LSb16DLL_DELAY_PHASE_SEL_VAL                        9
    #define       bDLL_DELAY_PHASE_SEL_VAL                     5
    #define   MSK32DLL_DELAY_PHASE_SEL_VAL                        0x00003E00
    ///////////////////////////////////////////////////////////

    typedef struct SIE_DLL_DELAY {
    ///////////////////////////////////////////////////////////
    #define   GET32DLL_DELAY_DELAY_TEST_VAL(r32)               _BFGET_(r32, 8, 0)
    #define   SET32DLL_DELAY_DELAY_TEST_VAL(r32,v)             _BFSET_(r32, 8, 0,v)
    #define   GET16DLL_DELAY_DELAY_TEST_VAL(r16)               _BFGET_(r16, 8, 0)
    #define   SET16DLL_DELAY_DELAY_TEST_VAL(r16,v)             _BFSET_(r16, 8, 0,v)

    #define   GET32DLL_DELAY_PHASE_SEL_VAL(r32)                _BFGET_(r32,13, 9)
    #define   SET32DLL_DELAY_PHASE_SEL_VAL(r32,v)              _BFSET_(r32,13, 9,v)
    #define   GET16DLL_DELAY_PHASE_SEL_VAL(r16)                _BFGET_(r16,13, 9)
    #define   SET16DLL_DELAY_PHASE_SEL_VAL(r16,v)              _BFSET_(r16,13, 9,v)

            UNSG32 u_DELAY_TEST_VAL                            :  9;
            UNSG32 u_PHASE_SEL_VAL                             :  5;
            UNSG32 RSVDx0_b14                                  : 18;
    ///////////////////////////////////////////////////////////
    } SIE_DLL_DELAY;

    ///////////////////////////////////////////////////////////
     SIGN32 DLL_DELAY_drvrd(SIE_DLL_DELAY *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 DLL_DELAY_drvwr(SIE_DLL_DELAY *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void DLL_DELAY_reset(SIE_DLL_DELAY *p);
     SIGN32 DLL_DELAY_cmp  (SIE_DLL_DELAY *p, SIE_DLL_DELAY *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define DLL_DELAY_check(p,pie,pfx,hLOG) DLL_DELAY_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define DLL_DELAY_print(p,    pfx,hLOG) DLL_DELAY_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_PHY
#define h_PHY (){}

    #define     RA_PHY_Timing                                  0x0000

    #define     BA_PHY_Timing_rlat                             0x0000
    #define     B16PHY_Timing_rlat                             0x0000
    #define   LSb32PHY_Timing_rlat                                0
    #define   LSb16PHY_Timing_rlat                                0
    #define       bPHY_Timing_rlat                             3
    #define   MSK32PHY_Timing_rlat                                0x00000007

    #define     BA_PHY_Timing_en2vld                           0x0000
    #define     B16PHY_Timing_en2vld                           0x0000
    #define   LSb32PHY_Timing_en2vld                              3
    #define   LSb16PHY_Timing_en2vld                              3
    #define       bPHY_Timing_en2vld                           3
    #define   MSK32PHY_Timing_en2vld                              0x00000038

    #define     BA_PHY_Timing_full_preamble                    0x0000
    #define     B16PHY_Timing_full_preamble                    0x0000
    #define   LSb32PHY_Timing_full_preamble                       6
    #define   LSb16PHY_Timing_full_preamble                       6
    #define       bPHY_Timing_full_preamble                    1
    #define   MSK32PHY_Timing_full_preamble                       0x00000040
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_Enable                                  0x0004

    #define     BA_PHY_Enable_OEAC                             0x0004
    #define     B16PHY_Enable_OEAC                             0x0004
    #define   LSb32PHY_Enable_OEAC                                0
    #define   LSb16PHY_Enable_OEAC                                0
    #define       bPHY_Enable_OEAC                             1
    #define   MSK32PHY_Enable_OEAC                                0x00000001

    #define     BA_PHY_Enable_OECK                             0x0004
    #define     B16PHY_Enable_OECK                             0x0004
    #define   LSb32PHY_Enable_OECK                                1
    #define   LSb16PHY_Enable_OECK                                1
    #define       bPHY_Enable_OECK                             1
    #define   MSK32PHY_Enable_OECK                                0x00000002

    #define     BA_PHY_Enable_OERST                            0x0004
    #define     B16PHY_Enable_OERST                            0x0004
    #define   LSb32PHY_Enable_OERST                               2
    #define   LSb16PHY_Enable_OERST                               2
    #define       bPHY_Enable_OERST                            1
    #define   MSK32PHY_Enable_OERST                               0x00000004

    #define     BA_PHY_Enable_AC_RECEN                         0x0004
    #define     B16PHY_Enable_AC_RECEN                         0x0004
    #define   LSb32PHY_Enable_AC_RECEN                            3
    #define   LSb16PHY_Enable_AC_RECEN                            3
    #define       bPHY_Enable_AC_RECEN                         1
    #define   MSK32PHY_Enable_AC_RECEN                            0x00000008

    #define     BA_PHY_Enable_CK_RECEN                         0x0004
    #define     B16PHY_Enable_CK_RECEN                         0x0004
    #define   LSb32PHY_Enable_CK_RECEN                            4
    #define   LSb16PHY_Enable_CK_RECEN                            4
    #define       bPHY_Enable_CK_RECEN                         1
    #define   MSK32PHY_Enable_CK_RECEN                            0x00000010

    #define     BA_PHY_Enable_RST_RECEN                        0x0004
    #define     B16PHY_Enable_RST_RECEN                        0x0004
    #define   LSb32PHY_Enable_RST_RECEN                           5
    #define   LSb16PHY_Enable_RST_RECEN                           5
    #define       bPHY_Enable_RST_RECEN                        1
    #define   MSK32PHY_Enable_RST_RECEN                           0x00000020

    #define     BA_PHY_Enable_RST_Output                       0x0004
    #define     B16PHY_Enable_RST_Output                       0x0004
    #define   LSb32PHY_Enable_RST_Output                          6
    #define   LSb16PHY_Enable_RST_Output                          6
    #define       bPHY_Enable_RST_Output                       1
    #define   MSK32PHY_Enable_RST_Output                          0x00000040
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_DLL                                     0x0008

    #define     BA_PHY_DLL_RESET                               0x0008
    #define     B16PHY_DLL_RESET                               0x0008
    #define   LSb32PHY_DLL_RESET                                  0
    #define   LSb16PHY_DLL_RESET                                  0
    #define       bPHY_DLL_RESET                               1
    #define   MSK32PHY_DLL_RESET                                  0x00000001

    #define     BA_PHY_DLL_BYPASS_EN                           0x0008
    #define     B16PHY_DLL_BYPASS_EN                           0x0008
    #define   LSb32PHY_DLL_BYPASS_EN                              1
    #define   LSb16PHY_DLL_BYPASS_EN                              1
    #define       bPHY_DLL_BYPASS_EN                           1
    #define   MSK32PHY_DLL_BYPASS_EN                              0x00000002

    #define     BA_PHY_DLL_AUTO_UPDATE_EN                      0x0008
    #define     B16PHY_DLL_AUTO_UPDATE_EN                      0x0008
    #define   LSb32PHY_DLL_AUTO_UPDATE_EN                         2
    #define   LSb16PHY_DLL_AUTO_UPDATE_EN                         2
    #define       bPHY_DLL_AUTO_UPDATE_EN                      1
    #define   MSK32PHY_DLL_AUTO_UPDATE_EN                         0x00000004

    #define     BA_PHY_DLL_UPDATE_EN                           0x0008
    #define     B16PHY_DLL_UPDATE_EN                           0x0008
    #define   LSb32PHY_DLL_UPDATE_EN                              3
    #define   LSb16PHY_DLL_UPDATE_EN                              3
    #define       bPHY_DLL_UPDATE_EN                           1
    #define   MSK32PHY_DLL_UPDATE_EN                              0x00000008

    #define     BA_PHY_DLL_UPDATE_MASK                         0x0008
    #define     B16PHY_DLL_UPDATE_MASK                         0x0008
    #define   LSb32PHY_DLL_UPDATE_MASK                            4
    #define   LSb16PHY_DLL_UPDATE_MASK                            4
    #define       bPHY_DLL_UPDATE_MASK                         8
    #define   MSK32PHY_DLL_UPDATE_MASK                            0x00000FF0

    #define     BA_PHY_DLL_DELAY_TEST                          0x0009
    #define     B16PHY_DLL_DELAY_TEST                          0x0008
    #define   LSb32PHY_DLL_DELAY_TEST                             12
    #define   LSb16PHY_DLL_DELAY_TEST                             12
    #define       bPHY_DLL_DELAY_TEST                          9
    #define   MSK32PHY_DLL_DELAY_TEST                             0x001FF000

    #define     BA_PHY_DLL_PHSEL                               0x000A
    #define     B16PHY_DLL_PHSEL                               0x000A
    #define   LSb32PHY_DLL_PHSEL                                  21
    #define   LSb16PHY_DLL_PHSEL                                  5
    #define       bPHY_DLL_PHSEL                               5
    #define   MSK32PHY_DLL_PHSEL                                  0x03E00000

    #define     BA_PHY_DLL_TEST_EN                             0x000B
    #define     B16PHY_DLL_TEST_EN                             0x000A
    #define   LSb32PHY_DLL_TEST_EN                                26
    #define   LSb16PHY_DLL_TEST_EN                                10
    #define       bPHY_DLL_TEST_EN                             1
    #define   MSK32PHY_DLL_TEST_EN                                0x04000000
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_DELAY_VAL0                              0x000C
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_DELAY_VAL1                              0x0010
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_DELAY_VAL2                              0x0014
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_DELAY_VAL3                              0x0018
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_DELAY_VAL4                              0x001C
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_DELAY_VAL5                              0x0020
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_DELAY_VAL6                              0x0024
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_DELAY_VAL7                              0x0028
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_PLL1                                    0x002C

    #define     BA_PHY_PLL1_RESET                              0x002C
    #define     B16PHY_PLL1_RESET                              0x002C
    #define   LSb32PHY_PLL1_RESET                                 0
    #define   LSb16PHY_PLL1_RESET                                 0
    #define       bPHY_PLL1_RESET                              1
    #define   MSK32PHY_PLL1_RESET                                 0x00000001

    #define     BA_PHY_PLL1_SYNC_TYPE                          0x002C
    #define     B16PHY_PLL1_SYNC_TYPE                          0x002C
    #define   LSb32PHY_PLL1_SYNC_TYPE                             1
    #define   LSb16PHY_PLL1_SYNC_TYPE                             1
    #define       bPHY_PLL1_SYNC_TYPE                          1
    #define   MSK32PHY_PLL1_SYNC_TYPE                             0x00000002

    #define     BA_PHY_PLL1_FBDIV                              0x002C
    #define     B16PHY_PLL1_FBDIV                              0x002C
    #define   LSb32PHY_PLL1_FBDIV                                 2
    #define   LSb16PHY_PLL1_FBDIV                                 2
    #define       bPHY_PLL1_FBDIV                              9
    #define   MSK32PHY_PLL1_FBDIV                                 0x000007FC

    #define     BA_PHY_PLL1_FBDLY                              0x002D
    #define     B16PHY_PLL1_FBDLY                              0x002C
    #define   LSb32PHY_PLL1_FBDLY                                 11
    #define   LSb16PHY_PLL1_FBDLY                                 11
    #define       bPHY_PLL1_FBDLY                              4
    #define   MSK32PHY_PLL1_FBDLY                                 0x00007800

    #define     BA_PHY_PLL1_ICP                                0x002D
    #define     B16PHY_PLL1_ICP                                0x002C
    #define   LSb32PHY_PLL1_ICP                                   15
    #define   LSb16PHY_PLL1_ICP                                   15
    #define       bPHY_PLL1_ICP                                3
    #define   MSK32PHY_PLL1_ICP                                   0x00038000

    #define     BA_PHY_PLL1_KVCO                               0x002E
    #define     B16PHY_PLL1_KVCO                               0x002E
    #define   LSb32PHY_PLL1_KVCO                                  18
    #define   LSb16PHY_PLL1_KVCO                                  2
    #define       bPHY_PLL1_KVCO                               3
    #define   MSK32PHY_PLL1_KVCO                                  0x001C0000

    #define     BA_PHY_PLL1_PU_PLL                             0x002E
    #define     B16PHY_PLL1_PU_PLL                             0x002E
    #define   LSb32PHY_PLL1_PU_PLL                                21
    #define   LSb16PHY_PLL1_PU_PLL                                5
    #define       bPHY_PLL1_PU_PLL                             1
    #define   MSK32PHY_PLL1_PU_PLL                                0x00200000
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_PLL2                                    0x0030

    #define     BA_PHY_PLL2_REFDIV                             0x0030
    #define     B16PHY_PLL2_REFDIV                             0x0030
    #define   LSb32PHY_PLL2_REFDIV                                0
    #define   LSb16PHY_PLL2_REFDIV                                0
    #define       bPHY_PLL2_REFDIV                             9
    #define   MSK32PHY_PLL2_REFDIV                                0x000001FF

    #define     BA_PHY_PLL2_REFDLY                             0x0031
    #define     B16PHY_PLL2_REFDLY                             0x0030
    #define   LSb32PHY_PLL2_REFDLY                                9
    #define   LSb16PHY_PLL2_REFDLY                                9
    #define       bPHY_PLL2_REFDLY                             4
    #define   MSK32PHY_PLL2_REFDLY                                0x00001E00

    #define     BA_PHY_PLL2_RESERVE                            0x0031
    #define     B16PHY_PLL2_RESERVE                            0x0030
    #define   LSb32PHY_PLL2_RESERVE                               13
    #define   LSb16PHY_PLL2_RESERVE                               13
    #define       bPHY_PLL2_RESERVE                            2
    #define   MSK32PHY_PLL2_RESERVE                               0x00006000

    #define     BA_PHY_PLL2_SEL                                0x0031
    #define     B16PHY_PLL2_SEL                                0x0030
    #define   LSb32PHY_PLL2_SEL                                   15
    #define   LSb16PHY_PLL2_SEL                                   15
    #define       bPHY_PLL2_SEL                                2
    #define   MSK32PHY_PLL2_SEL                                   0x00018000

    #define     BA_PHY_PLL2_TEST_MON                           0x0032
    #define     B16PHY_PLL2_TEST_MON                           0x0032
    #define   LSb32PHY_PLL2_TEST_MON                              17
    #define   LSb16PHY_PLL2_TEST_MON                              1
    #define       bPHY_PLL2_TEST_MON                           4
    #define   MSK32PHY_PLL2_TEST_MON                              0x001E0000

    #define     BA_PHY_PLL2_VCO_VRNG                           0x0032
    #define     B16PHY_PLL2_VCO_VRNG                           0x0032
    #define   LSb32PHY_PLL2_VCO_VRNG                              21
    #define   LSb16PHY_PLL2_VCO_VRNG                              5
    #define       bPHY_PLL2_VCO_VRNG                           2
    #define   MSK32PHY_PLL2_VCO_VRNG                              0x00600000

    #define     BA_PHY_PLL2_VCOFBST                            0x0032
    #define     B16PHY_PLL2_VCOFBST                            0x0032
    #define   LSb32PHY_PLL2_VCOFBST                               23
    #define   LSb16PHY_PLL2_VCOFBST                               7
    #define       bPHY_PLL2_VCOFBST                            1
    #define   MSK32PHY_PLL2_VCOFBST                               0x00800000

    #define     BA_PHY_PLL2_VDDL                               0x0033
    #define     B16PHY_PLL2_VDDL                               0x0032
    #define   LSb32PHY_PLL2_VDDL                                  24
    #define   LSb16PHY_PLL2_VDDL                                  8
    #define       bPHY_PLL2_VDDL                               2
    #define   MSK32PHY_PLL2_VDDL                                  0x03000000

    #define     BA_PHY_PLL2_VDDM                               0x0033
    #define     B16PHY_PLL2_VDDM                               0x0032
    #define   LSb32PHY_PLL2_VDDM                                  26
    #define   LSb16PHY_PLL2_VDDM                                  10
    #define       bPHY_PLL2_VDDM                               2
    #define   MSK32PHY_PLL2_VDDM                                  0x0C000000
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_STS                                     0x0034

    #define     BA_PHY_STS_CAL_DONE                            0x0034
    #define     B16PHY_STS_CAL_DONE                            0x0034
    #define   LSb32PHY_STS_CAL_DONE                               0
    #define   LSb16PHY_STS_CAL_DONE                               0
    #define       bPHY_STS_CAL_DONE                            1
    #define   MSK32PHY_STS_CAL_DONE                               0x00000001

    #define     BA_PHY_STS_CAL_ZPR                             0x0034
    #define     B16PHY_STS_CAL_ZPR                             0x0034
    #define   LSb32PHY_STS_CAL_ZPR                                1
    #define   LSb16PHY_STS_CAL_ZPR                                1
    #define       bPHY_STS_CAL_ZPR                             4
    #define   MSK32PHY_STS_CAL_ZPR                                0x0000001E

    #define     BA_PHY_STS_CAL_ZNR                             0x0034
    #define     B16PHY_STS_CAL_ZNR                             0x0034
    #define   LSb32PHY_STS_CAL_ZNR                                5
    #define   LSb16PHY_STS_CAL_ZNR                                5
    #define       bPHY_STS_CAL_ZNR                             4
    #define   MSK32PHY_STS_CAL_ZNR                                0x000001E0

    #define     BA_PHY_STS_PLL0_LOCK                           0x0035
    #define     B16PHY_STS_PLL0_LOCK                           0x0034
    #define   LSb32PHY_STS_PLL0_LOCK                              9
    #define   LSb16PHY_STS_PLL0_LOCK                              9
    #define       bPHY_STS_PLL0_LOCK                           1
    #define   MSK32PHY_STS_PLL0_LOCK                              0x00000200

    #define     BA_PHY_STS_PLL1_LOCK                           0x0035
    #define     B16PHY_STS_PLL1_LOCK                           0x0034
    #define   LSb32PHY_STS_PLL1_LOCK                              10
    #define   LSb16PHY_STS_PLL1_LOCK                              10
    #define       bPHY_STS_PLL1_LOCK                           1
    #define   MSK32PHY_STS_PLL1_LOCK                              0x00000400

    #define     BA_PHY_STS_DLL0_DELAY_OUT                      0x0035
    #define     B16PHY_STS_DLL0_DELAY_OUT                      0x0034
    #define   LSb32PHY_STS_DLL0_DELAY_OUT                         11
    #define   LSb16PHY_STS_DLL0_DELAY_OUT                         11
    #define       bPHY_STS_DLL0_DELAY_OUT                      8
    #define   MSK32PHY_STS_DLL0_DELAY_OUT                         0x0007F800

    #define     BA_PHY_STS_DLL1_DELAY_OUT                      0x0036
    #define     B16PHY_STS_DLL1_DELAY_OUT                      0x0036
    #define   LSb32PHY_STS_DLL1_DELAY_OUT                         19
    #define   LSb16PHY_STS_DLL1_DELAY_OUT                         3
    #define       bPHY_STS_DLL1_DELAY_OUT                      8
    #define   MSK32PHY_STS_DLL1_DELAY_OUT                         0x07F80000
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_CH0_DLL_STS                             0x0038

    #define     BA_PHY_CH0_DLL_STS_DLL0_DELAY_OUT              0x0038
    #define     B16PHY_CH0_DLL_STS_DLL0_DELAY_OUT              0x0038
    #define   LSb32PHY_CH0_DLL_STS_DLL0_DELAY_OUT                 0
    #define   LSb16PHY_CH0_DLL_STS_DLL0_DELAY_OUT                 0
    #define       bPHY_CH0_DLL_STS_DLL0_DELAY_OUT              8
    #define   MSK32PHY_CH0_DLL_STS_DLL0_DELAY_OUT                 0x000000FF

    #define     BA_PHY_CH0_DLL_STS_DLL1_DELAY_OUT              0x0039
    #define     B16PHY_CH0_DLL_STS_DLL1_DELAY_OUT              0x0038
    #define   LSb32PHY_CH0_DLL_STS_DLL1_DELAY_OUT                 8
    #define   LSb16PHY_CH0_DLL_STS_DLL1_DELAY_OUT                 8
    #define       bPHY_CH0_DLL_STS_DLL1_DELAY_OUT              8
    #define   MSK32PHY_CH0_DLL_STS_DLL1_DELAY_OUT                 0x0000FF00

    #define     BA_PHY_CH0_DLL_STS_DLL2_DELAY_OUT              0x003A
    #define     B16PHY_CH0_DLL_STS_DLL2_DELAY_OUT              0x003A
    #define   LSb32PHY_CH0_DLL_STS_DLL2_DELAY_OUT                 16
    #define   LSb16PHY_CH0_DLL_STS_DLL2_DELAY_OUT                 0
    #define       bPHY_CH0_DLL_STS_DLL2_DELAY_OUT              8
    #define   MSK32PHY_CH0_DLL_STS_DLL2_DELAY_OUT                 0x00FF0000

    #define     BA_PHY_CH0_DLL_STS_DLL3_DELAY_OUT              0x003B
    #define     B16PHY_CH0_DLL_STS_DLL3_DELAY_OUT              0x003A
    #define   LSb32PHY_CH0_DLL_STS_DLL3_DELAY_OUT                 24
    #define   LSb16PHY_CH0_DLL_STS_DLL3_DELAY_OUT                 8
    #define       bPHY_CH0_DLL_STS_DLL3_DELAY_OUT              8
    #define   MSK32PHY_CH0_DLL_STS_DLL3_DELAY_OUT                 0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_CH1_DLL_STS                             0x003C

    #define     BA_PHY_CH1_DLL_STS_DLL4_DELAY_OUT              0x003C
    #define     B16PHY_CH1_DLL_STS_DLL4_DELAY_OUT              0x003C
    #define   LSb32PHY_CH1_DLL_STS_DLL4_DELAY_OUT                 0
    #define   LSb16PHY_CH1_DLL_STS_DLL4_DELAY_OUT                 0
    #define       bPHY_CH1_DLL_STS_DLL4_DELAY_OUT              8
    #define   MSK32PHY_CH1_DLL_STS_DLL4_DELAY_OUT                 0x000000FF

    #define     BA_PHY_CH1_DLL_STS_DLL5_DELAY_OUT              0x003D
    #define     B16PHY_CH1_DLL_STS_DLL5_DELAY_OUT              0x003C
    #define   LSb32PHY_CH1_DLL_STS_DLL5_DELAY_OUT                 8
    #define   LSb16PHY_CH1_DLL_STS_DLL5_DELAY_OUT                 8
    #define       bPHY_CH1_DLL_STS_DLL5_DELAY_OUT              8
    #define   MSK32PHY_CH1_DLL_STS_DLL5_DELAY_OUT                 0x0000FF00

    #define     BA_PHY_CH1_DLL_STS_DLL6_DELAY_OUT              0x003E
    #define     B16PHY_CH1_DLL_STS_DLL6_DELAY_OUT              0x003E
    #define   LSb32PHY_CH1_DLL_STS_DLL6_DELAY_OUT                 16
    #define   LSb16PHY_CH1_DLL_STS_DLL6_DELAY_OUT                 0
    #define       bPHY_CH1_DLL_STS_DLL6_DELAY_OUT              8
    #define   MSK32PHY_CH1_DLL_STS_DLL6_DELAY_OUT                 0x00FF0000

    #define     BA_PHY_CH1_DLL_STS_DLL7_DELAY_OUT              0x003F
    #define     B16PHY_CH1_DLL_STS_DLL7_DELAY_OUT              0x003E
    #define   LSb32PHY_CH1_DLL_STS_DLL7_DELAY_OUT                 24
    #define   LSb16PHY_CH1_DLL_STS_DLL7_DELAY_OUT                 8
    #define       bPHY_CH1_DLL_STS_DLL7_DELAY_OUT              8
    #define   MSK32PHY_CH1_DLL_STS_DLL7_DELAY_OUT                 0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_DQ                                      0x0040
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_DQS                                     0x0048
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_ADCM                                    0x0050
    ///////////////////////////////////////////////////////////
    #define     RA_PHY_CK                                      0x0058
    ///////////////////////////////////////////////////////////

    typedef struct SIE_PHY {
    ///////////////////////////////////////////////////////////
    #define   GET32PHY_Timing_rlat(r32)                        _BFGET_(r32, 2, 0)
    #define   SET32PHY_Timing_rlat(r32,v)                      _BFSET_(r32, 2, 0,v)
    #define   GET16PHY_Timing_rlat(r16)                        _BFGET_(r16, 2, 0)
    #define   SET16PHY_Timing_rlat(r16,v)                      _BFSET_(r16, 2, 0,v)

    #define   GET32PHY_Timing_en2vld(r32)                      _BFGET_(r32, 5, 3)
    #define   SET32PHY_Timing_en2vld(r32,v)                    _BFSET_(r32, 5, 3,v)
    #define   GET16PHY_Timing_en2vld(r16)                      _BFGET_(r16, 5, 3)
    #define   SET16PHY_Timing_en2vld(r16,v)                    _BFSET_(r16, 5, 3,v)

    #define   GET32PHY_Timing_full_preamble(r32)               _BFGET_(r32, 6, 6)
    #define   SET32PHY_Timing_full_preamble(r32,v)             _BFSET_(r32, 6, 6,v)
    #define   GET16PHY_Timing_full_preamble(r16)               _BFGET_(r16, 6, 6)
    #define   SET16PHY_Timing_full_preamble(r16,v)             _BFSET_(r16, 6, 6,v)

    #define     w32PHY_Timing                                  {\
            UNSG32 uTiming_rlat                                :  3;\
            UNSG32 uTiming_en2vld                              :  3;\
            UNSG32 uTiming_full_preamble                       :  1;\
            UNSG32 RSVDx0_b7                                   : 25;\
          }
    union { UNSG32 u32PHY_Timing;
            struct w32PHY_Timing;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PHY_Enable_OEAC(r32)                        _BFGET_(r32, 0, 0)
    #define   SET32PHY_Enable_OEAC(r32,v)                      _BFSET_(r32, 0, 0,v)
    #define   GET16PHY_Enable_OEAC(r16)                        _BFGET_(r16, 0, 0)
    #define   SET16PHY_Enable_OEAC(r16,v)                      _BFSET_(r16, 0, 0,v)

    #define   GET32PHY_Enable_OECK(r32)                        _BFGET_(r32, 1, 1)
    #define   SET32PHY_Enable_OECK(r32,v)                      _BFSET_(r32, 1, 1,v)
    #define   GET16PHY_Enable_OECK(r16)                        _BFGET_(r16, 1, 1)
    #define   SET16PHY_Enable_OECK(r16,v)                      _BFSET_(r16, 1, 1,v)

    #define   GET32PHY_Enable_OERST(r32)                       _BFGET_(r32, 2, 2)
    #define   SET32PHY_Enable_OERST(r32,v)                     _BFSET_(r32, 2, 2,v)
    #define   GET16PHY_Enable_OERST(r16)                       _BFGET_(r16, 2, 2)
    #define   SET16PHY_Enable_OERST(r16,v)                     _BFSET_(r16, 2, 2,v)

    #define   GET32PHY_Enable_AC_RECEN(r32)                    _BFGET_(r32, 3, 3)
    #define   SET32PHY_Enable_AC_RECEN(r32,v)                  _BFSET_(r32, 3, 3,v)
    #define   GET16PHY_Enable_AC_RECEN(r16)                    _BFGET_(r16, 3, 3)
    #define   SET16PHY_Enable_AC_RECEN(r16,v)                  _BFSET_(r16, 3, 3,v)

    #define   GET32PHY_Enable_CK_RECEN(r32)                    _BFGET_(r32, 4, 4)
    #define   SET32PHY_Enable_CK_RECEN(r32,v)                  _BFSET_(r32, 4, 4,v)
    #define   GET16PHY_Enable_CK_RECEN(r16)                    _BFGET_(r16, 4, 4)
    #define   SET16PHY_Enable_CK_RECEN(r16,v)                  _BFSET_(r16, 4, 4,v)

    #define   GET32PHY_Enable_RST_RECEN(r32)                   _BFGET_(r32, 5, 5)
    #define   SET32PHY_Enable_RST_RECEN(r32,v)                 _BFSET_(r32, 5, 5,v)
    #define   GET16PHY_Enable_RST_RECEN(r16)                   _BFGET_(r16, 5, 5)
    #define   SET16PHY_Enable_RST_RECEN(r16,v)                 _BFSET_(r16, 5, 5,v)

    #define   GET32PHY_Enable_RST_Output(r32)                  _BFGET_(r32, 6, 6)
    #define   SET32PHY_Enable_RST_Output(r32,v)                _BFSET_(r32, 6, 6,v)
    #define   GET16PHY_Enable_RST_Output(r16)                  _BFGET_(r16, 6, 6)
    #define   SET16PHY_Enable_RST_Output(r16,v)                _BFSET_(r16, 6, 6,v)

    #define     w32PHY_Enable                                  {\
            UNSG32 uEnable_OEAC                                :  1;\
            UNSG32 uEnable_OECK                                :  1;\
            UNSG32 uEnable_OERST                               :  1;\
            UNSG32 uEnable_AC_RECEN                            :  1;\
            UNSG32 uEnable_CK_RECEN                            :  1;\
            UNSG32 uEnable_RST_RECEN                           :  1;\
            UNSG32 uEnable_RST_Output                          :  1;\
            UNSG32 RSVDx4_b7                                   : 25;\
          }
    union { UNSG32 u32PHY_Enable;
            struct w32PHY_Enable;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PHY_DLL_RESET(r32)                          _BFGET_(r32, 0, 0)
    #define   SET32PHY_DLL_RESET(r32,v)                        _BFSET_(r32, 0, 0,v)
    #define   GET16PHY_DLL_RESET(r16)                          _BFGET_(r16, 0, 0)
    #define   SET16PHY_DLL_RESET(r16,v)                        _BFSET_(r16, 0, 0,v)

    #define   GET32PHY_DLL_BYPASS_EN(r32)                      _BFGET_(r32, 1, 1)
    #define   SET32PHY_DLL_BYPASS_EN(r32,v)                    _BFSET_(r32, 1, 1,v)
    #define   GET16PHY_DLL_BYPASS_EN(r16)                      _BFGET_(r16, 1, 1)
    #define   SET16PHY_DLL_BYPASS_EN(r16,v)                    _BFSET_(r16, 1, 1,v)

    #define   GET32PHY_DLL_AUTO_UPDATE_EN(r32)                 _BFGET_(r32, 2, 2)
    #define   SET32PHY_DLL_AUTO_UPDATE_EN(r32,v)               _BFSET_(r32, 2, 2,v)
    #define   GET16PHY_DLL_AUTO_UPDATE_EN(r16)                 _BFGET_(r16, 2, 2)
    #define   SET16PHY_DLL_AUTO_UPDATE_EN(r16,v)               _BFSET_(r16, 2, 2,v)

    #define   GET32PHY_DLL_UPDATE_EN(r32)                      _BFGET_(r32, 3, 3)
    #define   SET32PHY_DLL_UPDATE_EN(r32,v)                    _BFSET_(r32, 3, 3,v)
    #define   GET16PHY_DLL_UPDATE_EN(r16)                      _BFGET_(r16, 3, 3)
    #define   SET16PHY_DLL_UPDATE_EN(r16,v)                    _BFSET_(r16, 3, 3,v)

    #define   GET32PHY_DLL_UPDATE_MASK(r32)                    _BFGET_(r32,11, 4)
    #define   SET32PHY_DLL_UPDATE_MASK(r32,v)                  _BFSET_(r32,11, 4,v)
    #define   GET16PHY_DLL_UPDATE_MASK(r16)                    _BFGET_(r16,11, 4)
    #define   SET16PHY_DLL_UPDATE_MASK(r16,v)                  _BFSET_(r16,11, 4,v)

    #define   GET32PHY_DLL_DELAY_TEST(r32)                     _BFGET_(r32,20,12)
    #define   SET32PHY_DLL_DELAY_TEST(r32,v)                   _BFSET_(r32,20,12,v)

    #define   GET32PHY_DLL_PHSEL(r32)                          _BFGET_(r32,25,21)
    #define   SET32PHY_DLL_PHSEL(r32,v)                        _BFSET_(r32,25,21,v)
    #define   GET16PHY_DLL_PHSEL(r16)                          _BFGET_(r16, 9, 5)
    #define   SET16PHY_DLL_PHSEL(r16,v)                        _BFSET_(r16, 9, 5,v)

    #define   GET32PHY_DLL_TEST_EN(r32)                        _BFGET_(r32,26,26)
    #define   SET32PHY_DLL_TEST_EN(r32,v)                      _BFSET_(r32,26,26,v)
    #define   GET16PHY_DLL_TEST_EN(r16)                        _BFGET_(r16,10,10)
    #define   SET16PHY_DLL_TEST_EN(r16,v)                      _BFSET_(r16,10,10,v)

    #define     w32PHY_DLL                                     {\
            UNSG32 uDLL_RESET                                  :  1;\
            UNSG32 uDLL_BYPASS_EN                              :  1;\
            UNSG32 uDLL_AUTO_UPDATE_EN                         :  1;\
            UNSG32 uDLL_UPDATE_EN                              :  1;\
            UNSG32 uDLL_UPDATE_MASK                            :  8;\
            UNSG32 uDLL_DELAY_TEST                             :  9;\
            UNSG32 uDLL_PHSEL                                  :  5;\
            UNSG32 uDLL_TEST_EN                                :  1;\
            UNSG32 RSVDx8_b27                                  :  5;\
          }
    union { UNSG32 u32PHY_DLL;
            struct w32PHY_DLL;
          };
    ///////////////////////////////////////////////////////////
              SIE_DLL_DELAY                                    ie_DELAY_VAL0;
    ///////////////////////////////////////////////////////////
              SIE_DLL_DELAY                                    ie_DELAY_VAL1;
    ///////////////////////////////////////////////////////////
              SIE_DLL_DELAY                                    ie_DELAY_VAL2;
    ///////////////////////////////////////////////////////////
              SIE_DLL_DELAY                                    ie_DELAY_VAL3;
    ///////////////////////////////////////////////////////////
              SIE_DLL_DELAY                                    ie_DELAY_VAL4;
    ///////////////////////////////////////////////////////////
              SIE_DLL_DELAY                                    ie_DELAY_VAL5;
    ///////////////////////////////////////////////////////////
              SIE_DLL_DELAY                                    ie_DELAY_VAL6;
    ///////////////////////////////////////////////////////////
              SIE_DLL_DELAY                                    ie_DELAY_VAL7;
    ///////////////////////////////////////////////////////////
    #define   GET32PHY_PLL1_RESET(r32)                         _BFGET_(r32, 0, 0)
    #define   SET32PHY_PLL1_RESET(r32,v)                       _BFSET_(r32, 0, 0,v)
    #define   GET16PHY_PLL1_RESET(r16)                         _BFGET_(r16, 0, 0)
    #define   SET16PHY_PLL1_RESET(r16,v)                       _BFSET_(r16, 0, 0,v)

    #define   GET32PHY_PLL1_SYNC_TYPE(r32)                     _BFGET_(r32, 1, 1)
    #define   SET32PHY_PLL1_SYNC_TYPE(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   GET16PHY_PLL1_SYNC_TYPE(r16)                     _BFGET_(r16, 1, 1)
    #define   SET16PHY_PLL1_SYNC_TYPE(r16,v)                   _BFSET_(r16, 1, 1,v)

    #define   GET32PHY_PLL1_FBDIV(r32)                         _BFGET_(r32,10, 2)
    #define   SET32PHY_PLL1_FBDIV(r32,v)                       _BFSET_(r32,10, 2,v)
    #define   GET16PHY_PLL1_FBDIV(r16)                         _BFGET_(r16,10, 2)
    #define   SET16PHY_PLL1_FBDIV(r16,v)                       _BFSET_(r16,10, 2,v)

    #define   GET32PHY_PLL1_FBDLY(r32)                         _BFGET_(r32,14,11)
    #define   SET32PHY_PLL1_FBDLY(r32,v)                       _BFSET_(r32,14,11,v)
    #define   GET16PHY_PLL1_FBDLY(r16)                         _BFGET_(r16,14,11)
    #define   SET16PHY_PLL1_FBDLY(r16,v)                       _BFSET_(r16,14,11,v)

    #define   GET32PHY_PLL1_ICP(r32)                           _BFGET_(r32,17,15)
    #define   SET32PHY_PLL1_ICP(r32,v)                         _BFSET_(r32,17,15,v)

    #define   GET32PHY_PLL1_KVCO(r32)                          _BFGET_(r32,20,18)
    #define   SET32PHY_PLL1_KVCO(r32,v)                        _BFSET_(r32,20,18,v)
    #define   GET16PHY_PLL1_KVCO(r16)                          _BFGET_(r16, 4, 2)
    #define   SET16PHY_PLL1_KVCO(r16,v)                        _BFSET_(r16, 4, 2,v)

    #define   GET32PHY_PLL1_PU_PLL(r32)                        _BFGET_(r32,21,21)
    #define   SET32PHY_PLL1_PU_PLL(r32,v)                      _BFSET_(r32,21,21,v)
    #define   GET16PHY_PLL1_PU_PLL(r16)                        _BFGET_(r16, 5, 5)
    #define   SET16PHY_PLL1_PU_PLL(r16,v)                      _BFSET_(r16, 5, 5,v)

    #define     w32PHY_PLL1                                    {\
            UNSG32 uPLL1_RESET                                 :  1;\
            UNSG32 uPLL1_SYNC_TYPE                             :  1;\
            UNSG32 uPLL1_FBDIV                                 :  9;\
            UNSG32 uPLL1_FBDLY                                 :  4;\
            UNSG32 uPLL1_ICP                                   :  3;\
            UNSG32 uPLL1_KVCO                                  :  3;\
            UNSG32 uPLL1_PU_PLL                                :  1;\
            UNSG32 RSVDx2C_b22                                 : 10;\
          }
    union { UNSG32 u32PHY_PLL1;
            struct w32PHY_PLL1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PHY_PLL2_REFDIV(r32)                        _BFGET_(r32, 8, 0)
    #define   SET32PHY_PLL2_REFDIV(r32,v)                      _BFSET_(r32, 8, 0,v)
    #define   GET16PHY_PLL2_REFDIV(r16)                        _BFGET_(r16, 8, 0)
    #define   SET16PHY_PLL2_REFDIV(r16,v)                      _BFSET_(r16, 8, 0,v)

    #define   GET32PHY_PLL2_REFDLY(r32)                        _BFGET_(r32,12, 9)
    #define   SET32PHY_PLL2_REFDLY(r32,v)                      _BFSET_(r32,12, 9,v)
    #define   GET16PHY_PLL2_REFDLY(r16)                        _BFGET_(r16,12, 9)
    #define   SET16PHY_PLL2_REFDLY(r16,v)                      _BFSET_(r16,12, 9,v)

    #define   GET32PHY_PLL2_RESERVE(r32)                       _BFGET_(r32,14,13)
    #define   SET32PHY_PLL2_RESERVE(r32,v)                     _BFSET_(r32,14,13,v)
    #define   GET16PHY_PLL2_RESERVE(r16)                       _BFGET_(r16,14,13)
    #define   SET16PHY_PLL2_RESERVE(r16,v)                     _BFSET_(r16,14,13,v)

    #define   GET32PHY_PLL2_SEL(r32)                           _BFGET_(r32,16,15)
    #define   SET32PHY_PLL2_SEL(r32,v)                         _BFSET_(r32,16,15,v)

    #define   GET32PHY_PLL2_TEST_MON(r32)                      _BFGET_(r32,20,17)
    #define   SET32PHY_PLL2_TEST_MON(r32,v)                    _BFSET_(r32,20,17,v)
    #define   GET16PHY_PLL2_TEST_MON(r16)                      _BFGET_(r16, 4, 1)
    #define   SET16PHY_PLL2_TEST_MON(r16,v)                    _BFSET_(r16, 4, 1,v)

    #define   GET32PHY_PLL2_VCO_VRNG(r32)                      _BFGET_(r32,22,21)
    #define   SET32PHY_PLL2_VCO_VRNG(r32,v)                    _BFSET_(r32,22,21,v)
    #define   GET16PHY_PLL2_VCO_VRNG(r16)                      _BFGET_(r16, 6, 5)
    #define   SET16PHY_PLL2_VCO_VRNG(r16,v)                    _BFSET_(r16, 6, 5,v)

    #define   GET32PHY_PLL2_VCOFBST(r32)                       _BFGET_(r32,23,23)
    #define   SET32PHY_PLL2_VCOFBST(r32,v)                     _BFSET_(r32,23,23,v)
    #define   GET16PHY_PLL2_VCOFBST(r16)                       _BFGET_(r16, 7, 7)
    #define   SET16PHY_PLL2_VCOFBST(r16,v)                     _BFSET_(r16, 7, 7,v)

    #define   GET32PHY_PLL2_VDDL(r32)                          _BFGET_(r32,25,24)
    #define   SET32PHY_PLL2_VDDL(r32,v)                        _BFSET_(r32,25,24,v)
    #define   GET16PHY_PLL2_VDDL(r16)                          _BFGET_(r16, 9, 8)
    #define   SET16PHY_PLL2_VDDL(r16,v)                        _BFSET_(r16, 9, 8,v)

    #define   GET32PHY_PLL2_VDDM(r32)                          _BFGET_(r32,27,26)
    #define   SET32PHY_PLL2_VDDM(r32,v)                        _BFSET_(r32,27,26,v)
    #define   GET16PHY_PLL2_VDDM(r16)                          _BFGET_(r16,11,10)
    #define   SET16PHY_PLL2_VDDM(r16,v)                        _BFSET_(r16,11,10,v)

    #define     w32PHY_PLL2                                    {\
            UNSG32 uPLL2_REFDIV                                :  9;\
            UNSG32 uPLL2_REFDLY                                :  4;\
            UNSG32 uPLL2_RESERVE                               :  2;\
            UNSG32 uPLL2_SEL                                   :  2;\
            UNSG32 uPLL2_TEST_MON                              :  4;\
            UNSG32 uPLL2_VCO_VRNG                              :  2;\
            UNSG32 uPLL2_VCOFBST                               :  1;\
            UNSG32 uPLL2_VDDL                                  :  2;\
            UNSG32 uPLL2_VDDM                                  :  2;\
            UNSG32 RSVDx30_b28                                 :  4;\
          }
    union { UNSG32 u32PHY_PLL2;
            struct w32PHY_PLL2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PHY_STS_CAL_DONE(r32)                       _BFGET_(r32, 0, 0)
    #define   SET32PHY_STS_CAL_DONE(r32,v)                     _BFSET_(r32, 0, 0,v)
    #define   GET16PHY_STS_CAL_DONE(r16)                       _BFGET_(r16, 0, 0)
    #define   SET16PHY_STS_CAL_DONE(r16,v)                     _BFSET_(r16, 0, 0,v)

    #define   GET32PHY_STS_CAL_ZPR(r32)                        _BFGET_(r32, 4, 1)
    #define   SET32PHY_STS_CAL_ZPR(r32,v)                      _BFSET_(r32, 4, 1,v)
    #define   GET16PHY_STS_CAL_ZPR(r16)                        _BFGET_(r16, 4, 1)
    #define   SET16PHY_STS_CAL_ZPR(r16,v)                      _BFSET_(r16, 4, 1,v)

    #define   GET32PHY_STS_CAL_ZNR(r32)                        _BFGET_(r32, 8, 5)
    #define   SET32PHY_STS_CAL_ZNR(r32,v)                      _BFSET_(r32, 8, 5,v)
    #define   GET16PHY_STS_CAL_ZNR(r16)                        _BFGET_(r16, 8, 5)
    #define   SET16PHY_STS_CAL_ZNR(r16,v)                      _BFSET_(r16, 8, 5,v)

    #define   GET32PHY_STS_PLL0_LOCK(r32)                      _BFGET_(r32, 9, 9)
    #define   SET32PHY_STS_PLL0_LOCK(r32,v)                    _BFSET_(r32, 9, 9,v)
    #define   GET16PHY_STS_PLL0_LOCK(r16)                      _BFGET_(r16, 9, 9)
    #define   SET16PHY_STS_PLL0_LOCK(r16,v)                    _BFSET_(r16, 9, 9,v)

    #define   GET32PHY_STS_PLL1_LOCK(r32)                      _BFGET_(r32,10,10)
    #define   SET32PHY_STS_PLL1_LOCK(r32,v)                    _BFSET_(r32,10,10,v)
    #define   GET16PHY_STS_PLL1_LOCK(r16)                      _BFGET_(r16,10,10)
    #define   SET16PHY_STS_PLL1_LOCK(r16,v)                    _BFSET_(r16,10,10,v)

    #define   GET32PHY_STS_DLL0_DELAY_OUT(r32)                 _BFGET_(r32,18,11)
    #define   SET32PHY_STS_DLL0_DELAY_OUT(r32,v)               _BFSET_(r32,18,11,v)

    #define   GET32PHY_STS_DLL1_DELAY_OUT(r32)                 _BFGET_(r32,26,19)
    #define   SET32PHY_STS_DLL1_DELAY_OUT(r32,v)               _BFSET_(r32,26,19,v)
    #define   GET16PHY_STS_DLL1_DELAY_OUT(r16)                 _BFGET_(r16,10, 3)
    #define   SET16PHY_STS_DLL1_DELAY_OUT(r16,v)               _BFSET_(r16,10, 3,v)

    #define     w32PHY_STS                                     {\
            UNSG32 uSTS_CAL_DONE                               :  1;\
            UNSG32 uSTS_CAL_ZPR                                :  4;\
            UNSG32 uSTS_CAL_ZNR                                :  4;\
            UNSG32 uSTS_PLL0_LOCK                              :  1;\
            UNSG32 uSTS_PLL1_LOCK                              :  1;\
            UNSG32 uSTS_DLL0_DELAY_OUT                         :  8;\
            UNSG32 uSTS_DLL1_DELAY_OUT                         :  8;\
            UNSG32 RSVDx34_b27                                 :  5;\
          }
    union { UNSG32 u32PHY_STS;
            struct w32PHY_STS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PHY_CH0_DLL_STS_DLL0_DELAY_OUT(r32)         _BFGET_(r32, 7, 0)
    #define   SET32PHY_CH0_DLL_STS_DLL0_DELAY_OUT(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16PHY_CH0_DLL_STS_DLL0_DELAY_OUT(r16)         _BFGET_(r16, 7, 0)
    #define   SET16PHY_CH0_DLL_STS_DLL0_DELAY_OUT(r16,v)       _BFSET_(r16, 7, 0,v)

    #define   GET32PHY_CH0_DLL_STS_DLL1_DELAY_OUT(r32)         _BFGET_(r32,15, 8)
    #define   SET32PHY_CH0_DLL_STS_DLL1_DELAY_OUT(r32,v)       _BFSET_(r32,15, 8,v)
    #define   GET16PHY_CH0_DLL_STS_DLL1_DELAY_OUT(r16)         _BFGET_(r16,15, 8)
    #define   SET16PHY_CH0_DLL_STS_DLL1_DELAY_OUT(r16,v)       _BFSET_(r16,15, 8,v)

    #define   GET32PHY_CH0_DLL_STS_DLL2_DELAY_OUT(r32)         _BFGET_(r32,23,16)
    #define   SET32PHY_CH0_DLL_STS_DLL2_DELAY_OUT(r32,v)       _BFSET_(r32,23,16,v)
    #define   GET16PHY_CH0_DLL_STS_DLL2_DELAY_OUT(r16)         _BFGET_(r16, 7, 0)
    #define   SET16PHY_CH0_DLL_STS_DLL2_DELAY_OUT(r16,v)       _BFSET_(r16, 7, 0,v)

    #define   GET32PHY_CH0_DLL_STS_DLL3_DELAY_OUT(r32)         _BFGET_(r32,31,24)
    #define   SET32PHY_CH0_DLL_STS_DLL3_DELAY_OUT(r32,v)       _BFSET_(r32,31,24,v)
    #define   GET16PHY_CH0_DLL_STS_DLL3_DELAY_OUT(r16)         _BFGET_(r16,15, 8)
    #define   SET16PHY_CH0_DLL_STS_DLL3_DELAY_OUT(r16,v)       _BFSET_(r16,15, 8,v)

    #define     w32PHY_CH0_DLL_STS                             {\
            UNSG32 uCH0_DLL_STS_DLL0_DELAY_OUT                 :  8;\
            UNSG32 uCH0_DLL_STS_DLL1_DELAY_OUT                 :  8;\
            UNSG32 uCH0_DLL_STS_DLL2_DELAY_OUT                 :  8;\
            UNSG32 uCH0_DLL_STS_DLL3_DELAY_OUT                 :  8;\
          }
    union { UNSG32 u32PHY_CH0_DLL_STS;
            struct w32PHY_CH0_DLL_STS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PHY_CH1_DLL_STS_DLL4_DELAY_OUT(r32)         _BFGET_(r32, 7, 0)
    #define   SET32PHY_CH1_DLL_STS_DLL4_DELAY_OUT(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16PHY_CH1_DLL_STS_DLL4_DELAY_OUT(r16)         _BFGET_(r16, 7, 0)
    #define   SET16PHY_CH1_DLL_STS_DLL4_DELAY_OUT(r16,v)       _BFSET_(r16, 7, 0,v)

    #define   GET32PHY_CH1_DLL_STS_DLL5_DELAY_OUT(r32)         _BFGET_(r32,15, 8)
    #define   SET32PHY_CH1_DLL_STS_DLL5_DELAY_OUT(r32,v)       _BFSET_(r32,15, 8,v)
    #define   GET16PHY_CH1_DLL_STS_DLL5_DELAY_OUT(r16)         _BFGET_(r16,15, 8)
    #define   SET16PHY_CH1_DLL_STS_DLL5_DELAY_OUT(r16,v)       _BFSET_(r16,15, 8,v)

    #define   GET32PHY_CH1_DLL_STS_DLL6_DELAY_OUT(r32)         _BFGET_(r32,23,16)
    #define   SET32PHY_CH1_DLL_STS_DLL6_DELAY_OUT(r32,v)       _BFSET_(r32,23,16,v)
    #define   GET16PHY_CH1_DLL_STS_DLL6_DELAY_OUT(r16)         _BFGET_(r16, 7, 0)
    #define   SET16PHY_CH1_DLL_STS_DLL6_DELAY_OUT(r16,v)       _BFSET_(r16, 7, 0,v)

    #define   GET32PHY_CH1_DLL_STS_DLL7_DELAY_OUT(r32)         _BFGET_(r32,31,24)
    #define   SET32PHY_CH1_DLL_STS_DLL7_DELAY_OUT(r32,v)       _BFSET_(r32,31,24,v)
    #define   GET16PHY_CH1_DLL_STS_DLL7_DELAY_OUT(r16)         _BFGET_(r16,15, 8)
    #define   SET16PHY_CH1_DLL_STS_DLL7_DELAY_OUT(r16,v)       _BFSET_(r16,15, 8,v)

    #define     w32PHY_CH1_DLL_STS                             {\
            UNSG32 uCH1_DLL_STS_DLL4_DELAY_OUT                 :  8;\
            UNSG32 uCH1_DLL_STS_DLL5_DELAY_OUT                 :  8;\
            UNSG32 uCH1_DLL_STS_DLL6_DELAY_OUT                 :  8;\
            UNSG32 uCH1_DLL_STS_DLL7_DELAY_OUT                 :  8;\
          }
    union { UNSG32 u32PHY_CH1_DLL_STS;
            struct w32PHY_CH1_DLL_STS;
          };
    ///////////////////////////////////////////////////////////
              SIE_DRV_TERM                                     ie_DQ;
    ///////////////////////////////////////////////////////////
              SIE_DRV_TERM                                     ie_DQS;
    ///////////////////////////////////////////////////////////
              SIE_DRV_TERM                                     ie_ADCM;
    ///////////////////////////////////////////////////////////
              SIE_DRV_TERM                                     ie_CK;
    ///////////////////////////////////////////////////////////
    } SIE_PHY;

    typedef union  T32PHY_Timing
          { UNSG32 u32;
            struct w32PHY_Timing;
                 } T32PHY_Timing;
    typedef union  T32PHY_Enable
          { UNSG32 u32;
            struct w32PHY_Enable;
                 } T32PHY_Enable;
    typedef union  T32PHY_DLL
          { UNSG32 u32;
            struct w32PHY_DLL;
                 } T32PHY_DLL;
    typedef union  T32PHY_PLL1
          { UNSG32 u32;
            struct w32PHY_PLL1;
                 } T32PHY_PLL1;
    typedef union  T32PHY_PLL2
          { UNSG32 u32;
            struct w32PHY_PLL2;
                 } T32PHY_PLL2;
    typedef union  T32PHY_STS
          { UNSG32 u32;
            struct w32PHY_STS;
                 } T32PHY_STS;
    typedef union  T32PHY_CH0_DLL_STS
          { UNSG32 u32;
            struct w32PHY_CH0_DLL_STS;
                 } T32PHY_CH0_DLL_STS;
    typedef union  T32PHY_CH1_DLL_STS
          { UNSG32 u32;
            struct w32PHY_CH1_DLL_STS;
                 } T32PHY_CH1_DLL_STS;
    ///////////////////////////////////////////////////////////

    typedef union  TPHY_Timing
          { UNSG32 u32[1];
            struct {
            struct w32PHY_Timing;
                   };
                 } TPHY_Timing;
    typedef union  TPHY_Enable
          { UNSG32 u32[1];
            struct {
            struct w32PHY_Enable;
                   };
                 } TPHY_Enable;
    typedef union  TPHY_DLL
          { UNSG32 u32[1];
            struct {
            struct w32PHY_DLL;
                   };
                 } TPHY_DLL;
    typedef union  TPHY_PLL1
          { UNSG32 u32[1];
            struct {
            struct w32PHY_PLL1;
                   };
                 } TPHY_PLL1;
    typedef union  TPHY_PLL2
          { UNSG32 u32[1];
            struct {
            struct w32PHY_PLL2;
                   };
                 } TPHY_PLL2;
    typedef union  TPHY_STS
          { UNSG32 u32[1];
            struct {
            struct w32PHY_STS;
                   };
                 } TPHY_STS;
    typedef union  TPHY_CH0_DLL_STS
          { UNSG32 u32[1];
            struct {
            struct w32PHY_CH0_DLL_STS;
                   };
                 } TPHY_CH0_DLL_STS;
    typedef union  TPHY_CH1_DLL_STS
          { UNSG32 u32[1];
            struct {
            struct w32PHY_CH1_DLL_STS;
                   };
                 } TPHY_CH1_DLL_STS;

    ///////////////////////////////////////////////////////////
     SIGN32 PHY_drvrd(SIE_PHY *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 PHY_drvwr(SIE_PHY *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void PHY_reset(SIE_PHY *p);
     SIGN32 PHY_cmp  (SIE_PHY *p, SIE_PHY *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define PHY_check(p,pie,pfx,hLOG) PHY_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define PHY_print(p,    pfx,hLOG) PHY_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_TTBPort
#define h_TTBPort (){}

    #define     RA_TTBPort_Setup                               0x0000

    #define     BA_TTBPort_Setup_enable                        0x0000
    #define     B16TTBPort_Setup_enable                        0x0000
    #define   LSb32TTBPort_Setup_enable                           0
    #define   LSb16TTBPort_Setup_enable                           0
    #define       bTTBPort_Setup_enable                        1
    #define   MSK32TTBPort_Setup_enable                           0x00000001

    #define     BA_TTBPort_Setup_interrupt                     0x0000
    #define     B16TTBPort_Setup_interrupt                     0x0000
    #define   LSb32TTBPort_Setup_interrupt                        1
    #define   LSb16TTBPort_Setup_interrupt                        1
    #define       bTTBPort_Setup_interrupt                     1
    #define   MSK32TTBPort_Setup_interrupt                        0x00000002

    #define     BA_TTBPort_Setup_recordInfoSel                 0x0000
    #define     B16TTBPort_Setup_recordInfoSel                 0x0000
    #define   LSb32TTBPort_Setup_recordInfoSel                    2
    #define   LSb16TTBPort_Setup_recordInfoSel                    2
    #define       bTTBPort_Setup_recordInfoSel                 2
    #define   MSK32TTBPort_Setup_recordInfoSel                    0x0000000C

    #define     BA_TTBPort_Setup_no_cycle_afterward            0x0000
    #define     B16TTBPort_Setup_no_cycle_afterward            0x0000
    #define   LSb32TTBPort_Setup_no_cycle_afterward               4
    #define   LSb16TTBPort_Setup_no_cycle_afterward               4
    #define       bTTBPort_Setup_no_cycle_afterward            16
    #define   MSK32TTBPort_Setup_no_cycle_afterward               0x000FFFF0

    #define     BA_TTBPort_Setup_RESERVE_MODE                  0x0002
    #define     B16TTBPort_Setup_RESERVE_MODE                  0x0002
    #define   LSb32TTBPort_Setup_RESERVE_MODE                     20
    #define   LSb16TTBPort_Setup_RESERVE_MODE                     4
    #define       bTTBPort_Setup_RESERVE_MODE                  12
    #define   MSK32TTBPort_Setup_RESERVE_MODE                     0xFFF00000
    ///////////////////////////////////////////////////////////
    #define     RA_TTBPort_MonAddr                             0x0004

    #define     BA_TTBPort_MonAddr_RESERVE_MADD                0x0004
    #define     B16TTBPort_MonAddr_RESERVE_MADD                0x0004
    #define   LSb32TTBPort_MonAddr_RESERVE_MADD                   0
    #define   LSb16TTBPort_MonAddr_RESERVE_MADD                   0
    #define       bTTBPort_MonAddr_RESERVE_MADD                3
    #define   MSK32TTBPort_MonAddr_RESERVE_MADD                   0x00000007

    #define     BA_TTBPort_MonAddr_mAddr                       0x0004
    #define     B16TTBPort_MonAddr_mAddr                       0x0004
    #define   LSb32TTBPort_MonAddr_mAddr                          3
    #define   LSb16TTBPort_MonAddr_mAddr                          3
    #define       bTTBPort_MonAddr_mAddr                       29
    #define   MSK32TTBPort_MonAddr_mAddr                          0xFFFFFFF8
    ///////////////////////////////////////////////////////////
    #define     RA_TTBPort_MonAddrMask                         0x0008

    #define     BA_TTBPort_MonAddrMask_RESERVE_MKADD           0x0008
    #define     B16TTBPort_MonAddrMask_RESERVE_MKADD           0x0008
    #define   LSb32TTBPort_MonAddrMask_RESERVE_MKADD              0
    #define   LSb16TTBPort_MonAddrMask_RESERVE_MKADD              0
    #define       bTTBPort_MonAddrMask_RESERVE_MKADD           3
    #define   MSK32TTBPort_MonAddrMask_RESERVE_MKADD              0x00000007

    #define     BA_TTBPort_MonAddrMask_mAddrMask               0x0008
    #define     B16TTBPort_MonAddrMask_mAddrMask               0x0008
    #define   LSb32TTBPort_MonAddrMask_mAddrMask                  3
    #define   LSb16TTBPort_MonAddrMask_mAddrMask                  3
    #define       bTTBPort_MonAddrMask_mAddrMask               29
    #define   MSK32TTBPort_MonAddrMask_mAddrMask                  0xFFFFFFF8
    ///////////////////////////////////////////////////////////
    #define     RA_TTBPort_MonLen                              0x000C

    #define     BA_TTBPort_MonLen_mLen                         0x000C
    #define     B16TTBPort_MonLen_mLen                         0x000C
    #define   LSb32TTBPort_MonLen_mLen                            0
    #define   LSb16TTBPort_MonLen_mLen                            0
    #define       bTTBPort_MonLen_mLen                         4
    #define   MSK32TTBPort_MonLen_mLen                            0x0000000F

    #define     BA_TTBPort_MonLen_mLenMask                     0x000C
    #define     B16TTBPort_MonLen_mLenMask                     0x000C
    #define   LSb32TTBPort_MonLen_mLenMask                        4
    #define   LSb16TTBPort_MonLen_mLenMask                        4
    #define       bTTBPort_MonLen_mLenMask                     4
    #define   MSK32TTBPort_MonLen_mLenMask                        0x000000F0

    #define     BA_TTBPort_MonLen_RESERVE_MLEN                 0x000D
    #define     B16TTBPort_MonLen_RESERVE_MLEN                 0x000C
    #define   LSb32TTBPort_MonLen_RESERVE_MLEN                    8
    #define   LSb16TTBPort_MonLen_RESERVE_MLEN                    8
    #define       bTTBPort_MonLen_RESERVE_MLEN                 24
    #define   MSK32TTBPort_MonLen_RESERVE_MLEN                    0xFFFFFF00
    ///////////////////////////////////////////////////////////
    #define     RA_TTBPort_Status                              0x0010

    #define     BA_TTBPort_Status_no_of_trigger                0x0010
    #define     B16TTBPort_Status_no_of_trigger                0x0010
    #define   LSb32TTBPort_Status_no_of_trigger                   0
    #define   LSb16TTBPort_Status_no_of_trigger                   0
    #define       bTTBPort_Status_no_of_trigger                16
    #define   MSK32TTBPort_Status_no_of_trigger                   0x0000FFFF

    #define     BA_TTBPort_Status_RESERVE_STATUS               0x0012
    #define     B16TTBPort_Status_RESERVE_STATUS               0x0012
    #define   LSb32TTBPort_Status_RESERVE_STATUS                  16
    #define   LSb16TTBPort_Status_RESERVE_STATUS                  0
    #define       bTTBPort_Status_RESERVE_STATUS               16
    #define   MSK32TTBPort_Status_RESERVE_STATUS                  0xFFFF0000
    ///////////////////////////////////////////////////////////
    #define     RA_TTBPort_TriggerAddr                         0x0014

    #define     BA_TTBPort_TriggerAddr_RESERVE_TADDR           0x0014
    #define     B16TTBPort_TriggerAddr_RESERVE_TADDR           0x0014
    #define   LSb32TTBPort_TriggerAddr_RESERVE_TADDR              0
    #define   LSb16TTBPort_TriggerAddr_RESERVE_TADDR              0
    #define       bTTBPort_TriggerAddr_RESERVE_TADDR           3
    #define   MSK32TTBPort_TriggerAddr_RESERVE_TADDR              0x00000007

    #define     BA_TTBPort_TriggerAddr_tAddr                   0x0014
    #define     B16TTBPort_TriggerAddr_tAddr                   0x0014
    #define   LSb32TTBPort_TriggerAddr_tAddr                      3
    #define   LSb16TTBPort_TriggerAddr_tAddr                      3
    #define       bTTBPort_TriggerAddr_tAddr                   29
    #define   MSK32TTBPort_TriggerAddr_tAddr                      0xFFFFFFF8
    ///////////////////////////////////////////////////////////
    #define     RA_TTBPort_TAddrMask                           0x0018

    #define     BA_TTBPort_TAddrMask_RESERVE_TAM               0x0018
    #define     B16TTBPort_TAddrMask_RESERVE_TAM               0x0018
    #define   LSb32TTBPort_TAddrMask_RESERVE_TAM                  0
    #define   LSb16TTBPort_TAddrMask_RESERVE_TAM                  0
    #define       bTTBPort_TAddrMask_RESERVE_TAM               3
    #define   MSK32TTBPort_TAddrMask_RESERVE_TAM                  0x00000007

    #define     BA_TTBPort_TAddrMask_tAddrMask                 0x0018
    #define     B16TTBPort_TAddrMask_tAddrMask                 0x0018
    #define   LSb32TTBPort_TAddrMask_tAddrMask                    3
    #define   LSb16TTBPort_TAddrMask_tAddrMask                    3
    #define       bTTBPort_TAddrMask_tAddrMask                 29
    #define   MSK32TTBPort_TAddrMask_tAddrMask                    0xFFFFFFF8
    ///////////////////////////////////////////////////////////
    #define     RA_TTBPort_TriggerPoint                        0x001C

    #define     BA_TTBPort_TriggerPoint_tPos                   0x001C
    #define     B16TTBPort_TriggerPoint_tPos                   0x001C
    #define   LSb32TTBPort_TriggerPoint_tPos                      0
    #define   LSb16TTBPort_TriggerPoint_tPos                      0
    #define       bTTBPort_TriggerPoint_tPos                   32
    #define   MSK32TTBPort_TriggerPoint_tPos                      0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_TTBPort {
    ///////////////////////////////////////////////////////////
    #define   GET32TTBPort_Setup_enable(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32TTBPort_Setup_enable(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16TTBPort_Setup_enable(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16TTBPort_Setup_enable(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32TTBPort_Setup_interrupt(r32)                _BFGET_(r32, 1, 1)
    #define   SET32TTBPort_Setup_interrupt(r32,v)              _BFSET_(r32, 1, 1,v)
    #define   GET16TTBPort_Setup_interrupt(r16)                _BFGET_(r16, 1, 1)
    #define   SET16TTBPort_Setup_interrupt(r16,v)              _BFSET_(r16, 1, 1,v)

    #define   GET32TTBPort_Setup_recordInfoSel(r32)            _BFGET_(r32, 3, 2)
    #define   SET32TTBPort_Setup_recordInfoSel(r32,v)          _BFSET_(r32, 3, 2,v)
    #define   GET16TTBPort_Setup_recordInfoSel(r16)            _BFGET_(r16, 3, 2)
    #define   SET16TTBPort_Setup_recordInfoSel(r16,v)          _BFSET_(r16, 3, 2,v)

    #define   GET32TTBPort_Setup_no_cycle_afterward(r32)       _BFGET_(r32,19, 4)
    #define   SET32TTBPort_Setup_no_cycle_afterward(r32,v)     _BFSET_(r32,19, 4,v)

    #define   GET32TTBPort_Setup_RESERVE_MODE(r32)             _BFGET_(r32,31,20)
    #define   SET32TTBPort_Setup_RESERVE_MODE(r32,v)           _BFSET_(r32,31,20,v)
    #define   GET16TTBPort_Setup_RESERVE_MODE(r16)             _BFGET_(r16,15, 4)
    #define   SET16TTBPort_Setup_RESERVE_MODE(r16,v)           _BFSET_(r16,15, 4,v)

    #define     w32TTBPort_Setup                               {\
            UNSG32 uSetup_enable                               :  1;\
            UNSG32 uSetup_interrupt                            :  1;\
            UNSG32 uSetup_recordInfoSel                        :  2;\
            UNSG32 uSetup_no_cycle_afterward                   : 16;\
            UNSG32 uSetup_RESERVE_MODE                         : 12;\
          }
    union { UNSG32 u32TTBPort_Setup;
            struct w32TTBPort_Setup;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TTBPort_MonAddr_RESERVE_MADD(r32)           _BFGET_(r32, 2, 0)
    #define   SET32TTBPort_MonAddr_RESERVE_MADD(r32,v)         _BFSET_(r32, 2, 0,v)
    #define   GET16TTBPort_MonAddr_RESERVE_MADD(r16)           _BFGET_(r16, 2, 0)
    #define   SET16TTBPort_MonAddr_RESERVE_MADD(r16,v)         _BFSET_(r16, 2, 0,v)

    #define   GET32TTBPort_MonAddr_mAddr(r32)                  _BFGET_(r32,31, 3)
    #define   SET32TTBPort_MonAddr_mAddr(r32,v)                _BFSET_(r32,31, 3,v)

    #define     w32TTBPort_MonAddr                             {\
            UNSG32 uMonAddr_RESERVE_MADD                       :  3;\
            UNSG32 uMonAddr_mAddr                              : 29;\
          }
    union { UNSG32 u32TTBPort_MonAddr;
            struct w32TTBPort_MonAddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TTBPort_MonAddrMask_RESERVE_MKADD(r32)      _BFGET_(r32, 2, 0)
    #define   SET32TTBPort_MonAddrMask_RESERVE_MKADD(r32,v)    _BFSET_(r32, 2, 0,v)
    #define   GET16TTBPort_MonAddrMask_RESERVE_MKADD(r16)      _BFGET_(r16, 2, 0)
    #define   SET16TTBPort_MonAddrMask_RESERVE_MKADD(r16,v)    _BFSET_(r16, 2, 0,v)

    #define   GET32TTBPort_MonAddrMask_mAddrMask(r32)          _BFGET_(r32,31, 3)
    #define   SET32TTBPort_MonAddrMask_mAddrMask(r32,v)        _BFSET_(r32,31, 3,v)

    #define     w32TTBPort_MonAddrMask                         {\
            UNSG32 uMonAddrMask_RESERVE_MKADD                  :  3;\
            UNSG32 uMonAddrMask_mAddrMask                      : 29;\
          }
    union { UNSG32 u32TTBPort_MonAddrMask;
            struct w32TTBPort_MonAddrMask;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TTBPort_MonLen_mLen(r32)                    _BFGET_(r32, 3, 0)
    #define   SET32TTBPort_MonLen_mLen(r32,v)                  _BFSET_(r32, 3, 0,v)
    #define   GET16TTBPort_MonLen_mLen(r16)                    _BFGET_(r16, 3, 0)
    #define   SET16TTBPort_MonLen_mLen(r16,v)                  _BFSET_(r16, 3, 0,v)

    #define   GET32TTBPort_MonLen_mLenMask(r32)                _BFGET_(r32, 7, 4)
    #define   SET32TTBPort_MonLen_mLenMask(r32,v)              _BFSET_(r32, 7, 4,v)
    #define   GET16TTBPort_MonLen_mLenMask(r16)                _BFGET_(r16, 7, 4)
    #define   SET16TTBPort_MonLen_mLenMask(r16,v)              _BFSET_(r16, 7, 4,v)

    #define   GET32TTBPort_MonLen_RESERVE_MLEN(r32)            _BFGET_(r32,31, 8)
    #define   SET32TTBPort_MonLen_RESERVE_MLEN(r32,v)          _BFSET_(r32,31, 8,v)

    #define     w32TTBPort_MonLen                              {\
            UNSG32 uMonLen_mLen                                :  4;\
            UNSG32 uMonLen_mLenMask                            :  4;\
            UNSG32 uMonLen_RESERVE_MLEN                        : 24;\
          }
    union { UNSG32 u32TTBPort_MonLen;
            struct w32TTBPort_MonLen;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TTBPort_Status_no_of_trigger(r32)           _BFGET_(r32,15, 0)
    #define   SET32TTBPort_Status_no_of_trigger(r32,v)         _BFSET_(r32,15, 0,v)
    #define   GET16TTBPort_Status_no_of_trigger(r16)           _BFGET_(r16,15, 0)
    #define   SET16TTBPort_Status_no_of_trigger(r16,v)         _BFSET_(r16,15, 0,v)

    #define   GET32TTBPort_Status_RESERVE_STATUS(r32)          _BFGET_(r32,31,16)
    #define   SET32TTBPort_Status_RESERVE_STATUS(r32,v)        _BFSET_(r32,31,16,v)
    #define   GET16TTBPort_Status_RESERVE_STATUS(r16)          _BFGET_(r16,15, 0)
    #define   SET16TTBPort_Status_RESERVE_STATUS(r16,v)        _BFSET_(r16,15, 0,v)

    #define     w32TTBPort_Status                              {\
            UNSG32 uStatus_no_of_trigger                       : 16;\
            UNSG32 uStatus_RESERVE_STATUS                      : 16;\
          }
    union { UNSG32 u32TTBPort_Status;
            struct w32TTBPort_Status;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TTBPort_TriggerAddr_RESERVE_TADDR(r32)      _BFGET_(r32, 2, 0)
    #define   SET32TTBPort_TriggerAddr_RESERVE_TADDR(r32,v)    _BFSET_(r32, 2, 0,v)
    #define   GET16TTBPort_TriggerAddr_RESERVE_TADDR(r16)      _BFGET_(r16, 2, 0)
    #define   SET16TTBPort_TriggerAddr_RESERVE_TADDR(r16,v)    _BFSET_(r16, 2, 0,v)

    #define   GET32TTBPort_TriggerAddr_tAddr(r32)              _BFGET_(r32,31, 3)
    #define   SET32TTBPort_TriggerAddr_tAddr(r32,v)            _BFSET_(r32,31, 3,v)

    #define     w32TTBPort_TriggerAddr                         {\
            UNSG32 uTriggerAddr_RESERVE_TADDR                  :  3;\
            UNSG32 uTriggerAddr_tAddr                          : 29;\
          }
    union { UNSG32 u32TTBPort_TriggerAddr;
            struct w32TTBPort_TriggerAddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TTBPort_TAddrMask_RESERVE_TAM(r32)          _BFGET_(r32, 2, 0)
    #define   SET32TTBPort_TAddrMask_RESERVE_TAM(r32,v)        _BFSET_(r32, 2, 0,v)
    #define   GET16TTBPort_TAddrMask_RESERVE_TAM(r16)          _BFGET_(r16, 2, 0)
    #define   SET16TTBPort_TAddrMask_RESERVE_TAM(r16,v)        _BFSET_(r16, 2, 0,v)

    #define   GET32TTBPort_TAddrMask_tAddrMask(r32)            _BFGET_(r32,31, 3)
    #define   SET32TTBPort_TAddrMask_tAddrMask(r32,v)          _BFSET_(r32,31, 3,v)

    #define     w32TTBPort_TAddrMask                           {\
            UNSG32 uTAddrMask_RESERVE_TAM                      :  3;\
            UNSG32 uTAddrMask_tAddrMask                        : 29;\
          }
    union { UNSG32 u32TTBPort_TAddrMask;
            struct w32TTBPort_TAddrMask;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32TTBPort_TriggerPoint_tPos(r32)              _BFGET_(r32,31, 0)
    #define   SET32TTBPort_TriggerPoint_tPos(r32,v)            _BFSET_(r32,31, 0,v)

    #define     w32TTBPort_TriggerPoint                        {\
            UNSG32 uTriggerPoint_tPos                          : 32;\
          }
    union { UNSG32 u32TTBPort_TriggerPoint;
            struct w32TTBPort_TriggerPoint;
          };
    ///////////////////////////////////////////////////////////
    } SIE_TTBPort;

    typedef union  T32TTBPort_Setup
          { UNSG32 u32;
            struct w32TTBPort_Setup;
                 } T32TTBPort_Setup;
    typedef union  T32TTBPort_MonAddr
          { UNSG32 u32;
            struct w32TTBPort_MonAddr;
                 } T32TTBPort_MonAddr;
    typedef union  T32TTBPort_MonAddrMask
          { UNSG32 u32;
            struct w32TTBPort_MonAddrMask;
                 } T32TTBPort_MonAddrMask;
    typedef union  T32TTBPort_MonLen
          { UNSG32 u32;
            struct w32TTBPort_MonLen;
                 } T32TTBPort_MonLen;
    typedef union  T32TTBPort_Status
          { UNSG32 u32;
            struct w32TTBPort_Status;
                 } T32TTBPort_Status;
    typedef union  T32TTBPort_TriggerAddr
          { UNSG32 u32;
            struct w32TTBPort_TriggerAddr;
                 } T32TTBPort_TriggerAddr;
    typedef union  T32TTBPort_TAddrMask
          { UNSG32 u32;
            struct w32TTBPort_TAddrMask;
                 } T32TTBPort_TAddrMask;
    typedef union  T32TTBPort_TriggerPoint
          { UNSG32 u32;
            struct w32TTBPort_TriggerPoint;
                 } T32TTBPort_TriggerPoint;
    ///////////////////////////////////////////////////////////

    typedef union  TTTBPort_Setup
          { UNSG32 u32[1];
            struct {
            struct w32TTBPort_Setup;
                   };
                 } TTTBPort_Setup;
    typedef union  TTTBPort_MonAddr
          { UNSG32 u32[1];
            struct {
            struct w32TTBPort_MonAddr;
                   };
                 } TTTBPort_MonAddr;
    typedef union  TTTBPort_MonAddrMask
          { UNSG32 u32[1];
            struct {
            struct w32TTBPort_MonAddrMask;
                   };
                 } TTTBPort_MonAddrMask;
    typedef union  TTTBPort_MonLen
          { UNSG32 u32[1];
            struct {
            struct w32TTBPort_MonLen;
                   };
                 } TTTBPort_MonLen;
    typedef union  TTTBPort_Status
          { UNSG32 u32[1];
            struct {
            struct w32TTBPort_Status;
                   };
                 } TTTBPort_Status;
    typedef union  TTTBPort_TriggerAddr
          { UNSG32 u32[1];
            struct {
            struct w32TTBPort_TriggerAddr;
                   };
                 } TTTBPort_TriggerAddr;
    typedef union  TTTBPort_TAddrMask
          { UNSG32 u32[1];
            struct {
            struct w32TTBPort_TAddrMask;
                   };
                 } TTTBPort_TAddrMask;
    typedef union  TTTBPort_TriggerPoint
          { UNSG32 u32[1];
            struct {
            struct w32TTBPort_TriggerPoint;
                   };
                 } TTTBPort_TriggerPoint;

    ///////////////////////////////////////////////////////////
     SIGN32 TTBPort_drvrd(SIE_TTBPort *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 TTBPort_drvwr(SIE_TTBPort *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void TTBPort_reset(SIE_TTBPort *p);
     SIGN32 TTBPort_cmp  (SIE_TTBPort *p, SIE_TTBPort *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define TTBPort_check(p,pie,pfx,hLOG) TTBPort_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define TTBPort_print(p,    pfx,hLOG) TTBPort_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_MctrlDual
#define h_MctrlDual (){}

    #define     RA_MctrlDual_CommConfig                        0x0000

    #define     BA_MctrlDual_CommConfig_dual_channel           0x0000
    #define     B16MctrlDual_CommConfig_dual_channel           0x0000
    #define   LSb32MctrlDual_CommConfig_dual_channel              0
    #define   LSb16MctrlDual_CommConfig_dual_channel              0
    #define       bMctrlDual_CommConfig_dual_channel           1
    #define   MSK32MctrlDual_CommConfig_dual_channel              0x00000001

    #define     BA_MctrlDual_CommConfig_asymAddrMode           0x0000
    #define     B16MctrlDual_CommConfig_asymAddrMode           0x0000
    #define   LSb32MctrlDual_CommConfig_asymAddrMode              1
    #define   LSb16MctrlDual_CommConfig_asymAddrMode              1
    #define       bMctrlDual_CommConfig_asymAddrMode           2
    #define   MSK32MctrlDual_CommConfig_asymAddrMode              0x00000006

    #define     BA_MctrlDual_CommConfig_asymAddrSel            0x0000
    #define     B16MctrlDual_CommConfig_asymAddrSel            0x0000
    #define   LSb32MctrlDual_CommConfig_asymAddrSel               3
    #define   LSb16MctrlDual_CommConfig_asymAddrSel               3
    #define       bMctrlDual_CommConfig_asymAddrSel            2
    #define   MSK32MctrlDual_CommConfig_asymAddrSel               0x00000018

    #define     BA_MctrlDual_CommConfig_al_sel                 0x0000
    #define     B16MctrlDual_CommConfig_al_sel                 0x0000
    #define   LSb32MctrlDual_CommConfig_al_sel                    5
    #define   LSb16MctrlDual_CommConfig_al_sel                    5
    #define       bMctrlDual_CommConfig_al_sel                 3
    #define   MSK32MctrlDual_CommConfig_al_sel                    0x000000E0

    #define     BA_MctrlDual_CommConfig_cl_sel                 0x0001
    #define     B16MctrlDual_CommConfig_cl_sel                 0x0000
    #define   LSb32MctrlDual_CommConfig_cl_sel                    8
    #define   LSb16MctrlDual_CommConfig_cl_sel                    8
    #define       bMctrlDual_CommConfig_cl_sel                 4
    #define   MSK32MctrlDual_CommConfig_cl_sel                    0x00000F00

    #define     BA_MctrlDual_CommConfig_bank_mode              0x0001
    #define     B16MctrlDual_CommConfig_bank_mode              0x0000
    #define   LSb32MctrlDual_CommConfig_bank_mode                 12
    #define   LSb16MctrlDual_CommConfig_bank_mode                 12
    #define       bMctrlDual_CommConfig_bank_mode              1
    #define   MSK32MctrlDual_CommConfig_bank_mode                 0x00001000

    #define     BA_MctrlDual_CommConfig_mem_type               0x0001
    #define     B16MctrlDual_CommConfig_mem_type               0x0000
    #define   LSb32MctrlDual_CommConfig_mem_type                  13
    #define   LSb16MctrlDual_CommConfig_mem_type                  13
    #define       bMctrlDual_CommConfig_mem_type               3
    #define   MSK32MctrlDual_CommConfig_mem_type                  0x0000E000
    #define        MctrlDual_CommConfig_mem_type_memTypeDDRI                0x0
    #define        MctrlDual_CommConfig_mem_type_memTypeSDR                 0x1
    #define        MctrlDual_CommConfig_mem_type_memTypeMDDR                0x2
    #define        MctrlDual_CommConfig_mem_type_memTypeMSDR                0x3
    #define        MctrlDual_CommConfig_mem_type_memTypeDDRII               0x4
    #define        MctrlDual_CommConfig_mem_type_memTypeDDRIII              0x6

    #define     BA_MctrlDual_CommConfig_data_width             0x0002
    #define     B16MctrlDual_CommConfig_data_width             0x0002
    #define   LSb32MctrlDual_CommConfig_data_width                16
    #define   LSb16MctrlDual_CommConfig_data_width                0
    #define       bMctrlDual_CommConfig_data_width             2
    #define   MSK32MctrlDual_CommConfig_data_width                0x00030000
    #define        MctrlDual_CommConfig_data_width_BY16                     0x0
    #define        MctrlDual_CommConfig_data_width_BY32                     0x1
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Trigger                           0x0004

    #define     BA_MctrlDual_Trigger_sdram0_init_req           0x0004
    #define     B16MctrlDual_Trigger_sdram0_init_req           0x0004
    #define   LSb32MctrlDual_Trigger_sdram0_init_req              0
    #define   LSb16MctrlDual_Trigger_sdram0_init_req              0
    #define       bMctrlDual_Trigger_sdram0_init_req           1
    #define   MSK32MctrlDual_Trigger_sdram0_init_req              0x00000001

    #define     BA_MctrlDual_Trigger_sdram1_init_req           0x0004
    #define     B16MctrlDual_Trigger_sdram1_init_req           0x0004
    #define   LSb32MctrlDual_Trigger_sdram1_init_req              1
    #define   LSb16MctrlDual_Trigger_sdram1_init_req              1
    #define       bMctrlDual_Trigger_sdram1_init_req           1
    #define   MSK32MctrlDual_Trigger_sdram1_init_req              0x00000002

    #define     BA_MctrlDual_Trigger_pwrsav_exitReq_ch0        0x0004
    #define     B16MctrlDual_Trigger_pwrsav_exitReq_ch0        0x0004
    #define   LSb32MctrlDual_Trigger_pwrsav_exitReq_ch0           2
    #define   LSb16MctrlDual_Trigger_pwrsav_exitReq_ch0           2
    #define       bMctrlDual_Trigger_pwrsav_exitReq_ch0        1
    #define   MSK32MctrlDual_Trigger_pwrsav_exitReq_ch0           0x00000004

    #define     BA_MctrlDual_Trigger_pwrsav_exitReq_ch1        0x0004
    #define     B16MctrlDual_Trigger_pwrsav_exitReq_ch1        0x0004
    #define   LSb32MctrlDual_Trigger_pwrsav_exitReq_ch1           3
    #define   LSb16MctrlDual_Trigger_pwrsav_exitReq_ch1           3
    #define       bMctrlDual_Trigger_pwrsav_exitReq_ch1        1
    #define   MSK32MctrlDual_Trigger_pwrsav_exitReq_ch1           0x00000008

    #define     BA_MctrlDual_Trigger_pwrsav_actReq_ch0         0x0004
    #define     B16MctrlDual_Trigger_pwrsav_actReq_ch0         0x0004
    #define   LSb32MctrlDual_Trigger_pwrsav_actReq_ch0            4
    #define   LSb16MctrlDual_Trigger_pwrsav_actReq_ch0            4
    #define       bMctrlDual_Trigger_pwrsav_actReq_ch0         1
    #define   MSK32MctrlDual_Trigger_pwrsav_actReq_ch0            0x00000010

    #define     BA_MctrlDual_Trigger_pwrsav_actReq_ch1         0x0004
    #define     B16MctrlDual_Trigger_pwrsav_actReq_ch1         0x0004
    #define   LSb32MctrlDual_Trigger_pwrsav_actReq_ch1            5
    #define   LSb16MctrlDual_Trigger_pwrsav_actReq_ch1            5
    #define       bMctrlDual_Trigger_pwrsav_actReq_ch1         1
    #define   MSK32MctrlDual_Trigger_pwrsav_actReq_ch1            0x00000020

    #define     BA_MctrlDual_Trigger_auto_pwrsav_ch0           0x0004
    #define     B16MctrlDual_Trigger_auto_pwrsav_ch0           0x0004
    #define   LSb32MctrlDual_Trigger_auto_pwrsav_ch0              6
    #define   LSb16MctrlDual_Trigger_auto_pwrsav_ch0              6
    #define       bMctrlDual_Trigger_auto_pwrsav_ch0           1
    #define   MSK32MctrlDual_Trigger_auto_pwrsav_ch0              0x00000040

    #define     BA_MctrlDual_Trigger_auto_pwrsav_ch1           0x0004
    #define     B16MctrlDual_Trigger_auto_pwrsav_ch1           0x0004
    #define   LSb32MctrlDual_Trigger_auto_pwrsav_ch1              7
    #define   LSb16MctrlDual_Trigger_auto_pwrsav_ch1              7
    #define       bMctrlDual_Trigger_auto_pwrsav_ch1           1
    #define   MSK32MctrlDual_Trigger_auto_pwrsav_ch1              0x00000080

    #define     BA_MctrlDual_Trigger_wBufferFlushCh0           0x0005
    #define     B16MctrlDual_Trigger_wBufferFlushCh0           0x0004
    #define   LSb32MctrlDual_Trigger_wBufferFlushCh0              8
    #define   LSb16MctrlDual_Trigger_wBufferFlushCh0              8
    #define       bMctrlDual_Trigger_wBufferFlushCh0           1
    #define   MSK32MctrlDual_Trigger_wBufferFlushCh0              0x00000100

    #define     BA_MctrlDual_Trigger_wBufferFlushCh1           0x0005
    #define     B16MctrlDual_Trigger_wBufferFlushCh1           0x0004
    #define   LSb32MctrlDual_Trigger_wBufferFlushCh1              9
    #define   LSb16MctrlDual_Trigger_wBufferFlushCh1              9
    #define       bMctrlDual_Trigger_wBufferFlushCh1           1
    #define   MSK32MctrlDual_Trigger_wBufferFlushCh1              0x00000200

    #define     BA_MctrlDual_Trigger_cal_req                   0x0005
    #define     B16MctrlDual_Trigger_cal_req                   0x0004
    #define   LSb32MctrlDual_Trigger_cal_req                      10
    #define   LSb16MctrlDual_Trigger_cal_req                      10
    #define       bMctrlDual_Trigger_cal_req                   1
    #define   MSK32MctrlDual_Trigger_cal_req                      0x00000400

    #define     BA_MctrlDual_Trigger_ttbFlush                  0x0005
    #define     B16MctrlDual_Trigger_ttbFlush                  0x0004
    #define   LSb32MctrlDual_Trigger_ttbFlush                     11
    #define   LSb16MctrlDual_Trigger_ttbFlush                     11
    #define       bMctrlDual_Trigger_ttbFlush                  1
    #define   MSK32MctrlDual_Trigger_ttbFlush                     0x00000800

    #define     BA_MctrlDual_Trigger_loopBackTestCh0           0x0005
    #define     B16MctrlDual_Trigger_loopBackTestCh0           0x0004
    #define   LSb32MctrlDual_Trigger_loopBackTestCh0              12
    #define   LSb16MctrlDual_Trigger_loopBackTestCh0              12
    #define       bMctrlDual_Trigger_loopBackTestCh0           1
    #define   MSK32MctrlDual_Trigger_loopBackTestCh0              0x00001000

    #define     BA_MctrlDual_Trigger_loopBackTestCh1           0x0005
    #define     B16MctrlDual_Trigger_loopBackTestCh1           0x0004
    #define   LSb32MctrlDual_Trigger_loopBackTestCh1              13
    #define   LSb16MctrlDual_Trigger_loopBackTestCh1              13
    #define       bMctrlDual_Trigger_loopBackTestCh1           1
    #define   MSK32MctrlDual_Trigger_loopBackTestCh1              0x00002000
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Status                            0x0008

    #define     BA_MctrlDual_Status_sdram0_init_done           0x0008
    #define     B16MctrlDual_Status_sdram0_init_done           0x0008
    #define   LSb32MctrlDual_Status_sdram0_init_done              0
    #define   LSb16MctrlDual_Status_sdram0_init_done              0
    #define       bMctrlDual_Status_sdram0_init_done           1
    #define   MSK32MctrlDual_Status_sdram0_init_done              0x00000001

    #define     BA_MctrlDual_Status_sdram1_init_done           0x0008
    #define     B16MctrlDual_Status_sdram1_init_done           0x0008
    #define   LSb32MctrlDual_Status_sdram1_init_done              1
    #define   LSb16MctrlDual_Status_sdram1_init_done              1
    #define       bMctrlDual_Status_sdram1_init_done           1
    #define   MSK32MctrlDual_Status_sdram1_init_done              0x00000002

    #define     BA_MctrlDual_Status_pwrsav_exitDone_ch0        0x0008
    #define     B16MctrlDual_Status_pwrsav_exitDone_ch0        0x0008
    #define   LSb32MctrlDual_Status_pwrsav_exitDone_ch0           2
    #define   LSb16MctrlDual_Status_pwrsav_exitDone_ch0           2
    #define       bMctrlDual_Status_pwrsav_exitDone_ch0        1
    #define   MSK32MctrlDual_Status_pwrsav_exitDone_ch0           0x00000004

    #define     BA_MctrlDual_Status_pwrsav_exitDone_ch1        0x0008
    #define     B16MctrlDual_Status_pwrsav_exitDone_ch1        0x0008
    #define   LSb32MctrlDual_Status_pwrsav_exitDone_ch1           3
    #define   LSb16MctrlDual_Status_pwrsav_exitDone_ch1           3
    #define       bMctrlDual_Status_pwrsav_exitDone_ch1        1
    #define   MSK32MctrlDual_Status_pwrsav_exitDone_ch1           0x00000008

    #define     BA_MctrlDual_Status_pwrsav_actDone_ch0         0x0008
    #define     B16MctrlDual_Status_pwrsav_actDone_ch0         0x0008
    #define   LSb32MctrlDual_Status_pwrsav_actDone_ch0            4
    #define   LSb16MctrlDual_Status_pwrsav_actDone_ch0            4
    #define       bMctrlDual_Status_pwrsav_actDone_ch0         1
    #define   MSK32MctrlDual_Status_pwrsav_actDone_ch0            0x00000010

    #define     BA_MctrlDual_Status_pwrsav_actDone_ch1         0x0008
    #define     B16MctrlDual_Status_pwrsav_actDone_ch1         0x0008
    #define   LSb32MctrlDual_Status_pwrsav_actDone_ch1            5
    #define   LSb16MctrlDual_Status_pwrsav_actDone_ch1            5
    #define       bMctrlDual_Status_pwrsav_actDone_ch1         1
    #define   MSK32MctrlDual_Status_pwrsav_actDone_ch1            0x00000020

    #define     BA_MctrlDual_Status_aps_on_ch0                 0x0008
    #define     B16MctrlDual_Status_aps_on_ch0                 0x0008
    #define   LSb32MctrlDual_Status_aps_on_ch0                    6
    #define   LSb16MctrlDual_Status_aps_on_ch0                    6
    #define       bMctrlDual_Status_aps_on_ch0                 1
    #define   MSK32MctrlDual_Status_aps_on_ch0                    0x00000040

    #define     BA_MctrlDual_Status_aps_on_ch1                 0x0008
    #define     B16MctrlDual_Status_aps_on_ch1                 0x0008
    #define   LSb32MctrlDual_Status_aps_on_ch1                    7
    #define   LSb16MctrlDual_Status_aps_on_ch1                    7
    #define       bMctrlDual_Status_aps_on_ch1                 1
    #define   MSK32MctrlDual_Status_aps_on_ch1                    0x00000080

    #define     BA_MctrlDual_Status_wBufferEmptyCh0            0x0009
    #define     B16MctrlDual_Status_wBufferEmptyCh0            0x0008
    #define   LSb32MctrlDual_Status_wBufferEmptyCh0               8
    #define   LSb16MctrlDual_Status_wBufferEmptyCh0               8
    #define       bMctrlDual_Status_wBufferEmptyCh0            1
    #define   MSK32MctrlDual_Status_wBufferEmptyCh0               0x00000100

    #define     BA_MctrlDual_Status_wBufferEmptyCh1            0x0009
    #define     B16MctrlDual_Status_wBufferEmptyCh1            0x0008
    #define   LSb32MctrlDual_Status_wBufferEmptyCh1               9
    #define   LSb16MctrlDual_Status_wBufferEmptyCh1               9
    #define       bMctrlDual_Status_wBufferEmptyCh1            1
    #define   MSK32MctrlDual_Status_wBufferEmptyCh1               0x00000200

    #define     BA_MctrlDual_Status_inq0_empty                 0x0009
    #define     B16MctrlDual_Status_inq0_empty                 0x0008
    #define   LSb32MctrlDual_Status_inq0_empty                    10
    #define   LSb16MctrlDual_Status_inq0_empty                    10
    #define       bMctrlDual_Status_inq0_empty                 1
    #define   MSK32MctrlDual_Status_inq0_empty                    0x00000400

    #define     BA_MctrlDual_Status_inq0_full                  0x0009
    #define     B16MctrlDual_Status_inq0_full                  0x0008
    #define   LSb32MctrlDual_Status_inq0_full                     11
    #define   LSb16MctrlDual_Status_inq0_full                     11
    #define       bMctrlDual_Status_inq0_full                  1
    #define   MSK32MctrlDual_Status_inq0_full                     0x00000800

    #define     BA_MctrlDual_Status_inq1_empty                 0x0009
    #define     B16MctrlDual_Status_inq1_empty                 0x0008
    #define   LSb32MctrlDual_Status_inq1_empty                    12
    #define   LSb16MctrlDual_Status_inq1_empty                    12
    #define       bMctrlDual_Status_inq1_empty                 1
    #define   MSK32MctrlDual_Status_inq1_empty                    0x00001000

    #define     BA_MctrlDual_Status_inq1_full                  0x0009
    #define     B16MctrlDual_Status_inq1_full                  0x0008
    #define   LSb32MctrlDual_Status_inq1_full                     13
    #define   LSb16MctrlDual_Status_inq1_full                     13
    #define       bMctrlDual_Status_inq1_full                  1
    #define   MSK32MctrlDual_Status_inq1_full                     0x00002000

    #define     BA_MctrlDual_Status_ttbEmpty                   0x0009
    #define     B16MctrlDual_Status_ttbEmpty                   0x0008
    #define   LSb32MctrlDual_Status_ttbEmpty                      14
    #define   LSb16MctrlDual_Status_ttbEmpty                      14
    #define       bMctrlDual_Status_ttbEmpty                   1
    #define   MSK32MctrlDual_Status_ttbEmpty                      0x00004000

    #define     BA_MctrlDual_Status_loopBackPassCh0            0x0009
    #define     B16MctrlDual_Status_loopBackPassCh0            0x0008
    #define   LSb32MctrlDual_Status_loopBackPassCh0               15
    #define   LSb16MctrlDual_Status_loopBackPassCh0               15
    #define       bMctrlDual_Status_loopBackPassCh0            1
    #define   MSK32MctrlDual_Status_loopBackPassCh0               0x00008000

    #define     BA_MctrlDual_Status_loopBackDoneCh0            0x000A
    #define     B16MctrlDual_Status_loopBackDoneCh0            0x000A
    #define   LSb32MctrlDual_Status_loopBackDoneCh0               16
    #define   LSb16MctrlDual_Status_loopBackDoneCh0               0
    #define       bMctrlDual_Status_loopBackDoneCh0            1
    #define   MSK32MctrlDual_Status_loopBackDoneCh0               0x00010000

    #define     BA_MctrlDual_Status_loopBackPassCh1            0x000A
    #define     B16MctrlDual_Status_loopBackPassCh1            0x000A
    #define   LSb32MctrlDual_Status_loopBackPassCh1               17
    #define   LSb16MctrlDual_Status_loopBackPassCh1               1
    #define       bMctrlDual_Status_loopBackPassCh1            1
    #define   MSK32MctrlDual_Status_loopBackPassCh1               0x00020000

    #define     BA_MctrlDual_Status_loopBackDoneCh1            0x000A
    #define     B16MctrlDual_Status_loopBackDoneCh1            0x000A
    #define   LSb32MctrlDual_Status_loopBackDoneCh1               18
    #define   LSb16MctrlDual_Status_loopBackDoneCh1               2
    #define       bMctrlDual_Status_loopBackDoneCh1            1
    #define   MSK32MctrlDual_Status_loopBackDoneCh1               0x00040000
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_SettingChannel0                   0x000C

    #define     BA_MctrlDual_SettingChannel0_noColumnAddrCh0   0x000C
    #define     B16MctrlDual_SettingChannel0_noColumnAddrCh0   0x000C
    #define   LSb32MctrlDual_SettingChannel0_noColumnAddrCh0      0
    #define   LSb16MctrlDual_SettingChannel0_noColumnAddrCh0      0
    #define       bMctrlDual_SettingChannel0_noColumnAddrCh0   4
    #define   MSK32MctrlDual_SettingChannel0_noColumnAddrCh0      0x0000000F

    #define     BA_MctrlDual_SettingChannel0_noRowAddrCh0      0x000C
    #define     B16MctrlDual_SettingChannel0_noRowAddrCh0      0x000C
    #define   LSb32MctrlDual_SettingChannel0_noRowAddrCh0         4
    #define   LSb16MctrlDual_SettingChannel0_noRowAddrCh0         4
    #define       bMctrlDual_SettingChannel0_noRowAddrCh0      4
    #define   MSK32MctrlDual_SettingChannel0_noRowAddrCh0         0x000000F0

    #define     BA_MctrlDual_SettingChannel0_bankSelCh0        0x000D
    #define     B16MctrlDual_SettingChannel0_bankSelCh0        0x000C
    #define   LSb32MctrlDual_SettingChannel0_bankSelCh0           8
    #define   LSb16MctrlDual_SettingChannel0_bankSelCh0           8
    #define       bMctrlDual_SettingChannel0_bankSelCh0        1
    #define   MSK32MctrlDual_SettingChannel0_bankSelCh0           0x00000100
    #define        MctrlDual_SettingChannel0_bankSelCh0_SDRAM4BankCH0              0x0
    #define        MctrlDual_SettingChannel0_bankSelCh0_SDRAM8BankCH0              0x1

    #define     BA_MctrlDual_SettingChannel0_noChipSelectCh0   0x000D
    #define     B16MctrlDual_SettingChannel0_noChipSelectCh0   0x000C
    #define   LSb32MctrlDual_SettingChannel0_noChipSelectCh0      9
    #define   LSb16MctrlDual_SettingChannel0_noChipSelectCh0      9
    #define       bMctrlDual_SettingChannel0_noChipSelectCh0   2
    #define   MSK32MctrlDual_SettingChannel0_noChipSelectCh0      0x00000600
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_SettingChannel1                   0x0010

    #define     BA_MctrlDual_SettingChannel1_noColumnAddrCh1   0x0010
    #define     B16MctrlDual_SettingChannel1_noColumnAddrCh1   0x0010
    #define   LSb32MctrlDual_SettingChannel1_noColumnAddrCh1      0
    #define   LSb16MctrlDual_SettingChannel1_noColumnAddrCh1      0
    #define       bMctrlDual_SettingChannel1_noColumnAddrCh1   4
    #define   MSK32MctrlDual_SettingChannel1_noColumnAddrCh1      0x0000000F

    #define     BA_MctrlDual_SettingChannel1_noRowAddrCh1      0x0010
    #define     B16MctrlDual_SettingChannel1_noRowAddrCh1      0x0010
    #define   LSb32MctrlDual_SettingChannel1_noRowAddrCh1         4
    #define   LSb16MctrlDual_SettingChannel1_noRowAddrCh1         4
    #define       bMctrlDual_SettingChannel1_noRowAddrCh1      4
    #define   MSK32MctrlDual_SettingChannel1_noRowAddrCh1         0x000000F0

    #define     BA_MctrlDual_SettingChannel1_bankSelCh1        0x0011
    #define     B16MctrlDual_SettingChannel1_bankSelCh1        0x0010
    #define   LSb32MctrlDual_SettingChannel1_bankSelCh1           8
    #define   LSb16MctrlDual_SettingChannel1_bankSelCh1           8
    #define       bMctrlDual_SettingChannel1_bankSelCh1        1
    #define   MSK32MctrlDual_SettingChannel1_bankSelCh1           0x00000100
    #define        MctrlDual_SettingChannel1_bankSelCh1_SDRAM4BankCH1              0x0
    #define        MctrlDual_SettingChannel1_bankSelCh1_SDRAM8BankCH1              0x1

    #define     BA_MctrlDual_SettingChannel1_noChipSelectCh1   0x0011
    #define     B16MctrlDual_SettingChannel1_noChipSelectCh1   0x0010
    #define   LSb32MctrlDual_SettingChannel1_noChipSelectCh1      9
    #define   LSb16MctrlDual_SettingChannel1_noChipSelectCh1      9
    #define       bMctrlDual_SettingChannel1_noChipSelectCh1   2
    #define   MSK32MctrlDual_SettingChannel1_noChipSelectCh1      0x00000600
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Timing1                           0x0014

    #define     BA_MctrlDual_Timing1_init_tras                 0x0014
    #define     B16MctrlDual_Timing1_init_tras                 0x0014
    #define   LSb32MctrlDual_Timing1_init_tras                    0
    #define   LSb16MctrlDual_Timing1_init_tras                    0
    #define       bMctrlDual_Timing1_init_tras                 8
    #define   MSK32MctrlDual_Timing1_init_tras                    0x000000FF

    #define     BA_MctrlDual_Timing1_init_trfc                 0x0015
    #define     B16MctrlDual_Timing1_init_trfc                 0x0014
    #define   LSb32MctrlDual_Timing1_init_trfc                    8
    #define   LSb16MctrlDual_Timing1_init_trfc                    8
    #define       bMctrlDual_Timing1_init_trfc                 8
    #define   MSK32MctrlDual_Timing1_init_trfc                    0x0000FF00

    #define     BA_MctrlDual_Timing1_init_trc                  0x0016
    #define     B16MctrlDual_Timing1_init_trc                  0x0016
    #define   LSb32MctrlDual_Timing1_init_trc                     16
    #define   LSb16MctrlDual_Timing1_init_trc                     0
    #define       bMctrlDual_Timing1_init_trc                  8
    #define   MSK32MctrlDual_Timing1_init_trc                     0x00FF0000

    #define     BA_MctrlDual_Timing1_init_refc                 0x0017
    #define     B16MctrlDual_Timing1_init_refc                 0x0016
    #define   LSb32MctrlDual_Timing1_init_refc                    24
    #define   LSb16MctrlDual_Timing1_init_refc                    8
    #define       bMctrlDual_Timing1_init_refc                 8
    #define   MSK32MctrlDual_Timing1_init_refc                    0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Timing2                           0x0018

    #define     BA_MctrlDual_Timing2_init_trp                  0x0018
    #define     B16MctrlDual_Timing2_init_trp                  0x0018
    #define   LSb32MctrlDual_Timing2_init_trp                     0
    #define   LSb16MctrlDual_Timing2_init_trp                     0
    #define       bMctrlDual_Timing2_init_trp                  4
    #define   MSK32MctrlDual_Timing2_init_trp                     0x0000000F

    #define     BA_MctrlDual_Timing2_init_trrd                 0x0018
    #define     B16MctrlDual_Timing2_init_trrd                 0x0018
    #define   LSb32MctrlDual_Timing2_init_trrd                    4
    #define   LSb16MctrlDual_Timing2_init_trrd                    4
    #define       bMctrlDual_Timing2_init_trrd                 4
    #define   MSK32MctrlDual_Timing2_init_trrd                    0x000000F0

    #define     BA_MctrlDual_Timing2_init_trcd                 0x0019
    #define     B16MctrlDual_Timing2_init_trcd                 0x0018
    #define   LSb32MctrlDual_Timing2_init_trcd                    8
    #define   LSb16MctrlDual_Timing2_init_trcd                    8
    #define       bMctrlDual_Timing2_init_trcd                 4
    #define   MSK32MctrlDual_Timing2_init_trcd                    0x00000F00

    #define     BA_MctrlDual_Timing2_init_twr                  0x0019
    #define     B16MctrlDual_Timing2_init_twr                  0x0018
    #define   LSb32MctrlDual_Timing2_init_twr                     12
    #define   LSb16MctrlDual_Timing2_init_twr                     12
    #define       bMctrlDual_Timing2_init_twr                  4
    #define   MSK32MctrlDual_Timing2_init_twr                     0x0000F000

    #define     BA_MctrlDual_Timing2_init_trtp                 0x001A
    #define     B16MctrlDual_Timing2_init_trtp                 0x001A
    #define   LSb32MctrlDual_Timing2_init_trtp                    16
    #define   LSb16MctrlDual_Timing2_init_trtp                    0
    #define       bMctrlDual_Timing2_init_trtp                 4
    #define   MSK32MctrlDual_Timing2_init_trtp                    0x000F0000

    #define     BA_MctrlDual_Timing2_init_twtr                 0x001A
    #define     B16MctrlDual_Timing2_init_twtr                 0x001A
    #define   LSb32MctrlDual_Timing2_init_twtr                    20
    #define   LSb16MctrlDual_Timing2_init_twtr                    4
    #define       bMctrlDual_Timing2_init_twtr                 4
    #define   MSK32MctrlDual_Timing2_init_twtr                    0x00F00000

    #define     BA_MctrlDual_Timing2_init_tmrd                 0x001B
    #define     B16MctrlDual_Timing2_init_tmrd                 0x001A
    #define   LSb32MctrlDual_Timing2_init_tmrd                    24
    #define   LSb16MctrlDual_Timing2_init_tmrd                    8
    #define       bMctrlDual_Timing2_init_tmrd                 4
    #define   MSK32MctrlDual_Timing2_init_tmrd                    0x0F000000
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Timing3                           0x001C

    #define     BA_MctrlDual_Timing3_init_txsnr                0x001C
    #define     B16MctrlDual_Timing3_init_txsnr                0x001C
    #define   LSb32MctrlDual_Timing3_init_txsnr                   0
    #define   LSb16MctrlDual_Timing3_init_txsnr                   0
    #define       bMctrlDual_Timing3_init_txsnr                8
    #define   MSK32MctrlDual_Timing3_init_txsnr                   0x000000FF

    #define     BA_MctrlDual_Timing3_init_tick                 0x001D
    #define     B16MctrlDual_Timing3_init_tick                 0x001C
    #define   LSb32MctrlDual_Timing3_init_tick                    8
    #define   LSb16MctrlDual_Timing3_init_tick                    8
    #define       bMctrlDual_Timing3_init_tick                 8
    #define   MSK32MctrlDual_Timing3_init_tick                    0x0000FF00

    #define     BA_MctrlDual_Timing3_init_tfaw                 0x001E
    #define     B16MctrlDual_Timing3_init_tfaw                 0x001E
    #define   LSb32MctrlDual_Timing3_init_tfaw                    16
    #define   LSb16MctrlDual_Timing3_init_tfaw                    0
    #define       bMctrlDual_Timing3_init_tfaw                 8
    #define   MSK32MctrlDual_Timing3_init_tfaw                    0x00FF0000

    #define     BA_MctrlDual_Timing3_init_trfcmax              0x001F
    #define     B16MctrlDual_Timing3_init_trfcmax              0x001E
    #define   LSb32MctrlDual_Timing3_init_trfcmax                 24
    #define   LSb16MctrlDual_Timing3_init_trfcmax                 8
    #define       bMctrlDual_Timing3_init_trfcmax              8
    #define   MSK32MctrlDual_Timing3_init_trfcmax                 0xFF000000
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Timing4                           0x0020

    #define     BA_MctrlDual_Timing4_init_trst                 0x0020
    #define     B16MctrlDual_Timing4_init_trst                 0x0020
    #define   LSb32MctrlDual_Timing4_init_trst                    0
    #define   LSb16MctrlDual_Timing4_init_trst                    0
    #define       bMctrlDual_Timing4_init_trst                 12
    #define   MSK32MctrlDual_Timing4_init_trst                    0x00000FFF

    #define     BA_MctrlDual_Timing4_init_tmod                 0x0021
    #define     B16MctrlDual_Timing4_init_tmod                 0x0020
    #define   LSb32MctrlDual_Timing4_init_tmod                    12
    #define   LSb16MctrlDual_Timing4_init_tmod                    12
    #define       bMctrlDual_Timing4_init_tmod                 4
    #define   MSK32MctrlDual_Timing4_init_tmod                    0x0000F000

    #define     BA_MctrlDual_Timing4_init_tzqi                 0x0022
    #define     B16MctrlDual_Timing4_init_tzqi                 0x0022
    #define   LSb32MctrlDual_Timing4_init_tzqi                    16
    #define   LSb16MctrlDual_Timing4_init_tzqi                    0
    #define       bMctrlDual_Timing4_init_tzqi                 8
    #define   MSK32MctrlDual_Timing4_init_tzqi                    0x00FF0000
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Control1                          0x0024

    #define     BA_MctrlDual_Control1_init_taps                0x0024
    #define     B16MctrlDual_Control1_init_taps                0x0024
    #define   LSb32MctrlDual_Control1_init_taps                   0
    #define   LSb16MctrlDual_Control1_init_taps                   0
    #define       bMctrlDual_Control1_init_taps                8
    #define   MSK32MctrlDual_Control1_init_taps                   0x000000FF

    #define     BA_MctrlDual_Control1_wCollectEnb              0x0025
    #define     B16MctrlDual_Control1_wCollectEnb              0x0024
    #define   LSb32MctrlDual_Control1_wCollectEnb                 8
    #define   LSb16MctrlDual_Control1_wCollectEnb                 8
    #define       bMctrlDual_Control1_wCollectEnb              1
    #define   MSK32MctrlDual_Control1_wCollectEnb                 0x00000100

    #define     BA_MctrlDual_Control1_bankStatusArb            0x0025
    #define     B16MctrlDual_Control1_bankStatusArb            0x0024
    #define   LSb32MctrlDual_Control1_bankStatusArb               9
    #define   LSb16MctrlDual_Control1_bankStatusArb               9
    #define       bMctrlDual_Control1_bankStatusArb            1
    #define   MSK32MctrlDual_Control1_bankStatusArb               0x00000200

    #define     BA_MctrlDual_Control1_xbarQosEnb               0x0025
    #define     B16MctrlDual_Control1_xbarQosEnb               0x0024
    #define   LSb32MctrlDual_Control1_xbarQosEnb                  10
    #define   LSb16MctrlDual_Control1_xbarQosEnb                  10
    #define       bMctrlDual_Control1_xbarQosEnb               1
    #define   MSK32MctrlDual_Control1_xbarQosEnb                  0x00000400

    #define     BA_MctrlDual_Control1_bQReorder                0x0025
    #define     B16MctrlDual_Control1_bQReorder                0x0024
    #define   LSb32MctrlDual_Control1_bQReorder                   11
    #define   LSb16MctrlDual_Control1_bQReorder                   11
    #define       bMctrlDual_Control1_bQReorder                4
    #define   MSK32MctrlDual_Control1_bQReorder                   0x00007800

    #define     BA_MctrlDual_Control1_cpu_read_force           0x0025
    #define     B16MctrlDual_Control1_cpu_read_force           0x0024
    #define   LSb32MctrlDual_Control1_cpu_read_force              15
    #define   LSb16MctrlDual_Control1_cpu_read_force              15
    #define       bMctrlDual_Control1_cpu_read_force           1
    #define   MSK32MctrlDual_Control1_cpu_read_force              0x00008000

    #define     BA_MctrlDual_Control1_pcmdq_size               0x0026
    #define     B16MctrlDual_Control1_pcmdq_size               0x0026
    #define   LSb32MctrlDual_Control1_pcmdq_size                  16
    #define   LSb16MctrlDual_Control1_pcmdq_size                  0
    #define       bMctrlDual_Control1_pcmdq_size               2
    #define   MSK32MctrlDual_Control1_pcmdq_size                  0x00030000

    #define     BA_MctrlDual_Control1_apre_on                  0x0026
    #define     B16MctrlDual_Control1_apre_on                  0x0026
    #define   LSb32MctrlDual_Control1_apre_on                     18
    #define   LSb16MctrlDual_Control1_apre_on                     2
    #define       bMctrlDual_Control1_apre_on                  1
    #define   MSK32MctrlDual_Control1_apre_on                     0x00040000

    #define     BA_MctrlDual_Control1_sdarb_on                 0x0026
    #define     B16MctrlDual_Control1_sdarb_on                 0x0026
    #define   LSb32MctrlDual_Control1_sdarb_on                    19
    #define   LSb16MctrlDual_Control1_sdarb_on                    3
    #define       bMctrlDual_Control1_sdarb_on                 1
    #define   MSK32MctrlDual_Control1_sdarb_on                    0x00080000

    #define     BA_MctrlDual_Control1_perfCountEnb             0x0026
    #define     B16MctrlDual_Control1_perfCountEnb             0x0026
    #define   LSb32MctrlDual_Control1_perfCountEnb                20
    #define   LSb16MctrlDual_Control1_perfCountEnb                4
    #define       bMctrlDual_Control1_perfCountEnb             1
    #define   MSK32MctrlDual_Control1_perfCountEnb                0x00100000

    #define     BA_MctrlDual_Control1_cpuThrottle              0x0026
    #define     B16MctrlDual_Control1_cpuThrottle              0x0026
    #define   LSb32MctrlDual_Control1_cpuThrottle                 21
    #define   LSb16MctrlDual_Control1_cpuThrottle                 5
    #define       bMctrlDual_Control1_cpuThrottle              1
    #define   MSK32MctrlDual_Control1_cpuThrottle                 0x00200000

    #define     BA_MctrlDual_Control1_wait4QoS                 0x0026
    #define     B16MctrlDual_Control1_wait4QoS                 0x0026
    #define   LSb32MctrlDual_Control1_wait4QoS                    22
    #define   LSb16MctrlDual_Control1_wait4QoS                    6
    #define       bMctrlDual_Control1_wait4QoS                 1
    #define   MSK32MctrlDual_Control1_wait4QoS                    0x00400000
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Control2                          0x0028

    #define     BA_MctrlDual_Control2_xsrlmr_on                0x0028
    #define     B16MctrlDual_Control2_xsrlmr_on                0x0028
    #define   LSb32MctrlDual_Control2_xsrlmr_on                   0
    #define   LSb16MctrlDual_Control2_xsrlmr_on                   0
    #define       bMctrlDual_Control2_xsrlmr_on                1
    #define   MSK32MctrlDual_Control2_xsrlmr_on                   0x00000001

    #define     BA_MctrlDual_Control2_pwrsav_mode              0x0028
    #define     B16MctrlDual_Control2_pwrsav_mode              0x0028
    #define   LSb32MctrlDual_Control2_pwrsav_mode                 1
    #define   LSb16MctrlDual_Control2_pwrsav_mode                 1
    #define       bMctrlDual_Control2_pwrsav_mode              3
    #define   MSK32MctrlDual_Control2_pwrsav_mode                 0x0000000E

    #define     BA_MctrlDual_Control2_tcsr_sel                 0x0028
    #define     B16MctrlDual_Control2_tcsr_sel                 0x0028
    #define   LSb32MctrlDual_Control2_tcsr_sel                    4
    #define   LSb16MctrlDual_Control2_tcsr_sel                    4
    #define       bMctrlDual_Control2_tcsr_sel                 2
    #define   MSK32MctrlDual_Control2_tcsr_sel                    0x00000030

    #define     BA_MctrlDual_Control2_srcv_sel                 0x0028
    #define     B16MctrlDual_Control2_srcv_sel                 0x0028
    #define   LSb32MctrlDual_Control2_srcv_sel                    6
    #define   LSb16MctrlDual_Control2_srcv_sel                    6
    #define       bMctrlDual_Control2_srcv_sel                 3
    #define   MSK32MctrlDual_Control2_srcv_sel                    0x000001C0

    #define     BA_MctrlDual_Control2_sref_rate                0x0029
    #define     B16MctrlDual_Control2_sref_rate                0x0028
    #define   LSb32MctrlDual_Control2_sref_rate                   9
    #define   LSb16MctrlDual_Control2_sref_rate                   9
    #define       bMctrlDual_Control2_sref_rate                1
    #define   MSK32MctrlDual_Control2_sref_rate                   0x00000200

    #define     BA_MctrlDual_Control2_qout_oen                 0x0029
    #define     B16MctrlDual_Control2_qout_oen                 0x0028
    #define   LSb32MctrlDual_Control2_qout_oen                    10
    #define   LSb16MctrlDual_Control2_qout_oen                    10
    #define       bMctrlDual_Control2_qout_oen                 1
    #define   MSK32MctrlDual_Control2_qout_oen                    0x00000400

    #define     BA_MctrlDual_Control2_rtt_sel                  0x0029
    #define     B16MctrlDual_Control2_rtt_sel                  0x0028
    #define   LSb32MctrlDual_Control2_rtt_sel                     11
    #define   LSb16MctrlDual_Control2_rtt_sel                     11
    #define       bMctrlDual_Control2_rtt_sel                  3
    #define   MSK32MctrlDual_Control2_rtt_sel                     0x00003800

    #define     BA_MctrlDual_Control2_odrs_sel                 0x0029
    #define     B16MctrlDual_Control2_odrs_sel                 0x0028
    #define   LSb32MctrlDual_Control2_odrs_sel                    14
    #define   LSb16MctrlDual_Control2_odrs_sel                    14
    #define       bMctrlDual_Control2_odrs_sel                 2
    #define   MSK32MctrlDual_Control2_odrs_sel                    0x0000C000

    #define     BA_MctrlDual_Control2_twr_sel                  0x002A
    #define     B16MctrlDual_Control2_twr_sel                  0x002A
    #define   LSb32MctrlDual_Control2_twr_sel                     16
    #define   LSb16MctrlDual_Control2_twr_sel                     0
    #define       bMctrlDual_Control2_twr_sel                  3
    #define   MSK32MctrlDual_Control2_twr_sel                     0x00070000

    #define     BA_MctrlDual_Control2_pdext_sel                0x002A
    #define     B16MctrlDual_Control2_pdext_sel                0x002A
    #define   LSb32MctrlDual_Control2_pdext_sel                   19
    #define   LSb16MctrlDual_Control2_pdext_sel                   3
    #define       bMctrlDual_Control2_pdext_sel                1
    #define   MSK32MctrlDual_Control2_pdext_sel                   0x00080000

    #define     BA_MctrlDual_Control2_rtt_wr                   0x002A
    #define     B16MctrlDual_Control2_rtt_wr                   0x002A
    #define   LSb32MctrlDual_Control2_rtt_wr                      20
    #define   LSb16MctrlDual_Control2_rtt_wr                      4
    #define       bMctrlDual_Control2_rtt_wr                   2
    #define   MSK32MctrlDual_Control2_rtt_wr                      0x00300000

    #define     BA_MctrlDual_Control2_asr_sel                  0x002A
    #define     B16MctrlDual_Control2_asr_sel                  0x002A
    #define   LSb32MctrlDual_Control2_asr_sel                     22
    #define   LSb16MctrlDual_Control2_asr_sel                     6
    #define       bMctrlDual_Control2_asr_sel                  1
    #define   MSK32MctrlDual_Control2_asr_sel                     0x00400000

    #define     BA_MctrlDual_Control2_cwl_sel                  0x002A
    #define     B16MctrlDual_Control2_cwl_sel                  0x002A
    #define   LSb32MctrlDual_Control2_cwl_sel                     23
    #define   LSb16MctrlDual_Control2_cwl_sel                     7
    #define       bMctrlDual_Control2_cwl_sel                  3
    #define   MSK32MctrlDual_Control2_cwl_sel                     0x03800000

    #define     BA_MctrlDual_Control2_mpr_sel                  0x002B
    #define     B16MctrlDual_Control2_mpr_sel                  0x002A
    #define   LSb32MctrlDual_Control2_mpr_sel                     26
    #define   LSb16MctrlDual_Control2_mpr_sel                     10
    #define       bMctrlDual_Control2_mpr_sel                  1
    #define   MSK32MctrlDual_Control2_mpr_sel                     0x04000000

    #define     BA_MctrlDual_Control2_mpr_addr_sel             0x002B
    #define     B16MctrlDual_Control2_mpr_addr_sel             0x002A
    #define   LSb32MctrlDual_Control2_mpr_addr_sel                27
    #define   LSb16MctrlDual_Control2_mpr_addr_sel                11
    #define       bMctrlDual_Control2_mpr_addr_sel             2
    #define   MSK32MctrlDual_Control2_mpr_addr_sel                0x18000000

    #define     BA_MctrlDual_Control2_wlevel_sel               0x002B
    #define     B16MctrlDual_Control2_wlevel_sel               0x002A
    #define   LSb32MctrlDual_Control2_wlevel_sel                  29
    #define   LSb16MctrlDual_Control2_wlevel_sel                  13
    #define       bMctrlDual_Control2_wlevel_sel               1
    #define   MSK32MctrlDual_Control2_wlevel_sel                  0x20000000
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_PriorityLevel                     0x002C

    #define     BA_MctrlDual_PriorityLevel_CPU0WritePriorityLevel 0x002C
    #define     B16MctrlDual_PriorityLevel_CPU0WritePriorityLevel 0x002C
    #define   LSb32MctrlDual_PriorityLevel_CPU0WritePriorityLevel    0
    #define   LSb16MctrlDual_PriorityLevel_CPU0WritePriorityLevel    0
    #define       bMctrlDual_PriorityLevel_CPU0WritePriorityLevel 2
    #define   MSK32MctrlDual_PriorityLevel_CPU0WritePriorityLevel    0x00000003

    #define     BA_MctrlDual_PriorityLevel_CPU1WritePriorityLevel 0x002C
    #define     B16MctrlDual_PriorityLevel_CPU1WritePriorityLevel 0x002C
    #define   LSb32MctrlDual_PriorityLevel_CPU1WritePriorityLevel    2
    #define   LSb16MctrlDual_PriorityLevel_CPU1WritePriorityLevel    2
    #define       bMctrlDual_PriorityLevel_CPU1WritePriorityLevel 2
    #define   MSK32MctrlDual_PriorityLevel_CPU1WritePriorityLevel    0x0000000C

    #define     BA_MctrlDual_PriorityLevel_writePriorityLevel  0x002C
    #define     B16MctrlDual_PriorityLevel_writePriorityLevel  0x002C
    #define   LSb32MctrlDual_PriorityLevel_writePriorityLevel     4
    #define   LSb16MctrlDual_PriorityLevel_writePriorityLevel     4
    #define       bMctrlDual_PriorityLevel_writePriorityLevel  2
    #define   MSK32MctrlDual_PriorityLevel_writePriorityLevel     0x00000030

    #define     BA_MctrlDual_PriorityLevel_CPU0ReadPriorityLevel 0x002C
    #define     B16MctrlDual_PriorityLevel_CPU0ReadPriorityLevel 0x002C
    #define   LSb32MctrlDual_PriorityLevel_CPU0ReadPriorityLevel    6
    #define   LSb16MctrlDual_PriorityLevel_CPU0ReadPriorityLevel    6
    #define       bMctrlDual_PriorityLevel_CPU0ReadPriorityLevel 2
    #define   MSK32MctrlDual_PriorityLevel_CPU0ReadPriorityLevel    0x000000C0

    #define     BA_MctrlDual_PriorityLevel_CPU1ReadPriorityLevel 0x002D
    #define     B16MctrlDual_PriorityLevel_CPU1ReadPriorityLevel 0x002C
    #define   LSb32MctrlDual_PriorityLevel_CPU1ReadPriorityLevel    8
    #define   LSb16MctrlDual_PriorityLevel_CPU1ReadPriorityLevel    8
    #define       bMctrlDual_PriorityLevel_CPU1ReadPriorityLevel 2
    #define   MSK32MctrlDual_PriorityLevel_CPU1ReadPriorityLevel    0x00000300

    #define     BA_MctrlDual_PriorityLevel_read1PriorityLevel  0x002D
    #define     B16MctrlDual_PriorityLevel_read1PriorityLevel  0x002C
    #define   LSb32MctrlDual_PriorityLevel_read1PriorityLevel     10
    #define   LSb16MctrlDual_PriorityLevel_read1PriorityLevel     10
    #define       bMctrlDual_PriorityLevel_read1PriorityLevel  2
    #define   MSK32MctrlDual_PriorityLevel_read1PriorityLevel     0x00000C00

    #define     BA_MctrlDual_PriorityLevel_read2PriorityLevel  0x002D
    #define     B16MctrlDual_PriorityLevel_read2PriorityLevel  0x002C
    #define   LSb32MctrlDual_PriorityLevel_read2PriorityLevel     12
    #define   LSb16MctrlDual_PriorityLevel_read2PriorityLevel     12
    #define       bMctrlDual_PriorityLevel_read2PriorityLevel  2
    #define   MSK32MctrlDual_PriorityLevel_read2PriorityLevel     0x00003000

    #define     BA_MctrlDual_PriorityLevel_read3PriorityLevel  0x002D
    #define     B16MctrlDual_PriorityLevel_read3PriorityLevel  0x002C
    #define   LSb32MctrlDual_PriorityLevel_read3PriorityLevel     14
    #define   LSb16MctrlDual_PriorityLevel_read3PriorityLevel     14
    #define       bMctrlDual_PriorityLevel_read3PriorityLevel  2
    #define   MSK32MctrlDual_PriorityLevel_read3PriorityLevel     0x0000C000

    #define     BA_MctrlDual_PriorityLevel_read4PriorityLevel  0x002E
    #define     B16MctrlDual_PriorityLevel_read4PriorityLevel  0x002E
    #define   LSb32MctrlDual_PriorityLevel_read4PriorityLevel     16
    #define   LSb16MctrlDual_PriorityLevel_read4PriorityLevel     0
    #define       bMctrlDual_PriorityLevel_read4PriorityLevel  2
    #define   MSK32MctrlDual_PriorityLevel_read4PriorityLevel     0x00030000

    #define     BA_MctrlDual_PriorityLevel_read5PriorityLevel  0x002E
    #define     B16MctrlDual_PriorityLevel_read5PriorityLevel  0x002E
    #define   LSb32MctrlDual_PriorityLevel_read5PriorityLevel     18
    #define   LSb16MctrlDual_PriorityLevel_read5PriorityLevel     2
    #define       bMctrlDual_PriorityLevel_read5PriorityLevel  2
    #define   MSK32MctrlDual_PriorityLevel_read5PriorityLevel     0x000C0000

    #define     BA_MctrlDual_PriorityLevel_priorityMode        0x002E
    #define     B16MctrlDual_PriorityLevel_priorityMode        0x002E
    #define   LSb32MctrlDual_PriorityLevel_priorityMode           20
    #define   LSb16MctrlDual_PriorityLevel_priorityMode           4
    #define       bMctrlDual_PriorityLevel_priorityMode        1
    #define   MSK32MctrlDual_PriorityLevel_priorityMode           0x00100000

    #define     BA_MctrlDual_PriorityLevel_chArb0To1StopCount  0x002E
    #define     B16MctrlDual_PriorityLevel_chArb0To1StopCount  0x002E
    #define   LSb32MctrlDual_PriorityLevel_chArb0To1StopCount     21
    #define   LSb16MctrlDual_PriorityLevel_chArb0To1StopCount     5
    #define       bMctrlDual_PriorityLevel_chArb0To1StopCount  4
    #define   MSK32MctrlDual_PriorityLevel_chArb0To1StopCount     0x01E00000

    #define     BA_MctrlDual_PriorityLevel_chArb1To2StopCount  0x002F
    #define     B16MctrlDual_PriorityLevel_chArb1To2StopCount  0x002E
    #define   LSb32MctrlDual_PriorityLevel_chArb1To2StopCount     25
    #define   LSb16MctrlDual_PriorityLevel_chArb1To2StopCount     9
    #define       bMctrlDual_PriorityLevel_chArb1To2StopCount  4
    #define   MSK32MctrlDual_PriorityLevel_chArb1To2StopCount     0x1E000000
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_TTBStartAddr                      0x0030

    #define     BA_MctrlDual_TTBStartAddr_RESERVE_TTBSart      0x0030
    #define     B16MctrlDual_TTBStartAddr_RESERVE_TTBSart      0x0030
    #define   LSb32MctrlDual_TTBStartAddr_RESERVE_TTBSart         0
    #define   LSb16MctrlDual_TTBStartAddr_RESERVE_TTBSart         0
    #define       bMctrlDual_TTBStartAddr_RESERVE_TTBSart      5
    #define   MSK32MctrlDual_TTBStartAddr_RESERVE_TTBSart         0x0000001F

    #define     BA_MctrlDual_TTBStartAddr_ttbStartAddr         0x0030
    #define     B16MctrlDual_TTBStartAddr_ttbStartAddr         0x0030
    #define   LSb32MctrlDual_TTBStartAddr_ttbStartAddr            5
    #define   LSb16MctrlDual_TTBStartAddr_ttbStartAddr            5
    #define       bMctrlDual_TTBStartAddr_ttbStartAddr         27
    #define   MSK32MctrlDual_TTBStartAddr_ttbStartAddr            0xFFFFFFE0
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_TTBEndAddr                        0x0034

    #define     BA_MctrlDual_TTBEndAddr_RESERVE_TTBEnd         0x0034
    #define     B16MctrlDual_TTBEndAddr_RESERVE_TTBEnd         0x0034
    #define   LSb32MctrlDual_TTBEndAddr_RESERVE_TTBEnd            0
    #define   LSb16MctrlDual_TTBEndAddr_RESERVE_TTBEnd            0
    #define       bMctrlDual_TTBEndAddr_RESERVE_TTBEnd         5
    #define   MSK32MctrlDual_TTBEndAddr_RESERVE_TTBEnd            0x0000001F

    #define     BA_MctrlDual_TTBEndAddr_ttbEndAddr             0x0034
    #define     B16MctrlDual_TTBEndAddr_ttbEndAddr             0x0034
    #define   LSb32MctrlDual_TTBEndAddr_ttbEndAddr                5
    #define   LSb16MctrlDual_TTBEndAddr_ttbEndAddr                5
    #define       bMctrlDual_TTBEndAddr_ttbEndAddr             27
    #define   MSK32MctrlDual_TTBEndAddr_ttbEndAddr                0xFFFFFFE0
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_TTBWritePrt                       0x0038

    #define     BA_MctrlDual_TTBWritePrt_RESERVE_TTBWPTR       0x0038
    #define     B16MctrlDual_TTBWritePrt_RESERVE_TTBWPTR       0x0038
    #define   LSb32MctrlDual_TTBWritePrt_RESERVE_TTBWPTR          0
    #define   LSb16MctrlDual_TTBWritePrt_RESERVE_TTBWPTR          0
    #define       bMctrlDual_TTBWritePrt_RESERVE_TTBWPTR       5
    #define   MSK32MctrlDual_TTBWritePrt_RESERVE_TTBWPTR          0x0000001F

    #define     BA_MctrlDual_TTBWritePrt_ttbWPtr               0x0038
    #define     B16MctrlDual_TTBWritePrt_ttbWPtr               0x0038
    #define   LSb32MctrlDual_TTBWritePrt_ttbWPtr                  5
    #define   LSb16MctrlDual_TTBWritePrt_ttbWPtr                  5
    #define       bMctrlDual_TTBWritePrt_ttbWPtr               27
    #define   MSK32MctrlDual_TTBWritePrt_ttbWPtr                  0xFFFFFFE0
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_cpu0WPort                         0x003C
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_cpu1WPort                         0x005C
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_busWPort                          0x007C
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_cpu0RPort                         0x009C
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_cpu1RPort                         0x00BC
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_busRPort1                         0x00DC
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_busRPort2                         0x00FC
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_busRPort3                         0x011C
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_busRPort4                         0x013C
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_busRPort5                         0x015C
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_DebugTrigger                      0x017C

    #define     BA_MctrlDual_DebugTrigger_mt_loop              0x017C
    #define     B16MctrlDual_DebugTrigger_mt_loop              0x017C
    #define   LSb32MctrlDual_DebugTrigger_mt_loop                 0
    #define   LSb16MctrlDual_DebugTrigger_mt_loop                 0
    #define       bMctrlDual_DebugTrigger_mt_loop              1
    #define   MSK32MctrlDual_DebugTrigger_mt_loop                 0x00000001

    #define     BA_MctrlDual_DebugTrigger_mt_active            0x017C
    #define     B16MctrlDual_DebugTrigger_mt_active            0x017C
    #define   LSb32MctrlDual_DebugTrigger_mt_active               1
    #define   LSb16MctrlDual_DebugTrigger_mt_active               1
    #define       bMctrlDual_DebugTrigger_mt_active            1
    #define   MSK32MctrlDual_DebugTrigger_mt_active               0x00000002

    #define     BA_MctrlDual_DebugTrigger_mt_mode              0x017C
    #define     B16MctrlDual_DebugTrigger_mt_mode              0x017C
    #define   LSb32MctrlDual_DebugTrigger_mt_mode                 2
    #define   LSb16MctrlDual_DebugTrigger_mt_mode                 2
    #define       bMctrlDual_DebugTrigger_mt_mode              1
    #define   MSK32MctrlDual_DebugTrigger_mt_mode                 0x00000004

    #define     BA_MctrlDual_DebugTrigger_mt_coe               0x017C
    #define     B16MctrlDual_DebugTrigger_mt_coe               0x017C
    #define   LSb32MctrlDual_DebugTrigger_mt_coe                  3
    #define   LSb16MctrlDual_DebugTrigger_mt_coe                  3
    #define       bMctrlDual_DebugTrigger_mt_coe               1
    #define   MSK32MctrlDual_DebugTrigger_mt_coe                  0x00000008
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_DebugStAddr                       0x0180

    #define     BA_MctrlDual_DebugStAddr_addr_start            0x0180
    #define     B16MctrlDual_DebugStAddr_addr_start            0x0180
    #define   LSb32MctrlDual_DebugStAddr_addr_start               0
    #define   LSb16MctrlDual_DebugStAddr_addr_start               0
    #define       bMctrlDual_DebugStAddr_addr_start            32
    #define   MSK32MctrlDual_DebugStAddr_addr_start               0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_DebugEdAddr                       0x0184

    #define     BA_MctrlDual_DebugEdAddr_addr_end              0x0184
    #define     B16MctrlDual_DebugEdAddr_addr_end              0x0184
    #define   LSb32MctrlDual_DebugEdAddr_addr_end                 0
    #define   LSb16MctrlDual_DebugEdAddr_addr_end                 0
    #define       bMctrlDual_DebugEdAddr_addr_end              32
    #define   MSK32MctrlDual_DebugEdAddr_addr_end                 0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_DebugStatus                       0x0188

    #define     BA_MctrlDual_DebugStatus_mt_done               0x0188
    #define     B16MctrlDual_DebugStatus_mt_done               0x0188
    #define   LSb32MctrlDual_DebugStatus_mt_done                  0
    #define   LSb16MctrlDual_DebugStatus_mt_done                  0
    #define       bMctrlDual_DebugStatus_mt_done               1
    #define   MSK32MctrlDual_DebugStatus_mt_done                  0x00000001

    #define     BA_MctrlDual_DebugStatus_mt_error              0x0188
    #define     B16MctrlDual_DebugStatus_mt_error              0x0188
    #define   LSb32MctrlDual_DebugStatus_mt_error                 1
    #define   LSb16MctrlDual_DebugStatus_mt_error                 1
    #define       bMctrlDual_DebugStatus_mt_error              2
    #define   MSK32MctrlDual_DebugStatus_mt_error                 0x00000006
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_DebugRaddr                        0x018C

    #define     BA_MctrlDual_DebugRaddr_mt_raddr               0x018C
    #define     B16MctrlDual_DebugRaddr_mt_raddr               0x018C
    #define   LSb32MctrlDual_DebugRaddr_mt_raddr                  0
    #define   LSb16MctrlDual_DebugRaddr_mt_raddr                  0
    #define       bMctrlDual_DebugRaddr_mt_raddr               32
    #define   MSK32MctrlDual_DebugRaddr_mt_raddr                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_DebugRdata0                       0x0190

    #define     BA_MctrlDual_DebugRdata0_mt_rdata0             0x0190
    #define     B16MctrlDual_DebugRdata0_mt_rdata0             0x0190
    #define   LSb32MctrlDual_DebugRdata0_mt_rdata0                0
    #define   LSb16MctrlDual_DebugRdata0_mt_rdata0                0
    #define       bMctrlDual_DebugRdata0_mt_rdata0             32
    #define   MSK32MctrlDual_DebugRdata0_mt_rdata0                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_DebugRdata1                       0x0194

    #define     BA_MctrlDual_DebugRdata1_mt_rdata1             0x0194
    #define     B16MctrlDual_DebugRdata1_mt_rdata1             0x0194
    #define   LSb32MctrlDual_DebugRdata1_mt_rdata1                0
    #define   LSb16MctrlDual_DebugRdata1_mt_rdata1                0
    #define       bMctrlDual_DebugRdata1_mt_rdata1             32
    #define   MSK32MctrlDual_DebugRdata1_mt_rdata1                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_DebugOData                        0x0198

    #define     BA_MctrlDual_DebugOData_mt_odd                 0x0198
    #define     B16MctrlDual_DebugOData_mt_odd                 0x0198
    #define   LSb32MctrlDual_DebugOData_mt_odd                    0
    #define   LSb16MctrlDual_DebugOData_mt_odd                    0
    #define       bMctrlDual_DebugOData_mt_odd                 32
    #define   MSK32MctrlDual_DebugOData_mt_odd                    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_DebugEData                        0x019C

    #define     BA_MctrlDual_DebugEData_mt_even                0x019C
    #define     B16MctrlDual_DebugEData_mt_even                0x019C
    #define   LSb32MctrlDual_DebugEData_mt_even                   0
    #define   LSb16MctrlDual_DebugEData_mt_even                   0
    #define       bMctrlDual_DebugEData_mt_even                32
    #define   MSK32MctrlDual_DebugEData_mt_even                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_PHY                               0x01A0
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_InterruptMask                     0x0200

    #define     BA_MctrlDual_InterruptMask_intMaskPort0        0x0200
    #define     B16MctrlDual_InterruptMask_intMaskPort0        0x0200
    #define   LSb32MctrlDual_InterruptMask_intMaskPort0           0
    #define   LSb16MctrlDual_InterruptMask_intMaskPort0           0
    #define       bMctrlDual_InterruptMask_intMaskPort0        1
    #define   MSK32MctrlDual_InterruptMask_intMaskPort0           0x00000001

    #define     BA_MctrlDual_InterruptMask_intMaskPort1        0x0200
    #define     B16MctrlDual_InterruptMask_intMaskPort1        0x0200
    #define   LSb32MctrlDual_InterruptMask_intMaskPort1           1
    #define   LSb16MctrlDual_InterruptMask_intMaskPort1           1
    #define       bMctrlDual_InterruptMask_intMaskPort1        1
    #define   MSK32MctrlDual_InterruptMask_intMaskPort1           0x00000002

    #define     BA_MctrlDual_InterruptMask_intMaskPort2        0x0200
    #define     B16MctrlDual_InterruptMask_intMaskPort2        0x0200
    #define   LSb32MctrlDual_InterruptMask_intMaskPort2           2
    #define   LSb16MctrlDual_InterruptMask_intMaskPort2           2
    #define       bMctrlDual_InterruptMask_intMaskPort2        1
    #define   MSK32MctrlDual_InterruptMask_intMaskPort2           0x00000004

    #define     BA_MctrlDual_InterruptMask_intMaskPort3        0x0200
    #define     B16MctrlDual_InterruptMask_intMaskPort3        0x0200
    #define   LSb32MctrlDual_InterruptMask_intMaskPort3           3
    #define   LSb16MctrlDual_InterruptMask_intMaskPort3           3
    #define       bMctrlDual_InterruptMask_intMaskPort3        1
    #define   MSK32MctrlDual_InterruptMask_intMaskPort3           0x00000008

    #define     BA_MctrlDual_InterruptMask_intMaskPor4         0x0200
    #define     B16MctrlDual_InterruptMask_intMaskPor4         0x0200
    #define   LSb32MctrlDual_InterruptMask_intMaskPor4            4
    #define   LSb16MctrlDual_InterruptMask_intMaskPor4            4
    #define       bMctrlDual_InterruptMask_intMaskPor4         1
    #define   MSK32MctrlDual_InterruptMask_intMaskPor4            0x00000010

    #define     BA_MctrlDual_InterruptMask_intMaskPort5        0x0200
    #define     B16MctrlDual_InterruptMask_intMaskPort5        0x0200
    #define   LSb32MctrlDual_InterruptMask_intMaskPort5           5
    #define   LSb16MctrlDual_InterruptMask_intMaskPort5           5
    #define       bMctrlDual_InterruptMask_intMaskPort5        1
    #define   MSK32MctrlDual_InterruptMask_intMaskPort5           0x00000020

    #define     BA_MctrlDual_InterruptMask_intMaskCpu0R        0x0200
    #define     B16MctrlDual_InterruptMask_intMaskCpu0R        0x0200
    #define   LSb32MctrlDual_InterruptMask_intMaskCpu0R           6
    #define   LSb16MctrlDual_InterruptMask_intMaskCpu0R           6
    #define       bMctrlDual_InterruptMask_intMaskCpu0R        1
    #define   MSK32MctrlDual_InterruptMask_intMaskCpu0R           0x00000040

    #define     BA_MctrlDual_InterruptMask_intMaskCpu1R        0x0200
    #define     B16MctrlDual_InterruptMask_intMaskCpu1R        0x0200
    #define   LSb32MctrlDual_InterruptMask_intMaskCpu1R           7
    #define   LSb16MctrlDual_InterruptMask_intMaskCpu1R           7
    #define       bMctrlDual_InterruptMask_intMaskCpu1R        1
    #define   MSK32MctrlDual_InterruptMask_intMaskCpu1R           0x00000080

    #define     BA_MctrlDual_InterruptMask_intMaskCpu0W        0x0201
    #define     B16MctrlDual_InterruptMask_intMaskCpu0W        0x0200
    #define   LSb32MctrlDual_InterruptMask_intMaskCpu0W           8
    #define   LSb16MctrlDual_InterruptMask_intMaskCpu0W           8
    #define       bMctrlDual_InterruptMask_intMaskCpu0W        1
    #define   MSK32MctrlDual_InterruptMask_intMaskCpu0W           0x00000100

    #define     BA_MctrlDual_InterruptMask_intMaskCpu1W        0x0201
    #define     B16MctrlDual_InterruptMask_intMaskCpu1W        0x0200
    #define   LSb32MctrlDual_InterruptMask_intMaskCpu1W           9
    #define   LSb16MctrlDual_InterruptMask_intMaskCpu1W           9
    #define       bMctrlDual_InterruptMask_intMaskCpu1W        1
    #define   MSK32MctrlDual_InterruptMask_intMaskCpu1W           0x00000200
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_interruptStatus                   0x0204

    #define     BA_MctrlDual_interruptStatus_interruptPort0    0x0204
    #define     B16MctrlDual_interruptStatus_interruptPort0    0x0204
    #define   LSb32MctrlDual_interruptStatus_interruptPort0       0
    #define   LSb16MctrlDual_interruptStatus_interruptPort0       0
    #define       bMctrlDual_interruptStatus_interruptPort0    1
    #define   MSK32MctrlDual_interruptStatus_interruptPort0       0x00000001

    #define     BA_MctrlDual_interruptStatus_interruptPort1    0x0204
    #define     B16MctrlDual_interruptStatus_interruptPort1    0x0204
    #define   LSb32MctrlDual_interruptStatus_interruptPort1       1
    #define   LSb16MctrlDual_interruptStatus_interruptPort1       1
    #define       bMctrlDual_interruptStatus_interruptPort1    1
    #define   MSK32MctrlDual_interruptStatus_interruptPort1       0x00000002

    #define     BA_MctrlDual_interruptStatus_interruptPort2    0x0204
    #define     B16MctrlDual_interruptStatus_interruptPort2    0x0204
    #define   LSb32MctrlDual_interruptStatus_interruptPort2       2
    #define   LSb16MctrlDual_interruptStatus_interruptPort2       2
    #define       bMctrlDual_interruptStatus_interruptPort2    1
    #define   MSK32MctrlDual_interruptStatus_interruptPort2       0x00000004

    #define     BA_MctrlDual_interruptStatus_interruptPort3    0x0204
    #define     B16MctrlDual_interruptStatus_interruptPort3    0x0204
    #define   LSb32MctrlDual_interruptStatus_interruptPort3       3
    #define   LSb16MctrlDual_interruptStatus_interruptPort3       3
    #define       bMctrlDual_interruptStatus_interruptPort3    1
    #define   MSK32MctrlDual_interruptStatus_interruptPort3       0x00000008

    #define     BA_MctrlDual_interruptStatus_interruptPort4    0x0204
    #define     B16MctrlDual_interruptStatus_interruptPort4    0x0204
    #define   LSb32MctrlDual_interruptStatus_interruptPort4       4
    #define   LSb16MctrlDual_interruptStatus_interruptPort4       4
    #define       bMctrlDual_interruptStatus_interruptPort4    1
    #define   MSK32MctrlDual_interruptStatus_interruptPort4       0x00000010

    #define     BA_MctrlDual_interruptStatus_interruptPort5    0x0204
    #define     B16MctrlDual_interruptStatus_interruptPort5    0x0204
    #define   LSb32MctrlDual_interruptStatus_interruptPort5       5
    #define   LSb16MctrlDual_interruptStatus_interruptPort5       5
    #define       bMctrlDual_interruptStatus_interruptPort5    1
    #define   MSK32MctrlDual_interruptStatus_interruptPort5       0x00000020

    #define     BA_MctrlDual_interruptStatus_interruptCpu0W    0x0204
    #define     B16MctrlDual_interruptStatus_interruptCpu0W    0x0204
    #define   LSb32MctrlDual_interruptStatus_interruptCpu0W       6
    #define   LSb16MctrlDual_interruptStatus_interruptCpu0W       6
    #define       bMctrlDual_interruptStatus_interruptCpu0W    1
    #define   MSK32MctrlDual_interruptStatus_interruptCpu0W       0x00000040

    #define     BA_MctrlDual_interruptStatus_interruptCpu0R    0x0204
    #define     B16MctrlDual_interruptStatus_interruptCpu0R    0x0204
    #define   LSb32MctrlDual_interruptStatus_interruptCpu0R       7
    #define   LSb16MctrlDual_interruptStatus_interruptCpu0R       7
    #define       bMctrlDual_interruptStatus_interruptCpu0R    1
    #define   MSK32MctrlDual_interruptStatus_interruptCpu0R       0x00000080

    #define     BA_MctrlDual_interruptStatus_interruptCpu1W    0x0205
    #define     B16MctrlDual_interruptStatus_interruptCpu1W    0x0204
    #define   LSb32MctrlDual_interruptStatus_interruptCpu1W       8
    #define   LSb16MctrlDual_interruptStatus_interruptCpu1W       8
    #define       bMctrlDual_interruptStatus_interruptCpu1W    1
    #define   MSK32MctrlDual_interruptStatus_interruptCpu1W       0x00000100

    #define     BA_MctrlDual_interruptStatus_interruptCpu1R    0x0205
    #define     B16MctrlDual_interruptStatus_interruptCpu1R    0x0204
    #define   LSb32MctrlDual_interruptStatus_interruptCpu1R       9
    #define   LSb16MctrlDual_interruptStatus_interruptCpu1R       9
    #define       bMctrlDual_interruptStatus_interruptCpu1R    1
    #define   MSK32MctrlDual_interruptStatus_interruptCpu1R       0x00000200
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_LFSRData                          0x0208

    #define     BA_MctrlDual_LFSRData_lfsrOut                  0x0208
    #define     B16MctrlDual_LFSRData_lfsrOut                  0x0208
    #define   LSb32MctrlDual_LFSRData_lfsrOut                     0
    #define   LSb16MctrlDual_LFSRData_lfsrOut                     0
    #define       bMctrlDual_LFSRData_lfsrOut                  32
    #define   MSK32MctrlDual_LFSRData_lfsrOut                     0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_PerfCounterRst                    0x020C

    #define     BA_MctrlDual_PerfCounterRst_cycleCountRst      0x020C
    #define     B16MctrlDual_PerfCounterRst_cycleCountRst      0x020C
    #define   LSb32MctrlDual_PerfCounterRst_cycleCountRst         0
    #define   LSb16MctrlDual_PerfCounterRst_cycleCountRst         0
    #define       bMctrlDual_PerfCounterRst_cycleCountRst      1
    #define   MSK32MctrlDual_PerfCounterRst_cycleCountRst         0x00000001

    #define     BA_MctrlDual_PerfCounterRst_ctrlIdleCountRst   0x020C
    #define     B16MctrlDual_PerfCounterRst_ctrlIdleCountRst   0x020C
    #define   LSb32MctrlDual_PerfCounterRst_ctrlIdleCountRst      1
    #define   LSb16MctrlDual_PerfCounterRst_ctrlIdleCountRst      1
    #define       bMctrlDual_PerfCounterRst_ctrlIdleCountRst   1
    #define   MSK32MctrlDual_PerfCounterRst_ctrlIdleCountRst      0x00000002

    #define     BA_MctrlDual_PerfCounterRst_dBusBusyCh0CountRst 0x020C
    #define     B16MctrlDual_PerfCounterRst_dBusBusyCh0CountRst 0x020C
    #define   LSb32MctrlDual_PerfCounterRst_dBusBusyCh0CountRst    2
    #define   LSb16MctrlDual_PerfCounterRst_dBusBusyCh0CountRst    2
    #define       bMctrlDual_PerfCounterRst_dBusBusyCh0CountRst 1
    #define   MSK32MctrlDual_PerfCounterRst_dBusBusyCh0CountRst    0x00000004

    #define     BA_MctrlDual_PerfCounterRst_dBusBusyCh1CountRst 0x020C
    #define     B16MctrlDual_PerfCounterRst_dBusBusyCh1CountRst 0x020C
    #define   LSb32MctrlDual_PerfCounterRst_dBusBusyCh1CountRst    3
    #define   LSb16MctrlDual_PerfCounterRst_dBusBusyCh1CountRst    3
    #define       bMctrlDual_PerfCounterRst_dBusBusyCh1CountRst 1
    #define   MSK32MctrlDual_PerfCounterRst_dBusBusyCh1CountRst    0x00000008

    #define     BA_MctrlDual_PerfCounterRst_busyCh0CountRst    0x020C
    #define     B16MctrlDual_PerfCounterRst_busyCh0CountRst    0x020C
    #define   LSb32MctrlDual_PerfCounterRst_busyCh0CountRst       4
    #define   LSb16MctrlDual_PerfCounterRst_busyCh0CountRst       4
    #define       bMctrlDual_PerfCounterRst_busyCh0CountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_busyCh0CountRst       0x00000010

    #define     BA_MctrlDual_PerfCounterRst_busyCh1CountRst    0x020C
    #define     B16MctrlDual_PerfCounterRst_busyCh1CountRst    0x020C
    #define   LSb32MctrlDual_PerfCounterRst_busyCh1CountRst       5
    #define   LSb16MctrlDual_PerfCounterRst_busyCh1CountRst       5
    #define       bMctrlDual_PerfCounterRst_busyCh1CountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_busyCh1CountRst       0x00000020

    #define     BA_MctrlDual_PerfCounterRst_cpu0WReqCountRst   0x020C
    #define     B16MctrlDual_PerfCounterRst_cpu0WReqCountRst   0x020C
    #define   LSb32MctrlDual_PerfCounterRst_cpu0WReqCountRst      6
    #define   LSb16MctrlDual_PerfCounterRst_cpu0WReqCountRst      6
    #define       bMctrlDual_PerfCounterRst_cpu0WReqCountRst   1
    #define   MSK32MctrlDual_PerfCounterRst_cpu0WReqCountRst      0x00000040

    #define     BA_MctrlDual_PerfCounterRst_cpu1WReqCountRst   0x020C
    #define     B16MctrlDual_PerfCounterRst_cpu1WReqCountRst   0x020C
    #define   LSb32MctrlDual_PerfCounterRst_cpu1WReqCountRst      7
    #define   LSb16MctrlDual_PerfCounterRst_cpu1WReqCountRst      7
    #define       bMctrlDual_PerfCounterRst_cpu1WReqCountRst   1
    #define   MSK32MctrlDual_PerfCounterRst_cpu1WReqCountRst      0x00000080

    #define     BA_MctrlDual_PerfCounterRst_cpu0RReqCountRst   0x020D
    #define     B16MctrlDual_PerfCounterRst_cpu0RReqCountRst   0x020C
    #define   LSb32MctrlDual_PerfCounterRst_cpu0RReqCountRst      8
    #define   LSb16MctrlDual_PerfCounterRst_cpu0RReqCountRst      8
    #define       bMctrlDual_PerfCounterRst_cpu0RReqCountRst   1
    #define   MSK32MctrlDual_PerfCounterRst_cpu0RReqCountRst      0x00000100

    #define     BA_MctrlDual_PerfCounterRst_cpu1RReqCountRst   0x020D
    #define     B16MctrlDual_PerfCounterRst_cpu1RReqCountRst   0x020C
    #define   LSb32MctrlDual_PerfCounterRst_cpu1RReqCountRst      9
    #define   LSb16MctrlDual_PerfCounterRst_cpu1RReqCountRst      9
    #define       bMctrlDual_PerfCounterRst_cpu1RReqCountRst   1
    #define   MSK32MctrlDual_PerfCounterRst_cpu1RReqCountRst      0x00000200

    #define     BA_MctrlDual_PerfCounterRst_port0ReqCountRst   0x020D
    #define     B16MctrlDual_PerfCounterRst_port0ReqCountRst   0x020C
    #define   LSb32MctrlDual_PerfCounterRst_port0ReqCountRst      10
    #define   LSb16MctrlDual_PerfCounterRst_port0ReqCountRst      10
    #define       bMctrlDual_PerfCounterRst_port0ReqCountRst   1
    #define   MSK32MctrlDual_PerfCounterRst_port0ReqCountRst      0x00000400

    #define     BA_MctrlDual_PerfCounterRst_port1ReqCountRst   0x020D
    #define     B16MctrlDual_PerfCounterRst_port1ReqCountRst   0x020C
    #define   LSb32MctrlDual_PerfCounterRst_port1ReqCountRst      11
    #define   LSb16MctrlDual_PerfCounterRst_port1ReqCountRst      11
    #define       bMctrlDual_PerfCounterRst_port1ReqCountRst   1
    #define   MSK32MctrlDual_PerfCounterRst_port1ReqCountRst      0x00000800

    #define     BA_MctrlDual_PerfCounterRst_port2ReqCountRst   0x020D
    #define     B16MctrlDual_PerfCounterRst_port2ReqCountRst   0x020C
    #define   LSb32MctrlDual_PerfCounterRst_port2ReqCountRst      12
    #define   LSb16MctrlDual_PerfCounterRst_port2ReqCountRst      12
    #define       bMctrlDual_PerfCounterRst_port2ReqCountRst   1
    #define   MSK32MctrlDual_PerfCounterRst_port2ReqCountRst      0x00001000

    #define     BA_MctrlDual_PerfCounterRst_port3ReqCountRst   0x020D
    #define     B16MctrlDual_PerfCounterRst_port3ReqCountRst   0x020C
    #define   LSb32MctrlDual_PerfCounterRst_port3ReqCountRst      13
    #define   LSb16MctrlDual_PerfCounterRst_port3ReqCountRst      13
    #define       bMctrlDual_PerfCounterRst_port3ReqCountRst   1
    #define   MSK32MctrlDual_PerfCounterRst_port3ReqCountRst      0x00002000

    #define     BA_MctrlDual_PerfCounterRst_port4ReqCountRst   0x020D
    #define     B16MctrlDual_PerfCounterRst_port4ReqCountRst   0x020C
    #define   LSb32MctrlDual_PerfCounterRst_port4ReqCountRst      14
    #define   LSb16MctrlDual_PerfCounterRst_port4ReqCountRst      14
    #define       bMctrlDual_PerfCounterRst_port4ReqCountRst   1
    #define   MSK32MctrlDual_PerfCounterRst_port4ReqCountRst      0x00004000

    #define     BA_MctrlDual_PerfCounterRst_port5ReqCountRst   0x020D
    #define     B16MctrlDual_PerfCounterRst_port5ReqCountRst   0x020C
    #define   LSb32MctrlDual_PerfCounterRst_port5ReqCountRst      15
    #define   LSb16MctrlDual_PerfCounterRst_port5ReqCountRst      15
    #define       bMctrlDual_PerfCounterRst_port5ReqCountRst   1
    #define   MSK32MctrlDual_PerfCounterRst_port5ReqCountRst      0x00008000

    #define     BA_MctrlDual_PerfCounterRst_port0FCCountRst    0x020E
    #define     B16MctrlDual_PerfCounterRst_port0FCCountRst    0x020E
    #define   LSb32MctrlDual_PerfCounterRst_port0FCCountRst       16
    #define   LSb16MctrlDual_PerfCounterRst_port0FCCountRst       0
    #define       bMctrlDual_PerfCounterRst_port0FCCountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_port0FCCountRst       0x00010000

    #define     BA_MctrlDual_PerfCounterRst_port1FCCountRst    0x020E
    #define     B16MctrlDual_PerfCounterRst_port1FCCountRst    0x020E
    #define   LSb32MctrlDual_PerfCounterRst_port1FCCountRst       17
    #define   LSb16MctrlDual_PerfCounterRst_port1FCCountRst       1
    #define       bMctrlDual_PerfCounterRst_port1FCCountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_port1FCCountRst       0x00020000

    #define     BA_MctrlDual_PerfCounterRst_port2FCCountRst    0x020E
    #define     B16MctrlDual_PerfCounterRst_port2FCCountRst    0x020E
    #define   LSb32MctrlDual_PerfCounterRst_port2FCCountRst       18
    #define   LSb16MctrlDual_PerfCounterRst_port2FCCountRst       2
    #define       bMctrlDual_PerfCounterRst_port2FCCountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_port2FCCountRst       0x00040000

    #define     BA_MctrlDual_PerfCounterRst_port3FCCountRst    0x020E
    #define     B16MctrlDual_PerfCounterRst_port3FCCountRst    0x020E
    #define   LSb32MctrlDual_PerfCounterRst_port3FCCountRst       19
    #define   LSb16MctrlDual_PerfCounterRst_port3FCCountRst       3
    #define       bMctrlDual_PerfCounterRst_port3FCCountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_port3FCCountRst       0x00080000

    #define     BA_MctrlDual_PerfCounterRst_port4FCCountRst    0x020E
    #define     B16MctrlDual_PerfCounterRst_port4FCCountRst    0x020E
    #define   LSb32MctrlDual_PerfCounterRst_port4FCCountRst       20
    #define   LSb16MctrlDual_PerfCounterRst_port4FCCountRst       4
    #define       bMctrlDual_PerfCounterRst_port4FCCountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_port4FCCountRst       0x00100000

    #define     BA_MctrlDual_PerfCounterRst_port5FCCountRst    0x020E
    #define     B16MctrlDual_PerfCounterRst_port5FCCountRst    0x020E
    #define   LSb32MctrlDual_PerfCounterRst_port5FCCountRst       21
    #define   LSb16MctrlDual_PerfCounterRst_port5FCCountRst       5
    #define       bMctrlDual_PerfCounterRst_port5FCCountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_port5FCCountRst       0x00200000

    #define     BA_MctrlDual_PerfCounterRst_cpu0WSeCountRst    0x020E
    #define     B16MctrlDual_PerfCounterRst_cpu0WSeCountRst    0x020E
    #define   LSb32MctrlDual_PerfCounterRst_cpu0WSeCountRst       22
    #define   LSb16MctrlDual_PerfCounterRst_cpu0WSeCountRst       6
    #define       bMctrlDual_PerfCounterRst_cpu0WSeCountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_cpu0WSeCountRst       0x00400000

    #define     BA_MctrlDual_PerfCounterRst_cpu0RSeCountRst    0x020E
    #define     B16MctrlDual_PerfCounterRst_cpu0RSeCountRst    0x020E
    #define   LSb32MctrlDual_PerfCounterRst_cpu0RSeCountRst       23
    #define   LSb16MctrlDual_PerfCounterRst_cpu0RSeCountRst       7
    #define       bMctrlDual_PerfCounterRst_cpu0RSeCountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_cpu0RSeCountRst       0x00800000

    #define     BA_MctrlDual_PerfCounterRst_extMWSeCountRst    0x020F
    #define     B16MctrlDual_PerfCounterRst_extMWSeCountRst    0x020E
    #define   LSb32MctrlDual_PerfCounterRst_extMWSeCountRst       24
    #define   LSb16MctrlDual_PerfCounterRst_extMWSeCountRst       8
    #define       bMctrlDual_PerfCounterRst_extMWSeCountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_extMWSeCountRst       0x01000000

    #define     BA_MctrlDual_PerfCounterRst_extMRSeCountRst    0x020F
    #define     B16MctrlDual_PerfCounterRst_extMRSeCountRst    0x020E
    #define   LSb32MctrlDual_PerfCounterRst_extMRSeCountRst       25
    #define   LSb16MctrlDual_PerfCounterRst_extMRSeCountRst       9
    #define       bMctrlDual_PerfCounterRst_extMRSeCountRst    1
    #define   MSK32MctrlDual_PerfCounterRst_extMRSeCountRst       0x02000000
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_CycleCount                        0x0210

    #define     BA_MctrlDual_CycleCount_value                  0x0210
    #define     B16MctrlDual_CycleCount_value                  0x0210
    #define   LSb32MctrlDual_CycleCount_value                     0
    #define   LSb16MctrlDual_CycleCount_value                     0
    #define       bMctrlDual_CycleCount_value                  32
    #define   MSK32MctrlDual_CycleCount_value                     0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_ReqIdleCount                      0x0214

    #define     BA_MctrlDual_ReqIdleCount_value                0x0214
    #define     B16MctrlDual_ReqIdleCount_value                0x0214
    #define   LSb32MctrlDual_ReqIdleCount_value                   0
    #define   LSb16MctrlDual_ReqIdleCount_value                   0
    #define       bMctrlDual_ReqIdleCount_value                32
    #define   MSK32MctrlDual_ReqIdleCount_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_oneChActCount                     0x0218

    #define     BA_MctrlDual_oneChActCount_value               0x0218
    #define     B16MctrlDual_oneChActCount_value               0x0218
    #define   LSb32MctrlDual_oneChActCount_value                  0
    #define   LSb16MctrlDual_oneChActCount_value                  0
    #define       bMctrlDual_oneChActCount_value               32
    #define   MSK32MctrlDual_oneChActCount_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_DBusyCountCh0                     0x021C

    #define     BA_MctrlDual_DBusyCountCh0_value               0x021C
    #define     B16MctrlDual_DBusyCountCh0_value               0x021C
    #define   LSb32MctrlDual_DBusyCountCh0_value                  0
    #define   LSb16MctrlDual_DBusyCountCh0_value                  0
    #define       bMctrlDual_DBusyCountCh0_value               32
    #define   MSK32MctrlDual_DBusyCountCh0_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_DBusyCountCh1                     0x0220

    #define     BA_MctrlDual_DBusyCountCh1_value               0x0220
    #define     B16MctrlDual_DBusyCountCh1_value               0x0220
    #define   LSb32MctrlDual_DBusyCountCh1_value                  0
    #define   LSb16MctrlDual_DBusyCountCh1_value                  0
    #define       bMctrlDual_DBusyCountCh1_value               32
    #define   MSK32MctrlDual_DBusyCountCh1_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_busyCountCh0                      0x0224

    #define     BA_MctrlDual_busyCountCh0_value                0x0224
    #define     B16MctrlDual_busyCountCh0_value                0x0224
    #define   LSb32MctrlDual_busyCountCh0_value                   0
    #define   LSb16MctrlDual_busyCountCh0_value                   0
    #define       bMctrlDual_busyCountCh0_value                32
    #define   MSK32MctrlDual_busyCountCh0_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_busyCountCh1                      0x0228

    #define     BA_MctrlDual_busyCountCh1_value                0x0228
    #define     B16MctrlDual_busyCountCh1_value                0x0228
    #define   LSb32MctrlDual_busyCountCh1_value                   0
    #define   LSb16MctrlDual_busyCountCh1_value                   0
    #define       bMctrlDual_busyCountCh1_value                32
    #define   MSK32MctrlDual_busyCountCh1_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_CPU0WReqCount                     0x022C

    #define     BA_MctrlDual_CPU0WReqCount_value               0x022C
    #define     B16MctrlDual_CPU0WReqCount_value               0x022C
    #define   LSb32MctrlDual_CPU0WReqCount_value                  0
    #define   LSb16MctrlDual_CPU0WReqCount_value                  0
    #define       bMctrlDual_CPU0WReqCount_value               32
    #define   MSK32MctrlDual_CPU0WReqCount_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_CPU1WReqCount                     0x0230

    #define     BA_MctrlDual_CPU1WReqCount_value               0x0230
    #define     B16MctrlDual_CPU1WReqCount_value               0x0230
    #define   LSb32MctrlDual_CPU1WReqCount_value                  0
    #define   LSb16MctrlDual_CPU1WReqCount_value                  0
    #define       bMctrlDual_CPU1WReqCount_value               32
    #define   MSK32MctrlDual_CPU1WReqCount_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_CPU0RReqCount                     0x0234

    #define     BA_MctrlDual_CPU0RReqCount_value               0x0234
    #define     B16MctrlDual_CPU0RReqCount_value               0x0234
    #define   LSb32MctrlDual_CPU0RReqCount_value                  0
    #define   LSb16MctrlDual_CPU0RReqCount_value                  0
    #define       bMctrlDual_CPU0RReqCount_value               32
    #define   MSK32MctrlDual_CPU0RReqCount_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_CPU1RReqCount                     0x0238

    #define     BA_MctrlDual_CPU1RReqCount_value               0x0238
    #define     B16MctrlDual_CPU1RReqCount_value               0x0238
    #define   LSb32MctrlDual_CPU1RReqCount_value                  0
    #define   LSb16MctrlDual_CPU1RReqCount_value                  0
    #define       bMctrlDual_CPU1RReqCount_value               32
    #define   MSK32MctrlDual_CPU1RReqCount_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port0ReqCount                     0x023C

    #define     BA_MctrlDual_Port0ReqCount_value               0x023C
    #define     B16MctrlDual_Port0ReqCount_value               0x023C
    #define   LSb32MctrlDual_Port0ReqCount_value                  0
    #define   LSb16MctrlDual_Port0ReqCount_value                  0
    #define       bMctrlDual_Port0ReqCount_value               32
    #define   MSK32MctrlDual_Port0ReqCount_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port1ReqCount                     0x0240

    #define     BA_MctrlDual_Port1ReqCount_value               0x0240
    #define     B16MctrlDual_Port1ReqCount_value               0x0240
    #define   LSb32MctrlDual_Port1ReqCount_value                  0
    #define   LSb16MctrlDual_Port1ReqCount_value                  0
    #define       bMctrlDual_Port1ReqCount_value               32
    #define   MSK32MctrlDual_Port1ReqCount_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port2ReqCount                     0x0244

    #define     BA_MctrlDual_Port2ReqCount_value               0x0244
    #define     B16MctrlDual_Port2ReqCount_value               0x0244
    #define   LSb32MctrlDual_Port2ReqCount_value                  0
    #define   LSb16MctrlDual_Port2ReqCount_value                  0
    #define       bMctrlDual_Port2ReqCount_value               32
    #define   MSK32MctrlDual_Port2ReqCount_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port3ReqCount                     0x0248

    #define     BA_MctrlDual_Port3ReqCount_value               0x0248
    #define     B16MctrlDual_Port3ReqCount_value               0x0248
    #define   LSb32MctrlDual_Port3ReqCount_value                  0
    #define   LSb16MctrlDual_Port3ReqCount_value                  0
    #define       bMctrlDual_Port3ReqCount_value               32
    #define   MSK32MctrlDual_Port3ReqCount_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port4ReqCount                     0x024C

    #define     BA_MctrlDual_Port4ReqCount_value               0x024C
    #define     B16MctrlDual_Port4ReqCount_value               0x024C
    #define   LSb32MctrlDual_Port4ReqCount_value                  0
    #define   LSb16MctrlDual_Port4ReqCount_value                  0
    #define       bMctrlDual_Port4ReqCount_value               32
    #define   MSK32MctrlDual_Port4ReqCount_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port5ReqCount                     0x0250

    #define     BA_MctrlDual_Port5ReqCount_value               0x0250
    #define     B16MctrlDual_Port5ReqCount_value               0x0250
    #define   LSb32MctrlDual_Port5ReqCount_value                  0
    #define   LSb16MctrlDual_Port5ReqCount_value                  0
    #define       bMctrlDual_Port5ReqCount_value               32
    #define   MSK32MctrlDual_Port5ReqCount_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port0FCCount                      0x0254

    #define     BA_MctrlDual_Port0FCCount_value                0x0254
    #define     B16MctrlDual_Port0FCCount_value                0x0254
    #define   LSb32MctrlDual_Port0FCCount_value                   0
    #define   LSb16MctrlDual_Port0FCCount_value                   0
    #define       bMctrlDual_Port0FCCount_value                32
    #define   MSK32MctrlDual_Port0FCCount_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port1FCCount                      0x0258

    #define     BA_MctrlDual_Port1FCCount_value                0x0258
    #define     B16MctrlDual_Port1FCCount_value                0x0258
    #define   LSb32MctrlDual_Port1FCCount_value                   0
    #define   LSb16MctrlDual_Port1FCCount_value                   0
    #define       bMctrlDual_Port1FCCount_value                32
    #define   MSK32MctrlDual_Port1FCCount_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port2FCCount                      0x025C

    #define     BA_MctrlDual_Port2FCCount_value                0x025C
    #define     B16MctrlDual_Port2FCCount_value                0x025C
    #define   LSb32MctrlDual_Port2FCCount_value                   0
    #define   LSb16MctrlDual_Port2FCCount_value                   0
    #define       bMctrlDual_Port2FCCount_value                32
    #define   MSK32MctrlDual_Port2FCCount_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port3FCCount                      0x0260

    #define     BA_MctrlDual_Port3FCCount_value                0x0260
    #define     B16MctrlDual_Port3FCCount_value                0x0260
    #define   LSb32MctrlDual_Port3FCCount_value                   0
    #define   LSb16MctrlDual_Port3FCCount_value                   0
    #define       bMctrlDual_Port3FCCount_value                32
    #define   MSK32MctrlDual_Port3FCCount_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port4FCCount                      0x0264

    #define     BA_MctrlDual_Port4FCCount_value                0x0264
    #define     B16MctrlDual_Port4FCCount_value                0x0264
    #define   LSb32MctrlDual_Port4FCCount_value                   0
    #define   LSb16MctrlDual_Port4FCCount_value                   0
    #define       bMctrlDual_Port4FCCount_value                32
    #define   MSK32MctrlDual_Port4FCCount_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Port5FCCount                      0x0268

    #define     BA_MctrlDual_Port5FCCount_value                0x0268
    #define     B16MctrlDual_Port5FCCount_value                0x0268
    #define   LSb32MctrlDual_Port5FCCount_value                   0
    #define   LSb16MctrlDual_Port5FCCount_value                   0
    #define       bMctrlDual_Port5FCCount_value                32
    #define   MSK32MctrlDual_Port5FCCount_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Cpu0WSeCount                      0x026C

    #define     BA_MctrlDual_Cpu0WSeCount_value                0x026C
    #define     B16MctrlDual_Cpu0WSeCount_value                0x026C
    #define   LSb32MctrlDual_Cpu0WSeCount_value                   0
    #define   LSb16MctrlDual_Cpu0WSeCount_value                   0
    #define       bMctrlDual_Cpu0WSeCount_value                32
    #define   MSK32MctrlDual_Cpu0WSeCount_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_Cpu0RSeCount                      0x0270

    #define     BA_MctrlDual_Cpu0RSeCount_value                0x0270
    #define     B16MctrlDual_Cpu0RSeCount_value                0x0270
    #define   LSb32MctrlDual_Cpu0RSeCount_value                   0
    #define   LSb16MctrlDual_Cpu0RSeCount_value                   0
    #define       bMctrlDual_Cpu0RSeCount_value                32
    #define   MSK32MctrlDual_Cpu0RSeCount_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_ExtMWSeCount                      0x0274

    #define     BA_MctrlDual_ExtMWSeCount_value                0x0274
    #define     B16MctrlDual_ExtMWSeCount_value                0x0274
    #define   LSb32MctrlDual_ExtMWSeCount_value                   0
    #define   LSb16MctrlDual_ExtMWSeCount_value                   0
    #define       bMctrlDual_ExtMWSeCount_value                32
    #define   MSK32MctrlDual_ExtMWSeCount_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_ExtMRSeCount                      0x0278

    #define     BA_MctrlDual_ExtMRSeCount_value                0x0278
    #define     B16MctrlDual_ExtMRSeCount_value                0x0278
    #define   LSb32MctrlDual_ExtMRSeCount_value                   0
    #define   LSb16MctrlDual_ExtMRSeCount_value                   0
    #define       bMctrlDual_ExtMRSeCount_value                32
    #define   MSK32MctrlDual_ExtMRSeCount_value                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_tRCDBlockCount0                   0x027C

    #define     BA_MctrlDual_tRCDBlockCount0_value             0x027C
    #define     B16MctrlDual_tRCDBlockCount0_value             0x027C
    #define   LSb32MctrlDual_tRCDBlockCount0_value                0
    #define   LSb16MctrlDual_tRCDBlockCount0_value                0
    #define       bMctrlDual_tRCDBlockCount0_value             32
    #define   MSK32MctrlDual_tRCDBlockCount0_value                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_ACTBlockCount0                    0x0280

    #define     BA_MctrlDual_ACTBlockCount0_value              0x0280
    #define     B16MctrlDual_ACTBlockCount0_value              0x0280
    #define   LSb32MctrlDual_ACTBlockCount0_value                 0
    #define   LSb16MctrlDual_ACTBlockCount0_value                 0
    #define       bMctrlDual_ACTBlockCount0_value              32
    #define   MSK32MctrlDual_ACTBlockCount0_value                 0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_PREBlockCount0                    0x0284

    #define     BA_MctrlDual_PREBlockCount0_value              0x0284
    #define     B16MctrlDual_PREBlockCount0_value              0x0284
    #define   LSb32MctrlDual_PREBlockCount0_value                 0
    #define   LSb16MctrlDual_PREBlockCount0_value                 0
    #define       bMctrlDual_PREBlockCount0_value              32
    #define   MSK32MctrlDual_PREBlockCount0_value                 0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_RWBlockCount0                     0x0288

    #define     BA_MctrlDual_RWBlockCount0_value               0x0288
    #define     B16MctrlDual_RWBlockCount0_value               0x0288
    #define   LSb32MctrlDual_RWBlockCount0_value                  0
    #define   LSb16MctrlDual_RWBlockCount0_value                  0
    #define       bMctrlDual_RWBlockCount0_value               32
    #define   MSK32MctrlDual_RWBlockCount0_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_REFCountCh0                       0x028C

    #define     BA_MctrlDual_REFCountCh0_value                 0x028C
    #define     B16MctrlDual_REFCountCh0_value                 0x028C
    #define   LSb32MctrlDual_REFCountCh0_value                    0
    #define   LSb16MctrlDual_REFCountCh0_value                    0
    #define       bMctrlDual_REFCountCh0_value                 32
    #define   MSK32MctrlDual_REFCountCh0_value                    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_tRCDBlockCount1                   0x0290

    #define     BA_MctrlDual_tRCDBlockCount1_value             0x0290
    #define     B16MctrlDual_tRCDBlockCount1_value             0x0290
    #define   LSb32MctrlDual_tRCDBlockCount1_value                0
    #define   LSb16MctrlDual_tRCDBlockCount1_value                0
    #define       bMctrlDual_tRCDBlockCount1_value             32
    #define   MSK32MctrlDual_tRCDBlockCount1_value                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_ACTBlockCount1                    0x0294

    #define     BA_MctrlDual_ACTBlockCount1_value              0x0294
    #define     B16MctrlDual_ACTBlockCount1_value              0x0294
    #define   LSb32MctrlDual_ACTBlockCount1_value                 0
    #define   LSb16MctrlDual_ACTBlockCount1_value                 0
    #define       bMctrlDual_ACTBlockCount1_value              32
    #define   MSK32MctrlDual_ACTBlockCount1_value                 0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_PREBlockCount1                    0x0298

    #define     BA_MctrlDual_PREBlockCount1_value              0x0298
    #define     B16MctrlDual_PREBlockCount1_value              0x0298
    #define   LSb32MctrlDual_PREBlockCount1_value                 0
    #define   LSb16MctrlDual_PREBlockCount1_value                 0
    #define       bMctrlDual_PREBlockCount1_value              32
    #define   MSK32MctrlDual_PREBlockCount1_value                 0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_RWBlockCount1                     0x029C

    #define     BA_MctrlDual_RWBlockCount1_value               0x029C
    #define     B16MctrlDual_RWBlockCount1_value               0x029C
    #define   LSb32MctrlDual_RWBlockCount1_value                  0
    #define   LSb16MctrlDual_RWBlockCount1_value                  0
    #define       bMctrlDual_RWBlockCount1_value               32
    #define   MSK32MctrlDual_RWBlockCount1_value                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_REFCountCh1                       0x02A0

    #define     BA_MctrlDual_REFCountCh1_value                 0x02A0
    #define     B16MctrlDual_REFCountCh1_value                 0x02A0
    #define   LSb32MctrlDual_REFCountCh1_value                    0
    #define   LSb16MctrlDual_REFCountCh1_value                    0
    #define       bMctrlDual_REFCountCh1_value                 32
    #define   MSK32MctrlDual_REFCountCh1_value                    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_rBufBlockCount0                   0x02A4

    #define     BA_MctrlDual_rBufBlockCount0_value             0x02A4
    #define     B16MctrlDual_rBufBlockCount0_value             0x02A4
    #define   LSb32MctrlDual_rBufBlockCount0_value                0
    #define   LSb16MctrlDual_rBufBlockCount0_value                0
    #define       bMctrlDual_rBufBlockCount0_value             32
    #define   MSK32MctrlDual_rBufBlockCount0_value                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_rBufBlockCount1                   0x02A8

    #define     BA_MctrlDual_rBufBlockCount1_value             0x02A8
    #define     B16MctrlDual_rBufBlockCount1_value             0x02A8
    #define   LSb32MctrlDual_rBufBlockCount1_value                0
    #define   LSb16MctrlDual_rBufBlockCount1_value                0
    #define       bMctrlDual_rBufBlockCount1_value             32
    #define   MSK32MctrlDual_rBufBlockCount1_value                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_rBufBlockCount2                   0x02AC

    #define     BA_MctrlDual_rBufBlockCount2_value             0x02AC
    #define     B16MctrlDual_rBufBlockCount2_value             0x02AC
    #define   LSb32MctrlDual_rBufBlockCount2_value                0
    #define   LSb16MctrlDual_rBufBlockCount2_value                0
    #define       bMctrlDual_rBufBlockCount2_value             32
    #define   MSK32MctrlDual_rBufBlockCount2_value                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_GFXCmdGnt                         0x02B0

    #define     BA_MctrlDual_GFXCmdGnt_pattern                 0x02B0
    #define     B16MctrlDual_GFXCmdGnt_pattern                 0x02B0
    #define   LSb32MctrlDual_GFXCmdGnt_pattern                    0
    #define   LSb16MctrlDual_GFXCmdGnt_pattern                    0
    #define       bMctrlDual_GFXCmdGnt_pattern                 32
    #define   MSK32MctrlDual_GFXCmdGnt_pattern                    0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_SeCtrlCpu0WMem                    0x02B4
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_SeCtrlCpu0RMem                    0x02D4
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_SeCtrlExtMWMem                    0x02F4
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_SeCtrlExtMRMem                    0x0314
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackExpLow0                   0x0334

    #define     BA_MctrlDual_loopBackExpLow0_value             0x0334
    #define     B16MctrlDual_loopBackExpLow0_value             0x0334
    #define   LSb32MctrlDual_loopBackExpLow0_value                0
    #define   LSb16MctrlDual_loopBackExpLow0_value                0
    #define       bMctrlDual_loopBackExpLow0_value             32
    #define   MSK32MctrlDual_loopBackExpLow0_value                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackExpHigh0                  0x0338

    #define     BA_MctrlDual_loopBackExpHigh0_value            0x0338
    #define     B16MctrlDual_loopBackExpHigh0_value            0x0338
    #define   LSb32MctrlDual_loopBackExpHigh0_value               0
    #define   LSb16MctrlDual_loopBackExpHigh0_value               0
    #define       bMctrlDual_loopBackExpHigh0_value            32
    #define   MSK32MctrlDual_loopBackExpHigh0_value               0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackRcvLow0                   0x033C

    #define     BA_MctrlDual_loopBackRcvLow0_value             0x033C
    #define     B16MctrlDual_loopBackRcvLow0_value             0x033C
    #define   LSb32MctrlDual_loopBackRcvLow0_value                0
    #define   LSb16MctrlDual_loopBackRcvLow0_value                0
    #define       bMctrlDual_loopBackRcvLow0_value             32
    #define   MSK32MctrlDual_loopBackRcvLow0_value                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackRcvHigh0                  0x0340

    #define     BA_MctrlDual_loopBackRcvHigh0_value            0x0340
    #define     B16MctrlDual_loopBackRcvHigh0_value            0x0340
    #define   LSb32MctrlDual_loopBackRcvHigh0_value               0
    #define   LSb16MctrlDual_loopBackRcvHigh0_value               0
    #define       bMctrlDual_loopBackRcvHigh0_value            32
    #define   MSK32MctrlDual_loopBackRcvHigh0_value               0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackDQM0                      0x0344

    #define     BA_MctrlDual_loopBackDQM0_dqm_exp              0x0344
    #define     B16MctrlDual_loopBackDQM0_dqm_exp              0x0344
    #define   LSb32MctrlDual_loopBackDQM0_dqm_exp                 0
    #define   LSb16MctrlDual_loopBackDQM0_dqm_exp                 0
    #define       bMctrlDual_loopBackDQM0_dqm_exp              8
    #define   MSK32MctrlDual_loopBackDQM0_dqm_exp                 0x000000FF

    #define     BA_MctrlDual_loopBackDQM0_dqm_rcv              0x0345
    #define     B16MctrlDual_loopBackDQM0_dqm_rcv              0x0344
    #define   LSb32MctrlDual_loopBackDQM0_dqm_rcv                 8
    #define   LSb16MctrlDual_loopBackDQM0_dqm_rcv                 8
    #define       bMctrlDual_loopBackDQM0_dqm_rcv              8
    #define   MSK32MctrlDual_loopBackDQM0_dqm_rcv                 0x0000FF00
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackExpAC0                    0x0348

    #define     BA_MctrlDual_loopBackExpAC0_value              0x0348
    #define     B16MctrlDual_loopBackExpAC0_value              0x0348
    #define   LSb32MctrlDual_loopBackExpAC0_value                 0
    #define   LSb16MctrlDual_loopBackExpAC0_value                 0
    #define       bMctrlDual_loopBackExpAC0_value              23
    #define   MSK32MctrlDual_loopBackExpAC0_value                 0x007FFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackRcvAC0                    0x034C

    #define     BA_MctrlDual_loopBackRcvAC0_value              0x034C
    #define     B16MctrlDual_loopBackRcvAC0_value              0x034C
    #define   LSb32MctrlDual_loopBackRcvAC0_value                 0
    #define   LSb16MctrlDual_loopBackRcvAC0_value                 0
    #define       bMctrlDual_loopBackRcvAC0_value              23
    #define   MSK32MctrlDual_loopBackRcvAC0_value                 0x007FFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackExpLow1                   0x0350

    #define     BA_MctrlDual_loopBackExpLow1_value             0x0350
    #define     B16MctrlDual_loopBackExpLow1_value             0x0350
    #define   LSb32MctrlDual_loopBackExpLow1_value                0
    #define   LSb16MctrlDual_loopBackExpLow1_value                0
    #define       bMctrlDual_loopBackExpLow1_value             32
    #define   MSK32MctrlDual_loopBackExpLow1_value                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackExpHigh1                  0x0354

    #define     BA_MctrlDual_loopBackExpHigh1_value            0x0354
    #define     B16MctrlDual_loopBackExpHigh1_value            0x0354
    #define   LSb32MctrlDual_loopBackExpHigh1_value               0
    #define   LSb16MctrlDual_loopBackExpHigh1_value               0
    #define       bMctrlDual_loopBackExpHigh1_value            32
    #define   MSK32MctrlDual_loopBackExpHigh1_value               0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackRcvLow1                   0x0358

    #define     BA_MctrlDual_loopBackRcvLow1_value             0x0358
    #define     B16MctrlDual_loopBackRcvLow1_value             0x0358
    #define   LSb32MctrlDual_loopBackRcvLow1_value                0
    #define   LSb16MctrlDual_loopBackRcvLow1_value                0
    #define       bMctrlDual_loopBackRcvLow1_value             32
    #define   MSK32MctrlDual_loopBackRcvLow1_value                0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackRcvHigh1                  0x035C

    #define     BA_MctrlDual_loopBackRcvHigh1_value            0x035C
    #define     B16MctrlDual_loopBackRcvHigh1_value            0x035C
    #define   LSb32MctrlDual_loopBackRcvHigh1_value               0
    #define   LSb16MctrlDual_loopBackRcvHigh1_value               0
    #define       bMctrlDual_loopBackRcvHigh1_value            32
    #define   MSK32MctrlDual_loopBackRcvHigh1_value               0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackDQM1                      0x0360

    #define     BA_MctrlDual_loopBackDQM1_dqm_exp              0x0360
    #define     B16MctrlDual_loopBackDQM1_dqm_exp              0x0360
    #define   LSb32MctrlDual_loopBackDQM1_dqm_exp                 0
    #define   LSb16MctrlDual_loopBackDQM1_dqm_exp                 0
    #define       bMctrlDual_loopBackDQM1_dqm_exp              8
    #define   MSK32MctrlDual_loopBackDQM1_dqm_exp                 0x000000FF

    #define     BA_MctrlDual_loopBackDQM1_dqm_rcv              0x0361
    #define     B16MctrlDual_loopBackDQM1_dqm_rcv              0x0360
    #define   LSb32MctrlDual_loopBackDQM1_dqm_rcv                 8
    #define   LSb16MctrlDual_loopBackDQM1_dqm_rcv                 8
    #define       bMctrlDual_loopBackDQM1_dqm_rcv              8
    #define   MSK32MctrlDual_loopBackDQM1_dqm_rcv                 0x0000FF00
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackExpAC1                    0x0364

    #define     BA_MctrlDual_loopBackExpAC1_value              0x0364
    #define     B16MctrlDual_loopBackExpAC1_value              0x0364
    #define   LSb32MctrlDual_loopBackExpAC1_value                 0
    #define   LSb16MctrlDual_loopBackExpAC1_value                 0
    #define       bMctrlDual_loopBackExpAC1_value              23
    #define   MSK32MctrlDual_loopBackExpAC1_value                 0x007FFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_MctrlDual_loopBackRcvAC1                    0x0368

    #define     BA_MctrlDual_loopBackRcvAC1_value              0x0368
    #define     B16MctrlDual_loopBackRcvAC1_value              0x0368
    #define   LSb32MctrlDual_loopBackRcvAC1_value                 0
    #define   LSb16MctrlDual_loopBackRcvAC1_value                 0
    #define       bMctrlDual_loopBackRcvAC1_value              23
    #define   MSK32MctrlDual_loopBackRcvAC1_value                 0x007FFFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_MctrlDual {
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_CommConfig_dual_channel(r32)      _BFGET_(r32, 0, 0)
    #define   SET32MctrlDual_CommConfig_dual_channel(r32,v)    _BFSET_(r32, 0, 0,v)
    #define   GET16MctrlDual_CommConfig_dual_channel(r16)      _BFGET_(r16, 0, 0)
    #define   SET16MctrlDual_CommConfig_dual_channel(r16,v)    _BFSET_(r16, 0, 0,v)

    #define   GET32MctrlDual_CommConfig_asymAddrMode(r32)      _BFGET_(r32, 2, 1)
    #define   SET32MctrlDual_CommConfig_asymAddrMode(r32,v)    _BFSET_(r32, 2, 1,v)
    #define   GET16MctrlDual_CommConfig_asymAddrMode(r16)      _BFGET_(r16, 2, 1)
    #define   SET16MctrlDual_CommConfig_asymAddrMode(r16,v)    _BFSET_(r16, 2, 1,v)

    #define   GET32MctrlDual_CommConfig_asymAddrSel(r32)       _BFGET_(r32, 4, 3)
    #define   SET32MctrlDual_CommConfig_asymAddrSel(r32,v)     _BFSET_(r32, 4, 3,v)
    #define   GET16MctrlDual_CommConfig_asymAddrSel(r16)       _BFGET_(r16, 4, 3)
    #define   SET16MctrlDual_CommConfig_asymAddrSel(r16,v)     _BFSET_(r16, 4, 3,v)

    #define   GET32MctrlDual_CommConfig_al_sel(r32)            _BFGET_(r32, 7, 5)
    #define   SET32MctrlDual_CommConfig_al_sel(r32,v)          _BFSET_(r32, 7, 5,v)
    #define   GET16MctrlDual_CommConfig_al_sel(r16)            _BFGET_(r16, 7, 5)
    #define   SET16MctrlDual_CommConfig_al_sel(r16,v)          _BFSET_(r16, 7, 5,v)

    #define   GET32MctrlDual_CommConfig_cl_sel(r32)            _BFGET_(r32,11, 8)
    #define   SET32MctrlDual_CommConfig_cl_sel(r32,v)          _BFSET_(r32,11, 8,v)
    #define   GET16MctrlDual_CommConfig_cl_sel(r16)            _BFGET_(r16,11, 8)
    #define   SET16MctrlDual_CommConfig_cl_sel(r16,v)          _BFSET_(r16,11, 8,v)

    #define   GET32MctrlDual_CommConfig_bank_mode(r32)         _BFGET_(r32,12,12)
    #define   SET32MctrlDual_CommConfig_bank_mode(r32,v)       _BFSET_(r32,12,12,v)
    #define   GET16MctrlDual_CommConfig_bank_mode(r16)         _BFGET_(r16,12,12)
    #define   SET16MctrlDual_CommConfig_bank_mode(r16,v)       _BFSET_(r16,12,12,v)

    #define   GET32MctrlDual_CommConfig_mem_type(r32)          _BFGET_(r32,15,13)
    #define   SET32MctrlDual_CommConfig_mem_type(r32,v)        _BFSET_(r32,15,13,v)
    #define   GET16MctrlDual_CommConfig_mem_type(r16)          _BFGET_(r16,15,13)
    #define   SET16MctrlDual_CommConfig_mem_type(r16,v)        _BFSET_(r16,15,13,v)

    #define   GET32MctrlDual_CommConfig_data_width(r32)        _BFGET_(r32,17,16)
    #define   SET32MctrlDual_CommConfig_data_width(r32,v)      _BFSET_(r32,17,16,v)
    #define   GET16MctrlDual_CommConfig_data_width(r16)        _BFGET_(r16, 1, 0)
    #define   SET16MctrlDual_CommConfig_data_width(r16,v)      _BFSET_(r16, 1, 0,v)

    #define     w32MctrlDual_CommConfig                        {\
            UNSG32 uCommConfig_dual_channel                    :  1;\
            UNSG32 uCommConfig_asymAddrMode                    :  2;\
            UNSG32 uCommConfig_asymAddrSel                     :  2;\
            UNSG32 uCommConfig_al_sel                          :  3;\
            UNSG32 uCommConfig_cl_sel                          :  4;\
            UNSG32 uCommConfig_bank_mode                       :  1;\
            UNSG32 uCommConfig_mem_type                        :  3;\
            UNSG32 uCommConfig_data_width                      :  2;\
            UNSG32 RSVDx0_b18                                  : 14;\
          }
    union { UNSG32 u32MctrlDual_CommConfig;
            struct w32MctrlDual_CommConfig;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Trigger_sdram0_init_req(r32)      _BFGET_(r32, 0, 0)
    #define   SET32MctrlDual_Trigger_sdram0_init_req(r32,v)    _BFSET_(r32, 0, 0,v)
    #define   GET16MctrlDual_Trigger_sdram0_init_req(r16)      _BFGET_(r16, 0, 0)
    #define   SET16MctrlDual_Trigger_sdram0_init_req(r16,v)    _BFSET_(r16, 0, 0,v)

    #define   GET32MctrlDual_Trigger_sdram1_init_req(r32)      _BFGET_(r32, 1, 1)
    #define   SET32MctrlDual_Trigger_sdram1_init_req(r32,v)    _BFSET_(r32, 1, 1,v)
    #define   GET16MctrlDual_Trigger_sdram1_init_req(r16)      _BFGET_(r16, 1, 1)
    #define   SET16MctrlDual_Trigger_sdram1_init_req(r16,v)    _BFSET_(r16, 1, 1,v)

    #define   GET32MctrlDual_Trigger_pwrsav_exitReq_ch0(r32)   _BFGET_(r32, 2, 2)
    #define   SET32MctrlDual_Trigger_pwrsav_exitReq_ch0(r32,v) _BFSET_(r32, 2, 2,v)
    #define   GET16MctrlDual_Trigger_pwrsav_exitReq_ch0(r16)   _BFGET_(r16, 2, 2)
    #define   SET16MctrlDual_Trigger_pwrsav_exitReq_ch0(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32MctrlDual_Trigger_pwrsav_exitReq_ch1(r32)   _BFGET_(r32, 3, 3)
    #define   SET32MctrlDual_Trigger_pwrsav_exitReq_ch1(r32,v) _BFSET_(r32, 3, 3,v)
    #define   GET16MctrlDual_Trigger_pwrsav_exitReq_ch1(r16)   _BFGET_(r16, 3, 3)
    #define   SET16MctrlDual_Trigger_pwrsav_exitReq_ch1(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32MctrlDual_Trigger_pwrsav_actReq_ch0(r32)    _BFGET_(r32, 4, 4)
    #define   SET32MctrlDual_Trigger_pwrsav_actReq_ch0(r32,v)  _BFSET_(r32, 4, 4,v)
    #define   GET16MctrlDual_Trigger_pwrsav_actReq_ch0(r16)    _BFGET_(r16, 4, 4)
    #define   SET16MctrlDual_Trigger_pwrsav_actReq_ch0(r16,v)  _BFSET_(r16, 4, 4,v)

    #define   GET32MctrlDual_Trigger_pwrsav_actReq_ch1(r32)    _BFGET_(r32, 5, 5)
    #define   SET32MctrlDual_Trigger_pwrsav_actReq_ch1(r32,v)  _BFSET_(r32, 5, 5,v)
    #define   GET16MctrlDual_Trigger_pwrsav_actReq_ch1(r16)    _BFGET_(r16, 5, 5)
    #define   SET16MctrlDual_Trigger_pwrsav_actReq_ch1(r16,v)  _BFSET_(r16, 5, 5,v)

    #define   GET32MctrlDual_Trigger_auto_pwrsav_ch0(r32)      _BFGET_(r32, 6, 6)
    #define   SET32MctrlDual_Trigger_auto_pwrsav_ch0(r32,v)    _BFSET_(r32, 6, 6,v)
    #define   GET16MctrlDual_Trigger_auto_pwrsav_ch0(r16)      _BFGET_(r16, 6, 6)
    #define   SET16MctrlDual_Trigger_auto_pwrsav_ch0(r16,v)    _BFSET_(r16, 6, 6,v)

    #define   GET32MctrlDual_Trigger_auto_pwrsav_ch1(r32)      _BFGET_(r32, 7, 7)
    #define   SET32MctrlDual_Trigger_auto_pwrsav_ch1(r32,v)    _BFSET_(r32, 7, 7,v)
    #define   GET16MctrlDual_Trigger_auto_pwrsav_ch1(r16)      _BFGET_(r16, 7, 7)
    #define   SET16MctrlDual_Trigger_auto_pwrsav_ch1(r16,v)    _BFSET_(r16, 7, 7,v)

    #define   GET32MctrlDual_Trigger_wBufferFlushCh0(r32)      _BFGET_(r32, 8, 8)
    #define   SET32MctrlDual_Trigger_wBufferFlushCh0(r32,v)    _BFSET_(r32, 8, 8,v)
    #define   GET16MctrlDual_Trigger_wBufferFlushCh0(r16)      _BFGET_(r16, 8, 8)
    #define   SET16MctrlDual_Trigger_wBufferFlushCh0(r16,v)    _BFSET_(r16, 8, 8,v)

    #define   GET32MctrlDual_Trigger_wBufferFlushCh1(r32)      _BFGET_(r32, 9, 9)
    #define   SET32MctrlDual_Trigger_wBufferFlushCh1(r32,v)    _BFSET_(r32, 9, 9,v)
    #define   GET16MctrlDual_Trigger_wBufferFlushCh1(r16)      _BFGET_(r16, 9, 9)
    #define   SET16MctrlDual_Trigger_wBufferFlushCh1(r16,v)    _BFSET_(r16, 9, 9,v)

    #define   GET32MctrlDual_Trigger_cal_req(r32)              _BFGET_(r32,10,10)
    #define   SET32MctrlDual_Trigger_cal_req(r32,v)            _BFSET_(r32,10,10,v)
    #define   GET16MctrlDual_Trigger_cal_req(r16)              _BFGET_(r16,10,10)
    #define   SET16MctrlDual_Trigger_cal_req(r16,v)            _BFSET_(r16,10,10,v)

    #define   GET32MctrlDual_Trigger_ttbFlush(r32)             _BFGET_(r32,11,11)
    #define   SET32MctrlDual_Trigger_ttbFlush(r32,v)           _BFSET_(r32,11,11,v)
    #define   GET16MctrlDual_Trigger_ttbFlush(r16)             _BFGET_(r16,11,11)
    #define   SET16MctrlDual_Trigger_ttbFlush(r16,v)           _BFSET_(r16,11,11,v)

    #define   GET32MctrlDual_Trigger_loopBackTestCh0(r32)      _BFGET_(r32,12,12)
    #define   SET32MctrlDual_Trigger_loopBackTestCh0(r32,v)    _BFSET_(r32,12,12,v)
    #define   GET16MctrlDual_Trigger_loopBackTestCh0(r16)      _BFGET_(r16,12,12)
    #define   SET16MctrlDual_Trigger_loopBackTestCh0(r16,v)    _BFSET_(r16,12,12,v)

    #define   GET32MctrlDual_Trigger_loopBackTestCh1(r32)      _BFGET_(r32,13,13)
    #define   SET32MctrlDual_Trigger_loopBackTestCh1(r32,v)    _BFSET_(r32,13,13,v)
    #define   GET16MctrlDual_Trigger_loopBackTestCh1(r16)      _BFGET_(r16,13,13)
    #define   SET16MctrlDual_Trigger_loopBackTestCh1(r16,v)    _BFSET_(r16,13,13,v)

    #define     w32MctrlDual_Trigger                           {\
            UNSG32 uTrigger_sdram0_init_req                    :  1;\
            UNSG32 uTrigger_sdram1_init_req                    :  1;\
            UNSG32 uTrigger_pwrsav_exitReq_ch0                 :  1;\
            UNSG32 uTrigger_pwrsav_exitReq_ch1                 :  1;\
            UNSG32 uTrigger_pwrsav_actReq_ch0                  :  1;\
            UNSG32 uTrigger_pwrsav_actReq_ch1                  :  1;\
            UNSG32 uTrigger_auto_pwrsav_ch0                    :  1;\
            UNSG32 uTrigger_auto_pwrsav_ch1                    :  1;\
            UNSG32 uTrigger_wBufferFlushCh0                    :  1;\
            UNSG32 uTrigger_wBufferFlushCh1                    :  1;\
            UNSG32 uTrigger_cal_req                            :  1;\
            UNSG32 uTrigger_ttbFlush                           :  1;\
            UNSG32 uTrigger_loopBackTestCh0                    :  1;\
            UNSG32 uTrigger_loopBackTestCh1                    :  1;\
            UNSG32 RSVDx4_b14                                  : 18;\
          }
    union { UNSG32 u32MctrlDual_Trigger;
            struct w32MctrlDual_Trigger;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Status_sdram0_init_done(r32)      _BFGET_(r32, 0, 0)
    #define   SET32MctrlDual_Status_sdram0_init_done(r32,v)    _BFSET_(r32, 0, 0,v)
    #define   GET16MctrlDual_Status_sdram0_init_done(r16)      _BFGET_(r16, 0, 0)
    #define   SET16MctrlDual_Status_sdram0_init_done(r16,v)    _BFSET_(r16, 0, 0,v)

    #define   GET32MctrlDual_Status_sdram1_init_done(r32)      _BFGET_(r32, 1, 1)
    #define   SET32MctrlDual_Status_sdram1_init_done(r32,v)    _BFSET_(r32, 1, 1,v)
    #define   GET16MctrlDual_Status_sdram1_init_done(r16)      _BFGET_(r16, 1, 1)
    #define   SET16MctrlDual_Status_sdram1_init_done(r16,v)    _BFSET_(r16, 1, 1,v)

    #define   GET32MctrlDual_Status_pwrsav_exitDone_ch0(r32)   _BFGET_(r32, 2, 2)
    #define   SET32MctrlDual_Status_pwrsav_exitDone_ch0(r32,v) _BFSET_(r32, 2, 2,v)
    #define   GET16MctrlDual_Status_pwrsav_exitDone_ch0(r16)   _BFGET_(r16, 2, 2)
    #define   SET16MctrlDual_Status_pwrsav_exitDone_ch0(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32MctrlDual_Status_pwrsav_exitDone_ch1(r32)   _BFGET_(r32, 3, 3)
    #define   SET32MctrlDual_Status_pwrsav_exitDone_ch1(r32,v) _BFSET_(r32, 3, 3,v)
    #define   GET16MctrlDual_Status_pwrsav_exitDone_ch1(r16)   _BFGET_(r16, 3, 3)
    #define   SET16MctrlDual_Status_pwrsav_exitDone_ch1(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32MctrlDual_Status_pwrsav_actDone_ch0(r32)    _BFGET_(r32, 4, 4)
    #define   SET32MctrlDual_Status_pwrsav_actDone_ch0(r32,v)  _BFSET_(r32, 4, 4,v)
    #define   GET16MctrlDual_Status_pwrsav_actDone_ch0(r16)    _BFGET_(r16, 4, 4)
    #define   SET16MctrlDual_Status_pwrsav_actDone_ch0(r16,v)  _BFSET_(r16, 4, 4,v)

    #define   GET32MctrlDual_Status_pwrsav_actDone_ch1(r32)    _BFGET_(r32, 5, 5)
    #define   SET32MctrlDual_Status_pwrsav_actDone_ch1(r32,v)  _BFSET_(r32, 5, 5,v)
    #define   GET16MctrlDual_Status_pwrsav_actDone_ch1(r16)    _BFGET_(r16, 5, 5)
    #define   SET16MctrlDual_Status_pwrsav_actDone_ch1(r16,v)  _BFSET_(r16, 5, 5,v)

    #define   GET32MctrlDual_Status_aps_on_ch0(r32)            _BFGET_(r32, 6, 6)
    #define   SET32MctrlDual_Status_aps_on_ch0(r32,v)          _BFSET_(r32, 6, 6,v)
    #define   GET16MctrlDual_Status_aps_on_ch0(r16)            _BFGET_(r16, 6, 6)
    #define   SET16MctrlDual_Status_aps_on_ch0(r16,v)          _BFSET_(r16, 6, 6,v)

    #define   GET32MctrlDual_Status_aps_on_ch1(r32)            _BFGET_(r32, 7, 7)
    #define   SET32MctrlDual_Status_aps_on_ch1(r32,v)          _BFSET_(r32, 7, 7,v)
    #define   GET16MctrlDual_Status_aps_on_ch1(r16)            _BFGET_(r16, 7, 7)
    #define   SET16MctrlDual_Status_aps_on_ch1(r16,v)          _BFSET_(r16, 7, 7,v)

    #define   GET32MctrlDual_Status_wBufferEmptyCh0(r32)       _BFGET_(r32, 8, 8)
    #define   SET32MctrlDual_Status_wBufferEmptyCh0(r32,v)     _BFSET_(r32, 8, 8,v)
    #define   GET16MctrlDual_Status_wBufferEmptyCh0(r16)       _BFGET_(r16, 8, 8)
    #define   SET16MctrlDual_Status_wBufferEmptyCh0(r16,v)     _BFSET_(r16, 8, 8,v)

    #define   GET32MctrlDual_Status_wBufferEmptyCh1(r32)       _BFGET_(r32, 9, 9)
    #define   SET32MctrlDual_Status_wBufferEmptyCh1(r32,v)     _BFSET_(r32, 9, 9,v)
    #define   GET16MctrlDual_Status_wBufferEmptyCh1(r16)       _BFGET_(r16, 9, 9)
    #define   SET16MctrlDual_Status_wBufferEmptyCh1(r16,v)     _BFSET_(r16, 9, 9,v)

    #define   GET32MctrlDual_Status_inq0_empty(r32)            _BFGET_(r32,10,10)
    #define   SET32MctrlDual_Status_inq0_empty(r32,v)          _BFSET_(r32,10,10,v)
    #define   GET16MctrlDual_Status_inq0_empty(r16)            _BFGET_(r16,10,10)
    #define   SET16MctrlDual_Status_inq0_empty(r16,v)          _BFSET_(r16,10,10,v)

    #define   GET32MctrlDual_Status_inq0_full(r32)             _BFGET_(r32,11,11)
    #define   SET32MctrlDual_Status_inq0_full(r32,v)           _BFSET_(r32,11,11,v)
    #define   GET16MctrlDual_Status_inq0_full(r16)             _BFGET_(r16,11,11)
    #define   SET16MctrlDual_Status_inq0_full(r16,v)           _BFSET_(r16,11,11,v)

    #define   GET32MctrlDual_Status_inq1_empty(r32)            _BFGET_(r32,12,12)
    #define   SET32MctrlDual_Status_inq1_empty(r32,v)          _BFSET_(r32,12,12,v)
    #define   GET16MctrlDual_Status_inq1_empty(r16)            _BFGET_(r16,12,12)
    #define   SET16MctrlDual_Status_inq1_empty(r16,v)          _BFSET_(r16,12,12,v)

    #define   GET32MctrlDual_Status_inq1_full(r32)             _BFGET_(r32,13,13)
    #define   SET32MctrlDual_Status_inq1_full(r32,v)           _BFSET_(r32,13,13,v)
    #define   GET16MctrlDual_Status_inq1_full(r16)             _BFGET_(r16,13,13)
    #define   SET16MctrlDual_Status_inq1_full(r16,v)           _BFSET_(r16,13,13,v)

    #define   GET32MctrlDual_Status_ttbEmpty(r32)              _BFGET_(r32,14,14)
    #define   SET32MctrlDual_Status_ttbEmpty(r32,v)            _BFSET_(r32,14,14,v)
    #define   GET16MctrlDual_Status_ttbEmpty(r16)              _BFGET_(r16,14,14)
    #define   SET16MctrlDual_Status_ttbEmpty(r16,v)            _BFSET_(r16,14,14,v)

    #define   GET32MctrlDual_Status_loopBackPassCh0(r32)       _BFGET_(r32,15,15)
    #define   SET32MctrlDual_Status_loopBackPassCh0(r32,v)     _BFSET_(r32,15,15,v)
    #define   GET16MctrlDual_Status_loopBackPassCh0(r16)       _BFGET_(r16,15,15)
    #define   SET16MctrlDual_Status_loopBackPassCh0(r16,v)     _BFSET_(r16,15,15,v)

    #define   GET32MctrlDual_Status_loopBackDoneCh0(r32)       _BFGET_(r32,16,16)
    #define   SET32MctrlDual_Status_loopBackDoneCh0(r32,v)     _BFSET_(r32,16,16,v)
    #define   GET16MctrlDual_Status_loopBackDoneCh0(r16)       _BFGET_(r16, 0, 0)
    #define   SET16MctrlDual_Status_loopBackDoneCh0(r16,v)     _BFSET_(r16, 0, 0,v)

    #define   GET32MctrlDual_Status_loopBackPassCh1(r32)       _BFGET_(r32,17,17)
    #define   SET32MctrlDual_Status_loopBackPassCh1(r32,v)     _BFSET_(r32,17,17,v)
    #define   GET16MctrlDual_Status_loopBackPassCh1(r16)       _BFGET_(r16, 1, 1)
    #define   SET16MctrlDual_Status_loopBackPassCh1(r16,v)     _BFSET_(r16, 1, 1,v)

    #define   GET32MctrlDual_Status_loopBackDoneCh1(r32)       _BFGET_(r32,18,18)
    #define   SET32MctrlDual_Status_loopBackDoneCh1(r32,v)     _BFSET_(r32,18,18,v)
    #define   GET16MctrlDual_Status_loopBackDoneCh1(r16)       _BFGET_(r16, 2, 2)
    #define   SET16MctrlDual_Status_loopBackDoneCh1(r16,v)     _BFSET_(r16, 2, 2,v)

    #define     w32MctrlDual_Status                            {\
            UNSG32 uStatus_sdram0_init_done                    :  1;\
            UNSG32 uStatus_sdram1_init_done                    :  1;\
            UNSG32 uStatus_pwrsav_exitDone_ch0                 :  1;\
            UNSG32 uStatus_pwrsav_exitDone_ch1                 :  1;\
            UNSG32 uStatus_pwrsav_actDone_ch0                  :  1;\
            UNSG32 uStatus_pwrsav_actDone_ch1                  :  1;\
            UNSG32 uStatus_aps_on_ch0                          :  1;\
            UNSG32 uStatus_aps_on_ch1                          :  1;\
            UNSG32 uStatus_wBufferEmptyCh0                     :  1;\
            UNSG32 uStatus_wBufferEmptyCh1                     :  1;\
            UNSG32 uStatus_inq0_empty                          :  1;\
            UNSG32 uStatus_inq0_full                           :  1;\
            UNSG32 uStatus_inq1_empty                          :  1;\
            UNSG32 uStatus_inq1_full                           :  1;\
            UNSG32 uStatus_ttbEmpty                            :  1;\
            UNSG32 uStatus_loopBackPassCh0                     :  1;\
            UNSG32 uStatus_loopBackDoneCh0                     :  1;\
            UNSG32 uStatus_loopBackPassCh1                     :  1;\
            UNSG32 uStatus_loopBackDoneCh1                     :  1;\
            UNSG32 RSVDx8_b19                                  : 13;\
          }
    union { UNSG32 u32MctrlDual_Status;
            struct w32MctrlDual_Status;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_SettingChannel0_noColumnAddrCh0(r32) _BFGET_(r32, 3, 0)
    #define   SET32MctrlDual_SettingChannel0_noColumnAddrCh0(r32,v) _BFSET_(r32, 3, 0,v)
    #define   GET16MctrlDual_SettingChannel0_noColumnAddrCh0(r16) _BFGET_(r16, 3, 0)
    #define   SET16MctrlDual_SettingChannel0_noColumnAddrCh0(r16,v) _BFSET_(r16, 3, 0,v)

    #define   GET32MctrlDual_SettingChannel0_noRowAddrCh0(r32) _BFGET_(r32, 7, 4)
    #define   SET32MctrlDual_SettingChannel0_noRowAddrCh0(r32,v) _BFSET_(r32, 7, 4,v)
    #define   GET16MctrlDual_SettingChannel0_noRowAddrCh0(r16) _BFGET_(r16, 7, 4)
    #define   SET16MctrlDual_SettingChannel0_noRowAddrCh0(r16,v) _BFSET_(r16, 7, 4,v)

    #define   GET32MctrlDual_SettingChannel0_bankSelCh0(r32)   _BFGET_(r32, 8, 8)
    #define   SET32MctrlDual_SettingChannel0_bankSelCh0(r32,v) _BFSET_(r32, 8, 8,v)
    #define   GET16MctrlDual_SettingChannel0_bankSelCh0(r16)   _BFGET_(r16, 8, 8)
    #define   SET16MctrlDual_SettingChannel0_bankSelCh0(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32MctrlDual_SettingChannel0_noChipSelectCh0(r32) _BFGET_(r32,10, 9)
    #define   SET32MctrlDual_SettingChannel0_noChipSelectCh0(r32,v) _BFSET_(r32,10, 9,v)
    #define   GET16MctrlDual_SettingChannel0_noChipSelectCh0(r16) _BFGET_(r16,10, 9)
    #define   SET16MctrlDual_SettingChannel0_noChipSelectCh0(r16,v) _BFSET_(r16,10, 9,v)

    #define     w32MctrlDual_SettingChannel0                   {\
            UNSG32 uSettingChannel0_noColumnAddrCh0            :  4;\
            UNSG32 uSettingChannel0_noRowAddrCh0               :  4;\
            UNSG32 uSettingChannel0_bankSelCh0                 :  1;\
            UNSG32 uSettingChannel0_noChipSelectCh0            :  2;\
            UNSG32 RSVDxC_b11                                  : 21;\
          }
    union { UNSG32 u32MctrlDual_SettingChannel0;
            struct w32MctrlDual_SettingChannel0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_SettingChannel1_noColumnAddrCh1(r32) _BFGET_(r32, 3, 0)
    #define   SET32MctrlDual_SettingChannel1_noColumnAddrCh1(r32,v) _BFSET_(r32, 3, 0,v)
    #define   GET16MctrlDual_SettingChannel1_noColumnAddrCh1(r16) _BFGET_(r16, 3, 0)
    #define   SET16MctrlDual_SettingChannel1_noColumnAddrCh1(r16,v) _BFSET_(r16, 3, 0,v)

    #define   GET32MctrlDual_SettingChannel1_noRowAddrCh1(r32) _BFGET_(r32, 7, 4)
    #define   SET32MctrlDual_SettingChannel1_noRowAddrCh1(r32,v) _BFSET_(r32, 7, 4,v)
    #define   GET16MctrlDual_SettingChannel1_noRowAddrCh1(r16) _BFGET_(r16, 7, 4)
    #define   SET16MctrlDual_SettingChannel1_noRowAddrCh1(r16,v) _BFSET_(r16, 7, 4,v)

    #define   GET32MctrlDual_SettingChannel1_bankSelCh1(r32)   _BFGET_(r32, 8, 8)
    #define   SET32MctrlDual_SettingChannel1_bankSelCh1(r32,v) _BFSET_(r32, 8, 8,v)
    #define   GET16MctrlDual_SettingChannel1_bankSelCh1(r16)   _BFGET_(r16, 8, 8)
    #define   SET16MctrlDual_SettingChannel1_bankSelCh1(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32MctrlDual_SettingChannel1_noChipSelectCh1(r32) _BFGET_(r32,10, 9)
    #define   SET32MctrlDual_SettingChannel1_noChipSelectCh1(r32,v) _BFSET_(r32,10, 9,v)
    #define   GET16MctrlDual_SettingChannel1_noChipSelectCh1(r16) _BFGET_(r16,10, 9)
    #define   SET16MctrlDual_SettingChannel1_noChipSelectCh1(r16,v) _BFSET_(r16,10, 9,v)

    #define     w32MctrlDual_SettingChannel1                   {\
            UNSG32 uSettingChannel1_noColumnAddrCh1            :  4;\
            UNSG32 uSettingChannel1_noRowAddrCh1               :  4;\
            UNSG32 uSettingChannel1_bankSelCh1                 :  1;\
            UNSG32 uSettingChannel1_noChipSelectCh1            :  2;\
            UNSG32 RSVDx10_b11                                 : 21;\
          }
    union { UNSG32 u32MctrlDual_SettingChannel1;
            struct w32MctrlDual_SettingChannel1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Timing1_init_tras(r32)            _BFGET_(r32, 7, 0)
    #define   SET32MctrlDual_Timing1_init_tras(r32,v)          _BFSET_(r32, 7, 0,v)
    #define   GET16MctrlDual_Timing1_init_tras(r16)            _BFGET_(r16, 7, 0)
    #define   SET16MctrlDual_Timing1_init_tras(r16,v)          _BFSET_(r16, 7, 0,v)

    #define   GET32MctrlDual_Timing1_init_trfc(r32)            _BFGET_(r32,15, 8)
    #define   SET32MctrlDual_Timing1_init_trfc(r32,v)          _BFSET_(r32,15, 8,v)
    #define   GET16MctrlDual_Timing1_init_trfc(r16)            _BFGET_(r16,15, 8)
    #define   SET16MctrlDual_Timing1_init_trfc(r16,v)          _BFSET_(r16,15, 8,v)

    #define   GET32MctrlDual_Timing1_init_trc(r32)             _BFGET_(r32,23,16)
    #define   SET32MctrlDual_Timing1_init_trc(r32,v)           _BFSET_(r32,23,16,v)
    #define   GET16MctrlDual_Timing1_init_trc(r16)             _BFGET_(r16, 7, 0)
    #define   SET16MctrlDual_Timing1_init_trc(r16,v)           _BFSET_(r16, 7, 0,v)

    #define   GET32MctrlDual_Timing1_init_refc(r32)            _BFGET_(r32,31,24)
    #define   SET32MctrlDual_Timing1_init_refc(r32,v)          _BFSET_(r32,31,24,v)
    #define   GET16MctrlDual_Timing1_init_refc(r16)            _BFGET_(r16,15, 8)
    #define   SET16MctrlDual_Timing1_init_refc(r16,v)          _BFSET_(r16,15, 8,v)

    #define     w32MctrlDual_Timing1                           {\
            UNSG32 uTiming1_init_tras                          :  8;\
            UNSG32 uTiming1_init_trfc                          :  8;\
            UNSG32 uTiming1_init_trc                           :  8;\
            UNSG32 uTiming1_init_refc                          :  8;\
          }
    union { UNSG32 u32MctrlDual_Timing1;
            struct w32MctrlDual_Timing1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Timing2_init_trp(r32)             _BFGET_(r32, 3, 0)
    #define   SET32MctrlDual_Timing2_init_trp(r32,v)           _BFSET_(r32, 3, 0,v)
    #define   GET16MctrlDual_Timing2_init_trp(r16)             _BFGET_(r16, 3, 0)
    #define   SET16MctrlDual_Timing2_init_trp(r16,v)           _BFSET_(r16, 3, 0,v)

    #define   GET32MctrlDual_Timing2_init_trrd(r32)            _BFGET_(r32, 7, 4)
    #define   SET32MctrlDual_Timing2_init_trrd(r32,v)          _BFSET_(r32, 7, 4,v)
    #define   GET16MctrlDual_Timing2_init_trrd(r16)            _BFGET_(r16, 7, 4)
    #define   SET16MctrlDual_Timing2_init_trrd(r16,v)          _BFSET_(r16, 7, 4,v)

    #define   GET32MctrlDual_Timing2_init_trcd(r32)            _BFGET_(r32,11, 8)
    #define   SET32MctrlDual_Timing2_init_trcd(r32,v)          _BFSET_(r32,11, 8,v)
    #define   GET16MctrlDual_Timing2_init_trcd(r16)            _BFGET_(r16,11, 8)
    #define   SET16MctrlDual_Timing2_init_trcd(r16,v)          _BFSET_(r16,11, 8,v)

    #define   GET32MctrlDual_Timing2_init_twr(r32)             _BFGET_(r32,15,12)
    #define   SET32MctrlDual_Timing2_init_twr(r32,v)           _BFSET_(r32,15,12,v)
    #define   GET16MctrlDual_Timing2_init_twr(r16)             _BFGET_(r16,15,12)
    #define   SET16MctrlDual_Timing2_init_twr(r16,v)           _BFSET_(r16,15,12,v)

    #define   GET32MctrlDual_Timing2_init_trtp(r32)            _BFGET_(r32,19,16)
    #define   SET32MctrlDual_Timing2_init_trtp(r32,v)          _BFSET_(r32,19,16,v)
    #define   GET16MctrlDual_Timing2_init_trtp(r16)            _BFGET_(r16, 3, 0)
    #define   SET16MctrlDual_Timing2_init_trtp(r16,v)          _BFSET_(r16, 3, 0,v)

    #define   GET32MctrlDual_Timing2_init_twtr(r32)            _BFGET_(r32,23,20)
    #define   SET32MctrlDual_Timing2_init_twtr(r32,v)          _BFSET_(r32,23,20,v)
    #define   GET16MctrlDual_Timing2_init_twtr(r16)            _BFGET_(r16, 7, 4)
    #define   SET16MctrlDual_Timing2_init_twtr(r16,v)          _BFSET_(r16, 7, 4,v)

    #define   GET32MctrlDual_Timing2_init_tmrd(r32)            _BFGET_(r32,27,24)
    #define   SET32MctrlDual_Timing2_init_tmrd(r32,v)          _BFSET_(r32,27,24,v)
    #define   GET16MctrlDual_Timing2_init_tmrd(r16)            _BFGET_(r16,11, 8)
    #define   SET16MctrlDual_Timing2_init_tmrd(r16,v)          _BFSET_(r16,11, 8,v)

    #define     w32MctrlDual_Timing2                           {\
            UNSG32 uTiming2_init_trp                           :  4;\
            UNSG32 uTiming2_init_trrd                          :  4;\
            UNSG32 uTiming2_init_trcd                          :  4;\
            UNSG32 uTiming2_init_twr                           :  4;\
            UNSG32 uTiming2_init_trtp                          :  4;\
            UNSG32 uTiming2_init_twtr                          :  4;\
            UNSG32 uTiming2_init_tmrd                          :  4;\
            UNSG32 RSVDx18_b28                                 :  4;\
          }
    union { UNSG32 u32MctrlDual_Timing2;
            struct w32MctrlDual_Timing2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Timing3_init_txsnr(r32)           _BFGET_(r32, 7, 0)
    #define   SET32MctrlDual_Timing3_init_txsnr(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16MctrlDual_Timing3_init_txsnr(r16)           _BFGET_(r16, 7, 0)
    #define   SET16MctrlDual_Timing3_init_txsnr(r16,v)         _BFSET_(r16, 7, 0,v)

    #define   GET32MctrlDual_Timing3_init_tick(r32)            _BFGET_(r32,15, 8)
    #define   SET32MctrlDual_Timing3_init_tick(r32,v)          _BFSET_(r32,15, 8,v)
    #define   GET16MctrlDual_Timing3_init_tick(r16)            _BFGET_(r16,15, 8)
    #define   SET16MctrlDual_Timing3_init_tick(r16,v)          _BFSET_(r16,15, 8,v)

    #define   GET32MctrlDual_Timing3_init_tfaw(r32)            _BFGET_(r32,23,16)
    #define   SET32MctrlDual_Timing3_init_tfaw(r32,v)          _BFSET_(r32,23,16,v)
    #define   GET16MctrlDual_Timing3_init_tfaw(r16)            _BFGET_(r16, 7, 0)
    #define   SET16MctrlDual_Timing3_init_tfaw(r16,v)          _BFSET_(r16, 7, 0,v)

    #define   GET32MctrlDual_Timing3_init_trfcmax(r32)         _BFGET_(r32,31,24)
    #define   SET32MctrlDual_Timing3_init_trfcmax(r32,v)       _BFSET_(r32,31,24,v)
    #define   GET16MctrlDual_Timing3_init_trfcmax(r16)         _BFGET_(r16,15, 8)
    #define   SET16MctrlDual_Timing3_init_trfcmax(r16,v)       _BFSET_(r16,15, 8,v)

    #define     w32MctrlDual_Timing3                           {\
            UNSG32 uTiming3_init_txsnr                         :  8;\
            UNSG32 uTiming3_init_tick                          :  8;\
            UNSG32 uTiming3_init_tfaw                          :  8;\
            UNSG32 uTiming3_init_trfcmax                       :  8;\
          }
    union { UNSG32 u32MctrlDual_Timing3;
            struct w32MctrlDual_Timing3;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Timing4_init_trst(r32)            _BFGET_(r32,11, 0)
    #define   SET32MctrlDual_Timing4_init_trst(r32,v)          _BFSET_(r32,11, 0,v)
    #define   GET16MctrlDual_Timing4_init_trst(r16)            _BFGET_(r16,11, 0)
    #define   SET16MctrlDual_Timing4_init_trst(r16,v)          _BFSET_(r16,11, 0,v)

    #define   GET32MctrlDual_Timing4_init_tmod(r32)            _BFGET_(r32,15,12)
    #define   SET32MctrlDual_Timing4_init_tmod(r32,v)          _BFSET_(r32,15,12,v)
    #define   GET16MctrlDual_Timing4_init_tmod(r16)            _BFGET_(r16,15,12)
    #define   SET16MctrlDual_Timing4_init_tmod(r16,v)          _BFSET_(r16,15,12,v)

    #define   GET32MctrlDual_Timing4_init_tzqi(r32)            _BFGET_(r32,23,16)
    #define   SET32MctrlDual_Timing4_init_tzqi(r32,v)          _BFSET_(r32,23,16,v)
    #define   GET16MctrlDual_Timing4_init_tzqi(r16)            _BFGET_(r16, 7, 0)
    #define   SET16MctrlDual_Timing4_init_tzqi(r16,v)          _BFSET_(r16, 7, 0,v)

    #define     w32MctrlDual_Timing4                           {\
            UNSG32 uTiming4_init_trst                          : 12;\
            UNSG32 uTiming4_init_tmod                          :  4;\
            UNSG32 uTiming4_init_tzqi                          :  8;\
            UNSG32 RSVDx20_b24                                 :  8;\
          }
    union { UNSG32 u32MctrlDual_Timing4;
            struct w32MctrlDual_Timing4;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Control1_init_taps(r32)           _BFGET_(r32, 7, 0)
    #define   SET32MctrlDual_Control1_init_taps(r32,v)         _BFSET_(r32, 7, 0,v)
    #define   GET16MctrlDual_Control1_init_taps(r16)           _BFGET_(r16, 7, 0)
    #define   SET16MctrlDual_Control1_init_taps(r16,v)         _BFSET_(r16, 7, 0,v)

    #define   GET32MctrlDual_Control1_wCollectEnb(r32)         _BFGET_(r32, 8, 8)
    #define   SET32MctrlDual_Control1_wCollectEnb(r32,v)       _BFSET_(r32, 8, 8,v)
    #define   GET16MctrlDual_Control1_wCollectEnb(r16)         _BFGET_(r16, 8, 8)
    #define   SET16MctrlDual_Control1_wCollectEnb(r16,v)       _BFSET_(r16, 8, 8,v)

    #define   GET32MctrlDual_Control1_bankStatusArb(r32)       _BFGET_(r32, 9, 9)
    #define   SET32MctrlDual_Control1_bankStatusArb(r32,v)     _BFSET_(r32, 9, 9,v)
    #define   GET16MctrlDual_Control1_bankStatusArb(r16)       _BFGET_(r16, 9, 9)
    #define   SET16MctrlDual_Control1_bankStatusArb(r16,v)     _BFSET_(r16, 9, 9,v)

    #define   GET32MctrlDual_Control1_xbarQosEnb(r32)          _BFGET_(r32,10,10)
    #define   SET32MctrlDual_Control1_xbarQosEnb(r32,v)        _BFSET_(r32,10,10,v)
    #define   GET16MctrlDual_Control1_xbarQosEnb(r16)          _BFGET_(r16,10,10)
    #define   SET16MctrlDual_Control1_xbarQosEnb(r16,v)        _BFSET_(r16,10,10,v)

    #define   GET32MctrlDual_Control1_bQReorder(r32)           _BFGET_(r32,14,11)
    #define   SET32MctrlDual_Control1_bQReorder(r32,v)         _BFSET_(r32,14,11,v)
    #define   GET16MctrlDual_Control1_bQReorder(r16)           _BFGET_(r16,14,11)
    #define   SET16MctrlDual_Control1_bQReorder(r16,v)         _BFSET_(r16,14,11,v)

    #define   GET32MctrlDual_Control1_cpu_read_force(r32)      _BFGET_(r32,15,15)
    #define   SET32MctrlDual_Control1_cpu_read_force(r32,v)    _BFSET_(r32,15,15,v)
    #define   GET16MctrlDual_Control1_cpu_read_force(r16)      _BFGET_(r16,15,15)
    #define   SET16MctrlDual_Control1_cpu_read_force(r16,v)    _BFSET_(r16,15,15,v)

    #define   GET32MctrlDual_Control1_pcmdq_size(r32)          _BFGET_(r32,17,16)
    #define   SET32MctrlDual_Control1_pcmdq_size(r32,v)        _BFSET_(r32,17,16,v)
    #define   GET16MctrlDual_Control1_pcmdq_size(r16)          _BFGET_(r16, 1, 0)
    #define   SET16MctrlDual_Control1_pcmdq_size(r16,v)        _BFSET_(r16, 1, 0,v)

    #define   GET32MctrlDual_Control1_apre_on(r32)             _BFGET_(r32,18,18)
    #define   SET32MctrlDual_Control1_apre_on(r32,v)           _BFSET_(r32,18,18,v)
    #define   GET16MctrlDual_Control1_apre_on(r16)             _BFGET_(r16, 2, 2)
    #define   SET16MctrlDual_Control1_apre_on(r16,v)           _BFSET_(r16, 2, 2,v)

    #define   GET32MctrlDual_Control1_sdarb_on(r32)            _BFGET_(r32,19,19)
    #define   SET32MctrlDual_Control1_sdarb_on(r32,v)          _BFSET_(r32,19,19,v)
    #define   GET16MctrlDual_Control1_sdarb_on(r16)            _BFGET_(r16, 3, 3)
    #define   SET16MctrlDual_Control1_sdarb_on(r16,v)          _BFSET_(r16, 3, 3,v)

    #define   GET32MctrlDual_Control1_perfCountEnb(r32)        _BFGET_(r32,20,20)
    #define   SET32MctrlDual_Control1_perfCountEnb(r32,v)      _BFSET_(r32,20,20,v)
    #define   GET16MctrlDual_Control1_perfCountEnb(r16)        _BFGET_(r16, 4, 4)
    #define   SET16MctrlDual_Control1_perfCountEnb(r16,v)      _BFSET_(r16, 4, 4,v)

    #define   GET32MctrlDual_Control1_cpuThrottle(r32)         _BFGET_(r32,21,21)
    #define   SET32MctrlDual_Control1_cpuThrottle(r32,v)       _BFSET_(r32,21,21,v)
    #define   GET16MctrlDual_Control1_cpuThrottle(r16)         _BFGET_(r16, 5, 5)
    #define   SET16MctrlDual_Control1_cpuThrottle(r16,v)       _BFSET_(r16, 5, 5,v)

    #define   GET32MctrlDual_Control1_wait4QoS(r32)            _BFGET_(r32,22,22)
    #define   SET32MctrlDual_Control1_wait4QoS(r32,v)          _BFSET_(r32,22,22,v)
    #define   GET16MctrlDual_Control1_wait4QoS(r16)            _BFGET_(r16, 6, 6)
    #define   SET16MctrlDual_Control1_wait4QoS(r16,v)          _BFSET_(r16, 6, 6,v)

    #define     w32MctrlDual_Control1                          {\
            UNSG32 uControl1_init_taps                         :  8;\
            UNSG32 uControl1_wCollectEnb                       :  1;\
            UNSG32 uControl1_bankStatusArb                     :  1;\
            UNSG32 uControl1_xbarQosEnb                        :  1;\
            UNSG32 uControl1_bQReorder                         :  4;\
            UNSG32 uControl1_cpu_read_force                    :  1;\
            UNSG32 uControl1_pcmdq_size                        :  2;\
            UNSG32 uControl1_apre_on                           :  1;\
            UNSG32 uControl1_sdarb_on                          :  1;\
            UNSG32 uControl1_perfCountEnb                      :  1;\
            UNSG32 uControl1_cpuThrottle                       :  1;\
            UNSG32 uControl1_wait4QoS                          :  1;\
            UNSG32 RSVDx24_b23                                 :  9;\
          }
    union { UNSG32 u32MctrlDual_Control1;
            struct w32MctrlDual_Control1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Control2_xsrlmr_on(r32)           _BFGET_(r32, 0, 0)
    #define   SET32MctrlDual_Control2_xsrlmr_on(r32,v)         _BFSET_(r32, 0, 0,v)
    #define   GET16MctrlDual_Control2_xsrlmr_on(r16)           _BFGET_(r16, 0, 0)
    #define   SET16MctrlDual_Control2_xsrlmr_on(r16,v)         _BFSET_(r16, 0, 0,v)

    #define   GET32MctrlDual_Control2_pwrsav_mode(r32)         _BFGET_(r32, 3, 1)
    #define   SET32MctrlDual_Control2_pwrsav_mode(r32,v)       _BFSET_(r32, 3, 1,v)
    #define   GET16MctrlDual_Control2_pwrsav_mode(r16)         _BFGET_(r16, 3, 1)
    #define   SET16MctrlDual_Control2_pwrsav_mode(r16,v)       _BFSET_(r16, 3, 1,v)

    #define   GET32MctrlDual_Control2_tcsr_sel(r32)            _BFGET_(r32, 5, 4)
    #define   SET32MctrlDual_Control2_tcsr_sel(r32,v)          _BFSET_(r32, 5, 4,v)
    #define   GET16MctrlDual_Control2_tcsr_sel(r16)            _BFGET_(r16, 5, 4)
    #define   SET16MctrlDual_Control2_tcsr_sel(r16,v)          _BFSET_(r16, 5, 4,v)

    #define   GET32MctrlDual_Control2_srcv_sel(r32)            _BFGET_(r32, 8, 6)
    #define   SET32MctrlDual_Control2_srcv_sel(r32,v)          _BFSET_(r32, 8, 6,v)
    #define   GET16MctrlDual_Control2_srcv_sel(r16)            _BFGET_(r16, 8, 6)
    #define   SET16MctrlDual_Control2_srcv_sel(r16,v)          _BFSET_(r16, 8, 6,v)

    #define   GET32MctrlDual_Control2_sref_rate(r32)           _BFGET_(r32, 9, 9)
    #define   SET32MctrlDual_Control2_sref_rate(r32,v)         _BFSET_(r32, 9, 9,v)
    #define   GET16MctrlDual_Control2_sref_rate(r16)           _BFGET_(r16, 9, 9)
    #define   SET16MctrlDual_Control2_sref_rate(r16,v)         _BFSET_(r16, 9, 9,v)

    #define   GET32MctrlDual_Control2_qout_oen(r32)            _BFGET_(r32,10,10)
    #define   SET32MctrlDual_Control2_qout_oen(r32,v)          _BFSET_(r32,10,10,v)
    #define   GET16MctrlDual_Control2_qout_oen(r16)            _BFGET_(r16,10,10)
    #define   SET16MctrlDual_Control2_qout_oen(r16,v)          _BFSET_(r16,10,10,v)

    #define   GET32MctrlDual_Control2_rtt_sel(r32)             _BFGET_(r32,13,11)
    #define   SET32MctrlDual_Control2_rtt_sel(r32,v)           _BFSET_(r32,13,11,v)
    #define   GET16MctrlDual_Control2_rtt_sel(r16)             _BFGET_(r16,13,11)
    #define   SET16MctrlDual_Control2_rtt_sel(r16,v)           _BFSET_(r16,13,11,v)

    #define   GET32MctrlDual_Control2_odrs_sel(r32)            _BFGET_(r32,15,14)
    #define   SET32MctrlDual_Control2_odrs_sel(r32,v)          _BFSET_(r32,15,14,v)
    #define   GET16MctrlDual_Control2_odrs_sel(r16)            _BFGET_(r16,15,14)
    #define   SET16MctrlDual_Control2_odrs_sel(r16,v)          _BFSET_(r16,15,14,v)

    #define   GET32MctrlDual_Control2_twr_sel(r32)             _BFGET_(r32,18,16)
    #define   SET32MctrlDual_Control2_twr_sel(r32,v)           _BFSET_(r32,18,16,v)
    #define   GET16MctrlDual_Control2_twr_sel(r16)             _BFGET_(r16, 2, 0)
    #define   SET16MctrlDual_Control2_twr_sel(r16,v)           _BFSET_(r16, 2, 0,v)

    #define   GET32MctrlDual_Control2_pdext_sel(r32)           _BFGET_(r32,19,19)
    #define   SET32MctrlDual_Control2_pdext_sel(r32,v)         _BFSET_(r32,19,19,v)
    #define   GET16MctrlDual_Control2_pdext_sel(r16)           _BFGET_(r16, 3, 3)
    #define   SET16MctrlDual_Control2_pdext_sel(r16,v)         _BFSET_(r16, 3, 3,v)

    #define   GET32MctrlDual_Control2_rtt_wr(r32)              _BFGET_(r32,21,20)
    #define   SET32MctrlDual_Control2_rtt_wr(r32,v)            _BFSET_(r32,21,20,v)
    #define   GET16MctrlDual_Control2_rtt_wr(r16)              _BFGET_(r16, 5, 4)
    #define   SET16MctrlDual_Control2_rtt_wr(r16,v)            _BFSET_(r16, 5, 4,v)

    #define   GET32MctrlDual_Control2_asr_sel(r32)             _BFGET_(r32,22,22)
    #define   SET32MctrlDual_Control2_asr_sel(r32,v)           _BFSET_(r32,22,22,v)
    #define   GET16MctrlDual_Control2_asr_sel(r16)             _BFGET_(r16, 6, 6)
    #define   SET16MctrlDual_Control2_asr_sel(r16,v)           _BFSET_(r16, 6, 6,v)

    #define   GET32MctrlDual_Control2_cwl_sel(r32)             _BFGET_(r32,25,23)
    #define   SET32MctrlDual_Control2_cwl_sel(r32,v)           _BFSET_(r32,25,23,v)
    #define   GET16MctrlDual_Control2_cwl_sel(r16)             _BFGET_(r16, 9, 7)
    #define   SET16MctrlDual_Control2_cwl_sel(r16,v)           _BFSET_(r16, 9, 7,v)

    #define   GET32MctrlDual_Control2_mpr_sel(r32)             _BFGET_(r32,26,26)
    #define   SET32MctrlDual_Control2_mpr_sel(r32,v)           _BFSET_(r32,26,26,v)
    #define   GET16MctrlDual_Control2_mpr_sel(r16)             _BFGET_(r16,10,10)
    #define   SET16MctrlDual_Control2_mpr_sel(r16,v)           _BFSET_(r16,10,10,v)

    #define   GET32MctrlDual_Control2_mpr_addr_sel(r32)        _BFGET_(r32,28,27)
    #define   SET32MctrlDual_Control2_mpr_addr_sel(r32,v)      _BFSET_(r32,28,27,v)
    #define   GET16MctrlDual_Control2_mpr_addr_sel(r16)        _BFGET_(r16,12,11)
    #define   SET16MctrlDual_Control2_mpr_addr_sel(r16,v)      _BFSET_(r16,12,11,v)

    #define   GET32MctrlDual_Control2_wlevel_sel(r32)          _BFGET_(r32,29,29)
    #define   SET32MctrlDual_Control2_wlevel_sel(r32,v)        _BFSET_(r32,29,29,v)
    #define   GET16MctrlDual_Control2_wlevel_sel(r16)          _BFGET_(r16,13,13)
    #define   SET16MctrlDual_Control2_wlevel_sel(r16,v)        _BFSET_(r16,13,13,v)

    #define     w32MctrlDual_Control2                          {\
            UNSG32 uControl2_xsrlmr_on                         :  1;\
            UNSG32 uControl2_pwrsav_mode                       :  3;\
            UNSG32 uControl2_tcsr_sel                          :  2;\
            UNSG32 uControl2_srcv_sel                          :  3;\
            UNSG32 uControl2_sref_rate                         :  1;\
            UNSG32 uControl2_qout_oen                          :  1;\
            UNSG32 uControl2_rtt_sel                           :  3;\
            UNSG32 uControl2_odrs_sel                          :  2;\
            UNSG32 uControl2_twr_sel                           :  3;\
            UNSG32 uControl2_pdext_sel                         :  1;\
            UNSG32 uControl2_rtt_wr                            :  2;\
            UNSG32 uControl2_asr_sel                           :  1;\
            UNSG32 uControl2_cwl_sel                           :  3;\
            UNSG32 uControl2_mpr_sel                           :  1;\
            UNSG32 uControl2_mpr_addr_sel                      :  2;\
            UNSG32 uControl2_wlevel_sel                        :  1;\
            UNSG32 RSVDx28_b30                                 :  2;\
          }
    union { UNSG32 u32MctrlDual_Control2;
            struct w32MctrlDual_Control2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_PriorityLevel_CPU0WritePriorityLevel(r32) _BFGET_(r32, 1, 0)
    #define   SET32MctrlDual_PriorityLevel_CPU0WritePriorityLevel(r32,v) _BFSET_(r32, 1, 0,v)
    #define   GET16MctrlDual_PriorityLevel_CPU0WritePriorityLevel(r16) _BFGET_(r16, 1, 0)
    #define   SET16MctrlDual_PriorityLevel_CPU0WritePriorityLevel(r16,v) _BFSET_(r16, 1, 0,v)

    #define   GET32MctrlDual_PriorityLevel_CPU1WritePriorityLevel(r32) _BFGET_(r32, 3, 2)
    #define   SET32MctrlDual_PriorityLevel_CPU1WritePriorityLevel(r32,v) _BFSET_(r32, 3, 2,v)
    #define   GET16MctrlDual_PriorityLevel_CPU1WritePriorityLevel(r16) _BFGET_(r16, 3, 2)
    #define   SET16MctrlDual_PriorityLevel_CPU1WritePriorityLevel(r16,v) _BFSET_(r16, 3, 2,v)

    #define   GET32MctrlDual_PriorityLevel_writePriorityLevel(r32) _BFGET_(r32, 5, 4)
    #define   SET32MctrlDual_PriorityLevel_writePriorityLevel(r32,v) _BFSET_(r32, 5, 4,v)
    #define   GET16MctrlDual_PriorityLevel_writePriorityLevel(r16) _BFGET_(r16, 5, 4)
    #define   SET16MctrlDual_PriorityLevel_writePriorityLevel(r16,v) _BFSET_(r16, 5, 4,v)

    #define   GET32MctrlDual_PriorityLevel_CPU0ReadPriorityLevel(r32) _BFGET_(r32, 7, 6)
    #define   SET32MctrlDual_PriorityLevel_CPU0ReadPriorityLevel(r32,v) _BFSET_(r32, 7, 6,v)
    #define   GET16MctrlDual_PriorityLevel_CPU0ReadPriorityLevel(r16) _BFGET_(r16, 7, 6)
    #define   SET16MctrlDual_PriorityLevel_CPU0ReadPriorityLevel(r16,v) _BFSET_(r16, 7, 6,v)

    #define   GET32MctrlDual_PriorityLevel_CPU1ReadPriorityLevel(r32) _BFGET_(r32, 9, 8)
    #define   SET32MctrlDual_PriorityLevel_CPU1ReadPriorityLevel(r32,v) _BFSET_(r32, 9, 8,v)
    #define   GET16MctrlDual_PriorityLevel_CPU1ReadPriorityLevel(r16) _BFGET_(r16, 9, 8)
    #define   SET16MctrlDual_PriorityLevel_CPU1ReadPriorityLevel(r16,v) _BFSET_(r16, 9, 8,v)

    #define   GET32MctrlDual_PriorityLevel_read1PriorityLevel(r32) _BFGET_(r32,11,10)
    #define   SET32MctrlDual_PriorityLevel_read1PriorityLevel(r32,v) _BFSET_(r32,11,10,v)
    #define   GET16MctrlDual_PriorityLevel_read1PriorityLevel(r16) _BFGET_(r16,11,10)
    #define   SET16MctrlDual_PriorityLevel_read1PriorityLevel(r16,v) _BFSET_(r16,11,10,v)

    #define   GET32MctrlDual_PriorityLevel_read2PriorityLevel(r32) _BFGET_(r32,13,12)
    #define   SET32MctrlDual_PriorityLevel_read2PriorityLevel(r32,v) _BFSET_(r32,13,12,v)
    #define   GET16MctrlDual_PriorityLevel_read2PriorityLevel(r16) _BFGET_(r16,13,12)
    #define   SET16MctrlDual_PriorityLevel_read2PriorityLevel(r16,v) _BFSET_(r16,13,12,v)

    #define   GET32MctrlDual_PriorityLevel_read3PriorityLevel(r32) _BFGET_(r32,15,14)
    #define   SET32MctrlDual_PriorityLevel_read3PriorityLevel(r32,v) _BFSET_(r32,15,14,v)
    #define   GET16MctrlDual_PriorityLevel_read3PriorityLevel(r16) _BFGET_(r16,15,14)
    #define   SET16MctrlDual_PriorityLevel_read3PriorityLevel(r16,v) _BFSET_(r16,15,14,v)

    #define   GET32MctrlDual_PriorityLevel_read4PriorityLevel(r32) _BFGET_(r32,17,16)
    #define   SET32MctrlDual_PriorityLevel_read4PriorityLevel(r32,v) _BFSET_(r32,17,16,v)
    #define   GET16MctrlDual_PriorityLevel_read4PriorityLevel(r16) _BFGET_(r16, 1, 0)
    #define   SET16MctrlDual_PriorityLevel_read4PriorityLevel(r16,v) _BFSET_(r16, 1, 0,v)

    #define   GET32MctrlDual_PriorityLevel_read5PriorityLevel(r32) _BFGET_(r32,19,18)
    #define   SET32MctrlDual_PriorityLevel_read5PriorityLevel(r32,v) _BFSET_(r32,19,18,v)
    #define   GET16MctrlDual_PriorityLevel_read5PriorityLevel(r16) _BFGET_(r16, 3, 2)
    #define   SET16MctrlDual_PriorityLevel_read5PriorityLevel(r16,v) _BFSET_(r16, 3, 2,v)

    #define   GET32MctrlDual_PriorityLevel_priorityMode(r32)   _BFGET_(r32,20,20)
    #define   SET32MctrlDual_PriorityLevel_priorityMode(r32,v) _BFSET_(r32,20,20,v)
    #define   GET16MctrlDual_PriorityLevel_priorityMode(r16)   _BFGET_(r16, 4, 4)
    #define   SET16MctrlDual_PriorityLevel_priorityMode(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32MctrlDual_PriorityLevel_chArb0To1StopCount(r32) _BFGET_(r32,24,21)
    #define   SET32MctrlDual_PriorityLevel_chArb0To1StopCount(r32,v) _BFSET_(r32,24,21,v)
    #define   GET16MctrlDual_PriorityLevel_chArb0To1StopCount(r16) _BFGET_(r16, 8, 5)
    #define   SET16MctrlDual_PriorityLevel_chArb0To1StopCount(r16,v) _BFSET_(r16, 8, 5,v)

    #define   GET32MctrlDual_PriorityLevel_chArb1To2StopCount(r32) _BFGET_(r32,28,25)
    #define   SET32MctrlDual_PriorityLevel_chArb1To2StopCount(r32,v) _BFSET_(r32,28,25,v)
    #define   GET16MctrlDual_PriorityLevel_chArb1To2StopCount(r16) _BFGET_(r16,12, 9)
    #define   SET16MctrlDual_PriorityLevel_chArb1To2StopCount(r16,v) _BFSET_(r16,12, 9,v)

    #define     w32MctrlDual_PriorityLevel                     {\
            UNSG32 uPriorityLevel_CPU0WritePriorityLevel       :  2;\
            UNSG32 uPriorityLevel_CPU1WritePriorityLevel       :  2;\
            UNSG32 uPriorityLevel_writePriorityLevel           :  2;\
            UNSG32 uPriorityLevel_CPU0ReadPriorityLevel        :  2;\
            UNSG32 uPriorityLevel_CPU1ReadPriorityLevel        :  2;\
            UNSG32 uPriorityLevel_read1PriorityLevel           :  2;\
            UNSG32 uPriorityLevel_read2PriorityLevel           :  2;\
            UNSG32 uPriorityLevel_read3PriorityLevel           :  2;\
            UNSG32 uPriorityLevel_read4PriorityLevel           :  2;\
            UNSG32 uPriorityLevel_read5PriorityLevel           :  2;\
            UNSG32 uPriorityLevel_priorityMode                 :  1;\
            UNSG32 uPriorityLevel_chArb0To1StopCount           :  4;\
            UNSG32 uPriorityLevel_chArb1To2StopCount           :  4;\
            UNSG32 RSVDx2C_b29                                 :  3;\
          }
    union { UNSG32 u32MctrlDual_PriorityLevel;
            struct w32MctrlDual_PriorityLevel;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_TTBStartAddr_RESERVE_TTBSart(r32) _BFGET_(r32, 4, 0)
    #define   SET32MctrlDual_TTBStartAddr_RESERVE_TTBSart(r32,v) _BFSET_(r32, 4, 0,v)
    #define   GET16MctrlDual_TTBStartAddr_RESERVE_TTBSart(r16) _BFGET_(r16, 4, 0)
    #define   SET16MctrlDual_TTBStartAddr_RESERVE_TTBSart(r16,v) _BFSET_(r16, 4, 0,v)

    #define   GET32MctrlDual_TTBStartAddr_ttbStartAddr(r32)    _BFGET_(r32,31, 5)
    #define   SET32MctrlDual_TTBStartAddr_ttbStartAddr(r32,v)  _BFSET_(r32,31, 5,v)

    #define     w32MctrlDual_TTBStartAddr                      {\
            UNSG32 uTTBStartAddr_RESERVE_TTBSart               :  5;\
            UNSG32 uTTBStartAddr_ttbStartAddr                  : 27;\
          }
    union { UNSG32 u32MctrlDual_TTBStartAddr;
            struct w32MctrlDual_TTBStartAddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_TTBEndAddr_RESERVE_TTBEnd(r32)    _BFGET_(r32, 4, 0)
    #define   SET32MctrlDual_TTBEndAddr_RESERVE_TTBEnd(r32,v)  _BFSET_(r32, 4, 0,v)
    #define   GET16MctrlDual_TTBEndAddr_RESERVE_TTBEnd(r16)    _BFGET_(r16, 4, 0)
    #define   SET16MctrlDual_TTBEndAddr_RESERVE_TTBEnd(r16,v)  _BFSET_(r16, 4, 0,v)

    #define   GET32MctrlDual_TTBEndAddr_ttbEndAddr(r32)        _BFGET_(r32,31, 5)
    #define   SET32MctrlDual_TTBEndAddr_ttbEndAddr(r32,v)      _BFSET_(r32,31, 5,v)

    #define     w32MctrlDual_TTBEndAddr                        {\
            UNSG32 uTTBEndAddr_RESERVE_TTBEnd                  :  5;\
            UNSG32 uTTBEndAddr_ttbEndAddr                      : 27;\
          }
    union { UNSG32 u32MctrlDual_TTBEndAddr;
            struct w32MctrlDual_TTBEndAddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_TTBWritePrt_RESERVE_TTBWPTR(r32)  _BFGET_(r32, 4, 0)
    #define   SET32MctrlDual_TTBWritePrt_RESERVE_TTBWPTR(r32,v) _BFSET_(r32, 4, 0,v)
    #define   GET16MctrlDual_TTBWritePrt_RESERVE_TTBWPTR(r16)  _BFGET_(r16, 4, 0)
    #define   SET16MctrlDual_TTBWritePrt_RESERVE_TTBWPTR(r16,v) _BFSET_(r16, 4, 0,v)

    #define   GET32MctrlDual_TTBWritePrt_ttbWPtr(r32)          _BFGET_(r32,31, 5)
    #define   SET32MctrlDual_TTBWritePrt_ttbWPtr(r32,v)        _BFSET_(r32,31, 5,v)

    #define     w32MctrlDual_TTBWritePrt                       {\
            UNSG32 uTTBWritePrt_RESERVE_TTBWPTR                :  5;\
            UNSG32 uTTBWritePrt_ttbWPtr                        : 27;\
          }
    union { UNSG32 u32MctrlDual_TTBWritePrt;
            struct w32MctrlDual_TTBWritePrt;
          };
    ///////////////////////////////////////////////////////////
              SIE_TTBPort                                      ie_cpu0WPort;
    ///////////////////////////////////////////////////////////
              SIE_TTBPort                                      ie_cpu1WPort;
    ///////////////////////////////////////////////////////////
              SIE_TTBPort                                      ie_busWPort;
    ///////////////////////////////////////////////////////////
              SIE_TTBPort                                      ie_cpu0RPort;
    ///////////////////////////////////////////////////////////
              SIE_TTBPort                                      ie_cpu1RPort;
    ///////////////////////////////////////////////////////////
              SIE_TTBPort                                      ie_busRPort1;
    ///////////////////////////////////////////////////////////
              SIE_TTBPort                                      ie_busRPort2;
    ///////////////////////////////////////////////////////////
              SIE_TTBPort                                      ie_busRPort3;
    ///////////////////////////////////////////////////////////
              SIE_TTBPort                                      ie_busRPort4;
    ///////////////////////////////////////////////////////////
              SIE_TTBPort                                      ie_busRPort5;
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_DebugTrigger_mt_loop(r32)         _BFGET_(r32, 0, 0)
    #define   SET32MctrlDual_DebugTrigger_mt_loop(r32,v)       _BFSET_(r32, 0, 0,v)
    #define   GET16MctrlDual_DebugTrigger_mt_loop(r16)         _BFGET_(r16, 0, 0)
    #define   SET16MctrlDual_DebugTrigger_mt_loop(r16,v)       _BFSET_(r16, 0, 0,v)

    #define   GET32MctrlDual_DebugTrigger_mt_active(r32)       _BFGET_(r32, 1, 1)
    #define   SET32MctrlDual_DebugTrigger_mt_active(r32,v)     _BFSET_(r32, 1, 1,v)
    #define   GET16MctrlDual_DebugTrigger_mt_active(r16)       _BFGET_(r16, 1, 1)
    #define   SET16MctrlDual_DebugTrigger_mt_active(r16,v)     _BFSET_(r16, 1, 1,v)

    #define   GET32MctrlDual_DebugTrigger_mt_mode(r32)         _BFGET_(r32, 2, 2)
    #define   SET32MctrlDual_DebugTrigger_mt_mode(r32,v)       _BFSET_(r32, 2, 2,v)
    #define   GET16MctrlDual_DebugTrigger_mt_mode(r16)         _BFGET_(r16, 2, 2)
    #define   SET16MctrlDual_DebugTrigger_mt_mode(r16,v)       _BFSET_(r16, 2, 2,v)

    #define   GET32MctrlDual_DebugTrigger_mt_coe(r32)          _BFGET_(r32, 3, 3)
    #define   SET32MctrlDual_DebugTrigger_mt_coe(r32,v)        _BFSET_(r32, 3, 3,v)
    #define   GET16MctrlDual_DebugTrigger_mt_coe(r16)          _BFGET_(r16, 3, 3)
    #define   SET16MctrlDual_DebugTrigger_mt_coe(r16,v)        _BFSET_(r16, 3, 3,v)

    #define     w32MctrlDual_DebugTrigger                      {\
            UNSG32 uDebugTrigger_mt_loop                       :  1;\
            UNSG32 uDebugTrigger_mt_active                     :  1;\
            UNSG32 uDebugTrigger_mt_mode                       :  1;\
            UNSG32 uDebugTrigger_mt_coe                        :  1;\
            UNSG32 RSVDx17C_b4                                 : 28;\
          }
    union { UNSG32 u32MctrlDual_DebugTrigger;
            struct w32MctrlDual_DebugTrigger;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_DebugStAddr_addr_start(r32)       _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_DebugStAddr_addr_start(r32,v)     _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_DebugStAddr                       {\
            UNSG32 uDebugStAddr_addr_start                     : 32;\
          }
    union { UNSG32 u32MctrlDual_DebugStAddr;
            struct w32MctrlDual_DebugStAddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_DebugEdAddr_addr_end(r32)         _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_DebugEdAddr_addr_end(r32,v)       _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_DebugEdAddr                       {\
            UNSG32 uDebugEdAddr_addr_end                       : 32;\
          }
    union { UNSG32 u32MctrlDual_DebugEdAddr;
            struct w32MctrlDual_DebugEdAddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_DebugStatus_mt_done(r32)          _BFGET_(r32, 0, 0)
    #define   SET32MctrlDual_DebugStatus_mt_done(r32,v)        _BFSET_(r32, 0, 0,v)
    #define   GET16MctrlDual_DebugStatus_mt_done(r16)          _BFGET_(r16, 0, 0)
    #define   SET16MctrlDual_DebugStatus_mt_done(r16,v)        _BFSET_(r16, 0, 0,v)

    #define   GET32MctrlDual_DebugStatus_mt_error(r32)         _BFGET_(r32, 2, 1)
    #define   SET32MctrlDual_DebugStatus_mt_error(r32,v)       _BFSET_(r32, 2, 1,v)
    #define   GET16MctrlDual_DebugStatus_mt_error(r16)         _BFGET_(r16, 2, 1)
    #define   SET16MctrlDual_DebugStatus_mt_error(r16,v)       _BFSET_(r16, 2, 1,v)

    #define     w32MctrlDual_DebugStatus                       {\
            UNSG32 uDebugStatus_mt_done                        :  1;\
            UNSG32 uDebugStatus_mt_error                       :  2;\
            UNSG32 RSVDx188_b3                                 : 29;\
          }
    union { UNSG32 u32MctrlDual_DebugStatus;
            struct w32MctrlDual_DebugStatus;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_DebugRaddr_mt_raddr(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_DebugRaddr_mt_raddr(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_DebugRaddr                        {\
            UNSG32 uDebugRaddr_mt_raddr                        : 32;\
          }
    union { UNSG32 u32MctrlDual_DebugRaddr;
            struct w32MctrlDual_DebugRaddr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_DebugRdata0_mt_rdata0(r32)        _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_DebugRdata0_mt_rdata0(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_DebugRdata0                       {\
            UNSG32 uDebugRdata0_mt_rdata0                      : 32;\
          }
    union { UNSG32 u32MctrlDual_DebugRdata0;
            struct w32MctrlDual_DebugRdata0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_DebugRdata1_mt_rdata1(r32)        _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_DebugRdata1_mt_rdata1(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_DebugRdata1                       {\
            UNSG32 uDebugRdata1_mt_rdata1                      : 32;\
          }
    union { UNSG32 u32MctrlDual_DebugRdata1;
            struct w32MctrlDual_DebugRdata1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_DebugOData_mt_odd(r32)            _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_DebugOData_mt_odd(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_DebugOData                        {\
            UNSG32 uDebugOData_mt_odd                          : 32;\
          }
    union { UNSG32 u32MctrlDual_DebugOData;
            struct w32MctrlDual_DebugOData;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_DebugEData_mt_even(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_DebugEData_mt_even(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_DebugEData                        {\
            UNSG32 uDebugEData_mt_even                         : 32;\
          }
    union { UNSG32 u32MctrlDual_DebugEData;
            struct w32MctrlDual_DebugEData;
          };
    ///////////////////////////////////////////////////////////
              SIE_PHY                                          ie_PHY;
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_InterruptMask_intMaskPort0(r32)   _BFGET_(r32, 0, 0)
    #define   SET32MctrlDual_InterruptMask_intMaskPort0(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16MctrlDual_InterruptMask_intMaskPort0(r16)   _BFGET_(r16, 0, 0)
    #define   SET16MctrlDual_InterruptMask_intMaskPort0(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32MctrlDual_InterruptMask_intMaskPort1(r32)   _BFGET_(r32, 1, 1)
    #define   SET32MctrlDual_InterruptMask_intMaskPort1(r32,v) _BFSET_(r32, 1, 1,v)
    #define   GET16MctrlDual_InterruptMask_intMaskPort1(r16)   _BFGET_(r16, 1, 1)
    #define   SET16MctrlDual_InterruptMask_intMaskPort1(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32MctrlDual_InterruptMask_intMaskPort2(r32)   _BFGET_(r32, 2, 2)
    #define   SET32MctrlDual_InterruptMask_intMaskPort2(r32,v) _BFSET_(r32, 2, 2,v)
    #define   GET16MctrlDual_InterruptMask_intMaskPort2(r16)   _BFGET_(r16, 2, 2)
    #define   SET16MctrlDual_InterruptMask_intMaskPort2(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32MctrlDual_InterruptMask_intMaskPort3(r32)   _BFGET_(r32, 3, 3)
    #define   SET32MctrlDual_InterruptMask_intMaskPort3(r32,v) _BFSET_(r32, 3, 3,v)
    #define   GET16MctrlDual_InterruptMask_intMaskPort3(r16)   _BFGET_(r16, 3, 3)
    #define   SET16MctrlDual_InterruptMask_intMaskPort3(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32MctrlDual_InterruptMask_intMaskPor4(r32)    _BFGET_(r32, 4, 4)
    #define   SET32MctrlDual_InterruptMask_intMaskPor4(r32,v)  _BFSET_(r32, 4, 4,v)
    #define   GET16MctrlDual_InterruptMask_intMaskPor4(r16)    _BFGET_(r16, 4, 4)
    #define   SET16MctrlDual_InterruptMask_intMaskPor4(r16,v)  _BFSET_(r16, 4, 4,v)

    #define   GET32MctrlDual_InterruptMask_intMaskPort5(r32)   _BFGET_(r32, 5, 5)
    #define   SET32MctrlDual_InterruptMask_intMaskPort5(r32,v) _BFSET_(r32, 5, 5,v)
    #define   GET16MctrlDual_InterruptMask_intMaskPort5(r16)   _BFGET_(r16, 5, 5)
    #define   SET16MctrlDual_InterruptMask_intMaskPort5(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32MctrlDual_InterruptMask_intMaskCpu0R(r32)   _BFGET_(r32, 6, 6)
    #define   SET32MctrlDual_InterruptMask_intMaskCpu0R(r32,v) _BFSET_(r32, 6, 6,v)
    #define   GET16MctrlDual_InterruptMask_intMaskCpu0R(r16)   _BFGET_(r16, 6, 6)
    #define   SET16MctrlDual_InterruptMask_intMaskCpu0R(r16,v) _BFSET_(r16, 6, 6,v)

    #define   GET32MctrlDual_InterruptMask_intMaskCpu1R(r32)   _BFGET_(r32, 7, 7)
    #define   SET32MctrlDual_InterruptMask_intMaskCpu1R(r32,v) _BFSET_(r32, 7, 7,v)
    #define   GET16MctrlDual_InterruptMask_intMaskCpu1R(r16)   _BFGET_(r16, 7, 7)
    #define   SET16MctrlDual_InterruptMask_intMaskCpu1R(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32MctrlDual_InterruptMask_intMaskCpu0W(r32)   _BFGET_(r32, 8, 8)
    #define   SET32MctrlDual_InterruptMask_intMaskCpu0W(r32,v) _BFSET_(r32, 8, 8,v)
    #define   GET16MctrlDual_InterruptMask_intMaskCpu0W(r16)   _BFGET_(r16, 8, 8)
    #define   SET16MctrlDual_InterruptMask_intMaskCpu0W(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32MctrlDual_InterruptMask_intMaskCpu1W(r32)   _BFGET_(r32, 9, 9)
    #define   SET32MctrlDual_InterruptMask_intMaskCpu1W(r32,v) _BFSET_(r32, 9, 9,v)
    #define   GET16MctrlDual_InterruptMask_intMaskCpu1W(r16)   _BFGET_(r16, 9, 9)
    #define   SET16MctrlDual_InterruptMask_intMaskCpu1W(r16,v) _BFSET_(r16, 9, 9,v)

    #define     w32MctrlDual_InterruptMask                     {\
            UNSG32 uInterruptMask_intMaskPort0                 :  1;\
            UNSG32 uInterruptMask_intMaskPort1                 :  1;\
            UNSG32 uInterruptMask_intMaskPort2                 :  1;\
            UNSG32 uInterruptMask_intMaskPort3                 :  1;\
            UNSG32 uInterruptMask_intMaskPor4                  :  1;\
            UNSG32 uInterruptMask_intMaskPort5                 :  1;\
            UNSG32 uInterruptMask_intMaskCpu0R                 :  1;\
            UNSG32 uInterruptMask_intMaskCpu1R                 :  1;\
            UNSG32 uInterruptMask_intMaskCpu0W                 :  1;\
            UNSG32 uInterruptMask_intMaskCpu1W                 :  1;\
            UNSG32 RSVDx200_b10                                : 22;\
          }
    union { UNSG32 u32MctrlDual_InterruptMask;
            struct w32MctrlDual_InterruptMask;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_interruptStatus_interruptPort0(r32) _BFGET_(r32, 0, 0)
    #define   SET32MctrlDual_interruptStatus_interruptPort0(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16MctrlDual_interruptStatus_interruptPort0(r16) _BFGET_(r16, 0, 0)
    #define   SET16MctrlDual_interruptStatus_interruptPort0(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32MctrlDual_interruptStatus_interruptPort1(r32) _BFGET_(r32, 1, 1)
    #define   SET32MctrlDual_interruptStatus_interruptPort1(r32,v) _BFSET_(r32, 1, 1,v)
    #define   GET16MctrlDual_interruptStatus_interruptPort1(r16) _BFGET_(r16, 1, 1)
    #define   SET16MctrlDual_interruptStatus_interruptPort1(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32MctrlDual_interruptStatus_interruptPort2(r32) _BFGET_(r32, 2, 2)
    #define   SET32MctrlDual_interruptStatus_interruptPort2(r32,v) _BFSET_(r32, 2, 2,v)
    #define   GET16MctrlDual_interruptStatus_interruptPort2(r16) _BFGET_(r16, 2, 2)
    #define   SET16MctrlDual_interruptStatus_interruptPort2(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32MctrlDual_interruptStatus_interruptPort3(r32) _BFGET_(r32, 3, 3)
    #define   SET32MctrlDual_interruptStatus_interruptPort3(r32,v) _BFSET_(r32, 3, 3,v)
    #define   GET16MctrlDual_interruptStatus_interruptPort3(r16) _BFGET_(r16, 3, 3)
    #define   SET16MctrlDual_interruptStatus_interruptPort3(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32MctrlDual_interruptStatus_interruptPort4(r32) _BFGET_(r32, 4, 4)
    #define   SET32MctrlDual_interruptStatus_interruptPort4(r32,v) _BFSET_(r32, 4, 4,v)
    #define   GET16MctrlDual_interruptStatus_interruptPort4(r16) _BFGET_(r16, 4, 4)
    #define   SET16MctrlDual_interruptStatus_interruptPort4(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32MctrlDual_interruptStatus_interruptPort5(r32) _BFGET_(r32, 5, 5)
    #define   SET32MctrlDual_interruptStatus_interruptPort5(r32,v) _BFSET_(r32, 5, 5,v)
    #define   GET16MctrlDual_interruptStatus_interruptPort5(r16) _BFGET_(r16, 5, 5)
    #define   SET16MctrlDual_interruptStatus_interruptPort5(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32MctrlDual_interruptStatus_interruptCpu0W(r32) _BFGET_(r32, 6, 6)
    #define   SET32MctrlDual_interruptStatus_interruptCpu0W(r32,v) _BFSET_(r32, 6, 6,v)
    #define   GET16MctrlDual_interruptStatus_interruptCpu0W(r16) _BFGET_(r16, 6, 6)
    #define   SET16MctrlDual_interruptStatus_interruptCpu0W(r16,v) _BFSET_(r16, 6, 6,v)

    #define   GET32MctrlDual_interruptStatus_interruptCpu0R(r32) _BFGET_(r32, 7, 7)
    #define   SET32MctrlDual_interruptStatus_interruptCpu0R(r32,v) _BFSET_(r32, 7, 7,v)
    #define   GET16MctrlDual_interruptStatus_interruptCpu0R(r16) _BFGET_(r16, 7, 7)
    #define   SET16MctrlDual_interruptStatus_interruptCpu0R(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32MctrlDual_interruptStatus_interruptCpu1W(r32) _BFGET_(r32, 8, 8)
    #define   SET32MctrlDual_interruptStatus_interruptCpu1W(r32,v) _BFSET_(r32, 8, 8,v)
    #define   GET16MctrlDual_interruptStatus_interruptCpu1W(r16) _BFGET_(r16, 8, 8)
    #define   SET16MctrlDual_interruptStatus_interruptCpu1W(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32MctrlDual_interruptStatus_interruptCpu1R(r32) _BFGET_(r32, 9, 9)
    #define   SET32MctrlDual_interruptStatus_interruptCpu1R(r32,v) _BFSET_(r32, 9, 9,v)
    #define   GET16MctrlDual_interruptStatus_interruptCpu1R(r16) _BFGET_(r16, 9, 9)
    #define   SET16MctrlDual_interruptStatus_interruptCpu1R(r16,v) _BFSET_(r16, 9, 9,v)

    #define     w32MctrlDual_interruptStatus                   {\
            UNSG32 uinterruptStatus_interruptPort0             :  1;\
            UNSG32 uinterruptStatus_interruptPort1             :  1;\
            UNSG32 uinterruptStatus_interruptPort2             :  1;\
            UNSG32 uinterruptStatus_interruptPort3             :  1;\
            UNSG32 uinterruptStatus_interruptPort4             :  1;\
            UNSG32 uinterruptStatus_interruptPort5             :  1;\
            UNSG32 uinterruptStatus_interruptCpu0W             :  1;\
            UNSG32 uinterruptStatus_interruptCpu0R             :  1;\
            UNSG32 uinterruptStatus_interruptCpu1W             :  1;\
            UNSG32 uinterruptStatus_interruptCpu1R             :  1;\
            UNSG32 RSVDx204_b10                                : 22;\
          }
    union { UNSG32 u32MctrlDual_interruptStatus;
            struct w32MctrlDual_interruptStatus;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_LFSRData_lfsrOut(r32)             _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_LFSRData_lfsrOut(r32,v)           _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_LFSRData                          {\
            UNSG32 uLFSRData_lfsrOut                           : 32;\
          }
    union { UNSG32 u32MctrlDual_LFSRData;
            struct w32MctrlDual_LFSRData;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_PerfCounterRst_cycleCountRst(r32) _BFGET_(r32, 0, 0)
    #define   SET32MctrlDual_PerfCounterRst_cycleCountRst(r32,v) _BFSET_(r32, 0, 0,v)
    #define   GET16MctrlDual_PerfCounterRst_cycleCountRst(r16) _BFGET_(r16, 0, 0)
    #define   SET16MctrlDual_PerfCounterRst_cycleCountRst(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32MctrlDual_PerfCounterRst_ctrlIdleCountRst(r32) _BFGET_(r32, 1, 1)
    #define   SET32MctrlDual_PerfCounterRst_ctrlIdleCountRst(r32,v) _BFSET_(r32, 1, 1,v)
    #define   GET16MctrlDual_PerfCounterRst_ctrlIdleCountRst(r16) _BFGET_(r16, 1, 1)
    #define   SET16MctrlDual_PerfCounterRst_ctrlIdleCountRst(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32MctrlDual_PerfCounterRst_dBusBusyCh0CountRst(r32) _BFGET_(r32, 2, 2)
    #define   SET32MctrlDual_PerfCounterRst_dBusBusyCh0CountRst(r32,v) _BFSET_(r32, 2, 2,v)
    #define   GET16MctrlDual_PerfCounterRst_dBusBusyCh0CountRst(r16) _BFGET_(r16, 2, 2)
    #define   SET16MctrlDual_PerfCounterRst_dBusBusyCh0CountRst(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32MctrlDual_PerfCounterRst_dBusBusyCh1CountRst(r32) _BFGET_(r32, 3, 3)
    #define   SET32MctrlDual_PerfCounterRst_dBusBusyCh1CountRst(r32,v) _BFSET_(r32, 3, 3,v)
    #define   GET16MctrlDual_PerfCounterRst_dBusBusyCh1CountRst(r16) _BFGET_(r16, 3, 3)
    #define   SET16MctrlDual_PerfCounterRst_dBusBusyCh1CountRst(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32MctrlDual_PerfCounterRst_busyCh0CountRst(r32) _BFGET_(r32, 4, 4)
    #define   SET32MctrlDual_PerfCounterRst_busyCh0CountRst(r32,v) _BFSET_(r32, 4, 4,v)
    #define   GET16MctrlDual_PerfCounterRst_busyCh0CountRst(r16) _BFGET_(r16, 4, 4)
    #define   SET16MctrlDual_PerfCounterRst_busyCh0CountRst(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32MctrlDual_PerfCounterRst_busyCh1CountRst(r32) _BFGET_(r32, 5, 5)
    #define   SET32MctrlDual_PerfCounterRst_busyCh1CountRst(r32,v) _BFSET_(r32, 5, 5,v)
    #define   GET16MctrlDual_PerfCounterRst_busyCh1CountRst(r16) _BFGET_(r16, 5, 5)
    #define   SET16MctrlDual_PerfCounterRst_busyCh1CountRst(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32MctrlDual_PerfCounterRst_cpu0WReqCountRst(r32) _BFGET_(r32, 6, 6)
    #define   SET32MctrlDual_PerfCounterRst_cpu0WReqCountRst(r32,v) _BFSET_(r32, 6, 6,v)
    #define   GET16MctrlDual_PerfCounterRst_cpu0WReqCountRst(r16) _BFGET_(r16, 6, 6)
    #define   SET16MctrlDual_PerfCounterRst_cpu0WReqCountRst(r16,v) _BFSET_(r16, 6, 6,v)

    #define   GET32MctrlDual_PerfCounterRst_cpu1WReqCountRst(r32) _BFGET_(r32, 7, 7)
    #define   SET32MctrlDual_PerfCounterRst_cpu1WReqCountRst(r32,v) _BFSET_(r32, 7, 7,v)
    #define   GET16MctrlDual_PerfCounterRst_cpu1WReqCountRst(r16) _BFGET_(r16, 7, 7)
    #define   SET16MctrlDual_PerfCounterRst_cpu1WReqCountRst(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32MctrlDual_PerfCounterRst_cpu0RReqCountRst(r32) _BFGET_(r32, 8, 8)
    #define   SET32MctrlDual_PerfCounterRst_cpu0RReqCountRst(r32,v) _BFSET_(r32, 8, 8,v)
    #define   GET16MctrlDual_PerfCounterRst_cpu0RReqCountRst(r16) _BFGET_(r16, 8, 8)
    #define   SET16MctrlDual_PerfCounterRst_cpu0RReqCountRst(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32MctrlDual_PerfCounterRst_cpu1RReqCountRst(r32) _BFGET_(r32, 9, 9)
    #define   SET32MctrlDual_PerfCounterRst_cpu1RReqCountRst(r32,v) _BFSET_(r32, 9, 9,v)
    #define   GET16MctrlDual_PerfCounterRst_cpu1RReqCountRst(r16) _BFGET_(r16, 9, 9)
    #define   SET16MctrlDual_PerfCounterRst_cpu1RReqCountRst(r16,v) _BFSET_(r16, 9, 9,v)

    #define   GET32MctrlDual_PerfCounterRst_port0ReqCountRst(r32) _BFGET_(r32,10,10)
    #define   SET32MctrlDual_PerfCounterRst_port0ReqCountRst(r32,v) _BFSET_(r32,10,10,v)
    #define   GET16MctrlDual_PerfCounterRst_port0ReqCountRst(r16) _BFGET_(r16,10,10)
    #define   SET16MctrlDual_PerfCounterRst_port0ReqCountRst(r16,v) _BFSET_(r16,10,10,v)

    #define   GET32MctrlDual_PerfCounterRst_port1ReqCountRst(r32) _BFGET_(r32,11,11)
    #define   SET32MctrlDual_PerfCounterRst_port1ReqCountRst(r32,v) _BFSET_(r32,11,11,v)
    #define   GET16MctrlDual_PerfCounterRst_port1ReqCountRst(r16) _BFGET_(r16,11,11)
    #define   SET16MctrlDual_PerfCounterRst_port1ReqCountRst(r16,v) _BFSET_(r16,11,11,v)

    #define   GET32MctrlDual_PerfCounterRst_port2ReqCountRst(r32) _BFGET_(r32,12,12)
    #define   SET32MctrlDual_PerfCounterRst_port2ReqCountRst(r32,v) _BFSET_(r32,12,12,v)
    #define   GET16MctrlDual_PerfCounterRst_port2ReqCountRst(r16) _BFGET_(r16,12,12)
    #define   SET16MctrlDual_PerfCounterRst_port2ReqCountRst(r16,v) _BFSET_(r16,12,12,v)

    #define   GET32MctrlDual_PerfCounterRst_port3ReqCountRst(r32) _BFGET_(r32,13,13)
    #define   SET32MctrlDual_PerfCounterRst_port3ReqCountRst(r32,v) _BFSET_(r32,13,13,v)
    #define   GET16MctrlDual_PerfCounterRst_port3ReqCountRst(r16) _BFGET_(r16,13,13)
    #define   SET16MctrlDual_PerfCounterRst_port3ReqCountRst(r16,v) _BFSET_(r16,13,13,v)

    #define   GET32MctrlDual_PerfCounterRst_port4ReqCountRst(r32) _BFGET_(r32,14,14)
    #define   SET32MctrlDual_PerfCounterRst_port4ReqCountRst(r32,v) _BFSET_(r32,14,14,v)
    #define   GET16MctrlDual_PerfCounterRst_port4ReqCountRst(r16) _BFGET_(r16,14,14)
    #define   SET16MctrlDual_PerfCounterRst_port4ReqCountRst(r16,v) _BFSET_(r16,14,14,v)

    #define   GET32MctrlDual_PerfCounterRst_port5ReqCountRst(r32) _BFGET_(r32,15,15)
    #define   SET32MctrlDual_PerfCounterRst_port5ReqCountRst(r32,v) _BFSET_(r32,15,15,v)
    #define   GET16MctrlDual_PerfCounterRst_port5ReqCountRst(r16) _BFGET_(r16,15,15)
    #define   SET16MctrlDual_PerfCounterRst_port5ReqCountRst(r16,v) _BFSET_(r16,15,15,v)

    #define   GET32MctrlDual_PerfCounterRst_port0FCCountRst(r32) _BFGET_(r32,16,16)
    #define   SET32MctrlDual_PerfCounterRst_port0FCCountRst(r32,v) _BFSET_(r32,16,16,v)
    #define   GET16MctrlDual_PerfCounterRst_port0FCCountRst(r16) _BFGET_(r16, 0, 0)
    #define   SET16MctrlDual_PerfCounterRst_port0FCCountRst(r16,v) _BFSET_(r16, 0, 0,v)

    #define   GET32MctrlDual_PerfCounterRst_port1FCCountRst(r32) _BFGET_(r32,17,17)
    #define   SET32MctrlDual_PerfCounterRst_port1FCCountRst(r32,v) _BFSET_(r32,17,17,v)
    #define   GET16MctrlDual_PerfCounterRst_port1FCCountRst(r16) _BFGET_(r16, 1, 1)
    #define   SET16MctrlDual_PerfCounterRst_port1FCCountRst(r16,v) _BFSET_(r16, 1, 1,v)

    #define   GET32MctrlDual_PerfCounterRst_port2FCCountRst(r32) _BFGET_(r32,18,18)
    #define   SET32MctrlDual_PerfCounterRst_port2FCCountRst(r32,v) _BFSET_(r32,18,18,v)
    #define   GET16MctrlDual_PerfCounterRst_port2FCCountRst(r16) _BFGET_(r16, 2, 2)
    #define   SET16MctrlDual_PerfCounterRst_port2FCCountRst(r16,v) _BFSET_(r16, 2, 2,v)

    #define   GET32MctrlDual_PerfCounterRst_port3FCCountRst(r32) _BFGET_(r32,19,19)
    #define   SET32MctrlDual_PerfCounterRst_port3FCCountRst(r32,v) _BFSET_(r32,19,19,v)
    #define   GET16MctrlDual_PerfCounterRst_port3FCCountRst(r16) _BFGET_(r16, 3, 3)
    #define   SET16MctrlDual_PerfCounterRst_port3FCCountRst(r16,v) _BFSET_(r16, 3, 3,v)

    #define   GET32MctrlDual_PerfCounterRst_port4FCCountRst(r32) _BFGET_(r32,20,20)
    #define   SET32MctrlDual_PerfCounterRst_port4FCCountRst(r32,v) _BFSET_(r32,20,20,v)
    #define   GET16MctrlDual_PerfCounterRst_port4FCCountRst(r16) _BFGET_(r16, 4, 4)
    #define   SET16MctrlDual_PerfCounterRst_port4FCCountRst(r16,v) _BFSET_(r16, 4, 4,v)

    #define   GET32MctrlDual_PerfCounterRst_port5FCCountRst(r32) _BFGET_(r32,21,21)
    #define   SET32MctrlDual_PerfCounterRst_port5FCCountRst(r32,v) _BFSET_(r32,21,21,v)
    #define   GET16MctrlDual_PerfCounterRst_port5FCCountRst(r16) _BFGET_(r16, 5, 5)
    #define   SET16MctrlDual_PerfCounterRst_port5FCCountRst(r16,v) _BFSET_(r16, 5, 5,v)

    #define   GET32MctrlDual_PerfCounterRst_cpu0WSeCountRst(r32) _BFGET_(r32,22,22)
    #define   SET32MctrlDual_PerfCounterRst_cpu0WSeCountRst(r32,v) _BFSET_(r32,22,22,v)
    #define   GET16MctrlDual_PerfCounterRst_cpu0WSeCountRst(r16) _BFGET_(r16, 6, 6)
    #define   SET16MctrlDual_PerfCounterRst_cpu0WSeCountRst(r16,v) _BFSET_(r16, 6, 6,v)

    #define   GET32MctrlDual_PerfCounterRst_cpu0RSeCountRst(r32) _BFGET_(r32,23,23)
    #define   SET32MctrlDual_PerfCounterRst_cpu0RSeCountRst(r32,v) _BFSET_(r32,23,23,v)
    #define   GET16MctrlDual_PerfCounterRst_cpu0RSeCountRst(r16) _BFGET_(r16, 7, 7)
    #define   SET16MctrlDual_PerfCounterRst_cpu0RSeCountRst(r16,v) _BFSET_(r16, 7, 7,v)

    #define   GET32MctrlDual_PerfCounterRst_extMWSeCountRst(r32) _BFGET_(r32,24,24)
    #define   SET32MctrlDual_PerfCounterRst_extMWSeCountRst(r32,v) _BFSET_(r32,24,24,v)
    #define   GET16MctrlDual_PerfCounterRst_extMWSeCountRst(r16) _BFGET_(r16, 8, 8)
    #define   SET16MctrlDual_PerfCounterRst_extMWSeCountRst(r16,v) _BFSET_(r16, 8, 8,v)

    #define   GET32MctrlDual_PerfCounterRst_extMRSeCountRst(r32) _BFGET_(r32,25,25)
    #define   SET32MctrlDual_PerfCounterRst_extMRSeCountRst(r32,v) _BFSET_(r32,25,25,v)
    #define   GET16MctrlDual_PerfCounterRst_extMRSeCountRst(r16) _BFGET_(r16, 9, 9)
    #define   SET16MctrlDual_PerfCounterRst_extMRSeCountRst(r16,v) _BFSET_(r16, 9, 9,v)

    #define     w32MctrlDual_PerfCounterRst                    {\
            UNSG32 uPerfCounterRst_cycleCountRst               :  1;\
            UNSG32 uPerfCounterRst_ctrlIdleCountRst            :  1;\
            UNSG32 uPerfCounterRst_dBusBusyCh0CountRst         :  1;\
            UNSG32 uPerfCounterRst_dBusBusyCh1CountRst         :  1;\
            UNSG32 uPerfCounterRst_busyCh0CountRst             :  1;\
            UNSG32 uPerfCounterRst_busyCh1CountRst             :  1;\
            UNSG32 uPerfCounterRst_cpu0WReqCountRst            :  1;\
            UNSG32 uPerfCounterRst_cpu1WReqCountRst            :  1;\
            UNSG32 uPerfCounterRst_cpu0RReqCountRst            :  1;\
            UNSG32 uPerfCounterRst_cpu1RReqCountRst            :  1;\
            UNSG32 uPerfCounterRst_port0ReqCountRst            :  1;\
            UNSG32 uPerfCounterRst_port1ReqCountRst            :  1;\
            UNSG32 uPerfCounterRst_port2ReqCountRst            :  1;\
            UNSG32 uPerfCounterRst_port3ReqCountRst            :  1;\
            UNSG32 uPerfCounterRst_port4ReqCountRst            :  1;\
            UNSG32 uPerfCounterRst_port5ReqCountRst            :  1;\
            UNSG32 uPerfCounterRst_port0FCCountRst             :  1;\
            UNSG32 uPerfCounterRst_port1FCCountRst             :  1;\
            UNSG32 uPerfCounterRst_port2FCCountRst             :  1;\
            UNSG32 uPerfCounterRst_port3FCCountRst             :  1;\
            UNSG32 uPerfCounterRst_port4FCCountRst             :  1;\
            UNSG32 uPerfCounterRst_port5FCCountRst             :  1;\
            UNSG32 uPerfCounterRst_cpu0WSeCountRst             :  1;\
            UNSG32 uPerfCounterRst_cpu0RSeCountRst             :  1;\
            UNSG32 uPerfCounterRst_extMWSeCountRst             :  1;\
            UNSG32 uPerfCounterRst_extMRSeCountRst             :  1;\
            UNSG32 RSVDx20C_b26                                :  6;\
          }
    union { UNSG32 u32MctrlDual_PerfCounterRst;
            struct w32MctrlDual_PerfCounterRst;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_CycleCount_value(r32)             _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_CycleCount_value(r32,v)           _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_CycleCount                        {\
            UNSG32 uCycleCount_value                           : 32;\
          }
    union { UNSG32 u32MctrlDual_CycleCount;
            struct w32MctrlDual_CycleCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_ReqIdleCount_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_ReqIdleCount_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_ReqIdleCount                      {\
            UNSG32 uReqIdleCount_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_ReqIdleCount;
            struct w32MctrlDual_ReqIdleCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_oneChActCount_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_oneChActCount_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_oneChActCount                     {\
            UNSG32 uoneChActCount_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_oneChActCount;
            struct w32MctrlDual_oneChActCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_DBusyCountCh0_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_DBusyCountCh0_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_DBusyCountCh0                     {\
            UNSG32 uDBusyCountCh0_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_DBusyCountCh0;
            struct w32MctrlDual_DBusyCountCh0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_DBusyCountCh1_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_DBusyCountCh1_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_DBusyCountCh1                     {\
            UNSG32 uDBusyCountCh1_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_DBusyCountCh1;
            struct w32MctrlDual_DBusyCountCh1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_busyCountCh0_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_busyCountCh0_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_busyCountCh0                      {\
            UNSG32 ubusyCountCh0_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_busyCountCh0;
            struct w32MctrlDual_busyCountCh0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_busyCountCh1_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_busyCountCh1_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_busyCountCh1                      {\
            UNSG32 ubusyCountCh1_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_busyCountCh1;
            struct w32MctrlDual_busyCountCh1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_CPU0WReqCount_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_CPU0WReqCount_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_CPU0WReqCount                     {\
            UNSG32 uCPU0WReqCount_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_CPU0WReqCount;
            struct w32MctrlDual_CPU0WReqCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_CPU1WReqCount_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_CPU1WReqCount_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_CPU1WReqCount                     {\
            UNSG32 uCPU1WReqCount_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_CPU1WReqCount;
            struct w32MctrlDual_CPU1WReqCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_CPU0RReqCount_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_CPU0RReqCount_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_CPU0RReqCount                     {\
            UNSG32 uCPU0RReqCount_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_CPU0RReqCount;
            struct w32MctrlDual_CPU0RReqCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_CPU1RReqCount_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_CPU1RReqCount_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_CPU1RReqCount                     {\
            UNSG32 uCPU1RReqCount_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_CPU1RReqCount;
            struct w32MctrlDual_CPU1RReqCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port0ReqCount_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port0ReqCount_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port0ReqCount                     {\
            UNSG32 uPort0ReqCount_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_Port0ReqCount;
            struct w32MctrlDual_Port0ReqCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port1ReqCount_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port1ReqCount_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port1ReqCount                     {\
            UNSG32 uPort1ReqCount_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_Port1ReqCount;
            struct w32MctrlDual_Port1ReqCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port2ReqCount_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port2ReqCount_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port2ReqCount                     {\
            UNSG32 uPort2ReqCount_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_Port2ReqCount;
            struct w32MctrlDual_Port2ReqCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port3ReqCount_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port3ReqCount_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port3ReqCount                     {\
            UNSG32 uPort3ReqCount_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_Port3ReqCount;
            struct w32MctrlDual_Port3ReqCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port4ReqCount_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port4ReqCount_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port4ReqCount                     {\
            UNSG32 uPort4ReqCount_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_Port4ReqCount;
            struct w32MctrlDual_Port4ReqCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port5ReqCount_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port5ReqCount_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port5ReqCount                     {\
            UNSG32 uPort5ReqCount_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_Port5ReqCount;
            struct w32MctrlDual_Port5ReqCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port0FCCount_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port0FCCount_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port0FCCount                      {\
            UNSG32 uPort0FCCount_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_Port0FCCount;
            struct w32MctrlDual_Port0FCCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port1FCCount_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port1FCCount_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port1FCCount                      {\
            UNSG32 uPort1FCCount_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_Port1FCCount;
            struct w32MctrlDual_Port1FCCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port2FCCount_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port2FCCount_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port2FCCount                      {\
            UNSG32 uPort2FCCount_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_Port2FCCount;
            struct w32MctrlDual_Port2FCCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port3FCCount_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port3FCCount_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port3FCCount                      {\
            UNSG32 uPort3FCCount_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_Port3FCCount;
            struct w32MctrlDual_Port3FCCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port4FCCount_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port4FCCount_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port4FCCount                      {\
            UNSG32 uPort4FCCount_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_Port4FCCount;
            struct w32MctrlDual_Port4FCCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Port5FCCount_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Port5FCCount_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Port5FCCount                      {\
            UNSG32 uPort5FCCount_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_Port5FCCount;
            struct w32MctrlDual_Port5FCCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Cpu0WSeCount_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Cpu0WSeCount_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Cpu0WSeCount                      {\
            UNSG32 uCpu0WSeCount_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_Cpu0WSeCount;
            struct w32MctrlDual_Cpu0WSeCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_Cpu0RSeCount_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_Cpu0RSeCount_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_Cpu0RSeCount                      {\
            UNSG32 uCpu0RSeCount_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_Cpu0RSeCount;
            struct w32MctrlDual_Cpu0RSeCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_ExtMWSeCount_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_ExtMWSeCount_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_ExtMWSeCount                      {\
            UNSG32 uExtMWSeCount_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_ExtMWSeCount;
            struct w32MctrlDual_ExtMWSeCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_ExtMRSeCount_value(r32)           _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_ExtMRSeCount_value(r32,v)         _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_ExtMRSeCount                      {\
            UNSG32 uExtMRSeCount_value                         : 32;\
          }
    union { UNSG32 u32MctrlDual_ExtMRSeCount;
            struct w32MctrlDual_ExtMRSeCount;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_tRCDBlockCount0_value(r32)        _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_tRCDBlockCount0_value(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_tRCDBlockCount0                   {\
            UNSG32 utRCDBlockCount0_value                      : 32;\
          }
    union { UNSG32 u32MctrlDual_tRCDBlockCount0;
            struct w32MctrlDual_tRCDBlockCount0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_ACTBlockCount0_value(r32)         _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_ACTBlockCount0_value(r32,v)       _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_ACTBlockCount0                    {\
            UNSG32 uACTBlockCount0_value                       : 32;\
          }
    union { UNSG32 u32MctrlDual_ACTBlockCount0;
            struct w32MctrlDual_ACTBlockCount0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_PREBlockCount0_value(r32)         _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_PREBlockCount0_value(r32,v)       _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_PREBlockCount0                    {\
            UNSG32 uPREBlockCount0_value                       : 32;\
          }
    union { UNSG32 u32MctrlDual_PREBlockCount0;
            struct w32MctrlDual_PREBlockCount0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_RWBlockCount0_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_RWBlockCount0_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_RWBlockCount0                     {\
            UNSG32 uRWBlockCount0_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_RWBlockCount0;
            struct w32MctrlDual_RWBlockCount0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_REFCountCh0_value(r32)            _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_REFCountCh0_value(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_REFCountCh0                       {\
            UNSG32 uREFCountCh0_value                          : 32;\
          }
    union { UNSG32 u32MctrlDual_REFCountCh0;
            struct w32MctrlDual_REFCountCh0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_tRCDBlockCount1_value(r32)        _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_tRCDBlockCount1_value(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_tRCDBlockCount1                   {\
            UNSG32 utRCDBlockCount1_value                      : 32;\
          }
    union { UNSG32 u32MctrlDual_tRCDBlockCount1;
            struct w32MctrlDual_tRCDBlockCount1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_ACTBlockCount1_value(r32)         _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_ACTBlockCount1_value(r32,v)       _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_ACTBlockCount1                    {\
            UNSG32 uACTBlockCount1_value                       : 32;\
          }
    union { UNSG32 u32MctrlDual_ACTBlockCount1;
            struct w32MctrlDual_ACTBlockCount1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_PREBlockCount1_value(r32)         _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_PREBlockCount1_value(r32,v)       _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_PREBlockCount1                    {\
            UNSG32 uPREBlockCount1_value                       : 32;\
          }
    union { UNSG32 u32MctrlDual_PREBlockCount1;
            struct w32MctrlDual_PREBlockCount1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_RWBlockCount1_value(r32)          _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_RWBlockCount1_value(r32,v)        _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_RWBlockCount1                     {\
            UNSG32 uRWBlockCount1_value                        : 32;\
          }
    union { UNSG32 u32MctrlDual_RWBlockCount1;
            struct w32MctrlDual_RWBlockCount1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_REFCountCh1_value(r32)            _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_REFCountCh1_value(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_REFCountCh1                       {\
            UNSG32 uREFCountCh1_value                          : 32;\
          }
    union { UNSG32 u32MctrlDual_REFCountCh1;
            struct w32MctrlDual_REFCountCh1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_rBufBlockCount0_value(r32)        _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_rBufBlockCount0_value(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_rBufBlockCount0                   {\
            UNSG32 urBufBlockCount0_value                      : 32;\
          }
    union { UNSG32 u32MctrlDual_rBufBlockCount0;
            struct w32MctrlDual_rBufBlockCount0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_rBufBlockCount1_value(r32)        _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_rBufBlockCount1_value(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_rBufBlockCount1                   {\
            UNSG32 urBufBlockCount1_value                      : 32;\
          }
    union { UNSG32 u32MctrlDual_rBufBlockCount1;
            struct w32MctrlDual_rBufBlockCount1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_rBufBlockCount2_value(r32)        _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_rBufBlockCount2_value(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_rBufBlockCount2                   {\
            UNSG32 urBufBlockCount2_value                      : 32;\
          }
    union { UNSG32 u32MctrlDual_rBufBlockCount2;
            struct w32MctrlDual_rBufBlockCount2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_GFXCmdGnt_pattern(r32)            _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_GFXCmdGnt_pattern(r32,v)          _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_GFXCmdGnt                         {\
            UNSG32 uGFXCmdGnt_pattern                          : 32;\
          }
    union { UNSG32 u32MctrlDual_GFXCmdGnt;
            struct w32MctrlDual_GFXCmdGnt;
          };
    ///////////////////////////////////////////////////////////
              SIE_seRegion                                     ie_SeCtrlCpu0WMem;
    ///////////////////////////////////////////////////////////
              SIE_seRegion                                     ie_SeCtrlCpu0RMem;
    ///////////////////////////////////////////////////////////
              SIE_seRegion                                     ie_SeCtrlExtMWMem;
    ///////////////////////////////////////////////////////////
              SIE_seRegion                                     ie_SeCtrlExtMRMem;
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackExpLow0_value(r32)        _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_loopBackExpLow0_value(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_loopBackExpLow0                   {\
            UNSG32 uloopBackExpLow0_value                      : 32;\
          }
    union { UNSG32 u32MctrlDual_loopBackExpLow0;
            struct w32MctrlDual_loopBackExpLow0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackExpHigh0_value(r32)       _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_loopBackExpHigh0_value(r32,v)     _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_loopBackExpHigh0                  {\
            UNSG32 uloopBackExpHigh0_value                     : 32;\
          }
    union { UNSG32 u32MctrlDual_loopBackExpHigh0;
            struct w32MctrlDual_loopBackExpHigh0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackRcvLow0_value(r32)        _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_loopBackRcvLow0_value(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_loopBackRcvLow0                   {\
            UNSG32 uloopBackRcvLow0_value                      : 32;\
          }
    union { UNSG32 u32MctrlDual_loopBackRcvLow0;
            struct w32MctrlDual_loopBackRcvLow0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackRcvHigh0_value(r32)       _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_loopBackRcvHigh0_value(r32,v)     _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_loopBackRcvHigh0                  {\
            UNSG32 uloopBackRcvHigh0_value                     : 32;\
          }
    union { UNSG32 u32MctrlDual_loopBackRcvHigh0;
            struct w32MctrlDual_loopBackRcvHigh0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackDQM0_dqm_exp(r32)         _BFGET_(r32, 7, 0)
    #define   SET32MctrlDual_loopBackDQM0_dqm_exp(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16MctrlDual_loopBackDQM0_dqm_exp(r16)         _BFGET_(r16, 7, 0)
    #define   SET16MctrlDual_loopBackDQM0_dqm_exp(r16,v)       _BFSET_(r16, 7, 0,v)

    #define   GET32MctrlDual_loopBackDQM0_dqm_rcv(r32)         _BFGET_(r32,15, 8)
    #define   SET32MctrlDual_loopBackDQM0_dqm_rcv(r32,v)       _BFSET_(r32,15, 8,v)
    #define   GET16MctrlDual_loopBackDQM0_dqm_rcv(r16)         _BFGET_(r16,15, 8)
    #define   SET16MctrlDual_loopBackDQM0_dqm_rcv(r16,v)       _BFSET_(r16,15, 8,v)

    #define     w32MctrlDual_loopBackDQM0                      {\
            UNSG32 uloopBackDQM0_dqm_exp                       :  8;\
            UNSG32 uloopBackDQM0_dqm_rcv                       :  8;\
            UNSG32 RSVDx344_b16                                : 16;\
          }
    union { UNSG32 u32MctrlDual_loopBackDQM0;
            struct w32MctrlDual_loopBackDQM0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackExpAC0_value(r32)         _BFGET_(r32,22, 0)
    #define   SET32MctrlDual_loopBackExpAC0_value(r32,v)       _BFSET_(r32,22, 0,v)

    #define     w32MctrlDual_loopBackExpAC0                    {\
            UNSG32 uloopBackExpAC0_value                       : 23;\
            UNSG32 RSVDx348_b23                                :  9;\
          }
    union { UNSG32 u32MctrlDual_loopBackExpAC0;
            struct w32MctrlDual_loopBackExpAC0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackRcvAC0_value(r32)         _BFGET_(r32,22, 0)
    #define   SET32MctrlDual_loopBackRcvAC0_value(r32,v)       _BFSET_(r32,22, 0,v)

    #define     w32MctrlDual_loopBackRcvAC0                    {\
            UNSG32 uloopBackRcvAC0_value                       : 23;\
            UNSG32 RSVDx34C_b23                                :  9;\
          }
    union { UNSG32 u32MctrlDual_loopBackRcvAC0;
            struct w32MctrlDual_loopBackRcvAC0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackExpLow1_value(r32)        _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_loopBackExpLow1_value(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_loopBackExpLow1                   {\
            UNSG32 uloopBackExpLow1_value                      : 32;\
          }
    union { UNSG32 u32MctrlDual_loopBackExpLow1;
            struct w32MctrlDual_loopBackExpLow1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackExpHigh1_value(r32)       _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_loopBackExpHigh1_value(r32,v)     _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_loopBackExpHigh1                  {\
            UNSG32 uloopBackExpHigh1_value                     : 32;\
          }
    union { UNSG32 u32MctrlDual_loopBackExpHigh1;
            struct w32MctrlDual_loopBackExpHigh1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackRcvLow1_value(r32)        _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_loopBackRcvLow1_value(r32,v)      _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_loopBackRcvLow1                   {\
            UNSG32 uloopBackRcvLow1_value                      : 32;\
          }
    union { UNSG32 u32MctrlDual_loopBackRcvLow1;
            struct w32MctrlDual_loopBackRcvLow1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackRcvHigh1_value(r32)       _BFGET_(r32,31, 0)
    #define   SET32MctrlDual_loopBackRcvHigh1_value(r32,v)     _BFSET_(r32,31, 0,v)

    #define     w32MctrlDual_loopBackRcvHigh1                  {\
            UNSG32 uloopBackRcvHigh1_value                     : 32;\
          }
    union { UNSG32 u32MctrlDual_loopBackRcvHigh1;
            struct w32MctrlDual_loopBackRcvHigh1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackDQM1_dqm_exp(r32)         _BFGET_(r32, 7, 0)
    #define   SET32MctrlDual_loopBackDQM1_dqm_exp(r32,v)       _BFSET_(r32, 7, 0,v)
    #define   GET16MctrlDual_loopBackDQM1_dqm_exp(r16)         _BFGET_(r16, 7, 0)
    #define   SET16MctrlDual_loopBackDQM1_dqm_exp(r16,v)       _BFSET_(r16, 7, 0,v)

    #define   GET32MctrlDual_loopBackDQM1_dqm_rcv(r32)         _BFGET_(r32,15, 8)
    #define   SET32MctrlDual_loopBackDQM1_dqm_rcv(r32,v)       _BFSET_(r32,15, 8,v)
    #define   GET16MctrlDual_loopBackDQM1_dqm_rcv(r16)         _BFGET_(r16,15, 8)
    #define   SET16MctrlDual_loopBackDQM1_dqm_rcv(r16,v)       _BFSET_(r16,15, 8,v)

    #define     w32MctrlDual_loopBackDQM1                      {\
            UNSG32 uloopBackDQM1_dqm_exp                       :  8;\
            UNSG32 uloopBackDQM1_dqm_rcv                       :  8;\
            UNSG32 RSVDx360_b16                                : 16;\
          }
    union { UNSG32 u32MctrlDual_loopBackDQM1;
            struct w32MctrlDual_loopBackDQM1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackExpAC1_value(r32)         _BFGET_(r32,22, 0)
    #define   SET32MctrlDual_loopBackExpAC1_value(r32,v)       _BFSET_(r32,22, 0,v)

    #define     w32MctrlDual_loopBackExpAC1                    {\
            UNSG32 uloopBackExpAC1_value                       : 23;\
            UNSG32 RSVDx364_b23                                :  9;\
          }
    union { UNSG32 u32MctrlDual_loopBackExpAC1;
            struct w32MctrlDual_loopBackExpAC1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32MctrlDual_loopBackRcvAC1_value(r32)         _BFGET_(r32,22, 0)
    #define   SET32MctrlDual_loopBackRcvAC1_value(r32,v)       _BFSET_(r32,22, 0,v)

    #define     w32MctrlDual_loopBackRcvAC1                    {\
            UNSG32 uloopBackRcvAC1_value                       : 23;\
            UNSG32 RSVDx368_b23                                :  9;\
          }
    union { UNSG32 u32MctrlDual_loopBackRcvAC1;
            struct w32MctrlDual_loopBackRcvAC1;
          };
    ///////////////////////////////////////////////////////////
    } SIE_MctrlDual;

    typedef union  T32MctrlDual_CommConfig
          { UNSG32 u32;
            struct w32MctrlDual_CommConfig;
                 } T32MctrlDual_CommConfig;
    typedef union  T32MctrlDual_Trigger
          { UNSG32 u32;
            struct w32MctrlDual_Trigger;
                 } T32MctrlDual_Trigger;
    typedef union  T32MctrlDual_Status
          { UNSG32 u32;
            struct w32MctrlDual_Status;
                 } T32MctrlDual_Status;
    typedef union  T32MctrlDual_SettingChannel0
          { UNSG32 u32;
            struct w32MctrlDual_SettingChannel0;
                 } T32MctrlDual_SettingChannel0;
    typedef union  T32MctrlDual_SettingChannel1
          { UNSG32 u32;
            struct w32MctrlDual_SettingChannel1;
                 } T32MctrlDual_SettingChannel1;
    typedef union  T32MctrlDual_Timing1
          { UNSG32 u32;
            struct w32MctrlDual_Timing1;
                 } T32MctrlDual_Timing1;
    typedef union  T32MctrlDual_Timing2
          { UNSG32 u32;
            struct w32MctrlDual_Timing2;
                 } T32MctrlDual_Timing2;
    typedef union  T32MctrlDual_Timing3
          { UNSG32 u32;
            struct w32MctrlDual_Timing3;
                 } T32MctrlDual_Timing3;
    typedef union  T32MctrlDual_Timing4
          { UNSG32 u32;
            struct w32MctrlDual_Timing4;
                 } T32MctrlDual_Timing4;
    typedef union  T32MctrlDual_Control1
          { UNSG32 u32;
            struct w32MctrlDual_Control1;
                 } T32MctrlDual_Control1;
    typedef union  T32MctrlDual_Control2
          { UNSG32 u32;
            struct w32MctrlDual_Control2;
                 } T32MctrlDual_Control2;
    typedef union  T32MctrlDual_PriorityLevel
          { UNSG32 u32;
            struct w32MctrlDual_PriorityLevel;
                 } T32MctrlDual_PriorityLevel;
    typedef union  T32MctrlDual_TTBStartAddr
          { UNSG32 u32;
            struct w32MctrlDual_TTBStartAddr;
                 } T32MctrlDual_TTBStartAddr;
    typedef union  T32MctrlDual_TTBEndAddr
          { UNSG32 u32;
            struct w32MctrlDual_TTBEndAddr;
                 } T32MctrlDual_TTBEndAddr;
    typedef union  T32MctrlDual_TTBWritePrt
          { UNSG32 u32;
            struct w32MctrlDual_TTBWritePrt;
                 } T32MctrlDual_TTBWritePrt;
    typedef union  T32MctrlDual_DebugTrigger
          { UNSG32 u32;
            struct w32MctrlDual_DebugTrigger;
                 } T32MctrlDual_DebugTrigger;
    typedef union  T32MctrlDual_DebugStAddr
          { UNSG32 u32;
            struct w32MctrlDual_DebugStAddr;
                 } T32MctrlDual_DebugStAddr;
    typedef union  T32MctrlDual_DebugEdAddr
          { UNSG32 u32;
            struct w32MctrlDual_DebugEdAddr;
                 } T32MctrlDual_DebugEdAddr;
    typedef union  T32MctrlDual_DebugStatus
          { UNSG32 u32;
            struct w32MctrlDual_DebugStatus;
                 } T32MctrlDual_DebugStatus;
    typedef union  T32MctrlDual_DebugRaddr
          { UNSG32 u32;
            struct w32MctrlDual_DebugRaddr;
                 } T32MctrlDual_DebugRaddr;
    typedef union  T32MctrlDual_DebugRdata0
          { UNSG32 u32;
            struct w32MctrlDual_DebugRdata0;
                 } T32MctrlDual_DebugRdata0;
    typedef union  T32MctrlDual_DebugRdata1
          { UNSG32 u32;
            struct w32MctrlDual_DebugRdata1;
                 } T32MctrlDual_DebugRdata1;
    typedef union  T32MctrlDual_DebugOData
          { UNSG32 u32;
            struct w32MctrlDual_DebugOData;
                 } T32MctrlDual_DebugOData;
    typedef union  T32MctrlDual_DebugEData
          { UNSG32 u32;
            struct w32MctrlDual_DebugEData;
                 } T32MctrlDual_DebugEData;
    typedef union  T32MctrlDual_InterruptMask
          { UNSG32 u32;
            struct w32MctrlDual_InterruptMask;
                 } T32MctrlDual_InterruptMask;
    typedef union  T32MctrlDual_interruptStatus
          { UNSG32 u32;
            struct w32MctrlDual_interruptStatus;
                 } T32MctrlDual_interruptStatus;
    typedef union  T32MctrlDual_LFSRData
          { UNSG32 u32;
            struct w32MctrlDual_LFSRData;
                 } T32MctrlDual_LFSRData;
    typedef union  T32MctrlDual_PerfCounterRst
          { UNSG32 u32;
            struct w32MctrlDual_PerfCounterRst;
                 } T32MctrlDual_PerfCounterRst;
    typedef union  T32MctrlDual_CycleCount
          { UNSG32 u32;
            struct w32MctrlDual_CycleCount;
                 } T32MctrlDual_CycleCount;
    typedef union  T32MctrlDual_ReqIdleCount
          { UNSG32 u32;
            struct w32MctrlDual_ReqIdleCount;
                 } T32MctrlDual_ReqIdleCount;
    typedef union  T32MctrlDual_oneChActCount
          { UNSG32 u32;
            struct w32MctrlDual_oneChActCount;
                 } T32MctrlDual_oneChActCount;
    typedef union  T32MctrlDual_DBusyCountCh0
          { UNSG32 u32;
            struct w32MctrlDual_DBusyCountCh0;
                 } T32MctrlDual_DBusyCountCh0;
    typedef union  T32MctrlDual_DBusyCountCh1
          { UNSG32 u32;
            struct w32MctrlDual_DBusyCountCh1;
                 } T32MctrlDual_DBusyCountCh1;
    typedef union  T32MctrlDual_busyCountCh0
          { UNSG32 u32;
            struct w32MctrlDual_busyCountCh0;
                 } T32MctrlDual_busyCountCh0;
    typedef union  T32MctrlDual_busyCountCh1
          { UNSG32 u32;
            struct w32MctrlDual_busyCountCh1;
                 } T32MctrlDual_busyCountCh1;
    typedef union  T32MctrlDual_CPU0WReqCount
          { UNSG32 u32;
            struct w32MctrlDual_CPU0WReqCount;
                 } T32MctrlDual_CPU0WReqCount;
    typedef union  T32MctrlDual_CPU1WReqCount
          { UNSG32 u32;
            struct w32MctrlDual_CPU1WReqCount;
                 } T32MctrlDual_CPU1WReqCount;
    typedef union  T32MctrlDual_CPU0RReqCount
          { UNSG32 u32;
            struct w32MctrlDual_CPU0RReqCount;
                 } T32MctrlDual_CPU0RReqCount;
    typedef union  T32MctrlDual_CPU1RReqCount
          { UNSG32 u32;
            struct w32MctrlDual_CPU1RReqCount;
                 } T32MctrlDual_CPU1RReqCount;
    typedef union  T32MctrlDual_Port0ReqCount
          { UNSG32 u32;
            struct w32MctrlDual_Port0ReqCount;
                 } T32MctrlDual_Port0ReqCount;
    typedef union  T32MctrlDual_Port1ReqCount
          { UNSG32 u32;
            struct w32MctrlDual_Port1ReqCount;
                 } T32MctrlDual_Port1ReqCount;
    typedef union  T32MctrlDual_Port2ReqCount
          { UNSG32 u32;
            struct w32MctrlDual_Port2ReqCount;
                 } T32MctrlDual_Port2ReqCount;
    typedef union  T32MctrlDual_Port3ReqCount
          { UNSG32 u32;
            struct w32MctrlDual_Port3ReqCount;
                 } T32MctrlDual_Port3ReqCount;
    typedef union  T32MctrlDual_Port4ReqCount
          { UNSG32 u32;
            struct w32MctrlDual_Port4ReqCount;
                 } T32MctrlDual_Port4ReqCount;
    typedef union  T32MctrlDual_Port5ReqCount
          { UNSG32 u32;
            struct w32MctrlDual_Port5ReqCount;
                 } T32MctrlDual_Port5ReqCount;
    typedef union  T32MctrlDual_Port0FCCount
          { UNSG32 u32;
            struct w32MctrlDual_Port0FCCount;
                 } T32MctrlDual_Port0FCCount;
    typedef union  T32MctrlDual_Port1FCCount
          { UNSG32 u32;
            struct w32MctrlDual_Port1FCCount;
                 } T32MctrlDual_Port1FCCount;
    typedef union  T32MctrlDual_Port2FCCount
          { UNSG32 u32;
            struct w32MctrlDual_Port2FCCount;
                 } T32MctrlDual_Port2FCCount;
    typedef union  T32MctrlDual_Port3FCCount
          { UNSG32 u32;
            struct w32MctrlDual_Port3FCCount;
                 } T32MctrlDual_Port3FCCount;
    typedef union  T32MctrlDual_Port4FCCount
          { UNSG32 u32;
            struct w32MctrlDual_Port4FCCount;
                 } T32MctrlDual_Port4FCCount;
    typedef union  T32MctrlDual_Port5FCCount
          { UNSG32 u32;
            struct w32MctrlDual_Port5FCCount;
                 } T32MctrlDual_Port5FCCount;
    typedef union  T32MctrlDual_Cpu0WSeCount
          { UNSG32 u32;
            struct w32MctrlDual_Cpu0WSeCount;
                 } T32MctrlDual_Cpu0WSeCount;
    typedef union  T32MctrlDual_Cpu0RSeCount
          { UNSG32 u32;
            struct w32MctrlDual_Cpu0RSeCount;
                 } T32MctrlDual_Cpu0RSeCount;
    typedef union  T32MctrlDual_ExtMWSeCount
          { UNSG32 u32;
            struct w32MctrlDual_ExtMWSeCount;
                 } T32MctrlDual_ExtMWSeCount;
    typedef union  T32MctrlDual_ExtMRSeCount
          { UNSG32 u32;
            struct w32MctrlDual_ExtMRSeCount;
                 } T32MctrlDual_ExtMRSeCount;
    typedef union  T32MctrlDual_tRCDBlockCount0
          { UNSG32 u32;
            struct w32MctrlDual_tRCDBlockCount0;
                 } T32MctrlDual_tRCDBlockCount0;
    typedef union  T32MctrlDual_ACTBlockCount0
          { UNSG32 u32;
            struct w32MctrlDual_ACTBlockCount0;
                 } T32MctrlDual_ACTBlockCount0;
    typedef union  T32MctrlDual_PREBlockCount0
          { UNSG32 u32;
            struct w32MctrlDual_PREBlockCount0;
                 } T32MctrlDual_PREBlockCount0;
    typedef union  T32MctrlDual_RWBlockCount0
          { UNSG32 u32;
            struct w32MctrlDual_RWBlockCount0;
                 } T32MctrlDual_RWBlockCount0;
    typedef union  T32MctrlDual_REFCountCh0
          { UNSG32 u32;
            struct w32MctrlDual_REFCountCh0;
                 } T32MctrlDual_REFCountCh0;
    typedef union  T32MctrlDual_tRCDBlockCount1
          { UNSG32 u32;
            struct w32MctrlDual_tRCDBlockCount1;
                 } T32MctrlDual_tRCDBlockCount1;
    typedef union  T32MctrlDual_ACTBlockCount1
          { UNSG32 u32;
            struct w32MctrlDual_ACTBlockCount1;
                 } T32MctrlDual_ACTBlockCount1;
    typedef union  T32MctrlDual_PREBlockCount1
          { UNSG32 u32;
            struct w32MctrlDual_PREBlockCount1;
                 } T32MctrlDual_PREBlockCount1;
    typedef union  T32MctrlDual_RWBlockCount1
          { UNSG32 u32;
            struct w32MctrlDual_RWBlockCount1;
                 } T32MctrlDual_RWBlockCount1;
    typedef union  T32MctrlDual_REFCountCh1
          { UNSG32 u32;
            struct w32MctrlDual_REFCountCh1;
                 } T32MctrlDual_REFCountCh1;
    typedef union  T32MctrlDual_rBufBlockCount0
          { UNSG32 u32;
            struct w32MctrlDual_rBufBlockCount0;
                 } T32MctrlDual_rBufBlockCount0;
    typedef union  T32MctrlDual_rBufBlockCount1
          { UNSG32 u32;
            struct w32MctrlDual_rBufBlockCount1;
                 } T32MctrlDual_rBufBlockCount1;
    typedef union  T32MctrlDual_rBufBlockCount2
          { UNSG32 u32;
            struct w32MctrlDual_rBufBlockCount2;
                 } T32MctrlDual_rBufBlockCount2;
    typedef union  T32MctrlDual_GFXCmdGnt
          { UNSG32 u32;
            struct w32MctrlDual_GFXCmdGnt;
                 } T32MctrlDual_GFXCmdGnt;
    typedef union  T32MctrlDual_loopBackExpLow0
          { UNSG32 u32;
            struct w32MctrlDual_loopBackExpLow0;
                 } T32MctrlDual_loopBackExpLow0;
    typedef union  T32MctrlDual_loopBackExpHigh0
          { UNSG32 u32;
            struct w32MctrlDual_loopBackExpHigh0;
                 } T32MctrlDual_loopBackExpHigh0;
    typedef union  T32MctrlDual_loopBackRcvLow0
          { UNSG32 u32;
            struct w32MctrlDual_loopBackRcvLow0;
                 } T32MctrlDual_loopBackRcvLow0;
    typedef union  T32MctrlDual_loopBackRcvHigh0
          { UNSG32 u32;
            struct w32MctrlDual_loopBackRcvHigh0;
                 } T32MctrlDual_loopBackRcvHigh0;
    typedef union  T32MctrlDual_loopBackDQM0
          { UNSG32 u32;
            struct w32MctrlDual_loopBackDQM0;
                 } T32MctrlDual_loopBackDQM0;
    typedef union  T32MctrlDual_loopBackExpAC0
          { UNSG32 u32;
            struct w32MctrlDual_loopBackExpAC0;
                 } T32MctrlDual_loopBackExpAC0;
    typedef union  T32MctrlDual_loopBackRcvAC0
          { UNSG32 u32;
            struct w32MctrlDual_loopBackRcvAC0;
                 } T32MctrlDual_loopBackRcvAC0;
    typedef union  T32MctrlDual_loopBackExpLow1
          { UNSG32 u32;
            struct w32MctrlDual_loopBackExpLow1;
                 } T32MctrlDual_loopBackExpLow1;
    typedef union  T32MctrlDual_loopBackExpHigh1
          { UNSG32 u32;
            struct w32MctrlDual_loopBackExpHigh1;
                 } T32MctrlDual_loopBackExpHigh1;
    typedef union  T32MctrlDual_loopBackRcvLow1
          { UNSG32 u32;
            struct w32MctrlDual_loopBackRcvLow1;
                 } T32MctrlDual_loopBackRcvLow1;
    typedef union  T32MctrlDual_loopBackRcvHigh1
          { UNSG32 u32;
            struct w32MctrlDual_loopBackRcvHigh1;
                 } T32MctrlDual_loopBackRcvHigh1;
    typedef union  T32MctrlDual_loopBackDQM1
          { UNSG32 u32;
            struct w32MctrlDual_loopBackDQM1;
                 } T32MctrlDual_loopBackDQM1;
    typedef union  T32MctrlDual_loopBackExpAC1
          { UNSG32 u32;
            struct w32MctrlDual_loopBackExpAC1;
                 } T32MctrlDual_loopBackExpAC1;
    typedef union  T32MctrlDual_loopBackRcvAC1
          { UNSG32 u32;
            struct w32MctrlDual_loopBackRcvAC1;
                 } T32MctrlDual_loopBackRcvAC1;
    ///////////////////////////////////////////////////////////

    typedef union  TMctrlDual_CommConfig
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_CommConfig;
                   };
                 } TMctrlDual_CommConfig;
    typedef union  TMctrlDual_Trigger
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Trigger;
                   };
                 } TMctrlDual_Trigger;
    typedef union  TMctrlDual_Status
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Status;
                   };
                 } TMctrlDual_Status;
    typedef union  TMctrlDual_SettingChannel0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_SettingChannel0;
                   };
                 } TMctrlDual_SettingChannel0;
    typedef union  TMctrlDual_SettingChannel1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_SettingChannel1;
                   };
                 } TMctrlDual_SettingChannel1;
    typedef union  TMctrlDual_Timing1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Timing1;
                   };
                 } TMctrlDual_Timing1;
    typedef union  TMctrlDual_Timing2
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Timing2;
                   };
                 } TMctrlDual_Timing2;
    typedef union  TMctrlDual_Timing3
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Timing3;
                   };
                 } TMctrlDual_Timing3;
    typedef union  TMctrlDual_Timing4
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Timing4;
                   };
                 } TMctrlDual_Timing4;
    typedef union  TMctrlDual_Control1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Control1;
                   };
                 } TMctrlDual_Control1;
    typedef union  TMctrlDual_Control2
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Control2;
                   };
                 } TMctrlDual_Control2;
    typedef union  TMctrlDual_PriorityLevel
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_PriorityLevel;
                   };
                 } TMctrlDual_PriorityLevel;
    typedef union  TMctrlDual_TTBStartAddr
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_TTBStartAddr;
                   };
                 } TMctrlDual_TTBStartAddr;
    typedef union  TMctrlDual_TTBEndAddr
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_TTBEndAddr;
                   };
                 } TMctrlDual_TTBEndAddr;
    typedef union  TMctrlDual_TTBWritePrt
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_TTBWritePrt;
                   };
                 } TMctrlDual_TTBWritePrt;
    typedef union  TMctrlDual_DebugTrigger
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_DebugTrigger;
                   };
                 } TMctrlDual_DebugTrigger;
    typedef union  TMctrlDual_DebugStAddr
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_DebugStAddr;
                   };
                 } TMctrlDual_DebugStAddr;
    typedef union  TMctrlDual_DebugEdAddr
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_DebugEdAddr;
                   };
                 } TMctrlDual_DebugEdAddr;
    typedef union  TMctrlDual_DebugStatus
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_DebugStatus;
                   };
                 } TMctrlDual_DebugStatus;
    typedef union  TMctrlDual_DebugRaddr
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_DebugRaddr;
                   };
                 } TMctrlDual_DebugRaddr;
    typedef union  TMctrlDual_DebugRdata0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_DebugRdata0;
                   };
                 } TMctrlDual_DebugRdata0;
    typedef union  TMctrlDual_DebugRdata1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_DebugRdata1;
                   };
                 } TMctrlDual_DebugRdata1;
    typedef union  TMctrlDual_DebugOData
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_DebugOData;
                   };
                 } TMctrlDual_DebugOData;
    typedef union  TMctrlDual_DebugEData
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_DebugEData;
                   };
                 } TMctrlDual_DebugEData;
    typedef union  TMctrlDual_InterruptMask
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_InterruptMask;
                   };
                 } TMctrlDual_InterruptMask;
    typedef union  TMctrlDual_interruptStatus
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_interruptStatus;
                   };
                 } TMctrlDual_interruptStatus;
    typedef union  TMctrlDual_LFSRData
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_LFSRData;
                   };
                 } TMctrlDual_LFSRData;
    typedef union  TMctrlDual_PerfCounterRst
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_PerfCounterRst;
                   };
                 } TMctrlDual_PerfCounterRst;
    typedef union  TMctrlDual_CycleCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_CycleCount;
                   };
                 } TMctrlDual_CycleCount;
    typedef union  TMctrlDual_ReqIdleCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_ReqIdleCount;
                   };
                 } TMctrlDual_ReqIdleCount;
    typedef union  TMctrlDual_oneChActCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_oneChActCount;
                   };
                 } TMctrlDual_oneChActCount;
    typedef union  TMctrlDual_DBusyCountCh0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_DBusyCountCh0;
                   };
                 } TMctrlDual_DBusyCountCh0;
    typedef union  TMctrlDual_DBusyCountCh1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_DBusyCountCh1;
                   };
                 } TMctrlDual_DBusyCountCh1;
    typedef union  TMctrlDual_busyCountCh0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_busyCountCh0;
                   };
                 } TMctrlDual_busyCountCh0;
    typedef union  TMctrlDual_busyCountCh1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_busyCountCh1;
                   };
                 } TMctrlDual_busyCountCh1;
    typedef union  TMctrlDual_CPU0WReqCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_CPU0WReqCount;
                   };
                 } TMctrlDual_CPU0WReqCount;
    typedef union  TMctrlDual_CPU1WReqCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_CPU1WReqCount;
                   };
                 } TMctrlDual_CPU1WReqCount;
    typedef union  TMctrlDual_CPU0RReqCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_CPU0RReqCount;
                   };
                 } TMctrlDual_CPU0RReqCount;
    typedef union  TMctrlDual_CPU1RReqCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_CPU1RReqCount;
                   };
                 } TMctrlDual_CPU1RReqCount;
    typedef union  TMctrlDual_Port0ReqCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port0ReqCount;
                   };
                 } TMctrlDual_Port0ReqCount;
    typedef union  TMctrlDual_Port1ReqCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port1ReqCount;
                   };
                 } TMctrlDual_Port1ReqCount;
    typedef union  TMctrlDual_Port2ReqCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port2ReqCount;
                   };
                 } TMctrlDual_Port2ReqCount;
    typedef union  TMctrlDual_Port3ReqCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port3ReqCount;
                   };
                 } TMctrlDual_Port3ReqCount;
    typedef union  TMctrlDual_Port4ReqCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port4ReqCount;
                   };
                 } TMctrlDual_Port4ReqCount;
    typedef union  TMctrlDual_Port5ReqCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port5ReqCount;
                   };
                 } TMctrlDual_Port5ReqCount;
    typedef union  TMctrlDual_Port0FCCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port0FCCount;
                   };
                 } TMctrlDual_Port0FCCount;
    typedef union  TMctrlDual_Port1FCCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port1FCCount;
                   };
                 } TMctrlDual_Port1FCCount;
    typedef union  TMctrlDual_Port2FCCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port2FCCount;
                   };
                 } TMctrlDual_Port2FCCount;
    typedef union  TMctrlDual_Port3FCCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port3FCCount;
                   };
                 } TMctrlDual_Port3FCCount;
    typedef union  TMctrlDual_Port4FCCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port4FCCount;
                   };
                 } TMctrlDual_Port4FCCount;
    typedef union  TMctrlDual_Port5FCCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Port5FCCount;
                   };
                 } TMctrlDual_Port5FCCount;
    typedef union  TMctrlDual_Cpu0WSeCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Cpu0WSeCount;
                   };
                 } TMctrlDual_Cpu0WSeCount;
    typedef union  TMctrlDual_Cpu0RSeCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_Cpu0RSeCount;
                   };
                 } TMctrlDual_Cpu0RSeCount;
    typedef union  TMctrlDual_ExtMWSeCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_ExtMWSeCount;
                   };
                 } TMctrlDual_ExtMWSeCount;
    typedef union  TMctrlDual_ExtMRSeCount
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_ExtMRSeCount;
                   };
                 } TMctrlDual_ExtMRSeCount;
    typedef union  TMctrlDual_tRCDBlockCount0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_tRCDBlockCount0;
                   };
                 } TMctrlDual_tRCDBlockCount0;
    typedef union  TMctrlDual_ACTBlockCount0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_ACTBlockCount0;
                   };
                 } TMctrlDual_ACTBlockCount0;
    typedef union  TMctrlDual_PREBlockCount0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_PREBlockCount0;
                   };
                 } TMctrlDual_PREBlockCount0;
    typedef union  TMctrlDual_RWBlockCount0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_RWBlockCount0;
                   };
                 } TMctrlDual_RWBlockCount0;
    typedef union  TMctrlDual_REFCountCh0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_REFCountCh0;
                   };
                 } TMctrlDual_REFCountCh0;
    typedef union  TMctrlDual_tRCDBlockCount1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_tRCDBlockCount1;
                   };
                 } TMctrlDual_tRCDBlockCount1;
    typedef union  TMctrlDual_ACTBlockCount1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_ACTBlockCount1;
                   };
                 } TMctrlDual_ACTBlockCount1;
    typedef union  TMctrlDual_PREBlockCount1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_PREBlockCount1;
                   };
                 } TMctrlDual_PREBlockCount1;
    typedef union  TMctrlDual_RWBlockCount1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_RWBlockCount1;
                   };
                 } TMctrlDual_RWBlockCount1;
    typedef union  TMctrlDual_REFCountCh1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_REFCountCh1;
                   };
                 } TMctrlDual_REFCountCh1;
    typedef union  TMctrlDual_rBufBlockCount0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_rBufBlockCount0;
                   };
                 } TMctrlDual_rBufBlockCount0;
    typedef union  TMctrlDual_rBufBlockCount1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_rBufBlockCount1;
                   };
                 } TMctrlDual_rBufBlockCount1;
    typedef union  TMctrlDual_rBufBlockCount2
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_rBufBlockCount2;
                   };
                 } TMctrlDual_rBufBlockCount2;
    typedef union  TMctrlDual_GFXCmdGnt
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_GFXCmdGnt;
                   };
                 } TMctrlDual_GFXCmdGnt;
    typedef union  TMctrlDual_loopBackExpLow0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackExpLow0;
                   };
                 } TMctrlDual_loopBackExpLow0;
    typedef union  TMctrlDual_loopBackExpHigh0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackExpHigh0;
                   };
                 } TMctrlDual_loopBackExpHigh0;
    typedef union  TMctrlDual_loopBackRcvLow0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackRcvLow0;
                   };
                 } TMctrlDual_loopBackRcvLow0;
    typedef union  TMctrlDual_loopBackRcvHigh0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackRcvHigh0;
                   };
                 } TMctrlDual_loopBackRcvHigh0;
    typedef union  TMctrlDual_loopBackDQM0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackDQM0;
                   };
                 } TMctrlDual_loopBackDQM0;
    typedef union  TMctrlDual_loopBackExpAC0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackExpAC0;
                   };
                 } TMctrlDual_loopBackExpAC0;
    typedef union  TMctrlDual_loopBackRcvAC0
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackRcvAC0;
                   };
                 } TMctrlDual_loopBackRcvAC0;
    typedef union  TMctrlDual_loopBackExpLow1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackExpLow1;
                   };
                 } TMctrlDual_loopBackExpLow1;
    typedef union  TMctrlDual_loopBackExpHigh1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackExpHigh1;
                   };
                 } TMctrlDual_loopBackExpHigh1;
    typedef union  TMctrlDual_loopBackRcvLow1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackRcvLow1;
                   };
                 } TMctrlDual_loopBackRcvLow1;
    typedef union  TMctrlDual_loopBackRcvHigh1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackRcvHigh1;
                   };
                 } TMctrlDual_loopBackRcvHigh1;
    typedef union  TMctrlDual_loopBackDQM1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackDQM1;
                   };
                 } TMctrlDual_loopBackDQM1;
    typedef union  TMctrlDual_loopBackExpAC1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackExpAC1;
                   };
                 } TMctrlDual_loopBackExpAC1;
    typedef union  TMctrlDual_loopBackRcvAC1
          { UNSG32 u32[1];
            struct {
            struct w32MctrlDual_loopBackRcvAC1;
                   };
                 } TMctrlDual_loopBackRcvAC1;

    ///////////////////////////////////////////////////////////
     SIGN32 MctrlDual_drvrd(SIE_MctrlDual *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 MctrlDual_drvwr(SIE_MctrlDual *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void MctrlDual_reset(SIE_MctrlDual *p);
     SIGN32 MctrlDual_cmp  (SIE_MctrlDual *p, SIE_MctrlDual *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define MctrlDual_check(p,pie,pfx,hLOG) MctrlDual_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define MctrlDual_print(p,    pfx,hLOG) MctrlDual_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif

