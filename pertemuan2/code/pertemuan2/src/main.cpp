#include <Arduino.h>

#define LED 2              // Pin LED
#define POTENTIOMETER 34   // Pin potensiometer

void setup() {
  Serial.begin(115200);  // Inisialisasi serial
  pinMode(LED, OUTPUT);    // Set pin LED sebagai output
  Serial.println("Serial ready");  // Print "Serial ready"
}

void loop() {
  int sensorValue = analogRead(POTENTIOMETER);  // Baca nilai potensiometer
  int brightness = map(sensorValue, 0, 4095, 0, 255);  // Map nilai ke 0-255
  Serial.println(brightness);  // Print nilai ke serial monitor
  analogWrite(LED, brightness);                 // Atur kecerahan LED
  delay(10);                                   // Tunggu 10 ms
}