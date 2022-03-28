#include <Arduino.h>
#include <SSD1306.h>

#define lo_plus D5
#define lo_minus D6

SSD1306Wire display(0x3c, SDA, SCL);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);   //Start Serial connection
  Serial.println();       //printing free space on Serial monitor 
  Serial.println();       //printing free space on Serial monitor

  pinMode(lo_plus, INPUT); // Setup for leads off detection LO +
  pinMode(lo_minus, INPUT); // Setup for leads off detection LO -

  display.init();         //initialising display/oled

}

void loop() {
  // put your main code here, to run repeatedly:
   
  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
  Serial.println('!');
  }
  else{
  // send the value of analog input 0:
  Serial.println(analogRead(A0));
  }
  //Wait for a bit to keep serial data from saturating
  delay(1);
}
