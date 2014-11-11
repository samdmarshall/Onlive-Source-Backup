#ifndef __ASM_ARM_ARCH_NAND_PRIV_H
#define __ASM_ARM_ARCH_NAND_PRIV_H
#include <linux/mtd/partitions.h>

// Used generally 2 kinds of Nand devices, all from Samsung
// 1. 2K page devices, including K9WAG08U1A (SLC 8Gb*2CE) and K9G8G08U0M (MLC 8Gb)
// 2. 4K page devicees, including K9GAG08U0M (MLC 16Gb)

// Samsung K9WAG08U1A, composed of 2 K9K8G08U0A.
// K9K8G08U0A: 8Gb {(2K + 64)-Bytes/Page, 64-Pages/Block, i.e. 8K-Blocks = 512K-pages}

#define MV_NAND_NDSR_WRCMDREQ		0x00000001			// Write command request bit
#define MV_NAND_NDSR_RDDREQ		0x00000002			// Read Data Request
#define MV_NAND_NDSR_RDY0			0x00000800			// ND_nCS0 Ready bit
#define MV_NAND_NDSR_CS0_CMDD		0x00000100			// ND_nCS0 command is done
#define MV_NAND_NDSR_CS0_PAGED		0x00000400			// ND_nCS0 has completed a page of access
#define MV_NAND_NDSR_CS0_BBD		0x00000040			// ND_nCS0 program/erase auto-status-read failure 
#define MV_NAND_NDSR_RDY1			0x00001000			// ND_nCS1 Ready bit
#define MV_NAND_NDSR_CS1_CMDD		0x00000080			// ND_nCS1 command is done
#define MV_NAND_NDSR_CS1_PAGED		0x00000200			// ND_nCS1 has completed a page of access
#define MV_NAND_NDSR_CS1_BBD		0x00000040			// ND_nCS1 program/erase auto-status-read failure 
#define MV_NAND_NDSR_WRDREQ		0x00000004			// Write Data request
#define MV_NAND_NDSR_CORERR		0x00000008			// Correctable error detected
#define MV_NAND_NDSR_UNCERR		0x00000010			// UnCorrectable error detected
#define MV_NAND_NDCR_ND_RUN		0x10000000			// NAND Controller RUN mode bit
#define MV_NAND_NDCB_CSEL			0x01000000			// NAND nCS1 or nCS0
#define MV_NAND_NDCB_NC			0x00100000			// NAND nCS1 or nCS0
#define MV_NAND_NDCB_AUTO_RS		0x02000000			// NAND automatic check status
#define MV_NAND_NDCR_SPARE_EN		0x80000000
#define MV_NAND_NDCR_ECC_EN		0x40000000
#define MV_NAND_NDCR_STOP_ON_UNCOR	0x00080000


#define MV_NAND_NDCR_PG_PER_BLK_BITS			2
#define MV_NAND_NDCR_PG_PER_BLK_LSB			13

#define MV_NAND_NDCR_PG_SIZE_BITS            2
#define MV_NAND_NDCR_PG_SIZE_LSB         24

#define MV_NAND_NDCR_PG4096                 0x2
#define MV_NAND_NDCR_PG2048                 0x1
#define MV_NAND_NDCR_PG8192                 0x3
#define MV_NAND_NDCR_PG512                 0x0
enum mv_pages_per_block_bits
{
	PAGES_PER_BLOCK_32,
	PAGES_PER_BLOCK_128,
	PAGES_PER_BLOCK_64,
	PAGES_PER_BLOCK_256
};

// SPARE_EN, ECC_EN, BCH_EN
#define MV_NAND_SPARE_2048_0xx		0
#define MV_NAND_SPARE_2048_10x		64
#define MV_NAND_SPARE_2048_110		40
#define MV_NAND_SPARE_2048_111		32

#define MV_NAND_SPARE_4096_0xx      0
#define MV_NAND_SPARE_4096_110      80
#define MV_NAND_SPARE_4096_111      64
#define MV_NAND_SPARE_4096_SIZE     128
// default register values
// spare_en(0), ecc_en(0), dma_en(0), ND_RUN(0)
// DFC_data_bus_width(8-bit), device_data_bus_width(8-bit), page_size(2048)
// SEQ_DIS(1), clean_stop(0), FORCE_CSX(0), clear_page_count(0)
// STOP_ON_UNCOR(0), read_ID_byte_count(5)
// Row_Address_Start_Position(3rd cycle), pages_per_block(64), arbitrator_en(1)
// all_iterrupts(disabled)
#define MV_NAND_NDCR_VAL			0x0185DFFF	// 0x0D075FFF
/* 
 * for 150MHz NFC clock, NDTR0CS0=0x84c20a0a, NAND device is MLC_2K_K9G8G08X0M
 * .tADL[31:27]=16(16*6.6ns=105.6ns > 100ns)
 * .SelCntr=1, .Rd_Cnt_Del[25:22]=3(4*6.6ns=26.6ns, must be longer than prop delay)
 * .tCH[21:19]=0(1*6.6ns=6.6ns > 5ns hold time)
 * .tCS[18:16]=2(3*6.6ns=19.8ns = 20ns setup time)
 * .tWH[13:11]=1(2*6.6ns=13.2ns > 10ns nWE high hold time)
 * .tWP[10:8]=2(3*6.6ns=19.8ns > 15ns nWE pulse width) {note:.tWH+.tWP>=2}
 * .etRP[6]=0
 * .tRH[5:3]=1(2*6.6ns=13.2ns > 10ns nRE high hold time)
 * .tRP[2:0]=2(3*6.6ns=19.8ns > 15ns nRE pulse time) {note:.tRH+.tRP>=2}
 */
#define MV_NAND_NDTR0CS0_VAL    0x8ccb1a13//0x84c20a0a//Diag=0x84820a10
/*
 * for 150MHz, NDTR1CS0=0x0c2d80A2, NAND device is MLC_2K_K9G8G08X0M
 * .tR[31:16]=0x3000(0x3000*6.6ns=81.1us > 60us! .Wait_Mode is enabled.)
 * .Wait_Mode[15]=1, .PRESCALE[14]=0
 * .tWHR[7:4]=12((12-tWH)*6.6ns=60ns = 60ns WE high to RE low time for status read)
 * .tAR[3:0]=4(4*6.6ns > 10ns ALE to RE delay)
 */
#define MV_NAND_NDTR1CS0_VAL    0x300080c4//0x0c2d80A2//0x0c2d00A2
// .nd_re_en=1, .nd_re_sel=0
// .nd_re_del_0=7
#define MV_NAND_NDREDEL_VAL		0
// Write 1 to reset status bits, write 0 to reserved and read-only bits
#define MV_NAND_NDSR_VAL			0xFFFFFFFF	// 0x00000FFF
#define MV_NAND_NDECCCTRL_VAL		0x0

/*
  NAND_NDCR,
	NAND_NDTR0CS0,
	NAND_NDTR1CS0,
	NAND_NDREDEL,
	NAND_NDSR,
	NAND_NDECCCTRL,
	
*/
struct mv_nand_data {
	u16		szofpg;	  /* size of page *KB */
	u16		szofblk;	/* size of block *KB */
	u8    conspare ;    /* SPARE_EN */
	u8    conecc;       /* ECC */
	u8    conbch;        /* BCH */
	u8    constp;   /* STOP on uncorrectible error */
	u8    numofchip;    /*num of chip */
	u8    bus_width_16;
	u8    reset;
	u8    ncs  ;    /*chip selected,will be changed if using chip select*/
	u32   ndcr;
	u32   ndtr0cs0;
	u32   ndtr1cs0;
	u32   ndredel;
	u32   ndsr;
	u32   ndeccctrl;
	struct mtd_partition* (*partition_info)(int, int*);
};

#define MV_NAND_REG_WRITE_BYTE(base, offset, val)    \
    writeb(val, base + offset)
#define MV_NAND_REG_WRITE_WORD(base, offset, val)    \
    writew(val, base + offset)
#define MV_NAND_REG_WRITE_DWORD(base, offset, val)    \
    writel(val, base + offset)
    
#define MV_NAND_REG_READ_BYTE(base, offset)			\
	readb(base + offset)
#define MV_NAND_REG_READ_WORD(base, offset)			\
	readw(base + offset)
#define MV_NAND_REG_READ_DWORD(base, offset)			\
	readl(base + offset)
	
#define MV_NAND_NDCR_OFFSET		0x00			// Offset to Control Register
#define MV_NAND_NDTR0CS0_OFFSET	0x04			// Offest to Timing Paramter 0
#define MV_NAND_NDTR1CS0_OFFSET	0x0C			// Offest to Timing Paramter 0
#define MV_NAND_NDSR_OFFSET		0x14			// Offset to Status Register
#define MV_NAND_NDREDEL_OFFSET		0x24			// Offset to Read Enable Return Delay Register
#define MV_NAND_NDECCCTRL_OFFSET	0x28			// Offset to ECC Control Register
#define MV_NAND_NDDB_OFFSET		0x40			// Offset to data buffer (input and output)
#define MV_NAND_NDCB0_OFFSET		0x48			// Offset to Command Buffer 0
#define MV_NAND_NDCB1_OFFSET        0x4c
#define MV_NAND_NDCB2_OFFSET        0x50 
#define MV_NAND_NDCB3_OFFSET        0x54 



#define MV_NAN_NDCB0_LENOVRD_BITS      28
#endif
