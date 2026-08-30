#!/usr/bin/env pwsh
#Requires -Version 7.0

param(
	[string]$Preset = "windows-release",
	[int]$Jobs = (Get-CimInstance -ClassName Win32_ComputerSystem).NumberOfLogicalProcessors,
	[ValidateSet("all", "bench", "target")]
	[string]$Mode = "all",
	[string]$Target = "",
	[switch]$Clean
)

$ErrorActionPreference = "Stop"

# Generate CMakePresets.json with Windows presets
$CMakePresetsContent = @"
{
  "version": 3,
  "configurePresets": [
	{
	  "name": "windows-base",
	  "hidden": true,
	  "generator": "Visual Studio 17 2022",
	  "binaryDir": "`${sourceDir}/build/`${presetName}",
	  "cacheVariables": {
		"CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
	  }
	},
	{
	  "name": "windows-debug",
	  "inherits": "windows-base",
	  "cacheVariables": {
		"CMAKE_BUILD_TYPE": "Debug"
	  }
	},
	{
	  "name": "windows-release",
	  "inherits": "windows-base",
	  "cacheVariables": {
		"CMAKE_BUILD_TYPE": "Release"
	  }
	}
  ],
  "buildPresets": [
	{
	  "name": "windows-debug",
	  "configurePreset": "windows-debug"
	},
	{
	  "name": "windows-release",
	  "configurePreset": "windows-release"
	}
  ],
  "testPresets": [
	{
	  "name": "windows-debug",
	  "configurePreset": "windows-debug",
	  "output": {
		"outputOnFailure": true
	  }
	},
	{
	  "name": "windows-release",
	  "configurePreset": "windows-release",
	  "output": {
		"outputOnFailure": true
	  }
	}
  ]
}
"@

Set-Content -Path "CMakePresets.json" -Value $CMakePresetsContent
Write-Host "CMakePresets.json generated for Windows."

$RootDir = Get-Location
$BuildDir = Join-Path $RootDir "build" $Preset

if ($Clean) {
	Write-Host "[clean] Removing $BuildDir"
	if (Test-Path $BuildDir) {
		Remove-Item -Path $BuildDir -Recurse -Force
	}
}

New-Item -ItemType Directory -Path $BuildDir -Force | Out-Null

Write-Host "[1/2] Configuring preset: $Preset"
& cmake --preset $Preset

Write-Host "[2/2] Building"
switch ($Mode) {
	"all" {
		& cmake --build --preset $Preset --config Release --parallel $Jobs
	}
	"bench" {
		& cmake --build --preset $Preset --config Release --parallel $Jobs --target socket_boost_bench store_bench
	}
	"target" {
		if (-not $Target) {
			Write-Error "--target requires a target name."
			exit 1
		}
		& cmake --build --preset $Preset --config Release --parallel $Jobs --target $Target
	}
}

Write-Host "Build finished successfully."
