# Request Macro Values :
# G_PROGNAME, G_PROG_TYPE, G_PROG_PATH, G_OBJ_PATH, G_LIB_PATH, G_SRCS, G_LIBS, G_INC_PATH, G_DEFINE, G_CFLAG

G_PROG_NAME = BUP_Test_Impl
G_PROG_TYPE = library
#G_PROG_TYPE = execute

_BASE_PATH = ../..
_OUT_PATH = ../../../../../__output
_SRC_PATH = $(_BASE_PATH)/src

G_PROG_PATH = $(_OUT_PATH)/lib/SLP
G_OBJ_PATH = $(_OUT_PATH)/obj/$(G_PROG_NAME)/SLP
G_LIB_PATH = $(_OUT_PATH)/lib/SLP

G_DEFINE = _LINUX_ __ARM_ UNICODE _UNICODE
G_CFLAG = 


G_INC_PATH = ../../../../../inc
G_INC_PATH += ../../../../../SYSAPI/inc
G_INC_PATH += ../../../../../UI_Base/inc
G_INC_PATH += ../../../../../UI_Base/A_UIEng/inc
G_INC_PATH += ../../../../../EXT_LIB/A_Util/inc
G_INC_PATH += ../../../../../EXT_LIB/A_Math/inc
G_INC_PATH += ../../../../../UI_Base/Component/BasePkg/inc
G_INC_PATH += ../../../../../UI_Base/Component/ExtPkg/inc
G_INC_PATH += ../../../../BUP_Launcher/inc


_SRC_PATH1 = $(_SRC_PATH)
_SRCS1_T = EntryPtr.c APP_Entry.c EqSetWnd.c Global.c MainWnd.c SettingWnd.c SoundTestWnd.c TestWnd.c 
_SRCS1 = $(addprefix $(_SRC_PATH1)/, $(_SRCS1_T))

G_SRCS = $(_SRCS1)


G_LIBS = 
