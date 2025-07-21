#include <Arduino.h>
#include<Servo.h>
Servo ESC;
int potoRead;
int pwmValue;
float filteredValue = 1000; 
float alpha = 0.1;
void setup() {
  Serial.begin(9600);
  pinMode(6, INPUT_PULLUP);
  ESC.attach(9, 1000, 2000);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(6) == HIGH){//stop switch
    pwmValue=1000;
    ESC.writeMicroseconds(pwmValue);
    Serial.println("Duty: 0%");
    delay(20);
    return;
  }
  potoRead=analogRead(A0);
  pwmValue = map(potoRead, 0, 1023, 1000, 1900);
  filteredValue = alpha * pwmValue + (1 - alpha) * filteredValue;
  pwmValue = (int)filteredValue;
  ESC.writeMicroseconds(pwmValue);  
  // Calculate and print duty cycle percentage
  float duty = (pwmValue - 1000) / 1000.0 * 100.0;
  Serial.print("Duty: ");
  Serial.print(duty, 1);  // 1 decimal place
  Serial.println("%");
  delay(20);
}
