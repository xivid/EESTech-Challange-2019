
String getStatusJson() {
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
  message += "\",\n\"RSSI\":";
  message += String(WiFi.RSSI());
  message += "\n},\n";
  message += "\"system\":{\n\"freeHeap\":";
  message += ESP.getFreeHeap();
  message += ",\n\"sketchSize\":";
  message += ESP.getSketchSize();
  message += ",\n\"sketchMD5\":\"";
  message += ESP.getSketchMD5();
  message += "\",\n\"resetReason\":\"";
  message += ESP.getResetReason();
  message += "\",\n\"upTime\":";
  message += millis();
  message += "\n},";
  message += "\n\"state\":{\n\"led\":";
  message += ledStatus ? "true" : "false";
  message += "\n},\n\"properties\":{";
  message += "\n\"dhtMeasurementPeriodMs\":" + String(dhtMeasurementPeriodMs);
  message += ",\n\"lightMeasurementPeriodMs\":" + String(lightMeasurementPeriodMs);
  message += "\n}\n}";
  return message;
}



void handleStatus(){
  if (server.hasArg("format") && server.arg("format") == "json") {
    server.send(200, "application/json", getStatusJson());
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
