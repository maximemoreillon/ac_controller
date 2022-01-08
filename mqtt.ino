void mqtt_config(){
  iot_kernel.mqtt.setCallback(mqtt_message_callback);

  // overwrite default topics
  iot_kernel.mqtt_status_topic = MQTT_AC_STATUS_TOPIC;
  iot_kernel.mqtt_command_topic = MQTT_AC_COMMAND_TOPIC;
}

void mqtt_message_callback(char* topic, byte* payload, unsigned int length) {

  Serial.print(F("[MQTTT] message received on "));
  Serial.print(topic);
  Serial.println(", payload: ");

  // Parsing payload as JSON
  StaticJsonDocument<200> inbound_JSON_message;
  deserializeJson(inbound_JSON_message, payload);

  const char* command_state = inbound_JSON_message["state"];

  if( strcmp(command_state, "OFF")==0 || strcmp(command_state, "off")==0 ) {
    Serial.println(F("[IR] Turning AC and HEATING OFF"));
    IR_send_signal(IR_signal_off);
    AC_state = command_state;
    MQTT_publish_AC_state();
  }
  else if(strcmp(command_state, "ac_on")==0) {
    Serial.println(F("[IR] Turning HEATER ON and AC OFF"));
    IR_send_signal(IR_signal_heater_on);
    AC_state = command_state;
    MQTT_publish_AC_state();
  }
  else if(strcmp(command_state, "heater_on")==0) {
    Serial.println(F("[IR] Turning AC ON and HEATER OFF"));
    IR_send_signal(IR_signal_cooler_on);
    AC_state = command_state;
    MQTT_publish_AC_state();
  }
}

void MQTT_publish_AC_state(){
  StaticJsonDocument<200> outbound_JSON_message;
  outbound_JSON_message["state"] = AC_state;
  char JSONmessageBuffer[100];
  serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(F("[MQTT] publish AC state"));
  iot_kernel.mqtt.publish(MQTT_AC_STATUS_TOPIC, JSONmessageBuffer, MQTT_RETAIN);
}
