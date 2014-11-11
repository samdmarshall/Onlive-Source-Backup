#ifndef SecureAccess_h
#define SecureAccess_h (){}


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



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif

