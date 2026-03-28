/*
________/\\\\\\\\\__/\\\________/\\\_____/\\\\\\\\\__________/\\\\\\__//_____/\\\\\\\\\\\___        
 _____/\\\////////__\/\\\_______\/\\\___/\\\\\\\\\\\\\______/\\\///\\\/_____/\\\/////////\\\_       
  ___/\\\/___________\/\\\_______\/\\\__/\\\/////////\\\___/\\\/____//\\\___\//\\\______\///__      
   __/\\\_____________\/\\\\\\\\\\\\\\\_\/\\\_______\/\\\__/\\\____//\//\\\___\////\\\_________     
    _\/\\\_____________\/\\\/////////\\\_\/\\\\\\\\\\\\\\\_\/\\\__//___\/\\\______\////\\\______    
     _\//\\\____________\/\\\_______\/\\\_\/\\\/////////\\\_\//\\//_____/\\\__________\////\\\___   
      __\///\\\__________\/\\\_______\/\\\_\/\\\_______\/\\\__\\//\\\__/\\\_____/\\\______\//\\\__  
       ____\////\\\\\\\\\_\/\\\_______\/\\\_\/\\\_______\/\\\__//\///\\\\\/_____\///\\\\\\\\\\\/___ 
        _______\/////////__\///________\///__\///________\//////____\/////_________\///////////_____

          CHAØS v1.0.0
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
  Chaos Toggle ==> Pin 2
  Trigger Voice 1 (Bang!) ==> Pin 3
  Trigger Voice 2 (Bang!) ==> Pin 4 
  
  /////DIGITAL OUTS//////
  LED Teensy Power Indicator ==> Pin 13
  
  /////ANALOG INS - POTS//////
  Voice 1 CV Pitch (Speed) ==> Pin 15 (A1)
  Voice 2 CV Decay (Head) ==> Pin 17 (A3)
  Voice 2 Pitch Pot (Tail) ==> Pin 19 (A5)
  Voice 1 Decay Pot (Specimen) ==> Pin 21 (A7)

  /////ANALOG INS - JACKS//////
  Voice 1 CV Pitch ==> Pin 16 (A2)
  Voice 2 CV Decay ==> Pin 18 (A4)
  Voice 2 CV Pitch ==> Pin 20 (A6)
  Voice 1 CV Decay ==> Pin 22 (A8)
  
  /////ANALOG OUTS//////
  Audio Ouput == > DAC/A14 
  LED Audio Indicator ==> Pin 23 (A9)
  
/////////////////////////////////////////////////////////////////
            TODO 
///////////////////////////////////////////////////////////////// 

Make the third voice/accent more pronounced.
Make chaos mode change values more.
Perhaps:
 add chaosDecay = 1.0
 add chaosPitch = 1.0
when off, alwasy this value, when on,
when collision trigger happens, set a random value. (or chance on any trigger as well)
These are multiplied to each voice.
etc. 

*/
#define MOZZI_CONTROL_RATE 256

#include <ADC.h>
#include <Mozzi.h>
#include <Oscil.h>

#include <tables/whitenoise8192_int8.h>
#include <tables/brownnoise8192_int8.h>
#include <StateVariable.h>
#include <Ead.h>
#include <mozzi_rand.h>

Oscil < WHITENOISE8192_NUM_CELLS, AUDIO_RATE > aNoise(WHITENOISE8192_DATA);
Oscil < BROWNNOISE8192_NUM_CELLS, AUDIO_RATE > bNoise(BROWNNOISE8192_DATA);
Oscil < WHITENOISE8192_NUM_CELLS, AUDIO_RATE > glitchNoise(WHITENOISE8192_DATA);

Ead envelopeOne(MOZZI_CONTROL_RATE);
Ead envelopeTwo(MOZZI_CONTROL_RATE);
Ead collisionEnv(MOZZI_CONTROL_RATE);

StateVariable < BANDPASS > filter1;
StateVariable < HIGHPASS > filter2;

int trigOneState = 0;
int trigTwoState = 0;
int lastTrigOneState = 0;
int lastTrigTwoState = 0;
int toggle = 0;

unsigned long trigOneTime = 0;
unsigned long trigTwoTime = 0;
unsigned long collisionUntil = 0;

bool trigBoth = false;
bool collisionHandled = false;

int gainOne;
int gainTwo;
int collisionGain = 0;

int pitchPotOne, pitchPotTwo;
int pitchOneCvRaw, pitchTwoCvRaw;
int pitchOneMath, pitchTwoMath;

int decayPotOne, decayPotTwo;
int decayOneCvRaw, decayTwoCvRaw;
int decayOneMath, decayTwoMath;

float pitchOne;
float pitchTwo;

int decayOne;
int decayTwo;
int collisionDecay;

int filterFreq1;
int filterFreq2;

bool glitchAccent = false;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  randSeed();

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);

  pinMode(13, OUTPUT); // audio out
  pinMode(A9, OUTPUT); // light

  startMozzi(MOZZI_CONTROL_RATE);
}

void updateControl() {

  aNoise.setPhase(rand((unsigned int) WHITENOISE8192_NUM_CELLS));
  bNoise.setPhase(rand((unsigned int) BROWNNOISE8192_NUM_CELLS));

  trigOneState = digitalRead(3);
  trigTwoState = digitalRead(2);
  toggle = digitalRead(4);

  // --------------------------
  // DRUM 1
  // --------------------------

  decayPotOne = analogRead(A7);
  decayOneCvRaw = analogRead(A8) - 512;
  decayOneMath = constrain(decayPotOne - decayOneCvRaw, 1, 1023);
  decayOne = map(decayOneMath, 1, 1023, 5, 500);

  if (trigOneState != lastTrigOneState && trigOneState == HIGH) {
    trigOneTime = millis();
    envelopeOne.start(10, decayOne + rand(50));
  }

  lastTrigOneState = trigOneState;

  pitchPotOne = analogRead(A1);
  pitchOneCvRaw = analogRead(A2) - 512;
  pitchOneMath = constrain(pitchPotOne - pitchOneCvRaw, 1, 1023);
  pitchOne = mapfloat(pitchOneMath, 1, 1023, 0.02, 1.5);

  // --------------------------
  // DRUM 2
  // --------------------------

  decayPotTwo = analogRead(A3);
  decayTwoCvRaw = analogRead(A4) - 512;
  decayTwoMath = constrain(decayPotTwo - decayTwoCvRaw, 1, 1023);
  decayTwo = map(decayTwoMath, 1, 1023, 5, 500);

  if (trigTwoState != lastTrigTwoState && trigTwoState == HIGH) {
    trigTwoTime = millis();
    envelopeTwo.start(10, decayTwo + rand(50));
  }

  lastTrigTwoState = trigTwoState;

  pitchPotTwo = analogRead(A5);
  pitchTwoCvRaw = analogRead(A6) - 512;
  pitchTwoMath = constrain(pitchPotTwo - pitchTwoCvRaw, 1, 1023);
  pitchTwo = mapfloat(pitchTwoMath, 1, 1023, 0.02, 1.5);

  // --------------------------
  // FILTER PERSONALITY
  // --------------------------

  if (toggle == HIGH) {
    filterFreq1 = map(pitchOneMath, 1, 1023, 90, 2600);
    filterFreq2 = map(pitchTwoMath, 1, 1023, 70, 2200);
  } else {
    filterFreq1 = map(pitchOneMath, 1, 1023, 120, 3500);
    filterFreq2 = map(pitchTwoMath, 1, 1023, 100, 2800);
  }

  filter1.setResonance(130);
  filter2.setResonance(130);
  trigBoth = false;
  glitchAccent = false;

  // --------------------------
  // COLLISION
  // --------------------------

  if (abs((long)(trigOneTime - trigTwoTime)) < 10 && !collisionHandled) {

    trigBoth = true;
    collisionHandled = true;

    collisionDecay = constrain((decayOne + decayTwo) / 4, 20, 180);

    collisionEnv.start(2, collisionDecay);
    collisionUntil = millis() + 15;

    int chosen;

    if (toggle == LOW) {
      chosen = rand(4);
    } else {
      chosen = rand(2) + 2;
    }

    if (chosen == 0) {
      if (toggle == LOW) {
        glitchNoise.setFreq(rand(600));
      } else {
        glitchAccent = true;
        glitchNoise.setFreq(rand(2500));
        filter1.setResonance(180);
        filter2.setResonance(180);
      }
    } else if (chosen == 1) {
      if (toggle == LOW) {
        glitchNoise.setFreq(rand(900));
      } else {
        glitchNoise.setFreq(rand(2100));
        filter1.setResonance(130);
        filter2.setResonance(130);
      }
    } else if (chosen == 2) {
      if (toggle == LOW) {
        pitchOne *= 0.8;
        pitchTwo *= 1.2;
        glitchNoise.setFreq(rand(1500));
      } else {
        glitchNoise.setFreq(rand(1100));
        filter1.setResonance(110);
        filter2.setResonance(110);
      }
    } else {
      pitchOne *= 1.1;
      pitchTwo *= 1.4;
      if (toggle == LOW) {
        glitchNoise.setFreq(rand(1100));
      } else {
        glitchNoise.setFreq(rand(2100));
        filter1.setResonance(170);
        filter2.setResonance(170);
      }
    }

    glitchNoise.setPhase(rand((unsigned int) WHITENOISE8192_NUM_CELLS));
    digitalWrite(13, HIGH);
  }

  if (trigOneState == LOW && trigTwoState == LOW) {
    collisionHandled = false;
    digitalWrite(13, LOW);
  }

  gainOne = envelopeOne.next();
  gainTwo = envelopeTwo.next();
  collisionGain = collisionEnv.next();

  aNoise.setFreq(pitchOne + gainOne * 0.002);
  bNoise.setFreq(pitchTwo + gainTwo * 0.002);

  filter1.setCentreFreq(filterFreq1);
  filter2.setCentreFreq(filterFreq2);
}

AudioOutput updateAudio() {
  int voice1 = gainOne * filter1.next(aNoise.next());
  int voice2 = gainTwo * filter2.next(bNoise.next());

  int glitch = 0;

  if (glitchAccent && millis() < collisionUntil) {
    glitch = (glitchNoise.next() * collisionGain) >> 7;
  }

  int out = (voice1 >> 2) + (voice2 >> 2) + glitch;

  if (toggle == HIGH) out &= 0xFFF8;
  else out &= 0xFFFC;

  analogWrite(A9, abs(out) >> 4);

  return MonoOutput::from16Bit(out);
}

void loop() {
  audioHook();
}