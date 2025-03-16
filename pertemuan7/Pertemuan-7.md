### **Pertemuan 7: Web Server pada ESP32/ESP8266**

#### **1. Teori Web Server**
**Materi:**
- **Apa itu Web Server?**
  - Sebuah server yang menyediakan halaman web yang dapat diakses melalui browser.
  - ESP32/ESP8266 dapat berfungsi sebagai web server untuk menampilkan data atau mengontrol perangkat.

- **Komponen Web Server:**
  1. **HTTP Request**: Permintaan dari client (browser) ke server.
  2. **HTTP Response**: Tanggapan dari server ke client.
  3. **HTML (Hypertext Markup Language)**: Bahasa untuk membuat halaman web.

- **Library yang Digunakan:**
  - `WiFi.h`: Untuk mengelola koneksi Wi-Fi.
  - `WebServer.h`: Untuk membuat web server (ESP32).
  - `ESP8266WebServer.h`: Untuk membuat web server (ESP8266).
  - `DHT.h`: Untuk membaca data dari sensor DHT11/DHT22.

**Referensi Bacaan:**
- [ESP32 Web Server Guide](https://randomnerdtutorials.com/esp32-web-server-arduino-ide/)
- [ESP8266 Web Server Guide](https://randomnerdtutorials.com/esp8266-web-server-with-arduino-ide/)

---

#### **2. Praktikum 1: Membuat Web Server Sederhana**
**Tujuan:**  
Membuat web server sederhana untuk menampilkan teks "Hello, World!".

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
#include <WebServer.h> // Untuk ESP32
// #include <ESP8266WebServer.h> // Untuk ESP8266

const char* ssid = "NAMA_WIFI";       // Nama jaringan Wi-Fi
const char* password = "PASSWORD_WIFI"; // Password Wi-Fi

WebServer server(80); // Buat objek web server di port 80

void handleRoot() {
  server.send(200, "text/plain", "Hello, World!"); // Kirim respons ke client
}

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

  // Atur routing web server
  server.on("/", handleRoot); // Tentukan fungsi untuk root URL
  server.begin(); // Mulai web server
  Serial.println("Web server berjalan!");
}

void loop() {
  server.handleClient(); // Tangani request dari client
}
```

**Penjelasan:**
- `WebServer server(80)`: Membuat objek web server di port 80.
- `server.on("/", handleRoot)`: Menentukan fungsi yang akan dipanggil saat root URL diakses.
- `server.send(200, "text/plain", "Hello, World!")`: Mengirim respons ke client.
- `server.handleClient()`: Menangani request dari client.

---

#### **2: Membaca Data dari Sensor DHT11/DHT22**
**Tujuan:**  
Membaca data suhu dan kelembaban dari sensor DHT11/DHT22.

**Komponen yang Dibutuhkan:**
- ESP32/ESP8266
- Sensor DHT11/DHT22
- Resistor 10kΩ (untuk DHT11)
- Breadboard dan kabel jumper

**Rangkaian Elektronika:**
1. Hubungkan **VCC** sensor ke **3.3V** ESP.
2. Hubungkan **GND** sensor ke **GND** ESP.
3. Hubungkan **DATA** sensor ke pin GPIO4 (ESP32) atau D4 (ESP8266).
4. Pasang resistor **10kΩ** antara pin DATA dan VCC (hanya untuk DHT11).

**Skema Rangkaian:**
```
ESP32/ESP8266:
- 3.3V -> VCC DHT
- GND  -> GND DHT
- GPIO4/D4 -> DATA DHT
- Resistor 10kΩ antara DATA dan VCC (untuk DHT11)
```

**Kode Program:**
```cpp
#include <DHT.h>

#define DHTPIN 4      // Pin DATA DHT (GPIO4/D4)
#define DHTTYPE DHT22 // Tipe sensor (DHT11 atau DHT22)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature(); // Baca suhu (°C)
  float humidity = dht.readHumidity();       // Baca kelembaban (%)

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca data dari sensor DHT!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" °C | Kelembaban: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000); // Tunggu 2 detik
}
```

**Instalasi Library di PlatformIO:**
- Tambahkan library berikut di `platformio.ini`:
  ```ini
  lib_deps = 
    adafruit/DHT sensor library@^1.4.4
  ```

---

#### **3. Praktikum 3: Membuat Web Server Sederhana**
**Tujuan:**  
Membuat web server untuk menampilkan data suhu dan kelembaban dari sensor DHT11/DHT22.

**Komponen yang Dibutuhkan:**
- ESP32/ESP8266
- Sensor DHT11/DHT22
- Resistor 10kΩ (untuk DHT11)
- Breadboard dan kabel jumper

**Rangkaian Elektronika:**
- Sama seperti rangkaian pada Praktikum 1.

**Kode Program:**
**Untuk ESP32:**
```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 4      // Pin DATA DHT (GPIO4)
#define DHTTYPE DHT22 // Tipe sensor (DHT11 atau DHT22)

const char* ssid = "NAMA_WIFI";       // Nama jaringan Wi-Fi
const char* password = "PASSWORD_WIFI"; // Password Wi-Fi

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80); // Port 80 untuk HTTP

void handleRoot() {
  float temperature = dht.readTemperature(); // Baca suhu (°C)
  float humidity = dht.readHumidity();       // Baca kelembaban (%)

  if (isnan(temperature) || isnan(humidity)) {
    server.send(500, "text/plain", "Gagal membaca data dari sensor DHT!");
    return;
  }

  // Buat halaman HTML
  String html = "<html><body>";
  html += "<h1>Data Sensor DHT</h1>";
  html += "<p>Suhu: " + String(temperature) + " °C</p>";
  html += "<p>Kelembaban: " + String(humidity) + " %</p>";
  html += "</body></html>";

  server.send(200, "text/html", html); // Kirim respons ke client
}

void setup() {
  Serial.begin(115200);
  dht.begin();

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

  // Atur routing web server
  server.on("/", handleRoot); // Tampilkan data sensor di root
  server.begin();
  Serial.println("Web server berjalan!");
}

void loop() {
  server.handleClient(); // Tangani request dari client
}
```

**Untuk ESP8266:**
```cpp
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN D4      // Pin DATA DHT (D4)
#define DHTTYPE DHT22 // Tipe sensor (DHT11 atau DHT22)

const char* ssid = "NAMA_WIFI";       // Nama jaringan Wi-Fi
const char* password = "PASSWORD_WIFI"; // Password Wi-Fi

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80); // Port 80 untuk HTTP

void handleRoot() {
  float temperature = dht.readTemperature(); // Baca suhu (°C)
  float humidity = dht.readHumidity();       // Baca kelembaban (%)

  if (isnan(temperature) || isnan(humidity)) {
    server.send(500, "text/plain", "Gagal membaca data dari sensor DHT!");
    return;
  }

  // Buat halaman HTML
  String html = "<html><body>";
  html += "<h1>Data Sensor DHT</h1>";
  html += "<p>Suhu: " + String(temperature) + " °C</p>";
  html += "<p>Kelembaban: " + String(humidity) + " %</p>";
  html += "</body></html>";

  server.send(200, "text/html", html); // Kirim respons ke client
}

void setup() {
  Serial.begin(115200);
  dht.begin();

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

  // Atur routing web server
  server.on("/", handleRoot); // Tampilkan data sensor di root
  server.begin();
  Serial.println("Web server berjalan!");
}

void loop() {
  server.handleClient(); // Tangani request dari client
}
```

---

### **Tugas Praktikum**
1. **Tugas 1:**  
   Tambahkan tombol di halaman web untuk mengontrol LED pada ESP.
2. **Tugas 2:**  
   Buat halaman web yang menampilkan grafik data suhu dan kelembaban menggunakan library Chart.js.
3. **Tugas 3:**
   Asyncrhronous/realtime data

---

### **Rangkuman Pertemuan 7**
1. **Teori:**
   - Memahami konsep web server dan HTTP.
2. **Praktik:**
   - Membaca data dari sensor DHT11/DHT22.
   - Membuat web server untuk menampilkan data sensor.
3. **Rangkaian:**
   - Skema koneksi sensor DHT11/DHT22.
4. **Troubleshooting:**
   - Pastikan sensor DHT terhubung dengan benar.
   - Periksa koneksi Wi-Fi.

---

### **Referensi Tambahan**
- [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
- [ESP32 Web Server Example](https://randomnerdtutorials.com/esp32-web-server-arduino-ide/)
- [ESP8266 Web Server Example](https://randomnerdtutorials.com/esp8266-web-server-with-arduino-ide/)