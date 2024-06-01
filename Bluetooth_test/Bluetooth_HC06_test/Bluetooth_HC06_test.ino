#include <SoftwareSerial.h>

#define BT_TX 10
#define BT_RX 11

SoftwareSerial bluetooth(BT_TX, BT_RX);

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
}

void loop() {
  if(bluetooth.available()){
    char received = bluetooth.read();
    Serial.write(received);
  }

  if(Serial.available()){
    char transmitted = Serial.read();
    bluetooth.write(transmitted);
  }
}