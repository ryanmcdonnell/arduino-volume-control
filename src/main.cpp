#include <Arduino.h>
#include <SPI.h>

const byte address = 0x00;
const int encoderA = PIN2;
const int encoderB = PIN4;
const int encoderSwitch = PIN3;
const int leftPad = PIN5;
const int rightPad = PIN6;

const int volumeUpOhms = 1000; // 1k
const int volumeDownOhms = 3200; // 3.2k
const int potMaxSteps = 128;
const int potMaxOhms = 10000; // 10k

const int volUpStepValue = (volumeUpOhms / potMaxOhms) * potMaxSteps;
const int volDownStepValue = (volumeDownOhms / potMaxOhms) * potMaxSteps;
const int muteStepValue = 0;

void digitalPotWrite(int padCS, int value)
{
  digitalWrite(padCS, LOW);
  
  // Button pressed
  SPI.transfer(address);
  SPI.transfer(value);
  
  delay(100);
  
  // Button released
  //SPI.transfer(address);
  //SPI.transfer(potMaxSteps);

  digitalWrite(padCS, HIGH);
}

void encoderTurned() {
  int valA = digitalRead(encoderA);
  int valB = digitalRead(encoderB);
  int r = 0;
  
  // Compare pins to determine in which direction encoder was turned
  if (valA != valB) {
      // Clockwise
      digitalPotWrite(leftPad, volUpStepValue);
  }
  else {
      // Counter-clockwise
      digitalPotWrite(leftPad, volDownStepValue);
  }
}

void encoderSwitchPushed() {
  digitalPotWrite(rightPad, muteStepValue);
}

void setup() {
  pinMode(encoderA, INPUT);  
  pinMode(encoderB, INPUT);
  pinMode(encoderSwitch, INPUT);

  pinMode(leftPad, OUTPUT);
  pinMode(rightPad, OUTPUT);

  SPI.begin();

  attachInterrupt(digitalPinToInterrupt(encoderA), encoderTurned, RISING); // Only trigger in high position
  attachInterrupt(digitalPinToInterrupt(encoderSwitch), encoderSwitchPushed, RISING);
}

void loop() {
  for (int i = 50; i <= 128; i++)
  {
    digitalPotWrite(leftPad, i);
    digitalPotWrite(rightPad, i);
    delay(10);
  }

  for (int i = 128; i >= 50; i--)
  {
    digitalPotWrite(leftPad, i);
    digitalPotWrite(rightPad, i);
    delay(10);
  } 
}