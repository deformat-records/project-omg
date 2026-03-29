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
    
			   BRØTHER v0.9.0                       
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
	Random Mode ==> Pin 2
	Trigger Fixed Sample (Bang!) ==> Pin 3
	Trigger Random (Bang!) ==> Pin 4 

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

// NOTE
	
	This version has a "random" mode which makes the
	standard trigger behave as a random trigger.
	In later versions, this was swapped out to allow for
	a second sample bank.

*/
#define MOZZI_CONTROL_RATE 256
#define SAMPLE_COUNT 16

#include <ADC.h>
#include <Mozzi.h>
#include <Sample.h>
#include <mozzi_rand.h>

// --------------------------
// SAMPLE TABLES
// --------------------------

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

// --------------------------
// SAMPLE OBJECT
// --------------------------

Sample < AMENBROTHERSIXTEEN_NUM_CELLS, AUDIO_RATE > aSample(AMENBROTHERONE_DATA);

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
int randomMode = 0;

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

void setSample(int index)
{
	switch(index)
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

    frozenStretch = mapfloat(constrain(potStretch + cvStretch, 0, 1023), 0, 1023, 0.125, 3.0);

    int multRaw = constrain(potMult + cvMult, 0, 1023);
    int multIndex = map(multRaw, 0, 1023, 0, 7);
    frozenMultiplier = multTable[multIndex];

    frozenPitch = mapfloat(constrain(potPitch + cvPitch, 0, 1023), 0, 1023, 0.75, 1.5);

    if (randomTrigger) {
        frozenSample = rand(SAMPLE_COUNT); // Change to sample count here (CONSTANTLY forgot this in dev)
    } else {
        frozenSample = map(constrain(potSample + cvSample, 0, 1023), 0, 1023, 0, SAMPLE_COUNT-1); // Change this number too
    }

    setSample(frozenSample);

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
    randomMode = digitalRead(2);

    trigFixed = digitalRead(3);
    trigRandom = digitalRead(4);

    bool fixedEvent = false;
    bool randomEvent = false;

    if (trigFixed != lastTrigFixed && trigFixed == HIGH) fixedEvent = true;
    if (trigRandom != lastTrigRandom && trigRandom == HIGH) randomEvent = true;

    lastTrigFixed = trigFixed;
    lastTrigRandom = trigRandom;

    if (randomMode == HIGH) {
        bool temp = fixedEvent;
        fixedEvent = randomEvent;
        randomEvent = temp;
    }

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