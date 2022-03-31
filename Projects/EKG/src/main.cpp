#include <Arduino.h>
#include <SSD1306.h>
#include<ESP8266TimerInterrupt.h>
#include <ESP8266_ISR_Timer.h>
#define USING_TIM_DIV1                false           // for shortest and most accurate timer
#define USING_TIM_DIV16               true           // for medium time and medium accurate timer
#define USING_TIM_DIV256              false            // for longest timer but least accurate. Default

#include "ESP8266TimerInterrupt.h"
#include "ESP8266_ISR_Timer.h"

// Init ESP8266 timer 1
ESP8266Timer ITimer;

void IRAM_ATTR TimerHandler()
{
ISR_timer.run();
}

#define HW_TIMER_INTERVAL_MS 50L

int loPlus = D6;
int loMinus = D5;
int ECGout = A0;

void InterruptFunction()
{

}

SSD1306Wire display(0x3c, SDA, SCL);  //Setup display connection

void setup() {

Serial.begin(115200); // initialize the serial communication

pinMode(loPlus, INPUT); // Setup for leads off detection LO +
pinMode(loMinus, INPUT); // Setup for leads off detection LO –

//Interval in microsecs
ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS*1000,TimerHandler);

display.init(); //initialise display
display.clear();  //clear display
display.flipScreenVertically(); //fliiping for ease of use
timer1_attachInterrupt(onTimerISR);

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
