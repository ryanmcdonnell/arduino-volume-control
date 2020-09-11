#include <Arduino.h>
#include <SPI.h>

const int encoderA = PIN2;
const int encoderB = PIN4;
const int encoderSwitch = PIN3;
const int CS = 10;

byte leftPadAddress = B00000000;
byte rightPadAddress = B00010000;

const int volumeUpOhms = 1000; // 1k
const int volumeDownOhms = 3200; // 3.2k
const int potMaxSteps = 128;
const long potMaxOhms = 100000L; // 100k

enum SteeringWheelButton {
  None,
  VolumeUp,
  VolumeDown,
  Mute
};

SteeringWheelButton buttonToBePressed;

void setPot(byte address, int step)
{
  digitalWrite(CS, LOW);

  SPI.transfer(address);
  SPI.transfer(step);

  digitalWrite(CS, HIGH);
}

void pressButton(SteeringWheelButton button)
{
  byte address;
  int r;

  switch(button) {
    case VolumeUp:
      address = leftPadAddress;
      r = 1000; // 1k ohms
      break;
    case VolumeDown:
      address = leftPadAddress;
      r = 3200; // 3.2k ohms
      break;
    case Mute:
      address = rightPadAddress;
      r = 0; // 0 ohm
      break;
    case None:
      return;
  }

  float value = (((float)r / potMaxOhms) * potMaxSteps);
  int step = floor(value);

  // Button pressed
  setPot(address, step);
  
  delay(70);
  
  // Button released
  setPot(address, potMaxSteps);
}

void encoderTurned() {
  if(buttonToBePressed != None) return;

  int valA = digitalRead(encoderA);
  int valB = digitalRead(encoderB);
  
  // Compare pins to determine in which direction encoder was turned
  if (valA != valB) {
      // Clockwise
      buttonToBePressed = VolumeUp;
  }
  else {
      // Counter-clockwise
      buttonToBePressed = VolumeDown;
  }
}

void encoderSwitchPushed() {
  if(buttonToBePressed != None) return;

  buttonToBePressed = Mute;
}

void setup() {
  pinMode(encoderA, INPUT);  
  pinMode(encoderB, INPUT);
  pinMode(encoderSwitch, INPUT);

  pinMode(CS, OUTPUT);
  SPI.begin();

  // Set steady state
  setPot(leftPadAddress, potMaxSteps);
  setPot(rightPadAddress, potMaxSteps);

  attachInterrupt(digitalPinToInterrupt(encoderA), encoderTurned, RISING); // Only trigger in high position
  attachInterrupt(digitalPinToInterrupt(encoderSwitch), encoderSwitchPushed, RISING);
}

void loop() {
  if(buttonToBePressed != None) {
    
    pressButton(buttonToBePressed);

    buttonToBePressed = None;
  }

  delay(10);
}