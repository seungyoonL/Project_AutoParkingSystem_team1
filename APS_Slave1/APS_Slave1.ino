#define frontDC1  0x04  // 2
#define frontDC2  0x08  // 3
#define frontDCpwm  0x10 // 4

#define frontDC1left  0x20  // 5
#define frontDC2left  0x40  // 6
#define frontDCleftpwm  0x80 // 7

#define rearDC1  0x01 // 8
#define rearDC2  0x02 // 9
#define rearDCpwm  0x04  // 10

#define rearDC1left  0x08 // 11
#define rearDC2left  0x10 // 12
#define rearDCleftpwm  0x20  // 13

volatile uint8_t slave1 = 0x02; // 0b 00000010
volatile uint8_t data = 60; //dummy data
volatile uint8_t* p_data = &data;

void slave_setup();
void slave_communication_start();

void dc_setup();
void dc_gogogogo();
void dc_backbackbackback();
void dc_backgogoback();
void dc_gobackgoback();
void dc_allstop();

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
  dc_setup();
}

void loop() {
  slave_communication_start(); // 통신 시작

  if (data == 1) { // 평행주차
    while (data == 1) { // 앞으로 가면서 센서값 받아오기
      dc_gogogogo();
    }


    if (data == 10) { // 왼쪽 공간 찾음
      delay(500);
      for(uint8_t i = 0; i < 8; i++){ // 약간 후진
        dc_backbackbackback();
      }

      delay(1500); // 바퀴 회전 기다림

      while(data == 10) { // 왼쪽으로 이동
        dc_backbackbackback();
      }

      if (data == 11) { // 주차 완료
        dc_allstop();
        
        data = 60; // data 값 초기화
      }
    } else if (data == 15) { // 오른쪽 공간 찾음
      delay(500);
      for(uint8_t i = 0; i < 8; i++){ // 약간 후진
        dc_backbackbackback();
      }

      delay(1500); // 바퀴 회전 기다림

      while(data == 15) { // 오른쪽으로 이동
        dc_backbackbackback();
      }

      if (data == 16) { // 주차 완료
        dc_allstop();
        
        data = 60; // data 값 초기화
      }
    }
  } else if (data == 2) {
    while (data == 2) { // 앞으로 가면서 센서값 받아오기
      dc_gogogogo();
    }

    if (data == 20) { // 왼쪽 공간 찾음
      delay(500);
      for(uint8_t i = 0; i < 8; i++){ // 약간 후진
        dc_backbackbackback();
      }

      delay(1500); // 바퀴 회전 기다림

      for(uint8_t i = 0; i < 8; i++){ // 90도 회전
        dc_backbackbackback();
      }

      delay(1500); // 바퀴 회전 기다림

      while(data == 20) { // 왼쪽으로 이동
        dc_backbackbackback();
      }

      if (data == 21) { // 주차 완료
        dc_allstop();
        
        data = 60; // data 값 초기화
      }
    } else if (data == 25) { // 오른쪽 공간 찾음
      delay(500);
      for(uint8_t i = 0; i < 8; i++){ // 약간 후진
        dc_backbackbackback();
      }

      delay(1500); // 바퀴 회전 기다림

      for(uint8_t i = 0; i < 8; i++){ // 90도 회전
        dc_backbackbackback();
      }

      while(data == 25) { // 오른쪽으로 이동
        dc_backbackbackback();
      }

      if (data == 26) { // 주차 완료
        dc_allstop();
        
        data = 60; // data 값 초기화
      }
    }
  }


  Serial.print(".");
  delay(1000);
}

void slave_setup() {
  cli(); // Global Interrupt Disable
  TWAR = slave1; // Slave 주소 설정
  TWAR |= (1 << TWGCE); // General call Enable
  TWCR |= (1 << TWEA) | (1 << TWEN) | (1 << TWIE); // TWEA, TWI, TWIE Enable
  sei(); // Global Interrupt Enable
}

void slave_communication_start() {
  TWCR |= (1 << TWINT); // 통신시작
}

void dc_setup() {
  DDRD |= frontDC1; 
  DDRD |= frontDC2;
  DDRD |= frontDCpwm;

  DDRD |= frontDC1left;
  DDRD |= frontDC2left;
  DDRD |= frontDCleftpwm;

  DDRB |= rearDC1;
  DDRB |= rearDC2;
  DDRB |= rearDCpwm;

  DDRB |= rearDC1left;
  DDRB |= rearDC2left;
  DDRB |= rearDCleftpwm;
}

void dc_gogogogo() {     // PWM 속도 적당히 조절 후 앞으로 돌리기
  // PORTB |= rearDC1;
  // PORTB &=~ rearDC2;
  // PORTD |= frontDC1left;
  // PORTD &=~ frontDC2left;
  // PORTD |= frontDC1;
  // PORTD &=~ frontDC2;
  // PORTB |= rearDC1left;
  // PORTB &=~ rearDC2left;
  // for(uint8_t i = 0; i < 50; i++){
  //   PORTB |= rearDCpwm;
  //   PORTD |= frontDCleftpwm;
  //   PORTD |= frontDCpwm;
  //   PORTB |= rearDCleftpwm;
  //   delayMicroseconds(19000);
  //   PORTB &=~ rearDCpwm;
  //   PORTD &=~ frontDCleftpwm;
  //   PORTD &=~ frontDCpwm;
  //   PORTB &=~ rearDCleftpwm;
  //   delayMicroseconds(1000);
  // }
  // delay(5);
  PORTB |= (rearDC1 | rearDC1left);
  PORTB &=~ (rearDC2 | rearDC2left);
  PORTD |= (frontDC1left | frontDC1);
  PORTD &=~ (frontDC2left | frontDC2);

  for(uint8_t i = 0; i < 50; i++){
    PORTB |= (rearDCpwm | rearDCleftpwm);
    PORTD |= (frontDCleftpwm | frontDCpwm);
    delayMicroseconds(19000);
    PORTB &=~ (rearDCpwm | rearDCleftpwm);
    PORTD &=~ (frontDCleftpwm | frontDCpwm);
    delayMicroseconds(1000);
  }
  delay(5);
}

void dc_backbackbackback() {     // PWM 속도 적당히 조절 후 앞으로 돌리기
  // PORTB |= rearDC2;
  // PORTB &=~ rearDC1;
  // PORTD |= frontDC2left;
  // PORTD &=~ frontDC1left;
  // PORTD |= frontDC2;
  // PORTD &=~ frontDC1;
  // PORTB |= rearDC2left;
  // PORTB &=~ rearDC1left;
  // for(uint8_t i = 0; i < 50; i++){
  //   PORTB |= rearDCpwm;
  //   PORTD |= frontDCleftpwm;
  //   PORTD |= frontDCpwm;
  //   PORTB |= rearDCleftpwm;
  //   delayMicroseconds(19000);
  //   PORTB &=~ rearDCpwm;
  //   PORTD &=~ frontDCleftpwm;
  //   PORTD &=~ frontDCpwm;
  //   PORTB &=~ rearDCleftpwm;
  //   delayMicroseconds(1000);
  // }
  // delay(5);
  PORTB |= (rearDC2 | rearDC2left);
  PORTB &=~ (rearDC1 | rearDC1left);
  PORTD |= (frontDC2left | frontDC2);
  PORTD &=~ (frontDC1left | frontDC1);
  for(uint8_t i = 0; i < 50; i++){
    PORTB |= (rearDCpwm | rearDCleftpwm);
    PORTD |= (frontDCleftpwm | frontDCpwm);
    delayMicroseconds(19000);
    PORTB &=~ (rearDCpwm | rearDCleftpwm);
    PORTD &=~ (frontDCleftpwm | frontDCpwm);
    delayMicroseconds(1000);
  }
  delay(5);
}

void dc_backgogoback() {     // PWM 속도 적당히 조절 후 앞으로 돌리기
  PORTB |= rearDC1;
  PORTB &=~ rearDC2;
  PORTD |= frontDC1left;
  PORTD &=~ frontDC2left;
  PORTD |= frontDC2;
  PORTD &=~ frontDC1;
  PORTB |= rearDC2left;
  PORTB &=~ rearDC1left;
  for(uint8_t i = 0; i < 50; i++){
    PORTB |= rearDCpwm;
    PORTD |= frontDCleftpwm;
    PORTD |= frontDCpwm;
    PORTB |= rearDCleftpwm;
    delayMicroseconds(19000);
    PORTB &=~ rearDCpwm;
    PORTD &=~ frontDCleftpwm;
    PORTD &=~ frontDCpwm;
    PORTB &=~ rearDCleftpwm;
    delayMicroseconds(1000);
  }
  delay(5);
}

void dc_gobackgoback() {     // PWM 속도 적당히 조절 후 앞으로 돌리기
  PORTB |= rearDC1;
  PORTB &=~ rearDC2;
  PORTD |= frontDC2left;
  PORTD &=~ frontDC1left;
  PORTD |= frontDC1;
  PORTD &=~ frontDC2;
  PORTB |= rearDC2left;
  PORTB &=~ rearDC1left;
  for(uint8_t i = 0; i < 50; i++){
    PORTB |= rearDCpwm;
    PORTD |= frontDCleftpwm;
    PORTD |= frontDCpwm;
    PORTB |= rearDCleftpwm;
    delayMicroseconds(19000);
    PORTB &=~ rearDCpwm;
    PORTD &=~ frontDCleftpwm;
    PORTD &=~ frontDCpwm;
    PORTB &=~ rearDCleftpwm;
    delayMicroseconds(1000);
  }
  delay(5);
}

void dc_allstop() {   //PWM 속도 적당히 조절하여 가만히 있도록 하기
  PORTB &=~ rearDC2;
  PORTB &=~ rearDC1;
  PORTB &=~ rearDC2left;
  PORTB &=~ rearDC1left;  
  PORTD &=~ frontDC1left;
  PORTD &=~ frontDC2left;
  PORTD &=~ frontDC1;
  PORTD &=~ frontDC2;
  delay(1500);
}