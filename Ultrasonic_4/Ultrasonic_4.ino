// 차량 전장, 전폭 설정
// 초음파 센서 Trig, Echo 핀 번호 설정

#define pinTrig_front     0x08
#define pinEcho_front     0x04
#define pinTrig_back     0x40
#define pinEcho_back     0x20
#define pinTrig_left     0x02
#define pinEcho_left     0x01
#define pinTrig_right     0x10
#define pinEcho_right     0x08

#define Car_Length    30
#define Car_Width     10

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

// Serial moritor 에 거리, 횟수 표시
void showDistance(double d_1, double d_2, double d_3, double d_4, uint8_t c_left, uint8_t c_right) {
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

  Serial.println();
}

// 횟수 카운트
uint8_t calculateWidth(uint8_t length, uint8_t cnt) {
  if(length > Car_Length) {
    cnt ++ ;
  }
  else if(length < Car_Length) {
    cnt = 0;
  }

  return cnt;
}

// Trig는 출력으로, Echo는 입력으로 설정
void setup() {
  Serial.begin(9600);

  DDRD |= pinTrig_front;
  DDRD |= pinTrig_back;
  DDRB |= pinTrig_left;
  DDRB |= pinTrig_right;

  DDRD &= ~pinEcho_front;
  DDRD &= ~pinEcho_back;
  DDRB &= ~pinEcho_left;
  DDRB &= ~pinEcho_right;
}

void loop() {
  static uint8_t count_left = 0;
  static uint8_t count_right = 0;

  double distance_front = distanceCm_front();
  double distance_back = distanceCm_back();
  double distance_left = distanceCm_left();
  double distance_right = distanceCm_right();

  count_left = calculateWidth(distance_left, count_left);
  count_right = calculateWidth(distance_right, count_right);

  showDistance(distance_front, distance_back, distance_left, distance_right, count_left, count_right);

  delay(1000);
}