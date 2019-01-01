#
# EXT_LIB/freetypeLib
#



TROOT_DIR	=	../../../../

SRC_DIR	=	../../src
INC_DIR	=	Inc
OBJ_DIR	=	$(TROOT_DIR)__output/obj
EXT_LIB_DIR	=	$(TROOT_DIR)EXT_LIB


SDE_VER = 6

COACH_VER_HEX = 0x102


SRCS	=	$(SRC_DIR)/autofit/autofit.c	\
		  $(SRC_DIR)/bdf/bdf.c	\
		  $(SRC_DIR)/cff/cff.c	\
		  $(SRC_DIR)/base/ftbase.c	\
		  $(SRC_DIR)/base/ftbbox.c	\
		  $(SRC_DIR)/base/ftbdf.c	\
		  $(SRC_DIR)/base/ftbitmap.c	\
		  $(SRC_DIR)/base/ftgasp.c	\
		  $(SRC_DIR)/base/ftglyph.c	\
		  $(SRC_DIR)/base/ftgxval.c	\
		  $(SRC_DIR)/base/ftinit.c	\
		  $(SRC_DIR)/base/ftmm.c	\
		  $(SRC_DIR)/base/ftotval.c	\
		  $(SRC_DIR)/base/ftpfr.c	\
		  $(SRC_DIR)/base/ftstroke.c	\
		  $(SRC_DIR)/base/ftsynth.c	\
		  $(SRC_DIR)/base/ftsystem.c	\
		  $(SRC_DIR)/base/fttype1.c	\
		  $(SRC_DIR)/base/ftwinfnt.c	\
		  $(SRC_DIR)/base/ftxf86.c	\
		  $(SRC_DIR)/base/ftdebug.c	\
		  $(SRC_DIR)/cache/ftcache.c	\
		  $(SRC_DIR)/gzip/ftgzip.c	\
		  $(SRC_DIR)/lzw/ftlzw.c	\
		  $(SRC_DIR)/pcf/pcf.c	\
		  $(SRC_DIR)/pfr/pfr.c	\
		  $(SRC_DIR)/psaux/psaux.c	\
		  $(SRC_DIR)/pshinter/pshinter.c	\
		  $(SRC_DIR)/psnames/psmodule.c	\
		  $(SRC_DIR)/raster/raster.c	\
		  $(SRC_DIR)/sfnt/sfnt.c	\
		  $(SRC_DIR)/smooth/smooth.c	\
		  $(SRC_DIR)/truetype/truetype.c	\
		  $(SRC_DIR)/type1/type1.c	\
		  $(SRC_DIR)/cid/type1cid.c	\
		  $(SRC_DIR)/type42/type42.c	\
		  $(SRC_DIR)/winfonts/winfnt.c


OBJS = $(notdir $(SRCS:.c=.o))


SYSAPI_DIR	= $(TROOT_DIR)SYSAPI
TARGET = $(TROOT_DIR)__output/lib/tx_debug/freetypelib.a


UIBASE_DIR	= $(TROOT_DIR)UI_Base
ZORANFWDIR = $(TROOT_DIR)HZ1010/CoachFW/C10p
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
	-I$(TROOT_DIR)EXT_LIB/zlib	\
	-I../../include/freetype/config	\
	-I../../include/freetype/internal	\
	-I../../include/freetype	\
	-I../../include	\
	-I../../src/autofit	\
	-I../../src/bdf	\
	-I../../src/cff	\
	-I../../src/base	\
	-I../../src/cache	\
	-I../../src/cache	\
	-I../../src/gzip	\
	-I../../src/lzw	\
	-I../../src/pcf	\
	-I../../src/pfr	\
	-I../../src/psaux	\
	-I../../src/pshinter	\
	-I../../src/psnames	\
	-I../../src/raster	\
	-I../../src/sfnt	\
	-I../../src/smooth	\
	-I../../src/truetype	\
	-I../../src/type1	\
	-I../../src/type42	\
	-I../../src/winfonts	\
	
INCS += \
 
TR32 = -O2
 
 
#DEFINES = -D__MIPS_ -DDEBUG -DTX_DISABLE_INLINE -DGNU -DFW_VERSION=$(COACH_VER_HEX) -DZSDE=$(SDE_VER)
#GCCFLAGS = $(DEFINES) $(INCS) -G 0 -mno-float -mno-long-calls -mno-embedded-data -c -march=mips32 -mtune=4kem  -EL -O2  -Wsign-compare -Wreturn-type -Wuninitialized -Wimplicit -fshort-wchar -fno-zero-initialized-in-bss -fasynchronous-unwind-tables
#GPPFLAGS = $(DEFINES) $(INCS) -G 0 -mno-float -membedded-data -muninit-const-in-rodata -fcommon -c -nostdinc -EL -O2 -march=mips32 -mtune=4kem  -fno-zero-initialized-in-bss -fasynchronous-unwind-tables  -Wsign-compare -Wreturn-type -Wuninitialized -Wimplicit -mno-long-calls
 
//DEFINES = -D__MIPS_ -D_TX_ -DDEBUG -DTX_DISABLE_INLINE -DGNU -DFW_VERSION=$(COACH_VER_HEX) -DZSDE=$(SDE_VER)	-DFT2_BUILD_LIBRARY
DEFINES = -D__MIPS_ -D_TX_ -DNODEBUG -DTX_DISABLE_INLINE -DGNU -DFW_VERSION=$(COACH_VER_HEX) -DZSDE=$(SDE_VER)	-DFT2_BUILD_LIBRARY


#GCCFLAGS = $(DEFINES) $(INCS) -ffunction-sections -fno-rtti -fno-exceptions -D_THREADX -mno-float -G 0 \
#	-membedded-data -muninit-const-in-rodata -fcommon -c -nostdinc \
#    -EL $(TR32) -march=mips32 -mtune=4kem  -fno-zero-initialized-in-bss -fasynchronous-unwind-tables\
#    -Wsign-compare -Wreturn-type -Wuninitialized -Wimplicit -mno-long-calls -DTX_DISABLE_INLINE -w

#CPPFLAGS = $(GCCFLAGS) -D_UNICODE -D_LASTPLAY_ -DFPM_DEFAULT -DHAVE_CONFIG_H -DASO_ZEROCHECK \
#	 -DM_SCORE_SIGNED_16BIT -D_COREEXT -D_FILEBASE -D_CSP_ -DD_CSP_2_0_ -D_THREADX $(INCS)

CPPFLAGS = $(DEFINES) $(INCS) -G 0 -mno-float -membedded-data -muninit-const-in-rodata -fcommon -c -nostdinc \
           -EL -O2 -march=mips32 -mtune=4kem  -fno-zero-initialized-in-bss -fasynchronous-unwind-tables  -Wsign-compare -Wreturn-type -Wuninitialized -Wimplicit -mno-long-calls -fshort-wchar


CFLAGS = $(DEFINES) $(INCS) -G 0 -mno-float -membedded-data -muninit-const-in-rodata -fcommon -c -nostdinc \
           -EL -O2 -march=mips32 -mtune=4kem  -fno-zero-initialized-in-bss -fasynchronous-unwind-tables  -Wsign-compare -Wreturn-type -Wuninitialized -Wimplicit -mno-long-calls -fshort-wchar


BIN_ROOT = /cygdrive/c/sde6/bin
RANLIB   = sde-ranlib
CXX      = sde-gcc -x c++
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
	@$(CC) -c $(CFLAGS) -o $@ $<

dep:
	@for X in $(SRCS) ;\
	do \
		$(CC) -MM -MD $(INCS) $(CFLAGS) $$X;\
	done


clean:
	rm $(TARGET) $(OBJS)

mostlyclean: $(RM) $(OBJS)
