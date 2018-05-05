// which analog pin to connect
#define SERIESRESISTOR 10000 
#define THERMISTORNOMINAL 10000      
#define TEMPERATURENOMINAL 25
#define BCOEFFICIENT 3950

#define NUMSAMPLES 100

#define PHOTORESISTORPIN A0
#define THERMISTORPIN A1

uint16_t samplesLight[NUMSAMPLES];
uint16_t samplesTemp[NUMSAMPLES];
 
void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);
  Serial.println();
}
 
void loop(void) {
  uint8_t i;
  float averageLight;
  float averageTemp;

  // Get light readings
  for (i=0; i< NUMSAMPLES; i++) {
   samplesLight[i] = analogRead(PHOTORESISTORPIN);
   samplesTemp[i] = analogRead(THERMISTORPIN);
   //delay(50);
  }
  averageLight = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     averageLight += samplesLight[i];
  }
  averageLight /= NUMSAMPLES;
  averageLight = 1023 - averageLight;
  Serial.print(averageLight / 10);
  // end of light readings

  Serial.print(" ");

  averageTemp = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     averageTemp += samplesTemp[i];
  }
  averageTemp /= NUMSAMPLES;
  averageTemp = 1023 / averageTemp - 1;
  averageTemp = SERIESRESISTOR / averageTemp;
 
  float steinhart;
  steinhart = averageTemp / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  steinhart = int(steinhart);
  
  Serial.print(steinhart);
  //Serial.print(" ");
  
  Serial.println();
  delay(100);
}
