#include <SoftwareSerial.h>

#define BT_TX 10
#define BT_RX 11

SoftwareSerial btSerial(BT_TX, BT_RX);

void setup() {
  Serial.begin(9600);
  btSerial.begin(38400);
}

void loop() {
  if(btSerial.available()){
    char received = btSerial.read();
    Serial.write(received);
  }

  if(Serial.available()){
    char transmitted = Serial.read();
    btSerial.write(transmitted);
  }
}