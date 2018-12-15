/**
  @file UI_Disp.c
  @brief Display Processor에 관한 API
  @remark Display Processor에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Disp.h
*/
#include "AT_Types.h"
//#include "A_Queue.h"
#include "UI_MNG.h"
#include "UI_API/UI_API.h"


static atBOOL bInvalidate;
static atRECT rtDrawRect;
static atHEVENTPROC g_hDispProc;

static atREGION g_refresh_region = {0, 0, 0, 0};
static atHCRSECTION disp_lock = atNULL;

static atRECT g_clip;
static atBOOL g_clip_flag = atFALSE;


static void _lockarea(void)
{
	if (disp_lock)
		atUIAPI_EnterCriticalSection(disp_lock);
}

static void _unlockarea(void)
{
	if (disp_lock)
		atUIAPI_LeaveCriticalSection(disp_lock);
}

static atBOOL _need_redraw(void)
{
	return (g_refresh_region.nWidth > 0) && (g_refresh_region.nHeight > 0);	
}

/** 
 @brief Display Processor Tick (내부 호출용)
 @return 없음
*/
atBOOL atUIAPI_TickDispProcess(void)
{
	while( _need_redraw() ){
		atUIAPI_BeginDraw();	
		atUIAPI_DisplayInvalideArea();
		atUIAPI_EndDraw();
	}

	return 0;
}

static atBOOL _dispproc( atHPROCESSOR hProcessor, void *param )
{
//	if( _need_redraw() ){
//		atKERNEL_LockScreen();

		atUIAPI_TickDispProcess();

//		atKERNEL_UnlockScreen();
//	}

	return 0;
}


/** 
 @brief Display Processor를 초기화한다. (내부 호출용)
 @return 없음
*/
atBOOL atUIAPI_InitDisplayProcessor(void)
{
    g_hDispProc = atUIAPI_CreateProcessor( _AT("DispProc"), 0, _dispproc, atNULL );

    disp_lock	= atUIAPI_CreateCriticalSection();

	SET_REGION( g_refresh_region, 0, 0, 0, 0 );
	
    bInvalidate = atFALSE;
	g_clip_flag = atFALSE;
	
    return atTRUE;
}


/** 
 @brief Display Processor를 종료한다. (내부 호출용)
 @return 없음
*/
atVOID atUIAPI_FinalDisplayProcessor(void)
{
    bInvalidate = atFALSE;

    if( g_hDispProc){
        atUIAPI_DestroyProcessor( g_hDispProc );
        g_hDispProc = atNULL;
    }

    if( disp_lock ){
		atUIAPI_DestroyCriticalSection(disp_lock);
		disp_lock = atNULL;
    }
}    


/** 
 @brief 클리핑 영역을 고정시킨다.
 @param lpClipRect [in] : 새로 지정할 클리핑 영역
 @param bForce [in] : 이전에 클리핑 영역이 Lock된 상태라도 강제로 진행한다.
 @return 없음
*/
atVOID atUIAPI_LockDrawClipArea( atRECT *lpClipRect, atBOOL bForce )
{
	if( !bForce && g_clip_flag ) return;

	g_clip = *atUIAPI_GetScreenClipRect();
	atUIAPI_SetScreenClipRect2( lpClipRect );
	g_clip_flag = atTRUE;
}


/** 
 @brief 클리핑 영역을 고정을 해제하고, 이전 클리핑 영역으로 되돌린다.
 @return 없음
*/
atVOID atUIAPI_UnlockDrawClipArea( NOARGS )
{
	atUIAPI_SetScreenClipRect2( &g_clip );
	g_clip_flag = atFALSE;
}


/** 
 @brief 클리핑 영역 지정한 것을 해제하고, 전체 화면을 클리핑 영역으로 설정한다.
 @return 없음
*/
atVOID atUIAPI_ResetDrawClipArea( NOARGS )
{
	atUIAPI_SetScreenClipRect( 0, 0, atUIAPI_GetScreenWidth(), atUIAPI_GetScreenHeight() );
	g_clip_flag = atFALSE;
}


/** 
 @brief 화면 전체를 갱신 영역으로 지정한다.
 @return 없음
*/
atVOID atUIAPI_AddInvalideAreaAll( NOARGS )
{
    _lockarea();
	SET_REGION( g_refresh_region, 0, 0, atUIAPI_GetScreenWidth(), atUIAPI_GetScreenHeight() );	
   	bInvalidate = atTRUE;
	_unlockarea();

	if( !atUIAPI_IsActiveProcessor(g_hDispProc) )
		atUIAPI_WakeProcessor( g_hDispProc );
}


/** 
 @brief 화면 갱신 영역을 추가한다.
 @param lpRegion [in] : 추가할 갱신 영역
 @return 없음
*/
atVOID atUIAPI_AddInvalideArea( atREGION *lpRegion )
{
	atRECT a, b;

    _lockarea();

	if( (g_refresh_region.nWidth <= 0) || (g_refresh_region.nHeight <= 0) ){
		g_refresh_region = *lpRegion;
	} else {
		REGION2RECT( a, g_refresh_region );
		REGION2RECT( b, *lpRegion );	
		atUIAPI_UnionRect( &a, &a, &b );	
		RECT2REGION( g_refresh_region, a );
	}

	if( g_refresh_region.nX < 0 ){ 
		g_refresh_region.nWidth += g_refresh_region.nX; 
		g_refresh_region.nX = 0; 
	}
	if( g_refresh_region.nY < 0 ){ 
		g_refresh_region.nHeight += g_refresh_region.nY;
		g_refresh_region.nY = 0;
	}
	if( (g_refresh_region.nX + g_refresh_region.nWidth) > atUIAPI_GetScreenWidth() ) 
		g_refresh_region.nWidth = atUIAPI_GetScreenWidth() - g_refresh_region.nX;
	if( (g_refresh_region.nY + g_refresh_region.nHeight) > atUIAPI_GetScreenHeight() ) 
		g_refresh_region.nHeight = atUIAPI_GetScreenHeight() - g_refresh_region.nY;
	
    bInvalidate = atTRUE;	

	_unlockarea();

	if( !atUIAPI_IsActiveProcessor(g_hDispProc) )
		atUIAPI_WakeProcessor( g_hDispProc );
}


/** 
 @brief 화면 갱신 영역을 추가한다.
 @param lpRect [in] : 추가할 갱신 영역
 @return 없음
*/
atVOID atUIAPI_AddInvalideRect( atRECT *lpRect )
{
	atREGION rgn;
	RECT2REGION( rgn, *lpRect );
	
	atUIAPI_AddInvalideArea( &rgn );
}


/** 
 @brief 설정된 화면 갱신 영역을 모두 초기화 한다.
 @return 없음
*/
atVOID       atUIAPI_ClearInvalideArea(void)
{
    _lockarea();
	
    SET_REGION( g_refresh_region, 0, 0, 0, 0 );	
    bInvalidate = atFALSE;
	
    _unlockarea();
}


/** 
 @brief 현재 지정된 갱신 영역을 실제로 화면에 표시한다.
 @return 없음
*/
atVOID atUIAPI_DisplayInvalideArea(NOARGS)
{
	atREGION rgn, *soft_rgn;
	atRECT rct, soft_rct;
	atBOOL bIsRefresh, bIsSoftRefresh;

	if (!_need_redraw()) return;

	_lockarea();
	rgn = g_refresh_region;
	atUIAPI_ClearInvalideArea();
	_unlockarea();

	REGION2RECT(rct, rgn);
	soft_rgn = atSOFTKEY_GetSoftKeyArea();
	if (soft_rgn != atNULL) {
	    REGION2RECT(soft_rct, *soft_rgn);
	    bIsSoftRefresh = atUIAPI_IsIntersectRect(&rct, &soft_rct);
    } else {
	    bIsSoftRefresh = atFALSE;
	}

    bIsRefresh = atTRUE;
	if( bIsSoftRefresh ){
		if( rct.nStartY >= soft_rct.nStartY ) bIsRefresh = atFALSE;
		else rct.nEndY = soft_rct.nStartY - 1;			
	}

	if( !bIsRefresh && !bIsSoftRefresh ) return;
	
    atKERNEL_LockScreen();

	if( bIsRefresh ){
		atUIAPI_LockDrawClipArea( &rct, atTRUE );
	       atAPP_RedrawAllWindowArea( atAPPMNG_GetCurApp(), &rct );
		atUIAPI_ResetDrawClipArea();
	}

	if( bIsSoftRefresh ) atSOFTKEY_DrawSoftKey();

	atUIAPI_FlushScreenRgn( rgn.nX, rgn.nY, rgn.nWidth, rgn.nHeight );
	
    atKERNEL_UnlockScreen();
}


/** 
 @brief 화면 갱신을 시작한다.
 @return 없음
 @remark 현재 구현되어 있지 않아, 사용해도 효과가 없다.
*/
atVOID atUIAPI_StartDisplay( NOARGS )
{
//	atUIAPI_ResumeProcessor(g_hDispProc);
}


/** 
 @brief 화면 갱신을 끝낸다.
 @return 없음
 @remark 현재 구현되어 있지 않아, 사용해도 효과가 없다.
*/
atVOID atUIAPI_StopDisplay( NOARGS )
{
//	atUIAPI_SuspendProcessor(g_hDispProc);
}


/** 
 @brief 화면 전체를 그린다. (내부용)
 @return 없음
*/
atVOID atUIAPI_DisplayAll( NOARGS )
{
//    if( !bInvalidate ) return;
	
	atUIAPI_ClearInvalideArea();
	
    atKERNEL_LockScreen();
    atAPP_RedrawAllWindow( atAPPMNG_GetCurApp() );
    atSOFTKEY_DrawSoftKey();
	atUIAPI_FlushScreen();
    atKERNEL_UnlockScreen();
}


/** 
 @brief 강제로 화면 전체를 그린다. (내부용)
 @return 없음
*/
atVOID atUIAPI_DisplayForceAll( NOARGS )
{
    bInvalidate = atTRUE;
    atUIAPI_DisplayAll();
}


