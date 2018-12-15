//
// TickerBox.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_TICKERBOX_H_
#define __CMP_TICKERBOX_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif
    

#define atTICKERBOXATTR_OUTLINE	0x8000
#define atTICKERBOXATTR_OPAQUE	0x4000
#define atTICKERBOXATTR_ALIGN_MASK		0x0F00
#define atTICKERBOXATTR_ALIGN_TOP			0x0000
#define atTICKERBOXATTR_ALIGN_MIDDLE		0x0100
#define atTICKERBOXATTR_ALIGN_BOTTOM		0x0200


atHCONTROL atTICKERBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
	                                 atLPTSTR szText, atINT nLen, 
	                                 atLONG nTickerInterval, atINT nTickerInc, atINT nTickerInitTick,
	                                 atINT nLeftMargin, atINT nRightMargin,
	                                 atHBITMAP	hBgImageLeft, atHBITMAP	hBgImageCenter, atHBITMAP hBgImageRight );

atLPTSTR atTICKERBOX_GetText(atHCONTROL hControl);
void        atTICKERBOX_SetText(atHCONTROL hControl, atLPTSTR szNewText, atINT nLen);
atU32       atTICKERBOX_GetAttr(atHCONTROL hControl );
void        atTICKERBOX_SetAttr(atHCONTROL hControl, atU32 uAttr );
void        atTICKERBOX_SetTickerInterval(atHCONTROL hControl, atLONG interval );
void        atTICKERBOX_StartTicker(atHCONTROL hControl );
void        atTICKERBOX_StopTicker(atHCONTROL hControl );
void        atTICKERBOX_ResetTick(atHCONTROL hControl);
void 	atTICKERBOX_SetBGBitmap(atHCONTROL hControl, atHBITMAP hBgImageLeft, atHBITMAP hBgImageCenter, atHBITMAP hBgImageRight );

atBOOL atTICKERBOX_SetFontSize(atHCONTROL hControl, int nFontSize);
atINT  atTICKERBOX_GetFontSize(atHCONTROL hControl);
atBOOL atTICKERBOX_SetHLWordAtrribute(atHCONTROL hControl, atBYTE * bNewWordHL, atINT nLen);
atBOOL atTICKERBOX_SetColorHighLight(atHCONTROL, atCOLOR colorHL);

#ifdef __cplusplus
}
#endif


#endif // __TICKERBOX_H_
