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

    if (isnan(h) || isnan(t)) {
      //Serial.println(F("[DHT] reading error"));
      return;
    }
    
    /*
    Serial.print(F("[DHT] reading successsful: "));
    Serial.print(t);
    Serial.print(" C, ");
    Serial.print(h);
    Serial.println(" %");
    */
    
    DHT_temperature = t;
    DHT_humidity = h;
  }

  // Send MQTT value periodically
  if( now - last_DHT_publish_time > DHT_PUBLISH_PERIOD) {
    last_DHT_publish_time = now;
    // Only send valid temperature
    if(DHT_temperature != 0 && DHT_humidity != 0) {
      // Prerpare a JSON message
      StaticJsonDocument<200> outbound_JSON_message;
  
      // Add the DHT reading to the JSON message
      outbound_JSON_message["temperature"] = (String)DHT_temperature;
      outbound_JSON_message["humidity"] = (String)DHT_humidity;
      
      // Serialize JSON into a char array
      char JSONmessageBuffer[100];
      serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));
  
      // Send the char array
      //Serial.println(F("[MQTT] publish of DHT measurement"));
      iot_kernel.mqtt.publish(MQTT_DHT_STATUS_TOPIC, JSONmessageBuffer, MQTT_RETAIN);
    }
  }
}

void read_PIR(){
  static long last_PIR_reading;
  int PIR_reading = digitalRead(PIR_PIN);
  if(PIR_reading != last_PIR_reading) {
    // Motion sensor reading changed
    last_PIR_reading = PIR_reading;
    
    // Prerpare a JSON message
    StaticJsonDocument<200> outbound_JSON_message;

    // Add the motion detection state to the JSON message
    if(PIR_reading == HIGH){
      outbound_JSON_message["state"] = "motion";
    }
    else {
      outbound_JSON_message["state"] = "idle";
    }

    // Serialize JSON into a char array
    char JSONmessageBuffer[100];
    serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));

    // Send the char array
    //Serial.println(F("[MQTT] publish of motion detector state"));
    iot_kernel.mqtt.publish(MQTT_MOTION_STATUS_TOPIC, JSONmessageBuffer, MQTT_RETAIN);
    
  }
}
