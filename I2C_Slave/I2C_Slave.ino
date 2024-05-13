uint8_t slave1 = 0x02 // 0b 00000010

void setup() {
  void slave_setup();
}

void loop() {

}

void slave_setup() {
  TWAR = 0x02;
  TWCR |= (1 << TWEA) | (1 << TWEN); // TWEA 및 TWI Enable
}

void slave_read_address() {
  while((TWSR & 0xF8) != 0x60){ // TWSR에 0x60 입력될 때까지
    TWCR |= (1 << TWINT); // 통신시작
    while (!(TWCR & (1 << TWINT))); // TWINT가 다시 1 될때까지 기다림
  }
}

void slave_read_data(uint8_t data) {
  TWCR |= (1 << TWINT); // 통신 시작
  while (!(TWCR & (1 << TWINT))); // TWINT가 다시 1 될때까지 기다림
  while ((TWSR & 0xF8) != 0x80); // TWSR에 0x80 입력될 때까지 기다림
  data = TWDR; // data에 TWDR값 복사
}

void slave_write_address() {
  while((TWSR & 0xF8) != 0xA8){ // TWSR에 0xA8 입력될 때까지
    TWCR |= (1 << TWINT); // 통신시작
    while (!(TWCR & (1 << TWINT))); // TWINT가 다시 1 될때까지 기다림
  }
}

void slave_write_data(uint8_t data) {
  TWDR = data; // TWDR에 data값 입력
  TWCR |= (1 << TWINT); // 통신 시작
  while (!(TWCR & (1 << TWINT))); // TWINT가 다시 1 될때까지 기다림
  while ((TWSR & 0xF8) != 0xC0); // TWSR에 0xC0 입력될 때까지 기다림
}