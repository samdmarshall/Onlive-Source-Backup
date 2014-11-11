
#ifndef	CUTIL
#define	CUTIL						"           CUTIL >>>    "
/**	CUTIL
 */

#include	"cinclude.h"



#ifdef	__cplusplus
/**	SECTION - template utilities
 */
	/******************************************************************************************************************
	*	DEFINITION - universal type print template
	******************************************************************************************************************/
	template <class T> INLINE void _iout(T v, UNSG32 hfp)				{ _fLOG(hfp, ("%d", v)); }
	template <> INLINE void _iout<UNSG8>(UNSG8 v, UNSG32 hfp)			{ _fLOG(hfp, ("%02X", v)); }
	template <> INLINE void _iout<UNSG16>(UNSG16 v, UNSG32 hfp)			{ _fLOG(hfp, ("%04X", v)); }
	template <> INLINE void _iout<UNSG32>(UNSG32 v, UNSG32 hfp)			{ _fLOG(hfp, ("%08X", v)); }

/**	ENDOFSECTION
 */
	extern	"C"
	{
#endif

/**	SECTION - global temopral buffers
 */
	Global	UNSG32	garru32[KILO];
	Global	SIGN32	GlobalAssign(*garrs32, (SIGN32*)garru32);
	Global	char	GlobalAssign(*garrs8, ((char*)garru32) + KILO * 3);
	Global	UNSG32	GlobalAssign(**garrpu32, (UNSG32**)garru32);
	Global	SIGN32	GlobalAssign(**garrps32, (SIGN32**)garru32);
	Global	char	GlobalAssign(**garrps8, (char**)garru32);

/**	ENDOFSECTION
 */



/**	SECTION - common memory functions in "../_util/api.h" & "../_util/api.c"
 */
	/******************************************************************************************************************
	*	Function: _api_PckCpy
	*	Description: Software optimized 32B-packed (required at 8B aligned boundaries) memory copy.
	******************************************************************************************************************/
	INLINE	void	_api_PckCpy(
						void	*p,						/*!	Destination pointer !*/
						void	*psrc,					/*!	Source pointer !*/
						SIGN32	s32packs				/*!	Count of 32B packages to copy !*/
						);

	/******************************************************************************************************************
	*	Function: _api_PckSet
	*	Description: Software optimized 32B-packed (required at 8B aligned boundaries) memory set.
	******************************************************************************************************************/
	INLINE	void	_api_PckSet(
						void	*p,						/*!	Memory pointer (8B aligned) !*/
						SIGN32	s32v,					/*!	32b Value to be set !*/
						SIGN32	s32packs				/*!	Count (at least 1) of 32B packages to set !*/
						);

	/******************************************************************************************************************
	*	Software optimized inline implementation
	******************************************************************************************************************/
	#include	"api.h"

	/******************************************************************************************************************
	*	Function: _api_MemCpy
	*	Description: Software optimized memory copy.
	******************************************************************************************************************/
	void	_api_MemCpy(void	*p,						/*!	Destination pointer !*/
						void	*psrc,					/*!	Source pointer !*/
						SIGN32	s32bytes				/*!	Count of bytes to copy !*/
						);

	/******************************************************************************************************************
	*	Function: _api_MemSet
	*	Description: Software optimized memory set.
	******************************************************************************************************************/
	void	_api_MemSet(void	*p,						/*!	Memory pointer (should be 's32unit'-byte aligned) !*/
						SIGN32	s32v,					/*!	Value (8/16/32b) to be set !*/
						SIGN32	s32unit,				/*!	1/2/4 (only) to set 8/16/32b value !*/
						SIGN32	s32bytes				/*!	Total bytes (should be times of 's32unit') to be set !*/
						);
/**	ENDOFSECTION
 */



#ifndef	__CODE_LINK__
/**	__CODE_LINK__: all
 */

/**	SECTION - miscellaneous functions in "../_util/api.c"
 */
	/******************************************************************************************************************
	*	Function: _api_curSec
	*	Description: Get accurate current time.
	*	Return:			REAL64						-	Current time in sec
	******************************************************************************************************************/
	REAL64	_api_curSec(char	*ps8time				/*!	Non-zero to receive current time display in format:
															"Feb 14, 2000 (Mon) - 06:30:45"
															!*/
						);

	/******************************************************************************************************************
	*	Function: _api_random
	*	Description: Get unified random integer.
	*	Return:			SIGN32						-	Generated random number
	******************************************************************************************************************/
	SIGN32	_api_random(UNSG32	*pu32s,					/*!	Non-zero to hold current random seed, or
															NULL to use default (hidden) seed
															!*/
						SIGN32	s32bound				/*!	Random range:
															0 for auto (re)seeding
															>0: range from 0 to s32bound
															<0: range from s32bound to - s32bound
															!*/
						);
	#define	randSeed(ps, s)								do{	*(ps) = (s); _api_random(ps, s); }while(0)
	#define	randInit(s)									do{	UNSG32 u; randSeed(&u, s); }while(0)
	#define	randN(n)									_api_random(NULL, n)

	/******************************************************************************************************************
	*	Function: _api_rdmIdx
	*	Description: Get random selection of events.
	*	Return:			SIGN32						-	Event index
	******************************************************************************************************************/
	SIGN32	_api_rdmIdx(UNSG32	*pu32s,					/*!	Non-zero to hold current random seed, or
															NULL to use default (hidden) seed
															!*/
						SIGN32	s32num,					/*!	Number of events !*/
						SIGN32	*ps32rate				/*!	Events' probabilities !*/
						);

	/******************************************************************************************************************
	*	Function: _api_rdmExp
	*	Description: Get a random value based on base-exp distribution.
	*		RandomLimit = scl*(base^(exp+1)-1)/(base-1)-1
	*	Return:			SIGN32						-	Generated random number
	******************************************************************************************************************/
	SIGN32	_api_rdmExp(UNSG32	*pu32s,					/*!	Non-zero to hold current random seed, or
															NULL to use default (hidden) seed
															!*/
						REAL64	r64scl,					/*!	Even probability zone scale factor
															>0: unsigned return range [0, RandomLimit]
															<0: signed return range [-RandomLimit, RandomLimit]
															!*/
						REAL64	r64base,				/*!	>1 required, 2 for Golomb-Exp binary !*/
						SIGN32	s32expMax				/*!	Maximum exp-order, 0 for even distribution !*/
						);
	#define	randGolomb(ps, k, expb)						_api_rdmExp(ps, 1 << (k), 2, expb)

	/******************************************************************************************************************
	*	Function: _api_rdmGma
	*	Description: Get a random value based on distribution of exponential gamma curve.
	*	Return:			SIGN32						-	Generated random number
	******************************************************************************************************************/
	SIGN32	_api_rdmGma(UNSG32	*pu32s,					/*!	Non-zero to hold current random seed, or
															NULL to use default (hidden) seed
															!*/
						SIGN32	s32x,					/*!	1~99, gamma point value percentage !*/
						SIGN32	s32y,					/*!	1~99, gamma point probability percentage !*/
						SIGN32	s32bound				/*!	Random range:
															0 for auto (re)seeding
															>0: range from 0 to s32bound
															<0: range from s32bound to ABS(s32bound)
															!*/
						);
/**	ENDOFSECTION
 */



/**	SECTION - string/file functions in "../_util/api.c"
 */
	/******************************************************************************************************************
	*	Function: StringPath
	*	Description: Replace ' ', '.', '/' or '\' to '_' in a string of file path.
	******************************************************************************************************************/
	void	StringPath(	char	*ps8path,				/*!	Full path name to process !*/
						char	*ps8str					/*!	Pre-allocated buffer to return processed path
															NULL will process to input string buffer (ps8path) itself
															!*/
						);

	/******************************************************************************************************************
	*	Function: ModifyFileSuffix
	*	Description: Replace file extension.
	******************************************************************************************************************/
	void	ModifyFileSuffix(
						char	*ps8path,				/*!	File name (optional with path) to modify !*/
						char	*ps8sfx					/*!	New suffix to be replaced to (".exe e.g.")
															NULL to remove current extension only
															!*/
						);

	/******************************************************************************************************************
	*	Function: MergePath
	*	Description: Merge a directory and a file name to a full path.
	******************************************************************************************************************/
	void	MergePath(	char	*ps8path,				/*!	Pre-allocated buffer to return merged path !*/
						char	*ps8dir,				/*!	Name of directory to merge
															Note: with/without '/' or '\' at the end both supported
															!*/
						char	*ps8filename			/*!	File name to merge !*/
						);

	/******************************************************************************************************************
	*	Function: SplitPath
	*	Description: Split a full path to a directory and a file name.
	******************************************************************************************************************/
	void	SplitPath(	char	*ps8path,				/*!	Full path name to split !*/
						char	*ps8dir,				/*!	Non-zero to return directory name ended with '/' !*/
						char	*ps8filename			/*!	Non-zero to return file name !*/
						);

	/******************************************************************************************************************
	*	Function: StringHeading
	*	Description: Padding a string to a aligned length with '\t' or ' '.
	******************************************************************************************************************/
	void	StringHeading(
						char	*ps8str,				/*!	Pre-allocated string buffer !*/
						char	cpadding,				/*!	'\t' or ' ' !*/
						SIGN32	s32alignment			/*!	Number of padding symbols to align,
															positive for left align (pad on right);
															negative for right align (pad on left);
															!*/
						);

	/******************************************************************************************************************
	*	Function: _dirmk
	*	Description: Test and create (if not existing) a directory.
	******************************************************************************************************************/
	void	_dirmk(		char	*ps8dir					/*!	Name of directory to be created !*/
						);

	/******************************************************************************************************************
	*	Function: _filemk
	*	Description: Test and create (if not existing) a file.
	*	Return:			FILE*						-	Opend file handle
	******************************************************************************************************************/
	FILE*	_filemk(	char	*ps8path,				/*!	Full path name to be created !*/
						char	*ps8openmode			/*!	Standard open mode ("wt" e.g.) !*/
						);

	/******************************************************************************************************************
	*	Function: _comment
	*	Description: Output a comment line to a file.
	******************************************************************************************************************/
	void	_comment(	char	*mark,					/*!	"/""*" or "////" or ";;" or "##" !*/
						FILE	*fp,					/*!	Output file !*/
						SIGN32	s32tab,					/*!	Number of 4-space tabs in front !*/
						SIGN32	bHeadLine,				/*!	Comment head-line (1) or bottom-line (0) !*/
						char	*ps8comment				/*!	Comment content !*/
						);
	#define	_fcomment(fp, tab, head, comment)			_comment("/*", fp, tab, head, comment)
	#define	_lcomment(fp, tab, head, comment)			_comment("////", fp, tab, head, comment)

/**	ENDOFSECTION
 */



/**	SECTION - functions in "../_util/cfg.c"
 */
	/******************************************************************************************************************
	*	Function: _cfgAPI_Format
	*	Description: Filter and format configuration file from a text file.
	*	Return:			SIGN32						-	SUCCESS or
	*													ERR_FILE if the text or configuration file could not open
	******************************************************************************************************************/
	SIGN32	_cfgAPI_Format(
						char	*ps8txt,				/*!	Orignal text file name !*/
						char	*ps8cfg,				/*!	Destination configuration file name !*/
						char	s8Keep,					/*!	Lead symbol keep same line, '#' as a sample !*/
						char	s8Remove,				/*!	Lead symbol keep following lines, '*' as a sample !*/
						char	*ps8Begin,				/*!	Keyword to begin a section, "$INTERFACE" as a sample !*/
						char	*ps8End,				/*!	Keyword to end a section, "$ENDOFINTERFACE" as a sample !*/
						char	*arrps8fmt[],			/*!	Format list, for each format,
																1st char is the separator,
																Followed by the output format
															Sample: "%\n    "
															!*/
						SIGN32	s32fmt					/*!	Count of formats !*/
						);

	/******************************************************************************************************************
	*	Function: _cfgAPI_PreProcess
	*	Description: Configuration file pre-process.
	*	Return:			FILE*						-	Opend temporal expanded file handle (need 'fclose')
	******************************************************************************************************************/
	FILE*	_cfgAPI_PreProcess(
						char	*ps8cfg,				/*!	Configuration file name !*/
						char	*ps8option,				/*!	Configuration parsing option string !*/
						SIGN32	bInsertLink				/*!	If insert '$link filename' for output !*/
						);

	/******************************************************************************************************************
	*	Function: _cfgAPI_LineParsing
	*	Description: Parse given string (optional from file) to separated words.
	*	Return:			SIGN32						-	>= 0 for number of words parsed, or
	*													ERR_MISMATCH if hit file end
	******************************************************************************************************************/
	SIGN32	_cfgAPI_LineParsing(
						char	*ps8str,				/*!	Non-zero to store input string line !*/
						FILE	*fpCFG,					/*!	Non-zero to load next line from file !*/

						char	*ps8QuoteList,			/*!	List of skipping quotes (ignoring stuff inside quote pairs)
															NULL to use default set:
															{ '\"', '\'' }
															!*/
						SIGN32	s32QuoteCount,			/*!	Count of skipping quotes
															0 to disable quote scan
															!*/
						char	*ps8SymbolList,			/*!	List of unwanted symbols
															NULL to use default set:
															{ '\n', '\r', ' ', '\t', '\"', '\'', ',', ';', '', '' }
															!*/
						SIGN32	s32SymbolCount,			/*!	Count of unwanted symbols
															0 to disable packing
															!*/
						char	**pps8CommentList,		/*!	List of comment flags (comments will be deleted first)
															NULL to use default set:
															{ "//", "/""*" }
															!*/
						SIGN32	s32CommentCount,		/*!	Count of comment flags
															0 to disable comment scan
															!*/
						char	**pps8word				/*!	Null pps8word[i] to return parsed word pointer, or
															Non-zero to receive word string (copy)
															!*/
						);

	/******************************************************************************************************************
	*	Function: _cfgAPI_GetItem
	*	Description: Seek item (match prefix or whole word) in configuration file based on name and parse to words.
	*	Return:			SIGN32						-	>= 0 for number of words parsed, or
	*													ERR_FILE if 'fpCFG' is NULL or
	*													ERR_MISMATCH if item not found in file
	******************************************************************************************************************/
	SIGN32	_cfgAPI_GetItem(
						char	*ps8str,				/*!	Non-zero to store next string line !*/
						FILE	*fpCFG,					/*!	Configuration file !*/
						SIGN32	bpfx,					/*!	If only do prefix-match !*/
						char	*ps8ItemName,			/*!	Name of the item to seek !*/
						char	*ps8ItemStop,			/*!	Name of the item to stop seeking,
															"" to seek from current to EOF, or
															NULL to seek entire file
															!*/
						char	**pps8word,				/*!	Null pps8word[i] to return parsed word pointer, or
															Non-zero to receive word string (copy)
															!*/
						char	*ps8SymbolList			/*!	List of unwanted symbols, Null to use default !*/
						);

	/******************************************************************************************************************
	*	Function: _cfgAPI_GetItemString
	*	Description: Seek item in configuration file based on name and return the description string.
	*	Returns:		SIGN32						-	SUCCESS or
	*													ERR_FILE if 'fpCFG' is NULL or
	*													ERR_MISMATCH if item not found in file
	******************************************************************************************************************/
	SIGN32	_cfgAPI_GetItemString(
						FILE	*fpCFG,					/*!	Configuration file !*/
						char	*ps8ItemName,			/*!	Name of the item to seek !*/
						char	*ps8ItemStop,			/*!	Name of the item to stop seeking,
															"" to seek from current to EOF, or
															NULL to seek entire file
															!*/
						char	*ps8ItemString			/*!	For returning the description of the item to seek !*/
						);

	/******************************************************************************************************************
	*	Function: _cfgAPI_GetItemValue
	*	Description: Seek item in configuration file based on name and return the integer value.
	*	Returns:		SIGN32						-	SUCCESS or
	*													ERR_FILE if 'fpCFG' is NULL or
	*													ERR_MISMATCH if item not found in file
	******************************************************************************************************************/
	SIGN32	_cfgAPI_GetItemValue(
						FILE	*fpCFG,					/*!	Configuration file !*/
						char	*ps8ItemName,			/*!	Name of the item to seek !*/
						char	*ps8ItemStop,			/*!	Name of the item to stop seeking,
															"" to seek from current to EOF, or
															NULL to seek entire file
															!*/
						SIGN32	*ps32ItemValue			/*!	For returning the integer value of the item to seek !*/
						);

	/******************************************************************************************************************
	*	Function: _cfgAPI_GetItemHexValue
	*	Description: Seek item (hex format) in configuration file based on name and return the integer value.
	*	Returns:		SIGN32						-	SUCCESS or
	*													ERR_FILE if 'fpCFG' is NULL or
	*													ERR_MISMATCH if item not found in file
	******************************************************************************************************************/
	SIGN32	_cfgAPI_GetItemHexValue(
						FILE	*fpCFG,					/*!	Configuration file !*/
						char	*ps8ItemName,			/*!	Name of the item to seek !*/
						char	*ps8ItemStop,			/*!	Name of the item to stop seeking,
															"" to seek from current to EOF, or
															NULL to seek entire file
															!*/
						UNSG32	*pu32ItemValue			/*!	For returning the integer value of the item to seek !*/
						);

	/******************************************************************************************************************
	*	Function: _cfgAPI_GetItemRealValue
	*	Description: Seek item in configuration file based on name and return the real value.
	*	Returns:		SIGN32						-	SUCCESS or
	*													ERR_FILE if 'fpCFG' is NULL or
	*													ERR_MISMATCH if item not found in file
	******************************************************************************************************************/
	SIGN32	_cfgAPI_GetItemRealValue(
						FILE	*fpCFG,					/*!	Configuration file !*/
						char	*ps8ItemName,			/*!	Name of the item to seek !*/
						char	*ps8ItemStop,			/*!	Name of the item to stop seeking,
															"" to seek from current to EOF, or
															NULL to seek entire file
															!*/
						REAL64	*pf64ItemValue			/*!	For returning the real value of the item to seek !*/
						);

	/******************************************************************************************************************
	*	Function: _cfgAPI_FileMergeIn
	*	Description: Merge a "section file" to another file at specified "section".
	*	Returns:		SIGN32						-	SUCCESS or
	*													ERR_FILE if file open failed
	******************************************************************************************************************/
	SIGN32	_cfgAPI_FileMergeIn(
						char	*ps8file,				/*!	Name of file to merge to !*/
						char	*ps8section,			/*!	Name of section file to merge !*/
						char	*ps8descMerge,			/*!	Specified "section identifier" !*/
						char	*ps8commentPfx			/*!	Non-zero to ouput comment prefix and update date/time !*/
						);
/**	ENDOFSECTION
 */

/**	__CODE_LINK__: all
 */
#endif

#ifdef	__cplusplus
	}
#endif



/**	CUTIL
 */
#endif

/**	ENDOFFILE: cutil.h ************************************************************************************************
 */

