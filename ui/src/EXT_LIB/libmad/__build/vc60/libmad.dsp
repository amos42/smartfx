# Microsoft Developer Studio Project File - Name="libmad" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libmad - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libmad.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libmad.mak" CFG="libmad - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libmad - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libmad - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libmad - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libmad - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "libmad"
# PROP Scc_LocalPath "..\..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libmad - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "../../../../../SYSAPI/inc" /I "../inc" /I "../../inc" /I "../../../../SYSAPI/inc" /I "../../../../inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libmad - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../../../SYSAPI/inc" /I "../inc" /I "../../inc" /I "../../../../SYSAPI/inc" /I "../../../../inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libmad - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../__Output/obj/libmad/Win32Unicode Debug"
# PROP Intermediate_Dir "../../../../__Output/obj/libmad/Win32Unicode Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../inc" /I "../../../../SYSAPI/inc" /I "../../../../inc" /I "../../../B_NMEA/inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "_LIB" /D "UNICODE" /D "FPM_INTEL" /D "OPT_ACCURACY" /D "HAVE_CONFIG_H" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../__Output/lib/UnicodeDebug/libmad.lib"

!ELSEIF  "$(CFG)" == "libmad - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../__Output/obj/libmad/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../../__Output/obj/libmad/Win32UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../inc" /I "../../inc" /I "../../../../SYSAPI/inc" /I "../../../../inc" /I "../../../B_NMEA/inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "_LIB" /D "UNICODE" /D "FPM_INTEL" /D "OPT_ACCURACY" /D "HAVE_CONFIG_H" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../__Output/lib/UnicodeRelease/libmad.lib"

!ENDIF 

# Begin Target

# Name "libmad - Win32 Release"
# Name "libmad - Win32 Debug"
# Name "libmad - Win32 Unicode Debug"
# Name "libmad - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\bit.c
# End Source File
# Begin Source File

SOURCE=..\..\src\decoder.c
# End Source File
# Begin Source File

SOURCE=..\..\src\fixed.c
# End Source File
# Begin Source File

SOURCE=..\..\src\frame.c
# End Source File
# Begin Source File

SOURCE=..\..\src\huffman.c
# End Source File
# Begin Source File

SOURCE=..\..\src\layer12.c
# End Source File
# Begin Source File

SOURCE=..\..\src\layer3.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libmad.c
# End Source File
# Begin Source File

SOURCE=..\..\src\stream.c
# End Source File
# Begin Source File

SOURCE=..\..\src\synth.c
# End Source File
# Begin Source File

SOURCE=..\..\src\timer.c
# End Source File
# Begin Source File

SOURCE=..\..\src\version.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\bit.h
# End Source File
# Begin Source File

SOURCE=..\..\src\config.h
# End Source File
# Begin Source File

SOURCE=..\..\src\decoder.h
# End Source File
# Begin Source File

SOURCE=..\..\src\fixed.h
# End Source File
# Begin Source File

SOURCE=..\..\src\frame.h
# End Source File
# Begin Source File

SOURCE=..\..\src\global.h
# End Source File
# Begin Source File

SOURCE=..\..\src\huffman.h
# End Source File
# Begin Source File

SOURCE=..\..\src\layer12.h
# End Source File
# Begin Source File

SOURCE=..\..\src\layer3.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libmad.h
# End Source File
# Begin Source File

SOURCE=..\..\src\mad.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stream.h
# End Source File
# Begin Source File

SOURCE=..\..\src\synth.h
# End Source File
# Begin Source File

SOURCE=..\..\src\timer.h
# End Source File
# Begin Source File

SOURCE=..\..\src\version.h
# End Source File
# End Group
# Begin Group "OEM"

# PROP Default_Filter "*.c,*.h"
# Begin Source File

SOURCE=..\..\OEM\SndHeapManager.c
# End Source File
# Begin Source File

SOURCE=..\..\OEM\SndHeapManager.h
# End Source File
# End Group
# End Target
# End Project
