# Computer-only regression checks. Does not open serial ports or upload.
param([switch]$SkipBuild)
$ErrorActionPreference = 'Stop'
$repo = Split-Path -Parent $PSScriptRoot
$entry = Join-Path $repo 'robot.ps1'
foreach ($file in @('robot.ps1','toolchain.psd1','scripts/build.ps1')) {
    $tokens = $null; $errors = $null
    [System.Management.Automation.Language.Parser]::ParseFile((Join-Path $repo $file), [ref]$tokens, [ref]$errors) | Out-Null
    if ($errors.Count) { throw ($errors | Out-String) }
}
foreach ($taskName in @('upload','status','monitor')) {
    try { & $entry $taskName -Port COM7; throw 'Guard failed to reject missing confirmation' }
    catch { if ($_.Exception.Message -notlike 'Disconnect servo power*') { throw } }
}
try { & $entry upload -ServoPowerDisconnected; throw 'Guard failed to reject missing port' }
catch { if ($_.Exception.Message -notlike 'Specify the identified*') { throw } }
Write-Host 'PASS syntax and pre-serial/pre-upload argument guards. No ports opened.'
if (!$SkipBuild) {
    foreach ($sketchName in @('Basics','Accelerometer','Gyroscope','Temperature','Display','Servo','Integration','Hello','Scanner','Face','Motion','Companion')) {
        Write-Host "Building $sketchName"
        & $entry build -Sketch $sketchName
    }
}
