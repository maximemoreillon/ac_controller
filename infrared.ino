void IR_send_signal() {
  // Send the infrared signal
  
  // send control pulses and pauses
  IR_send_pulse(START_PULSE); // long start pulse  
  IR_send_pause(START_PAUSE); // long start delay
  IR_send_pulse(PULSE_LEN); // regular pulse before data
  
  // send data
  for (int i=0; i < sizeof(IR_signal); i++){
    
    //send bit (pause)
    byte bit = IR_signal[i];
    if (bit == 0) {
      IR_send_pause(PAUSE_LOW);
    }
    if (bit == 1) {
      IR_send_pause(PAUSE_HIGH);
    }
    
    //send a pulse after the bit (every pause is separated by a pulse of PULSE_LEN)
    IR_send_pulse(PULSE_LEN);
  }
}

void IR_send_pulse(int pulse_length) {
  // Send one pulse of a given length
  
  int IR_on = 0;
  long startMicros = micros();
  while (micros() < (startMicros + pulse_length)){
    // toggle pin and wait 26 us to make it a pulse
    if (IR_on == 0) {
      IR_on = 1;
    }
    else {
      IR_on = 0;
    }
    digitalWrite(IR_LED_PIN, IR_on);
    
    // IR LED frequency must be around 37-38kHz
    // Approx pulse period = 26us
    // A period consists of the LED being toggled twice => 13us between toggles
    delayMicroseconds(13);
  }
  
  // turn off IR after pulse is complete
  digitalWrite(IR_LED_PIN, LOW);
}

void IR_send_pause(int pause_length) {
  // A pause is just not doing anything for a given amount of time
  delayMicroseconds(pause_length);
}



void IR_set_bits(int index, int bit_count, int value) {
  for (int i=0; i<bit_count; i++) {
    IR_signal[i+index] =  bitRead(value, i);
    IR_signal[i+index+8] = !IR_signal[i+index]; // Parity
  }
}
