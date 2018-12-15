//
// Key.c
//
////////////////////////////////////////////////////////////////

#include "SYSAPI.h"
#include "OEM_SA_Key.h"
#include <gtk/gtk.h>


static SYS_FUNC_INPUTNOTIFY *g_fnInputFunc = atNULL;

extern int		g_SYS_nScale;


static gboolean motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event)
{
	int x, y, state;

	if(event->is_hint)
	{
		gtk_widget_get_pointer(widget, &x, &y);
	}
	else
	{
		x = event->x;
		y = event->y;
	}

	state = event->state;
	if(state & GDK_BUTTON1_MASK)
	{
//		g_printf("########## motion_notify_event_cb::MouseMove (%d, %d)\n", (int)event->x, (int)event->y);
		//atUIENG_OnMouseMove( 1, (int)event->x_root, (int)event->y_root );
//		atUIENG_OnMouseMove( 1, (int)event->x, (int)event->y );
		if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHMOVE, (int)event->x, (int)event->y );
	}

	return 1;
}

static gboolean button_press_event_cb(GtkWidget *widget, GdkEventButton *event)
{
	if(event->button == 1 && event->type == GDK_BUTTON_PRESS)
	{
//		g_printf("########## button_press_event_cb::MouseDown (%d, %d)\n", (int)event->x, (int)event->y);
		//atUIENG_OnMouseDown( 1, (int)event->x_root, (int)event->y_root );
//		atUIENG_OnMouseDown( 1, (int)event->x, (int)event->y );
		if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHDOWN, (int)event->x, (int)event->y );
	}

	return 1;
}

static gboolean button_release_event_cb(GtkWidget *widget, GdkEventButton *event)
{
	if(event->button == 1)
	{
//		g_printf("########## button_release_event_cb::MouseUp (%d, %d)\n", (int)event->x, (int)event->y);
		//atUIENG_OnMouseUp( 1, (int)event->x_root, (int)event->y_root );
//		atUIENG_OnMouseUp( 1, (int)event->x, (int)event->y );
		if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_TOUCHUP, (int)event->x, (int)event->y );
	}

	return 1;
}

static int _OEM_SYSAPI_ConvOEMKey2VChar( int OEMKey )
{
	char ch = SYS_VKEY_NULL;

	if(OEMKey > 9 && OEMKey < 19)	//'1' ~ '9'
	{
		ch = OEMKey + 39;
	}
	else
	{
		switch( OEMKey ) {
			case 19: //'0'
				ch = '0'; break;
			case 49: //'`'
				ch = '`'; break;
			case 20: //'-'
				ch = '-'; break;
			case 21: //'='
				ch = '='; break;
			///////////////////////////////////////////////////////////////////////////////////////
			case 24: //'q'
				ch = 'q'; break;
			case 25: //'w'
				ch = 'w'; break;
			case 26: //'e'
				ch = 'e'; break;
			case 27: //'r'
				ch = 'r'; break;
			case 28: //'t'
				ch = 't'; break;
			case 29: //'y'
				ch = 'y'; break;
			case 30: //'u'
				ch = 'u'; break;
			case 31: //'i'
				ch = 'i'; break;
			case 32: //'o'
				ch = 'o'; break;
			case 33: //'p'
				ch = 'p'; break;
			case 34: //'['
				ch = '['; break;
			case 35: //']'
				ch = ']'; break;
			case 51: //'\'
				ch = '\\'; break;
			///////////////////////////////////////////////////////////////////////////////////////
			case 38: //'a'
				ch = 'a'; break;
			case 39: //'s'
				ch = 's'; break;
			case 40: //'d'
				ch = 'd'; break;
			case 41: //'f'
				ch = 'f'; break;
			case 42: //'g'
				ch = 'g'; break;
			case 43: //'h'
				ch = 'h'; break;
			case 44: //'j'
				ch = 'j'; break;
			case 45: //'k'
				ch = 'k'; break;
			case 46: //'l'
				ch = 'l'; break;
			case 47: //';'
				ch = ';'; break;
			case 48: //'''
				ch = '\''; break;
			///////////////////////////////////////////////////////////////////////////////////////
			case 52: //'z'
				ch = 'z'; break;
			case 53: //'x'
				ch = 'x'; break;
			case 54: //'c'
				ch = 'c'; break;
			case 55: //'v'
				ch = 'v'; break;
			case 56: //'b'
				ch = 'b'; break;
			case 57: //'n'
				ch = 'n'; break;
			case 58: //'m'
				ch = 'm'; break;
			case 59: //','
				ch = ','; break;
			case 60: //'.'
				ch = '.'; break;
			case 61: //'/'
				ch = '/'; break;
		}
	}

	return ch;
}

static gboolean key_press_event_cb(GtkWidget *widget, GdkEventKey *event)
{
	int vkey = OEM_SYSAPI_ConvOEMKey2VKey( event->hardware_keycode );
//	g_printf("########## key_press_event_cb : %d(%d)\n", vkey, event->hardware_keycode);
//	atUIENG_OnKeyDown( vkey );
	if(vkey != SYS_VKEY_NULL)
	{
		if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_KEYDOWN, vkey, 0 );
	}
	else
	{
		vkey = _OEM_SYSAPI_ConvOEMKey2VChar( event->hardware_keycode );
		if(vkey != SYS_VKEY_NULL)
		{
			if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_CHAR, vkey, 0 );
		}
	}
	return 1;
}

static gboolean key_release_event_cb(GtkWidget *widget, GdkEventKey *event)
{
	int vkey = OEM_SYSAPI_ConvOEMKey2VKey( event->hardware_keycode );
//	g_printf("########## key_release_event_cb : %d(%d)\n", vkey, event->hardware_keycode);
//	atUIENG_OnKeyUp( vkey );
	if( g_fnInputFunc ) g_fnInputFunc( SYS_MSG_KEYUP, vkey, 0 );
	return 1;
}


atBOOL OEM_SYSAPI_InitInputDev(  atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 )
{
	g_fnInputFunc = atNULL;

	if(lSysInst)
	{
		// MouseMove Event
		g_signal_connect(G_OBJECT(lSysInst), "motion_notify_event",
							  G_CALLBACK(motion_notify_event_cb), NULL);
		// MouseDown Event
		g_signal_connect(G_OBJECT(lSysInst), "button_press_event",
							  G_CALLBACK(button_press_event_cb), NULL);
		// MouseUp Event
		g_signal_connect(G_OBJECT(lSysInst), "button_release_event",
							  G_CALLBACK(button_release_event_cb), NULL);
		// KeyDown Event
		g_signal_connect(G_OBJECT(lSysInst), "key_press_event",
							  G_CALLBACK(key_press_event_cb), NULL);
		// KeyUp Event
		g_signal_connect(G_OBJECT(lSysInst), "key_release_event",
							  G_CALLBACK(key_release_event_cb), NULL);
	}

	return atTRUE;
}


void OEM_SYSAPI_FinalInputDev(void)
{
	g_fnInputFunc = atNULL;

}


void	OEM_SYSAPI_SetInputNotifyFunc( SYS_FUNC_INPUTNOTIFY *fnInputFunc )
{
	g_fnInputFunc = fnInputFunc;
}

int OEM_SYSAPI_ConvOEMKey2VKey( int OEMKey )
{
	int vk = SYS_VKEY_NULL;

	switch( OEMKey ) {
		case 36: 	//ENTER(BODY)
		case 110:	//HOME(MIDDLE)
		case 104:	//ENTER(KEY-PAD)
			vk = SYS_VKEY_SELECT;
			break;
		case 22:		//BACK-SPACE(BODY)
			vk = SYS_VKEY_CLEAR;
			break;
		case 111:	//UP(MIDDLE)
			vk = SYS_VKEY_UP;
			break;
		case 113:	//LEFT(MIDDLE)
			vk = SYS_VKEY_LEFT;
			break;
		case 114:	//RIGHT(MIDDLE)
			vk = SYS_VKEY_RIGHT;
			break;
		case 116:	//DOWN(MIDDLE)
			vk = SYS_VKEY_DOWN;
			break;
		case 118:	//INSERT(MIDDLE)
			vk = SYS_VKEY_SOFT1;
			break;
		case 112:	//PAGE-UP(MIDDLE)
			vk = SYS_VKEY_SOFT2;
			break;
		case 119:	//DELETE(MIDDLE)
			vk = SYS_VKEY_SEND;
			break;
		case 117:	//PAGE-DOWN(MIDDLE)
			vk = SYS_VKEY_END;
			break;
		case 90:		//0(KEY-PAD)
		//case 19:		//0(BODY)
			vk = SYS_VKEY_0;
			break;
		case 87:		//1(KEY-PAD)
		//case 10:		//1(BODY)
			vk = SYS_VKEY_1;
			break;
		case 88:		//2(KEY-PAD)
		//case 11:		//2(BODY)
			vk = SYS_VKEY_2;
			break;
		case 89:		//3(KEY-PAD)
		//case 12:		//3(BODY)
			vk = SYS_VKEY_3;
			break;
		case 83:		//4(KEY-PAD)
		//case 13:		//4(BODY)
			vk = SYS_VKEY_4;
			break;
		case 84:		//5(KEY-PAD)
		//case 14:		//5(BODY)
			vk = SYS_VKEY_5;
			break;
		case 85:		//6(KEY-PAD)
		//case 15:		//6(BODY)
			vk = SYS_VKEY_6;
			break;
		case 79:		//7(KEY-PAD)
		//case 16:		//7(BODY)
			vk = SYS_VKEY_7;
			break;
		case 80:		//8(KEY-PAD)
		//case 17:		//8(BODY)
			vk = SYS_VKEY_8;
			break;
		case 81:		//9(KEY-PAD)
		//case 18:		//9(BODY)
			vk = SYS_VKEY_9;
			break;
		case 21: 	//'='(MIDDLE) Samsung Demo
		case 86:		//+(KEY-PAD)
			vk = SYS_VKEY_STAR;
			break;
		case 20: 	//'-'(MIDDLE) Samsung Demo
		case 82:		//-(KEY-PAD)
			vk = SYS_VKEY_POUND;
			break;
	}

	return vk;
}


int OEM_SYSAPI_ConvVKey2OEMKey( int VKey )
{
    int ok = 0;

ok = VKey;

    return ok;
}


int OEM_SYSAPI_ConvOEMPtr2VPtr( int *x, int *y )
{
	if(g_SYS_nScale <= 1) return 0;

	*x = *x / g_SYS_nScale;
	*y = *y / g_SYS_nScale;
	return 0;
}


int OEM_SYSAPI_ConvVPtr2OemPtr( int *x, int *y )
{
	if(g_SYS_nScale <= 1) return 0;

	*x = *x * g_SYS_nScale;
	*y = *y * g_SYS_nScale;
	return 0;
}
