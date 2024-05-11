#define pinTrig_1     3
#define pinEcho_1     2

#define pinTrig_2     6
#define pinEcho_2     5

#define pinTrig_3     9
#define pinEcho_3     8

#define pinTrig_4     12
#define pinEcho_4     11

double measureDistanceCm_1() {
  digitalWrite(pinTrig_1, LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrig_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig_1, LOW);

  double duration = pulseIn(pinEcho_1, HIGH);

  double cm = (duration/2) * 0.0343;
  return cm;
}

double measureDistanceCm_2() {
  digitalWrite(pinTrig_2, LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrig_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig_2, LOW);

  double duration = pulseIn(pinEcho_2, HIGH);

  double cm = (duration/2) * 0.0343;
  return cm;
}

double measureDistanceCm_3() {
  digitalWrite(pinTrig_3, LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrig_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig_3, LOW);

  double duration = pulseIn(pinEcho_3, HIGH);

  double cm = (duration/2) * 0.0343;
  return cm;
}

double measureDistanceCm_4() {
  digitalWrite(pinTrig_4, LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrig_4, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig_4, LOW);

  double duration = pulseIn(pinEcho_4, HIGH);

  double cm = (duration/2) * 0.0343;
  return cm;
}

void setup() {
  Serial.begin(9600);

  pinMode(pinTrig_1, OUTPUT);
  pinMode(pinEcho_1, INPUT);

  pinMode(pinTrig_2, OUTPUT);
  pinMode(pinEcho_2, INPUT);

  pinMode(pinTrig_3, OUTPUT);
  pinMode(pinEcho_3, INPUT);

  pinMode(pinTrig_4, OUTPUT);
  pinMode(pinEcho_4, INPUT);
}

void loop() {
  double distance_1 = measureDistanceCm_1();
  double distance_2 = measureDistanceCm_2();
  double distance_3 = measureDistanceCm_3();
  double distance_4 = measureDistanceCm_4();
  Serial.print("1st: ");
  Serial.print(distance_1);
  Serial.print("   ");

  Serial.print("2nd: ");
  Serial.print(distance_2);
  Serial.print("   ");

  Serial.print("3rd: ");
  Serial.print(distance_3);
  Serial.print("   ");

  Serial.print("4th: ");
  Serial.println(distance_4);

  delay(1000);
}