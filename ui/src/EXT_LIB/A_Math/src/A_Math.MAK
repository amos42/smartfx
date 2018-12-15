#=================================#
#                                 #
#  A_Math Library MAKE File       #
#            Chu, Kyong-min       #
#                                 #
#  Usege : Make.exe -f A_Math.mak   #
#                                 #
#=================================#


PROJECT = A_Math

#TARGET = X86
#TARGET = ARM7
#COMPILER = BC
#COMPILER = VC


#============================
# Input : PROJECT, TARGET, COMPILER
# Output : OUT, TGT, OBJ_SUF, PROJECT_FILE
!include "../ALib_Def.min"
#============================

all : $(PROJECT_FILE)


#============================

OBJS =  $(TGT)\INTSqrt.$(OBJ_SUF)       \  
        $(TGT)\TriFunc.$(OBJ_SUF)       \
        $(TGT)\TriFunc2.$(OBJ_SUF)      \
        $(TGT)\Vector3.$(OBJ_SUF)       \
        $(TGT)\Matrix4.$(OBJ_SUF)

#============================


!if "$(TARGET)" == "X86"
!if "$(COMPILER)" == "BC"
CC_INC = -I. -I./Base -I./Tri -I./3D -I../A_VM/__Output/Include
!elif "$(COMPILER)" == "VC"
CC_INC = -I. -I./Base -I./Tri -I./3D -I../A_VM/__Output/Include
!endif
!elif "$(TARGET)" == "ARM7"
CC_INC = -I . -I ./Base -I ./Tri -I ./3D -I ../A_VM/__Output/Include
!elif "$(TARGET)" == "ARM9"
CC_INC = -I . -I ./Base -I ./Tri -I ./3D -I ../A_VM/__Output/Include 
!endif


#============================
# Input : PROJECT, PROJECT_FILE, TARGET, COMPILER, OBJS, CC_INC
# Output :
!include "../../Base_Rul.min"
#============================



#==========< Path Start >============#

.path.h = ./;../a_vm/__output/include

#============< Path End >============#



#==========< Start >============#


# Math util

$(TGT)\intsqrt.$(OBJ_SUF)     : Base/intsqrt.c     Base/intsqrt.h BaseDef.h RealNum.h
$(TGT)\trifunc.$(OBJ_SUF)     : Tri/trifunc.c      Tri/trifunc.h  BaseDef.h RealNum.h
$(TGT)\trifunc2.$(OBJ_SUF)    : Tri/trifunc2.c     Tri/trifunc.h  BaseDef.h RealNum.h


# 3D Math

$(TGT)\Vector3.$(OBJ_SUF)    : 3d/Vector3.c    3d/Vector3.h  BaseDef.h RealNum.h
$(TGT)\Matrix4.$(OBJ_SUF)    : 3d/Matrix4.c    3d/Matrix4.h  BaseDef.h RealNum.h


#===========< END >=============#
