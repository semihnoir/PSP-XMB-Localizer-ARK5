# Localizer PSP — ARK-5 Port

![Credits image shown in System Information](assets/semihnoir.png)

## About the Original Project

**Localizer™: Translate your XMB!** is a PSP homebrew plugin originally developed by **Draan**.
Its purpose is to allow users to translate XMB themes that use languages not officially supported by the PSP.
With Localizer, users can enjoy their favourite CTF and F0 themes, switch between themes using plugins such as CXMB, and have their XMB translated into their preferred language.

Originally released as an entry for the **PSP Genesis Competition 2011**, Localizer received recognition as *Wololo's Favourite Open Source Entry* in the Plugins category.

This project preserves the original concept and functionality created by Draan.

## About This Port

The original release supported firmware versions such as 6.60 and PRO LME.
This port adapts the project for **PSP firmware 6.61 running ARK-5** (tested with ARK 5.1.0).

The original source code was reviewed, and the firmware-compatibility parts were analysed and adjusted for the ARK-5 environment with assistance from Anthropic Claude. The adapted build was subsequently tested on real hardware.

The goal of this port is to bring Localizer to PSP systems running 6.61 ARK-5 while preserving the original functionality and vision of the project.

### What changed compared to the original

* **6.61 support.** 6.61 reports devkit version `0x06060110`, which the original did not know, so it silently patched nothing. It now uses the 6.60 layout (the 6.60 and 6.61 `paf.prx` / `sysconf_plugin.prx` are identical at every patched address).
* **Safety checks.** The hard-coded offsets are verified (call targets, export slots) before anything is patched; on a mismatch the plugin leaves the firmware untouched instead of crashing.
* **ARK-5 crash fix.** On ARK-5 the console powered off right after the plugin started. Loading the embedded user module immediately from the plugin loader's context was the culprit, so the kernel module now waits briefly before loading it (see `LOAD_DELAY_US` in `src/kernel/main.c`).
* **No crash without `localizer.dat`.** A missing database used to dereference a NULL pointer.
* **Optional debug logs** (`./build.sh debug`) – `ms0:/seplugins/localizer.log` and `localizer_k.log`. The release build writes nothing.
* **Builds with a current toolchain** ([pspdev](https://pspdev.github.io), GCC 15) instead of the 2011 PSPSDK.
* **About image cycle.** The System Information page loads a single image (`flash0:/vsh/resource/name_plate.png`). Each time it is opened the plugin serves the next one: `semihnoir.png` → `genesis.png` → the original PSP image.
* **Turkish database** (`strings/strings_tr.txt`, `dist/localizer_tr.dat`).

## Installation

1. Copy `dist/localizer.prx` and a language database (e.g. `dist/localizer_tr.dat`, renamed to `localizer.dat`) to `ms0:/seplugins/` (`ef0:/seplugins/` on a PSP Go).
2. Add this line to `ms0:/seplugins/vsh.txt` (or the ARK `PLUGINS.TXT`):
   ```
   vsh, ms0:/seplugins/localizer.prx, on
   ```
3. Reboot.

`localizer.dat` is looked up in `ef0:/seplugins/` first, then `ms0:/seplugins/`.

## Building

Plugin (Linux / WSL with [pspdev](https://pspdev.github.io) installed):

```bash
./build.sh          # dist/localizer.prx
./build.sh debug    # dist/localizer_debug.prx (writes the log files)
```

Language database (Windows, uses Draan's `Paxor.exe`; its source is in `src/Paxor3`):

```bat
tools\make_db.bat tr      :: strings\strings_tr.txt -> dist\localizer_tr.dat
```

## Writing translations – things worth knowing

* The strings file is UTF-16 LE with BOM, one `'label' = 'text'` entry per line. Keep `%1`, `%2`, `™`, `®` and the invisible button glyphs (U+F8xx) intact.
* **Paxor ends a text at the first `'`.** An apostrophe inside a value silently cuts the string, so avoid it (this is why e.g. Turkish suffixes after names are written without an apostrophe).
* The XMB font has no glyph for some characters; they show up as `_`. For Turkish that affects `ğ Ğ ş Ş İ`, so the Turkish file uses `g G s S I` instead (`ı ö ü ç` work).

## Status

* Tested on PSP, firmware 6.61, ARK 5.1.0: plugin loads and the XMB is translated.
* The silent release build and the About image cycle are new and have had less testing than the debug build; if anything misbehaves, try `dist/localizer_debug.prx` and look at the log files.

## Credits & Links

* **Original developer:** Draan (Herby)
* **Original project:** [Localizer PSP — GameBrew](https://www.gamebrew.org/wiki/Localizer_PSP)
* **Compatibility port & adaptation:** [SemihNoir](https://github.com/semihnoir)
* **AI-assisted development:** Anthropic Claude (contributor)
* Draan's own credits (Bubbletune, Davee, krzychuuu & Erokesha, mint) are kept in [`docs/README_original_Draan.txt`](docs/README_original_Draan.txt), which must stay with any redistribution.

## Licence / Disclaimer

This is an **unofficial** compatibility port. Localizer™ remains the original work of Draan; all credit for the original plugin, concept and development belongs to its creator.
The original readme allows redistribution as long as its information is kept; no other licence was published with the source, so none is added here.
**Use at your own risk.**

## Repository layout

```
assets/    images shown in System Information
dist/      built plugin (release + debug) and Turkish database
docs/      original readmes
src/       kernel/ user/ (plugin), Paxor3/ (database tool)
strings/   translation sources (English reference, Turkish, old Polish sample)
tools/     Paxor.exe and make_db.bat
build.sh   plugin build script
```
