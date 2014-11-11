
/*******************************************************************************
*******************************************************************************/



#ifndef __GALOIS_ALLOC_H__
#define __GALOIS_ALLOC_H__

void *Galois_align_malloc(UINT32 size, UINT32 align);
void Galois_align_free(void * buf);

#endif
