/**
 * @example HTTPGET.ino
 * @brief The HTTPGET demo of library WeeESP8266. 
 * @author Wu Pengfei<pengfei.wu@itead.cc> 
 * @date 2015.03
 * 
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "ESP8266.h"
#include <SoftwareSerial.h>
#include <SimpleDHT.h>

#define SSID        "VM8353338-2G"
#define PASSWORD    "LinuxMint1"
#define HOST_NAME   "mjb.daveops.co"
#define HOST_PORT   (3100)

#define URL_GET     "GET /latest HTTP/1.1\r\nHost: mjb.daveops.co:3100\r\nConnection: close\r\n\r\n"
#define URL_POST    "POST / HTTP/1.1\r\nHost: mjb.daveops.co:3100\r\nContent-Type: application/json\r\nContent-Length: "

SoftwareSerial serialwifi(2,3);
ESP8266 wifi(serialwifi, 9600);
SimpleDHT11 dht11;

int pinPir = 7;
int pinLed = 4;
int pinDHT11 = 6;

void setup(void)
{
    Serial.begin(57600);
    Serial.print("...");

    //Serial.print("FW Version:");
    //Serial.println(wifi.getVersion().c_str());

    if (wifi.setOprToStationSoftAP()) {
        //Serial.print("to station + softap ok\r\n");
    } else {
        //Serial.print("to station + softap err\r\n");
    }

    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("WiFi OK...");
    } else {
        //Serial.print("Join AP failure\r\n");
    }
    
    if (wifi.disableMUX()) {
        //Serial.print("single ok\r\n");
    } else {
        //Serial.print("single err\r\n");
    }
    pinMode(pinPir, INPUT);
    pinMode(pinLed, OUTPUT);
        
    Serial.println("done");
}

void postUpdate(String postString){ 
  
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
      Serial.println("TCP+\r\n");
  }
  
  const char *hello = new char[postString.length()+1];
  strcpy(hello, postString.c_str());
  wifi.send((const uint8_t*)hello, strlen(hello));
  //return;
  uint8_t buffer[256] = {0};
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
  if (len > 0) {
      //Serial.print("Received:[");
      for(uint32_t i = 0; i < len; i++) {
          Serial.print((char)buffer[i]);
      }
  }

  wifi.releaseTCP();
}


bool detect(){
  int PIR = digitalRead(pinPir);
  bool motion = false;
  if (PIR == HIGH){
    digitalWrite(pinLed, HIGH);
    motion = true;
  }else{
    digitalWrite(pinLed, LOW);
    motion = false;
  }
  return motion;
}

int readTemp(){

  int celcius = 0;
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    //message = "Read DHT11 failed";
    celcius = 255;
  }else{
    celcius = int(temperature);
  }
  return celcius;
}

String constructPost(String room, int celcius){
  String POST = URL_POST;
  String json = "{\r\n\"Room\":\"" + room + "\",\r\n\"Temperature\":\"" + String(celcius) + "\"\r\n}\r\n\r\n";
  int jsonLen = json.length();
  POST += String(jsonLen) + "\r\n\r\n";
  POST += json + "\r\n";
  return POST;
}


void loop(void)
{
    if (detect()){
      Serial.print("Motion detected : ");
      int celcius = readTemp();
      Serial.println(celcius);
      String post = constructPost("Living Room", celcius);
      Serial.println("POST is:");
      Serial.print(post);
      postUpdate(post);
      while (detect());
      delay(2000);
    }
    //getLatest();
    delay(1000);
}
     
