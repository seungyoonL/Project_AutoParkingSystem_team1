#include <SoftwareSerial.h>

#define BT_TX 10
#define BT_RX 11

SoftwareSerial btSerial(BT_TX, BT_RX);

uint8_t slave1 = 0x02; // 0b 00000010

void master_setup();
void master_write_start();
void master_write_address(uint8_t slave_address);
void master_write_data(uint8_t data);
void master_write_stop();
void master_read_start();
void master_read_address(uint8_t slave_address);
void master_read_data(uint8_t* p_data);
void master_read_stop();

void setup() {
  Serial.begin(9600);
  btSerial.begin(38400);
  delay(2000);
  master_setup();
}

void loop() {
  if(btSerial.available()){
    char received = btSerial.read();
    uint8_t data = 99;
    if (received == '0') {
      data = 0;
    } else if (received == '1') {
      data = 1;
    } else if (received == '2') {
      data = 2;
    }

    if (received != '\n' && data != 99){
      if (data == 0){
        master_read_start();
        master_read_address(slave1);
        master_read_data(&data);
        master_read_stop();

        Serial.print("Data: ");
        Serial.println(data);
      } else{
        master_write_start();
        master_write_address(slave1);
        master_write_data(data);
        master_write_stop();

        Serial.print("Data: ");
        Serial.println(data);
      }
    }
  }
}

void master_setup() {
  TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // Prescaler 0
  TWBR = 12; // 16Mhz / (16 + 2 * 12(TWBR) * 1(Prescaler 0)) = 400kHz => 최대 통신속도
  TWCR |= (1 << TWEN); // TWI Enable
}

void master_write_start() {
  TWCR |= (1 << TWINT) | (1 << TWSTA); // START 보내기 및 통신 시작(TWINT에 1 넣기 => toggle)
  while (!(TWCR & (1 << TWINT))); // TWINT가 다시 1 될때까지 기다림
  while ((TWSR & 0xF8) != 0x08); // TWSR에 0x08 입력될 때까지 기다림
}

void master_write_address(uint8_t slave_address) {
  TWDR = slave_address + 0; // TWDR에 slave_address 및 write모드(0) 입력
  TWCR &= ~(1 << TWSTA); // START 비트 0으로 만들어줌
  TWCR |= (1 << TWINT); // 통신 시작
  while (!(TWCR & (1 << TWINT))); // TWINT가 다시 1 될때까지 기다림
  while ((TWSR & 0xF8) != 0x18); // TWSR에 0x18 입력될 때까지 기다림
}

void master_write_data(uint8_t data) {
  TWDR = data; // TWDR에 data값 입력
  TWCR |= (1 << TWINT); // 통신 시작
  while (!(TWCR & (1 << TWINT))); // TWINT가 다시 1 될때까지 기다림
  while ((TWSR & 0xF8) != 0x28); // TWSR에 0x18 입력될 때까지 기다림
}

void master_write_stop() {
  TWCR |= (1 << TWINT) | (1 << TWSTO); // STOP 보내기 및 통신 시작
  while (TWCR & (1 << TWSTO)); // TWSTO가 다시 0 될때까지 기다림
}

void master_read_start() {
  TWCR |= (1 << TWINT) | (1 << TWSTA); // START 보내기 및 통신 시작
  while (!(TWCR & (1 << TWINT))); // TWINT가 다시 1 될때까지 기다림
  while ((TWSR & 0xF8) != 0x08); // TWSR에 0x08 입력될 때까지 기다림
}

void master_read_address(uint8_t slave_address) {
  TWDR = slave_address + 1; // TWDR에 slave_address 및 read모드(1) 입력
  TWCR &= ~(1 << TWSTA); // START 비트 0으로 만들어줌
  TWCR |= (1 << TWINT); // 통신 시작
  while (!(TWCR & (1 << TWINT))); // TWINT가 다시 1 될때까지 기다림
  while ((TWSR & 0xF8) != 0x40); // TWSR에 0x40 입력될 때까지 기다림
}

void master_read_data(uint8_t* p_data) {
  while ((TWSR & 0xF8) != 0x58){
    TWCR |= (1 << TWINT); // 통신 시작
    while (!(TWCR & (1 << TWINT))); // TWINT가 다시 1 될때까지 기다림
  } // TWSR에 0x58 입력될 때까지 기다림
  *p_data = TWDR; // data에 TWDR값 입력
}

void master_read_stop() {
  TWCR |= (1 << TWINT) | (1 << TWSTO); // STOP 보내기 및 통신 시작
  while (TWCR & (1 << TWSTO)); // TWSTO가 다시 0 될때까지 기다림
}