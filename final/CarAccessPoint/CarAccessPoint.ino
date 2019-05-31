/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <AsyncUDP.h>

//#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "SmartCar";
const char *password = "LQWLQWLQW";
char *lastCommand = "";
AsyncUDP udp;
WiFiServer server(80);

/* Car control */
void forward() {
  lastCommand = "forward";
  Serial.println("sending 19->1");
  digitalWrite(19, 1);
}

void cancelForward() {
  lastCommand = "cancelForward";
    Serial.println("sending 19->0");

  digitalWrite(19, 0);
}

void backward() {
  lastCommand = "backward";
      Serial.println("sending 21->1");

  digitalWrite(21, 1);
}

void cancelBackward() {
  lastCommand = "cancelBackward";
        Serial.println("sending 21->0");

  digitalWrite(21, 0);
}

void left() {
  lastCommand = "left";
          Serial.println("sending 22->1");

  digitalWrite(22, 1);
}

void cancelLeft() {
  lastCommand = "cancelLeft";
            Serial.println("sending 22->0");

  digitalWrite(22, 0);
}

void right() {
  lastCommand = "right";
            Serial.println("sending 23->1");

  digitalWrite(23, 1);
}

void cancelRight() {
  lastCommand = "cancelRight";
            Serial.println("sending 23->0");

  digitalWrite(23, 0);
}

/* Car control end */

void setup() {
//  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  digitalWrite(19, 0);
  digitalWrite(21, 0);
  digitalWrite(22, 0);
  digitalWrite(23, 0);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("ssid: ");
  Serial.println(ssid);
  Serial.print("password: ");
  Serial.println(password);
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("Setup HTTP server on port 80");
  server.begin();

  while (!udp.listen(1234)) {
    Serial.println("UDP unable to listen on 1234, retrying...");
  }
  Serial.print("UDP Listening on IP: ");
  Serial.println(WiFi.localIP());
  udp.onPacket([](AsyncUDPPacket packet) {
//      Serial.print("UDP Packet Type: ");
//      Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
//      Serial.print(", From: ");
//      Serial.print(packet.remoteIP());
//      Serial.print(":");
//      Serial.print(packet.remotePort());
//      Serial.print(", To: ");
//      Serial.print(packet.localIP());
//      Serial.print(":");
//      Serial.print(packet.localPort());
//      Serial.print(", Length: ");
//      Serial.print(packet.length());
//      Serial.print(", Data: ");
//      Serial.write(packet.data(), packet.length());
//      Serial.println();
      if (packet.length() > 0) {
        char c = *(char*)packet.data();
        Serial.print("c="); Serial.println(c);
        switch (c) {
          case 'f': forward(); break;
          case 'b': backward(); break;
          case 'l': left(); break;
          case 'r': right(); break;
          case 'F': cancelForward(); break;
          case 'B': cancelBackward(); break;
          case 'L': cancelLeft(); break;
          case 'R': cancelRight(); break;
          default: Serial.println("Unknown data, dropped");
        }
        
        //reply to the client
        packet.printf("Got %c", c);
      }
  });  
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New HTTP Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<h1>Last command:"); client.print(lastCommand); client.print("</h1>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("HTTP Client Disconnected.");
  }
}
