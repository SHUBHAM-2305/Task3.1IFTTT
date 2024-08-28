#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>

// WiFi credentials
const char WIFI_SSID[] = "...";
const char WIFI_PASSWORD[] = "hello12345";

// IFTTT webhook details
const char IFTTT_HOST[] = "maker.ifttt.com";
const String IFTTT_PATH = "/trigger/Sunlight_Alert/with/key/kZlTxirYtYDXfH2ccKYDpgSzjum-B4VBXMV32k2Jas8";

// Light sensor threshold values
const float DAYLIGHT_THRESHOLD = 360.0;

WiFiClient wifiClient;
BH1750 lightSensor;

void setup() 
{
  Serial.begin(9600); 
  connectToWiFi();
  
  Wire.begin();
  lightSensor.begin();
  
  Serial.println(F("BH1750 Light Sensor initialized"));
}

void loop() 
{
  float lightIntensity = lightSensor.readLightLevel();
  Serial.print("Current Light Intensity: ");
  Serial.println(lightIntensity);
  
  checkAndTriggerEvent(lightIntensity);

  delay(30000); // Wait for 30 seconds before reading light again
}

void connectToWiFi() 
{
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi!");
}

void checkAndTriggerEvent(float lightIntensity) 
{
  if (lightIntensity >= DAYLIGHT_THRESHOLD) 
  {
    triggerIFTTTEvent("Daylight");
  } 
}

void triggerIFTTTEvent(String eventName) 
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    connectToWiFi();
  }

  if (wifiClient.connect(IFTTT_HOST, 80)) 
  {
    wifiClient.println("GET " + IFTTT_PATH + " HTTP/1.1");
    wifiClient.println("Host: " + String(IFTTT_HOST));
    wifiClient.println("Connection: close");
    wifiClient.println();
    
    delay(500);
    wifiClient.stop();
    Serial.println("Data sent successfully: " + eventName);
  } 
  else 
  {
    Serial.println("Unable to connect to IFTTT server");
  }
}
