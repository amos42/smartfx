//
// MyControl.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_PROGRESSBAR_H_
#define __CMP_PROGRESSBAR_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atPROGRESSBAR_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
								 int style, 	int nCursorX, int nCursorY, int nCursorLimit );

int atPROGRESSBAR_GetSize( atHCONTROL hControl );
void atPROGRESSBAR_SetSize( atHCONTROL hControl, int size );
int atPROGRESSBAR_GetPosition( atHCONTROL hControl );
int atPROGRESSBAR_SetPosition( atHCONTROL hControl, int pos );


#ifdef __cplusplus
}
#endif


#endif // _MYCONTROL_H_INCLUDED
