#include <Arduino.h>
#include <Wire.h>
#include <SSD1306.h>


SSD1306Wire display(0x3c, SDA, SCL); 
#define DEMO_DURATION 3000



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("start");
    // Initialising the UI will init the display too.
  display.init();

  //display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Hello world");
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("hallo");
  delay(1000);
}