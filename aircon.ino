/*
 * AIR CONDITIONER CONTROLLER
 * Maxime MOREILLON
 * 
 * Board type: Wemos D1 Mini
 * 
 */


// Libraries
#include <ESP8266WiFi.h> // Main ESP8266 library
#include <ArduinoOTA.h> // OTA update library
#include <WiFiUdp.h> // Required for OTA
#include <AsyncMqttClient.h> // MQTT library
#include <Ticker.h> // Used when reconecting MQTT upon wifi drop
#include <DHT.h> // Temperature and humidity sensor
#include <ArduinoJson.h>

#include "credentials.h"

// Wifi
Ticker wifi_reconnect_timer;
WiFiEventHandler wifi_connect_handler;
WiFiEventHandler wifi_disconnect_handler;

// OTA
#define HOSTNAME "aircon"

// MQTT
AsyncMqttClient MQTT_client;
Ticker MQTT_reconnect_timer;
#define MQTT_BROKER_ADDRESS IPAddress(192, 168, 1, 2)
#define MQTT_PORT 1883

#define MQTT_AC_COMMAND_TOPIC "ac/living/command"
#define MQTT_AC_STATUS_TOPIC "ac/living/status"
#define MQTT_HEATER_COMMAND_TOPIC "heater/living/command"
#define MQTT_HEATER_STATUS_TOPIC "heater/living/status"

#define MQTT_MOTION_STATUS_TOPIC "motion/living/status"
#define MQTT_DHT_STATUS_TOPIC "dht/living/status"

#define MQTT_QOS 1
#define MQTT_RETAIN true

// Pin mapping
#define DHT_PIN D1
#define PIR_PIN D2
#define IR_LED_PIN D6

// AC variables
char* AC_status;
char* heater_status;

// IR LED parameters
byte IR_signal[] = {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,0,1,1,0,1,1,0,0,1,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,0,1,1,0,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,1,0,1,1,1,1,0,0,0,1,0,1,1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};

// IR signal bit mapping
#define TEMPERATURE_BITS_INDEX 106
#define MODE_BITS_INDEX 200
#define POWER_BIT_INDEX 220
#define FAN_BITS_INDEX 204
#define SPECIAL_BITS_INDEX 88

#define TEMPERATURE_BIT_COUNT 5
#define MODE_BIT_COUNT 3
#define FAN_BIT_COUNT 3
#define SPECIAL_BIT_COUNT 8
#define POWER_BIT_COUNT 1

#define POWER_ON 1
#define POWER_OFF 0

#define MODE_COOLING 3
#define MODE_HEATING 6

#define TEMPERATURE_HEATING 20
#define TEMPERATURE_COOLING 27

#define FAN_SILENT 1
#define FAN_LOW 2
#define FAN_MID 3
#define FAN_HIGH 4
#define FAN_AUTO 5

// IR signal timing
#define START_PULSE 3424
#define START_PAUSE 1663
#define PULSE_LEN 485
#define PAUSE_HIGH 1175
#define PAUSE_LOW 346

// PIR variables
int last_PIR_reading;

// DHT variables
DHT dht(DHT_PIN, DHT22);
#define DHT_PUBLISH_PERIOD 300000 // [ms] = 5 minutes
#define DHT_READ_PERIOD 10000 // [ms] = 10 seconds
long last_DHT_publish_time, last_DHT_read_time;

float DHT_temperature = 0;
float DHT_humidity = 0;

void setup() {

  // Mandatory initial delay
  delay(10);

  // Serial init
  Serial.begin(115200);
  Serial.println();
  Serial.println(); // Separate serial stream from initial gibberish
  Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));

  // IO init
  pinMode(IR_LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  dht.begin();
  wifi_setup();
  MQTT_setup();
  OTA_setup();

  wifi_connect();
}

void loop() {
  ArduinoOTA.handle();
  read_PIR();
  read_DHT();
}
