#include <Arduino.h>

#define TX_PIN 17  // GPIO17 (ESP32)
#define RX_PIN 16  // GPIO16 (ESP32)
#define LED_PIN 2  // GPIO2 (LED built-in ESP32)

void setup() {
  Serial.begin(115200); // UART ke komputer
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); // UART ke perangkat lain
  pinMode(LED_PIN, OUTPUT); // Set pin LED sebagai output
}

void loop() {
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n'); // Baca data
    Serial.print("Received: ");
    Serial.println(data); // Tampilkan di Serial Monitor

    // Jika pesan "tombol ditekan" diterima, nyalakan LED
    if (data == "tombol ditekan") {
      digitalWrite(LED_PIN, HIGH); // Nyalakan LED
      delay(1000); // Biarkan LED menyala selama 1 detik
      digitalWrite(LED_PIN, LOW); // Matikan LED
    }
  }
}