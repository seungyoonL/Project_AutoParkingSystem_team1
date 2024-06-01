volatile uint8_t slave1 = 0x02; // 0b 00000010
volatile uint8_t data = 60; //dummy data
volatile uint8_t* p_data = &data;

void slave_setup();
void slave_communication_start();

ISR(TWI_vect) {
  switch (TWSR & 0xF8) {
    case 0x60: // Master에서 보낸 주소값과 일치, Master write모드 입력 받을 시(Slave read모드)
      slave_communication_start(); // 통신 시작
      break;
    case 0x80: // TWDR에 Master의 data값 입력 완료시
      *p_data = TWDR; // data에 TWDR값 입력
      Serial.print("Received data: ");
      Serial.println(data);
      slave_communication_start(); // 통신 시작
      break;

    case 0xA8: // Master에서 보낸 주소값과 일치, Master read모드 입력 받을 시(Slave write모드)
      TWDR = data; // TWDR에 Slave의 data값 입력
      slave_communication_start(); // 통신 시작
      break;
    case 0xC0: // data 보내기 성공시
      Serial.print("Transferred data: ");
      Serial.println(data);
      slave_communication_start(); // 통신 시작
      break;

    default:
      slave_communication_start(); // 통신 시작
      break;
  }
}

void setup() {
  Serial.begin(9600);
  slave_setup();
}

void loop() {
  slave_communication_start(); // 통신 시작
  Serial.print(".");
  delay(1000);
  data = 60;
}

void slave_setup() {
  cli(); // Global Interrupt Disable
  TWAR = slave1; // Slave 주소 설정
  TWCR |= (1 << TWEA) | (1 << TWEN) | (1 << TWIE); // TWEA, TWI, TWIE Enable
  sei(); // Global Interrupt Enable
}

void slave_communication_start() {
    TWCR |= (1 << TWINT); // 통신시작
}