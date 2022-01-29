/* This code is to use with Adafruit BMP280           (Metric)
 * It measures both temperature and pressure and it displays them on the Serial monitor with the altitude
 * It's a modified version of the Adafruit example code
 * Refer to www.surtrtech.com or SurtrTech Youtube channel
 */

#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_BMP280 bmp; // I2C Interface
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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
  lcdOpening("BMP280");
  
  Serial.println(F("BMP280 test"));

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
    delay(1000);
    
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" ºC");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure()/100); //displaying the Pressure in hPa, you can change the unit
    Serial.println(" hPa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1019.66)); //The "1019.66" is the pressure(hPa) at sea level in day in your region
    Serial.println(" m");                    //If you don't know it, modify it until you get your current altitude

    Serial.println();

    display.clearDisplay();
    
    // display temperature
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Temperature: ");
    display.setTextSize(1);
    display.setCursor(0,10);
    display.print(bmp.readTemperature());
    display.print(" ");
    display.setTextSize(1);
    display.write(167);
    display.setTextSize(1);
    display.print("C");

    // display pressure
    display.setTextSize(1);
    display.setCursor(0, 22);
    display.print("Pressure: ");
    display.setTextSize(1);
    display.setCursor(0, 32);
    display.print(bmp.readPressure()/100);
    display.print(" hPa"); 

    // display Approx altitude
    display.setTextSize(1);
    display.setCursor(0, 45);
    display.print("Approx altitude: ");
    display.setTextSize(1);
    display.setCursor(0, 55);
    display.print(bmp.readAltitude(1019.66));
    display.print(" m"); 
  
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
