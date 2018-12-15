//
// EditBox.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_EDITBOXEX_H_
#define __CMP_EDITBOXEX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define atEDITBOXATTR_EXT_USE_OIC			0x80000000
#define atEDITBOXATTR_EXT_OUTLINE			0x00008000
#define atEDITBOXATTR_EXT_OPAQUE			0x00004000
#define atEDITBOXATTR_EXT_ALIGN_MASK		0x000000FF

// Kyle Start [Add 2009/06/03-17:41]
#define atEDITBOXVALUE_PAGEING				5
// Kyle [Add 2009/06/03-17:41] End

typedef enum {
    atEDITBOX_EXT_INPUTMODE_NONE = 0,
    
	atEDITBOX_EXT_INPUTMODE_DUMMY,
	
	atEDITBOX_EXT_INPUTMODE_NUMERIC_ONLY,
	atEDITBOX_EXT_INPUTMODE_ENG_LOWERCASE_ONLY,
	atEDITBOX_EXT_INPUTMODE_ENG_UPPERCASE_ONLY,

	atEDITBOX_EXT_INPUTMODE_ENG_LOWERCASE,
	atEDITBOX_EXT_INPUTMODE_ENG_UPPERCASE,
	
	atEDITBOX_EXT_INPUTMODE_LOWERCASE,
	atEDITBOX_EXT_INPUTMODE_UPPERCASE,
	
	atEDITBOX_EXT_INPUTMODE_NUMERIC_AND_LOWERCASE,
	
	atEDITBOX_EXT_INPUTMODE_MAX
} atEDITBOX_EXT_INPUTMODE;

typedef atINT (atEDITBOX_EXT_INPUT_FUNC)( atHCONTROL, atINT, atINT, atINT );


atHCONTROL atEDITBOX_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText,
						atBOOL bUseOIC,
						atEDITBOX_EXT_INPUTMODE lstInputMode[], atINT nInputModeCount, atINT nCurInputMode,
						 atHBITMAP lpNormalImg_Left, atHBITMAP lpNormalImg_Center, atHBITMAP lpNormalImg_Right,
						 atHBITMAP lpFocusedImg_Left, atHBITMAP lpFocusedImg_Center, atHBITMAP lpFocusedImg_Right,
						 atHBITMAP lpDisabledImg_Left, atHBITMAP lpDisabledImg_Center, atHBITMAP lpDisabledImg_Right );

atUI_HIME atEDITBOX_EXT_GetIME( atHCONTROL hControl );
atLPTSTR	atEDITBOX_EXT_GetText( atHCONTROL hControl, atLPTSTR szDesText );
atLPTSTR	atEDITBOX_EXT_GetFullText( atHCONTROL hControl, atLPTSTR szDesText );
void	    atEDITBOX_EXT_SetText( atHCONTROL hControl, atLPTSTR szNewText );
int		    atEDITBOX_EXT_GetFullTextLength( atHCONTROL hControl );
int		    atEDITBOX_EXT_GetTextLength( atHCONTROL hControl );
atU32	    atEDITBOX_EXT_GetAttr( atHCONTROL hControl );
void	    atEDITBOX_EXT_SetAttr( atHCONTROL hControl, atU32 uAttr );
void		atEDITBOX_EXT_SetAlign( atHCONTROL hControl, int align );
atBOOL		atEDITBOX_EXT_IsReadOnly( atHCONTROL hControl );
void	    atEDITBOX_EXT_ReadOnly( atHCONTROL hControl, atBOOL bReadOnly );
void		atEDITBOX_EXT_SetAwaysBlink( atHCONTROL hControl, atBOOL bAwaysBlink );
// Kyle Start [Add 2009/06/17-20:50]
int		    atEDITBOX_EXT_SetBufferMaxLength( atHCONTROL hControl, int nLen );
int		    atEDITBOX_EXT_GetBufferMaxLength( atHCONTROL hControl );
// Kyle [Add 2009/06/17-20:50] End
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
