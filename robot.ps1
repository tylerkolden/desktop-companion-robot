[CmdletBinding()]
param(
    [ValidateSet('help','setup','doctor','build','ports','upload','status','monitor')][string]$Task = 'help',
    [ValidateSet('Companion','Hello','Scanner','Face','Motion')][string]$Sketch = 'Companion',
    [ValidatePattern('^COM[1-9][0-9]*$')][string]$Port,
    [ValidateSet(115200,230400,460800,921600)][int]$UploadBaud = 115200,
    [switch]$ServoPowerDisconnected
)
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
if ($Task -eq 'help') {
    Write-Output @'
Run from PowerShell in this repository:
  .\robot.ps1 setup                  Download pinned local tools and libraries
  .\robot.ps1 doctor                 Show toolchain and installed versions
  .\robot.ps1 build -Sketch Hello    Compile a lesson (no upload)
  .\robot.ps1 build                  Compile Companion (no upload)
  .\robot.ps1 ports                  List ports; never selects one automatically
  .\robot.ps1 status -Port COM7 -ServoPowerDisconnected
  .\robot.ps1 upload -Sketch Hello -Port COM7 -ServoPowerDisconnected
  .\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
Replace COM7 with your identified XIAO port. The switch confirms that servo
power is physically disconnected; software cannot verify it. Upload compiles
first and replaces the installed program. Opening a port can reset the board.
Monitor is interactive; Ctrl+C closes it. Start at docs/START-HERE.md.
Available sketches: Hello, Scanner, Face, Motion, Companion.
Tools download into ignored .tools/ inside this checkout.
Uploads request 115200 baud by default; override with -UploadBaud 921600.
'@
    return
}
if ($env:OS -ne 'Windows_NT' -or ![Environment]::Is64BitOperatingSystem) { throw 'Automated setup supports Windows x64. See docs/SETUP.md.' }
if ($Task -in @('upload','status','monitor')) {
    if (!$Port) { throw 'Specify the identified XIAO port with -Port COMn; first run ports.' }
    if (!$ServoPowerDisconnected) { throw 'Disconnect servo power, then add -ServoPowerDisconnected. Opening a port may reset the board.' }
}
$root = $PSScriptRoot
$lock = Import-PowerShellDataFile (Join-Path $root 'toolchain.psd1')
$toolDir = Join-Path $root '.tools'
New-Item -ItemType Directory -Force $toolDir | Out-Null
$lease = $null; $drive = $null; $savedEnv = @{}
$oldLocation = Get-Location
try {
    try { $lease = [IO.File]::Open((Join-Path $toolDir 'session.lock'), 'OpenOrCreate', 'ReadWrite', 'None') }
    catch { throw 'Another robot command is running in this checkout. Finish or close it first.' }
    Get-ChildItem Env: | Where-Object Name -Like 'ARDUINO_*' | ForEach-Object {
        $savedEnv[$_.Name] = $_.Value
        [Environment]::SetEnvironmentVariable($_.Name, $null, 'Process')
    }
    foreach ($letter in @('R','S','T','U','V','W','X','Y','Z')) {
        if (!(Get-PSDrive -Name $letter -ErrorAction SilentlyContinue) -and !(Test-Path ($letter + ':\'))) { $drive = $letter + ':'; break }
    }
    if (!$drive) { throw 'No free drive letter for the temporary short compiler path.' }
    & subst.exe $drive $root
    if ($LASTEXITCODE -ne 0) { $drive = $null; throw 'Could not create temporary short path.' }
    $short = $drive + '/'
    Set-Location $short
    $cli = $short + '.tools/cli/arduino-cli.exe'
    $config = $short + '.tools/arduino-cli.yaml'
    foreach ($dir in @('data','downloads','user','cache','tmp')) { New-Item -ItemType Directory -Force ($short + '.tools/' + $dir) | Out-Null }
    $yaml = @"
board_manager:
  additional_urls:
    - $($lock.BoardIndex)
directories:
  data: $($short).tools/data
  downloads: $($short).tools/downloads
  user: $($short).tools/user
build_cache:
  path: $($short).tools/cache
"@
    [IO.File]::WriteAllText($config, $yaml)
    function Invoke-Arduino([string[]]$Arguments) {
        & $cli @Arguments --config-file $config
        if ($LASTEXITCODE -ne 0) { throw "Arduino CLI failed ($LASTEXITCODE). Read the first error above; see docs/TROUBLESHOOTING.md." }
    }
    if ($Task -eq 'setup') {
        $archive = $short + '.tools/downloads/arduino-cli.zip'
        if (!(Test-Path $archive) -or (Get-FileHash $archive -Algorithm SHA256).Hash -ne $lock.CliSha256) {
            Write-Host 'Downloading Arduino CLI from the official pinned release...'
            Invoke-WebRequest -UseBasicParsing $lock.CliUrl -OutFile $archive
        }
        if ((Get-FileHash $archive -Algorithm SHA256).Hash -ne $lock.CliSha256) { throw 'CLI checksum mismatch; archive will not be executed.' }
        Expand-Archive -LiteralPath $archive -DestinationPath ($short + '.tools/cli') -Force
    }
    if (!(Test-Path $cli)) { throw 'Local tools are missing. Run .\robot.ps1 setup first.' }
    $versionText = (& $cli version | Out-String)
    if ($LASTEXITCODE -ne 0 -or $versionText -notmatch ('Version:\s*' + [regex]::Escape($lock.CliVersion) + '\b')) { throw 'Unexpected CLI version. Run .\robot.ps1 setup.' }
    switch ($Task) {
        'setup' {
            Invoke-Arduino @('core','update-index')
            Invoke-Arduino @('core','install',$lock.Core)
            Invoke-Arduino @('lib','update-index')
            Invoke-Arduino (@('lib','install','--no-deps') + $lock.Libraries)
            Write-Host 'Setup finished. Next: .\robot.ps1 build -Sketch Hello'
        }
        'doctor' {
            Write-Output $versionText
            Write-Host "Local tools: $toolDir"
            Invoke-Arduino @('core','list'); Invoke-Arduino @('lib','list')
            Write-Host 'Compare with toolchain.psd1. This inventory does not test hardware.'
        }
        'ports' { Invoke-Arduino @('board','list') }
        { $_ -in @('build','upload') } {
            $sketchPath = if ($Sketch -eq 'Companion') { 'firmware/Companion' } else { 'lessons/sketches/' + $Sketch }
            $output = 'build/' + $Sketch
            Invoke-Arduino @('compile','--fqbn',$lock.Fqbn,'--warnings','all','--output-dir',$output,$sketchPath)
            if ($Task -eq 'upload') {
                Write-Host "Uploading to $Port with requested baud $UploadBaud."
                Invoke-Arduino @('upload','--fqbn',$lock.Fqbn,'--port',$Port,'--upload-property',"upload.speed=$UploadBaud",'--input-dir',$output,$sketchPath)
                Write-Host 'Upload tool completed. Check serial output; physical behavior is not yet verified.'
            }
        }
        'monitor' { Invoke-Arduino @('monitor','--port',$Port,'--config','baudrate=115200') }
        'status' {
            $serial = New-Object System.IO.Ports.SerialPort $Port,115200,None,8,one
            try {
                $serial.DtrEnable = $false; $serial.RtsEnable = $false
                $serial.ReadTimeout = 500; $serial.WriteTimeout = 1000; $serial.NewLine = "`n"
                $serial.Open()
                Start-Sleep -Seconds 2
                $serial.WriteLine('help'); $serial.WriteLine('status')
                $timer = [Diagnostics.Stopwatch]::StartNew(); $received = ''
                while ($timer.Elapsed.TotalSeconds -lt 6) {
                    $chunk = $serial.ReadExisting()
                    if ($chunk) { $received += $chunk; Write-Host -NoNewline $chunk }
                    Start-Sleep -Milliseconds 100
                }
                New-Item -ItemType Directory -Force 'build/logs' | Out-Null
                $logPath = 'build/logs/status-' + (Get-Date -Format 'yyyyMMdd-HHmmss') + '.txt'
                [IO.File]::WriteAllText((Join-Path (Get-Location) $logPath), $received)
                if (!$received) { Write-Warning 'No reply does not prove the board is blank. See troubleshooting.' }
                Write-Host "`nCapture: $logPath. Review before adding to docs/TEST-LOG.md."
            } finally { if ($serial.IsOpen) { $serial.Close() }; $serial.Dispose() }
        }
    }
} finally {
    Set-Location $oldLocation
    if ($drive) { & subst.exe $drive /D }
    foreach ($key in $savedEnv.Keys) { [Environment]::SetEnvironmentVariable($key, $savedEnv[$key], 'Process') }
    if ($lease) { $lease.Dispose() }
}
