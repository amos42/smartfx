//
// MyControl.h
//
///////////////////////////////////////////////////////////////////////

#ifndef __CMP_RADIOGROUP_H_
#define __CMP_RADIOGROUP_H_


#ifndef __A_UIENG_H_
#include "A_UIENG.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


atHCONTROL atRADIOGROUP_Create( int nIndex, int nX, int nY, int nWidth, int nHeight, 
									atTCHAR (*szButtonMsg)[32], int ButtonCnt, int init_idx, int item_height );

int atRADIOGROUP_GetItemCount( atHCONTROL hControl );
int atRADIOGROUP_AddItem( atHCONTROL hControl, atLPTSTR itm_str );
int atRADIOGROUP_GetSelectIndex( atHCONTROL hControl );
void atRADIOGROUP_SetSelectIndex( atHCONTROL hControl, int idx );


#ifdef __cplusplus
}
#endif


#endif // __RADIOBUTTON_H_
