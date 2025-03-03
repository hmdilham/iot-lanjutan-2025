#include <Arduino.h>

void setup() {
  Serial.begin(115200); // Inisialisasi UART dengan baud rate 115200
  Serial.println("Hello, ESP32 Ready to receive data");
}

void loop() {
  if (Serial.available()) { // Cek apakah ada data yang diterima
    String data = Serial.readStringUntil('\n'); // Baca data
    Serial.print("Received: ");
    Serial.println(data); // Tampilkan data yang diterima
  }
}