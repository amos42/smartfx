//
// MyControl.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_BUTTON_H_
#define __CMP_BUTTON_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL  atBUTTON_Create( int nID, int nX, int nY, int nWidth, int nHeight, atLPTSTR szText ); 

atVOID	    atBUTTON_SetText( atHCONTROL hControl, atLPTSTR szText );
atLPTSTR    atBUTTON_GetText( atHCONTROL hControl );


#ifdef __cplusplus
}
#endif


#endif // __BUTTON_H_
