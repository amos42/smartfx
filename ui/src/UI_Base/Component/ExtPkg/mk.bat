@echo off
if "%MSYS_HOME%"=="" set MSYS_HOME=C:/DEV/msys64
set MINGW_HOME=%MSYS_HOME%/mingw64

set PATH0=%PATH%
set PATH=%MSYS_HOME%/usr/bin;%MINGW_HOME%/bin;%PATH%

if "%KEIL_HOME%"=="" set KEIL_HOME=C:/Keil_v5

rem set CC0=%CC%
rem set CXX0=%CXX%
rem set AR0=%AR%
set CC=%KEIL_HOME%/ARM/ARMCC/bin/armcc.exe
set CXX=%KEIL_HOME%/ARM/ARMCC/bin/armcc.exe
set AR=%KEIL_HOME%/ARM/ARMCC/bin/armar.exe

set PROJ_PATH=%~dp0

pushd %PROJ_PATH%
set PROJ_PATH=.

if "%ALL_OUTPUT_HOME%"=="" (
  set OUTPUT_HOME=%PROJ_PATH%/../../../../__Output
) else (
  set OUTPUT_HOME=%ALL_OUTPUT_HOME%
)

if "%DEVNAME%"=="" set DEVNAME=SmartFX
set USER_EXT_OP=-D __UVISION_VERSION="515" -D _RTE_ -D STM32F746xx -D USE_HAL_DRIVER -D STM32F746xx -D USE_STM32746G_DISCO -D USE_IOEXPANDER -D __MICROLIB
set USER_EXT_OP=%USER_EXT_OP% --cpu Cortex-M7.fp.sp --fpu=FPv4-SP --apcs=interwork --split_sections --C99

mingw32-make.exe -f %PROJ_PATH%/__build/ARM_Keil/makefile -C %PROJ_PATH% %1 %2 %3 %4

popd

set CC=%CC0%
set CXX=%CXX0%
set AR=%AR0%

set PATH=%PATH0%
set PATH0=
