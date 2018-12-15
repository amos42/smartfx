#ifndef __CMP_CHECKBOX_H_
#define __CMP_CHECKBOX_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL 	atCHECKBOX_Create( atINT nIndex, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText, atBOOL bChecked );

void		atCHECKBOX_SetCheck(atHCONTROL hControl, atBOOL bCheck);
atBOOL		atCHECKBOX_IsChecked(atHCONTROL hControl);


#ifdef __cplusplus
}
#endif

#endif // __CHECKBOX_H_
