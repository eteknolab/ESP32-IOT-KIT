#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino_JSON.h>
#include "SPIFFS.h"
#include <Adafruit_SSD1306.h>

const char* ssid = "ASUS12";                //isi dengan nama wifi sumber hostspot
const char* password = "1234567890";        //isi dengan password sumber hostspot
                                            //cek alamat ip di serial monitor

AsyncWebServer server(80);
AsyncEventSource events("/events");
JSONVar readings;
Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

unsigned long lastTime = 0;  
unsigned long lastTimeTemperature = 0;
unsigned long lastTimeAcc = 0;
unsigned long gyroDelay = 10;
unsigned long temperatureDelay = 1000;
unsigned long accelerometerDelay = 200;

sensors_event_t a, g, temp;

float gyroX, gyroY, gyroZ;
float accX, accY, accZ;
float temperature;

//Gyroscope sensor deviation
float gyroXerror = 0.29;
float gyroYerror = 0.00;
float gyroZerror = 0.07;


void initLCD(){
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.display();
  delay(4000);
  delay(500); // Pause for 2 seconds
  display.setTextSize(1);
  display.setTextColor(WHITE);
  lcdOpening("MPU6050");
}

void initMPU(){
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
}

void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

void initWiFi() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting to WiFi..."); display.println("Connecting to WiFi..."); display.display();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println(WiFi.localIP());

  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println(WiFi.localIP());
  display.display();
}

String getGyroReadings(){
  mpu.getEvent(&a, &g, &temp);

  float gyroX_temp = g.gyro.x;
    if(abs(gyroX_temp) > (gyroXerror + 0.01)) {
    gyroX += gyroX_temp/100.00;
//    Serial.print("gyroX_temp = \t"); Serial.println(abs(gyroX_temp));
//    Serial.print("gyroXerror = \t"); Serial.println(gyroXerror);
//    Serial.print("gyroX = \t"); Serial.println(gyroX);
//    Serial.println();
    }

//  if(abs(gyroX_temp) < ( gyroXerror - 0.01))  {
//    gyroX -= gyroX_temp/10.00;
//    }
    
//  static unsigned long nilai;
//
//  if (millis() - nilai > 1000){
//    Serial.print("gyroX_temp = \t"); Serial.println(abs(gyroX_temp));
//    Serial.print("gyroXerror = \t"); Serial.println(gyroXerror);
//    Serial.print("gyroX = \t"); Serial.println(gyroX);
//    Serial.println();
//    nilai = millis();
//    }
  
  float gyroY_temp = g.gyro.y;
  //  gyroY = g.gyro.y;
  if(abs(gyroY_temp) > gyroYerror) {
    gyroY += gyroY_temp/70.00;
  }

  float gyroZ_temp = g.gyro.z;
  //gyroZ = g.gyro.z;
  if(abs(gyroZ_temp) >  (gyroZerror + 0.00)) {
    gyroZ += gyroZ_temp/90.00;
    }

//  if(abs(gyroZ_temp) <  (gyroZerror - 0.02)) {
//    gyroZ -= gyroZ_temp/90.00;
//    }

  readings["gyroX"] = String(gyroX);
  readings["gyroY"] = String(gyroY);
  readings["gyroZ"] = String(gyroZ);

  String jsonString = JSON.stringify(readings);
  return jsonString;
}

String getAccReadings() {
  mpu.getEvent(&a, &g, &temp);
  // Get current acceleration values
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;
  readings["accX"] = String(accX);
  readings["accY"] = String(accY);
  readings["accZ"] = String(accZ);
  String accString = JSON.stringify (readings);
  return accString;
}


String getTemperature(){
  mpu.getEvent(&a, &g, &temp);
  temperature = temp.temperature;
  return String(temperature);
}


void setup() {
  Serial.begin(115200);
  initLCD();
  initWiFi();
  initSPIFFS();
  initMPU();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

   server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroX=0;
    gyroY=0;
    gyroZ=0;
    request->send(200, "text/plain", "OK");
  });

 server.on("/resetX", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroX=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetY", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroY=0;
    request->send(200, "text/plain", "OK");
  });

 server.on("/resetZ", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroZ=0;
    request->send(200, "text/plain", "OK");
  });

 events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  server.begin();

}

void loop() {
 if ((millis() - lastTime) > gyroDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getGyroReadings().c_str(),"gyro_readings",millis());
    lastTime = millis();
    }
  if ((millis() - lastTimeAcc) > accelerometerDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getAccReadings().c_str(),"accelerometer_readings",millis());
    lastTimeAcc = millis();
    }
  if ((millis() - lastTimeTemperature) > temperatureDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getTemperature().c_str(),"temperature_reading",millis());
    lastTimeTemperature = millis();
    }
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
