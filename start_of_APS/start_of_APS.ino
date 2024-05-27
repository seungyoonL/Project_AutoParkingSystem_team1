#define FRONT_SERVO 0x01
#define REAR_SERVO 0x02

// 초음파 센서 Trig, Echo 핀 번호 설정
#define pinTrig_front     0x08
#define pinEcho_front     0x04
#define pinTrig_back      0x40
#define pinEcho_back      0x20
#define pinTrig_left      0x04
#define pinEcho_left      0x08
#define pinTrig_right     0x10
#define pinEcho_right     0x20

// 차량 전장, 전폭 설정
#define Car_Length        30
#define Car_Width         8

// 측정 주기, 차량 속도
#define Time_Interval     200
#define Velocity_MeterperSecond   1

#define Specific_Count    5

// // 모터 기본 제어핀 2개
// #define frontDC1 0x04
// #define frontDC2 0x08
// // 모터 PWM 제어핀 1개
// #define frontDCpwm 0x80
// #define PPPWM 0x10

uint8_t frontDC1 = 0x04;
uint8_t frontDC2 = 0x08;
uint8_t frontDC1left = 0x10;
uint8_t frontDC2left = 0x20;

uint8_t rearDC1 = 0x02;
uint8_t rearDC2 = 0x04;
uint8_t rearDC1left = 0x10;
uint8_t rearDC2left = 0x20;

uint8_t frontDCpwm = 0x80;
uint8_t frontDCleftpwm = 0x01;
uint8_t rearDCpwm = 0x08;
uint8_t rearDCleftpwm = 0x40;

// volatile uint16_t value = 0;
uint8_t AAA = 0;

// void DCpwmspeedfront() {     // PWM 속도 적당히 조절 후 앞으로 돌리기
//   PORTD |= frontDC1;
//   PORTD &=~ frontDC2;
//   for(uint8_t i = 0; i < 50; i++){
//     PORTD |= (frontDCpwm | PPPWM);
//     delayMicroseconds(19000);
//     PORTD &=~ (frontDCpwm | PPPWM);
//     delayMicroseconds(1000);
//   }
//   delay(5);
// }
// void DCpwmspeedback() {   //PWM 속도 적당히 조절 후 뒤로 돌리기
//   PORTD |= frontDC2;
//   PORTD &=~ frontDC1;
//   for(uint8_t i = 0; i < 50; i++){
//     PORTD |= (frontDCpwm | PPPWM);
//     delayMicroseconds(19000);
//     PORTD &=~ (frontDCpwm | PPPWM);
//     delayMicroseconds(1000);
//   }
//   delay(5);
// }
// void DCpwmspeedstop1500() {   //PWM 속도 적당히 조절하여 가만히 있도록 하기
//   PORTD &=~ frontDC2;
//   PORTD &=~ frontDC1;
//   delay(1500);
// }

void gogogogo() {     // PWM 속도 적당히 조절 후 앞으로 돌리기
  PORTB |= rearDC1;
  PORTB &=~ rearDC2;
  PORTB |= frontDC1left;
  PORTB &=~ frontDC2left;
  PORTD |= frontDC1;
  PORTD &=~ frontDC2;
  PORTD |=rearDC1left;
  PORTD &=~ rearDC2left;
  for(uint8_t i = 0; i < 50; i++){
    PORTB |= rearDCpwm;
    PORTB |= frontDCleftpwm;
    PORTD |= frontDCpwm;
    PORTD |= rearDCleftpwm;
    delayMicroseconds(19000);
    PORTB &=~ rearDCpwm;
    PORTB &=~ frontDCleftpwm;
    PORTD &=~ frontDCpwm;
    PORTD &=~ rearDCleftpwm;
    delayMicroseconds(1000);
  }
  delay(5);
}

void backbackbackback() {     // PWM 속도 적당히 조절 후 앞으로 돌리기
  PORTB |= rearDC2;
  PORTB &=~ rearDC1;
  PORTB |= frontDC2left;
  PORTB &=~ frontDC1left;
  PORTD |= frontDC2;
  PORTD &=~ frontDC1;
  PORTD |=rearDC2left;
  PORTD &=~ rearDC1left;
  for(uint8_t i = 0; i < 50; i++){
    PORTB |= rearDCpwm;
    PORTB |= frontDCleftpwm;
    PORTD |= frontDCpwm;
    PORTD |= rearDCleftpwm;
    delayMicroseconds(19000);
    PORTB &=~ rearDCpwm;
    PORTB &=~ frontDCleftpwm;
    PORTD &=~ frontDCpwm;
    PORTD &=~ rearDCleftpwm;
    delayMicroseconds(1000);
  }
  delay(5);
}

void backgogoback() {     // PWM 속도 적당히 조절 후 앞으로 돌리기
  PORTB |= rearDC1;
  PORTB &=~ rearDC2;
  PORTB |= frontDC1left;
  PORTB &=~ frontDC2left;
  PORTD |= frontDC2;
  PORTD &=~ frontDC1;
  PORTD |=rearDC2left;
  PORTD &=~ rearDC1left;
  for(uint8_t i = 0; i < 50; i++){
    PORTB |= rearDCpwm;
    PORTB |= frontDCleftpwm;
    PORTD |= frontDCpwm;
    PORTD |= rearDCleftpwm;
    delayMicroseconds(19000);
    PORTB &=~ rearDCpwm;
    PORTB &=~ frontDCleftpwm;
    PORTD &=~ frontDCpwm;
    PORTD &=~ rearDCleftpwm;
    delayMicroseconds(1000);
  }
  delay(5);
}

void gobackgoback() {     // PWM 속도 적당히 조절 후 앞으로 돌리기
  PORTB |= rearDC1;
  PORTB &=~ rearDC2;
  PORTB |= frontDC2left;
  PORTB &=~ frontDC1left;
  PORTD |= frontDC1;
  PORTD &=~ frontDC2;
  PORTD |= rearDC2left;
  PORTD &=~ rearDC1left;
  for(uint8_t i = 0; i < 50; i++){
    PORTB |= rearDCpwm;
    PORTB |= frontDCleftpwm;
    PORTD |= frontDCpwm;
    PORTD |= rearDCleftpwm;
    delayMicroseconds(19000);
    PORTB &=~ rearDCpwm;
    PORTB &=~ frontDCleftpwm;
    PORTD &=~ frontDCpwm;
    PORTD &=~ rearDCleftpwm;
    delayMicroseconds(1000);
  }
  delay(5);
}

void allstop() {   //PWM 속도 적당히 조절하여 가만히 있도록 하기
  PORTB &=~ rearDC2;
  PORTB &=~ rearDC1;
  PORTB &=~ rearDC2left;
  PORTB &=~ rearDC1left;  
  PORTB &=~ frontDC1left;
  PORTB &=~ frontDC2left;
  PORTB &=~ frontDC1;
  PORTB &=~ frontDC2;
  delay(1500);
}

void DCmotor_ready() {   //모터 기본 핀 설정 셋업
  DDRD |= frontDC2;
  DDRD |= frontDC1; 
  DDRB |= frontDC1left;
  DDRB |= frontDC2left;
  DDRD |= frontDCpwm;
  DDRB |= frontDCleftpwm;
  DDRB |= rearDC1;
  DDRB |= rearDC2;
  DDRD |= rearDC1left;
  DDRD |= rearDC2left;
  DDRD |= rearDCpwm;
  DDRB |= rearDCleftpwm;

}

//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  // 마프 1조 Auto Parking System 팀플 시작!
  
  DDRB |= FRONT_SERVO; // 서보모터 PWM(주황색) 핀을 출력으로 연결시킴
  DDRB |= REAR_SERVO;

  DCmotor_ready();
  
  Serial.begin(9600);
  // light_sensor_servo(); // 조도센서 기본설정 셋업

  front_motor0(); // 처음엔 조향각 0도
  rear_motor0();
  
  allstop(); // DC모터 4개 모두 정지

  ultraSonic_setup();
}

void loop() {
  static uint8_t count_left = 0;
  static uint8_t count_right = 0;

  uint16_t lengthmm_left;
  uint16_t lengthmm_right;

  double distance_front = distanceCm_front();
  double distance_back = distanceCm_back();
  double distance_left = distanceCm_left();
  double distance_right = distanceCm_right();

  // uint16_t* distance = detectCm_4direction();

  // Serial.println(distance[3]);

  // distance = detectCm_4direction();

  count_left = countLength(distance_left, count_left);
  count_right = countLength(distance_right, count_right);

  // 탐지 공간의 폭 계산
  lengthmm_left = count_left * Time_Interval * Velocity_MeterperSecond;
  lengthmm_right = count_right * Time_Interval * Velocity_MeterperSecond;

  showDistance(distance_front, distance_back, distance_left, distance_right, count_left, count_right, lengthmm_left, lengthmm_right);

  delay(Time_Interval);

  bool decision = decideParking(lengthmm_left);

  ///////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////

  //front_motor0();
  //front_motor90();
  //front_motor180();
  // light_sensor_servo_loop(); //ADC 돌려서 값 보기
  // delay(500); // 텀은 0.5초 정도

  gogogogo();
  
  if (decision == 1){ 
    delay(500);
    for(uint8_t i = 0; i < 8; i++){
      backbackbackcback();
    };
    // delay(1000);               // 2초 있다가 앞바퀴 2개 완전히 돌리기
    front_motor180();
    rear_motor180();

    //delay(5000);

    // uint16_t* update = detectCm_4direction();
    double update_distance = distanceCm_left();
    Serial.print("update_distance: ");
    Serial.println(update_distance);
    while(update_distance > 5){
      for(uint8_t i = 0; i < 10; i++){
        backgogoback();
              // uint16_t* new_update = detectCm_4direction();
      // update = detectCm_4direction();
        double new_update_distance = distanceCm_left();
        Serial.print("new_update_distance: ");
        Serial.println(new_update_distance);
      // light_sensor_servo_loop();   // 계속해서 그 조건이 유지되면 바퀴 완전히 돌린거 계속 쭉 유지한다
        if (new_update_distance < 5){           // 만약에 그 조건이 유지되지 못하면 value를 초기화 함으로써 와일문을 탈출함
          AAA = 1;
          break;      
        }
      }
      if(AAA == 1){
        allstop();
        front_motor0();
        rear_motor0();
        while(1) allstop();
      }
    }
    // front_motor0();         // 탈출 후 0도로 만들고 내가 관찰하는 value 값이 저 조건 만족안하면 계속 0도 유지
    // rear_motor0();
  }

  // else if (decision == 1){    // 2초 후 앞바퀴 완전히 돌리고 DC모터가 동작할 시간 2.5초 주고 다시 0도 만들기
  //   delay(2000);
  //   front_motor180();
  //   rear_motor180();
  //   delay(2500);
  //   front_motor0(); // 이때 0도 만들고 내가 관찰하는 값이 저 조건 아니면 계속 0도 유지
  //   rear_motor0();
  // }
  // else {
  //   front_motor0(); // 아무 조건도 만족못하면 그냥 0도 유지
  //   rear_motor0();
  // }
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

// void light_sensor_servo() {
//   ADMUX |= (0 << REFS1) | (1 << REFS0); // 기준전압으로 
//   ADMUX |= (0 << ADLAR); // 오른 정렬
//   ADMUX |= (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0); // ADC[0]자리를 사용
//   ADCSRA |= (1 << ADEN); //ADC 가능
//   ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //prescaler 128 
// }

// void light_sensor_servo_loop() {
//   while(ADCSRA & (1 << ADSC)); //ADC끝날때까지 기다리기
//   ADCSRA |= (1 << ADSC); // ADC시작
//   value = ADC;
//   Serial.println(value);
// }

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// 초음파 센서 핀 초기 설정
void ultraSonic_setup() {
  DDRD |= pinTrig_front;
  DDRD |= pinTrig_back;
  DDRB |= pinTrig_left;
  DDRB |= pinTrig_right;

  DDRD &= ~pinEcho_front;
  DDRD &= ~pinEcho_back;
  DDRB &= ~pinEcho_left;
  DDRB &= ~pinEcho_right;
}

// 전방 거리 측정
double distanceCm_front() {
  PORTD &= ~pinTrig_front;
  delayMicroseconds(5);
  
  PORTD |= pinTrig_front;
  delayMicroseconds(10);

  PORTD &= ~pinTrig_front;

  double duration = pulseIn(2, HIGH, 100000);

  double cm = (duration/2) * 0.0343;
  return cm;
}

// 후방 거리 측정
double distanceCm_back() {
  PORTD &= ~pinTrig_back;
  delayMicroseconds(5);
  
  PORTD |= pinTrig_back;
  delayMicroseconds(10);

  PORTD &= ~pinTrig_back;

  double duration = pulseIn(5, HIGH, 100000);

  double cm = (duration/2) * 0.0343;
  return cm;
}

// 좌측방 거리 측정
double distanceCm_left() {
  PORTB &= ~pinTrig_left;
  delayMicroseconds(5);
  
  PORTB |= pinTrig_left;
  delayMicroseconds(10);

  PORTB &= ~pinTrig_left;

  double duration = pulseIn(11, HIGH, 100000);

  double cm = (duration/2) * 0.0343;
  return cm;
}

// 우측방 거리 측정
double distanceCm_right() {
  PORTB &= ~pinTrig_right;
  delayMicroseconds(5);
  
  PORTB |= pinTrig_right;
  delayMicroseconds(10);

  PORTB &= ~pinTrig_right;

  double duration = pulseIn(13, HIGH, 100000);

  double cm = (duration/2) * 0.0343;
  return cm;
}

// uint16_t* detectCm_4direction() {
//   double distance_front = distanceCm_front();
//   double distance_back = distanceCm_back();
//   double distance_left = distanceCm_left();
//   double distance_right = distanceCm_right();

//   Serial.println(distance_right);

//   static uint16_t distance_array[4] = {distance_front, distance_back, distance_left, distance_right};

//   return distance_array;
// }

// Serial moritor 에 거리, 횟수, 폭 표시
void showDistance(double d_1, double d_2, double d_3, double d_4, uint8_t c_left, uint8_t c_right, uint16_t l_left, uint16_t l_right) {
  Serial.print("front: ");
  Serial.print(d_1);
  Serial.print("   ");
  Serial.print("back: ");
  Serial.print(d_2);
  Serial.print("   ");
  Serial.print("left: ");
  Serial.print(d_3);
  Serial.print("   ");
  Serial.print("right: ");
  Serial.println(d_4);

  Serial.print("count_left: ");
  Serial.print(c_left);
  Serial.print("   ");
  Serial.print("count_right: ");
  Serial.println(c_right);

  Serial.print("length_left: ");
  Serial.print(l_left);
  Serial.print("   ");
  Serial.print("length_right: ");
  Serial.println(l_right);

  Serial.println();
}

// 횟수 카운트
uint8_t countLength(uint8_t width, uint8_t cnt) {
  if(width > Car_Width + 2) {
    cnt += 1;

    // if(cnt * Time_Interval * Velocity_MeterperSecond > Car_Width * 1000) {
    // return cnt * Time_Interval * Velocity_MeterperSecond;
    // }
  }
  else if(width < Car_Width + 2) {
    cnt = 0;

    // return 0;
  }
  // width = cnt * Time_Interval * Velocity_MeterperSecond;
  return cnt;
  // return width;
}

bool decideParking(uint16_t length) {
  if(length >= Specific_Count * Time_Interval) {
    return 1;
  }
  else {
    return 0;
  }
}

//개념 생각한것
//일단 초음파센서로 부터 받아오는값 10번이상(차 길이 이상, 평행주차 가능)찍히면, 차폭만큼만 일단 서보모터 돌아가게 하고 dc모터 돌어갈 시간을 준다.
// 초음파 센서로부터 읽히는 그 값 자체도 받아와서 주차를 한 후에 부족함이 있으면 그 값을 실시간으로 계속 보면서 서보모터 바퀴각을 180도 풀지 말지 결정한다. 