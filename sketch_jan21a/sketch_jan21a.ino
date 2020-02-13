#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTTYPE DHT11

#define dht_pin 0
DHT dht(dht_pin, DHTTYPE);


String hum_str;
char hum[50];
String topic_str;
char top[50];
char data[50];
const char* ssid = "dellpc";
const char* password =  "cengizberat";
const char* mqttServer = "hairdresser.cloudmqtt.com";
const int mqttPort = 18583;
const char* mqttUser = "zvqjyyxs";
const char* mqttPassword = "TCPXdcN-wbgN";

 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
 
  WiFi.begin(ssid, password);
  dht.begin(); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  
  client.subscribe("test/home");
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  String topic_str = topic;
  if (topic_str == "test/home"){
    Serial.print("mesaj vaar su topicten  :");
    Serial.println(topic_str);
    digitalWrite(LED_BUILTIN, LOW);   
    delay(4000);                      
    digitalWrite(LED_BUILTIN, HIGH);    
    delay(1000);
    for (int i = 0; i < length; i++) {
      data[i] = payload[i];
      
      }
    
    
    Serial.print(data);
    
  }
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    data[i]= payload[i];
    
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  client.loop();
  float h = dht.readHumidity();
  hum_str = String(h);
  hum_str.toCharArray(hum, hum_str.length() + 1);
  
  client.publish("test/plant", hum);
  delay(2000);
}
