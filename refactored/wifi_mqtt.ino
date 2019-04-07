#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "EESTech"
#define STAPSK  "Challenge2019Accepted"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;


String chipID;
String topic_event_connection;
String topic_command_led;
String topic_event_led;
String topic_event_button_pressed;
String topic_event_button_released;
String topic41;
String topic42;
String topic_status;
String topic_command_set_property;


ESP8266WebServer server(80);
WiFiClientSecure net;
MQTTClient client(512);



/* For MQTT begin */
void connect() {
  //connecting wifi
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("ok");
  //wifi connected

  //connecting mqtt server
  net.setInsecure();
  client.setWill(topic_event_connection.c_str(), "0", true, 2);
  Serial.print("\nconnecting...");
  while (!client.connect(chipID.c_str(), "team2", "cse47uhg")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");
  Serial.println("\npublishing topic_event_connection 1");
  //mqtt server connected
  
  client.publish(topic_event_connection, "1", true, 0);
  
  //subscribe topics
  client.subscribe(topic_command_led);
  client.subscribe(topic_command_set_property);
}

void setupMQTT() {
  chipID = String(ESP.getChipId(), HEX);
  
  topic_event_connection = "/teams/team2/devices/";
  topic_event_connection += chipID;
  topic_event_connection += "/events/connection";

  topic_command_led = "/teams/team2/devices/";
  topic_command_led += chipID;
  topic_command_led += "/commands/led";

  topic_event_led = "/teams/team2/devices/";
  topic_event_led += chipID;
  topic_event_led += "/events/led_state";

  topic_event_button_pressed = "/teams/team2/devices/";
  topic_event_button_pressed += chipID;
  topic_event_button_pressed += "/events/button/pressed";
  
  topic_event_button_released = "/teams/team2/devices/";
  topic_event_button_released += chipID;
  topic_event_button_released += "/events/button/released";
  
  topic41 = "/teams/team2/devices/";
  topic41 += chipID;
  topic41 += "/events/light";
  
  topic42 = "/teams/team2/devices/";
  topic42 += chipID;
  topic42 += "/events/dht";

  topic_status = "/teams/team2/devices/";
  topic_status += chipID;
  topic_status += "/status";

  topic_command_set_property = "/teams/team2/devices/";
  topic_command_set_property += chipID;
  topic_command_set_property += "/commands/set_property";
  
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  //
  // MQTT brokers usually use port 8883 for secure connections.
  client.begin("vm1.siroki.cc", 8883, net);
  client.onMessage(messageReceived);

  connect();
}
/* For MQTT end */
