#include <time.h>
#include <MQTT.h>

#include "photo.h"
#include "mydht.h"
#include "neopixel.h"

unsigned long dhtMeasurementPeriodMs = 10000;
unsigned long lightMeasurementPeriodMs = 10000;

const int PotentionmeterPin = A0;
int lastPotention = analogRead(PotentionmeterPin);

const int buttonPin = D3;
const int ledPin = 13;
int ledStatus = 0;
int count =0;

/* different publish function*/

/* For LED begin */
void setLed(int status) {
  digitalWrite(ledPin, status);
  ledStatus = status;
  client.publish(topic_event_led, String(status), true, 0);
  Serial.println("\npublishing topic_event_led" + String(status));
}
/* For LED end */

/* publish function for sensors(dht,light) */
unsigned long lastMillisDht = 0, lastMillisLight = 0;
void publishSensorStatus() {
  unsigned long now = millis();
  
  if (now - lastMillisDht > dhtMeasurementPeriodMs) {
    lastMillisDht = now;
    Serial.println("Publishing DHT data");
    client.publish(topic42, dhtLoop());
  }

  if (now - lastMillisLight > lightMeasurementPeriodMs) {
    lastMillisLight = now;
    Serial.println("Publishing Light data");
    client.publish(topic41, String(photoLoop()));
  }
}

/* publish function for json status */
unsigned long lastMillisMqtt = 0;
void publishMqttStatus() {
  if(millis() - lastMillisMqtt > 30000) {
    lastMillisMqtt=millis();
    Serial.println("Publishing JSON status");
    client.publish(topic_status, getStatusJson());
  }
}
/* publish function for keypad */
void publishKeypad(){
  char k;
  if(k=customKeypad.getKey()){
   client.publish("/teams/team2/devices/c715d/events/keypad", String(k));
  }
}

/* publish fucntion for potentiometer */
void publishPotentiometer(){
  if((abs(analogRead(PotentionmeterPin) - lastPotention)  > 10) || (count == 1)){
    client.publish("/teams/team2/devices/24b5e8/events/potmeter", String(analogRead(PotentionmeterPin)));
    lastPotention = analogRead(PotentionmeterPin);
    count ++;
  }
}

//what's this?
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if (topic == topic_command_led) {
    if (payload == "1") { 
      setLed(1); 
    }
    else if (payload == "0") {
      setLed(0);
    }
  } else if (topic == topic_command_set_property) {
    int colonIndex = payload.indexOf('=');
    String key = payload.substring(0, colonIndex);
    String val = payload.substring(colonIndex + 1);
    if (key == "dhtMeasurementPeriodMs") {
      dhtMeasurementPeriodMs = val.toInt();
      Serial.println("Set dhtMeasurementPeriodMs to " + String(val.toInt()));
    } else if (key == "lightMeasurementPeriodMs") {
      lightMeasurementPeriodMs = val.toInt();
      Serial.println("Set lightMeasurementPeriodMs to " + String(val.toInt()));
    }
  }
}


void setup(void) {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  setLed(0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  photoSetup();
  dhtSetup();
   
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("team2-2")) {
    Serial.println("MDNS responder started");
  }

  setupWebServer();

  setupMQTT();

  strip.Begin();
}


void loop(void) {  
  server.handleClient();
  MDNS.update();

  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    Serial.println("[WARN] MQTTClient disconnected! Reconnecting...");
    connect();
  }

  checkButtonDebounced();

  publishSensorStatus();

  publishMqttStatus();

  updateNeoClock(); 

  publishKeypad()

}
