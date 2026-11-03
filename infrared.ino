// Protocol of the AC remote, as recorded (see the README for the reference):
// header, then each byte LSB first, a bit being a mark followed by a short (0) or long (1) space.
// Each signal is sent twice.
#define IR_HEADER_MARK 3000 // [us]
#define IR_HEADER_SPACE_1 3000 // [us]
#define IR_HEADER_SPACE_2 4400 // [us]
#define IR_BIT_MARK 530 // [us]
#define IR_ZERO_SPACE 550 // [us]
#define IR_ONE_SPACE 1650 // [us]
#define IR_REPEAT_SPACE 39500 // [us]

void IR_send_signal(const uint8_t* bytes, size_t length) {
  for(int repeat = 0; repeat < 2; repeat++) {
    if(repeat) IR_send_pause(IR_REPEAT_SPACE);

    IR_send_pulse(IR_HEADER_MARK);
    IR_send_pause(IR_HEADER_SPACE_1);
    IR_send_pulse(IR_HEADER_MARK);
    IR_send_pause(IR_HEADER_SPACE_2);

    for(size_t i = 0; i < length; i++) {
      for(int bit = 0; bit < 8; bit++) {
        IR_send_pulse(IR_BIT_MARK);
        IR_send_pause((bytes[i] >> bit) & 1 ? IR_ONE_SPACE : IR_ZERO_SPACE);
      }
    }
    IR_send_pulse(IR_BIT_MARK);
  }
}

void IR_send_pulse(unsigned int pulse_length) {
  // Toggles the LED at ~38kHz (26us period => 13us between toggles) for the duration of the pulse
  int IR_on = 0;
  unsigned long startMicros = micros();
  while (micros() - startMicros < pulse_length){
    IR_on = 1 - IR_on;
    digitalWrite(IR_EMITTER_PIN, IR_on);
    delayMicroseconds(13);
  }
  digitalWrite(IR_EMITTER_PIN, LOW);
}

void IR_send_pause(unsigned int pause_length) {
  delayMicroseconds(pause_length);
}
