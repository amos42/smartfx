# Microsoft Developer Studio Project File - Name="GrpX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=GrpX - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GrpX.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GrpX.mak" CFG="GrpX - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GrpX - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GrpX - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "GrpX - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "GrpX - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "GrpX"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GrpX - Win32 Release"

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

!ELSEIF  "$(CFG)" == "GrpX - Win32 Debug"

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

!ELSEIF  "$(CFG)" == "GrpX - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GrpX___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "GrpX___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../__Output/obj/GrpX/Win32UnicodeDebug"
# PROP Intermediate_Dir "../../../../__Output/obj/GrpX/Win32UnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../GrpX/inc" /I "../../../A_Common/inc" /I "../../../A_Math/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../inc" /I "../../../../inc" /I "../../../A_Math/inc" /I "../../../../Ext_LIB/msgIP/Inc" /D "WIN32" /D "_DEBUG" /D "UNICODE" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__Output\lib\UnicodeDebug\GrpX.lib"

!ELSEIF  "$(CFG)" == "GrpX - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GrpX___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "GrpX___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../__Output/obj/GrpX/Win32UnicodeRelease"
# PROP Intermediate_Dir "../../../../__Output/obj/GrpX/Win32UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../../../GrpX/inc" /I "../../../A_Common/inc" /I "../../../A_Math/inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../inc" /I "../../../../inc" /I "../../../A_Math/inc" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\__Output\lib\UnicodeRelease\GrpX.lib"

!ENDIF 

# Begin Target

# Name "GrpX - Win32 Release"
# Name "GrpX - Win32 Debug"
# Name "GrpX - Win32 Unicode Debug"
# Name "GrpX - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\Graphic\_AA_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_AA_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_AB_int.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_AB_table.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_AB_table.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_ClipPoly_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_ClipPoly_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_CVX2_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_CVX2_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_CVX_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_CVX_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\_IMG_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\_IMG_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_LIN2_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_LIN2_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_LIN_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_LIN_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_LIN_Int.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_OL_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_OL_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_PLY2_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_PLY2_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_PLY_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_PLY_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_QUD_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_QUD_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Text\_STR_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Text\_STR_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_TRI_func.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_TRI_func.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\_TRI_Int.h
# End Source File
# Begin Source File

SOURCE=..\..\src\kernel\Canvas.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\CIRC_FAA.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\CIRC_FT.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\CIRC_T.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic2\CIRCL_PB.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\CIRCLE.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\CIRCLE_F.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\CLRSCR.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\ELPS.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\ELPS_F.c
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

SOURCE=..\..\src\Bitmap\GETIMG.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\GETPIXEL.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\HLINE_TX.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\IMG_A.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\IMG_D.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\IMG_GL_P.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\IMG_GL_R.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\IMG_LOW.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\IMG_R.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\IMG_RA.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\IMG_RT.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\IMG_S.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\IMG_T.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\IMG_TS.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\LINE.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\LINE_AA.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\LINE_C.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\LINE_H.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\LINE_HT.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic2\LINE_PB.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\LINE_V.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\LINE_VT.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\LINE_W.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\LOADIMG.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\LOADMSK.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic2\PB_LOW.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PIXEL_AA.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic2\PIXEL_PB.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PLY_F.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PLY_FAA.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PLY_FB.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PLY_L.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PLY_LW.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic2\PLY_PB.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PPLY_F.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PPLY_FAA.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PPLY_FB.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PPLY_L.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PPLY_LW.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic2\PPLY_PB.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\PUTPIXEL.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\Quad.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\Quad_F.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\Quad_FAA.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic2\QUAD_PB.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\RECT.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\RECT_F.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\RECT_FT.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\RECT_FX.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic2\RECT_PB.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\RECT_T.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Bitmap\RECT_TX.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\RECT_W.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Text\STR.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Text\STR_R.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Text\STRBM.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\TRI.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\TRI_F.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic\TRI_FAA.c
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphic2\TRI_PB.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\inc\GrpX.h
# End Source File
# Begin Source File

SOURCE=.\inc\GrpX_CFG.h
# End Source File
# End Group
# End Target
# End Project
