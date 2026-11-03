// MQTT
#define MQTT_AC_COMMAND_TOPIC "ac/bedroom/command"
#define MQTT_AC_STATUS_TOPIC "ac/bedroom/status"
#define MQTT_MOTION_STATUS_TOPIC "motion/bedroom/status"
#define MQTT_DHT_STATUS_TOPIC "dht/bedroom/status"

// IR signals, as decoded from the remote (see infrared.ino)
// Each byte is a 4 bit value followed by its complement
// Byte 3: mode (F0: off, D2: cool, C3: heat), byte 6: probably the temperature
const uint8_t IR_signal_off[] = {0x70, 0x8F, 0xF0, 0x3C, 0xF0, 0x4B, 0xF0, 0xF0, 0xD2, 0xF0, 0xF0, 0xF0};
const uint8_t IR_signal_cooler_on[] = {0x70, 0x8F, 0xD2, 0x3C, 0xF0, 0x4B, 0xF0, 0xF0, 0xD2, 0xF0, 0xF0, 0xF0};
const uint8_t IR_signal_heater_on[] = {0x70, 0x8F, 0xC3, 0x3C, 0xF0, 0x1E, 0xF0, 0xF0, 0xD2, 0xF0, 0xF0, 0xF0};
