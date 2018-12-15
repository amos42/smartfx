@echo off

set KEIL_HOME=c:/dev/keil_v5

set _DEVNAME_ORG=%DEVNAME%
set DEVNAME=SmartFX
set _USER_EXT_OP_ORG=%USER_EXT_OP%
set USER_EXT_OP=-D __UVISION_VERSION="515" -D _RTE_ -D STM32F746xx -D USE_HAL_DRIVER -D STM32F746xx -D USE_STM32746G_DISCO -D USE_IOEXPANDER -D __MICROLIB
set USER_EXT_OP=%USER_EXT_OP% --cpu Cortex-M7.fp.sp --fpu=FPv4-SP --apcs=interwork --split_sections --C99

set _CURDIR=%~dp0
rem set ALL_OUTPUT_HOME=%_CURDIR%__Output 

set BUILDS=src/EXT_LIB/A_Util src/EXT_LIB/A_Math src/EXT_LIB/A_Res src/EXT_LIB/GrpX src/EXT_LIB/freetype-2.3.9 
set BUILDS=%BUILDS% src/SYSAPI 
set BUILDS=%BUILDS% src/UI_Base/A_UIEng src/UI_Base/Component/BasePkg src/UI_Base/Component/ExtPkg
set BUILDS=%BUILDS% src/OEM/SYSAPI_OEM/SmartFX src/OEM/AUP_OEM/SmartFX
set BUILDS=%BUILDS% src/Apps/APP_SmartFx

for %%i in (%BUILDS%) do call %%i/mk.bat %1 %2 %3 %4

set BUILDS=

set USER_EXT_OP=%_USER_EXT_OP_ORG%
set _USER_EXT_OP_ORG=
set DEVNAME=%_DEVNAME_ORG%
set _DEVNAME_ORG=

echo === all done ===
