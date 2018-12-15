# Request Macro Values :
# G_PROGNAME, G_PROG_TYPE, G_PROG_PATH, G_OBJ_PATH, G_LIB_PATH, G_SRCS, G_LIBS, G_INC_PATH, G_DEFINE, G_CFLAG

G_PROG_NAME = BUP_Launcher
#G_PROG_TYPE = library
G_PROG_TYPE = execute

_BASE_PATH = ../..
_OUT_PATH = ../../../../__output
_SRC_PATH = $(_BASE_PATH)/src

G_PROG_PATH = $(_OUT_PATH)/bin/SLP
G_OBJ_PATH = $(_OUT_PATH)/obj/$(G_PROG_NAME)/SLP
G_LIB_PATH = $(_OUT_PATH)/lib/SLP

G_DEFINE = _LINUX_ __ARM_ UNICODE _UNICODE
G_CFLAG = -lstdc++


G_INC_PATH = ../../../../inc

G_INC_PATH += $(_SRC_PATH)
G_INC_PATH += $(_SRC_PATH)/inc

_SRC_PATH1 = $(_SRC_PATH)
_SRCS1_T = Global.c 
_SRCS1 = $(addprefix $(_SRC_PATH1)/, $(_SRCS1_T))


G_SRCS = $(_SRCS1) 


G_LIBS = COMP_ExtPkg COMP_BasePkg utilLib netDB bgmLib mmLib rpLib routedbLib zlib dsdbLib A_UIEng commLib \
         rgLib SYSAPI_OEM_SLP SYSAPI BUP_EXT BUP_OEM_SLP A_Res freetype GrpX Globe3D B_NMEA AGG \
         mad Landmark3D A_RES A_Math
