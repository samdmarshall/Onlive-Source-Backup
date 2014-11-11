######################################################
# Build configurations

include $(TOPDIR)/config

######################################################
# Module library

MODULE_LIBRARY = libOSAL.a

tmp_SoC = $(SoC)
ifeq ($(SoC), Berlin)
tmp_SoC = Galois
endif

ifeq ($(SINGLE_CPU), on)

MODULE_SRCS =   source_SingleCPU/OSAL.c \
                source_SingleCPU/ICCFIFO.c \
                source_SingleCPU/MsgQ.c \
                source_SingleCPU/MsgQ_ITC.c \
                source_SingleCPU/MsgQ_ICC.c \
                source_SingleCPU/CC_MsgQ.c \
                source_SingleCPU/MsgQEx_ITC.c \
                source_SingleCPU/MsgQEx_ICC.c \
                source_SingleCPU/CC_MsgQEx.c \
                source_SingleCPU/CC_RPC_TaskPool.c \
                source_SingleCPU/CC_RPC_ClntPool.c \
                source_SingleCPU/CC_RPC.c \
                source_SingleCPU/CC_CBuf.c \
                source_SingleCPU/RPC_UseCaseTemplet_new.c \
                source_SingleCPU/RPCConfig_UseCaseTemplet_new.c \
                source_SingleCPU/$(OS)/OSALSem.c \
                source_SingleCPU/$(OS)/OSALMutex.c \
                source_SingleCPU/$(OS)/OSALTask.c \
                source_SingleCPU/$(OS)/OSALTimer.c \
                source_SingleCPU/$(OS)/CCDSS.c \
                source_SingleCPU/$(OS)/CCUDP.c
                
else

MODULE_SRCS = $(wildcard source_MultiCPU/*.c) $(wildcard source_MultiCPU/$(tmp_SoC)_$(OS)/*.c)  $(wildcard source_MultiCPU/$(OS)/*.c)

endif

MODULE_OBJS = $(patsubst %.c,%.o,$(patsubst %.s,%.o,$(MODULE_SRCS)))

######################################################
# Header files

PUBLIC_HEADER_FILES = $(wildcard include/*.h) $(wildcard include/$(tmp_SoC)_$(OS)/*.h) $(wildcard include/CPU$(CPUINDEX)/*.h)

######################################################
# Module flags

MODULE_CFLAGS =  -I$(TOPDIR)/OSAL/include \
				 -I$(TOPDIR)/OSAL/include/CPU$(CPUINDEX)
				 
ifeq ($(SINGLE_CPU), on)

MODULE_CFLAGS +=  -I$(TOPDIR)/OSAL/source_SingleCPU \
                  -I$(TOPDIR)/OSAL/source_SingleCPU/$(OS)

else

MODULE_CFLAGS +=  -I$(TOPDIR)/OSAL/source_MultiCPU \
                  -I$(TOPDIR)/OSAL/source_MultiCPU/$(tmp_SoC)_$(OS)

endif

######################################################
# Common dependency process

include $(TOPDIR)/Scripts/mk_default.in
