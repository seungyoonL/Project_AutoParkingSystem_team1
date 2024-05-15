// 모터 기본 제어핀 2개
int frontDC1 = 0x04;
int frontDC2 = 0x08;

int A = 0;
int B = 0;

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
void DCpwmspeedstop1500() {   //PWM 속도 적당히 조절하여 가만히 있도록 하기
  PORTD &=~ frontDC2;
  PORTD &=~ frontDC1;
  delay(1500);
}

void setup() {   //모터 기본 핀 설정 셋업
  DDRD |= frontDC2;
  DDRD |= frontDC1; 
  // analogWrite(5, 255);
}

void loop() {           
  DCpwmspeed();     // DC모터가 평상시엔 그냥 앞으로 돌고 있음
  if (A == 0){          // 들어간 공간을 찾으면
    for(uint8_t i = 0; i < 2; i++){   // 일단 후진 조금 한다음
      DCpwmspeedback();
    };
    DCpwmspeedstop1500();      // 서보모터 완전히 돌아갈 1.5초동안 멈추기
    for(uint8_t i = 0; i < 5; i++){
      DCpwmspeedback();             // 차 점점 주차 시키다가 
      if(B == 0){                        // 간견 3cm 미만으로 떨어지는 조건 만족하자 마자
        while(1) DCpwmspeedstop1500();   // 차 멈추는 함수 무한루프 돌림으로써 주차 완료시킴.
      };
    }
  };
  // delay(1000);                      // 1초간 쉬게 하기.
  // for(uint8_t i = 0; i < 5; i++){   // 모터 뒤로 일정시간 돌아가게 한다음
  //   DCpwmspeedback();
  // };
  // delay(1000);                      // 1초간 쉬게 하기.
}