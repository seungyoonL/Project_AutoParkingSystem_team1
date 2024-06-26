#include <SoftwareSerial.h>

#define BT_TX 12  // 12
#define BT_RX 13  // 13

// 초음파 센서 Trig, Echo 핀 번호 설정
#define pinTrig_front     0x10  // 4
#define pinEcho_front     0x20  // 5
#define pinTrig_back      0x40  // 6
#define pinEcho_back      0x80  // 7
#define pinTrig_left      0x01  // 8
#define pinEcho_left      0x02  // 9
#define pinTrig_right     0x04  // 10
#define pinEcho_right     0x08  // 11

// 적외선 센서 출력 핀
#define ir_output          0x04  // 2

// 차량 전장, 전폭 설정
#define Car_Length        180
#define Car_Width         140

// 측정 주기, 차량 속도
#define Time_Interval     20

#define Velocity_MeterperSecond   1

#define Specific_Count    8

SoftwareSerial bluetooth(BT_TX, BT_RX);

volatile uint8_t chosen_slave = 0x00;
volatile uint8_t general_call = 0x00;
volatile uint8_t slave1 = 0x02; // 0b 00000010
volatile uint8_t slave2 = 0x04; // 0b 00000100
volatile uint8_t data = 99; // dummy data
volatile uint8_t* p_data = &data;

volatile bool decision = false;
volatile bool decision_left = false;
volatile bool decision_right = false;

volatile uint8_t direction = 0;

volatile uint8_t count_left = 0;
volatile uint8_t count_right = 0;
volatile uint8_t count_front = 0;
volatile uint8_t count_back = 0;
volatile uint8_t count = 0;

volatile uint16_t ir_value = 600;

void master_setup();
void master_communication_start();
void master_write_start();
void master_write_stop();
void master_read_start();
void master_read_stop();

void ultraSonic_setup();
uint16_t distanceMm_front();
uint16_t distanceMm_back();
uint16_t distanceMm_left();
uint16_t distanceMm_right();
void showDistance(uint16_t d_1, uint16_t d_2, uint16_t d_3, uint16_t d_4, uint8_t c_left, uint8_t c_right, uint16_t l_left, uint16_t l_right);
uint8_t countLength_P(uint16_t width, uint16_t cnt);
uint8_t countWidth_T(uint16_t length, uint16_t cnt);

bool decideParking_P(uint16_t length);
bool decideParking_T(uint16_t width);

void ADC_front_setup();
void ADC_back_setup();
void ADC_left_setup();
void ADC_right_setup();
void infraredray_setup();
void external_setup();
void ADC_pulse();

ISR(TWI_vect) {
  switch (TWSR & 0xF8) {
    case 0x08: // START 보내기 성공시
      if (data == 0) { // Master read모드
        TWDR = chosen_slave + 1; // TWDR에 slave_address 및 read모드(1) 입력
      } else { // Master write모드
        TWDR = chosen_slave + 0; // TWDR에 slave_address 및 write모드(0) 입력
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

// 외부 인터럽트 서비스 루틴
ISR(INT0_vect) {
  decision = false;
  direction = 0;
  ir_value = 600;

  ADCSRA &= ~(1 << ADEN);   // ADC 비활성화

  data = 30;
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  master_setup();
  ultraSonic_setup();
  infraredray_setup();
  external_setup();
}

void loop() {
  while(bluetooth.available()) { // 블루투스 값 받기
    char received = bluetooth.read();
    if (received == '0') { // 0이면 읽기, 1 또는 2면 쓰기
      data = 0;
    } else if (received == '1') { // 평행주차
      data = 1;
    } else if (received == '2') { // T주차
      data = 2;
    }
    
  if (bluetooth.available()) char dummy = bluetooth.read(); // '\n' 입력 안되게 버퍼에서 제거
  } // 블루투스 값 받기 끝

  if (data == 1) { // 평행주차
    chosen_slave = general_call; // Slave 모두에게 보내기
    master_write_start();
    delay(10);
    

    while(!decision) { // 주차 공간 찾기

      // 전방, 후방, 좌측, 우측 거리 측정
      uint16_t distance_front = distanceMm_front();
      uint16_t distance_back = distanceMm_back();
      uint16_t distance_left = distanceMm_left();
      uint16_t distance_right = distanceMm_right();

      count_left = countLength_P(distance_left, count_left);
      count_right = countLength_P(distance_right, count_right);

      uint16_t lengthmm_left = count_left * Time_Interval * Velocity_MeterperSecond;
      uint16_t lengthmm_right = count_right * Time_Interval * Velocity_MeterperSecond;

      showDistance(distance_front, distance_back, distance_left, distance_right, count_left, count_right, lengthmm_left, lengthmm_right);

      // 전방 20cm 이내에 장애물 감지되면 정지
      if (distance_front <= 200) {
        data = 50;
        master_write_start();

        delay(10);

        decision = true;
      }

      delay(Time_Interval);

      decision_left = decideParking_P(lengthmm_left);
      decision_right = decideParking_P(lengthmm_right);

      if (decision_right == true) {
        decision = true;
        direction = 2; // 오른쪽 공간 찾음
      } else if (decision_left == true) {
        decision = true;
        direction = 1; // 왼쪽 공간 찾음
      }
    }

    if (direction == 1) { // 왼쪽 공간 찾았으면
      ADC_right_setup();  // 오른쪽 적외선 센서 활성화

      count_left = 0;
      count_right = 0;

      data = 10;
      master_write_start();
      
      delay(3000); // DC 멈출 때까지 기다림

      ADCSRA |= (1 << ADSC);    // 주차공간 찾으면 ADC 시작

      while (decision == true) { // 왼쪽 물체에 가까워질 때까지
        ADC_pulse();

        if (data == 30) {
          master_write_start();
          delay(1000); // DC 멈출 때까지 기다림

          data = 99; // data 값 초기화
        }

        // 주차하면서 거리값을 계속 받아옴
        uint16_t new_update_distance = distanceMm_left();
        Serial.print("new_update_distance: ");
        Serial.println(new_update_distance);

        // 남은 공간이 15cm 이하이면 정지
        if (new_update_distance <= 150) {
          count += 1;
        } else if (new_update_distance > 150) {
          count = 0;
        }

        if (count >= 3) {
          data = 11;
          master_write_start();
          delay(1000); // DC 멈출 때까지 기다림
          
          decision = false;
          direction = 0; // 방향 초기화
          data = 99; // data 값 초기화

          ADCSRA &= ~(1 << ADEN);
        }
      }
    } else if (direction == 2) { // 오른쪽 공간 찾았으면
      ADC_left_setup();  // 왼쪽 적외선 센서 활성화
      delay(10);

      count_left = 0;
      count_right = 0;

      data = 15;
      master_write_start();

      delay(3000); // DC 멈출 때까지 기다림

      ADCSRA |= (1 << ADSC);    // 주차공간 찾으면 ADC 시작

      while (decision == true) { // 오른쪽 물체에 가까워질 때까지
        ADC_pulse();

        if (data == 30) {
          master_write_start();
          delay(1000); // DC 멈출 때까지 기다림

          data = 99; // data 값 초기화
        }

        // 주차하면서 거리값을 계속 받아옴
        uint16_t new_update_distance = distanceMm_right();
        Serial.print("new_update_distance: ");
        Serial.println(new_update_distance);

        // 남은 공간이 15cm 이하이면 정지
        if (new_update_distance <= 150) {
          count += 1;
        } else if (new_update_distance > 150) {
          count = 0;
        }

        if (count >= 3) {
          data = 16;
          master_write_start();
          delay(1000); // DC 멈출 때까지 기다림
          
          decision = false;
          direction = 0; // 방향 초기화
          data = 99; // data 값 초기화

          ADCSRA &= ~(1 << ADEN);
        }
      }
    }
  } else if (data == 2) { // T자 주차
    chosen_slave = general_call; // Slave 모두에게 보내기
    master_write_start();
    delay(10);
    

    while(!decision) { // 주차공간 찾기

      // 전방, 후방, 좌측, 우측 거리 측정
      uint16_t distance_front = distanceMm_front();
      uint16_t distance_back = distanceMm_back();
      uint16_t distance_left = distanceMm_left();
      uint16_t distance_right = distanceMm_right();

      count_left = countWidth_T(distance_left, count_left);
      count_right = countWidth_T(distance_right, count_right);

      uint16_t lengthmm_left = count_left * Time_Interval * Velocity_MeterperSecond;
      uint16_t lengthmm_right = count_right * Time_Interval * Velocity_MeterperSecond;

      showDistance(distance_front, distance_back, distance_left, distance_right, count_left, count_right, lengthmm_left, lengthmm_right);

      if (distance_front <= 200) {    // 전방 20cm 이내에 장애물 감지되면 정지
        data = 50;
        master_write_start();

        delay(10);

        decision = true;
      }

      delay(Time_Interval);

      decision_left = decideParking_T(lengthmm_left);
      decision_right = decideParking_T(lengthmm_right);

      if (decision_right == true) {
        decision = true;
        direction = 2; // 오른쪽 공간 찾음
      } else if (decision_left == true) {
        decision = true;
        direction = 1; // 왼쪽 공간 찾음
      }
    }

    if (direction == 1) { // 왼쪽 공간 찾았으면
      ADC_front_setup();  // 앞쪽 적외선 센서 활성화
      delay(10);

      count_left = 0;
      count_right = 0;

      data = 20;
      master_write_start();
      
      delay(4500); // DC 멈출 때까지 기다림

      ADCSRA |= (1 << ADSC);    // 주차공간 찾으면 ADC 시작

      while (decision == true) { // 뒤쪽 물체에 가까워질 때까지
        ADC_pulse();

        if (data == 30) {
          master_write_start();
          delay(1000); // DC 멈출 때까지 기다림

          data = 99; // data 값 초기화
        }

        // 주차하면서 거리값을 계속 받아옴
        uint16_t new_update_distance = distanceMm_back();
        Serial.print("new_update_distance: ");
        Serial.println(new_update_distance);

        // 남은 공간이 15cm 이하이면 정지
        if (new_update_distance <= 150) {
          count += 1;
        } else if (new_update_distance > 150) {
          count = 0;
        }

        if (count >= 3) {
          data = 21;
          master_write_start();
          delay(1000); // DC 멈출 때까지 기다림
          
          decision = false;
          direction = 0; // 방향 초기화
          data = 99; // data 값 초기화

          // ADC_disable();
          ADCSRA &= ~(1 << ADEN);
        }
      }
    } else if (direction == 2) { // 오른쪽 공간 찾았으면
      ADC_front_setup();  // 앞쪽 적외선 센서 활성화
      delay(10);

      count_left = 0;
      count_right = 0;

      data = 25;
      master_write_start();
      
      delay(4500); // DC 멈출 때까지 기다림

      ADCSRA |= (1 << ADSC);    // 주차공간 찾으면 ADC 시작

      while (decision == true) { // 뒤쪽 물체에 가까워질 때까지
        ADC_pulse();

        if (data == 30) {
          master_write_start();
          delay(1000); // DC 멈출 때까지 기다림

          data = 99; // data 값 초기화
        }

        // 주차하면서 거리값을 계속 받아옴
        uint16_t new_update_distance = distanceMm_back();
        Serial.print("new_update_distance: ");
        Serial.println(new_update_distance);

        // 남은 공간이 15cm 이하이면 정지
        if (new_update_distance <= 150) {
          count += 1;
        } else if (new_update_distance > 150) {
          count = 0;
        }

        if (count >= 3) {
          data = 26;
          master_write_start();
          delay(1000); // DC 멈출 때까지 기다림
          
          decision = false;
          direction = 0; // 방향 초기화
          data = 99; // data 값 초기화

          ADCSRA &= ~(1 << ADEN);
        }
      }
    }
  }
  decision = false;
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

  uint16_t duration = pulseIn(5, HIGH);

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

  uint16_t duration = pulseIn(7, HIGH);

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

  uint16_t duration = pulseIn(9, HIGH);

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

  uint16_t duration = pulseIn(11, HIGH);

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
uint8_t countLength_P(uint16_t width, uint16_t cnt) {
  if(width >= Car_Width + 160) {
    cnt += 1;
  } else {
    cnt = 0;
  }
  return cnt;
}

uint8_t countWidth_T(uint16_t length, uint16_t cnt) {
  if(length >= Car_Length + 120) {
    cnt += 1;
  } else {
    cnt = 0;
  }
  return cnt;
}

// 주차 가능 여부 판단
bool decideParking_P(uint16_t length) {
  if(length >= Specific_Count * Time_Interval * Velocity_MeterperSecond) {
    return 1;
  } else {
    return 0;
  }
}

bool decideParking_T(uint16_t width) {
  if(width >= Specific_Count * Time_Interval * Velocity_MeterperSecond) {
    return 1;
  }
  else {
    return 0;
  }
}

// 전방 적외선 센서 설정 A0
void ADC_front_setup() {
  ADMUX &= ~ (1 << REFS1);
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN);
  ADMUX &= ~ (1 << ADLAR);

  ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));

  ADCSRA |= (1 << ADATE);

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  ADCSRB &= ~ ((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
}

// 후방 적외선 센서 설정 A1
void ADC_back_setup() {
  ADMUX &= ~ (1 << REFS1);
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN);
  ADMUX &= ~ (1 << ADLAR);

  ADMUX &= ~ ((1 << MUX3) | (1 << MUX2) | (1 << MUX1));
  ADMUX |= (1 << MUX0);

  ADCSRA |= (1 << ADATE);

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  ADCSRB &= ~ ((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
}

// 좌측방 적외선 센서 설정 A2
void ADC_left_setup() {
  ADMUX &= ~ (1 << REFS1);
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN);
  ADMUX &= ~ (1 << ADLAR);

  ADMUX &= ~ ((1 << MUX3) | (1 << MUX2) | (1 << MUX0));
  ADMUX |= (1 << MUX1);

  ADCSRA |= (1 << ADATE);

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  ADCSRB &= ~ ((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
}

// 우측방 적외선 센서 설정 A3
void ADC_right_setup() {
  ADMUX &= ~ (1 << REFS1);
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN);
  ADMUX &= ~ (1 << ADLAR);

  ADMUX &= ~ ((1 << MUX3) | (1 << MUX2));
  ADMUX |= ((1 << MUX1) | (1 << MUX0));

  ADCSRA |= (1 << ADATE);

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  ADCSRB &= ~ ((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
}

// 적외선 센서 연결핀 설정
void infraredray_setup() {
  DDRD |= ir_output;
}

// 외부 인터럽트 설정
void external_setup() {
  SREG |= (1 << SREG_I);

  EICRA |= (1 << ISC01);
  EICRA &= ~(1 << ISC00);
  EIMSK |= (1 << INT0);
}

// 흰 선 감지 -> high, 검은색(주차장 바닥) 감지 -> low
void ADC_pulse() {

  Serial.print("ir_value:");
  Serial.println(ir_value);

  // 검은색: 600 이상, 흰색: 250 이하
  if (ir_value >= 600) {
    PORTD &= ~ir_output;
  } else if (ir_value <= 250) {
    PORTD |= ir_output;
  }

  ir_value = ADC;
}

