#include <SoftwareSerial.h>

#define BT_TX 12  // 12
#define BT_RX 13  // 13

#define FRONT_SERVO 0x04  // 2
#define REAR_SERVO  0x08  // 3

// 초음파 센서 Trig, Echo 핀 번호 설정
#define pinTrig_front     0x10  // 4
#define pinEcho_front     0x20  // 5
#define pinTrig_back      0x40  // 6
#define pinEcho_back      0x80  // 7
#define pinTrig_left      0x01  // 8
#define pinEcho_left      0x02  // 9
#define pinTrig_right     0x04  // 10
#define pinEcho_right     0x08  // 11

// 차량 전장, 전폭 설정
#define Car_Length        300
#define Car_Width         80

// 측정 주기, 차량 속도
#define Time_Interval     20

#define Velocity_MeterperSecond   1

#define Specific_Count    10

SoftwareSerial bluetooth(BT_TX, BT_RX);

volatile uint8_t slave1 = 0x02; // 0b 00000010
volatile uint8_t data = 99; // dummy data
volatile uint8_t* p_data = &data;

volatile bool decision = false;
// volatile static uint8_t count_left = 0;
// volatile static uint8_t count_right = 0;
// volatile static uint8_t count = 0;

void master_setup();
void master_communication_start();
void master_write_start();
void master_write_stop();
void master_read_start();
void master_read_stop();

void servo_setup();
void servo_front_0();
void servo_front_90();
void servo_front_180();
void servo_rear_0();
void servo_rear_90();
void servo_rear_180();

void ultraSonic_setup();
uint16_t distanceMm_front();
uint16_t distanceMm_back();
uint16_t distanceMm_left();
uint16_t distanceMm_right();
void showDistance(uint16_t d_1, uint16_t d_2, uint16_t d_3, uint16_t d_4, uint8_t c_left, uint8_t c_right, uint16_t l_left, uint16_t l_right);
uint8_t countLength(uint8_t width, uint8_t cnt);
bool decideParking(uint16_t length);

ISR(TWI_vect) {
  switch (TWSR & 0xF8) {
    case 0x08: // START 보내기 성공시
      if (data == 0) { // Master read모드
        TWDR = slave1 + 1; // TWDR에 slave_address 및 read모드(1) 입력
      } else { // Master write모드
        TWDR = slave1 + 0; // TWDR에 slave_address 및 write모드(0) 입력
      }
      TWCR &= ~(1 << TWSTA); // START 비트 0으로 만들어줌
      master_communication_start(); // 통신 시작
      break;
    case 0x18: // SLA+W 보내기 성공시
      TWDR = data; // TWDR에 Master의 data값 입력
      master_communication_start(); // 통신 시작
      break;
    case 0x28: // data 보내기 성공시
      Serial.print("Transferred data: ");
      Serial.println(data);
      master_write_stop(); // STOP 보내기
      break;

    case 0x40: // SLA+R 보내기 성공시
      master_communication_start(); // 통신 시작
      break;
    case 0x58: // TWDR에 Slave의 data값 입력 완료시
      *p_data = TWDR; // data에 TWDR값 입력
      Serial.print("Received data: ");
      Serial.println(data);
      master_read_stop(); // STOP 보내기
      break;
      
    default:
      master_communication_start(); // 통신 시작
      break;
  }
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  master_setup();
  servo_setup();
  ultraSonic_setup();
}

void loop() {
  while(bluetooth.available()) {
    char received = bluetooth.read();
    if (received == '0') { // 0이면 읽기, 1 또는 2면 쓰기
      data = 0;
    } else if (received == '1') { // 평행주차
      data = 1;
    } else if (received == '2') { // T주차
      data = 2;
    }
    
  if (bluetooth.available()) char dummy = bluetooth.read(); // '\n' 입력 안되게 버퍼에서 제거
  }

  static uint8_t count_left = 0;
  static uint8_t count_right = 0;
  static uint8_t count = 0;

  if (data == 1){ // 평행주차
    master_write_start();
    delay(10);
    

    while(!decision) { // 주차공간 찾기

      uint16_t distance_front = distanceMm_front();
      uint16_t distance_back = distanceMm_back();
      uint16_t distance_left = distanceMm_left();
      uint16_t distance_right = distanceMm_right();

      count_left = countLength(distance_left, count_left);
      count_right = countLength(distance_right, count_right);

      uint16_t lengthmm_left = count_left * Time_Interval * Velocity_MeterperSecond;
      uint16_t lengthmm_right = count_right * Time_Interval * Velocity_MeterperSecond;

      showDistance(distance_front, distance_back, distance_left, distance_right, count_left, count_right, lengthmm_left, lengthmm_right);

      delay(Time_Interval);

      decision = decideParking(lengthmm_left);
    }

    if (decision == 1) { // 주차공간 찾으면
      count_left = 0;

      data = 11;
      master_write_start();
      
      delay(2500); // DC 멈출 때까지 기다림

      servo_front_180(); // 바퀴 회전
      servo_rear_180(); // 바퀴 회전

      while (decision == 1) { // 가까워질 때까지
        uint16_t new_update_distance = distanceMm_left();
        Serial.print("new_update_distance: ");
        Serial.println(new_update_distance);

        if (new_update_distance <= 50) {
          count += 1;
        } else if (new_update_distance > 50) {
          count = 0;
        }

        if (count >= 10) {
          decision = 0;

          data = 12;
          master_write_start();
          delay(1000); // DC 멈출 때까지 기다림

          servo_front_0();
          servo_rear_0();

          data = 99; // data 값 초기화
        }
      }
    }
  }


  Serial.print(".");
  delay(1000);
}

void master_setup() {
  cli(); // Global Interrupt Disable
  TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // Prescaler 0
  TWBR = 12; // 16Mhz / (16 + 2 * 12(TWBR) * 1(Prescaler 0)) = 400kHz => 최대 통신속도
  TWCR |= (1 << TWEN) | (1 << TWIE); // TWI, TWIE Enable
  sei(); // Global Interrupt Enable
}

void master_communication_start() {
  TWCR |= (1 << TWINT); // 통신시작(TWINT에 1 넣기 => toggle)
}

void master_write_start() {
  TWCR |= (1 << TWINT) | (1 << TWSTA); // START 보내기 및 통신 시작
}

void master_write_stop() {
  TWCR |= (1 << TWINT) | (1 << TWSTO); // STOP 보내기 및 통신 시작
  while (TWCR & (1 << TWSTO)); // TWSTO가 다시 0 될때까지 기다림
}

void master_read_start() {
  TWCR |= (1 << TWINT) | (1 << TWSTA); // START 보내기 및 통신 시작
}

void master_read_stop() {
  TWCR |= (1 << TWINT) | (1 << TWSTO); // STOP 보내기 및 통신 시작
  while (TWCR & (1 << TWSTO)); // TWSTO가 다시 0 될때까지 기다림
}

void servo_setup() {
  DDRD |= FRONT_SERVO; // 서보모터 PWM(주황색) 핀을 출력으로 연결시킴
  DDRD |= REAR_SERVO;

  servo_front_0();
  servo_rear_0();
}

void servo_front_0() { // 서보모터 0도                     
   for(uint8_t i = 0; i < 50; i++) {
    PORTD |= FRONT_SERVO;
    delayMicroseconds(500);
    PORTD &=~ FRONT_SERVO;
    delayMicroseconds(19500);
  }
}

void servo_front_90() { // 서보모터 90도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= FRONT_SERVO;
    delayMicroseconds(1500);
    PORTD &=~ FRONT_SERVO;
    delayMicroseconds(18500);
  }
}

void servo_front_180() { // 서보모터 180도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= FRONT_SERVO;
    delayMicroseconds(2500);
    PORTD &=~ FRONT_SERVO;
    delayMicroseconds(17500);
  }
}

void servo_rear_0() { // 서보모터 0도                     
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= REAR_SERVO;
    delayMicroseconds(500);
    PORTD &=~ REAR_SERVO;
    delayMicroseconds(19500);
  }
}

void servo_rear_90() { // 서보모터 90도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= REAR_SERVO;
    delayMicroseconds(1500);
    PORTD &=~ REAR_SERVO;
    delayMicroseconds(18500);
  }
}

void servo_rear_180() { // 서보모터 180도
  for(uint8_t i = 0; i < 50; i++) {
    PORTD |= REAR_SERVO;
    delayMicroseconds(2500);
    PORTD &=~ REAR_SERVO;
    delayMicroseconds(17500);
  }
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

  uint16_t duration = pulseIn(5, HIGH, 100000);

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

  uint16_t duration = pulseIn(7, HIGH, 100000);

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

  uint16_t duration = pulseIn(9, HIGH, 100000);

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

  uint16_t duration = pulseIn(11, HIGH, 100000);

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

bool decideParking(uint16_t length) {
  if(length >= Specific_Count * Time_Interval * Velocity_MeterperSecond) {
    return 1;
  }
  else {
    return 0;
  }
}