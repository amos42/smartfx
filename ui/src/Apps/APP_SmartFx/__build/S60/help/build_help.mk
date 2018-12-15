# ============================================================================
#  Name	 : build_help.mk
#  Part of  : BUP_Test
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : BUP_Test
#
#  Description: This make file will build the application help file (.hlp)
# 
# ============================================================================

do_nothing :
	@rem do_nothing

# build the help from the MAKMAKE step so the header file generated
# will be found by cpp.exe when calculating the dependency information
# in the mmp makefiles.

MAKMAKE : BUP_Test_0xED86F74E.hlp
BUP_Test_0xED86F74E.hlp : BUP_Test.xml BUP_Test.cshlp Custom.xml
	cshlpcmp BUP_Test.cshlp
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
	copy BUP_Test_0xED86F74E.hlp $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
endif

BLD : do_nothing

CLEAN :
	del BUP_Test_0xED86F74E.hlp
	del BUP_Test_0xED86F74E.hlp.hrh

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo BUP_Test_0xED86F74E.hlp

FINAL : do_nothing
