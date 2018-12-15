//
// MyControl.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_SCROLLBAR_H_
#define __CMP_SCROLLBAR_H_

#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


/*
typedef struct _atSCROLLBAR {
	atHBITMAP	lpScrollbarImage;
	atHBITMAP	lpScrollcursorImage;

	int style;
	
	int	nSize;
	int nPosition;

	int nCursorX, nCursorY;
	int nCursorLimit;
} atSCROLLBAR,  *atLPSCROLLBAR;
*/


atHCONTROL atSCROLLBAR_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
								 int style, 	int nCursorX, int nCursorY, int nCursorLimit );

int atSCROLLBAR_GetSize( atHCONTROL hControl );
void atSCROLLBAR_SetSize( atHCONTROL hControl, int size );
int atSCROLLBAR_GetPosition( atHCONTROL hControl );
int atSCROLLBAR_SetPosition( atHCONTROL hControl, int pos );
int atSCROLLBAR_Scroll( atHCONTROL hControl, int value );


#ifdef __cplusplus
}
#endif


#endif // __SCROLLBAR_H_
