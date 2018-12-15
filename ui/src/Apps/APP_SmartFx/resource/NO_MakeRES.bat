@echo off

pushd .
cd .\SFX_IMG
"../../../../../__TOOLS/__Output/bin/MAKERES.exe" SFX_UI.rc
popd
copy ".\SFX_IMG.\UI.res" .

rem "../../../../__TOOLS/__Output/bin/DUMP.exe" UI.res UI_dump.c

pause

