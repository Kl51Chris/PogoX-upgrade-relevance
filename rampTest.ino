#include <Servo.h>

Servo ESC;
const int minPulse = 1400;
const int midPulse = 1500;
const int maxPulse = 1600;
const int duration = 3000; // 3 seconds in milliseconds
const int stepDelay = 10;  // ms between steps

void setup() {
  Serial.begin(9600);
  ESC.attach(9, 1000, 2000);
  delay(2000); // give ESC time to initialize
  rampPulse(midPulse, maxPulse, duration); // 1500 to 1600
  rampPulse(maxPulse, minPulse, duration); // 1600 to 1400
  rampPulse(minPulse, midPulse, duration); // 1400 to 1500
  ESC.writeMicroseconds(1500);
  delay(2000); // wait 2 seconds before repeating
}

void loop() {
}

void rampPulse(int startVal, int endVal, int totalTime) {
  int steps = totalTime / stepDelay;
  float delta = (float)(endVal - startVal) / steps;

  for (int i = 0; i <= steps; i++) {
    int pwmValue = startVal + delta * i;
    ESC.writeMicroseconds(pwmValue);
    Serial.println(pwmValue);
    delay(stepDelay);
  }
}
