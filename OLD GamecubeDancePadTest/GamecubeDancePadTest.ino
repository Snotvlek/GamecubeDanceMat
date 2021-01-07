#include <SoftwareSerial.h>
#define DAT 3
#define LED 5

//https://os.mbed.com/users/christopherjwang/code/gamecube_controller/wiki/Homepage#:~:text=VU-,Gamecube%20Controller%20Protocol,command%20sent%20to%20the%20controller.
//Documentation on Gamecube controller scheme and polling modes

void setup() {
  pinMode(DAT, INPUT);
  Serial.begin(115200);
}

void loop() {
  pinMode(DAT,INPUT);
  int x = digitalRead(DAT);
  Serial.println(x);
  x == 1 ? digitalWrite(LED,HIGH) : digitalWrite(LED,LOW);
/*
  String str = Serial.readString();
  if(str == "poll"){
    pinMode(DAT, OUTPUT);
    
    }
    */
  
  delay(50);

}
