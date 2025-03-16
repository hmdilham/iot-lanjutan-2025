#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "ssid";       // Nama jaringan Wi-Fi
const char* password = "ssid-password"; // Password Wi-Fi

void setup() {
  Serial.begin(115200);

  // Mulai koneksi Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nTerhubung ke Wi-Fi!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());

  // Buat HTTP POST request
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://jsonplaceholder.typicode.com/posts"); // URL target
    http.addHeader("Content-Type", "application/json"); // Set header

    // Buat data JSON
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["title"] = "Hello ESP32";
    jsonDoc["body"] = "Ini adalah data dari ESP32";
    jsonDoc["userId"] = 1;
    String jsonString;
    serializeJson(jsonDoc, jsonString);

    int httpCode = http.POST(jsonString); // Kirim request

    if (httpCode > 0) {
      String payload = http.getString(); // Baca respons
      Serial.println("Respons dari server:");
      Serial.println(payload);
    } else {
      Serial.println("Error pada HTTP request");
    }

    http.end(); // Tutup koneksi
  }
}

void loop() {
  // Kosongkan loop
}