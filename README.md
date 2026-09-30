# RaceMetrics 0.11 — Racial XP Multipliers

Adds racial XP multipliers below Attributes in the character Stats window. Uses actual race modifiers for the character shown in that window and native translated stat names. Bonuses are green, penalties red; neutral x1 rows are hidden. No extra tooltips or race name. No gameplay or save changes; Import Game is not needed.

Requirements: Windows x64, Kenshi 1.0.65, RE_Kenshi and KenshiLib. Built/import-checked against RE_Kenshi 0.3.5 / KenshiLib 0.5.0. User confirmed the previous Russian v0.10 interface on Kenshi 1.0.65 Steam.

Install with the game closed: copy the RaceMetrics folder to Kenshi/mods and enable it in the launcher. RaceMetrics.dll, RaceMetrics.mod and RE_Kenshi.json must be together. Disable the mod and restart to remove its interface changes.

Languages: English, Russian, German, Spanish, French, Brazilian Portuguese, Japanese, Korean and Simplified Chinese. The heading follows Kenshi's active LocaleManager language, not Windows. Unknown languages use the English heading. Native stat names follow the game's translation; missing names fall back to English. Numbers use dot or comma for the selected language. The native font supports the installed locale. Width is measured from the translated heading and skill names; the heading stays on one line.

localization.json is the translator-facing source, NOT runtime configuration. After editing it, run tools/generate_localization.py and rebuild the DLL. Do not modify the game's own translation files.

Validation: v0.11 compilation/import checks passed; standalone C++ tests passed for nine languages, regional aliases, fallback and number formatting. Shared geometry tests passed 312 cases. The author confirmed v0.11 working in game. Three supplied English screenshots verify different characters with 10, 12 and 6 displayed modifiers. This is not verification of all nine translations. Translations other than English/Russian have not had native-speaker review. User performs runtime testing. Very long texts, large modded race lists, small screen resolutions and live UI scaling need testing.

Active source: src/RaceMetrics.cpp, src/StatsPanelLayout.h, src/RaceLocalization.h. Built using VC2010 x64 ABI. License: GPL-3.0-or-later, see LICENSE.

Русский: добавляет «Расовые множители опыта» под «Качествами». Заголовок автоматически выбирается по языку игры, названия характеристик берутся из её перевода. Версия 0.10 проверена пользователем. Работа версии 0.11 подтверждена пользователем; все девять языков отдельно не проверялись.


## Links and source review

- [Nexus Mods page](https://www.nexusmods.com/kenshi/mods/2196)
- [Build instructions](BUILD.md)
- [RE_Kenshi](https://github.com/BFrizzleFoShizzle/RE_Kenshi)
- [KenshiLib](https://github.com/KenshiReclaimer/KenshiLib)

This repository contains the plugin source corresponding to Nexus version 0.11. Game binaries, SDK dependencies and compiled DLLs are not committed. RaceMetrics.mod is the small launcher activation stub; RE_Kenshi.json declares the plugin DLL. The source reads race and locale data and hooks CharacterStatsWindow::_NV_update to draw native MyGUI widgets. It does not write save files.

The initial Nexus binary upload (file 6258) was quarantined by automated checks; review is pending. The exact trigger has not been identified. This repository provides source and build instructions for review; publishing source does not itself clear the quarantine.

Developed with AI assistance. RE_Kenshi, KenshiLib and Kenshi belong to their respective authors.
