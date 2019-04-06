#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
// #include <ArduinoJson.h>
#include <time.h>
#include <MQTT.h>

#ifndef STASSID
#define STASSID "EESTech"
#define STAPSK  "Challenge2019Accepted"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

WiFiClientSecure net;
MQTTClient client;
String chipID;
String topic_event_connection;
String topic_command_led;
String topic_event_led;
String topic_event_button_pressed;
String topic_event_button_released;

const int buttonPin = D3;
const int ledPin = 13;
int ledStatus = 0;

/* For LED begin */
void setLed(int status) {
  digitalWrite(ledPin, status);
  ledStatus = status;
  client.publish(topic_event_led, String(status), true, 0);
  Serial.println("\npublishing topic_event_led" + String(status));
}
/* For LED end */

/* For MQTT begin */
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("ok");

  net.setInsecure();

  client.setWill(topic_event_connection.c_str(), "0", true, 2);
  Serial.print("\nconnecting...");
  while (!client.connect(chipID.c_str(), "team2", "cse47uhg")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  Serial.println("\npublishing topic_event_connection 1");
  client.publish(topic_event_connection, "1", true, 0);

  client.subscribe(topic_command_led);
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if (topic == topic_command_led) {
    if (payload == "1") { 
      setLed(1); 
    }
    else if (payload == "0") {
      setLed(0);
    }
  }
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
  
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  //
  // MQTT brokers usually use port 8883 for secure connections.
  client.begin("vm1.siroki.cc", 8883, net);
  client.onMessage(messageReceived);

  connect();
}
/* For MQTT end */

unsigned long lastMillis = 0;

void handleRoot() {
  server.send(200, "text/plain", "team2");
}


void handleLed() {
  if (server.hasArg("set")) {
    String val = server.arg("set");
    Serial.print("set=");
    Serial.println(val);
    if (val == "1") {
      setLed(1);
      server.send(200, "text/plain", "turned on the LED");
    }
    else if (val == "0") {
      setLed(0);
      server.send(200, "text/plain", "turned off the LED");
    }
  } else {
    server.send(200, "text/plain", String(ledStatus));
  }
}

void handleStatus(){
  if (server.hasArg("format") && server.arg("format") == "json") {
    /*
    String message = "{\n\"team\":\"team2\",\n\"chipId\":\"";
    message += ESP.getChipId();
    message += "\",\n\"WiFi\":{\n\"MAC\":\"";
    message += WiFi.macAddress();
    message += "\",\n\"IP\":\"";
    server.send(200, "application/json", message);*/
     String message = "{\n\"team\":\"team2\",\n\"chipId\":\"";
     message += String(ESP.getChipId(), HEX);
     message += "\",\n\"WiFi\":{\n\"MAC\":\"";
     message += WiFi.macAddress();
     message += "\",\n\"IP\":\"";
     message += WiFi.localIP().toString();
     message += "\",\n\"GatewayIP\":\"";
     message += WiFi.gatewayIP().toString();
     message += "\",\n\"hostname\":\"";
     message += WiFi.hostname();
     message += "\",\n\"SSID\":\"";
     message += WiFi.SSID();
     message += "\",\n\"BSSID\":\"";
     message += WiFi.BSSIDstr();
     message += "\",\n\"RSSI\":\"";
     message += String(WiFi.RSSI());
     message += "\"},\n";
     message += "\"system\":{\n\"freeHeap\":\"";
     message += ESP.getFreeHeap();
     message += "\",\n\"sketchSize\":\"";
     message += ESP.getSketchSize();
     message += "\",\n\"sketchMD5\":\"";
     message += ESP.getSketchMD5();
     message += "\",\n\"resetReason\":\"";
     message += ESP.getResetReason();
     message += "\",\n\"upTime\":\"";
     message += millis();
     message += "\"\n},";
     message += "\n\"state\":{\n\"led\":";
     message += ledStatus ? "true" : "false";
     message += "\n}\n}";
     server.send(200, "application/json", message);
  } else {
    String message = "team: team2\n";
    message += "chipId: ";
    message += ESP.getChipId();
    message += "\nmac: ";
    message += WiFi.macAddress();
    message += "\nip: ";
    message += WiFi.localIP().toString();
    message += "\nssid: ";
    message += WiFi.SSID();
    message += "\nbssid: ";
    message += WiFi.BSSIDstr();
    message += "\nrssi: ";
    message += String(WiFi.RSSI());
    message += "\nled: ";
    message += String(ledStatus);
    server.send(200, "text/plain", message);
  }
}

void handleTime() {
  configTime(3 * 3600, 0, "time.google.com", "pool.ntp.org");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

  server.send(200, "text/plain", asctime(&timeinfo));
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setupWebServer() {
  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/led", handleLed);

  server.on("/status", handleStatus);

  server.on("/time", handleTime);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}


void setup(void) {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  setLed(0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

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
}

/* Button debouncing and checking */
int buttonState = HIGH;
unsigned long lastPressedMillis = 0;

int lastButtonState = HIGH;   // the previous reading from the input pin
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void checkButtonDebounced() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);  

  unsigned long now = millis();
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:
  
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = now;
  }

  if ((now - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      int newButtonState = reading;
    
      if (buttonState == HIGH && newButtonState == LOW) {
        lastPressedMillis = now;
        Serial.println("publishing pressed");
        client.publish(topic_event_button_pressed, "", true, 0);
      } else if (buttonState == LOW && newButtonState == HIGH) {
        Serial.println("publishing released");
        client.publish(topic_event_button_released, String(now - lastPressedMillis), true, 0);
      }

      buttonState = newButtonState;
    }
  }
  
  lastButtonState = reading;
}
/* Button end */

void loop(void) {
  checkButtonDebounced();
  
  server.handleClient();
  MDNS.update();

  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
}
