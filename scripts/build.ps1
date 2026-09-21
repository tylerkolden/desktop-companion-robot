# Compatibility entry point. All tools now live inside this checkout.
param([switch]$Install)
$ErrorActionPreference = 'Stop'
$entry = Join-Path (Split-Path -Parent $PSScriptRoot) 'robot.ps1'
if ($Install) { & $entry setup }
& $entry build
