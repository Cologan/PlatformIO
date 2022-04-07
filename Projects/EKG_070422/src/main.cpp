#include <Arduino.h>
#include <SSD1306.h>
#include <ESP8266TimerInterrupt.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define USING_TIM_DIV16         true        // for longest timer but least accurate. Default
#define TIMER_INTERVAL_MS       4
#define loPlus                  D6
#define loMinus                 D5
#define ECGout                  A0
#define ARRAY_SIZE              500       //Defining array size 7500 *data at 250 hz =30 sec data

long int dataarray[ARRAY_SIZE]={0};             //creating empty array
long int Headindex=0;                          //write index
long int Tailindex=0;                          //read index
long int data=0;                               //fill status 

int X=0;
int X0;
int Y0;
int Y;
long int i=0;

const char *ssid =  "FRITZ!Box 7590 VL";  
const char *pass =  "56616967766283031728";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;                       // local port to listen on
char incomingPacket[255];                               // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back


int data_from_buffer=0;

SSD1306Wire display(0x3c, SDA, SCL);      //init oled 

ESP8266Timer ITimer;                      // Init ESP8266 timer 1 , using timer 1 since timer 0 is used by wifi

void fake_data(){         //Fake data for testing
  
  dataarray[Headindex]=2*(abs((i % 12) - 6));  //This gives a triangular wave of period 12, oscillating between 6 and 0.
  /*
  Square Wave
  y = (x++ % 6) < 3 ? 3 : 0;
  This gives a regular square wave of period 6, oscillating between 3 and 0.

  Sine Wave
  y = 3 * sin((float)x / 10);
  This gives a sine wave of period 20 pi, oscillating between 3 and -3
  */
  i++;
  i=i%100000;
  Headindex++;      //write index
  data++;           //incrementing fill status
  Headindex=Headindex%ARRAY_SIZE;   //wraparound
  //Serial.print("Saving fake data ");
  //Serial.println(i%30);
  }

long int ecgreader(){                          //Function to read ecg signals
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
  for (int j = 0; j<ARRAY_SIZE; j++){
    dataarray[Headindex]=ecgreader();  
    Headindex++;                           //write index
    data++;                                //incrementing fill status
    Headindex=Headindex%ARRAY_SIZE;        //wraparound
    Serial.print("Saving ");
    delay(4);                              //wait for 4 milli sec
  }
}

void buffer_read(){                       //Ringbuffer to read data
  if(data==0){
    Serial.println("No data in buffer");
  }else{
    data_from_buffer=dataarray[Tailindex];
    Serial.println(data_from_buffer);
    Tailindex++;
    Tailindex=Tailindex%ARRAY_SIZE;
    data--;
  }
}

void IRAM_ATTR TimerHandler(){            //Timer ISR
  //buffer_save();
  fake_data();
}

void draw_grid(){         //Draw X and Y Axis
  display.drawHorizontalLine(0,2,120);
  display.drawString(120,4,"X");
  display.drawVerticalLine(3,15,60);
  display.drawString(0,0,"Y");
  display.display();
}

/*void draw_graph(){        //function to draw graph on oled screen
  Y=dataarray[Tailindex]+32;
  Tailindex++;
  Tailindex=Tailindex%ARRAY_SIZE;
  data--;
  display.drawLine(X0,Y0+32,X,Y);
  display.display();
  X=X+5;
  X0=X;
  Y0=Y;
}*/

void wifi_connection(){   //Wifi connection setup
                  
  Serial.println("Connecting to ");
  Serial.println(ssid); 
  WiFi.begin(ssid, pass); 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Udp.begin(localUdpPort);
  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  Serial.print("UDP Port:\t");
  Serial.println(localUdpPort);
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
  wifi_connection();
  ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler);

  pinMode(loPlus, INPUT); // Setup for leads off detection LO +
  pinMode(loMinus, INPUT); // Setup for leads off detection LO –

  display.init();
  display.clear();
  display.flipScreenVertically();
  draw_grid();

}

void loop() {             // put your main code here, to run repeatedly:
  
  //display.drawString(14,10,"Hello World");
  //display.display();
  //ecgreader();

  // while(data!=100){
  //   buffer_save();
  // }
   //buffer_read();
   //draw_graph();



  Y=dataarray[Tailindex]+32;
  Tailindex++;
  Tailindex=Tailindex%ARRAY_SIZE;
  data--;
  display.drawLine(X0,Y0,X+5,Y);
  display.display();
  X=X+5;
  X0=X;
  Y0=Y;

  if(X>=128){
    display.clear();
    draw_grid();
    X=0;
    X0=0;
  }

  delay(10);      //refresh at 2miliseconds

  

}