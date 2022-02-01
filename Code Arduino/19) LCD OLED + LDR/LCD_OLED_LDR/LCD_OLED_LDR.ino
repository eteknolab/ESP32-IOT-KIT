#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 34;

// variable for storing the potentiometer value
int potValue = 0;

void setup() {
  Serial.begin(115200);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
  display.display();
  delay(4000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  lcdOpening("LDR");
}

void loop() {
  delay(100);
  
  // Reading potentiometer value
  potValue = analogRead(potPin);
  Serial.print("LDR "); Serial.print("\t: "); Serial.println(potValue);

  display.clearDisplay();
  
  // display ADC Potentiometer 
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LDR: ");
  display.setTextSize(2);
  display.setCursor(0,15);
  display.print(potValue);

  display.display(); 
  }

void lcdOpening(char *teks){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,10);
  display.print("Test Sensor");
  display.setTextSize(2);
  display.setCursor(0,30);
  display.print(teks); 

  display.display(); 
  delay(5000);
  }
