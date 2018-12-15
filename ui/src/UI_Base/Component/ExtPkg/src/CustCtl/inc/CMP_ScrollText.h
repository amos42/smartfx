#ifndef __CMP_SCROLLTEXT_H_
#define	__CMP_SCROLLTEXT_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atSCROLLTEXT_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int attr, 
								//atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom, atHBITMAP lpScrollcursorImage );
								atHBITMAP lpScrollbarImgTop, atHBITMAP lpScrollbarImage, atHBITMAP lpScrollbarImgBottom,
								atHBITMAP lpScrollCursorImgTop, atHBITMAP lpScrollCursorImgMid, atHBITMAP lpScrollCursorImgBot );

int 	     atSCROLLTEXT_SetText( atHCONTROL hControl, atLPTSTR text );
atLPTSTR      atSCROLLTEXT_GetText( atHCONTROL hControl );

void         atSCROLLTEXT_ClearAllItem( atHCONTROL hControl );
int		 atSCROLLTEXT_Recalc( atHCONTROL hControl, int sidx, atBOOL IsAll );
int          atSCROLLTEXT_AddNewlineText( atHCONTROL hControl, atLPTSTR text );

void		atSCROLLTEXT_SetMargin( atHCONTROL hControl, atINT nMarginTop, atINT nMarginBottom, atINT nMarginLeft, atINT nMarginRight );
void		atSCROLLTEXT_SetLineGap( atHCONTROL hControl, atINT nLineGap );

void         atSCROLLTEXT_Scroll( atHCONTROL hControl, int nDirect );


#ifdef __cplusplus
}
#endif


#endif
