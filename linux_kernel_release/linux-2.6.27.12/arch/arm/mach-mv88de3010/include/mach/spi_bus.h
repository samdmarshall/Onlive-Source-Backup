#ifndef __GALOIS_SPI_BUS_H__
#define __GALOIS_SPI_BUS_H__
/*
 * NOTICE:
 * 1. only half-duplex mode is supported.
 * 2. transmit only, receive only, transmit and then receive, 3 modes are supported.
 * 3. the maximum count for transmit and receive are 256
 */

/* slv_addr */
#define SPI_SLAVE_ID_M25P40a	0
#define SPI_SLAVE_ID_M25P40b	1
#define SPI_SLAVE_ID_M25P40c	2
#define SPI_SLAVE_ID_WM8768a	0
#define SPI_SLAVE_ID_WM8751La	1
#define SPI_SLAVE_ID_BOEa		2
/* xd_mode */
#define SPIDEV_XDMODE0_WOA7D9		0
#define SPIDEV_XDMODE1_RWI8A24D8n	1
#define SPIDEV_XDMODE2_WOA8D8n		2
/* clock mode */
#define SPI_CLOCKMODE0_POL0PH0		0
#define SPI_CLOCKMODE1_POL0PH1		1
#define SPI_CLOCKMODE2_POL1PH0		2
#define SPI_CLOCKMODE3_POL1PH1		3
/* speed */
#define SPI_SPEED_200KHZ	200
/* frame width */
#define SPI_FRAME_WIDTH8	8

/*
 * ioctl commands
 */
#define SPI_IOCTL_SETINFO	0x1111
#define SPI_IOCTL_GETINFO	0x1004
#define SPI_IOCTL_READ		0x1980	/* obsoleted */
#define SPI_IOCTL_WRITE		0x2008	/* obsoleted */
#define SPI_IOCTL_READWRITE	0x0426
#define SPI_IOCTL_READWRITE_CHAR	0x6666

typedef struct galois_spi_info {
	uint mst_id;		/* user input: master num you access */
	uint slave_id;		/* user input: which CS_n line */
	uint xd_mode; 		/* user input: obsoleted, ignore it */
	uint clock_mode;	/* user input: control polarity and phrase of SPI proto */
	uint speed;			/* user input: control baudrate, 200KHz by default */
	uint frame_width;	/* user input: frame width, 8bits by default */
	int irq;	/* kernel info, don't touch */
	int usable;	/* kernel info, don't touch */
} galois_spi_info_t;

/*
 * obsoleted structure, don't use it anymore
 */
typedef struct galois_spi_cmd {
	uint sub_cmd;	/* internal addr or command, xd_mode specific */
	uint count;		/* rw number in @buf */
	uint *buf;		/* XXX: it's uint type, i2c is char type */
} galois_spi_cmd_t;

/*
 * galois_spi_rw_t should be used with SPI_IOCTL_READWRITE
 */
typedef struct galois_spi_rw {
	uint mst_id;	/* user input: master num you're operating */
	uint slave_id;	/* user input: the slave's address you're operating */

	uint rd_cnt;	/* read number in frame_width, <=256 */
	uint *rd_buf;	/* data buffer for read */
	uint wr_cnt;	/* write number in frame_width, <=256 */
	uint *wr_buf;	/* data buffer for write */
} galois_spi_rw_t;

/*
 * galois_spi_rw_char_t should be used with SPI_IOCTL_READWRITE_CHAR
 */
typedef struct galois_spi_rw_char {
	uint mst_id;	/* user input: master num you're operating */
	uint slave_id;	/* user input: the slave's address you're operating */

	uint rd_cnt;	/* read number in bytes, cnt/(frame_width/8)<=256 */
	unsigned char *rd_buf;	/* data buffer for read */
	uint wr_cnt;	/* write number in bytes, cnt/(frame_width/8)<=256 */
	unsigned char *wr_buf;	/* data buffer for write */
} galois_spi_rw_char_t;

#endif /* __GALOIS_SPI_BUS_H__ */
