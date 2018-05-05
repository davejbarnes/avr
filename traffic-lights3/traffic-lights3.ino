/*
 * Traffic lights V3
 * 
 * using a 595 shift register chip
 * 
 * 
 * 
 */


//  Pins to use

int pinSwitch = 9;
int pinData = 11;
int pinClock = 12;
int pinLatch = 8;
int pinBuzzer = 10;

// LED bit values

int tred = 128;
int tyellow = 64;
int tgreen = 32;
int pred = 8;
int pgreen = 4;
int pwait = 1;


int lvalue = 255;

int stage1 = tred + pred;
int stage2 = tred + tyellow + pred;
int stage3 = tgreen + pred;
int stage4 = tyellow + pred;

int stopgo = 4000;
int steady = 1000;

int stage[4] = {stage1, stage2, stage3, stage4};
int times[4] = {stopgo, steady, stopgo, steady};

int flash = 300;      // flash interval for yellow
int brightness = 128;  // 0-255, the brightness of the LEDs

int walking = 4000;   // amount of time for pedestrian green light to be on
unsigned long endTime;              // holds the end time of the current state
unsigned long crossing = 61000;     // time to wait before another crossing
unsigned long crossperiod = 0;      // end time of current crossing embargo

int pbuttonState = 0;   // used to hold the state of the pedestrian button
int requested = 0;      // used to hold the state of crossing requests

int currentState = 0;   // this will hold the traffic light state
                        // 0 = stop
                        // 1 = stop but ready
                        // 2 = go
                        // 3 = ready to stop

int previousState = 99;  // values as above, initial value must be > 0


void setup() {
  // put your setup code here, to run once:

  pinMode(pinData, OUTPUT);
  pinMode(pinClock, OUTPUT);
  pinMode(pinLatch, OUTPUT);
  pinMode(pinSwitch, INPUT);
  pinMode(pinBuzzer, OUTPUT);

  digitalWrite(pinSwitch, LOW);

  digitalWrite(pinLatch, LOW);
  shiftOut(pinData, pinClock, LSBFIRST, lvalue);
  digitalWrite(pinLatch, HIGH);

  Serial.begin(9600);

}

void setLights(){
  if (requested > 0) lvalue = lvalue + 1;
  digitalWrite(pinLatch, LOW);
  shiftOut(pinData, pinClock, LSBFIRST, lvalue);
  digitalWrite(pinLatch, HIGH);  
}

void crossnow(){
  Serial.println("Crosing now");
  lvalue = tred + pgreen;
  setLights();
  digitalWrite(pinBuzzer, HIGH);
  delay(walking);
  digitalWrite(pinBuzzer, LOW);
  Serial.println("Done crossing");
  requested = 0;
}

void loop() {

  pbuttonState = digitalRead(pinSwitch);

  if (pbuttonState & requested == 0) {
    Serial.println("Crossing requested");
    lvalue = lvalue + 1;
    setLights();
    if (currentState > 0) {
      requested = 1;
      Serial.println("Crossing at next red light");
    } else {
      Serial.println("Crossing delayed until next red light");
      requested = 2;
    }
  }
  
   switch (currentState) {

    case 0:
      if (currentState != previousState) {
        endTime = millis();
        previousState = currentState;
      }

      if ((stopgo + endTime) < millis()) {
        currentState = 1;
        previousState = 0;
      } else {
        lvalue = stage[currentState];
        setLights();
        if (requested == 1 & millis() > crossperiod) {
          delay(steady);
          requested = 0;
          crossnow();
          lvalue = 0;
          setLights();
                    
          Serial.println("Flashing");
          for (int f = 1; f < 8; f++) {
            lvalue = tyellow + pgreen + tred;
            setLights();
            delay(flash);
            lvalue = tred;
            setLights();
            delay(flash);
          }
          lvalue = stage[1];
          setLights();
          Serial.println("Don't cross!");
          delay(steady);
          Serial.println("Continuing to green");
          lvalue = 0;
          setLights();
          currentState = 2;
          previousState = 1;
          crossperiod = millis() + crossing;
        }
        break;
      }
    case 1:
      if (currentState != previousState) {
        endTime = millis();
        previousState = currentState;
      }
      if (requested == 2) requested = 1;
      if ((steady + endTime) < millis()) {
        currentState = 2;
        lvalue = 0;
        setLights();
        previousState = 1;
      } else {
        lvalue = stage[currentState];
        setLights();
        break;
      }

    case 2:
      if (currentState != previousState) {
        endTime = millis() + stopgo;
        previousState = currentState;
      }
      if (millis() > endTime) {
        currentState = 3;
        lvalue = 0;
        setLights();
        previousState = 2;
      } else {
        lvalue = stage[currentState];
        setLights();
        break;
      }

    case 3:
      if (currentState != previousState) {
        endTime = millis() + (steady * 1.5);
        previousState = currentState;
      }
      if (millis() > endTime) {
        currentState = 0;
        previousState = 3;
      } else {
        lvalue = stage[currentState];
        setLights();
        break;
      }
  }

  delay(100);

}


