/*
 * Air conditioner controller
 * Maxime MOREILLON
 * 
 * Board type: Wemos D1 Mini
 * 
 */

#include "IotKernel.h"

// Libraries
#include <DHT.h> // Temperature and humidity sensor

//#include "aircon_kitchen_nagoya.h";
#include "aircon_bedroom_nagoya.h";

// Pin mapping
#define DHT_PIN D1
#define PIR_PIN D2
#define IR_LED_PIN D6
#define IR_EMITTER_PIN D6 // alias

// DHT
#define DHT_PUBLISH_PERIOD 60000 // [ms] = 1 minute
#define DHT_READ_PERIOD 10000 // [ms] = 10 seconds

IotKernel iot_kernel("aircon","0.0.1"); 

// AC variables
const char* AC_state = "unknown";

// DHT variables
DHT dht(DHT_PIN, DHT22);

void setup() {

  // Mandatory initial delay
  delay(10);

  // IO init
  pinMode(IR_LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  dht.begin();
  iot_kernel.init();
  mqtt_config();

}

void loop() {
  iot_kernel.loop();
  read_PIR();
  read_DHT();

}
