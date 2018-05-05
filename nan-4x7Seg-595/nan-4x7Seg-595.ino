/*
 * 0 to 9999
 * Controlled by 595 8 bit shift register
 * Common cathode 4 x 7seg display
 * 
 * 
 */


// Pins for the 7seg
int digit[4] = {2,3,4,5};

// Pins for the shift register
int pinData = 7;
int pinLatch = 8;
int pinClock = 9;

// Pins for the buttons
int button[4] = {6,10,12};

// Pins for the rotary encoder
int pinA = A0;
int pinB = A1;
int pinC = A2;
int pinLed = A3;

// Variable to hold the current timer value
int currentValue[5] = {1,2,3,4};


//                     dGFEDCBA
byte segments[11] =  {B00111111,  // = 0
                      B00000110,  // = 1
                      B01011011,  // = 2
                      B01001111,  // = 3
                      B01100110,  // = 4
                      B01101101,  // = 5
                      B01111101,  // = 6
                      B00000111,  // = 7
                      B01111111,  // = 8
                      B01100111,  // = 9
                      B00000000   // = clear
                      };

int digStart=0;
unsigned long timerEnd;   // end time of current tick
int timerLength = 1000;    // length of each tick in milliseconds
int counterEnd = 5999;
int counterCurrent = 0;   // number of ticks
int adjustedCounter = 0;  // number of ticks adjusted to mmss

int counterMode = 0;        // 0 - stopped
                            // 1 - counting up
                            // 2 - counting down

int paused = 0;

int pressed = 0;
int pressedNow;
int d, x, pin;
unsigned long embargoButton = 0;    // end time in millis() of button embargo

int encoderPosCount = 0;
int pinALast;
int aVal;
boolean clockwise;
int brightness = 25;
unsigned long debounce;
int correction;



void setup() {
  // put your setup code here, to run once:

  pinMode(pinData, OUTPUT);
  pinMode(pinClock, OUTPUT);
  pinMode(pinLatch, OUTPUT);

  for (d=0; d<7; d++){
    pin = digit[d];
    pinMode(pin, OUTPUT);
    digitalWrite(d, HIGH);
  }

  for (int b=1; b<4; b++){
    pinMode(button[b], INPUT_PULLUP);
  }

  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pinC, INPUT_PULLUP);
  
  Serial.begin(9600);

  timerEnd = 0;

  Serial.print("Starting with counterMode = ");  
  Serial.println(counterMode);

  pinALast = digitalRead(pinA);

  Serial.print("Starting with pinALast = ");  
  Serial.println(pinALast);

  debounce = millis() + 200;
  updateDisplay(0);
}

void updateDisplay(int n){

  String in = String(n);
  int inLength = in.length();

  // Set all digits to clear first
  for (int x=0; x<4; x++){
    currentValue[x]=10;
  }

    // adjust the start of the for loop to have the number right alligned
    digStart=0;
    if (n < 1000) digStart=1;
    if (n < 100) digStart=2;
    if (n < 10) digStart=3;

  // Populate currentValue with numbers 0 to 9 or blank
  // rather than the ASCII values returned by the int() function

  for (int digs=0; digs<4; digs++){
    currentValue[digs+digStart]=int(in[digs])-48;
    if (currentValue[digs]<0) currentValue[digs]=10; // 10 is clear all
  }

  // Set each 7seg to its value and briefly set it's cathode LOW
  for (d=0; d<4; d++){

    //send and set the byte
    digitalWrite(pinLatch, LOW);
    shiftOut(pinData, pinClock, MSBFIRST, segments[currentValue[d]]);
    if (d == 1){
      byte addDot = segments[currentValue[d]];
      bitSet(addDot, 7);
      shiftOut(pinData, pinClock, MSBFIRST, addDot);
    }
    digitalWrite(pinLatch, HIGH);

    // set digit d cathode LOW
    digitalWrite(digit[d], LOW);
    delayMicroseconds(70);
    digitalWrite(digit[d], HIGH);
  }  

}

int readRotary(){

  correction = 0;
  aVal = digitalRead(pinA);
  if (aVal != pinALast & millis() > debounce) { 
    int multi = digitalRead(pinC);
    if (digitalRead(pinB) != aVal) { 
      clockwise = true;
      correction = 1;
    } else {
      clockwise = false;
      correction = -1;
    }
    if (multi < 1) correction = correction * 60;
    debounce = millis() + 70;
  }
  pinALast = aVal;

  counterCurrent = counterCurrent + correction;
  if (counterCurrent < 0) counterCurrent = counterEnd;
  if (counterCurrent > counterEnd) counterCurrent = 0;
}


int readButtons(){
  pressed = 0;
  if (millis() > embargoButton){
    for (int x=0; x<4; x++){
      pressedNow = digitalRead(button[x]);
      if (pressedNow != HIGH){
        pressed = x+1;
        embargoButton = millis()+250;
        if (pressed == 1) return 1;
        if (pressed == 2) return 2;
        if (pressed == 3) return 3;
      }
    }
  }else{
    return 0;
  }
}

void updateCounter(){
  if (millis() > timerEnd){
    timerEnd = millis() + timerLength;
    if (counterMode == 1){
      if (counterCurrent < counterEnd){
        counterCurrent++;
      }else{
        counterCurrent = 0;
        counterMode = 0;
      }
    }else if (counterMode == 2){
      if (counterCurrent > 0){
        counterCurrent--;
      }else{
        counterCurrent = 0;
        counterMode = 0;
      }
    }
    toMinSec(counterCurrent);
  }
}


int toMinSec(int ticks){
    int mins = ticks / 60;
    int secs = ticks - (mins * 60);
    adjustedCounter = (mins*100) + secs;

//    Serial.print("Input : ");
//    Serial.println(ticks);
//    Serial.print("Adjusted to mins and secs : ");
//    Serial.println(adjustedCounter);

}


void loop() {

// Update the counter
  updateCounter();

// Update the 7seg display
  updateDisplay(adjustedCounter);

// Read button status
  int button = readButtons();

  switch (button){

    case 1:
      counterMode = button;
      paused = 0;
      break;
      
    case 2:
      counterMode = button;
      paused = 0;
      break;

    case 3:
      if (paused == 0){
        paused = 1;
        counterMode = button;
      }else{
        paused = 0;
        counterMode = button;
        counterCurrent = 0;
      }
  }

// Read the status of the rotary encoder if we're in mode 0 (stopped)
  if (counterMode == 0 || counterMode == 3 ) readRotary();
  


// end of loop()
}



