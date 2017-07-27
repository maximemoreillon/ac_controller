void read_DHT(){

  long now = millis();

  // Reading sensor periodically
  if (now - last_DHT_read_time > DHT_READ_PERIOD) {
    last_DHT_read_time = now;
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("DHT reading error");
    }
    else {
      Serial.print("DHT reading successful: ");
      Serial.print(t);
      Serial.print(" C, ");
      Serial.print(h);
      Serial.println(" %");
      
      DHT_temperature = t;
      DHT_humidity = h;
    }
  }

  // Send MQTT value periodically
  if( now - last_DHT_publish_time > DHT_PUBLISH_PERIOD) {
    last_DHT_publish_time = now;
    // Only send valid temperature
    if(DHT_temperature != 0 && DHT_humidity != 0) {
      Serial.println("MQTT publish of DHT data");
      MQTT_publish_DHT(DHT_temperature, DHT_humidity);
    }
  }
}

void read_PIR(){
  int PIR_reading = digitalRead(PIR_PIN);
  if(PIR_reading != last_PIR_reading)
  {
    // Motion sensor reading changed
    last_PIR_reading = PIR_reading;
    if(PIR_reading == HIGH){
      Serial.println("Motion detected");
      Serial.println("MQTT publish of motion sensor status");
      MQTT_client.publish(MQTT_MOTION_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, "MOTION");
    }
    else {
      Serial.println("Motion stopped");
      Serial.println("MQTT publish of motion sensor status");
      MQTT_client.publish(MQTT_MOTION_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, "IDLE");
    }
  }
}
