/** 
  @file AnnBar.h
  @brief Annunciate Bar�� ���� API
  @remark AnnBar.c�� �������̽� ���
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see AnnBar.c
*/

#ifndef __ANNBAR_H_
#define __ANNBAR_H_


#include "UI_Types.h"
#include "UI_API/UI_API.h"


#ifdef __cplusplus
extern "C" {
#endif
    
typedef atVOID (atANNBAR_DRAWFUNC)( atHGDC );		///< atANNBAR_SetDrawFunc �Լ��� �Ķ���� Ÿ��
typedef atANNBAR_DRAWFUNC * atLPANNBAR_DRAWFUNC;	///< atANNBAR_DRAWFUNC�� ������ Ÿ��


atVOID		atANNBAR_InitAnnBar( atDWORD dwAttr );
atVOID		atANNBAR_FinalAnnBar( NOARGS );

atVOID		atANNBAR_ActivateAnnBar( NOARGS );
atVOID		atANNBAR_DeactivateAnnBar( NOARGS );
atBOOL		atANNBAR_IsActiveAnnBar( NOARGS );
atREGION *	atANNBAR_GetAnnBarArea( NOARGS );
atVOID		atANNBAR_SetAnnBarArea( atINT nX, atINT nY, atINT nWidth, atINT nHeight );
atVOID		atANNBAR_SetAnnBarAreaHeight( atINT nHeight );
atVOID		atANNBAR_RecalcAnnBarArea( NOARGS );
atVOID 		atANNBAR_SetAnnBarColor( atCOLOR clBgColor );
atVOID     	atANNBAR_SetDrawFunc( atLPANNBAR_DRAWFUNC fnDrawFunc );
atVOID 		atANNBAR_SetAnnBarBitmap( atHBITMAP hBgImageLeft, atHBITMAP hBgImageCenter, atHBITMAP hBgImageRight );
atVOID		atANNBAR_FlushAnnBarArea( NOARGS );
atVOID		atANNBAR_DrawAnnBar( NOARGS );

atVOID		atANNBAR_RefreshAnnBar( NOARGS );


#ifdef __cplusplus
}
#endif


#endif
