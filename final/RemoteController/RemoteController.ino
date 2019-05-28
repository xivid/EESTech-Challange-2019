/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>

const char* ssid     = "SmartCar";
const char* password = "LQWLQWLQW";

const char* host = "192.168.4.1";
const int httpPort = 80;
WiFiClient client;

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
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
    
    // Use WiFiClient class to create TCP connections
    Serial.print("connecting to ");
    Serial.println(host);
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
    }
}

void requestGet(char* url) {
    while (!client.connected()) {
      Serial.println("Client not connected, reconnecting...");
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
      }
    }
    
    // We now create a URI for the request
    Serial.print("Requesting URL: /");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET /") + String(url) + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: Keep-Alive\r\n\r\n");

//    Get response (no need?)
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }
}

int value = 0;
char* commands[] = {"forward", "backward", "left", "right"};
void loop()
{
    // TODO get command
    delay(5000);
    char* command = commands[value++ % 4];
    requestGet(command);
}
