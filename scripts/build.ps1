param(
    [string]$ArduinoCli = 'arduino-cli',
    [string]$ConfigFile = '',
    [switch]$Install
)
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot
# Optional machine-specific setup, excluded from Git. Never require a PATH edit.
$localSettings = Join-Path $repoRoot 'build.local.psd1'
$mappedDrive = $null
$settings = $null
if (Test-Path -LiteralPath $localSettings) {
    $settings = Import-PowerShellDataFile -LiteralPath $localSettings
    if (!$PSBoundParameters.ContainsKey('ArduinoCli')) { $ArduinoCli = $settings.ArduinoCli }
    if (!$PSBoundParameters.ContainsKey('ConfigFile')) { $ConfigFile = $settings.ConfigFile }
}
if (!(Get-Command $ArduinoCli -ErrorAction SilentlyContinue)) {
    throw 'Arduino CLI was not found. Install Arduino CLI or pass -ArduinoCli <full path to arduino-cli.exe>. -Install installs board packages and libraries, not the CLI itself. See firmware/README.md.'
}
try {
# GCC on Windows can fail on deeply nested toolchain paths. Map the configured
# workspace only for this build, without changing PATH or claiming an occupied drive.
if ($settings -and $settings.ShortPathRoot) {
    $shortRoot = (Resolve-Path -LiteralPath $settings.ShortPathRoot).Path.TrimEnd('\','/')
    foreach ($letter in @('R','S','T','U','V','W','X','Y','Z')) {
        if (!(Test-Path ($letter + ':\'))) { $mappedDrive = $letter + ':'; break }
    }
    if (!$mappedDrive) { throw 'No free drive letter for the short build path.' }
    & subst.exe $mappedDrive $shortRoot
    if ($LASTEXITCODE -ne 0) { throw 'Could not create short build path.' }
    $ArduinoCli = $ArduinoCli.Replace('\','/').Replace($shortRoot.Replace('\','/'), $mappedDrive)
    $repoRoot = $repoRoot.Replace('\','/').Replace($shortRoot.Replace('\','/'), $mappedDrive)
    $generatedConfig = Join-Path $repoRoot 'build/arduino-cli.local.yaml'
    New-Item -ItemType Directory -Force (Split-Path $generatedConfig) | Out-Null
    $configText = [IO.File]::ReadAllText($ConfigFile).Replace($shortRoot.Replace('\','/'), $mappedDrive)
    [IO.File]::WriteAllText($generatedConfig, $configText)
    $ConfigFile = $generatedConfig
}
$options = @()
if ($ConfigFile) { $options += @('--config-file', $ConfigFile) }
function Invoke-Arduino([string[]]$Arguments) {
    & $ArduinoCli @Arguments @options
    if ($LASTEXITCODE -ne 0) { throw "Arduino CLI failed with exit code $LASTEXITCODE" }
}
if ($Install) {
    Invoke-Arduino -Arguments @('core','update-index','--additional-urls','https://espressif.github.io/arduino-esp32/package_esp32_index.json')
    Invoke-Arduino -Arguments @('core','install','esp32:esp32@3.3.0','--additional-urls','https://espressif.github.io/arduino-esp32/package_esp32_index.json')
    Invoke-Arduino -Arguments @('lib','update-index')
    # All dependencies explicitly installed; prevent transitive latest-version upgrades.
    Invoke-Arduino -Arguments @('lib','install','--no-deps','Adafruit BusIO@1.17.4','Adafruit Unified Sensor@1.1.15','Adafruit GFX Library@1.12.6','Adafruit SSD1306@2.5.17','Adafruit MPU6050@2.2.6','Adafruit PWM Servo Driver Library@3.0.2')
}
Invoke-Arduino -Arguments @('compile','--fqbn','esp32:esp32:XIAO_ESP32C3:CDCOnBoot=default','--warnings','all','--output-dir',(Join-Path $repoRoot 'build/Companion'),(Join-Path $repoRoot 'firmware/Companion'))
} finally {
    if ($mappedDrive) { & subst.exe $mappedDrive /D }
}
