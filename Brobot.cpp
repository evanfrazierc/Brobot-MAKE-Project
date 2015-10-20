/*************************************************************
//  Brobot: Source File
//  This file contains the Brobot class function 
// 	definitions
//  Programmer: Chris Frazier
//  Date: 4/8/2015
**************************************************************/

#include "Brobot.h"

// Create led matrix instance using Adafruit's library
Adafruit_8x16matrix ledmatrix = Adafruit_8x16matrix();
LiquidCrystal_I2C myDisplay = LiquidCrystal_I2C(0x27, 16, 2);
// save some messages
const char emotion[] PROGMEM  = 	{"Emotion:        "};
const char blank[] PROGMEM  = 		{"                "};
const char hello[] PROGMEM  = 		{"Hello,          "};
const char imBrobot[] PROGMEM  = 	{"I'm Brobot!     "};
const char neutral[] PROGMEM  = 	{"NEUTRAL         "};
const char happy[] PROGMEM  = 		{"HAPPY           "};
const char sad[] PROGMEM  = 		{"SAD             "};
const char angry[] PROGMEM  = 		{"ANGRY           "};
const char surprised[] PROGMEM  = 	{"SURPRISED       "};
const char love[] PROGMEM  = 		{"LOVE            "};
// Constructor: lets user set the pins
Brobot::Brobot(byte pp, byte tp, byte lbp, byte rbp, byte ep, byte trgp){
	triggerPin = trgp;
	echoPin = ep;
	neckTimer = 0;
	neckSpeed = NECK_SPEED;
	previousEmotion = NEUTRAL;
	currentEmotion = HAPPY;
	nextEmotion = HAPPY;
	pinMode(echoPin, INPUT); //set pinmodes for prox sensor
	pinMode(triggerPin, OUTPUT);
	soundToggle = false;
	togglePause = false;
}

void Brobot::displayString(const char *message){
	for (k = 0; k < 15; k++)
    {
		myChar =  pgm_read_byte_near(message + k);
		myDisplay.print(myChar);
    }
}

// Used during setup; initializes Brobot's face
void Brobot::setup() {
	ledmatrix.begin(0x70);
	
	myDisplay.init(); //initialize the lcd
	myDisplay.backlight();//this turns the backlight on
	displayString(hello);
	//this sets the cursor of the display to the second row and 9th character position in that row
	myDisplay.setCursor(1,8);
	//then we print at the cursor position
	displayString(imBrobot);
	playComplete();
	delay(2000);
	myDisplay.setCursor(0,0);
	displayString(emotion);
    myDisplay.setCursor(1,8);
    displayString(neutral);
	_setup = true;
	updateFace();
	_setup = false;
}

// Allows main file access to next emotion
void Brobot::setEmotion(byte i) {
	nextEmotion = i;
}

// Decides which emotion to display on Brobot's face
void Brobot::updateFace() {
	// Generates random blinks
	if(currentEmotion != BLINK) {
		if(random(1000) == 0) {
			nextEmotion = BLINK;
			blinkTimer = 50;
		}
	} // decrements blink timer to make sure Brobot doesn't fall asleep!
	else if (currentEmotion == BLINK) {
		if(blinkTimer > 0) {
			blinkTimer--;
		}
		else {
		// Return to previous emotion after blink
		nextEmotion = previousEmotion;
		}
	} // check the prox sensor and surprise Brobot
	if(!_setup && !togglePause){
		if (checkProx()) {
			nextEmotion = SURPRISED;
			surpriseTimer = 100;
		} // make sure Brobot doesn't go into shock!
		else if(!checkProx()){
			if(surpriseTimer > 0) {
				surpriseTimer--;
			}
			else if(currentEmotion != BLINK){
				if (nextEmotion == currentEmotion) {
					// Return to previous emotion after surprise
					nextEmotion = previousEmotion;
				}
			}
		}
	}
	
	// Change emotions if next emotion is different
	// This prevents from updating every loop and wasting power
	if (nextEmotion != currentEmotion) {
		if (nextEmotion == NEUTRAL) {
			ledmatrix.clear();
			ledmatrix.drawBitmap(0, 0, neutralR_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			ledmatrix.drawBitmap(0, 8, neutralL_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			currentEmotion = NEUTRAL;
			previousEmotion = NEUTRAL;
			myDisplay.setCursor(1,8);
            displayString(neutral);
			soundToggle = true;
		}
		else if (nextEmotion == HAPPY) {
			ledmatrix.clear();
			ledmatrix.drawBitmap(0, 0, happyR_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			ledmatrix.drawBitmap(0, 8, happyL_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			currentEmotion = HAPPY;
			previousEmotion = HAPPY;
			myDisplay.setCursor(1,8);
            displayString(happy);
			soundToggle = true;
		}
		else if (nextEmotion == SAD) {
			ledmatrix.clear();
			ledmatrix.drawBitmap(0, 0, sadR_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			ledmatrix.drawBitmap(0, 8, sadL_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			currentEmotion = SAD;
			previousEmotion = SAD;
			myDisplay.setCursor(1,8);
            displayString(sad);
			soundToggle = true;
		}
		else if (nextEmotion == ANGRY) {
			ledmatrix.clear();
			ledmatrix.drawBitmap(0, 0, angryR_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			ledmatrix.drawBitmap(0, 8, angryL_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			currentEmotion = ANGRY;
			previousEmotion = ANGRY;
			myDisplay.setCursor(1,8);
            displayString(angry);
			soundToggle = true;
		}
		else if (nextEmotion == SURPRISED) {
			ledmatrix.clear();
			ledmatrix.drawBitmap(0, 0, surprisedR_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			ledmatrix.drawBitmap(0, 8, surprisedL_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			currentEmotion = SURPRISED;
			myDisplay.setCursor(1,8);
            displayString(surprised);
			soundToggle = true;
		}
		else if (nextEmotion == LOVE) {
			ledmatrix.clear();
			ledmatrix.drawBitmap(0, 0, loveR_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			ledmatrix.drawBitmap(0, 8, loveL_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			currentEmotion = LOVE;
			previousEmotion = LOVE;
			myDisplay.setCursor(1,8);
            displayString(love);
			soundToggle = true;
		}
		else if (nextEmotion == BLINK) {
			ledmatrix.clear();
			ledmatrix.drawBitmap(0, 0, blink_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			ledmatrix.drawBitmap(0, 8, blink_bmp, 8, 8, LED_ON);
			ledmatrix.writeDisplay();
			currentEmotion = BLINK;
		}
	}SoftwareServo::refresh();
}

// Check and see if someone is too close to Brobot
bool Brobot::checkProx(){
	digitalWrite(triggerPin, HIGH); // make a 10usec pulse
	delayMicroseconds(10);

	digitalWrite(triggerPin, LOW);

	float distance = pulseIn(echoPin, HIGH);  //now read the pulse that is sent back by the sensor
	//pulseIn returns the pulse length in usec

	distance = distance / 58;
	if (distance < 6){
		return true;
	}
	else{ 
		return false;
	}
}

void Brobot::checkBT(){
	while (Serial.available()) {
    delay(3);  
    incomingByte = Serial.read();
	}
	if ((incomingByte < 0xFE) && (incomingByte >= 0xE0)) {
		// Set emotion corresponding to byte received
		if (incomingByte > -1) {
		  Serial.println(incomingByte);
		  setEmotion(incomingByte);
		  if(incomingByte == 0xE0) {
		  headMoveToggle = !headMoveToggle;
		  }
		  if(incomingByte == 0xE1){
			upToggle = !upToggle;
		  }
		  else if(incomingByte == 0xE3){
			leftToggle = !leftToggle;
		  }
		  else if(incomingByte == 0xE2){
			rightToggle = !rightToggle;
		  }
		  else if(incomingByte == 0xE4){
			downToggle = !downToggle;
		  }
		}
	incomingByte = -1;
	}
}

// Plays a full file from beginning to end with no pause.
void Brobot::playComplete() {
  // call our helper to find and play this name
  //SoftwareServo::refresh();
      //delay(30);

	if(currentEmotion == NEUTRAL){
		randSound = random(1,15);
		if(randSound == 1) {
			playFile("NEUT1.WAV");
		}
		else if(randSound == 2){
			playFile("NEUT2.WAV");
		}
  }
  else if(currentEmotion == HAPPY){
	randSound = random(1,3);
	if(randSound == 1) {
		playFile("HAPPY1.WAV");
	}
	else if(randSound == 2){
		playFile("HAPPY2.WAV");
	}
	else if(randSound == 3){
		playFile("HAPPY3.WAV");
	}
  }
  else if(currentEmotion == SAD){
	randSound = random(1,3);
	if(randSound == 1) {
		playFile("SAD1.WAV");
	}
	else if(randSound == 2){
		playFile("SAD2.WAV");
	}
	else if(randSound == 3){
		playFile("SAD3.WAV");
	}
	else if(randSound == 4){
		playFile("SAD4.WAV");
	}
  }
  else if(currentEmotion == ANGRY){
	randSound = random(1,3);
	if(randSound == 1) {
		playFile("ANGRY1.WAV");
	}
	else if(randSound == 2){
		playFile("ANGRY2.WAV");
	}
	else if(randSound == 3){
		playFile("ANGRY3.WAV");
	}
  }
  else if(currentEmotion == SURPRISED){
	randSound = random(1,6);
	if(randSound == 1) {
		playFile("SURP1.WAV");
	}
	else if(randSound == 2){
		playFile("SURP2.WAV");
	}
	else if(randSound == 3){
		playFile("SURP3.WAV");
	}
	else if(randSound == 4){
		playFile("SURP4.WAV");
	}
	else if(randSound == 5){
		playFile("SURP6.WAV");
	}
	else if(randSound == 6){
		playFile("SURP6.WAV");
	}
  }
  else if(currentEmotion == LOVE){
	randSound = random(1,3);
	if(randSound == 1) {
		playFile("LOVE1.WAV");
	}
	else if(randSound == 2){
		playFile("LOVE2.WAV");
	}
	else if(randSound == 3){
		playFile("LOVE3.WAV");
	}
  }soundToggle = false;

  while(wave.isplaying) {
	  // pause
  }
  // now its done playing
}
 
void Brobot::playFile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
}
// this handy function will return the number of bytes currently free in RAM, great for debugging!   
byte Brobot::freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 

void Brobot::sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}
  