# Microsoft Developer Studio Project File - Name="COMP_BasePkg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=COMP_BasePkg - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "COMP_BasePkg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "COMP_BasePkg.mak" CFG="COMP_BasePkg - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "COMP_BasePkg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "COMP_BasePkg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "COMP_BasePkg - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "COMP_BasePkg - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "COMP_BasePkg"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "COMP_BasePkg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../../../A_UIEng/inc" /I "../../../../../inc" /I "../../../../../EXT_LIB/A_Math/inc" /I "../../inc" /I "../../src/CustCtl/inc" /I "../../src/CustWin/inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "COMP_BasePkg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../../A_UIEng/inc" /I "../../../../../inc" /I "../../../../../EXT_LIB/A_Math/inc" /I "../../inc" /I "../../src/CustCtl/inc" /I "../../src/CustWin/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "COMP_BasePkg - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "COMP_BasePkg___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "COMP_BasePkg___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../../__Output/obj/Comp_BasePkg/Win32UnicodeDebug"
# PROP Intermediate_Dir "../../../../../__Output/obj/Comp_BasePkg/Win32UnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "inc" /I "src/CustCtl/inc" /I "src/CustWin/inc" /I "../../AEELib/src/A_Common/inc" /I "../../AEELib/src/A_UIEng/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../../../EXT_LIB/A_Math/inc" /I "../../../../A_UIEng/inc" /I "../../../../../inc" /I "../../inc" /I "../../src/CustCtl/inc" /I "../../src/CustWin/inc" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "UNICODE" /FD /GZ /c
# SUBTRACT CPP /u /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../../__Output/lib/UnicodeDebug/COMP_BasePkg.lib"

!ELSEIF  "$(CFG)" == "COMP_BasePkg - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "COMP_BasePkg___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "COMP_BasePkg___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../../__Output/obj/Comp_BasePkg/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../../../__Output/obj/Comp_BasePkg/Win32UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../../../A_UIEng/inc" /I "../../../../../inc" /I "../../../../../EXT_LIB/A_Math/inc" /I "../../inc" /I "../../src/CustCtl/inc" /I "../../src/CustWin/inc" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../../__Output/lib/UnicodeRelease/COMP_BasePkg.lib"

!ENDIF 

# Begin Target

# Name "COMP_BasePkg - Win32 Release"
# Name "COMP_BasePkg - Win32 Debug"
# Name "COMP_BasePkg - Win32 Unicode Debug"
# Name "COMP_BasePkg - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\AEECOMP_BasePkg.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\AEECOMP_BasePkg.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\BasePkg_Ctl.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\BasePkg_Win.h
# End Source File
# End Group
# Begin Group "CustCtl"

# PROP Default_Filter "*.c;*.h"
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_Button.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_Button.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_CheckBox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_CheckBox.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_DummyCtl.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_DummyCtl.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_EditBox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_EditBox.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_HSlider.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_HSlider.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_ImageBox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_ImageBox.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_ListBox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_ListBox.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_ProgressBar.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_ProgressBar.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_RadioButton.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_RadioButton.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_RadioGroup.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_RadioGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_ScrollBar.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_ScrollBar.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_SpinBox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_SpinBox.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_StaticBox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_StaticBox.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_VProgressBar.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_VProgressBar.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_VSlider.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_VSlider.h
# End Source File
# End Group
# Begin Group "CustWin"

# PROP Default_Filter "*.c;*.h"
# Begin Source File

SOURCE=..\..\src\CustWin\src\DummyWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\DummyWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\src\EditBoxWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\EditBoxWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\src\MemoryInfoWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\MemoryInfoWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\src\MsgBoxWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\MsgBoxWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\src\WindowInfoWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\WindowInfoWnd.h
# End Source File
# End Group
# End Target
# End Project
