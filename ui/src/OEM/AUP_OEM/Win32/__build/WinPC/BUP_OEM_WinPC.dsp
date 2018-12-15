# Microsoft Developer Studio Project File - Name="BUP_OEM_WINPC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=BUP_OEM_WINPC - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BUP_OEM_WinPC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BUP_OEM_WinPC.mak" CFG="BUP_OEM_WINPC - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BUP_OEM_WINPC - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BUP_OEM_WINPC - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BUP_OEM_WINPC - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BUP_OEM_WINPC - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "BUP_OEM_WINPC"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BUP_OEM_WINPC - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\..\EXT_LIB\lib\A_Util\inc" /I "..\..\..\SYSAPI_OEM\Symbian\OSAL\inc" /I "../../../../SYSAPI/inc" /I "../../../../UI_Base/A_UIEng/inc" /I "../../../../UI_Base/A_UIEng/OEM_inc" /I "../../../../UI_Base/ExtLib/A_Util/inc" /I "../../../../UI_Base/ExtLib/A_Math/inc" /I "../../../SYSAPI_OEM/Win32/WinPC/OSAL/inc" /I "../../../../inc" /I "../../../../EXT_LIB/A_Math/inc" /I "../../../../EXT_LIB/A_Res/inc" /I "../../../../EXT_LIB/GrpX/inc" /I "../../../../EXT_LIB/FreeTypeLib/include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BUP_OEM_WINPC - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\..\EXT_LIB\lib\A_Util\inc" /I "..\..\..\SYSAPI_OEM\Symbian\OSAL\inc" /I "../../../../SYSAPI/inc" /I "../../../../UI_Base/A_UIEng/inc" /I "../../../../UI_Base/A_UIEng/OEM_inc" /I "../../../../UI_Base/ExtLib/A_Util/inc" /I "../../../../UI_Base/ExtLib/A_Math/inc" /I "../../../SYSAPI_OEM/Win32/WinPC/OSAL/inc" /I "../../../../inc" /I "../../../../EXT_LIB/A_Math/inc" /I "../../../../EXT_LIB/A_Res/inc" /I "../../../../EXT_LIB/GrpX/inc" /I "../../../../EXT_LIB/FreeTypeLib/include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BUP_OEM_WINPC - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BUP_OEM_WINPC___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "BUP_OEM_WINPC___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../__Output/obj/BUP_OEM_WINPC/Win32UnicodeDebug"
# PROP Intermediate_Dir "../../../../__Output/obj/BUP_OEM_WINPC/Win32UnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../A_Common/inc" /I "../A_UIENG/OEM_inc" /I "../A_UIENG/inc" /I "../GrpX/inc" /I "../A_Math/inc" /I "../A_Res/inc" /I "../../../AEEEmulAPI" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\SYSAPI\inc" /I "..\..\..\SYSAPI_OEM\Win32\WinPC\OSAL\inc" /I "../../../../SYSAPI/inc" /I "../../../../UI_Base/A_UIEng/inc" /I "../../../../UI_Base/A_UIEng/OEM_inc" /I "../../../../UI_Base/ExtLib/A_Util/inc" /I "../../../../UI_Base/ExtLib/A_Math/inc" /I "../../../SYSAPI_OEM/Win32/WinPC/OSAL/inc" /I "../../../../inc" /I "../../../../EXT_LIB/A_Math/inc" /I "../../../../EXT_LIB/A_Res/inc" /I "../../../../EXT_LIB/GrpX/inc" /I "../../../../EXT_LIB/FreeType-2.3.9/include" /I "../../../../EXT_LIB/B_NMEA/inc" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "UNICODE" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__Output\lib\UnicodeDebug\BUP_OEM_WINPC.lib"

!ELSEIF  "$(CFG)" == "BUP_OEM_WINPC - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BUP_OEM_WINPC___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "BUP_OEM_WINPC___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../__Output/obj/BUP_OEM_WINPC/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../../__Output/obj/BUP_OEM_WINPC/Win32UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../A_Common/inc" /I "../A_UIENG/OEM_inc" /I "../A_UIENG/inc" /I "../GrpX/inc" /I "../A_Math/inc" /I "../A_Res/inc" /I "../../../AEEEmulAPI" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\..\SYSAPI\inc" /I "..\..\..\SYSAPI_OEM\Win32\WinPC\OSAL\inc" /I "../../../../SYSAPI/inc" /I "../../../../UI_Base/A_UIEng/inc" /I "../../../../UI_Base/A_UIEng/OEM_inc" /I "../../../../UI_Base/ExtLib/A_Util/inc" /I "../../../../UI_Base/ExtLib/A_Math/inc" /I "../../../SYSAPI_OEM/Win32/WinPC/OSAL/inc" /I "../../../../inc" /I "../../../../EXT_LIB/A_Math/inc" /I "../../../../EXT_LIB/A_Res/inc" /I "../../../../EXT_LIB/GrpX/inc" /I "../../../../EXT_LIB/FreeTypeLib/include" /I "../../../../EXT_LIB/B_NMEA/inc" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__Output\lib\UnicodeRelease\BUP_OEM_WINPC.lib"

!ENDIF 

# Begin Target

# Name "BUP_OEM_WINPC - Win32 Release"
# Name "BUP_OEM_WINPC - Win32 Debug"
# Name "BUP_OEM_WINPC - Win32 Unicode Debug"
# Name "BUP_OEM_WINPC - Win32 Unicode Release"
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
# Begin Source File

SOURCE=..\..\Win32\Common\oem_ui_debug_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_ui_file_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_ui_gdi_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_ui_key_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_ui_mem_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_ui_proc_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_ui_res_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_ui_str_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\Common\oem_ui_timer_win32.c
# End Source File
# End Group
# Begin Group "OEM_UI_Impl_WinCE4"

# PROP Default_Filter "*.c,*.cpp"
# End Group
# Begin Group "OEM_UI_Impl_WinPC"

# PROP Default_Filter "*.c,*.cpp,*.h"
# Begin Group "oem_ime"

# PROP Default_Filter "*.c,*.h"
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_ime\HanTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_ime\HanTable.h
# End Source File
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_ime\KorAutomata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_ime\KorAutomata.h
# End Source File
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_ime\oem_ui_ime_han.c.h
# End Source File
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_ime\oem_ui_ime_multitab.c.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_ui_font_winpc.c
# End Source File
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_ui_ime_winpc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win32\WinPC\oem_ui_system_winpc.c
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
