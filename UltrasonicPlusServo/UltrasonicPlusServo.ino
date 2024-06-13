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
#define Car_Length        300
#define Car_Width         80

// 측정 주기, 차량 속도
#define Time_Interval     20

#define Velocity_MeterperSecond   1

#define Specific_Count    10

void setup() {
  // 마프 1조 Auto Parking System 팀플 시작!
  
  DDRB |= FRONT_SERVO; // 서보모터 PWM(주황색) 핀을 출력으로 연결시킴
  DDRB |= REAR_SERVO;
  
  Serial.begin(9600);

  front_motor0(); // 처음엔 조향각 0도
  rear_motor0();

  ultraSonic_setup();
}

void loop() {
  static uint8_t count_left = 0;
  static uint8_t count_right = 0;

  uint16_t lengthmm_left;
  uint16_t lengthmm_right;

  uint16_t distance_front;
  uint16_t distance_back;
  uint16_t distance_left;
  uint16_t distance_right;

  distance_front = distanceMm_front();
  distance_back = distanceMm_back();
  distance_left = distanceMm_left();
  distance_right = distanceMm_right();

  count_left = countLength(distance_left, count_left);
  count_right = countLength(distance_right, count_right);

  // 탐지 공간의 폭 계산
  lengthmm_left = count_left * Time_Interval * Velocity_MeterperSecond;
  lengthmm_right = count_right * Time_Interval * Velocity_MeterperSecond;

  showDistance(distance_front, distance_back, distance_left, distance_right, count_left, count_right, lengthmm_left, lengthmm_right);

  delay(Time_Interval);

  bool decision = decideParking(lengthmm_left);

  if (decision == 1){ 
    delay(2000);               // 2초 있다가 앞바퀴 2개 완전히 돌리기
    front_motor180();
    rear_motor180();

    uint16_t update_distance = distanceMm_left();
    while(update_distance > 50){
      uint16_t new_update_distance = distanceMm_left();
      Serial.print("new_update_distance: ");
      Serial.println(new_update_distance);
      if (new_update_distance < 50){           // 만약에 그 조건이 유지되지 못하면 value를 초기화 함으로써 와일문을 탈출함
        count_left = 0;
        break;                 
      }
    }
    front_motor0();         // 탈출 후 0도로 만들고 내가 관찰하는 value 값이 저 조건 만족안하면 계속 0도 유지
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
    delayMicroseconds(1000);
    PORTB &=~ FRONT_SERVO;
    delayMicroseconds(19000);
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

// 전방 거리 측정 mm
uint16_t distanceMm_front() {
  PORTD &= ~pinTrig_front;
  delayMicroseconds(5);
  
  PORTD |= pinTrig_front;
  delayMicroseconds(10);

  PORTD &= ~pinTrig_front;

  uint16_t duration = pulseIn(2, HIGH, 100000);

  uint16_t mm = (duration/2) * 0.343;
  return mm;
}

// 후방 거리 측정 mm
uint16_t distanceMm_back() {
  PORTD &= ~pinTrig_back;
  delayMicroseconds(5);
  
  PORTD |= pinTrig_back;
  delayMicroseconds(10);

  PORTD &= ~pinTrig_back;

  uint16_t duration = pulseIn(5, HIGH, 100000);

  uint16_t mm = (duration/2) * 0.343;
  return mm;
}

// 좌측방 거리 측정 mm
uint16_t distanceMm_left() {
  PORTB &= ~pinTrig_left;
  delayMicroseconds(5);
  
  PORTB |= pinTrig_left;
  delayMicroseconds(10);

  PORTB &= ~pinTrig_left;

  uint16_t duration = pulseIn(11, HIGH, 100000);

  uint16_t mm = (duration/2) * 0.343;
  return mm;
}

// 우측방 거리 측정 mm
uint16_t distanceMm_right() {
  PORTB &= ~pinTrig_right;
  delayMicroseconds(5);
  
  PORTB |= pinTrig_right;
  delayMicroseconds(10);

  PORTB &= ~pinTrig_right;

  uint16_t duration = pulseIn(13, HIGH, 100000);

  uint16_t mm = (duration/2) * 0.343;
  return mm;
}

// Serial moritor 에 거리, 횟수, 폭 표시
void showDistance(uint16_t d_1, uint16_t d_2, uint16_t d_3, uint16_t d_4, uint8_t c_left, uint8_t c_right, uint16_t l_left, uint16_t l_right) {
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
  Serial.print(d_4);
  Serial.println("");

  Serial.print("count_left: ");
  Serial.print(c_left);
  Serial.print("   ");
  Serial.print("count_right: ");
  Serial.print(c_right);
  Serial.println("");

  Serial.print("length_left: ");
  Serial.print(l_left);
  Serial.print("   ");
  Serial.print("length_right: ");
  Serial.print(l_right);
  Serial.println("");

  Serial.println();
}

// 횟수 카운트
uint8_t countLength(uint8_t width, uint8_t cnt) {
  if(width > Car_Width + 30) {
    cnt += 1;
  }
  else if(width < Car_Width + 30) {
    cnt = 0;
  }
  return cnt;
}

// 공간이 차보다 넓으면 1 반환
bool decideParking(uint16_t length) {
  if(length >= Specific_Count * Time_Interval * Velocity_MeterperSecond) {
    return 1;
  }
  else {
    return 0;
  }
}