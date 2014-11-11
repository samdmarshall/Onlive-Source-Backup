#ifndef VideoOverlay_h
#define VideoOverlay_h (){}


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



#ifndef h_VQUAD
#define h_VQUAD (){}

    #define     RA_VQUAD_AYUV                                  0x0000

    #define     BA_VQUAD_AYUV_A                                0x0000
    #define     B16VQUAD_AYUV_A                                0x0000
    #define   LSb32VQUAD_AYUV_A                                   0
    #define   LSb16VQUAD_AYUV_A                                   0
    #define       bVQUAD_AYUV_A                                8
    #define   MSK32VQUAD_AYUV_A                                   0x000000FF

    #define     BA_VQUAD_AYUV_Y                                0x0001
    #define     B16VQUAD_AYUV_Y                                0x0000
    #define   LSb32VQUAD_AYUV_Y                                   8
    #define   LSb16VQUAD_AYUV_Y                                   8
    #define       bVQUAD_AYUV_Y                                8
    #define   MSK32VQUAD_AYUV_Y                                   0x0000FF00

    #define     BA_VQUAD_AYUV_U                                0x0002
    #define     B16VQUAD_AYUV_U                                0x0002
    #define   LSb32VQUAD_AYUV_U                                   16
    #define   LSb16VQUAD_AYUV_U                                   0
    #define       bVQUAD_AYUV_U                                8
    #define   MSK32VQUAD_AYUV_U                                   0x00FF0000

    #define     BA_VQUAD_AYUV_V                                0x0003
    #define     B16VQUAD_AYUV_V                                0x0002
    #define   LSb32VQUAD_AYUV_V                                   24
    #define   LSb16VQUAD_AYUV_V                                   8
    #define       bVQUAD_AYUV_V                                8
    #define   MSK32VQUAD_AYUV_V                                   0xFF000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_VQUAD {
    ///////////////////////////////////////////////////////////
    #define   GET32VQUAD_AYUV_A(r32)                           _BFGET_(r32, 7, 0)
    #define   SET32VQUAD_AYUV_A(r32,v)                         _BFSET_(r32, 7, 0,v)
    #define   GET16VQUAD_AYUV_A(r16)                           _BFGET_(r16, 7, 0)
    #define   SET16VQUAD_AYUV_A(r16,v)                         _BFSET_(r16, 7, 0,v)

    #define   GET32VQUAD_AYUV_Y(r32)                           _BFGET_(r32,15, 8)
    #define   SET32VQUAD_AYUV_Y(r32,v)                         _BFSET_(r32,15, 8,v)
    #define   GET16VQUAD_AYUV_Y(r16)                           _BFGET_(r16,15, 8)
    #define   SET16VQUAD_AYUV_Y(r16,v)                         _BFSET_(r16,15, 8,v)

    #define   GET32VQUAD_AYUV_U(r32)                           _BFGET_(r32,23,16)
    #define   SET32VQUAD_AYUV_U(r32,v)                         _BFSET_(r32,23,16,v)
    #define   GET16VQUAD_AYUV_U(r16)                           _BFGET_(r16, 7, 0)
    #define   SET16VQUAD_AYUV_U(r16,v)                         _BFSET_(r16, 7, 0,v)

    #define   GET32VQUAD_AYUV_V(r32)                           _BFGET_(r32,31,24)
    #define   SET32VQUAD_AYUV_V(r32,v)                         _BFSET_(r32,31,24,v)
    #define   GET16VQUAD_AYUV_V(r16)                           _BFGET_(r16,15, 8)
    #define   SET16VQUAD_AYUV_V(r16,v)                         _BFSET_(r16,15, 8,v)

    #define     w32VQUAD_AYUV                                  {\
            UNSG32 uAYUV_A                                     :  8;\
            UNSG32 uAYUV_Y                                     :  8;\
            UNSG32 uAYUV_U                                     :  8;\
            UNSG32 uAYUV_V                                     :  8;\
          }
    union { UNSG32 u32VQUAD_AYUV;
            struct w32VQUAD_AYUV;
          };
    ///////////////////////////////////////////////////////////
    } SIE_VQUAD;

    typedef union  T32VQUAD_AYUV
          { UNSG32 u32;
            struct w32VQUAD_AYUV;
                 } T32VQUAD_AYUV;
    ///////////////////////////////////////////////////////////

    typedef union  TVQUAD_AYUV
          { UNSG32 u32[1];
            struct {
            struct w32VQUAD_AYUV;
                   };
                 } TVQUAD_AYUV;

    ///////////////////////////////////////////////////////////
     SIGN32 VQUAD_drvrd(SIE_VQUAD *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 VQUAD_drvwr(SIE_VQUAD *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void VQUAD_reset(SIE_VQUAD *p);
     SIGN32 VQUAD_cmp  (SIE_VQUAD *p, SIE_VQUAD *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define VQUAD_check(p,pie,pfx,hLOG) VQUAD_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define VQUAD_print(p,    pfx,hLOG) VQUAD_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_FmtCnvt
#define h_FmtCnvt (){}

    #define     RA_FmtCnvt_VLUT                                0x0000
    #define     RA_FmtCnvt_palette                             0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_ccm00                               0x0400

    #define     BA_FmtCnvt_ccm00_v                             0x0400
    #define     B16FmtCnvt_ccm00_v                             0x0400
    #define   LSb32FmtCnvt_ccm00_v                                0
    #define   LSb16FmtCnvt_ccm00_v                                0
    #define       bFmtCnvt_ccm00_v                             11
    #define   MSK32FmtCnvt_ccm00_v                                0x000007FF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_ccm01                               0x0404

    #define     BA_FmtCnvt_ccm01_v                             0x0404
    #define     B16FmtCnvt_ccm01_v                             0x0404
    #define   LSb32FmtCnvt_ccm01_v                                0
    #define   LSb16FmtCnvt_ccm01_v                                0
    #define       bFmtCnvt_ccm01_v                             11
    #define   MSK32FmtCnvt_ccm01_v                                0x000007FF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_ccm02                               0x0408

    #define     BA_FmtCnvt_ccm02_v                             0x0408
    #define     B16FmtCnvt_ccm02_v                             0x0408
    #define   LSb32FmtCnvt_ccm02_v                                0
    #define   LSb16FmtCnvt_ccm02_v                                0
    #define       bFmtCnvt_ccm02_v                             11
    #define   MSK32FmtCnvt_ccm02_v                                0x000007FF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_ccm10                               0x040C

    #define     BA_FmtCnvt_ccm10_v                             0x040C
    #define     B16FmtCnvt_ccm10_v                             0x040C
    #define   LSb32FmtCnvt_ccm10_v                                0
    #define   LSb16FmtCnvt_ccm10_v                                0
    #define       bFmtCnvt_ccm10_v                             11
    #define   MSK32FmtCnvt_ccm10_v                                0x000007FF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_ccm11                               0x0410

    #define     BA_FmtCnvt_ccm11_v                             0x0410
    #define     B16FmtCnvt_ccm11_v                             0x0410
    #define   LSb32FmtCnvt_ccm11_v                                0
    #define   LSb16FmtCnvt_ccm11_v                                0
    #define       bFmtCnvt_ccm11_v                             11
    #define   MSK32FmtCnvt_ccm11_v                                0x000007FF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_ccm12                               0x0414

    #define     BA_FmtCnvt_ccm12_v                             0x0414
    #define     B16FmtCnvt_ccm12_v                             0x0414
    #define   LSb32FmtCnvt_ccm12_v                                0
    #define   LSb16FmtCnvt_ccm12_v                                0
    #define       bFmtCnvt_ccm12_v                             11
    #define   MSK32FmtCnvt_ccm12_v                                0x000007FF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_ccm20                               0x0418

    #define     BA_FmtCnvt_ccm20_v                             0x0418
    #define     B16FmtCnvt_ccm20_v                             0x0418
    #define   LSb32FmtCnvt_ccm20_v                                0
    #define   LSb16FmtCnvt_ccm20_v                                0
    #define       bFmtCnvt_ccm20_v                             11
    #define   MSK32FmtCnvt_ccm20_v                                0x000007FF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_ccm21                               0x041C

    #define     BA_FmtCnvt_ccm21_v                             0x041C
    #define     B16FmtCnvt_ccm21_v                             0x041C
    #define   LSb32FmtCnvt_ccm21_v                                0
    #define   LSb16FmtCnvt_ccm21_v                                0
    #define       bFmtCnvt_ccm21_v                             11
    #define   MSK32FmtCnvt_ccm21_v                                0x000007FF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_ccm22                               0x0420

    #define     BA_FmtCnvt_ccm22_v                             0x0420
    #define     B16FmtCnvt_ccm22_v                             0x0420
    #define   LSb32FmtCnvt_ccm22_v                                0
    #define   LSb16FmtCnvt_ccm22_v                                0
    #define       bFmtCnvt_ccm22_v                             11
    #define   MSK32FmtCnvt_ccm22_v                                0x000007FF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_cca0                                0x0424

    #define     BA_FmtCnvt_cca0_v                              0x0424
    #define     B16FmtCnvt_cca0_v                              0x0424
    #define   LSb32FmtCnvt_cca0_v                                 0
    #define   LSb16FmtCnvt_cca0_v                                 0
    #define       bFmtCnvt_cca0_v                              13
    #define   MSK32FmtCnvt_cca0_v                                 0x00001FFF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_cca1                                0x0428

    #define     BA_FmtCnvt_cca1_v                              0x0428
    #define     B16FmtCnvt_cca1_v                              0x0428
    #define   LSb32FmtCnvt_cca1_v                                 0
    #define   LSb16FmtCnvt_cca1_v                                 0
    #define       bFmtCnvt_cca1_v                              13
    #define   MSK32FmtCnvt_cca1_v                                 0x00001FFF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_cca2                                0x042C

    #define     BA_FmtCnvt_cca2_v                              0x042C
    #define     B16FmtCnvt_cca2_v                              0x042C
    #define   LSb32FmtCnvt_cca2_v                                 0
    #define   LSb16FmtCnvt_cca2_v                                 0
    #define       bFmtCnvt_cca2_v                              13
    #define   MSK32FmtCnvt_cca2_v                                 0x00001FFF
    ///////////////////////////////////////////////////////////
    #define     RA_FmtCnvt_input                               0x0430

    #define     BA_FmtCnvt_input_format                        0x0430
    #define     B16FmtCnvt_input_format                        0x0430
    #define   LSb32FmtCnvt_input_format                           0
    #define   LSb16FmtCnvt_input_format                           0
    #define       bFmtCnvt_input_format                        4
    #define   MSK32FmtCnvt_input_format                           0x0000000F
    #define        FmtCnvt_input_format_LUT8                                0x0
    #define        FmtCnvt_input_format_UYVY                                0x1
    #define        FmtCnvt_input_format_AYUV                                0x2
    #define        FmtCnvt_input_format_ARGB                                0x3
    #define        FmtCnvt_input_format_XRGB                                0x4
    #define        FmtCnvt_input_format_ARGB4444                            0x5
    #define        FmtCnvt_input_format_ARGB1555                            0x6
    #define        FmtCnvt_input_format_RGB565                              0x7

    #define     BA_FmtCnvt_input_order                         0x0430
    #define     B16FmtCnvt_input_order                         0x0430
    #define   LSb32FmtCnvt_input_order                            4
    #define   LSb16FmtCnvt_input_order                            4
    #define       bFmtCnvt_input_order                         2
    #define   MSK32FmtCnvt_input_order                            0x00000030
    #define        FmtCnvt_input_order_UYVY                                 0x0
    #define        FmtCnvt_input_order_YUYV                                 0x1
    #define        FmtCnvt_input_order_ARGB                                 0x0
    #define        FmtCnvt_input_order_ABGR                                 0x1
    #define        FmtCnvt_input_order_RGBA                                 0x2
    #define        FmtCnvt_input_order_BGRA                                 0x3
    ///////////////////////////////////////////////////////////

    typedef struct SIE_FmtCnvt {
    ///////////////////////////////////////////////////////////
              SIE_VQUAD                                        ie_palette[256];
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_ccm00_v(r32)                        _BFGET_(r32,10, 0)
    #define   SET32FmtCnvt_ccm00_v(r32,v)                      _BFSET_(r32,10, 0,v)
    #define   GET16FmtCnvt_ccm00_v(r16)                        _BFGET_(r16,10, 0)
    #define   SET16FmtCnvt_ccm00_v(r16,v)                      _BFSET_(r16,10, 0,v)

    #define     w32FmtCnvt_ccm00                               {\
            UNSG32 mccm00_v                                    : 11;\
            UNSG32 RSVDx400_b11                                : 21;\
          }
    union { UNSG32 u32FmtCnvt_ccm00;
            struct w32FmtCnvt_ccm00;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_ccm01_v(r32)                        _BFGET_(r32,10, 0)
    #define   SET32FmtCnvt_ccm01_v(r32,v)                      _BFSET_(r32,10, 0,v)
    #define   GET16FmtCnvt_ccm01_v(r16)                        _BFGET_(r16,10, 0)
    #define   SET16FmtCnvt_ccm01_v(r16,v)                      _BFSET_(r16,10, 0,v)

    #define     w32FmtCnvt_ccm01                               {\
            UNSG32 mccm01_v                                    : 11;\
            UNSG32 RSVDx404_b11                                : 21;\
          }
    union { UNSG32 u32FmtCnvt_ccm01;
            struct w32FmtCnvt_ccm01;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_ccm02_v(r32)                        _BFGET_(r32,10, 0)
    #define   SET32FmtCnvt_ccm02_v(r32,v)                      _BFSET_(r32,10, 0,v)
    #define   GET16FmtCnvt_ccm02_v(r16)                        _BFGET_(r16,10, 0)
    #define   SET16FmtCnvt_ccm02_v(r16,v)                      _BFSET_(r16,10, 0,v)

    #define     w32FmtCnvt_ccm02                               {\
            UNSG32 mccm02_v                                    : 11;\
            UNSG32 RSVDx408_b11                                : 21;\
          }
    union { UNSG32 u32FmtCnvt_ccm02;
            struct w32FmtCnvt_ccm02;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_ccm10_v(r32)                        _BFGET_(r32,10, 0)
    #define   SET32FmtCnvt_ccm10_v(r32,v)                      _BFSET_(r32,10, 0,v)
    #define   GET16FmtCnvt_ccm10_v(r16)                        _BFGET_(r16,10, 0)
    #define   SET16FmtCnvt_ccm10_v(r16,v)                      _BFSET_(r16,10, 0,v)

    #define     w32FmtCnvt_ccm10                               {\
            UNSG32 mccm10_v                                    : 11;\
            UNSG32 RSVDx40C_b11                                : 21;\
          }
    union { UNSG32 u32FmtCnvt_ccm10;
            struct w32FmtCnvt_ccm10;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_ccm11_v(r32)                        _BFGET_(r32,10, 0)
    #define   SET32FmtCnvt_ccm11_v(r32,v)                      _BFSET_(r32,10, 0,v)
    #define   GET16FmtCnvt_ccm11_v(r16)                        _BFGET_(r16,10, 0)
    #define   SET16FmtCnvt_ccm11_v(r16,v)                      _BFSET_(r16,10, 0,v)

    #define     w32FmtCnvt_ccm11                               {\
            UNSG32 mccm11_v                                    : 11;\
            UNSG32 RSVDx410_b11                                : 21;\
          }
    union { UNSG32 u32FmtCnvt_ccm11;
            struct w32FmtCnvt_ccm11;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_ccm12_v(r32)                        _BFGET_(r32,10, 0)
    #define   SET32FmtCnvt_ccm12_v(r32,v)                      _BFSET_(r32,10, 0,v)
    #define   GET16FmtCnvt_ccm12_v(r16)                        _BFGET_(r16,10, 0)
    #define   SET16FmtCnvt_ccm12_v(r16,v)                      _BFSET_(r16,10, 0,v)

    #define     w32FmtCnvt_ccm12                               {\
            UNSG32 mccm12_v                                    : 11;\
            UNSG32 RSVDx414_b11                                : 21;\
          }
    union { UNSG32 u32FmtCnvt_ccm12;
            struct w32FmtCnvt_ccm12;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_ccm20_v(r32)                        _BFGET_(r32,10, 0)
    #define   SET32FmtCnvt_ccm20_v(r32,v)                      _BFSET_(r32,10, 0,v)
    #define   GET16FmtCnvt_ccm20_v(r16)                        _BFGET_(r16,10, 0)
    #define   SET16FmtCnvt_ccm20_v(r16,v)                      _BFSET_(r16,10, 0,v)

    #define     w32FmtCnvt_ccm20                               {\
            UNSG32 mccm20_v                                    : 11;\
            UNSG32 RSVDx418_b11                                : 21;\
          }
    union { UNSG32 u32FmtCnvt_ccm20;
            struct w32FmtCnvt_ccm20;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_ccm21_v(r32)                        _BFGET_(r32,10, 0)
    #define   SET32FmtCnvt_ccm21_v(r32,v)                      _BFSET_(r32,10, 0,v)
    #define   GET16FmtCnvt_ccm21_v(r16)                        _BFGET_(r16,10, 0)
    #define   SET16FmtCnvt_ccm21_v(r16,v)                      _BFSET_(r16,10, 0,v)

    #define     w32FmtCnvt_ccm21                               {\
            UNSG32 mccm21_v                                    : 11;\
            UNSG32 RSVDx41C_b11                                : 21;\
          }
    union { UNSG32 u32FmtCnvt_ccm21;
            struct w32FmtCnvt_ccm21;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_ccm22_v(r32)                        _BFGET_(r32,10, 0)
    #define   SET32FmtCnvt_ccm22_v(r32,v)                      _BFSET_(r32,10, 0,v)
    #define   GET16FmtCnvt_ccm22_v(r16)                        _BFGET_(r16,10, 0)
    #define   SET16FmtCnvt_ccm22_v(r16,v)                      _BFSET_(r16,10, 0,v)

    #define     w32FmtCnvt_ccm22                               {\
            UNSG32 mccm22_v                                    : 11;\
            UNSG32 RSVDx420_b11                                : 21;\
          }
    union { UNSG32 u32FmtCnvt_ccm22;
            struct w32FmtCnvt_ccm22;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_cca0_v(r32)                         _BFGET_(r32,12, 0)
    #define   SET32FmtCnvt_cca0_v(r32,v)                       _BFSET_(r32,12, 0,v)
    #define   GET16FmtCnvt_cca0_v(r16)                         _BFGET_(r16,12, 0)
    #define   SET16FmtCnvt_cca0_v(r16,v)                       _BFSET_(r16,12, 0,v)

    #define     w32FmtCnvt_cca0                                {\
            UNSG32 scca0_v                                     : 13;\
            UNSG32 RSVDx424_b13                                : 19;\
          }
    union { UNSG32 u32FmtCnvt_cca0;
            struct w32FmtCnvt_cca0;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_cca1_v(r32)                         _BFGET_(r32,12, 0)
    #define   SET32FmtCnvt_cca1_v(r32,v)                       _BFSET_(r32,12, 0,v)
    #define   GET16FmtCnvt_cca1_v(r16)                         _BFGET_(r16,12, 0)
    #define   SET16FmtCnvt_cca1_v(r16,v)                       _BFSET_(r16,12, 0,v)

    #define     w32FmtCnvt_cca1                                {\
            UNSG32 scca1_v                                     : 13;\
            UNSG32 RSVDx428_b13                                : 19;\
          }
    union { UNSG32 u32FmtCnvt_cca1;
            struct w32FmtCnvt_cca1;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_cca2_v(r32)                         _BFGET_(r32,12, 0)
    #define   SET32FmtCnvt_cca2_v(r32,v)                       _BFSET_(r32,12, 0,v)
    #define   GET16FmtCnvt_cca2_v(r16)                         _BFGET_(r16,12, 0)
    #define   SET16FmtCnvt_cca2_v(r16,v)                       _BFSET_(r16,12, 0,v)

    #define     w32FmtCnvt_cca2                                {\
            UNSG32 scca2_v                                     : 13;\
            UNSG32 RSVDx42C_b13                                : 19;\
          }
    union { UNSG32 u32FmtCnvt_cca2;
            struct w32FmtCnvt_cca2;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FmtCnvt_input_format(r32)                   _BFGET_(r32, 3, 0)
    #define   SET32FmtCnvt_input_format(r32,v)                 _BFSET_(r32, 3, 0,v)
    #define   GET16FmtCnvt_input_format(r16)                   _BFGET_(r16, 3, 0)
    #define   SET16FmtCnvt_input_format(r16,v)                 _BFSET_(r16, 3, 0,v)

    #define   GET32FmtCnvt_input_order(r32)                    _BFGET_(r32, 5, 4)
    #define   SET32FmtCnvt_input_order(r32,v)                  _BFSET_(r32, 5, 4,v)
    #define   GET16FmtCnvt_input_order(r16)                    _BFGET_(r16, 5, 4)
    #define   SET16FmtCnvt_input_order(r16,v)                  _BFSET_(r16, 5, 4,v)

    #define     w32FmtCnvt_input                               {\
            UNSG32 uinput_format                               :  4;\
            UNSG32 uinput_order                                :  2;\
            UNSG32 RSVDx430_b6                                 : 26;\
          }
    union { UNSG32 u32FmtCnvt_input;
            struct w32FmtCnvt_input;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx434                                    [972];
    ///////////////////////////////////////////////////////////
    } SIE_FmtCnvt;

    typedef union  T32FmtCnvt_ccm00
          { UNSG32 u32;
            struct w32FmtCnvt_ccm00;
                 } T32FmtCnvt_ccm00;
    typedef union  T32FmtCnvt_ccm01
          { UNSG32 u32;
            struct w32FmtCnvt_ccm01;
                 } T32FmtCnvt_ccm01;
    typedef union  T32FmtCnvt_ccm02
          { UNSG32 u32;
            struct w32FmtCnvt_ccm02;
                 } T32FmtCnvt_ccm02;
    typedef union  T32FmtCnvt_ccm10
          { UNSG32 u32;
            struct w32FmtCnvt_ccm10;
                 } T32FmtCnvt_ccm10;
    typedef union  T32FmtCnvt_ccm11
          { UNSG32 u32;
            struct w32FmtCnvt_ccm11;
                 } T32FmtCnvt_ccm11;
    typedef union  T32FmtCnvt_ccm12
          { UNSG32 u32;
            struct w32FmtCnvt_ccm12;
                 } T32FmtCnvt_ccm12;
    typedef union  T32FmtCnvt_ccm20
          { UNSG32 u32;
            struct w32FmtCnvt_ccm20;
                 } T32FmtCnvt_ccm20;
    typedef union  T32FmtCnvt_ccm21
          { UNSG32 u32;
            struct w32FmtCnvt_ccm21;
                 } T32FmtCnvt_ccm21;
    typedef union  T32FmtCnvt_ccm22
          { UNSG32 u32;
            struct w32FmtCnvt_ccm22;
                 } T32FmtCnvt_ccm22;
    typedef union  T32FmtCnvt_cca0
          { UNSG32 u32;
            struct w32FmtCnvt_cca0;
                 } T32FmtCnvt_cca0;
    typedef union  T32FmtCnvt_cca1
          { UNSG32 u32;
            struct w32FmtCnvt_cca1;
                 } T32FmtCnvt_cca1;
    typedef union  T32FmtCnvt_cca2
          { UNSG32 u32;
            struct w32FmtCnvt_cca2;
                 } T32FmtCnvt_cca2;
    typedef union  T32FmtCnvt_input
          { UNSG32 u32;
            struct w32FmtCnvt_input;
                 } T32FmtCnvt_input;
    ///////////////////////////////////////////////////////////

    typedef union  TFmtCnvt_ccm00
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_ccm00;
                   };
                 } TFmtCnvt_ccm00;
    typedef union  TFmtCnvt_ccm01
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_ccm01;
                   };
                 } TFmtCnvt_ccm01;
    typedef union  TFmtCnvt_ccm02
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_ccm02;
                   };
                 } TFmtCnvt_ccm02;
    typedef union  TFmtCnvt_ccm10
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_ccm10;
                   };
                 } TFmtCnvt_ccm10;
    typedef union  TFmtCnvt_ccm11
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_ccm11;
                   };
                 } TFmtCnvt_ccm11;
    typedef union  TFmtCnvt_ccm12
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_ccm12;
                   };
                 } TFmtCnvt_ccm12;
    typedef union  TFmtCnvt_ccm20
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_ccm20;
                   };
                 } TFmtCnvt_ccm20;
    typedef union  TFmtCnvt_ccm21
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_ccm21;
                   };
                 } TFmtCnvt_ccm21;
    typedef union  TFmtCnvt_ccm22
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_ccm22;
                   };
                 } TFmtCnvt_ccm22;
    typedef union  TFmtCnvt_cca0
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_cca0;
                   };
                 } TFmtCnvt_cca0;
    typedef union  TFmtCnvt_cca1
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_cca1;
                   };
                 } TFmtCnvt_cca1;
    typedef union  TFmtCnvt_cca2
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_cca2;
                   };
                 } TFmtCnvt_cca2;
    typedef union  TFmtCnvt_input
          { UNSG32 u32[1];
            struct {
            struct w32FmtCnvt_input;
                   };
                 } TFmtCnvt_input;

    ///////////////////////////////////////////////////////////
     SIGN32 FmtCnvt_drvrd(SIE_FmtCnvt *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 FmtCnvt_drvwr(SIE_FmtCnvt *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void FmtCnvt_reset(SIE_FmtCnvt *p);
     SIGN32 FmtCnvt_cmp  (SIE_FmtCnvt *p, SIE_FmtCnvt *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define FmtCnvt_check(p,pie,pfx,hLOG) FmtCnvt_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define FmtCnvt_print(p,    pfx,hLOG) FmtCnvt_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_VPPInputCC
#define h_VPPInputCC (){}

    #define     RA_VPPInputCC_mosd                             0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_VPPInputCC_ig                               0x0800
    ///////////////////////////////////////////////////////////

    typedef struct SIE_VPPInputCC {
    ///////////////////////////////////////////////////////////
              SIE_FmtCnvt                                      ie_mosd;
    ///////////////////////////////////////////////////////////
              SIE_FmtCnvt                                      ie_ig;
    ///////////////////////////////////////////////////////////
    } SIE_VPPInputCC;

    ///////////////////////////////////////////////////////////
     SIGN32 VPPInputCC_drvrd(SIE_VPPInputCC *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 VPPInputCC_drvwr(SIE_VPPInputCC *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void VPPInputCC_reset(SIE_VPPInputCC *p);
     SIGN32 VPPInputCC_cmp  (SIE_VPPInputCC *p, SIE_VPPInputCC *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define VPPInputCC_check(p,pie,pfx,hLOG) VPPInputCC_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define VPPInputCC_print(p,    pfx,hLOG) VPPInputCC_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_winCFG
#define h_winCFG (){}

    #define     RA_winCFG_LOC                                  0x0000

    #define     BA_winCFG_LOC_X                                0x0000
    #define     B16winCFG_LOC_X                                0x0000
    #define   LSb32winCFG_LOC_X                                   0
    #define   LSb16winCFG_LOC_X                                   0
    #define       bwinCFG_LOC_X                                11
    #define   MSK32winCFG_LOC_X                                   0x000007FF

    #define     BA_winCFG_LOC_Y                                0x0002
    #define     B16winCFG_LOC_Y                                0x0002
    #define   LSb32winCFG_LOC_Y                                   16
    #define   LSb16winCFG_LOC_Y                                   0
    #define       bwinCFG_LOC_Y                                11
    #define   MSK32winCFG_LOC_Y                                   0x07FF0000
    ///////////////////////////////////////////////////////////
    #define     RA_winCFG_SIZE                                 0x0004

    #define     BA_winCFG_SIZE_X                               0x0004
    #define     B16winCFG_SIZE_X                               0x0004
    #define   LSb32winCFG_SIZE_X                                  0
    #define   LSb16winCFG_SIZE_X                                  0
    #define       bwinCFG_SIZE_X                               11
    #define   MSK32winCFG_SIZE_X                                  0x000007FF

    #define     BA_winCFG_SIZE_Y                               0x0006
    #define     B16winCFG_SIZE_Y                               0x0006
    #define   LSb32winCFG_SIZE_Y                                  16
    #define   LSb16winCFG_SIZE_Y                                  0
    #define       bwinCFG_SIZE_Y                               11
    #define   MSK32winCFG_SIZE_Y                                  0x07FF0000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_winCFG {
    ///////////////////////////////////////////////////////////
    #define   GET32winCFG_LOC_X(r32)                           _BFGET_(r32,10, 0)
    #define   SET32winCFG_LOC_X(r32,v)                         _BFSET_(r32,10, 0,v)
    #define   GET16winCFG_LOC_X(r16)                           _BFGET_(r16,10, 0)
    #define   SET16winCFG_LOC_X(r16,v)                         _BFSET_(r16,10, 0,v)

    #define   GET32winCFG_LOC_Y(r32)                           _BFGET_(r32,26,16)
    #define   SET32winCFG_LOC_Y(r32,v)                         _BFSET_(r32,26,16,v)
    #define   GET16winCFG_LOC_Y(r16)                           _BFGET_(r16,10, 0)
    #define   SET16winCFG_LOC_Y(r16,v)                         _BFSET_(r16,10, 0,v)

    #define     w32winCFG_LOC                                  {\
            UNSG32 uLOC_X                                      : 11;\
            UNSG32 RSVDx0_b11                                  :  5;\
            UNSG32 uLOC_Y                                      : 11;\
            UNSG32 RSVDx0_b27                                  :  5;\
          }
    union { UNSG32 u32winCFG_LOC;
            struct w32winCFG_LOC;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32winCFG_SIZE_X(r32)                          _BFGET_(r32,10, 0)
    #define   SET32winCFG_SIZE_X(r32,v)                        _BFSET_(r32,10, 0,v)
    #define   GET16winCFG_SIZE_X(r16)                          _BFGET_(r16,10, 0)
    #define   SET16winCFG_SIZE_X(r16,v)                        _BFSET_(r16,10, 0,v)

    #define   GET32winCFG_SIZE_Y(r32)                          _BFGET_(r32,26,16)
    #define   SET32winCFG_SIZE_Y(r32,v)                        _BFSET_(r32,26,16,v)
    #define   GET16winCFG_SIZE_Y(r16)                          _BFGET_(r16,10, 0)
    #define   SET16winCFG_SIZE_Y(r16,v)                        _BFSET_(r16,10, 0,v)

    #define     w32winCFG_SIZE                                 {\
            UNSG32 uSIZE_X                                     : 11;\
            UNSG32 RSVDx4_b11                                  :  5;\
            UNSG32 uSIZE_Y                                     : 11;\
            UNSG32 RSVDx4_b27                                  :  5;\
          }
    union { UNSG32 u32winCFG_SIZE;
            struct w32winCFG_SIZE;
          };
    ///////////////////////////////////////////////////////////
    } SIE_winCFG;

    typedef union  T32winCFG_LOC
          { UNSG32 u32;
            struct w32winCFG_LOC;
                 } T32winCFG_LOC;
    typedef union  T32winCFG_SIZE
          { UNSG32 u32;
            struct w32winCFG_SIZE;
                 } T32winCFG_SIZE;
    ///////////////////////////////////////////////////////////

    typedef union  TwinCFG_LOC
          { UNSG32 u32[1];
            struct {
            struct w32winCFG_LOC;
                   };
                 } TwinCFG_LOC;
    typedef union  TwinCFG_SIZE
          { UNSG32 u32[1];
            struct {
            struct w32winCFG_SIZE;
                   };
                 } TwinCFG_SIZE;

    ///////////////////////////////////////////////////////////
     SIGN32 winCFG_drvrd(SIE_winCFG *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 winCFG_drvwr(SIE_winCFG *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void winCFG_reset(SIE_winCFG *p);
     SIGN32 winCFG_cmp  (SIE_winCFG *p, SIE_winCFG *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define winCFG_check(p,pie,pfx,hLOG) winCFG_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define winCFG_print(p,    pfx,hLOG) winCFG_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_PlaneOverlay
#define h_PlaneOverlay (){}

    #define     RA_PlaneOverlay_input                          0x0000

    #define     BA_PlaneOverlay_input_X                        0x0000
    #define     B16PlaneOverlay_input_X                        0x0000
    #define   LSb32PlaneOverlay_input_X                           0
    #define   LSb16PlaneOverlay_input_X                           0
    #define       bPlaneOverlay_input_X                        11
    #define   MSK32PlaneOverlay_input_X                           0x000007FF

    #define     BA_PlaneOverlay_input_cropLeft                 0x0001
    #define     B16PlaneOverlay_input_cropLeft                 0x0000
    #define   LSb32PlaneOverlay_input_cropLeft                    11
    #define   LSb16PlaneOverlay_input_cropLeft                    11
    #define       bPlaneOverlay_input_cropLeft                 5
    #define   MSK32PlaneOverlay_input_cropLeft                    0x0000F800

    #define     BA_PlaneOverlay_input_Y                        0x0002
    #define     B16PlaneOverlay_input_Y                        0x0002
    #define   LSb32PlaneOverlay_input_Y                           16
    #define   LSb16PlaneOverlay_input_Y                           0
    #define       bPlaneOverlay_input_Y                        11
    #define   MSK32PlaneOverlay_input_Y                           0x07FF0000

    #define     BA_PlaneOverlay_input_cropTop                  0x0003
    #define     B16PlaneOverlay_input_cropTop                  0x0002
    #define   LSb32PlaneOverlay_input_cropTop                     27
    #define   LSb16PlaneOverlay_input_cropTop                     11
    #define       bPlaneOverlay_input_cropTop                  4
    #define   MSK32PlaneOverlay_input_cropTop                     0x78000000

    #define     BA_PlaneOverlay_input_rule                     0x0003
    #define     B16PlaneOverlay_input_rule                     0x0002
    #define   LSb32PlaneOverlay_input_rule                        31
    #define   LSb16PlaneOverlay_input_rule                        15
    #define       bPlaneOverlay_input_rule                     1
    #define   MSK32PlaneOverlay_input_rule                        0x80000000
    #define        PlaneOverlay_input_rule_premult                          0x1
    #define        PlaneOverlay_input_rule_nonpremult                       0x0
    ///////////////////////////////////////////////////////////
    #define     RA_PlaneOverlay_window                         0x0004
    ///////////////////////////////////////////////////////////
    #define     RA_PlaneOverlay_cfg                            0x000C

    #define     BA_PlaneOverlay_cfg_GA                         0x000C
    #define     B16PlaneOverlay_cfg_GA                         0x000C
    #define   LSb32PlaneOverlay_cfg_GA                            0
    #define   LSb16PlaneOverlay_cfg_GA                            0
    #define       bPlaneOverlay_cfg_GA                         9
    #define   MSK32PlaneOverlay_cfg_GA                            0x000001FF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_PlaneOverlay {
    ///////////////////////////////////////////////////////////
    #define   GET32PlaneOverlay_input_X(r32)                   _BFGET_(r32,10, 0)
    #define   SET32PlaneOverlay_input_X(r32,v)                 _BFSET_(r32,10, 0,v)
    #define   GET16PlaneOverlay_input_X(r16)                   _BFGET_(r16,10, 0)
    #define   SET16PlaneOverlay_input_X(r16,v)                 _BFSET_(r16,10, 0,v)

    #define   GET32PlaneOverlay_input_cropLeft(r32)            _BFGET_(r32,15,11)
    #define   SET32PlaneOverlay_input_cropLeft(r32,v)          _BFSET_(r32,15,11,v)
    #define   GET16PlaneOverlay_input_cropLeft(r16)            _BFGET_(r16,15,11)
    #define   SET16PlaneOverlay_input_cropLeft(r16,v)          _BFSET_(r16,15,11,v)

    #define   GET32PlaneOverlay_input_Y(r32)                   _BFGET_(r32,26,16)
    #define   SET32PlaneOverlay_input_Y(r32,v)                 _BFSET_(r32,26,16,v)
    #define   GET16PlaneOverlay_input_Y(r16)                   _BFGET_(r16,10, 0)
    #define   SET16PlaneOverlay_input_Y(r16,v)                 _BFSET_(r16,10, 0,v)

    #define   GET32PlaneOverlay_input_cropTop(r32)             _BFGET_(r32,30,27)
    #define   SET32PlaneOverlay_input_cropTop(r32,v)           _BFSET_(r32,30,27,v)
    #define   GET16PlaneOverlay_input_cropTop(r16)             _BFGET_(r16,14,11)
    #define   SET16PlaneOverlay_input_cropTop(r16,v)           _BFSET_(r16,14,11,v)

    #define   GET32PlaneOverlay_input_rule(r32)                _BFGET_(r32,31,31)
    #define   SET32PlaneOverlay_input_rule(r32,v)              _BFSET_(r32,31,31,v)
    #define   GET16PlaneOverlay_input_rule(r16)                _BFGET_(r16,15,15)
    #define   SET16PlaneOverlay_input_rule(r16,v)              _BFSET_(r16,15,15,v)

    #define     w32PlaneOverlay_input                          {\
            UNSG32 uinput_X                                    : 11;\
            UNSG32 uinput_cropLeft                             :  5;\
            UNSG32 uinput_Y                                    : 11;\
            UNSG32 uinput_cropTop                              :  4;\
            UNSG32 uinput_rule                                 :  1;\
          }
    union { UNSG32 u32PlaneOverlay_input;
            struct w32PlaneOverlay_input;
          };
    ///////////////////////////////////////////////////////////
              SIE_winCFG                                       ie_window;
    ///////////////////////////////////////////////////////////
    #define   GET32PlaneOverlay_cfg_GA(r32)                    _BFGET_(r32, 8, 0)
    #define   SET32PlaneOverlay_cfg_GA(r32,v)                  _BFSET_(r32, 8, 0,v)
    #define   GET16PlaneOverlay_cfg_GA(r16)                    _BFGET_(r16, 8, 0)
    #define   SET16PlaneOverlay_cfg_GA(r16,v)                  _BFSET_(r16, 8, 0,v)

    #define     w32PlaneOverlay_cfg                            {\
            UNSG32 ucfg_GA                                     :  9;\
            UNSG32 RSVDxC_b9                                   : 23;\
          }
    union { UNSG32 u32PlaneOverlay_cfg;
            struct w32PlaneOverlay_cfg;
          };
    ///////////////////////////////////////////////////////////
    } SIE_PlaneOverlay;

    typedef union  T32PlaneOverlay_input
          { UNSG32 u32;
            struct w32PlaneOverlay_input;
                 } T32PlaneOverlay_input;
    typedef union  T32PlaneOverlay_cfg
          { UNSG32 u32;
            struct w32PlaneOverlay_cfg;
                 } T32PlaneOverlay_cfg;
    ///////////////////////////////////////////////////////////

    typedef union  TPlaneOverlay_input
          { UNSG32 u32[1];
            struct {
            struct w32PlaneOverlay_input;
                   };
                 } TPlaneOverlay_input;
    typedef union  TPlaneOverlay_cfg
          { UNSG32 u32[1];
            struct {
            struct w32PlaneOverlay_cfg;
                   };
                 } TPlaneOverlay_cfg;

    ///////////////////////////////////////////////////////////
     SIGN32 PlaneOverlay_drvrd(SIE_PlaneOverlay *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 PlaneOverlay_drvwr(SIE_PlaneOverlay *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void PlaneOverlay_reset(SIE_PlaneOverlay *p);
     SIGN32 PlaneOverlay_cmp  (SIE_PlaneOverlay *p, SIE_PlaneOverlay *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define PlaneOverlay_check(p,pie,pfx,hLOG) PlaneOverlay_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define PlaneOverlay_print(p,    pfx,hLOG) PlaneOverlay_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_AllPlaneOverlay
#define h_AllPlaneOverlay (){}

    #define     RA_AllPlaneOverlay_bg                          0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_mv                          0x0010
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_pip                         0x0020
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_pg                          0x0030
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_ig                          0x0040
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_cursor                      0x0050
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_mosd                        0x0060
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_colorKeyEnb                 0x0070

    #define     BA_AllPlaneOverlay_colorKeyEnb_Y               0x0070
    #define     B16AllPlaneOverlay_colorKeyEnb_Y               0x0070
    #define   LSb32AllPlaneOverlay_colorKeyEnb_Y                  0
    #define   LSb16AllPlaneOverlay_colorKeyEnb_Y                  0
    #define       bAllPlaneOverlay_colorKeyEnb_Y               1
    #define   MSK32AllPlaneOverlay_colorKeyEnb_Y                  0x00000001

    #define     BA_AllPlaneOverlay_colorKeyEnb_U               0x0070
    #define     B16AllPlaneOverlay_colorKeyEnb_U               0x0070
    #define   LSb32AllPlaneOverlay_colorKeyEnb_U                  1
    #define   LSb16AllPlaneOverlay_colorKeyEnb_U                  1
    #define       bAllPlaneOverlay_colorKeyEnb_U               1
    #define   MSK32AllPlaneOverlay_colorKeyEnb_U                  0x00000002

    #define     BA_AllPlaneOverlay_colorKeyEnb_V               0x0070
    #define     B16AllPlaneOverlay_colorKeyEnb_V               0x0070
    #define   LSb32AllPlaneOverlay_colorKeyEnb_V                  2
    #define   LSb16AllPlaneOverlay_colorKeyEnb_V                  2
    #define       bAllPlaneOverlay_colorKeyEnb_V               1
    #define   MSK32AllPlaneOverlay_colorKeyEnb_V                  0x00000004
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_colorKeyMin                 0x0074

    #define     BA_AllPlaneOverlay_colorKeyMin_Y               0x0074
    #define     B16AllPlaneOverlay_colorKeyMin_Y               0x0074
    #define   LSb32AllPlaneOverlay_colorKeyMin_Y                  0
    #define   LSb16AllPlaneOverlay_colorKeyMin_Y                  0
    #define       bAllPlaneOverlay_colorKeyMin_Y               8
    #define   MSK32AllPlaneOverlay_colorKeyMin_Y                  0x000000FF

    #define     BA_AllPlaneOverlay_colorKeyMin_U               0x0075
    #define     B16AllPlaneOverlay_colorKeyMin_U               0x0074
    #define   LSb32AllPlaneOverlay_colorKeyMin_U                  8
    #define   LSb16AllPlaneOverlay_colorKeyMin_U                  8
    #define       bAllPlaneOverlay_colorKeyMin_U               8
    #define   MSK32AllPlaneOverlay_colorKeyMin_U                  0x0000FF00

    #define     BA_AllPlaneOverlay_colorKeyMin_V               0x0076
    #define     B16AllPlaneOverlay_colorKeyMin_V               0x0076
    #define   LSb32AllPlaneOverlay_colorKeyMin_V                  16
    #define   LSb16AllPlaneOverlay_colorKeyMin_V                  0
    #define       bAllPlaneOverlay_colorKeyMin_V               8
    #define   MSK32AllPlaneOverlay_colorKeyMin_V                  0x00FF0000
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_colorKeyMax                 0x0078

    #define     BA_AllPlaneOverlay_colorKeyMax_Y               0x0078
    #define     B16AllPlaneOverlay_colorKeyMax_Y               0x0078
    #define   LSb32AllPlaneOverlay_colorKeyMax_Y                  0
    #define   LSb16AllPlaneOverlay_colorKeyMax_Y                  0
    #define       bAllPlaneOverlay_colorKeyMax_Y               8
    #define   MSK32AllPlaneOverlay_colorKeyMax_Y                  0x000000FF

    #define     BA_AllPlaneOverlay_colorKeyMax_U               0x0079
    #define     B16AllPlaneOverlay_colorKeyMax_U               0x0078
    #define   LSb32AllPlaneOverlay_colorKeyMax_U                  8
    #define   LSb16AllPlaneOverlay_colorKeyMax_U                  8
    #define       bAllPlaneOverlay_colorKeyMax_U               8
    #define   MSK32AllPlaneOverlay_colorKeyMax_U                  0x0000FF00

    #define     BA_AllPlaneOverlay_colorKeyMax_V               0x007A
    #define     B16AllPlaneOverlay_colorKeyMax_V               0x007A
    #define   LSb32AllPlaneOverlay_colorKeyMax_V                  16
    #define   LSb16AllPlaneOverlay_colorKeyMax_V                  0
    #define       bAllPlaneOverlay_colorKeyMax_V               8
    #define   MSK32AllPlaneOverlay_colorKeyMax_V                  0x00FF0000
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_mvColor                     0x007C

    #define     BA_AllPlaneOverlay_mvColor_Y                   0x007C
    #define     B16AllPlaneOverlay_mvColor_Y                   0x007C
    #define   LSb32AllPlaneOverlay_mvColor_Y                      0
    #define   LSb16AllPlaneOverlay_mvColor_Y                      0
    #define       bAllPlaneOverlay_mvColor_Y                   8
    #define   MSK32AllPlaneOverlay_mvColor_Y                      0x000000FF

    #define     BA_AllPlaneOverlay_mvColor_U                   0x007D
    #define     B16AllPlaneOverlay_mvColor_U                   0x007C
    #define   LSb32AllPlaneOverlay_mvColor_U                      8
    #define   LSb16AllPlaneOverlay_mvColor_U                      8
    #define       bAllPlaneOverlay_mvColor_U                   8
    #define   MSK32AllPlaneOverlay_mvColor_U                      0x0000FF00

    #define     BA_AllPlaneOverlay_mvColor_V                   0x007E
    #define     B16AllPlaneOverlay_mvColor_V                   0x007E
    #define   LSb32AllPlaneOverlay_mvColor_V                      16
    #define   LSb16AllPlaneOverlay_mvColor_V                      0
    #define       bAllPlaneOverlay_mvColor_V                   8
    #define   MSK32AllPlaneOverlay_mvColor_V                      0x00FF0000
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_pipColor                    0x0080

    #define     BA_AllPlaneOverlay_pipColor_Y                  0x0080
    #define     B16AllPlaneOverlay_pipColor_Y                  0x0080
    #define   LSb32AllPlaneOverlay_pipColor_Y                     0
    #define   LSb16AllPlaneOverlay_pipColor_Y                     0
    #define       bAllPlaneOverlay_pipColor_Y                  8
    #define   MSK32AllPlaneOverlay_pipColor_Y                     0x000000FF

    #define     BA_AllPlaneOverlay_pipColor_U                  0x0081
    #define     B16AllPlaneOverlay_pipColor_U                  0x0080
    #define   LSb32AllPlaneOverlay_pipColor_U                     8
    #define   LSb16AllPlaneOverlay_pipColor_U                     8
    #define       bAllPlaneOverlay_pipColor_U                  8
    #define   MSK32AllPlaneOverlay_pipColor_U                     0x0000FF00

    #define     BA_AllPlaneOverlay_pipColor_V                  0x0082
    #define     B16AllPlaneOverlay_pipColor_V                  0x0082
    #define   LSb32AllPlaneOverlay_pipColor_V                     16
    #define   LSb16AllPlaneOverlay_pipColor_V                     0
    #define       bAllPlaneOverlay_pipColor_V                  8
    #define   MSK32AllPlaneOverlay_pipColor_V                     0x00FF0000
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_mvSrc                       0x0084

    #define     BA_AllPlaneOverlay_mvSrc_sel                   0x0084
    #define     B16AllPlaneOverlay_mvSrc_sel                   0x0084
    #define   LSb32AllPlaneOverlay_mvSrc_sel                      0
    #define   LSb16AllPlaneOverlay_mvSrc_sel                      0
    #define       bAllPlaneOverlay_mvSrc_sel                   1
    #define   MSK32AllPlaneOverlay_mvSrc_sel                      0x00000001
    #define        AllPlaneOverlay_mvSrc_sel_dma                            0x0
    #define        AllPlaneOverlay_mvSrc_sel_reg                            0x1
    ///////////////////////////////////////////////////////////
    #define     RA_AllPlaneOverlay_pipSrc                      0x0088

    #define     BA_AllPlaneOverlay_pipSrc_sel                  0x0088
    #define     B16AllPlaneOverlay_pipSrc_sel                  0x0088
    #define   LSb32AllPlaneOverlay_pipSrc_sel                     0
    #define   LSb16AllPlaneOverlay_pipSrc_sel                     0
    #define       bAllPlaneOverlay_pipSrc_sel                  1
    #define   MSK32AllPlaneOverlay_pipSrc_sel                     0x00000001
    #define        AllPlaneOverlay_pipSrc_sel_dma                           0x0
    #define        AllPlaneOverlay_pipSrc_sel_reg                           0x1
    ///////////////////////////////////////////////////////////

    typedef struct SIE_AllPlaneOverlay {
    ///////////////////////////////////////////////////////////
              SIE_PlaneOverlay                                 ie_bg;
    ///////////////////////////////////////////////////////////
              SIE_PlaneOverlay                                 ie_mv;
    ///////////////////////////////////////////////////////////
              SIE_PlaneOverlay                                 ie_pip;
    ///////////////////////////////////////////////////////////
              SIE_PlaneOverlay                                 ie_pg;
    ///////////////////////////////////////////////////////////
              SIE_PlaneOverlay                                 ie_ig;
    ///////////////////////////////////////////////////////////
              SIE_PlaneOverlay                                 ie_cursor;
    ///////////////////////////////////////////////////////////
              SIE_PlaneOverlay                                 ie_mosd;
    ///////////////////////////////////////////////////////////
    #define   GET32AllPlaneOverlay_colorKeyEnb_Y(r32)          _BFGET_(r32, 0, 0)
    #define   SET32AllPlaneOverlay_colorKeyEnb_Y(r32,v)        _BFSET_(r32, 0, 0,v)
    #define   GET16AllPlaneOverlay_colorKeyEnb_Y(r16)          _BFGET_(r16, 0, 0)
    #define   SET16AllPlaneOverlay_colorKeyEnb_Y(r16,v)        _BFSET_(r16, 0, 0,v)

    #define   GET32AllPlaneOverlay_colorKeyEnb_U(r32)          _BFGET_(r32, 1, 1)
    #define   SET32AllPlaneOverlay_colorKeyEnb_U(r32,v)        _BFSET_(r32, 1, 1,v)
    #define   GET16AllPlaneOverlay_colorKeyEnb_U(r16)          _BFGET_(r16, 1, 1)
    #define   SET16AllPlaneOverlay_colorKeyEnb_U(r16,v)        _BFSET_(r16, 1, 1,v)

    #define   GET32AllPlaneOverlay_colorKeyEnb_V(r32)          _BFGET_(r32, 2, 2)
    #define   SET32AllPlaneOverlay_colorKeyEnb_V(r32,v)        _BFSET_(r32, 2, 2,v)
    #define   GET16AllPlaneOverlay_colorKeyEnb_V(r16)          _BFGET_(r16, 2, 2)
    #define   SET16AllPlaneOverlay_colorKeyEnb_V(r16,v)        _BFSET_(r16, 2, 2,v)

    #define     w32AllPlaneOverlay_colorKeyEnb                 {\
            UNSG32 ucolorKeyEnb_Y                              :  1;\
            UNSG32 ucolorKeyEnb_U                              :  1;\
            UNSG32 ucolorKeyEnb_V                              :  1;\
            UNSG32 RSVDx70_b3                                  : 29;\
          }
    union { UNSG32 u32AllPlaneOverlay_colorKeyEnb;
            struct w32AllPlaneOverlay_colorKeyEnb;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32AllPlaneOverlay_colorKeyMin_Y(r32)          _BFGET_(r32, 7, 0)
    #define   SET32AllPlaneOverlay_colorKeyMin_Y(r32,v)        _BFSET_(r32, 7, 0,v)
    #define   GET16AllPlaneOverlay_colorKeyMin_Y(r16)          _BFGET_(r16, 7, 0)
    #define   SET16AllPlaneOverlay_colorKeyMin_Y(r16,v)        _BFSET_(r16, 7, 0,v)

    #define   GET32AllPlaneOverlay_colorKeyMin_U(r32)          _BFGET_(r32,15, 8)
    #define   SET32AllPlaneOverlay_colorKeyMin_U(r32,v)        _BFSET_(r32,15, 8,v)
    #define   GET16AllPlaneOverlay_colorKeyMin_U(r16)          _BFGET_(r16,15, 8)
    #define   SET16AllPlaneOverlay_colorKeyMin_U(r16,v)        _BFSET_(r16,15, 8,v)

    #define   GET32AllPlaneOverlay_colorKeyMin_V(r32)          _BFGET_(r32,23,16)
    #define   SET32AllPlaneOverlay_colorKeyMin_V(r32,v)        _BFSET_(r32,23,16,v)
    #define   GET16AllPlaneOverlay_colorKeyMin_V(r16)          _BFGET_(r16, 7, 0)
    #define   SET16AllPlaneOverlay_colorKeyMin_V(r16,v)        _BFSET_(r16, 7, 0,v)

    #define     w32AllPlaneOverlay_colorKeyMin                 {\
            UNSG32 ucolorKeyMin_Y                              :  8;\
            UNSG32 ucolorKeyMin_U                              :  8;\
            UNSG32 ucolorKeyMin_V                              :  8;\
            UNSG32 RSVDx74_b24                                 :  8;\
          }
    union { UNSG32 u32AllPlaneOverlay_colorKeyMin;
            struct w32AllPlaneOverlay_colorKeyMin;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32AllPlaneOverlay_colorKeyMax_Y(r32)          _BFGET_(r32, 7, 0)
    #define   SET32AllPlaneOverlay_colorKeyMax_Y(r32,v)        _BFSET_(r32, 7, 0,v)
    #define   GET16AllPlaneOverlay_colorKeyMax_Y(r16)          _BFGET_(r16, 7, 0)
    #define   SET16AllPlaneOverlay_colorKeyMax_Y(r16,v)        _BFSET_(r16, 7, 0,v)

    #define   GET32AllPlaneOverlay_colorKeyMax_U(r32)          _BFGET_(r32,15, 8)
    #define   SET32AllPlaneOverlay_colorKeyMax_U(r32,v)        _BFSET_(r32,15, 8,v)
    #define   GET16AllPlaneOverlay_colorKeyMax_U(r16)          _BFGET_(r16,15, 8)
    #define   SET16AllPlaneOverlay_colorKeyMax_U(r16,v)        _BFSET_(r16,15, 8,v)

    #define   GET32AllPlaneOverlay_colorKeyMax_V(r32)          _BFGET_(r32,23,16)
    #define   SET32AllPlaneOverlay_colorKeyMax_V(r32,v)        _BFSET_(r32,23,16,v)
    #define   GET16AllPlaneOverlay_colorKeyMax_V(r16)          _BFGET_(r16, 7, 0)
    #define   SET16AllPlaneOverlay_colorKeyMax_V(r16,v)        _BFSET_(r16, 7, 0,v)

    #define     w32AllPlaneOverlay_colorKeyMax                 {\
            UNSG32 ucolorKeyMax_Y                              :  8;\
            UNSG32 ucolorKeyMax_U                              :  8;\
            UNSG32 ucolorKeyMax_V                              :  8;\
            UNSG32 RSVDx78_b24                                 :  8;\
          }
    union { UNSG32 u32AllPlaneOverlay_colorKeyMax;
            struct w32AllPlaneOverlay_colorKeyMax;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32AllPlaneOverlay_mvColor_Y(r32)              _BFGET_(r32, 7, 0)
    #define   SET32AllPlaneOverlay_mvColor_Y(r32,v)            _BFSET_(r32, 7, 0,v)
    #define   GET16AllPlaneOverlay_mvColor_Y(r16)              _BFGET_(r16, 7, 0)
    #define   SET16AllPlaneOverlay_mvColor_Y(r16,v)            _BFSET_(r16, 7, 0,v)

    #define   GET32AllPlaneOverlay_mvColor_U(r32)              _BFGET_(r32,15, 8)
    #define   SET32AllPlaneOverlay_mvColor_U(r32,v)            _BFSET_(r32,15, 8,v)
    #define   GET16AllPlaneOverlay_mvColor_U(r16)              _BFGET_(r16,15, 8)
    #define   SET16AllPlaneOverlay_mvColor_U(r16,v)            _BFSET_(r16,15, 8,v)

    #define   GET32AllPlaneOverlay_mvColor_V(r32)              _BFGET_(r32,23,16)
    #define   SET32AllPlaneOverlay_mvColor_V(r32,v)            _BFSET_(r32,23,16,v)
    #define   GET16AllPlaneOverlay_mvColor_V(r16)              _BFGET_(r16, 7, 0)
    #define   SET16AllPlaneOverlay_mvColor_V(r16,v)            _BFSET_(r16, 7, 0,v)

    #define     w32AllPlaneOverlay_mvColor                     {\
            UNSG32 umvColor_Y                                  :  8;\
            UNSG32 umvColor_U                                  :  8;\
            UNSG32 umvColor_V                                  :  8;\
            UNSG32 RSVDx7C_b24                                 :  8;\
          }
    union { UNSG32 u32AllPlaneOverlay_mvColor;
            struct w32AllPlaneOverlay_mvColor;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32AllPlaneOverlay_pipColor_Y(r32)             _BFGET_(r32, 7, 0)
    #define   SET32AllPlaneOverlay_pipColor_Y(r32,v)           _BFSET_(r32, 7, 0,v)
    #define   GET16AllPlaneOverlay_pipColor_Y(r16)             _BFGET_(r16, 7, 0)
    #define   SET16AllPlaneOverlay_pipColor_Y(r16,v)           _BFSET_(r16, 7, 0,v)

    #define   GET32AllPlaneOverlay_pipColor_U(r32)             _BFGET_(r32,15, 8)
    #define   SET32AllPlaneOverlay_pipColor_U(r32,v)           _BFSET_(r32,15, 8,v)
    #define   GET16AllPlaneOverlay_pipColor_U(r16)             _BFGET_(r16,15, 8)
    #define   SET16AllPlaneOverlay_pipColor_U(r16,v)           _BFSET_(r16,15, 8,v)

    #define   GET32AllPlaneOverlay_pipColor_V(r32)             _BFGET_(r32,23,16)
    #define   SET32AllPlaneOverlay_pipColor_V(r32,v)           _BFSET_(r32,23,16,v)
    #define   GET16AllPlaneOverlay_pipColor_V(r16)             _BFGET_(r16, 7, 0)
    #define   SET16AllPlaneOverlay_pipColor_V(r16,v)           _BFSET_(r16, 7, 0,v)

    #define     w32AllPlaneOverlay_pipColor                    {\
            UNSG32 upipColor_Y                                 :  8;\
            UNSG32 upipColor_U                                 :  8;\
            UNSG32 upipColor_V                                 :  8;\
            UNSG32 RSVDx80_b24                                 :  8;\
          }
    union { UNSG32 u32AllPlaneOverlay_pipColor;
            struct w32AllPlaneOverlay_pipColor;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32AllPlaneOverlay_mvSrc_sel(r32)              _BFGET_(r32, 0, 0)
    #define   SET32AllPlaneOverlay_mvSrc_sel(r32,v)            _BFSET_(r32, 0, 0,v)
    #define   GET16AllPlaneOverlay_mvSrc_sel(r16)              _BFGET_(r16, 0, 0)
    #define   SET16AllPlaneOverlay_mvSrc_sel(r16,v)            _BFSET_(r16, 0, 0,v)

    #define     w32AllPlaneOverlay_mvSrc                       {\
            UNSG32 umvSrc_sel                                  :  1;\
            UNSG32 RSVDx84_b1                                  : 31;\
          }
    union { UNSG32 u32AllPlaneOverlay_mvSrc;
            struct w32AllPlaneOverlay_mvSrc;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32AllPlaneOverlay_pipSrc_sel(r32)             _BFGET_(r32, 0, 0)
    #define   SET32AllPlaneOverlay_pipSrc_sel(r32,v)           _BFSET_(r32, 0, 0,v)
    #define   GET16AllPlaneOverlay_pipSrc_sel(r16)             _BFGET_(r16, 0, 0)
    #define   SET16AllPlaneOverlay_pipSrc_sel(r16,v)           _BFSET_(r16, 0, 0,v)

    #define     w32AllPlaneOverlay_pipSrc                      {\
            UNSG32 upipSrc_sel                                 :  1;\
            UNSG32 RSVDx88_b1                                  : 31;\
          }
    union { UNSG32 u32AllPlaneOverlay_pipSrc;
            struct w32AllPlaneOverlay_pipSrc;
          };
    ///////////////////////////////////////////////////////////
    } SIE_AllPlaneOverlay;

    typedef union  T32AllPlaneOverlay_colorKeyEnb
          { UNSG32 u32;
            struct w32AllPlaneOverlay_colorKeyEnb;
                 } T32AllPlaneOverlay_colorKeyEnb;
    typedef union  T32AllPlaneOverlay_colorKeyMin
          { UNSG32 u32;
            struct w32AllPlaneOverlay_colorKeyMin;
                 } T32AllPlaneOverlay_colorKeyMin;
    typedef union  T32AllPlaneOverlay_colorKeyMax
          { UNSG32 u32;
            struct w32AllPlaneOverlay_colorKeyMax;
                 } T32AllPlaneOverlay_colorKeyMax;
    typedef union  T32AllPlaneOverlay_mvColor
          { UNSG32 u32;
            struct w32AllPlaneOverlay_mvColor;
                 } T32AllPlaneOverlay_mvColor;
    typedef union  T32AllPlaneOverlay_pipColor
          { UNSG32 u32;
            struct w32AllPlaneOverlay_pipColor;
                 } T32AllPlaneOverlay_pipColor;
    typedef union  T32AllPlaneOverlay_mvSrc
          { UNSG32 u32;
            struct w32AllPlaneOverlay_mvSrc;
                 } T32AllPlaneOverlay_mvSrc;
    typedef union  T32AllPlaneOverlay_pipSrc
          { UNSG32 u32;
            struct w32AllPlaneOverlay_pipSrc;
                 } T32AllPlaneOverlay_pipSrc;
    ///////////////////////////////////////////////////////////

    typedef union  TAllPlaneOverlay_colorKeyEnb
          { UNSG32 u32[1];
            struct {
            struct w32AllPlaneOverlay_colorKeyEnb;
                   };
                 } TAllPlaneOverlay_colorKeyEnb;
    typedef union  TAllPlaneOverlay_colorKeyMin
          { UNSG32 u32[1];
            struct {
            struct w32AllPlaneOverlay_colorKeyMin;
                   };
                 } TAllPlaneOverlay_colorKeyMin;
    typedef union  TAllPlaneOverlay_colorKeyMax
          { UNSG32 u32[1];
            struct {
            struct w32AllPlaneOverlay_colorKeyMax;
                   };
                 } TAllPlaneOverlay_colorKeyMax;
    typedef union  TAllPlaneOverlay_mvColor
          { UNSG32 u32[1];
            struct {
            struct w32AllPlaneOverlay_mvColor;
                   };
                 } TAllPlaneOverlay_mvColor;
    typedef union  TAllPlaneOverlay_pipColor
          { UNSG32 u32[1];
            struct {
            struct w32AllPlaneOverlay_pipColor;
                   };
                 } TAllPlaneOverlay_pipColor;
    typedef union  TAllPlaneOverlay_mvSrc
          { UNSG32 u32[1];
            struct {
            struct w32AllPlaneOverlay_mvSrc;
                   };
                 } TAllPlaneOverlay_mvSrc;
    typedef union  TAllPlaneOverlay_pipSrc
          { UNSG32 u32[1];
            struct {
            struct w32AllPlaneOverlay_pipSrc;
                   };
                 } TAllPlaneOverlay_pipSrc;

    ///////////////////////////////////////////////////////////
     SIGN32 AllPlaneOverlay_drvrd(SIE_AllPlaneOverlay *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 AllPlaneOverlay_drvwr(SIE_AllPlaneOverlay *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void AllPlaneOverlay_reset(SIE_AllPlaneOverlay *p);
     SIGN32 AllPlaneOverlay_cmp  (SIE_AllPlaneOverlay *p, SIE_AllPlaneOverlay *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define AllPlaneOverlay_check(p,pie,pfx,hLOG) AllPlaneOverlay_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define AllPlaneOverlay_print(p,    pfx,hLOG) AllPlaneOverlay_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_OverlayCFG
#define h_OverlayCFG (){}

    #define     RA_OverlayCFG_cc                               0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_OverlayCFG_screen                           0x0800

    #define     BA_OverlayCFG_screen_X                         0x0800
    #define     B16OverlayCFG_screen_X                         0x0800
    #define   LSb32OverlayCFG_screen_X                            0
    #define   LSb16OverlayCFG_screen_X                            0
    #define       bOverlayCFG_screen_X                         11
    #define   MSK32OverlayCFG_screen_X                            0x000007FF

    #define     BA_OverlayCFG_screen_Y                         0x0802
    #define     B16OverlayCFG_screen_Y                         0x0802
    #define   LSb32OverlayCFG_screen_Y                            16
    #define   LSb16OverlayCFG_screen_Y                            0
    #define       bOverlayCFG_screen_Y                         11
    #define   MSK32OverlayCFG_screen_Y                            0x07FF0000
    ///////////////////////////////////////////////////////////
    #define     RA_OverlayCFG_background                       0x0804

    #define     BA_OverlayCFG_background_Y                     0x0804
    #define     B16OverlayCFG_background_Y                     0x0804
    #define   LSb32OverlayCFG_background_Y                        0
    #define   LSb16OverlayCFG_background_Y                        0
    #define       bOverlayCFG_background_Y                     8
    #define   MSK32OverlayCFG_background_Y                        0x000000FF

    #define     BA_OverlayCFG_background_U                     0x0805
    #define     B16OverlayCFG_background_U                     0x0804
    #define   LSb32OverlayCFG_background_U                        8
    #define   LSb16OverlayCFG_background_U                        8
    #define       bOverlayCFG_background_U                     8
    #define   MSK32OverlayCFG_background_U                        0x0000FF00

    #define     BA_OverlayCFG_background_V                     0x0806
    #define     B16OverlayCFG_background_V                     0x0806
    #define   LSb32OverlayCFG_background_V                        16
    #define   LSb16OverlayCFG_background_V                        0
    #define       bOverlayCFG_background_V                     8
    #define   MSK32OverlayCFG_background_V                        0x00FF0000
    ///////////////////////////////////////////////////////////
    #define     RA_OverlayCFG_mask                             0x0808

    #define     BA_OverlayCFG_mask_bg                          0x0808
    #define     B16OverlayCFG_mask_bg                          0x0808
    #define   LSb32OverlayCFG_mask_bg                             0
    #define   LSb16OverlayCFG_mask_bg                             0
    #define       bOverlayCFG_mask_bg                          1
    #define   MSK32OverlayCFG_mask_bg                             0x00000001

    #define     BA_OverlayCFG_mask_mv                          0x0808
    #define     B16OverlayCFG_mask_mv                          0x0808
    #define   LSb32OverlayCFG_mask_mv                             1
    #define   LSb16OverlayCFG_mask_mv                             1
    #define       bOverlayCFG_mask_mv                          1
    #define   MSK32OverlayCFG_mask_mv                             0x00000002

    #define     BA_OverlayCFG_mask_pip                         0x0808
    #define     B16OverlayCFG_mask_pip                         0x0808
    #define   LSb32OverlayCFG_mask_pip                            2
    #define   LSb16OverlayCFG_mask_pip                            2
    #define       bOverlayCFG_mask_pip                         1
    #define   MSK32OverlayCFG_mask_pip                            0x00000004

    #define     BA_OverlayCFG_mask_pg                          0x0808
    #define     B16OverlayCFG_mask_pg                          0x0808
    #define   LSb32OverlayCFG_mask_pg                             3
    #define   LSb16OverlayCFG_mask_pg                             3
    #define       bOverlayCFG_mask_pg                          1
    #define   MSK32OverlayCFG_mask_pg                             0x00000008

    #define     BA_OverlayCFG_mask_ig                          0x0808
    #define     B16OverlayCFG_mask_ig                          0x0808
    #define   LSb32OverlayCFG_mask_ig                             4
    #define   LSb16OverlayCFG_mask_ig                             4
    #define       bOverlayCFG_mask_ig                          1
    #define   MSK32OverlayCFG_mask_ig                             0x00000010

    #define     BA_OverlayCFG_mask_cursor                      0x0808
    #define     B16OverlayCFG_mask_cursor                      0x0808
    #define   LSb32OverlayCFG_mask_cursor                         5
    #define   LSb16OverlayCFG_mask_cursor                         5
    #define       bOverlayCFG_mask_cursor                      1
    #define   MSK32OverlayCFG_mask_cursor                         0x00000020

    #define     BA_OverlayCFG_mask_mosd                        0x0808
    #define     B16OverlayCFG_mask_mosd                        0x0808
    #define   LSb32OverlayCFG_mask_mosd                           6
    #define   LSb16OverlayCFG_mask_mosd                           6
    #define       bOverlayCFG_mask_mosd                        1
    #define   MSK32OverlayCFG_mask_mosd                           0x00000040
    ///////////////////////////////////////////////////////////
    #define     RA_OverlayCFG_cfg                              0x080C

    #define     BA_OverlayCFG_cfg_progressiveOut               0x080C
    #define     B16OverlayCFG_cfg_progressiveOut               0x080C
    #define   LSb32OverlayCFG_cfg_progressiveOut                  0
    #define   LSb16OverlayCFG_cfg_progressiveOut                  0
    #define       bOverlayCFG_cfg_progressiveOut               1
    #define   MSK32OverlayCFG_cfg_progressiveOut                  0x00000001

    #define     BA_OverlayCFG_cfg_progressiveOutSel            0x080C
    #define     B16OverlayCFG_cfg_progressiveOutSel            0x080C
    #define   LSb32OverlayCFG_cfg_progressiveOutSel               1
    #define   LSb16OverlayCFG_cfg_progressiveOutSel               1
    #define       bOverlayCFG_cfg_progressiveOutSel            1
    #define   MSK32OverlayCFG_cfg_progressiveOutSel               0x00000002
    #define        OverlayCFG_cfg_progressiveOutSel_XYZ422                  0x0
    #define        OverlayCFG_cfg_progressiveOutSel_XYZ444                  0x1

    #define     BA_OverlayCFG_cfg_interlaceOut                 0x080C
    #define     B16OverlayCFG_cfg_interlaceOut                 0x080C
    #define   LSb32OverlayCFG_cfg_interlaceOut                    2
    #define   LSb16OverlayCFG_cfg_interlaceOut                    2
    #define       bOverlayCFG_cfg_interlaceOut                 1
    #define   MSK32OverlayCFG_cfg_interlaceOut                    0x00000004

    #define     BA_OverlayCFG_cfg_feedbackSel                  0x080C
    #define     B16OverlayCFG_cfg_feedbackSel                  0x080C
    #define   LSb32OverlayCFG_cfg_feedbackSel                     3
    #define   LSb16OverlayCFG_cfg_feedbackSel                     3
    #define       bOverlayCFG_cfg_feedbackSel                  2
    #define   MSK32OverlayCFG_cfg_feedbackSel                     0x00000018
    #define        OverlayCFG_cfg_feedbackSel_up2pip                        0x0
    #define        OverlayCFG_cfg_feedbackSel_up2pg                         0x1
    #define        OverlayCFG_cfg_feedbackSel_up2ig                         0x2
    #define        OverlayCFG_cfg_feedbackSel_up2mosd                       0x3
    ///////////////////////////////////////////////////////////
    #define     RA_OverlayCFG_start                            0x0810

    #define     BA_OverlayCFG_start_enb                        0x0810
    #define     B16OverlayCFG_start_enb                        0x0810
    #define   LSb32OverlayCFG_start_enb                           0
    #define   LSb16OverlayCFG_start_enb                           0
    #define       bOverlayCFG_start_enb                        1
    #define   MSK32OverlayCFG_start_enb                           0x00000001
    ///////////////////////////////////////////////////////////

    typedef struct SIE_OverlayCFG {
    ///////////////////////////////////////////////////////////
              SIE_FmtCnvt                                      ie_cc;
    ///////////////////////////////////////////////////////////
    #define   GET32OverlayCFG_screen_X(r32)                    _BFGET_(r32,10, 0)
    #define   SET32OverlayCFG_screen_X(r32,v)                  _BFSET_(r32,10, 0,v)
    #define   GET16OverlayCFG_screen_X(r16)                    _BFGET_(r16,10, 0)
    #define   SET16OverlayCFG_screen_X(r16,v)                  _BFSET_(r16,10, 0,v)

    #define   GET32OverlayCFG_screen_Y(r32)                    _BFGET_(r32,26,16)
    #define   SET32OverlayCFG_screen_Y(r32,v)                  _BFSET_(r32,26,16,v)
    #define   GET16OverlayCFG_screen_Y(r16)                    _BFGET_(r16,10, 0)
    #define   SET16OverlayCFG_screen_Y(r16,v)                  _BFSET_(r16,10, 0,v)

    #define     w32OverlayCFG_screen                           {\
            UNSG32 uscreen_X                                   : 11;\
            UNSG32 RSVDx800_b11                                :  5;\
            UNSG32 uscreen_Y                                   : 11;\
            UNSG32 RSVDx800_b27                                :  5;\
          }
    union { UNSG32 u32OverlayCFG_screen;
            struct w32OverlayCFG_screen;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32OverlayCFG_background_Y(r32)                _BFGET_(r32, 7, 0)
    #define   SET32OverlayCFG_background_Y(r32,v)              _BFSET_(r32, 7, 0,v)
    #define   GET16OverlayCFG_background_Y(r16)                _BFGET_(r16, 7, 0)
    #define   SET16OverlayCFG_background_Y(r16,v)              _BFSET_(r16, 7, 0,v)

    #define   GET32OverlayCFG_background_U(r32)                _BFGET_(r32,15, 8)
    #define   SET32OverlayCFG_background_U(r32,v)              _BFSET_(r32,15, 8,v)
    #define   GET16OverlayCFG_background_U(r16)                _BFGET_(r16,15, 8)
    #define   SET16OverlayCFG_background_U(r16,v)              _BFSET_(r16,15, 8,v)

    #define   GET32OverlayCFG_background_V(r32)                _BFGET_(r32,23,16)
    #define   SET32OverlayCFG_background_V(r32,v)              _BFSET_(r32,23,16,v)
    #define   GET16OverlayCFG_background_V(r16)                _BFGET_(r16, 7, 0)
    #define   SET16OverlayCFG_background_V(r16,v)              _BFSET_(r16, 7, 0,v)

    #define     w32OverlayCFG_background                       {\
            UNSG32 ubackground_Y                               :  8;\
            UNSG32 ubackground_U                               :  8;\
            UNSG32 ubackground_V                               :  8;\
            UNSG32 RSVDx804_b24                                :  8;\
          }
    union { UNSG32 u32OverlayCFG_background;
            struct w32OverlayCFG_background;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32OverlayCFG_mask_bg(r32)                     _BFGET_(r32, 0, 0)
    #define   SET32OverlayCFG_mask_bg(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   GET16OverlayCFG_mask_bg(r16)                     _BFGET_(r16, 0, 0)
    #define   SET16OverlayCFG_mask_bg(r16,v)                   _BFSET_(r16, 0, 0,v)

    #define   GET32OverlayCFG_mask_mv(r32)                     _BFGET_(r32, 1, 1)
    #define   SET32OverlayCFG_mask_mv(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   GET16OverlayCFG_mask_mv(r16)                     _BFGET_(r16, 1, 1)
    #define   SET16OverlayCFG_mask_mv(r16,v)                   _BFSET_(r16, 1, 1,v)

    #define   GET32OverlayCFG_mask_pip(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32OverlayCFG_mask_pip(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16OverlayCFG_mask_pip(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16OverlayCFG_mask_pip(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define   GET32OverlayCFG_mask_pg(r32)                     _BFGET_(r32, 3, 3)
    #define   SET32OverlayCFG_mask_pg(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   GET16OverlayCFG_mask_pg(r16)                     _BFGET_(r16, 3, 3)
    #define   SET16OverlayCFG_mask_pg(r16,v)                   _BFSET_(r16, 3, 3,v)

    #define   GET32OverlayCFG_mask_ig(r32)                     _BFGET_(r32, 4, 4)
    #define   SET32OverlayCFG_mask_ig(r32,v)                   _BFSET_(r32, 4, 4,v)
    #define   GET16OverlayCFG_mask_ig(r16)                     _BFGET_(r16, 4, 4)
    #define   SET16OverlayCFG_mask_ig(r16,v)                   _BFSET_(r16, 4, 4,v)

    #define   GET32OverlayCFG_mask_cursor(r32)                 _BFGET_(r32, 5, 5)
    #define   SET32OverlayCFG_mask_cursor(r32,v)               _BFSET_(r32, 5, 5,v)
    #define   GET16OverlayCFG_mask_cursor(r16)                 _BFGET_(r16, 5, 5)
    #define   SET16OverlayCFG_mask_cursor(r16,v)               _BFSET_(r16, 5, 5,v)

    #define   GET32OverlayCFG_mask_mosd(r32)                   _BFGET_(r32, 6, 6)
    #define   SET32OverlayCFG_mask_mosd(r32,v)                 _BFSET_(r32, 6, 6,v)
    #define   GET16OverlayCFG_mask_mosd(r16)                   _BFGET_(r16, 6, 6)
    #define   SET16OverlayCFG_mask_mosd(r16,v)                 _BFSET_(r16, 6, 6,v)

    #define     w32OverlayCFG_mask                             {\
            UNSG32 umask_bg                                    :  1;\
            UNSG32 umask_mv                                    :  1;\
            UNSG32 umask_pip                                   :  1;\
            UNSG32 umask_pg                                    :  1;\
            UNSG32 umask_ig                                    :  1;\
            UNSG32 umask_cursor                                :  1;\
            UNSG32 umask_mosd                                  :  1;\
            UNSG32 RSVDx808_b7                                 : 25;\
          }
    union { UNSG32 u32OverlayCFG_mask;
            struct w32OverlayCFG_mask;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32OverlayCFG_cfg_progressiveOut(r32)          _BFGET_(r32, 0, 0)
    #define   SET32OverlayCFG_cfg_progressiveOut(r32,v)        _BFSET_(r32, 0, 0,v)
    #define   GET16OverlayCFG_cfg_progressiveOut(r16)          _BFGET_(r16, 0, 0)
    #define   SET16OverlayCFG_cfg_progressiveOut(r16,v)        _BFSET_(r16, 0, 0,v)

    #define   GET32OverlayCFG_cfg_progressiveOutSel(r32)       _BFGET_(r32, 1, 1)
    #define   SET32OverlayCFG_cfg_progressiveOutSel(r32,v)     _BFSET_(r32, 1, 1,v)
    #define   GET16OverlayCFG_cfg_progressiveOutSel(r16)       _BFGET_(r16, 1, 1)
    #define   SET16OverlayCFG_cfg_progressiveOutSel(r16,v)     _BFSET_(r16, 1, 1,v)

    #define   GET32OverlayCFG_cfg_interlaceOut(r32)            _BFGET_(r32, 2, 2)
    #define   SET32OverlayCFG_cfg_interlaceOut(r32,v)          _BFSET_(r32, 2, 2,v)
    #define   GET16OverlayCFG_cfg_interlaceOut(r16)            _BFGET_(r16, 2, 2)
    #define   SET16OverlayCFG_cfg_interlaceOut(r16,v)          _BFSET_(r16, 2, 2,v)

    #define   GET32OverlayCFG_cfg_feedbackSel(r32)             _BFGET_(r32, 4, 3)
    #define   SET32OverlayCFG_cfg_feedbackSel(r32,v)           _BFSET_(r32, 4, 3,v)
    #define   GET16OverlayCFG_cfg_feedbackSel(r16)             _BFGET_(r16, 4, 3)
    #define   SET16OverlayCFG_cfg_feedbackSel(r16,v)           _BFSET_(r16, 4, 3,v)

    #define     w32OverlayCFG_cfg                              {\
            UNSG32 ucfg_progressiveOut                         :  1;\
            UNSG32 ucfg_progressiveOutSel                      :  1;\
            UNSG32 ucfg_interlaceOut                           :  1;\
            UNSG32 ucfg_feedbackSel                            :  2;\
            UNSG32 RSVDx80C_b5                                 : 27;\
          }
    union { UNSG32 u32OverlayCFG_cfg;
            struct w32OverlayCFG_cfg;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32OverlayCFG_start_enb(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32OverlayCFG_start_enb(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16OverlayCFG_start_enb(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16OverlayCFG_start_enb(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define     w32OverlayCFG_start                            {\
            UNSG32 ustart_enb                                  :  1;\
            UNSG32 RSVDx810_b1                                 : 31;\
          }
    union { UNSG32 u32OverlayCFG_start;
            struct w32OverlayCFG_start;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx814                                    [1004];
    ///////////////////////////////////////////////////////////
    } SIE_OverlayCFG;

    typedef union  T32OverlayCFG_screen
          { UNSG32 u32;
            struct w32OverlayCFG_screen;
                 } T32OverlayCFG_screen;
    typedef union  T32OverlayCFG_background
          { UNSG32 u32;
            struct w32OverlayCFG_background;
                 } T32OverlayCFG_background;
    typedef union  T32OverlayCFG_mask
          { UNSG32 u32;
            struct w32OverlayCFG_mask;
                 } T32OverlayCFG_mask;
    typedef union  T32OverlayCFG_cfg
          { UNSG32 u32;
            struct w32OverlayCFG_cfg;
                 } T32OverlayCFG_cfg;
    typedef union  T32OverlayCFG_start
          { UNSG32 u32;
            struct w32OverlayCFG_start;
                 } T32OverlayCFG_start;
    ///////////////////////////////////////////////////////////

    typedef union  TOverlayCFG_screen
          { UNSG32 u32[1];
            struct {
            struct w32OverlayCFG_screen;
                   };
                 } TOverlayCFG_screen;
    typedef union  TOverlayCFG_background
          { UNSG32 u32[1];
            struct {
            struct w32OverlayCFG_background;
                   };
                 } TOverlayCFG_background;
    typedef union  TOverlayCFG_mask
          { UNSG32 u32[1];
            struct {
            struct w32OverlayCFG_mask;
                   };
                 } TOverlayCFG_mask;
    typedef union  TOverlayCFG_cfg
          { UNSG32 u32[1];
            struct {
            struct w32OverlayCFG_cfg;
                   };
                 } TOverlayCFG_cfg;
    typedef union  TOverlayCFG_start
          { UNSG32 u32[1];
            struct {
            struct w32OverlayCFG_start;
                   };
                 } TOverlayCFG_start;

    ///////////////////////////////////////////////////////////
     SIGN32 OverlayCFG_drvrd(SIE_OverlayCFG *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 OverlayCFG_drvwr(SIE_OverlayCFG *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void OverlayCFG_reset(SIE_OverlayCFG *p);
     SIGN32 OverlayCFG_cmp  (SIE_OverlayCFG *p, SIE_OverlayCFG *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define OverlayCFG_check(p,pie,pfx,hLOG) OverlayCFG_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define OverlayCFG_print(p,    pfx,hLOG) OverlayCFG_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_VPPOverlayArray
#define h_VPPOverlayArray (){}

    #define     RA_VPPOverlayArray_cpcb                        0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_VPPOverlayArray_planes                      0x1800
    ///////////////////////////////////////////////////////////
    #define     RA_VPPOverlayArray_altOut                      0x188C

    #define     BA_VPPOverlayArray_altOut_enb                  0x188C
    #define     B16VPPOverlayArray_altOut_enb                  0x188C
    #define   LSb32VPPOverlayArray_altOut_enb                     0
    #define   LSb16VPPOverlayArray_altOut_enb                     0
    #define       bVPPOverlayArray_altOut_enb                  1
    #define   MSK32VPPOverlayArray_altOut_enb                     0x00000001
    ///////////////////////////////////////////////////////////

    typedef struct SIE_VPPOverlayArray {
    ///////////////////////////////////////////////////////////
              SIE_OverlayCFG                                   ie_cpcb[2];
    ///////////////////////////////////////////////////////////
              SIE_AllPlaneOverlay                              ie_planes;
    ///////////////////////////////////////////////////////////
    #define   GET32VPPOverlayArray_altOut_enb(r32)             _BFGET_(r32, 0, 0)
    #define   SET32VPPOverlayArray_altOut_enb(r32,v)           _BFSET_(r32, 0, 0,v)
    #define   GET16VPPOverlayArray_altOut_enb(r16)             _BFGET_(r16, 0, 0)
    #define   SET16VPPOverlayArray_altOut_enb(r16,v)           _BFSET_(r16, 0, 0,v)

    #define     w32VPPOverlayArray_altOut                      {\
            UNSG32 ualtOut_enb                                 :  1;\
            UNSG32 RSVDx188C_b1                                : 31;\
          }
    union { UNSG32 u32VPPOverlayArray_altOut;
            struct w32VPPOverlayArray_altOut;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx1890                                   [880];
    ///////////////////////////////////////////////////////////
    } SIE_VPPOverlayArray;

    typedef union  T32VPPOverlayArray_altOut
          { UNSG32 u32;
            struct w32VPPOverlayArray_altOut;
                 } T32VPPOverlayArray_altOut;
    ///////////////////////////////////////////////////////////

    typedef union  TVPPOverlayArray_altOut
          { UNSG32 u32[1];
            struct {
            struct w32VPPOverlayArray_altOut;
                   };
                 } TVPPOverlayArray_altOut;

    ///////////////////////////////////////////////////////////
     SIGN32 VPPOverlayArray_drvrd(SIE_VPPOverlayArray *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 VPPOverlayArray_drvwr(SIE_VPPOverlayArray *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void VPPOverlayArray_reset(SIE_VPPOverlayArray *p);
     SIGN32 VPPOverlayArray_cmp  (SIE_VPPOverlayArray *p, SIE_VPPOverlayArray *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define VPPOverlayArray_check(p,pie,pfx,hLOG) VPPOverlayArray_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define VPPOverlayArray_print(p,    pfx,hLOG) VPPOverlayArray_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif

