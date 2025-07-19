#include <Arduino.h>
#include<Servo.h>
Servo ESC;
int potoRead;
int pwmValue;
float filteredValue = 1500; 
float alpha = 0.1;
float fliteredValue2 = 1500;
void setup() {
  Serial.begin(9600);
  pinMode(6, INPUT_PULLUP);
  ESC.attach(9, 1000, 2000);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(6) == HIGH){//stop switch
    pwmValue=1500;
    ESC.writeMicroseconds(pwmValue);
    Serial.println(pwmValue);
    delay(20);
    return;
  }
  potoRead=analogRead(A0);
  pwmValue = map(potoRead, 0, 1023, 1100, 1900);
  filteredValue = alpha * pwmValue + (1 - alpha) * filteredValue;
  pwmValue = (int)filteredValue;
  // (pwmValue<1600 && pwmValue>1400) alpha=0.05; else alpha= 0.1;
  ESC.writeMicroseconds(pwmValue);  
  Serial.println(pwmValue);

  delay(20);
}
