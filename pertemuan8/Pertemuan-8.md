### **Pertemuan 8: Wi-Fi Manager**
#### **1. Konsep Wi-Fi Manager**
Wi-Fi Manager memungkinkan ESP32/ESP8266 menyimpan kredensial Wi-Fi di memori (EEPROM/Flash) tanpa hardcoding. Fitur utama:
- **Access Point (AP) Mode**: ESP membuat hotspot sementara untuk konfigurasi.
- **Web Interface**: Halaman web untuk input SSID dan password.
- **Auto-reconnect**: Terhubung otomatis ke jaringan setelah konfigurasi.

---

#### **2. Library yang Dibutuhkan**
- **ESP32**: `WiFiManager` oleh tzapu (install via Library Manager).
- **ESP8266**: `ESP8266WiFi` dan `DNSServer`.

---

#### **3. Implementasi Praktis**

##### **Komponen yang Dibutuhkan**:
- ESP32/ESP8266
- Kabel USB
- Tombol reset (opsional, untuk trigger konfigurasi ulang)

##### **Rangkaian Elektronika**:
Tidak diperlukan komponen tambahan selain ESP dan kabel USB. Jika ingin tombol reset:
```
ESP32/ESP8266:
- GPIO0 -> Tombol -> GND (untuk force config mode)
```

---

### **Kode Program Lengkap**
#### **Untuk ESP32**:
```cpp
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
```

#### **Untuk ESP8266**:
```cpp
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#define TRIGGER_PIN D3 // GPIO0 untuk ESP8266

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  WiFiManager wm;
  
  // Uncomment untuk reset settings
  // wm.resetSettings();

  wm.setConfigPortalTimeout(120);

  bool res = wm.autoConnect("ESP8266-Config");

  if (!res) {
    Serial.println("Failed to connect");
    ESP.restart();
  } else {
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  if (digitalRead(TRIGGER_PIN) == LOW) {
    WiFiManager wm;
    wm.startConfigPortal("ESP8266-Config");
  }
}
```

---

### **Penjelasan Kode**:
1. **Inisialisasi**:
   - `WiFiManager wm`: Objek untuk mengelola Wi-Fi.
   - `wm.autoConnect("NamaAP")`: Membuat AP dengan SSID "NamaAP" jika tidak terkoneksi.

2. **Mode Konfigurasi**:
   - Buka `http://192.168.4.1` di browser saat ESP dalam AP mode.
   - Pilih jaringan Wi-Fi dan masukkan password.

3. **Trigger Manual**:
   - Tekan tombol di GPIO0/D3 untuk memaksa masuk mode AP.

4. **Penyimpanan Kredensial**:
   - Setelah konfigurasi, ESP akan otomatis menyimpan SSID/password di flash.

---

### **Demonstrasi Web Interface**
Saat mode AP aktif:
1. Hubungkan ke SSID `ESP32-Config` atau `ESP8266-Config`.
2. Buka browser, akses `192.168.4.1`.
3. Tampilan web:
   ```
   [Wi-Fi Manager]
   Pilih jaringan:
   - Dropdown daftar Wi-Fi
   - Input password
   - Tombol "Save"
   ```

---

### **Integrasi dengan Sensor (Contoh: DHT22)**
Tambahkan kode ini di `loop()` untuk menampilkan data sensor setelah terkoneksi:
```cpp
#include <DHT.h>
DHT dht(D4, DHT22);

void loop() {
  // ... (kode Wi-Fi Manager sebelumnya)

  if (WiFi.status() == WL_CONNECTED) {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    Serial.print("Suhu: "); Serial.print(temp); Serial.println(" °C");
    Serial.print("Kelembaban: "); Serial.print(hum); Serial.println(" %");
    delay(2000);
  }
}
```

---

### **Troubleshooting**
1. **AP Mode Tidak Muncul**:
   - Pastikan tidak ada kredensial tersimpan (gunakan `wm.resetSettings()`).
   - Periksa koneksi tombol trigger.

2. **Gagal Connect ke Wi-Fi**:
   - Pastikan sinyal Wi-Fi cukup kuat.
   - Cek password yang dimasukkan.

3. **IP Address Tidak Muncul**:
   - Restart ESP setelah konfigurasi.

---

### **Aplikasi dalam Proyek IoT**
1. **Smart Home**:
   - Device bisa dipindahkan ke jaringan baru tanpa reprogramming.
2. **Sensor Wireless**:
   - Deploy sensor di lapangan dengan konfigurasi lapangan.

---

### **Referensi**
- [WiFiManager Library Docs](https://github.com/tzapu/WiFiManager)
- [ESP32 Wi-Fi Manager Tutorial](https://randomnerdtutorials.com/wifimanager-with-esp8266-autoconnect-custom-parameter-and-manage-your-ssid-and-password/)
