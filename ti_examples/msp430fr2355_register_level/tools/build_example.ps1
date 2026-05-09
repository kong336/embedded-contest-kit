param(
    [ValidateSet("blink_led", "uart_hello")]
    [string]$Example = "blink_led"
)

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))
$projectRoot = Join-Path $repoRoot "ti_examples\msp430fr2355_register_level"
$sourceFile = Join-Path $projectRoot ("src\" + $Example + ".c")
$buildDir = Join-Path $projectRoot ("build\" + $Example)

$compilerRoot = "D:\TI_CCS\ccs\tools\compiler\ti-cgt-msp430_21.6.1.LTS"
$compilerLibDir = Join-Path $compilerRoot "lib"
$deviceInclude = "D:\TI_CCS\ccs\ccs_base\msp430\include"
$compilerInclude = Join-Path $compilerRoot "include"
$runtimeLib = Join-Path $compilerRoot "lib\rts430x_lc_sd_eabi.lib"
$linkerCommandFile = Join-Path $deviceInclude "lnk_msp430fr2355.cmd"

$cl430 = Join-Path $compilerRoot "bin\cl430.exe"
$lnk430 = Join-Path $compilerRoot "bin\lnk430.exe"

foreach ($path in @($cl430, $lnk430, $deviceInclude, $compilerInclude, $runtimeLib, $linkerCommandFile, $sourceFile)) {
    if (-not (Test-Path $path)) {
        throw "Required TI path was not found: $path"
    }
}

New-Item -ItemType Directory -Force $buildDir | Out-Null
$objFile = Join-Path $buildDir ($Example + ".obj")
$outFile = Join-Path $buildDir ($Example + ".out")
$mapFile = Join-Path $buildDir ($Example + ".map")

& $cl430 `
    --compile_only `
    --silicon_version=mspx `
    --define=__MSP430FR2355__ `
    --include_path=$deviceInclude `
    --include_path=$compilerInclude `
    --obj_directory=$buildDir `
    $sourceFile

if ($LASTEXITCODE -ne 0) {
    throw "cl430 failed with exit code $LASTEXITCODE"
}

& $lnk430 `
    -c `
    --silicon_version=mspx `
    -o $outFile `
    -m $mapFile `
    --search_path=$compilerLibDir `
    --search_path=$deviceInclude `
    $runtimeLib `
    $objFile `
    $linkerCommandFile

if ($LASTEXITCODE -ne 0) {
    throw "lnk430 failed with exit code $LASTEXITCODE"
}

Write-Output ("Built example: " + $Example)
Write-Output ("Object file : " + $objFile)
Write-Output ("Output file : " + $outFile)
Write-Output ("Map file    : " + $mapFile)
