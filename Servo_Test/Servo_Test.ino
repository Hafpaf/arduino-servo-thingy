#include <Servo.h>

Servo myservo;

int pos = 0;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(D1); //attaches to specefik pin
}

void loop() {
  // put your main code here, to run repeatedly:

  for (pos = 0; pos <= 180; pos += 1){ //goes from 0 to 180 degrees
  myservo.write(pos);
  delay(15);
  }
}

