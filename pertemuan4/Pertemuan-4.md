### **Pertemuan 4: Protokol Komunikasi - I2C**

#### **1. Teori I2C (Inter-Integrated Circuit)**
**Materi:**
- **Apa itu I2C?**
  - Protokol komunikasi serial **sinkron** yang menggunakan dua pin: **SDA (Serial Data)** dan **SCL (Serial Clock)**.
  - Mendukung komunikasi **multi-master** dan **multi-slave**.
  - Kecepatan: Standar (100 kbps), Fast Mode (400 kbps), High-Speed (3.4 Mbps).

- **Komponen Frame I2C:**
  1. **Start Condition**: Sinyal LOW pada SDA saat SCL HIGH.
  2. **Address Frame**: 7-bit atau 10-bit alamat slave.
  3. **Read/Write Bit**: Menentukan operasi (0 = write, 1 = read).
  4. **ACK/NACK**: Konfirmasi dari slave/master.
  5. **Data Frame**: 8-bit data.
  6. **Stop Condition**: Sinyal HIGH pada SDA saat SCL HIGH.

- **Alamat I2C:**
  - Setiap slave memiliki alamat unik (0x08 hingga 0x77 untuk 7-bit).
  - Contoh alamat sensor: BMP280 (0x76 atau 0x77), OLED (0x3C).

**Referensi Bacaan:**
- [I2C Communication Guide](https://www.circuitbasics.com/basics-of-the-i2c-communication-protocol/)
- [ESP32 I2C Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html)

---

#### **2. Praktikum 1: Membaca Data dari Sensor I2C (BMP280)**
**Tujuan:**  
Membaca data suhu dan tekanan dari sensor BMP280 menggunakan I2C.

**Komponen yang Dibutuhkan:**
- ESP32/ESP8266
- Sensor BMP280
- Resistor 4.7kΩ (2 buah, pull-up SDA & SCL)
- Breadboard dan kabel jumper

**Rangkaian Elektronika:**
1. Hubungkan **VCC** sensor ke **3.3V** ESP.
2. Hubungkan **GND** sensor ke **GND** ESP.
3. Hubungkan **SDA** sensor ke **SDA** ESP (ESP32: GPIO21, ESP8266: D2).
4. Hubungkan **SCL** sensor ke **SCL** ESP (ESP32: GPIO22, ESP8266: D1).
5. Pasang resistor **4.7kΩ** antara SDA dan 3.3V (pull-up).
6. Pasang resistor **4.7kΩ** antara SCL dan 3.3V (pull-up).

**Skema Rangkaian:**
```
ESP32/ESP8266:
- 3.3V -> VCC BMP280
- GND  -> GND BMP280
- SDA (GPIO21/D2) -> SDA BMP280 -> Resistor 4.7kΩ -> 3.3V
- SCL (GPIO22/D1) -> SCL BMP280 -> Resistor 4.7kΩ -> 3.3V
```

**Kode Program:**
```cpp
#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // Objek sensor BMP280

void setup() {
  Serial.begin(115200);
  if (!bmp.begin(0x76)) { // Alamat BMP280: 0x76 atau 0x77
    Serial.println("Sensor tidak ditemukan!");
    while (1);
  }
}

void loop() {
  Serial.print("Suhu: ");
  Serial.print(bmp.readTemperature()); // Baca suhu (°C)
  Serial.print(" °C | Tekanan: ");
  Serial.print(bmp.readPressure() / 100.0F); // Baca tekanan (hPa)
  Serial.println(" hPa");
  delay(2000);
}
```

**Instalasi Library di PlatformIO:**
- Tambahkan library berikut di `platformio.ini`:
  ```ini
  lib_deps = 
    adafruit/Adafruit BMP280 Library@^2.6.6
  ```

---

### **Kode Program Untuk data sensor ke OLED Display**

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Konfigurasi OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 // Reset pin (jika tidak digunakan, set ke -1)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Konfigurasi BMP280
Adafruit_BMP280 bmp;

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Inisialisasi OLED
  if (!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println("OLED tidak ditemukan!");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("OLED siap!");
  display.display();
  delay(2000);

  // Inisialisasi BMP280
  if (!bmp.begin(0x76)) { // Alamat BMP280: 0x76 atau 0x77
    Serial.println("Sensor BMP280 tidak ditemukan!");
    while (1);
  }
}

void loop() {
  // Baca data dari BMP280
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;

  // Tampilkan data di Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" °C | Tekanan: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // Tampilkan data di OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Data Sensor BMP280:");
  display.print("Suhu: ");
  display.print(temperature);
  display.println(" C");
  display.print("Tekanan: ");
  display.print(pressure);
  display.println(" hPa");
  display.display();

  delay(2000); // Tunggu 2 detik
}
```

---

### **Penjelasan Kode Program**

1. **Library yang Digunakan:**
   - `Wire.h`: Untuk komunikasi I2C.
   - `Adafruit_BMP280.h`: Untuk membaca data dari sensor BMP280.
   - `Adafruit_GFX.h` dan `Adafruit_SSD1306.h`: Untuk mengontrol OLED display.

2. **Inisialisasi OLED:**
   - Menggunakan library `Adafruit_SSD1306` untuk mengontrol OLED.
   - Jika OLED tidak terdeteksi, program akan berhenti.

3. **Inisialisasi BMP280:**
   - Menggunakan library `Adafruit_BMP280` untuk membaca suhu dan tekanan.
   - Jika sensor tidak terdeteksi, program akan berhenti.

4. **Menampilkan Data di OLED:**
   - Data suhu dan tekanan ditampilkan di OLED dengan format yang rapi.

5. **Menampilkan Data di Serial Monitor:**
   - Data suhu dan tekanan juga ditampilkan di Serial Monitor untuk debugging.

---

### **Rangkaian Elektronika**

#### **Komponen yang Dibutuhkan:**
- ESP32
- Sensor BMP280
- OLED SSD1306 128x64 (I2C)
- Resistor 4.7kΩ (2 buah, pull-up SDA & SCL)
- Breadboard dan kabel jumper

#### **Skema Rangkaian:**
```
ESP32:
- 3.3V -> VCC BMP280 -> VCC OLED
- GND  -> GND BMP280 -> GND OLED
- SDA (GPIO21) -> SDA BMP280 -> SDA OLED -> Resistor 4.7kΩ -> 3.3V
- SCL (GPIO22) -> SCL BMP280 -> SCL OLED -> Resistor 4.7kΩ -> 3.3V
```

---

### **Instalasi Library di PlatformIO**

Tambahkan library berikut di `platformio.ini`:
```ini
lib_deps = 
  adafruit/Adafruit BMP280 Library@^2.6.6
  adafruit/Adafruit SSD1306@^2.5.7
  adafruit/Adafruit GFX Library@^1.11.3
```

---

### **Tampilan di OLED**

OLED akan menampilkan:
```
Data Sensor BMP280:
Suhu: 25.3 C
Tekanan: 1013.2 hPa
```

---

#### **3. Praktikum 2: Komunikasi I2C Antara Dua ESP32/ESP8266**
**Tujuan:**  
Mengirim data antara dua ESP32/ESP8266 menggunakan I2C (satu sebagai master, satu sebagai slave).

**Komponen yang Dibutuhkan:**
- 2x ESP32/ESP8266
- Resistor 4.7kΩ (2 buah)
- Breadboard dan kabel jumper

**Rangkaian Elektronika:**
1. Hubungkan **SDA Master** ke **SDA Slave**.
2. Hubungkan **SCL Master** ke **SCL Slave**.
3. Hubungkan **GND** Master ke **GND** Slave.
4. Pasang resistor **4.7kΩ** antara SDA dan 3.3V (pull-up).
5. Pasang resistor **4.7kΩ** antara SCL dan 3.3V (pull-up).

**Skema Rangkaian:**
```
Master (ESP32)          Slave (ESP32)
SDA (GPIO21) ----------- SDA (GPIO21)
SCL (GPIO22) ----------- SCL (GPIO22)
GND -------------------- GND
```

**Kode Program:**
**Master (Mengirim Data):**
```cpp
#include <Wire.h>

#define SLAVE_ADDR 0x08 // Alamat slave

void setup() {
  Wire.begin(); // Inisialisasi I2C sebagai master
  Serial.begin(115200);
}

void loop() {
  Wire.beginTransmission(SLAVE_ADDR); // Mulai transmisi ke slave
  Wire.write("Hello Slave!");         // Kirim data
  Wire.endTransmission();             // Akhiri transmisi
  Serial.println("Data terkirim!");
  delay(1000);
}
```

**Slave (Menerima Data):**
```cpp
#include <Wire.h>

#define SLAVE_ADDR 0x08 // Alamat slave

void receiveData(int byteCount) {
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}

void setup() {
  Wire.begin(SLAVE_ADDR);   // Inisialisasi I2C sebagai slave
  Wire.onReceive(receiveData); // Callback saat data diterima
  Serial.begin(115200);
}

void loop() {
  // Kosongkan loop, data diproses via callback
}
```

---

#### **4. Troubleshooting I2C**
**Masalah Umum dan Solusi:**
1. **Sensor Tidak Terdeteksi**:
   - Periksa alamat I2C dengan kode scanner:
     ```cpp
     #include <Wire.h>
     void setup() {
       Serial.begin(115200);
       Wire.begin();
     }
     void loop() {
       byte error, address;
       for (address = 1; address < 127; address++) {
         Wire.beginTransmission(address);
         error = Wire.endTransmission();
         if (error == 0) {
           Serial.print("Device ditemukan di alamat 0x");
           Serial.println(address, HEX);
         }
       }
       delay(5000);
     }
     ```
   - Pastikan kabel SDA/SCL terhubung dengan benar.
2. **Data Tidak Valid**:
   - Tambahkan resistor pull-up 4.7kΩ pada SDA dan SCL.
   - Periksa sumber daya (pastikan sensor menggunakan 3.3V).

---

### **Tugas Praktikum**
1. **Tugas 1**:  
   Gunakan sensor I2C lain (contoh: OLED SSD1306) untuk menampilkan data suhu dari BMP280.
2. **Tugas 2**:  
   Buat komunikasi dua arah antara master dan slave, di mana slave mengirim balasan ke master.

---

### **Rangkuman Pertemuan 4**
1. **Teori**:
   - Memahami konsep I2C, alamat slave, dan frame data.
2. **Praktik**:
   - Membaca data dari sensor BMP280.
   - Komunikasi I2C antar-ESP32/ESP8266.
3. **Rangkaian**:
   - Skema koneksi sensor dan pull-up resistor.
   - Koneksi master-slave I2C.
4. **Troubleshooting**:
   - Scanner I2C dan penanganan error.

---

### **Referensi Tambahan**
- [Adafruit BMP280 Library](https://github.com/adafruit/Adafruit_BMP280_Library)
- [ESP32 I2C Example](https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/)
- [I2C Scanner Code](https://playground.arduino.cc/Main/I2cScanner/)

---