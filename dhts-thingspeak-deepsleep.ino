/*
 * low consumption temp/humidity sensor
 * inspiration
 * https://openhomeautomation.net/esp8266-battery/
 */
 
// Library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <Interval.h>
#include <ThingSpeak.h>

#define DHTTYPE DHT11   // DHT Shield uses DHT 11
#define DHTPIN 2       // DHT Shield uses pin D4 (GPIO2)

// ThingSpeak
unsigned long myChannelNumber = XXXXXX;
const char * myWriteAPIKey = "XXXXXXXXXXXXX";

// WiFi settings
const char* ssid = "xxxxxx";
const char* password = "xxxxxxxxxxxxxxxxxxxx";

// Time to sleep (in seconds):
const int sleepTimeS = 10*60;

void write_ThingsSpeak(){

  float humidity, temperature; // raw values from sensors
  DHT dht(DHTPIN, DHTTYPE);

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  //send data to thingSpeak
  ThingSpeak.setField(1,temperature);
  ThingSpeak.setField(2,humidity);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);        
}

void setup() 
{

  // Serial
  Serial.begin(9600);
  Serial.println("ESP8266 in normal mode");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Print the IP address
  Serial.println(WiFi.localIP()); 
}

void loop() 
{
  // Use WiFiClient class to create TCP connections
  WiFiClient wfclient;

  //setup thingSpeak
  Serial.println("start sending data to ThingSpeak.com");
  ThingSpeak.begin(wfclient);
  write_ThingsSpeak();
  Serial.println("end sending data to ThingSpeak.com");

  Serial.println();
  Serial.println("closing connection");

  // Sleep
  Serial.println("ESP8266 in sleep mode");
  ESP.deepSleep(sleepTimeS * 1000000);
}
