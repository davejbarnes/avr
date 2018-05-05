//**************************************************************//
//  Name    : shiftOutCode, Hello World                                
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis 
//  Date    : 25 Oct, 2006    
//  Modified: 23 Mar 2010                                 
//  Version : 2.0                                             
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                           
//****************************************************************

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

int duration = 50;
int endTime;
int strobe = 100;

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  Serial.begin(9600);
  digitalWrite(dataPin, 0);
  digitalWrite(clockPin, 0);
}

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (int which=1; which < 8; which++){
    for (int  numberToDisplay = 1; numberToDisplay < 256; numberToDisplay=numberToDisplay*2) {
      // take the latchPin low so 
      // the LEDs don't change while you're sending in bits:
      //int which=random(1,4);
      digitalWrite(latchPin, LOW);
      // shift out the bits:
      if (which & 1){
        shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
      }else{
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
      }
  
      if (which & 2){
        shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
      }else{
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
      }
  
      if (which & 4){
        shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
      }else{
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
      }
  
      //take the latch pin high so the LEDs will light up:
      
      digitalWrite(latchPin, HIGH);
  
      // pause before next value:
      Serial.print(which*10);
      Serial.print(" ");
      Serial.println(byte(numberToDisplay));
      delay(duration);
    }
      for (int  numberToDisplay = 64; numberToDisplay > 1; numberToDisplay=numberToDisplay/2) {
      // take the latchPin low so 
      // the LEDs don't change while you're sending in bits:
      //int which=random(1,4);
      digitalWrite(latchPin, LOW);
      // shift out the bits:
      if (which & 1){
        shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
      }else{
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
      }
  
      if (which & 2){
        shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
      }else{
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
      }
  
      if (which & 4){
        shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
      }else{
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
      }
  
      //take the latch pin high so the LEDs will light up:
      
      digitalWrite(latchPin, HIGH);
  
      // pause before next value:
      Serial.print(which*10);
      Serial.print(" ");
      Serial.println(byte(numberToDisplay));
      delay(duration);
    }
  }
}
