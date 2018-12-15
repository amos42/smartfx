/**
  @file UI_Entry.c
  @brief UI������ Entroy Pointer
  @remark UI������ Entroy Pointer
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see UI_Entry.h
*/
#include "UI_Types.h"
#include "UI_MNG.h"


#define __DO_NOT_MULTIKEY_  (0)


#if __DO_NOT_MULTIKEY_
static int g_nCurKey;
#endif


/** 
 @brief UI ���� �ʱ�ȭ

 UI ������ �ʱ�ȭ �Ѵ�.\n
 ���� ��뿡 �ռ� �ݵ�� �� �ѹ� ȣ���� ��� �Ѵ�.

 @param lpInitData [in] : OEM ���̾ ������ �Ķ����
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
*/
atBOOL   atUIENG_Initialize( atUIAPI_INIT_DATA *lpInitData )
{
#if __DO_NOT_MULTIKEY_
    g_nCurKey = 0x00;
#endif

    return atKERNEL_Initialize( lpInitData );    
}


/** 
 @brief UI ���� ����
 
 UI ������ �����Ų��.

 @return ����
*/
void atUIENG_Finalize( void )
{
#if __DO_NOT_MULTIKEY_
    g_nCurKey = 0x00;
#endif

    atKERNEL_Finalize();
}


/** 
 @brief KeyDown�� ���� ��Ʈ�� ����Ʈ
 
 KeyDown�� ���� ��Ʈ�� ����Ʈ

 @param nKeyCode [in] : Ű�ڵ�
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
*/
atBOOL atUIENG_OnKeyDown(int nKeyCode)
{
#if __DO_NOT_MULTIKEY_
	if( g_nCurKey != 0 ) return atFALSE;
	g_nCurKey = nKeyCode;
#endif
	
	atKERNEL_KeyDown(nKeyCode);
	return atTRUE;
}


/** 
 @brief Long KeyDown�� ���� ��Ʈ�� ����Ʈ
 
 Long KeyDown�� ���� ��Ʈ�� ����Ʈ

 @param nKeyCode [in] : Ű�ڵ�
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
*/
atBOOL atUIENG_OnLongKeyDown(int nKeyCode)
{
	atKERNEL_LongKeyDown(nKeyCode);
	return atTRUE;
}


/** 
 @brief KeyUp�� ���� ��Ʈ�� ����Ʈ
 
 KeyUp�� ���� ��Ʈ�� ����Ʈ

 @param nKeyCode [in] : Ű�ڵ�
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
 @remark KeyDown�� KeyUp�� �ݵ�� ������ ȣ��Ǿ�� �Ѵ�.
*/
atBOOL atUIENG_OnKeyUp(int nKeyCode)
{
#if __DO_NOT_MULTIKEY_
	if( g_nCurKey != nKeyCode ) return atFALSE;
	g_nCurKey = 0x00;
#endif
	
	atKERNEL_KeyUp( nKeyCode );
	return atTRUE;
}


/** 
 @brief ����Ű �Է��� �޴� ��Ʈ�� ����Ʈ
 
 ����Ű �Է��� �޴� ��Ʈ�� ����Ʈ

 @param nCharCode [in] : Char �ڵ�
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
*/
atBOOL atUIENG_OnKeyChar(int nCharCode)
{
	// �Ϲ� �����ڷ� �Է��� �����Ѵ�.
	if( (nCharCode != ' ') && (nCharCode != '-') && (nCharCode != '.') &&
	   !(nCharCode >= '0' && nCharCode <= '9') &&
	   !(nCharCode >= 'a' && nCharCode <= 'z') &&
	   !(nCharCode >= 'A' && nCharCode <= 'Z') ) return atFALSE;
	
	atKERNEL_KeyChar( nCharCode );

	return atTRUE;
}


/** 
 @brief �ý��� Ű�� ���� ��Ʈ�� ����Ʈ
 
 �ý��� Ű�� ���� ��Ʈ�� ����Ʈ

 @param nKeyCode [in] : Ű�ڵ�
 @return �����ϸ� atTRUE, �����ϸ� atFALSE
 @remark �ý��ۿ� ���� �ٸ��� ó���Ǳ⿡, ȣȯ���� ������ �� ����.
*/
atBOOL atUIENG_OnSysKeyDown(int nVKeyCode)
{
	if( nVKeyCode == 0 ) return atFALSE;
	atKERNEL_SysKeyDown(nVKeyCode);
	return atTRUE;
}


/** 
 @brief ȭ���� ������ ���� ��Ʈ�� ����Ʈ
 
 ȭ���� ������ ���� ��Ʈ�� ����Ʈ

 @return ����
*/
void atUIENG_OnPaint(void)
{    
    atKERNEL_Paint();
}


/** 
 @brief ���÷��� �ý����� ũ�� ���� �̺�Ʈ�� ���� ��Ʈ�� ����Ʈ
 
 ���÷��� �ý����� ũ�� ���� �̺�Ʈ�� ���� ��Ʈ�� ����Ʈ

 @param width, height [in] : ���� �ٲ� ���÷��� ��ġ�� ũ��
 @return ����
*/
void atUIENG_OnResize( int width, int height )
{
	atKERNEL_Resize( width, height );
}


/** 
 @brief MouseDown�� ���� ��Ʈ�� ����Ʈ
 
 MouseDown�� ���� ��Ʈ�� ����Ʈ

 @param btn [in] : ���� ��ư
 @param x, y [in] : ���콺�� ���� ��ǥ
 @return ����
*/
void atUIENG_OnMouseDown( int btn, int x, int y )
{    
	atUIAPI_ConvOEMPtr2VPtr( &x, &y );
	atKERNEL_MouseDown( btn, x, y );
}


/** 
 @brief MouseMove�� ���� ��Ʈ�� ����Ʈ
 
 MouseMove�� ���� ��Ʈ�� ����Ʈ

 @param btn [in] : ���� ���� �ִ� ���콺 ��ư
 @param x, y [in] : ���콺�� �̵��� ��ǥ
 @return ����
*/
void atUIENG_OnMouseMove( int btn, int x, int y )
{    
	atUIAPI_ConvOEMPtr2VPtr( &x, &y );
	atKERNEL_MouseMove( btn, x, y );
}


/** 
 @brief MouseUp�� ���� ��Ʈ�� ����Ʈ
 
 MouseUp�� ���� ��Ʈ�� ����Ʈ

 @param btn [in] : ���콺�� �� ��ư
 @param x, y [in] : ���콺 ��ư�� �� ��ǥ
 @return ����
 @remark MouseDown�� MouseUp�� �ݵ�� ������ �߻��Ǿ�� �Ѵ�.
*/
void atUIENG_OnMouseUp( int btn, int x, int y )
{    
	atUIAPI_ConvOEMPtr2VPtr( &x, &y );
	atKERNEL_MouseUp( btn, x, y );
}


/** 
 @brief Mouse ��ư�� ��� ���� �̺�Ʈ�� ��Ʈ�� ����Ʈ
 
 Mouse ��ư�� ��� ���� �̺�Ʈ�� ��Ʈ�� ����Ʈ

 @param btn [in] : ���콺�� ��� ���� ��ư
 @param x, y [in] : ���콺 ��� ������ ���� ��ǥ
 @return ����
*/
void atUIENG_OnMouseLongClick( int btn, int x, int y )
{    
	atUIAPI_ConvOEMPtr2VPtr( &x, &y );
	atKERNEL_MouseLongClick( btn, x, y );
}


/** 
 @brief �ý����� Pause��Ų��.
 
 �ý����� Pause��Ų��.

 @return ����
*/
void atUIENG_OnPause(void)
{
	atKERNEL_Pause();
}


/** 
 @brief Pause�� �ý����� Resume��Ų��.
 
 Pause�� �ý����� Resume��Ų��.

 @return ����
*/
void atUIENG_OnResume(void)
{
	atKERNEL_Resume();
}


/** 
 @brief �ý��ۿ� Notify�� ������.
 
 �ý��ۿ� Notify�� ������.

 @param nParam1, nParam2 [in] : �̺�Ʈ�� �Ķ����
 @return ����
*/
void atUIENG_OnSystemNotify( long nParam1, long nParam2 )
{
    atKERNEL_SystemNotify( nParam1, nParam2 );
}


/** 
 @brief Timer tick.
 
 �ý��ۿ� Timer tick�� ������.

 @return ����
*/
void atUIENG_OnTimerTick() 
{
    //atUIAPI_TimerTick();
}
