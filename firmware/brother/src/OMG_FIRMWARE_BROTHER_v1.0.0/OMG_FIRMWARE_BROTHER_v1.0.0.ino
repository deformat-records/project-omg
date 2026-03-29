/*
__/\\\\\\\\\\\\\______/\\\\\\\\\___________/\\\\\___//__/\\\\\\\\\\\\\\\__/\\\________/\\\__/\\\\\\\\\\\\\\\____/\\\\\\\\\_____        
 _\/\\\/////////\\\__/\\\///////\\\_______/\\\///\\//___\///////\\\/////__\/\\\_______\/\\\_\/\\\///////////___/\\\///////\\\___       
  _\/\\\_______\/\\\_\/\\\_____\/\\\_____/\\\/__\///\\\________\/\\\_______\/\\\_______\/\\\_\/\\\_____________\/\\\_____\/\\\___      
   _\/\\\\\\\\\\\\\\__\/\\\\\\\\\\\/_____/\\\____//\//\\\_______\/\\\_______\/\\\\\\\\\\\\\\\_\/\\\\\\\\\\\_____\/\\\\\\\\\\\/____     
    _\/\\\/////////\\\_\/\\\//////\\\____\/\\\__//___\/\\\_______\/\\\_______\/\\\/////////\\\_\/\\\///////______\/\\\//////\\\____    
     _\/\\\_______\/\\\_\/\\\____\//\\\___\//\\\/_____/\\\________\/\\\_______\/\\\_______\/\\\_\/\\\_____________\/\\\____\//\\\___   
      _\/\\\_______\/\\\_\/\\\_____\//\\\___\///\\\__/\\\__________\/\\\_______\/\\\_______\/\\\_\/\\\_____________\/\\\_____\//\\\__  
       _\/\\\\\\\\\\\\\/__\/\\\______\//\\\__//\///\\\\\/___________\/\\\_______\/\\\_______\/\\\_\/\\\\\\\\\\\\\\\_\/\\\______\//\\\_ 
        _\/////////////____\///________\///_//____\/////_____________\///________\///________\///__\///////////////__\///________\///__
            
			   BRØTHER v1.0.0                       
		  Developed by John Merrik                  
				for Deformat                        
			  For use with the                      
	   Genetically Modified Oscillator              
					  by                            
			  Moffenzeef Modular                    
		Created by Ross Fish July 2015              
				 CC-BY-NC-SA                        
		 http://moffenzeefmodular.com  
				 
	Mozzi Synthesis Library by Tim Barrass CC-BY-NC-SA
	Upgraded to work with Mozzi2 & modern ADC by Merrik

	Based Heavily on Fish's Code for BØBCAT
	Moffenzeef is not DEAD

	Information & Help:  
	http://github.com/moffenzeefmodular/GMO/wiki

/////////////////////////////////////////////////////////////////
            PINOUT 
/////////////////////////////////////////////////////////////////

	/////DIGITAL INS//////
	Break Select ==> Pin 4
	Trigger Fixed Sample (Bang!) ==> Pin 2
	Trigger Random (Bang!) ==> Pin 3 

	/////DIGITAL OUTS//////
	LED Teensy Power Indicator ==> Pin 13

	/////ANALOG INS - POTS//////
	Stetch Potentiometer (Speed) ==> Pin 15 (A1)
	Speed Multiplier (Head) ==> Pin 17 (A3)
	Pitch Potentiometer (Tail) ==> Pin 19 (A5)
	Fixed Sample Potentiometer (Specimen) ==> Pin 21 (A7)

	/////ANALOG INS - JACKS//////
	Stetch CV (Speed) ==> Pin 16 (A2)
	Speed Mult CV (Head) ==> Pin 18 (A4)
	Pitch CV (Tail) ==> Pin 20 (A6)
	Fixed Sample CV (Specimen) ==> Pin 22 (A8)

	/////ANALOG OUTS//////
	Audio Ouput == > DAC/A14 
	LED Audio Indicator ==> Pin 23 (A9)
  
// TODO

	Change/adjust range on pitch, stretch, mult.
	Can be way too much.  
	Test memory limitations, add more samples per break (24, then 32?)

*/
#define MOZZI_CONTROL_RATE 256
#define SAMPLE_COUNT 16			// number of samples per "bank"
#define MAX_NUM_CELLS 2730 		// if using multiple banks, replace with longest (from script)

#include <ADC.h>
#include <Mozzi.h>
#include <Sample.h>
#include <mozzi_rand.h>

// --------------------------
// SAMPLE TABLES
// --------------------------

//Amen Breaks
#include "Wavetables/amenbrotherone_int8.h"
#include "Wavetables/amenbrothertwo_int8.h"
#include "Wavetables/amenbrotherthree_int8.h"
#include "Wavetables/amenbrotherfour_int8.h"
#include "Wavetables/amenbrotherfive_int8.h"
#include "Wavetables/amenbrothersix_int8.h"
#include "Wavetables/amenbrotherseven_int8.h"
#include "Wavetables/amenbrothereight_int8.h"
#include "Wavetables/amenbrothernine_int8.h"
#include "Wavetables/amenbrotherten_int8.h"
#include "Wavetables/amenbrothereleven_int8.h"
#include "Wavetables/amenbrothertwelve_int8.h"
#include "Wavetables/amenbrotherthirteen_int8.h"
#include "Wavetables/amenbrotherfourteen_int8.h"
#include "Wavetables/amenbrotherfifteen_int8.h"
#include "Wavetables/amenbrothersixteen_int8.h"
//Think Breaks
#include "Wavetables/thinkbrotherone_int8.h"
#include "Wavetables/thinkbrothertwo_int8.h"
#include "Wavetables/thinkbrotherthree_int8.h"
#include "Wavetables/thinkbrotherfour_int8.h"
#include "Wavetables/thinkbrotherfive_int8.h"
#include "Wavetables/thinkbrothersix_int8.h"
#include "Wavetables/thinkbrotherseven_int8.h"
#include "Wavetables/thinkbrothereight_int8.h"
#include "Wavetables/thinkbrothernine_int8.h"
#include "Wavetables/thinkbrotherten_int8.h"
#include "Wavetables/thinkbrothereleven_int8.h"
#include "Wavetables/thinkbrothertwelve_int8.h"
#include "Wavetables/thinkbrotherthirteen_int8.h"
#include "Wavetables/thinkbrotherfourteen_int8.h"
#include "Wavetables/thinkbrotherfifteen_int8.h"
#include "Wavetables/thinkbrothersixteen_int8.h"

// --------------------------
// SAMPLE OBJECT
// --------------------------

Sample<MAX_NUM_CELLS, AUDIO_RATE> aSample(AMENBROTHERONE_DATA); 	// replace with any sample data

// --------------------------
// PLAYBACK ENGINE
// --------------------------

float phase = 0.0;
float increment = 1.0;
int currentLength = AMENBROTHERONE_NUM_CELLS;
bool playing = false;

// --------------------------
// TRIGGER STATE
// --------------------------

int trigFixed = 0;
int trigRandom = 0;
int lastTrigFixed = 0;
int lastTrigRandom = 0;
int breakMode = 0;

// --------------------------
// CONTROL SNAPSHOT
// --------------------------

float frozenStretch = 1.0;
float frozenMultiplier = 1.0;
float frozenPitch = 1.0;

int frozenSample = 0;

// --------------------------
// MULTIPLIER TABLE
// --------------------------

float multTable[8] = {
    -4.0,
    -2.0,
    -1.0,
    -0.5,
    0.5,
    1.0,
    2.0,
    4.0
};

// --------------------------
// FLOAT MAP
// --------------------------

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// --------------------------
// SAMPLE SWITCH
// --------------------------

void setSample(int mode, int index)
{
	switch(mode)
	{
		case 0: switch(index)
		{
			case 0: aSample.setTable(AMENBROTHERONE_DATA); currentLength = AMENBROTHERONE_NUM_CELLS; break;
			case 1: aSample.setTable(AMENBROTHERTWO_DATA); currentLength = AMENBROTHERTWO_NUM_CELLS; break;
			case 2: aSample.setTable(AMENBROTHERTHREE_DATA); currentLength = AMENBROTHERTHREE_NUM_CELLS; break;
			case 3: aSample.setTable(AMENBROTHERFOUR_DATA); currentLength = AMENBROTHERFOUR_NUM_CELLS; break;
			case 4: aSample.setTable(AMENBROTHERFIVE_DATA); currentLength = AMENBROTHERFIVE_NUM_CELLS; break;
			case 5: aSample.setTable(AMENBROTHERSIX_DATA); currentLength = AMENBROTHERSIX_NUM_CELLS; break;
			case 6: aSample.setTable(AMENBROTHERSEVEN_DATA); currentLength = AMENBROTHERSEVEN_NUM_CELLS; break;
			case 7: aSample.setTable(AMENBROTHEREIGHT_DATA); currentLength = AMENBROTHEREIGHT_NUM_CELLS; break;
			case 8: aSample.setTable(AMENBROTHERNINE_DATA); currentLength = AMENBROTHERNINE_NUM_CELLS; break;
			case 9: aSample.setTable(AMENBROTHERTEN_DATA); currentLength = AMENBROTHERTEN_NUM_CELLS; break;
			case 10: aSample.setTable(AMENBROTHERELEVEN_DATA); currentLength = AMENBROTHERELEVEN_NUM_CELLS; break;
			case 11: aSample.setTable(AMENBROTHERTWELVE_DATA); currentLength = AMENBROTHERTWELVE_NUM_CELLS; break;
			case 12: aSample.setTable(AMENBROTHERTHIRTEEN_DATA); currentLength = AMENBROTHERTHIRTEEN_NUM_CELLS; break;
			case 13: aSample.setTable(AMENBROTHERFOURTEEN_DATA); currentLength = AMENBROTHERFOURTEEN_NUM_CELLS; break;
			case 14: aSample.setTable(AMENBROTHERFIFTEEN_DATA); currentLength = AMENBROTHERFIFTEEN_NUM_CELLS; break;
			case 15: aSample.setTable(AMENBROTHERSIXTEEN_DATA); currentLength = AMENBROTHERSIXTEEN_NUM_CELLS; break;
		}
		break;
		case 1: switch(index)
		{
			case 0: aSample.setTable(THINKBROTHERONE_DATA); currentLength = THINKBROTHERONE_NUM_CELLS; break;
			case 1: aSample.setTable(THINKBROTHERTWO_DATA); currentLength = THINKBROTHERTWO_NUM_CELLS; break;
			case 2: aSample.setTable(THINKBROTHERTHREE_DATA); currentLength = THINKBROTHERTHREE_NUM_CELLS; break;
			case 3: aSample.setTable(THINKBROTHERFOUR_DATA); currentLength = THINKBROTHERFOUR_NUM_CELLS; break;
			case 4: aSample.setTable(THINKBROTHERFIVE_DATA); currentLength = THINKBROTHERFIVE_NUM_CELLS; break;
			case 5: aSample.setTable(THINKBROTHERSIX_DATA); currentLength = THINKBROTHERSIX_NUM_CELLS; break;
			case 6: aSample.setTable(THINKBROTHERSEVEN_DATA); currentLength = THINKBROTHERSEVEN_NUM_CELLS; break;
			case 7: aSample.setTable(THINKBROTHEREIGHT_DATA); currentLength = THINKBROTHEREIGHT_NUM_CELLS; break;
			case 8: aSample.setTable(THINKBROTHERNINE_DATA); currentLength = THINKBROTHERNINE_NUM_CELLS; break;
			case 9: aSample.setTable(THINKBROTHERTEN_DATA); currentLength = THINKBROTHERTEN_NUM_CELLS; break;
			case 10: aSample.setTable(THINKBROTHERELEVEN_DATA); currentLength = THINKBROTHERELEVEN_NUM_CELLS; break;
			case 11: aSample.setTable(THINKBROTHERTWELVE_DATA); currentLength = THINKBROTHERTWELVE_NUM_CELLS; break;
			case 12: aSample.setTable(THINKBROTHERTHIRTEEN_DATA); currentLength = THINKBROTHERTHIRTEEN_NUM_CELLS; break;
			case 13: aSample.setTable(THINKBROTHERFOURTEEN_DATA); currentLength = THINKBROTHERFOURTEEN_NUM_CELLS; break;
			case 14: aSample.setTable(THINKBROTHERFIFTEEN_DATA); currentLength = THINKBROTHERFIFTEEN_NUM_CELLS; break;
			case 15: aSample.setTable(THINKBROTHERSIXTEEN_DATA); currentLength = THINKBROTHERSIXTEEN_NUM_CELLS; break;
		}
		break;
	}
}

// --------------------------
// TRIGGER SAMPLE
// --------------------------

void triggerSample(bool randomTrigger) {
    int potStretch = analogRead(A1);
    int cvStretch = analogRead(A2) - 512;

    int potMult = analogRead(A3);
    int cvMult = analogRead(A4) - 512;

    int potPitch = analogRead(A5);
    int cvPitch = analogRead(A6) - 512;

    int potSample = analogRead(A7);
    int cvSample = analogRead(A8) - 512;

    breakMode = digitalRead(4);

    frozenStretch = mapfloat(constrain(potStretch + cvStretch, 0, 1023), 0, 1023, 0.125, 3.0);

    int multRaw = constrain(potMult + cvMult, 0, 1023);
    int multIndex = map(multRaw, 0, 1023, 0, 7);
    frozenMultiplier = multTable[multIndex];

    frozenPitch = mapfloat(constrain(potPitch + cvPitch, 0, 1023), 0, 1023, 0.75, 1.5);

    if (randomTrigger) {
        frozenSample = rand(SAMPLE_COUNT); // Change to sample count here (CONSTANTLY forgot this in dev)
    } else {
        frozenSample = map(constrain(potSample + cvSample, 0, 1023), 0, 1023, 0, SAMPLE_COUNT-1); // Change this too
    }

    setSample(breakMode, frozenSample);

    increment = frozenStretch * frozenMultiplier * frozenPitch;

    if (increment >= 0)
        phase = 0;
    else
        phase = currentLength - 1;

    playing = true;
}

// --------------------------
// SETUP
// --------------------------

void setup() {
    randSeed();

    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);

    pinMode(13, OUTPUT);
    pinMode(A9, OUTPUT);

    //digitalWrite(13, HIGH);
    startMozzi(MOZZI_CONTROL_RATE);
}

// --------------------------
// CONTROL
// --------------------------

void updateControl() {
		// swapped these ins from the last version.
		// this whole section needs cleaning for 1.1.0
    trigFixed = digitalRead(3);
    trigRandom = digitalRead(2);

    bool fixedEvent = false;
    bool randomEvent = false;

    if (trigFixed != lastTrigFixed && trigFixed == HIGH) fixedEvent = true;
    if (trigRandom != lastTrigRandom && trigRandom == HIGH) randomEvent = true;

    lastTrigFixed = trigFixed;
    lastTrigRandom = trigRandom;

    if (randomEvent)
        triggerSample(true);
    else if (fixedEvent)
        triggerSample(false);
}

// --------------------------
// AUDIO
// --------------------------

AudioOutput updateAudio() {
    int out = 0;

    if (playing) {
        int index = (int) phase;

        if (index >= 0 && index < currentLength) {
            out = aSample.atIndex(index) * 120;
            phase += increment;
        } else {
            playing = false;
        }
    }

    analogWrite(A9, abs(out) >> 4);

    return MonoOutput::from16Bit(out);
}

// --------------------------
// LOOP
// --------------------------

void loop() {
    audioHook();
}