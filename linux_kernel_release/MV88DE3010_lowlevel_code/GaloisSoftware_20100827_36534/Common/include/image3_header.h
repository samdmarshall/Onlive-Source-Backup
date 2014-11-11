#ifndef __IMAGE3_HEADER_H
#define	__IMAGE3_HEADER_H

#define	MAX_LINUX_BOOTARGS_LEN		4096

typedef struct __sm_image_attr{
	int bsm_image_active ;						// whether need to load sm image 
	unsigned int uism_image_load_addr ; 		// the address sm image is loaded to
	unsigned int uism_image_ori_size ;			// the original size of sm image
	unsigned int uism_image_final_size ;		// the final size of sm image
	int bsm_image_encrypt ;						// whether sm image is encrypted
	unsigned int uism_encrypt_image_size ;		// the size of encrypted sm image 
	unsigned int uism_encrypt_image_body_size ; // the size of encrypted image body	
	int bsm_image_bss_init ; 					// whether need to init sm image's bbs
	unsigned int uism_bss_start_addr ; 			// the start address of sm bss section
	unsigned int uism_bss_length ;				// the length of sm bss section
}sm_image_attr ; 

typedef	struct __cpu0_image_attr{
	int bcpu0_image_active ; 
	unsigned int uicpu0_image_load_addr ; 
	unsigned int uicpu0_image_ori_size ; 
	unsigned int uicpu0_image_final_size ; 
	int bcpu0_image_encrypt ;	
	unsigned int uicpu0_encrypt_image_size ; 
	unsigned int uicpu0_encrypt_image_body_size ; 
	int bcpu0_image_bss_init ; 
	unsigned int uicpu0_bss_start_addr ; 
	unsigned int uicpu0_bss_length ; 
}cpu0_image_attr ; 

typedef	struct __cpu1_image_attr{
	int bcpu1_image_active ; 
	unsigned int uicpu1_image_load_addr ;
	unsigned int uicpu1_image_ori_size ; 
	unsigned int uicpu1_image_final_size ; 
	int bcpu1_image_compress ;
	unsigned int uicpu1_compress_image_size ; 
	int bcpu1_image_encrypt ; 
	unsigned int uicpu1_encrypt_image_size ; 
	unsigned int uicpu1_encrypt_image_body_size ; 
	int bcpu1_image_bss_init ; 
	unsigned int uicpu1_bss_start_addr ; 
	unsigned int uicpu1_bss_length ;
	unsigned int uicpu1_figo_section_start_addr ; 
	unsigned int uicpu1_figo_section_size ;
}cpu1_image_attr ; 


#define		IMAGE_ENCRYPTION_HEADER_MAX				256
#define		FIGO_SECTION_MAX						(100<<10)
typedef	struct Encryption_Header{
	unsigned int uiSM_Encrypt_Image_Header_Offset ; 
	unsigned int uiSM_Encrypt_Image_Header_Size ;
	unsigned int uiCPU0_Encrypt_Image_Header_Offset ; 
	unsigned int uiCPU0_Encrypt_Image_Header_Size ; 
	unsigned int uiCPU1_Encrypt_Image_Header_Offset ; 
	unsigned int uiCPU1_Encrypt_Image_Header_Size ;
	unsigned int uiCPU1_Figo_Section_Offset ;
	unsigned int uiCPU1_Figo_Section_Size ; 
	unsigned int reserve[56];			// 64 int align
}Encryption_Header ;

typedef struct  __Mem_Layout{
	unsigned int uizsp_mem_size ; 
	unsigned int uicpu1_mem_size ; 
	unsigned int uicpu0_mem_size ; 
	unsigned int uishm_mem_size ; 
}Mem_Layout ; 

typedef struct  {
	unsigned int size; 
}recovery_ou_attr; 

typedef	struct __Image3_Attr{
	sm_image_attr 			sm_param ;
	cpu0_image_attr 		cpu0_param ; 
	cpu1_image_attr			cpu1_param ;
	recovery_ou_attr		recou_param;
	Mem_Layout			mem_layout ; 
	unsigned char 		linux_bootargs[MAX_LINUX_BOOTARGS_LEN];
}Image3_Attr ;

#endif
