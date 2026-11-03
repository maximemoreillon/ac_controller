int last_PIR_reading = -1; // -1: not read yet

void read_DHT(){

  static long last_DHT_publish_time;
  static long last_DHT_read_time;

  static float DHT_temperature = 0;
  static float DHT_humidity = 0;

  long now = millis();

  // Reading sensor periodically
  if (now - last_DHT_read_time > DHT_READ_PERIOD) {
    last_DHT_read_time = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) return;

    DHT_temperature = t;
    DHT_humidity = h;
  }

  // Send MQTT value periodically
  if( now - last_DHT_publish_time > DHT_PUBLISH_PERIOD) {
    last_DHT_publish_time = now;

    // Only send valid readings
    if(DHT_temperature == 0 || DHT_humidity == 0) return;

    JsonDocument outbound_JSON_message;
    outbound_JSON_message["temperature"] = DHT_temperature;
    outbound_JSON_message["humidity"] = DHT_humidity;

    char JSONmessageBuffer[100];
    serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));
    iot_kernel.mqtt.publish((iot_kernel.mqttBaseTopic + "/temperature").c_str(), JSONmessageBuffer, MQTT_RETAIN);
  }
}

void read_PIR(){
  int PIR_reading = digitalRead(PIR_PIN);
  if(PIR_reading == last_PIR_reading) return;
  last_PIR_reading = PIR_reading;
  publish_PIR_state();
}

void publish_PIR_state(){
  if(last_PIR_reading < 0) return;

  JsonDocument outbound_JSON_message;
  outbound_JSON_message["state"] = last_PIR_reading == HIGH ? "motion" : "idle";

  char JSONmessageBuffer[100];
  serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));
  iot_kernel.mqtt.publish((iot_kernel.mqttBaseTopic + "/motion").c_str(), JSONmessageBuffer, MQTT_RETAIN);
}
