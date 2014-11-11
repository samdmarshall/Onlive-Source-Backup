/**********************************************************************************************************************
*
*
*
*
**********************************************************************************************************************/

#ifndef	UTIL_API
#define	UTIL_API					"        UTIL_API >>>    "
/**	UTIL_API
 */



/**	SECTION - memory operations
 */
	/******************************************************************************************************************
	*	Function: _api_PckCpy
	*	Prototype description: ../cutil.h
	******************************************************************************************************************/
	INLINE	void	_api_PckCpy(
						void	*p,
						void	*psrc,
						SIGN32	s32packs
						)
	{
#ifdef	MMX_WIN32
	__asm
	{
			mov			$CNT,				s32packs
			cmp			$CNT,				0
			je			$quit

			mov			eax,				psrc
			mov			ebx,				p
			movq		mm0,				$QPTR [eax +   0]
			movq		mm1,				$QPTR [eax +   8]
			movq		mm2,				$QPTR [eax +  16]
			movq		mm3,				$QPTR [eax +  24]
	$cp32B:
			add			eax,				32
			sub			$CNT,				1
			je			$flush

			movq		$QPTR [ebx +   0],	mm0
			movq		mm0,				$QPTR [eax +   0]
			movq		$QPTR [ebx +   8],	mm1
			movq		mm1,				$QPTR [eax +   8]
			movq		$QPTR [ebx +  16],	mm2
			movq		mm2,				$QPTR [eax +  16]
			movq		$QPTR [ebx +  24],	mm3
			movq		mm3,				$QPTR [eax +  24]

			add			ebx,				32
			jmp			$cp32B
	$flush:
			movq		$QPTR [ebx +   0],	mm0
			movq		$QPTR [ebx +   8],	mm1
			movq		$QPTR [ebx +  16],	mm2
			movq		$QPTR [ebx +  24],	mm3
	$quit:
			emms
	}
#else
	SIGN32	i, *p32 = (SIGN32*)p, *p32src = (SIGN32*)psrc;
	for(i = s32packs; i > 0; i --) {
		p32[0] = p32src[0]; p32[1] = p32src[1]; p32[2] = p32src[2]; p32[3] = p32src[3];
		p32[4] = p32src[4]; p32[5] = p32src[5]; p32[6] = p32src[6]; p32[7] = p32src[7];
		p32 += 8; p32src += 8;
	}
#endif
	/**	ENDOFFUNCTION: _api_PckCpy **/
	}



	/******************************************************************************************************************
	*	Function: _api_PckSet
	*	Prototype description: ../cutil.h
	******************************************************************************************************************/
	INLINE	void	_api_PckSet(
						void	*p,
						SIGN32	s32v,
						SIGN32	s32packs
						)
	{
#ifdef	MMX_WIN32
	SIGN32	arr32[2] = { s32v, s32v }, *p32 = (SIGN32 *)arr32;			/* Initiate 8B buffer */
	__asm
	{
			mov			$CNT,				s32packs
			cmp			$CNT,				0
			je			$quit

			mov			eax,				p32
			mov			ebx,				p
			movq		mm0,				$QPTR [eax]
	$wr32B:
			movq		$QPTR [ebx +   0],	mm0
			movq		$QPTR [ebx +   8],	mm0
			movq		$QPTR [ebx +  16],	mm0
			movq		$QPTR [ebx +  24],	mm0

			add			ebx,				32
			sub			$CNT,				1
			jne			$wr32B
	$quit:
			emms
	}
#else
	SIGN32	i, *p32 = (SIGN32*)p;
	for(i = s32packs; i > 0; i --) {
		p32[0] = p32[1] = p32[2] = p32[3] = p32[4] = p32[5] = p32[6] = p32[7] = s32v; p32 += 8;
	}
#endif
	/**	ENDOFFUNCTION: _api_PckSet **/
	}

/**	ENDOFSECTION
 */



/**	UTIL_API
 */
#endif

/**	ENDOFFILE: api.h **************************************************************************************************
 */

