# ============================================================================
#  Name	 : build_help.mk
#  Part of  : TornadoS
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : TornadoS
#
#  Description: This make file will build the application help file (.hlp)
# 
# ============================================================================

do_nothing :
	@rem do_nothing

# build the help from the MAKMAKE step so the header file generated
# will be found by cpp.exe when calculating the dependency information
# in the mmp makefiles.

MAKMAKE : TornadoS_0xEBB22191.hlp
TornadoS_0xEBB22191.hlp : TornadoS.xml TornadoS.cshlp Custom.xml
	cshlpcmp TornadoS.cshlp
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
	copy TornadoS_0xEBB22191.hlp $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
endif

BLD : do_nothing

CLEAN :
	del TornadoS_0xEBB22191.hlp
	del TornadoS_0xEBB22191.hlp.hrh

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo TornadoS_0xEBB22191.hlp

FINAL : do_nothing
