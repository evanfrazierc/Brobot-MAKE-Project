/*************************************************************
//  Brobot: Header File
//  This file contains the Brobot class function and variable 
// 	declarations as well as the led bitmaps for each emotion
//  Programmer: Chris Frazier
//  Date: 4/8/2015
**************************************************************/

#ifndef BROBOT_H
#define BROBOT_H

#include "Arduino.h"
#include "C:\Users\ChristopherEvan\Documents\Arduino\libraries\Wire\Wire.h"
#include "C:\Users\ChristopherEvan\Documents\Arduino\libraries\SoftwareServo\SoftwareServo.h"
#include "C:\Users\ChristopherEvan\Documents\Arduino\libraries\Adafruit_LEDBackpack\Adafruit_LEDBackpack.h"
#include "C:\Users\ChristopherEvan\Documents\Arduino\libraries\Adafruit_GFX\Adafruit_GFX.h"
#include "C:\Users\ChristopherEvan\Documents\Arduino\libraries\WaveHC\FatReader.h"
#include "C:\Users\ChristopherEvan\Documents\Arduino\libraries\WaveHC\SdReader.h"
#include <avr/pgmspace.h>
#include "C:\Users\ChristopherEvan\Documents\Arduino\libraries\WaveHC\WaveUtil.h"
#include "C:\Users\ChristopherEvan\Documents\Arduino\libraries\WaveHC\WaveHC.h"
#include "C:\Users\ChristopherEvan\Documents\Arduino\libraries\LiquidCrystal_I2C\LiquidCrystal_I2C.h"

// Each emotion is represented by an integer from 0 - 6
#define NEUTRAL 0xF0
#define HAPPY 0xF1
#define SAD 0xF2
#define ANGRY 0xF3
#define SURPRISED 0xF4
#define LOVE 0xF5
#define BLINK 0xF6
#define NECK_SPEED 1
#define MAX_TILT_ANGLE 135
#define MIN_TILT_ANGLE 65
#define MAX_PAN_ANGLE 120
#define MIN_PAN_ANGLE 40

class Brobot{
public:
	Brobot(byte, byte, byte, byte, byte, byte); //Constructor
	void setup();
	void setEmotion(byte); // Allows main file to access current emotion
	void updateFace();
	bool checkProx();
	void playComplete();
	void playFile(char*);
	byte freeRam();
	void sdErrorCheck();
	void checkBT();
	SoftwareServo pan;
	SoftwareServo tilt;
	SoftwareServo leftBrow;
	SoftwareServo rightBrow;
	byte currentEmotion;
	byte nextEmotion;
	byte incomingByte;
	byte currentPanAngle;
	byte currentTiltAngle;
	byte nextPanAngle;
	byte nextTiltAngle;
	byte currentLBAngle;
	byte currentRBAngle;
	byte nextLBAngle;
	byte nextRBAngle;
	byte neckSpeed;
	byte neckTimer;
	bool headMoveToggle;
	bool upToggle;
	bool downToggle;
	bool leftToggle;
	bool rightToggle;
	bool soundToggle;
	bool togglePause;
	SdReader card;    // This object holds the information for the card
	FatVolume vol;    // This holds the information for the partition on the card
	FatReader root;   // This holds the information for the filesystem on the card
	FatReader f;      // This holds the information for the file we're play
	WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time
private:
	void displayString(const char *);
	byte previousEmotion;
	byte blinkTimer;
	byte surpriseTimer;
	byte triggerPin;
	byte echoPin;
	bool _setup;
	byte randSound;
	byte k;
	char myChar;
};

#endif

static const uint8_t PROGMEM
happyL_bmp[] =
{ B00000000,
B00011100,
B00100100,
B01011100,
B01011100,
B00100100,
B00011100,
B00000000 },
happyR_bmp[] =
{ B00000000,
B00011100,
B00100100,
B01011100,
B01011100,
B00100100,
B00011100,
B00000000 },
neutralL_bmp[] =
{ B00000000,
B00111100,
B01000010,
B01011010,
B01011010,
B01000010,
B00111100,
B00000000 },
neutralR_bmp[] =
{ B00000000,
B00111100,
B01000010,
B01011010,
B01011010,
B01000010,
B00111100,
B00000000 },
angryL_bmp[] =
{ B00000000,
B00001100,
B00010010,
B00111010,
B01011010,
B11000010,
B00111100,
B00000000 },
angryR_bmp[] =
{ B00000000,
B00111100,
B11000010,
B01011010,
B00111010,
B00010010,
B00001100,
B00000000 },
surprisedL_bmp[] =
{ B01111110,
B10000001,
B10000001,
B10011001,
B10011001,
B10000001,
B10000001,
B01111110 },
surprisedR_bmp[] =
{ B01111110,
B10000001,
B10000001,
B10011001,
B10011001,
B10000001,
B10000001,
B01111110 },
sadL_bmp[] =
{ B00000000,
B00111100,
B01000010,
B01011010,
B00111010,
B00010010,
B00001100,
B00000000 },
sadR_bmp[] =
{ B00000000,
B00001100,
B00010010,
B00111010,
B01011010,
B01000010,
B00111100,
B00000000 },
loveL_bmp[] =
{ B01111000,
B10000100,
B10000010,
B01011001,
B01011001,
B10000010,
B10000100,
B01111000 },
loveR_bmp[] =
{ B01111000,
B10000100,
B10000010,
B01011001,
B01011001,
B10000010,
B10000100,
B01111000 },
blink_bmp[] =
{ B00000000,
B00000100,
B00000010,
B00000010,
B00000010,
B00000010,
B00000100,
B00000000 };

