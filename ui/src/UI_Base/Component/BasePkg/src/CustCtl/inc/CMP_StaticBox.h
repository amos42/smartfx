//
// StaticBox.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_STATICBOX_H_
#define __CMP_STATICBOX_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define atSTATICBOXATTR_OUTLINE			0x8000
#define atSTATICBOXATTR_OPAQUE			0x4000
#define atSTATICBOXATTR_ALIGN_MASK		0x00FF


atHCONTROL atSTATICBOX_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText ); 
atHCONTROL atSTATICBOX_CreateIDS( int nIndex, int nX, int nY, int nWidth, int nHeight, atLONG nTextIDS); 

atU32		atSTATICBOX_GetAttr(atHCONTROL hControl );
void		atSTATICBOX_SetAttr(atHCONTROL hControl, atU32 uAttr );
atLPTSTR	atSTATICBOX_GetText(atHCONTROL hControl);
void		atSTATICBOX_SetText(atHCONTROL hControl, atLPTSTR szNewText);
void		atSTATICBOX_SetTextIDS(atHCONTROL hControl, atDWORD nTextIds);
int			atSTATICBOX_GetTextLength( atHCONTROL hControl );
void		atSTATICBOX_SetAlign(atHCONTROL hControl, int align );


#ifdef __cplusplus
}
#endif

#endif // __STATICBOX_H_
