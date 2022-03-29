#include <Arduino.h>
#include <SSD1306.h>

int loPlus = D6;
int loMinus = D5;
int ECGout = A0;

SSD1306Wire display(0x3c, SDA, SCL);  //Setup display connection

void setup() {

Serial.begin(115200); // initialize the serial communication

pinMode(loPlus, INPUT); // Setup for leads off detection LO +
pinMode(loMinus, INPUT); // Setup for leads off detection LO –

display.init(); //initialise display
display.clear();  //clear display
display.flipScreenVertically(); //fliiping for ease of use

}


void loop()
{
  display.clear();
if((digitalRead(loPlus) == 1)||(digitalRead(loMinus) == 1))
{
Serial.println("!");
display.drawString(0,0,"!");
display.display();
}
else
{
// send the value of analog input 0:
Serial.println(analogRead(ECGout));
display.drawString(0,0,String(analogRead(ECGout)));
display.display();
}

//Waiting to keep serial data away from saturating
delay(5);
}
