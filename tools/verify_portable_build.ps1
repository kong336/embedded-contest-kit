param(
    [ValidateSet("gcc", "clang", "cl")]
    [string]$Compiler = "clang"
)

$repoRoot = Split-Path -Parent $PSScriptRoot
$buildRoot = Join-Path $repoRoot ".build\$Compiler"
$genericInclude = Join-Path $repoRoot "generic_c\include"
$genericSources = Get-ChildItem (Join-Path $repoRoot "generic_c\src") -Filter *.c | ForEach-Object { $_.FullName }
$exampleSources = Get-ChildItem (Join-Path $repoRoot "examples") -Filter *.c | ForEach-Object { $_.FullName }
$allSources = @($genericSources + $exampleSources)

New-Item -ItemType Directory -Force -Path $buildRoot | Out-Null

switch ($Compiler) {
    "gcc" {
        $compilerPath = "$env:LOCALAPPDATA\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\gcc.exe"
        foreach ($file in $allSources) {
            $out = Join-Path $buildRoot (([System.IO.Path]::GetFileNameWithoutExtension($file)) + ".o")
            & $compilerPath -std=c11 -Wall -Wextra -Werror -I $genericInclude -c $file -o $out
            if ($LASTEXITCODE -ne 0) {
                exit $LASTEXITCODE
            }
        }
    }
    "clang" {
        $compilerPath = "C:\Program Files\LLVM\bin\clang.exe"
        foreach ($file in $allSources) {
            $out = Join-Path $buildRoot (([System.IO.Path]::GetFileNameWithoutExtension($file)) + ".o")
            & $compilerPath -std=c11 -Wall -Wextra -Werror -I $genericInclude -c $file -o $out
            if ($LASTEXITCODE -ne 0) {
                exit $LASTEXITCODE
            }
        }
    }
    "cl" {
        $vsDevCmd = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
        if (-not (Test-Path $vsDevCmd)) {
            $vsDevCmd = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
        }

        foreach ($file in $allSources) {
            $out = Join-Path $buildRoot (([System.IO.Path]::GetFileNameWithoutExtension($file)) + ".obj")
            $cmd = 'call "' + $vsDevCmd + '" -arch=x64 -host_arch=x64 >nul && cl /nologo /W4 /WX /I "' + $genericInclude + '" /c "' + $file + '" /Fo"' + $out + '"'
            cmd /c $cmd
            if ($LASTEXITCODE -ne 0) {
                exit $LASTEXITCODE
            }
        }
    }
}

Write-Host "$Compiler portable build verification passed."
