// Ultrasonic sensor poc

// Now with added liquid crystal display

#include <LiquidCrystal.h>
LiquidCrystal myLCD(10,9,5,4,3,2);


int pinTrig = 13;
int pinEcho = 11;

int samples = 10;

float pingTime;
float pingSum;
float averagePingTime;
float targetDistance;
float speedOfSound = 340;   // speed of sounds in metres/sec



void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  myLCD.begin(16,2);
  myLCD.setCursor(0,0);
  myLCD.print("Dave's distance");
  myLCD.setCursor(0,1);
  myLCD.print("metre !!");

  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  delay(3000);
}

void printSerial(){
  
  Serial.print("Average round trip time is ");
  Serial.print(averagePingTime);
  Serial.println(" microseconds");
  Serial.print("Target distance is ");
  Serial.print(targetDistance);
  Serial.println(" cms away");
  Serial.println(" ");

}

void printLCD(){
  
  myLCD.setCursor(0,0);
  myLCD.print("Target distance:");
  myLCD.setCursor(0,1);
  myLCD.print("                ");
  myLCD.setCursor(0,1);
  myLCD.print(targetDistance);
  myLCD.print(" cm");
}

void loop() {
  // put your main code here, to run repeatedly:

  pingSum=0;
  
  for (int count=1; count < samples+1; count++){
    digitalWrite(pinTrig, LOW);
    delayMicroseconds(2000);
    digitalWrite(pinTrig, HIGH);
    delayMicroseconds(15);
    digitalWrite(pinTrig, LOW);

    pingTime = pulseIn(pinEcho, HIGH);
    pingSum = pingSum + pingTime;  

    delayMicroseconds(2000);

  }

  averagePingTime = pingSum / samples;

  targetDistance = ((speedOfSound * (averagePingTime / 1000000) /2 ) * 100);

  //printSerial();
  printLCD();  

  delay(3000);

}
