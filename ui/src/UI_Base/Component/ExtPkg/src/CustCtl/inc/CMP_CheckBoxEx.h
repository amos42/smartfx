#ifndef __CMP_CHECKBOXEX_H_
#define __CMP_CHECKBOXEX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL 	atCHECKBOX_EXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText, atBOOL bChecked, 
						 				atHBITMAP lpNormalImage, atHBITMAP lpCheckImage);
void 		atCHECKBOX_EXT_Destroy( atHCONTROL hControl );
void 		atCHECKBOX_EXT_Draw( atHGDC hCanvas, atHCONTROL hControl, atBOOL bFocused );

void		atCHECKBOX_EXT_SetCheck(atHCONTROL hControl, atBOOL bCheck);
atBOOL		atCHECKBOX_EXT_IsChecked(atHCONTROL hControl);


#ifdef __cplusplus
}
#endif


#endif // __CHECKBOX_H_
