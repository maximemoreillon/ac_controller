void MQTT_setup(){
  Serial.println(F("[MQTT] MQTT setup"));
  MQTT_client.setServer(MQTT_BROKER_ADDRESS, MQTT_PORT);
  MQTT_client.setCallback(MQTT_message_callback);
}

void MQTT_connection_manager(){

  static int MQTT_connected = -1; // 1: connected, 0: disconnected, -1: unknown
  static long last_MQTT_connection_attempt;
  
  if(!MQTT_client.connected()) {
    if(MQTT_connected != 0){
      // MQTT connection status changed to "disconnected"
      MQTT_connected = 0;
      Serial.print(F("[MQTT] Disconnected: "));
      Serial.println(MQTT_client.state());
    }
        
    if(millis() - last_MQTT_connection_attempt > 1000){
      last_MQTT_connection_attempt = millis();

      // Prepare a last will
      StaticJsonDocument<200> outbound_JSON_message;
      outbound_JSON_message["state"] = "disconnected";
      char last_will[100];
      serializeJson(outbound_JSON_message, last_will, sizeof(last_will));
      
      MQTT_client.connect(
        HOSTNAME, 
        MQTT_USERNAME, 
        MQTT_PASSWORD, 
        MQTT_AC_STATUS_TOPIC, 
        MQTT_QOS, 
        MQTT_RETAIN, 
        last_will);
    }
        
  } else {
    if(MQTT_connected != 1){
      // MQTT connection status changed to "connected"
      MQTT_connected = 1;

      Serial.println(F("[MQTT] Connected"));
      Serial.println(F("[MQTT] Subscribing to topics"));
      MQTT_client.subscribe(MQTT_AC_COMMAND_TOPIC);

      Serial.println(F("[MQTT] sending AC state update"));
      MQTT_publish_AC_state();
    }
  }
}

void MQTT_message_callback(char* topic, byte* payload, unsigned int length) {

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
  MQTT_client.publish(MQTT_AC_STATUS_TOPIC, JSONmessageBuffer, MQTT_RETAIN);
}
