#include <SoftwareSerial.h>

#include <util/delay_basic.h>
//void_delay_loop_1 (uint8_t __count)
//void_delay_loop_2 (uint16_t __count)

SoftwareSerial serial(0,1); // tx, rx

#define DAT 3
#define LED 5
//https://os.mbed.com/users/christopherjwang/code/gamecube_controller/wiki/Homepage#:~:text=VU-,Gamecube%20Controller%20Protocol,command%20sent%20to%20the%20controller.
//http://www.int03.co.uk/crema/hardware/gamecube/gc-control.htm
//Documentation on Gamecube controller scheme and polling modes

//https://www.reddit.com/r/AskElectronics/comments/3gl3b3/gamecube_controller_protocol/
//discussion on arduino protocol for gamecube controllers

//DE ARDUINO LEONARDO HEEFT CPU CYCLE SPEED VAN 16MHZ, dus hij neemt 0.06 microseconden tijd per cycle.
//voor één microseconde delay heb je dus 1/0.06 = 16+(2/3) cycle nodig. Sinds de library per 3 cycles één erbij optelt, kan je 5,5 invoeren in de counter.
//5,5 is niet handig voor een timer met int, maar de gemeten snelheid was 3.97ms per voor een high of low bit. 
//Dus ongeveer 1ms precies is 5 beter, sinds het iets sneller is dan 4ms per bit

//https://roboticsbackend.com/arduino-fast-digitalwrite/
//omdat digitalWrite zo tering lang duurt (48cycles, 3microseconden) gebruik ik hier direct port manipulation

//drie soorten poorten: B, C en D. Staan op de ondergenoemde data sheet (plus arduino comment section ook nog)
//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf
//https://forum.arduino.cc/index.php?topic=241369.0
// de Arduino leonardo is gebasseerd op de ATMEGA32U4, de PWM pin 5 correspondeert met PC6, pin 31 dus C6. 
//Dus PWM Pin 5 is C6

void set_pin_state(int pin, bool lowOrHigh){ //Set pin low for 3us, then high for 1us
//lowOrHigh = false will send low signal. High will send high signal
  
  //An average of 10 successive measurements gave around 348us total time. Assuming a total of 24+64 = 88 bits, that equates to 3.95us per bit (wat de controller verstuurt)
  //van de documentatie in de gc-control.htm
  pinMode(pin, OUTPUT);
  //pinmode heeft een overheid van 125ns dus 0,125 microseconden, wat wel kan in de 4us tijdspan van een bit.
  
  //digitalWrite(pin, LOW);
  lowOrHigh ? PORTC = C00000000 : PORTC = C00000001;  
  void_delay_loop_1(17); //iterate 17 keer, dus 51 cycles, dus 3.06 microseconden (iets meer dan 3 zodat de totale aantal ms dichterbij de 4 is.)
  
  //digitalWrite(pin, HIGH);
  lowOrHigh ? PORTC = C00000000 : PORTC = C00000001;  
  void_delay_loop_1(5);//wacht 5 iterations, dus 15 cycles, dus 0.9microseconden (dichterbij dan 1us kunnen we helaas niet komen)
}

void setup() {
  pinMode(DAT, INPUT);
  serial.begin(115200);

}

void loop() {
  
        
}
