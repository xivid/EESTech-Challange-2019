int neo_hour =0;
int neo_minute =0;
unsigned long lastMillisNeoClock = 0;
bool lastBlinkStatusNeoClock = false;
int lastHourIndex = -1;
int lastMinuteIndex = -1;

void handleNeoClock() {
  if (server.hasArg("h") && server.hasArg("m")) {
    neo_hour = server.arg("h").toInt();
    neo_minute = server.arg("m").toInt();
    Serial.println("Set neoClock to " + String(neo_hour) + ":" + String(neo_minute));
    server.send(200, "text/plain", "Set time: " + String(neo_hour) + ":" + String(neo_minute));
  } else {
    server.send(200, "text/plain", "Current time: " + String(neo_hour) + ":" + String(neo_minute));
  }
}

//led clock

void updateNeoClock() {
  if(millis() - lastMillisNeoClock > 500) {
    lastMillisNeoClock = millis();

    int hourIndex = (neo_hour % 12) * 2;
    if (neo_minute > 30) hourIndex++;
    int minuteIndex = int (neo_minute / 2.5);

    if (hourIndex != lastHourIndex || minuteIndex != lastMinuteIndex) {
      for (int i = 0; i < 24; i++) {
        strip.SetPixelColor(i, black);
      }
        
      strip.SetPixelColor(hourIndex, red);
      // strip.SetPixelColor((hourIndex + 23) % 24, red);
      // strip.SetPixelColor((hourIndex + 1) % 24, red);
    
      lastHourIndex = hourIndex;
      lastMinuteIndex = minuteIndex;
    }
    
    strip.SetPixelColor(minuteIndex, lastBlinkStatusNeoClock ? green : (hourIndex == minuteIndex ? red : black)); 
    lastBlinkStatusNeoClock = !lastBlinkStatusNeoClock;
    
    strip.Show();
  }
}
//led clock end
