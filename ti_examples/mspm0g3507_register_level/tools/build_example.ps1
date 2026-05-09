param(
    [ValidateSet("blink_led", "uart_hello")]
    [string]$Example = "blink_led"
)

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))
$projectRoot = Join-Path $repoRoot "ti_examples\mspm0g3507_register_level"
$includeDir = Join-Path $projectRoot "include"
$startupSource = Join-Path $projectRoot "src\startup_mspm0g350x_ticlang.c"
$sourceFile = Join-Path $projectRoot ("src\" + $Example + ".c")
$linkerCommandFile = Join-Path $projectRoot "linker\mspm0g3507.cmd"
$buildDir = Join-Path $projectRoot ("build\" + $Example)

$compilerRoot = "D:\TI_CCS\ccs\tools\compiler\ti-cgt-armllvm_4.0.4.LTS"
$tiarmclang = Join-Path $compilerRoot "bin\tiarmclang.exe"

foreach ($path in @($tiarmclang, $includeDir, $startupSource, $sourceFile, $linkerCommandFile)) {
    if (-not (Test-Path $path)) {
        throw "Required TI path was not found: $path"
    }
}

New-Item -ItemType Directory -Force $buildDir | Out-Null
$outFile = Join-Path $buildDir ($Example + ".out")
$mapFile = Join-Path $buildDir ($Example + ".map")

& $tiarmclang `
    -mcpu=cortex-m0plus `
    -mthumb `
    -O1 `
    -g `
    -D__MSPM0G3507__ `
    -I $includeDir `
    $startupSource `
    $sourceFile `
    $linkerCommandFile `
    -Xlinker -m `
    -Xlinker $mapFile `
    -o $outFile

if ($LASTEXITCODE -ne 0) {
    throw "tiarmclang failed with exit code $LASTEXITCODE"
}

Write-Output ("Built example: " + $Example)
Write-Output ("Output file : " + $outFile)
Write-Output ("Map file    : " + $mapFile)
