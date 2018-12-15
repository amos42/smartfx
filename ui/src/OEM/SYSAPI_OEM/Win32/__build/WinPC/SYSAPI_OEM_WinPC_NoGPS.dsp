# Microsoft Developer Studio Project File - Name="SYSAPI_OEM_WINPC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SYSAPI_OEM_WINPC - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SYSAPI_OEM_WinPC_NoGPS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SYSAPI_OEM_WinPC_NoGPS.mak" CFG="SYSAPI_OEM_WINPC - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SYSAPI_OEM_WINPC - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SYSAPI_OEM_WINPC - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SYSAPI_OEM_WINPC - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SYSAPI_OEM_WINPC - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "SYSAPI_OEM_WINPC"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "../../../../OEM/B_NMEA/inc" /I "../../../../OEM/B_COMM/inc" /I "../../../../EXT_LIB/A_Util/inc" /I "../../../../EXT_LIB/B_NMEA/inc" /I "../../../../EXT_LIB/B_COMM/inc" /I "../../../../SYSAPI/inc" /I "../../../../SYSAPI/OEM_inc" /I "../../../../inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../../OEM/B_NMEA/inc" /I "../../../../OEM/B_COMM/inc" /I "../../../../EXT_LIB/A_Util/inc" /I "../../../../EXT_LIB/B_NMEA/inc" /I "../../../../EXT_LIB/B_COMM/inc" /I "../../../../SYSAPI/inc" /I "../../../../SYSAPI/OEM_inc" /I "../../../../inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SYSAPI_OEM_WINPC___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "SYSAPI_OEM_WINPC___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../__Output/obj/SYSAPI_OEM_WINPC_NoGps/Win32UnicodeDebug"
# PROP Intermediate_Dir "../../../../__Output/obj/SYSAPI_OEM_WINPC_NoGps/Win32UnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../A_Common/inc" /I "../A_UIENG/OEM_inc" /I "../A_UIENG/inc" /I "../GrpX/inc" /I "../A_Math/inc" /I "../A_Res/inc" /I "../../../AEEEmulAPI" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../../EXT_LIB/A_Util/inc" /I "../../../../EXT_LIB/B_NMEA/inc" /I "../../../../EXT_LIB/B_COMM/inc" /I "../../../../SYSAPI/inc" /I "../../../../SYSAPI/OEM_inc" /I "../../../../inc" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "UNICODE" /D "_UNICODE" /D "__NO_GPS_" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__Output\lib\UnicodeDebug\SYSAPI_OEM_WINPC_NoGps.lib"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SYSAPI_OEM_WINPC___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "SYSAPI_OEM_WINPC___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../__Output/obj/SYSAPI_OEM_WINPC/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../../__Output/obj/SYSAPI_OEM_WINPC/Win32UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../A_Common/inc" /I "../A_UIENG/OEM_inc" /I "../A_UIENG/inc" /I "../GrpX/inc" /I "../A_Math/inc" /I "../A_Res/inc" /I "../../../AEEEmulAPI" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../Win32/WinPC" /I "../../../../Core" /I "../../../../OEM/B_NMEA/inc" /I "../../../../OEM/B_COMM/inc" /I "../../../../EXT_LIB/A_Util/inc" /I "../../../../EXT_LIB/B_NMEA/inc" /I "../../../../EXT_LIB/B_COMM/inc" /I "../../../../SYSAPI/inc" /I "../../../../SYSAPI/OEM_inc" /I "../../../../inc" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /D "_UNICODE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__Output\lib\UnicodeRelease\SYSAPI_OEM_WINPC.lib"

!ENDIF 

# Begin Target

# Name "SYSAPI_OEM_WINPC - Win32 Release"
# Name "SYSAPI_OEM_WINPC - Win32 Debug"
# Name "SYSAPI_OEM_WINPC - Win32 Unicode Debug"
# Name "SYSAPI_OEM_WINPC - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "OEM_UI_Impl_VD"

# PROP Default_Filter ""
# End Group
# Begin Group "OEM_UI_Impl_Emul"

# PROP Default_Filter ""
# End Group
# Begin Group "OEM_UI_Impl_Win32"

# PROP Default_Filter ""
# Begin Group "GPS_OSAL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Win32\Common\osal_gps\comm.cpp

!IF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Release"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Debug"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\osal_gps\comm.h

!IF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Release"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Debug"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\osal_gps\commGPS.cpp

!IF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Release"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Debug"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\osal_gps\commGPS.h

!IF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Release"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Debug"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\osal_gps\commTMC.cpp

!IF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Release"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Debug"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\osal_gps\commTMC.h

!IF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Release"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Debug"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\osal_gps\gps.cpp

!IF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Release"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Debug"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\osal_gps\gps.h

!IF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Release"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Debug"

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SYSAPI_OEM_WINPC - Win32 Unicode Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Overload"

# PROP Default_Filter "*.c,*.cpp"
# Begin Source File

SOURCE=..\..\Win32\Common\overload\_new_delete_ovr.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Win32\Common\oem_sa_gps_win32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_sa_mem_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_sa_sound_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_sa_str_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_sa_task_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_sa_timer_win32.c
# End Source File
# End Group
# Begin Group "OEM_UI_Impl_WinCE4"

# PROP Default_Filter "*.c,*.cpp"
# End Group
# Begin Group "OEM_UI_Impl_WinPC"

# PROP Default_Filter "*.c,*.cpp,*.h"
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_sa_debug_winpc.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_sa_file_winpc.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_sa_key_winpc.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_sa_scr_winpc.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_sa_system_winpc.c
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
