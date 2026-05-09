$llvmBin = "C:\Program Files\LLVM\bin"
$gccBin = "$env:LOCALAPPDATA\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin"
$vsBuildTools = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
$vsCommunity = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

if (Test-Path $llvmBin) {
    $env:Path = "$llvmBin;$env:Path"
}

if (Test-Path $gccBin) {
    $env:Path = "$gccBin;$env:Path"
}

Write-Host "clang location: $llvmBin"
Write-Host "gcc location:   $gccBin"
Write-Host ""

if (Test-Path (Join-Path $llvmBin "clang.exe")) {
    & (Join-Path $llvmBin "clang.exe") --version | Select-Object -First 1
}

if (Test-Path (Join-Path $gccBin "gcc.exe")) {
    & (Join-Path $gccBin "gcc.exe") --version | Select-Object -First 1
}

Write-Host ""
Write-Host "MSVC shell entry:"
if (Test-Path $vsBuildTools) {
    Write-Host "cmd /k `"$vsBuildTools`" -arch=x64 -host_arch=x64"
} elseif (Test-Path $vsCommunity) {
    Write-Host "cmd /k `"$vsCommunity`" -arch=x64 -host_arch=x64"
} else {
    Write-Host "VsDevCmd.bat not found."
}
