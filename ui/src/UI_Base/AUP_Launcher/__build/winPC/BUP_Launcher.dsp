# Microsoft Developer Studio Project File - Name="BUP_Launcher" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BUP_Launcher - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BUP_Launcher.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BUP_Launcher.mak" CFG="BUP_Launcher - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BUP_Launcher - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BUP_Launcher - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "BUP_Launcher - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "BUP_Launcher - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "BUP_Launcher"
# PROP Scc_LocalPath "..\..\..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BUP_Launcher - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../../../UI_BASE/A_UIEng/Lib/A_Common/inc" /I "../../../../UI_OEM" /I "../../../../UI_Base/A_UIEng/inc" /I "../../../../UI_BASE/ExtLib/A_Common/inc" /I "../../../../UI_BASE/A_UIEng/Lib/A_Util/inc" /I "../../../../Core" /I "../../../../Core/inc" /I "../../../../UI_BASE/Component/BasePkg/inc" /I "../../../../UI_BASE/Component/ExtPkg/inc" /I "../../../../UI_OEM/BUP_OEM/Lib/stl_lib" /I "../../../../UI_OEM/BUP_OEM/Lib/libsnd" /I "../../src/ui" /I "../../src/inc" /I "." /I "../../../../UI_BASE/ExtLib/A3D/inc" /I "../../src" /I "../../../../UI_Base/inc" /I "../../../../UI_Base/ExtLib/A_Math/inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "BUP_Launcher - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../../UI_BASE/A_UIEng/Lib/A_Common/inc" /I "../../../../UI_OEM" /I "../../../../UI_Base/A_UIEng/inc" /I "../../../../UI_BASE/ExtLib/A_Common/inc" /I "../../../../UI_BASE/A_UIEng/Lib/A_Util/inc" /I "../../../../Core" /I "../../../../Core/inc" /I "../../../../UI_BASE/Component/BasePkg/inc" /I "../../../../UI_BASE/Component/ExtPkg/inc" /I "../../../../UI_OEM/BUP_OEM/Lib/stl_lib" /I "../../../../UI_OEM/BUP_OEM/Lib/libsnd" /I "../../src/ui" /I "../../src/inc" /I "." /I "../../../../UI_BASE/ExtLib/A3D/inc" /I "../../src" /I "../../../../UI_Base/inc" /I "../../../../UI_Base/ExtLib/A_Math/inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "BUP_Launcher - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BUP_Launcher___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BUP_Launcher___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../__Output/obj/BUP_Launcher/Win32UnicodeDebug"
# PROP Intermediate_Dir "../../../../__Output/obj/BUP_Launcher/Win32UnicodeDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "." /I "../../src" /I "../../../../UI_Base/inc" /I "../../../../UI_Base/A_UIEng/inc" /I "../../../../UI_BASE/A_UIEng/Lib/A_Common/inc" /I "../../../../UI_BASE/A_UIEng/Lib/A_Util/inc" /I "../../../../Core" /I "../../../../Core/inc" /I "../../../../UI_BASE/Component/BasePkg/inc" /I "../../../../UI_BASE/Component/ExtPkg/inc" /I "../../../../UI_OEM" /I "../../../../UI_OEM/BUP_OEM/Lib/stl_lib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../../inc" /I "../../../../SYSAPI/inc" /I "../../../../EXT_LIB/A_Math/inc" /I "../../../../EXT_LIB/Grpx/inc" /I "../../../../EXT_LIB/AGG/inc" /I "../../../../UI_Base/A_UIEng/inc" /I "../../../../Core" /I "../../../../Core/inc" /I "../../../../UI_BASE/Component/BasePkg/inc" /I "../../../../UI_BASE/Component/ExtPkg/inc" /I "../../../../EXT_LIB/stl_lib" /I "../../../../EXT_Lib/libsnd/inc" /I "../../src/ui" /I "../../src/inc" /I "." /I "../../../../EXT_LIB/A3D/inc" /I "../../src" /I "../../../../UI_Base/inc" /I "../../../../OEM/SYSAPI_OEM/Win32/WinPC/OSAL/inc" /I "../../../../EXT_LIB/Globe3D/inc" /I "../../../../EXT_LIB/Landmark3D/inc" /I "../../../../EXT_LIB/B_NMEA/inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FR /YX"tornado.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib OCLogLib.lib /nologo /subsystem:windows /map:"../../../../__Output/bin/Win32UnicodeDebug/BUP_Launcher.map" /debug /machine:I386 /nodefaultlib:"MSVCRTD" /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /out:"../../../../__Output/bin/Win32UnicodeDebug/BUP_Launcher.exe" /libpath:"../../../../__Output/lib/UnicodeDebug/"
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
TargetDir=\Project\bns\SLP\__Output\bin\Win32UnicodeDebug
WkspName=TornadoPC
TargetName=BUP_Launcher
SOURCE="$(InputPath)"
PostBuild_Cmds=copy $(TargetDir)\$(TargetName).exe $(TargetDir)\$(WkspName).exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "BUP_Launcher - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BUP_Launcher___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "BUP_Launcher___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../__Output/obj/BUP_Launcher/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../../__Output/obj/BUP_Launcher/Win32UnicodeRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "." /I "../../src" /I "../../../../UI_Base/inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../../../inc" /I "../../../../SYSAPI/inc" /I "../../../../EXT_LIB/A_Math/inc" /I "../../../../EXT_LIB/Grpx/inc" /I "../../../../EXT_LIB/AGG/inc" /I "../../../../EXT_LIB/Globe3D/inc" /I "../../../../UI_Base/A_UIEng/inc" /I "../../../../Core" /I "../../../../Core/inc" /I "../../../../UI_BASE/Component/BasePkg/inc" /I "../../../../UI_BASE/Component/ExtPkg/inc" /I "../../../../EXT_LIB/stl_lib" /I "../../../../EXT_Lib/libsnd/inc" /I "../../src/ui" /I "../../src/inc" /I "." /I "../../../../EXT_LIB/A3D/inc" /I "../../src" /I "../../../../UI_Base/inc" /I "../../../../OEM/SYSAPI_OEM/Win32/WinPC/OSAL/inc" /I "../../../../EXT_LIB/Landmark3D/inc" /I "../../../../EXT_LIB/B_NMEA/inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib rpLib.lib bgmLib.lib zlib.lib freetype239MT.lib winmm.lib bnfsnd.lib libmad.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"MSVCRTD" /nodefaultlib:"MSVCRT" /nodefaultlib:"LIBCD" /nodefaultlib:"LIBC" /out:"../../../../__Output/bin/Win32UnicodeRelease/BUP_Launcher.exe" /libpath:"../../../../__Output/lib/UnicodeRelease/"

!ENDIF 

# Begin Target

# Name "BUP_Launcher - Win32 Release"
# Name "BUP_Launcher - Win32 Debug"
# Name "BUP_Launcher - Win32 Unicode Debug"
# Name "BUP_Launcher - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BUP_Launcher.cpp
# End Source File
# Begin Source File

SOURCE=.\BUP_Launcher.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\EntryPtr.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\BUP_Launcher.ico
# End Source File
# Begin Source File

SOURCE=.\small.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
