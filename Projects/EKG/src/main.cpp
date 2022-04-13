#include <Arduino.h>
#include <SSD1306.h>
#include <ESP8266WiFi.h>							// Instructions from https://tttapa.github.io/ESP8266/Chap07%20-%20Wi-Fi%20Connections.html
#include <WiFiUdp.h>								//For functions relating to UDP ports, Instructions from https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/udp-examples.html
#include <ESP8266TimerInterrupt.h>

#define USING_TIM_DIV16        true        // for longest timer but least accurate. Default
#define TIMER_INTERVAL_MS       4
#define IoPlus D6
#define IoMinus D5
#define ECGout A0
#define ARRAY_SIZE 7500

//Arrayconfig
int dataarray[ARRAY_SIZE]={0};                  //empty array
int Headindex=0;                               //start index at 0 
int Tailindex=0;                                
int data=0;

long int i=0;

const char* ssid     = "FRITZ!Box 7590 VL";         // The SSID (name) of the Wi-Fi network you want to connect to !propably needs changing, taken from pdf
const char* password = "56616967766283031728";     // The password of the Wi-Fi network

int data_from_buffer=0;

SSD1306Wire display(0x3c, SDA, SCL);  //Setup display connection

ESP8266Timer ITimer;                  //Init Timer 1

//UDP_Config
/*WifiConfig & UDP
WiFiUDP Udp;
char incomingPacket[256];							//what size
char replyPacket[] = "Hi there! Got the message :-)"; //debug
unsigned int Serverport = 80;					//default
unsigned int UDP_PORT = 4210;         //default
*/

//ServerConfig
//WiFiServer server(Serverport);

//int16_t displayArray[128];
/*
void buffer_save(){                       //Ringbuffer to save data
  dataarray[Headindex]=ecgreader();  
  Headindex++;                            //write index
  data++;                                 //incrementing fill status
  Headindex=Headindex%ARRAY_SIZE;         //wraparound
  Serial.print("Saving ");
  //delay(4);                               //wait for 4 milli sec
} 
   
int16_t buffer_read(){                       //Ringbuffer to read data
  if(data==0){
    Serial.println("No data in buffer");
    break;
  }else{
    test=dataarray[Tailindex];
    Serial.println(test);
    Tailindex++;
    Tailindex=Tailindex%ARRAY_SIZE;
    data--;
    return test;
  }
  //delay(100);
}

void populate_displayarray()
{
for ((int j=0;j<=128;j++)||data==0) //or jumpflag ||() but why
{
int16_t m1=buffer_read();
int16_t m2=buffer_read();
int16_t m3=buffer_read();
int16_t m4=buffer_read();
displayArray[j]=(m1+m2+m3+m4)/4;
};
};

void display_drawgraph()
{
for (int i=0;i<128;i++)
{
  display.drawline(displayArray[i],i,displayArray[i+1],i+1);
}
*/

void fake_data(){         //Fake data for testing
  
  dataarray[Headindex]=2*(abs((i % 12) - 6));  //This gives a triangular wave of period 12, oscillating between 6 and 0.
  i++;
  i=i%100000;
  Headindex++;      //write index
  data++;           //incrementing fill status
  Headindex=Headindex%ARRAY_SIZE;   //wraparound

  }

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
  delay(10)				// recommended delay, establish Serial first ? -> necessary ?
  
  //Wifi Init
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  WiFi.begin(ssid, password);             // Connect to the network
    int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');	//Gibt Verbindungsbestätigung aus
	if (i==30)
	{
		serial.print('program terminated');
		while(true){};
	};
  }
  serial.println("Connection established!");

  //GPIO Init
  pinMode(loPlus, INPUT); // Setup for leads off detection LO +
  pinMode(loMinus, INPUT); // Setup for leads off detection LO –

  //Display Init
  display.init(); //initialise display
  display.clear();  //clear display
  display.flipScreenVertically(); //flipping for ease of use

  //Wifi IP Adress readout
  display.println('\n');
  display.println("Connection established!");  
  display.print("IP address:\t");
  display.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  display.print("UDP Port is: "); display.println(UDP_PORT);
  serial.print("IP address:\t");
  serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  serial.print("UDP Port is: "); display.println(UDP_PORT);
  
  delay (3000);							  //Connection established soll 3 Sekunden lang 
  display.clear();
  
  //Timer Init
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