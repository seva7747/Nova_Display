# Build and flash the one active display state. Usage: .\flash.ps1 [COM3]
# Windows counterpart to NovaLive/flash.sh (that one's for macOS - different
# IDF export script and no /dev/cu.* ports here).
param(
    [string]$Port
)

$Here = Split-Path -Parent $MyInvocation.MyCommand.Path
$IdfDir = if ($env:IDF_PATH) { $env:IDF_PATH } else { "C:\esp\esp-idf-v5.5" }

if (-not (Test-Path "$IdfDir\export.ps1")) {
    Write-Error "ESP-IDF not found at $IdfDir (set `$env:IDF_PATH` or run the installer first)."
    exit 1
}

. "$IdfDir\export.ps1" | Out-Null

if (-not $Port) {
    $Port = (Get-CimInstance -ClassName Win32_PnPEntity | Where-Object { $_.Name -match "\(COM\d+\)" } |
             ForEach-Object { if ($_.Name -match "\((COM\d+)\)") { $Matches[1] } } | Select-Object -First 1)
}
if (-not $Port) {
    Write-Error "No board detected. Plug it in, or pass the port: .\flash.ps1 COM3"
    exit 1
}

Set-Location $Here
Write-Host "==> building and flashing to $Port (first build downloads components and takes a few minutes)"
idf.py -p $Port -b 460800 flash
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
Write-Host "==> done. Watch it with: idf.py -p $Port monitor"
