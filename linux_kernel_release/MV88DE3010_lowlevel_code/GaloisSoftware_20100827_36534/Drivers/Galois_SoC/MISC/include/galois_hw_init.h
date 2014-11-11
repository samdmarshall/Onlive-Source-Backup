#include "galois_io.h"
#include "ctypes.h"


void Set_PIC_GInt(UNSG32 cpu_id, UNSG32 enable);
void initialize_Galois_hw_at_very_beginning(UNSG32 cpu_id);
void initialize_Galois_hw(UNSG32 cpu_id);
void initialize_PIN_MUX(void);
void initialize_CLOCK(void);
void initialize_PIC(UNSG32 cpu_id);

void initialize_GFX(UNSG32 cpu_id);
void initialize_ICTL(UNSG32 cpu_id);
void initialize_DMA_VPRO(UNSG32 cpu_id);
void initialize_SEMA_VPRO(UNSG32 cpu_id);
void initialize_VOP(UNSG32 cpu_id);
void initialize_DMA_AVIO(UNSG32 cpu_id);
void initialize_SATA(UNSG32 cpu_id);
void initialize_ETH(UNSG32 cpu_id);
void initialize_DRM(UNSG32 cpu_id);
void initialize_I2S(UNSG32 cpu_id);
void initialize_inter_Host(UNSG32 cpu_id);

void initialize_ictl_detail(UNSG32 cpu_id);
// peripheral
UNSG32 initialize_timer(UNSG32 cpu_id, UNSG32 timer_N, UNSG32 load_value);
UNSG32 initialize_spi(UNSG32 cpu_id, UNSG32 vector_id);
UNSG32 initialize_i2c(UNSG32 cpu_id, UNSG32 vector_id);
UNSG32 initialize_wdt(UNSG32 cpu_id, UNSG32 vector_id);
UNSG32 GaloisGetVideoDecBase(void);
UNSG32 GaloisGetVPPBase(void);
