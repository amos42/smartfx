# Microsoft Developer Studio Project File - Name="A_UIEng" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=A_UIEng - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "A_UIEng.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "A_UIEng.mak" CFG="A_UIEng - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "A_UIEng - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "A_UIEng - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "A_UIEng - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "A_UIEng - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "A_UIEng"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "A_UIEng - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "../../../../inc" /I "../../inc" /I "../../OEM_inc" /I "../../../../EXT_LIB/A_Common/inc" /I "../../../../EXT_LIB/A_Util/inc" /I "../../../../EXT_LIB/A_Math/inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "A_UIEng - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../../inc" /I "../../inc" /I "../../OEM_inc" /I "../../../../EXT_LIB/A_Common/inc" /I "../../../../EXT_LIB/A_Util/inc" /I "../../../../EXT_LIB/A_Math/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "A_UIEng - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "A_UIEng___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "A_UIEng___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../__output/obj/A_UIEng/Win32UnicodeDebug"
# PROP Intermediate_Dir "../../../../__output/obj/A_UIEng/Win32UnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../A_UIEng/inc" /I "../../../../inc" /I "../../../A_Util/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../../inc" /I "../../../A_Util/inc" /I "../../../../EXT_LIB/A_Common/inc" /I "../../../../EXT_LIB/A_Util/inc" /I "../../../../EXT_LIB/A_Math/inc" /I "../../inc" /I "../../OEM_inc" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "UNICODE" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__output\lib\UnicodeDebug\A_UIEng.lib"

!ELSEIF  "$(CFG)" == "A_UIEng - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "A_UIEng___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "A_UIEng___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../__output/obj/A_UIEng/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../../__output/obj/A_UIEng/Win32UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../../../A_UIEng/inc" /I "../../../../inc" /I "../../../A_Util/inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../lib/A_Common/inc" /I "../../lib/A_Util/inc" /I "../../../../inc" /I "../../../A_Util/inc" /I "../../inc" /I "../../OEM_inc" /I "../../../../EXT_LIB/A_Common/inc" /I "../../../../EXT_LIB/A_Util/inc" /I "../../../../EXT_LIB/A_Math/inc" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__output\lib\UnicodeRelease\A_UIEng.lib"

!ENDIF 

# Begin Target

# Name "A_UIEng - Win32 Release"
# Name "A_UIEng - Win32 Debug"
# Name "A_UIEng - Win32 Unicode Debug"
# Name "A_UIEng - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "UI_API"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_APIs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_Debug.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_File.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_Font.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_gdi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_IME.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_key.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_List.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\src\UI_API\UI_Proc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_Res.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_str.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_SysUtil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\UI_API\UI_Timer.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\AnnBar.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\App.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\AppMng.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\Control.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\CtlMng.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\Msg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\SoftKey.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\UI_Desk.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Core\UI_Disp.c
# End Source File
# Begin Source File

SOURCE=..\..\src\EntryPtr\UI_Entry.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Core\UI_Event.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Core\UI_Kern.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Util\UI_Util.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\Window.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\WinMng.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\WinObj.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\WinObjPT.c
# End Source File
# Begin Source File

SOURCE=..\..\..\A_UIEng\src\Core\WObjMng.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "UI_API_H"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_API.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_Cfg.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_Debug.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_File.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_Font.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_gdi.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_IME.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_key.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_List.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_mem.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_Proc.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_Res.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_Sound.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_str.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_SysUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_API\UI_Timer.h
# End Source File
# End Group
# Begin Group "UI_OEM_H"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_Debug.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_File.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_Font.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_GDI.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_Key.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_Mem.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_Proc.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_Res.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_Sound.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_str.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_System.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_UI_Timer.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\inc\A_UI_MSG.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\A_UIENG.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\AnnBar.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\App.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\AppMng.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Control.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\CtlMng.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Msg.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SoftKey.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_Desk.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_Disp.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_Entry.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_Event.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_Kern.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_MNG.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_OpDef.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_Types.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\UI_Util.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\Window.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\WinMng.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\WinObj.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\WinObjPT.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\WObjMng.h
# End Source File
# End Group
# Begin Group "A_Common"

# PROP Default_Filter "*.c,*.h"
# Begin Source File

SOURCE=..\..\..\..\EXT_LIB\A_Common\inc\AT_Real.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\EXT_LIB\A_Common\inc\AT_Types.h
# End Source File
# End Group
# End Target
# End Project
