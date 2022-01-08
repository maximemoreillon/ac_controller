void wifi_setup() {

  Serial.println(F("[WiFi] Wifi starting"));

  WiFi.hostname(HOSTNAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}


void wifi_connection_manager(){

  static int wifi_connected = -1; // 1: connected, 0: disconnected, -1: unknown
  
  if(WiFi.status() != WL_CONNECTED) {
    if(wifi_connected != 0){
      // Wifi connection status changed to "disconnected"
      wifi_connected = 0;
      Serial.println(F("[WiFi] Disconnected"));
    }
  } else {
    if(wifi_connected != 1){
      // Wifi connection status changed to "connected"
      wifi_connected = 1;
      Serial.println(F("[WiFi] Connected"));
    }
  }
  
}
