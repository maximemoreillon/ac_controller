void MQTT_setup(){

  // Callbacks
  MQTT_client.onConnect(MQTT_connect_callback);
  MQTT_client.onDisconnect(MQTT_disconnect_callback);
  MQTT_client.onSubscribe(MQTT_subscribe_callback);
  MQTT_client.onUnsubscribe(MQTT_unsubscribe_callback);
  MQTT_client.onMessage(MQTT_message_callback);
  MQTT_client.onPublish(MQTT_publish_callback);

  // Settings
  MQTT_client.setServer(MQTT_BROKER_ADDRESS, MQTT_PORT);
  MQTT_client.setCredentials(MQTT_USERNAME, MQTT_PASSWORD);
}

void MQTT_connect() {
  Serial.println("MQTT connecting...");

  MQTT_reconnect_timer.detach();
  MQTT_client.connect();
}

void MQTT_connect_callback(bool sessionPresent) {
  Serial.println("MQTT connected");

  // Subscribing to command topics
  MQTT_client.subscribe(MQTT_AC_COMMAND_TOPIC, MQTT_QOS);
  MQTT_client.subscribe(MQTT_HEATER_COMMAND_TOPIC, MQTT_QOS);

  // Update AC status
  MQTT_client.publish(MQTT_AC_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, AC_status);
  MQTT_client.publish(MQTT_HEATER_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, heater_status);
}

void MQTT_disconnect_callback(AsyncMqttClientDisconnectReason reason) {
  Serial.println("MQTT disconnected");

  if (WiFi.isConnected()) {
    MQTT_reconnect_timer.attach(2, MQTT_connect);
  }
}

void MQTT_subscribe_callback(uint16_t packetId, uint8_t qos) {
  Serial.println("MQTT subscribed");
}

void MQTT_unsubscribe_callback(uint16_t packetId) {
  Serial.println("MQTT unsubscribed");
}

void MQTT_message_callback(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {

  Serial.print("MQTTT message received: ");
  Serial.print("  topic: ");
  Serial.print(topic);
  Serial.print("  payload: ");
  Serial.print(payload);
  Serial.print("  qos: ");
  Serial.print(properties.qos);
  Serial.print("  dup: ");
  Serial.print(properties.dup);
  Serial.print("  retain: ");
  Serial.print(properties.retain);
  Serial.print("  len: ");
  Serial.print(len);
  Serial.print("  index: ");
  Serial.print(index);
  Serial.print("  total: ");
  Serial.print(total);
  Serial.println("");

  if(strncmp(payload, "OFF", len)==0) {
    Serial.println("Turning OFF");
    heater_status = "OFF";
    AC_status = "OFF";
    IR_set_bits(POWER_BIT_INDEX, POWER_BIT_COUNT, POWER_OFF);
  }
  else if(strncmp(payload, "ON", len)==0) {

    IR_set_bits(POWER_BIT_INDEX, POWER_BIT_COUNT, POWER_ON);

    if(strncmp(topic, MQTT_HEATER_COMMAND_TOPIC, len)==0){
      Serial.println("Turning heater ON and AC OFF");
      AC_status = "OFF";
      heater_status = "ON";

      IR_set_bits(MODE_BITS_INDEX, MODE_BIT_COUNT, MODE_HEATING);
      IR_set_bits(TEMPERATURE_BITS_INDEX, TEMPERATURE_BIT_COUNT, TEMPERATURE_HEATING);
    }
    else if(strncmp(topic, MQTT_AC_COMMAND_TOPIC, len)==0){
      Serial.println("Turning AC ON and heater OFF");
      AC_status = "ON";
      heater_status = "OFF";

      IR_set_bits(MODE_BITS_INDEX, MODE_BIT_COUNT, MODE_COOLING);
      IR_set_bits(TEMPERATURE_BITS_INDEX, TEMPERATURE_BIT_COUNT, TEMPERATURE_COOLING);
    }
  }

  
  IR_set_bits(FAN_BITS_INDEX, FAN_BIT_COUNT, FAN_SILENT);
  IR_send_signal();

  Serial.println("MQTT publish of air conditioner status");
  MQTT_client.publish(MQTT_AC_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, AC_status);
  MQTT_client.publish(MQTT_HEATER_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, heater_status);

}

void MQTT_publish_callback(uint16_t packetId) {
  Serial.println("MQTT published");
}

void MQTT_publish_DHT(float p_temperature, float p_humidity)
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["temperature"] = (String)p_temperature;
  root["humidity"] = (String)p_humidity;
  char data[200];
  root.printTo(data, root.measureLength() + 1);
  MQTT_client.publish(MQTT_DHT_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, data);
}
