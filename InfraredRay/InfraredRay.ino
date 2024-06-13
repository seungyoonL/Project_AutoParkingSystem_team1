// #define IR_LED         0x08
// #define IR_Receiver    0x04

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
//   pinMode(A0,INPUT);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//  int value = analogRead(A0);
//  Serial.println(value);
//  delay(100);
// }


void init_ADC() {
  ADMUX &= ~ (1 << REFS1);
  ADMUX |= (1 << REFS0);

  ADMUX &= ~ (1 << ADLAR);
  // ADMUX |= (1 << ADLAR);

  // ADMUX &= ~ ((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
  
  ADMUX &= ~ ((1 << MUX3) | (1 << MUX2) | (1 << MUX0));
  ADMUX |= (1 << MUX1);

  // ADMUX &= ~ ((1 << MUX3) | (1 << MUX2));
  // ADMUX |= (1 << MUX1) | (1 << MUX0);

  ADCSRA |= (1 << ADEN);

  ADCSRA |= (1 << ADATE);

  // ADC Interrupt Enable
  // ADCSRA |= (1 << ADIE);

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  ADCSRB &= ~ ((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
}

void setup() {
  Serial.begin(9600);

  init_ADC();

  ADCSRA |= (1 << ADSC);
}

void loop() {
  ADCSRA |= (1 << ADSC);

  uint16_t value = ADC;
  // uint8_t value = ADCH;
  Serial.println(value);

  delay(50);
}