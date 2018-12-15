# Microsoft Developer Studio Project File - Name="COMP_ExtPkg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=COMP_ExtPkg - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "COMP_ExtPkg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "COMP_ExtPkg.mak" CFG="COMP_ExtPkg - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "COMP_ExtPkg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "COMP_ExtPkg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "COMP_ExtPkg - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "COMP_ExtPkg - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "COMP_ExtPkg"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "COMP_ExtPkg - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "../../../../A_UIEng/inc" /I "../../../../ExtLib/A_Common/inc" /I "../../inc" /I "../../src/CustCtl/inc" /I "../../src/CustWin/inc" /I "../../../BasePkg/inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "COMP_ExtPkg - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../../A_UIEng/inc" /I "../../../../ExtLib/A_Common/inc" /I "../../inc" /I "../../src/CustCtl/inc" /I "../../src/CustWin/inc" /I "../../../BasePkg/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "COMP_ExtPkg - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "COMP_ExtPkg___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "COMP_ExtPkg___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../../__Output/obj/Comp_ExtPkg/Win32UnicodeDebug"
# PROP Intermediate_Dir "../../../../../__Output/obj/Comp_ExtPkg/Win32UnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "inc" /I "src/CustCtl/inc" /I "src/CustWin/inc" /I "../../AEELib/src/A_Common/inc" /I "../../AEELib/src/A_UIENG/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../../A_UIEng/inc" /I "../../../../../inc" /I "../../../../../EXT_LIB/A_Math/inc" /I "../../inc" /I "../../src/CustCtl/inc" /I "../../src/CustWin/inc" /I "../../../BasePkg/inc" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "UNICODE" /FD /GZ /c
# SUBTRACT CPP /u /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../../__Output/lib/UnicodeDebug/COMP_ExtPkg.lib"

!ELSEIF  "$(CFG)" == "COMP_ExtPkg - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "COMP_ExtPkg___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "COMP_ExtPkg___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../../__Output/obj/Comp_ExtPkg/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../../../__Output/obj/Comp_ExtPkg/Win32UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../../../A_UIEng/inc" /I "../../../../../inc" /I "../../../../../EXT_LIB/A_Math/inc" /I "../../inc" /I "../../src/CustCtl/inc" /I "../../src/CustWin/inc" /I "../../../BasePkg/inc" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../../__Output/lib/UnicodeRelease/COMP_ExtPkg.lib"

!ENDIF 

# Begin Target

# Name "COMP_ExtPkg - Win32 Release"
# Name "COMP_ExtPkg - Win32 Debug"
# Name "COMP_ExtPkg - Win32 Unicode Debug"
# Name "COMP_ExtPkg - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\AEECOMP_ExtPkg.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\AEECOMP_ExtPkg.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\ExtPkg_Ctl.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\ExtPkg_Win.h
# End Source File
# End Group
# Begin Group "CustCtl"

# PROP Default_Filter "*.c;*.h"
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_AniButton.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_AniButton.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_ButtonEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_ButtonEx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_CheckBoxEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_CheckBoxEx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_EditBoxEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_EditBoxEx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_HSliderEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_HSliderEx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_ImageButton.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_ImageButton.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_ListBoxEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_ListBoxEx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_MListBox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_MListBox.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_ProgressBarEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_ProgressBarEx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_RadioButtonEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_RadioButtonEx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_RadioGroupEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_RadioGroupEx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_ScrollBarEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_ScrollBarEx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_ScrollPanel.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_ScrollPanel.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_ScrollText.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_ScrollText.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_SpinBoxEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_SpinBoxEx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_TickerBox.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_TickerBox.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_VProgressBarEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_VProgressBarEx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\src\CMP_VSliderEx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustCtl\inc\CMP_VSliderEx.h
# End Source File
# End Group
# Begin Group "CustWin"

# PROP Default_Filter "*.c;*.h"
# Begin Source File

SOURCE=..\..\src\CustWin\src\FileListWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\FileListWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\src\HSliderWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\HSliderWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\src\ListBoxWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\ListBoxWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\src\MsgBoxExWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\MsgBoxExWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\src\PopupMenuWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\PopupMenuWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\src\ProgressWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\ProgressWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\src\RadioGroupWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\RadioGroupWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\src\TextViewWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\CustWin\inc\TextViewWnd.h
# End Source File
# End Group
# End Target
# End Project
