/* 
 *  Traffic lights
 *  Dave's first project
*/

int red = 9;          // the PWM pin the red LED is attached to
int amber = 10;       // the PWM pin the amber LED is attached to
int green = 11;       // the PWM pin the green LED is attached to
int pred = 12;
int pgreen = 13;
int pbutton = 8;      // the PWM pin the pedestrian button is attached to
int stopgo = 2000;    // the length of time to be on red or green
int steady = 500;   // the amount of time amber is on with red or on it's own

int brightness = 128;

int pbuttonState = 0;

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(red, OUTPUT);
  pinMode(amber, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(pred, OUTPUT);
  pinMode(pgreen, OUTPUT);
  pinMode(pbutton, INPUT);

  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // set the brightness of red to full
  analogWrite(red, 128);

  pbuttonState = digitalRead(pbutton);
  if (pbuttonState){
    analogWrite(pgreen, 128);
    analogWrite(pred, 0);
  }else{
    analogWrite(pred, 128);
    analogWrite(pgreen, 0);
  }
  
  Serial.println(pbuttonState);
  delay(stopgo);
  analogWrite(amber, 128);
  delay(steady);
  analogWrite(red, 0);
  analogWrite(amber, 0);
  analogWrite(green, 128);
  delay(stopgo);
  analogWrite(amber, 128);
  analogWrite(green, 0);
  delay(steady*1.5);
  analogWrite(amber, 0);
}
