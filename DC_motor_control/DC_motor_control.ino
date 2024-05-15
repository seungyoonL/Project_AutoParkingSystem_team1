// 모터 기본 제어핀 2개
int frontDC1 = 0x01;
int frontDC2 = 0x02;

// 모터 PWM 제어핀 1개 
int frontDCpwm = 0x80;

void DCpwmspeed() {     // PWM 속도 적당히 조절 후 앞으로 돌리기
  PORTD |= frontDC1;
  PORTD &=~ frontDC2;
  for(uint8_t i = 0; i < 50; i++){
    PORTD |= frontDCpwm;
    delayMicroseconds(10000);
    PORTD &=~ frontDCpwm;
    delayMicroseconds(10000);
  };
  delay(5);
}
void DCpwmspeedback() {   //PWM 속도 적당히 조절 후 뒤로 돌리기
  PORTD |= frontDC2;
  PORTD &=~ frontDC1;
  for(uint8_t i = 0; i < 50; i++){
    PORTD |= frontDCpwm;
    delayMicroseconds(10000);
    PORTD &=~ frontDCpwm;
    delayMicroseconds(10000);
  };
  delay(5);
}

void setup() {   //모터 기본 핀 설정 셋업
  DDRD |= frontDC2;
  DDRD |= frontDC1; 
  // analogWrite(5, 255);
}

void loop() {              
  for(uint8_t i = 0; i < 5; i++){   // 모터를 앞으로 일정시간동안 돌아가게 한다음
    DCpwmspeed();
  };
  delay(1000);                      // 1초간 쉬게 하기.
  for(uint8_t i = 0; i < 5; i++){   // 모터 뒤로 일정시간 돌아가게 한다음
    DCpwmspeedback();
  };
  delay(1000);                      // 1초간 쉬게 하기.
}