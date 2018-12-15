@echo off
if "%MSYS_HOME%"=="" set MSYS_HOME=C:/DEV/msys64
set MINGW_HOME=%MSYS_HOME%/mingw64

set PATH0=%PATH%
set PATH=%MSYS_HOME%/usr/bin;%MINGW_HOME%/bin;%PATH%

set CC0=%CC%
set CXX0=%CXX%
set AR0=%AR%
rem set CC=mingw64-gcc
rem set CXX=mingw64-g++
rem set AR=mingw64-ar

set PROJ_PATH=%~dp0

pushd %PROJ_PATH%
set PROJ_PATH=.

if "%ALL_OUTPUT_HOME%"=="" (
  set OUTPUT_HOME=%PROJ_PATH%/../../../../__Output
) else (
  set OUTPUT_HOME=%ALL_OUTPUT_HOME%
)

set DEVNAME=
set USER_EXT_OP=
mingw32-make.exe -f %PROJ_PATH%/__build/GNU_make/makefile -C %PROJ_PATH% %1 %2 %3 %4

popd

set CC=%CC0%
set CXX=%CXX0%
set AR=%AR0%

set PATH=%PATH0%
set PATH0=
