/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
  Written by BARRAGAN and modified by Scott Fitzgerald
*********/
#include <Adafruit_SSD1306.h>
#include <Servo.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
const int potPin = 34;
int potValue = 0;

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  lcdOpening("M. Servo");
  
  myservo.attach(13);  // attaches the servo on pin 13 to the servo object
}

void loop() {
  potValue = analogRead(potPin);
  pos = map(potValue, 0,4095,0,180);
  myservo.write(pos);

  display.clearDisplay();
  
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Servo Pos: ");
  display.setTextSize(2);
  display.setCursor(0,11);
  display.print(pos);
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);

  display.setTextSize(1);
  display.setCursor(0,30);
  display.print("ADC:");
  display.setTextSize(2);
  display.setCursor(0,40);
  display.print(potValue);
  
  display.display();
  delay(100);  
}


void lcdOpening(char *teks){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,10);
  display.print("Test Aktuator");
  display.setTextSize(2);
  display.setCursor(0,30);
  display.print(teks); 

  display.display(); 
  delay(5000);
}
