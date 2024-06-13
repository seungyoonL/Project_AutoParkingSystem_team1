#define FRONT_LEFT_SERVO 0x10  // 4
#define FRONT_RIGHT_SERVO  0x20  // 5
#define REAR_LEFT_SERVO 0x40  // 6
#define REAR_RIGHT_SERVO  0x80  // 7

volatile uint8_t slave2 = 0x04; // 0b 00000100
volatile uint8_t data = 60; //dummy data
volatile uint8_t* p_data = &data;

void slave_setup();
void slave_communication_start();

void servo_setup();
void servo_front_left_0();
void servo_front_left_90();
void servo_front_left_180();
void servo_front_right_0();
void servo_front_right_90();
void servo_front_right_180();
void servo_rear_left_0();
void servo_rear_left_90();
void servo_rear_left_180();
void servo_rear_right_0();
void servo_rear_right_90();
void servo_rear_right_180();

ISR(TWI_vect) {
  switch (TWSR & 0xF8) {
    case 0x60: // Master에서 보낸 주소값과 일치, Master write모드 입력 받을 시(Slave read모드)
      slave_communication_start(); // 통신 시작
      break;
    case 0x70: // general call
      slave_communication_start(); // 통신 시작
      break;
    case 0x80: // TWDR에 Master의 data값 입력 완료시
      *p_data = TWDR; // data에 TWDR값 입력
      Serial.print("Received data: ");
      Serial.println(data);
      slave_communication_start(); // 통신 시작
      break;
    case 0x90: // general call, data값 입력 완료시
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
}

void loop() {
  slave_communication_start(); // 통신 시작

  if (data == 1) { // 평행주차
    while (data == 1); // 후진 기다림

    if (data == 10) { // 바퀴 회전
      delay(2500);

      servo_front_left_90();
      servo_front_right_90();
      servo_rear_left_90();
      servo_rear_right_90();

      while(data == 10); // 왼쪽 이동 기다림

<<<<<<< HEAD
      if (data == 11 || data == 30) { // 이동 완료시
=======
      if (data == 11) { // 이동 완료시
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
        delay(1000);

        servo_front_left_0();
        servo_front_right_0();
        servo_rear_left_0();
        servo_rear_right_0();

<<<<<<< HEAD
        servo_front_left_0();
        servo_front_right_0();
        servo_rear_left_0();
        servo_rear_right_0();

=======
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
        data = 60;
      }
    } else if (data == 15) { // 바퀴 회전
      delay(2500);

      servo_front_left_90();
      servo_front_right_90();
      servo_rear_left_90();
      servo_rear_right_90();

      while(data == 15); // 오른쪽 이동 기다림

<<<<<<< HEAD
      if (data == 16 || data == 30) { // 이동 완료시
=======
      if (data == 16) { // 이동 완료시
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
        delay(1000);

        servo_front_left_0();
        servo_front_right_0();
        servo_rear_left_0();
        servo_rear_right_0();

<<<<<<< HEAD
        servo_front_left_0();
        servo_front_right_0();
        servo_rear_left_0();
        servo_rear_right_0();

=======
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
        data = 60;
      }
    }
  } else if (data == 2) { // T자 주차
    while (data == 2); // 후진 기다림

    if (data == 20) {
      delay(2500);
      // 60도 회전
      servo_front_left_60();
      servo_front_right_60();
      servo_rear_left_60();
      servo_rear_right_60();

      delay(2500);
      // 후진 하기 위해 정렬
      servo_front_left_0();
      servo_front_right_0();
      servo_rear_left_0();
      servo_rear_right_0();

<<<<<<< HEAD
      servo_front_left_0();
      servo_front_right_0();
      servo_rear_left_0();
      servo_rear_right_0();

      while(data == 20); // 후진 기다림

      if (data == 21 || data == 30) { // 주차 완료시
=======
      while(data == 20); // 후진 기다림

      if (data == 21) { // 주차 완료시
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
        delay(1000);

        servo_front_left_0();
        servo_front_right_0();
        servo_rear_left_0();
        servo_rear_right_0();

<<<<<<< HEAD
        servo_front_left_0();
        servo_front_right_0();
        servo_rear_left_0();
        servo_rear_right_0();

=======
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
        data = 60;
      }
    } else if (data == 25) {
      delay(2500);
      // 60도 회전
      servo_front_left_60();
      servo_front_right_60();
      servo_rear_left_60();
      servo_rear_right_60();

      delay(2500);
      // 후진 하기 위해 정렬
      servo_front_left_0();
      servo_front_right_0();
      servo_rear_left_0();
      servo_rear_right_0();

<<<<<<< HEAD
      servo_front_left_0();
      servo_front_right_0();
      servo_rear_left_0();
      servo_rear_right_0();

      while(data == 25); // 후진 기다림

      if (data == 26 || data == 30) { // 주차 완료시
=======
      while(data == 25); // 후진 기다림

      if (data == 26) { // 주차 완료시
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
        delay(1000);

        servo_front_left_0();
        servo_front_right_0();
        servo_rear_left_0();
        servo_rear_right_0();

<<<<<<< HEAD
        servo_front_left_0();
        servo_front_right_0();
        servo_rear_left_0();
        servo_rear_right_0();

=======
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
        data = 60;
      }
    }
  }


  Serial.print(".");
  delay(1000);
}

void slave_setup() {
  cli(); // Global Interrupt Disable
  TWAR = slave2; // Slave 주소 설정
  TWAR |= (1 << TWGCE); // General call Enable
  TWCR |= (1 << TWEA) | (1 << TWEN) | (1 << TWIE); // TWEA, TWI, TWIE Enable
  sei(); // Global Interrupt Enable
}

void slave_communication_start() {
  TWCR |= (1 << TWINT); // 통신시작
}

void servo_setup() {
  DDRD |= FRONT_LEFT_SERVO; // 서보모터 PWM(주황색) 핀을 출력으로 연결시킴
  DDRD |= FRONT_RIGHT_SERVO;
  DDRD |= REAR_LEFT_SERVO;
  DDRD |= REAR_RIGHT_SERVO;

  servo_front_left_0();
  servo_front_right_0();
  servo_rear_left_0();
  servo_rear_right_0();
<<<<<<< HEAD

  servo_front_left_0();
  servo_front_right_0();
  servo_rear_left_0();
  servo_rear_right_0();
=======
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
}

void servo_front_left_0() { // 서보모터 기준도                     
   for(uint8_t i = 0; i < 50; i++) {
    PORTD |= FRONT_LEFT_SERVO;
<<<<<<< HEAD
    delayMicroseconds(1500);
    PORTD &=~ FRONT_LEFT_SERVO;
    delayMicroseconds(18500);
=======
    delayMicroseconds(1505);
    PORTD &=~ FRONT_LEFT_SERVO;
    delayMicroseconds(18495);
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
  }
}

void servo_front_left_90() { // 서보모터 오른90도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= FRONT_LEFT_SERVO;
    delayMicroseconds(2500);
    PORTD &=~ FRONT_LEFT_SERVO;
    delayMicroseconds(17500);
  }
}

void servo_front_left_60() { // 서보모터 60도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= FRONT_LEFT_SERVO;
    delayMicroseconds(2100);
    PORTD &=~ FRONT_LEFT_SERVO;
    delayMicroseconds(17900);
  }
}

void servo_front_right_0() { // 서보모터 기준도                     
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= FRONT_RIGHT_SERVO;
    delayMicroseconds(1500);
    PORTD &=~ FRONT_RIGHT_SERVO;
    delayMicroseconds(18500);
  }
}

void servo_front_right_90() { // 서보모터 왼90도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= FRONT_RIGHT_SERVO;
<<<<<<< HEAD
    delayMicroseconds(500);
    PORTD &=~ FRONT_RIGHT_SERVO;
    delayMicroseconds(19500);
=======
    delayMicroseconds(480);
    PORTD &=~ FRONT_RIGHT_SERVO;
    delayMicroseconds(19520);
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
  }
}

void servo_front_right_60() { // 서보모터 60도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= FRONT_RIGHT_SERVO;
    delayMicroseconds(900);
    PORTD &=~ FRONT_RIGHT_SERVO;
    delayMicroseconds(19100);
  }
}

void servo_rear_left_0() { // 서보모터 기준도                     
   for(uint8_t i = 0; i < 50; i++) {
    PORTD |= REAR_LEFT_SERVO;
    delayMicroseconds(1500);
    PORTD &=~ REAR_LEFT_SERVO;
    delayMicroseconds(18500);
  }
}

void servo_rear_left_90() { // 서보모터 오른90도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= REAR_LEFT_SERVO;
    delayMicroseconds(500);
    PORTD &=~ REAR_LEFT_SERVO;
    delayMicroseconds(19500);
  }
}

void servo_rear_left_60() { // 서보모터 60도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= REAR_LEFT_SERVO;
    delayMicroseconds(900);
    PORTD &=~ REAR_LEFT_SERVO;
    delayMicroseconds(19100);
  }
}

void servo_rear_right_0() { // 서보모터 기준도                     
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= REAR_RIGHT_SERVO;
<<<<<<< HEAD
    delayMicroseconds(1500);
=======
    delayMicroseconds(1490);
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
    PORTD &=~ REAR_RIGHT_SERVO;
    delayMicroseconds(18500);
  }
}

void servo_rear_right_90() { // 서보모터 오른90도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= REAR_RIGHT_SERVO;
<<<<<<< HEAD
    delayMicroseconds(2500);
    PORTD &=~ REAR_RIGHT_SERVO;
    delayMicroseconds(17500);
=======
    delayMicroseconds(2450);
    PORTD &=~ REAR_RIGHT_SERVO;
    delayMicroseconds(17550);
>>>>>>> 857a61e066f12ad9b0e94ddcb10aa9918409c131
  }
}

void servo_rear_right_60() { // 서보모터 60도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= REAR_RIGHT_SERVO;
    delayMicroseconds(2100);
    PORTD &=~ REAR_RIGHT_SERVO;
    delayMicroseconds(17900);
  }
}