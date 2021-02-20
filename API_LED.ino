void SensorLed_On (WiFiClient client){
  digitalWrite(LED_BUILTIN, false);
  print_to_web("{\"status\": \"Ok\"}", client, false);
}

void SensorLed_Off(WiFiClient client) {
  digitalWrite(LED_BUILTIN, true);
  print_to_web("{\"status\": \"Ok\"}", client, false);
}

void SensorLed_Status(WiFiClient client) {
  String str = String("{\"Led status\": \"");
  if (get_LedStatus()) {
    str = str + String("On");
  } else {
    str = str + String("Off");
  }
  str = str + String("\"}");
  print_to_web(str, client, false);  
}

boolean get_LedStatus () {
  return (!digitalRead(LED_BUILTIN));
}
