
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
