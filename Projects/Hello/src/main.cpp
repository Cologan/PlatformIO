#include <Arduino.h>

//#include <iostream>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   //begins seriall at 9600 
  Serial.println("Hello World from setup");

}

void loop() {
  // put your main code here, to run repeatedly:
  //std::cout<<"Hello World"<<std::endl;      //dose not work here because we are using serial monitor on arduino 
  Serial.println("Hello World");
  delay(5000);    //1000 = 1 second

}