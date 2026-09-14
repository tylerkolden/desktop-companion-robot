param(
    [string]$ArduinoCli = 'arduino-cli',
    [string]$ConfigFile = '',
    [switch]$Install
)
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot
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
