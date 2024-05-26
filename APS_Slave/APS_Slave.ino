#define FRONT_SERVO 0x01  // 8
#define REAR_SERVO  0x02  // 9

// 모터 기본 제어핀 2개
#define frontDC1    0x04  // 2
#define frontDC2    0x08  // 3
// 모터 PWM 제어핀 1개
#define frontDCpwm  0x80  // 7

volatile uint8_t slave1 = 0x02; // 0b 00000010
volatile uint8_t data = 60; //dummy data
volatile uint8_t* p_data = &data;

void slave_setup();
void slave_communication_start();

void servo_setup();
void front_motor0();
void front_motor90();
void front_motor180();
void rear_motor0();
void rear_motor90();
void rear_motor180();

void DC_setup();
void DCpwmspeed();
void DCpwmspeedback();
void DCpwmspeedstop1500();

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
  
  servo_setup();
  DC_setup();
}

void loop() {
  slave_communication_start(); // 통신 시작

  while (data == 1) { // 평행주차
    DCpwmspeed();
  }
  if (data == 11) { // decision == 1
    for(uint8_t i = 0; i < 8; i++){
      DCpwmspeedback();
    }
    front_motor180();
    rear_motor180();

    while(data == 11) {
      for(uint8_t i = 0; i < 100; i++){
      DCpwmspeedback();
      }
    }

    if (data == 12) {
      DCpwmspeedstop1500();
      front_motor0();
      rear_motor0();
    }
  }


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

void servo_setup() {
  DDRB |= FRONT_SERVO; // 서보모터 PWM(주황색) 핀을 출력으로 연결시킴
  DDRB |= REAR_SERVO;
}

void front_motor0() { // 서보모터 0도                     
   for(uint8_t i = 0; i < 50; i++) {
    PORTB |= FRONT_SERVO;
    delayMicroseconds(500);
    PORTB &=~ FRONT_SERVO;
    delayMicroseconds(19500);
  }
}

void front_motor90() { // 서보모터 90도
  for(uint8_t i = 0; i < 50; i++) {
    PORTB |= FRONT_SERVO;
    delayMicroseconds(1500);
    PORTB &=~ FRONT_SERVO;
    delayMicroseconds(18500);
  }
}

void front_motor180() { // 서보모터 180도
  for(uint8_t i = 0; i < 50; i++) {
    PORTB |= FRONT_SERVO;
    delayMicroseconds(2500);
    PORTB &=~ FRONT_SERVO;
    delayMicroseconds(17500);
  }
}

void rear_motor0() { // 서보모터 0도                     
  for(uint8_t i = 0; i < 50; i++) {
    PORTB |= REAR_SERVO;
    delayMicroseconds(500);
    PORTB &=~ REAR_SERVO;
    delayMicroseconds(19500);
  }
}

void rear_motor90() { // 서보모터 90도
  for(uint8_t i = 0; i < 50; i++) {
    PORTB |= REAR_SERVO;
    delayMicroseconds(1500);
    PORTB &=~ REAR_SERVO;
    delayMicroseconds(18500);
  }
}

void rear_motor180() { // 서보모터 180도
  for(uint8_t i = 0; i < 50; i++) {
    PORTB |= REAR_SERVO;
    delayMicroseconds(2500);
    PORTB &=~ REAR_SERVO;
    delayMicroseconds(17500);
  }
}

void DC_setup() {
  DDRD |= frontDC2;    // DC모터 앞쪽,오른쪽 제어 핀 출력으로 연결시킴
  DDRD |= frontDC1;
  DDRD |= frontDCpwm;

  front_motor0(); // 처음엔 조향각 0도
  rear_motor0();
}

void DCpwmspeed() {     // PWM 속도 적당히 조절 후 앞으로 돌리기
  PORTD |= frontDC1;
  PORTD &=~ frontDC2;
  for(uint8_t i = 0; i < 250; i++){
    PORTD |= frontDCpwm;
    delayMicroseconds(1900);
    PORTD &=~ frontDCpwm;
    delayMicroseconds(100);
  }
  delay(5);
}

void DCpwmspeedback() {   //PWM 속도 적당히 조절 후 뒤로 돌리기
  PORTD |= frontDC2;
  PORTD &=~ frontDC1;
  for(uint8_t i = 0; i < 50; i++){
    PORTD |= frontDCpwm;
    delayMicroseconds(19000);
    PORTD &=~ frontDCpwm;
    delayMicroseconds(1000);
  }
  delay(5);
}

void DCpwmspeedstop1500() {   //PWM 속도 적당히 조절하여 가만히 있도록 하기
  PORTD &=~ frontDC2;
  PORTD &=~ frontDC1;
  delay(1500);
}