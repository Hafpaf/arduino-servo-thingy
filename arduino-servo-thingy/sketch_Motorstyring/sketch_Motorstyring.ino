#include <Keyboard.h>
#include <HID.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  
  Keyboard.begin();

}

void loop() {
  // put your main code here, to run repeatedly:


  //LeftArrow
  if Keyboard.press(0xD8){
    digitalWrite(D1, HIGH);
    else if Keyboard.release(0xD8){
      digitalWrite(D1, LOW);
    }
  } 
    
  

  //RightArrow
  if Keyboard.press(0xD7){
    digitalWrite(D2, HIGH);
    else if digitalWrite(D2, LOW);
  }


  // UpArrow
  if Keyboard.press(0xDA){
    digitalWrite(D1 , HIGH) and DigitalWrite(D2,HIGH);
    else digitalWrite(D1 , LOW) and DigitalWrite(D2,LOW);
  }
  

  
  
}
