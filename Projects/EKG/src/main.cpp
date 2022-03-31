#include <Arduino.h>
#include <SSD1306.h>
//#include <ESP8266TimerInterrupt.h>
//#include <ESP8266_ISR_Timer.h>
#include <Ticker.h>


int loPlus = D6;
int loMinus = D5;
int ECGout = A0;

SSD1306Wire display(0x3c, SDA, SCL);  //Setup display connection

void ICACHE_RAM_ATTR onTimerISR()
{
buffer.write();
}

void setup() {
/*
Funktion display: soll checken ob relevante menge an daten geschrieben wurde (250 Hz -> 60Hz*2 ~ Alle 4 Werte als tick resultiert grob in 2 Sekunden auf Display)
Soll mit jedem Poll die letzten 128 Werte auf display ausgeschrieben werden
/Aufgabe Daniel: Timer und Buffer
/-> Anschauen Wifi & Server
*/

Serial.begin(115200); // initialize the serial communication

//GPIO Init
pinMode(loPlus, INPUT); // Setup for leads off detection LO +
pinMode(loMinus, INPUT); // Setup for leads off detection LO –

//Timer Init
timer1_attachInterrupt (onTimerISR);
timer1_enable(TIM_DIV16,TIM_EDGE,TIM_LOOP);
timer1_write(20000);

//Display Init
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
