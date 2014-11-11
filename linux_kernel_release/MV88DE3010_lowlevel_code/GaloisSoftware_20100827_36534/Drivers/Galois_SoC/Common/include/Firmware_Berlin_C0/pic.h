/**********************************************************************************************************************
**********************************************************************************************************************/
#ifdef	_pic_h
#else
#define	_pic_h						"           pic_h >>>    "
#include	"ctypes.h"

#pragma	pack(4)

#ifdef	__cplusplus
	extern	"C"
	{
#endif



/**********************************************************************************************************************
**********************************************************************************************************************/
#ifdef	h_cmn
#else
#define	h_cmn

#define	\
	AR_cmn \
		18
#define	\
	AB_cmn \
		7

typedef struct SIE_cmn {

	#define	RA_cmn_fSrcPolSel0 			0x00000
	#define	w32cmn_fSrcPolSel0 			{										\
					UNSG32				ufSrcPolSel0_pol			 : 32;		\
										}
	/* @'00000 */	union {	UNSG32		u32cmn_fSrcPolSel0;
								struct	w32cmn_fSrcPolSel0;
										};

	/* [31: 0] */	#define	  BA_cmn_fSrcPolSel0_pol				 0x00000
	/* ---:--- */	#define	    bcmn_fSrcPolSel0_pol				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fSrcPolSel0_pol				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_fSrcPolSel0_pol				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_fSrcPolSel0_pol(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_fSrcPolSel0_pol(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_fSrcPolSel0_pol(r32,v)				do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_fSrcPolSel0_pol(r16,v)				do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_fSrcPolSel1 			0x00004
	#define	w32cmn_fSrcPolSel1 			{										\
					UNSG32				ufSrcPolSel1_pol			 : 32;		\
										}
	/* @'00004 */	union {	UNSG32		u32cmn_fSrcPolSel1;
								struct	w32cmn_fSrcPolSel1;
										};

	/* [31: 0] */	#define	  BA_cmn_fSrcPolSel1_pol				 0x00004
	/* ---:--- */	#define	    bcmn_fSrcPolSel1_pol				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fSrcPolSel1_pol				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_fSrcPolSel1_pol				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_fSrcPolSel1_pol(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_fSrcPolSel1_pol(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_fSrcPolSel1_pol(r32,v)				do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_fSrcPolSel1_pol(r16,v)				do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_fSrcPolSel2 			0x00008
	#define	w32cmn_fSrcPolSel2 			{										\
					UNSG32				ufSrcPolSel2_pol			 : 32;		\
										}
	/* @'00008 */	union {	UNSG32		u32cmn_fSrcPolSel2;
								struct	w32cmn_fSrcPolSel2;
										};

	/* [31: 0] */	#define	  BA_cmn_fSrcPolSel2_pol				 0x00008
	/* ---:--- */	#define	    bcmn_fSrcPolSel2_pol				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fSrcPolSel2_pol				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_fSrcPolSel2_pol				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_fSrcPolSel2_pol(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_fSrcPolSel2_pol(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_fSrcPolSel2_pol(r32,v)				do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_fSrcPolSel2_pol(r16,v)				do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_fSrcSensSel0 		0x0000C
	#define	w32cmn_fSrcSensSel0 		{										\
					UNSG32				ufSrcSensSel0_sens			 : 32;		\
										}
	/* @'0000C */	union {	UNSG32		u32cmn_fSrcSensSel0;
								struct	w32cmn_fSrcSensSel0;
										};

	/* [31: 0] */	#define	  BA_cmn_fSrcSensSel0_sens				 0x0000C
	/* ---:--- */	#define	    bcmn_fSrcSensSel0_sens				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fSrcSensSel0_sens				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_fSrcSensSel0_sens				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_fSrcSensSel0_sens(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_fSrcSensSel0_sens(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_fSrcSensSel0_sens(r32,v)			do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_fSrcSensSel0_sens(r16,v)			do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_fSrcSensSel1 		0x00010
	#define	w32cmn_fSrcSensSel1 		{										\
					UNSG32				ufSrcSensSel1_sens			 : 32;		\
										}
	/* @'00010 */	union {	UNSG32		u32cmn_fSrcSensSel1;
								struct	w32cmn_fSrcSensSel1;
										};

	/* [31: 0] */	#define	  BA_cmn_fSrcSensSel1_sens				 0x00010
	/* ---:--- */	#define	    bcmn_fSrcSensSel1_sens				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fSrcSensSel1_sens				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_fSrcSensSel1_sens				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_fSrcSensSel1_sens(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_fSrcSensSel1_sens(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_fSrcSensSel1_sens(r32,v)			do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_fSrcSensSel1_sens(r16,v)			do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_fSrcSensSel2 		0x00014
	#define	w32cmn_fSrcSensSel2 		{										\
					UNSG32				ufSrcSensSel2_sens			 : 32;		\
										}
	/* @'00014 */	union {	UNSG32		u32cmn_fSrcSensSel2;
								struct	w32cmn_fSrcSensSel2;
										};

	/* [31: 0] */	#define	  BA_cmn_fSrcSensSel2_sens				 0x00014
	/* ---:--- */	#define	    bcmn_fSrcSensSel2_sens				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fSrcSensSel2_sens				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_fSrcSensSel2_sens				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_fSrcSensSel2_sens(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_fSrcSensSel2_sens(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_fSrcSensSel2_sens(r32,v)			do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_fSrcSensSel2_sens(r16,v)			do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_fIntSts0 			0x00018
	#define	w32cmn_fIntSts0 			{										\
					UNSG32				ufIntSts0_ists_0i			 :  1;		\
					UNSG32				ufIntSts0_ists_1i			 :  1;		\
					UNSG32				ufIntSts0_ists_2i			 :  1;		\
					UNSG32				ufIntSts0_ists_3i			 :  1;		\
					UNSG32				ufIntSts0_ists_4i			 :  1;		\
					UNSG32				ufIntSts0_ists_5i			 :  1;		\
					UNSG32				ufIntSts0_ists_6i			 :  1;		\
					UNSG32				ufIntSts0_ists_7i			 :  1;		\
					UNSG32				ufIntSts0_ists_8i			 :  1;		\
					UNSG32				ufIntSts0_ists_9i			 :  1;		\
					UNSG32				ufIntSts0_ists_10i			 :  1;		\
					UNSG32				ufIntSts0_ists_11i			 :  1;		\
					UNSG32				ufIntSts0_ists_12i			 :  1;		\
					UNSG32				ufIntSts0_ists_13i			 :  1;		\
					UNSG32				ufIntSts0_ists_14i			 :  1;		\
					UNSG32				ufIntSts0_ists_15i			 :  1;		\
					UNSG32				ufIntSts0_ists_16i			 :  1;		\
					UNSG32				ufIntSts0_ists_17i			 :  1;		\
					UNSG32				ufIntSts0_ists_18i			 :  1;		\
					UNSG32				ufIntSts0_ists_19i			 :  1;		\
					UNSG32				ufIntSts0_ists_20i			 :  1;		\
					UNSG32				ufIntSts0_ists_21i			 :  1;		\
					UNSG32				ufIntSts0_ists_22i			 :  1;		\
					UNSG32				ufIntSts0_ists_23i			 :  1;		\
					UNSG32				ufIntSts0_ists_24i			 :  1;		\
					UNSG32				ufIntSts0_ists_25i			 :  1;		\
					UNSG32				ufIntSts0_ists_26i			 :  1;		\
					UNSG32				ufIntSts0_ists_27i			 :  1;		\
					UNSG32				ufIntSts0_ists_28i			 :  1;		\
					UNSG32				ufIntSts0_ists_29i			 :  1;		\
					UNSG32				ufIntSts0_ists_30i			 :  1;		\
					UNSG32				ufIntSts0_ists_31i			 :  1;		\
										}
	/* @'00018 */	union {	UNSG32		u32cmn_fIntSts0;
								struct	w32cmn_fIntSts0;
										};

	/* [ 0: 0] */	#define	  BA_cmn_fIntSts0_ists_0i				 0x00018
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_0i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_0i				    0 /*: 0*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_0i				       0x00000001
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_0i(r32)				 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_0i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_0i(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_0i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [ 1: 1] */	#define	  BA_cmn_fIntSts0_ists_1i				 0x00018
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_1i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_1i				    1 /*: 1*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_1i				       0x00000002
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_1i(r32)				 (((r32)>> 1)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_1i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_1i(r32,v)			do{(r32)&=~(0x0001<< 1);(r32)|=((v)&0x0001)<< 1;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_1i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [ 2: 2] */	#define	  BA_cmn_fIntSts0_ists_2i				 0x00018
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_2i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_2i				    2 /*: 2*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_2i				       0x00000004
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_2i(r32)				 (((r32)>> 2)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_2i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_2i(r32,v)			do{(r32)&=~(0x0001<< 2);(r32)|=((v)&0x0001)<< 2;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_2i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [ 3: 3] */	#define	  BA_cmn_fIntSts0_ists_3i				 0x00018
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_3i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_3i				    3 /*: 3*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_3i				       0x00000008
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_3i(r32)				 (((r32)>> 3)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_3i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_3i(r32,v)			do{(r32)&=~(0x0001<< 3);(r32)|=((v)&0x0001)<< 3;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_3i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [ 4: 4] */	#define	  BA_cmn_fIntSts0_ists_4i				 0x00018
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_4i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_4i				    4 /*: 4*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_4i				       0x00000010
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_4i(r32)				 (((r32)>> 4)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_4i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_4i(r32,v)			do{(r32)&=~(0x0001<< 4);(r32)|=((v)&0x0001)<< 4;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_4i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [ 5: 5] */	#define	  BA_cmn_fIntSts0_ists_5i				 0x00018
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_5i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_5i				    5 /*: 5*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_5i				       0x00000020
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_5i(r32)				 (((r32)>> 5)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_5i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_5i(r32,v)			do{(r32)&=~(0x0001<< 5);(r32)|=((v)&0x0001)<< 5;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_5i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [ 6: 6] */	#define	  BA_cmn_fIntSts0_ists_6i				 0x00018
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_6i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_6i				    6 /*: 6*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_6i				       0x00000040
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_6i(r32)				 (((r32)>> 6)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_6i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_6i(r32,v)			do{(r32)&=~(0x0001<< 6);(r32)|=((v)&0x0001)<< 6;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_6i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [ 7: 7] */	#define	  BA_cmn_fIntSts0_ists_7i				 0x00018
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_7i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_7i				    7 /*: 7*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_7i				       0x00000080
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_7i(r32)				 (((r32)>> 7)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_7i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_7i(r32,v)			do{(r32)&=~(0x0001<< 7);(r32)|=((v)&0x0001)<< 7;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_7i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [ 8: 8] */	#define	  BA_cmn_fIntSts0_ists_8i				 0x00019
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_8i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_8i				    8 /*: 8*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_8i				       0x00000100
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_8i(r32)				 (((r32)>> 8)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_8i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_8i(r32,v)			do{(r32)&=~(0x0001<< 8);(r32)|=((v)&0x0001)<< 8;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_8i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [ 9: 9] */	#define	  BA_cmn_fIntSts0_ists_9i				 0x00019
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_9i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_9i				    9 /*: 9*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_9i				       0x00000200
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_9i(r32)				 (((r32)>> 9)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_9i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_9i(r32,v)			do{(r32)&=~(0x0001<< 9);(r32)|=((v)&0x0001)<< 9;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_9i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [10:10] */	#define	  BA_cmn_fIntSts0_ists_10i				 0x00019
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_10i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_10i				   10 /*:10*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_10i				       0x00000400
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_10i(r32)				 (((r32)>>10)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_10i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_10i(r32,v)			do{(r32)&=~(0x0001<<10);(r32)|=((v)&0x0001)<<10;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_10i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [11:11] */	#define	  BA_cmn_fIntSts0_ists_11i				 0x00019
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_11i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_11i				   11 /*:11*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_11i				       0x00000800
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_11i(r32)				 (((r32)>>11)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_11i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_11i(r32,v)			do{(r32)&=~(0x0001<<11);(r32)|=((v)&0x0001)<<11;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_11i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [12:12] */	#define	  BA_cmn_fIntSts0_ists_12i				 0x00019
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_12i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_12i				   12 /*:12*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_12i				       0x00001000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_12i(r32)				 (((r32)>>12)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_12i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_12i(r32,v)			do{(r32)&=~(0x0001<<12);(r32)|=((v)&0x0001)<<12;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_12i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [13:13] */	#define	  BA_cmn_fIntSts0_ists_13i				 0x00019
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_13i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_13i				   13 /*:13*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_13i				       0x00002000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_13i(r32)				 (((r32)>>13)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_13i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_13i(r32,v)			do{(r32)&=~(0x0001<<13);(r32)|=((v)&0x0001)<<13;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_13i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [14:14] */	#define	  BA_cmn_fIntSts0_ists_14i				 0x00019
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_14i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_14i				   14 /*:14*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_14i				       0x00004000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_14i(r32)				 (((r32)>>14)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_14i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_14i(r32,v)			do{(r32)&=~(0x0001<<14);(r32)|=((v)&0x0001)<<14;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_14i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [15:15] */	#define	  BA_cmn_fIntSts0_ists_15i				 0x00019
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_15i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_15i				   15 /*:15*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_15i				       0x00008000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_15i(r32)				 (((r32)>>15)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_15i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_15i(r32,v)			do{(r32)&=~(0x0001<<15);(r32)|=((v)&0x0001)<<15;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_15i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

	/* [16:16] */	#define	  BA_cmn_fIntSts0_ists_16i				 0x0001A
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_16i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_16i				   16 /*:16*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_16i				       0x00010000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_16i(r32)				 (((r32)>>16)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_16i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_16i(r32,v)			do{(r32)&=~(0x0001<<16);(r32)|=((v)&0x0001)<<16;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_16i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [17:17] */	#define	  BA_cmn_fIntSts0_ists_17i				 0x0001A
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_17i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_17i				   17 /*:17*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_17i				       0x00020000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_17i(r32)				 (((r32)>>17)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_17i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_17i(r32,v)			do{(r32)&=~(0x0001<<17);(r32)|=((v)&0x0001)<<17;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_17i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [18:18] */	#define	  BA_cmn_fIntSts0_ists_18i				 0x0001A
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_18i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_18i				   18 /*:18*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_18i				       0x00040000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_18i(r32)				 (((r32)>>18)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_18i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_18i(r32,v)			do{(r32)&=~(0x0001<<18);(r32)|=((v)&0x0001)<<18;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_18i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [19:19] */	#define	  BA_cmn_fIntSts0_ists_19i				 0x0001A
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_19i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_19i				   19 /*:19*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_19i				       0x00080000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_19i(r32)				 (((r32)>>19)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_19i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_19i(r32,v)			do{(r32)&=~(0x0001<<19);(r32)|=((v)&0x0001)<<19;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_19i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [20:20] */	#define	  BA_cmn_fIntSts0_ists_20i				 0x0001A
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_20i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_20i				   20 /*:20*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_20i				       0x00100000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_20i(r32)				 (((r32)>>20)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_20i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_20i(r32,v)			do{(r32)&=~(0x0001<<20);(r32)|=((v)&0x0001)<<20;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_20i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [21:21] */	#define	  BA_cmn_fIntSts0_ists_21i				 0x0001A
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_21i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_21i				   21 /*:21*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_21i				       0x00200000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_21i(r32)				 (((r32)>>21)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_21i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_21i(r32,v)			do{(r32)&=~(0x0001<<21);(r32)|=((v)&0x0001)<<21;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_21i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [22:22] */	#define	  BA_cmn_fIntSts0_ists_22i				 0x0001A
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_22i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_22i				   22 /*:22*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_22i				       0x00400000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_22i(r32)				 (((r32)>>22)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_22i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_22i(r32,v)			do{(r32)&=~(0x0001<<22);(r32)|=((v)&0x0001)<<22;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_22i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [23:23] */	#define	  BA_cmn_fIntSts0_ists_23i				 0x0001A
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_23i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_23i				   23 /*:23*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_23i				       0x00800000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_23i(r32)				 (((r32)>>23)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_23i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_23i(r32,v)			do{(r32)&=~(0x0001<<23);(r32)|=((v)&0x0001)<<23;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_23i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [24:24] */	#define	  BA_cmn_fIntSts0_ists_24i				 0x0001B
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_24i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_24i				   24 /*:24*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_24i				       0x01000000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_24i(r32)				 (((r32)>>24)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_24i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_24i(r32,v)			do{(r32)&=~(0x0001<<24);(r32)|=((v)&0x0001)<<24;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_24i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [25:25] */	#define	  BA_cmn_fIntSts0_ists_25i				 0x0001B
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_25i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_25i				   25 /*:25*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_25i				       0x02000000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_25i(r32)				 (((r32)>>25)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_25i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_25i(r32,v)			do{(r32)&=~(0x0001<<25);(r32)|=((v)&0x0001)<<25;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_25i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [26:26] */	#define	  BA_cmn_fIntSts0_ists_26i				 0x0001B
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_26i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_26i				   26 /*:26*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_26i				       0x04000000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_26i(r32)				 (((r32)>>26)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_26i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_26i(r32,v)			do{(r32)&=~(0x0001<<26);(r32)|=((v)&0x0001)<<26;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_26i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [27:27] */	#define	  BA_cmn_fIntSts0_ists_27i				 0x0001B
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_27i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_27i				   27 /*:27*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_27i				       0x08000000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_27i(r32)				 (((r32)>>27)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_27i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_27i(r32,v)			do{(r32)&=~(0x0001<<27);(r32)|=((v)&0x0001)<<27;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_27i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [28:28] */	#define	  BA_cmn_fIntSts0_ists_28i				 0x0001B
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_28i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_28i				   28 /*:28*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_28i				       0x10000000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_28i(r32)				 (((r32)>>28)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_28i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_28i(r32,v)			do{(r32)&=~(0x0001<<28);(r32)|=((v)&0x0001)<<28;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_28i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [29:29] */	#define	  BA_cmn_fIntSts0_ists_29i				 0x0001B
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_29i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_29i				   29 /*:29*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_29i				       0x20000000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_29i(r32)				 (((r32)>>29)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_29i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_29i(r32,v)			do{(r32)&=~(0x0001<<29);(r32)|=((v)&0x0001)<<29;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_29i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [30:30] */	#define	  BA_cmn_fIntSts0_ists_30i				 0x0001B
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_30i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_30i				   30 /*:30*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_30i				       0x40000000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_30i(r32)				 (((r32)>>30)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_30i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_30i(r32,v)			do{(r32)&=~(0x0001<<30);(r32)|=((v)&0x0001)<<30;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_30i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [31:31] */	#define	  BA_cmn_fIntSts0_ists_31i				 0x0001B
	/* ---:--- */	#define	    bcmn_fIntSts0_ists_31i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts0_ists_31i				   31 /*:31*/
	/* ---:--- */	#define	MSK32cmn_fIntSts0_ists_31i				       0x80000000
	/* ---:--- */	#define	GET32cmn_fIntSts0_ists_31i(r32)				 (((r32)>>31)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts0_ists_31i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts0_ists_31i(r32,v)			do{(r32)&=~(0x0001<<31);(r32)|=((v)&0x0001)<<31;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts0_ists_31i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

	#define	RA_cmn_fIntSts1 			0x0001C
	#define	w32cmn_fIntSts1 			{										\
					UNSG32				ufIntSts1_ists_0i			 :  1;		\
					UNSG32				ufIntSts1_ists_1i			 :  1;		\
					UNSG32				ufIntSts1_ists_2i			 :  1;		\
					UNSG32				ufIntSts1_ists_3i			 :  1;		\
					UNSG32				ufIntSts1_ists_4i			 :  1;		\
					UNSG32				ufIntSts1_ists_5i			 :  1;		\
					UNSG32				ufIntSts1_ists_6i			 :  1;		\
					UNSG32				ufIntSts1_ists_7i			 :  1;		\
					UNSG32				ufIntSts1_ists_8i			 :  1;		\
					UNSG32				ufIntSts1_ists_9i			 :  1;		\
					UNSG32				ufIntSts1_ists_10i			 :  1;		\
					UNSG32				ufIntSts1_ists_11i			 :  1;		\
					UNSG32				ufIntSts1_ists_12i			 :  1;		\
					UNSG32				ufIntSts1_ists_13i			 :  1;		\
					UNSG32				ufIntSts1_ists_14i			 :  1;		\
					UNSG32				ufIntSts1_ists_15i			 :  1;		\
					UNSG32				ufIntSts1_ists_16i			 :  1;		\
					UNSG32				ufIntSts1_ists_17i			 :  1;		\
					UNSG32				ufIntSts1_ists_18i			 :  1;		\
					UNSG32				ufIntSts1_ists_19i			 :  1;		\
					UNSG32				ufIntSts1_ists_20i			 :  1;		\
					UNSG32				ufIntSts1_ists_21i			 :  1;		\
					UNSG32				ufIntSts1_ists_22i			 :  1;		\
					UNSG32				ufIntSts1_ists_23i			 :  1;		\
					UNSG32				ufIntSts1_ists_24i			 :  1;		\
					UNSG32				ufIntSts1_ists_25i			 :  1;		\
					UNSG32				ufIntSts1_ists_26i			 :  1;		\
					UNSG32				ufIntSts1_ists_27i			 :  1;		\
					UNSG32				ufIntSts1_ists_28i			 :  1;		\
					UNSG32				ufIntSts1_ists_29i			 :  1;		\
					UNSG32				ufIntSts1_ists_30i			 :  1;		\
					UNSG32				ufIntSts1_ists_31i			 :  1;		\
										}
	/* @'0001C */	union {	UNSG32		u32cmn_fIntSts1;
								struct	w32cmn_fIntSts1;
										};

	/* [ 0: 0] */	#define	  BA_cmn_fIntSts1_ists_0i				 0x0001C
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_0i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_0i				    0 /*: 0*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_0i				       0x00000001
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_0i(r32)				 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_0i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_0i(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_0i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [ 1: 1] */	#define	  BA_cmn_fIntSts1_ists_1i				 0x0001C
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_1i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_1i				    1 /*: 1*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_1i				       0x00000002
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_1i(r32)				 (((r32)>> 1)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_1i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_1i(r32,v)			do{(r32)&=~(0x0001<< 1);(r32)|=((v)&0x0001)<< 1;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_1i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [ 2: 2] */	#define	  BA_cmn_fIntSts1_ists_2i				 0x0001C
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_2i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_2i				    2 /*: 2*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_2i				       0x00000004
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_2i(r32)				 (((r32)>> 2)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_2i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_2i(r32,v)			do{(r32)&=~(0x0001<< 2);(r32)|=((v)&0x0001)<< 2;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_2i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [ 3: 3] */	#define	  BA_cmn_fIntSts1_ists_3i				 0x0001C
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_3i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_3i				    3 /*: 3*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_3i				       0x00000008
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_3i(r32)				 (((r32)>> 3)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_3i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_3i(r32,v)			do{(r32)&=~(0x0001<< 3);(r32)|=((v)&0x0001)<< 3;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_3i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [ 4: 4] */	#define	  BA_cmn_fIntSts1_ists_4i				 0x0001C
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_4i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_4i				    4 /*: 4*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_4i				       0x00000010
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_4i(r32)				 (((r32)>> 4)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_4i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_4i(r32,v)			do{(r32)&=~(0x0001<< 4);(r32)|=((v)&0x0001)<< 4;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_4i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [ 5: 5] */	#define	  BA_cmn_fIntSts1_ists_5i				 0x0001C
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_5i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_5i				    5 /*: 5*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_5i				       0x00000020
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_5i(r32)				 (((r32)>> 5)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_5i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_5i(r32,v)			do{(r32)&=~(0x0001<< 5);(r32)|=((v)&0x0001)<< 5;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_5i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [ 6: 6] */	#define	  BA_cmn_fIntSts1_ists_6i				 0x0001C
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_6i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_6i				    6 /*: 6*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_6i				       0x00000040
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_6i(r32)				 (((r32)>> 6)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_6i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_6i(r32,v)			do{(r32)&=~(0x0001<< 6);(r32)|=((v)&0x0001)<< 6;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_6i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [ 7: 7] */	#define	  BA_cmn_fIntSts1_ists_7i				 0x0001C
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_7i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_7i				    7 /*: 7*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_7i				       0x00000080
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_7i(r32)				 (((r32)>> 7)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_7i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_7i(r32,v)			do{(r32)&=~(0x0001<< 7);(r32)|=((v)&0x0001)<< 7;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_7i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [ 8: 8] */	#define	  BA_cmn_fIntSts1_ists_8i				 0x0001D
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_8i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_8i				    8 /*: 8*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_8i				       0x00000100
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_8i(r32)				 (((r32)>> 8)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_8i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_8i(r32,v)			do{(r32)&=~(0x0001<< 8);(r32)|=((v)&0x0001)<< 8;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_8i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [ 9: 9] */	#define	  BA_cmn_fIntSts1_ists_9i				 0x0001D
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_9i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_9i				    9 /*: 9*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_9i				       0x00000200
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_9i(r32)				 (((r32)>> 9)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_9i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_9i(r32,v)			do{(r32)&=~(0x0001<< 9);(r32)|=((v)&0x0001)<< 9;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_9i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [10:10] */	#define	  BA_cmn_fIntSts1_ists_10i				 0x0001D
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_10i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_10i				   10 /*:10*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_10i				       0x00000400
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_10i(r32)				 (((r32)>>10)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_10i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_10i(r32,v)			do{(r32)&=~(0x0001<<10);(r32)|=((v)&0x0001)<<10;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_10i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [11:11] */	#define	  BA_cmn_fIntSts1_ists_11i				 0x0001D
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_11i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_11i				   11 /*:11*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_11i				       0x00000800
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_11i(r32)				 (((r32)>>11)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_11i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_11i(r32,v)			do{(r32)&=~(0x0001<<11);(r32)|=((v)&0x0001)<<11;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_11i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [12:12] */	#define	  BA_cmn_fIntSts1_ists_12i				 0x0001D
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_12i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_12i				   12 /*:12*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_12i				       0x00001000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_12i(r32)				 (((r32)>>12)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_12i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_12i(r32,v)			do{(r32)&=~(0x0001<<12);(r32)|=((v)&0x0001)<<12;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_12i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [13:13] */	#define	  BA_cmn_fIntSts1_ists_13i				 0x0001D
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_13i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_13i				   13 /*:13*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_13i				       0x00002000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_13i(r32)				 (((r32)>>13)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_13i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_13i(r32,v)			do{(r32)&=~(0x0001<<13);(r32)|=((v)&0x0001)<<13;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_13i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [14:14] */	#define	  BA_cmn_fIntSts1_ists_14i				 0x0001D
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_14i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_14i				   14 /*:14*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_14i				       0x00004000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_14i(r32)				 (((r32)>>14)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_14i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_14i(r32,v)			do{(r32)&=~(0x0001<<14);(r32)|=((v)&0x0001)<<14;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_14i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [15:15] */	#define	  BA_cmn_fIntSts1_ists_15i				 0x0001D
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_15i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_15i				   15 /*:15*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_15i				       0x00008000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_15i(r32)				 (((r32)>>15)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_15i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_15i(r32,v)			do{(r32)&=~(0x0001<<15);(r32)|=((v)&0x0001)<<15;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_15i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

	/* [16:16] */	#define	  BA_cmn_fIntSts1_ists_16i				 0x0001E
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_16i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_16i				   16 /*:16*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_16i				       0x00010000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_16i(r32)				 (((r32)>>16)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_16i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_16i(r32,v)			do{(r32)&=~(0x0001<<16);(r32)|=((v)&0x0001)<<16;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_16i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [17:17] */	#define	  BA_cmn_fIntSts1_ists_17i				 0x0001E
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_17i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_17i				   17 /*:17*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_17i				       0x00020000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_17i(r32)				 (((r32)>>17)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_17i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_17i(r32,v)			do{(r32)&=~(0x0001<<17);(r32)|=((v)&0x0001)<<17;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_17i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [18:18] */	#define	  BA_cmn_fIntSts1_ists_18i				 0x0001E
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_18i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_18i				   18 /*:18*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_18i				       0x00040000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_18i(r32)				 (((r32)>>18)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_18i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_18i(r32,v)			do{(r32)&=~(0x0001<<18);(r32)|=((v)&0x0001)<<18;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_18i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [19:19] */	#define	  BA_cmn_fIntSts1_ists_19i				 0x0001E
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_19i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_19i				   19 /*:19*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_19i				       0x00080000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_19i(r32)				 (((r32)>>19)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_19i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_19i(r32,v)			do{(r32)&=~(0x0001<<19);(r32)|=((v)&0x0001)<<19;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_19i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [20:20] */	#define	  BA_cmn_fIntSts1_ists_20i				 0x0001E
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_20i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_20i				   20 /*:20*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_20i				       0x00100000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_20i(r32)				 (((r32)>>20)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_20i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_20i(r32,v)			do{(r32)&=~(0x0001<<20);(r32)|=((v)&0x0001)<<20;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_20i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [21:21] */	#define	  BA_cmn_fIntSts1_ists_21i				 0x0001E
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_21i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_21i				   21 /*:21*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_21i				       0x00200000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_21i(r32)				 (((r32)>>21)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_21i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_21i(r32,v)			do{(r32)&=~(0x0001<<21);(r32)|=((v)&0x0001)<<21;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_21i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [22:22] */	#define	  BA_cmn_fIntSts1_ists_22i				 0x0001E
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_22i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_22i				   22 /*:22*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_22i				       0x00400000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_22i(r32)				 (((r32)>>22)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_22i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_22i(r32,v)			do{(r32)&=~(0x0001<<22);(r32)|=((v)&0x0001)<<22;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_22i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [23:23] */	#define	  BA_cmn_fIntSts1_ists_23i				 0x0001E
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_23i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_23i				   23 /*:23*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_23i				       0x00800000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_23i(r32)				 (((r32)>>23)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_23i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_23i(r32,v)			do{(r32)&=~(0x0001<<23);(r32)|=((v)&0x0001)<<23;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_23i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [24:24] */	#define	  BA_cmn_fIntSts1_ists_24i				 0x0001F
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_24i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_24i				   24 /*:24*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_24i				       0x01000000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_24i(r32)				 (((r32)>>24)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_24i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_24i(r32,v)			do{(r32)&=~(0x0001<<24);(r32)|=((v)&0x0001)<<24;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_24i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [25:25] */	#define	  BA_cmn_fIntSts1_ists_25i				 0x0001F
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_25i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_25i				   25 /*:25*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_25i				       0x02000000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_25i(r32)				 (((r32)>>25)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_25i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_25i(r32,v)			do{(r32)&=~(0x0001<<25);(r32)|=((v)&0x0001)<<25;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_25i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [26:26] */	#define	  BA_cmn_fIntSts1_ists_26i				 0x0001F
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_26i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_26i				   26 /*:26*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_26i				       0x04000000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_26i(r32)				 (((r32)>>26)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_26i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_26i(r32,v)			do{(r32)&=~(0x0001<<26);(r32)|=((v)&0x0001)<<26;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_26i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [27:27] */	#define	  BA_cmn_fIntSts1_ists_27i				 0x0001F
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_27i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_27i				   27 /*:27*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_27i				       0x08000000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_27i(r32)				 (((r32)>>27)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_27i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_27i(r32,v)			do{(r32)&=~(0x0001<<27);(r32)|=((v)&0x0001)<<27;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_27i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [28:28] */	#define	  BA_cmn_fIntSts1_ists_28i				 0x0001F
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_28i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_28i				   28 /*:28*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_28i				       0x10000000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_28i(r32)				 (((r32)>>28)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_28i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_28i(r32,v)			do{(r32)&=~(0x0001<<28);(r32)|=((v)&0x0001)<<28;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_28i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [29:29] */	#define	  BA_cmn_fIntSts1_ists_29i				 0x0001F
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_29i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_29i				   29 /*:29*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_29i				       0x20000000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_29i(r32)				 (((r32)>>29)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_29i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_29i(r32,v)			do{(r32)&=~(0x0001<<29);(r32)|=((v)&0x0001)<<29;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_29i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [30:30] */	#define	  BA_cmn_fIntSts1_ists_30i				 0x0001F
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_30i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_30i				   30 /*:30*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_30i				       0x40000000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_30i(r32)				 (((r32)>>30)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_30i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_30i(r32,v)			do{(r32)&=~(0x0001<<30);(r32)|=((v)&0x0001)<<30;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_30i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [31:31] */	#define	  BA_cmn_fIntSts1_ists_31i				 0x0001F
	/* ---:--- */	#define	    bcmn_fIntSts1_ists_31i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts1_ists_31i				   31 /*:31*/
	/* ---:--- */	#define	MSK32cmn_fIntSts1_ists_31i				       0x80000000
	/* ---:--- */	#define	GET32cmn_fIntSts1_ists_31i(r32)				 (((r32)>>31)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts1_ists_31i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts1_ists_31i(r32,v)			do{(r32)&=~(0x0001<<31);(r32)|=((v)&0x0001)<<31;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts1_ists_31i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

	#define	RA_cmn_fIntSts2 			0x00020
	#define	w32cmn_fIntSts2 			{										\
					UNSG32				ufIntSts2_ists_0i			 :  1;		\
					UNSG32				ufIntSts2_ists_1i			 :  1;		\
					UNSG32				ufIntSts2_ists_2i			 :  1;		\
					UNSG32				ufIntSts2_ists_3i			 :  1;		\
					UNSG32				ufIntSts2_ists_4i			 :  1;		\
					UNSG32				ufIntSts2_ists_5i			 :  1;		\
					UNSG32				ufIntSts2_ists_6i			 :  1;		\
					UNSG32				ufIntSts2_ists_7i			 :  1;		\
					UNSG32				ufIntSts2_ists_8i			 :  1;		\
					UNSG32				ufIntSts2_ists_9i			 :  1;		\
					UNSG32				ufIntSts2_ists_10i			 :  1;		\
					UNSG32				ufIntSts2_ists_11i			 :  1;		\
					UNSG32				ufIntSts2_ists_12i			 :  1;		\
					UNSG32				ufIntSts2_ists_13i			 :  1;		\
					UNSG32				ufIntSts2_ists_14i			 :  1;		\
					UNSG32				ufIntSts2_ists_15i			 :  1;		\
					UNSG32				ufIntSts2_ists_16i			 :  1;		\
					UNSG32				ufIntSts2_ists_17i			 :  1;		\
					UNSG32				ufIntSts2_ists_18i			 :  1;		\
					UNSG32				ufIntSts2_ists_19i			 :  1;		\
					UNSG32				ufIntSts2_ists_20i			 :  1;		\
					UNSG32				ufIntSts2_ists_21i			 :  1;		\
					UNSG32				ufIntSts2_ists_22i			 :  1;		\
					UNSG32				ufIntSts2_ists_23i			 :  1;		\
					UNSG32				ufIntSts2_ists_24i			 :  1;		\
					UNSG32				ufIntSts2_ists_25i			 :  1;		\
					UNSG32				ufIntSts2_ists_26i			 :  1;		\
					UNSG32				ufIntSts2_ists_27i			 :  1;		\
					UNSG32				ufIntSts2_ists_28i			 :  1;		\
					UNSG32				ufIntSts2_ists_29i			 :  1;		\
					UNSG32				ufIntSts2_ists_30i			 :  1;		\
					UNSG32				ufIntSts2_ists_31i			 :  1;		\
										}
	/* @'00020 */	union {	UNSG32		u32cmn_fIntSts2;
								struct	w32cmn_fIntSts2;
										};

	/* [ 0: 0] */	#define	  BA_cmn_fIntSts2_ists_0i				 0x00020
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_0i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_0i				    0 /*: 0*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_0i				       0x00000001
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_0i(r32)				 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_0i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_0i(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_0i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [ 1: 1] */	#define	  BA_cmn_fIntSts2_ists_1i				 0x00020
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_1i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_1i				    1 /*: 1*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_1i				       0x00000002
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_1i(r32)				 (((r32)>> 1)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_1i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_1i(r32,v)			do{(r32)&=~(0x0001<< 1);(r32)|=((v)&0x0001)<< 1;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_1i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [ 2: 2] */	#define	  BA_cmn_fIntSts2_ists_2i				 0x00020
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_2i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_2i				    2 /*: 2*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_2i				       0x00000004
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_2i(r32)				 (((r32)>> 2)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_2i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_2i(r32,v)			do{(r32)&=~(0x0001<< 2);(r32)|=((v)&0x0001)<< 2;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_2i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [ 3: 3] */	#define	  BA_cmn_fIntSts2_ists_3i				 0x00020
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_3i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_3i				    3 /*: 3*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_3i				       0x00000008
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_3i(r32)				 (((r32)>> 3)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_3i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_3i(r32,v)			do{(r32)&=~(0x0001<< 3);(r32)|=((v)&0x0001)<< 3;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_3i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [ 4: 4] */	#define	  BA_cmn_fIntSts2_ists_4i				 0x00020
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_4i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_4i				    4 /*: 4*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_4i				       0x00000010
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_4i(r32)				 (((r32)>> 4)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_4i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_4i(r32,v)			do{(r32)&=~(0x0001<< 4);(r32)|=((v)&0x0001)<< 4;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_4i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [ 5: 5] */	#define	  BA_cmn_fIntSts2_ists_5i				 0x00020
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_5i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_5i				    5 /*: 5*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_5i				       0x00000020
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_5i(r32)				 (((r32)>> 5)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_5i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_5i(r32,v)			do{(r32)&=~(0x0001<< 5);(r32)|=((v)&0x0001)<< 5;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_5i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [ 6: 6] */	#define	  BA_cmn_fIntSts2_ists_6i				 0x00020
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_6i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_6i				    6 /*: 6*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_6i				       0x00000040
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_6i(r32)				 (((r32)>> 6)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_6i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_6i(r32,v)			do{(r32)&=~(0x0001<< 6);(r32)|=((v)&0x0001)<< 6;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_6i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [ 7: 7] */	#define	  BA_cmn_fIntSts2_ists_7i				 0x00020
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_7i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_7i				    7 /*: 7*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_7i				       0x00000080
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_7i(r32)				 (((r32)>> 7)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_7i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_7i(r32,v)			do{(r32)&=~(0x0001<< 7);(r32)|=((v)&0x0001)<< 7;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_7i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [ 8: 8] */	#define	  BA_cmn_fIntSts2_ists_8i				 0x00021
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_8i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_8i				    8 /*: 8*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_8i				       0x00000100
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_8i(r32)				 (((r32)>> 8)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_8i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_8i(r32,v)			do{(r32)&=~(0x0001<< 8);(r32)|=((v)&0x0001)<< 8;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_8i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [ 9: 9] */	#define	  BA_cmn_fIntSts2_ists_9i				 0x00021
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_9i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_9i				    9 /*: 9*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_9i				       0x00000200
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_9i(r32)				 (((r32)>> 9)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_9i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_9i(r32,v)			do{(r32)&=~(0x0001<< 9);(r32)|=((v)&0x0001)<< 9;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_9i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [10:10] */	#define	  BA_cmn_fIntSts2_ists_10i				 0x00021
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_10i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_10i				   10 /*:10*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_10i				       0x00000400
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_10i(r32)				 (((r32)>>10)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_10i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_10i(r32,v)			do{(r32)&=~(0x0001<<10);(r32)|=((v)&0x0001)<<10;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_10i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [11:11] */	#define	  BA_cmn_fIntSts2_ists_11i				 0x00021
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_11i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_11i				   11 /*:11*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_11i				       0x00000800
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_11i(r32)				 (((r32)>>11)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_11i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_11i(r32,v)			do{(r32)&=~(0x0001<<11);(r32)|=((v)&0x0001)<<11;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_11i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [12:12] */	#define	  BA_cmn_fIntSts2_ists_12i				 0x00021
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_12i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_12i				   12 /*:12*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_12i				       0x00001000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_12i(r32)				 (((r32)>>12)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_12i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_12i(r32,v)			do{(r32)&=~(0x0001<<12);(r32)|=((v)&0x0001)<<12;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_12i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [13:13] */	#define	  BA_cmn_fIntSts2_ists_13i				 0x00021
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_13i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_13i				   13 /*:13*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_13i				       0x00002000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_13i(r32)				 (((r32)>>13)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_13i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_13i(r32,v)			do{(r32)&=~(0x0001<<13);(r32)|=((v)&0x0001)<<13;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_13i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [14:14] */	#define	  BA_cmn_fIntSts2_ists_14i				 0x00021
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_14i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_14i				   14 /*:14*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_14i				       0x00004000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_14i(r32)				 (((r32)>>14)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_14i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_14i(r32,v)			do{(r32)&=~(0x0001<<14);(r32)|=((v)&0x0001)<<14;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_14i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [15:15] */	#define	  BA_cmn_fIntSts2_ists_15i				 0x00021
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_15i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_15i				   15 /*:15*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_15i				       0x00008000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_15i(r32)				 (((r32)>>15)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_15i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_15i(r32,v)			do{(r32)&=~(0x0001<<15);(r32)|=((v)&0x0001)<<15;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_15i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

	/* [16:16] */	#define	  BA_cmn_fIntSts2_ists_16i				 0x00022
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_16i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_16i				   16 /*:16*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_16i				       0x00010000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_16i(r32)				 (((r32)>>16)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_16i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_16i(r32,v)			do{(r32)&=~(0x0001<<16);(r32)|=((v)&0x0001)<<16;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_16i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [17:17] */	#define	  BA_cmn_fIntSts2_ists_17i				 0x00022
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_17i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_17i				   17 /*:17*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_17i				       0x00020000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_17i(r32)				 (((r32)>>17)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_17i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_17i(r32,v)			do{(r32)&=~(0x0001<<17);(r32)|=((v)&0x0001)<<17;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_17i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [18:18] */	#define	  BA_cmn_fIntSts2_ists_18i				 0x00022
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_18i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_18i				   18 /*:18*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_18i				       0x00040000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_18i(r32)				 (((r32)>>18)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_18i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_18i(r32,v)			do{(r32)&=~(0x0001<<18);(r32)|=((v)&0x0001)<<18;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_18i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [19:19] */	#define	  BA_cmn_fIntSts2_ists_19i				 0x00022
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_19i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_19i				   19 /*:19*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_19i				       0x00080000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_19i(r32)				 (((r32)>>19)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_19i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_19i(r32,v)			do{(r32)&=~(0x0001<<19);(r32)|=((v)&0x0001)<<19;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_19i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [20:20] */	#define	  BA_cmn_fIntSts2_ists_20i				 0x00022
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_20i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_20i				   20 /*:20*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_20i				       0x00100000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_20i(r32)				 (((r32)>>20)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_20i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_20i(r32,v)			do{(r32)&=~(0x0001<<20);(r32)|=((v)&0x0001)<<20;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_20i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [21:21] */	#define	  BA_cmn_fIntSts2_ists_21i				 0x00022
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_21i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_21i				   21 /*:21*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_21i				       0x00200000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_21i(r32)				 (((r32)>>21)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_21i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_21i(r32,v)			do{(r32)&=~(0x0001<<21);(r32)|=((v)&0x0001)<<21;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_21i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [22:22] */	#define	  BA_cmn_fIntSts2_ists_22i				 0x00022
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_22i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_22i				   22 /*:22*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_22i				       0x00400000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_22i(r32)				 (((r32)>>22)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_22i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_22i(r32,v)			do{(r32)&=~(0x0001<<22);(r32)|=((v)&0x0001)<<22;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_22i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [23:23] */	#define	  BA_cmn_fIntSts2_ists_23i				 0x00022
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_23i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_23i				   23 /*:23*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_23i				       0x00800000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_23i(r32)				 (((r32)>>23)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_23i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_23i(r32,v)			do{(r32)&=~(0x0001<<23);(r32)|=((v)&0x0001)<<23;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_23i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [24:24] */	#define	  BA_cmn_fIntSts2_ists_24i				 0x00023
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_24i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_24i				   24 /*:24*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_24i				       0x01000000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_24i(r32)				 (((r32)>>24)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_24i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_24i(r32,v)			do{(r32)&=~(0x0001<<24);(r32)|=((v)&0x0001)<<24;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_24i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [25:25] */	#define	  BA_cmn_fIntSts2_ists_25i				 0x00023
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_25i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_25i				   25 /*:25*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_25i				       0x02000000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_25i(r32)				 (((r32)>>25)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_25i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_25i(r32,v)			do{(r32)&=~(0x0001<<25);(r32)|=((v)&0x0001)<<25;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_25i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [26:26] */	#define	  BA_cmn_fIntSts2_ists_26i				 0x00023
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_26i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_26i				   26 /*:26*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_26i				       0x04000000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_26i(r32)				 (((r32)>>26)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_26i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_26i(r32,v)			do{(r32)&=~(0x0001<<26);(r32)|=((v)&0x0001)<<26;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_26i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [27:27] */	#define	  BA_cmn_fIntSts2_ists_27i				 0x00023
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_27i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_27i				   27 /*:27*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_27i				       0x08000000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_27i(r32)				 (((r32)>>27)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_27i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_27i(r32,v)			do{(r32)&=~(0x0001<<27);(r32)|=((v)&0x0001)<<27;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_27i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [28:28] */	#define	  BA_cmn_fIntSts2_ists_28i				 0x00023
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_28i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_28i				   28 /*:28*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_28i				       0x10000000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_28i(r32)				 (((r32)>>28)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_28i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_28i(r32,v)			do{(r32)&=~(0x0001<<28);(r32)|=((v)&0x0001)<<28;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_28i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [29:29] */	#define	  BA_cmn_fIntSts2_ists_29i				 0x00023
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_29i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_29i				   29 /*:29*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_29i				       0x20000000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_29i(r32)				 (((r32)>>29)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_29i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_29i(r32,v)			do{(r32)&=~(0x0001<<29);(r32)|=((v)&0x0001)<<29;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_29i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [30:30] */	#define	  BA_cmn_fIntSts2_ists_30i				 0x00023
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_30i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_30i				   30 /*:30*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_30i				       0x40000000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_30i(r32)				 (((r32)>>30)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_30i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_30i(r32,v)			do{(r32)&=~(0x0001<<30);(r32)|=((v)&0x0001)<<30;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_30i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [31:31] */	#define	  BA_cmn_fIntSts2_ists_31i				 0x00023
	/* ---:--- */	#define	    bcmn_fIntSts2_ists_31i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_fIntSts2_ists_31i				   31 /*:31*/
	/* ---:--- */	#define	MSK32cmn_fIntSts2_ists_31i				       0x80000000
	/* ---:--- */	#define	GET32cmn_fIntSts2_ists_31i(r32)				 (((r32)>>31)&0x0001)
	/* ---:--- */	#define	GET16cmn_fIntSts2_ists_31i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_fIntSts2_ists_31i(r32,v)			do{(r32)&=~(0x0001<<31);(r32)|=((v)&0x0001)<<31;}while(0)
	/* ---:--- */	#define	SET16cmn_fIntSts2_ists_31i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

	#define	RA_cmn_nSrcPolSel0 			0x00024
	#define	w32cmn_nSrcPolSel0 			{										\
					UNSG32				unSrcPolSel0_pol			 : 32;		\
										}
	/* @'00024 */	union {	UNSG32		u32cmn_nSrcPolSel0;
								struct	w32cmn_nSrcPolSel0;
										};

	/* [31: 0] */	#define	  BA_cmn_nSrcPolSel0_pol				 0x00024
	/* ---:--- */	#define	    bcmn_nSrcPolSel0_pol				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nSrcPolSel0_pol				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_nSrcPolSel0_pol				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_nSrcPolSel0_pol(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_nSrcPolSel0_pol(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_nSrcPolSel0_pol(r32,v)				do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_nSrcPolSel0_pol(r16,v)				do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_nSrcPolSel1 			0x00028
	#define	w32cmn_nSrcPolSel1 			{										\
					UNSG32				unSrcPolSel1_pol			 : 32;		\
										}
	/* @'00028 */	union {	UNSG32		u32cmn_nSrcPolSel1;
								struct	w32cmn_nSrcPolSel1;
										};

	/* [31: 0] */	#define	  BA_cmn_nSrcPolSel1_pol				 0x00028
	/* ---:--- */	#define	    bcmn_nSrcPolSel1_pol				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nSrcPolSel1_pol				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_nSrcPolSel1_pol				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_nSrcPolSel1_pol(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_nSrcPolSel1_pol(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_nSrcPolSel1_pol(r32,v)				do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_nSrcPolSel1_pol(r16,v)				do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_nSrcPolSel2 			0x0002C
	#define	w32cmn_nSrcPolSel2 			{										\
					UNSG32				unSrcPolSel2_pol			 : 32;		\
										}
	/* @'0002C */	union {	UNSG32		u32cmn_nSrcPolSel2;
								struct	w32cmn_nSrcPolSel2;
										};

	/* [31: 0] */	#define	  BA_cmn_nSrcPolSel2_pol				 0x0002C
	/* ---:--- */	#define	    bcmn_nSrcPolSel2_pol				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nSrcPolSel2_pol				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_nSrcPolSel2_pol				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_nSrcPolSel2_pol(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_nSrcPolSel2_pol(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_nSrcPolSel2_pol(r32,v)				do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_nSrcPolSel2_pol(r16,v)				do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_nSrcSensSel0 		0x00030
	#define	w32cmn_nSrcSensSel0 		{										\
					UNSG32				unSrcSensSel0_sens			 : 32;		\
										}
	/* @'00030 */	union {	UNSG32		u32cmn_nSrcSensSel0;
								struct	w32cmn_nSrcSensSel0;
										};

	/* [31: 0] */	#define	  BA_cmn_nSrcSensSel0_sens				 0x00030
	/* ---:--- */	#define	    bcmn_nSrcSensSel0_sens				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nSrcSensSel0_sens				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_nSrcSensSel0_sens				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_nSrcSensSel0_sens(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_nSrcSensSel0_sens(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_nSrcSensSel0_sens(r32,v)			do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_nSrcSensSel0_sens(r16,v)			do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_nSrcSensSel1 		0x00034
	#define	w32cmn_nSrcSensSel1 		{										\
					UNSG32				unSrcSensSel1_sens			 : 32;		\
										}
	/* @'00034 */	union {	UNSG32		u32cmn_nSrcSensSel1;
								struct	w32cmn_nSrcSensSel1;
										};

	/* [31: 0] */	#define	  BA_cmn_nSrcSensSel1_sens				 0x00034
	/* ---:--- */	#define	    bcmn_nSrcSensSel1_sens				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nSrcSensSel1_sens				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_nSrcSensSel1_sens				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_nSrcSensSel1_sens(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_nSrcSensSel1_sens(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_nSrcSensSel1_sens(r32,v)			do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_nSrcSensSel1_sens(r16,v)			do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_nSrcSensSel2 		0x00038
	#define	w32cmn_nSrcSensSel2 		{										\
					UNSG32				unSrcSensSel2_sens			 : 32;		\
										}
	/* @'00038 */	union {	UNSG32		u32cmn_nSrcSensSel2;
								struct	w32cmn_nSrcSensSel2;
										};

	/* [31: 0] */	#define	  BA_cmn_nSrcSensSel2_sens				 0x00038
	/* ---:--- */	#define	    bcmn_nSrcSensSel2_sens				   32 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nSrcSensSel2_sens				    0 /*:31*/
	/* ---:--- */	#define	MSK32cmn_nSrcSensSel2_sens				       0xFFFFFFFF
	/* ---:--- */	#define	GET32cmn_nSrcSensSel2_sens(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16cmn_nSrcSensSel2_sens(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32cmn_nSrcSensSel2_sens(r32,v)			do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_nSrcSensSel2_sens(r16,v)			do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_cmn_nIntSts0 			0x0003C
	#define	w32cmn_nIntSts0 			{										\
					UNSG32				unIntSts0_ists_0i			 :  1;		\
					UNSG32				unIntSts0_ists_1i			 :  1;		\
					UNSG32				unIntSts0_ists_2i			 :  1;		\
					UNSG32				unIntSts0_ists_3i			 :  1;		\
					UNSG32				unIntSts0_ists_4i			 :  1;		\
					UNSG32				unIntSts0_ists_5i			 :  1;		\
					UNSG32				unIntSts0_ists_6i			 :  1;		\
					UNSG32				unIntSts0_ists_7i			 :  1;		\
					UNSG32				unIntSts0_ists_8i			 :  1;		\
					UNSG32				unIntSts0_ists_9i			 :  1;		\
					UNSG32				unIntSts0_ists_10i			 :  1;		\
					UNSG32				unIntSts0_ists_11i			 :  1;		\
					UNSG32				unIntSts0_ists_12i			 :  1;		\
					UNSG32				unIntSts0_ists_13i			 :  1;		\
					UNSG32				unIntSts0_ists_14i			 :  1;		\
					UNSG32				unIntSts0_ists_15i			 :  1;		\
					UNSG32				unIntSts0_ists_16i			 :  1;		\
					UNSG32				unIntSts0_ists_17i			 :  1;		\
					UNSG32				unIntSts0_ists_18i			 :  1;		\
					UNSG32				unIntSts0_ists_19i			 :  1;		\
					UNSG32				unIntSts0_ists_20i			 :  1;		\
					UNSG32				unIntSts0_ists_21i			 :  1;		\
					UNSG32				unIntSts0_ists_22i			 :  1;		\
					UNSG32				unIntSts0_ists_23i			 :  1;		\
					UNSG32				unIntSts0_ists_24i			 :  1;		\
					UNSG32				unIntSts0_ists_25i			 :  1;		\
					UNSG32				unIntSts0_ists_26i			 :  1;		\
					UNSG32				unIntSts0_ists_27i			 :  1;		\
					UNSG32				unIntSts0_ists_28i			 :  1;		\
					UNSG32				unIntSts0_ists_29i			 :  1;		\
					UNSG32				unIntSts0_ists_30i			 :  1;		\
					UNSG32				unIntSts0_ists_31i			 :  1;		\
										}
	/* @'0003C */	union {	UNSG32		u32cmn_nIntSts0;
								struct	w32cmn_nIntSts0;
										};

	/* [ 0: 0] */	#define	  BA_cmn_nIntSts0_ists_0i				 0x0003C
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_0i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_0i				    0 /*: 0*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_0i				       0x00000001
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_0i(r32)				 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_0i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_0i(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_0i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [ 1: 1] */	#define	  BA_cmn_nIntSts0_ists_1i				 0x0003C
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_1i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_1i				    1 /*: 1*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_1i				       0x00000002
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_1i(r32)				 (((r32)>> 1)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_1i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_1i(r32,v)			do{(r32)&=~(0x0001<< 1);(r32)|=((v)&0x0001)<< 1;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_1i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [ 2: 2] */	#define	  BA_cmn_nIntSts0_ists_2i				 0x0003C
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_2i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_2i				    2 /*: 2*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_2i				       0x00000004
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_2i(r32)				 (((r32)>> 2)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_2i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_2i(r32,v)			do{(r32)&=~(0x0001<< 2);(r32)|=((v)&0x0001)<< 2;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_2i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [ 3: 3] */	#define	  BA_cmn_nIntSts0_ists_3i				 0x0003C
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_3i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_3i				    3 /*: 3*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_3i				       0x00000008
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_3i(r32)				 (((r32)>> 3)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_3i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_3i(r32,v)			do{(r32)&=~(0x0001<< 3);(r32)|=((v)&0x0001)<< 3;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_3i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [ 4: 4] */	#define	  BA_cmn_nIntSts0_ists_4i				 0x0003C
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_4i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_4i				    4 /*: 4*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_4i				       0x00000010
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_4i(r32)				 (((r32)>> 4)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_4i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_4i(r32,v)			do{(r32)&=~(0x0001<< 4);(r32)|=((v)&0x0001)<< 4;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_4i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [ 5: 5] */	#define	  BA_cmn_nIntSts0_ists_5i				 0x0003C
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_5i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_5i				    5 /*: 5*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_5i				       0x00000020
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_5i(r32)				 (((r32)>> 5)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_5i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_5i(r32,v)			do{(r32)&=~(0x0001<< 5);(r32)|=((v)&0x0001)<< 5;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_5i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [ 6: 6] */	#define	  BA_cmn_nIntSts0_ists_6i				 0x0003C
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_6i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_6i				    6 /*: 6*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_6i				       0x00000040
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_6i(r32)				 (((r32)>> 6)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_6i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_6i(r32,v)			do{(r32)&=~(0x0001<< 6);(r32)|=((v)&0x0001)<< 6;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_6i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [ 7: 7] */	#define	  BA_cmn_nIntSts0_ists_7i				 0x0003C
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_7i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_7i				    7 /*: 7*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_7i				       0x00000080
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_7i(r32)				 (((r32)>> 7)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_7i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_7i(r32,v)			do{(r32)&=~(0x0001<< 7);(r32)|=((v)&0x0001)<< 7;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_7i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [ 8: 8] */	#define	  BA_cmn_nIntSts0_ists_8i				 0x0003D
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_8i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_8i				    8 /*: 8*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_8i				       0x00000100
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_8i(r32)				 (((r32)>> 8)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_8i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_8i(r32,v)			do{(r32)&=~(0x0001<< 8);(r32)|=((v)&0x0001)<< 8;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_8i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [ 9: 9] */	#define	  BA_cmn_nIntSts0_ists_9i				 0x0003D
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_9i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_9i				    9 /*: 9*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_9i				       0x00000200
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_9i(r32)				 (((r32)>> 9)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_9i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_9i(r32,v)			do{(r32)&=~(0x0001<< 9);(r32)|=((v)&0x0001)<< 9;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_9i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [10:10] */	#define	  BA_cmn_nIntSts0_ists_10i				 0x0003D
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_10i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_10i				   10 /*:10*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_10i				       0x00000400
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_10i(r32)				 (((r32)>>10)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_10i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_10i(r32,v)			do{(r32)&=~(0x0001<<10);(r32)|=((v)&0x0001)<<10;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_10i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [11:11] */	#define	  BA_cmn_nIntSts0_ists_11i				 0x0003D
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_11i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_11i				   11 /*:11*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_11i				       0x00000800
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_11i(r32)				 (((r32)>>11)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_11i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_11i(r32,v)			do{(r32)&=~(0x0001<<11);(r32)|=((v)&0x0001)<<11;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_11i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [12:12] */	#define	  BA_cmn_nIntSts0_ists_12i				 0x0003D
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_12i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_12i				   12 /*:12*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_12i				       0x00001000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_12i(r32)				 (((r32)>>12)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_12i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_12i(r32,v)			do{(r32)&=~(0x0001<<12);(r32)|=((v)&0x0001)<<12;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_12i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [13:13] */	#define	  BA_cmn_nIntSts0_ists_13i				 0x0003D
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_13i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_13i				   13 /*:13*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_13i				       0x00002000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_13i(r32)				 (((r32)>>13)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_13i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_13i(r32,v)			do{(r32)&=~(0x0001<<13);(r32)|=((v)&0x0001)<<13;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_13i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [14:14] */	#define	  BA_cmn_nIntSts0_ists_14i				 0x0003D
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_14i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_14i				   14 /*:14*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_14i				       0x00004000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_14i(r32)				 (((r32)>>14)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_14i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_14i(r32,v)			do{(r32)&=~(0x0001<<14);(r32)|=((v)&0x0001)<<14;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_14i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [15:15] */	#define	  BA_cmn_nIntSts0_ists_15i				 0x0003D
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_15i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_15i				   15 /*:15*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_15i				       0x00008000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_15i(r32)				 (((r32)>>15)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_15i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_15i(r32,v)			do{(r32)&=~(0x0001<<15);(r32)|=((v)&0x0001)<<15;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_15i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

	/* [16:16] */	#define	  BA_cmn_nIntSts0_ists_16i				 0x0003E
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_16i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_16i				   16 /*:16*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_16i				       0x00010000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_16i(r32)				 (((r32)>>16)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_16i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_16i(r32,v)			do{(r32)&=~(0x0001<<16);(r32)|=((v)&0x0001)<<16;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_16i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [17:17] */	#define	  BA_cmn_nIntSts0_ists_17i				 0x0003E
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_17i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_17i				   17 /*:17*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_17i				       0x00020000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_17i(r32)				 (((r32)>>17)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_17i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_17i(r32,v)			do{(r32)&=~(0x0001<<17);(r32)|=((v)&0x0001)<<17;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_17i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [18:18] */	#define	  BA_cmn_nIntSts0_ists_18i				 0x0003E
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_18i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_18i				   18 /*:18*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_18i				       0x00040000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_18i(r32)				 (((r32)>>18)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_18i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_18i(r32,v)			do{(r32)&=~(0x0001<<18);(r32)|=((v)&0x0001)<<18;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_18i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [19:19] */	#define	  BA_cmn_nIntSts0_ists_19i				 0x0003E
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_19i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_19i				   19 /*:19*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_19i				       0x00080000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_19i(r32)				 (((r32)>>19)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_19i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_19i(r32,v)			do{(r32)&=~(0x0001<<19);(r32)|=((v)&0x0001)<<19;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_19i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [20:20] */	#define	  BA_cmn_nIntSts0_ists_20i				 0x0003E
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_20i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_20i				   20 /*:20*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_20i				       0x00100000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_20i(r32)				 (((r32)>>20)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_20i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_20i(r32,v)			do{(r32)&=~(0x0001<<20);(r32)|=((v)&0x0001)<<20;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_20i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [21:21] */	#define	  BA_cmn_nIntSts0_ists_21i				 0x0003E
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_21i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_21i				   21 /*:21*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_21i				       0x00200000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_21i(r32)				 (((r32)>>21)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_21i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_21i(r32,v)			do{(r32)&=~(0x0001<<21);(r32)|=((v)&0x0001)<<21;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_21i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [22:22] */	#define	  BA_cmn_nIntSts0_ists_22i				 0x0003E
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_22i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_22i				   22 /*:22*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_22i				       0x00400000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_22i(r32)				 (((r32)>>22)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_22i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_22i(r32,v)			do{(r32)&=~(0x0001<<22);(r32)|=((v)&0x0001)<<22;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_22i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [23:23] */	#define	  BA_cmn_nIntSts0_ists_23i				 0x0003E
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_23i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_23i				   23 /*:23*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_23i				       0x00800000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_23i(r32)				 (((r32)>>23)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_23i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_23i(r32,v)			do{(r32)&=~(0x0001<<23);(r32)|=((v)&0x0001)<<23;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_23i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [24:24] */	#define	  BA_cmn_nIntSts0_ists_24i				 0x0003F
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_24i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_24i				   24 /*:24*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_24i				       0x01000000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_24i(r32)				 (((r32)>>24)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_24i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_24i(r32,v)			do{(r32)&=~(0x0001<<24);(r32)|=((v)&0x0001)<<24;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_24i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [25:25] */	#define	  BA_cmn_nIntSts0_ists_25i				 0x0003F
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_25i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_25i				   25 /*:25*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_25i				       0x02000000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_25i(r32)				 (((r32)>>25)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_25i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_25i(r32,v)			do{(r32)&=~(0x0001<<25);(r32)|=((v)&0x0001)<<25;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_25i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [26:26] */	#define	  BA_cmn_nIntSts0_ists_26i				 0x0003F
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_26i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_26i				   26 /*:26*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_26i				       0x04000000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_26i(r32)				 (((r32)>>26)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_26i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_26i(r32,v)			do{(r32)&=~(0x0001<<26);(r32)|=((v)&0x0001)<<26;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_26i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [27:27] */	#define	  BA_cmn_nIntSts0_ists_27i				 0x0003F
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_27i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_27i				   27 /*:27*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_27i				       0x08000000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_27i(r32)				 (((r32)>>27)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_27i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_27i(r32,v)			do{(r32)&=~(0x0001<<27);(r32)|=((v)&0x0001)<<27;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_27i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [28:28] */	#define	  BA_cmn_nIntSts0_ists_28i				 0x0003F
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_28i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_28i				   28 /*:28*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_28i				       0x10000000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_28i(r32)				 (((r32)>>28)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_28i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_28i(r32,v)			do{(r32)&=~(0x0001<<28);(r32)|=((v)&0x0001)<<28;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_28i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [29:29] */	#define	  BA_cmn_nIntSts0_ists_29i				 0x0003F
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_29i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_29i				   29 /*:29*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_29i				       0x20000000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_29i(r32)				 (((r32)>>29)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_29i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_29i(r32,v)			do{(r32)&=~(0x0001<<29);(r32)|=((v)&0x0001)<<29;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_29i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [30:30] */	#define	  BA_cmn_nIntSts0_ists_30i				 0x0003F
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_30i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_30i				   30 /*:30*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_30i				       0x40000000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_30i(r32)				 (((r32)>>30)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_30i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_30i(r32,v)			do{(r32)&=~(0x0001<<30);(r32)|=((v)&0x0001)<<30;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_30i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [31:31] */	#define	  BA_cmn_nIntSts0_ists_31i				 0x0003F
	/* ---:--- */	#define	    bcmn_nIntSts0_ists_31i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts0_ists_31i				   31 /*:31*/
	/* ---:--- */	#define	MSK32cmn_nIntSts0_ists_31i				       0x80000000
	/* ---:--- */	#define	GET32cmn_nIntSts0_ists_31i(r32)				 (((r32)>>31)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts0_ists_31i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts0_ists_31i(r32,v)			do{(r32)&=~(0x0001<<31);(r32)|=((v)&0x0001)<<31;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts0_ists_31i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

	#define	RA_cmn_nIntSts1 			0x00040
	#define	w32cmn_nIntSts1 			{										\
					UNSG32				unIntSts1_ists_0i			 :  1;		\
					UNSG32				unIntSts1_ists_1i			 :  1;		\
					UNSG32				unIntSts1_ists_2i			 :  1;		\
					UNSG32				unIntSts1_ists_3i			 :  1;		\
					UNSG32				unIntSts1_ists_4i			 :  1;		\
					UNSG32				unIntSts1_ists_5i			 :  1;		\
					UNSG32				unIntSts1_ists_6i			 :  1;		\
					UNSG32				unIntSts1_ists_7i			 :  1;		\
					UNSG32				unIntSts1_ists_8i			 :  1;		\
					UNSG32				unIntSts1_ists_9i			 :  1;		\
					UNSG32				unIntSts1_ists_10i			 :  1;		\
					UNSG32				unIntSts1_ists_11i			 :  1;		\
					UNSG32				unIntSts1_ists_12i			 :  1;		\
					UNSG32				unIntSts1_ists_13i			 :  1;		\
					UNSG32				unIntSts1_ists_14i			 :  1;		\
					UNSG32				unIntSts1_ists_15i			 :  1;		\
					UNSG32				unIntSts1_ists_16i			 :  1;		\
					UNSG32				unIntSts1_ists_17i			 :  1;		\
					UNSG32				unIntSts1_ists_18i			 :  1;		\
					UNSG32				unIntSts1_ists_19i			 :  1;		\
					UNSG32				unIntSts1_ists_20i			 :  1;		\
					UNSG32				unIntSts1_ists_21i			 :  1;		\
					UNSG32				unIntSts1_ists_22i			 :  1;		\
					UNSG32				unIntSts1_ists_23i			 :  1;		\
					UNSG32				unIntSts1_ists_24i			 :  1;		\
					UNSG32				unIntSts1_ists_25i			 :  1;		\
					UNSG32				unIntSts1_ists_26i			 :  1;		\
					UNSG32				unIntSts1_ists_27i			 :  1;		\
					UNSG32				unIntSts1_ists_28i			 :  1;		\
					UNSG32				unIntSts1_ists_29i			 :  1;		\
					UNSG32				unIntSts1_ists_30i			 :  1;		\
					UNSG32				unIntSts1_ists_31i			 :  1;		\
										}
	/* @'00040 */	union {	UNSG32		u32cmn_nIntSts1;
								struct	w32cmn_nIntSts1;
										};

	/* [ 0: 0] */	#define	  BA_cmn_nIntSts1_ists_0i				 0x00040
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_0i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_0i				    0 /*: 0*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_0i				       0x00000001
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_0i(r32)				 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_0i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_0i(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_0i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [ 1: 1] */	#define	  BA_cmn_nIntSts1_ists_1i				 0x00040
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_1i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_1i				    1 /*: 1*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_1i				       0x00000002
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_1i(r32)				 (((r32)>> 1)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_1i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_1i(r32,v)			do{(r32)&=~(0x0001<< 1);(r32)|=((v)&0x0001)<< 1;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_1i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [ 2: 2] */	#define	  BA_cmn_nIntSts1_ists_2i				 0x00040
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_2i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_2i				    2 /*: 2*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_2i				       0x00000004
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_2i(r32)				 (((r32)>> 2)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_2i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_2i(r32,v)			do{(r32)&=~(0x0001<< 2);(r32)|=((v)&0x0001)<< 2;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_2i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [ 3: 3] */	#define	  BA_cmn_nIntSts1_ists_3i				 0x00040
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_3i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_3i				    3 /*: 3*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_3i				       0x00000008
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_3i(r32)				 (((r32)>> 3)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_3i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_3i(r32,v)			do{(r32)&=~(0x0001<< 3);(r32)|=((v)&0x0001)<< 3;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_3i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [ 4: 4] */	#define	  BA_cmn_nIntSts1_ists_4i				 0x00040
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_4i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_4i				    4 /*: 4*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_4i				       0x00000010
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_4i(r32)				 (((r32)>> 4)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_4i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_4i(r32,v)			do{(r32)&=~(0x0001<< 4);(r32)|=((v)&0x0001)<< 4;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_4i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [ 5: 5] */	#define	  BA_cmn_nIntSts1_ists_5i				 0x00040
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_5i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_5i				    5 /*: 5*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_5i				       0x00000020
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_5i(r32)				 (((r32)>> 5)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_5i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_5i(r32,v)			do{(r32)&=~(0x0001<< 5);(r32)|=((v)&0x0001)<< 5;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_5i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [ 6: 6] */	#define	  BA_cmn_nIntSts1_ists_6i				 0x00040
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_6i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_6i				    6 /*: 6*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_6i				       0x00000040
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_6i(r32)				 (((r32)>> 6)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_6i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_6i(r32,v)			do{(r32)&=~(0x0001<< 6);(r32)|=((v)&0x0001)<< 6;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_6i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [ 7: 7] */	#define	  BA_cmn_nIntSts1_ists_7i				 0x00040
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_7i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_7i				    7 /*: 7*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_7i				       0x00000080
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_7i(r32)				 (((r32)>> 7)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_7i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_7i(r32,v)			do{(r32)&=~(0x0001<< 7);(r32)|=((v)&0x0001)<< 7;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_7i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [ 8: 8] */	#define	  BA_cmn_nIntSts1_ists_8i				 0x00041
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_8i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_8i				    8 /*: 8*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_8i				       0x00000100
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_8i(r32)				 (((r32)>> 8)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_8i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_8i(r32,v)			do{(r32)&=~(0x0001<< 8);(r32)|=((v)&0x0001)<< 8;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_8i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [ 9: 9] */	#define	  BA_cmn_nIntSts1_ists_9i				 0x00041
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_9i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_9i				    9 /*: 9*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_9i				       0x00000200
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_9i(r32)				 (((r32)>> 9)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_9i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_9i(r32,v)			do{(r32)&=~(0x0001<< 9);(r32)|=((v)&0x0001)<< 9;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_9i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [10:10] */	#define	  BA_cmn_nIntSts1_ists_10i				 0x00041
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_10i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_10i				   10 /*:10*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_10i				       0x00000400
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_10i(r32)				 (((r32)>>10)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_10i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_10i(r32,v)			do{(r32)&=~(0x0001<<10);(r32)|=((v)&0x0001)<<10;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_10i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [11:11] */	#define	  BA_cmn_nIntSts1_ists_11i				 0x00041
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_11i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_11i				   11 /*:11*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_11i				       0x00000800
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_11i(r32)				 (((r32)>>11)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_11i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_11i(r32,v)			do{(r32)&=~(0x0001<<11);(r32)|=((v)&0x0001)<<11;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_11i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [12:12] */	#define	  BA_cmn_nIntSts1_ists_12i				 0x00041
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_12i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_12i				   12 /*:12*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_12i				       0x00001000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_12i(r32)				 (((r32)>>12)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_12i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_12i(r32,v)			do{(r32)&=~(0x0001<<12);(r32)|=((v)&0x0001)<<12;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_12i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [13:13] */	#define	  BA_cmn_nIntSts1_ists_13i				 0x00041
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_13i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_13i				   13 /*:13*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_13i				       0x00002000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_13i(r32)				 (((r32)>>13)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_13i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_13i(r32,v)			do{(r32)&=~(0x0001<<13);(r32)|=((v)&0x0001)<<13;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_13i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [14:14] */	#define	  BA_cmn_nIntSts1_ists_14i				 0x00041
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_14i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_14i				   14 /*:14*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_14i				       0x00004000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_14i(r32)				 (((r32)>>14)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_14i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_14i(r32,v)			do{(r32)&=~(0x0001<<14);(r32)|=((v)&0x0001)<<14;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_14i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [15:15] */	#define	  BA_cmn_nIntSts1_ists_15i				 0x00041
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_15i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_15i				   15 /*:15*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_15i				       0x00008000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_15i(r32)				 (((r32)>>15)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_15i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_15i(r32,v)			do{(r32)&=~(0x0001<<15);(r32)|=((v)&0x0001)<<15;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_15i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

	/* [16:16] */	#define	  BA_cmn_nIntSts1_ists_16i				 0x00042
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_16i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_16i				   16 /*:16*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_16i				       0x00010000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_16i(r32)				 (((r32)>>16)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_16i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_16i(r32,v)			do{(r32)&=~(0x0001<<16);(r32)|=((v)&0x0001)<<16;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_16i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [17:17] */	#define	  BA_cmn_nIntSts1_ists_17i				 0x00042
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_17i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_17i				   17 /*:17*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_17i				       0x00020000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_17i(r32)				 (((r32)>>17)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_17i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_17i(r32,v)			do{(r32)&=~(0x0001<<17);(r32)|=((v)&0x0001)<<17;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_17i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [18:18] */	#define	  BA_cmn_nIntSts1_ists_18i				 0x00042
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_18i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_18i				   18 /*:18*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_18i				       0x00040000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_18i(r32)				 (((r32)>>18)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_18i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_18i(r32,v)			do{(r32)&=~(0x0001<<18);(r32)|=((v)&0x0001)<<18;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_18i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [19:19] */	#define	  BA_cmn_nIntSts1_ists_19i				 0x00042
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_19i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_19i				   19 /*:19*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_19i				       0x00080000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_19i(r32)				 (((r32)>>19)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_19i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_19i(r32,v)			do{(r32)&=~(0x0001<<19);(r32)|=((v)&0x0001)<<19;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_19i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [20:20] */	#define	  BA_cmn_nIntSts1_ists_20i				 0x00042
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_20i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_20i				   20 /*:20*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_20i				       0x00100000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_20i(r32)				 (((r32)>>20)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_20i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_20i(r32,v)			do{(r32)&=~(0x0001<<20);(r32)|=((v)&0x0001)<<20;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_20i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [21:21] */	#define	  BA_cmn_nIntSts1_ists_21i				 0x00042
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_21i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_21i				   21 /*:21*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_21i				       0x00200000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_21i(r32)				 (((r32)>>21)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_21i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_21i(r32,v)			do{(r32)&=~(0x0001<<21);(r32)|=((v)&0x0001)<<21;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_21i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [22:22] */	#define	  BA_cmn_nIntSts1_ists_22i				 0x00042
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_22i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_22i				   22 /*:22*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_22i				       0x00400000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_22i(r32)				 (((r32)>>22)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_22i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_22i(r32,v)			do{(r32)&=~(0x0001<<22);(r32)|=((v)&0x0001)<<22;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_22i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [23:23] */	#define	  BA_cmn_nIntSts1_ists_23i				 0x00042
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_23i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_23i				   23 /*:23*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_23i				       0x00800000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_23i(r32)				 (((r32)>>23)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_23i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_23i(r32,v)			do{(r32)&=~(0x0001<<23);(r32)|=((v)&0x0001)<<23;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_23i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [24:24] */	#define	  BA_cmn_nIntSts1_ists_24i				 0x00043
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_24i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_24i				   24 /*:24*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_24i				       0x01000000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_24i(r32)				 (((r32)>>24)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_24i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_24i(r32,v)			do{(r32)&=~(0x0001<<24);(r32)|=((v)&0x0001)<<24;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_24i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [25:25] */	#define	  BA_cmn_nIntSts1_ists_25i				 0x00043
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_25i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_25i				   25 /*:25*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_25i				       0x02000000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_25i(r32)				 (((r32)>>25)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_25i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_25i(r32,v)			do{(r32)&=~(0x0001<<25);(r32)|=((v)&0x0001)<<25;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_25i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [26:26] */	#define	  BA_cmn_nIntSts1_ists_26i				 0x00043
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_26i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_26i				   26 /*:26*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_26i				       0x04000000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_26i(r32)				 (((r32)>>26)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_26i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_26i(r32,v)			do{(r32)&=~(0x0001<<26);(r32)|=((v)&0x0001)<<26;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_26i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [27:27] */	#define	  BA_cmn_nIntSts1_ists_27i				 0x00043
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_27i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_27i				   27 /*:27*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_27i				       0x08000000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_27i(r32)				 (((r32)>>27)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_27i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_27i(r32,v)			do{(r32)&=~(0x0001<<27);(r32)|=((v)&0x0001)<<27;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_27i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [28:28] */	#define	  BA_cmn_nIntSts1_ists_28i				 0x00043
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_28i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_28i				   28 /*:28*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_28i				       0x10000000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_28i(r32)				 (((r32)>>28)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_28i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_28i(r32,v)			do{(r32)&=~(0x0001<<28);(r32)|=((v)&0x0001)<<28;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_28i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [29:29] */	#define	  BA_cmn_nIntSts1_ists_29i				 0x00043
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_29i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_29i				   29 /*:29*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_29i				       0x20000000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_29i(r32)				 (((r32)>>29)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_29i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_29i(r32,v)			do{(r32)&=~(0x0001<<29);(r32)|=((v)&0x0001)<<29;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_29i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [30:30] */	#define	  BA_cmn_nIntSts1_ists_30i				 0x00043
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_30i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_30i				   30 /*:30*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_30i				       0x40000000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_30i(r32)				 (((r32)>>30)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_30i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_30i(r32,v)			do{(r32)&=~(0x0001<<30);(r32)|=((v)&0x0001)<<30;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_30i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [31:31] */	#define	  BA_cmn_nIntSts1_ists_31i				 0x00043
	/* ---:--- */	#define	    bcmn_nIntSts1_ists_31i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts1_ists_31i				   31 /*:31*/
	/* ---:--- */	#define	MSK32cmn_nIntSts1_ists_31i				       0x80000000
	/* ---:--- */	#define	GET32cmn_nIntSts1_ists_31i(r32)				 (((r32)>>31)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts1_ists_31i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts1_ists_31i(r32,v)			do{(r32)&=~(0x0001<<31);(r32)|=((v)&0x0001)<<31;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts1_ists_31i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

	#define	RA_cmn_nIntSts2 			0x00044
	#define	w32cmn_nIntSts2 			{										\
					UNSG32				unIntSts2_ists_0i			 :  1;		\
					UNSG32				unIntSts2_ists_1i			 :  1;		\
					UNSG32				unIntSts2_ists_2i			 :  1;		\
					UNSG32				unIntSts2_ists_3i			 :  1;		\
					UNSG32				unIntSts2_ists_4i			 :  1;		\
					UNSG32				unIntSts2_ists_5i			 :  1;		\
					UNSG32				unIntSts2_ists_6i			 :  1;		\
					UNSG32				unIntSts2_ists_7i			 :  1;		\
					UNSG32				unIntSts2_ists_8i			 :  1;		\
					UNSG32				unIntSts2_ists_9i			 :  1;		\
					UNSG32				unIntSts2_ists_10i			 :  1;		\
					UNSG32				unIntSts2_ists_11i			 :  1;		\
					UNSG32				unIntSts2_ists_12i			 :  1;		\
					UNSG32				unIntSts2_ists_13i			 :  1;		\
					UNSG32				unIntSts2_ists_14i			 :  1;		\
					UNSG32				unIntSts2_ists_15i			 :  1;		\
					UNSG32				unIntSts2_ists_16i			 :  1;		\
					UNSG32				unIntSts2_ists_17i			 :  1;		\
					UNSG32				unIntSts2_ists_18i			 :  1;		\
					UNSG32				unIntSts2_ists_19i			 :  1;		\
					UNSG32				unIntSts2_ists_20i			 :  1;		\
					UNSG32				unIntSts2_ists_21i			 :  1;		\
					UNSG32				unIntSts2_ists_22i			 :  1;		\
					UNSG32				unIntSts2_ists_23i			 :  1;		\
					UNSG32				unIntSts2_ists_24i			 :  1;		\
					UNSG32				unIntSts2_ists_25i			 :  1;		\
					UNSG32				unIntSts2_ists_26i			 :  1;		\
					UNSG32				unIntSts2_ists_27i			 :  1;		\
					UNSG32				unIntSts2_ists_28i			 :  1;		\
					UNSG32				unIntSts2_ists_29i			 :  1;		\
					UNSG32				unIntSts2_ists_30i			 :  1;		\
					UNSG32				unIntSts2_ists_31i			 :  1;		\
										}
	/* @'00044 */	union {	UNSG32		u32cmn_nIntSts2;
								struct	w32cmn_nIntSts2;
										};

	/* [ 0: 0] */	#define	  BA_cmn_nIntSts2_ists_0i				 0x00044
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_0i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_0i				    0 /*: 0*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_0i				       0x00000001
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_0i(r32)				 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_0i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_0i(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_0i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [ 1: 1] */	#define	  BA_cmn_nIntSts2_ists_1i				 0x00044
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_1i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_1i				    1 /*: 1*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_1i				       0x00000002
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_1i(r32)				 (((r32)>> 1)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_1i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_1i(r32,v)			do{(r32)&=~(0x0001<< 1);(r32)|=((v)&0x0001)<< 1;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_1i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [ 2: 2] */	#define	  BA_cmn_nIntSts2_ists_2i				 0x00044
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_2i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_2i				    2 /*: 2*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_2i				       0x00000004
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_2i(r32)				 (((r32)>> 2)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_2i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_2i(r32,v)			do{(r32)&=~(0x0001<< 2);(r32)|=((v)&0x0001)<< 2;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_2i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [ 3: 3] */	#define	  BA_cmn_nIntSts2_ists_3i				 0x00044
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_3i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_3i				    3 /*: 3*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_3i				       0x00000008
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_3i(r32)				 (((r32)>> 3)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_3i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_3i(r32,v)			do{(r32)&=~(0x0001<< 3);(r32)|=((v)&0x0001)<< 3;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_3i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [ 4: 4] */	#define	  BA_cmn_nIntSts2_ists_4i				 0x00044
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_4i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_4i				    4 /*: 4*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_4i				       0x00000010
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_4i(r32)				 (((r32)>> 4)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_4i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_4i(r32,v)			do{(r32)&=~(0x0001<< 4);(r32)|=((v)&0x0001)<< 4;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_4i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [ 5: 5] */	#define	  BA_cmn_nIntSts2_ists_5i				 0x00044
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_5i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_5i				    5 /*: 5*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_5i				       0x00000020
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_5i(r32)				 (((r32)>> 5)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_5i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_5i(r32,v)			do{(r32)&=~(0x0001<< 5);(r32)|=((v)&0x0001)<< 5;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_5i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [ 6: 6] */	#define	  BA_cmn_nIntSts2_ists_6i				 0x00044
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_6i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_6i				    6 /*: 6*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_6i				       0x00000040
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_6i(r32)				 (((r32)>> 6)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_6i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_6i(r32,v)			do{(r32)&=~(0x0001<< 6);(r32)|=((v)&0x0001)<< 6;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_6i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [ 7: 7] */	#define	  BA_cmn_nIntSts2_ists_7i				 0x00044
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_7i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_7i				    7 /*: 7*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_7i				       0x00000080
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_7i(r32)				 (((r32)>> 7)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_7i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_7i(r32,v)			do{(r32)&=~(0x0001<< 7);(r32)|=((v)&0x0001)<< 7;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_7i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [ 8: 8] */	#define	  BA_cmn_nIntSts2_ists_8i				 0x00045
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_8i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_8i				    8 /*: 8*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_8i				       0x00000100
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_8i(r32)				 (((r32)>> 8)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_8i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_8i(r32,v)			do{(r32)&=~(0x0001<< 8);(r32)|=((v)&0x0001)<< 8;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_8i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [ 9: 9] */	#define	  BA_cmn_nIntSts2_ists_9i				 0x00045
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_9i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_9i				    9 /*: 9*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_9i				       0x00000200
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_9i(r32)				 (((r32)>> 9)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_9i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_9i(r32,v)			do{(r32)&=~(0x0001<< 9);(r32)|=((v)&0x0001)<< 9;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_9i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [10:10] */	#define	  BA_cmn_nIntSts2_ists_10i				 0x00045
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_10i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_10i				   10 /*:10*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_10i				       0x00000400
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_10i(r32)				 (((r32)>>10)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_10i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_10i(r32,v)			do{(r32)&=~(0x0001<<10);(r32)|=((v)&0x0001)<<10;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_10i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [11:11] */	#define	  BA_cmn_nIntSts2_ists_11i				 0x00045
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_11i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_11i				   11 /*:11*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_11i				       0x00000800
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_11i(r32)				 (((r32)>>11)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_11i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_11i(r32,v)			do{(r32)&=~(0x0001<<11);(r32)|=((v)&0x0001)<<11;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_11i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [12:12] */	#define	  BA_cmn_nIntSts2_ists_12i				 0x00045
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_12i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_12i				   12 /*:12*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_12i				       0x00001000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_12i(r32)				 (((r32)>>12)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_12i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_12i(r32,v)			do{(r32)&=~(0x0001<<12);(r32)|=((v)&0x0001)<<12;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_12i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [13:13] */	#define	  BA_cmn_nIntSts2_ists_13i				 0x00045
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_13i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_13i				   13 /*:13*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_13i				       0x00002000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_13i(r32)				 (((r32)>>13)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_13i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_13i(r32,v)			do{(r32)&=~(0x0001<<13);(r32)|=((v)&0x0001)<<13;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_13i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [14:14] */	#define	  BA_cmn_nIntSts2_ists_14i				 0x00045
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_14i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_14i				   14 /*:14*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_14i				       0x00004000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_14i(r32)				 (((r32)>>14)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_14i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_14i(r32,v)			do{(r32)&=~(0x0001<<14);(r32)|=((v)&0x0001)<<14;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_14i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [15:15] */	#define	  BA_cmn_nIntSts2_ists_15i				 0x00045
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_15i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_15i				   15 /*:15*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_15i				       0x00008000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_15i(r32)				 (((r32)>>15)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_15i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_15i(r32,v)			do{(r32)&=~(0x0001<<15);(r32)|=((v)&0x0001)<<15;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_15i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

	/* [16:16] */	#define	  BA_cmn_nIntSts2_ists_16i				 0x00046
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_16i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_16i				   16 /*:16*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_16i				       0x00010000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_16i(r32)				 (((r32)>>16)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_16i(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_16i(r32,v)			do{(r32)&=~(0x0001<<16);(r32)|=((v)&0x0001)<<16;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_16i(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	/* [17:17] */	#define	  BA_cmn_nIntSts2_ists_17i				 0x00046
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_17i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_17i				   17 /*:17*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_17i				       0x00020000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_17i(r32)				 (((r32)>>17)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_17i(r16)				 (((r16)>> 1)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_17i(r32,v)			do{(r32)&=~(0x0001<<17);(r32)|=((v)&0x0001)<<17;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_17i(r16,v)			do{(r16)&=~(0x0001<< 1);(r16)|=((v)&0x0001)<< 1;}while(0)

	/* [18:18] */	#define	  BA_cmn_nIntSts2_ists_18i				 0x00046
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_18i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_18i				   18 /*:18*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_18i				       0x00040000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_18i(r32)				 (((r32)>>18)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_18i(r16)				 (((r16)>> 2)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_18i(r32,v)			do{(r32)&=~(0x0001<<18);(r32)|=((v)&0x0001)<<18;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_18i(r16,v)			do{(r16)&=~(0x0001<< 2);(r16)|=((v)&0x0001)<< 2;}while(0)

	/* [19:19] */	#define	  BA_cmn_nIntSts2_ists_19i				 0x00046
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_19i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_19i				   19 /*:19*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_19i				       0x00080000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_19i(r32)				 (((r32)>>19)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_19i(r16)				 (((r16)>> 3)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_19i(r32,v)			do{(r32)&=~(0x0001<<19);(r32)|=((v)&0x0001)<<19;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_19i(r16,v)			do{(r16)&=~(0x0001<< 3);(r16)|=((v)&0x0001)<< 3;}while(0)

	/* [20:20] */	#define	  BA_cmn_nIntSts2_ists_20i				 0x00046
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_20i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_20i				   20 /*:20*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_20i				       0x00100000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_20i(r32)				 (((r32)>>20)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_20i(r16)				 (((r16)>> 4)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_20i(r32,v)			do{(r32)&=~(0x0001<<20);(r32)|=((v)&0x0001)<<20;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_20i(r16,v)			do{(r16)&=~(0x0001<< 4);(r16)|=((v)&0x0001)<< 4;}while(0)

	/* [21:21] */	#define	  BA_cmn_nIntSts2_ists_21i				 0x00046
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_21i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_21i				   21 /*:21*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_21i				       0x00200000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_21i(r32)				 (((r32)>>21)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_21i(r16)				 (((r16)>> 5)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_21i(r32,v)			do{(r32)&=~(0x0001<<21);(r32)|=((v)&0x0001)<<21;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_21i(r16,v)			do{(r16)&=~(0x0001<< 5);(r16)|=((v)&0x0001)<< 5;}while(0)

	/* [22:22] */	#define	  BA_cmn_nIntSts2_ists_22i				 0x00046
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_22i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_22i				   22 /*:22*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_22i				       0x00400000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_22i(r32)				 (((r32)>>22)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_22i(r16)				 (((r16)>> 6)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_22i(r32,v)			do{(r32)&=~(0x0001<<22);(r32)|=((v)&0x0001)<<22;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_22i(r16,v)			do{(r16)&=~(0x0001<< 6);(r16)|=((v)&0x0001)<< 6;}while(0)

	/* [23:23] */	#define	  BA_cmn_nIntSts2_ists_23i				 0x00046
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_23i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_23i				   23 /*:23*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_23i				       0x00800000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_23i(r32)				 (((r32)>>23)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_23i(r16)				 (((r16)>> 7)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_23i(r32,v)			do{(r32)&=~(0x0001<<23);(r32)|=((v)&0x0001)<<23;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_23i(r16,v)			do{(r16)&=~(0x0001<< 7);(r16)|=((v)&0x0001)<< 7;}while(0)

	/* [24:24] */	#define	  BA_cmn_nIntSts2_ists_24i				 0x00047
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_24i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_24i				   24 /*:24*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_24i				       0x01000000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_24i(r32)				 (((r32)>>24)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_24i(r16)				 (((r16)>> 8)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_24i(r32,v)			do{(r32)&=~(0x0001<<24);(r32)|=((v)&0x0001)<<24;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_24i(r16,v)			do{(r16)&=~(0x0001<< 8);(r16)|=((v)&0x0001)<< 8;}while(0)

	/* [25:25] */	#define	  BA_cmn_nIntSts2_ists_25i				 0x00047
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_25i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_25i				   25 /*:25*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_25i				       0x02000000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_25i(r32)				 (((r32)>>25)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_25i(r16)				 (((r16)>> 9)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_25i(r32,v)			do{(r32)&=~(0x0001<<25);(r32)|=((v)&0x0001)<<25;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_25i(r16,v)			do{(r16)&=~(0x0001<< 9);(r16)|=((v)&0x0001)<< 9;}while(0)

	/* [26:26] */	#define	  BA_cmn_nIntSts2_ists_26i				 0x00047
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_26i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_26i				   26 /*:26*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_26i				       0x04000000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_26i(r32)				 (((r32)>>26)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_26i(r16)				 (((r16)>>10)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_26i(r32,v)			do{(r32)&=~(0x0001<<26);(r32)|=((v)&0x0001)<<26;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_26i(r16,v)			do{(r16)&=~(0x0001<<10);(r16)|=((v)&0x0001)<<10;}while(0)

	/* [27:27] */	#define	  BA_cmn_nIntSts2_ists_27i				 0x00047
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_27i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_27i				   27 /*:27*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_27i				       0x08000000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_27i(r32)				 (((r32)>>27)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_27i(r16)				 (((r16)>>11)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_27i(r32,v)			do{(r32)&=~(0x0001<<27);(r32)|=((v)&0x0001)<<27;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_27i(r16,v)			do{(r16)&=~(0x0001<<11);(r16)|=((v)&0x0001)<<11;}while(0)

	/* [28:28] */	#define	  BA_cmn_nIntSts2_ists_28i				 0x00047
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_28i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_28i				   28 /*:28*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_28i				       0x10000000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_28i(r32)				 (((r32)>>28)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_28i(r16)				 (((r16)>>12)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_28i(r32,v)			do{(r32)&=~(0x0001<<28);(r32)|=((v)&0x0001)<<28;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_28i(r16,v)			do{(r16)&=~(0x0001<<12);(r16)|=((v)&0x0001)<<12;}while(0)

	/* [29:29] */	#define	  BA_cmn_nIntSts2_ists_29i				 0x00047
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_29i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_29i				   29 /*:29*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_29i				       0x20000000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_29i(r32)				 (((r32)>>29)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_29i(r16)				 (((r16)>>13)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_29i(r32,v)			do{(r32)&=~(0x0001<<29);(r32)|=((v)&0x0001)<<29;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_29i(r16,v)			do{(r16)&=~(0x0001<<13);(r16)|=((v)&0x0001)<<13;}while(0)

	/* [30:30] */	#define	  BA_cmn_nIntSts2_ists_30i				 0x00047
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_30i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_30i				   30 /*:30*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_30i				       0x40000000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_30i(r32)				 (((r32)>>30)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_30i(r16)				 (((r16)>>14)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_30i(r32,v)			do{(r32)&=~(0x0001<<30);(r32)|=((v)&0x0001)<<30;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_30i(r16,v)			do{(r16)&=~(0x0001<<14);(r16)|=((v)&0x0001)<<14;}while(0)

	/* [31:31] */	#define	  BA_cmn_nIntSts2_ists_31i				 0x00047
	/* ---:--- */	#define	    bcmn_nIntSts2_ists_31i				    1 /*bit*/
	/* ---:--- */	#define	LSb32cmn_nIntSts2_ists_31i				   31 /*:31*/
	/* ---:--- */	#define	MSK32cmn_nIntSts2_ists_31i				       0x80000000
	/* ---:--- */	#define	GET32cmn_nIntSts2_ists_31i(r32)				 (((r32)>>31)&0x0001)
	/* ---:--- */	#define	GET16cmn_nIntSts2_ists_31i(r16)				 (((r16)>>15)&0x0001)
	/* ---:--- */	#define	SET32cmn_nIntSts2_ists_31i(r32,v)			do{(r32)&=~(0x0001<<31);(r32)|=((v)&0x0001)<<31;}while(0)
	/* ---:--- */	#define	SET16cmn_nIntSts2_ists_31i(r16,v)			do{(r16)&=~(0x0001<<15);(r16)|=((v)&0x0001)<<15;}while(0)

} SIE_cmn;

	#define	dftcmn_fSrcPolSel0 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_fSrcPolSel0;
										} T32cmn_fSrcPolSel0;
	#define	dftcmn_fSrcPolSel1 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_fSrcPolSel1;
										} T32cmn_fSrcPolSel1;
	#define	dftcmn_fSrcPolSel2 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_fSrcPolSel2;
										} T32cmn_fSrcPolSel2;
	#define	dftcmn_fSrcSensSel0 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_fSrcSensSel0;
										} T32cmn_fSrcSensSel0;
	#define	dftcmn_fSrcSensSel1 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_fSrcSensSel1;
										} T32cmn_fSrcSensSel1;
	#define	dftcmn_fSrcSensSel2 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_fSrcSensSel2;
										} T32cmn_fSrcSensSel2;
	#define	dftcmn_fIntSts0 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_fIntSts0;
										} T32cmn_fIntSts0;
	#define	dftcmn_fIntSts1 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_fIntSts1;
										} T32cmn_fIntSts1;
	#define	dftcmn_fIntSts2 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_fIntSts2;
										} T32cmn_fIntSts2;
	#define	dftcmn_nSrcPolSel0 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_nSrcPolSel0;
										} T32cmn_nSrcPolSel0;
	#define	dftcmn_nSrcPolSel1 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_nSrcPolSel1;
										} T32cmn_nSrcPolSel1;
	#define	dftcmn_nSrcPolSel2 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_nSrcPolSel2;
										} T32cmn_nSrcPolSel2;
	#define	dftcmn_nSrcSensSel0 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_nSrcSensSel0;
										} T32cmn_nSrcSensSel0;
	#define	dftcmn_nSrcSensSel1 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_nSrcSensSel1;
										} T32cmn_nSrcSensSel1;
	#define	dftcmn_nSrcSensSel2 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_nSrcSensSel2;
										} T32cmn_nSrcSensSel2;
	#define	dftcmn_nIntSts0 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_nIntSts0;
										} T32cmn_nIntSts0;
	#define	dftcmn_nIntSts1 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_nIntSts1;
										} T32cmn_nIntSts1;
	#define	dftcmn_nIntSts2 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32cmn_nIntSts2;
										} T32cmn_nIntSts2;

/* Member function: read (or self test read) from SRAM or flat register interface and return words read */
SIGN32	cmn_drvrd (SIE_cmn *p, UNSG32 u32base, SIGN32 bMEM, SIGN32 bST);

/* Member function: generate write-commands or direct-write (or self test write) and return write count */
SIGN32	cmn_drvwr (SIE_cmn *p, UNSG32 u32base, SIGN32 bMEM, SIGN32 bST, UNSG32 *pu32cmd);

/* Member function: element reset */
void	cmn_reset (SIE_cmn *p);

/* Member function: check elements, return SUCCESS if identical or ERR_MISMATCH */
#define	cmn_check(p,pie,ps8name,hfpErrLOG)		cmn_cmp(p,pie, ps8name,hfpErrLOG,0,0)
#define	cmn_print(p,    ps8name,hfpErrLOG)		cmn_cmp(p,NULL,ps8name,hfpErrLOG,0,0)
SIGN32	cmn_cmp   (SIE_cmn *p, SIE_cmn *pie, char *ps8name, UNSG32 hfpErrLOG, SIGN32 bMEM, SIGN32 bST);

/* Member function: import interface values from a text string of compact all bits */
SIGN32	cmn_import(SIE_cmn *p, char *regs);

/* Member function: export interface values to a text string of compact all bits */
SIGN32	cmn_export(SIE_cmn *p, char *regs);

#endif
/**********************************************************************************************************************
**********************************************************************************************************************/



/**********************************************************************************************************************
**********************************************************************************************************************/
#ifdef	h_perPIC
#else
#define	h_perPIC

#define	\
	AR_perPIC \
		8
#define	\
	AB_perPIC \
		5

typedef struct SIE_perPIC {

	#define	RA_perPIC_fTgtPolSel 		0x00000
	#define	w32perPIC_fTgtPolSel 		{										\
					UNSG32				ufTgtPolSel_pol				 :  1;		\
					UNSG32				_x00000	: 31;							\
										}
	/* @'00000 */	union {	UNSG32		u32perPIC_fTgtPolSel;
								struct	w32perPIC_fTgtPolSel;
										};

	/* [ 0: 0] */	#define	  BA_perPIC_fTgtPolSel_pol				 0x00000
	/* ---:--- */	#define	    bperPIC_fTgtPolSel_pol				    1 /*bit*/
	/* ---:--- */	#define	LSb32perPIC_fTgtPolSel_pol				    0 /*: 0*/
	/* ---:--- */	#define	MSK32perPIC_fTgtPolSel_pol				       0x00000001
	/* ---:--- */	#define	GET32perPIC_fTgtPolSel_pol(r32)				 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16perPIC_fTgtPolSel_pol(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32perPIC_fTgtPolSel_pol(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16perPIC_fTgtPolSel_pol(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	#define	RA_perPIC_fTgtSensSel 		0x00004
	#define	w32perPIC_fTgtSensSel 		{										\
					UNSG32				ufTgtSensSel_sens			 :  1;		\
					UNSG32				_x00004	: 31;							\
										}
	/* @'00004 */	union {	UNSG32		u32perPIC_fTgtSensSel;
								struct	w32perPIC_fTgtSensSel;
										};

	/* [ 0: 0] */	#define	  BA_perPIC_fTgtSensSel_sens			 0x00004
	/* ---:--- */	#define	    bperPIC_fTgtSensSel_sens			    1 /*bit*/
	/* ---:--- */	#define	LSb32perPIC_fTgtSensSel_sens			    0 /*: 0*/
	/* ---:--- */	#define	MSK32perPIC_fTgtSensSel_sens			       0x00000001
	/* ---:--- */	#define	GET32perPIC_fTgtSensSel_sens(r32)			 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16perPIC_fTgtSensSel_sens(r16)			 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32perPIC_fTgtSensSel_sens(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16perPIC_fTgtSensSel_sens(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	#define	RA_perPIC_fIntE 			0x00008
	#define	w32perPIC_fIntE 			{										\
					UNSG32				ufIntE_ie					 : 32;		\
										}
	/* @'00008 */	union {	UNSG32		u32perPIC_fIntE;
								struct	w32perPIC_fIntE;
										};

	/* [31: 0] */	#define	  BA_perPIC_fIntE_ie					 0x00008
	/* ---:--- */	#define	    bperPIC_fIntE_ie					   32 /*bit*/
	/* ---:--- */	#define	LSb32perPIC_fIntE_ie					    0 /*:31*/
	/* ---:--- */	#define	MSK32perPIC_fIntE_ie					       0xFFFFFFFF
	/* ---:--- */	#define	GET32perPIC_fIntE_ie(r32)					 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16perPIC_fIntE_ie(r16)					 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32perPIC_fIntE_ie(r32,v)					do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16perPIC_fIntE_ie(r16,v)					do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_perPIC_fGIntE 			0x0000C
	#define	w32perPIC_fGIntE 			{										\
					UNSG32				ufGIntE_gIe					 :  1;		\
					UNSG32				_x0000C	: 31;							\
										}
	/* @'0000C */	union {	UNSG32		u32perPIC_fGIntE;
								struct	w32perPIC_fGIntE;
										};

	/* [ 0: 0] */	#define	  BA_perPIC_fGIntE_gIe					 0x0000C
	/* ---:--- */	#define	    bperPIC_fGIntE_gIe					    1 /*bit*/
	/* ---:--- */	#define	LSb32perPIC_fGIntE_gIe					    0 /*: 0*/
	/* ---:--- */	#define	MSK32perPIC_fGIntE_gIe					       0x00000001
	/* ---:--- */	#define	GET32perPIC_fGIntE_gIe(r32)					 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16perPIC_fGIntE_gIe(r16)					 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32perPIC_fGIntE_gIe(r32,v)				do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16perPIC_fGIntE_gIe(r16,v)				do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	#define	RA_perPIC_nTgtPolSel 		0x00010
	#define	w32perPIC_nTgtPolSel 		{										\
					UNSG32				unTgtPolSel_pol				 :  1;		\
					UNSG32				_x00010	: 31;							\
										}
	/* @'00010 */	union {	UNSG32		u32perPIC_nTgtPolSel;
								struct	w32perPIC_nTgtPolSel;
										};

	/* [ 0: 0] */	#define	  BA_perPIC_nTgtPolSel_pol				 0x00010
	/* ---:--- */	#define	    bperPIC_nTgtPolSel_pol				    1 /*bit*/
	/* ---:--- */	#define	LSb32perPIC_nTgtPolSel_pol				    0 /*: 0*/
	/* ---:--- */	#define	MSK32perPIC_nTgtPolSel_pol				       0x00000001
	/* ---:--- */	#define	GET32perPIC_nTgtPolSel_pol(r32)				 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16perPIC_nTgtPolSel_pol(r16)				 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32perPIC_nTgtPolSel_pol(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16perPIC_nTgtPolSel_pol(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	#define	RA_perPIC_nTgtSensSel 		0x00014
	#define	w32perPIC_nTgtSensSel 		{										\
					UNSG32				unTgtSensSel_sens			 :  1;		\
					UNSG32				_x00014	: 31;							\
										}
	/* @'00014 */	union {	UNSG32		u32perPIC_nTgtSensSel;
								struct	w32perPIC_nTgtSensSel;
										};

	/* [ 0: 0] */	#define	  BA_perPIC_nTgtSensSel_sens			 0x00014
	/* ---:--- */	#define	    bperPIC_nTgtSensSel_sens			    1 /*bit*/
	/* ---:--- */	#define	LSb32perPIC_nTgtSensSel_sens			    0 /*: 0*/
	/* ---:--- */	#define	MSK32perPIC_nTgtSensSel_sens			       0x00000001
	/* ---:--- */	#define	GET32perPIC_nTgtSensSel_sens(r32)			 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16perPIC_nTgtSensSel_sens(r16)			 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32perPIC_nTgtSensSel_sens(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16perPIC_nTgtSensSel_sens(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	#define	RA_perPIC_nIntE 			0x00018
	#define	w32perPIC_nIntE 			{										\
					UNSG32				unIntE_ie					 : 32;		\
										}
	/* @'00018 */	union {	UNSG32		u32perPIC_nIntE;
								struct	w32perPIC_nIntE;
										};

	/* [31: 0] */	#define	  BA_perPIC_nIntE_ie					 0x00018
	/* ---:--- */	#define	    bperPIC_nIntE_ie					   32 /*bit*/
	/* ---:--- */	#define	LSb32perPIC_nIntE_ie					    0 /*:31*/
	/* ---:--- */	#define	MSK32perPIC_nIntE_ie					       0xFFFFFFFF
	/* ---:--- */	#define	GET32perPIC_nIntE_ie(r32)					 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16perPIC_nIntE_ie(r16)					 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32perPIC_nIntE_ie(r32,v)					do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16perPIC_nIntE_ie(r16,v)					do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_perPIC_nGIntE 			0x0001C
	#define	w32perPIC_nGIntE 			{										\
					UNSG32				unGIntE_gIe					 :  1;		\
					UNSG32				_x0001C	: 31;							\
										}
	/* @'0001C */	union {	UNSG32		u32perPIC_nGIntE;
								struct	w32perPIC_nGIntE;
										};

	/* [ 0: 0] */	#define	  BA_perPIC_nGIntE_gIe					 0x0001C
	/* ---:--- */	#define	    bperPIC_nGIntE_gIe					    1 /*bit*/
	/* ---:--- */	#define	LSb32perPIC_nGIntE_gIe					    0 /*: 0*/
	/* ---:--- */	#define	MSK32perPIC_nGIntE_gIe					       0x00000001
	/* ---:--- */	#define	GET32perPIC_nGIntE_gIe(r32)					 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16perPIC_nGIntE_gIe(r16)					 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32perPIC_nGIntE_gIe(r32,v)				do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16perPIC_nGIntE_gIe(r16,v)				do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

} SIE_perPIC;

	#define	dftperPIC_fTgtPolSel 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32perPIC_fTgtPolSel;
										} T32perPIC_fTgtPolSel;
	#define	dftperPIC_fTgtSensSel 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32perPIC_fTgtSensSel;
										} T32perPIC_fTgtSensSel;
	#define	dftperPIC_fIntE 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32perPIC_fIntE;
										} T32perPIC_fIntE;
	#define	dftperPIC_fGIntE 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32perPIC_fGIntE;
										} T32perPIC_fGIntE;
	#define	dftperPIC_nTgtPolSel 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32perPIC_nTgtPolSel;
										} T32perPIC_nTgtPolSel;
	#define	dftperPIC_nTgtSensSel 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32perPIC_nTgtSensSel;
										} T32perPIC_nTgtSensSel;
	#define	dftperPIC_nIntE 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32perPIC_nIntE;
										} T32perPIC_nIntE;
	#define	dftperPIC_nGIntE 			0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32perPIC_nGIntE;
										} T32perPIC_nGIntE;

/* Member function: read (or self test read) from SRAM or flat register interface and return words read */
SIGN32	perPIC_drvrd (SIE_perPIC *p, UNSG32 u32base, SIGN32 bMEM, SIGN32 bST);

/* Member function: generate write-commands or direct-write (or self test write) and return write count */
SIGN32	perPIC_drvwr (SIE_perPIC *p, UNSG32 u32base, SIGN32 bMEM, SIGN32 bST, UNSG32 *pu32cmd);

/* Member function: element reset */
void	perPIC_reset (SIE_perPIC *p);

/* Member function: check elements, return SUCCESS if identical or ERR_MISMATCH */
#define	perPIC_check(p,pie,ps8name,hfpErrLOG)		perPIC_cmp(p,pie, ps8name,hfpErrLOG,0,0)
#define	perPIC_print(p,    ps8name,hfpErrLOG)		perPIC_cmp(p,NULL,ps8name,hfpErrLOG,0,0)
SIGN32	perPIC_cmp   (SIE_perPIC *p, SIE_perPIC *pie, char *ps8name, UNSG32 hfpErrLOG, SIGN32 bMEM, SIGN32 bST);

/* Member function: import interface values from a text string of compact all bits */
SIGN32	perPIC_import(SIE_perPIC *p, char *regs);

/* Member function: export interface values to a text string of compact all bits */
SIGN32	perPIC_export(SIE_perPIC *p, char *regs);

#endif
/**********************************************************************************************************************
**********************************************************************************************************************/



/**********************************************************************************************************************
**********************************************************************************************************************/
#ifdef	h_PIC
#else
#define	h_PIC

#define	\
	AR_PIC \
		54
#define	\
	AB_PIC \
		8

typedef struct SIE_PIC {

	#define	RA_PIC_CPU0 				0x00000
	/* @'00000 */	SIE_perPIC 					ie_CPU0;

	#define	RA_PIC_CPU1 				0x00020
	/* @'00020 */	SIE_perPIC 					ie_CPU1;

	#define	RA_PIC_PCIE 				0x00040
	/* @'00040 */	SIE_perPIC 					ie_PCIE;

	#define	RA_PIC_int_CPU0toCPU1 		0x00060
	#define	w32PIC_int_CPU0toCPU1 		{										\
					UNSG32				uint_CPU0toCPU1_int			 :  1;		\
					UNSG32				_x00060	: 31;							\
										}
	/* @'00060 */	union {	UNSG32		u32PIC_int_CPU0toCPU1;
								struct	w32PIC_int_CPU0toCPU1;
										};

	/* [ 0: 0] */	#define	  BA_PIC_int_CPU0toCPU1_int				 0x00060
	/* ---:--- */	#define	    bPIC_int_CPU0toCPU1_int				    1 /*bit*/
	/* ---:--- */	#define	LSb32PIC_int_CPU0toCPU1_int				    0 /*: 0*/
	/* ---:--- */	#define	MSK32PIC_int_CPU0toCPU1_int				       0x00000001
	/* ---:--- */	#define	GET32PIC_int_CPU0toCPU1_int(r32)			 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16PIC_int_CPU0toCPU1_int(r16)			 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32PIC_int_CPU0toCPU1_int(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_int_CPU0toCPU1_int(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	#define	RA_PIC_data_CPU0toCPU1 		0x00064
	#define	w32PIC_data_CPU0toCPU1 		{										\
					UNSG32				udata_CPU0toCPU1_data		 : 32;		\
										}
	/* @'00064 */	union {	UNSG32		u32PIC_data_CPU0toCPU1;
								struct	w32PIC_data_CPU0toCPU1;
										};

	/* [31: 0] */	#define	  BA_PIC_data_CPU0toCPU1_data			 0x00064
	/* ---:--- */	#define	    bPIC_data_CPU0toCPU1_data			   32 /*bit*/
	/* ---:--- */	#define	LSb32PIC_data_CPU0toCPU1_data			    0 /*:31*/
	/* ---:--- */	#define	MSK32PIC_data_CPU0toCPU1_data			       0xFFFFFFFF
	/* ---:--- */	#define	GET32PIC_data_CPU0toCPU1_data(r32)			 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16PIC_data_CPU0toCPU1_data(r16)			 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32PIC_data_CPU0toCPU1_data(r32,v)		do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_data_CPU0toCPU1_data(r16,v)		do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_PIC_int_CPU0toPCIE 		0x00068
	#define	w32PIC_int_CPU0toPCIE 		{										\
					UNSG32				uint_CPU0toPCIE_int			 :  1;		\
					UNSG32				_x00068	: 31;							\
										}
	/* @'00068 */	union {	UNSG32		u32PIC_int_CPU0toPCIE;
								struct	w32PIC_int_CPU0toPCIE;
										};

	/* [ 0: 0] */	#define	  BA_PIC_int_CPU0toPCIE_int				 0x00068
	/* ---:--- */	#define	    bPIC_int_CPU0toPCIE_int				    1 /*bit*/
	/* ---:--- */	#define	LSb32PIC_int_CPU0toPCIE_int				    0 /*: 0*/
	/* ---:--- */	#define	MSK32PIC_int_CPU0toPCIE_int				       0x00000001
	/* ---:--- */	#define	GET32PIC_int_CPU0toPCIE_int(r32)			 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16PIC_int_CPU0toPCIE_int(r16)			 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32PIC_int_CPU0toPCIE_int(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_int_CPU0toPCIE_int(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	#define	RA_PIC_data_CPU0toPCIE 		0x0006C
	#define	w32PIC_data_CPU0toPCIE 		{										\
					UNSG32				udata_CPU0toPCIE_data		 : 32;		\
										}
	/* @'0006C */	union {	UNSG32		u32PIC_data_CPU0toPCIE;
								struct	w32PIC_data_CPU0toPCIE;
										};

	/* [31: 0] */	#define	  BA_PIC_data_CPU0toPCIE_data			 0x0006C
	/* ---:--- */	#define	    bPIC_data_CPU0toPCIE_data			   32 /*bit*/
	/* ---:--- */	#define	LSb32PIC_data_CPU0toPCIE_data			    0 /*:31*/
	/* ---:--- */	#define	MSK32PIC_data_CPU0toPCIE_data			       0xFFFFFFFF
	/* ---:--- */	#define	GET32PIC_data_CPU0toPCIE_data(r32)			 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16PIC_data_CPU0toPCIE_data(r16)			 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32PIC_data_CPU0toPCIE_data(r32,v)		do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_data_CPU0toPCIE_data(r16,v)		do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_PIC_int_CPU1toCPU0 		0x00070
	#define	w32PIC_int_CPU1toCPU0 		{										\
					UNSG32				uint_CPU1toCPU0_int			 :  1;		\
					UNSG32				_x00070	: 31;							\
										}
	/* @'00070 */	union {	UNSG32		u32PIC_int_CPU1toCPU0;
								struct	w32PIC_int_CPU1toCPU0;
										};

	/* [ 0: 0] */	#define	  BA_PIC_int_CPU1toCPU0_int				 0x00070
	/* ---:--- */	#define	    bPIC_int_CPU1toCPU0_int				    1 /*bit*/
	/* ---:--- */	#define	LSb32PIC_int_CPU1toCPU0_int				    0 /*: 0*/
	/* ---:--- */	#define	MSK32PIC_int_CPU1toCPU0_int				       0x00000001
	/* ---:--- */	#define	GET32PIC_int_CPU1toCPU0_int(r32)			 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16PIC_int_CPU1toCPU0_int(r16)			 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32PIC_int_CPU1toCPU0_int(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_int_CPU1toCPU0_int(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	#define	RA_PIC_data_CPU1toCPU0 		0x00074
	#define	w32PIC_data_CPU1toCPU0 		{										\
					UNSG32				udata_CPU1toCPU0_data		 : 32;		\
										}
	/* @'00074 */	union {	UNSG32		u32PIC_data_CPU1toCPU0;
								struct	w32PIC_data_CPU1toCPU0;
										};

	/* [31: 0] */	#define	  BA_PIC_data_CPU1toCPU0_data			 0x00074
	/* ---:--- */	#define	    bPIC_data_CPU1toCPU0_data			   32 /*bit*/
	/* ---:--- */	#define	LSb32PIC_data_CPU1toCPU0_data			    0 /*:31*/
	/* ---:--- */	#define	MSK32PIC_data_CPU1toCPU0_data			       0xFFFFFFFF
	/* ---:--- */	#define	GET32PIC_data_CPU1toCPU0_data(r32)			 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16PIC_data_CPU1toCPU0_data(r16)			 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32PIC_data_CPU1toCPU0_data(r32,v)		do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_data_CPU1toCPU0_data(r16,v)		do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_PIC_int_CPU1toPCIE 		0x00078
	#define	w32PIC_int_CPU1toPCIE 		{										\
					UNSG32				uint_CPU1toPCIE_int			 :  1;		\
					UNSG32				_x00078	: 31;							\
										}
	/* @'00078 */	union {	UNSG32		u32PIC_int_CPU1toPCIE;
								struct	w32PIC_int_CPU1toPCIE;
										};

	/* [ 0: 0] */	#define	  BA_PIC_int_CPU1toPCIE_int				 0x00078
	/* ---:--- */	#define	    bPIC_int_CPU1toPCIE_int				    1 /*bit*/
	/* ---:--- */	#define	LSb32PIC_int_CPU1toPCIE_int				    0 /*: 0*/
	/* ---:--- */	#define	MSK32PIC_int_CPU1toPCIE_int				       0x00000001
	/* ---:--- */	#define	GET32PIC_int_CPU1toPCIE_int(r32)			 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16PIC_int_CPU1toPCIE_int(r16)			 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32PIC_int_CPU1toPCIE_int(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_int_CPU1toPCIE_int(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	#define	RA_PIC_data_CPU1toPCIE 		0x0007C
	#define	w32PIC_data_CPU1toPCIE 		{										\
					UNSG32				udata_CPU1toPCIE_data		 : 32;		\
										}
	/* @'0007C */	union {	UNSG32		u32PIC_data_CPU1toPCIE;
								struct	w32PIC_data_CPU1toPCIE;
										};

	/* [31: 0] */	#define	  BA_PIC_data_CPU1toPCIE_data			 0x0007C
	/* ---:--- */	#define	    bPIC_data_CPU1toPCIE_data			   32 /*bit*/
	/* ---:--- */	#define	LSb32PIC_data_CPU1toPCIE_data			    0 /*:31*/
	/* ---:--- */	#define	MSK32PIC_data_CPU1toPCIE_data			       0xFFFFFFFF
	/* ---:--- */	#define	GET32PIC_data_CPU1toPCIE_data(r32)			 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16PIC_data_CPU1toPCIE_data(r16)			 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32PIC_data_CPU1toPCIE_data(r32,v)		do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_data_CPU1toPCIE_data(r16,v)		do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_PIC_int_PCIEtoCPU0 		0x00080
	#define	w32PIC_int_PCIEtoCPU0 		{										\
					UNSG32				uint_PCIEtoCPU0_int			 :  1;		\
					UNSG32				_x00080	: 31;							\
										}
	/* @'00080 */	union {	UNSG32		u32PIC_int_PCIEtoCPU0;
								struct	w32PIC_int_PCIEtoCPU0;
										};

	/* [ 0: 0] */	#define	  BA_PIC_int_PCIEtoCPU0_int				 0x00080
	/* ---:--- */	#define	    bPIC_int_PCIEtoCPU0_int				    1 /*bit*/
	/* ---:--- */	#define	LSb32PIC_int_PCIEtoCPU0_int				    0 /*: 0*/
	/* ---:--- */	#define	MSK32PIC_int_PCIEtoCPU0_int				       0x00000001
	/* ---:--- */	#define	GET32PIC_int_PCIEtoCPU0_int(r32)			 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16PIC_int_PCIEtoCPU0_int(r16)			 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32PIC_int_PCIEtoCPU0_int(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_int_PCIEtoCPU0_int(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	#define	RA_PIC_data_PCIEtoCPU0 		0x00084
	#define	w32PIC_data_PCIEtoCPU0 		{										\
					UNSG32				udata_PCIEtoCPU0_data		 : 32;		\
										}
	/* @'00084 */	union {	UNSG32		u32PIC_data_PCIEtoCPU0;
								struct	w32PIC_data_PCIEtoCPU0;
										};

	/* [31: 0] */	#define	  BA_PIC_data_PCIEtoCPU0_data			 0x00084
	/* ---:--- */	#define	    bPIC_data_PCIEtoCPU0_data			   32 /*bit*/
	/* ---:--- */	#define	LSb32PIC_data_PCIEtoCPU0_data			    0 /*:31*/
	/* ---:--- */	#define	MSK32PIC_data_PCIEtoCPU0_data			       0xFFFFFFFF
	/* ---:--- */	#define	GET32PIC_data_PCIEtoCPU0_data(r32)			 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16PIC_data_PCIEtoCPU0_data(r16)			 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32PIC_data_PCIEtoCPU0_data(r32,v)		do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_data_PCIEtoCPU0_data(r16,v)		do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_PIC_int_PCIEtoCPU1 		0x00088
	#define	w32PIC_int_PCIEtoCPU1 		{										\
					UNSG32				uint_PCIEtoCPU1_int			 :  1;		\
					UNSG32				_x00088	: 31;							\
										}
	/* @'00088 */	union {	UNSG32		u32PIC_int_PCIEtoCPU1;
								struct	w32PIC_int_PCIEtoCPU1;
										};

	/* [ 0: 0] */	#define	  BA_PIC_int_PCIEtoCPU1_int				 0x00088
	/* ---:--- */	#define	    bPIC_int_PCIEtoCPU1_int				    1 /*bit*/
	/* ---:--- */	#define	LSb32PIC_int_PCIEtoCPU1_int				    0 /*: 0*/
	/* ---:--- */	#define	MSK32PIC_int_PCIEtoCPU1_int				       0x00000001
	/* ---:--- */	#define	GET32PIC_int_PCIEtoCPU1_int(r32)			 (((r32)>> 0)&0x0001)
	/* ---:--- */	#define	GET16PIC_int_PCIEtoCPU1_int(r16)			 (((r16)>> 0)&0x0001)
	/* ---:--- */	#define	SET32PIC_int_PCIEtoCPU1_int(r32,v)			do{(r32)&=~(0x0001<< 0);(r32)|=((v)&0x0001)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_int_PCIEtoCPU1_int(r16,v)			do{(r16)&=~(0x0001<< 0);(r16)|=((v)&0x0001)<< 0;}while(0)

	#define	RA_PIC_data_PCIEtoCPU1 		0x0008C
	#define	w32PIC_data_PCIEtoCPU1 		{										\
					UNSG32				udata_PCIEtoCPU1_data		 : 32;		\
										}
	/* @'0008C */	union {	UNSG32		u32PIC_data_PCIEtoCPU1;
								struct	w32PIC_data_PCIEtoCPU1;
										};

	/* [31: 0] */	#define	  BA_PIC_data_PCIEtoCPU1_data			 0x0008C
	/* ---:--- */	#define	    bPIC_data_PCIEtoCPU1_data			   32 /*bit*/
	/* ---:--- */	#define	LSb32PIC_data_PCIEtoCPU1_data			    0 /*:31*/
	/* ---:--- */	#define	MSK32PIC_data_PCIEtoCPU1_data			       0xFFFFFFFF
	/* ---:--- */	#define	GET32PIC_data_PCIEtoCPU1_data(r32)			 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16PIC_data_PCIEtoCPU1_data(r16)			 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32PIC_data_PCIEtoCPU1_data(r32,v)		do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16PIC_data_PCIEtoCPU1_data(r16,v)		do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

	#define	RA_PIC_cmn 					0x00090
	/* @'00090 */	SIE_cmn 					ie_cmn;

} SIE_PIC;

	#define	dftPIC_int_CPU0toCPU1 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_int_CPU0toCPU1;
										} T32PIC_int_CPU0toCPU1;
	#define	dftPIC_data_CPU0toCPU1 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_data_CPU0toCPU1;
										} T32PIC_data_CPU0toCPU1;
	#define	dftPIC_int_CPU0toPCIE 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_int_CPU0toPCIE;
										} T32PIC_int_CPU0toPCIE;
	#define	dftPIC_data_CPU0toPCIE 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_data_CPU0toPCIE;
										} T32PIC_data_CPU0toPCIE;
	#define	dftPIC_int_CPU1toCPU0 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_int_CPU1toCPU0;
										} T32PIC_int_CPU1toCPU0;
	#define	dftPIC_data_CPU1toCPU0 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_data_CPU1toCPU0;
										} T32PIC_data_CPU1toCPU0;
	#define	dftPIC_int_CPU1toPCIE 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_int_CPU1toPCIE;
										} T32PIC_int_CPU1toPCIE;
	#define	dftPIC_data_CPU1toPCIE 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_data_CPU1toPCIE;
										} T32PIC_data_CPU1toPCIE;
	#define	dftPIC_int_PCIEtoCPU0 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_int_PCIEtoCPU0;
										} T32PIC_int_PCIEtoCPU0;
	#define	dftPIC_data_PCIEtoCPU0 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_data_PCIEtoCPU0;
										} T32PIC_data_PCIEtoCPU0;
	#define	dftPIC_int_PCIEtoCPU1 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_int_PCIEtoCPU1;
										} T32PIC_int_PCIEtoCPU1;
	#define	dftPIC_data_PCIEtoCPU1 		0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32PIC_data_PCIEtoCPU1;
										} T32PIC_data_PCIEtoCPU1;

/* Member function: read (or self test read) from SRAM or flat register interface and return words read */
SIGN32	PIC_drvrd (SIE_PIC *p, UNSG32 u32base, SIGN32 bMEM, SIGN32 bST);

/* Member function: generate write-commands or direct-write (or self test write) and return write count */
SIGN32	PIC_drvwr (SIE_PIC *p, UNSG32 u32base, SIGN32 bMEM, SIGN32 bST, UNSG32 *pu32cmd);

/* Member function: element reset */
void	PIC_reset (SIE_PIC *p);

/* Member function: check elements, return SUCCESS if identical or ERR_MISMATCH */
#define	PIC_check(p,pie,ps8name,hfpErrLOG)		PIC_cmp(p,pie, ps8name,hfpErrLOG,0,0)
#define	PIC_print(p,    ps8name,hfpErrLOG)		PIC_cmp(p,NULL,ps8name,hfpErrLOG,0,0)
SIGN32	PIC_cmp   (SIE_PIC *p, SIE_PIC *pie, char *ps8name, UNSG32 hfpErrLOG, SIGN32 bMEM, SIGN32 bST);

/* Member function: import interface values from a text string of compact all bits */
SIGN32	PIC_import(SIE_PIC *p, char *regs);

/* Member function: export interface values to a text string of compact all bits */
SIGN32	PIC_export(SIE_PIC *p, char *regs);

#endif
/**********************************************************************************************************************
**********************************************************************************************************************/



/**********************************************************************************************************************
**********************************************************************************************************************/
#ifdef	h_IRQ
#else
#define	h_IRQ

#define	\
	AR_IRQ \
		1
#define	\
	AB_IRQ \
		2

	#define	IRQ_dHubIntrAvio0 			0x0
	#define	IRQ_dHubIntrAvio1 			0x1
	#define	IRQ_dHubIntrVpro 			0x2
	#define	IRQ_ictlInst0CPUIrq 		0x3
	#define	IRQ_pex2intr_int 			0x4
	#define	IRQ_pex2intr_err 			0x5
	#define	IRQ_intrGfxM0 				0x6
	#define	IRQ_SATAintr 				0x7
	#define	IRQ_ethIrq 					0x8
	#define	IRQ_drmIntr 				0x9
	#define	IRQ_drmFigoIntr 			0xA
	#define	IRQ_usb0Interrupt 			0xB
	#define	IRQ_usb1Interrupt 			0xC
	#define	IRQ_sm2socInt 				0xD
	#define	IRQ_ptpIrq 					0xE
	#define	IRQ_sm2socHwInt0 			0xF
	#define	IRQ_sm2socHwInt1 			0x10
	#define	IRQ_sdio_interrupt 			0x11
	#define	IRQ_intrPb 					0x12
	#define	IRQ_zspInt 					0x13
	#define	IRQ_TOTAL_IRQ 				0x14

typedef struct SIE_IRQ {

	#define	RA_IRQ_dummy 				0x00000
	#define	w32IRQ_dummy 				{										\
					UNSG32				udummy_0x00000000			 : 32;		\
										}
	/* @'00000 */	union {	UNSG32		u32IRQ_dummy;
								struct	w32IRQ_dummy;
										};

	/* [31: 0] */	#define	  BA_IRQ_dummy_0x00000000				 0x00000
	/* ---:--- */	#define	    bIRQ_dummy_0x00000000				   32 /*bit*/
	/* ---:--- */	#define	LSb32IRQ_dummy_0x00000000				    0 /*:31*/
	/* ---:--- */	#define	MSK32IRQ_dummy_0x00000000				       0xFFFFFFFF
	/* ---:--- */	#define	GET32IRQ_dummy_0x00000000(r32)				 (((r32)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	GET16IRQ_dummy_0x00000000(r16)				 (((r16)>> 0)&0xFFFFFFFF)
	/* ---:--- */	#define	SET32IRQ_dummy_0x00000000(r32,v)			do{(r32)&=~(0xFFFFFFFF<< 0);(r32)|=((v)&0xFFFFFFFF)<< 0;}while(0)
	/* ---:--- */	#define	SET16IRQ_dummy_0x00000000(r16,v)			do{(r16)&=~(0xFFFFFFFF<< 0);(r16)|=((v)&0xFFFFFFFF)<< 0;}while(0)

} SIE_IRQ;

	#define	dftIRQ_dummy 				0x00000000
			typedef	union {	UNSG32		u32;
								struct	w32IRQ_dummy;
										} T32IRQ_dummy;

/* Member function: read (or self test read) from SRAM or flat register interface and return words read */
SIGN32	IRQ_drvrd (SIE_IRQ *p, UNSG32 u32base, SIGN32 bMEM, SIGN32 bST);

/* Member function: generate write-commands or direct-write (or self test write) and return write count */
SIGN32	IRQ_drvwr (SIE_IRQ *p, UNSG32 u32base, SIGN32 bMEM, SIGN32 bST, UNSG32 *pu32cmd);

/* Member function: element reset */
void	IRQ_reset (SIE_IRQ *p);

/* Member function: check elements, return SUCCESS if identical or ERR_MISMATCH */
#define	IRQ_check(p,pie,ps8name,hfpErrLOG)		IRQ_cmp(p,pie, ps8name,hfpErrLOG,0,0)
#define	IRQ_print(p,    ps8name,hfpErrLOG)		IRQ_cmp(p,NULL,ps8name,hfpErrLOG,0,0)
SIGN32	IRQ_cmp   (SIE_IRQ *p, SIE_IRQ *pie, char *ps8name, UNSG32 hfpErrLOG, SIGN32 bMEM, SIGN32 bST);

/* Member function: import interface values from a text string of compact all bits */
SIGN32	IRQ_import(SIE_IRQ *p, char *regs);

/* Member function: export interface values to a text string of compact all bits */
SIGN32	IRQ_export(SIE_IRQ *p, char *regs);

#endif
/**********************************************************************************************************************
**********************************************************************************************************************/



#pragma	pack()

#ifdef	__cplusplus
	}
#endif

#endif	/* _pic_h */
/**********************************************************************************************************************
**********************************************************************************************************************/

