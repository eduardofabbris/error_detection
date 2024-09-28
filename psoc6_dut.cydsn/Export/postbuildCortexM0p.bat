@REM This script allows a 3rd party IDE to use cymcuelftool.exe to perform
@REM the post processing that is necessary to update the *.elf file
@REM generated by the linker into a complete memory image to use in
@REM programming the PSoC.
@REM NOTE: This script is auto generated. Do not modify.
IF /I "%~5" NEQ "iar" GOTO not_iar
chdir /d "%~2\Export"
@IF %errorlevel% NEQ 0 EXIT /b %errorlevel%
move /Y "%~1" "%~3\%~n1_link.out"
@IF %errorlevel% NEQ 0 EXIT /b %errorlevel%
"%~2\Export\cymcuelftool.exe" --sign "%~3\%~n1_link.out" --output "%~1"
@IF %errorlevel% NEQ 0 EXIT /b %errorlevel%
EXIT

:not_iar
