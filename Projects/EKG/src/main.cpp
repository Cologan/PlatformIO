#include <Arduino.h>
#include <SSD1306.h>
//#include <ESP8266WiFi.h>
#include <Ticker.h>


#define ARRAY_SIZE 7500

int loPlus = D6;
int loMinus = D5;
int ECGout = A0;
int dataarray[ARRAY_SIZE]={0};      //werten für 30 secs abtasten 250 hz
int Headindex=0;                //start index at 0 
int Tailindex=0;
int data=0;

SSD1306Wire display(0x3c, SDA, SCL);  //Setup display connection


void buffer_write(){  //Funktion deklarieren und definieren 
  dataarray[Headindex]=analogRead(ECGout);  //starts writing data from 0
  Headindex++;      //read index
  Headindex=Headindex%ARRAY_SIZE;
  //delay(4);     //wait 4 milisecs 
};    
   
void IRAM_ATTR onTimerISR(){
    buffer_write();
    Serial.println("inside isr");
    //timer1_write(400000);//4ms
}

int data_to_display(){
  data=dataarray[Tailindex];
  Tailindex++;
  Tailindex=Tailindex%ARRAY_SIZE;
  return data;
}

void setup() {

  Serial.begin(115200); // initialize the serial communication

  pinMode(loPlus, INPUT); // Setup for leads off detection LO +
  pinMode(loMinus, INPUT); // Setup for leads off detection LO –

  display.init(); //initialise display
  display.clear();  //clear display
  display.flipScreenVertically(); //fliiping for ease of use

  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  timer1_write(20000); 
}


void loop()
{
display.clear();
//Timer attach every 4ms


  display.drawString(0,0,String(data_to_display()));
  display.display();
  Serial.println("inside display");
  

  delay(10);

}
