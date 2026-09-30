param([Parameter(Mandatory=$true)][string]$Dependencies)
$ErrorActionPreference='Stop'
$deps=(Resolve-Path -LiteralPath $Dependencies).Path
$inc=Join-Path $deps 'KenshiLib\Include'
$env:INCLUDE="$inc;$inc\ogre;$deps\boost_1_60_0;$env:INCLUDE"
$env:LIB="$deps\KenshiLib\Libraries;$env:LIB"
Push-Location $PSScriptRoot
try {
    & cl.exe /nologo /LD /O2 /GL /MD /EHsc /DNDEBUG /DNOMINMAX /DWIN32_LEAN_AND_MEAN /D_WIN64 /DWIN32 /D_WINDOWS /D_USRDLL /wd4251 "/FI$inc\kenshi\util\OgreUnordered.h" src\RaceMetrics.cpp /FoRaceMetrics.obj /link /LTCG /MACHINE:X64 /OUT:RaceMetrics.dll KenshiLib.lib OgreMain_x64.lib MyGUIEngine_x64.lib kernel32.lib user32.lib
    if($LASTEXITCODE -ne 0){throw 'Build failed'}
} finally { Pop-Location }
