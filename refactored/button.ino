
/* Button debouncing and checking */
int buttonState = HIGH;
unsigned long lastPressedMillis = 0;

int lastButtonState = HIGH;   // the previous reading from the input pin
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 10;    // the debounce time; increase if the output flickers

void checkButtonDebounced() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);  

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:
  
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      int newButtonState = reading;
    
      if (buttonState == HIGH && newButtonState == LOW) {
        lastPressedMillis = millis();
        Serial.println("publishing pressed");
        client.publish(topic_event_button_pressed, "", true, 0);
      } else if (buttonState == LOW && newButtonState == HIGH) {
        Serial.println("publishing released");
        client.publish(topic_event_button_released, String(millis() - lastPressedMillis), true, 0);
      }

      buttonState = newButtonState;
    }
  }
  
  lastButtonState = reading;
}
/* Button end */
