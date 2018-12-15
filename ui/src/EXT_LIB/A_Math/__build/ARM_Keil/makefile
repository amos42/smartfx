# 
# Usege : make -f <proj_root>/Build/makefile -C <proj_root> 
#
 
BUILD_SCRIPT_VERSION := 1.2.0

.PHONY : app_version app_clean build_version


all : app_build
	
clean : app_clean

version : build_version


_SPACE := \ #
_SPACE_ESC := &sp;#
BUILD_ROOT := $(subst $(_SPACE_ESC),$(_SPACE),$(dir $(subst $(_SPACE),$(_SPACE_ESC),$(subst \,/,$(MAKEFILE_LIST)))))

include $(BUILD_ROOT)/funcs.mk

ifneq ($(strip $(PROJ_PATH)),)
PROJ_ROOT := $(call BSLASH2SLASH,$(PROJ_PATH))
else
PROJ_ROOT := ./
endif

include $(PROJ_ROOT)/project_def.prop
-include $(PROJ_ROOT)/build_def.prop

-include $(BUILD_ROOT)/basedef.mk
-include $(BUILD_ROOT)/tooldef.mk
-include $(BUILD_ROOT)/flags.mk


BUILD_ARCH := ARM#

APPTYPE := $(type)

OUTPUT_HOME ?= $(PROJ_ROOT)
OUTPUT_HOME := $(strip $(call BSLASH2SLASH,$(OUTPUT_HOME)))
OBJ_OUTPUT := $(OUTPUT_HOME)/temp/$(BUILD_ARCH)$(DEVNAME)/$(APPNAME)/$(BUILD_CONFIG)

LOWER_APPNAME := $(call LOWER_CASE,$(APPNAME))
APPID2 := $(subst $(basename $(APPID)).,,$(APPID))

ifeq ($(strip $(APPTYPE)),app)
APPFILE := $(OUTPUT_HOME)/bin/$(BUILD_ARCH)$(DEVNAME)/$(BUILD_CONFIG)/$(LOWER_APPNAME)
endif
ifeq ($(strip $(APPTYPE)),staticLib)
APPFILE := $(OUTPUT_HOME)/lib/$(BUILD_ARCH)$(DEVNAME)/$(BUILD_CONFIG)/lib$(LOWER_APPNAME).a
endif
ifeq ($(strip $(APPTYPE)),sharedLib)
APPFILE := $(OUTPUT_HOME)/lib/$(BUILD_ARCH)$(DEVNAME)/$(BUILD_CONFIG)/lib$(LOWER_APPNAME).so
endif

ifneq ($(strip $(PLATFORM_INCS)),)
PLATFORM_INCS_FILE := $(OBJ_OUTPUT)/platform_incs_file.inc
endif

include $(BUILD_ROOT)/build_c.mk


# for gcc
#M_OPT := -MMD -MP
#M_OPR := -MF "$$(@:%.o=%.d)"

# for armcc
M_OPT := 
M_OPR := --omf_browse="$$(@:%.o=%.crf)" --depend="$$(@:%.o=%.d)" 


C_OPT := $(COMPILE_FLAGS) $(TC_COMPILER_MISC) $(RS_COMPILER_MISC) $(EXT_OP) $(M_OPT)
CPP_OPT := $(CPP_COMPILE_FLAGS) $(TC_COMPILER_MISC) $(RS_COMPILER_MISC) $(EXT_OP) $(M_OPT)
C_OPT_FILE := $(PLATFORM_INCS_FILE)

OBJS := # 

# Global C/C++
ifeq ($(strip $(USER_ROOT)),)
USER_ROOT := $(PROJ_ROOT)
endif
$(eval $(call C_PROC_RAW,$(OBJ_OUTPUT),$(USER_SRCS),$(USER_INC_DIRS),$(USER_INC_FILES),$(USER_DEFS),$(USER_UNDEFS),$(C_OPT),$(C_OPT_FILE),C,c,$(CC),OBJS))
$(eval $(call C_PROC_RAW,$(OBJ_OUTPUT),$(USER_SRCS),$(USER_INC_DIRS),$(USER_CPP_INC_FILES),$(USER_CPP_DEFS),$(USER_CPP_UNDEFS),$(CPP_OPT),$(C_OPT_FILE),C++,cpp,$(CXX),OBJS))

# Individual C/C++
ifneq ($(strip $(USER_EXT_C_KEYS)),)
$(foreach var,$(USER_EXT_C_KEYS),$(eval $(call C_PROC_RAW,$(OBJ_OUTPUT),$(USER_EXT_$(var)_SRCS),$(USER_EXT_$(var)_INC_DIRS),$(USER_EXT_$(var)_INC_FILES),$(USER_EXT_$(var)_DEFS),$(USER_EXT_$(var)_UNDEFS),$(C_OPT),$(C_OPT_FILE),C,c,$(CC),OBJS)))
$(foreach var,$(USER_EXT_C_KEYS),$(eval $(call C_PROC_RAW,$(OBJ_OUTPUT),$(USER_EXT_$(var)_SRCS),$(USER_EXT_$(var)_INC_DIRS),$(USER_EXT_$(var)_CPP_INC_FILES),$(USER_EXT_$(var)_CPP_DEFS),$(USER_EXT_$(var)_CPP_UNDEFS),$(C_OPT),$(C_OPT_FILE),C++,cpp,$(CXX),OBJS)))
endif


ifneq ($(strip $(USER_LIB_DIRS)),)
_ENC_USER_LIB_DIRS := $(call ENCODE_4MAKE,$(USER_LIB_DIRS))
_ENC_USER_LIB_DIRS := $(addprefix -L,$(call $(_ENC_USER_LIB_DIRS))
LIBPATHS := $(call DECODE_4MAKE,$(_ENC_USER_LIB_DIRS))
endif

LIBS += $(addprefix -l,$(USER_LIBS))

UOBJS += $(USER_OBJS)

DEPS := $(OBJS:.o=.d)

ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif


ifeq ($(strip $(APPTYPE)),app)
EXT_OP := -fPIE 
$(APPFILE) : $(OBJS) $(UOBJS)
	@echo '  Building target: $@'
	@echo '  Invoking: C/C++ Linker'
	$(call MAKEDIRS,$(@D))
	$(CXX) -o $(APPFILE) $(OBJS) $(UOBJS) $(LIBPATHS) -Xlinker --as-needed $(LIBS) $(LINK_FLAGS) $(TC_LINKER_MISC) $(RS_LINKER_MISC) -pie -lpthread -Xlinker -rpath="/home/developer/sdk_tools/lib" --sysroot="$(SYSROOT)" -Xlinker --version-script="$(PROJ_ROOT).exportMap" $(RS_LIB_PATHS) $(RS_LIBRARIES) -Xlinker -rpath="/opt/usr/apps/$(APPID)/lib" -Werror-implicit-function-declaration
	@echo '  Finished building target: $@'
endif
ifeq ($(strip $(APPTYPE)),staticLib)
EXT_OP := -fPIE
$(APPFILE) : $(OBJS) $(UOBJS)
	@echo '  Building target: $@'
	@echo '  Invoking: Archive utility'
	$(call MAKEDIRS,$(@D))
	$(AR) -r $(APPFILE) $(OBJS) $(UOBJS) $(AR_FLAGS)
	@echo '  Finished building target: $@'
endif
ifeq ($(strip $(APPTYPE)),sharedLib)
EXT_OP := -fPIC 
$(APPFILE) : $(OBJS) $(UOBJS)
	@echo '  Building target: $@'
	@echo '  Invoking: C/C++ Linker'
	$(call MAKEDIRS,$(@D))
	$(CXX) -o $(APPFILE) $(OBJS) $(UOBJS) $(LIBPATHS) -Xlinker --as-needed $(LIBS) $(LINK_FLAGS) $(TC_LINKER_MISC) $(RS_LINKER_MISC) -shared -lpthread --sysroot="$(SYSROOT)" $(RS_LIB_PATHS) $(RS_LIBRARIES)
	@echo '  Finished building target: $@'
endif


$(OBJ_OUTPUT) :
	$(call MAKEDIRS,$@)

$(OUTPUT_HOME) :
	$(call MAKEDIRS,$@)

  
ifneq ($(strip $(PLATFORM_INCS)),)
$(PLATFORM_INCS_FILE) : $(OBJ_OUTPUT)
	@echo '  Building inc file: $@'
ifneq ($(findstring Windows,$(OS)),)
ifneq ($(findstring 3.82,$(MAKE_VERSION)),)
	$(file > $@,$(PLATFORM_INCS))
else
	@echo $(PLATFORM_INCS) > $@
endif    
else
	@echo $(PLATFORM_INCS) > $@
endif
endif


secondary-outputs : 

-include appendix.mk

app_build : $(OUTPUT_HOME) $(APPFILE) secondary-outputs
	@echo ========= done =========


app_clean :
	rm -f $(APPFILE)
	rm -rf $(OBJ_OUTPUT)

build_version :
	@echo makefile.mk : $(BUILD_SCRIPT_VERSION)
