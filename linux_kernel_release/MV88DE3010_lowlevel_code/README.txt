Please use mk_llcode.sh script under MV88DE3010_lowlevel_code dir to generate MV88DE3010 low level code.
This README lists the GaloisSoftware dir should contains the following codes:

1. only $Common  Drivers  OSAL" is included
[yfzheng@sc-pc557 galois_code]$ ls GaloisSoftware/
Common  Drivers  OSAL

2. the subdir structure
GaloisSoftware_090928/Common:
include source 

GaloisSoftware_090928/Drivers:
Galois_SoC

GaloisSoftware_090928/OSAL:
include  source


3. the Galois_SoC... dir
GaloisSoftware_090928/Drivers/Galois_SoC:
APB  Common  DEV_INIT  DHUB  GPIO  MISC  PINMUX

GaloisSoftware_090928/Common/include:
avxbv_buf.h     const.h          errcode_bdg.h    errcode_dvdspu.h  errcode_net.h     errcode_vpp.h   idlist.h      soc_watchdog.h
BoundChecker.h  dbg_internal.h   errcode_bd.h     errcode_gfx.h     errcode_ou.h      ErrorCode.h     interface.h   std_list.h
cmd_buf.h       dbg_settings.h   errcode_debug.h  errcode_hddvd.h   errcode_pe.h      galois_alloc.h  ir_key_def.h  std_map.h
com_config.h    debug.h          errcode_dmx.h    errcode_hdi.h     errcode_system.h  galois_queue.h  MV_SHM_def.h  std_string.h
com_type.h      errcode_audio.h  errcode_dom.h    errcode_hdmv.h    errcode_vfd.h     GaloisTime.h    pe_common.h   Trace.h

GaloisSoftware_090928/OSAL/include:
CPU0  CPU2              MV_SHM_def.h  OSAL_config.h  OSAL_type.h                     RPC_UseCaseTemplet_Client_new.h  testtool.h
CPU1  MV_CC_GSConfig.h  OSAL_api.h    OSAL_debug.h   RPCConfig_UseCaseTemplet_new.h  RPC_UseCaseTemplet_Server_new.h

GaloisSoftware_090928/OSAL/source:
CC_CBuf_new.c       CCProtocol.c       CC_RPC_TaskPool.h  Galois_Linux         ICCHAL.h      MsgQEx_ITC.c      MV_CC_GSConfig.o                testtool.c
CC_CBuf_type.h      CCProtocol.h       CC_RPC_type.h      Galois_Linux_Kernel  ICCNet.h      MsgQEx_ITC.h      Orion_eCos
CC_DSS_Client.c     CCPrTest.c         CCUDP.c            ICCFIFOArray.c       Linux         MsgQ.h            OSAL.c
CC_DSS.h            CCPrTest.h         CCUDP.h            ICCFIFOArray.h       MidSrv.c      MsgQ_ICC.c        PCSim_eCos
CC_DSS_Server.c     CC_RPC_ClntPool.c  CCUDPTest.c        ICCFIFOArray.o       MidSrv.h      MsgQ_ICC.h        PCSim_Linux
CC_MsgQEx.c         CC_RPC_ClntPool.h  CCUDPTest.h        ICCFIFO.c            MsgQ.c        MsgQ_ITC.c        PC_Win32
CC_MsgQ_Internal.h  CC_RPC_new.c       eCos               ICCFIFO.h            MsgQEx_ICC.c  MsgQ_ITC.h        RPCConfig_UseCaseTemplet_new.c
CC_MsgQ_new.c       CC_RPC_TaskPool.c  Galois_eCos        ICCFIFO.o            MsgQEx_ICC.h  MV_CC_GSConfig.c  RPC_UseCaseTemplet_new.c

