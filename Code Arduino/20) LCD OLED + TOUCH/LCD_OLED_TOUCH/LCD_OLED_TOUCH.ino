#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int touchPin = 4; 
const int ledPin = 16;

// change with your threshold value
const int threshold = 20;
// variable for storing the touch pin value 
int touchValue;

void setup() {
  Serial.begin(115200);
  pinMode (ledPin, OUTPUT);
  
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

  // read the state of the pushbutton value:
  touchValue = touchRead(touchPin);
  Serial.print("Touch "); Serial.print("\t: "); Serial.println(touchValue);
  
  display.clearDisplay();
  
  // display Touch Value 
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Touch: ");
  display.setTextSize(2);
  display.setCursor(0,15);
  display.print(touchValue);

  if(touchValue < threshold){
    // turn LED on
    digitalWrite(ledPin, HIGH);
    Serial.println(" - LED on");

    display.setTextSize(1);
    display.setCursor(0,35);
    display.print(" - LED on");
    }
    
  else{
    // turn LED off
    digitalWrite(ledPin, LOW);
    Serial.println(" - LED off");

    display.setTextSize(1);
    display.setCursor(0,35);
    display.print(" - LED off");
    }

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
