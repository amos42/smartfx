# Add tools flags defines

ifeq ($(strip $(BUILD_CONFIG)),Debug)
OPTIMIZATION_OP := -O0
DEBUG_OP := -g -D_DEBUG
CPP_OPTIMIZATION_OP := -O0
CPP_DEBUG_OP := -g -D_DEBUG
else
ifeq ($(strip $(BUILD_CONFIG)),Release)
OPTIMIZATION_OP := -O3
DEBUG_OP :=
CPP_OPTIMIZATION_OP := -O3
CPP_DEBUG_OP :=
endif
endif

ifeq ($(strip $(charset)),unicode)
CHARSET_OP := -DUNICODE -finput-charset=CP949 -fexec-charset=UCS-2LE
else
ifeq ($(strip $(charset)),mbc)
CHARSET_OP := -finput-charset=CP949 -fexec-charset=UCS-2LE
endif
endif

# --omf_browse "STM32746G_DISCOVERY\xxx.crf" --depend "STM32746G_DISCOVERY\xxx.d" %1 

COMPILE_FLAGS = $(DEBUG_OP) $(OPTIMIZATION_OP) $(CHARSET_OP) $(USER_EXT_OP) -c
#COMPILE_FLAGS = $(DEBUG_OP) $(OPTIMIZATION_OP) $(CHARSET_OP) $(USER_EXT_OP) -c --cpu Cortex-M7.fp.sp --fpu=FPv4-SP --apcs=interwork --split_sections --C99
                                                                                                                                           
CPP_COMPILE_FLAGS = $(CPP_DEBUG_OP) $(CPP_OPTIMIZATION_OP) $(USER_EXT_OP) -c -cpp
#CPP_COMPILE_FLAGS = $(CPP_DEBUG_OP) $(CPP_OPTIMIZATION_OP) $(USER_EXT_OP) -c -cpp --cpu Cortex-M7.fp.sp --fpu=FPv4-SP --apcs=interwork --split_sections --C99

LINK_FLAGS = -L"$(PROJ_PATH)/lib" -Wl,--no-undefined

AR_FLAGS = 

