# Building RaceMetrics

Use the Visual C++ 2010 x64 compiler, Windows SDK 7.1 and the matching KenshiLib SDK dependencies. Newer compiler ABIs are not interchangeable.

Dependencies: https://github.com/BFrizzleFoShizzle/KenshiLib_Examples_deps
Validated commit: b566d74bf3d74629cc2fb632a97595b8202993f1
Clone with Git LFS installed; do not use a source ZIP (the repository contains LFS files). See the dependency repository's licensing for its headers/libraries. Game binaries and SDK dependencies are not distributed in this archive.

From a VC2010 x64 developer prompt with Windows SDK 7.1 configured, run:

    powershell -File build.ps1 -Dependencies C:\path\to\KenshiLib_Examples_deps

This builds RaceMetrics.dll in the mod folder. If editing localization.json, run `python tools/generate_localization.py` first. Standalone tests are in tests; they do not launch Kenshi.

Source hooks CharacterStatsWindow::_NV_update, calls the original first, then draws native MyGUI widgets. It reads RaceData::getStatMod and LocaleManager. No save writing or gameplay mutation is performed.


## Standalone tests

In the same VC2010 x64 developer prompt, from the repository root:

    cl /nologo /EHsc /MD tests\race_locale_test.cpp /Ferace_locale_test.exe
    .\race_locale_test.exe
    cl /nologo /EHsc /MD tests\stats_panel_test.cpp /Festats_panel_test.exe
    .\stats_panel_test.exe

The tests do not require or launch the game. In-game rendering is checked manually.
