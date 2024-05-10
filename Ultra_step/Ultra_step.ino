#include <Stepper.h>

#define pinTrig 0x20
#define pinEcho 0x40
#define servoPin 0x80


// 2048:한바퀴(360도), 1024:반바퀴(180도)...
const int stepsPerRevolution = 2048; 
// 모터 드라이브에 연결된 핀 IN4, IN2, IN3, IN1
Stepper myStepper(stepsPerRevolution,11,9,10,8);  

uint8_t count = 0;



double measureDistanceCm(){
  //digitalWrite(pinTrig, LOW);
  PORTD &= ~pinTrig;
  delayMicroseconds(5);
  //digitalWrite(pinTrig, HIGH);
  PORTD |= pinTrig;
  delayMicroseconds(10);
  //digitalWrite(pinTrig, LOW);
  PORTD &= ~pinTrig;

  double duration = pulseIn(6, HIGH);

  double cm = (duration/2) * 0.0343;
  return cm;
}

void init_Serial() {
  Serial.begin(9600);
  delay(1000);
}

void init_Ultra() {
  DDRD |= pinTrig;
  DDRD &= ~pinEcho;
}

void init_Servo() {
  DDRD |= 0x80;
}

void setup() {
  init_Serial();
  init_Ultra();
  init_Servo();
  myStepper.setSpeed(14);
}

void Servoturn_0() {
  for (uint8_t i = 0; i < 80; i++){
    PORTD |= servoPin;
    delayMicroseconds(500);
    PORTD &= ~servoPin;
    delayMicroseconds(19500);
  };
}

void Servoturn_45() {
  for (uint8_t i = 0; i < 80; i++){
    PORTD |= servoPin;
    delayMicroseconds(1000);
    PORTD &= ~servoPin;
    delayMicroseconds(19000);
  };
}

void Servoturn_90() {
  for (uint8_t i = 0; i < 100; i++){
    PORTD |= servoPin;
    delayMicroseconds(1500);
    PORTD &= ~servoPin;
    delayMicroseconds(18500);
  };
}

void loop() {
  double distance = measureDistanceCm();
  Serial.println(distance);
  if (distance < 5){
    Serial.print("5");
    Servoturn_0();
    myStepper.step(stepsPerRevolution);
  } else if (distance >= 15){
    myStepper.step(-stepsPerRevolution);
    delay(200);
    Servoturn_90();
    delay(200);
    myStepper.step(-stepsPerRevolution);
  } 
}
