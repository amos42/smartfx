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

CHARSET_OP :=
ifeq ($(strip $(charset)),unicode)
CHARSET_OP += -DUNICODE -D__UNICODE
else
ifeq ($(strip $(charset)),mbc)
CHARSET_OP +=
endif
endif

COMPILE_FLAGS = $(DEBUG_OP) $(OPTIMIZATION_OP) $(CHARSET_OP) $(USER_EXT_OP) -c
                                                                                                                                           
CPP_COMPILE_FLAGS = $(CPP_DEBUG_OP) $(CPP_OPTIMIZATION_OP) $(USER_EXT_OP) -c -cpp

LINK_FLAGS = -L"$(PROJ_PATH)/lib" -Wl,--no-undefined

AR_FLAGS = 

