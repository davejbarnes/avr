
int pwait = 3;        // pin the waiting LED is on
int pred = 5;         // pin the pedestrian red LED is on
int pgreen = 6;       // pin the pedestrian green LED is on
int pbutton = 8;      // pin the pedestrian button is attached to
int red = 9;          // pin the red LED is attached to
int amber = 10;       // pin the amber LED is attached to
int green = 11;       // pin the green LED is attached to
int pbuzzer = 12;     // pin the buzzer is on

int stopgo = 6000;    // the length of time to be on red or green
int steady = 1500;    // the amount of time amber is on with red or on it's own
int walking = 4000;   // amount of time for pedestrian green light to be on
int flash = 300;      // flash interval for amber
int brightness = 128;  // 0-255, the brightness of the LEDs

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

  Serial.begin(9600);

  Serial.println("Starting pin assignment");
  for (int p = 3; p < 13; p++) {
    pinMode(p, OUTPUT);
    Serial.print("Setting pin ");
    Serial.print(p);
    Serial.println(" as ouput");
  }

  Serial.print("Setting pin ");
  Serial.print(pbutton);
  Serial.println(" as input");
  pinMode(pbutton, INPUT);

  analogWrite(pred, brightness);

  Serial.println("Setup complete");
}

void loop() {

  pbuttonState = digitalRead(pbutton);

  if (pbuttonState & requested == 0) {
    Serial.println("Crossing requested");
    analogWrite(pwait, brightness);
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
        analogWrite(red, brightness);
        if (requested == 1 & millis() > crossperiod) {
          delay(steady);
          Serial.println("Crossing now");
          analogWrite(pred, 0);
          analogWrite(pgreen, brightness);
          analogWrite(pwait, 0);
          int freq = walking / 12;
          for (int b = 1; b < 9; b++) {
            digitalWrite(pbuzzer, HIGH);
            delay(freq);
            digitalWrite(pbuzzer, LOW);
            delay(freq / 2);
          }
          delay(freq * 2);
          analogWrite(pgreen, 0);
          analogWrite(red, 0);
                    
          Serial.println("Flashing");
          for (int f = 1; f < 8; f++) {
            analogWrite(amber, brightness);
            analogWrite(pgreen, brightness);
            delay(flash);
            analogWrite(amber, 0);
            analogWrite(pgreen, 0);
            delay(flash);
          }
          analogWrite(pred, brightness);
          Serial.println("Don't cross!");
          analogWrite(amber, brightness);
          delay(steady);
          Serial.println("Continuing to green");
          analogWrite(amber, 0);
          currentState = 2;
          previousState = 1;
          requested = 0;
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
        analogWrite(amber, 0);
        analogWrite(red, 0);
        previousState = 1;
      } else {
        analogWrite(amber, brightness);
        break;
      }

    case 2:
      if (currentState != previousState) {
        endTime = millis() + stopgo;
        previousState = currentState;
      }
      if (millis() > endTime) {
        currentState = 3;
        analogWrite(green, 0);
        previousState = 2;
      } else {
        analogWrite(green, brightness);
        break;
      }

    case 3:
      if (currentState != previousState) {
        endTime = millis() + (steady * 1.5);
        previousState = currentState;
      }
      if (millis() > endTime) {
        currentState = 0;
        analogWrite(amber, 0);
        analogWrite(red, brightness);
        previousState = 3;
      } else {
        analogWrite(amber, brightness);
        break;
      }
  }

  delay(100);
}


