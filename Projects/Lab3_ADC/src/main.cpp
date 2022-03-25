#include <Arduino.h>    //Library for Arduino dev Boards
#include <SSD1306.h>    //Library for Monochrome oled display

#define adc_create_pin D7
#define adc_read_pin A0


// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA, SCL); 
/*ADDRESS, SDA, SCL  -  SDA and SCL usually populate 
automatically based on your board's pins_arduino.h*/

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);           //Start Serial Monitor at 115200 Baud

  //Set-up display
  display.init();                 //initialise Display
  display.flipScreenVertically(); //flipping orientation for convinience (optional)  

  //set-up the pin for writing
  pinMode(adc_create_pin, OUTPUT);
  pinMode(adc_read_pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  display.drawString(0,0,"Hello World from Oled");    //Text to print 
  display.display();                                  //printing on display
}