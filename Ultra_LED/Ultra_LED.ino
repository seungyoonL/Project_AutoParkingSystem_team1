#define pinTrig 0x04
#define pinEcho 0x08

#define LED_2 0x02
#define LED_1 0x01

double measureDistanceCm(){
  //digitalWrite(pinTrig, LOW);
  PORTB &= ~pinTrig;
  delayMicroseconds(5);
  //digitalWrite(pinTrig, HIGH);
  PORTB |= pinTrig;
  delayMicroseconds(10);
  //digitalWrite(pinTrig, LOW);
  PORTB &= ~pinTrig;

  double duration = pulseIn(12, HIGH);

  double cm = (duration/2) * 0.0343;
  return cm;
}

void init_LED() {
  DDRB |= pinTrig | LED_2 | LED_1;
  DDRB &= ~pinEcho;
}

void init_Serial() {
  Serial.begin(9600);
  delay(1000);
}

void init_ADC() {
  ADMUX |= (0 << REFS1) | (1<<REFS0);

  ADMUX |= (0 << ADLAR);

  ADMUX |= (0<< MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);

  ADCSRA |= (1 << ADEN);

  ADCSRA |= (1<< ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void setup() {
  init_Serial();
  init_ADC();
  init_LED();
}

void loop() {
  double distance = measureDistanceCm();
  Serial.println(distance);

  if (distance >= 50.0) {
    PORTB |= LED_2 | LED_1;
  }
  else{
    PORTB &= ~(LED_2 | LED_1);
  }
  delay(1000);
}
