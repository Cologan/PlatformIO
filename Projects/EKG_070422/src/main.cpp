//=============================Includes and Defines==========================================================================
#include <Arduino.h>                                    // Arduino configs
#include <SSD1306.h>                                    // Display Configs
#define USING_TIM_DIV16         true                    // need to be defined before include
#include <ESP8266TimerInterrupt.h>                      // NodeMCU timer and interrupt library
#include <ESP8266WiFi.h>                                // NodeMCU Wifi Library
#include <WiFiUdp.h>                                    // Library for UDP connection

#define TIMER_INTERVAL_MS       4                       // Time to call ISR, in milliseconds
#define loPlus                  D6                      // LO+ output from ECG Sensor
#define loMinus                 D5                      // LO- output from ECG Sensor
#define ECGout                  A0                      // ECG Read Output 
#define ARRAY_SIZE              7500                    // Defining array size 7500 *data at 250 hz =30 sec data
//===========================================================================================================================

//=============================Globals=======================================================================================
long int dataarray[ARRAY_SIZE]={0};                     // Array with 0
long int Headindex=0;                                   // Write index
long int Tailindex=0;                                   // Read index
long int data=0;                                        // Fill status 
int X=0;
int X0;
int Y0;
int Y;
long int i=0;                                           // Var required by fakedata function
int data_from_buffer=0;                                 // Var to temp save data read from array
//===========================================================================================================================

//=============================Wifi- Config==================================================================================
const char *ssid =  "FRITZ!Box 7590 VL";                // Name of Wifi
const char *pass =  "56616967766283031728";             // Password
//===========================================================================================================================

//=============================UDP===========================================================================================
WiFiUDP Udp;                                            
unsigned int localUdpPort = 4210;                       // local port to listen on
char incomingPacket[255];                               // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back
//===========================================================================================================================

//=============================Global Setup==================================================================================
SSD1306Wire display(0x3c, SDA, SCL);                    // init oled 
ESP8266Timer ITimer;                                    // Init ESP8266 timer 1 , using timer 1 since timer 0 is used by wifi
//===========================================================================================================================

//=============================Function Declarations=========================================================================
void fake_data();                                       // Fake data for testing
long int ecgreader();                                   // Function to read ecg signals
void buffer_save();                                     // Ringbuffer to save data
void buffer_read();                                     // Ringbuffer to read data
void IRAM_ATTR TimerHandler();                          // Timer ISR
void draw_grid();                                       // Draw X and Y Axis
void draw_graph();                                      // Function to draw graph on oled screen
void wifi_connection();                                 // Wifi connection setup
//===========================================================================================================================

//=============================SETUP=========================================================================================
void setup() {                                          // put your setup code here, to run once:
  
  Serial.begin(115200);
  while(!Serial);

  delay(300);
  
  Serial.print(F("\nECG Lab Project "));                //Project Title
  Serial.println(ARDUINO_BOARD);                        //Board name
  Serial.print(F("CPU Frequency = "));                  //CPU stats
  Serial.print(F_CPU / 1000000); 
  Serial.println(F(" MHz"));
  //wifi_connection();
  ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler);

  pinMode(loPlus, INPUT);                               // Setup for leads off detection LO +
  pinMode(loMinus, INPUT);                              // Setup for leads off detection LO –

  display.init();
  display.clear();
  display.flipScreenVertically();
  draw_grid();

}
//===========================================================================================================================

//=============================LOOP==========================================================================================
void loop() {                                           // put your main code here, to run repeatedly:
  //draw_graph();
  buffer_read();
  delay(4);                                             //refresh at 2miliseconds
}
//===========================================================================================================================


//=============================Function Definations==========================================================================

void fake_data(){                                       // Fake data for testing

  dataarray[Headindex]=100*(abs((i++ % 12) - 6));       //This gives a triangular wave of period 12, oscillating between 6 and 0. multiplaying with 100 for bigger amplitude
  //dataarray[Headindex]=10*((i++ % 12) < 6 ? 6 : 0);     //This gives a regular square wave of period 12, oscillating between 6 and 0.
  //dataarray[Headindex]=3 * sin((float)i / 10);          //This gives a sine wave of period 20 pi, oscillating between 3 and -3

  i=i%100000;
  Headindex++;                                            //write index
  data++;                                                 //incrementing fill status
  Headindex=Headindex%ARRAY_SIZE;                         //wraparound
}

long int ecgreader(){                                   // Function to read ecg signals
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
}

void buffer_save(){                                     // Ringbuffer to save data
  for (int j = 0; j<ARRAY_SIZE; j++){
    dataarray[Headindex]=ecgreader();  
    Headindex++;                                        //write index
    data++;                                             //incrementing fill status
    Headindex=Headindex%ARRAY_SIZE;                     //wraparound
    Serial.print("Saving ");
    //delay(4);                                         //wait for 4 milli sec
  }
}

void buffer_read(){                                     // Ringbuffer to read data
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

void IRAM_ATTR TimerHandler(){                          // Timer ISR
  //buffer_save();
  fake_data();
}

void draw_grid(){                                       // Draw X and Y Axis
  display.drawHorizontalLine(0,2,120);
  display.drawString(120,4,"X");
  display.drawVerticalLine(3,15,60);
  display.drawString(0,0,"Y");
  display.display();
}

void draw_graph(){                                      // Function to draw graph on oled screen
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
}

void wifi_connection(){                                 // Wifi connection setup
                  
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

//===========================================================================================================================
