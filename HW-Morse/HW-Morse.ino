// From http://arduino.cc/forum/index.php/topic,43903.0.html#10

byte text[] = "SOS";         // Transmittet text

// Morse code generator for the Arduino
// Transmitted text is placed in the first line
// Tempo sets the speed of a dot etc.
// Morde code was tanken from http://en.wikipedia.org/wiki/Morse_code
// Text is looping and prior to transmitting the active pin blinks rapidly
// Version 1.0, made by Fletcher Chr

int ledPin = 13;                       // Active led pin
int tempo = 300;                       // timing of one dot


void setup () {
 pinMode(ledPin, OUTPUT);
 Serial.begin(9600);
}

void start(){                         // rapid blink at startup
 for(int i = 0; i < 50; i++){
   digitalWrite(ledPin, HIGH);
   delay(25);
   digitalWrite(ledPin, LOW);
   delay(25);
 }
 delay(7 * tempo);
}

void dot() {                          // we need more dots
 digitalWrite(ledPin, HIGH);
 delay(1 * tempo);
 digitalWrite(ledPin, LOW);
 delay(1 * tempo);
}

void dash() {                        // a dash and a splash
 digitalWrite(ledPin, HIGH);
 delay(3 * tempo);
 digitalWrite(ledPin, LOW);
 delay(1 * tempo);
}

void morse(byte letter) {           // time to transmit
 Serial.println(letter, DEC);
 if (letter == 'a' or letter == 'A') {dot(); dash();}
 if (letter == 'b' or letter == 'B') {dash(); dot(); dot(); dot();}
 if (letter == 'c' or letter == 'C') {dash(); dot(); dash(); dot();}
 if (letter == 'd' or letter == 'D') {dash(); dot(); dot();}
 if (letter == 'e' or letter == 'E') {dot();}
 if (letter == 'f' or letter == 'F') {dot(); dot(); dash(); dot();}
 if (letter == 'g' or letter == 'G') {dash(); dash(); dot();}
 if (letter == 'h' or letter == 'H') {dot(); dot(); dot(); dot();}
 if (letter == 'i' or letter == 'I') {dot(); dot();}
 if (letter == 'j' or letter == 'J') {dot(); dash(); dash(); dash();}
 if (letter == 'k' or letter == 'K') {dash(); dot(); dash();}
 if (letter == 'l' or letter == 'L') {dot(); dash(); dot(); dot();}
 if (letter == 'm' or letter == 'M') {dash(); dash();}
 if (letter == 'n' or letter == 'N') {dash(); dot();}
 if (letter == 'o' or letter == 'O') {dash(); dash(); dash();}
 if (letter == 'p' or letter == 'P') {dot(); dash(); dash(); dot();}
 if (letter == 'q' or letter == 'Q') {dash(); dash(); dot(); dash();}
 if (letter == 'r' or letter == 'R') {dot(); dash(); dot();}
 if (letter == 's' or letter == 'S') {dot(); dot(); dot();}
 if (letter == 't' or letter == 'T') {dash();}
 if (letter == 'u' or letter == 'U') {dot(); dot(); dash();}
 if (letter == 'v' or letter == 'V') {dot(); dot(); dot(); dash();}
 if (letter == 'w' or letter == 'W') {dot(); dash(); dash();}
 if (letter == 'x' or letter == 'X') {dash(); dot(); dot(); dash();}
 if (letter == 'y' or letter == 'Y') {dash(); dot(); dash(); dash();}
 if (letter == 'z' or letter == 'Z') {dash(); dash(); dot(); dot();}
 if (letter == '1') {dot(); dash(); dash(); dash(); dash();}
 if (letter == '2') {dot(); dot(); dash(); dash(); dash();}
 if (letter == '3') {dot(); dot(); dot(); dash(); dash();}
 if (letter == '4') {dot(); dot(); dot(); dot(); dash();}
 if (letter == '5') {dot(); dot(); dot(); dot(); dot();}
 if (letter == '6') {dash(); dot(); dot(); dot(); dot();}
 if (letter == '7') {dash(); dash(); dot(); dot(); dot();}
 if (letter == '8') {dash(); dash(); dash(); dot(); dot();}
 if (letter == '9') {dash(); dash(); dash(); dash(); dot();}
 if (letter == '0') {dash(); dash(); dash(); dash(); dash();}
 if (letter == ' ') {delay(5 * tempo);}                        // This makes 7 * tempo for space

 // and now for some national/special letters
 // values were foundt with Serial.print(letter, DEC)

 if (letter == 166 or letter == 134) {dot(); dash(); dot(); dash();}     // æ/Æ is recognized as 166/134
 if (letter == 184 or letter == 152) {dash(); dash();dash();dot();}      // ø/Ø is recognized as 184/152
 if (letter == 165 or letter == 133) {dot();dash();dash();dot();dash();} // å/Å is recognized as 165/133

 delay(2 * tempo);      // this makes 3 * tempo for letter end, and 7 * tempo for space
}


void loop () {
 start();
 for (int i = 0; i < sizeof(text); i++) {
   morse(text[i]);
 }
}
