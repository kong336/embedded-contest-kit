@echo off
set "VSDEVCMD=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
if not exist "%VSDEVCMD%" set "VSDEVCMD=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

if not exist "%VSDEVCMD%" (
    echo VsDevCmd.bat not found.
    echo Please check your Visual Studio Build Tools installation.
    pause
    exit /b 1
)

call "%VSDEVCMD%" -arch=x64 -host_arch=x64
echo.
echo MSVC environment is ready. You can use cl, nmake, cmake, and related tools here.
cmd /k
