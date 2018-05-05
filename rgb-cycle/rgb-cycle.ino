int redPin = 9;
int greenPin = 10;
int bluePin = 11;

int brightness = 64;

void setup() {
  // put your setup code here, to run once:

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  analogWrite(redPin, 64);
  delay(1000);

  for (int r=0; r<brightness; r=r+4){
    for (int g=0; g<brightness; g=g+4){
      for (int b=0; b<brightness; b=b+4){
        analogWrite(redPin, r);
        analogWrite(greenPin, g);
        analogWrite(bluePin, b);
        Serial.print("R=");
        Serial.print(r);
        Serial.print(" G=");
        Serial.print(g);
        Serial.print(" B=");
        Serial.println(b);
        delay(100);
      }
      delay(100);
    }
    delay(100);
  }
  delay(100);

}
