#include <SoftwareSerial.h>

#define BT_TX 12
#define BT_RX 13

SoftwareSerial bluetooth(BT_TX, BT_RX);

volatile uint8_t slave1 = 0x02; // 0b 00000010
volatile uint8_t data = 99; // dummy data
volatile uint8_t* p_data = &data;

void master_setup();
void master_communication_start();
void master_write_start();
void master_write_stop();
void master_read_start();
void master_read_stop();

ISR(TWI_vect) {
  switch (TWSR & 0xF8) {
    case 0x08: // START 보내기 성공시
      if (data == 0) { // Master read모드
        TWDR = slave1 + 1; // TWDR에 slave_address 및 read모드(1) 입력
      } else { // Master write모드
        TWDR = slave1 + 0; // TWDR에 slave_address 및 write모드(0) 입력
      }
      TWCR &= ~(1 << TWSTA); // START 비트 0으로 만들어줌
      master_communication_start(); // 통신 시작
      break;
    case 0x18: // SLA+W 보내기 성공시
      TWDR = data; // TWDR에 Master의 data값 입력
      master_communication_start(); // 통신 시작
      break;
    case 0x28: // data 보내기 성공시
      master_write_stop(); // STOP 보내기
      break;

    case 0x40: // SLA+R 보내기 성공시
      master_communication_start(); // 통신 시작
      break;
    case 0x58: // TWDR에 Slave의 data값 입력 완료시
      *p_data = TWDR; // data에 TWDR값 입력
      master_read_stop(); // STOP 보내기
      break;
      
    default:
      master_communication_start(); // 통신 시작
      break;
  }
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  master_setup();
}

void loop() {
  while(bluetooth.available()) {
    char received = bluetooth.read();
    if (received == '0') { // 0이면 읽기, 1 또는 2면 쓰기
      data = 0;
    } else if (received == '1') {
      data = 1;
    } else if (received == '2') {
      data = 2;
    }

    if ((received != '\n') && (data != 99) ) { //
      if (data == 0) {
        master_read_start(); // 읽기 시작

        Serial.print("Received data: ");
        Serial.println(data);
      } else {
        master_write_start(); // 쓰기 시작

        Serial.print("Transferred data: ");
        Serial.println(data);
      }
    }

  if(bluetooth.available()) char dummy = bluetooth.read(); // '\n' 입력 안되게 버퍼에서 제거
  }
}

void master_setup() {
  cli(); // Global Interrupt Disable
  TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // Prescaler 0
  TWBR = 12; // 16Mhz / (16 + 2 * 12(TWBR) * 1(Prescaler 0)) = 400kHz => 최대 통신속도
  TWCR |= (1 << TWEN) | (1 << TWIE); // TWI, TWIE Enable
  sei(); // Global Interrupt Enable
}

void master_communication_start() {
  TWCR |= (1 << TWINT); // 통신시작(TWINT에 1 넣기 => toggle)
}

void master_write_start() {
  TWCR |= (1 << TWINT) | (1 << TWSTA); // START 보내기 및 통신 시작
}

void master_write_stop() {
  TWCR |= (1 << TWINT) | (1 << TWSTO); // STOP 보내기 및 통신 시작
  while (TWCR & (1 << TWSTO)); // TWSTO가 다시 0 될때까지 기다림
}

void master_read_start() {
  TWCR |= (1 << TWINT) | (1 << TWSTA); // START 보내기 및 통신 시작
}

void master_read_stop() {
  TWCR |= (1 << TWINT) | (1 << TWSTO); // STOP 보내기 및 통신 시작
  while (TWCR & (1 << TWSTO)); // TWSTO가 다시 0 될때까지 기다림
}