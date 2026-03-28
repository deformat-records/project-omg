# CHAØS Firmware v1.0.0

**Developed by John Merrik for Deformat**
For use with the **Genetically Modified Oscillator (GMØ)** by [Moffenzeef Modular](https://moffenzeefmodular.com)
Original firmware by Ross Fish, July 2015
Mozzi Synthesis Library by Tim Barrass
Upgraded to work with Mozzi 2.x & modern ADC by John Merrik

*Based heavily on Fish's code for BØBCAT.*  
*CC-BY-NC-SA*

## Status

Status of this firmware is still incomplete, while a 1.0.0 is the version number,
it is more of a beta. The Chaos mode (the name of the module) is not how I desire it to behave.
This was more of an attempt to learn the firmware.

I am still posting it, and keeping the firmware in all versions here for the sake of others
learning, and using it.

---

## Overview

CHAØS is a percussive, three-voice alternate firmware for the GMØ.  
It features:

- Three independent triggers (two main voices + one accent/collision voice)
- Toggleable "chaos mode" for randomization of pitch and decay
- Filtered noise synthesis using Mozzi
- Backwards-compatible audio routines with modern Mozzi 2.x
- Designed to complement the original GMØ aesthetic

---

## Features

- Mono output  
- Two primary drum voices, one accent/collision voice
- Randomized pitch and decay when CHAØS toggle is on
- Noise generation using white and brown noise tables
- Adjustable CV inputs via pots and jacks
- LED indicators for trigger and audio activity
- Filter resonance control based on chaos mode

---

## Pinout

### Digital Inputs

| Function             | Pin |
|---------------------|-----|
| CHAØS Toggle         | 2   |
| Trigger Voice 1 (Bang!) | 3   |
| Trigger Voice 2 (Bang!) | 4   |

### Digital Outputs

| Function                  | Pin |
|----------------------------|-----|
| Teensy Power LED           | 13  |

### Analog Inputs – Pots

| Function                     | Pin |
|-------------------------------|-----|
| Voice 1 CV Pitch (Speed)      | A1  |
| Voice 2 CV Decay (Head)       | A3  |
| Voice 2 Pitch (Tail)          | A5  |
| Voice 1 Decay (Specimen)      | A7  |

### Analog Inputs – CV Jacks

| Function                  | Pin |
|----------------------------|-----|
| Voice 1 CV Pitch           | A2  |
| Voice 2 CV Decay           | A4  |
| Voice 2 CV Pitch           | A6  |
| Voice 1 CV Decay           | A8  |

### Analog Outputs

| Function                  | Pin |
|----------------------------|-----|
| Audio Output              | DAC/A14 |
| Audio LED Indicator       | A9      |

### Example Faceplate for Firmware

![Faceplate Mockup](/firmware/chaos/media/faceplate_mockup.png)

---

## Usage

1. Install Arduino IDE and Mozzi 2.x library
2. Connect GMØ to your computer
3. Open `OMG_FIRMWARE_CHAOS_vX.X.X.ino`
4. Compile and upload to the GMØ

Alternativly, there are compiled HEX files available.

### Notes

- CHAØS toggle enables randomized "chaos" mode
- Triggering both Bang! inputs at the same time generates a third accent voice
- Filter frequencies adjust depending on toggle state

---

## Current Development / TODO

- Make third voice/accent more pronounced
- Improve chaos mode range for pitch and decay
- Consider optional probability triggers for chaos events
- Further tuning of filter and resonance behavior

---

## References

- [Original GMØ Wiki](http://github.com/moffenzeefmodular/GMO/wiki)  
- [Mozzi Synthesis Library](https://sensorium.github.io/Mozzi/)  

---

## License

CC-BY-NC-SA (same as original Moffenzeef firmware)