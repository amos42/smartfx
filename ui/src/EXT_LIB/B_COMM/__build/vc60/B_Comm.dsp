# Microsoft Developer Studio Project File - Name="B_COMM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=B_COMM - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "B_Comm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "B_Comm.mak" CFG="B_COMM - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "B_COMM - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "B_COMM - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "B_COMM - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "B_COMM - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "B_COMM"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "B_COMM - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "../../../SYSAPI/inc" /I "../../../../SYSAPI/inc" /I "../../inc" /I "../../../../inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "B_COMM - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../SYSAPI/inc" /I "../../../../SYSAPI/inc" /I "../../inc" /I "../../../../inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "B_COMM - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "B_COMM___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "B_COMM___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../__Output/obj/B_COMM/Win32UnicodeDebug"
# PROP Intermediate_Dir "../../../../__Output/obj/B_COMM/Win32UnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../B_COMM/inc" /I "../../../A_Common/inc" /I "../../../A_Math/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../../SYSAPI/inc" /I "../../inc" /I "../../../../inc" /I "../../../B_NMEA/inc" /D "WIN32" /D "_DEBUG" /D "UNICODE" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__Output\lib\UnicodeDebug\B_COMM.lib"

!ELSEIF  "$(CFG)" == "B_COMM - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "B_COMM___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "B_COMM___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../__Output/obj/B_COMM/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../../__Output/obj/B_COMM/Win32UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../../../B_COMM/inc" /I "../../../A_Common/inc" /I "../../../A_Math/inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../../SYSAPI/inc" /I "../../../../SYSAPI/inc" /I "../../inc" /I "../../../../inc" /I "../../../B_NMEA/inc" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__Output\lib\UnicodeRelease\B_COMM.lib"

!ENDIF 

# Begin Target

# Name "B_COMM - Win32 Release"
# Name "B_COMM - Win32 Debug"
# Name "B_COMM - Win32 Unicode Debug"
# Name "B_COMM - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\_file.c
# End Source File
# Begin Source File

SOURCE=..\..\src\_file.h
# End Source File
# Begin Source File

SOURCE=..\..\src\_heap.c
# End Source File
# Begin Source File

SOURCE=..\..\src\_heap.h
# End Source File
# Begin Source File

SOURCE=..\..\src\_queue.c
# End Source File
# Begin Source File

SOURCE=..\..\src\_queue.h
# End Source File
# Begin Source File

SOURCE=..\..\src\_thread.c
# End Source File
# Begin Source File

SOURCE=..\..\src\_thread.h
# End Source File
# Begin Source File

SOURCE=..\..\src\B_Comm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\B_Comm_Log.cpp.inl
# End Source File
# Begin Source File

SOURCE=..\..\src\B_Comm_port.cpp.inl
# End Source File
# Begin Source File

SOURCE=..\..\src\B_Comm_Sim.cpp.inl
# End Source File
# Begin Source File

SOURCE=..\..\src\Comm_Data.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Comm_Def.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\B_COMM.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\BC_Def.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\BC_Types.h
# End Source File
# End Group
# End Target
# End Project
