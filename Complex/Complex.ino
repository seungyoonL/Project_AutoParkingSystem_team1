#define pinTrig 0x04
#define pinEcho 0x09

#define OC2A 0x08

#define LED_2 0x02
#define LED_1 0x01

uint16_t value = 0;

ISR(TIMER2_COMPA_vect) {
  value = ADC;

  if (ADC > 300) {
    OCR2A = 40;
  }
  else {
    OCR2A = 0;
  }

}

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
  DDRB |= pinTrig | LED_2; | LED_1;
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

void init_Timer() {
  cli();

  DDRB |= OC2A;

  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  TCCR2A |= (1 << COM2A1);

  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

  TIMSK2 |= (1 << OCIE2A);

  OCR2A = 0;

  sei();
}

void setup() {
  init_Serial();
  init_ADC();
  init_LED();
  init_Timer();
}

void loop() {
  double distance = measureDistanceCm();
  Serial.println(distance);

  if (distance >= 50.0) {
    PORTB |= LED_2; | LED_1;
  }
  else{
    PORTB &= ~(LED_2 | LED_1);
  }

  ADCSRA |= (1 << ADSC);

  while(ADCSRA & (1 << ADSC));

  Serial.println(value);
  delay(1000);
}
