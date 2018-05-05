/**
 * @example TCPClientSingle.ino
 * @brief The TCPClientSingle demo of library WeeESP8266. 
 * @author Wu Pengfei<pengfei.wu@itead.cc> 
 * @date 2015.02
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

#define SSID        "VM8353338-2G"
#define PASSWORD    "LinuxMint1"
#define HOST_NAME   "mjb.daveops.co"
#define HOST_PORT   (8080)

SoftwareSerial serialwifi(2,3);
ESP8266 wifi(serialwifi, 9600);

char CR = 13;
char LF = 10;

void setup(void)
{
    Serial.begin(57600);
    Serial.print("setup begin\r\n");
    
    Serial.print("FW Version:");
    Serial.println(wifi.getVersion().c_str());
      
    if (wifi.setOprToStationSoftAP()) {
        Serial.print("to station + softap ok\r\n");
    } else {
        Serial.print("to station + softap err\r\n");
    }
 
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
    } else {
        Serial.print("Join AP failure\r\n");
    }
    
    if (wifi.disableMUX()) {
        Serial.print("single ok\r\n");
    } else {
        Serial.print("single err\r\n");
    }
    
    Serial.print("setup end\r\n");
}
 
void loop(void)
{
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
        Serial.print("create tcp ok\r\n");
    } else {
        Serial.print("create tcp err\r\n");
    }

        
    String message = "GET / HTTP/1.1\r\n";
    message += "Host: ec2-35-177-202-134.eu-west-2.compute.amazonaws.com:8080\r\n";
    message += "User-Agent: curl/7.47.0\r\n";
    message += "Accept: */*\r\n";
    message += "\r\n";
    
    const char *hello = new char[message.length()+1];
    strcpy(hello, message.c_str());
    wifi.send((const uint8_t*)hello, strlen(hello));
    delete [] hello;
    Serial.print(message);

    message = "";

    char buffer[300];
    uint32_t len = wifi.recv(buffer, sizeof(buffer), 1000);
    Serial.print("Buffer length is : ");
    Serial.println(len);
    if (len > 0) {
        //Serial.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
        }
        //Serial.print("]\r\n");
    }

    //delete [] buffer;
    
    if (wifi.releaseTCP()) {
        Serial.print("release tcp ok\r\n");
    } else {
        Serial.print("release tcp err\r\n");
    }
    delay(5000);
}
     
