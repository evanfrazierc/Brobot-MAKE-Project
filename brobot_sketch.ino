/*************************************************************
//  Brobot: Main File
//  This file contains the setup and execution loop for the Arduino
//  while Brobot is in operation.
//  Programmer: Chris Frazier
//  Date: 4/8/2015
**************************************************************/

#include "Wire.h"
#include "SoftwareServo.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <Brobot.h>
#include <LiquidCrystal_I2C.h>
#include "WaveHC.h"

#define panPin 14  // This is the pan servo pin
#define tiltPin 15 // This is the tilt servo pin
#define leftBrowPin 16  // This is the left eyebrow servo pin
#define rightBrowPin 17 // This is the right eyebrow servo pin
#define echoPin 8  // This is the echo pin
#define triggerPin 7 // This is the trigger pin

// Create an instance of Brobot
Brobot myBrobot(panPin, tiltPin, leftBrowPin, rightBrowPin, echoPin, triggerPin);

void setup() {
  Serial.begin(9600); // Set baud rate for serial protocol
  
  putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(myBrobot.freeRam());      // if this is under 150 bytes it may spell trouble!
  
  // Set the output pins for the DAC control. This pins are defined in the library
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!myBrobot.card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    myBrobot.sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  myBrobot.card.partialBlockRead(true);
  
  // Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (myBrobot.vol.init(myBrobot.card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    myBrobot.sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(myBrobot.vol.fatType(),DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!myBrobot.root.openRoot(myBrobot.vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }
  // Initialize all of Brobot's public variables
  myBrobot.incomingByte = -1;
  myBrobot.currentPanAngle = 90;
  myBrobot.currentTiltAngle = 90;
  myBrobot.nextPanAngle = 90;
  myBrobot.nextTiltAngle = 90;
  myBrobot.currentLBAngle = 90;
  myBrobot.currentRBAngle = 90;
  myBrobot.neckTimer = 400;
  myBrobot.neckSpeed = NECK_SPEED;
  myBrobot.headMoveToggle = 0;
  myBrobot.leftToggle = 0;
  myBrobot.rightToggle = 0;
  myBrobot.upToggle = 0;
  myBrobot.downToggle = 0;
  
  // Run Brobot setup function (initializes LED eyes)
  myBrobot.setup();
  
  // Attach and initialize all 4 servos
  myBrobot.pan.attach(panPin);
  myBrobot.tilt.attach(tiltPin);
  myBrobot.leftBrow.attach(leftBrowPin);
  myBrobot.rightBrow.attach(rightBrowPin);
  myBrobot.pan.write(myBrobot.currentPanAngle);
  delay(20);
  myBrobot.tilt.write(myBrobot.currentTiltAngle);
  myBrobot.leftBrow.write(myBrobot.currentLBAngle);
  myBrobot.rightBrow.write(myBrobot.currentRBAngle);
  //SoftwareServo::refresh();
  delay(20);
  
  
  myBrobot.updateFace();
  
  delay(2000);
  SoftwareServo::refresh();
}
    
void loop() 
{ 
  // Every loop check for Bluetooth data
  myBrobot.checkBT();
  
  // The following code sets Brobot's eyebrow positions
  //  - -
  if(myBrobot.nextEmotion == NEUTRAL) {
      //myBrobot.togglePause = true;
      myBrobot.nextLBAngle = 90;
      myBrobot.nextRBAngle = 90;
      while((myBrobot.currentLBAngle != myBrobot.nextLBAngle) || (myBrobot.currentRBAngle != myBrobot.nextRBAngle)){
        if(myBrobot.currentLBAngle < myBrobot.nextLBAngle){
           myBrobot.currentLBAngle++;
        }
        else if(myBrobot.currentLBAngle > myBrobot.nextLBAngle){
           myBrobot.currentLBAngle--;
        }
        if(myBrobot.currentRBAngle < myBrobot.nextRBAngle){
           myBrobot.currentRBAngle++;
        }
        else if(myBrobot.currentRBAngle > myBrobot.nextRBAngle){
           myBrobot.currentRBAngle--;
        }
        myBrobot.leftBrow.write(myBrobot.currentLBAngle);
        myBrobot.rightBrow.write(myBrobot.currentRBAngle);
        SoftwareServo::refresh();
      }
      //if((myBrobot.currentLBAngle == myBrobot.nextLBAngle) && (myBrobot.currentRBAngle == myBrobot.nextRBAngle)){
      //   myBrobot.togglePause = false;
      //}
  }
  
  //  / \
  else if(myBrobot.nextEmotion == SURPRISED || myBrobot.nextEmotion == HAPPY || myBrobot.nextEmotion == SAD || myBrobot.nextEmotion == LOVE) {
      //myBrobot.togglePause = true;
      myBrobot.nextLBAngle = 120;
      myBrobot.nextRBAngle = 60;
      while((myBrobot.currentLBAngle != myBrobot.nextLBAngle) || (myBrobot.currentRBAngle != myBrobot.nextRBAngle)){
        if(myBrobot.currentLBAngle < myBrobot.nextLBAngle){
           myBrobot.currentLBAngle++;
        }
        else if(myBrobot.currentLBAngle > myBrobot.nextLBAngle){
           myBrobot.currentLBAngle--;
        }
        if(myBrobot.currentRBAngle < myBrobot.nextRBAngle){
           myBrobot.currentLBAngle++;
        }
        else if(myBrobot.currentRBAngle > myBrobot.nextRBAngle){
           myBrobot.currentRBAngle--;
        }
        myBrobot.leftBrow.write(myBrobot.currentLBAngle);
        myBrobot.rightBrow.write(myBrobot.currentRBAngle);
        SoftwareServo::refresh();
      }
      //if((myBrobot.currentLBAngle == myBrobot.nextLBAngle) && (myBrobot.currentRBAngle == myBrobot.nextRBAngle)){
      //   myBrobot.togglePause = false;
      //}
  }
  //  \ /
  else if(myBrobot.nextEmotion == ANGRY) {
      //myBrobot.togglePause = true;
      myBrobot.nextLBAngle = 60;
      myBrobot.nextRBAngle = 120;
      while((myBrobot.currentLBAngle != myBrobot.nextLBAngle) || (myBrobot.currentRBAngle != myBrobot.nextRBAngle)){
        if(myBrobot.currentLBAngle < myBrobot.nextLBAngle){
           myBrobot.currentLBAngle++;
        }
        else if(myBrobot.currentLBAngle > myBrobot.nextLBAngle){
           myBrobot.currentLBAngle--;
        }
        if(myBrobot.currentRBAngle < myBrobot.nextRBAngle){
           myBrobot.currentRBAngle++;
        }
        else if(myBrobot.currentRBAngle > myBrobot.nextRBAngle){
           myBrobot.currentRBAngle--;
        }
        myBrobot.leftBrow.write(myBrobot.currentLBAngle);
        myBrobot.rightBrow.write(myBrobot.currentRBAngle);
        SoftwareServo::refresh();
      }
      //if((myBrobot.currentLBAngle == myBrobot.nextLBAngle) && (myBrobot.currentRBAngle == myBrobot.nextRBAngle)){
      //   myBrobot.togglePause = false;
      //}
  }
  SoftwareServo::refresh();
  // Update Brobot's face each loop
  
  myBrobot.updateFace();
  
  /* The following code controls the pan/tilt neck servos
  // Brobot will randomly look in a defined set of angles for both pan and tilt.
  // Each loop, if the current servo position variable is not at the same position 
  // as the next position variable, the servos will increment/decrement the current 
  // angle by 1 as needed. The neckSpeed variable causes the pan/tilt servos to only 
  // change every *neckspeed* loop cycle. The neckTimer variable causes Brobot to pause
  // before looking in another direction.
  */
  
  if(myBrobot.headMoveToggle == 0){
    if (myBrobot.currentPanAngle == myBrobot.nextPanAngle){
      if(myBrobot.neckTimer == 0){
        myBrobot.nextPanAngle = random(MIN_PAN_ANGLE, MAX_PAN_ANGLE);
        myBrobot.nextTiltAngle = random(MIN_TILT_ANGLE, MAX_TILT_ANGLE);
      }
      else {
        myBrobot.neckTimer--;
      }
    }
    else {
      myBrobot.neckSpeed--;
      myBrobot.neckTimer = random(60, 80);
      if(myBrobot.neckSpeed == 0){
        if(millis() % 2 == 0) {
          if(myBrobot.currentPanAngle < myBrobot.nextPanAngle){
            myBrobot.currentPanAngle++;
          }
          else if(myBrobot.currentPanAngle > myBrobot.nextPanAngle){
            myBrobot.currentPanAngle--;
          }
          myBrobot.pan.write(myBrobot.currentPanAngle);
          SoftwareServo::refresh();
          //delay(10);
        }
        else {
          if(myBrobot.currentTiltAngle < myBrobot.nextTiltAngle){
            myBrobot.currentTiltAngle++;
          }
          else if(myBrobot.currentTiltAngle > myBrobot.nextTiltAngle){
            myBrobot.currentTiltAngle--;
          }
          myBrobot.tilt.write(myBrobot.currentTiltAngle);
          SoftwareServo::refresh();
        }
        myBrobot.neckSpeed = NECK_SPEED;
      }
    }
  }
  else {
    myBrobot.neckSpeed--;
    if(myBrobot.neckSpeed == 0){
      if(myBrobot.currentTiltAngle <= MAX_TILT_ANGLE) {
        if(myBrobot.upToggle){
          myBrobot.currentTiltAngle++;
        }
      }
      if(myBrobot.currentTiltAngle >= MIN_TILT_ANGLE) {
        if(myBrobot.downToggle){
          myBrobot.currentTiltAngle--;
        }
      }
      if(myBrobot.currentPanAngle <= MAX_PAN_ANGLE) {
        if(myBrobot.rightToggle){
          myBrobot.currentPanAngle++;
        }
      }
      if(myBrobot.currentPanAngle >= MIN_PAN_ANGLE) {
        if(myBrobot.leftToggle){
          myBrobot.currentPanAngle--;
        }
      }
      myBrobot.pan.write(myBrobot.currentPanAngle);
      myBrobot.tilt.write(myBrobot.currentTiltAngle);
      SoftwareServo::refresh();
      delay(10);
      myBrobot.neckSpeed = NECK_SPEED;
    }
  }
  
  if(myBrobot.soundToggle && !myBrobot.togglePause){
    SoftwareServo::refresh();
    //delay(200);
    myBrobot.playComplete();
  }
}





