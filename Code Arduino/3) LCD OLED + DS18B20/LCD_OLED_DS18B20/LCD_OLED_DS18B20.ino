#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 14;     

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  lcdOpening("DS18B20");
}

void loop() {
  delay(1000);
   
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  
  Serial.print("Temp C "); Serial.print("\t: "); Serial.print(temperatureC);Serial.println(" ºC");
  Serial.print("Temp F "); Serial.print("\t: "); Serial.print(temperatureF);Serial.println(" ºF");
  Serial.println();

  display.clearDisplay();

  // display temperature C 
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature C: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(temperatureC);
  display.print(" ");
  display.setTextSize(1);
  display.write(167);
  display.setTextSize(2);
  display.print("C");

    // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Temperature F: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(temperatureF);
  display.print(" ");
  display.setTextSize(1);
  display.write(167);
  display.setTextSize(2);
  display.print("F");

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
