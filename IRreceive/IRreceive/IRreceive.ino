//www.elegoo.com
//2016.06.13

#include <IRremote.h>
 
int RECV_PIN = 12;
int LED [5] = {8,9,10,11};
int brightness = 16;
int flashdelay = 5;

IRrecv irrecv(RECV_PIN);
 
decode_results results;
 
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  for (int i=0; i<4; i++){
    pinMode(LED[i],1);
  }
}
 
void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    for (int z=1; z<5; z++){
      for (int y=1; y<3; y++){
        for (int x=1; x<5; x++){
          for (int i=0; i<4; i++){
            digitalWrite(LED[i], brightness);
            delay(flashdelay);
          }
          delay(brightness);
          for (int i=0; i<4; i++){
            digitalWrite(LED[i], 0);
            //delay(flashdelay);
          }    
         delay(flashdelay);
        }
        delay(flashdelay*10);
      }
      delay(150);
    }
    irrecv.resume(); 
  }
}
