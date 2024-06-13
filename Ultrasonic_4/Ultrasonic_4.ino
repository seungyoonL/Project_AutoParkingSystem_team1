// 초음파 센서 Trig, Echo 핀 번호 설정
#define pinTrig_front     0x08
#define pinEcho_front     0x04
#define pinTrig_back      0x40
#define pinEcho_back      0x20
#define pinTrig_left      0x02
#define pinEcho_left      0x01
#define pinTrig_right     0x10
#define pinEcho_right     0x08

// 차량 전장, 전폭 설정
#define Car_Length        30
#define Car_Width         5

// 측정 주기, 차량 속도
#define Time_Interval     1000
#define Velocity_MeterperSecond   1

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

  double duration = pulseIn(2, HIGH);

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

  double duration = pulseIn(5, HIGH);

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

  double duration = pulseIn(8, HIGH);

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

  double duration = pulseIn(11, HIGH);

  double cm = (duration/2) * 0.0343;
  return cm;
}

// Serial moritor 에 거리, 횟수, 폭 표시
void showDistance(double d_1, double d_2, double d_3, double d_4, uint8_t c_left, uint8_t c_right, uint16_t w_left, uint16_t w_right) {
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

  Serial.print("width_left: ");
  Serial.print(w_left);
  Serial.print("   ");
  Serial.print("width_right: ");
  Serial.println(w_right);

  Serial.println();
}

// 횟수 카운트
uint8_t countWidth(uint8_t length, uint8_t cnt) {
  if(length > Car_Length) {
    cnt += 1;

    // if(cnt * Time_Interval * Velocity_MeterperSecond > Car_Width * 1000) {
    // return cnt * Time_Interval * Velocity_MeterperSecond;
    // }
  }
  else if(length < Car_Length) {
    cnt = 0;

    // return 0;
  }
  // width = cnt * Time_Interval * Velocity_MeterperSecond;
  return cnt;
  // return width;
}

// Trig는 출력으로, Echo는 입력으로 설정
void setup() {
  Serial.begin(9600);

  ultraSonic_setup();
}

void loop() {
  static uint8_t count_left = 0;
  static uint8_t count_right = 0;

  uint16_t widthmm_left;
  uint16_t widthmm_right;

  double distance_front = distanceCm_front();
  double distance_back = distanceCm_back();
  double distance_left = distanceCm_left();
  double distance_right = distanceCm_right();

  count_left = countWidth(distance_left, count_left);
  count_right = countWidth(distance_right, count_right);

  // 탐지 공간의 폭 계산
  widthmm_left = count_left * Time_Interval * Velocity_MeterperSecond;
  widthmm_right = count_right * Time_Interval * Velocity_MeterperSecond;

  showDistance(distance_front, distance_back, distance_left, distance_right, count_left, count_right, widthmm_left, widthmm_right);

  delay(Time_Interval);
}

// 탐지 공간이 차폭보다 크면 1 반환
bool decideParking(uint16_t width) {
  if(width > Car_Width) {
    return 1;
  }
  else {
    return 0;
  }
}