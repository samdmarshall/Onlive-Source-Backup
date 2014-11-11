#ifndef __GALOIS_TWSI_BUS_H__
#define __GALOIS_TWSI_BUS_H__

/* slv_addr */
#define TWSI_SLVADDR_THS8200a	0x20
#define TWSI_SLVADDR_CS4382A		0x18

/* addr_type */
#define TWSI_7BIT_SLAVE_ADDR		0
#define TWSI_10BIT_SLAVE_ADDR	1

/* speed_type, negleted currently */
#define TWSI_STANDARD_SPEED		0 /* 100K, 400K and 3.4MHz in standard type */
#define TWSI_CUSTOM_SPEED		1 /* any speed in custom type */

/* speed */
#define TWSI_SPEED_100			100
#define TWSI_SPEED_400			400
#define TWSI_SPEED_3400			3400

/*
 * ioctl commands
 */
#define TWSI_IOCTL_READWRITE	0x1619
#define TWSI_IOCTL_SETSPEED	0x1234

/*
 * struction maintained by linux kernel, user doesn't use it
 */
typedef struct galois_twsi_info {
	uint mst_id;	/* : master num you're operating */
	uint slv_addr;	/* : the slave's address you're operating */
	uint addr_type;	/* : the slave is 7Bit address or 10 Bit address */
	uint xd_mode; 	/* : defined by Xiaodong, see above TWSIDEV_XDMODExxx */
	uint speed_type;/* : TWSI_STANDARD_SPEED or TWSI_CUSTOM_SPEED, ignored now */
	uint speed;		/* : 100K, 400K or 3400 or..., ignored now */
	int irq;	/* : kernel info, don't touch */
	int usable;	/* : kernel info, don't touch */
} galois_twsi_info_t;

/*
 * read-write data from/to TWSI slave, TWSI_IOCTL_READWRITE
 */
typedef struct galois_twsi_rw {
	uint mst_id;	/* user input: master num you're operating */
	uint slv_addr;	/* user input: the slave's address you're operating */
	uint addr_type;	/* user input: the slave is 7Bit address or 10 Bit address */

	uint rd_cnt;			/* read number in byte */
	unsigned char *rd_buf;	/* data buffer for read */
	uint wr_cnt;			/* write number in byte */
	unsigned char *wr_buf;	/* data buffer for write */
} galois_twsi_rw_t;

/*
 * set twsi speed
 */
typedef struct galois_twsi_speed {
	uint mst_id;	/* user input: master num you're operating */
	uint speed_type;/* user input: TWSI_STANDARD_SPEED or TWSI_CUSTOM_SPEED */
	uint speed; 	/* user input: if speed==0, user old setting, else update twsi speed */
} galois_twsi_speed_t;

#endif /* __GALOIS_TWSI_BUS_H__ */
