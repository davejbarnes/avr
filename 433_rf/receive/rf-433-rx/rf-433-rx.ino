/* 
  RF Blink - Receiver sketch 
     Written by ScottC 17 Jun 2014
     Arduino IDE version 1.0.5
     Website: http://arduinobasics.blogspot.com
     Receiver: XY-MK-5V
     Description: A simple sketch used to test RF transmission/receiver.          
 ------------------------------------------------------------- */

 #define rfReceivePin A0  //RF Receiver pin = Analog pin 0
 #define ledPin 13        //Onboard LED = digital pin 13

 int state = 0;
 unsigned int data = 0;   // variable used to store received data
 const unsigned int upperThreshold = 80;  //upper threshold value
 const unsigned int lowerThreshold = 70;  //lower threshold value

 void setup(){
   pinMode(ledPin, OUTPUT);
   Serial.begin(9600);
 }

 void loop(){
  for (int samples=1; samples < 6; samples++){
    int average=0;
    data=analogRead(rfReceivePin);    //listen for data on Analog pin 0
    average += data;
  }

   data /= 5;
   
    if(data>upperThreshold){
     digitalWrite(ledPin, LOW);   //If a LOW signal is received, turn LED OFF
     Serial.print(data);
     Serial.print(" ");
     if (state != 0){
      state = 0;
     }
     //Serial.println(300);
   }
   
   if(data<lowerThreshold){
     digitalWrite(ledPin, HIGH);   //If a HIGH signal is received, turn LED ON
     Serial.print(data);
     Serial.print(" ");
     if (state != 1){
      state = 1;
     }
   }
   //Serial.println(600);
   delay(50);
   Serial.println();
 }
