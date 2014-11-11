#ifndef VFC_h
#define VFC_h (){}


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



#ifndef h_vProVFC
#define h_vProVFC (){}

    #define     RA_vProVFC_FLOW                                0x0000

    #define     BA_vProVFC_FLOW_enable                         0x0000
    #define     B16vProVFC_FLOW_enable                         0x0000
    #define   LSb32vProVFC_FLOW_enable                            0
    #define   LSb16vProVFC_FLOW_enable                            0
    #define       bvProVFC_FLOW_enable                         1
    #define   MSK32vProVFC_FLOW_enable                            0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_vProVFC_CTRL                                0x0004

    #define     BA_vProVFC_CTRL_mode                           0x0004
    #define     B16vProVFC_CTRL_mode                           0x0004
    #define   LSb32vProVFC_CTRL_mode                              0
    #define   LSb16vProVFC_CTRL_mode                              0
    #define       bvProVFC_CTRL_mode                           2
    #define   MSK32vProVFC_CTRL_mode                              0x00000003
    #define        vProVFC_CTRL_mode_UYVY                                   0x0
    #define        vProVFC_CTRL_mode_PLANAR                                 0x1
    ///////////////////////////////////////////////////////////
    #define     RA_vProVFC_STATUS                              0x0008

    #define     BA_vProVFC_STATUS_done                         0x0008
    #define     B16vProVFC_STATUS_done                         0x0008
    #define   LSb32vProVFC_STATUS_done                            0
    #define   LSb16vProVFC_STATUS_done                            0
    #define       bvProVFC_STATUS_done                         1
    #define   MSK32vProVFC_STATUS_done                            0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_vProVFC_BASE_Y                              0x000C

    #define     BA_vProVFC_BASE_Y_addr                         0x000C
    #define     B16vProVFC_BASE_Y_addr                         0x000C
    #define   LSb32vProVFC_BASE_Y_addr                            0
    #define   LSb16vProVFC_BASE_Y_addr                            0
    #define       bvProVFC_BASE_Y_addr                         32
    #define   MSK32vProVFC_BASE_Y_addr                            0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_vProVFC_BASE_CB                             0x0010

    #define     BA_vProVFC_BASE_CB_addr                        0x0010
    #define     B16vProVFC_BASE_CB_addr                        0x0010
    #define   LSb32vProVFC_BASE_CB_addr                           0
    #define   LSb16vProVFC_BASE_CB_addr                           0
    #define       bvProVFC_BASE_CB_addr                        32
    #define   MSK32vProVFC_BASE_CB_addr                           0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_vProVFC_BASE_CR                             0x0014

    #define     BA_vProVFC_BASE_CR_addr                        0x0014
    #define     B16vProVFC_BASE_CR_addr                        0x0014
    #define   LSb32vProVFC_BASE_CR_addr                           0
    #define   LSb16vProVFC_BASE_CR_addr                           0
    #define       bvProVFC_BASE_CR_addr                        32
    #define   MSK32vProVFC_BASE_CR_addr                           0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_vProVFC_SIZE                                0x0018

    #define     BA_vProVFC_SIZE_width                          0x0018
    #define     B16vProVFC_SIZE_width                          0x0018
    #define   LSb32vProVFC_SIZE_width                             0
    #define   LSb16vProVFC_SIZE_width                             0
    #define       bvProVFC_SIZE_width                          16
    #define   MSK32vProVFC_SIZE_width                             0x0000FFFF

    #define     BA_vProVFC_SIZE_height                         0x001A
    #define     B16vProVFC_SIZE_height                         0x001A
    #define   LSb32vProVFC_SIZE_height                            16
    #define   LSb16vProVFC_SIZE_height                            0
    #define       bvProVFC_SIZE_height                         16
    #define   MSK32vProVFC_SIZE_height                            0xFFFF0000
    ///////////////////////////////////////////////////////////
    #define     RA_vProVFC_STRIDE                              0x001C

    #define     BA_vProVFC_STRIDE_luma                         0x001C
    #define     B16vProVFC_STRIDE_luma                         0x001C
    #define   LSb32vProVFC_STRIDE_luma                            0
    #define   LSb16vProVFC_STRIDE_luma                            0
    #define       bvProVFC_STRIDE_luma                         16
    #define   MSK32vProVFC_STRIDE_luma                            0x0000FFFF

    #define     BA_vProVFC_STRIDE_chroma                       0x001E
    #define     B16vProVFC_STRIDE_chroma                       0x001E
    #define   LSb32vProVFC_STRIDE_chroma                          16
    #define   LSb16vProVFC_STRIDE_chroma                          0
    #define       bvProVFC_STRIDE_chroma                       16
    #define   MSK32vProVFC_STRIDE_chroma                          0xFFFF0000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_vProVFC {
    ///////////////////////////////////////////////////////////
    #define   GET32vProVFC_FLOW_enable(r32)                    _BFGET_(r32, 0, 0)
    #define   SET32vProVFC_FLOW_enable(r32,v)                  _BFSET_(r32, 0, 0,v)
    #define   GET16vProVFC_FLOW_enable(r16)                    _BFGET_(r16, 0, 0)
    #define   SET16vProVFC_FLOW_enable(r16,v)                  _BFSET_(r16, 0, 0,v)

    #define     w32vProVFC_FLOW                                {\
            UNSG32 uFLOW_enable                                :  1;\
            UNSG32 RSVDx0_b1                                   : 31;\
          }
    union { UNSG32 u32vProVFC_FLOW;
            struct w32vProVFC_FLOW;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32vProVFC_CTRL_mode(r32)                      _BFGET_(r32, 1, 0)
    #define   SET32vProVFC_CTRL_mode(r32,v)                    _BFSET_(r32, 1, 0,v)
    #define   GET16vProVFC_CTRL_mode(r16)                      _BFGET_(r16, 1, 0)
    #define   SET16vProVFC_CTRL_mode(r16,v)                    _BFSET_(r16, 1, 0,v)

    #define     w32vProVFC_CTRL                                {\
            UNSG32 uCTRL_mode                                  :  2;\
            UNSG32 RSVDx4_b2                                   : 30;\
          }
    union { UNSG32 u32vProVFC_CTRL;
            struct w32vProVFC_CTRL;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32vProVFC_STATUS_done(r32)                    _BFGET_(r32, 0, 0)
    #define   SET32vProVFC_STATUS_done(r32,v)                  _BFSET_(r32, 0, 0,v)
    #define   GET16vProVFC_STATUS_done(r16)                    _BFGET_(r16, 0, 0)
    #define   SET16vProVFC_STATUS_done(r16,v)                  _BFSET_(r16, 0, 0,v)

    #define     w32vProVFC_STATUS                              {\
            UNSG32 uSTATUS_done                                :  1;\
            UNSG32 RSVDx8_b1                                   : 31;\
          }
    union { UNSG32 u32vProVFC_STATUS;
            struct w32vProVFC_STATUS;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32vProVFC_BASE_Y_addr(r32)                    _BFGET_(r32,31, 0)
    #define   SET32vProVFC_BASE_Y_addr(r32,v)                  _BFSET_(r32,31, 0,v)

    #define     w32vProVFC_BASE_Y                              {\
            UNSG32 uBASE_Y_addr                                : 32;\
          }
    union { UNSG32 u32vProVFC_BASE_Y;
            struct w32vProVFC_BASE_Y;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32vProVFC_BASE_CB_addr(r32)                   _BFGET_(r32,31, 0)
    #define   SET32vProVFC_BASE_CB_addr(r32,v)                 _BFSET_(r32,31, 0,v)

    #define     w32vProVFC_BASE_CB                             {\
            UNSG32 uBASE_CB_addr                               : 32;\
          }
    union { UNSG32 u32vProVFC_BASE_CB;
            struct w32vProVFC_BASE_CB;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32vProVFC_BASE_CR_addr(r32)                   _BFGET_(r32,31, 0)
    #define   SET32vProVFC_BASE_CR_addr(r32,v)                 _BFSET_(r32,31, 0,v)

    #define     w32vProVFC_BASE_CR                             {\
            UNSG32 uBASE_CR_addr                               : 32;\
          }
    union { UNSG32 u32vProVFC_BASE_CR;
            struct w32vProVFC_BASE_CR;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32vProVFC_SIZE_width(r32)                     _BFGET_(r32,15, 0)
    #define   SET32vProVFC_SIZE_width(r32,v)                   _BFSET_(r32,15, 0,v)
    #define   GET16vProVFC_SIZE_width(r16)                     _BFGET_(r16,15, 0)
    #define   SET16vProVFC_SIZE_width(r16,v)                   _BFSET_(r16,15, 0,v)

    #define   GET32vProVFC_SIZE_height(r32)                    _BFGET_(r32,31,16)
    #define   SET32vProVFC_SIZE_height(r32,v)                  _BFSET_(r32,31,16,v)
    #define   GET16vProVFC_SIZE_height(r16)                    _BFGET_(r16,15, 0)
    #define   SET16vProVFC_SIZE_height(r16,v)                  _BFSET_(r16,15, 0,v)

    #define     w32vProVFC_SIZE                                {\
            UNSG32 uSIZE_width                                 : 16;\
            UNSG32 uSIZE_height                                : 16;\
          }
    union { UNSG32 u32vProVFC_SIZE;
            struct w32vProVFC_SIZE;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32vProVFC_STRIDE_luma(r32)                    _BFGET_(r32,15, 0)
    #define   SET32vProVFC_STRIDE_luma(r32,v)                  _BFSET_(r32,15, 0,v)
    #define   GET16vProVFC_STRIDE_luma(r16)                    _BFGET_(r16,15, 0)
    #define   SET16vProVFC_STRIDE_luma(r16,v)                  _BFSET_(r16,15, 0,v)

    #define   GET32vProVFC_STRIDE_chroma(r32)                  _BFGET_(r32,31,16)
    #define   SET32vProVFC_STRIDE_chroma(r32,v)                _BFSET_(r32,31,16,v)
    #define   GET16vProVFC_STRIDE_chroma(r16)                  _BFGET_(r16,15, 0)
    #define   SET16vProVFC_STRIDE_chroma(r16,v)                _BFSET_(r16,15, 0,v)

    #define     w32vProVFC_STRIDE                              {\
            UNSG32 uSTRIDE_luma                                : 16;\
            UNSG32 uSTRIDE_chroma                              : 16;\
          }
    union { UNSG32 u32vProVFC_STRIDE;
            struct w32vProVFC_STRIDE;
          };
    ///////////////////////////////////////////////////////////
    } SIE_vProVFC;

    typedef union  T32vProVFC_FLOW
          { UNSG32 u32;
            struct w32vProVFC_FLOW;
                 } T32vProVFC_FLOW;
    typedef union  T32vProVFC_CTRL
          { UNSG32 u32;
            struct w32vProVFC_CTRL;
                 } T32vProVFC_CTRL;
    typedef union  T32vProVFC_STATUS
          { UNSG32 u32;
            struct w32vProVFC_STATUS;
                 } T32vProVFC_STATUS;
    typedef union  T32vProVFC_BASE_Y
          { UNSG32 u32;
            struct w32vProVFC_BASE_Y;
                 } T32vProVFC_BASE_Y;
    typedef union  T32vProVFC_BASE_CB
          { UNSG32 u32;
            struct w32vProVFC_BASE_CB;
                 } T32vProVFC_BASE_CB;
    typedef union  T32vProVFC_BASE_CR
          { UNSG32 u32;
            struct w32vProVFC_BASE_CR;
                 } T32vProVFC_BASE_CR;
    typedef union  T32vProVFC_SIZE
          { UNSG32 u32;
            struct w32vProVFC_SIZE;
                 } T32vProVFC_SIZE;
    typedef union  T32vProVFC_STRIDE
          { UNSG32 u32;
            struct w32vProVFC_STRIDE;
                 } T32vProVFC_STRIDE;
    ///////////////////////////////////////////////////////////

    typedef union  TvProVFC_FLOW
          { UNSG32 u32[1];
            struct {
            struct w32vProVFC_FLOW;
                   };
                 } TvProVFC_FLOW;
    typedef union  TvProVFC_CTRL
          { UNSG32 u32[1];
            struct {
            struct w32vProVFC_CTRL;
                   };
                 } TvProVFC_CTRL;
    typedef union  TvProVFC_STATUS
          { UNSG32 u32[1];
            struct {
            struct w32vProVFC_STATUS;
                   };
                 } TvProVFC_STATUS;
    typedef union  TvProVFC_BASE_Y
          { UNSG32 u32[1];
            struct {
            struct w32vProVFC_BASE_Y;
                   };
                 } TvProVFC_BASE_Y;
    typedef union  TvProVFC_BASE_CB
          { UNSG32 u32[1];
            struct {
            struct w32vProVFC_BASE_CB;
                   };
                 } TvProVFC_BASE_CB;
    typedef union  TvProVFC_BASE_CR
          { UNSG32 u32[1];
            struct {
            struct w32vProVFC_BASE_CR;
                   };
                 } TvProVFC_BASE_CR;
    typedef union  TvProVFC_SIZE
          { UNSG32 u32[1];
            struct {
            struct w32vProVFC_SIZE;
                   };
                 } TvProVFC_SIZE;
    typedef union  TvProVFC_STRIDE
          { UNSG32 u32[1];
            struct {
            struct w32vProVFC_STRIDE;
                   };
                 } TvProVFC_STRIDE;

    ///////////////////////////////////////////////////////////
     SIGN32 vProVFC_drvrd(SIE_vProVFC *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 vProVFC_drvwr(SIE_vProVFC *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void vProVFC_reset(SIE_vProVFC *p);
     SIGN32 vProVFC_cmp  (SIE_vProVFC *p, SIE_vProVFC *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define vProVFC_check(p,pie,pfx,hLOG) vProVFC_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define vProVFC_print(p,    pfx,hLOG) vProVFC_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif

