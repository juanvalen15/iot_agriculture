#include <Wire.h>
#include "Adafruit_SI1145.h"
#include "ESP8266WiFi.h"
#include "SHT1x.h" // Include Sensirion library

Adafruit_SI1145 uv = Adafruit_SI1145();
// Sensor pins
#define dataPin  D7
#define clockPin D6

// variables for iot
const char* ssid = "ziggo-ap-3836909";
const char* password = "UU2[-SUB4b:U3";
const char* server = "api.carriots.com";

const String APIKEY = "61b01d105611b4c76d33080ee64eccd02f1c163625c9bb24ac5648b4f72e0b81";
const String DEVICE = "wemosD1@juanvalen15.juanvalen15";

WiFiClient client;

// Variables for the temperature & humidity sensor
float temperature;
float humidity;
float dewpoint;

// Create sensor instance
SHT1x sht1x(dataPin, clockPin);

void setup() {
 Serial.begin(115200);
 delay(1000);
 // start wifi
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }

 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}






// send stream
void sendStream()
{
 // SENSOR TEMPERATURE
  float temp_c;
  float temp_f;
  float humidity;

  // Read values from the sensor
  temperature   = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();


  int   UVvisible = uv.readVisible();
  int UVIR = uv.readIR();
  float UVindex = uv.readUV()/100;



 //const int httpPort = 80;
 if (client.connect(server, 80)) { // If there's a successful connection
 Serial.println(F("connected"));

 //sensors.requestTemperatures();
 //Serial.println(sensors.getTempCByIndex(0));
 String temperature = String(temperature);

 // construct the data json
String json = "{\"protocol\":\"v2\",\"device\":\"" + DEVICE + "\",\"at\":\"now\",\"data\":{\"temperature\":\"" + temperature + "\"}}";

 // Make an HTTP request
 client.println("POST /streams HTTP/1.1");
 client.println("Host: api.carriots.com");
 client.println("Accept: application/json");
 client.println("User-Agent: Arduino-Carriots");
 client.println("Content-Type: application/json");
 client.print("carriots.apikey: ");
 client.println(APIKEY);
 client.print("Content-Length: ");
 int thisLength = json.length();
 client.println(thisLength);
 client.println("Connection: close");
 client.println();
 client.println(json);



 while(client.available()){
 String line = client.readStringUntil('\r');
 Serial.print(line);
 }


 }
 else {
 // If server connection failed:
 Serial.println(F("connection failed"));
 }
}




void loop() {
 Serial.println(F("SENDING...."));
 sendStream();
 delay(120000);
}
