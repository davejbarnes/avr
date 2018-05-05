#include <LiquidCrystal.h>
LiquidCrystal myLCD(10,9,5,4,3,2);


void setup() {
  // put your setup code here, to run once:

  myLCD.begin(16,2);
  myLCD.setCursor(0,0);
  myLCD.print("Hello World!");

}

void loop() {
  // put your main code here, to run repeatedly:

}
