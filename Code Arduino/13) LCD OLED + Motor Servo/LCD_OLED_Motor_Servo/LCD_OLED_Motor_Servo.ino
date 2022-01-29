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
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Servo Pos: ");
    display.setTextSize(2);
    display.setCursor(0,10);
    display.print(pos);
    display.setTextSize(1);
    display.cp437(true);
    display.write(167);
    display.display();
    
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Servo Pos: ");
    display.setTextSize(2);
    display.setCursor(0,10);
    display.print(pos);
    display.setTextSize(1);
    display.cp437(true);
    display.write(167);
    display.display();
    
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
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
