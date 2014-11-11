#ifndef pwm_h
#define pwm_h (){}


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



#ifndef h_PWM
#define h_PWM (){}

    #define     RA_PWM_pwmCh0En                                0x0000

    #define     BA_PWM_pwmCh0En_pwmCh0En                       0x0000
    #define     B16PWM_pwmCh0En_pwmCh0En                       0x0000
    #define   LSb32PWM_pwmCh0En_pwmCh0En                          0
    #define   LSb16PWM_pwmCh0En_pwmCh0En                          0
    #define       bPWM_pwmCh0En_pwmCh0En                       1
    #define   MSK32PWM_pwmCh0En_pwmCh0En                          0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh0Ctrl                              0x0004

    #define     BA_PWM_pwmCh0Ctrl_pwmCh0PreScale               0x0004
    #define     B16PWM_pwmCh0Ctrl_pwmCh0PreScale               0x0004
    #define   LSb32PWM_pwmCh0Ctrl_pwmCh0PreScale                  0
    #define   LSb16PWM_pwmCh0Ctrl_pwmCh0PreScale                  0
    #define       bPWM_pwmCh0Ctrl_pwmCh0PreScale               3
    #define   MSK32PWM_pwmCh0Ctrl_pwmCh0PreScale                  0x00000007

    #define     BA_PWM_pwmCh0Ctrl_pwmCh0Pol                    0x0004
    #define     B16PWM_pwmCh0Ctrl_pwmCh0Pol                    0x0004
    #define   LSb32PWM_pwmCh0Ctrl_pwmCh0Pol                       3
    #define   LSb16PWM_pwmCh0Ctrl_pwmCh0Pol                       3
    #define       bPWM_pwmCh0Ctrl_pwmCh0Pol                    1
    #define   MSK32PWM_pwmCh0Ctrl_pwmCh0Pol                       0x00000008
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh0Duty                              0x0008

    #define     BA_PWM_pwmCh0Duty_pwmCh0Duty                   0x0008
    #define     B16PWM_pwmCh0Duty_pwmCh0Duty                   0x0008
    #define   LSb32PWM_pwmCh0Duty_pwmCh0Duty                      0
    #define   LSb16PWM_pwmCh0Duty_pwmCh0Duty                      0
    #define       bPWM_pwmCh0Duty_pwmCh0Duty                   16
    #define   MSK32PWM_pwmCh0Duty_pwmCh0Duty                      0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh0TCnt                              0x000C

    #define     BA_PWM_pwmCh0TCnt_pwmCh0TCnt                   0x000C
    #define     B16PWM_pwmCh0TCnt_pwmCh0TCnt                   0x000C
    #define   LSb32PWM_pwmCh0TCnt_pwmCh0TCnt                      0
    #define   LSb16PWM_pwmCh0TCnt_pwmCh0TCnt                      0
    #define       bPWM_pwmCh0TCnt_pwmCh0TCnt                   16
    #define   MSK32PWM_pwmCh0TCnt_pwmCh0TCnt                      0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh1En                                0x0010

    #define     BA_PWM_pwmCh1En_pwmCh1En                       0x0010
    #define     B16PWM_pwmCh1En_pwmCh1En                       0x0010
    #define   LSb32PWM_pwmCh1En_pwmCh1En                          0
    #define   LSb16PWM_pwmCh1En_pwmCh1En                          0
    #define       bPWM_pwmCh1En_pwmCh1En                       1
    #define   MSK32PWM_pwmCh1En_pwmCh1En                          0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh1Ctrl                              0x0014

    #define     BA_PWM_pwmCh1Ctrl_pwmCh1PreScale               0x0014
    #define     B16PWM_pwmCh1Ctrl_pwmCh1PreScale               0x0014
    #define   LSb32PWM_pwmCh1Ctrl_pwmCh1PreScale                  0
    #define   LSb16PWM_pwmCh1Ctrl_pwmCh1PreScale                  0
    #define       bPWM_pwmCh1Ctrl_pwmCh1PreScale               3
    #define   MSK32PWM_pwmCh1Ctrl_pwmCh1PreScale                  0x00000007

    #define     BA_PWM_pwmCh1Ctrl_pwmCh1Pol                    0x0014
    #define     B16PWM_pwmCh1Ctrl_pwmCh1Pol                    0x0014
    #define   LSb32PWM_pwmCh1Ctrl_pwmCh1Pol                       3
    #define   LSb16PWM_pwmCh1Ctrl_pwmCh1Pol                       3
    #define       bPWM_pwmCh1Ctrl_pwmCh1Pol                    1
    #define   MSK32PWM_pwmCh1Ctrl_pwmCh1Pol                       0x00000008
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh1Duty                              0x0018

    #define     BA_PWM_pwmCh1Duty_pwmCh1Duty                   0x0018
    #define     B16PWM_pwmCh1Duty_pwmCh1Duty                   0x0018
    #define   LSb32PWM_pwmCh1Duty_pwmCh1Duty                      0
    #define   LSb16PWM_pwmCh1Duty_pwmCh1Duty                      0
    #define       bPWM_pwmCh1Duty_pwmCh1Duty                   16
    #define   MSK32PWM_pwmCh1Duty_pwmCh1Duty                      0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh1TCnt                              0x001C

    #define     BA_PWM_pwmCh1TCnt_pwmCh1TCnt                   0x001C
    #define     B16PWM_pwmCh1TCnt_pwmCh1TCnt                   0x001C
    #define   LSb32PWM_pwmCh1TCnt_pwmCh1TCnt                      0
    #define   LSb16PWM_pwmCh1TCnt_pwmCh1TCnt                      0
    #define       bPWM_pwmCh1TCnt_pwmCh1TCnt                   16
    #define   MSK32PWM_pwmCh1TCnt_pwmCh1TCnt                      0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh2En                                0x0020

    #define     BA_PWM_pwmCh2En_pwmCh2En                       0x0020
    #define     B16PWM_pwmCh2En_pwmCh2En                       0x0020
    #define   LSb32PWM_pwmCh2En_pwmCh2En                          0
    #define   LSb16PWM_pwmCh2En_pwmCh2En                          0
    #define       bPWM_pwmCh2En_pwmCh2En                       1
    #define   MSK32PWM_pwmCh2En_pwmCh2En                          0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh2Ctrl                              0x0024

    #define     BA_PWM_pwmCh2Ctrl_pwmCh2PreScale               0x0024
    #define     B16PWM_pwmCh2Ctrl_pwmCh2PreScale               0x0024
    #define   LSb32PWM_pwmCh2Ctrl_pwmCh2PreScale                  0
    #define   LSb16PWM_pwmCh2Ctrl_pwmCh2PreScale                  0
    #define       bPWM_pwmCh2Ctrl_pwmCh2PreScale               3
    #define   MSK32PWM_pwmCh2Ctrl_pwmCh2PreScale                  0x00000007

    #define     BA_PWM_pwmCh2Ctrl_pwmCh2Pol                    0x0024
    #define     B16PWM_pwmCh2Ctrl_pwmCh2Pol                    0x0024
    #define   LSb32PWM_pwmCh2Ctrl_pwmCh2Pol                       3
    #define   LSb16PWM_pwmCh2Ctrl_pwmCh2Pol                       3
    #define       bPWM_pwmCh2Ctrl_pwmCh2Pol                    1
    #define   MSK32PWM_pwmCh2Ctrl_pwmCh2Pol                       0x00000008
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh2Duty                              0x0028

    #define     BA_PWM_pwmCh2Duty_pwmCh2Duty                   0x0028
    #define     B16PWM_pwmCh2Duty_pwmCh2Duty                   0x0028
    #define   LSb32PWM_pwmCh2Duty_pwmCh2Duty                      0
    #define   LSb16PWM_pwmCh2Duty_pwmCh2Duty                      0
    #define       bPWM_pwmCh2Duty_pwmCh2Duty                   16
    #define   MSK32PWM_pwmCh2Duty_pwmCh2Duty                      0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh2TCnt                              0x002C

    #define     BA_PWM_pwmCh2TCnt_pwmCh2TCnt                   0x002C
    #define     B16PWM_pwmCh2TCnt_pwmCh2TCnt                   0x002C
    #define   LSb32PWM_pwmCh2TCnt_pwmCh2TCnt                      0
    #define   LSb16PWM_pwmCh2TCnt_pwmCh2TCnt                      0
    #define       bPWM_pwmCh2TCnt_pwmCh2TCnt                   16
    #define   MSK32PWM_pwmCh2TCnt_pwmCh2TCnt                      0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh3En                                0x0030

    #define     BA_PWM_pwmCh3En_pwmCh3En                       0x0030
    #define     B16PWM_pwmCh3En_pwmCh3En                       0x0030
    #define   LSb32PWM_pwmCh3En_pwmCh3En                          0
    #define   LSb16PWM_pwmCh3En_pwmCh3En                          0
    #define       bPWM_pwmCh3En_pwmCh3En                       1
    #define   MSK32PWM_pwmCh3En_pwmCh3En                          0x00000001
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh3Ctrl                              0x0034

    #define     BA_PWM_pwmCh3Ctrl_pwmCh3PreScale               0x0034
    #define     B16PWM_pwmCh3Ctrl_pwmCh3PreScale               0x0034
    #define   LSb32PWM_pwmCh3Ctrl_pwmCh3PreScale                  0
    #define   LSb16PWM_pwmCh3Ctrl_pwmCh3PreScale                  0
    #define       bPWM_pwmCh3Ctrl_pwmCh3PreScale               3
    #define   MSK32PWM_pwmCh3Ctrl_pwmCh3PreScale                  0x00000007

    #define     BA_PWM_pwmCh3Ctrl_pwmCh3Pol                    0x0034
    #define     B16PWM_pwmCh3Ctrl_pwmCh3Pol                    0x0034
    #define   LSb32PWM_pwmCh3Ctrl_pwmCh3Pol                       3
    #define   LSb16PWM_pwmCh3Ctrl_pwmCh3Pol                       3
    #define       bPWM_pwmCh3Ctrl_pwmCh3Pol                    1
    #define   MSK32PWM_pwmCh3Ctrl_pwmCh3Pol                       0x00000008
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh3Duty                              0x0038

    #define     BA_PWM_pwmCh3Duty_pwmCh3Duty                   0x0038
    #define     B16PWM_pwmCh3Duty_pwmCh3Duty                   0x0038
    #define   LSb32PWM_pwmCh3Duty_pwmCh3Duty                      0
    #define   LSb16PWM_pwmCh3Duty_pwmCh3Duty                      0
    #define       bPWM_pwmCh3Duty_pwmCh3Duty                   16
    #define   MSK32PWM_pwmCh3Duty_pwmCh3Duty                      0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh3TCnt                              0x003C

    #define     BA_PWM_pwmCh3TCnt_pwmCh3TCnt                   0x003C
    #define     B16PWM_pwmCh3TCnt_pwmCh3TCnt                   0x003C
    #define   LSb32PWM_pwmCh3TCnt_pwmCh3TCnt                      0
    #define   LSb16PWM_pwmCh3TCnt_pwmCh3TCnt                      0
    #define       bPWM_pwmCh3TCnt_pwmCh3TCnt                   16
    #define   MSK32PWM_pwmCh3TCnt_pwmCh3TCnt                      0x0000FFFF
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh01Ctr                              0x0040

    #define     BA_PWM_pwmCh01Ctr_pwmCh0Ctr                    0x0040
    #define     B16PWM_pwmCh01Ctr_pwmCh0Ctr                    0x0040
    #define   LSb32PWM_pwmCh01Ctr_pwmCh0Ctr                       0
    #define   LSb16PWM_pwmCh01Ctr_pwmCh0Ctr                       0
    #define       bPWM_pwmCh01Ctr_pwmCh0Ctr                    16
    #define   MSK32PWM_pwmCh01Ctr_pwmCh0Ctr                       0x0000FFFF

    #define     BA_PWM_pwmCh01Ctr_pwmCh1Ctr                    0x0042
    #define     B16PWM_pwmCh01Ctr_pwmCh1Ctr                    0x0042
    #define   LSb32PWM_pwmCh01Ctr_pwmCh1Ctr                       16
    #define   LSb16PWM_pwmCh01Ctr_pwmCh1Ctr                       0
    #define       bPWM_pwmCh01Ctr_pwmCh1Ctr                    16
    #define   MSK32PWM_pwmCh01Ctr_pwmCh1Ctr                       0xFFFF0000
    ///////////////////////////////////////////////////////////
    #define     RA_PWM_pwmCh23Ctr                              0x0044

    #define     BA_PWM_pwmCh23Ctr_pwmCh2Ctr                    0x0044
    #define     B16PWM_pwmCh23Ctr_pwmCh2Ctr                    0x0044
    #define   LSb32PWM_pwmCh23Ctr_pwmCh2Ctr                       0
    #define   LSb16PWM_pwmCh23Ctr_pwmCh2Ctr                       0
    #define       bPWM_pwmCh23Ctr_pwmCh2Ctr                    16
    #define   MSK32PWM_pwmCh23Ctr_pwmCh2Ctr                       0x0000FFFF

    #define     BA_PWM_pwmCh23Ctr_pwmCh3Ctr                    0x0046
    #define     B16PWM_pwmCh23Ctr_pwmCh3Ctr                    0x0046
    #define   LSb32PWM_pwmCh23Ctr_pwmCh3Ctr                       16
    #define   LSb16PWM_pwmCh23Ctr_pwmCh3Ctr                       0
    #define       bPWM_pwmCh23Ctr_pwmCh3Ctr                    16
    #define   MSK32PWM_pwmCh23Ctr_pwmCh3Ctr                       0xFFFF0000
    ///////////////////////////////////////////////////////////

    typedef struct SIE_PWM {
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh0En_pwmCh0En(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32PWM_pwmCh0En_pwmCh0En(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16PWM_pwmCh0En_pwmCh0En(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16PWM_pwmCh0En_pwmCh0En(r16,v)                _BFSET_(r16, 0, 0,v)

    #define     w32PWM_pwmCh0En                                {\
            UNSG32 upwmCh0En_pwmCh0En                          :  1;\
            UNSG32 RSVDx0_b1                                   : 31;\
          }
    union { UNSG32 u32PWM_pwmCh0En;
            struct w32PWM_pwmCh0En;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh0Ctrl_pwmCh0PreScale(r32)          _BFGET_(r32, 2, 0)
    #define   SET32PWM_pwmCh0Ctrl_pwmCh0PreScale(r32,v)        _BFSET_(r32, 2, 0,v)
    #define   GET16PWM_pwmCh0Ctrl_pwmCh0PreScale(r16)          _BFGET_(r16, 2, 0)
    #define   SET16PWM_pwmCh0Ctrl_pwmCh0PreScale(r16,v)        _BFSET_(r16, 2, 0,v)

    #define   GET32PWM_pwmCh0Ctrl_pwmCh0Pol(r32)               _BFGET_(r32, 3, 3)
    #define   SET32PWM_pwmCh0Ctrl_pwmCh0Pol(r32,v)             _BFSET_(r32, 3, 3,v)
    #define   GET16PWM_pwmCh0Ctrl_pwmCh0Pol(r16)               _BFGET_(r16, 3, 3)
    #define   SET16PWM_pwmCh0Ctrl_pwmCh0Pol(r16,v)             _BFSET_(r16, 3, 3,v)

    #define     w32PWM_pwmCh0Ctrl                              {\
            UNSG32 upwmCh0Ctrl_pwmCh0PreScale                  :  3;\
            UNSG32 upwmCh0Ctrl_pwmCh0Pol                       :  1;\
            UNSG32 RSVDx4_b4                                   : 28;\
          }
    union { UNSG32 u32PWM_pwmCh0Ctrl;
            struct w32PWM_pwmCh0Ctrl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh0Duty_pwmCh0Duty(r32)              _BFGET_(r32,15, 0)
    #define   SET32PWM_pwmCh0Duty_pwmCh0Duty(r32,v)            _BFSET_(r32,15, 0,v)
    #define   GET16PWM_pwmCh0Duty_pwmCh0Duty(r16)              _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh0Duty_pwmCh0Duty(r16,v)            _BFSET_(r16,15, 0,v)

    #define     w32PWM_pwmCh0Duty                              {\
            UNSG32 upwmCh0Duty_pwmCh0Duty                      : 16;\
            UNSG32 RSVDx8_b16                                  : 16;\
          }
    union { UNSG32 u32PWM_pwmCh0Duty;
            struct w32PWM_pwmCh0Duty;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh0TCnt_pwmCh0TCnt(r32)              _BFGET_(r32,15, 0)
    #define   SET32PWM_pwmCh0TCnt_pwmCh0TCnt(r32,v)            _BFSET_(r32,15, 0,v)
    #define   GET16PWM_pwmCh0TCnt_pwmCh0TCnt(r16)              _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh0TCnt_pwmCh0TCnt(r16,v)            _BFSET_(r16,15, 0,v)

    #define     w32PWM_pwmCh0TCnt                              {\
            UNSG32 upwmCh0TCnt_pwmCh0TCnt                      : 16;\
            UNSG32 RSVDxC_b16                                  : 16;\
          }
    union { UNSG32 u32PWM_pwmCh0TCnt;
            struct w32PWM_pwmCh0TCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh1En_pwmCh1En(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32PWM_pwmCh1En_pwmCh1En(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16PWM_pwmCh1En_pwmCh1En(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16PWM_pwmCh1En_pwmCh1En(r16,v)                _BFSET_(r16, 0, 0,v)

    #define     w32PWM_pwmCh1En                                {\
            UNSG32 upwmCh1En_pwmCh1En                          :  1;\
            UNSG32 RSVDx10_b1                                  : 31;\
          }
    union { UNSG32 u32PWM_pwmCh1En;
            struct w32PWM_pwmCh1En;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh1Ctrl_pwmCh1PreScale(r32)          _BFGET_(r32, 2, 0)
    #define   SET32PWM_pwmCh1Ctrl_pwmCh1PreScale(r32,v)        _BFSET_(r32, 2, 0,v)
    #define   GET16PWM_pwmCh1Ctrl_pwmCh1PreScale(r16)          _BFGET_(r16, 2, 0)
    #define   SET16PWM_pwmCh1Ctrl_pwmCh1PreScale(r16,v)        _BFSET_(r16, 2, 0,v)

    #define   GET32PWM_pwmCh1Ctrl_pwmCh1Pol(r32)               _BFGET_(r32, 3, 3)
    #define   SET32PWM_pwmCh1Ctrl_pwmCh1Pol(r32,v)             _BFSET_(r32, 3, 3,v)
    #define   GET16PWM_pwmCh1Ctrl_pwmCh1Pol(r16)               _BFGET_(r16, 3, 3)
    #define   SET16PWM_pwmCh1Ctrl_pwmCh1Pol(r16,v)             _BFSET_(r16, 3, 3,v)

    #define     w32PWM_pwmCh1Ctrl                              {\
            UNSG32 upwmCh1Ctrl_pwmCh1PreScale                  :  3;\
            UNSG32 upwmCh1Ctrl_pwmCh1Pol                       :  1;\
            UNSG32 RSVDx14_b4                                  : 28;\
          }
    union { UNSG32 u32PWM_pwmCh1Ctrl;
            struct w32PWM_pwmCh1Ctrl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh1Duty_pwmCh1Duty(r32)              _BFGET_(r32,15, 0)
    #define   SET32PWM_pwmCh1Duty_pwmCh1Duty(r32,v)            _BFSET_(r32,15, 0,v)
    #define   GET16PWM_pwmCh1Duty_pwmCh1Duty(r16)              _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh1Duty_pwmCh1Duty(r16,v)            _BFSET_(r16,15, 0,v)

    #define     w32PWM_pwmCh1Duty                              {\
            UNSG32 upwmCh1Duty_pwmCh1Duty                      : 16;\
            UNSG32 RSVDx18_b16                                 : 16;\
          }
    union { UNSG32 u32PWM_pwmCh1Duty;
            struct w32PWM_pwmCh1Duty;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh1TCnt_pwmCh1TCnt(r32)              _BFGET_(r32,15, 0)
    #define   SET32PWM_pwmCh1TCnt_pwmCh1TCnt(r32,v)            _BFSET_(r32,15, 0,v)
    #define   GET16PWM_pwmCh1TCnt_pwmCh1TCnt(r16)              _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh1TCnt_pwmCh1TCnt(r16,v)            _BFSET_(r16,15, 0,v)

    #define     w32PWM_pwmCh1TCnt                              {\
            UNSG32 upwmCh1TCnt_pwmCh1TCnt                      : 16;\
            UNSG32 RSVDx1C_b16                                 : 16;\
          }
    union { UNSG32 u32PWM_pwmCh1TCnt;
            struct w32PWM_pwmCh1TCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh2En_pwmCh2En(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32PWM_pwmCh2En_pwmCh2En(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16PWM_pwmCh2En_pwmCh2En(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16PWM_pwmCh2En_pwmCh2En(r16,v)                _BFSET_(r16, 0, 0,v)

    #define     w32PWM_pwmCh2En                                {\
            UNSG32 upwmCh2En_pwmCh2En                          :  1;\
            UNSG32 RSVDx20_b1                                  : 31;\
          }
    union { UNSG32 u32PWM_pwmCh2En;
            struct w32PWM_pwmCh2En;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh2Ctrl_pwmCh2PreScale(r32)          _BFGET_(r32, 2, 0)
    #define   SET32PWM_pwmCh2Ctrl_pwmCh2PreScale(r32,v)        _BFSET_(r32, 2, 0,v)
    #define   GET16PWM_pwmCh2Ctrl_pwmCh2PreScale(r16)          _BFGET_(r16, 2, 0)
    #define   SET16PWM_pwmCh2Ctrl_pwmCh2PreScale(r16,v)        _BFSET_(r16, 2, 0,v)

    #define   GET32PWM_pwmCh2Ctrl_pwmCh2Pol(r32)               _BFGET_(r32, 3, 3)
    #define   SET32PWM_pwmCh2Ctrl_pwmCh2Pol(r32,v)             _BFSET_(r32, 3, 3,v)
    #define   GET16PWM_pwmCh2Ctrl_pwmCh2Pol(r16)               _BFGET_(r16, 3, 3)
    #define   SET16PWM_pwmCh2Ctrl_pwmCh2Pol(r16,v)             _BFSET_(r16, 3, 3,v)

    #define     w32PWM_pwmCh2Ctrl                              {\
            UNSG32 upwmCh2Ctrl_pwmCh2PreScale                  :  3;\
            UNSG32 upwmCh2Ctrl_pwmCh2Pol                       :  1;\
            UNSG32 RSVDx24_b4                                  : 28;\
          }
    union { UNSG32 u32PWM_pwmCh2Ctrl;
            struct w32PWM_pwmCh2Ctrl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh2Duty_pwmCh2Duty(r32)              _BFGET_(r32,15, 0)
    #define   SET32PWM_pwmCh2Duty_pwmCh2Duty(r32,v)            _BFSET_(r32,15, 0,v)
    #define   GET16PWM_pwmCh2Duty_pwmCh2Duty(r16)              _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh2Duty_pwmCh2Duty(r16,v)            _BFSET_(r16,15, 0,v)

    #define     w32PWM_pwmCh2Duty                              {\
            UNSG32 upwmCh2Duty_pwmCh2Duty                      : 16;\
            UNSG32 RSVDx28_b16                                 : 16;\
          }
    union { UNSG32 u32PWM_pwmCh2Duty;
            struct w32PWM_pwmCh2Duty;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh2TCnt_pwmCh2TCnt(r32)              _BFGET_(r32,15, 0)
    #define   SET32PWM_pwmCh2TCnt_pwmCh2TCnt(r32,v)            _BFSET_(r32,15, 0,v)
    #define   GET16PWM_pwmCh2TCnt_pwmCh2TCnt(r16)              _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh2TCnt_pwmCh2TCnt(r16,v)            _BFSET_(r16,15, 0,v)

    #define     w32PWM_pwmCh2TCnt                              {\
            UNSG32 upwmCh2TCnt_pwmCh2TCnt                      : 16;\
            UNSG32 RSVDx2C_b16                                 : 16;\
          }
    union { UNSG32 u32PWM_pwmCh2TCnt;
            struct w32PWM_pwmCh2TCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh3En_pwmCh3En(r32)                  _BFGET_(r32, 0, 0)
    #define   SET32PWM_pwmCh3En_pwmCh3En(r32,v)                _BFSET_(r32, 0, 0,v)
    #define   GET16PWM_pwmCh3En_pwmCh3En(r16)                  _BFGET_(r16, 0, 0)
    #define   SET16PWM_pwmCh3En_pwmCh3En(r16,v)                _BFSET_(r16, 0, 0,v)

    #define     w32PWM_pwmCh3En                                {\
            UNSG32 upwmCh3En_pwmCh3En                          :  1;\
            UNSG32 RSVDx30_b1                                  : 31;\
          }
    union { UNSG32 u32PWM_pwmCh3En;
            struct w32PWM_pwmCh3En;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh3Ctrl_pwmCh3PreScale(r32)          _BFGET_(r32, 2, 0)
    #define   SET32PWM_pwmCh3Ctrl_pwmCh3PreScale(r32,v)        _BFSET_(r32, 2, 0,v)
    #define   GET16PWM_pwmCh3Ctrl_pwmCh3PreScale(r16)          _BFGET_(r16, 2, 0)
    #define   SET16PWM_pwmCh3Ctrl_pwmCh3PreScale(r16,v)        _BFSET_(r16, 2, 0,v)

    #define   GET32PWM_pwmCh3Ctrl_pwmCh3Pol(r32)               _BFGET_(r32, 3, 3)
    #define   SET32PWM_pwmCh3Ctrl_pwmCh3Pol(r32,v)             _BFSET_(r32, 3, 3,v)
    #define   GET16PWM_pwmCh3Ctrl_pwmCh3Pol(r16)               _BFGET_(r16, 3, 3)
    #define   SET16PWM_pwmCh3Ctrl_pwmCh3Pol(r16,v)             _BFSET_(r16, 3, 3,v)

    #define     w32PWM_pwmCh3Ctrl                              {\
            UNSG32 upwmCh3Ctrl_pwmCh3PreScale                  :  3;\
            UNSG32 upwmCh3Ctrl_pwmCh3Pol                       :  1;\
            UNSG32 RSVDx34_b4                                  : 28;\
          }
    union { UNSG32 u32PWM_pwmCh3Ctrl;
            struct w32PWM_pwmCh3Ctrl;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh3Duty_pwmCh3Duty(r32)              _BFGET_(r32,15, 0)
    #define   SET32PWM_pwmCh3Duty_pwmCh3Duty(r32,v)            _BFSET_(r32,15, 0,v)
    #define   GET16PWM_pwmCh3Duty_pwmCh3Duty(r16)              _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh3Duty_pwmCh3Duty(r16,v)            _BFSET_(r16,15, 0,v)

    #define     w32PWM_pwmCh3Duty                              {\
            UNSG32 upwmCh3Duty_pwmCh3Duty                      : 16;\
            UNSG32 RSVDx38_b16                                 : 16;\
          }
    union { UNSG32 u32PWM_pwmCh3Duty;
            struct w32PWM_pwmCh3Duty;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh3TCnt_pwmCh3TCnt(r32)              _BFGET_(r32,15, 0)
    #define   SET32PWM_pwmCh3TCnt_pwmCh3TCnt(r32,v)            _BFSET_(r32,15, 0,v)
    #define   GET16PWM_pwmCh3TCnt_pwmCh3TCnt(r16)              _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh3TCnt_pwmCh3TCnt(r16,v)            _BFSET_(r16,15, 0,v)

    #define     w32PWM_pwmCh3TCnt                              {\
            UNSG32 upwmCh3TCnt_pwmCh3TCnt                      : 16;\
            UNSG32 RSVDx3C_b16                                 : 16;\
          }
    union { UNSG32 u32PWM_pwmCh3TCnt;
            struct w32PWM_pwmCh3TCnt;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh01Ctr_pwmCh0Ctr(r32)               _BFGET_(r32,15, 0)
    #define   SET32PWM_pwmCh01Ctr_pwmCh0Ctr(r32,v)             _BFSET_(r32,15, 0,v)
    #define   GET16PWM_pwmCh01Ctr_pwmCh0Ctr(r16)               _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh01Ctr_pwmCh0Ctr(r16,v)             _BFSET_(r16,15, 0,v)

    #define   GET32PWM_pwmCh01Ctr_pwmCh1Ctr(r32)               _BFGET_(r32,31,16)
    #define   SET32PWM_pwmCh01Ctr_pwmCh1Ctr(r32,v)             _BFSET_(r32,31,16,v)
    #define   GET16PWM_pwmCh01Ctr_pwmCh1Ctr(r16)               _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh01Ctr_pwmCh1Ctr(r16,v)             _BFSET_(r16,15, 0,v)

    #define     w32PWM_pwmCh01Ctr                              {\
            UNSG32 upwmCh01Ctr_pwmCh0Ctr                       : 16;\
            UNSG32 upwmCh01Ctr_pwmCh1Ctr                       : 16;\
          }
    union { UNSG32 u32PWM_pwmCh01Ctr;
            struct w32PWM_pwmCh01Ctr;
          };
    ///////////////////////////////////////////////////////////
    #define   GET32PWM_pwmCh23Ctr_pwmCh2Ctr(r32)               _BFGET_(r32,15, 0)
    #define   SET32PWM_pwmCh23Ctr_pwmCh2Ctr(r32,v)             _BFSET_(r32,15, 0,v)
    #define   GET16PWM_pwmCh23Ctr_pwmCh2Ctr(r16)               _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh23Ctr_pwmCh2Ctr(r16,v)             _BFSET_(r16,15, 0,v)

    #define   GET32PWM_pwmCh23Ctr_pwmCh3Ctr(r32)               _BFGET_(r32,31,16)
    #define   SET32PWM_pwmCh23Ctr_pwmCh3Ctr(r32,v)             _BFSET_(r32,31,16,v)
    #define   GET16PWM_pwmCh23Ctr_pwmCh3Ctr(r16)               _BFGET_(r16,15, 0)
    #define   SET16PWM_pwmCh23Ctr_pwmCh3Ctr(r16,v)             _BFSET_(r16,15, 0,v)

    #define     w32PWM_pwmCh23Ctr                              {\
            UNSG32 upwmCh23Ctr_pwmCh2Ctr                       : 16;\
            UNSG32 upwmCh23Ctr_pwmCh3Ctr                       : 16;\
          }
    union { UNSG32 u32PWM_pwmCh23Ctr;
            struct w32PWM_pwmCh23Ctr;
          };
    ///////////////////////////////////////////////////////////
    } SIE_PWM;

    typedef union  T32PWM_pwmCh0En
          { UNSG32 u32;
            struct w32PWM_pwmCh0En;
                 } T32PWM_pwmCh0En;
    typedef union  T32PWM_pwmCh0Ctrl
          { UNSG32 u32;
            struct w32PWM_pwmCh0Ctrl;
                 } T32PWM_pwmCh0Ctrl;
    typedef union  T32PWM_pwmCh0Duty
          { UNSG32 u32;
            struct w32PWM_pwmCh0Duty;
                 } T32PWM_pwmCh0Duty;
    typedef union  T32PWM_pwmCh0TCnt
          { UNSG32 u32;
            struct w32PWM_pwmCh0TCnt;
                 } T32PWM_pwmCh0TCnt;
    typedef union  T32PWM_pwmCh1En
          { UNSG32 u32;
            struct w32PWM_pwmCh1En;
                 } T32PWM_pwmCh1En;
    typedef union  T32PWM_pwmCh1Ctrl
          { UNSG32 u32;
            struct w32PWM_pwmCh1Ctrl;
                 } T32PWM_pwmCh1Ctrl;
    typedef union  T32PWM_pwmCh1Duty
          { UNSG32 u32;
            struct w32PWM_pwmCh1Duty;
                 } T32PWM_pwmCh1Duty;
    typedef union  T32PWM_pwmCh1TCnt
          { UNSG32 u32;
            struct w32PWM_pwmCh1TCnt;
                 } T32PWM_pwmCh1TCnt;
    typedef union  T32PWM_pwmCh2En
          { UNSG32 u32;
            struct w32PWM_pwmCh2En;
                 } T32PWM_pwmCh2En;
    typedef union  T32PWM_pwmCh2Ctrl
          { UNSG32 u32;
            struct w32PWM_pwmCh2Ctrl;
                 } T32PWM_pwmCh2Ctrl;
    typedef union  T32PWM_pwmCh2Duty
          { UNSG32 u32;
            struct w32PWM_pwmCh2Duty;
                 } T32PWM_pwmCh2Duty;
    typedef union  T32PWM_pwmCh2TCnt
          { UNSG32 u32;
            struct w32PWM_pwmCh2TCnt;
                 } T32PWM_pwmCh2TCnt;
    typedef union  T32PWM_pwmCh3En
          { UNSG32 u32;
            struct w32PWM_pwmCh3En;
                 } T32PWM_pwmCh3En;
    typedef union  T32PWM_pwmCh3Ctrl
          { UNSG32 u32;
            struct w32PWM_pwmCh3Ctrl;
                 } T32PWM_pwmCh3Ctrl;
    typedef union  T32PWM_pwmCh3Duty
          { UNSG32 u32;
            struct w32PWM_pwmCh3Duty;
                 } T32PWM_pwmCh3Duty;
    typedef union  T32PWM_pwmCh3TCnt
          { UNSG32 u32;
            struct w32PWM_pwmCh3TCnt;
                 } T32PWM_pwmCh3TCnt;
    typedef union  T32PWM_pwmCh01Ctr
          { UNSG32 u32;
            struct w32PWM_pwmCh01Ctr;
                 } T32PWM_pwmCh01Ctr;
    typedef union  T32PWM_pwmCh23Ctr
          { UNSG32 u32;
            struct w32PWM_pwmCh23Ctr;
                 } T32PWM_pwmCh23Ctr;
    ///////////////////////////////////////////////////////////

    typedef union  TPWM_pwmCh0En
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh0En;
                   };
                 } TPWM_pwmCh0En;
    typedef union  TPWM_pwmCh0Ctrl
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh0Ctrl;
                   };
                 } TPWM_pwmCh0Ctrl;
    typedef union  TPWM_pwmCh0Duty
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh0Duty;
                   };
                 } TPWM_pwmCh0Duty;
    typedef union  TPWM_pwmCh0TCnt
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh0TCnt;
                   };
                 } TPWM_pwmCh0TCnt;
    typedef union  TPWM_pwmCh1En
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh1En;
                   };
                 } TPWM_pwmCh1En;
    typedef union  TPWM_pwmCh1Ctrl
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh1Ctrl;
                   };
                 } TPWM_pwmCh1Ctrl;
    typedef union  TPWM_pwmCh1Duty
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh1Duty;
                   };
                 } TPWM_pwmCh1Duty;
    typedef union  TPWM_pwmCh1TCnt
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh1TCnt;
                   };
                 } TPWM_pwmCh1TCnt;
    typedef union  TPWM_pwmCh2En
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh2En;
                   };
                 } TPWM_pwmCh2En;
    typedef union  TPWM_pwmCh2Ctrl
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh2Ctrl;
                   };
                 } TPWM_pwmCh2Ctrl;
    typedef union  TPWM_pwmCh2Duty
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh2Duty;
                   };
                 } TPWM_pwmCh2Duty;
    typedef union  TPWM_pwmCh2TCnt
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh2TCnt;
                   };
                 } TPWM_pwmCh2TCnt;
    typedef union  TPWM_pwmCh3En
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh3En;
                   };
                 } TPWM_pwmCh3En;
    typedef union  TPWM_pwmCh3Ctrl
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh3Ctrl;
                   };
                 } TPWM_pwmCh3Ctrl;
    typedef union  TPWM_pwmCh3Duty
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh3Duty;
                   };
                 } TPWM_pwmCh3Duty;
    typedef union  TPWM_pwmCh3TCnt
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh3TCnt;
                   };
                 } TPWM_pwmCh3TCnt;
    typedef union  TPWM_pwmCh01Ctr
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh01Ctr;
                   };
                 } TPWM_pwmCh01Ctr;
    typedef union  TPWM_pwmCh23Ctr
          { UNSG32 u32[1];
            struct {
            struct w32PWM_pwmCh23Ctr;
                   };
                 } TPWM_pwmCh23Ctr;

    ///////////////////////////////////////////////////////////
     SIGN32 PWM_drvrd(SIE_PWM *p, UNSG32 base, SIGN32 mem, SIGN32 tst);
     SIGN32 PWM_drvwr(SIE_PWM *p, UNSG32 base, SIGN32 mem, SIGN32 tst, UNSG32 *pcmd);
       void PWM_reset(SIE_PWM *p);
     SIGN32 PWM_cmp  (SIE_PWM *p, SIE_PWM *pie, char *pfx, void *hLOG, SIGN32 mem, SIGN32 tst);
    #define PWM_check(p,pie,pfx,hLOG) PWM_cmp(p,pie,pfx,(void*)(hLOG),0,0)
    #define PWM_print(p,    pfx,hLOG) PWM_cmp(p,0,  pfx,(void*)(hLOG),0,0)

#endif



#ifdef __cplusplus
  }
#endif
#pragma  pack()

#endif

