### **Pertemuan 6: Wi-Fi dan HTTP Client**

#### **1. Teori Wi-Fi dan HTTP**
**Materi:**
- **Wi-Fi pada ESP32/ESP8266:**
  - ESP32 dan ESP8266 memiliki modul Wi-Fi terintegrasi yang mendukung mode **Station (STA)** dan **Access Point (AP)**.
  - **Station (STA)**: ESP terhubung ke jaringan Wi-Fi yang sudah ada.
  - **Access Point (AP)**: ESP membuat jaringan Wi-Fi sendiri.

- **HTTP (Hypertext Transfer Protocol):**
  - Protokol untuk mengirim dan menerima data antara client (ESP) dan server.
  - **HTTP GET**: Meminta data dari server.
  - **HTTP POST**: Mengirim data ke server.

- **Library yang Digunakan:**
  - `WiFi.h`: Untuk mengelola koneksi Wi-Fi.
  - `HTTPClient.h`: Untuk membuat HTTP request.

**Referensi Bacaan:**
- [ESP32 Wi-Fi Guide](https://randomnerdtutorials.com/esp32-wi-fi/)
- [ESP8266 Wi-Fi Guide](https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/)

---

#### **2. Praktikum 1: Menghubungkan ESP32/ESP8266 ke Jaringan Wi-Fi**
**Tujuan:**  
Menghubungkan ESP32/ESP8266 ke jaringan Wi-Fi dan menampilkan status koneksi.

**Komponen yang Dibutuhkan:**
- ESP32/ESP8266
- Kabel USB
- Komputer

**Rangkaian Elektronika:**
- Tidak diperlukan komponen tambahan selain ESP32/ESP8266 dan kabel USB.

**Kode Program:**
```cpp
#include <WiFi.h> // Untuk ESP32
// #include <ESP8266WiFi.h> // Untuk ESP8266

const char* ssid = "NAMA_WIFI";       // Nama jaringan Wi-Fi
const char* password = "PASSWORD_WIFI"; // Password Wi-Fi

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
}

void loop() {
  // Kosongkan loop, koneksi Wi-Fi sudah terjaga
}
```

**Penjelasan:**
- `WiFi.begin(ssid, password)`: Memulai koneksi ke jaringan Wi-Fi.
- `WiFi.status()`: Mengecek status koneksi Wi-Fi.
- `WiFi.localIP()`: Mendapatkan alamat IP lokal ESP.

---

#### **3. Praktikum 2: Membuat HTTP GET Request**
**Tujuan:**  
Mengirim HTTP GET request ke server dan menampilkan responsnya.

**Komponen yang Dibutuhkan:**
- ESP32/ESP8266
- Kabel USB
- Komputer

**Rangkaian Elektronika:**
- Tidak diperlukan komponen tambahan selain ESP32/ESP8266 dan kabel USB.

**Kode Program:**
```cpp
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "NAMA_WIFI";       // Nama jaringan Wi-Fi
const char* password = "PASSWORD_WIFI"; // Password Wi-Fi

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

  // Buat HTTP GET request
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://jsonplaceholder.typicode.com/posts/1"); // URL target
    int httpCode = http.GET(); // Kirim request

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
```

**Penjelasan:**
- `HTTPClient http`: Objek untuk membuat HTTP request.
- `http.begin(url)`: Membuka koneksi ke URL target.
- `http.GET()`: Mengirim HTTP GET request.
- `http.getString()`: Membaca respons dari server.

---

#### **4. Praktikum 3: Membuat HTTP POST Request**
**Tujuan:**  
Mengirim HTTP POST request ke server dengan data JSON.

**Komponen yang Dibutuhkan:**
- ESP32/ESP8266
- Kabel USB
- Komputer

**Rangkaian Elektronika:**
- Tidak diperlukan komponen tambahan selain ESP32/ESP8266 dan kabel USB.

**Kode Program:**
```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "NAMA_WIFI";       // Nama jaringan Wi-Fi
const char* password = "PASSWORD_WIFI"; // Password Wi-Fi

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
```

**Penjelasan:**
- `http.addHeader("Content-Type", "application/json")`: Menambahkan header untuk data JSON.
- `StaticJsonDocument<200> jsonDoc`: Membuat objek JSON.
- `http.POST(jsonString)`: Mengirim HTTP POST request dengan data JSON.

---

### **Tugas Praktikum**
1. **Tugas 1:**  
   Modifikasi kode untuk mengirim data suhu dari BMP280 ke server menggunakan HTTP POST.
2. **Tugas 2:**  
   Buat sistem yang mengirim data sensor ke server setiap 10 detik.

---

### **Rangkuman Pertemuan 6**
1. **Teori:**
   - Memahami konsep Wi-Fi dan HTTP.
2. **Praktik:**
   - Menghubungkan ESP32/ESP8266 ke Wi-Fi.
   - Membuat HTTP GET dan POST request.
3. **Rangkaian:**
   - Tidak diperlukan rangkaian tambahan selain ESP32/ESP8266.
4. **Troubleshooting:**
   - Pastikan SSID dan password Wi-Fi benar.
   - Periksa koneksi internet.

---

### **Referensi Tambahan**
- [ESP32 HTTP GET/POST Example](https://techtutorialsx.com/2017/12/17/esp32-arduino-http-get-request/)
- [ArduinoJson Library](https://arduinojson.org/)