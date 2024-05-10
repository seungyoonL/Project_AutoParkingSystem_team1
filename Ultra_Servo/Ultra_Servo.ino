#define pinTrig 0x01
#define pinEcho 0x02
#define servoPin 0x04

double measureDistanceCm(){
  //digitalWrite(pinTrig, LOW);
  PORTB &= ~pinTrig;
  delayMicroseconds(5);
  //digitalWrite(pinTrig, HIGH);
  PORTB |= pinTrig;
  delayMicroseconds(10);
  //digitalWrite(pinTrig, LOW);
  PORTB &= ~pinTrig;

  double duration = pulseIn(9, HIGH);

  double cm = (duration/2) * 0.0343;
  return cm;
}

void init_Serial() {
  Serial.begin(9600);
  delay(1000);
}

void init_Ultra() {
  DDRB |= pinTrig;
  DDRB &= ~pinEcho;
}

void init_Servo() {
  DDRB |= 0x04;
}

void setup() {
  init_Serial();
  init_Ultra();
  init_Servo();
}

void Servoturn_45() {
  for (uint8_t i = 0; i < 60; i++){
    PORTB |= servoPin;
    delayMicroseconds(1000);
    PORTB &= ~servoPin;
    delayMicroseconds(19000);
  };
}

void Servoturn_90() {
  for (uint8_t i = 0; i < 60; i++){
    PORTB |= servoPin;
    delayMicroseconds(1500);
    PORTB &= ~servoPin;
    delayMicroseconds(18500);
  };
}

void loop() {
  double distance = measureDistanceCm();
  Serial.println(distance);
  if (distance < 200){
    Servoturn_45();
    delay(5000);
  } else{
    Servoturn_90();
    delay(5000);
  }
  delay(100);
}
