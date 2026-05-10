param(
    [int]$PortStart = 49620,
    [int]$PortEnd = 49629
)

$foundPort = $null
$health = $null

for ($port = $PortStart; $port -le $PortEnd; $port++) {
    try {
        $response = Invoke-RestMethod -Uri "http://127.0.0.1:$port/health" -TimeoutSec 2 -ErrorAction Stop
        if ($response.service -eq "easyeda-bridge") {
            $foundPort = $port
            $health = $response
            break
        }
    } catch {
        continue
    }
}

if ($null -eq $foundPort) {
    Write-Host "No EasyEDA bridge found on ports $PortStart-$PortEnd."
    Write-Host "Install and load the Run API Gateway extension, then start the bridge again."
    exit 1
}

Write-Host "EasyEDA bridge found on port $foundPort."
Write-Host ("Health: " + ($health | ConvertTo-Json -Compress))

try {
    $windows = Invoke-RestMethod -Uri "http://127.0.0.1:$foundPort/eda-windows" -TimeoutSec 2 -ErrorAction Stop
    Write-Host ("EDA windows: " + ($windows | ConvertTo-Json -Compress))
} catch {
    Write-Host "Bridge is alive, but /eda-windows is not available yet."
}
