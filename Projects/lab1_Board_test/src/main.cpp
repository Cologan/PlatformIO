#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:

  //Start Serial connection with Baudrate 115200
  Serial.begin(115200); 
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  //Print Text on Serial Monitor
  Serial.println("Hello from NodeMCU");  

  //Wait before going in the loop again
  delay(1000);

}