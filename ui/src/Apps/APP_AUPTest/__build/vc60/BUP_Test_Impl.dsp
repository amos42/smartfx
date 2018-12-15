# Microsoft Developer Studio Project File - Name="BUP_Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=BUP_Test - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BUP_Test_Impl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BUP_Test_Impl.mak" CFG="BUP_Test - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BUP_Test - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BUP_Test - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BUP_Test - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BUP_Test - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "BUP_Test"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BUP_Test - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "../../inc" /I "../../../../inc" /I "../../../A_Math/inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BUP_Test - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../inc" /I "../../../../inc" /I "../../../A_Math/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BUP_Test - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GrpX___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "GrpX___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../../__Output/obj/BUP_Test/Win32UnicodeDebug"
# PROP Intermediate_Dir "../../../../../__Output/obj/BUP_Test/Win32UnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../BUP_Test/inc" /I "../../../AGG/inc" /I "../../../A_Common/inc" /I "../../../A_Math/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /GX /Zi /Od /I "../../../../../inc" /I "../../../../../SYSAPI/inc" /I "../../../../../EXT_LIB/A_Math/inc" /I "../../../../../UI_Base/BUP/inc" /I "../../../../../UI_BASE/Component/BasePkg/inc" /I "../../../../../UI_BASE/Component/ExtPkg/inc" /I "../../src" /I "../../../../../UI_Base/inc" /I "../../../../BUP_Launcher/inc" /D "WIN32" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "_LIB" /FAcs /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\__Output\lib\UnicodeDebug\BUP_Test.lib"

!ELSEIF  "$(CFG)" == "BUP_Test - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GrpX___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "GrpX___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../__Output/obj/BUP_Test/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../../__Output/obj/BUP_Test/Win32UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../../../BUP_Test/inc" /I "../../../A_Common/inc" /I "../../../A_Math/inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../inc" /I "../../../../inc" /I "../../../A_Math/inc" /I "../../../../Core" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__Output\lib\UnicodeRelease\BUP_Test.lib"

!ENDIF 

# Begin Target

# Name "BUP_Test - Win32 Release"
# Name "BUP_Test - Win32 Debug"
# Name "BUP_Test - Win32 Unicode Debug"
# Name "BUP_Test - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\APP_Entry.c
# End Source File
# Begin Source File

SOURCE=..\..\src\APP_Entry.h
# End Source File
# Begin Source File

SOURCE=..\..\src\EntryPtr.c
# End Source File
# Begin Source File

SOURCE=..\..\src\EqSetWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\EqSetWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Global.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Global.h
# End Source File
# Begin Source File

SOURCE=..\..\src\MainWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\MainWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\SettingWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SettingWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sound_data.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sound_data_16b.h
# End Source File
# Begin Source File

SOURCE=..\..\src\SoundTestWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SoundTestWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TestWnd.c
# End Source File
# Begin Source File

SOURCE=..\..\src\TestWnd.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
