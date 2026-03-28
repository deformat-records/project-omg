# Project ØMG

Alternate firmware, scripts, and utilities for the **MØFFENZEEF GMØ**.

This repository is a labor of love. 
A series of code, ways to experiment, ways to modernize, and expand the capabilities of the GMØ while respecting the original aesthetic and workflow of the [Moffenzeef GMO](https://github.com/moffenzeefmodular/GMO).

---

## Overview

This repository (will or does) contain:

- Alternate firmware builds (CHAØS, BRØTHER, ØMG)  
- Source code and compiled hex files  
- Python utilities to help create or modify firmware  
- Updated original firmware, modernized to compile with:
  - [Mozzi](https://github.com/sensorium/Mozzi/) 2.x
  - Current Arduino IDE & libraries
  - Current ADC library  

The focus is not about redesining the GMØ, but improving compatibility.
Modifying the genetics of the GMØ

---

## Current Firmware Variants

### CHAØS

Status: Work in progress

- Percussive, three-voice firmware
- Snare-like, bass, and accent hits
- Third voice triggers when both BANG! inputs are active
- Toggle adds high-level randomization for unpredictable patterns
- Heavily inspired by BØBCAT

### BRØTHER

Status: Work in progress (Experimental)

- Amen-break-inspired firmware
- Supports backward playback, pitch/time stretching, and randomness
- Uses Mozzi 2.x features
- Designed for richer percussion options with int8 sample tables

### ØMG (Experimental)

Status: Unavailble, but coming soon

- Tape-scrub / vinyl scratch emulator
- Still in development, buggy, and experimental
- Was the original firmware designed for this, and the basis of the project
- Somehow is the least complete of all firmwares...
- ØMG is GMØ backwords, hence the dj scratch name

### Guide to flashing

Still eseentially the same as the original [Moffenzeef GMO](https://github.com/moffenzeefmodular/GMO).
Although no longer constrained to older versions of Arduino

![description](../media/flashing.png)

It is still a LOT of fun though, I wanted to presere that level of fun... ofc.

---

## Scripts & Utilities

Scripts help automate sample conversion and firmware preparation.

- **[wav_to_int8](https://github.com/deformat-records/deformat-scripts/tree/main/scripts/wav_to_int8)** – Convert WAV files to Mozzi-compatible int8 headers
- Future scripts will be added to automate firmware modifications, sample chopping, and more

---

## Future & Modernization

- Code updated for current compilers and Mozzi 2.x  
- Python scripts simplify audio conversion and firmware creation  
- Easier integration for new users  
- Preserves original aesthetic and source code style  
- Potential future plans:
  - VCV Rack module adaptation  
  - Custom faceplates for original GMO hardware  
  - Hardware redesign (depending on community interest)  
  
![description](../media/faceplate_mockup.png)

---

## Media

[BRØTHER Firmware in Action](https://youtu.be/FSbequE4ePk) - A live jam done with the BRØTHER firmware
[BRØTHER Demo Short](https://youtube.com/shorts/FwxSVU1Roo8) - The initial demonstration of the BRØTHER firmware

---

## Notes

- This repository is experimental and intended for users familiar with Arduino, Mozzi, and GMØ firmware
- Some firmwares are tightly experimental; others are updated originals
- Feedback, collaboration, and contributions are welcome
- Some scripts and code are adapted or inspired from public sources (Mozzi, BØBCAT, etc.)
- Probably still just as complicated to flash as the original

## Support this Project

---

## Links

- [Original Moffenzeef GMO](https://github.com/moffenzeefmodular/GMO)  
- [WAV to int8 Script](https://github.com/deformat-records/deformat-scripts/tree/main/scripts/wav_to_int8)
- [Mozzi](https://github.com/sensorium/Mozzi/)