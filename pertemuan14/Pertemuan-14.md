### **Pertemuan 14: Over-The-Air (OTA) Update dengan ElegantOTA**

#### **Tujuan Pembelajaran**
- Memahami konsep Over-The-Air (OTA) update pada ESP32.
- Mengenal library **ElegantOTA** beserta arsitektur kerjanya.
- Mampu mengkonfigurasi PlatformIO untuk mendukung ElegantOTA.
- Mampu mengupload firmware baru ke ESP32 melalui jaringan WiFi (tanpa kabel USB).
- Mempraktikkan skenario sederhana: mengubah pin LED dari `GPIO 2` ke `GPIO 4` melalui OTA.

---

#### **1. Pengenalan OTA (Over-The-Air Update)**

**Apa itu OTA?**
- OTA adalah mekanisme pembaruan firmware (program) microcontroller **melalui jaringan nirkabel** (WiFi/Bluetooth) tanpa perlu mencabut perangkat dan menyambungkannya kembali ke komputer melalui kabel USB.
- ESP32/ESP8266 menyediakan partisi khusus (`ota_0`, `ota_1`) di flash memory yang memungkinkan firmware lama dan baru hidup berdampingan saat proses update.

**Alur Dasar OTA pada ESP32**:
```
[Firmware Lama Berjalan di ota_0]
            ↓
   Terima file .bin via WiFi
            ↓
[Firmware Baru ditulis ke ota_1]
            ↓
   Verifikasi checksum / signature
            ↓
   Update otadata → boot dari ota_1
            ↓
        ESP32 Restart
            ↓
[Firmware Baru Berjalan di ota_1]
```

**Tipe OTA yang Umum di ESP32**:
| Tipe | Mekanisme | Kelebihan | Kekurangan |
|---|---|---|---|
| ArduinoOTA | Push dari Arduino IDE/PlatformIO via mDNS | Cepat, terintegrasi IDE | Hanya dari komputer di jaringan yang sama |
| HTTP OTA | Pull `.bin` dari URL | Cocok untuk fleet besar | Perlu server hosting |
| Web OTA (ElegantOTA, AsyncElegantOTA) | Upload `.bin` via halaman web | UI mudah, tidak perlu IDE | Perlu manual upload |

---

#### **2. Mengapa OTA Penting untuk IoT?**

- **Perangkat Tersembunyi**: Sensor IoT sering dipasang di plafon, dinding, atau lokasi sulit dijangkau — mencabutnya untuk update adalah pekerjaan besar.
- **Skala Banyak Perangkat**: Memperbarui 100 ESP32 satu per satu dengan kabel USB tidak realistis.
- **Iterasi Cepat**: Saat development, OTA mempersingkat siklus *edit → compile → flash → test*.
- **Patch Bug Kritis**: Bug keamanan atau bug fungsional bisa diperbaiki tanpa kunjungan fisik ke lokasi perangkat.

---

#### **3. Pengenalan ElegantOTA**

**Apa itu ElegantOTA?**
- Library OTA berbasis web yang menyediakan halaman HTML modern untuk mengupload firmware (`.bin`) atau filesystem image (`.bin`) ke ESP32/ESP8266.
- Dikembangkan oleh **Ayush Sharma** ([github.com/ayushsharma82/ElegantOTA](https://github.com/ayushsharma82/ElegantOTA)).
- Versi 3.x mendukung dua mode web server: **AsyncWebServer** (default) dan **WebServer** sinkron (dengan build flag).

**Fitur Utama**:
- UI bersih dengan progress bar.
- Mendukung upload **firmware** maupun **filesystem image** (LittleFS/SPIFFS).
- Mendukung autentikasi (username + password).
- Callback `onStart`, `onProgress`, `onEnd` untuk logging custom.
- Reboot otomatis setelah upload sukses.

**Tampilan Halaman**:
```
┌─────────────────────────────────────┐
│   ESP32 OTA - ElegantOTA v3         │
├─────────────────────────────────────┤
│   ⬡  Firmware                       │
│   ⬡  Filesystem                     │
│                                     │
│   [Pilih file .bin]   [Update]      │
│   ▓▓▓▓▓▓▓░░░░░░░░  60%              │
└─────────────────────────────────────┘
```

**Perbandingan Library OTA**:
| Library | Web UI | Async Support | Ukuran | Mudah Digunakan |
|---|---|---|---|---|
| ArduinoOTA | Tidak | - | Kecil | Sedang |
| HTTPUpdate | Tidak (manual) | Tidak | Kecil | Sedang |
| ElegantOTA | Ya (modern) | Ya | Sedang | Sangat mudah |
| AsyncElegantOTA (lama) | Ya | Ya | Sedang | Mudah (deprecated) |

---

#### **4. Konfigurasi PlatformIO untuk ElegantOTA**

**Konfigurasi `platformio.ini`** (mode WebServer sinkron — paling sederhana):
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps =
  ayushsharma82/ElegantOTA@^3.1.5
build_flags =
  -D ELEGANTOTA_USE_ASYNC_WEBSERVER=0
```

**Konfigurasi `platformio.ini`** (mode AsyncWebServer — direkomendasikan untuk request berat):
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps =
  ayushsharma82/ElegantOTA@^3.1.5
  ayushsharma82/ESPAsyncWebServer@^3.1.0
  esp32async/AsyncTCP@^3.2.0
build_flags =
  -D ELEGANTOTA_USE_ASYNC_WEBSERVER=1
```

> **Catatan**: Build flag `ELEGANTOTA_USE_ASYNC_WEBSERVER` **wajib** ditentukan secara eksplisit. Jika tidak diset, library akan menggunakan default (async = 1) — dan jika dependensi async tidak ada, kompilasi akan gagal.

**Partisi Default Sudah Mendukung OTA**:
- Tidak perlu custom `partitions.csv` untuk OTA — partisi bawaan ESP32 (`default.csv`) sudah memiliki dua slot aplikasi (`app0` dan `app1`).
- Jika sebelumnya menggunakan partisi LittleFS custom (Pertemuan 13), pastikan masih ada minimal dua slot `app` agar OTA bisa berjalan.

**Contoh Partisi yang Mendukung OTA + LittleFS**:
```csv
# Name,    Type, SubType, Offset,  Size,    Flags
nvs,       data, nvs,     0x9000,  0x5000,
otadata,   data, ota,     0xe000,  0x2000,
app0,      app,  ota_0,   0x10000, 0x180000,
app1,      app,  ota_1,   0x190000,0x180000,
spiffs,    data, spiffs,  0x310000,0xE0000,
```

---

#### **5. Kode Program: Blink LED + ElegantOTA**

**Skenario Awal**:
- ESP32 mengedipkan LED di pin **GPIO 2** setiap 1 detik.
- ESP32 menjalankan web server pada port 80.
- Halaman `/update` menyediakan UI ElegantOTA untuk upload firmware baru.

**File: `src/main.cpp`** (versi awal — LED di GPIO 2)
```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ElegantOTA.h>

// === Konfigurasi WiFi ===
const char* WIFI_SSID = "NAMA_WIFI";
const char* WIFI_PASS = "PASSWORD_WIFI";

// === Pin LED (akan diubah via OTA) ===
#define LED_PIN 2   // <-- nantinya akan diubah ke GPIO 4

// === Web Server ===
WebServer server(80);

// === Variabel timing non-blocking ===
unsigned long lastBlink = 0;
const unsigned long BLINK_INTERVAL = 1000;
bool ledState = false;

// === Variabel untuk callback OTA ===
unsigned long otaStartTime = 0;

// Halaman utama
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>ESP32 OTA Demo</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:sans-serif;text-align:center;margin-top:40px}";
  html += "a.btn{background:#1976d2;color:#fff;padding:10px 20px;";
  html += "text-decoration:none;border-radius:5px}</style></head><body>";
  html += "<h1>ESP32 OTA Demo</h1>";
  html += "<p>LED aktif di pin <b>GPIO " + String(LED_PIN) + "</b></p>";
  html += "<p>IP Address: " + WiFi.localIP().toString() + "</p>";
  html += "<p>Uptime: " + String(millis() / 1000) + " detik</p>";
  html += "<a class='btn' href='/update'>Buka Halaman OTA</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Callback ElegantOTA
void onOTAStart() {
  Serial.println("\n[OTA] Update dimulai...");
  otaStartTime = millis();
}

void onOTAProgress(size_t current, size_t final) {
  if (millis() - otaStartTime > 1000) {
    Serial.printf("[OTA] Progress: %u / %u bytes (%.1f%%)\n",
      current, final, (current * 100.0) / final);
    otaStartTime = millis();
  }
}

void onOTAEnd(bool success) {
  if (success) {
    Serial.println("[OTA] Update SUKSES. ESP32 akan restart...");
  } else {
    Serial.println("[OTA] Update GAGAL!");
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Koneksi WiFi
  Serial.printf("Menghubungkan ke WiFi '%s'", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nTerhubung. IP: %s\n", WiFi.localIP().toString().c_str());

  // Konfigurasi route web server
  server.on("/", handleRoot);

  // Inisialisasi ElegantOTA
  ElegantOTA.begin(&server);
  // Aktifkan autentikasi (opsional tetapi direkomendasikan)
  ElegantOTA.setAuth("admin", "admin123");
  // Pasang callback
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  server.begin();
  Serial.println("Web server aktif. Buka http://" + WiFi.localIP().toString());
  Serial.println("Halaman OTA: http://" + WiFi.localIP().toString() + "/update");
}

void loop() {
  server.handleClient();
  ElegantOTA.loop();

  // Blink LED non-blocking
  if (millis() - lastBlink >= BLINK_INTERVAL) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    lastBlink = millis();
  }
}
```

**Penjelasan Kode**:

1. **`WebServer server(80)`** — Web server sinkron bawaan ESP32, port 80 (HTTP standar).
2. **`ElegantOTA.begin(&server)`** — Mendaftarkan halaman `/update` ke web server.
3. **`ElegantOTA.setAuth(...)`** — Menambahkan basic auth (username + password) agar tidak sembarang orang bisa upload firmware.
4. **`ElegantOTA.loop()`** — Wajib dipanggil di `loop()` untuk menangani proses upload, verifikasi, dan reboot.
5. **Blink Non-Blocking** — Menggunakan `millis()` (bukan `delay()`) supaya web server tetap responsif selama LED berkedip.
6. **`handleRoot()`** — Menampilkan info pin LED yang sedang aktif, sehingga setelah OTA kita bisa langsung cek perubahannya di browser.

---

#### **6. Praktikum: Ubah Pin LED dari GPIO 2 ke GPIO 4 via OTA**

**Tujuan**:
- Mengupload firmware versi 1 (LED di GPIO 2) via kabel USB.
- Mengubah baris `#define LED_PIN 2` menjadi `#define LED_PIN 4`.
- Mengupload firmware versi 2 **tanpa kabel** menggunakan ElegantOTA.
- Memverifikasi LED berpindah ke pin baru.

**Persiapan Hardware**:
- ESP32 DevKit (sembarang varian).
- 2 buah LED + resistor 220Ω → satu di **GPIO 2**, satu di **GPIO 4**.
  - LED di GPIO 2 sudah onboard pada banyak board ESP32 — opsional pakai LED eksternal.
- Breadboard + kabel jumper.

**Wiring**:
```
GPIO 2 ──[220Ω]──|>|── GND   (LED 1)
GPIO 4 ──[220Ω]──|>|── GND   (LED 2)
```

---

##### **Langkah 1 — Upload Firmware Versi 1 via USB**

1. Sambungkan ESP32 ke komputer via USB.
2. Edit `WIFI_SSID` dan `WIFI_PASS` di `main.cpp` sesuai jaringan Anda.
3. Pastikan `#define LED_PIN 2`.
4. Klik **Upload** di PlatformIO.
5. Buka Serial Monitor, catat IP address yang muncul. Contoh:
   ```
   Terhubung. IP: 192.168.1.105
   Web server aktif. Buka http://192.168.1.105
   Halaman OTA: http://192.168.1.105/update
   ```
6. Buka `http://192.168.1.105` di browser → halaman menampilkan **"LED aktif di pin GPIO 2"**.
7. Verifikasi LED di GPIO 2 berkedip setiap 1 detik.

---

##### **Langkah 2 — Modifikasi Kode (Tanpa Cabut Kabel atau Tanpa Kabel)**

Edit `src/main.cpp`, ubah baris:
```cpp
#define LED_PIN 2
```
menjadi:
```cpp
#define LED_PIN 4
```

> **Tips**: Bisa juga sambil mengubah `BLINK_INTERVAL` (misal dari `1000` ke `200` ms) supaya perbedaan firmware lama vs baru sangat terlihat.

---

##### **Langkah 3 — Build Firmware Baru (Tanpa Upload USB)**

Di PlatformIO:
- Klik **Build** (ikon centang) — **bukan Upload**.
- Setelah sukses, file `.bin` akan dihasilkan di:
  ```
  .pio/build/esp32dev/firmware.bin
  ```
- Catat path ini, akan diunggah lewat browser di langkah berikutnya.

---

##### **Langkah 4 — Upload Firmware via ElegantOTA**

1. Buka browser → kunjungi `http://<IP_ESP32>/update`.
   Contoh: `http://192.168.1.105/update`.
2. Browser akan meminta **username dan password** → masukkan `admin` / `admin123` (sesuai `setAuth`).
3. Pilih mode **OTA Mode: Firmware**.
4. Klik **Choose File** → pilih `.pio/build/esp32dev/firmware.bin`.
5. Klik **Update** → tunggu progress bar mencapai 100%.
6. Pesan **"OTA Update Finished!"** akan muncul → ESP32 otomatis restart.

**Output Serial Monitor saat OTA**:
```
[OTA] Update dimulai...
[OTA] Progress: 32768 / 953664 bytes (3.4%)
[OTA] Progress: 65536 / 953664 bytes (6.9%)
...
[OTA] Progress: 953664 / 953664 bytes (100.0%)
[OTA] Update SUKSES. ESP32 akan restart...

ets Jul 29 2019 12:21:46
rst:0xc (SW_CPU_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
...
Terhubung. IP: 192.168.1.105
```

---

##### **Langkah 5 — Verifikasi Hasil OTA**

1. Refresh halaman `http://192.168.1.105` di browser.
2. Sekarang halaman menampilkan **"LED aktif di pin GPIO 4"**.
3. **LED di GPIO 2 BERHENTI berkedip**.
4. **LED di GPIO 4 MULAI berkedip** setiap 1 detik.

**Yang Berhasil Dibuktikan**:
- ESP32 menerima firmware baru hanya melalui WiFi — tanpa kabel USB.
- Perubahan kode (pin LED) langsung tercermin di hardware setelah reboot.
- Proses ini bisa diulang berulang kali tanpa pernah menyentuh ESP32 secara fisik.

---

##### **Langkah 6 — Eksperimen Lanjutan (Opsional)**

Coba ubah hal-hal berikut, build, lalu upload via OTA:

| Eksperimen | Perubahan Kode | Hasil yang Diharapkan |
|---|---|---|
| Pin LED ke GPIO 5 | `#define LED_PIN 5` | LED pindah ke GPIO 5 |
| LED kedap-kedip cepat | `BLINK_INTERVAL = 200` | Kedipan jadi 5x/detik |
| Tambah endpoint `/info` | `server.on("/info", ...)` | Endpoint baru muncul tanpa flash USB |
| Ubah username OTA | `ElegantOTA.setAuth("user2", "pass2")` | Login lama ditolak, login baru diterima |

---

#### **7. Workflow OTA dalam Tim Pengembangan**

```mermaid
graph LR
    A[Developer Edit Kode] --> B[Build .bin di PlatformIO]
    B --> C{Upload Method}
    C -->|Initial Flash| D[USB Cable Upload]
    C -->|Subsequent Updates| E[ElegantOTA Web UI]
    E --> F[ESP32 Verifikasi & Reboot]
    F --> G[Firmware Baru Aktif]
    D --> G
```

**Best Practice**:
1. **Selalu nyalakan autentikasi** (`ElegantOTA.setAuth(...)`) di production.
2. **Versi firmware**: tampilkan versi build di halaman utama (mis. `__DATE__` dan `__TIME__`) supaya tahu versi mana yang sedang berjalan.
3. **Backup firmware lama**: simpan `.bin` lama sebelum upload baru, sebagai fallback jika ada bug.
4. **Stress test**: jangan upload OTA saat ESP32 sedang menjalankan task kritis (misal: kontrol motor) — bisa menyebabkan glitch.

---

#### **8. Pertimbangan Keamanan**

| Risiko | Dampak | Mitigasi |
|---|---|---|
| Tanpa autentikasi | Siapapun di jaringan bisa upload firmware jahat | `ElegantOTA.setAuth(user, pass)` wajib di production |
| Password lemah | Brute force mudah | Gunakan password panjang (>12 karakter) |
| HTTP plaintext | Password & firmware bisa di-sniff | Gunakan HTTPS atau OTA hanya di jaringan internal terpercaya |
| Tidak ada signature | Firmware palsu bisa diterima | Gunakan signed OTA (advanced — `Update.installSignature()`) |
| Brick saat OTA gagal | ESP32 mati total | Mekanisme rollback otomatis sudah ada di partisi `otadata` |

---

#### **9. Troubleshooting**

| Masalah | Kemungkinan Penyebab | Solusi |
|---|---|---|
| `Update Failed: Not Enough Space` | Partisi `app1` lebih kecil dari ukuran firmware baru | Gunakan partisi default OTA (2x ~1.6 MB) atau perbesar partisi `app1` |
| Halaman `/update` tidak muncul | `ElegantOTA.begin()` belum dipanggil sebelum `server.begin()` | Pindahkan `ElegantOTA.begin()` sebelum `server.begin()` |
| Compile error: `'AsyncWebServer' was not declared` | Build flag async/sync tidak konsisten dengan header yang di-include | Cocokkan `ELEGANTOTA_USE_ASYNC_WEBSERVER` dengan header yang di-include |
| Browser minta password berulang kali | Browser cache stale | Logout/clear cookies, atau coba mode incognito |
| OTA berhenti di tengah jalan | WiFi lemah / packet loss | Dekatkan ESP32 ke router, atau gunakan kabel USB untuk firmware besar |
| ESP32 restart loop setelah OTA | Firmware baru bug fatal di `setup()` | Flash ulang via USB; pertimbangkan rollback otomatis |
| `loop()` macet selama OTA | `delay()` panjang menghalangi `ElegantOTA.loop()` | Ganti `delay()` dengan timer non-blocking (`millis()`) |
| LED tidak berkedip setelah OTA | Pin GPIO baru tidak valid (mis. pin strapping) | Hindari GPIO 0, 2, 12, 15 untuk output sembarang — gunakan GPIO 4, 5, 16, 17, dll. |

**GPIO yang Aman untuk Output Digital di ESP32**:
- Aman: GPIO 4, 5, 13, 14, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33
- Hati-hati (strapping pin): GPIO 0, 2, 12, 15
- Hindari (input only): GPIO 34, 35, 36, 39

---

#### **10. Implementasi Praktis**

**Komponen yang Dibutuhkan**:
- ESP32 DevKit (4 MB flash atau lebih)
- Kabel USB (hanya untuk upload pertama)
- 2 buah LED + resistor 220Ω
- Breadboard + jumper
- Akses WiFi yang sama antara komputer developer dan ESP32

**Yang Akan Dikerjakan**:
- [x] Upload firmware versi 1 dengan LED di GPIO 2 via USB
- [x] Edit kode: ubah `LED_PIN` ke GPIO 4
- [x] Build firmware versi 2 (tanpa upload USB)
- [x] Upload firmware versi 2 via ElegantOTA web UI
- [x] Verifikasi LED berpindah dari GPIO 2 ke GPIO 4
- [x] Eksperimen tambahan: ubah interval blink, tambah endpoint baru

---

#### **11. Referensi**
- [ElegantOTA GitHub Repository](https://github.com/ayushsharma82/ElegantOTA)
- [ElegantOTA Documentation](https://docs.elegantota.pro/)
- [ESP32 Arduino OTA - Espressif Docs](https://docs.espressif.com/projects/arduino-esp32/en/latest/ota_web_update.html)
- [ESP32 Partition Tables - Espressif Docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/partition-tables.html)
- [ESPAsyncWebServer Library](https://github.com/ESP32Async/ESPAsyncWebServer)

---
