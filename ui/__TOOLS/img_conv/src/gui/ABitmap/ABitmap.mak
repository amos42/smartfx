#============================#
#                            #
#  AGE Library MAKE File     #
#           Chu, Kyong-min   #
#                            #
#============================#

#===========================
# Compiler & Option
#===========================
BCB = c:\dev\borland\BCB6

CC_EXE  = $(BCB)\bin\bcc32.exe
CPP_EXE = $(BCB)\bin\bcc32.exe
ASM_EXE = $(BCB)\bin\bcc32.exe
LIB_EXE = $(BCB)\bin\tlib.exe

_DEBUG = YES

!if "$(_DEBUG)" == "YES"
#CC_OPTION  = -c -Od -H=$(BCB)\lib\vcl60.csm -Hc -Vx -Ve -X- -r- -a8 -b- -k -y -v -vi- -tW -tWM
#CPP_OPTION = -c -Od -H=$(BCB)\lib\vcl60.csm -Hc -Vx -Ve -X- -r- -a8 -b- -k -y -v -vi- -tW -tWM
CC_OPTION  = -c -Od -H=$(BCB)\lib\vcl60.csm -Hc -Vx -Ve -X- -r- -a8 -b- -k -y -v -vi-
CPP_OPTION = -c -Od -H=$(BCB)\lib\vcl60.csm -Hc -Vx -Ve -X- -r- -a8 -b- -k -y -v -vi-
PAS_OPTION = -$Y+ -$W -$O- -v -JPHNE -M
ASM_OPTION = /mx /w2 /zi
LIB_OPTION = /u
!else
CC_OPTION  = -c -O2 -H=$(BCB)\lib\vcl60.csm -Hc -Vx -Ve -X- -a8 -b- -k- -vi -tW -tWM
CPP_OPTION = -c -O2 -H=$(BCB)\lib\vcl60.csm -Hc -Vx -Ve -X- -a8 -b- -k- -vi -tW -tWM
PAS_OPTION = -$Y- -$L- -$D- -v -JPHNE -M
ASM_OPTION = /mx /w2 /zn
LIB_OPTION = /u
!endif


LIB_FILE = ABitmap.lib
H_FILE   = ABitmap.h

OBJS =  MyBitmap.obj Dither.obj

LIBS =

all : $(LIB_FILE)


#===========================
# Library
#===========================

$(LIB_FILE) : $(OBJS) $(LIBS) $(H_FILE)

  @$(LIB_EXE) $(LIB_OPTION) $(LIB_FILE) $(OBJS) $(LIBS)
  

#===========================
# Modules
#===========================

.c.obj:
  @$(CC_EXE) $(CC_OPTION) $<
.cpp.obj:
  @$(CPP_EXE) $(CPP_OPTION) $<
.asm.obj:
  @$(ASM_EXE) $(ASM_OPTION) $<


# Resource
mybitmap.obj    :   mybitmap.cpp      abitmap.h mybitmap.h dither.h
dither.obj      :   dither.cpp        abitmap.h dither.h


#===========< END >=============#
