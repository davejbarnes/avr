#include <IRremote.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define pin_IR_recieve 10
#define pin_IR_send 3
IRrecv irrecv(pin_IR_recieve);
decode_results results;
IRsend irsend;


int pin_button[6] = {5, 6, 7, 8, 9};
int pin_red = A0;
int pin_green = A1;
int pin_blue = A2;

int sender = 0;
int learn = 1;
int holding = 2;
int controllermode;

unsigned long IRcode[5] = {0, 0, 0, 0};
int IRtype[5] = {-1, -1, -1, -1};

int debounce = 400;
unsigned long debouncebegin = 0;

void setup() {
  // put your setup code here, to run once:

  int16_t  x1, y1;
  uint16_t w, h;

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.getTextBounds("Ready", 0, 32, &x1, &y1, &w, &h);
  int cxleft = int((128-w)/2);
  int cyleft = int(32-h);
  display.setCursor(cxleft,cyleft);
  display.println ("Ready");
  display.display();

  for (int i=0; i<5; i++){
    pinMode(pin_button[i], INPUT_PULLUP);
  }
  
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

  irrecv.enableIRIn();
  Serial.begin(9600);
  controllermode = sender;

  Serial.println("Starting with no codes stored");
  Serial.println("Mode 0 = Sender         Mode 1 = Learning");
  Serial.println();
  
}

int bitCount(unsigned int n) {
  int counter = 0;
  while(n) {
      counter += n % 2;
      n >>= 1;
  }
  return counter;
}


void sendCode(int button){

  switch (IRtype[button]){

  case 1:
    irsend.sendNEC(IRcode[button], bitCount(IRcode[button]));
    Serial.print("Hardware send: ");
    Serial.println(IRcode[button]);

  case 2:
    irsend.sendSony(IRcode[button], bitCount(IRcode[button]));
    Serial.print("Hardware send: ");
    Serial.println(IRcode[button]);

  case 3:
    irsend.sendRC5(IRcode[button], bitCount(IRcode[button]));
    Serial.print("Hardware send: ");
    Serial.println(IRcode[button]);

  case 4:
    irsend.sendRC6(IRcode[button], bitCount(IRcode[button]));
    Serial.print("Hardware send: ");
    Serial.println(IRcode[button]);

  case 11: 
    irsend.sendSAMSUNG(IRcode[button], bitCount(IRcode[button]));
    Serial.print("Hardware send: ");
    Serial.println(IRcode[button]);

  case 5:
    irsend.sendDISH(IRcode[button], bitCount(IRcode[button]));
    Serial.print("Hardware send: ");
    Serial.println(IRcode[button]);

  }
}

void show(String message){
  int16_t  x1, y1;
  uint16_t w, h;
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(1);
  display.getTextBounds(message.c_str(), 0, 0, &x1, &y1, &w, &h);
  int cxleft = int((128-w)/2);
  int cyleft = h;
  display.setCursor(cxleft,cyleft);
  display.clearDisplay();
  display.println(message);
  display.display();
  display.setFont();  
}

void showSmall(String message){
  int l = message.length();
  int xleft = (20 - xleft)/2;
  display.setCursor(xleft,17);
  //display.clearDisplay();
  display.println(message);
  display.display();
  display.setFont();  
}

void loop() {

  if (millis() > debouncebegin + debounce){
    for (int button=0; button < 5; button++){
  
      int button_status = digitalRead(pin_button[button]);
      if (button_status == 0){
        debouncebegin = millis();
        String message;
  
        switch (button){
  
          case 4:
            controllermode = !controllermode;
            message = "Mode " + String(controllermode);
            show(message);
            break;
  
          default:
            if (controllermode == sender){
              if (IRcode[button] != 0){
                 Serial.print(F("sending code "));
                 Serial.print(IRcode[button], HEX);
                 Serial.print(F(", type "));
                 Serial.print(IRtype[button]);
                message = "Sending " + String(button);
                show(message);
                sendCode(button);
              }else{
                 Serial.print(F("No code stored at "));
                 Serial.print(button+1);
                message = "No code at " + String(button);
                show(message);
                message = F("Please store a code");
                showSmall(message);
              }
              
            }else
            
            if (controllermode == holding){
              IRcode[button] = results.value;
              IRtype[button] = results.decode_type;
              controllermode = 0;
              message = "Saved to " + String(button);
              show(message);
              Serial.print(", returning to sender mode");            
            }else
            {
              Serial.print("No code held for storing");
              digitalWrite(pin_blue, LOW);
              digitalWrite(pin_red, HIGH);
              delay(100);
              digitalWrite(pin_red, LOW);
              digitalWrite(pin_blue, HIGH);
              message = "Store what?";
              show(message);
            }
            
        }
        Serial.println();

      }
      
    }
  }
  
  switch (controllermode){
    case 0:
      digitalWrite(pin_green, HIGH);
      digitalWrite(pin_blue, LOW);
      digitalWrite(pin_red, LOW);
      delay(5);
      digitalWrite(pin_green, LOW);
      delay(5);
      break;

    case 1:
      digitalWrite(pin_blue, HIGH);
      digitalWrite(pin_red, LOW);
      digitalWrite(pin_green, LOW);
      delay(5);
      digitalWrite(pin_blue, LOW);
      delay(5);
      break;

    case 2:
      digitalWrite(pin_red, HIGH);
      digitalWrite(pin_blue, HIGH);
      digitalWrite(pin_green, LOW);
      delay(5);
      digitalWrite(pin_red, LOW);
      digitalWrite(pin_blue, LOW);
      delay(5);
      
  }

  if (controllermode == learn){
    if (irrecv.decode(&results)){
      if (results.value != 0xFFFFFFFF){
        show("Received code");
        String code = String(results.value) + " of type " + String(results.decode_type);
        showSmall(code);
        Serial.print(results.value, HEX);
        Serial.print(" type ");
        Serial.println(results.decode_type);
        int itype = results.decode_type;
        if ((itype > 0 && itype < 6) || itype == 11){
          controllermode = holding;
          Serial.print("Currently holding code ");
          Serial.print(results.value, HEX);
          Serial.print(" of type ");
          Serial.println(results.decode_type);
        }
      }
      irrecv.resume();
    }
  }
  
}





