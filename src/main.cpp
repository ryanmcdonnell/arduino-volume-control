#include <Arduino.h>
#include <HID-Project.h>

int pinA = 2;
int pinB = 4;
int volume = 0;

void encoderTurned() {
  int valA = digitalRead(pinA);
  int valB = digitalRead(pinB);
  
  // Compare pins to determine in which direction encoder was turned
  if (valA != valB) {
      // Clockwise
      volume++;
      Consumer.write(MEDIA_VOL_UP);
  }
  else {
      // Counter-clockwise
      volume--;
      Consumer.write(MEDIA_VOL_DOWN);
  }

  Serial.print("Volume: ");
  Serial.println(volume);
}

void setup() {
  Serial.begin(115200);

  pinMode(pinA, INPUT);  
  pinMode(pinB, INPUT);

  attachInterrupt(0, encoderTurned, RISING); // Only trigger in high position

  Consumer.begin();
}

void loop() {}