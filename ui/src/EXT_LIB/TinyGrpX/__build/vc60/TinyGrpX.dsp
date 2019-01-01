# Microsoft Developer Studio Project File - Name="TinyGrpX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TinyGrpX - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TinyGrpX.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TinyGrpX.mak" CFG="TinyGrpX - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TinyGrpX - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TinyGrpX - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "TinyGrpX - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "TinyGrpX - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "TinyGrpX"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TinyGrpX - Win32 Release"

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

!ELSEIF  "$(CFG)" == "TinyGrpX - Win32 Debug"

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

!ELSEIF  "$(CFG)" == "TinyGrpX - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TinyGrpX___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "TinyGrpX___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../__Output/obj/TinyGrpX/Win32UnicodeDebug"
# PROP Intermediate_Dir "../../../../__Output/obj/TinyGrpX/Win32UnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../TinyGrpX/inc" /I "../../../A_Common/inc" /I "../../../A_Math/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../inc" /I "../../../../inc" /I "../../../A_Math/inc" /I "../../../../Ext_LIB/msgIP/Inc" /D "WIN32" /D "_DEBUG" /D "UNICODE" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__Output\lib\UnicodeDebug\TinyGrpX.lib"

!ELSEIF  "$(CFG)" == "TinyGrpX - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TinyGrpX___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "TinyGrpX___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../__Output/obj/TinyGrpX/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../../__Output/obj/TinyGrpX/Win32UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../../../TinyGrpX/inc" /I "../../../A_Common/inc" /I "../../../A_Math/inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../inc" /I "../../../../inc" /I "../../../A_Math/inc" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__Output\lib\UnicodeRelease\TinyGrpX.lib"

!ENDIF 

# Begin Target

# Name "TinyGrpX - Win32 Release"
# Name "TinyGrpX - Win32 Debug"
# Name "TinyGrpX - Win32 Unicode Debug"
# Name "TinyGrpX - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\Text\_STR_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Text\_STR_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\kernel\Canvas.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Text\FONT.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Text\FontDmp_E.h
# End Source File
# Begin Source File

SOURCE=..\..\src\kernel\G_kernel.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\GETPIXEL.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\LINE.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\LINE_H.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\LINE_V.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PUTPIXEL.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\RECT.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\RECT_F.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Text\STR.c
# End Source File
# Begin Source File

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\inc\TinyGrpX.h
# End Source File
# Begin Source File

SOURCE=.\inc\TinyGrpX_CFG.h
# End Source File
# End Group
# End Target
# End Project
