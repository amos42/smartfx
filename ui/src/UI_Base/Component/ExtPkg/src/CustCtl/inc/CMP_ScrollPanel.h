//
// ScrollPannel.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_SCROLLPANEL_H_
#define __CMP_SCROLLPANEL_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef atVOID (atSCROLLPANEL_DRAWCLIENT_FUNC)( atHCONTROL hControl, atHGDC hCanvas, atINT nWidth, atINT nHeight, atBOOL bFocused, atINT nScrollPosX, atINT nScrollPosY, atLPVOID lpParam );
typedef atINT (atSCROLLPANEL_TOUCHCLIENT_FUNC)( atHCONTROL hControl, atINT nMsg, atINT nX, atINT nY, atINT nWidth, atINT nHeight, atLPVOID lpParam );

typedef struct _tagSCROLLPANEL_FUNCS {
	atSCROLLPANEL_DRAWCLIENT_FUNC	*fnDrawClient;
	atSCROLLPANEL_TOUCHCLIENT_FUNC	*fnTouchClient;
} atSCROLLPANEL_FUNCS, * atLPSCROLLPANEL_FUNCS;

atHCONTROL atSCROLLPANEL_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, int nClientWidth, int nClientHeight, atDWORD dwAttr,
 								 //atHBITMAP hScrollbarImgTop, atHBITMAP hScrollbarImage, atHBITMAP hScrollbarImgBottom, atHBITMAP hScrollcursorImage );
								 atHBITMAP hScrollbarImgTop, atHBITMAP hScrollbarImage, atHBITMAP hScrollbarImgBottom,
								 atHBITMAP hScrollCursorImgTop, atHBITMAP hScrollCursorImgMid, atHBITMAP hScrollCursorImgBot );


void	atSCROLLPANEL_GetFuncs( atHCONTROL hControl, atLPSCROLLPANEL_FUNCS lpFuncs, atLPVOID *lppFuncParam );
void atSCROLLPANEL_SetupFuncs( atHCONTROL hControl, atLPSCROLLPANEL_FUNCS lpFuncs, atLPVOID lpFuncParam );
atHCONTROL atSCROLLPANEL_GetPanelControl( atHCONTROL hControl );
// Kyle Start [Add 2009/06/22-14:25]
atHCONTROL atSCROLLPANEL_GetScrollBar( atHCONTROL hControl );
// Kyle [Add 2009/06/22-14:25] End
atBOOL	atSCROLLPANEL_AddControl( atHCONTROL hControl, atHCONTROL hChildControl );
atVOID	atSCROLLPANEL_SetControlFocus( atHCONTROL hControl, atHCONTROL hChildControl );
atHCONTROL atSCROLLPANEL_GetControlFocus( atHCONTROL hControl );
atBOOL	atSCROLLPANEL_SetScrollPos( atHCONTROL hControl, int pos );
atINT	atSCROLLPANEL_GetScrollPos( atHCONTROL hControl );
atBOOL	atSCROLLPANEL_SetClientSize(atHCONTROL hControl, int nClientWidth, int nClientHeight);
atBOOL	atSCROLLPANEL_GetClientSize(atHCONTROL hControl, int* nClientWidth, int* nClientHeight);


#ifdef __cplusplus
}
#endif


#endif // __SCROLLPANNEL_H_
