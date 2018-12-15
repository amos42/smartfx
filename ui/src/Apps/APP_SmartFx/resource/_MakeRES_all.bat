@echo off

del .\SFX_IMG\*.img

for /R SFX_IMG %%a in (*.bmp) do "../../../../__TOOLS/__Output/bin/img_conv.exe" 2 %%a * * 16 3

pushd .
cd .\SFX_IMG
"../../../../../__TOOLS/__Output/bin/MAKERES.exe" SFX_UI.rc
popd
copy ".\SFX_IMG.\UI.res" .

"../../../../__TOOLS/__Output/bin/DUMP.exe" UI.res UI_dump.c

pause

