#include <SoftwareSerial.h>

SoftwareSerial serialwifi(2,3);
//SoftwareSerial Serial(10,11);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(57600);
  serialwifi.begin(9600);


}

void loop() {
  // put your main code here, to run repeatedly:

  if (serialwifi.available()){
    Serial.write(serialwifi.read());
    //Serial.println("Got something from ESP!");
  }

  if (Serial.available()){
    serialwifi.write(Serial.read());
    //Serial.println("Sending somthing to ESP");
  }

}
