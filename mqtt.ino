void mqtt_config(){
  iot_kernel.mqtt.setCallback(mqtt_message_callback);

  // NOTE: subscriptions are lost on disconnect, hence subscribing on every connection
  iot_kernel.onMqttConnect([]() {
    iot_kernel.mqtt.subscribe(iot_kernel.mqttCommandTopic.c_str());
    publish_PIR_state();
  });
}

void mqtt_message_callback(char* topic, byte* payload, unsigned int length) {

  Serial.print(F("[MQTT] message received on "));
  Serial.print(topic);
  Serial.print(", payload: ");
  Serial.write(payload, length);
  Serial.println();

  // NOTE: the payload is not null-terminated, hence passing its length
  JsonDocument inbound_JSON_message;
  if(deserializeJson(inbound_JSON_message, payload, length)) return;

  const char* command = inbound_JSON_message["state"];
  if(!command) return;

  if(strcasecmp(command, "off") == 0) IR_send_signal(IR_signal_off, sizeof(IR_signal_off));
  else if(strcmp(command, "ac_on") == 0) IR_send_signal(IR_signal_cooler_on, sizeof(IR_signal_cooler_on));
  else if(strcmp(command, "heater_on") == 0) IR_send_signal(IR_signal_heater_on, sizeof(IR_signal_heater_on));
  else {
    Serial.printf("[MQTT] Unknown command: %s\n", command);
    return;
  }

  Serial.printf("[IR] Sent %s\n", command);
  AC_state = command;
  MQTT_publish_AC_state();
}

void MQTT_publish_AC_state(){
  JsonDocument outbound_JSON_message;
  outbound_JSON_message["state"] = AC_state;
  char JSONmessageBuffer[100];
  serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));
  iot_kernel.mqtt.publish(iot_kernel.mqttStateTopic.c_str(), JSONmessageBuffer, MQTT_RETAIN);
}
