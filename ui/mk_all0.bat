@echo off

set _CURDIR=%~dp0
rem set ALL_OUTPUT_HOME=%_CURDIR%__Output2 

set BUILDS=src/EXT_LIB/A_Util src/EXT_LIB/A_Math src/EXT_LIB/A_Res src/EXT_LIB/GrpX src/EXT_LIB/freetype-2.3.9 
set BUILDS=%BUILDS% src/SYSAPI 
set BUILDS=%BUILDS% src/UI_Base/A_UIEng src/UI_Base/Component/BasePkg src/UI_Base/Component/ExtPkg
set BUILDS=%BUILDS% src/OEM/SYSAPI_OEM/Linux src/OEM/AUP_OEM/Linux
set BUILDS=%BUILDS% src/Apps/APP_SmartFx

for %%i in (%BUILDS%) do call %%i/mk0.bat %1 %2 %3 %4

set BUILDS=
set ALL_OUTPUT_HOME=

echo === all done ===
