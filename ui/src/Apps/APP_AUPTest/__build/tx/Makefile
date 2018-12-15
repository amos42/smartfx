#
#  Product/Columbus
#



TROOT_DIR	=	../../../../

SRC_DIR	=	$(TROOT_DIR)Product/Columbus
INC_DIR	=	Inc
OBJ_DIR	=	$(TROOT_DIR)__output/obj
EXT_LIB_DIR	=	$(TROOT_DIR)EXT_LIB


SDE_VER = 6

COACH_VER_HEX = 0x102


SRCS	=	$(SRC_DIR)/_build/tx/ColumbusApplication.cpp	\
	$(SRC_DIR)/_build/tx/APP_Entry.cpp	\
	$(SRC_DIR)/_build/tx/EqSetWnd.cpp	\
	$(SRC_DIR)/_build/tx/Global.cpp	\
	$(SRC_DIR)/_build/tx/MainWnd.cpp	\
	$(SRC_DIR)/_build/tx/SettingWnd.cpp	\
	$(SRC_DIR)/_build/tx/TestWnd.cpp
	

				
OBJS = $(notdir $(SRCS:.cpp=.o))


SYSAPI_DIR	= $(TROOT_DIR)SYSAPI
TARGET = $(TROOT_DIR)__output/lib/tx_debug/columbus_navi.a


UIBASE_DIR	= $(TROOT_DIR)UI_Base
ZORANFWDIR = $(TROOT_DIR)_SDK/HZ1010/CoachFW/C10p
ZORANFW_LIB_DIR = $(ZORANFWDIR)/Lib
ZORANFW_OSLIB_DIR = $(ZORANFWDIR)/Os/Lib


INCS = \
	-I$(ZORANFWDIR)/Independent/CppStl/include/backward  \
	-I$(ZORANFWDIR)/Independent/CppStl/include/std  \
	-I$(ZORANFWDIR)/Independent/CppStl/include/c_std		\
	-I$(ZORANFWDIR)/Independent/CppStl/include/c			\
	-I$(ZORANFWDIR)/Independent/CppStl/include				\
	-I$(ZORANFWDIR)/Independent/CppStl/include/debug		\
	-I$(ZORANFWDIR)/Independent/Cpp/libsupc++				\
	-I$(ZORANFWDIR)/Independent/Cpp						\
	-I$(ZORANFWDIR)/Independent/CppStl						\
    -I/cygdrive/c/sde6/sde/Include \
	-I$(ZORANFWDIR)/Include			\
	-I$(ZORANFWDIR)/Os  			\
	-I$(ZORANFWDIR)/zcam/include	\
	-I$(ZORANFWDIR)/zcam/api		\
	-I$(ZORANFWDIR)/Ip/Include		\
	-I$(ZORANFWDIR)/zcam/zmovie/include		\
	-I$(ZORANFWDIR)/zcam/zagents/include	\
	-I$(ZORANFWDIR)/Os/Fs/fx		\
	-I$(ZORANFWDIR)/Os/Rtos/tx		\
	-I$(ZORANFWDIR)/Os/include		\
	-I$(ZORANFWDIR)/Os/Display		\
	-I$(ZORANFWDIR)/Os/Sensor/Statistics	\
	-I$(TROOT_DIR)inc	\
	-I$(SYSAPI_DIR)/inc	\
	-I$(TROOT_DIR)Core/inc	\
	-I$(TROOT_DIR)Core	\
	-I$(TROOT_DIR)OEM/SYSAPI_OEM/tx/OSAL/inc	\
	-I$(TROOT_DIR)EXT_LIB/zlib	\
	-I$(TROOT_DIR)UI_Base/BUP/inc	\
	-I$(TROOT_DIR)UI_Base/Component/BasePkg/inc	\
	-I$(TROOT_DIR)UI_Base/Component/ExtPkg/inc	\
	
	
INCS += \
 
TR32 = -O2
 

DEFINES = -D__MIPS_ -DDEBUG -D_TX_ -DTX_DISABLE_INLINE -DGNU -DFW_VERSION=$(COACH_VER_HEX) -DZSDE=$(SDE_VER) 

CPPFLAGS = $(DEFINES) $(INCS) -G 0 -mno-float -membedded-data -muninit-const-in-rodata -fcommon -c -nostdinc \
           -EL -O2 -march=mips32 -mtune=4kem  -fno-zero-initialized-in-bss -fasynchronous-unwind-tables  -Wsign-compare -Wreturn-type -Wuninitialized -Wimplicit -mno-long-calls


#CFLAGS = $(DEFINES) $(INCS) -G 0 -mno-float -membedded-data -muninit-const-in-rodata -fcommon -c -nostdinc \
#           -EL -O2 -march=mips32 -mtune=4kem  -fno-zero-initialized-in-bss -fasynchronous-unwind-tables  -Wsign-compare -Wreturn-type -Wuninitialized -Wimplicit -mno-long-calls -fwide-exce_charset

CFLAGS = $(DEFINES) $(INCS) -G 0 -mno-float -membedded-data -muninit-const-in-rodata -fcommon -c -nostdinc \
           -EL -O2 -march=mips32 -mtune=4kem  -fno-zero-initialized-in-bss -fasynchronous-unwind-tables  -Wsign-compare -Wreturn-type -Wuninitialized -Wimplicit -mno-long-calls

           

CCFLAGS = $(CFLAGS)



BIN_ROOT = /cygdrive/c/sde6/bin
RANLIB   = sde-ranlib
#CC       = sde-gcc -x c++
CC       = sde-gcc
AR       = sde-ar
LD		 = sde-ld

all: dep $(TARGET)

$(TARGET): $(OBJS)
	$(AR) cru $@ $^
	$(RANLIB) $@
	

ifneq ($(MAKECMDGOALS),clean)
    sinclude $(OBJS:.o=.d)
endif

$(OBJS):
	@echo Compiling  $@ $<
	@$(CC) -c $(CCFLAGS) -o $@ $<

dep:
	@for X in $(SRCS) ;\
	do \
		$(CC) -MM -MD $(INCS) $(CCFLAGS) $$X;\
	done


clean:
	rm $(TARGET) $(OBJS)

mostlyclean: $(RM) $(OBJS)

