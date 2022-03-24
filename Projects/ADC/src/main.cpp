#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SSD1306.h>

#define adc_creator D7
#define adc_reader A0

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int adc_value =0;
// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float adc_to_voltage(int adc_read){        //function to convert adc values to voltage 
    return (0.00322*adc_read);              //NodeMCU max voltage 3.3 V
}


void setup() {
  pinMode(adc_creator, OUTPUT);
  Serial.begin(9600);

  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  

}

void loop() {
  oled.clearDisplay(); // clear display
  if((adc_value/1024)==0){
  digitalWrite(adc_creator, HIGH);        //turns digitalpin to High
  }else{
    digitalWrite(adc_creator, LOW);       //turns digitalpin to Low
  }
  adc_value=analogRead(adc_reader);       //Reads Analoge value from the Pin A0 on nodeMCU
  oled.setTextSize(1);                    // text size
  oled.setTextColor(WHITE);               // text color
  oled.setCursor(0, 10);                  // position to display
  oled.print("ADC Value: ");              // text to display
  oled.print(adc_to_voltage(adc_value));  //print outs voltage
  oled.print(" V");              // text to display
  oled.display();                         // show on OLED
  delay(500);

}

