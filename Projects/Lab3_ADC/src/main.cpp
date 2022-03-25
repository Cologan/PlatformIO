#include <Arduino.h>    //Library for Arduino dev Boards
#include <SSD1306.h>    //Library for Monochrome oled display

#define adc_create_pin D7
#define adc_read_pin A0

float adc_read_value = 0;

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
 
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Inside loop main");

  for(int Duty_cycle = 0; Duty_cycle < 10; Duty_cycle++){
    Serial.println("Inside for loop 1");   
    analogWrite(adc_create_pin, Duty_cycle);
    adc_read_value=analogRead(adc_read_pin)*0.0032;
    display.clear();
    display.drawString(0,0,"ADC Value is: ");           //Text to print 
    display.drawString(0,10,String(adc_read_value));           //Text to print 
    display.drawString(0,20,"V");           //Text to print 
    display.display();
    Serial.println(adc_read_value);
    delay(1000);
  }
  Serial.println("for loop 1 end");   


  for(int Duty_cycle = 10; Duty_cycle > 1; Duty_cycle--){
    Serial.println("Inside for loop 2"); 
    analogWrite(adc_create_pin, Duty_cycle);
    adc_read_value=(adc_read_pin)* 0.0032;
    display.clear();
    display.drawString(0,0,"ADC Value is: ");           //Text to print 
    display.drawString(0,10,String(adc_read_value));           //Text to print 
    display.drawString(0,20,"V");           //Text to print 
    display.display();
    Serial.println(adc_read_value);
    delay(1000);
  }
  Serial.println("for loop 2 end");   
}