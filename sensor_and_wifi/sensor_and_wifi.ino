#include <Wire.h>
#include "Adafruit_SI1145.h"
#include "ESP8266WiFi.h"
// Include Sensirion library
#include "SHT1x.h"

Adafruit_SI1145 uv = Adafruit_SI1145();
// Sensor pins
#define dataPin  D7
#define clockPin D6

// variables for iot
const char* server = "api.carriots.com";

const String APIKEY = "61b01d105611b4c76d33080ee64eccd02f1c163625c9bb24ac5648b4f72e0b81";
const String DEVICE = "wemosD1@juanvalen15.juanvalen15"; 

// Variables for the temperature & humidity sensor
float temperature;
float humidity;
float dewpoint;

// Create sensor instance
SHT1x sht1x(dataPin, clockPin);

void setup() {
   
  Serial.begin(115200);

  // configuration for the light sensor -----------------------
  Serial.println("Adafruit SI1145 test");
  if (! uv.begin()) {
    Serial.println("Didn't find Si1145");
    while (1);
  }
  Serial.println("OK!");
  // ----------------------------------------------------------


  // configuration for the wifi module ------------------------
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");  
  // ----------------------------------------------------------
}

void loop() {

  lightSENSOR();
  wifiMODULE();
  SHT1sensor();
 
}



// Light sensor function
void lightSENSOR(){
  Serial.println("===================");
  Serial.print("Vis: "); Serial.println(uv.readVisible());
  Serial.print("IR: "); Serial.println(uv.readIR());
  
  // Uncomment if you have an IR LED attached to LED pin!
  //Serial.print("Prox: "); Serial.println(uv.readProx());

  float UVindex = uv.readUV();
  // the index is multiplied by 100 so to get the
  // integer index, divide by 100!
  UVindex /= 100.0;  
  Serial.print("UV: ");  Serial.println(UVindex);

  delay(1000);  
}


//  WIFI module
void wifiMODULE() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } 
  else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      //Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);  
}


// temp + humidity sensor
void SHT1sensor(){
  float temp_c;
  float temp_f;
  float humidity;
 
  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();
 
  // Print the values to the serial port
  Serial.print("Temperature: ");
  Serial.print(temp_c, DEC);
  Serial.print("C / ");
  Serial.print(temp_f, DEC);
  Serial.print("F. Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(2000);
}


// send stream
