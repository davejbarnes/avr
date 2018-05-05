int pinA = A0; // Connected to CLK on KY-040
int pinB = A1; // Connected to DT on KY-040
int pinLed = 5;
int encoderPosCount = 0;
int pinALast;
int aVal;
boolean clockwise;
int brightness = 25;
unsigned long debounce;

void setup() {
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pinLed, OUTPUT);
  /* Read Pin A
    Whatever state it's in will reflect the last position
  */
  pinALast = digitalRead(pinA);
  Serial.begin(9600);
}


void loop() {
  aVal = digitalRead(pinA);
  if (aVal != pinALast & millis() > debounce) { 
    
    if (digitalRead(pinB) != aVal) { 
      if (encoderPosCount < 10){
        encoderPosCount++;
        analogWrite(pinLed, encoderPosCount * brightness);
      }
      clockwise = true;
    } else {
      if (encoderPosCount > 0){
        encoderPosCount--;
        analogWrite(pinLed, encoderPosCount * brightness);
      }
      clockwise = false;
    }
    Serial.print(char(12));
    Serial.print("Rotated: ");
    if (clockwise) {
      Serial.print("clockwise, ");
    } else {
      Serial.print("counterclockwise,  ");
    }
    Serial.print("Encoder Position: ");
    Serial.println(encoderPosCount);
    debounce = millis() + 70;
  }
  pinALast = aVal;
}
