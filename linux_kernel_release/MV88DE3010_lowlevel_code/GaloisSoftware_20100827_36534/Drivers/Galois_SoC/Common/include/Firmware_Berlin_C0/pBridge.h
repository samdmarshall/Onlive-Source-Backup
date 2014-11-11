#ifndef pBridge_h
#define pBridge_h (){}


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



#ifndef h_SemaINTR
#define h_SemaINTR (){}

    #define     RA_SemaINTR_mask                               0x0000

    #define     BA_SemaINTR_mask_empty                         0x0000
    #define     B16SemaINTR_mask_empty                         0x0000
    #define   LSb32SemaINTR_mask_empty                            0
    #define   LSb16SemaINTR_mask_empty                            0
    #define       bSemaINTR_mask_empty                         1
    #define   MSK32SemaINTR_mask_empty                            0x00000001

    #define     BA_SemaINTR_mask_full                          0x0000
    #define     B16SemaINTR_mask_full                          0x0000
    #define   LSb32SemaINTR_mask_full                             1
    #define   LSb16SemaINTR_mask_full                             1
    #define       bSemaINTR_mask_full                          1
    #define   MSK32SemaINTR_mask_full                             0x00000002

    #define     BA_SemaINTR_mask_almostEmpty                   0x0000
    #define     B16SemaINTR_mask_almostEmpty                   0x0000
    #define   LSb32SemaINTR_mask_almostEmpty                      2
    #define   LSb16SemaINTR_mask_almostEmpty                      2
    #define       bSemaINTR_mask_almostEmpty                   1
    #define   MSK32SemaINTR_mask_almostEmpty                      0x00000004

    #define     BA_SemaINTR_mask_almostFull                    0x0000
    #define     B16SemaINTR_mask_almostFull                    0x0000
    #define   LSb32SemaINTR_mask_almostFull                       3
    #define   LSb16SemaINTR_mask_almostFull                       3
    #define       bSemaINTR_mask_almostFull                    1
    #define   MSK32SemaINTR_mask_almostFull                       0x00000008
    ///////////////////////////////////////////////////////////

    typedef struct SIE_SemaINTR {
    ///////////////////////////////////////////////////////////
    #define   GET32SemaINTR_mask_empty(r32)                    _BFGET_(r32, 0, 0)
    #define   SET32SemaINTR_mask_empty(r32,v)                  _BFSET_(r32, 0, 0,v)
    #define   GET16SemaINTR_mask_empty(r16)                    _BFGET_(r16, 0, 0)
    #define   SET16SemaINTR_mask_empty(r16,v)                  _BFSET_(r16, 0, 0,v)

    #define   GET32SemaINTR_mask_full(r32)                     _BFGET_(r32, 1, 1)
    #define   SET32SemaINTR_mask_full(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   GET16SemaINTR_mask_full(r16)                     _BFGET_(r16, 1, 1)
    #define   SET16SemaINTR_mask_full(r16,v)                   _BFSET_(r16, 1, 1,v)

    #define   GET32SemaINTR_mask_almostEmpty(r32)              _BFGET_(r32, 2, 2)
    #define   SET32SemaINTR_mask_almostEmpty(r32,v)            _BFSET_(r32, 2, 2,v)
    #define   GET16SemaINTR_mask_almostEmpty(r16)              _BFGET_(r16, 2, 2)
    #define   SET16SemaINTR_mask_almostEmpty(r16,v)            _BFSET_(r16, 2, 2,v)

    #define   GET32SemaINTR_mask_almostFull(r32)               _BFGET_(r32, 3, 3)
    #define   SET32SemaINTR_mask_almostFull(r32,v)             _BFSET_(r32, 3, 3,v)
    #define   GET16SemaINTR_mask_almostFull(r16)               _BFGET_(r16, 3, 3)
    #define   SET16SemaINTR_mask_almostFull(r16,v)             _BFSET_(r16, 3, 3,v)

    #define     w32SemaINTR_mask                               {\
            UNSG32 umask_empty                                 :  1;\
            UNSG32 umask_full                                  :  1;\
            UNSG32 umask_almostEmpty                           :  1;\
            UNSG32 umask_almostFull                            :  1;\
            UNSG32 RSVDx0_b4                                   : 28;\
          }
    union { UNSG32 u32SemaINTR_mask;
            struct w32SemaINTR_mask;
          };
    ///////////////////////////////////////////////////////////
    } SIE_SemaINTR;

    typedef union  T32SemaINTR_mask
          { UNSG32 u32;
            struct w32SemaINTR_mask;
                 } T32SemaINTR_mask;
    ///////////////////////////////////////////////////////////

    typedef union  TSemaINTR_mask
          { UNSG32 u32[1];
            struct {
            struct w32SemaINTR_mask;
                   };
                 } TSemaINTR_mask;

    ///////////////////////////////////////////////////////////
     SIGN32 SemaINTR_drvrd(SIE_SemaINTR *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 SemaINTR_drvwr(SIE_SemaINTR *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void SemaINTR_reset(SIE_SemaINTR *p);
     SIGN32 SemaINTR_cmp  (SIE_SemaINTR *p, SIE_SemaINTR *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define SemaINTR_check(p,pie,pfx,hLOG) SemaINTR_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define SemaINTR_print(p,    pfx,hLOG) SemaINTR_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_Semaphore
#define h_Semaphore (){}

    #define     RA_Semaphore_CFG                               0x0000

    #define     BA_Semaphore_CFG_DEPTH                         0x0000
    #define     B16Semaphore_CFG_DEPTH                         0x0000
    #define   LSb32Semaphore_CFG_DEPTH                            0
    #define   LSb16Semaphore_CFG_DEPTH                            0
    #define       bSemaphore_CFG_DEPTH                         16
    #define   MSK32Semaphore_CFG_DEPTH                            0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_Semaphore_INTR                              0x0004
    ///////////////////////////////////////////////////////////
    #define     RA_Semaphore_mask                              0x0010

    #define     BA_Semaphore_mask_full                         0x0010
    #define     B16Semaphore_mask_full                         0x0010
    #define   LSb32Semaphore_mask_full                            0
    #define   LSb16Semaphore_mask_full                            0
    #define       bSemaphore_mask_full                         1
    #define   MSK32Semaphore_mask_full                            0x00000001

    #define     BA_Semaphore_mask_emp                          0x0010
    #define     B16Semaphore_mask_emp                          0x0010
    #define   LSb32Semaphore_mask_emp                             1
    #define   LSb16Semaphore_mask_emp                             1
    #define       bSemaphore_mask_emp                          1
    #define   MSK32Semaphore_mask_emp                             0x00000002
    ///////////////////////////////////////////////////////////

    typedef struct SIE_Semaphore {
    ///////////////////////////////////////////////////////////
    #define   GET32Semaphore_CFG_DEPTH(r32)                    _BFGET_(r32,15, 0)
    #define   SET32Semaphore_CFG_DEPTH(r32,v)                  _BFSET_(r32,15, 0,v)
    #define   GET16Semaphore_CFG_DEPTH(r16)                    _BFGET_(r16,15, 0)
    #define   SET16Semaphore_CFG_DEPTH(r16,v)                  _BFSET_(r16,15, 0,v)

    #define     w32Semaphore_CFG                               {\
            UNSG32 uCFG_DEPTH                                  : 16;\
            UNSG32 RSVDx0_b16                                  : 16;\
          }
    union { UNSG32 u32Semaphore_CFG;
            struct w32Semaphore_CFG;
          };
    ///////////////////////////////////////////////////////////
              SIE_SemaINTR                                     ie_INTR[3];
    ///////////////////////////////////////////////////////////
    #define   GET32Semaphore_mask_full(r32)                    _BFGET_(r32, 0, 0)
    #define   SET32Semaphore_mask_full(r32,v)                  _BFSET_(r32, 0, 0,v)
    #define   GET16Semaphore_mask_full(r16)                    _BFGET_(r16, 0, 0)
    #define   SET16Semaphore_mask_full(r16,v)                  _BFSET_(r16, 0, 0,v)

    #define   GET32Semaphore_mask_emp(r32)                     _BFGET_(r32, 1, 1)
    #define   SET32Semaphore_mask_emp(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   GET16Semaphore_mask_emp(r16)                     _BFGET_(r16, 1, 1)
    #define   SET16Semaphore_mask_emp(r16,v)                   _BFSET_(r16, 1, 1,v)

    #define     w32Semaphore_mask                              {\
            UNSG32 umask_full                                  :  1;\
            UNSG32 umask_emp                                   :  1;\
            UNSG32 RSVDx10_b2                                  : 30;\
          }
    union { UNSG32 u32Semaphore_mask;
            struct w32Semaphore_mask;
          };
    ///////////////////////////////////////////////////////////
    } SIE_Semaphore;

    typedef union  T32Semaphore_CFG
          { UNSG32 u32;
            struct w32Semaphore_CFG;
                 } T32Semaphore_CFG;
    typedef union  T32Semaphore_mask
          { UNSG32 u32;
            struct w32Semaphore_mask;
                 } T32Semaphore_mask;
    ///////////////////////////////////////////////////////////

    typedef union  TSemaphore_CFG
          { UNSG32 u32[1];
            struct {
            struct w32Semaphore_CFG;
                   };
                 } TSemaphore_CFG;
    typedef union  TSemaphore_mask
          { UNSG32 u32[1];
            struct {
            struct w32Semaphore_mask;
                   };
                 } TSemaphore_mask;

    ///////////////////////////////////////////////////////////
     SIGN32 Semaphore_drvrd(SIE_Semaphore *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 Semaphore_drvwr(SIE_Semaphore *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void Semaphore_reset(SIE_Semaphore *p);
     SIGN32 Semaphore_cmp  (SIE_Semaphore *p, SIE_Semaphore *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define Semaphore_check(p,pie,pfx,hLOG) Semaphore_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define Semaphore_print(p,    pfx,hLOG) Semaphore_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_SemaQuery
#define h_SemaQuery (){}

    #define     RA_SemaQuery_RESP                              0x0000

    #define     BA_SemaQuery_RESP_CNT                          0x0000
    #define     B16SemaQuery_RESP_CNT                          0x0000
    #define   LSb32SemaQuery_RESP_CNT                             0
    #define   LSb16SemaQuery_RESP_CNT                             0
    #define       bSemaQuery_RESP_CNT                          16
    #define   MSK32SemaQuery_RESP_CNT                             0x0000FFFF

    #define     BA_SemaQuery_RESP_PTR                          0x0002
    #define     B16SemaQuery_RESP_PTR                          0x0002
    #define   LSb32SemaQuery_RESP_PTR                             16
    #define   LSb16SemaQuery_RESP_PTR                             0
    #define       bSemaQuery_RESP_PTR                          16
    #define   MSK32SemaQuery_RESP_PTR                             0xFFFF0000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_SemaQuery {
    ///////////////////////////////////////////////////////////
    #define   GET32SemaQuery_RESP_CNT(r32)                     _BFGET_(r32,15, 0)
    #define   SET32SemaQuery_RESP_CNT(r32,v)                   _BFSET_(r32,15, 0,v)
    #define   GET16SemaQuery_RESP_CNT(r16)                     _BFGET_(r16,15, 0)
    #define   SET16SemaQuery_RESP_CNT(r16,v)                   _BFSET_(r16,15, 0,v)

    #define   GET32SemaQuery_RESP_PTR(r32)                     _BFGET_(r32,31,16)
    #define   SET32SemaQuery_RESP_PTR(r32,v)                   _BFSET_(r32,31,16,v)
    #define   GET16SemaQuery_RESP_PTR(r16)                     _BFGET_(r16,15, 0)
    #define   SET16SemaQuery_RESP_PTR(r16,v)                   _BFSET_(r16,15, 0,v)

    #define     w32SemaQuery_RESP                              {\
            UNSG32 uRESP_CNT                                   : 16;\
            UNSG32 uRESP_PTR                                   : 16;\
          }
    union { UNSG32 u32SemaQuery_RESP;
            struct w32SemaQuery_RESP;
          };
    ///////////////////////////////////////////////////////////
    } SIE_SemaQuery;

    typedef union  T32SemaQuery_RESP
          { UNSG32 u32;
            struct w32SemaQuery_RESP;
                 } T32SemaQuery_RESP;
    ///////////////////////////////////////////////////////////

    typedef union  TSemaQuery_RESP
          { UNSG32 u32[1];
            struct {
            struct w32SemaQuery_RESP;
                   };
                 } TSemaQuery_RESP;

    ///////////////////////////////////////////////////////////
     SIGN32 SemaQuery_drvrd(SIE_SemaQuery *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 SemaQuery_drvwr(SIE_SemaQuery *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void SemaQuery_reset(SIE_SemaQuery *p);
     SIGN32 SemaQuery_cmp  (SIE_SemaQuery *p, SIE_SemaQuery *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define SemaQuery_check(p,pie,pfx,hLOG) SemaQuery_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define SemaQuery_print(p,    pfx,hLOG) SemaQuery_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_SemaQueryMap
#define h_SemaQueryMap (){}

    #define     RA_SemaQueryMap_ADDR                           0x0000

    #define     BA_SemaQueryMap_ADDR_byte                      0x0000
    #define     B16SemaQueryMap_ADDR_byte                      0x0000
    #define   LSb32SemaQueryMap_ADDR_byte                         0
    #define   LSb16SemaQueryMap_ADDR_byte                         0
    #define       bSemaQueryMap_ADDR_byte                      2
    #define   MSK32SemaQueryMap_ADDR_byte                         0x00000003

    #define     BA_SemaQueryMap_ADDR_ID                        0x0000
    #define     B16SemaQueryMap_ADDR_ID                        0x0000
    #define   LSb32SemaQueryMap_ADDR_ID                           2
    #define   LSb16SemaQueryMap_ADDR_ID                           2
    #define       bSemaQueryMap_ADDR_ID                        5
    #define   MSK32SemaQueryMap_ADDR_ID                           0x0000007C

    #define     BA_SemaQueryMap_ADDR_master                    0x0000
    #define     B16SemaQueryMap_ADDR_master                    0x0000
    #define   LSb32SemaQueryMap_ADDR_master                       7
    #define   LSb16SemaQueryMap_ADDR_master                       7
    #define       bSemaQueryMap_ADDR_master                    1
    #define   MSK32SemaQueryMap_ADDR_master                       0x00000080
    #define        SemaQueryMap_ADDR_master_producer                        0x0
    #define        SemaQueryMap_ADDR_master_consumer                        0x1
    ///////////////////////////////////////////////////////////

    typedef struct SIE_SemaQueryMap {
    ///////////////////////////////////////////////////////////
    #define   GET32SemaQueryMap_ADDR_byte(r32)                 _BFGET_(r32, 1, 0)
    #define   SET32SemaQueryMap_ADDR_byte(r32,v)               _BFSET_(r32, 1, 0,v)
    #define   GET16SemaQueryMap_ADDR_byte(r16)                 _BFGET_(r16, 1, 0)
    #define   SET16SemaQueryMap_ADDR_byte(r16,v)               _BFSET_(r16, 1, 0,v)

    #define   GET32SemaQueryMap_ADDR_ID(r32)                   _BFGET_(r32, 6, 2)
    #define   SET32SemaQueryMap_ADDR_ID(r32,v)                 _BFSET_(r32, 6, 2,v)
    #define   GET16SemaQueryMap_ADDR_ID(r16)                   _BFGET_(r16, 6, 2)
    #define   SET16SemaQueryMap_ADDR_ID(r16,v)                 _BFSET_(r16, 6, 2,v)

    #define   GET32SemaQueryMap_ADDR_master(r32)               _BFGET_(r32, 7, 7)
    #define   SET32SemaQueryMap_ADDR_master(r32,v)             _BFSET_(r32, 7, 7,v)
    #define   GET16SemaQueryMap_ADDR_master(r16)               _BFGET_(r16, 7, 7)
    #define   SET16SemaQueryMap_ADDR_master(r16,v)             _BFSET_(r16, 7, 7,v)

    #define     w32SemaQueryMap_ADDR                           {\
            UNSG32 uADDR_byte                                  :  2;\
            UNSG32 uADDR_ID                                    :  5;\
            UNSG32 uADDR_master                                :  1;\
            UNSG32 RSVDx0_b8                                   : 24;\
          }
    union { UNSG32 u32SemaQueryMap_ADDR;
            struct w32SemaQueryMap_ADDR;
          };
    ///////////////////////////////////////////////////////////
    } SIE_SemaQueryMap;

    typedef union  T32SemaQueryMap_ADDR
          { UNSG32 u32;
            struct w32SemaQueryMap_ADDR;
                 } T32SemaQueryMap_ADDR;
    ///////////////////////////////////////////////////////////

    typedef union  TSemaQueryMap_ADDR
          { UNSG32 u32[1];
            struct {
            struct w32SemaQueryMap_ADDR;
                   };
                 } TSemaQueryMap_ADDR;

    ///////////////////////////////////////////////////////////
     SIGN32 SemaQueryMap_drvrd(SIE_SemaQueryMap *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 SemaQueryMap_drvwr(SIE_SemaQueryMap *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void SemaQueryMap_reset(SIE_SemaQueryMap *p);
     SIGN32 SemaQueryMap_cmp  (SIE_SemaQueryMap *p, SIE_SemaQueryMap *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define SemaQueryMap_check(p,pie,pfx,hLOG) SemaQueryMap_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define SemaQueryMap_print(p,    pfx,hLOG) SemaQueryMap_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_SemaHub
#define h_SemaHub (){}

    #define     RA_SemaHub_Query                               0x0000
    #define     RA_SemaHub_counter                             0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_SemaHub_ARR                                 0x0100
    #define     RA_SemaHub_cell                                0x0100
    ///////////////////////////////////////////////////////////
    #define     RA_SemaHub_PUSH                                0x0380

    #define     BA_SemaHub_PUSH_ID                             0x0380
    #define     B16SemaHub_PUSH_ID                             0x0380
    #define   LSb32SemaHub_PUSH_ID                                0
    #define   LSb16SemaHub_PUSH_ID                                0
    #define       bSemaHub_PUSH_ID                             8
    #define   MSK32SemaHub_PUSH_ID                                0x000000FF

    #define     BA_SemaHub_PUSH_delta                          0x0381
    #define     B16SemaHub_PUSH_delta                          0x0380
    #define   LSb32SemaHub_PUSH_delta                             8
    #define   LSb16SemaHub_PUSH_delta                             8
    #define       bSemaHub_PUSH_delta                          8
    #define   MSK32SemaHub_PUSH_delta                             0x0000FF00
    ///////////////////////////////////////////////////////////
    #define     RA_SemaHub_POP                                 0x0384

    #define     BA_SemaHub_POP_ID                              0x0384
    #define     B16SemaHub_POP_ID                              0x0384
    #define   LSb32SemaHub_POP_ID                                 0
    #define   LSb16SemaHub_POP_ID                                 0
    #define       bSemaHub_POP_ID                              8
    #define   MSK32SemaHub_POP_ID                                 0x000000FF

    #define     BA_SemaHub_POP_delta                           0x0385
    #define     B16SemaHub_POP_delta                           0x0384
    #define   LSb32SemaHub_POP_delta                              8
    #define   LSb16SemaHub_POP_delta                              8
    #define       bSemaHub_POP_delta                           8
    #define   MSK32SemaHub_POP_delta                              0x0000FF00
    ///////////////////////////////////////////////////////////
    #define     RA_SemaHub_empty                               0x0388

    #define     BA_SemaHub_empty_ST_0i                         0x0388
    #define     B16SemaHub_empty_ST_0i                         0x0388
    #define   LSb32SemaHub_empty_ST_0i                            0
    #define   LSb16SemaHub_empty_ST_0i                            0
    #define       bSemaHub_empty_ST_0i                         1
    #define   MSK32SemaHub_empty_ST_0i                            0x00000001

    #define     BA_SemaHub_empty_ST_1i                         0x0388
    #define     B16SemaHub_empty_ST_1i                         0x0388
    #define   LSb32SemaHub_empty_ST_1i                            1
    #define   LSb16SemaHub_empty_ST_1i                            1
    #define       bSemaHub_empty_ST_1i                         1
    #define   MSK32SemaHub_empty_ST_1i                            0x00000002

    #define     BA_SemaHub_empty_ST_2i                         0x0388
    #define     B16SemaHub_empty_ST_2i                         0x0388
    #define   LSb32SemaHub_empty_ST_2i                            2
    #define   LSb16SemaHub_empty_ST_2i                            2
    #define       bSemaHub_empty_ST_2i                         1
    #define   MSK32SemaHub_empty_ST_2i                            0x00000004

    #define     BA_SemaHub_empty_ST_3i                         0x0388
    #define     B16SemaHub_empty_ST_3i                         0x0388
    #define   LSb32SemaHub_empty_ST_3i                            3
    #define   LSb16SemaHub_empty_ST_3i                            3
    #define       bSemaHub_empty_ST_3i                         1
    #define   MSK32SemaHub_empty_ST_3i                            0x00000008

    #define     BA_SemaHub_empty_ST_4i                         0x0388
    #define     B16SemaHub_empty_ST_4i                         0x0388
    #define   LSb32SemaHub_empty_ST_4i                            4
    #define   LSb16SemaHub_empty_ST_4i                            4
    #define       bSemaHub_empty_ST_4i                         1
    #define   MSK32SemaHub_empty_ST_4i                            0x00000010

    #define     BA_SemaHub_empty_ST_5i                         0x0388
    #define     B16SemaHub_empty_ST_5i                         0x0388
    #define   LSb32SemaHub_empty_ST_5i                            5
    #define   LSb16SemaHub_empty_ST_5i                            5
    #define       bSemaHub_empty_ST_5i                         1
    #define   MSK32SemaHub_empty_ST_5i                            0x00000020

    #define     BA_SemaHub_empty_ST_6i                         0x0388
    #define     B16SemaHub_empty_ST_6i                         0x0388
    #define   LSb32SemaHub_empty_ST_6i                            6
    #define   LSb16SemaHub_empty_ST_6i                            6
    #define       bSemaHub_empty_ST_6i                         1
    #define   MSK32SemaHub_empty_ST_6i                            0x00000040

    #define     BA_SemaHub_empty_ST_7i                         0x0388
    #define     B16SemaHub_empty_ST_7i                         0x0388
    #define   LSb32SemaHub_empty_ST_7i                            7
    #define   LSb16SemaHub_empty_ST_7i                            7
    #define       bSemaHub_empty_ST_7i                         1
    #define   MSK32SemaHub_empty_ST_7i                            0x00000080

    #define     BA_SemaHub_empty_ST_8i                         0x0389
    #define     B16SemaHub_empty_ST_8i                         0x0388
    #define   LSb32SemaHub_empty_ST_8i                            8
    #define   LSb16SemaHub_empty_ST_8i                            8
    #define       bSemaHub_empty_ST_8i                         1
    #define   MSK32SemaHub_empty_ST_8i                            0x00000100

    #define     BA_SemaHub_empty_ST_9i                         0x0389
    #define     B16SemaHub_empty_ST_9i                         0x0388
    #define   LSb32SemaHub_empty_ST_9i                            9
    #define   LSb16SemaHub_empty_ST_9i                            9
    #define       bSemaHub_empty_ST_9i                         1
    #define   MSK32SemaHub_empty_ST_9i                            0x00000200

    #define     BA_SemaHub_empty_ST_10i                        0x0389
    #define     B16SemaHub_empty_ST_10i                        0x0388
    #define   LSb32SemaHub_empty_ST_10i                           10
    #define   LSb16SemaHub_empty_ST_10i                           10
    #define       bSemaHub_empty_ST_10i                        1
    #define   MSK32SemaHub_empty_ST_10i                           0x00000400

    #define     BA_SemaHub_empty_ST_11i                        0x0389
    #define     B16SemaHub_empty_ST_11i                        0x0388
    #define   LSb32SemaHub_empty_ST_11i                           11
    #define   LSb16SemaHub_empty_ST_11i                           11
    #define       bSemaHub_empty_ST_11i                        1
    #define   MSK32SemaHub_empty_ST_11i                           0x00000800

    #define     BA_SemaHub_empty_ST_12i                        0x0389
    #define     B16SemaHub_empty_ST_12i                        0x0388
    #define   LSb32SemaHub_empty_ST_12i                           12
    #define   LSb16SemaHub_empty_ST_12i                           12
    #define       bSemaHub_empty_ST_12i                        1
    #define   MSK32SemaHub_empty_ST_12i                           0x00001000

    #define     BA_SemaHub_empty_ST_13i                        0x0389
    #define     B16SemaHub_empty_ST_13i                        0x0388
    #define   LSb32SemaHub_empty_ST_13i                           13
    #define   LSb16SemaHub_empty_ST_13i                           13
    #define       bSemaHub_empty_ST_13i                        1
    #define   MSK32SemaHub_empty_ST_13i                           0x00002000

    #define     BA_SemaHub_empty_ST_14i                        0x0389
    #define     B16SemaHub_empty_ST_14i                        0x0388
    #define   LSb32SemaHub_empty_ST_14i                           14
    #define   LSb16SemaHub_empty_ST_14i                           14
    #define       bSemaHub_empty_ST_14i                        1
    #define   MSK32SemaHub_empty_ST_14i                           0x00004000

    #define     BA_SemaHub_empty_ST_15i                        0x0389
    #define     B16SemaHub_empty_ST_15i                        0x0388
    #define   LSb32SemaHub_empty_ST_15i                           15
    #define   LSb16SemaHub_empty_ST_15i                           15
    #define       bSemaHub_empty_ST_15i                        1
    #define   MSK32SemaHub_empty_ST_15i                           0x00008000

    #define     BA_SemaHub_empty_ST_16i                        0x038A
    #define     B16SemaHub_empty_ST_16i                        0x038A
    #define   LSb32SemaHub_empty_ST_16i                           16
    #define   LSb16SemaHub_empty_ST_16i                           0
    #define       bSemaHub_empty_ST_16i                        1
    #define   MSK32SemaHub_empty_ST_16i                           0x00010000

    #define     BA_SemaHub_empty_ST_17i                        0x038A
    #define     B16SemaHub_empty_ST_17i                        0x038A
    #define   LSb32SemaHub_empty_ST_17i                           17
    #define   LSb16SemaHub_empty_ST_17i                           1
    #define       bSemaHub_empty_ST_17i                        1
    #define   MSK32SemaHub_empty_ST_17i                           0x00020000

    #define     BA_SemaHub_empty_ST_18i                        0x038A
    #define     B16SemaHub_empty_ST_18i                        0x038A
    #define   LSb32SemaHub_empty_ST_18i                           18
    #define   LSb16SemaHub_empty_ST_18i                           2
    #define       bSemaHub_empty_ST_18i                        1
    #define   MSK32SemaHub_empty_ST_18i                           0x00040000

    #define     BA_SemaHub_empty_ST_19i                        0x038A
    #define     B16SemaHub_empty_ST_19i                        0x038A
    #define   LSb32SemaHub_empty_ST_19i                           19
    #define   LSb16SemaHub_empty_ST_19i                           3
    #define       bSemaHub_empty_ST_19i                        1
    #define   MSK32SemaHub_empty_ST_19i                           0x00080000

    #define     BA_SemaHub_empty_ST_20i                        0x038A
    #define     B16SemaHub_empty_ST_20i                        0x038A
    #define   LSb32SemaHub_empty_ST_20i                           20
    #define   LSb16SemaHub_empty_ST_20i                           4
    #define       bSemaHub_empty_ST_20i                        1
    #define   MSK32SemaHub_empty_ST_20i                           0x00100000

    #define     BA_SemaHub_empty_ST_21i                        0x038A
    #define     B16SemaHub_empty_ST_21i                        0x038A
    #define   LSb32SemaHub_empty_ST_21i                           21
    #define   LSb16SemaHub_empty_ST_21i                           5
    #define       bSemaHub_empty_ST_21i                        1
    #define   MSK32SemaHub_empty_ST_21i                           0x00200000

    #define     BA_SemaHub_empty_ST_22i                        0x038A
    #define     B16SemaHub_empty_ST_22i                        0x038A
    #define   LSb32SemaHub_empty_ST_22i                           22
    #define   LSb16SemaHub_empty_ST_22i                           6
    #define       bSemaHub_empty_ST_22i                        1
    #define   MSK32SemaHub_empty_ST_22i                           0x00400000

    #define     BA_SemaHub_empty_ST_23i                        0x038A
    #define     B16SemaHub_empty_ST_23i                        0x038A
    #define   LSb32SemaHub_empty_ST_23i                           23
    #define   LSb16SemaHub_empty_ST_23i                           7
    #define       bSemaHub_empty_ST_23i                        1
    #define   MSK32SemaHub_empty_ST_23i                           0x00800000

    #define     BA_SemaHub_empty_ST_24i                        0x038B
    #define     B16SemaHub_empty_ST_24i                        0x038A
    #define   LSb32SemaHub_empty_ST_24i                           24
    #define   LSb16SemaHub_empty_ST_24i                           8
    #define       bSemaHub_empty_ST_24i                        1
    #define   MSK32SemaHub_empty_ST_24i                           0x01000000

    #define     BA_SemaHub_empty_ST_25i                        0x038B
    #define     B16SemaHub_empty_ST_25i                        0x038A
    #define   LSb32SemaHub_empty_ST_25i                           25
    #define   LSb16SemaHub_empty_ST_25i                           9
    #define       bSemaHub_empty_ST_25i                        1
    #define   MSK32SemaHub_empty_ST_25i                           0x02000000

    #define     BA_SemaHub_empty_ST_26i                        0x038B
    #define     B16SemaHub_empty_ST_26i                        0x038A
    #define   LSb32SemaHub_empty_ST_26i                           26
    #define   LSb16SemaHub_empty_ST_26i                           10
    #define       bSemaHub_empty_ST_26i                        1
    #define   MSK32SemaHub_empty_ST_26i                           0x04000000

    #define     BA_SemaHub_empty_ST_27i                        0x038B
    #define     B16SemaHub_empty_ST_27i                        0x038A
    #define   LSb32SemaHub_empty_ST_27i                           27
    #define   LSb16SemaHub_empty_ST_27i                           11
    #define       bSemaHub_empty_ST_27i                        1
    #define   MSK32SemaHub_empty_ST_27i                           0x08000000

    #define     BA_SemaHub_empty_ST_28i                        0x038B
    #define     B16SemaHub_empty_ST_28i                        0x038A
    #define   LSb32SemaHub_empty_ST_28i                           28
    #define   LSb16SemaHub_empty_ST_28i                           12
    #define       bSemaHub_empty_ST_28i                        1
    #define   MSK32SemaHub_empty_ST_28i                           0x10000000

    #define     BA_SemaHub_empty_ST_29i                        0x038B
    #define     B16SemaHub_empty_ST_29i                        0x038A
    #define   LSb32SemaHub_empty_ST_29i                           29
    #define   LSb16SemaHub_empty_ST_29i                           13
    #define       bSemaHub_empty_ST_29i                        1
    #define   MSK32SemaHub_empty_ST_29i                           0x20000000

    #define     BA_SemaHub_empty_ST_30i                        0x038B
    #define     B16SemaHub_empty_ST_30i                        0x038A
    #define   LSb32SemaHub_empty_ST_30i                           30
    #define   LSb16SemaHub_empty_ST_30i                           14
    #define       bSemaHub_empty_ST_30i                        1
    #define   MSK32SemaHub_empty_ST_30i                           0x40000000

    #define     BA_SemaHub_empty_ST_31i                        0x038B
    #define     B16SemaHub_empty_ST_31i                        0x038A
    #define   LSb32SemaHub_empty_ST_31i                           31
    #define   LSb16SemaHub_empty_ST_31i                           15
    #define       bSemaHub_empty_ST_31i                        1
    #define   MSK32SemaHub_empty_ST_31i                           0x80000000
    ///////////////////////////////////////////////////////////
    #define     RA_SemaHub_full                                0x038C

    #define     BA_SemaHub_full_ST_0i                          0x038C
    #define     B16SemaHub_full_ST_0i                          0x038C
    #define   LSb32SemaHub_full_ST_0i                             0
    #define   LSb16SemaHub_full_ST_0i                             0
    #define       bSemaHub_full_ST_0i                          1
    #define   MSK32SemaHub_full_ST_0i                             0x00000001

    #define     BA_SemaHub_full_ST_1i                          0x038C
    #define     B16SemaHub_full_ST_1i                          0x038C
    #define   LSb32SemaHub_full_ST_1i                             1
    #define   LSb16SemaHub_full_ST_1i                             1
    #define       bSemaHub_full_ST_1i                          1
    #define   MSK32SemaHub_full_ST_1i                             0x00000002

    #define     BA_SemaHub_full_ST_2i                          0x038C
    #define     B16SemaHub_full_ST_2i                          0x038C
    #define   LSb32SemaHub_full_ST_2i                             2
    #define   LSb16SemaHub_full_ST_2i                             2
    #define       bSemaHub_full_ST_2i                          1
    #define   MSK32SemaHub_full_ST_2i                             0x00000004

    #define     BA_SemaHub_full_ST_3i                          0x038C
    #define     B16SemaHub_full_ST_3i                          0x038C
    #define   LSb32SemaHub_full_ST_3i                             3
    #define   LSb16SemaHub_full_ST_3i                             3
    #define       bSemaHub_full_ST_3i                          1
    #define   MSK32SemaHub_full_ST_3i                             0x00000008

    #define     BA_SemaHub_full_ST_4i                          0x038C
    #define     B16SemaHub_full_ST_4i                          0x038C
    #define   LSb32SemaHub_full_ST_4i                             4
    #define   LSb16SemaHub_full_ST_4i                             4
    #define       bSemaHub_full_ST_4i                          1
    #define   MSK32SemaHub_full_ST_4i                             0x00000010

    #define     BA_SemaHub_full_ST_5i                          0x038C
    #define     B16SemaHub_full_ST_5i                          0x038C
    #define   LSb32SemaHub_full_ST_5i                             5
    #define   LSb16SemaHub_full_ST_5i                             5
    #define       bSemaHub_full_ST_5i                          1
    #define   MSK32SemaHub_full_ST_5i                             0x00000020

    #define     BA_SemaHub_full_ST_6i                          0x038C
    #define     B16SemaHub_full_ST_6i                          0x038C
    #define   LSb32SemaHub_full_ST_6i                             6
    #define   LSb16SemaHub_full_ST_6i                             6
    #define       bSemaHub_full_ST_6i                          1
    #define   MSK32SemaHub_full_ST_6i                             0x00000040

    #define     BA_SemaHub_full_ST_7i                          0x038C
    #define     B16SemaHub_full_ST_7i                          0x038C
    #define   LSb32SemaHub_full_ST_7i                             7
    #define   LSb16SemaHub_full_ST_7i                             7
    #define       bSemaHub_full_ST_7i                          1
    #define   MSK32SemaHub_full_ST_7i                             0x00000080

    #define     BA_SemaHub_full_ST_8i                          0x038D
    #define     B16SemaHub_full_ST_8i                          0x038C
    #define   LSb32SemaHub_full_ST_8i                             8
    #define   LSb16SemaHub_full_ST_8i                             8
    #define       bSemaHub_full_ST_8i                          1
    #define   MSK32SemaHub_full_ST_8i                             0x00000100

    #define     BA_SemaHub_full_ST_9i                          0x038D
    #define     B16SemaHub_full_ST_9i                          0x038C
    #define   LSb32SemaHub_full_ST_9i                             9
    #define   LSb16SemaHub_full_ST_9i                             9
    #define       bSemaHub_full_ST_9i                          1
    #define   MSK32SemaHub_full_ST_9i                             0x00000200

    #define     BA_SemaHub_full_ST_10i                         0x038D
    #define     B16SemaHub_full_ST_10i                         0x038C
    #define   LSb32SemaHub_full_ST_10i                            10
    #define   LSb16SemaHub_full_ST_10i                            10
    #define       bSemaHub_full_ST_10i                         1
    #define   MSK32SemaHub_full_ST_10i                            0x00000400

    #define     BA_SemaHub_full_ST_11i                         0x038D
    #define     B16SemaHub_full_ST_11i                         0x038C
    #define   LSb32SemaHub_full_ST_11i                            11
    #define   LSb16SemaHub_full_ST_11i                            11
    #define       bSemaHub_full_ST_11i                         1
    #define   MSK32SemaHub_full_ST_11i                            0x00000800

    #define     BA_SemaHub_full_ST_12i                         0x038D
    #define     B16SemaHub_full_ST_12i                         0x038C
    #define   LSb32SemaHub_full_ST_12i                            12
    #define   LSb16SemaHub_full_ST_12i                            12
    #define       bSemaHub_full_ST_12i                         1
    #define   MSK32SemaHub_full_ST_12i                            0x00001000

    #define     BA_SemaHub_full_ST_13i                         0x038D
    #define     B16SemaHub_full_ST_13i                         0x038C
    #define   LSb32SemaHub_full_ST_13i                            13
    #define   LSb16SemaHub_full_ST_13i                            13
    #define       bSemaHub_full_ST_13i                         1
    #define   MSK32SemaHub_full_ST_13i                            0x00002000

    #define     BA_SemaHub_full_ST_14i                         0x038D
    #define     B16SemaHub_full_ST_14i                         0x038C
    #define   LSb32SemaHub_full_ST_14i                            14
    #define   LSb16SemaHub_full_ST_14i                            14
    #define       bSemaHub_full_ST_14i                         1
    #define   MSK32SemaHub_full_ST_14i                            0x00004000

    #define     BA_SemaHub_full_ST_15i                         0x038D
    #define     B16SemaHub_full_ST_15i                         0x038C
    #define   LSb32SemaHub_full_ST_15i                            15
    #define   LSb16SemaHub_full_ST_15i                            15
    #define       bSemaHub_full_ST_15i                         1
    #define   MSK32SemaHub_full_ST_15i                            0x00008000

    #define     BA_SemaHub_full_ST_16i                         0x038E
    #define     B16SemaHub_full_ST_16i                         0x038E
    #define   LSb32SemaHub_full_ST_16i                            16
    #define   LSb16SemaHub_full_ST_16i                            0
    #define       bSemaHub_full_ST_16i                         1
    #define   MSK32SemaHub_full_ST_16i                            0x00010000

    #define     BA_SemaHub_full_ST_17i                         0x038E
    #define     B16SemaHub_full_ST_17i                         0x038E
    #define   LSb32SemaHub_full_ST_17i                            17
    #define   LSb16SemaHub_full_ST_17i                            1
    #define       bSemaHub_full_ST_17i                         1
    #define   MSK32SemaHub_full_ST_17i                            0x00020000

    #define     BA_SemaHub_full_ST_18i                         0x038E
    #define     B16SemaHub_full_ST_18i                         0x038E
    #define   LSb32SemaHub_full_ST_18i                            18
    #define   LSb16SemaHub_full_ST_18i                            2
    #define       bSemaHub_full_ST_18i                         1
    #define   MSK32SemaHub_full_ST_18i                            0x00040000

    #define     BA_SemaHub_full_ST_19i                         0x038E
    #define     B16SemaHub_full_ST_19i                         0x038E
    #define   LSb32SemaHub_full_ST_19i                            19
    #define   LSb16SemaHub_full_ST_19i                            3
    #define       bSemaHub_full_ST_19i                         1
    #define   MSK32SemaHub_full_ST_19i                            0x00080000

    #define     BA_SemaHub_full_ST_20i                         0x038E
    #define     B16SemaHub_full_ST_20i                         0x038E
    #define   LSb32SemaHub_full_ST_20i                            20
    #define   LSb16SemaHub_full_ST_20i                            4
    #define       bSemaHub_full_ST_20i                         1
    #define   MSK32SemaHub_full_ST_20i                            0x00100000

    #define     BA_SemaHub_full_ST_21i                         0x038E
    #define     B16SemaHub_full_ST_21i                         0x038E
    #define   LSb32SemaHub_full_ST_21i                            21
    #define   LSb16SemaHub_full_ST_21i                            5
    #define       bSemaHub_full_ST_21i                         1
    #define   MSK32SemaHub_full_ST_21i                            0x00200000

    #define     BA_SemaHub_full_ST_22i                         0x038E
    #define     B16SemaHub_full_ST_22i                         0x038E
    #define   LSb32SemaHub_full_ST_22i                            22
    #define   LSb16SemaHub_full_ST_22i                            6
    #define       bSemaHub_full_ST_22i                         1
    #define   MSK32SemaHub_full_ST_22i                            0x00400000

    #define     BA_SemaHub_full_ST_23i                         0x038E
    #define     B16SemaHub_full_ST_23i                         0x038E
    #define   LSb32SemaHub_full_ST_23i                            23
    #define   LSb16SemaHub_full_ST_23i                            7
    #define       bSemaHub_full_ST_23i                         1
    #define   MSK32SemaHub_full_ST_23i                            0x00800000

    #define     BA_SemaHub_full_ST_24i                         0x038F
    #define     B16SemaHub_full_ST_24i                         0x038E
    #define   LSb32SemaHub_full_ST_24i                            24
    #define   LSb16SemaHub_full_ST_24i                            8
    #define       bSemaHub_full_ST_24i                         1
    #define   MSK32SemaHub_full_ST_24i                            0x01000000

    #define     BA_SemaHub_full_ST_25i                         0x038F
    #define     B16SemaHub_full_ST_25i                         0x038E
    #define   LSb32SemaHub_full_ST_25i                            25
    #define   LSb16SemaHub_full_ST_25i                            9
    #define       bSemaHub_full_ST_25i                         1
    #define   MSK32SemaHub_full_ST_25i                            0x02000000

    #define     BA_SemaHub_full_ST_26i                         0x038F
    #define     B16SemaHub_full_ST_26i                         0x038E
    #define   LSb32SemaHub_full_ST_26i                            26
    #define   LSb16SemaHub_full_ST_26i                            10
    #define       bSemaHub_full_ST_26i                         1
    #define   MSK32SemaHub_full_ST_26i                            0x04000000

    #define     BA_SemaHub_full_ST_27i                         0x038F
    #define     B16SemaHub_full_ST_27i                         0x038E
    #define   LSb32SemaHub_full_ST_27i                            27
    #define   LSb16SemaHub_full_ST_27i                            11
    #define       bSemaHub_full_ST_27i                         1
    #define   MSK32SemaHub_full_ST_27i                            0x08000000

    #define     BA_SemaHub_full_ST_28i                         0x038F
    #define     B16SemaHub_full_ST_28i                         0x038E
    #define   LSb32SemaHub_full_ST_28i                            28
    #define   LSb16SemaHub_full_ST_28i                            12
    #define       bSemaHub_full_ST_28i                         1
    #define   MSK32SemaHub_full_ST_28i                            0x10000000

    #define     BA_SemaHub_full_ST_29i                         0x038F
    #define     B16SemaHub_full_ST_29i                         0x038E
    #define   LSb32SemaHub_full_ST_29i                            29
    #define   LSb16SemaHub_full_ST_29i                            13
    #define       bSemaHub_full_ST_29i                         1
    #define   MSK32SemaHub_full_ST_29i                            0x20000000

    #define     BA_SemaHub_full_ST_30i                         0x038F
    #define     B16SemaHub_full_ST_30i                         0x038E
    #define   LSb32SemaHub_full_ST_30i                            30
    #define   LSb16SemaHub_full_ST_30i                            14
    #define       bSemaHub_full_ST_30i                         1
    #define   MSK32SemaHub_full_ST_30i                            0x40000000

    #define     BA_SemaHub_full_ST_31i                         0x038F
    #define     B16SemaHub_full_ST_31i                         0x038E
    #define   LSb32SemaHub_full_ST_31i                            31
    #define   LSb16SemaHub_full_ST_31i                            15
    #define       bSemaHub_full_ST_31i                         1
    #define   MSK32SemaHub_full_ST_31i                            0x80000000
    ///////////////////////////////////////////////////////////
    #define     RA_SemaHub_almostEmpty                         0x0390

    #define     BA_SemaHub_almostEmpty_ST_0i                   0x0390
    #define     B16SemaHub_almostEmpty_ST_0i                   0x0390
    #define   LSb32SemaHub_almostEmpty_ST_0i                      0
    #define   LSb16SemaHub_almostEmpty_ST_0i                      0
    #define       bSemaHub_almostEmpty_ST_0i                   1
    #define   MSK32SemaHub_almostEmpty_ST_0i                      0x00000001

    #define     BA_SemaHub_almostEmpty_ST_1i                   0x0390
    #define     B16SemaHub_almostEmpty_ST_1i                   0x0390
    #define   LSb32SemaHub_almostEmpty_ST_1i                      1
    #define   LSb16SemaHub_almostEmpty_ST_1i                      1
    #define       bSemaHub_almostEmpty_ST_1i                   1
    #define   MSK32SemaHub_almostEmpty_ST_1i                      0x00000002

    #define     BA_SemaHub_almostEmpty_ST_2i                   0x0390
    #define     B16SemaHub_almostEmpty_ST_2i                   0x0390
    #define   LSb32SemaHub_almostEmpty_ST_2i                      2
    #define   LSb16SemaHub_almostEmpty_ST_2i                      2
    #define       bSemaHub_almostEmpty_ST_2i                   1
    #define   MSK32SemaHub_almostEmpty_ST_2i                      0x00000004

    #define     BA_SemaHub_almostEmpty_ST_3i                   0x0390
    #define     B16SemaHub_almostEmpty_ST_3i                   0x0390
    #define   LSb32SemaHub_almostEmpty_ST_3i                      3
    #define   LSb16SemaHub_almostEmpty_ST_3i                      3
    #define       bSemaHub_almostEmpty_ST_3i                   1
    #define   MSK32SemaHub_almostEmpty_ST_3i                      0x00000008

    #define     BA_SemaHub_almostEmpty_ST_4i                   0x0390
    #define     B16SemaHub_almostEmpty_ST_4i                   0x0390
    #define   LSb32SemaHub_almostEmpty_ST_4i                      4
    #define   LSb16SemaHub_almostEmpty_ST_4i                      4
    #define       bSemaHub_almostEmpty_ST_4i                   1
    #define   MSK32SemaHub_almostEmpty_ST_4i                      0x00000010

    #define     BA_SemaHub_almostEmpty_ST_5i                   0x0390
    #define     B16SemaHub_almostEmpty_ST_5i                   0x0390
    #define   LSb32SemaHub_almostEmpty_ST_5i                      5
    #define   LSb16SemaHub_almostEmpty_ST_5i                      5
    #define       bSemaHub_almostEmpty_ST_5i                   1
    #define   MSK32SemaHub_almostEmpty_ST_5i                      0x00000020

    #define     BA_SemaHub_almostEmpty_ST_6i                   0x0390
    #define     B16SemaHub_almostEmpty_ST_6i                   0x0390
    #define   LSb32SemaHub_almostEmpty_ST_6i                      6
    #define   LSb16SemaHub_almostEmpty_ST_6i                      6
    #define       bSemaHub_almostEmpty_ST_6i                   1
    #define   MSK32SemaHub_almostEmpty_ST_6i                      0x00000040

    #define     BA_SemaHub_almostEmpty_ST_7i                   0x0390
    #define     B16SemaHub_almostEmpty_ST_7i                   0x0390
    #define   LSb32SemaHub_almostEmpty_ST_7i                      7
    #define   LSb16SemaHub_almostEmpty_ST_7i                      7
    #define       bSemaHub_almostEmpty_ST_7i                   1
    #define   MSK32SemaHub_almostEmpty_ST_7i                      0x00000080

    #define     BA_SemaHub_almostEmpty_ST_8i                   0x0391
    #define     B16SemaHub_almostEmpty_ST_8i                   0x0390
    #define   LSb32SemaHub_almostEmpty_ST_8i                      8
    #define   LSb16SemaHub_almostEmpty_ST_8i                      8
    #define       bSemaHub_almostEmpty_ST_8i                   1
    #define   MSK32SemaHub_almostEmpty_ST_8i                      0x00000100

    #define     BA_SemaHub_almostEmpty_ST_9i                   0x0391
    #define     B16SemaHub_almostEmpty_ST_9i                   0x0390
    #define   LSb32SemaHub_almostEmpty_ST_9i                      9
    #define   LSb16SemaHub_almostEmpty_ST_9i                      9
    #define       bSemaHub_almostEmpty_ST_9i                   1
    #define   MSK32SemaHub_almostEmpty_ST_9i                      0x00000200

    #define     BA_SemaHub_almostEmpty_ST_10i                  0x0391
    #define     B16SemaHub_almostEmpty_ST_10i                  0x0390
    #define   LSb32SemaHub_almostEmpty_ST_10i                     10
    #define   LSb16SemaHub_almostEmpty_ST_10i                     10
    #define       bSemaHub_almostEmpty_ST_10i                  1
    #define   MSK32SemaHub_almostEmpty_ST_10i                     0x00000400

    #define     BA_SemaHub_almostEmpty_ST_11i                  0x0391
    #define     B16SemaHub_almostEmpty_ST_11i                  0x0390
    #define   LSb32SemaHub_almostEmpty_ST_11i                     11
    #define   LSb16SemaHub_almostEmpty_ST_11i                     11
    #define       bSemaHub_almostEmpty_ST_11i                  1
    #define   MSK32SemaHub_almostEmpty_ST_11i                     0x00000800

    #define     BA_SemaHub_almostEmpty_ST_12i                  0x0391
    #define     B16SemaHub_almostEmpty_ST_12i                  0x0390
    #define   LSb32SemaHub_almostEmpty_ST_12i                     12
    #define   LSb16SemaHub_almostEmpty_ST_12i                     12
    #define       bSemaHub_almostEmpty_ST_12i                  1
    #define   MSK32SemaHub_almostEmpty_ST_12i                     0x00001000

    #define     BA_SemaHub_almostEmpty_ST_13i                  0x0391
    #define     B16SemaHub_almostEmpty_ST_13i                  0x0390
    #define   LSb32SemaHub_almostEmpty_ST_13i                     13
    #define   LSb16SemaHub_almostEmpty_ST_13i                     13
    #define       bSemaHub_almostEmpty_ST_13i                  1
    #define   MSK32SemaHub_almostEmpty_ST_13i                     0x00002000

    #define     BA_SemaHub_almostEmpty_ST_14i                  0x0391
    #define     B16SemaHub_almostEmpty_ST_14i                  0x0390
    #define   LSb32SemaHub_almostEmpty_ST_14i                     14
    #define   LSb16SemaHub_almostEmpty_ST_14i                     14
    #define       bSemaHub_almostEmpty_ST_14i                  1
    #define   MSK32SemaHub_almostEmpty_ST_14i                     0x00004000

    #define     BA_SemaHub_almostEmpty_ST_15i                  0x0391
    #define     B16SemaHub_almostEmpty_ST_15i                  0x0390
    #define   LSb32SemaHub_almostEmpty_ST_15i                     15
    #define   LSb16SemaHub_almostEmpty_ST_15i                     15
    #define       bSemaHub_almostEmpty_ST_15i                  1
    #define   MSK32SemaHub_almostEmpty_ST_15i                     0x00008000

    #define     BA_SemaHub_almostEmpty_ST_16i                  0x0392
    #define     B16SemaHub_almostEmpty_ST_16i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_16i                     16
    #define   LSb16SemaHub_almostEmpty_ST_16i                     0
    #define       bSemaHub_almostEmpty_ST_16i                  1
    #define   MSK32SemaHub_almostEmpty_ST_16i                     0x00010000

    #define     BA_SemaHub_almostEmpty_ST_17i                  0x0392
    #define     B16SemaHub_almostEmpty_ST_17i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_17i                     17
    #define   LSb16SemaHub_almostEmpty_ST_17i                     1
    #define       bSemaHub_almostEmpty_ST_17i                  1
    #define   MSK32SemaHub_almostEmpty_ST_17i                     0x00020000

    #define     BA_SemaHub_almostEmpty_ST_18i                  0x0392
    #define     B16SemaHub_almostEmpty_ST_18i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_18i                     18
    #define   LSb16SemaHub_almostEmpty_ST_18i                     2
    #define       bSemaHub_almostEmpty_ST_18i                  1
    #define   MSK32SemaHub_almostEmpty_ST_18i                     0x00040000

    #define     BA_SemaHub_almostEmpty_ST_19i                  0x0392
    #define     B16SemaHub_almostEmpty_ST_19i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_19i                     19
    #define   LSb16SemaHub_almostEmpty_ST_19i                     3
    #define       bSemaHub_almostEmpty_ST_19i                  1
    #define   MSK32SemaHub_almostEmpty_ST_19i                     0x00080000

    #define     BA_SemaHub_almostEmpty_ST_20i                  0x0392
    #define     B16SemaHub_almostEmpty_ST_20i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_20i                     20
    #define   LSb16SemaHub_almostEmpty_ST_20i                     4
    #define       bSemaHub_almostEmpty_ST_20i                  1
    #define   MSK32SemaHub_almostEmpty_ST_20i                     0x00100000

    #define     BA_SemaHub_almostEmpty_ST_21i                  0x0392
    #define     B16SemaHub_almostEmpty_ST_21i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_21i                     21
    #define   LSb16SemaHub_almostEmpty_ST_21i                     5
    #define       bSemaHub_almostEmpty_ST_21i                  1
    #define   MSK32SemaHub_almostEmpty_ST_21i                     0x00200000

    #define     BA_SemaHub_almostEmpty_ST_22i                  0x0392
    #define     B16SemaHub_almostEmpty_ST_22i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_22i                     22
    #define   LSb16SemaHub_almostEmpty_ST_22i                     6
    #define       bSemaHub_almostEmpty_ST_22i                  1
    #define   MSK32SemaHub_almostEmpty_ST_22i                     0x00400000

    #define     BA_SemaHub_almostEmpty_ST_23i                  0x0392
    #define     B16SemaHub_almostEmpty_ST_23i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_23i                     23
    #define   LSb16SemaHub_almostEmpty_ST_23i                     7
    #define       bSemaHub_almostEmpty_ST_23i                  1
    #define   MSK32SemaHub_almostEmpty_ST_23i                     0x00800000

    #define     BA_SemaHub_almostEmpty_ST_24i                  0x0393
    #define     B16SemaHub_almostEmpty_ST_24i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_24i                     24
    #define   LSb16SemaHub_almostEmpty_ST_24i                     8
    #define       bSemaHub_almostEmpty_ST_24i                  1
    #define   MSK32SemaHub_almostEmpty_ST_24i                     0x01000000

    #define     BA_SemaHub_almostEmpty_ST_25i                  0x0393
    #define     B16SemaHub_almostEmpty_ST_25i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_25i                     25
    #define   LSb16SemaHub_almostEmpty_ST_25i                     9
    #define       bSemaHub_almostEmpty_ST_25i                  1
    #define   MSK32SemaHub_almostEmpty_ST_25i                     0x02000000

    #define     BA_SemaHub_almostEmpty_ST_26i                  0x0393
    #define     B16SemaHub_almostEmpty_ST_26i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_26i                     26
    #define   LSb16SemaHub_almostEmpty_ST_26i                     10
    #define       bSemaHub_almostEmpty_ST_26i                  1
    #define   MSK32SemaHub_almostEmpty_ST_26i                     0x04000000

    #define     BA_SemaHub_almostEmpty_ST_27i                  0x0393
    #define     B16SemaHub_almostEmpty_ST_27i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_27i                     27
    #define   LSb16SemaHub_almostEmpty_ST_27i                     11
    #define       bSemaHub_almostEmpty_ST_27i                  1
    #define   MSK32SemaHub_almostEmpty_ST_27i                     0x08000000

    #define     BA_SemaHub_almostEmpty_ST_28i                  0x0393
    #define     B16SemaHub_almostEmpty_ST_28i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_28i                     28
    #define   LSb16SemaHub_almostEmpty_ST_28i                     12
    #define       bSemaHub_almostEmpty_ST_28i                  1
    #define   MSK32SemaHub_almostEmpty_ST_28i                     0x10000000

    #define     BA_SemaHub_almostEmpty_ST_29i                  0x0393
    #define     B16SemaHub_almostEmpty_ST_29i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_29i                     29
    #define   LSb16SemaHub_almostEmpty_ST_29i                     13
    #define       bSemaHub_almostEmpty_ST_29i                  1
    #define   MSK32SemaHub_almostEmpty_ST_29i                     0x20000000

    #define     BA_SemaHub_almostEmpty_ST_30i                  0x0393
    #define     B16SemaHub_almostEmpty_ST_30i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_30i                     30
    #define   LSb16SemaHub_almostEmpty_ST_30i                     14
    #define       bSemaHub_almostEmpty_ST_30i                  1
    #define   MSK32SemaHub_almostEmpty_ST_30i                     0x40000000

    #define     BA_SemaHub_almostEmpty_ST_31i                  0x0393
    #define     B16SemaHub_almostEmpty_ST_31i                  0x0392
    #define   LSb32SemaHub_almostEmpty_ST_31i                     31
    #define   LSb16SemaHub_almostEmpty_ST_31i                     15
    #define       bSemaHub_almostEmpty_ST_31i                  1
    #define   MSK32SemaHub_almostEmpty_ST_31i                     0x80000000
    ///////////////////////////////////////////////////////////
    #define     RA_SemaHub_almostFull                          0x0394

    #define     BA_SemaHub_almostFull_ST_0i                    0x0394
    #define     B16SemaHub_almostFull_ST_0i                    0x0394
    #define   LSb32SemaHub_almostFull_ST_0i                       0
    #define   LSb16SemaHub_almostFull_ST_0i                       0
    #define       bSemaHub_almostFull_ST_0i                    1
    #define   MSK32SemaHub_almostFull_ST_0i                       0x00000001

    #define     BA_SemaHub_almostFull_ST_1i                    0x0394
    #define     B16SemaHub_almostFull_ST_1i                    0x0394
    #define   LSb32SemaHub_almostFull_ST_1i                       1
    #define   LSb16SemaHub_almostFull_ST_1i                       1
    #define       bSemaHub_almostFull_ST_1i                    1
    #define   MSK32SemaHub_almostFull_ST_1i                       0x00000002

    #define     BA_SemaHub_almostFull_ST_2i                    0x0394
    #define     B16SemaHub_almostFull_ST_2i                    0x0394
    #define   LSb32SemaHub_almostFull_ST_2i                       2
    #define   LSb16SemaHub_almostFull_ST_2i                       2
    #define       bSemaHub_almostFull_ST_2i                    1
    #define   MSK32SemaHub_almostFull_ST_2i                       0x00000004

    #define     BA_SemaHub_almostFull_ST_3i                    0x0394
    #define     B16SemaHub_almostFull_ST_3i                    0x0394
    #define   LSb32SemaHub_almostFull_ST_3i                       3
    #define   LSb16SemaHub_almostFull_ST_3i                       3
    #define       bSemaHub_almostFull_ST_3i                    1
    #define   MSK32SemaHub_almostFull_ST_3i                       0x00000008

    #define     BA_SemaHub_almostFull_ST_4i                    0x0394
    #define     B16SemaHub_almostFull_ST_4i                    0x0394
    #define   LSb32SemaHub_almostFull_ST_4i                       4
    #define   LSb16SemaHub_almostFull_ST_4i                       4
    #define       bSemaHub_almostFull_ST_4i                    1
    #define   MSK32SemaHub_almostFull_ST_4i                       0x00000010

    #define     BA_SemaHub_almostFull_ST_5i                    0x0394
    #define     B16SemaHub_almostFull_ST_5i                    0x0394
    #define   LSb32SemaHub_almostFull_ST_5i                       5
    #define   LSb16SemaHub_almostFull_ST_5i                       5
    #define       bSemaHub_almostFull_ST_5i                    1
    #define   MSK32SemaHub_almostFull_ST_5i                       0x00000020

    #define     BA_SemaHub_almostFull_ST_6i                    0x0394
    #define     B16SemaHub_almostFull_ST_6i                    0x0394
    #define   LSb32SemaHub_almostFull_ST_6i                       6
    #define   LSb16SemaHub_almostFull_ST_6i                       6
    #define       bSemaHub_almostFull_ST_6i                    1
    #define   MSK32SemaHub_almostFull_ST_6i                       0x00000040

    #define     BA_SemaHub_almostFull_ST_7i                    0x0394
    #define     B16SemaHub_almostFull_ST_7i                    0x0394
    #define   LSb32SemaHub_almostFull_ST_7i                       7
    #define   LSb16SemaHub_almostFull_ST_7i                       7
    #define       bSemaHub_almostFull_ST_7i                    1
    #define   MSK32SemaHub_almostFull_ST_7i                       0x00000080

    #define     BA_SemaHub_almostFull_ST_8i                    0x0395
    #define     B16SemaHub_almostFull_ST_8i                    0x0394
    #define   LSb32SemaHub_almostFull_ST_8i                       8
    #define   LSb16SemaHub_almostFull_ST_8i                       8
    #define       bSemaHub_almostFull_ST_8i                    1
    #define   MSK32SemaHub_almostFull_ST_8i                       0x00000100

    #define     BA_SemaHub_almostFull_ST_9i                    0x0395
    #define     B16SemaHub_almostFull_ST_9i                    0x0394
    #define   LSb32SemaHub_almostFull_ST_9i                       9
    #define   LSb16SemaHub_almostFull_ST_9i                       9
    #define       bSemaHub_almostFull_ST_9i                    1
    #define   MSK32SemaHub_almostFull_ST_9i                       0x00000200

    #define     BA_SemaHub_almostFull_ST_10i                   0x0395
    #define     B16SemaHub_almostFull_ST_10i                   0x0394
    #define   LSb32SemaHub_almostFull_ST_10i                      10
    #define   LSb16SemaHub_almostFull_ST_10i                      10
    #define       bSemaHub_almostFull_ST_10i                   1
    #define   MSK32SemaHub_almostFull_ST_10i                      0x00000400

    #define     BA_SemaHub_almostFull_ST_11i                   0x0395
    #define     B16SemaHub_almostFull_ST_11i                   0x0394
    #define   LSb32SemaHub_almostFull_ST_11i                      11
    #define   LSb16SemaHub_almostFull_ST_11i                      11
    #define       bSemaHub_almostFull_ST_11i                   1
    #define   MSK32SemaHub_almostFull_ST_11i                      0x00000800

    #define     BA_SemaHub_almostFull_ST_12i                   0x0395
    #define     B16SemaHub_almostFull_ST_12i                   0x0394
    #define   LSb32SemaHub_almostFull_ST_12i                      12
    #define   LSb16SemaHub_almostFull_ST_12i                      12
    #define       bSemaHub_almostFull_ST_12i                   1
    #define   MSK32SemaHub_almostFull_ST_12i                      0x00001000

    #define     BA_SemaHub_almostFull_ST_13i                   0x0395
    #define     B16SemaHub_almostFull_ST_13i                   0x0394
    #define   LSb32SemaHub_almostFull_ST_13i                      13
    #define   LSb16SemaHub_almostFull_ST_13i                      13
    #define       bSemaHub_almostFull_ST_13i                   1
    #define   MSK32SemaHub_almostFull_ST_13i                      0x00002000

    #define     BA_SemaHub_almostFull_ST_14i                   0x0395
    #define     B16SemaHub_almostFull_ST_14i                   0x0394
    #define   LSb32SemaHub_almostFull_ST_14i                      14
    #define   LSb16SemaHub_almostFull_ST_14i                      14
    #define       bSemaHub_almostFull_ST_14i                   1
    #define   MSK32SemaHub_almostFull_ST_14i                      0x00004000

    #define     BA_SemaHub_almostFull_ST_15i                   0x0395
    #define     B16SemaHub_almostFull_ST_15i                   0x0394
    #define   LSb32SemaHub_almostFull_ST_15i                      15
    #define   LSb16SemaHub_almostFull_ST_15i                      15
    #define       bSemaHub_almostFull_ST_15i                   1
    #define   MSK32SemaHub_almostFull_ST_15i                      0x00008000

    #define     BA_SemaHub_almostFull_ST_16i                   0x0396
    #define     B16SemaHub_almostFull_ST_16i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_16i                      16
    #define   LSb16SemaHub_almostFull_ST_16i                      0
    #define       bSemaHub_almostFull_ST_16i                   1
    #define   MSK32SemaHub_almostFull_ST_16i                      0x00010000

    #define     BA_SemaHub_almostFull_ST_17i                   0x0396
    #define     B16SemaHub_almostFull_ST_17i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_17i                      17
    #define   LSb16SemaHub_almostFull_ST_17i                      1
    #define       bSemaHub_almostFull_ST_17i                   1
    #define   MSK32SemaHub_almostFull_ST_17i                      0x00020000

    #define     BA_SemaHub_almostFull_ST_18i                   0x0396
    #define     B16SemaHub_almostFull_ST_18i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_18i                      18
    #define   LSb16SemaHub_almostFull_ST_18i                      2
    #define       bSemaHub_almostFull_ST_18i                   1
    #define   MSK32SemaHub_almostFull_ST_18i                      0x00040000

    #define     BA_SemaHub_almostFull_ST_19i                   0x0396
    #define     B16SemaHub_almostFull_ST_19i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_19i                      19
    #define   LSb16SemaHub_almostFull_ST_19i                      3
    #define       bSemaHub_almostFull_ST_19i                   1
    #define   MSK32SemaHub_almostFull_ST_19i                      0x00080000

    #define     BA_SemaHub_almostFull_ST_20i                   0x0396
    #define     B16SemaHub_almostFull_ST_20i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_20i                      20
    #define   LSb16SemaHub_almostFull_ST_20i                      4
    #define       bSemaHub_almostFull_ST_20i                   1
    #define   MSK32SemaHub_almostFull_ST_20i                      0x00100000

    #define     BA_SemaHub_almostFull_ST_21i                   0x0396
    #define     B16SemaHub_almostFull_ST_21i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_21i                      21
    #define   LSb16SemaHub_almostFull_ST_21i                      5
    #define       bSemaHub_almostFull_ST_21i                   1
    #define   MSK32SemaHub_almostFull_ST_21i                      0x00200000

    #define     BA_SemaHub_almostFull_ST_22i                   0x0396
    #define     B16SemaHub_almostFull_ST_22i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_22i                      22
    #define   LSb16SemaHub_almostFull_ST_22i                      6
    #define       bSemaHub_almostFull_ST_22i                   1
    #define   MSK32SemaHub_almostFull_ST_22i                      0x00400000

    #define     BA_SemaHub_almostFull_ST_23i                   0x0396
    #define     B16SemaHub_almostFull_ST_23i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_23i                      23
    #define   LSb16SemaHub_almostFull_ST_23i                      7
    #define       bSemaHub_almostFull_ST_23i                   1
    #define   MSK32SemaHub_almostFull_ST_23i                      0x00800000

    #define     BA_SemaHub_almostFull_ST_24i                   0x0397
    #define     B16SemaHub_almostFull_ST_24i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_24i                      24
    #define   LSb16SemaHub_almostFull_ST_24i                      8
    #define       bSemaHub_almostFull_ST_24i                   1
    #define   MSK32SemaHub_almostFull_ST_24i                      0x01000000

    #define     BA_SemaHub_almostFull_ST_25i                   0x0397
    #define     B16SemaHub_almostFull_ST_25i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_25i                      25
    #define   LSb16SemaHub_almostFull_ST_25i                      9
    #define       bSemaHub_almostFull_ST_25i                   1
    #define   MSK32SemaHub_almostFull_ST_25i                      0x02000000

    #define     BA_SemaHub_almostFull_ST_26i                   0x0397
    #define     B16SemaHub_almostFull_ST_26i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_26i                      26
    #define   LSb16SemaHub_almostFull_ST_26i                      10
    #define       bSemaHub_almostFull_ST_26i                   1
    #define   MSK32SemaHub_almostFull_ST_26i                      0x04000000

    #define     BA_SemaHub_almostFull_ST_27i                   0x0397
    #define     B16SemaHub_almostFull_ST_27i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_27i                      27
    #define   LSb16SemaHub_almostFull_ST_27i                      11
    #define       bSemaHub_almostFull_ST_27i                   1
    #define   MSK32SemaHub_almostFull_ST_27i                      0x08000000

    #define     BA_SemaHub_almostFull_ST_28i                   0x0397
    #define     B16SemaHub_almostFull_ST_28i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_28i                      28
    #define   LSb16SemaHub_almostFull_ST_28i                      12
    #define       bSemaHub_almostFull_ST_28i                   1
    #define   MSK32SemaHub_almostFull_ST_28i                      0x10000000

    #define     BA_SemaHub_almostFull_ST_29i                   0x0397
    #define     B16SemaHub_almostFull_ST_29i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_29i                      29
    #define   LSb16SemaHub_almostFull_ST_29i                      13
    #define       bSemaHub_almostFull_ST_29i                   1
    #define   MSK32SemaHub_almostFull_ST_29i                      0x20000000

    #define     BA_SemaHub_almostFull_ST_30i                   0x0397
    #define     B16SemaHub_almostFull_ST_30i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_30i                      30
    #define   LSb16SemaHub_almostFull_ST_30i                      14
    #define       bSemaHub_almostFull_ST_30i                   1
    #define   MSK32SemaHub_almostFull_ST_30i                      0x40000000

    #define     BA_SemaHub_almostFull_ST_31i                   0x0397
    #define     B16SemaHub_almostFull_ST_31i                   0x0396
    #define   LSb32SemaHub_almostFull_ST_31i                      31
    #define   LSb16SemaHub_almostFull_ST_31i                      15
    #define       bSemaHub_almostFull_ST_31i                   1
    #define   MSK32SemaHub_almostFull_ST_31i                      0x80000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_SemaHub {
    ///////////////////////////////////////////////////////////
              SIE_SemaQuery                                    ie_counter[64];
    ///////////////////////////////////////////////////////////
              SIE_Semaphore                                    ie_cell[32];
    ///////////////////////////////////////////////////////////
    #define   GET32SemaHub_PUSH_ID(r32)                        _BFGET_(r32, 7, 0)
    #define   SET32SemaHub_PUSH_ID(r32,v)                      _BFSET_(r32, 7, 0,v)
    #define   GET16SemaHub_PUSH_ID(r16)                        _BFGET_(r16, 7, 0)
    #define   SET16SemaHub_PUSH_ID(r16,v)                      _BFSET_(r16, 7, 0,v)

    #define   GET32SemaHub_PUSH_delta(r32)                     _BFGET_(r32,15, 8)
    #define   SET32SemaHub_PUSH_delta(r32,v)                   _BFSET_(r32,15, 8,v)
    #define   GET16SemaHub_PUSH_delta(r16)                     _BFGET_(r16,15, 8)
    #define   SET16SemaHub_PUSH_delta(r16,v)                   _BFSET_(r16,15, 8,v)

    #define     w32SemaHub_PUSH                                {\
            UNSG32 uPUSH_ID                                    :  8;\
            UNSG32 uPUSH_delta                                 :  8;\
            UNSG32 RSVDx380_b16                                : 16;\
          }
    union { UNSG32 u32SemaHub_PUSH;
            struct w32SemaHub_PUSH;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32SemaHub_POP_ID(r32)                         _BFGET_(r32, 7, 0)
    #define   SET32SemaHub_POP_ID(r32,v)                       _BFSET_(r32, 7, 0,v)
    #define   GET16SemaHub_POP_ID(r16)                         _BFGET_(r16, 7, 0)
    #define   SET16SemaHub_POP_ID(r16,v)                       _BFSET_(r16, 7, 0,v)

    #define   GET32SemaHub_POP_delta(r32)                      _BFGET_(r32,15, 8)
    #define   SET32SemaHub_POP_delta(r32,v)                    _BFSET_(r32,15, 8,v)
    #define   GET16SemaHub_POP_delta(r16)                      _BFGET_(r16,15, 8)
    #define   SET16SemaHub_POP_delta(r16,v)                    _BFSET_(r16,15, 8,v)

    #define     w32SemaHub_POP                                 {\
            UNSG32 uPOP_ID                                     :  8;\
            UNSG32 uPOP_delta                                  :  8;\
            UNSG32 RSVDx384_b16                                : 16;\
          }
    union { UNSG32 u32SemaHub_POP;
            struct w32SemaHub_POP;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32SemaHub_empty_ST_0i(r32)                    _BFGET_(r32, 0, 0)
    #define   SET32SemaHub_empty_ST_0i(r32,v)                  _BFSET_(r32, 0, 0,v)
    #define   GET16SemaHub_empty_ST_0i(r16)                    _BFGET_(r16, 0, 0)
    #define   SET16SemaHub_empty_ST_0i(r16,v)                  _BFSET_(r16, 0, 0,v)

    #define   GET32SemaHub_empty_ST_1i(r32)                    _BFGET_(r32, 1, 1)
    #define   SET32SemaHub_empty_ST_1i(r32,v)                  _BFSET_(r32, 1, 1,v)
    #define   GET16SemaHub_empty_ST_1i(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16SemaHub_empty_ST_1i(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32SemaHub_empty_ST_2i(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32SemaHub_empty_ST_2i(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16SemaHub_empty_ST_2i(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16SemaHub_empty_ST_2i(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define   GET32SemaHub_empty_ST_3i(r32)                    _BFGET_(r32, 3, 3)
    #define   SET32SemaHub_empty_ST_3i(r32,v)                  _BFSET_(r32, 3, 3,v)
    #define   GET16SemaHub_empty_ST_3i(r16)                    _BFGET_(r16, 3, 3)
    #define   SET16SemaHub_empty_ST_3i(r16,v)                  _BFSET_(r16, 3, 3,v)

    #define   GET32SemaHub_empty_ST_4i(r32)                    _BFGET_(r32, 4, 4)
    #define   SET32SemaHub_empty_ST_4i(r32,v)                  _BFSET_(r32, 4, 4,v)
    #define   GET16SemaHub_empty_ST_4i(r16)                    _BFGET_(r16, 4, 4)
    #define   SET16SemaHub_empty_ST_4i(r16,v)                  _BFSET_(r16, 4, 4,v)

    #define   GET32SemaHub_empty_ST_5i(r32)                    _BFGET_(r32, 5, 5)
    #define   SET32SemaHub_empty_ST_5i(r32,v)                  _BFSET_(r32, 5, 5,v)
    #define   GET16SemaHub_empty_ST_5i(r16)                    _BFGET_(r16, 5, 5)
    #define   SET16SemaHub_empty_ST_5i(r16,v)                  _BFSET_(r16, 5, 5,v)

    #define   GET32SemaHub_empty_ST_6i(r32)                    _BFGET_(r32, 6, 6)
    #define   SET32SemaHub_empty_ST_6i(r32,v)                  _BFSET_(r32, 6, 6,v)
    #define   GET16SemaHub_empty_ST_6i(r16)                    _BFGET_(r16, 6, 6)
    #define   SET16SemaHub_empty_ST_6i(r16,v)                  _BFSET_(r16, 6, 6,v)

    #define   GET32SemaHub_empty_ST_7i(r32)                    _BFGET_(r32, 7, 7)
    #define   SET32SemaHub_empty_ST_7i(r32,v)                  _BFSET_(r32, 7, 7,v)
    #define   GET16SemaHub_empty_ST_7i(r16)                    _BFGET_(r16, 7, 7)
    #define   SET16SemaHub_empty_ST_7i(r16,v)                  _BFSET_(r16, 7, 7,v)

    #define   GET32SemaHub_empty_ST_8i(r32)                    _BFGET_(r32, 8, 8)
    #define   SET32SemaHub_empty_ST_8i(r32,v)                  _BFSET_(r32, 8, 8,v)
    #define   GET16SemaHub_empty_ST_8i(r16)                    _BFGET_(r16, 8, 8)
    #define   SET16SemaHub_empty_ST_8i(r16,v)                  _BFSET_(r16, 8, 8,v)

    #define   GET32SemaHub_empty_ST_9i(r32)                    _BFGET_(r32, 9, 9)
    #define   SET32SemaHub_empty_ST_9i(r32,v)                  _BFSET_(r32, 9, 9,v)
    #define   GET16SemaHub_empty_ST_9i(r16)                    _BFGET_(r16, 9, 9)
    #define   SET16SemaHub_empty_ST_9i(r16,v)                  _BFSET_(r16, 9, 9,v)

    #define   GET32SemaHub_empty_ST_10i(r32)                   _BFGET_(r32,10,10)
    #define   SET32SemaHub_empty_ST_10i(r32,v)                 _BFSET_(r32,10,10,v)
    #define   GET16SemaHub_empty_ST_10i(r16)                   _BFGET_(r16,10,10)
    #define   SET16SemaHub_empty_ST_10i(r16,v)                 _BFSET_(r16,10,10,v)

    #define   GET32SemaHub_empty_ST_11i(r32)                   _BFGET_(r32,11,11)
    #define   SET32SemaHub_empty_ST_11i(r32,v)                 _BFSET_(r32,11,11,v)
    #define   GET16SemaHub_empty_ST_11i(r16)                   _BFGET_(r16,11,11)
    #define   SET16SemaHub_empty_ST_11i(r16,v)                 _BFSET_(r16,11,11,v)

    #define   GET32SemaHub_empty_ST_12i(r32)                   _BFGET_(r32,12,12)
    #define   SET32SemaHub_empty_ST_12i(r32,v)                 _BFSET_(r32,12,12,v)
    #define   GET16SemaHub_empty_ST_12i(r16)                   _BFGET_(r16,12,12)
    #define   SET16SemaHub_empty_ST_12i(r16,v)                 _BFSET_(r16,12,12,v)

    #define   GET32SemaHub_empty_ST_13i(r32)                   _BFGET_(r32,13,13)
    #define   SET32SemaHub_empty_ST_13i(r32,v)                 _BFSET_(r32,13,13,v)
    #define   GET16SemaHub_empty_ST_13i(r16)                   _BFGET_(r16,13,13)
    #define   SET16SemaHub_empty_ST_13i(r16,v)                 _BFSET_(r16,13,13,v)

    #define   GET32SemaHub_empty_ST_14i(r32)                   _BFGET_(r32,14,14)
    #define   SET32SemaHub_empty_ST_14i(r32,v)                 _BFSET_(r32,14,14,v)
    #define   GET16SemaHub_empty_ST_14i(r16)                   _BFGET_(r16,14,14)
    #define   SET16SemaHub_empty_ST_14i(r16,v)                 _BFSET_(r16,14,14,v)

    #define   GET32SemaHub_empty_ST_15i(r32)                   _BFGET_(r32,15,15)
    #define   SET32SemaHub_empty_ST_15i(r32,v)                 _BFSET_(r32,15,15,v)
    #define   GET16SemaHub_empty_ST_15i(r16)                   _BFGET_(r16,15,15)
    #define   SET16SemaHub_empty_ST_15i(r16,v)                 _BFSET_(r16,15,15,v)

    #define   GET32SemaHub_empty_ST_16i(r32)                   _BFGET_(r32,16,16)
    #define   SET32SemaHub_empty_ST_16i(r32,v)                 _BFSET_(r32,16,16,v)
    #define   GET16SemaHub_empty_ST_16i(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16SemaHub_empty_ST_16i(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define   GET32SemaHub_empty_ST_17i(r32)                   _BFGET_(r32,17,17)
    #define   SET32SemaHub_empty_ST_17i(r32,v)                 _BFSET_(r32,17,17,v)
    #define   GET16SemaHub_empty_ST_17i(r16)                   _BFGET_(r16, 1, 1)
    #define   SET16SemaHub_empty_ST_17i(r16,v)                 _BFSET_(r16, 1, 1,v)

    #define   GET32SemaHub_empty_ST_18i(r32)                   _BFGET_(r32,18,18)
    #define   SET32SemaHub_empty_ST_18i(r32,v)                 _BFSET_(r32,18,18,v)
    #define   GET16SemaHub_empty_ST_18i(r16)                   _BFGET_(r16, 2, 2)
    #define   SET16SemaHub_empty_ST_18i(r16,v)                 _BFSET_(r16, 2, 2,v)

    #define   GET32SemaHub_empty_ST_19i(r32)                   _BFGET_(r32,19,19)
    #define   SET32SemaHub_empty_ST_19i(r32,v)                 _BFSET_(r32,19,19,v)
    #define   GET16SemaHub_empty_ST_19i(r16)                   _BFGET_(r16, 3, 3)
    #define   SET16SemaHub_empty_ST_19i(r16,v)                 _BFSET_(r16, 3, 3,v)

    #define   GET32SemaHub_empty_ST_20i(r32)                   _BFGET_(r32,20,20)
    #define   SET32SemaHub_empty_ST_20i(r32,v)                 _BFSET_(r32,20,20,v)
    #define   GET16SemaHub_empty_ST_20i(r16)                   _BFGET_(r16, 4, 4)
    #define   SET16SemaHub_empty_ST_20i(r16,v)                 _BFSET_(r16, 4, 4,v)

    #define   GET32SemaHub_empty_ST_21i(r32)                   _BFGET_(r32,21,21)
    #define   SET32SemaHub_empty_ST_21i(r32,v)                 _BFSET_(r32,21,21,v)
    #define   GET16SemaHub_empty_ST_21i(r16)                   _BFGET_(r16, 5, 5)
    #define   SET16SemaHub_empty_ST_21i(r16,v)                 _BFSET_(r16, 5, 5,v)

    #define   GET32SemaHub_empty_ST_22i(r32)                   _BFGET_(r32,22,22)
    #define   SET32SemaHub_empty_ST_22i(r32,v)                 _BFSET_(r32,22,22,v)
    #define   GET16SemaHub_empty_ST_22i(r16)                   _BFGET_(r16, 6, 6)
    #define   SET16SemaHub_empty_ST_22i(r16,v)                 _BFSET_(r16, 6, 6,v)

    #define   GET32SemaHub_empty_ST_23i(r32)                   _BFGET_(r32,23,23)
    #define   SET32SemaHub_empty_ST_23i(r32,v)                 _BFSET_(r32,23,23,v)
    #define   GET16SemaHub_empty_ST_23i(r16)                   _BFGET_(r16, 7, 7)
    #define   SET16SemaHub_empty_ST_23i(r16,v)                 _BFSET_(r16, 7, 7,v)

    #define   GET32SemaHub_empty_ST_24i(r32)                   _BFGET_(r32,24,24)
    #define   SET32SemaHub_empty_ST_24i(r32,v)                 _BFSET_(r32,24,24,v)
    #define   GET16SemaHub_empty_ST_24i(r16)                   _BFGET_(r16, 8, 8)
    #define   SET16SemaHub_empty_ST_24i(r16,v)                 _BFSET_(r16, 8, 8,v)

    #define   GET32SemaHub_empty_ST_25i(r32)                   _BFGET_(r32,25,25)
    #define   SET32SemaHub_empty_ST_25i(r32,v)                 _BFSET_(r32,25,25,v)
    #define   GET16SemaHub_empty_ST_25i(r16)                   _BFGET_(r16, 9, 9)
    #define   SET16SemaHub_empty_ST_25i(r16,v)                 _BFSET_(r16, 9, 9,v)

    #define   GET32SemaHub_empty_ST_26i(r32)                   _BFGET_(r32,26,26)
    #define   SET32SemaHub_empty_ST_26i(r32,v)                 _BFSET_(r32,26,26,v)
    #define   GET16SemaHub_empty_ST_26i(r16)                   _BFGET_(r16,10,10)
    #define   SET16SemaHub_empty_ST_26i(r16,v)                 _BFSET_(r16,10,10,v)

    #define   GET32SemaHub_empty_ST_27i(r32)                   _BFGET_(r32,27,27)
    #define   SET32SemaHub_empty_ST_27i(r32,v)                 _BFSET_(r32,27,27,v)
    #define   GET16SemaHub_empty_ST_27i(r16)                   _BFGET_(r16,11,11)
    #define   SET16SemaHub_empty_ST_27i(r16,v)                 _BFSET_(r16,11,11,v)

    #define   GET32SemaHub_empty_ST_28i(r32)                   _BFGET_(r32,28,28)
    #define   SET32SemaHub_empty_ST_28i(r32,v)                 _BFSET_(r32,28,28,v)
    #define   GET16SemaHub_empty_ST_28i(r16)                   _BFGET_(r16,12,12)
    #define   SET16SemaHub_empty_ST_28i(r16,v)                 _BFSET_(r16,12,12,v)

    #define   GET32SemaHub_empty_ST_29i(r32)                   _BFGET_(r32,29,29)
    #define   SET32SemaHub_empty_ST_29i(r32,v)                 _BFSET_(r32,29,29,v)
    #define   GET16SemaHub_empty_ST_29i(r16)                   _BFGET_(r16,13,13)
    #define   SET16SemaHub_empty_ST_29i(r16,v)                 _BFSET_(r16,13,13,v)

    #define   GET32SemaHub_empty_ST_30i(r32)                   _BFGET_(r32,30,30)
    #define   SET32SemaHub_empty_ST_30i(r32,v)                 _BFSET_(r32,30,30,v)
    #define   GET16SemaHub_empty_ST_30i(r16)                   _BFGET_(r16,14,14)
    #define   SET16SemaHub_empty_ST_30i(r16,v)                 _BFSET_(r16,14,14,v)

    #define   GET32SemaHub_empty_ST_31i(r32)                   _BFGET_(r32,31,31)
    #define   SET32SemaHub_empty_ST_31i(r32,v)                 _BFSET_(r32,31,31,v)
    #define   GET16SemaHub_empty_ST_31i(r16)                   _BFGET_(r16,15,15)
    #define   SET16SemaHub_empty_ST_31i(r16,v)                 _BFSET_(r16,15,15,v)

    #define     w32SemaHub_empty                               {\
            UNSG32 uempty_ST_0i                                :  1;\
            UNSG32 uempty_ST_1i                                :  1;\
            UNSG32 uempty_ST_2i                                :  1;\
            UNSG32 uempty_ST_3i                                :  1;\
            UNSG32 uempty_ST_4i                                :  1;\
            UNSG32 uempty_ST_5i                                :  1;\
            UNSG32 uempty_ST_6i                                :  1;\
            UNSG32 uempty_ST_7i                                :  1;\
            UNSG32 uempty_ST_8i                                :  1;\
            UNSG32 uempty_ST_9i                                :  1;\
            UNSG32 uempty_ST_10i                               :  1;\
            UNSG32 uempty_ST_11i                               :  1;\
            UNSG32 uempty_ST_12i                               :  1;\
            UNSG32 uempty_ST_13i                               :  1;\
            UNSG32 uempty_ST_14i                               :  1;\
            UNSG32 uempty_ST_15i                               :  1;\
            UNSG32 uempty_ST_16i                               :  1;\
            UNSG32 uempty_ST_17i                               :  1;\
            UNSG32 uempty_ST_18i                               :  1;\
            UNSG32 uempty_ST_19i                               :  1;\
            UNSG32 uempty_ST_20i                               :  1;\
            UNSG32 uempty_ST_21i                               :  1;\
            UNSG32 uempty_ST_22i                               :  1;\
            UNSG32 uempty_ST_23i                               :  1;\
            UNSG32 uempty_ST_24i                               :  1;\
            UNSG32 uempty_ST_25i                               :  1;\
            UNSG32 uempty_ST_26i                               :  1;\
            UNSG32 uempty_ST_27i                               :  1;\
            UNSG32 uempty_ST_28i                               :  1;\
            UNSG32 uempty_ST_29i                               :  1;\
            UNSG32 uempty_ST_30i                               :  1;\
            UNSG32 uempty_ST_31i                               :  1;\
          }
    union { UNSG32 u32SemaHub_empty;
            struct w32SemaHub_empty;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32SemaHub_full_ST_0i(r32)                     _BFGET_(r32, 0, 0)
    #define   SET32SemaHub_full_ST_0i(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   GET16SemaHub_full_ST_0i(r16)                     _BFGET_(r16, 0, 0)
    #define   SET16SemaHub_full_ST_0i(r16,v)                   _BFSET_(r16, 0, 0,v)

    #define   GET32SemaHub_full_ST_1i(r32)                     _BFGET_(r32, 1, 1)
    #define   SET32SemaHub_full_ST_1i(r32,v)                   _BFSET_(r32, 1, 1,v)
    #define   GET16SemaHub_full_ST_1i(r16)                     _BFGET_(r16, 1, 1)
    #define   SET16SemaHub_full_ST_1i(r16,v)                   _BFSET_(r16, 1, 1,v)

    #define   GET32SemaHub_full_ST_2i(r32)                     _BFGET_(r32, 2, 2)
    #define   SET32SemaHub_full_ST_2i(r32,v)                   _BFSET_(r32, 2, 2,v)
    #define   GET16SemaHub_full_ST_2i(r16)                     _BFGET_(r16, 2, 2)
    #define   SET16SemaHub_full_ST_2i(r16,v)                   _BFSET_(r16, 2, 2,v)

    #define   GET32SemaHub_full_ST_3i(r32)                     _BFGET_(r32, 3, 3)
    #define   SET32SemaHub_full_ST_3i(r32,v)                   _BFSET_(r32, 3, 3,v)
    #define   GET16SemaHub_full_ST_3i(r16)                     _BFGET_(r16, 3, 3)
    #define   SET16SemaHub_full_ST_3i(r16,v)                   _BFSET_(r16, 3, 3,v)

    #define   GET32SemaHub_full_ST_4i(r32)                     _BFGET_(r32, 4, 4)
    #define   SET32SemaHub_full_ST_4i(r32,v)                   _BFSET_(r32, 4, 4,v)
    #define   GET16SemaHub_full_ST_4i(r16)                     _BFGET_(r16, 4, 4)
    #define   SET16SemaHub_full_ST_4i(r16,v)                   _BFSET_(r16, 4, 4,v)

    #define   GET32SemaHub_full_ST_5i(r32)                     _BFGET_(r32, 5, 5)
    #define   SET32SemaHub_full_ST_5i(r32,v)                   _BFSET_(r32, 5, 5,v)
    #define   GET16SemaHub_full_ST_5i(r16)                     _BFGET_(r16, 5, 5)
    #define   SET16SemaHub_full_ST_5i(r16,v)                   _BFSET_(r16, 5, 5,v)

    #define   GET32SemaHub_full_ST_6i(r32)                     _BFGET_(r32, 6, 6)
    #define   SET32SemaHub_full_ST_6i(r32,v)                   _BFSET_(r32, 6, 6,v)
    #define   GET16SemaHub_full_ST_6i(r16)                     _BFGET_(r16, 6, 6)
    #define   SET16SemaHub_full_ST_6i(r16,v)                   _BFSET_(r16, 6, 6,v)

    #define   GET32SemaHub_full_ST_7i(r32)                     _BFGET_(r32, 7, 7)
    #define   SET32SemaHub_full_ST_7i(r32,v)                   _BFSET_(r32, 7, 7,v)
    #define   GET16SemaHub_full_ST_7i(r16)                     _BFGET_(r16, 7, 7)
    #define   SET16SemaHub_full_ST_7i(r16,v)                   _BFSET_(r16, 7, 7,v)

    #define   GET32SemaHub_full_ST_8i(r32)                     _BFGET_(r32, 8, 8)
    #define   SET32SemaHub_full_ST_8i(r32,v)                   _BFSET_(r32, 8, 8,v)
    #define   GET16SemaHub_full_ST_8i(r16)                     _BFGET_(r16, 8, 8)
    #define   SET16SemaHub_full_ST_8i(r16,v)                   _BFSET_(r16, 8, 8,v)

    #define   GET32SemaHub_full_ST_9i(r32)                     _BFGET_(r32, 9, 9)
    #define   SET32SemaHub_full_ST_9i(r32,v)                   _BFSET_(r32, 9, 9,v)
    #define   GET16SemaHub_full_ST_9i(r16)                     _BFGET_(r16, 9, 9)
    #define   SET16SemaHub_full_ST_9i(r16,v)                   _BFSET_(r16, 9, 9,v)

    #define   GET32SemaHub_full_ST_10i(r32)                    _BFGET_(r32,10,10)
    #define   SET32SemaHub_full_ST_10i(r32,v)                  _BFSET_(r32,10,10,v)
    #define   GET16SemaHub_full_ST_10i(r16)                    _BFGET_(r16,10,10)
    #define   SET16SemaHub_full_ST_10i(r16,v)                  _BFSET_(r16,10,10,v)

    #define   GET32SemaHub_full_ST_11i(r32)                    _BFGET_(r32,11,11)
    #define   SET32SemaHub_full_ST_11i(r32,v)                  _BFSET_(r32,11,11,v)
    #define   GET16SemaHub_full_ST_11i(r16)                    _BFGET_(r16,11,11)
    #define   SET16SemaHub_full_ST_11i(r16,v)                  _BFSET_(r16,11,11,v)

    #define   GET32SemaHub_full_ST_12i(r32)                    _BFGET_(r32,12,12)
    #define   SET32SemaHub_full_ST_12i(r32,v)                  _BFSET_(r32,12,12,v)
    #define   GET16SemaHub_full_ST_12i(r16)                    _BFGET_(r16,12,12)
    #define   SET16SemaHub_full_ST_12i(r16,v)                  _BFSET_(r16,12,12,v)

    #define   GET32SemaHub_full_ST_13i(r32)                    _BFGET_(r32,13,13)
    #define   SET32SemaHub_full_ST_13i(r32,v)                  _BFSET_(r32,13,13,v)
    #define   GET16SemaHub_full_ST_13i(r16)                    _BFGET_(r16,13,13)
    #define   SET16SemaHub_full_ST_13i(r16,v)                  _BFSET_(r16,13,13,v)

    #define   GET32SemaHub_full_ST_14i(r32)                    _BFGET_(r32,14,14)
    #define   SET32SemaHub_full_ST_14i(r32,v)                  _BFSET_(r32,14,14,v)
    #define   GET16SemaHub_full_ST_14i(r16)                    _BFGET_(r16,14,14)
    #define   SET16SemaHub_full_ST_14i(r16,v)                  _BFSET_(r16,14,14,v)

    #define   GET32SemaHub_full_ST_15i(r32)                    _BFGET_(r32,15,15)
    #define   SET32SemaHub_full_ST_15i(r32,v)                  _BFSET_(r32,15,15,v)
    #define   GET16SemaHub_full_ST_15i(r16)                    _BFGET_(r16,15,15)
    #define   SET16SemaHub_full_ST_15i(r16,v)                  _BFSET_(r16,15,15,v)

    #define   GET32SemaHub_full_ST_16i(r32)                    _BFGET_(r32,16,16)
    #define   SET32SemaHub_full_ST_16i(r32,v)                  _BFSET_(r32,16,16,v)
    #define   GET16SemaHub_full_ST_16i(r16)                    _BFGET_(r16, 0, 0)
    #define   SET16SemaHub_full_ST_16i(r16,v)                  _BFSET_(r16, 0, 0,v)

    #define   GET32SemaHub_full_ST_17i(r32)                    _BFGET_(r32,17,17)
    #define   SET32SemaHub_full_ST_17i(r32,v)                  _BFSET_(r32,17,17,v)
    #define   GET16SemaHub_full_ST_17i(r16)                    _BFGET_(r16, 1, 1)
    #define   SET16SemaHub_full_ST_17i(r16,v)                  _BFSET_(r16, 1, 1,v)

    #define   GET32SemaHub_full_ST_18i(r32)                    _BFGET_(r32,18,18)
    #define   SET32SemaHub_full_ST_18i(r32,v)                  _BFSET_(r32,18,18,v)
    #define   GET16SemaHub_full_ST_18i(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16SemaHub_full_ST_18i(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define   GET32SemaHub_full_ST_19i(r32)                    _BFGET_(r32,19,19)
    #define   SET32SemaHub_full_ST_19i(r32,v)                  _BFSET_(r32,19,19,v)
    #define   GET16SemaHub_full_ST_19i(r16)                    _BFGET_(r16, 3, 3)
    #define   SET16SemaHub_full_ST_19i(r16,v)                  _BFSET_(r16, 3, 3,v)

    #define   GET32SemaHub_full_ST_20i(r32)                    _BFGET_(r32,20,20)
    #define   SET32SemaHub_full_ST_20i(r32,v)                  _BFSET_(r32,20,20,v)
    #define   GET16SemaHub_full_ST_20i(r16)                    _BFGET_(r16, 4, 4)
    #define   SET16SemaHub_full_ST_20i(r16,v)                  _BFSET_(r16, 4, 4,v)

    #define   GET32SemaHub_full_ST_21i(r32)                    _BFGET_(r32,21,21)
    #define   SET32SemaHub_full_ST_21i(r32,v)                  _BFSET_(r32,21,21,v)
    #define   GET16SemaHub_full_ST_21i(r16)                    _BFGET_(r16, 5, 5)
    #define   SET16SemaHub_full_ST_21i(r16,v)                  _BFSET_(r16, 5, 5,v)

    #define   GET32SemaHub_full_ST_22i(r32)                    _BFGET_(r32,22,22)
    #define   SET32SemaHub_full_ST_22i(r32,v)                  _BFSET_(r32,22,22,v)
    #define   GET16SemaHub_full_ST_22i(r16)                    _BFGET_(r16, 6, 6)
    #define   SET16SemaHub_full_ST_22i(r16,v)                  _BFSET_(r16, 6, 6,v)

    #define   GET32SemaHub_full_ST_23i(r32)                    _BFGET_(r32,23,23)
    #define   SET32SemaHub_full_ST_23i(r32,v)                  _BFSET_(r32,23,23,v)
    #define   GET16SemaHub_full_ST_23i(r16)                    _BFGET_(r16, 7, 7)
    #define   SET16SemaHub_full_ST_23i(r16,v)                  _BFSET_(r16, 7, 7,v)

    #define   GET32SemaHub_full_ST_24i(r32)                    _BFGET_(r32,24,24)
    #define   SET32SemaHub_full_ST_24i(r32,v)                  _BFSET_(r32,24,24,v)
    #define   GET16SemaHub_full_ST_24i(r16)                    _BFGET_(r16, 8, 8)
    #define   SET16SemaHub_full_ST_24i(r16,v)                  _BFSET_(r16, 8, 8,v)

    #define   GET32SemaHub_full_ST_25i(r32)                    _BFGET_(r32,25,25)
    #define   SET32SemaHub_full_ST_25i(r32,v)                  _BFSET_(r32,25,25,v)
    #define   GET16SemaHub_full_ST_25i(r16)                    _BFGET_(r16, 9, 9)
    #define   SET16SemaHub_full_ST_25i(r16,v)                  _BFSET_(r16, 9, 9,v)

    #define   GET32SemaHub_full_ST_26i(r32)                    _BFGET_(r32,26,26)
    #define   SET32SemaHub_full_ST_26i(r32,v)                  _BFSET_(r32,26,26,v)
    #define   GET16SemaHub_full_ST_26i(r16)                    _BFGET_(r16,10,10)
    #define   SET16SemaHub_full_ST_26i(r16,v)                  _BFSET_(r16,10,10,v)

    #define   GET32SemaHub_full_ST_27i(r32)                    _BFGET_(r32,27,27)
    #define   SET32SemaHub_full_ST_27i(r32,v)                  _BFSET_(r32,27,27,v)
    #define   GET16SemaHub_full_ST_27i(r16)                    _BFGET_(r16,11,11)
    #define   SET16SemaHub_full_ST_27i(r16,v)                  _BFSET_(r16,11,11,v)

    #define   GET32SemaHub_full_ST_28i(r32)                    _BFGET_(r32,28,28)
    #define   SET32SemaHub_full_ST_28i(r32,v)                  _BFSET_(r32,28,28,v)
    #define   GET16SemaHub_full_ST_28i(r16)                    _BFGET_(r16,12,12)
    #define   SET16SemaHub_full_ST_28i(r16,v)                  _BFSET_(r16,12,12,v)

    #define   GET32SemaHub_full_ST_29i(r32)                    _BFGET_(r32,29,29)
    #define   SET32SemaHub_full_ST_29i(r32,v)                  _BFSET_(r32,29,29,v)
    #define   GET16SemaHub_full_ST_29i(r16)                    _BFGET_(r16,13,13)
    #define   SET16SemaHub_full_ST_29i(r16,v)                  _BFSET_(r16,13,13,v)

    #define   GET32SemaHub_full_ST_30i(r32)                    _BFGET_(r32,30,30)
    #define   SET32SemaHub_full_ST_30i(r32,v)                  _BFSET_(r32,30,30,v)
    #define   GET16SemaHub_full_ST_30i(r16)                    _BFGET_(r16,14,14)
    #define   SET16SemaHub_full_ST_30i(r16,v)                  _BFSET_(r16,14,14,v)

    #define   GET32SemaHub_full_ST_31i(r32)                    _BFGET_(r32,31,31)
    #define   SET32SemaHub_full_ST_31i(r32,v)                  _BFSET_(r32,31,31,v)
    #define   GET16SemaHub_full_ST_31i(r16)                    _BFGET_(r16,15,15)
    #define   SET16SemaHub_full_ST_31i(r16,v)                  _BFSET_(r16,15,15,v)

    #define     w32SemaHub_full                                {\
            UNSG32 ufull_ST_0i                                 :  1;\
            UNSG32 ufull_ST_1i                                 :  1;\
            UNSG32 ufull_ST_2i                                 :  1;\
            UNSG32 ufull_ST_3i                                 :  1;\
            UNSG32 ufull_ST_4i                                 :  1;\
            UNSG32 ufull_ST_5i                                 :  1;\
            UNSG32 ufull_ST_6i                                 :  1;\
            UNSG32 ufull_ST_7i                                 :  1;\
            UNSG32 ufull_ST_8i                                 :  1;\
            UNSG32 ufull_ST_9i                                 :  1;\
            UNSG32 ufull_ST_10i                                :  1;\
            UNSG32 ufull_ST_11i                                :  1;\
            UNSG32 ufull_ST_12i                                :  1;\
            UNSG32 ufull_ST_13i                                :  1;\
            UNSG32 ufull_ST_14i                                :  1;\
            UNSG32 ufull_ST_15i                                :  1;\
            UNSG32 ufull_ST_16i                                :  1;\
            UNSG32 ufull_ST_17i                                :  1;\
            UNSG32 ufull_ST_18i                                :  1;\
            UNSG32 ufull_ST_19i                                :  1;\
            UNSG32 ufull_ST_20i                                :  1;\
            UNSG32 ufull_ST_21i                                :  1;\
            UNSG32 ufull_ST_22i                                :  1;\
            UNSG32 ufull_ST_23i                                :  1;\
            UNSG32 ufull_ST_24i                                :  1;\
            UNSG32 ufull_ST_25i                                :  1;\
            UNSG32 ufull_ST_26i                                :  1;\
            UNSG32 ufull_ST_27i                                :  1;\
            UNSG32 ufull_ST_28i                                :  1;\
            UNSG32 ufull_ST_29i                                :  1;\
            UNSG32 ufull_ST_30i                                :  1;\
            UNSG32 ufull_ST_31i                                :  1;\
          }
    union { UNSG32 u32SemaHub_full;
            struct w32SemaHub_full;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32SemaHub_almostEmpty_ST_0i(r32)              _BFGET_(r32, 0, 0)
    #define   SET32SemaHub_almostEmpty_ST_0i(r32,v)            _BFSET_(r32, 0, 0,v)
    #define   GET16SemaHub_almostEmpty_ST_0i(r16)              _BFGET_(r16, 0, 0)
    #define   SET16SemaHub_almostEmpty_ST_0i(r16,v)            _BFSET_(r16, 0, 0,v)

    #define   GET32SemaHub_almostEmpty_ST_1i(r32)              _BFGET_(r32, 1, 1)
    #define   SET32SemaHub_almostEmpty_ST_1i(r32,v)            _BFSET_(r32, 1, 1,v)
    #define   GET16SemaHub_almostEmpty_ST_1i(r16)              _BFGET_(r16, 1, 1)
    #define   SET16SemaHub_almostEmpty_ST_1i(r16,v)            _BFSET_(r16, 1, 1,v)

    #define   GET32SemaHub_almostEmpty_ST_2i(r32)              _BFGET_(r32, 2, 2)
    #define   SET32SemaHub_almostEmpty_ST_2i(r32,v)            _BFSET_(r32, 2, 2,v)
    #define   GET16SemaHub_almostEmpty_ST_2i(r16)              _BFGET_(r16, 2, 2)
    #define   SET16SemaHub_almostEmpty_ST_2i(r16,v)            _BFSET_(r16, 2, 2,v)

    #define   GET32SemaHub_almostEmpty_ST_3i(r32)              _BFGET_(r32, 3, 3)
    #define   SET32SemaHub_almostEmpty_ST_3i(r32,v)            _BFSET_(r32, 3, 3,v)
    #define   GET16SemaHub_almostEmpty_ST_3i(r16)              _BFGET_(r16, 3, 3)
    #define   SET16SemaHub_almostEmpty_ST_3i(r16,v)            _BFSET_(r16, 3, 3,v)

    #define   GET32SemaHub_almostEmpty_ST_4i(r32)              _BFGET_(r32, 4, 4)
    #define   SET32SemaHub_almostEmpty_ST_4i(r32,v)            _BFSET_(r32, 4, 4,v)
    #define   GET16SemaHub_almostEmpty_ST_4i(r16)              _BFGET_(r16, 4, 4)
    #define   SET16SemaHub_almostEmpty_ST_4i(r16,v)            _BFSET_(r16, 4, 4,v)

    #define   GET32SemaHub_almostEmpty_ST_5i(r32)              _BFGET_(r32, 5, 5)
    #define   SET32SemaHub_almostEmpty_ST_5i(r32,v)            _BFSET_(r32, 5, 5,v)
    #define   GET16SemaHub_almostEmpty_ST_5i(r16)              _BFGET_(r16, 5, 5)
    #define   SET16SemaHub_almostEmpty_ST_5i(r16,v)            _BFSET_(r16, 5, 5,v)

    #define   GET32SemaHub_almostEmpty_ST_6i(r32)              _BFGET_(r32, 6, 6)
    #define   SET32SemaHub_almostEmpty_ST_6i(r32,v)            _BFSET_(r32, 6, 6,v)
    #define   GET16SemaHub_almostEmpty_ST_6i(r16)              _BFGET_(r16, 6, 6)
    #define   SET16SemaHub_almostEmpty_ST_6i(r16,v)            _BFSET_(r16, 6, 6,v)

    #define   GET32SemaHub_almostEmpty_ST_7i(r32)              _BFGET_(r32, 7, 7)
    #define   SET32SemaHub_almostEmpty_ST_7i(r32,v)            _BFSET_(r32, 7, 7,v)
    #define   GET16SemaHub_almostEmpty_ST_7i(r16)              _BFGET_(r16, 7, 7)
    #define   SET16SemaHub_almostEmpty_ST_7i(r16,v)            _BFSET_(r16, 7, 7,v)

    #define   GET32SemaHub_almostEmpty_ST_8i(r32)              _BFGET_(r32, 8, 8)
    #define   SET32SemaHub_almostEmpty_ST_8i(r32,v)            _BFSET_(r32, 8, 8,v)
    #define   GET16SemaHub_almostEmpty_ST_8i(r16)              _BFGET_(r16, 8, 8)
    #define   SET16SemaHub_almostEmpty_ST_8i(r16,v)            _BFSET_(r16, 8, 8,v)

    #define   GET32SemaHub_almostEmpty_ST_9i(r32)              _BFGET_(r32, 9, 9)
    #define   SET32SemaHub_almostEmpty_ST_9i(r32,v)            _BFSET_(r32, 9, 9,v)
    #define   GET16SemaHub_almostEmpty_ST_9i(r16)              _BFGET_(r16, 9, 9)
    #define   SET16SemaHub_almostEmpty_ST_9i(r16,v)            _BFSET_(r16, 9, 9,v)

    #define   GET32SemaHub_almostEmpty_ST_10i(r32)             _BFGET_(r32,10,10)
    #define   SET32SemaHub_almostEmpty_ST_10i(r32,v)           _BFSET_(r32,10,10,v)
    #define   GET16SemaHub_almostEmpty_ST_10i(r16)             _BFGET_(r16,10,10)
    #define   SET16SemaHub_almostEmpty_ST_10i(r16,v)           _BFSET_(r16,10,10,v)

    #define   GET32SemaHub_almostEmpty_ST_11i(r32)             _BFGET_(r32,11,11)
    #define   SET32SemaHub_almostEmpty_ST_11i(r32,v)           _BFSET_(r32,11,11,v)
    #define   GET16SemaHub_almostEmpty_ST_11i(r16)             _BFGET_(r16,11,11)
    #define   SET16SemaHub_almostEmpty_ST_11i(r16,v)           _BFSET_(r16,11,11,v)

    #define   GET32SemaHub_almostEmpty_ST_12i(r32)             _BFGET_(r32,12,12)
    #define   SET32SemaHub_almostEmpty_ST_12i(r32,v)           _BFSET_(r32,12,12,v)
    #define   GET16SemaHub_almostEmpty_ST_12i(r16)             _BFGET_(r16,12,12)
    #define   SET16SemaHub_almostEmpty_ST_12i(r16,v)           _BFSET_(r16,12,12,v)

    #define   GET32SemaHub_almostEmpty_ST_13i(r32)             _BFGET_(r32,13,13)
    #define   SET32SemaHub_almostEmpty_ST_13i(r32,v)           _BFSET_(r32,13,13,v)
    #define   GET16SemaHub_almostEmpty_ST_13i(r16)             _BFGET_(r16,13,13)
    #define   SET16SemaHub_almostEmpty_ST_13i(r16,v)           _BFSET_(r16,13,13,v)

    #define   GET32SemaHub_almostEmpty_ST_14i(r32)             _BFGET_(r32,14,14)
    #define   SET32SemaHub_almostEmpty_ST_14i(r32,v)           _BFSET_(r32,14,14,v)
    #define   GET16SemaHub_almostEmpty_ST_14i(r16)             _BFGET_(r16,14,14)
    #define   SET16SemaHub_almostEmpty_ST_14i(r16,v)           _BFSET_(r16,14,14,v)

    #define   GET32SemaHub_almostEmpty_ST_15i(r32)             _BFGET_(r32,15,15)
    #define   SET32SemaHub_almostEmpty_ST_15i(r32,v)           _BFSET_(r32,15,15,v)
    #define   GET16SemaHub_almostEmpty_ST_15i(r16)             _BFGET_(r16,15,15)
    #define   SET16SemaHub_almostEmpty_ST_15i(r16,v)           _BFSET_(r16,15,15,v)

    #define   GET32SemaHub_almostEmpty_ST_16i(r32)             _BFGET_(r32,16,16)
    #define   SET32SemaHub_almostEmpty_ST_16i(r32,v)           _BFSET_(r32,16,16,v)
    #define   GET16SemaHub_almostEmpty_ST_16i(r16)             _BFGET_(r16, 0, 0)
    #define   SET16SemaHub_almostEmpty_ST_16i(r16,v)           _BFSET_(r16, 0, 0,v)

    #define   GET32SemaHub_almostEmpty_ST_17i(r32)             _BFGET_(r32,17,17)
    #define   SET32SemaHub_almostEmpty_ST_17i(r32,v)           _BFSET_(r32,17,17,v)
    #define   GET16SemaHub_almostEmpty_ST_17i(r16)             _BFGET_(r16, 1, 1)
    #define   SET16SemaHub_almostEmpty_ST_17i(r16,v)           _BFSET_(r16, 1, 1,v)

    #define   GET32SemaHub_almostEmpty_ST_18i(r32)             _BFGET_(r32,18,18)
    #define   SET32SemaHub_almostEmpty_ST_18i(r32,v)           _BFSET_(r32,18,18,v)
    #define   GET16SemaHub_almostEmpty_ST_18i(r16)             _BFGET_(r16, 2, 2)
    #define   SET16SemaHub_almostEmpty_ST_18i(r16,v)           _BFSET_(r16, 2, 2,v)

    #define   GET32SemaHub_almostEmpty_ST_19i(r32)             _BFGET_(r32,19,19)
    #define   SET32SemaHub_almostEmpty_ST_19i(r32,v)           _BFSET_(r32,19,19,v)
    #define   GET16SemaHub_almostEmpty_ST_19i(r16)             _BFGET_(r16, 3, 3)
    #define   SET16SemaHub_almostEmpty_ST_19i(r16,v)           _BFSET_(r16, 3, 3,v)

    #define   GET32SemaHub_almostEmpty_ST_20i(r32)             _BFGET_(r32,20,20)
    #define   SET32SemaHub_almostEmpty_ST_20i(r32,v)           _BFSET_(r32,20,20,v)
    #define   GET16SemaHub_almostEmpty_ST_20i(r16)             _BFGET_(r16, 4, 4)
    #define   SET16SemaHub_almostEmpty_ST_20i(r16,v)           _BFSET_(r16, 4, 4,v)

    #define   GET32SemaHub_almostEmpty_ST_21i(r32)             _BFGET_(r32,21,21)
    #define   SET32SemaHub_almostEmpty_ST_21i(r32,v)           _BFSET_(r32,21,21,v)
    #define   GET16SemaHub_almostEmpty_ST_21i(r16)             _BFGET_(r16, 5, 5)
    #define   SET16SemaHub_almostEmpty_ST_21i(r16,v)           _BFSET_(r16, 5, 5,v)

    #define   GET32SemaHub_almostEmpty_ST_22i(r32)             _BFGET_(r32,22,22)
    #define   SET32SemaHub_almostEmpty_ST_22i(r32,v)           _BFSET_(r32,22,22,v)
    #define   GET16SemaHub_almostEmpty_ST_22i(r16)             _BFGET_(r16, 6, 6)
    #define   SET16SemaHub_almostEmpty_ST_22i(r16,v)           _BFSET_(r16, 6, 6,v)

    #define   GET32SemaHub_almostEmpty_ST_23i(r32)             _BFGET_(r32,23,23)
    #define   SET32SemaHub_almostEmpty_ST_23i(r32,v)           _BFSET_(r32,23,23,v)
    #define   GET16SemaHub_almostEmpty_ST_23i(r16)             _BFGET_(r16, 7, 7)
    #define   SET16SemaHub_almostEmpty_ST_23i(r16,v)           _BFSET_(r16, 7, 7,v)

    #define   GET32SemaHub_almostEmpty_ST_24i(r32)             _BFGET_(r32,24,24)
    #define   SET32SemaHub_almostEmpty_ST_24i(r32,v)           _BFSET_(r32,24,24,v)
    #define   GET16SemaHub_almostEmpty_ST_24i(r16)             _BFGET_(r16, 8, 8)
    #define   SET16SemaHub_almostEmpty_ST_24i(r16,v)           _BFSET_(r16, 8, 8,v)

    #define   GET32SemaHub_almostEmpty_ST_25i(r32)             _BFGET_(r32,25,25)
    #define   SET32SemaHub_almostEmpty_ST_25i(r32,v)           _BFSET_(r32,25,25,v)
    #define   GET16SemaHub_almostEmpty_ST_25i(r16)             _BFGET_(r16, 9, 9)
    #define   SET16SemaHub_almostEmpty_ST_25i(r16,v)           _BFSET_(r16, 9, 9,v)

    #define   GET32SemaHub_almostEmpty_ST_26i(r32)             _BFGET_(r32,26,26)
    #define   SET32SemaHub_almostEmpty_ST_26i(r32,v)           _BFSET_(r32,26,26,v)
    #define   GET16SemaHub_almostEmpty_ST_26i(r16)             _BFGET_(r16,10,10)
    #define   SET16SemaHub_almostEmpty_ST_26i(r16,v)           _BFSET_(r16,10,10,v)

    #define   GET32SemaHub_almostEmpty_ST_27i(r32)             _BFGET_(r32,27,27)
    #define   SET32SemaHub_almostEmpty_ST_27i(r32,v)           _BFSET_(r32,27,27,v)
    #define   GET16SemaHub_almostEmpty_ST_27i(r16)             _BFGET_(r16,11,11)
    #define   SET16SemaHub_almostEmpty_ST_27i(r16,v)           _BFSET_(r16,11,11,v)

    #define   GET32SemaHub_almostEmpty_ST_28i(r32)             _BFGET_(r32,28,28)
    #define   SET32SemaHub_almostEmpty_ST_28i(r32,v)           _BFSET_(r32,28,28,v)
    #define   GET16SemaHub_almostEmpty_ST_28i(r16)             _BFGET_(r16,12,12)
    #define   SET16SemaHub_almostEmpty_ST_28i(r16,v)           _BFSET_(r16,12,12,v)

    #define   GET32SemaHub_almostEmpty_ST_29i(r32)             _BFGET_(r32,29,29)
    #define   SET32SemaHub_almostEmpty_ST_29i(r32,v)           _BFSET_(r32,29,29,v)
    #define   GET16SemaHub_almostEmpty_ST_29i(r16)             _BFGET_(r16,13,13)
    #define   SET16SemaHub_almostEmpty_ST_29i(r16,v)           _BFSET_(r16,13,13,v)

    #define   GET32SemaHub_almostEmpty_ST_30i(r32)             _BFGET_(r32,30,30)
    #define   SET32SemaHub_almostEmpty_ST_30i(r32,v)           _BFSET_(r32,30,30,v)
    #define   GET16SemaHub_almostEmpty_ST_30i(r16)             _BFGET_(r16,14,14)
    #define   SET16SemaHub_almostEmpty_ST_30i(r16,v)           _BFSET_(r16,14,14,v)

    #define   GET32SemaHub_almostEmpty_ST_31i(r32)             _BFGET_(r32,31,31)
    #define   SET32SemaHub_almostEmpty_ST_31i(r32,v)           _BFSET_(r32,31,31,v)
    #define   GET16SemaHub_almostEmpty_ST_31i(r16)             _BFGET_(r16,15,15)
    #define   SET16SemaHub_almostEmpty_ST_31i(r16,v)           _BFSET_(r16,15,15,v)

    #define     w32SemaHub_almostEmpty                         {\
            UNSG32 ualmostEmpty_ST_0i                          :  1;\
            UNSG32 ualmostEmpty_ST_1i                          :  1;\
            UNSG32 ualmostEmpty_ST_2i                          :  1;\
            UNSG32 ualmostEmpty_ST_3i                          :  1;\
            UNSG32 ualmostEmpty_ST_4i                          :  1;\
            UNSG32 ualmostEmpty_ST_5i                          :  1;\
            UNSG32 ualmostEmpty_ST_6i                          :  1;\
            UNSG32 ualmostEmpty_ST_7i                          :  1;\
            UNSG32 ualmostEmpty_ST_8i                          :  1;\
            UNSG32 ualmostEmpty_ST_9i                          :  1;\
            UNSG32 ualmostEmpty_ST_10i                         :  1;\
            UNSG32 ualmostEmpty_ST_11i                         :  1;\
            UNSG32 ualmostEmpty_ST_12i                         :  1;\
            UNSG32 ualmostEmpty_ST_13i                         :  1;\
            UNSG32 ualmostEmpty_ST_14i                         :  1;\
            UNSG32 ualmostEmpty_ST_15i                         :  1;\
            UNSG32 ualmostEmpty_ST_16i                         :  1;\
            UNSG32 ualmostEmpty_ST_17i                         :  1;\
            UNSG32 ualmostEmpty_ST_18i                         :  1;\
            UNSG32 ualmostEmpty_ST_19i                         :  1;\
            UNSG32 ualmostEmpty_ST_20i                         :  1;\
            UNSG32 ualmostEmpty_ST_21i                         :  1;\
            UNSG32 ualmostEmpty_ST_22i                         :  1;\
            UNSG32 ualmostEmpty_ST_23i                         :  1;\
            UNSG32 ualmostEmpty_ST_24i                         :  1;\
            UNSG32 ualmostEmpty_ST_25i                         :  1;\
            UNSG32 ualmostEmpty_ST_26i                         :  1;\
            UNSG32 ualmostEmpty_ST_27i                         :  1;\
            UNSG32 ualmostEmpty_ST_28i                         :  1;\
            UNSG32 ualmostEmpty_ST_29i                         :  1;\
            UNSG32 ualmostEmpty_ST_30i                         :  1;\
            UNSG32 ualmostEmpty_ST_31i                         :  1;\
          }
    union { UNSG32 u32SemaHub_almostEmpty;
            struct w32SemaHub_almostEmpty;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32SemaHub_almostFull_ST_0i(r32)               _BFGET_(r32, 0, 0)
    #define   SET32SemaHub_almostFull_ST_0i(r32,v)             _BFSET_(r32, 0, 0,v)
    #define   GET16SemaHub_almostFull_ST_0i(r16)               _BFGET_(r16, 0, 0)
    #define   SET16SemaHub_almostFull_ST_0i(r16,v)             _BFSET_(r16, 0, 0,v)

    #define   GET32SemaHub_almostFull_ST_1i(r32)               _BFGET_(r32, 1, 1)
    #define   SET32SemaHub_almostFull_ST_1i(r32,v)             _BFSET_(r32, 1, 1,v)
    #define   GET16SemaHub_almostFull_ST_1i(r16)               _BFGET_(r16, 1, 1)
    #define   SET16SemaHub_almostFull_ST_1i(r16,v)             _BFSET_(r16, 1, 1,v)

    #define   GET32SemaHub_almostFull_ST_2i(r32)               _BFGET_(r32, 2, 2)
    #define   SET32SemaHub_almostFull_ST_2i(r32,v)             _BFSET_(r32, 2, 2,v)
    #define   GET16SemaHub_almostFull_ST_2i(r16)               _BFGET_(r16, 2, 2)
    #define   SET16SemaHub_almostFull_ST_2i(r16,v)             _BFSET_(r16, 2, 2,v)

    #define   GET32SemaHub_almostFull_ST_3i(r32)               _BFGET_(r32, 3, 3)
    #define   SET32SemaHub_almostFull_ST_3i(r32,v)             _BFSET_(r32, 3, 3,v)
    #define   GET16SemaHub_almostFull_ST_3i(r16)               _BFGET_(r16, 3, 3)
    #define   SET16SemaHub_almostFull_ST_3i(r16,v)             _BFSET_(r16, 3, 3,v)

    #define   GET32SemaHub_almostFull_ST_4i(r32)               _BFGET_(r32, 4, 4)
    #define   SET32SemaHub_almostFull_ST_4i(r32,v)             _BFSET_(r32, 4, 4,v)
    #define   GET16SemaHub_almostFull_ST_4i(r16)               _BFGET_(r16, 4, 4)
    #define   SET16SemaHub_almostFull_ST_4i(r16,v)             _BFSET_(r16, 4, 4,v)

    #define   GET32SemaHub_almostFull_ST_5i(r32)               _BFGET_(r32, 5, 5)
    #define   SET32SemaHub_almostFull_ST_5i(r32,v)             _BFSET_(r32, 5, 5,v)
    #define   GET16SemaHub_almostFull_ST_5i(r16)               _BFGET_(r16, 5, 5)
    #define   SET16SemaHub_almostFull_ST_5i(r16,v)             _BFSET_(r16, 5, 5,v)

    #define   GET32SemaHub_almostFull_ST_6i(r32)               _BFGET_(r32, 6, 6)
    #define   SET32SemaHub_almostFull_ST_6i(r32,v)             _BFSET_(r32, 6, 6,v)
    #define   GET16SemaHub_almostFull_ST_6i(r16)               _BFGET_(r16, 6, 6)
    #define   SET16SemaHub_almostFull_ST_6i(r16,v)             _BFSET_(r16, 6, 6,v)

    #define   GET32SemaHub_almostFull_ST_7i(r32)               _BFGET_(r32, 7, 7)
    #define   SET32SemaHub_almostFull_ST_7i(r32,v)             _BFSET_(r32, 7, 7,v)
    #define   GET16SemaHub_almostFull_ST_7i(r16)               _BFGET_(r16, 7, 7)
    #define   SET16SemaHub_almostFull_ST_7i(r16,v)             _BFSET_(r16, 7, 7,v)

    #define   GET32SemaHub_almostFull_ST_8i(r32)               _BFGET_(r32, 8, 8)
    #define   SET32SemaHub_almostFull_ST_8i(r32,v)             _BFSET_(r32, 8, 8,v)
    #define   GET16SemaHub_almostFull_ST_8i(r16)               _BFGET_(r16, 8, 8)
    #define   SET16SemaHub_almostFull_ST_8i(r16,v)             _BFSET_(r16, 8, 8,v)

    #define   GET32SemaHub_almostFull_ST_9i(r32)               _BFGET_(r32, 9, 9)
    #define   SET32SemaHub_almostFull_ST_9i(r32,v)             _BFSET_(r32, 9, 9,v)
    #define   GET16SemaHub_almostFull_ST_9i(r16)               _BFGET_(r16, 9, 9)
    #define   SET16SemaHub_almostFull_ST_9i(r16,v)             _BFSET_(r16, 9, 9,v)

    #define   GET32SemaHub_almostFull_ST_10i(r32)              _BFGET_(r32,10,10)
    #define   SET32SemaHub_almostFull_ST_10i(r32,v)            _BFSET_(r32,10,10,v)
    #define   GET16SemaHub_almostFull_ST_10i(r16)              _BFGET_(r16,10,10)
    #define   SET16SemaHub_almostFull_ST_10i(r16,v)            _BFSET_(r16,10,10,v)

    #define   GET32SemaHub_almostFull_ST_11i(r32)              _BFGET_(r32,11,11)
    #define   SET32SemaHub_almostFull_ST_11i(r32,v)            _BFSET_(r32,11,11,v)
    #define   GET16SemaHub_almostFull_ST_11i(r16)              _BFGET_(r16,11,11)
    #define   SET16SemaHub_almostFull_ST_11i(r16,v)            _BFSET_(r16,11,11,v)

    #define   GET32SemaHub_almostFull_ST_12i(r32)              _BFGET_(r32,12,12)
    #define   SET32SemaHub_almostFull_ST_12i(r32,v)            _BFSET_(r32,12,12,v)
    #define   GET16SemaHub_almostFull_ST_12i(r16)              _BFGET_(r16,12,12)
    #define   SET16SemaHub_almostFull_ST_12i(r16,v)            _BFSET_(r16,12,12,v)

    #define   GET32SemaHub_almostFull_ST_13i(r32)              _BFGET_(r32,13,13)
    #define   SET32SemaHub_almostFull_ST_13i(r32,v)            _BFSET_(r32,13,13,v)
    #define   GET16SemaHub_almostFull_ST_13i(r16)              _BFGET_(r16,13,13)
    #define   SET16SemaHub_almostFull_ST_13i(r16,v)            _BFSET_(r16,13,13,v)

    #define   GET32SemaHub_almostFull_ST_14i(r32)              _BFGET_(r32,14,14)
    #define   SET32SemaHub_almostFull_ST_14i(r32,v)            _BFSET_(r32,14,14,v)
    #define   GET16SemaHub_almostFull_ST_14i(r16)              _BFGET_(r16,14,14)
    #define   SET16SemaHub_almostFull_ST_14i(r16,v)            _BFSET_(r16,14,14,v)

    #define   GET32SemaHub_almostFull_ST_15i(r32)              _BFGET_(r32,15,15)
    #define   SET32SemaHub_almostFull_ST_15i(r32,v)            _BFSET_(r32,15,15,v)
    #define   GET16SemaHub_almostFull_ST_15i(r16)              _BFGET_(r16,15,15)
    #define   SET16SemaHub_almostFull_ST_15i(r16,v)            _BFSET_(r16,15,15,v)

    #define   GET32SemaHub_almostFull_ST_16i(r32)              _BFGET_(r32,16,16)
    #define   SET32SemaHub_almostFull_ST_16i(r32,v)            _BFSET_(r32,16,16,v)
    #define   GET16SemaHub_almostFull_ST_16i(r16)              _BFGET_(r16, 0, 0)
    #define   SET16SemaHub_almostFull_ST_16i(r16,v)            _BFSET_(r16, 0, 0,v)

    #define   GET32SemaHub_almostFull_ST_17i(r32)              _BFGET_(r32,17,17)
    #define   SET32SemaHub_almostFull_ST_17i(r32,v)            _BFSET_(r32,17,17,v)
    #define   GET16SemaHub_almostFull_ST_17i(r16)              _BFGET_(r16, 1, 1)
    #define   SET16SemaHub_almostFull_ST_17i(r16,v)            _BFSET_(r16, 1, 1,v)

    #define   GET32SemaHub_almostFull_ST_18i(r32)              _BFGET_(r32,18,18)
    #define   SET32SemaHub_almostFull_ST_18i(r32,v)            _BFSET_(r32,18,18,v)
    #define   GET16SemaHub_almostFull_ST_18i(r16)              _BFGET_(r16, 2, 2)
    #define   SET16SemaHub_almostFull_ST_18i(r16,v)            _BFSET_(r16, 2, 2,v)

    #define   GET32SemaHub_almostFull_ST_19i(r32)              _BFGET_(r32,19,19)
    #define   SET32SemaHub_almostFull_ST_19i(r32,v)            _BFSET_(r32,19,19,v)
    #define   GET16SemaHub_almostFull_ST_19i(r16)              _BFGET_(r16, 3, 3)
    #define   SET16SemaHub_almostFull_ST_19i(r16,v)            _BFSET_(r16, 3, 3,v)

    #define   GET32SemaHub_almostFull_ST_20i(r32)              _BFGET_(r32,20,20)
    #define   SET32SemaHub_almostFull_ST_20i(r32,v)            _BFSET_(r32,20,20,v)
    #define   GET16SemaHub_almostFull_ST_20i(r16)              _BFGET_(r16, 4, 4)
    #define   SET16SemaHub_almostFull_ST_20i(r16,v)            _BFSET_(r16, 4, 4,v)

    #define   GET32SemaHub_almostFull_ST_21i(r32)              _BFGET_(r32,21,21)
    #define   SET32SemaHub_almostFull_ST_21i(r32,v)            _BFSET_(r32,21,21,v)
    #define   GET16SemaHub_almostFull_ST_21i(r16)              _BFGET_(r16, 5, 5)
    #define   SET16SemaHub_almostFull_ST_21i(r16,v)            _BFSET_(r16, 5, 5,v)

    #define   GET32SemaHub_almostFull_ST_22i(r32)              _BFGET_(r32,22,22)
    #define   SET32SemaHub_almostFull_ST_22i(r32,v)            _BFSET_(r32,22,22,v)
    #define   GET16SemaHub_almostFull_ST_22i(r16)              _BFGET_(r16, 6, 6)
    #define   SET16SemaHub_almostFull_ST_22i(r16,v)            _BFSET_(r16, 6, 6,v)

    #define   GET32SemaHub_almostFull_ST_23i(r32)              _BFGET_(r32,23,23)
    #define   SET32SemaHub_almostFull_ST_23i(r32,v)            _BFSET_(r32,23,23,v)
    #define   GET16SemaHub_almostFull_ST_23i(r16)              _BFGET_(r16, 7, 7)
    #define   SET16SemaHub_almostFull_ST_23i(r16,v)            _BFSET_(r16, 7, 7,v)

    #define   GET32SemaHub_almostFull_ST_24i(r32)              _BFGET_(r32,24,24)
    #define   SET32SemaHub_almostFull_ST_24i(r32,v)            _BFSET_(r32,24,24,v)
    #define   GET16SemaHub_almostFull_ST_24i(r16)              _BFGET_(r16, 8, 8)
    #define   SET16SemaHub_almostFull_ST_24i(r16,v)            _BFSET_(r16, 8, 8,v)

    #define   GET32SemaHub_almostFull_ST_25i(r32)              _BFGET_(r32,25,25)
    #define   SET32SemaHub_almostFull_ST_25i(r32,v)            _BFSET_(r32,25,25,v)
    #define   GET16SemaHub_almostFull_ST_25i(r16)              _BFGET_(r16, 9, 9)
    #define   SET16SemaHub_almostFull_ST_25i(r16,v)            _BFSET_(r16, 9, 9,v)

    #define   GET32SemaHub_almostFull_ST_26i(r32)              _BFGET_(r32,26,26)
    #define   SET32SemaHub_almostFull_ST_26i(r32,v)            _BFSET_(r32,26,26,v)
    #define   GET16SemaHub_almostFull_ST_26i(r16)              _BFGET_(r16,10,10)
    #define   SET16SemaHub_almostFull_ST_26i(r16,v)            _BFSET_(r16,10,10,v)

    #define   GET32SemaHub_almostFull_ST_27i(r32)              _BFGET_(r32,27,27)
    #define   SET32SemaHub_almostFull_ST_27i(r32,v)            _BFSET_(r32,27,27,v)
    #define   GET16SemaHub_almostFull_ST_27i(r16)              _BFGET_(r16,11,11)
    #define   SET16SemaHub_almostFull_ST_27i(r16,v)            _BFSET_(r16,11,11,v)

    #define   GET32SemaHub_almostFull_ST_28i(r32)              _BFGET_(r32,28,28)
    #define   SET32SemaHub_almostFull_ST_28i(r32,v)            _BFSET_(r32,28,28,v)
    #define   GET16SemaHub_almostFull_ST_28i(r16)              _BFGET_(r16,12,12)
    #define   SET16SemaHub_almostFull_ST_28i(r16,v)            _BFSET_(r16,12,12,v)

    #define   GET32SemaHub_almostFull_ST_29i(r32)              _BFGET_(r32,29,29)
    #define   SET32SemaHub_almostFull_ST_29i(r32,v)            _BFSET_(r32,29,29,v)
    #define   GET16SemaHub_almostFull_ST_29i(r16)              _BFGET_(r16,13,13)
    #define   SET16SemaHub_almostFull_ST_29i(r16,v)            _BFSET_(r16,13,13,v)

    #define   GET32SemaHub_almostFull_ST_30i(r32)              _BFGET_(r32,30,30)
    #define   SET32SemaHub_almostFull_ST_30i(r32,v)            _BFSET_(r32,30,30,v)
    #define   GET16SemaHub_almostFull_ST_30i(r16)              _BFGET_(r16,14,14)
    #define   SET16SemaHub_almostFull_ST_30i(r16,v)            _BFSET_(r16,14,14,v)

    #define   GET32SemaHub_almostFull_ST_31i(r32)              _BFGET_(r32,31,31)
    #define   SET32SemaHub_almostFull_ST_31i(r32,v)            _BFSET_(r32,31,31,v)
    #define   GET16SemaHub_almostFull_ST_31i(r16)              _BFGET_(r16,15,15)
    #define   SET16SemaHub_almostFull_ST_31i(r16,v)            _BFSET_(r16,15,15,v)

    #define     w32SemaHub_almostFull                          {\
            UNSG32 ualmostFull_ST_0i                           :  1;\
            UNSG32 ualmostFull_ST_1i                           :  1;\
            UNSG32 ualmostFull_ST_2i                           :  1;\
            UNSG32 ualmostFull_ST_3i                           :  1;\
            UNSG32 ualmostFull_ST_4i                           :  1;\
            UNSG32 ualmostFull_ST_5i                           :  1;\
            UNSG32 ualmostFull_ST_6i                           :  1;\
            UNSG32 ualmostFull_ST_7i                           :  1;\
            UNSG32 ualmostFull_ST_8i                           :  1;\
            UNSG32 ualmostFull_ST_9i                           :  1;\
            UNSG32 ualmostFull_ST_10i                          :  1;\
            UNSG32 ualmostFull_ST_11i                          :  1;\
            UNSG32 ualmostFull_ST_12i                          :  1;\
            UNSG32 ualmostFull_ST_13i                          :  1;\
            UNSG32 ualmostFull_ST_14i                          :  1;\
            UNSG32 ualmostFull_ST_15i                          :  1;\
            UNSG32 ualmostFull_ST_16i                          :  1;\
            UNSG32 ualmostFull_ST_17i                          :  1;\
            UNSG32 ualmostFull_ST_18i                          :  1;\
            UNSG32 ualmostFull_ST_19i                          :  1;\
            UNSG32 ualmostFull_ST_20i                          :  1;\
            UNSG32 ualmostFull_ST_21i                          :  1;\
            UNSG32 ualmostFull_ST_22i                          :  1;\
            UNSG32 ualmostFull_ST_23i                          :  1;\
            UNSG32 ualmostFull_ST_24i                          :  1;\
            UNSG32 ualmostFull_ST_25i                          :  1;\
            UNSG32 ualmostFull_ST_26i                          :  1;\
            UNSG32 ualmostFull_ST_27i                          :  1;\
            UNSG32 ualmostFull_ST_28i                          :  1;\
            UNSG32 ualmostFull_ST_29i                          :  1;\
            UNSG32 ualmostFull_ST_30i                          :  1;\
            UNSG32 ualmostFull_ST_31i                          :  1;\
          }
    union { UNSG32 u32SemaHub_almostFull;
            struct w32SemaHub_almostFull;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx398                                    [104];
    ///////////////////////////////////////////////////////////
    } SIE_SemaHub;

    typedef union  T32SemaHub_PUSH
          { UNSG32 u32;
            struct w32SemaHub_PUSH;
                 } T32SemaHub_PUSH;
    typedef union  T32SemaHub_POP
          { UNSG32 u32;
            struct w32SemaHub_POP;
                 } T32SemaHub_POP;
    typedef union  T32SemaHub_empty
          { UNSG32 u32;
            struct w32SemaHub_empty;
                 } T32SemaHub_empty;
    typedef union  T32SemaHub_full
          { UNSG32 u32;
            struct w32SemaHub_full;
                 } T32SemaHub_full;
    typedef union  T32SemaHub_almostEmpty
          { UNSG32 u32;
            struct w32SemaHub_almostEmpty;
                 } T32SemaHub_almostEmpty;
    typedef union  T32SemaHub_almostFull
          { UNSG32 u32;
            struct w32SemaHub_almostFull;
                 } T32SemaHub_almostFull;
    ///////////////////////////////////////////////////////////

    typedef union  TSemaHub_PUSH
          { UNSG32 u32[1];
            struct {
            struct w32SemaHub_PUSH;
                   };
                 } TSemaHub_PUSH;
    typedef union  TSemaHub_POP
          { UNSG32 u32[1];
            struct {
            struct w32SemaHub_POP;
                   };
                 } TSemaHub_POP;
    typedef union  TSemaHub_empty
          { UNSG32 u32[1];
            struct {
            struct w32SemaHub_empty;
                   };
                 } TSemaHub_empty;
    typedef union  TSemaHub_full
          { UNSG32 u32[1];
            struct {
            struct w32SemaHub_full;
                   };
                 } TSemaHub_full;
    typedef union  TSemaHub_almostEmpty
          { UNSG32 u32[1];
            struct {
            struct w32SemaHub_almostEmpty;
                   };
                 } TSemaHub_almostEmpty;
    typedef union  TSemaHub_almostFull
          { UNSG32 u32[1];
            struct {
            struct w32SemaHub_almostFull;
                   };
                 } TSemaHub_almostFull;

    ///////////////////////////////////////////////////////////
     SIGN32 SemaHub_drvrd(SIE_SemaHub *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 SemaHub_drvwr(SIE_SemaHub *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void SemaHub_reset(SIE_SemaHub *p);
     SIGN32 SemaHub_cmp  (SIE_SemaHub *p, SIE_SemaHub *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define SemaHub_check(p,pie,pfx,hLOG) SemaHub_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define SemaHub_print(p,    pfx,hLOG) SemaHub_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_FiFo
#define h_FiFo (){}

    #define     RA_FiFo_CFG                                    0x0000

    #define     BA_FiFo_CFG_BASE                               0x0000
    #define     B16FiFo_CFG_BASE                               0x0000
    #define   LSb32FiFo_CFG_BASE                                  0
    #define   LSb16FiFo_CFG_BASE                                  0
    #define       bFiFo_CFG_BASE                               20
    #define   MSK32FiFo_CFG_BASE                                  0x000FFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_FiFo_START                                  0x0004

    #define     BA_FiFo_START_EN                               0x0004
    #define     B16FiFo_START_EN                               0x0004
    #define   LSb32FiFo_START_EN                                  0
    #define   LSb16FiFo_START_EN                                  0
    #define       bFiFo_START_EN                               1
    #define   MSK32FiFo_START_EN                                  0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_FiFo_CLEAR                                  0x0008

    #define     BA_FiFo_CLEAR_EN                               0x0008
    #define     B16FiFo_CLEAR_EN                               0x0008
    #define   LSb32FiFo_CLEAR_EN                                  0
    #define   LSb16FiFo_CLEAR_EN                                  0
    #define       bFiFo_CLEAR_EN                               1
    #define   MSK32FiFo_CLEAR_EN                                  0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_FiFo_FLUSH                                  0x000C

    #define     BA_FiFo_FLUSH_EN                               0x000C
    #define     B16FiFo_FLUSH_EN                               0x000C
    #define   LSb32FiFo_FLUSH_EN                                  0
    #define   LSb16FiFo_FLUSH_EN                                  0
    #define       bFiFo_FLUSH_EN                               1
    #define   MSK32FiFo_FLUSH_EN                                  0x00000001
    ///////////////////////////////////////////////////////////

    typedef struct SIE_FiFo {
    ///////////////////////////////////////////////////////////
    #define   GET32FiFo_CFG_BASE(r32)                          _BFGET_(r32,19, 0)
    #define   SET32FiFo_CFG_BASE(r32,v)                        _BFSET_(r32,19, 0,v)

    #define     w32FiFo_CFG                                    {\
            UNSG32 uCFG_BASE                                   : 20;\
            UNSG32 RSVDx0_b20                                  : 12;\
          }
    union { UNSG32 u32FiFo_CFG;
            struct w32FiFo_CFG;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FiFo_START_EN(r32)                          _BFGET_(r32, 0, 0)
    #define   SET32FiFo_START_EN(r32,v)                        _BFSET_(r32, 0, 0,v)
    #define   GET16FiFo_START_EN(r16)                          _BFGET_(r16, 0, 0)
    #define   SET16FiFo_START_EN(r16,v)                        _BFSET_(r16, 0, 0,v)

    #define     w32FiFo_START                                  {\
            UNSG32 uSTART_EN                                   :  1;\
            UNSG32 RSVDx4_b1                                   : 31;\
          }
    union { UNSG32 u32FiFo_START;
            struct w32FiFo_START;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FiFo_CLEAR_EN(r32)                          _BFGET_(r32, 0, 0)
    #define   SET32FiFo_CLEAR_EN(r32,v)                        _BFSET_(r32, 0, 0,v)
    #define   GET16FiFo_CLEAR_EN(r16)                          _BFGET_(r16, 0, 0)
    #define   SET16FiFo_CLEAR_EN(r16,v)                        _BFSET_(r16, 0, 0,v)

    #define     w32FiFo_CLEAR                                  {\
            UNSG32 uCLEAR_EN                                   :  1;\
            UNSG32 RSVDx8_b1                                   : 31;\
          }
    union { UNSG32 u32FiFo_CLEAR;
            struct w32FiFo_CLEAR;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32FiFo_FLUSH_EN(r32)                          _BFGET_(r32, 0, 0)
    #define   SET32FiFo_FLUSH_EN(r32,v)                        _BFSET_(r32, 0, 0,v)
    #define   GET16FiFo_FLUSH_EN(r16)                          _BFGET_(r16, 0, 0)
    #define   SET16FiFo_FLUSH_EN(r16,v)                        _BFSET_(r16, 0, 0,v)

    #define     w32FiFo_FLUSH                                  {\
            UNSG32 uFLUSH_EN                                   :  1;\
            UNSG32 RSVDxC_b1                                   : 31;\
          }
    union { UNSG32 u32FiFo_FLUSH;
            struct w32FiFo_FLUSH;
          };
    ///////////////////////////////////////////////////////////
    } SIE_FiFo;

    typedef union  T32FiFo_CFG
          { UNSG32 u32;
            struct w32FiFo_CFG;
                 } T32FiFo_CFG;
    typedef union  T32FiFo_START
          { UNSG32 u32;
            struct w32FiFo_START;
                 } T32FiFo_START;
    typedef union  T32FiFo_CLEAR
          { UNSG32 u32;
            struct w32FiFo_CLEAR;
                 } T32FiFo_CLEAR;
    typedef union  T32FiFo_FLUSH
          { UNSG32 u32;
            struct w32FiFo_FLUSH;
                 } T32FiFo_FLUSH;
    ///////////////////////////////////////////////////////////

    typedef union  TFiFo_CFG
          { UNSG32 u32[1];
            struct {
            struct w32FiFo_CFG;
                   };
                 } TFiFo_CFG;
    typedef union  TFiFo_START
          { UNSG32 u32[1];
            struct {
            struct w32FiFo_START;
                   };
                 } TFiFo_START;
    typedef union  TFiFo_CLEAR
          { UNSG32 u32[1];
            struct {
            struct w32FiFo_CLEAR;
                   };
                 } TFiFo_CLEAR;
    typedef union  TFiFo_FLUSH
          { UNSG32 u32[1];
            struct {
            struct w32FiFo_FLUSH;
                   };
                 } TFiFo_FLUSH;

    ///////////////////////////////////////////////////////////
     SIGN32 FiFo_drvrd(SIE_FiFo *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 FiFo_drvwr(SIE_FiFo *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void FiFo_reset(SIE_FiFo *p);
     SIGN32 FiFo_cmp  (SIE_FiFo *p, SIE_FiFo *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define FiFo_check(p,pie,pfx,hLOG) FiFo_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define FiFo_print(p,    pfx,hLOG) FiFo_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_HBO
#define h_HBO (){}

    #define     RA_HBO_FiFoCtl                                 0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_HBO_ARR                                     0x0400
    #define     RA_HBO_FiFo                                    0x0400
    ///////////////////////////////////////////////////////////
    #define     RA_HBO_BUSY                                    0x0600

    #define     BA_HBO_BUSY_ST                                 0x0600
    #define     B16HBO_BUSY_ST                                 0x0600
    #define   LSb32HBO_BUSY_ST                                    0
    #define   LSb16HBO_BUSY_ST                                    0
    #define       bHBO_BUSY_ST                                 32
    #define   MSK32HBO_BUSY_ST                                    0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_HBO {
    ///////////////////////////////////////////////////////////
              SIE_SemaHub                                      ie_FiFoCtl;
    ///////////////////////////////////////////////////////////
              SIE_FiFo                                         ie_FiFo[32];
    ///////////////////////////////////////////////////////////
    #define   GET32HBO_BUSY_ST(r32)                            _BFGET_(r32,31, 0)
    #define   SET32HBO_BUSY_ST(r32,v)                          _BFSET_(r32,31, 0,v)

    #define     w32HBO_BUSY                                    {\
            UNSG32 uBUSY_ST                                    : 32;\
          }
    union { UNSG32 u32HBO_BUSY;
            struct w32HBO_BUSY;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx604                                    [252];
    ///////////////////////////////////////////////////////////
    } SIE_HBO;

    typedef union  T32HBO_BUSY
          { UNSG32 u32;
            struct w32HBO_BUSY;
                 } T32HBO_BUSY;
    ///////////////////////////////////////////////////////////

    typedef union  THBO_BUSY
          { UNSG32 u32[1];
            struct {
            struct w32HBO_BUSY;
                   };
                 } THBO_BUSY;

    ///////////////////////////////////////////////////////////
     SIGN32 HBO_drvrd(SIE_HBO *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 HBO_drvwr(SIE_HBO *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void HBO_reset(SIE_HBO *p);
     SIGN32 HBO_cmp  (SIE_HBO *p, SIE_HBO *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define HBO_check(p,pie,pfx,hLOG) HBO_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define HBO_print(p,    pfx,hLOG) HBO_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_LLDesFmt
#define h_LLDesFmt (){}

    #define     RA_LLDesFmt_mem                                0x0000

    #define     BA_LLDesFmt_mem_size                           0x0000
    #define     B16LLDesFmt_mem_size                           0x0000
    #define   LSb32LLDesFmt_mem_size                              0
    #define   LSb16LLDesFmt_mem_size                              0
    #define       bLLDesFmt_mem_size                           16
    #define   MSK32LLDesFmt_mem_size                              0x0000FFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_LLDesFmt {
    ///////////////////////////////////////////////////////////
    #define   GET32LLDesFmt_mem_size(r32)                      _BFGET_(r32,15, 0)
    #define   SET32LLDesFmt_mem_size(r32,v)                    _BFSET_(r32,15, 0,v)
    #define   GET16LLDesFmt_mem_size(r16)                      _BFGET_(r16,15, 0)
    #define   SET16LLDesFmt_mem_size(r16,v)                    _BFSET_(r16,15, 0,v)

    #define     w32LLDesFmt_mem                                {\
            UNSG32 umem_size                                   : 16;\
            UNSG32 RSVDx0_b16                                  : 16;\
          }
    union { UNSG32 u32LLDesFmt_mem;
            struct w32LLDesFmt_mem;
          };
    ///////////////////////////////////////////////////////////
    } SIE_LLDesFmt;

    typedef union  T32LLDesFmt_mem
          { UNSG32 u32;
            struct w32LLDesFmt_mem;
                 } T32LLDesFmt_mem;
    ///////////////////////////////////////////////////////////

    typedef union  TLLDesFmt_mem
          { UNSG32 u32[1];
            struct {
            struct w32LLDesFmt_mem;
                   };
                 } TLLDesFmt_mem;

    ///////////////////////////////////////////////////////////
     SIGN32 LLDesFmt_drvrd(SIE_LLDesFmt *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 LLDesFmt_drvwr(SIE_LLDesFmt *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void LLDesFmt_reset(SIE_LLDesFmt *p);
     SIGN32 LLDesFmt_cmp  (SIE_LLDesFmt *p, SIE_LLDesFmt *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define LLDesFmt_check(p,pie,pfx,hLOG) LLDesFmt_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define LLDesFmt_print(p,    pfx,hLOG) LLDesFmt_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_dHubCmdHDR
#define h_dHubCmdHDR (){}

    #define     RA_dHubCmdHDR_DESC                             0x0000

    #define     BA_dHubCmdHDR_DESC_size                        0x0000
    #define     B16dHubCmdHDR_DESC_size                        0x0000
    #define   LSb32dHubCmdHDR_DESC_size                           0
    #define   LSb16dHubCmdHDR_DESC_size                           0
    #define       bdHubCmdHDR_DESC_size                        16
    #define   MSK32dHubCmdHDR_DESC_size                           0x0000FFFF

    #define     BA_dHubCmdHDR_DESC_sizeMTU                     0x0002
    #define     B16dHubCmdHDR_DESC_sizeMTU                     0x0002
    #define   LSb32dHubCmdHDR_DESC_sizeMTU                        16
    #define   LSb16dHubCmdHDR_DESC_sizeMTU                        0
    #define       bdHubCmdHDR_DESC_sizeMTU                     1
    #define   MSK32dHubCmdHDR_DESC_sizeMTU                        0x00010000

    #define     BA_dHubCmdHDR_DESC_semOpMTU                    0x0002
    #define     B16dHubCmdHDR_DESC_semOpMTU                    0x0002
    #define   LSb32dHubCmdHDR_DESC_semOpMTU                       17
    #define   LSb16dHubCmdHDR_DESC_semOpMTU                       1
    #define       bdHubCmdHDR_DESC_semOpMTU                    1
    #define   MSK32dHubCmdHDR_DESC_semOpMTU                       0x00020000

    #define     BA_dHubCmdHDR_DESC_chkSemId                    0x0002
    #define     B16dHubCmdHDR_DESC_chkSemId                    0x0002
    #define   LSb32dHubCmdHDR_DESC_chkSemId                       18
    #define   LSb16dHubCmdHDR_DESC_chkSemId                       2
    #define       bdHubCmdHDR_DESC_chkSemId                    5
    #define   MSK32dHubCmdHDR_DESC_chkSemId                       0x007C0000

    #define     BA_dHubCmdHDR_DESC_updSemId                    0x0002
    #define     B16dHubCmdHDR_DESC_updSemId                    0x0002
    #define   LSb32dHubCmdHDR_DESC_updSemId                       23
    #define   LSb16dHubCmdHDR_DESC_updSemId                       7
    #define       bdHubCmdHDR_DESC_updSemId                    5
    #define   MSK32dHubCmdHDR_DESC_updSemId                       0x0F800000

    #define     BA_dHubCmdHDR_DESC_interrupt                   0x0003
    #define     B16dHubCmdHDR_DESC_interrupt                   0x0002
    #define   LSb32dHubCmdHDR_DESC_interrupt                      28
    #define   LSb16dHubCmdHDR_DESC_interrupt                      12
    #define       bdHubCmdHDR_DESC_interrupt                   1
    #define   MSK32dHubCmdHDR_DESC_interrupt                      0x10000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_dHubCmdHDR {
    ///////////////////////////////////////////////////////////
    #define   GET32dHubCmdHDR_DESC_size(r32)                   _BFGET_(r32,15, 0)
    #define   SET32dHubCmdHDR_DESC_size(r32,v)                 _BFSET_(r32,15, 0,v)
    #define   GET16dHubCmdHDR_DESC_size(r16)                   _BFGET_(r16,15, 0)
    #define   SET16dHubCmdHDR_DESC_size(r16,v)                 _BFSET_(r16,15, 0,v)

    #define   GET32dHubCmdHDR_DESC_sizeMTU(r32)                _BFGET_(r32,16,16)
    #define   SET32dHubCmdHDR_DESC_sizeMTU(r32,v)              _BFSET_(r32,16,16,v)
    #define   GET16dHubCmdHDR_DESC_sizeMTU(r16)                _BFGET_(r16, 0, 0)
    #define   SET16dHubCmdHDR_DESC_sizeMTU(r16,v)              _BFSET_(r16, 0, 0,v)

    #define   GET32dHubCmdHDR_DESC_semOpMTU(r32)               _BFGET_(r32,17,17)
    #define   SET32dHubCmdHDR_DESC_semOpMTU(r32,v)             _BFSET_(r32,17,17,v)
    #define   GET16dHubCmdHDR_DESC_semOpMTU(r16)               _BFGET_(r16, 1, 1)
    #define   SET16dHubCmdHDR_DESC_semOpMTU(r16,v)             _BFSET_(r16, 1, 1,v)

    #define   GET32dHubCmdHDR_DESC_chkSemId(r32)               _BFGET_(r32,22,18)
    #define   SET32dHubCmdHDR_DESC_chkSemId(r32,v)             _BFSET_(r32,22,18,v)
    #define   GET16dHubCmdHDR_DESC_chkSemId(r16)               _BFGET_(r16, 6, 2)
    #define   SET16dHubCmdHDR_DESC_chkSemId(r16,v)             _BFSET_(r16, 6, 2,v)

    #define   GET32dHubCmdHDR_DESC_updSemId(r32)               _BFGET_(r32,27,23)
    #define   SET32dHubCmdHDR_DESC_updSemId(r32,v)             _BFSET_(r32,27,23,v)
    #define   GET16dHubCmdHDR_DESC_updSemId(r16)               _BFGET_(r16,11, 7)
    #define   SET16dHubCmdHDR_DESC_updSemId(r16,v)             _BFSET_(r16,11, 7,v)

    #define   GET32dHubCmdHDR_DESC_interrupt(r32)              _BFGET_(r32,28,28)
    #define   SET32dHubCmdHDR_DESC_interrupt(r32,v)            _BFSET_(r32,28,28,v)
    #define   GET16dHubCmdHDR_DESC_interrupt(r16)              _BFGET_(r16,12,12)
    #define   SET16dHubCmdHDR_DESC_interrupt(r16,v)            _BFSET_(r16,12,12,v)

    #define     w32dHubCmdHDR_DESC                             {\
            UNSG32 uDESC_size                                  : 16;\
            UNSG32 uDESC_sizeMTU                               :  1;\
            UNSG32 uDESC_semOpMTU                              :  1;\
            UNSG32 uDESC_chkSemId                              :  5;\
            UNSG32 uDESC_updSemId                              :  5;\
            UNSG32 uDESC_interrupt                             :  1;\
            UNSG32 RSVDx0_b29                                  :  3;\
          }
    union { UNSG32 u32dHubCmdHDR_DESC;
            struct w32dHubCmdHDR_DESC;
          };
    ///////////////////////////////////////////////////////////
    } SIE_dHubCmdHDR;

    typedef union  T32dHubCmdHDR_DESC
          { UNSG32 u32;
            struct w32dHubCmdHDR_DESC;
                 } T32dHubCmdHDR_DESC;
    ///////////////////////////////////////////////////////////

    typedef union  TdHubCmdHDR_DESC
          { UNSG32 u32[1];
            struct {
            struct w32dHubCmdHDR_DESC;
                   };
                 } TdHubCmdHDR_DESC;

    ///////////////////////////////////////////////////////////
     SIGN32 dHubCmdHDR_drvrd(SIE_dHubCmdHDR *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 dHubCmdHDR_drvwr(SIE_dHubCmdHDR *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void dHubCmdHDR_reset(SIE_dHubCmdHDR *p);
     SIGN32 dHubCmdHDR_cmp  (SIE_dHubCmdHDR *p, SIE_dHubCmdHDR *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define dHubCmdHDR_check(p,pie,pfx,hLOG) dHubCmdHDR_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define dHubCmdHDR_print(p,    pfx,hLOG) dHubCmdHDR_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_dHubCmd
#define h_dHubCmd (){}

    #define     RA_dHubCmd_MEM                                 0x0000

    #define     BA_dHubCmd_MEM_addr                            0x0000
    #define     B16dHubCmd_MEM_addr                            0x0000
    #define   LSb32dHubCmd_MEM_addr                               0
    #define   LSb16dHubCmd_MEM_addr                               0
    #define       bdHubCmd_MEM_addr                            32
    #define   MSK32dHubCmd_MEM_addr                               0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_dHubCmd_HDR                                 0x0004
    ///////////////////////////////////////////////////////////

    typedef struct SIE_dHubCmd {
    ///////////////////////////////////////////////////////////
    #define   GET32dHubCmd_MEM_addr(r32)                       _BFGET_(r32,31, 0)
    #define   SET32dHubCmd_MEM_addr(r32,v)                     _BFSET_(r32,31, 0,v)

    #define     w32dHubCmd_MEM                                 {\
            UNSG32 uMEM_addr                                   : 32;\
          }
    union { UNSG32 u32dHubCmd_MEM;
            struct w32dHubCmd_MEM;
          };
    ///////////////////////////////////////////////////////////
              SIE_dHubCmdHDR                                   ie_HDR;
    ///////////////////////////////////////////////////////////
    } SIE_dHubCmd;

    typedef union  T32dHubCmd_MEM
          { UNSG32 u32;
            struct w32dHubCmd_MEM;
                 } T32dHubCmd_MEM;
    ///////////////////////////////////////////////////////////

    typedef union  TdHubCmd_MEM
          { UNSG32 u32[1];
            struct {
            struct w32dHubCmd_MEM;
                   };
                 } TdHubCmd_MEM;

    ///////////////////////////////////////////////////////////
     SIGN32 dHubCmd_drvrd(SIE_dHubCmd *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 dHubCmd_drvwr(SIE_dHubCmd *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void dHubCmd_reset(SIE_dHubCmd *p);
     SIGN32 dHubCmd_cmp  (SIE_dHubCmd *p, SIE_dHubCmd *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define dHubCmd_check(p,pie,pfx,hLOG) dHubCmd_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define dHubCmd_print(p,    pfx,hLOG) dHubCmd_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_dHubChannel
#define h_dHubChannel (){}

    #define     RA_dHubChannel_CFG                             0x0000

    #define     BA_dHubChannel_CFG_MTU                         0x0000
    #define     B16dHubChannel_CFG_MTU                         0x0000
    #define   LSb32dHubChannel_CFG_MTU                            0
    #define   LSb16dHubChannel_CFG_MTU                            0
    #define       bdHubChannel_CFG_MTU                         2
    #define   MSK32dHubChannel_CFG_MTU                            0x00000003
    #define        dHubChannel_CFG_MTU_8byte                                0x0
    #define        dHubChannel_CFG_MTU_32byte                               0x1
    #define        dHubChannel_CFG_MTU_128byte                              0x2
    #define        dHubChannel_CFG_MTU_1024byte                             0x3

    #define     BA_dHubChannel_CFG_QoS                         0x0000
    #define     B16dHubChannel_CFG_QoS                         0x0000
    #define   LSb32dHubChannel_CFG_QoS                            2
    #define   LSb16dHubChannel_CFG_QoS                            2
    #define       bdHubChannel_CFG_QoS                         1
    #define   MSK32dHubChannel_CFG_QoS                            0x00000004

    #define     BA_dHubChannel_CFG_selfLoop                    0x0000
    #define     B16dHubChannel_CFG_selfLoop                    0x0000
    #define   LSb32dHubChannel_CFG_selfLoop                       3
    #define   LSb16dHubChannel_CFG_selfLoop                       3
    #define       bdHubChannel_CFG_selfLoop                    1
    #define   MSK32dHubChannel_CFG_selfLoop                       0x00000008

    #define     BA_dHubChannel_CFG_intrCtl                     0x0000
    #define     B16dHubChannel_CFG_intrCtl                     0x0000
    #define   LSb32dHubChannel_CFG_intrCtl                        4
    #define   LSb16dHubChannel_CFG_intrCtl                        4
    #define       bdHubChannel_CFG_intrCtl                     1
    #define   MSK32dHubChannel_CFG_intrCtl                        0x00000010
    #define        dHubChannel_CFG_intrCtl_cmdDone                          0x0
    #define        dHubChannel_CFG_intrCtl_chIdle                           0x1
    ///////////////////////////////////////////////////////////
    #define     RA_dHubChannel_START                           0x0004

    #define     BA_dHubChannel_START_EN                        0x0004
    #define     B16dHubChannel_START_EN                        0x0004
    #define   LSb32dHubChannel_START_EN                           0
    #define   LSb16dHubChannel_START_EN                           0
    #define       bdHubChannel_START_EN                        1
    #define   MSK32dHubChannel_START_EN                           0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_dHubChannel_CLEAR                           0x0008

    #define     BA_dHubChannel_CLEAR_EN                        0x0008
    #define     B16dHubChannel_CLEAR_EN                        0x0008
    #define   LSb32dHubChannel_CLEAR_EN                           0
    #define   LSb16dHubChannel_CLEAR_EN                           0
    #define       bdHubChannel_CLEAR_EN                        1
    #define   MSK32dHubChannel_CLEAR_EN                           0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_dHubChannel_FLUSH                           0x000C

    #define     BA_dHubChannel_FLUSH_EN                        0x000C
    #define     B16dHubChannel_FLUSH_EN                        0x000C
    #define   LSb32dHubChannel_FLUSH_EN                           0
    #define   LSb16dHubChannel_FLUSH_EN                           0
    #define       bdHubChannel_FLUSH_EN                        1
    #define   MSK32dHubChannel_FLUSH_EN                           0x00000001
    ///////////////////////////////////////////////////////////

    typedef struct SIE_dHubChannel {
    ///////////////////////////////////////////////////////////
    #define   GET32dHubChannel_CFG_MTU(r32)                    _BFGET_(r32, 1, 0)
    #define   SET32dHubChannel_CFG_MTU(r32,v)                  _BFSET_(r32, 1, 0,v)
    #define   GET16dHubChannel_CFG_MTU(r16)                    _BFGET_(r16, 1, 0)
    #define   SET16dHubChannel_CFG_MTU(r16,v)                  _BFSET_(r16, 1, 0,v)

    #define   GET32dHubChannel_CFG_QoS(r32)                    _BFGET_(r32, 2, 2)
    #define   SET32dHubChannel_CFG_QoS(r32,v)                  _BFSET_(r32, 2, 2,v)
    #define   GET16dHubChannel_CFG_QoS(r16)                    _BFGET_(r16, 2, 2)
    #define   SET16dHubChannel_CFG_QoS(r16,v)                  _BFSET_(r16, 2, 2,v)

    #define   GET32dHubChannel_CFG_selfLoop(r32)               _BFGET_(r32, 3, 3)
    #define   SET32dHubChannel_CFG_selfLoop(r32,v)             _BFSET_(r32, 3, 3,v)
    #define   GET16dHubChannel_CFG_selfLoop(r16)               _BFGET_(r16, 3, 3)
    #define   SET16dHubChannel_CFG_selfLoop(r16,v)             _BFSET_(r16, 3, 3,v)

    #define   GET32dHubChannel_CFG_intrCtl(r32)                _BFGET_(r32, 4, 4)
    #define   SET32dHubChannel_CFG_intrCtl(r32,v)              _BFSET_(r32, 4, 4,v)
    #define   GET16dHubChannel_CFG_intrCtl(r16)                _BFGET_(r16, 4, 4)
    #define   SET16dHubChannel_CFG_intrCtl(r16,v)              _BFSET_(r16, 4, 4,v)

    #define     w32dHubChannel_CFG                             {\
            UNSG32 uCFG_MTU                                    :  2;\
            UNSG32 uCFG_QoS                                    :  1;\
            UNSG32 uCFG_selfLoop                               :  1;\
            UNSG32 uCFG_intrCtl                                :  1;\
            UNSG32 RSVDx0_b5                                   : 27;\
          }
    union { UNSG32 u32dHubChannel_CFG;
            struct w32dHubChannel_CFG;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32dHubChannel_START_EN(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32dHubChannel_START_EN(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16dHubChannel_START_EN(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16dHubChannel_START_EN(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define     w32dHubChannel_START                           {\
            UNSG32 uSTART_EN                                   :  1;\
            UNSG32 RSVDx4_b1                                   : 31;\
          }
    union { UNSG32 u32dHubChannel_START;
            struct w32dHubChannel_START;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32dHubChannel_CLEAR_EN(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32dHubChannel_CLEAR_EN(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16dHubChannel_CLEAR_EN(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16dHubChannel_CLEAR_EN(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define     w32dHubChannel_CLEAR                           {\
            UNSG32 uCLEAR_EN                                   :  1;\
            UNSG32 RSVDx8_b1                                   : 31;\
          }
    union { UNSG32 u32dHubChannel_CLEAR;
            struct w32dHubChannel_CLEAR;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32dHubChannel_FLUSH_EN(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32dHubChannel_FLUSH_EN(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16dHubChannel_FLUSH_EN(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16dHubChannel_FLUSH_EN(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define     w32dHubChannel_FLUSH                           {\
            UNSG32 uFLUSH_EN                                   :  1;\
            UNSG32 RSVDxC_b1                                   : 31;\
          }
    union { UNSG32 u32dHubChannel_FLUSH;
            struct w32dHubChannel_FLUSH;
          };
    ///////////////////////////////////////////////////////////
    } SIE_dHubChannel;

    typedef union  T32dHubChannel_CFG
          { UNSG32 u32;
            struct w32dHubChannel_CFG;
                 } T32dHubChannel_CFG;
    typedef union  T32dHubChannel_START
          { UNSG32 u32;
            struct w32dHubChannel_START;
                 } T32dHubChannel_START;
    typedef union  T32dHubChannel_CLEAR
          { UNSG32 u32;
            struct w32dHubChannel_CLEAR;
                 } T32dHubChannel_CLEAR;
    typedef union  T32dHubChannel_FLUSH
          { UNSG32 u32;
            struct w32dHubChannel_FLUSH;
                 } T32dHubChannel_FLUSH;
    ///////////////////////////////////////////////////////////

    typedef union  TdHubChannel_CFG
          { UNSG32 u32[1];
            struct {
            struct w32dHubChannel_CFG;
                   };
                 } TdHubChannel_CFG;
    typedef union  TdHubChannel_START
          { UNSG32 u32[1];
            struct {
            struct w32dHubChannel_START;
                   };
                 } TdHubChannel_START;
    typedef union  TdHubChannel_CLEAR
          { UNSG32 u32[1];
            struct {
            struct w32dHubChannel_CLEAR;
                   };
                 } TdHubChannel_CLEAR;
    typedef union  TdHubChannel_FLUSH
          { UNSG32 u32[1];
            struct {
            struct w32dHubChannel_FLUSH;
                   };
                 } TdHubChannel_FLUSH;

    ///////////////////////////////////////////////////////////
     SIGN32 dHubChannel_drvrd(SIE_dHubChannel *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 dHubChannel_drvwr(SIE_dHubChannel *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void dHubChannel_reset(SIE_dHubChannel *p);
     SIGN32 dHubChannel_cmp  (SIE_dHubChannel *p, SIE_dHubChannel *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define dHubChannel_check(p,pie,pfx,hLOG) dHubChannel_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define dHubChannel_print(p,    pfx,hLOG) dHubChannel_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_dHubReg
#define h_dHubReg (){}

    #define     RA_dHubReg_SemaHub                             0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_dHubReg_HBO                                 0x0400
    ///////////////////////////////////////////////////////////
    #define     RA_dHubReg_ARR                                 0x0B00
    #define     RA_dHubReg_channelCtl                          0x0B00
    ///////////////////////////////////////////////////////////
    #define     RA_dHubReg_BUSY                                0x0C00

    #define     BA_dHubReg_BUSY_ST                             0x0C00
    #define     B16dHubReg_BUSY_ST                             0x0C00
    #define   LSb32dHubReg_BUSY_ST                                0
    #define   LSb16dHubReg_BUSY_ST                                0
    #define       bdHubReg_BUSY_ST                             16
    #define   MSK32dHubReg_BUSY_ST                                0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_dHubReg_PENDING                             0x0C04

    #define     BA_dHubReg_PENDING_ST                          0x0C04
    #define     B16dHubReg_PENDING_ST                          0x0C04
    #define   LSb32dHubReg_PENDING_ST                             0
    #define   LSb16dHubReg_PENDING_ST                             0
    #define       bdHubReg_PENDING_ST                          16
    #define   MSK32dHubReg_PENDING_ST                             0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_dHubReg_busRstEn                            0x0C08

    #define     BA_dHubReg_busRstEn_reg                        0x0C08
    #define     B16dHubReg_busRstEn_reg                        0x0C08
    #define   LSb32dHubReg_busRstEn_reg                           0
    #define   LSb16dHubReg_busRstEn_reg                           0
    #define       bdHubReg_busRstEn_reg                        1
    #define   MSK32dHubReg_busRstEn_reg                           0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_dHubReg_busRstDone                          0x0C0C

    #define     BA_dHubReg_busRstDone_reg                      0x0C0C
    #define     B16dHubReg_busRstDone_reg                      0x0C0C
    #define   LSb32dHubReg_busRstDone_reg                         0
    #define   LSb16dHubReg_busRstDone_reg                         0
    #define       bdHubReg_busRstDone_reg                      1
    #define   MSK32dHubReg_busRstDone_reg                         0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_dHubReg_flowCtl                             0x0C10

    #define     BA_dHubReg_flowCtl_rAlpha                      0x0C10
    #define     B16dHubReg_flowCtl_rAlpha                      0x0C10
    #define   LSb32dHubReg_flowCtl_rAlpha                         0
    #define   LSb16dHubReg_flowCtl_rAlpha                         0
    #define       bdHubReg_flowCtl_rAlpha                      8
    #define   MSK32dHubReg_flowCtl_rAlpha                         0x000000FF

    #define     BA_dHubReg_flowCtl_wAlpha                      0x0C11
    #define     B16dHubReg_flowCtl_wAlpha                      0x0C10
    #define   LSb32dHubReg_flowCtl_wAlpha                         8
    #define   LSb16dHubReg_flowCtl_wAlpha                         8
    #define       bdHubReg_flowCtl_wAlpha                      8
    #define   MSK32dHubReg_flowCtl_wAlpha                         0x0000FF00
    ///////////////////////////////////////////////////////////
    #define     RA_dHubReg_axiCmdCol                           0x0C14

    #define     BA_dHubReg_axiCmdCol_rCnt                      0x0C14
    #define     B16dHubReg_axiCmdCol_rCnt                      0x0C14
    #define   LSb32dHubReg_axiCmdCol_rCnt                         0
    #define   LSb16dHubReg_axiCmdCol_rCnt                         0
    #define       bdHubReg_axiCmdCol_rCnt                      16
    #define   MSK32dHubReg_axiCmdCol_rCnt                         0x0000FFFF

    #define     BA_dHubReg_axiCmdCol_wCnt                      0x0C16
    #define     B16dHubReg_axiCmdCol_wCnt                      0x0C16
    #define   LSb32dHubReg_axiCmdCol_wCnt                         16
    #define   LSb16dHubReg_axiCmdCol_wCnt                         0
    #define       bdHubReg_axiCmdCol_wCnt                      16
    #define   MSK32dHubReg_axiCmdCol_wCnt                         0xFFFF0000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_dHubReg {
    ///////////////////////////////////////////////////////////
              SIE_SemaHub                                      ie_SemaHub;
    ///////////////////////////////////////////////////////////
              SIE_HBO                                          ie_HBO;
    ///////////////////////////////////////////////////////////
              SIE_dHubChannel                                  ie_channelCtl[16];
    ///////////////////////////////////////////////////////////
    #define   GET32dHubReg_BUSY_ST(r32)                        _BFGET_(r32,15, 0)
    #define   SET32dHubReg_BUSY_ST(r32,v)                      _BFSET_(r32,15, 0,v)
    #define   GET16dHubReg_BUSY_ST(r16)                        _BFGET_(r16,15, 0)
    #define   SET16dHubReg_BUSY_ST(r16,v)                      _BFSET_(r16,15, 0,v)

    #define     w32dHubReg_BUSY                                {\
            UNSG32 uBUSY_ST                                    : 16;\
            UNSG32 RSVDxC00_b16                                : 16;\
          }
    union { UNSG32 u32dHubReg_BUSY;
            struct w32dHubReg_BUSY;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32dHubReg_PENDING_ST(r32)                     _BFGET_(r32,15, 0)
    #define   SET32dHubReg_PENDING_ST(r32,v)                   _BFSET_(r32,15, 0,v)
    #define   GET16dHubReg_PENDING_ST(r16)                     _BFGET_(r16,15, 0)
    #define   SET16dHubReg_PENDING_ST(r16,v)                   _BFSET_(r16,15, 0,v)

    #define     w32dHubReg_PENDING                             {\
            UNSG32 uPENDING_ST                                 : 16;\
            UNSG32 RSVDxC04_b16                                : 16;\
          }
    union { UNSG32 u32dHubReg_PENDING;
            struct w32dHubReg_PENDING;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32dHubReg_busRstEn_reg(r32)                   _BFGET_(r32, 0, 0)
    #define   SET32dHubReg_busRstEn_reg(r32,v)                 _BFSET_(r32, 0, 0,v)
    #define   GET16dHubReg_busRstEn_reg(r16)                   _BFGET_(r16, 0, 0)
    #define   SET16dHubReg_busRstEn_reg(r16,v)                 _BFSET_(r16, 0, 0,v)

    #define     w32dHubReg_busRstEn                            {\
            UNSG32 ubusRstEn_reg                               :  1;\
            UNSG32 RSVDxC08_b1                                 : 31;\
          }
    union { UNSG32 u32dHubReg_busRstEn;
            struct w32dHubReg_busRstEn;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32dHubReg_busRstDone_reg(r32)                 _BFGET_(r32, 0, 0)
    #define   SET32dHubReg_busRstDone_reg(r32,v)               _BFSET_(r32, 0, 0,v)
    #define   GET16dHubReg_busRstDone_reg(r16)                 _BFGET_(r16, 0, 0)
    #define   SET16dHubReg_busRstDone_reg(r16,v)               _BFSET_(r16, 0, 0,v)

    #define     w32dHubReg_busRstDone                          {\
            UNSG32 ubusRstDone_reg                             :  1;\
            UNSG32 RSVDxC0C_b1                                 : 31;\
          }
    union { UNSG32 u32dHubReg_busRstDone;
            struct w32dHubReg_busRstDone;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32dHubReg_flowCtl_rAlpha(r32)                 _BFGET_(r32, 7, 0)
    #define   SET32dHubReg_flowCtl_rAlpha(r32,v)               _BFSET_(r32, 7, 0,v)
    #define   GET16dHubReg_flowCtl_rAlpha(r16)                 _BFGET_(r16, 7, 0)
    #define   SET16dHubReg_flowCtl_rAlpha(r16,v)               _BFSET_(r16, 7, 0,v)

    #define   GET32dHubReg_flowCtl_wAlpha(r32)                 _BFGET_(r32,15, 8)
    #define   SET32dHubReg_flowCtl_wAlpha(r32,v)               _BFSET_(r32,15, 8,v)
    #define   GET16dHubReg_flowCtl_wAlpha(r16)                 _BFGET_(r16,15, 8)
    #define   SET16dHubReg_flowCtl_wAlpha(r16,v)               _BFSET_(r16,15, 8,v)

    #define     w32dHubReg_flowCtl                             {\
            UNSG32 uflowCtl_rAlpha                             :  8;\
            UNSG32 uflowCtl_wAlpha                             :  8;\
            UNSG32 RSVDxC10_b16                                : 16;\
          }
    union { UNSG32 u32dHubReg_flowCtl;
            struct w32dHubReg_flowCtl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32dHubReg_axiCmdCol_rCnt(r32)                 _BFGET_(r32,15, 0)
    #define   SET32dHubReg_axiCmdCol_rCnt(r32,v)               _BFSET_(r32,15, 0,v)
    #define   GET16dHubReg_axiCmdCol_rCnt(r16)                 _BFGET_(r16,15, 0)
    #define   SET16dHubReg_axiCmdCol_rCnt(r16,v)               _BFSET_(r16,15, 0,v)

    #define   GET32dHubReg_axiCmdCol_wCnt(r32)                 _BFGET_(r32,31,16)
    #define   SET32dHubReg_axiCmdCol_wCnt(r32,v)               _BFSET_(r32,31,16,v)
    #define   GET16dHubReg_axiCmdCol_wCnt(r16)                 _BFGET_(r16,15, 0)
    #define   SET16dHubReg_axiCmdCol_wCnt(r16,v)               _BFSET_(r16,15, 0,v)

    #define     w32dHubReg_axiCmdCol                           {\
            UNSG32 uaxiCmdCol_rCnt                             : 16;\
            UNSG32 uaxiCmdCol_wCnt                             : 16;\
          }
    union { UNSG32 u32dHubReg_axiCmdCol;
            struct w32dHubReg_axiCmdCol;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDxC18                                    [232];
    ///////////////////////////////////////////////////////////
    } SIE_dHubReg;

    typedef union  T32dHubReg_BUSY
          { UNSG32 u32;
            struct w32dHubReg_BUSY;
                 } T32dHubReg_BUSY;
    typedef union  T32dHubReg_PENDING
          { UNSG32 u32;
            struct w32dHubReg_PENDING;
                 } T32dHubReg_PENDING;
    typedef union  T32dHubReg_busRstEn
          { UNSG32 u32;
            struct w32dHubReg_busRstEn;
                 } T32dHubReg_busRstEn;
    typedef union  T32dHubReg_busRstDone
          { UNSG32 u32;
            struct w32dHubReg_busRstDone;
                 } T32dHubReg_busRstDone;
    typedef union  T32dHubReg_flowCtl
          { UNSG32 u32;
            struct w32dHubReg_flowCtl;
                 } T32dHubReg_flowCtl;
    typedef union  T32dHubReg_axiCmdCol
          { UNSG32 u32;
            struct w32dHubReg_axiCmdCol;
                 } T32dHubReg_axiCmdCol;
    ///////////////////////////////////////////////////////////

    typedef union  TdHubReg_BUSY
          { UNSG32 u32[1];
            struct {
            struct w32dHubReg_BUSY;
                   };
                 } TdHubReg_BUSY;
    typedef union  TdHubReg_PENDING
          { UNSG32 u32[1];
            struct {
            struct w32dHubReg_PENDING;
                   };
                 } TdHubReg_PENDING;
    typedef union  TdHubReg_busRstEn
          { UNSG32 u32[1];
            struct {
            struct w32dHubReg_busRstEn;
                   };
                 } TdHubReg_busRstEn;
    typedef union  TdHubReg_busRstDone
          { UNSG32 u32[1];
            struct {
            struct w32dHubReg_busRstDone;
                   };
                 } TdHubReg_busRstDone;
    typedef union  TdHubReg_flowCtl
          { UNSG32 u32[1];
            struct {
            struct w32dHubReg_flowCtl;
                   };
                 } TdHubReg_flowCtl;
    typedef union  TdHubReg_axiCmdCol
          { UNSG32 u32[1];
            struct {
            struct w32dHubReg_axiCmdCol;
                   };
                 } TdHubReg_axiCmdCol;

    ///////////////////////////////////////////////////////////
     SIGN32 dHubReg_drvrd(SIE_dHubReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 dHubReg_drvwr(SIE_dHubReg *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void dHubReg_reset(SIE_dHubReg *p);
     SIGN32 dHubReg_cmp  (SIE_dHubReg *p, SIE_dHubReg *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define dHubReg_check(p,pie,pfx,hLOG) dHubReg_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define dHubReg_print(p,    pfx,hLOG) dHubReg_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_dHubCmd2D
#define h_dHubCmd2D (){}

    #define     RA_dHubCmd2D_MEM                               0x0000

    #define     BA_dHubCmd2D_MEM_addr                          0x0000
    #define     B16dHubCmd2D_MEM_addr                          0x0000
    #define   LSb32dHubCmd2D_MEM_addr                             0
    #define   LSb16dHubCmd2D_MEM_addr                             0
    #define       bdHubCmd2D_MEM_addr                          32
    #define   MSK32dHubCmd2D_MEM_addr                             0xFFFFFFFF
    ///////////////////////////////////////////////////////////
    #define     RA_dHubCmd2D_DESC                              0x0004

    #define     BA_dHubCmd2D_DESC_stride                       0x0004
    #define     B16dHubCmd2D_DESC_stride                       0x0004
    #define   LSb32dHubCmd2D_DESC_stride                          0
    #define   LSb16dHubCmd2D_DESC_stride                          0
    #define       bdHubCmd2D_DESC_stride                       16
    #define   MSK32dHubCmd2D_DESC_stride                          0x0000FFFF

    #define     BA_dHubCmd2D_DESC_numLine                      0x0006
    #define     B16dHubCmd2D_DESC_numLine                      0x0006
    #define   LSb32dHubCmd2D_DESC_numLine                         16
    #define   LSb16dHubCmd2D_DESC_numLine                         0
    #define       bdHubCmd2D_DESC_numLine                      13
    #define   MSK32dHubCmd2D_DESC_numLine                         0x1FFF0000

    #define     BA_dHubCmd2D_DESC_hdrLoop                      0x0007
    #define     B16dHubCmd2D_DESC_hdrLoop                      0x0006
    #define   LSb32dHubCmd2D_DESC_hdrLoop                         29
    #define   LSb16dHubCmd2D_DESC_hdrLoop                         13
    #define       bdHubCmd2D_DESC_hdrLoop                      2
    #define   MSK32dHubCmd2D_DESC_hdrLoop                         0x60000000

    #define     BA_dHubCmd2D_DESC_interrupt                    0x0007
    #define     B16dHubCmd2D_DESC_interrupt                    0x0006
    #define   LSb32dHubCmd2D_DESC_interrupt                       31
    #define   LSb16dHubCmd2D_DESC_interrupt                       15
    #define       bdHubCmd2D_DESC_interrupt                    1
    #define   MSK32dHubCmd2D_DESC_interrupt                       0x80000000
    ///////////////////////////////////////////////////////////
    #define     RA_dHubCmd2D_START                             0x0008

    #define     BA_dHubCmd2D_START_EN                          0x0008
    #define     B16dHubCmd2D_START_EN                          0x0008
    #define   LSb32dHubCmd2D_START_EN                             0
    #define   LSb16dHubCmd2D_START_EN                             0
    #define       bdHubCmd2D_START_EN                          1
    #define   MSK32dHubCmd2D_START_EN                             0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_dHubCmd2D_CLEAR                             0x000C

    #define     BA_dHubCmd2D_CLEAR_EN                          0x000C
    #define     B16dHubCmd2D_CLEAR_EN                          0x000C
    #define   LSb32dHubCmd2D_CLEAR_EN                             0
    #define   LSb16dHubCmd2D_CLEAR_EN                             0
    #define       bdHubCmd2D_CLEAR_EN                          1
    #define   MSK32dHubCmd2D_CLEAR_EN                             0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_dHubCmd2D_HDR                               0x0010
    ///////////////////////////////////////////////////////////

    typedef struct SIE_dHubCmd2D {
    ///////////////////////////////////////////////////////////
    #define   GET32dHubCmd2D_MEM_addr(r32)                     _BFGET_(r32,31, 0)
    #define   SET32dHubCmd2D_MEM_addr(r32,v)                   _BFSET_(r32,31, 0,v)

    #define     w32dHubCmd2D_MEM                               {\
            UNSG32 uMEM_addr                                   : 32;\
          }
    union { UNSG32 u32dHubCmd2D_MEM;
            struct w32dHubCmd2D_MEM;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32dHubCmd2D_DESC_stride(r32)                  _BFGET_(r32,15, 0)
    #define   SET32dHubCmd2D_DESC_stride(r32,v)                _BFSET_(r32,15, 0,v)
    #define   GET16dHubCmd2D_DESC_stride(r16)                  _BFGET_(r16,15, 0)
    #define   SET16dHubCmd2D_DESC_stride(r16,v)                _BFSET_(r16,15, 0,v)

    #define   GET32dHubCmd2D_DESC_numLine(r32)                 _BFGET_(r32,28,16)
    #define   SET32dHubCmd2D_DESC_numLine(r32,v)               _BFSET_(r32,28,16,v)
    #define   GET16dHubCmd2D_DESC_numLine(r16)                 _BFGET_(r16,12, 0)
    #define   SET16dHubCmd2D_DESC_numLine(r16,v)               _BFSET_(r16,12, 0,v)

    #define   GET32dHubCmd2D_DESC_hdrLoop(r32)                 _BFGET_(r32,30,29)
    #define   SET32dHubCmd2D_DESC_hdrLoop(r32,v)               _BFSET_(r32,30,29,v)
    #define   GET16dHubCmd2D_DESC_hdrLoop(r16)                 _BFGET_(r16,14,13)
    #define   SET16dHubCmd2D_DESC_hdrLoop(r16,v)               _BFSET_(r16,14,13,v)

    #define   GET32dHubCmd2D_DESC_interrupt(r32)               _BFGET_(r32,31,31)
    #define   SET32dHubCmd2D_DESC_interrupt(r32,v)             _BFSET_(r32,31,31,v)
    #define   GET16dHubCmd2D_DESC_interrupt(r16)               _BFGET_(r16,15,15)
    #define   SET16dHubCmd2D_DESC_interrupt(r16,v)             _BFSET_(r16,15,15,v)

    #define     w32dHubCmd2D_DESC                              {\
            UNSG32 uDESC_stride                                : 16;\
            UNSG32 uDESC_numLine                               : 13;\
            UNSG32 uDESC_hdrLoop                               :  2;\
            UNSG32 uDESC_interrupt                             :  1;\
          }
    union { UNSG32 u32dHubCmd2D_DESC;
            struct w32dHubCmd2D_DESC;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32dHubCmd2D_START_EN(r32)                     _BFGET_(r32, 0, 0)
    #define   SET32dHubCmd2D_START_EN(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   GET16dHubCmd2D_START_EN(r16)                     _BFGET_(r16, 0, 0)
    #define   SET16dHubCmd2D_START_EN(r16,v)                   _BFSET_(r16, 0, 0,v)

    #define     w32dHubCmd2D_START                             {\
            UNSG32 uSTART_EN                                   :  1;\
            UNSG32 RSVDx8_b1                                   : 31;\
          }
    union { UNSG32 u32dHubCmd2D_START;
            struct w32dHubCmd2D_START;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32dHubCmd2D_CLEAR_EN(r32)                     _BFGET_(r32, 0, 0)
    #define   SET32dHubCmd2D_CLEAR_EN(r32,v)                   _BFSET_(r32, 0, 0,v)
    #define   GET16dHubCmd2D_CLEAR_EN(r16)                     _BFGET_(r16, 0, 0)
    #define   SET16dHubCmd2D_CLEAR_EN(r16,v)                   _BFSET_(r16, 0, 0,v)

    #define     w32dHubCmd2D_CLEAR                             {\
            UNSG32 uCLEAR_EN                                   :  1;\
            UNSG32 RSVDxC_b1                                   : 31;\
          }
    union { UNSG32 u32dHubCmd2D_CLEAR;
            struct w32dHubCmd2D_CLEAR;
          };
    ///////////////////////////////////////////////////////////
              SIE_dHubCmdHDR                                   ie_HDR[4];
    ///////////////////////////////////////////////////////////
    } SIE_dHubCmd2D;

    typedef union  T32dHubCmd2D_MEM
          { UNSG32 u32;
            struct w32dHubCmd2D_MEM;
                 } T32dHubCmd2D_MEM;
    typedef union  T32dHubCmd2D_DESC
          { UNSG32 u32;
            struct w32dHubCmd2D_DESC;
                 } T32dHubCmd2D_DESC;
    typedef union  T32dHubCmd2D_START
          { UNSG32 u32;
            struct w32dHubCmd2D_START;
                 } T32dHubCmd2D_START;
    typedef union  T32dHubCmd2D_CLEAR
          { UNSG32 u32;
            struct w32dHubCmd2D_CLEAR;
                 } T32dHubCmd2D_CLEAR;
    ///////////////////////////////////////////////////////////

    typedef union  TdHubCmd2D_MEM
          { UNSG32 u32[1];
            struct {
            struct w32dHubCmd2D_MEM;
                   };
                 } TdHubCmd2D_MEM;
    typedef union  TdHubCmd2D_DESC
          { UNSG32 u32[1];
            struct {
            struct w32dHubCmd2D_DESC;
                   };
                 } TdHubCmd2D_DESC;
    typedef union  TdHubCmd2D_START
          { UNSG32 u32[1];
            struct {
            struct w32dHubCmd2D_START;
                   };
                 } TdHubCmd2D_START;
    typedef union  TdHubCmd2D_CLEAR
          { UNSG32 u32[1];
            struct {
            struct w32dHubCmd2D_CLEAR;
                   };
                 } TdHubCmd2D_CLEAR;

    ///////////////////////////////////////////////////////////
     SIGN32 dHubCmd2D_drvrd(SIE_dHubCmd2D *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 dHubCmd2D_drvwr(SIE_dHubCmd2D *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void dHubCmd2D_reset(SIE_dHubCmd2D *p);
     SIGN32 dHubCmd2D_cmp  (SIE_dHubCmd2D *p, SIE_dHubCmd2D *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define dHubCmd2D_check(p,pie,pfx,hLOG) dHubCmd2D_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define dHubCmd2D_print(p,    pfx,hLOG) dHubCmd2D_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_dHubQuery
#define h_dHubQuery (){}

    #define     RA_dHubQuery_RESP                              0x0000

    #define     BA_dHubQuery_RESP_ST                           0x0000
    #define     B16dHubQuery_RESP_ST                           0x0000
    #define   LSb32dHubQuery_RESP_ST                              0
    #define   LSb16dHubQuery_RESP_ST                              0
    #define       bdHubQuery_RESP_ST                           16
    #define   MSK32dHubQuery_RESP_ST                              0x0000FFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_dHubQuery {
    ///////////////////////////////////////////////////////////
    #define   GET32dHubQuery_RESP_ST(r32)                      _BFGET_(r32,15, 0)
    #define   SET32dHubQuery_RESP_ST(r32,v)                    _BFSET_(r32,15, 0,v)
    #define   GET16dHubQuery_RESP_ST(r16)                      _BFGET_(r16,15, 0)
    #define   SET16dHubQuery_RESP_ST(r16,v)                    _BFSET_(r16,15, 0,v)

    #define     w32dHubQuery_RESP                              {\
            UNSG32 uRESP_ST                                    : 16;\
            UNSG32 RSVDx0_b16                                  : 16;\
          }
    union { UNSG32 u32dHubQuery_RESP;
            struct w32dHubQuery_RESP;
          };
    ///////////////////////////////////////////////////////////
    } SIE_dHubQuery;

    typedef union  T32dHubQuery_RESP
          { UNSG32 u32;
            struct w32dHubQuery_RESP;
                 } T32dHubQuery_RESP;
    ///////////////////////////////////////////////////////////

    typedef union  TdHubQuery_RESP
          { UNSG32 u32[1];
            struct {
            struct w32dHubQuery_RESP;
                   };
                 } TdHubQuery_RESP;

    ///////////////////////////////////////////////////////////
     SIGN32 dHubQuery_drvrd(SIE_dHubQuery *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 dHubQuery_drvwr(SIE_dHubQuery *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void dHubQuery_reset(SIE_dHubQuery *p);
     SIGN32 dHubQuery_cmp  (SIE_dHubQuery *p, SIE_dHubQuery *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define dHubQuery_check(p,pie,pfx,hLOG) dHubQuery_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define dHubQuery_print(p,    pfx,hLOG) dHubQuery_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_dHubReg2D
#define h_dHubReg2D (){}

    #define     RA_dHubReg2D_dHub                              0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_dHubReg2D_ARR                               0x0D00
    #define     RA_dHubReg2D_Cmd2D                             0x0D00
    ///////////////////////////////////////////////////////////
    #define     RA_dHubReg2D_BUSY                              0x0F00

    #define     BA_dHubReg2D_BUSY_ST                           0x0F00
    #define     B16dHubReg2D_BUSY_ST                           0x0F00
    #define   LSb32dHubReg2D_BUSY_ST                              0
    #define   LSb16dHubReg2D_BUSY_ST                              0
    #define       bdHubReg2D_BUSY_ST                           16
    #define   MSK32dHubReg2D_BUSY_ST                              0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_dHubReg2D_CH_ST                             0x0F40
    ///////////////////////////////////////////////////////////

    typedef struct SIE_dHubReg2D {
    ///////////////////////////////////////////////////////////
              SIE_dHubReg                                      ie_dHub;
    ///////////////////////////////////////////////////////////
              SIE_dHubCmd2D                                    ie_Cmd2D[16];
    ///////////////////////////////////////////////////////////
    #define   GET32dHubReg2D_BUSY_ST(r32)                      _BFGET_(r32,15, 0)
    #define   SET32dHubReg2D_BUSY_ST(r32,v)                    _BFSET_(r32,15, 0,v)
    #define   GET16dHubReg2D_BUSY_ST(r16)                      _BFGET_(r16,15, 0)
    #define   SET16dHubReg2D_BUSY_ST(r16,v)                    _BFSET_(r16,15, 0,v)

    #define     w32dHubReg2D_BUSY                              {\
            UNSG32 uBUSY_ST                                    : 16;\
            UNSG32 RSVDxF00_b16                                : 16;\
          }
    union { UNSG32 u32dHubReg2D_BUSY;
            struct w32dHubReg2D_BUSY;
          };
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDxF04                                    [60];
    ///////////////////////////////////////////////////////////
              SIE_dHubQuery                                    ie_CH_ST[16];
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDxF80                                    [128];
    ///////////////////////////////////////////////////////////
    } SIE_dHubReg2D;

    typedef union  T32dHubReg2D_BUSY
          { UNSG32 u32;
            struct w32dHubReg2D_BUSY;
                 } T32dHubReg2D_BUSY;
    ///////////////////////////////////////////////////////////

    typedef union  TdHubReg2D_BUSY
          { UNSG32 u32[1];
            struct {
            struct w32dHubReg2D_BUSY;
                   };
                 } TdHubReg2D_BUSY;

    ///////////////////////////////////////////////////////////
     SIGN32 dHubReg2D_drvrd(SIE_dHubReg2D *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 dHubReg2D_drvwr(SIE_dHubReg2D *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void dHubReg2D_reset(SIE_dHubReg2D *p);
     SIGN32 dHubReg2D_cmp  (SIE_dHubReg2D *p, SIE_dHubReg2D *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define dHubReg2D_check(p,pie,pfx,hLOG) dHubReg2D_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define dHubReg2D_print(p,    pfx,hLOG) dHubReg2D_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_BCMINSFMT
#define h_BCMINSFMT (){}

    #define     BA_BCMINSFMT_hdr                               0x0000
    #define     B16BCMINSFMT_hdr                               0x0000
    #define   LSb32BCMINSFMT_hdr                                  0
    #define   LSb16BCMINSFMT_hdr                                  0
    #define       bBCMINSFMT_hdr                               4
    #define   MSK32BCMINSFMT_hdr                                  0x0000000F
    #define        BCMINSFMT_hdr_CFGW                                       0x0
    #define        BCMINSFMT_hdr_LDFN                                       0x1
    #define        BCMINSFMT_hdr_RCMD                                       0x2
    #define        BCMINSFMT_hdr_WCMD                                       0x3
    #define        BCMINSFMT_hdr_RDAT                                       0x4
    #define        BCMINSFMT_hdr_WDAT                                       0x5
    #define        BCMINSFMT_hdr_SEMA                                       0x6
    #define        BCMINSFMT_hdr_NULL                                       0xF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_BCMINSFMT {
    ///////////////////////////////////////////////////////////
    #define   GET32BCMINSFMT_hdr(r32)                          _BFGET_(r32, 3, 0)
    #define   SET32BCMINSFMT_hdr(r32,v)                        _BFSET_(r32, 3, 0,v)
    #define   GET16BCMINSFMT_hdr(r16)                          _BFGET_(r16, 3, 0)
    #define   SET16BCMINSFMT_hdr(r16,v)                        _BFSET_(r16, 3, 0,v)

            UNSG32 u_hdr                                       :  4;
            UNSG32 RSVDx0_b4                                   : 28;
    ///////////////////////////////////////////////////////////
    } SIE_BCMINSFMT;

    ///////////////////////////////////////////////////////////
     SIGN32 BCMINSFMT_drvrd(SIE_BCMINSFMT *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BCMINSFMT_drvwr(SIE_BCMINSFMT *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BCMINSFMT_reset(SIE_BCMINSFMT *p);
     SIGN32 BCMINSFMT_cmp  (SIE_BCMINSFMT *p, SIE_BCMINSFMT *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BCMINSFMT_check(p,pie,pfx,hLOG) BCMINSFMT_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BCMINSFMT_print(p,    pfx,hLOG) BCMINSFMT_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_BCMCFGW
#define h_BCMCFGW (){}

    #define     BA_BCMCFGW_dat                                 0x0000
    #define     B16BCMCFGW_dat                                 0x0000
    #define   LSb32BCMCFGW_dat                                    0
    #define   LSb16BCMCFGW_dat                                    0
    #define       bBCMCFGW_dat                                 32
    #define   MSK32BCMCFGW_dat                                    0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    #define     BA_BCMCFGW_devAdr                              0x0004
    #define     B16BCMCFGW_devAdr                              0x0004
    #define   LSb32BCMCFGW_devAdr                                 0
    #define   LSb16BCMCFGW_devAdr                                 0
    #define       bBCMCFGW_devAdr                              28
    #define   MSK32BCMCFGW_devAdr                                 0x0FFFFFFF

    #define     BA_BCMCFGW_hdr                                 0x0007
    #define     B16BCMCFGW_hdr                                 0x0006
    #define   LSb32BCMCFGW_hdr                                    28
    #define   LSb16BCMCFGW_hdr                                    12
    #define       bBCMCFGW_hdr                                 4
    #define   MSK32BCMCFGW_hdr                                    0xF0000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_BCMCFGW {
    ///////////////////////////////////////////////////////////
    #define   GET32BCMCFGW_dat(r32)                            _BFGET_(r32,31, 0)
    #define   SET32BCMCFGW_dat(r32,v)                          _BFSET_(r32,31, 0,v)

            UNSG32 u_dat                                       : 32;
    ///////////////////////////////////////////////////////////
    #define   GET32BCMCFGW_devAdr(r32)                         _BFGET_(r32,27, 0)
    #define   SET32BCMCFGW_devAdr(r32,v)                       _BFSET_(r32,27, 0,v)

    #define   GET32BCMCFGW_hdr(r32)                            _BFGET_(r32,31,28)
    #define   SET32BCMCFGW_hdr(r32,v)                          _BFSET_(r32,31,28,v)
    #define   GET16BCMCFGW_hdr(r16)                            _BFGET_(r16,15,12)
    #define   SET16BCMCFGW_hdr(r16,v)                          _BFSET_(r16,15,12,v)

            UNSG32 u_devAdr                                    : 28;
            UNSG32 u_hdr                                       :  4;
    ///////////////////////////////////////////////////////////
    } SIE_BCMCFGW;

    ///////////////////////////////////////////////////////////
     SIGN32 BCMCFGW_drvrd(SIE_BCMCFGW *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BCMCFGW_drvwr(SIE_BCMCFGW *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BCMCFGW_reset(SIE_BCMCFGW *p);
     SIGN32 BCMCFGW_cmp  (SIE_BCMCFGW *p, SIE_BCMCFGW *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BCMCFGW_check(p,pie,pfx,hLOG) BCMCFGW_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BCMCFGW_print(p,    pfx,hLOG) BCMCFGW_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_BCMLDFN
#define h_BCMLDFN (){}

    #define     BA_BCMLDFN_ddrAdr                              0x0000
    #define     B16BCMLDFN_ddrAdr                              0x0000
    #define   LSb32BCMLDFN_ddrAdr                                 0
    #define   LSb16BCMLDFN_ddrAdr                                 0
    #define       bBCMLDFN_ddrAdr                              32
    #define   MSK32BCMLDFN_ddrAdr                                 0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    #define     BA_BCMLDFN_size                                0x0004
    #define     B16BCMLDFN_size                                0x0004
    #define   LSb32BCMLDFN_size                                   0
    #define   LSb16BCMLDFN_size                                   0
    #define       bBCMLDFN_size                                16
    #define   MSK32BCMLDFN_size                                   0x0000FFFF

    #define     BA_BCMLDFN_chId                                0x0006
    #define     B16BCMLDFN_chId                                0x0006
    #define   LSb32BCMLDFN_chId                                   16
    #define   LSb16BCMLDFN_chId                                   0
    #define       bBCMLDFN_chId                                4
    #define   MSK32BCMLDFN_chId                                   0x000F0000

    #define     BA_BCMLDFN_intr                                0x0006
    #define     B16BCMLDFN_intr                                0x0006
    #define   LSb32BCMLDFN_intr                                   20
    #define   LSb16BCMLDFN_intr                                   4
    #define       bBCMLDFN_intr                                1
    #define   MSK32BCMLDFN_intr                                   0x00100000

    #define     BA_BCMLDFN_rsvd                                0x0006
    #define     B16BCMLDFN_rsvd                                0x0006
    #define   LSb32BCMLDFN_rsvd                                   21
    #define   LSb16BCMLDFN_rsvd                                   5
    #define       bBCMLDFN_rsvd                                7
    #define   MSK32BCMLDFN_rsvd                                   0x0FE00000

    #define     BA_BCMLDFN_hdr                                 0x0007
    #define     B16BCMLDFN_hdr                                 0x0006
    #define   LSb32BCMLDFN_hdr                                    28
    #define   LSb16BCMLDFN_hdr                                    12
    #define       bBCMLDFN_hdr                                 4
    #define   MSK32BCMLDFN_hdr                                    0xF0000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_BCMLDFN {
    ///////////////////////////////////////////////////////////
    #define   GET32BCMLDFN_ddrAdr(r32)                         _BFGET_(r32,31, 0)
    #define   SET32BCMLDFN_ddrAdr(r32,v)                       _BFSET_(r32,31, 0,v)

            UNSG32 u_ddrAdr                                    : 32;
    ///////////////////////////////////////////////////////////
    #define   GET32BCMLDFN_size(r32)                           _BFGET_(r32,15, 0)
    #define   SET32BCMLDFN_size(r32,v)                         _BFSET_(r32,15, 0,v)
    #define   GET16BCMLDFN_size(r16)                           _BFGET_(r16,15, 0)
    #define   SET16BCMLDFN_size(r16,v)                         _BFSET_(r16,15, 0,v)

    #define   GET32BCMLDFN_chId(r32)                           _BFGET_(r32,19,16)
    #define   SET32BCMLDFN_chId(r32,v)                         _BFSET_(r32,19,16,v)
    #define   GET16BCMLDFN_chId(r16)                           _BFGET_(r16, 3, 0)
    #define   SET16BCMLDFN_chId(r16,v)                         _BFSET_(r16, 3, 0,v)

    #define   GET32BCMLDFN_intr(r32)                           _BFGET_(r32,20,20)
    #define   SET32BCMLDFN_intr(r32,v)                         _BFSET_(r32,20,20,v)
    #define   GET16BCMLDFN_intr(r16)                           _BFGET_(r16, 4, 4)
    #define   SET16BCMLDFN_intr(r16,v)                         _BFSET_(r16, 4, 4,v)

    #define   GET32BCMLDFN_rsvd(r32)                           _BFGET_(r32,27,21)
    #define   SET32BCMLDFN_rsvd(r32,v)                         _BFSET_(r32,27,21,v)
    #define   GET16BCMLDFN_rsvd(r16)                           _BFGET_(r16,11, 5)
    #define   SET16BCMLDFN_rsvd(r16,v)                         _BFSET_(r16,11, 5,v)

    #define   GET32BCMLDFN_hdr(r32)                            _BFGET_(r32,31,28)
    #define   SET32BCMLDFN_hdr(r32,v)                          _BFSET_(r32,31,28,v)
    #define   GET16BCMLDFN_hdr(r16)                            _BFGET_(r16,15,12)
    #define   SET16BCMLDFN_hdr(r16,v)                          _BFSET_(r16,15,12,v)

            UNSG32 u_size                                      : 16;
            UNSG32 u_chId                                      :  4;
            UNSG32 u_intr                                      :  1;
            UNSG32 u_rsvd                                      :  7;
            UNSG32 u_hdr                                       :  4;
    ///////////////////////////////////////////////////////////
    } SIE_BCMLDFN;

    ///////////////////////////////////////////////////////////
     SIGN32 BCMLDFN_drvrd(SIE_BCMLDFN *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BCMLDFN_drvwr(SIE_BCMLDFN *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BCMLDFN_reset(SIE_BCMLDFN *p);
     SIGN32 BCMLDFN_cmp  (SIE_BCMLDFN *p, SIE_BCMLDFN *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BCMLDFN_check(p,pie,pfx,hLOG) BCMLDFN_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BCMLDFN_print(p,    pfx,hLOG) BCMLDFN_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_BCMRDAT
#define h_BCMRDAT (){}

    #define     BA_BCMRDAT_size                                0x0000
    #define     B16BCMRDAT_size                                0x0000
    #define   LSb32BCMRDAT_size                                   0
    #define   LSb16BCMRDAT_size                                   0
    #define       bBCMRDAT_size                                16
    #define   MSK32BCMRDAT_size                                   0x0000FFFF

    #define     BA_BCMRDAT_mode                                0x0002
    #define     B16BCMRDAT_mode                                0x0002
    #define   LSb32BCMRDAT_mode                                   16
    #define   LSb16BCMRDAT_mode                                   0
    #define       bBCMRDAT_mode                                2
    #define   MSK32BCMRDAT_mode                                   0x00030000
    #define        BCMRDAT_mode_byte                                        0x0
    #define        BCMRDAT_mode_word                                        0x1
    #define        BCMRDAT_mode_dword                                       0x2

    #define     BA_BCMRDAT_endian                              0x0002
    #define     B16BCMRDAT_endian                              0x0002
    #define   LSb32BCMRDAT_endian                                 18
    #define   LSb16BCMRDAT_endian                                 2
    #define       bBCMRDAT_endian                              1
    #define   MSK32BCMRDAT_endian                                 0x00040000
    #define        BCMRDAT_endian_little_endian                             0x0
    #define        BCMRDAT_endian_bit_endian                                0x1

    #define     BA_BCMRDAT_last                                0x0002
    #define     B16BCMRDAT_last                                0x0002
    #define   LSb32BCMRDAT_last                                   19
    #define   LSb16BCMRDAT_last                                   3
    #define       bBCMRDAT_last                                1
    #define   MSK32BCMRDAT_last                                   0x00080000

    #define     BA_BCMRDAT_cUpdId                              0x0002
    #define     B16BCMRDAT_cUpdId                              0x0002
    #define   LSb32BCMRDAT_cUpdId                                 20
    #define   LSb16BCMRDAT_cUpdId                                 4
    #define       bBCMRDAT_cUpdId                              5
    #define   MSK32BCMRDAT_cUpdId                                 0x01F00000

    #define     BA_BCMRDAT_pUpdId                              0x0003
    #define     B16BCMRDAT_pUpdId                              0x0002
    #define   LSb32BCMRDAT_pUpdId                                 25
    #define   LSb16BCMRDAT_pUpdId                                 9
    #define       bBCMRDAT_pUpdId                              5
    #define   MSK32BCMRDAT_pUpdId                                 0x3E000000

    #define     BA_BCMRDAT_rsvd0                               0x0003
    #define     B16BCMRDAT_rsvd0                               0x0002
    #define   LSb32BCMRDAT_rsvd0                                  30
    #define   LSb16BCMRDAT_rsvd0                                  14
    #define       bBCMRDAT_rsvd0                               2
    #define   MSK32BCMRDAT_rsvd0                                  0xC0000000
    ///////////////////////////////////////////////////////////

    #define     BA_BCMRDAT_devAdr                              0x0004
    #define     B16BCMRDAT_devAdr                              0x0004
    #define   LSb32BCMRDAT_devAdr                                 0
    #define   LSb16BCMRDAT_devAdr                                 0
    #define       bBCMRDAT_devAdr                              28
    #define   MSK32BCMRDAT_devAdr                                 0x0FFFFFFF

    #define     BA_BCMRDAT_hdr                                 0x0007
    #define     B16BCMRDAT_hdr                                 0x0006
    #define   LSb32BCMRDAT_hdr                                    28
    #define   LSb16BCMRDAT_hdr                                    12
    #define       bBCMRDAT_hdr                                 4
    #define   MSK32BCMRDAT_hdr                                    0xF0000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_BCMRDAT {
    ///////////////////////////////////////////////////////////
    #define   GET32BCMRDAT_size(r32)                           _BFGET_(r32,15, 0)
    #define   SET32BCMRDAT_size(r32,v)                         _BFSET_(r32,15, 0,v)
    #define   GET16BCMRDAT_size(r16)                           _BFGET_(r16,15, 0)
    #define   SET16BCMRDAT_size(r16,v)                         _BFSET_(r16,15, 0,v)

    #define   GET32BCMRDAT_mode(r32)                           _BFGET_(r32,17,16)
    #define   SET32BCMRDAT_mode(r32,v)                         _BFSET_(r32,17,16,v)
    #define   GET16BCMRDAT_mode(r16)                           _BFGET_(r16, 1, 0)
    #define   SET16BCMRDAT_mode(r16,v)                         _BFSET_(r16, 1, 0,v)

    #define   GET32BCMRDAT_endian(r32)                         _BFGET_(r32,18,18)
    #define   SET32BCMRDAT_endian(r32,v)                       _BFSET_(r32,18,18,v)
    #define   GET16BCMRDAT_endian(r16)                         _BFGET_(r16, 2, 2)
    #define   SET16BCMRDAT_endian(r16,v)                       _BFSET_(r16, 2, 2,v)

    #define   GET32BCMRDAT_last(r32)                           _BFGET_(r32,19,19)
    #define   SET32BCMRDAT_last(r32,v)                         _BFSET_(r32,19,19,v)
    #define   GET16BCMRDAT_last(r16)                           _BFGET_(r16, 3, 3)
    #define   SET16BCMRDAT_last(r16,v)                         _BFSET_(r16, 3, 3,v)

    #define   GET32BCMRDAT_cUpdId(r32)                         _BFGET_(r32,24,20)
    #define   SET32BCMRDAT_cUpdId(r32,v)                       _BFSET_(r32,24,20,v)
    #define   GET16BCMRDAT_cUpdId(r16)                         _BFGET_(r16, 8, 4)
    #define   SET16BCMRDAT_cUpdId(r16,v)                       _BFSET_(r16, 8, 4,v)

    #define   GET32BCMRDAT_pUpdId(r32)                         _BFGET_(r32,29,25)
    #define   SET32BCMRDAT_pUpdId(r32,v)                       _BFSET_(r32,29,25,v)
    #define   GET16BCMRDAT_pUpdId(r16)                         _BFGET_(r16,13, 9)
    #define   SET16BCMRDAT_pUpdId(r16,v)                       _BFSET_(r16,13, 9,v)

    #define   GET32BCMRDAT_rsvd0(r32)                          _BFGET_(r32,31,30)
    #define   SET32BCMRDAT_rsvd0(r32,v)                        _BFSET_(r32,31,30,v)
    #define   GET16BCMRDAT_rsvd0(r16)                          _BFGET_(r16,15,14)
    #define   SET16BCMRDAT_rsvd0(r16,v)                        _BFSET_(r16,15,14,v)

            UNSG32 u_size                                      : 16;
            UNSG32 u_mode                                      :  2;
            UNSG32 u_endian                                    :  1;
            UNSG32 u_last                                      :  1;
            UNSG32 u_cUpdId                                    :  5;
            UNSG32 u_pUpdId                                    :  5;
            UNSG32 u_rsvd0                                     :  2;
    ///////////////////////////////////////////////////////////
    #define   GET32BCMRDAT_devAdr(r32)                         _BFGET_(r32,27, 0)
    #define   SET32BCMRDAT_devAdr(r32,v)                       _BFSET_(r32,27, 0,v)

    #define   GET32BCMRDAT_hdr(r32)                            _BFGET_(r32,31,28)
    #define   SET32BCMRDAT_hdr(r32,v)                          _BFSET_(r32,31,28,v)
    #define   GET16BCMRDAT_hdr(r16)                            _BFGET_(r16,15,12)
    #define   SET16BCMRDAT_hdr(r16,v)                          _BFSET_(r16,15,12,v)

            UNSG32 u_devAdr                                    : 28;
            UNSG32 u_hdr                                       :  4;
    ///////////////////////////////////////////////////////////
    } SIE_BCMRDAT;

    ///////////////////////////////////////////////////////////
     SIGN32 BCMRDAT_drvrd(SIE_BCMRDAT *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BCMRDAT_drvwr(SIE_BCMRDAT *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BCMRDAT_reset(SIE_BCMRDAT *p);
     SIGN32 BCMRDAT_cmp  (SIE_BCMRDAT *p, SIE_BCMRDAT *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BCMRDAT_check(p,pie,pfx,hLOG) BCMRDAT_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BCMRDAT_print(p,    pfx,hLOG) BCMRDAT_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_BCMWDAT
#define h_BCMWDAT (){}

    #define     BA_BCMWDAT_size                                0x0000
    #define     B16BCMWDAT_size                                0x0000
    #define   LSb32BCMWDAT_size                                   0
    #define   LSb16BCMWDAT_size                                   0
    #define       bBCMWDAT_size                                16
    #define   MSK32BCMWDAT_size                                   0x0000FFFF

    #define     BA_BCMWDAT_mode                                0x0002
    #define     B16BCMWDAT_mode                                0x0002
    #define   LSb32BCMWDAT_mode                                   16
    #define   LSb16BCMWDAT_mode                                   0
    #define       bBCMWDAT_mode                                2
    #define   MSK32BCMWDAT_mode                                   0x00030000
    #define        BCMWDAT_mode_byte                                        0x0
    #define        BCMWDAT_mode_word                                        0x1
    #define        BCMWDAT_mode_dword                                       0x2

    #define     BA_BCMWDAT_endian                              0x0002
    #define     B16BCMWDAT_endian                              0x0002
    #define   LSb32BCMWDAT_endian                                 18
    #define   LSb16BCMWDAT_endian                                 2
    #define       bBCMWDAT_endian                              1
    #define   MSK32BCMWDAT_endian                                 0x00040000
    #define        BCMWDAT_endian_little_endian                             0x0
    #define        BCMWDAT_endian_big_endian                                0x1

    #define     BA_BCMWDAT_last                                0x0002
    #define     B16BCMWDAT_last                                0x0002
    #define   LSb32BCMWDAT_last                                   19
    #define   LSb16BCMWDAT_last                                   3
    #define       bBCMWDAT_last                                1
    #define   MSK32BCMWDAT_last                                   0x00080000

    #define     BA_BCMWDAT_cUpdId                              0x0002
    #define     B16BCMWDAT_cUpdId                              0x0002
    #define   LSb32BCMWDAT_cUpdId                                 20
    #define   LSb16BCMWDAT_cUpdId                                 4
    #define       bBCMWDAT_cUpdId                              5
    #define   MSK32BCMWDAT_cUpdId                                 0x01F00000

    #define     BA_BCMWDAT_rsvd0                               0x0003
    #define     B16BCMWDAT_rsvd0                               0x0002
    #define   LSb32BCMWDAT_rsvd0                                  25
    #define   LSb16BCMWDAT_rsvd0                                  9
    #define       bBCMWDAT_rsvd0                               7
    #define   MSK32BCMWDAT_rsvd0                                  0xFE000000
    ///////////////////////////////////////////////////////////

    #define     BA_BCMWDAT_devAdr                              0x0004
    #define     B16BCMWDAT_devAdr                              0x0004
    #define   LSb32BCMWDAT_devAdr                                 0
    #define   LSb16BCMWDAT_devAdr                                 0
    #define       bBCMWDAT_devAdr                              28
    #define   MSK32BCMWDAT_devAdr                                 0x0FFFFFFF

    #define     BA_BCMWDAT_hdr                                 0x0007
    #define     B16BCMWDAT_hdr                                 0x0006
    #define   LSb32BCMWDAT_hdr                                    28
    #define   LSb16BCMWDAT_hdr                                    12
    #define       bBCMWDAT_hdr                                 4
    #define   MSK32BCMWDAT_hdr                                    0xF0000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_BCMWDAT {
    ///////////////////////////////////////////////////////////
    #define   GET32BCMWDAT_size(r32)                           _BFGET_(r32,15, 0)
    #define   SET32BCMWDAT_size(r32,v)                         _BFSET_(r32,15, 0,v)
    #define   GET16BCMWDAT_size(r16)                           _BFGET_(r16,15, 0)
    #define   SET16BCMWDAT_size(r16,v)                         _BFSET_(r16,15, 0,v)

    #define   GET32BCMWDAT_mode(r32)                           _BFGET_(r32,17,16)
    #define   SET32BCMWDAT_mode(r32,v)                         _BFSET_(r32,17,16,v)
    #define   GET16BCMWDAT_mode(r16)                           _BFGET_(r16, 1, 0)
    #define   SET16BCMWDAT_mode(r16,v)                         _BFSET_(r16, 1, 0,v)

    #define   GET32BCMWDAT_endian(r32)                         _BFGET_(r32,18,18)
    #define   SET32BCMWDAT_endian(r32,v)                       _BFSET_(r32,18,18,v)
    #define   GET16BCMWDAT_endian(r16)                         _BFGET_(r16, 2, 2)
    #define   SET16BCMWDAT_endian(r16,v)                       _BFSET_(r16, 2, 2,v)

    #define   GET32BCMWDAT_last(r32)                           _BFGET_(r32,19,19)
    #define   SET32BCMWDAT_last(r32,v)                         _BFSET_(r32,19,19,v)
    #define   GET16BCMWDAT_last(r16)                           _BFGET_(r16, 3, 3)
    #define   SET16BCMWDAT_last(r16,v)                         _BFSET_(r16, 3, 3,v)

    #define   GET32BCMWDAT_cUpdId(r32)                         _BFGET_(r32,24,20)
    #define   SET32BCMWDAT_cUpdId(r32,v)                       _BFSET_(r32,24,20,v)
    #define   GET16BCMWDAT_cUpdId(r16)                         _BFGET_(r16, 8, 4)
    #define   SET16BCMWDAT_cUpdId(r16,v)                       _BFSET_(r16, 8, 4,v)

    #define   GET32BCMWDAT_rsvd0(r32)                          _BFGET_(r32,31,25)
    #define   SET32BCMWDAT_rsvd0(r32,v)                        _BFSET_(r32,31,25,v)
    #define   GET16BCMWDAT_rsvd0(r16)                          _BFGET_(r16,15, 9)
    #define   SET16BCMWDAT_rsvd0(r16,v)                        _BFSET_(r16,15, 9,v)

            UNSG32 u_size                                      : 16;
            UNSG32 u_mode                                      :  2;
            UNSG32 u_endian                                    :  1;
            UNSG32 u_last                                      :  1;
            UNSG32 u_cUpdId                                    :  5;
            UNSG32 u_rsvd0                                     :  7;
    ///////////////////////////////////////////////////////////
    #define   GET32BCMWDAT_devAdr(r32)                         _BFGET_(r32,27, 0)
    #define   SET32BCMWDAT_devAdr(r32,v)                       _BFSET_(r32,27, 0,v)

    #define   GET32BCMWDAT_hdr(r32)                            _BFGET_(r32,31,28)
    #define   SET32BCMWDAT_hdr(r32,v)                          _BFSET_(r32,31,28,v)
    #define   GET16BCMWDAT_hdr(r16)                            _BFGET_(r16,15,12)
    #define   SET16BCMWDAT_hdr(r16,v)                          _BFSET_(r16,15,12,v)

            UNSG32 u_devAdr                                    : 28;
            UNSG32 u_hdr                                       :  4;
    ///////////////////////////////////////////////////////////
    } SIE_BCMWDAT;

    ///////////////////////////////////////////////////////////
     SIGN32 BCMWDAT_drvrd(SIE_BCMWDAT *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BCMWDAT_drvwr(SIE_BCMWDAT *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BCMWDAT_reset(SIE_BCMWDAT *p);
     SIGN32 BCMWDAT_cmp  (SIE_BCMWDAT *p, SIE_BCMWDAT *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BCMWDAT_check(p,pie,pfx,hLOG) BCMWDAT_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BCMWDAT_print(p,    pfx,hLOG) BCMWDAT_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_BCMRCMD
#define h_BCMRCMD (){}

    #define     BA_BCMRCMD_ddrAdr                              0x0000
    #define     B16BCMRCMD_ddrAdr                              0x0000
    #define   LSb32BCMRCMD_ddrAdr                                 0
    #define   LSb16BCMRCMD_ddrAdr                                 0
    #define       bBCMRCMD_ddrAdr                              32
    #define   MSK32BCMRCMD_ddrAdr                                 0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    #define     BA_BCMRCMD_size                                0x0004
    #define     B16BCMRCMD_size                                0x0004
    #define   LSb32BCMRCMD_size                                   0
    #define   LSb16BCMRCMD_size                                   0
    #define       bBCMRCMD_size                                16
    #define   MSK32BCMRCMD_size                                   0x0000FFFF

    #define     BA_BCMRCMD_chkSemId                            0x0006
    #define     B16BCMRCMD_chkSemId                            0x0006
    #define   LSb32BCMRCMD_chkSemId                               16
    #define   LSb16BCMRCMD_chkSemId                               0
    #define       bBCMRCMD_chkSemId                            5
    #define   MSK32BCMRCMD_chkSemId                               0x001F0000

    #define     BA_BCMRCMD_updSemId                            0x0006
    #define     B16BCMRCMD_updSemId                            0x0006
    #define   LSb32BCMRCMD_updSemId                               21
    #define   LSb16BCMRCMD_updSemId                               5
    #define       bBCMRCMD_updSemId                            5
    #define   MSK32BCMRCMD_updSemId                               0x03E00000

    #define     BA_BCMRCMD_rsvd                                0x0007
    #define     B16BCMRCMD_rsvd                                0x0006
    #define   LSb32BCMRCMD_rsvd                                   26
    #define   LSb16BCMRCMD_rsvd                                   10
    #define       bBCMRCMD_rsvd                                2
    #define   MSK32BCMRCMD_rsvd                                   0x0C000000

    #define     BA_BCMRCMD_hdr                                 0x0007
    #define     B16BCMRCMD_hdr                                 0x0006
    #define   LSb32BCMRCMD_hdr                                    28
    #define   LSb16BCMRCMD_hdr                                    12
    #define       bBCMRCMD_hdr                                 4
    #define   MSK32BCMRCMD_hdr                                    0xF0000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_BCMRCMD {
    ///////////////////////////////////////////////////////////
    #define   GET32BCMRCMD_ddrAdr(r32)                         _BFGET_(r32,31, 0)
    #define   SET32BCMRCMD_ddrAdr(r32,v)                       _BFSET_(r32,31, 0,v)

            UNSG32 u_ddrAdr                                    : 32;
    ///////////////////////////////////////////////////////////
    #define   GET32BCMRCMD_size(r32)                           _BFGET_(r32,15, 0)
    #define   SET32BCMRCMD_size(r32,v)                         _BFSET_(r32,15, 0,v)
    #define   GET16BCMRCMD_size(r16)                           _BFGET_(r16,15, 0)
    #define   SET16BCMRCMD_size(r16,v)                         _BFSET_(r16,15, 0,v)

    #define   GET32BCMRCMD_chkSemId(r32)                       _BFGET_(r32,20,16)
    #define   SET32BCMRCMD_chkSemId(r32,v)                     _BFSET_(r32,20,16,v)
    #define   GET16BCMRCMD_chkSemId(r16)                       _BFGET_(r16, 4, 0)
    #define   SET16BCMRCMD_chkSemId(r16,v)                     _BFSET_(r16, 4, 0,v)

    #define   GET32BCMRCMD_updSemId(r32)                       _BFGET_(r32,25,21)
    #define   SET32BCMRCMD_updSemId(r32,v)                     _BFSET_(r32,25,21,v)
    #define   GET16BCMRCMD_updSemId(r16)                       _BFGET_(r16, 9, 5)
    #define   SET16BCMRCMD_updSemId(r16,v)                     _BFSET_(r16, 9, 5,v)

    #define   GET32BCMRCMD_rsvd(r32)                           _BFGET_(r32,27,26)
    #define   SET32BCMRCMD_rsvd(r32,v)                         _BFSET_(r32,27,26,v)
    #define   GET16BCMRCMD_rsvd(r16)                           _BFGET_(r16,11,10)
    #define   SET16BCMRCMD_rsvd(r16,v)                         _BFSET_(r16,11,10,v)

    #define   GET32BCMRCMD_hdr(r32)                            _BFGET_(r32,31,28)
    #define   SET32BCMRCMD_hdr(r32,v)                          _BFSET_(r32,31,28,v)
    #define   GET16BCMRCMD_hdr(r16)                            _BFGET_(r16,15,12)
    #define   SET16BCMRCMD_hdr(r16,v)                          _BFSET_(r16,15,12,v)

            UNSG32 u_size                                      : 16;
            UNSG32 u_chkSemId                                  :  5;
            UNSG32 u_updSemId                                  :  5;
            UNSG32 u_rsvd                                      :  2;
            UNSG32 u_hdr                                       :  4;
    ///////////////////////////////////////////////////////////
    } SIE_BCMRCMD;

    ///////////////////////////////////////////////////////////
     SIGN32 BCMRCMD_drvrd(SIE_BCMRCMD *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BCMRCMD_drvwr(SIE_BCMRCMD *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BCMRCMD_reset(SIE_BCMRCMD *p);
     SIGN32 BCMRCMD_cmp  (SIE_BCMRCMD *p, SIE_BCMRCMD *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BCMRCMD_check(p,pie,pfx,hLOG) BCMRCMD_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BCMRCMD_print(p,    pfx,hLOG) BCMRCMD_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_BCMWCMD
#define h_BCMWCMD (){}

    #define     BA_BCMWCMD_ddrAdr                              0x0000
    #define     B16BCMWCMD_ddrAdr                              0x0000
    #define   LSb32BCMWCMD_ddrAdr                                 0
    #define   LSb16BCMWCMD_ddrAdr                                 0
    #define       bBCMWCMD_ddrAdr                              32
    #define   MSK32BCMWCMD_ddrAdr                                 0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    #define     BA_BCMWCMD_size                                0x0004
    #define     B16BCMWCMD_size                                0x0004
    #define   LSb32BCMWCMD_size                                   0
    #define   LSb16BCMWCMD_size                                   0
    #define       bBCMWCMD_size                                16
    #define   MSK32BCMWCMD_size                                   0x0000FFFF

    #define     BA_BCMWCMD_chkSemId                            0x0006
    #define     B16BCMWCMD_chkSemId                            0x0006
    #define   LSb32BCMWCMD_chkSemId                               16
    #define   LSb16BCMWCMD_chkSemId                               0
    #define       bBCMWCMD_chkSemId                            5
    #define   MSK32BCMWCMD_chkSemId                               0x001F0000

    #define     BA_BCMWCMD_updSemId                            0x0006
    #define     B16BCMWCMD_updSemId                            0x0006
    #define   LSb32BCMWCMD_updSemId                               21
    #define   LSb16BCMWCMD_updSemId                               5
    #define       bBCMWCMD_updSemId                            5
    #define   MSK32BCMWCMD_updSemId                               0x03E00000

    #define     BA_BCMWCMD_rsvd                                0x0007
    #define     B16BCMWCMD_rsvd                                0x0006
    #define   LSb32BCMWCMD_rsvd                                   26
    #define   LSb16BCMWCMD_rsvd                                   10
    #define       bBCMWCMD_rsvd                                2
    #define   MSK32BCMWCMD_rsvd                                   0x0C000000

    #define     BA_BCMWCMD_hdr                                 0x0007
    #define     B16BCMWCMD_hdr                                 0x0006
    #define   LSb32BCMWCMD_hdr                                    28
    #define   LSb16BCMWCMD_hdr                                    12
    #define       bBCMWCMD_hdr                                 4
    #define   MSK32BCMWCMD_hdr                                    0xF0000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_BCMWCMD {
    ///////////////////////////////////////////////////////////
    #define   GET32BCMWCMD_ddrAdr(r32)                         _BFGET_(r32,31, 0)
    #define   SET32BCMWCMD_ddrAdr(r32,v)                       _BFSET_(r32,31, 0,v)

            UNSG32 u_ddrAdr                                    : 32;
    ///////////////////////////////////////////////////////////
    #define   GET32BCMWCMD_size(r32)                           _BFGET_(r32,15, 0)
    #define   SET32BCMWCMD_size(r32,v)                         _BFSET_(r32,15, 0,v)
    #define   GET16BCMWCMD_size(r16)                           _BFGET_(r16,15, 0)
    #define   SET16BCMWCMD_size(r16,v)                         _BFSET_(r16,15, 0,v)

    #define   GET32BCMWCMD_chkSemId(r32)                       _BFGET_(r32,20,16)
    #define   SET32BCMWCMD_chkSemId(r32,v)                     _BFSET_(r32,20,16,v)
    #define   GET16BCMWCMD_chkSemId(r16)                       _BFGET_(r16, 4, 0)
    #define   SET16BCMWCMD_chkSemId(r16,v)                     _BFSET_(r16, 4, 0,v)

    #define   GET32BCMWCMD_updSemId(r32)                       _BFGET_(r32,25,21)
    #define   SET32BCMWCMD_updSemId(r32,v)                     _BFSET_(r32,25,21,v)
    #define   GET16BCMWCMD_updSemId(r16)                       _BFGET_(r16, 9, 5)
    #define   SET16BCMWCMD_updSemId(r16,v)                     _BFSET_(r16, 9, 5,v)

    #define   GET32BCMWCMD_rsvd(r32)                           _BFGET_(r32,27,26)
    #define   SET32BCMWCMD_rsvd(r32,v)                         _BFSET_(r32,27,26,v)
    #define   GET16BCMWCMD_rsvd(r16)                           _BFGET_(r16,11,10)
    #define   SET16BCMWCMD_rsvd(r16,v)                         _BFSET_(r16,11,10,v)

    #define   GET32BCMWCMD_hdr(r32)                            _BFGET_(r32,31,28)
    #define   SET32BCMWCMD_hdr(r32,v)                          _BFSET_(r32,31,28,v)
    #define   GET16BCMWCMD_hdr(r16)                            _BFGET_(r16,15,12)
    #define   SET16BCMWCMD_hdr(r16,v)                          _BFSET_(r16,15,12,v)

            UNSG32 u_size                                      : 16;
            UNSG32 u_chkSemId                                  :  5;
            UNSG32 u_updSemId                                  :  5;
            UNSG32 u_rsvd                                      :  2;
            UNSG32 u_hdr                                       :  4;
    ///////////////////////////////////////////////////////////
    } SIE_BCMWCMD;

    ///////////////////////////////////////////////////////////
     SIGN32 BCMWCMD_drvrd(SIE_BCMWCMD *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BCMWCMD_drvwr(SIE_BCMWCMD *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BCMWCMD_reset(SIE_BCMWCMD *p);
     SIGN32 BCMWCMD_cmp  (SIE_BCMWCMD *p, SIE_BCMWCMD *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BCMWCMD_check(p,pie,pfx,hLOG) BCMWCMD_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BCMWCMD_print(p,    pfx,hLOG) BCMWCMD_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_BCMSEMA
#define h_BCMSEMA (){}

    #define     BA_BCMSEMA_pUpdId                              0x0000
    #define     B16BCMSEMA_pUpdId                              0x0000
    #define   LSb32BCMSEMA_pUpdId                                 0
    #define   LSb16BCMSEMA_pUpdId                                 0
    #define       bBCMSEMA_pUpdId                              5
    #define   MSK32BCMSEMA_pUpdId                                 0x0000001F

    #define     BA_BCMSEMA_pChkId                              0x0000
    #define     B16BCMSEMA_pChkId                              0x0000
    #define   LSb32BCMSEMA_pChkId                                 5
    #define   LSb16BCMSEMA_pChkId                                 5
    #define       bBCMSEMA_pChkId                              5
    #define   MSK32BCMSEMA_pChkId                                 0x000003E0

    #define     BA_BCMSEMA_cUpdId                              0x0001
    #define     B16BCMSEMA_cUpdId                              0x0000
    #define   LSb32BCMSEMA_cUpdId                                 10
    #define   LSb16BCMSEMA_cUpdId                                 10
    #define       bBCMSEMA_cUpdId                              5
    #define   MSK32BCMSEMA_cUpdId                                 0x00007C00

    #define     BA_BCMSEMA_cChkId                              0x0001
    #define     B16BCMSEMA_cChkId                              0x0000
    #define   LSb32BCMSEMA_cChkId                                 15
    #define   LSb16BCMSEMA_cChkId                                 15
    #define       bBCMSEMA_cChkId                              5
    #define   MSK32BCMSEMA_cChkId                                 0x000F8000

    #define     BA_BCMSEMA_rsvd0                               0x0002
    #define     B16BCMSEMA_rsvd0                               0x0002
    #define   LSb32BCMSEMA_rsvd0                                  20
    #define   LSb16BCMSEMA_rsvd0                                  4
    #define       bBCMSEMA_rsvd0                               12
    #define   MSK32BCMSEMA_rsvd0                                  0xFFF00000
    ///////////////////////////////////////////////////////////

    #define     BA_BCMSEMA_rsvd1                               0x0004
    #define     B16BCMSEMA_rsvd1                               0x0004
    #define   LSb32BCMSEMA_rsvd1                                  0
    #define   LSb16BCMSEMA_rsvd1                                  0
    #define       bBCMSEMA_rsvd1                               28
    #define   MSK32BCMSEMA_rsvd1                                  0x0FFFFFFF

    #define     BA_BCMSEMA_hdr                                 0x0007
    #define     B16BCMSEMA_hdr                                 0x0006
    #define   LSb32BCMSEMA_hdr                                    28
    #define   LSb16BCMSEMA_hdr                                    12
    #define       bBCMSEMA_hdr                                 4
    #define   MSK32BCMSEMA_hdr                                    0xF0000000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_BCMSEMA {
    ///////////////////////////////////////////////////////////
    #define   GET32BCMSEMA_pUpdId(r32)                         _BFGET_(r32, 4, 0)
    #define   SET32BCMSEMA_pUpdId(r32,v)                       _BFSET_(r32, 4, 0,v)
    #define   GET16BCMSEMA_pUpdId(r16)                         _BFGET_(r16, 4, 0)
    #define   SET16BCMSEMA_pUpdId(r16,v)                       _BFSET_(r16, 4, 0,v)

    #define   GET32BCMSEMA_pChkId(r32)                         _BFGET_(r32, 9, 5)
    #define   SET32BCMSEMA_pChkId(r32,v)                       _BFSET_(r32, 9, 5,v)
    #define   GET16BCMSEMA_pChkId(r16)                         _BFGET_(r16, 9, 5)
    #define   SET16BCMSEMA_pChkId(r16,v)                       _BFSET_(r16, 9, 5,v)

    #define   GET32BCMSEMA_cUpdId(r32)                         _BFGET_(r32,14,10)
    #define   SET32BCMSEMA_cUpdId(r32,v)                       _BFSET_(r32,14,10,v)
    #define   GET16BCMSEMA_cUpdId(r16)                         _BFGET_(r16,14,10)
    #define   SET16BCMSEMA_cUpdId(r16,v)                       _BFSET_(r16,14,10,v)

    #define   GET32BCMSEMA_cChkId(r32)                         _BFGET_(r32,19,15)
    #define   SET32BCMSEMA_cChkId(r32,v)                       _BFSET_(r32,19,15,v)

    #define   GET32BCMSEMA_rsvd0(r32)                          _BFGET_(r32,31,20)
    #define   SET32BCMSEMA_rsvd0(r32,v)                        _BFSET_(r32,31,20,v)
    #define   GET16BCMSEMA_rsvd0(r16)                          _BFGET_(r16,15, 4)
    #define   SET16BCMSEMA_rsvd0(r16,v)                        _BFSET_(r16,15, 4,v)

            UNSG32 u_pUpdId                                    :  5;
            UNSG32 u_pChkId                                    :  5;
            UNSG32 u_cUpdId                                    :  5;
            UNSG32 u_cChkId                                    :  5;
            UNSG32 u_rsvd0                                     : 12;
    ///////////////////////////////////////////////////////////
    #define   GET32BCMSEMA_rsvd1(r32)                          _BFGET_(r32,27, 0)
    #define   SET32BCMSEMA_rsvd1(r32,v)                        _BFSET_(r32,27, 0,v)

    #define   GET32BCMSEMA_hdr(r32)                            _BFGET_(r32,31,28)
    #define   SET32BCMSEMA_hdr(r32,v)                          _BFSET_(r32,31,28,v)
    #define   GET16BCMSEMA_hdr(r16)                            _BFGET_(r16,15,12)
    #define   SET16BCMSEMA_hdr(r16,v)                          _BFSET_(r16,15,12,v)

            UNSG32 u_rsvd1                                     : 28;
            UNSG32 u_hdr                                       :  4;
    ///////////////////////////////////////////////////////////
    } SIE_BCMSEMA;

    ///////////////////////////////////////////////////////////
     SIGN32 BCMSEMA_drvrd(SIE_BCMSEMA *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BCMSEMA_drvwr(SIE_BCMSEMA *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BCMSEMA_reset(SIE_BCMSEMA *p);
     SIGN32 BCMSEMA_cmp  (SIE_BCMSEMA *p, SIE_BCMSEMA *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BCMSEMA_check(p,pie,pfx,hLOG) BCMSEMA_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BCMSEMA_print(p,    pfx,hLOG) BCMSEMA_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_BCMErrSt
#define h_BCMErrSt (){}

    #define     BA_BCMErrSt_addr                               0x0000
    #define     B16BCMErrSt_addr                               0x0000
    #define   LSb32BCMErrSt_addr                                  0
    #define   LSb16BCMErrSt_addr                                  0
    #define       bBCMErrSt_addr                               32
    #define   MSK32BCMErrSt_addr                                  0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    #define     BA_BCMErrSt_op                                 0x0004
    #define     B16BCMErrSt_op                                 0x0004
    #define   LSb32BCMErrSt_op                                    0
    #define   LSb16BCMErrSt_op                                    0
    #define       bBCMErrSt_op                                 1
    #define   MSK32BCMErrSt_op                                    0x00000001

    #define     BA_BCMErrSt_mode                               0x0004
    #define     B16BCMErrSt_mode                               0x0004
    #define   LSb32BCMErrSt_mode                                  1
    #define   LSb16BCMErrSt_mode                                  1
    #define       bBCMErrSt_mode                               1
    #define   MSK32BCMErrSt_mode                                  0x00000002
    #define        BCMErrSt_mode_Timeout                                    0x0
    #define        BCMErrSt_mode_Err                                        0x1
    ///////////////////////////////////////////////////////////

    typedef struct SIE_BCMErrSt {
    ///////////////////////////////////////////////////////////
    #define   GET32BCMErrSt_addr(r32)                          _BFGET_(r32,31, 0)
    #define   SET32BCMErrSt_addr(r32,v)                        _BFSET_(r32,31, 0,v)

            UNSG32 u_addr                                      : 32;
    ///////////////////////////////////////////////////////////
    #define   GET32BCMErrSt_op(r32)                            _BFGET_(r32, 0, 0)
    #define   SET32BCMErrSt_op(r32,v)                          _BFSET_(r32, 0, 0,v)
    #define   GET16BCMErrSt_op(r16)                            _BFGET_(r16, 0, 0)
    #define   SET16BCMErrSt_op(r16,v)                          _BFSET_(r16, 0, 0,v)

    #define   GET32BCMErrSt_mode(r32)                          _BFGET_(r32, 1, 1)
    #define   SET32BCMErrSt_mode(r32,v)                        _BFSET_(r32, 1, 1,v)
    #define   GET16BCMErrSt_mode(r16)                          _BFGET_(r16, 1, 1)
    #define   SET16BCMErrSt_mode(r16,v)                        _BFSET_(r16, 1, 1,v)

            UNSG32 u_op                                        :  1;
            UNSG32 u_mode                                      :  1;
            UNSG32 RSVDx4_b2                                   : 30;
    ///////////////////////////////////////////////////////////
    } SIE_BCMErrSt;

    ///////////////////////////////////////////////////////////
     SIGN32 BCMErrSt_drvrd(SIE_BCMErrSt *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BCMErrSt_drvwr(SIE_BCMErrSt *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BCMErrSt_reset(SIE_BCMErrSt *p);
     SIGN32 BCMErrSt_cmp  (SIE_BCMErrSt *p, SIE_BCMErrSt *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BCMErrSt_check(p,pie,pfx,hLOG) BCMErrSt_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BCMErrSt_print(p,    pfx,hLOG) BCMErrSt_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_BCM
#define h_BCM (){}

    #define     RA_BCM_BCMErrSt                                0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_BCM_base                                    0x0008

    #define     BA_BCM_base_adr                                0x0008
    #define     B16BCM_base_adr                                0x0008
    #define   LSb32BCM_base_adr                                   0
    #define   LSb16BCM_base_adr                                   0
    #define       bBCM_base_adr                                32
    #define   MSK32BCM_base_adr                                   0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_BCM {
    ///////////////////////////////////////////////////////////
              SIE_BCMErrSt                                     ie_BCMErrSt;
    ///////////////////////////////////////////////////////////
    #define   GET32BCM_base_adr(r32)                           _BFGET_(r32,31, 0)
    #define   SET32BCM_base_adr(r32,v)                         _BFSET_(r32,31, 0,v)

    #define     w32BCM_base                                    {\
            UNSG32 ubase_adr                                   : 32;\
          }
    union { UNSG32 u32BCM_base;
            struct w32BCM_base;
          };
    ///////////////////////////////////////////////////////////
    } SIE_BCM;

    typedef union  T32BCM_base
          { UNSG32 u32;
            struct w32BCM_base;
                 } T32BCM_base;
    ///////////////////////////////////////////////////////////

    typedef union  TBCM_base
          { UNSG32 u32[1];
            struct {
            struct w32BCM_base;
                   };
                 } TBCM_base;

    ///////////////////////////////////////////////////////////
     SIGN32 BCM_drvrd(SIE_BCM *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 BCM_drvwr(SIE_BCM *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void BCM_reset(SIE_BCM *p);
     SIGN32 BCM_cmp  (SIE_BCM *p, SIE_BCM *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define BCM_check(p,pie,pfx,hLOG) BCM_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define BCM_print(p,    pfx,hLOG) BCM_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_PBTcmEntry
#define h_PBTcmEntry (){}

    #define     BA_PBTcmEntry_dat                              0x0000
    #define     B16PBTcmEntry_dat                              0x0000
    #define   LSb32PBTcmEntry_dat                                 0
    #define   LSb16PBTcmEntry_dat                                 0
    #define       bPBTcmEntry_dat                              32
    #define   MSK32PBTcmEntry_dat                                 0xFFFFFFFF
    ///////////////////////////////////////////////////////////

    typedef struct SIE_PBTcmEntry {
    ///////////////////////////////////////////////////////////
    #define   GET32PBTcmEntry_dat(r32)                         _BFGET_(r32,31, 0)
    #define   SET32PBTcmEntry_dat(r32,v)                       _BFSET_(r32,31, 0,v)

            UNSG32 u_dat                                       : 32;
    ///////////////////////////////////////////////////////////
    } SIE_PBTcmEntry;

    ///////////////////////////////////////////////////////////
     SIGN32 PBTcmEntry_drvrd(SIE_PBTcmEntry *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 PBTcmEntry_drvwr(SIE_PBTcmEntry *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void PBTcmEntry_reset(SIE_PBTcmEntry *p);
     SIGN32 PBTcmEntry_cmp  (SIE_PBTcmEntry *p, SIE_PBTcmEntry *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define PBTcmEntry_check(p,pie,pfx,hLOG) PBTcmEntry_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define PBTcmEntry_print(p,    pfx,hLOG) PBTcmEntry_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_pBridge
#define h_pBridge (){}

    #define     RA_pBridge_tcm                                 0x0000
    ///////////////////////////////////////////////////////////
    #define     RA_pBridge_dHub                                0x0800
    ///////////////////////////////////////////////////////////
    #define     RA_pBridge_BCM                                 0x1800
    ///////////////////////////////////////////////////////////

    typedef struct SIE_pBridge {
    ///////////////////////////////////////////////////////////
              SIE_PBTcmEntry                                   ie_tcm[512];
    ///////////////////////////////////////////////////////////
              SIE_dHubReg2D                                    ie_dHub;
    ///////////////////////////////////////////////////////////
              SIE_BCM                                          ie_BCM;
    ///////////////////////////////////////////////////////////
             UNSG8 RSVDx180C                                   [2036];
    ///////////////////////////////////////////////////////////
    } SIE_pBridge;

    ///////////////////////////////////////////////////////////
     SIGN32 pBridge_drvrd(SIE_pBridge *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 pBridge_drvwr(SIE_pBridge *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void pBridge_reset(SIE_pBridge *p);
     SIGN32 pBridge_cmp  (SIE_pBridge *p, SIE_pBridge *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define pBridge_check(p,pie,pfx,hLOG) pBridge_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define pBridge_print(p,    pfx,hLOG) pBridge_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif

#ifndef h_PBSemaMap
#define h_PBSemaMap (){}

    #define     BA_PBSemaMap_dHubSemID                         0x0000
    #define     B16PBSemaMap_dHubSemID                         0x0000
    #define   LSb32PBSemaMap_dHubSemID                            0
    #define   LSb16PBSemaMap_dHubSemID                            0
    #define       bPBSemaMap_dHubSemID                         5
    #define   MSK32PBSemaMap_dHubSemID                            0x0000001F
    #define        PBSemaMap_dHubSemID_dHubCh0_intr                         0x0
    #define        PBSemaMap_dHubSemID_dHubCh1_intr                         0x1
    #define        PBSemaMap_dHubSemID_dHubCh2_intr                         0x2
    #define        PBSemaMap_dHubSemID_dHubCh3_intr                         0x3
    #define        PBSemaMap_dHubSemID_dHubCh4_intr                         0x4
    #define        PBSemaMap_dHubSemID_dHubCh5_intr                         0x5
    #define        PBSemaMap_dHubSemID_dHubCh6_intr                         0x6
    #define        PBSemaMap_dHubSemID_dHubCh7_intr                         0x7
    #define        PBSemaMap_dHubSemID_dHubCh8_intr                         0x8
    #define        PBSemaMap_dHubSemID_AHBMST_intr                          0x9
    #define        PBSemaMap_dHubSemID_dHub_LBCCmd                          0xA
    #define        PBSemaMap_dHubSemID_dHub_LBCDat                          0xB
    #define        PBSemaMap_dHubSemID_dHub_NFCCmd                          0xC
    #define        PBSemaMap_dHubSemID_dHub_NFCDat                          0xD
    #define        PBSemaMap_dHubSemID_dHub_APBTx0                          0xE
    #define        PBSemaMap_dHubSemID_dHub_APBRx0                          0xF
    #define        PBSemaMap_dHubSemID_dHub_APBTx1                          0x10
    #define        PBSemaMap_dHubSemID_dHub_APBRx1                          0x11
    #define        PBSemaMap_dHubSemID_dHub_APBTx2                          0x12
    #define        PBSemaMap_dHubSemID_dHub_APBRx2                          0x13
    #define        PBSemaMap_dHubSemID_CPU_dHubCh0                          0x14
    #define        PBSemaMap_dHubSemID_CPU_dHubCh1                          0x15
    #define        PBSemaMap_dHubSemID_CPU_dHubCh2                          0x16
    #define        PBSemaMap_dHubSemID_LBC_DATA_CP                          0x17
    #define        PBSemaMap_dHubSemID_NFC_DATA_CP                          0x18
    #define        PBSemaMap_dHubSemID_APB0_DATA_CP                         0x19
    #define        PBSemaMap_dHubSemID_APB1_DATA_CP                         0x1A
    #define        PBSemaMap_dHubSemID_APB2_DATA_CP                         0x1B
    ///////////////////////////////////////////////////////////

    typedef struct SIE_PBSemaMap {
    ///////////////////////////////////////////////////////////
    #define   GET32PBSemaMap_dHubSemID(r32)                    _BFGET_(r32, 4, 0)
    #define   SET32PBSemaMap_dHubSemID(r32,v)                  _BFSET_(r32, 4, 0,v)
    #define   GET16PBSemaMap_dHubSemID(r16)                    _BFGET_(r16, 4, 0)
    #define   SET16PBSemaMap_dHubSemID(r16,v)                  _BFSET_(r16, 4, 0,v)

            UNSG32 u_dHubSemID                                 :  5;
            UNSG32 RSVDx0_b5                                   : 27;
    ///////////////////////////////////////////////////////////
    } SIE_PBSemaMap;

    ///////////////////////////////////////////////////////////
     SIGN32 PBSemaMap_drvrd(SIE_PBSemaMap *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 PBSemaMap_drvwr(SIE_PBSemaMap *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void PBSemaMap_reset(SIE_PBSemaMap *p);
     SIGN32 PBSemaMap_cmp  (SIE_PBSemaMap *p, SIE_PBSemaMap *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define PBSemaMap_check(p,pie,pfx,hLOG) PBSemaMap_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define PBSemaMap_print(p,    pfx,hLOG) PBSemaMap_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif

