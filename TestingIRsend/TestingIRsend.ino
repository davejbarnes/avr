#include <IRremote.h>
IRsend irsend;

int bitCount(unsigned int n) {
  int counter = 0;
  while(n) {
      counter += n % 2;
      n >>= 1;
  }
  return counter;
}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  Serial.print("4C = ");
  Serial.println(0x4C);
  
}

void loop() {
  // put your main code here, to run repeatedly:

for (int i=1; i<11; i++){
  irsend.sendRC5(0x4C, bitCount(0x4c));
  delay(50);
  irsend.sendRC5(0x84C, bitCount(0x84c));
  delay(50);
}


//  analogWrite(3,255);
//  delay(500);
//  analogWrite(3,0);

  delay(5000);

}
