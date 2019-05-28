
/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include "AsyncUDP.h"
#include "myKeypad.h"

const char* ssid     = "SmartCar";
const char* password = "LQWLQWLQW";

const char* host = "192.168.4.1";
const int httpPort = 80;
AsyncUDP udp;
char* currentCommand = NULL;

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

    if(udp.connect(IPAddress(192,168,4,1), 1234)) {
        Serial.println("UDP connected");
        udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            // if received something, clear command
            currentCommand = NULL;
        });
        //Send unicast
        udp.print("Hello Server!");
    }

    customKeypad.addEventListener(keypadEvent);
}

void sendCommand(char* command) {
    Serial.print("Sending command ");
    Serial.println(command);
    udp.print(command);
}

void loop()
{
    customKeypad.getKey();
    if (currentCommand) {
      sendCommand(currentCommand);
    }
}


// Taking care of some special events.
void keypadEvent(KeypadEvent key){
    switch (customKeypad.getState()){
      case PRESSED:
        Serial.print("PRESSED "); Serial.println(key);
        switch (key) {
          case '2': currentCommand = "forward"; break;
          case '8': currentCommand = "backward"; break;
          case '4': currentCommand = "left"; break;
          case '6': currentCommand = "right"; break;
        }
        break;

     case RELEASED:
        Serial.print("RELEASED "); Serial.println(key);
        switch (key) {
          case '2': currentCommand = "ForwardCancel"; break;
          case '8': currentCommand = "BackwardCancel"; break;
          case '4': currentCommand = "LeftCancel"; break;
          case '6': currentCommand = "RightCancel"; break;
        }
        break;

      case HOLD:
        Serial.print("HOLDING "); Serial.println(key);
        break;
    }
}
