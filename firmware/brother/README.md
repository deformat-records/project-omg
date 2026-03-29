# BRØTHER Firmware v1.0.0

**Developed by John Merrik for Deformat**
For use with the **Genetically Modified Oscillator (GMØ)** by [Moffenzeef Modular](https://moffenzeefmodular.com)
Original firmware by Ross Fish, July 2015
Mozzi Synthesis Library by Tim Barrass
Upgraded to work with Mozzi 2.x & modern ADC by John Merrik

*Based heavily on Fish's code for BØBCAT.*  
*CC-BY-NC-SA*

---

## Version History

- **v0.9.0** – Initial beta with one bank (Amen breaks only)
- **v1.0.0** – Added second bank (Think breaks), updated pinout and triggers, modern Mozzi 2.x support

---

## Status

Status of this firmware is still nearly complete, with some touches up to the code.
I had wanted to add a second bank and did so in a hasty manner.
I would like to go back through.

I also updated the [wav_to_int8.py](https://github.com/deformat-records/deformat-scripts/blob/main/scripts/wav_to_int8) script to assist with max_samples for headers.
Ideally, making a more streamlined method for custom samples would be excellent.

---

## Overview

BRØTHER is a breakbeat driven, two-bank firmware for the GMØ module.
It features:

- Two banks of 16 samples each (Amen + Think)
- Three trigger inputs (Fixed, Random, Break Select)
- Adjustable pitch, stretch, and multiplier per sample
- Uses Mozzi for audio synthesis and output
- Designed for backwards-compatible audio routines and modern ADCs

---

## Features

- Mono output  
- Two selectable banks of breakbeats
- Randomized sample/slice selection on trigger
- Adjustable CV inputs via pots and jacks
- LED indicators for audio activity and power
- Back and Forth scrubbing/playing

---

## Pinout

### Digital Inputs

| Function             | Pin |
|----------------------|-----|
| Break Select (Bank)  | 4   |
| Trigger Fixed Sample | 2   |
| Trigger Random Sample| 3   |

### Digital Outputs

| Function                  | Pin |
|----------------------------|-----|
| Teensy Power LED           | 13  |

### Analog Inputs – Pots

| Function                     | Pin |
|-------------------------------|-----|
| Stretch (Speed)              | A1  |
| Multiplier (Head)            | A3  |
| Pitch (Tail)                 | A5  |
| Sample Select (Specimen)     | A7  |

### Analog Inputs – CV Jacks

| Function                     | Pin |
|-------------------------------|-----|
| Stretch CV (Speed)           | A2  |
| Multiplier CV (Head)         | A4  |
| Pitch CV (Tail)              | A6  |
| Sample Select CV (Specimen)  | A8  |

### Analog Outputs

| Function                  | Pin |
|----------------------------|-----|
| Audio Output              | DAC/A14 |
| Audio LED Indicator       | A9      |

### Example Faceplate for Firmware

![Faceplate Mockup](/firmware/brother/media/faceplate_mockup.png)

---

## Usage

1. Install Arduino IDE and Mozzi 2.x library
2. Connect GMØ to your computer
3. Open `OMG_FIRMWARE_BROTHER_vX.X.X.ino`
4. Compile and upload to the GMØ

Alternativly, there are compiled HEX files available.
And you can use tools like [wav_to_int8.py](https://github.com/deformat-records/deformat-scripts/blob/main/scripts/wav_to_int8) and [slicethon.py](https://github.com/deformat-records/deformat-scripts/blob/main/scripts/slicethon) for making your own banks.

## Notes

- Break Select chooses the sample bank (Amen vs Think)
- Trigger Fixed and Random allow flexible performance
- Sample playback speed and pitch are adjustable via pots and CV
- LED output indicates audio presence

---

## Current Development / TODO (v1.1.0+)

- Clean up trigger input mapping
- Adjust pitch/stretch ranges for better performance
- Test memory limits for additional samples per bank (24–32)
- Consider probability-based chaos/randomization
- Refactor code to use shared Wavetable folder

---

## References

- [Original GMØ Wiki](http://github.com/moffenzeefmodular/GMO/wiki)  
- [Mozzi Synthesis Library](https://sensorium.github.io/Mozzi/)  

---

## License

CC-BY-NC-SA (same as original Moffenzeef firmware)