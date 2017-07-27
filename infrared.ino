void IR_send_signal() {
  
  // send control pulses
  IR_send_pulse(START_PULSE); // long start pulse  
  delayMicroseconds(START_PAUSE); // long start delay
  IR_send_pulse(PULSE_LEN); // regular pulse before data
  
  // send data
  for (int i=0; i < sizeof(IR_signal); i++){
    
    //send bit (pause)
    byte bit = IR_signal[i];
    if (bit == 0){
      delayMicroseconds(PAUSE_LOW);
    }
    if (bit == 1){
      delayMicroseconds(PAUSE_HIGH);
    }
    
    //send pulse after bit
    IR_send_pulse(PULSE_LEN);
  }
}

void IR_send_pulse(int pulse_length){
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
    delayMicroseconds(26); // 1000000(us)/38000(hz) = 26(us)
  }
  
  // turn off IR after pulse is complete
  digitalWrite(IR_LED_PIN, LOW);
}



void IR_set_bits(int index, int bit_count, int value) {
  for (int i=0; i<bit_count; i++) {
    IR_signal[i+index] =  bitRead(value, i);
    IR_signal[i+index+8] = !IR_signal[i+index]; // Parity
  }
}

