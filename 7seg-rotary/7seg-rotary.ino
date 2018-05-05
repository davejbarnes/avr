// define the Uno pins we'll use

int pinA = 8;                 // pin for switch A on the RE
int pinB = 9;                 // pin for switch B on the RE
int pinLed = 5;               // pin for the indicator LED
int latchPin = 3;             // latch pin on the 595
int clockPin = 4;             // clock pin on the 595
int dataPin = 2;              // data pin on the 595


// define some other values we need

int encoderPosCount = 0;      // stores value set by the RE, 0 to 10
int minPos = 0;               // min value for the RE position
int maxPos = 10;              // max value for the RE position
int pinALast;                 // last read value of switch/pin A
int aVal;                     // latest read value of switch/pin A
boolean clockwise;            // whether last movememnt was clockwise
int brightness = 25;          // brightness step for the indicator LED
unsigned long debounce;       // debouce value to stop conflicting reads from RE

byte seven_seg_digits[11] = { B11111100,  // = 0        Each byte describes the segments
                              B01100000,  // = 1        to light on the 7seg
                              B11011010,  // = 2        A - G
                              B11110010,  // = 3
                              B01100110,  // = 4
                              B10110110,  // = 5
                              B10111110,  // = 6
                              B11100000,  // = 7
                              B11111110,  // = 8
                              B11100110,  // = 9
                              B01101110   // = H
                             };

void setup() {

  //  set the pin modes we need
  
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // set the starting state of switch/pin A
  pinALast = digitalRead(pinA);


  // set the 7seg to display 0 to begin
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, seven_seg_digits[0]);
  digitalWrite(latchPin, HIGH);

  // start the serial output for debug
  Serial.begin(9600);

}

void sevenSegWrite(byte digit) {

  // accept a byte defining the state of the 7seg and
  // write it to the 959

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, seven_seg_digits[digit]);  
  digitalWrite(latchPin, HIGH);
  
}


void loop() {
  aVal = digitalRead(pinA);
  if (aVal != pinALast & millis() > debounce) { 
    
    if (digitalRead(pinB) != aVal) { 
      if (encoderPosCount < maxPos){
        encoderPosCount++;
        analogWrite(pinLed, encoderPosCount * brightness);
        sevenSegWrite(encoderPosCount);
      }
      clockwise = true;
    } else {
      if (encoderPosCount > minPos){
        encoderPosCount--;
        analogWrite(pinLed, encoderPosCount * brightness);
        sevenSegWrite(encoderPosCount);
      }
      clockwise = false;
    }

    debounce = millis() + 70;

    // this is just debug to the serial interface
    
    Serial.print(char(12));
    Serial.print("Rotated: ");
    if (clockwise) {
      Serial.print("clockwise, ");
    } else {
      Serial.print("counterclockwise,  ");
    }
    Serial.print("Encoder Position: ");
    Serial.println(encoderPosCount);
    
  }
  pinALast = aVal;
}
