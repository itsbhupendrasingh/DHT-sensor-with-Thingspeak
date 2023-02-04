#include <ESP8266WiFi.h>
#include "DHT.h"
String apiKey = "JMNC0NNS9MK48QRD"; // write your "Write API key"
const char* ssid = "Office"; // write your "wifi name"
const char* password = "sauhard12"; // write your "wifi password"
const char* server = "api.thingspeak.com";
WiFiClient client;

DHT dht(D2, DHT11);  // (dht pin no, dht sensor type)
float Hum, Temp;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Serial Begin");
  dht.begin();
        
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
        
        
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
}

void loop() 
{
  DHT_data();

  delay(1000);

  if (client.connect(server,80))
    {
      fwd_to_Thingspeak();
    }
  client.stop();
  Serial.println("Waiting");
  delay(1000);
}

void DHT_data()
{
  Hum = dht.readHumidity();
  Temp = dht.readTemperature();                  
}
void fwd_to_Thingspeak()
{
  String postStr = apiKey;
  postStr +="&field1=";
  postStr += String(Hum);  // Humidity data
  postStr +="&field2=";
  postStr += String(Temp); // Temperature Data
  postStr += "\r\n\r\n";
          
  client.print("POST /update HTTP/1.1\n");
  client.print("Host: api.thingspeak.com\n");
  client.print("Connection: close\n");
  client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  client.print(postStr.length());
  client.print("\n\n");
  client.print(postStr);
          
  Serial.print("Send data to channel-1 ");
  Serial.print("Content-Length: ");
  Serial.print(postStr.length());
  Serial.print("Field-1: ");
  Serial.print(Hum); 
  Serial.print("Field-2: ");
  Serial.print(Temp); 
  Serial.println(" data send");            
}
