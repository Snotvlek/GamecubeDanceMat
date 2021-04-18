#include <SoftwareSerial.h>
#define btn_plus  1   //1
#define btn_A     3   //2
#define btn_right A3  //3
#define btn_X     5   //4
#define btn_down    A0  //5
#define btn_up  A1  //6
#define btn_Y     6   //7
#define btn_left  A2  //8
#define btn_B     4   //9
#define btn_min   2  //10

byte buttons[] = {btn_plus , btn_A , btn_right , btn_X , btn_up , btn_down , btn_Y , btn_left , btn_B , btn_min};
String names[] = {"plus",    "A",    "right",       "X",     "up",    "down",   "Y",      "left", "B",  "minus"};

//Ezelsbruggetje voor wiring: Plus Min A B X Y voor de digital pins, Up Down Left Right (volgorde Konami code) voor analog
bool getStatusOfButton(byte btn){
  return analogRead(btn) > 150; //return true if the value of btn is above threshold (in this case 100 of 1024)
}

void setup() {
  pinMode(btn_plus,INPUT); pinMode(btn_min,INPUT); pinMode(btn_A,INPUT); pinMode(btn_B,INPUT); pinMode(btn_X,INPUT); pinMode(btn_Y,INPUT); 
  pinMode(btn_up,INPUT); pinMode(btn_down,INPUT); pinMode(btn_left,INPUT); pinMode(btn_right,INPUT); 

  Serial.begin(9600);
}

void loop() {
  /*for(int i = 0; i < sizeof(buttons) ; i++)
  {
    if(buttons[i] == btn_up || buttons[i] == btn_down || buttons[i] == btn_left || buttons[i] == btn_right)
    {
      Serial.println("Value of " + names[i] + "\t is: " + (String)analogRead(buttons[i]));
    }
    else
    {
      Serial.println("Value of " + names[i] + "\t is: " + getStatusOfButton(buttons[i]));
    }
  }*/
  Serial.println("BtnA: "+ (String)digitalRead(btn_A));
    delay(150);
  }
