#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

for (int x=0; x<9; x++){
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(x,0);
  display.println("Hello, world!");
  display.display();
  delay(50);
  display.clearDisplay();
}
for (int x=8; x>=0; x--){
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(x,0);
  display.println("Hello, world!");
  display.display();
  delay(50);
  display.clearDisplay();
}
}
