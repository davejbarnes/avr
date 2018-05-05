#include <SimpleDHT.h>
#include "ESP8266.h"
#include <SoftwareSerial.h>

#define SSID        "VM8353338-2G"
#define PASSWORD    "LinuxMint1"

SimpleDHT11 dht11;
SoftwareSerial serialwifi(10,11);

ESP8266 wifi(serialwifi, 57600);

int pinPir = 2;
int pinLed = 5;
int pinDHT11 = 7;

int loopCount = 1;
//int temperature = 20;

int motion = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(pinPir, INPUT);
  pinMode(pinLed, OUTPUT);
  Serial.begin(115200);

  wifiConnect();
  wifi.enableMUX();  
}

void wifiConnect(){
  if (wifi.setOprToStationSoftAP()) Serial.println("Set to station mode");
  if (wifi.joinAP(SSID, PASSWORD)){
    Serial.println("Connected to wifi");
  }else{
    //Serial.println("Failed connection");
  }
}


void mitchux_connect(){
  wifi.createTCP(loopCount,"192.168.0.32", 3000);
  delay(50);
  //Serial.println(wifi.getIPStatus());
}

void mitchux_release(){
  wifi.releaseTCP(loopCount);
  delay(50);
  //Serial.println(wifi.getIPStatus());
}

void tcpSend(String message){

  const char *hello = new char[message.length()+1];
  strcpy(hello, message.c_str());
  mitchux_connect();
  delay(50);
  wifi.send(loopCount,(const uint8_t*)hello, strlen(hello));
  delay(50);
  Serial.print(message);
  delete [] hello;
  mitchux_release();
}

void loop() {

  int PIR = digitalRead(pinPir);
  if (PIR == HIGH){
    analogWrite(pinLed,64);
    motion = 1;
  }else{
    analogWrite(pinLed,0);
    motion = 0;
  }
  
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    Serial.print("Read DHT11 failed");
    return;
  }

  String message = String(loopCount) + " : Temperature is " + String(int(temperature)) + "\r\n";
  tcpSend(message);
  if ( motion == 1){
    String message = String("Motion detected") + "\r\n";
    tcpSend(message);
  }
  
  //Serial.println();
  loopCount++;
  if (loopCount > 4){
    loopCount = 1;
  }

  delay(1500);

}
