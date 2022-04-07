#include <Arduino.h>
#include <SSD1306.h>
#include <ESP8266WiFi.h>							// Instructions from https://tttapa.github.io/ESP8266/Chap07%20-%20Wi-Fi%20Connections.html
#include <WiFiUdp.h>								//For functions relating to UDP ports, Instructions from https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/udp-examples.html
#include <Ticker.h>
#define ARRAY_SIZE 7500

WiFiUDP Udp;
char incomingPacket[256];							//what size

char replyPacket[] = "Hi there! Got the message :-)"; //debug
unsigned int Serverport = 80;					//default
unsigned int UDP_PORT = 4210;         //default

const char* ssid     = "FRITZ!Box 7590 VL";         // The SSID (name) of the Wi-Fi network you want to connect to !propably needs changing, taken from pdf
const char* password = "56616967766283031728";     // The password of the Wi-Fi network

//ServerConfig
WiFiServer server(Serverport);

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