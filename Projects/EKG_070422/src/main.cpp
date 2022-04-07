#include <Arduino.h>
#include <SSD1306.h>
#include <ESP8266TimerInterrupt.h>
#include <ESP8266WiFi.h>

#define USING_TIM_DIV256        true        // for longest timer but least accurate. Default
#define TIMER_INTERVAL_MS       15000
#define loPlus                  D6
#define loMinus                 D5
#define ECGout                  A0
#define ARRAY_SIZE              7500       //Defining array size 7500 data at 250 hz =30 sec data

int dataarray[ARRAY_SIZE]={};             //empty array
int Headindex=0;                          //write index
int Tailindex=0;                          //read index
int data=0;                               //fill status 

const char *ssid =  "FRITZ!Box 7590 VL";  
const char *pass =  "56616967766283031728";

int test=0;

SSD1306Wire display(0x3c, SDA, SCL);      //init oled 

ESP8266Timer ITimer;                      // Init ESP8266 timer 1 , using timer 1 since timer 0 is used by wifi

int ecgreader(){                          //Function to read ecg signals
  //Serial.println("ECGreader");
  if((digitalRead(loPlus) == 1)||(digitalRead(loMinus) == 1)){
    Serial.println('!');
    return 0;
  }
  else{
    // send the value of analog input 0:
      Serial.println(analogRead(ECGout));
      return analogRead(ECGout);
  }
  //Wait for a bit to keep serial data from saturating
  
}

void buffer_save(){                       //Ringbuffer to save data
  dataarray[Headindex]=ecgreader();  
  Headindex++;                            //write index
  data++;                                 //incrementing fill status
  Headindex=Headindex%ARRAY_SIZE;         //wraparound
  Serial.print("Saving ");
  delay(4);                               //wait for 4 milli sec
}

void buffer_read(){                       //Ringbuffer to read data
while(1){
  if(data==0){
    Serial.println("No data in buffer");
    break;
  }else{
    test=dataarray[Tailindex];
    Serial.println(test);
    Tailindex++;
    Tailindex=Tailindex%ARRAY_SIZE;
    data--;
  }
  delay(100);
}

}

void IRAM_ATTR TimerHandler(){            //Timer ISR
  while(1){
    buffer_save();
  }
}

void draw_grid(){         //Draw X and Y Axis
  display.drawHorizontalLine(0,60,120);
  display.drawString(120,54,"X");
  display.drawVerticalLine(3,15,60);
  display.drawString(0,0,"Y");
}

void draw_graph(){        //function to draw graph on oled screen

}

void fake_data(){         //Fake data for testing
  for (int i=0; i<ARRAY_SIZE; i++){
  dataarray[Headindex]=abs((i % 12) - 6);  //This gives a triangular wave of period 12, oscillating between 6 and 0.
  /*
  Square Wave
  y = (x++ % 6) < 3 ? 3 : 0;
  This gives a regular square wave of period 6, oscillating between 3 and 0.

  Sine Wave
  y = 3 * sin((float)x / 10);
  This gives a sine wave of period 20 pi, oscillating between 3 and -3

  */
  Headindex++;      //write index
  data++;           //incrementing fill status
  Headindex=Headindex%ARRAY_SIZE;   //wraparound
  Serial.print("Saving fake data ");
  Serial.println(i%30);
  delay(1);
  }

}

void wifi_connection(){   //Wifi connection setup
                  
  Serial.println("Connecting to ");
  Serial.println(ssid); 
  WiFi.begin(ssid, pass); 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}




void setup() {            // put your setup code here, to run once:
  
  Serial.begin(115200);
  while(!Serial);

  delay(300);
  
  Serial.print(F("\nECG Lab Project ")); //Project Title
  Serial.println(ARDUINO_BOARD);         //Board name
  Serial.print(F("CPU Frequency = "));    //CPU stats
  Serial.print(F_CPU / 1000000); 
  Serial.println(F(" MHz"));
  //ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler);

  pinMode(loPlus, INPUT); // Setup for leads off detection LO +
  pinMode(loMinus, INPUT); // Setup for leads off detection LO –

  display.init();
  display.clear();
  display.flipScreenVertically();
  
}

void loop() {             // put your main code here, to run repeatedly:
  
  display.clear();
  draw_grid();
  display.drawString(14,10,"Hello World");
  display.display();
  //ecgreader();

  // while(data!=100){
  //   buffer_save();
  // }
  // buffer_read();
  // delay(1000);

  fake_data();
  buffer_read();

}