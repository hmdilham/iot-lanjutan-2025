#include <WiFi.h>
#include <WiFiManager.h> // Library Wi-Fi Manager
#include <WebServer.h>

#define TRIGGER_PIN 0 // GPIO0 untuk trigger config mode

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, INPUT_PULLUP); // Tombol untuk trigger config

  // Inisialisasi Wi-Fi Manager
  WiFiManager wm;

  // Uncomment untuk reset settings (testing)
  // wm.resetSettings();

  // Set timeout untuk AP mode (detik)
  wm.setConfigPortalTimeout(120);

  // Custom parameter (opsional)
  // WiFiManagerParameter custom_field("field_id", "Label", "default", 10);
  // wm.addParameter(&custom_field);

  // Jalankan Wi-Fi Manager
  bool res;
  res = wm.autoConnect("ESP32-Config"); // Nama AP mode

  if (!res) {
    Serial.println("Gagal terhubung. Timeout!");
    ESP.restart();
  } else {
    Serial.println("Terhubung ke Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  // Trigger config mode dengan tombol
  if (digitalRead(TRIGGER_PIN) == LOW) {
    WiFiManager wm;
    wm.startConfigPortal("ESP32-Config");
    Serial.println("Masuk mode konfigurasi!");
  }
}