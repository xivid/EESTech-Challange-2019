

void handleRoot() {
  server.send(200, "text/plain", "team2");
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
  server.on("/led", handleLed);
  server.on("/status", handleStatus);
  server.on("/time", handleTime);
  server.on("/neoclock", handleNeoClock);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}
