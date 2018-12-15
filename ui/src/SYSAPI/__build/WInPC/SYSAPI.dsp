# Microsoft Developer Studio Project File - Name="SYSAPI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SYSAPI - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SYSAPI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SYSAPI.mak" CFG="SYSAPI - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SYSAPI - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SYSAPI - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SYSAPI - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SYSAPI - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "SYSAPI"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SYSAPI - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "../../../inc" /I "../../inc" /I "../../OEM_inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SYSAPI - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../inc" /I "../../inc" /I "../../OEM_inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SYSAPI - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SYSAPI___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "SYSAPI___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../__output/obj/SYSAPI/Win32UnicodeDebug"
# PROP Intermediate_Dir "../../../__output/obj/SYSAPI/Win32UnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../SYSAPI/inc" /I "../../../A_Common/inc" /I "../../../A_Util/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../inc" /I "../../inc" /I "../../OEM_inc" /I "../../../EXT_LIB/B_NMEA/inc" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "UNICODE" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\__output\lib\UnicodeDebug\SYSAPI.lib"

!ELSEIF  "$(CFG)" == "SYSAPI - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SYSAPI___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "SYSAPI___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../__output/obj/SYSAPI/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../__output/obj/SYSAPI/Win32UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../../../SYSAPI/inc" /I "../../../A_Common/inc" /I "../../../A_Util/inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../../inc" /I "../../inc" /I "../../OEM_inc" /I "../../../EXT_LIB/B_NMEA/inc" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\__output\lib\UnicodeRelease\SYSAPI.lib"

!ENDIF 

# Begin Target

# Name "SYSAPI - Win32 Release"
# Name "SYSAPI - Win32 Debug"
# Name "SYSAPI - Win32 Unicode Debug"
# Name "SYSAPI - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\SA_Debug.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SA_File.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SA_GPS.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SA_key.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SA_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SA_Scr.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SA_Sound.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SA_str.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SA_SysUtil.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SA_Task.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SA_Timer.c
# End Source File
# Begin Source File

SOURCE=..\..\src\SYSAPI.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "SA_OEM_H"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_SA_Debug.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_SA_File.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_SA_Font.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_Inc\OEM_SA_GPS.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_SA_Key.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_SA_Mem.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_SA_Res.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_Inc\OEM_SA_Scr.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_SA_Sound.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_SA_str.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_SA_System.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_Inc\OEM_SA_Task.h
# End Source File
# Begin Source File

SOURCE=..\..\OEM_inc\OEM_SA_Timer.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\inc\SA_Cfg.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SA_Debug.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SA_File.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SA_GPS.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SA_key.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SA_mem.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SA_scr.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SA_Sound.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SA_str.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SA_SysUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SA_Task.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SA_Timer.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\SYSAPI.h
# End Source File
# End Group
# End Target
# End Project
