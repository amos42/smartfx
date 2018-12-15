//
// EditBox.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_EDITBOX_H_
#define __CMP_EDITBOX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define atEDITBOXATTR_USE_OIC			0x80000000
#define atEDITBOXATTR_OUTLINE			0x00008000
#define atEDITBOXATTR_OPAQUE			0x00004000
#define atEDITBOXATTR_ALIGN_MASK		0x000000FF


typedef enum {
	atEDITBOX_INPUTMODE_NONE = 0,
		
	atEDITBOX_INPUTMODE_DUMMY,
    
	atEDITBOX_INPUTMODE_NUMERIC_ONLY,
	atEDITBOX_INPUTMODE_ENG_LOWERCASE_ONLY,
	atEDITBOX_INPUTMODE_ENG_UPPERCASE_ONLY,

	atEDITBOX_INPUTMODE_ENG_LOWERCASE,
	atEDITBOX_INPUTMODE_ENG_UPPERCASE,
	
	atEDITBOX_INPUTMODE_LOWERCASE,
	atEDITBOX_INPUTMODE_UPPERCASE,
	
	atEDITBOX_INPUTMODE_NUMERIC_AND_LOWERCASE,  
	
	atEDITBOX_INPUTMODE_MAX
} atEDITBOX_INPUTMODE;

typedef atINT (atEDITBOX_INPUT_FUNC)( atHCONTROL, atINT, atINT, atINT );


atHCONTROL atEDITBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText,
								atEDITBOX_INPUTMODE lstInputMode[], atINT nInputModeCount, atINT nCurInputMode );

atUI_HIME atEDITBOX_GetIME( atHCONTROL hControl );
atLPTSTR	atEDITBOX_GetText( atHCONTROL hControl, atLPTSTR szDesText );
atLPTSTR	atEDITBOX_GetFullText( atHCONTROL hControl, atLPTSTR szDesText );
void	    atEDITBOX_SetText( atHCONTROL hControl, atLPTSTR szNewText );
int		    atEDITBOX_GetFullTextLength( atHCONTROL hControl );
int		    atEDITBOX_GetTextLength( atHCONTROL hControl );
atU32	    atEDITBOX_GetAttr( atHCONTROL hControl );
void	    atEDITBOX_SetAttr( atHCONTROL hControl, atU32 uAttr );
void		atEDITBOX_SetAlign( atHCONTROL hControl, int align );
atBOOL		atEDITBOX_IsReadOnly( atHCONTROL hControl );
void	    atEDITBOX_ReadOnly( atHCONTROL hControl, atBOOL bReadOnly );
/*
void	    atEDITBOX_SetInputModeArray( atHCONTROL hControl, ... );
void	    atEDITBOX_SetInputModeFunc( atHCONTROL hControl, atINT nInputMode, atEDITBOX_INPUT_FUNC *func );
atINT	    atEDITBOX_GetInputMode( atHCONTROL hControl );
void	    atEDITBOX_SetInputMode( atHCONTROL hControl, atINT nInputMode );
void	    atEDITBOX_ChangeNextInputMode( atHCONTROL hControl );
*/


#ifdef __cplusplus
}
#endif


#endif // __EDITBOX_H_
