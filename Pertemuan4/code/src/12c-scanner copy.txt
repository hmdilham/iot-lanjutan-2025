#include <Arduino.h>
#include <Wire.h>
void setup() {
  Serial.begin(115200);
  Wire.begin();
}
void loop() {
  byte error, address;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Device ditemukan di alamat 0x");
      Serial.println(address, HEX);
    }
  }
  delay(5000);
}