#define FRONT_SERVO 0x01
#define REAR_SERVO 0x02

volatile uint16_t value = 0;

void setup() {
  // 마프 1조 Auto Parking System 팀플 시작!
  
  DDRB |= FRONT_SERVO; // 서보모터 PWM(주황색) 핀을 출력으로 연결시킴
  DDRB |= REAR_SERVO;
  
  Serial.begin(9600);
  light_sensor_servo(); // 조도센서 기본설정 셋업

  front_motor0(); // 처음엔 조향각 0도
  rear_motor0();
}

void loop() {
  //front_motor0();
  //front_motor90();
  //front_motor180();
  light_sensor_servo_loop(); //ADC 돌려서 값 보기
  delay(500); // 텀은 0.5초 정도
  if (value > 1010 & value <1015){ 
    delay(2000);               // 2초 있다가 앞바퀴 2개 완전히 돌리기
    front_motor180();
    rear_motor180();
    delay(5000);
    while(value > 1010){
      uint8_t value2 = 1;    // 다른 값(value2)를 받아서 그값이 어떤 조건 만족하면 무한루프 탈출하고 바퀴각도 0도로 복원
      if (value2 == 1){      // 다른 값이 어떤 조건 만족못하면 무한루프 탈출못하고 계속 180도 유지.
        value = 0;
      }
    }
    front_motor0();         // 탈출 후 0도로 만들고 내가 관찰하는 value 값이 저 조건 만족안하면 계속 0도 유지
    rear_motor0();
  }
  else if (value > 1014 & value < 1050){    // 2초 후 앞바퀴 완전히 돌리고 DC모터가 동작할 시간 2.5초 주고 다시 0도 만들기
    delay(2000);
    front_motor180();
    rear_motor180();
    delay(2500);
    front_motor0(); // 이때 0도 만들고 내가 관찰하는 값이 저 조건 아니면 계속 0도 유지
    rear_motor0();
  }
  else {
    front_motor0(); // 아무 조건도 만족못하면 그냥 0도 유지
    rear_motor0();
  }
}

void front_motor0() { // 서보모터 0도                     
   for(uint8_t i = 0; i < 50; i++) {
    PORTB |= FRONT_SERVO;
    delayMicroseconds(500);
    PORTB &=~ FRONT_SERVO;
    delayMicroseconds(19500);
  };
}

void front_motor90() { // 서보모터 90도
    for(uint8_t i = 0; i < 50; i++) {
    PORTB |= FRONT_SERVO;
    delayMicroseconds(1500);
    PORTB &=~ FRONT_SERVO;
    delayMicroseconds(18500);
  };
}

void front_motor180() { // 서보모터 180도
    for(uint8_t i = 0; i < 50; i++) {
    PORTB |= FRONT_SERVO;
    delayMicroseconds(2500);
    PORTB &=~ FRONT_SERVO;
    delayMicroseconds(17500);
  };
}

void rear_motor0() { // 서보모터 0도                     
   for(uint8_t i = 0; i < 50; i++) {
    PORTB |= REAR_SERVO;
    delayMicroseconds(500);
    PORTB &=~ REAR_SERVO;
    delayMicroseconds(19500);
  };
}

void rear_motor90() { // 서보모터 90도
    for(uint8_t i = 0; i < 50; i++) {
    PORTB |= REAR_SERVO;
    delayMicroseconds(1500);
    PORTB &=~ REAR_SERVO;
    delayMicroseconds(18500);
  };
}

void rear_motor180() { // 서보모터 180도
    for(uint8_t i = 0; i < 50; i++) {
    PORTB |= REAR_SERVO;
    delayMicroseconds(2500);
    PORTB &=~ REAR_SERVO;
    delayMicroseconds(17500);
  };
}

void light_sensor_servo() {
  ADMUX |= (0 << REFS1) | (1 << REFS0); // 기준전압으로 
  ADMUX |= (0 << ADLAR); // 오른 정렬
  ADMUX |= (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0); // ADC[0]자리를 사용
  ADCSRA |= (1 << ADEN); //ADC 가능
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //prescaler 128 
}

void light_sensor_servo_loop() {
  while(ADCSRA & (1 << ADSC)); //ADC끝날때까지 기다리기
  ADCSRA |= (1 << ADSC); // ADC시작
  value = ADC;
  Serial.println(value);
}
