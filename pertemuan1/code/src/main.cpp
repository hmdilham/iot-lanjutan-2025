#include <Arduino.h>  // Wajib untuk PlatformIO

#define LED_BUILTIN 2 // GPIO 2 adalah LED bawaan ESP32

void setup() {
  Serial.begin(115200);  // Inisialisasi Serial Monitor dengan baudrate 115200
  pinMode(LED_BUILTIN, OUTPUT); // Set LED sebagai output
  Serial.println("ESP32 Initialized!"); // Tampilkan pesan awal
}

void loop() {
  Serial.println("LED ON");   // Tampilkan pesan di Serial Monitor
  digitalWrite(LED_BUILTIN, HIGH); // Nyalakan LED
  delay(1000);                      // Tunggu 1 detik

  Serial.println("LED OFF");  // Tampilkan pesan di Serial Monitor
  digitalWrite(LED_BUILTIN, LOW);  // Matikan LED
  delay(1000);                     // Tunggu 1 detik
}
