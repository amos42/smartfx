//
// MyControl.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_VPROGRESSBAR_H_
#define __CMP_VPROGRESSBAR_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atVPROGRESSBAR_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
								 int style, int nCursorX, int nCursorY, int nCursorLimit );

int atVPROGRESSBAR_GetSize( atHCONTROL hControl );
void atVPROGRESSBAR_SetSize( atHCONTROL hControl, int size );
int atVPROGRESSBAR_GetPosition( atHCONTROL hControl );
int atVPROGRESSBAR_SetPosition( atHCONTROL hControl, int pos );


#ifdef __cplusplus
}
#endif


#endif // _MYCONTROL_H_INCLUDED
