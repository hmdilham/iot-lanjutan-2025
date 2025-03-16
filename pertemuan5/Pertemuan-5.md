### **Pertemuan 5: Protokol Komunikasi - SPI**

#### **1. Teori SPI (Serial Peripheral Interface)**
**Materi:**
- **Apa itu SPI?**
  - Protokol komunikasi serial **sinkron** yang menggunakan empat pin utama:
    1. **MOSI (Master Out Slave In)**: Data dikirim dari master ke slave.
    2. **MISO (Master In Slave Out)**: Data dikirim dari slave ke master.
    3. **SCK (Serial Clock)**: Sinyal clock yang dihasilkan oleh master.
    4. **SS/CS (Slave Select/Chip Select)**: Digunakan untuk memilih slave yang aktif.
  - Mendukung komunikasi **full-duplex** (data dapat dikirim dan diterima secara bersamaan).
  - Kecepatan: Biasanya lebih cepat daripada I2C atau UART.

- **Komponen Frame SPI:**
  1. **Clock Polarity (CPOL)**: Menentukan status idle clock (HIGH atau LOW).
  2. **Clock Phase (CPHA)**: Menentukan kapan data di-sample (leading atau trailing edge).
  3. **Data Frame**: 8-bit atau 16-bit data.

- **Keuntungan SPI:**
  - Kecepatan tinggi.
  - Komunikasi full-duplex.
  - Sederhana dan mudah diimplementasikan.

**Referensi Bacaan:**
- [SPI Communication Guide](https://www.circuitbasics.com/basics-of-the-spi-communication-protocol/)
- [ESP32 SPI Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/spi_master.html)

---

#### **2. Praktikum 1: Menggunakan OLED SSD1306 dengan SPI**
**Tujuan:**  
Menampilkan data suhu dan tekanan dari sensor BMP280 pada OLED SSD1306 menggunakan protokol SPI.

**Komponen yang Dibutuhkan:**
- ESP32/ESP8266
- OLED SSD1306 128x64 (SPI)
- Sensor BMP280 (I2C)
- Resistor 4.7kΩ (2 buah, pull-up SDA & SCL)
- Breadboard dan kabel jumper

**Rangkaian Elektronika:**
1. **OLED SSD1306 (SPI):**
   - **VCC** -> 3.3V ESP
   - **GND** -> GND ESP
   - **D0/SCK** -> GPIO18 (ESP32) atau D5 (ESP8266)
   - **D1/MOSI** -> GPIO23 (ESP32) atau D7 (ESP8266)
   - **RES** -> GPIO21 (ESP32) atau D3 (ESP8266)
   - **DC** -> GPIO19 (ESP32) atau D4 (ESP8266)
   - **CS** -> GPIO5 (ESP32) atau D8 (ESP8266)

2. **Sensor BMP280 (I2C):**
   - **VCC** -> 3.3V ESP
   - **GND** -> GND ESP
   - **SDA** -> GPIO21 (ESP32) atau D2 (ESP8266)
   - **SCL** -> GPIO22 (ESP32) atau D1 (ESP8266)

**Skema Rangkaian:**
```
ESP32:
- 3.3V -> VCC OLED -> VCC BMP280
- GND  -> GND OLED -> GND BMP280
- GPIO18 -> SCK OLED
- GPIO23 -> MOSI OLED
- GPIO21 -> RES OLED
- GPIO19 -> DC OLED
- GPIO5  -> CS OLED
- GPIO21 -> SDA BMP280
- GPIO22 -> SCL BMP280
```

**Kode Program:**
```cpp
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Konfigurasi OLED (SPI)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI   23  // GPIO23 (ESP32)
#define OLED_CLK    18  // GPIO18 (ESP32)
#define OLED_DC     19  // GPIO19 (ESP32)
#define OLED_CS     5   // GPIO5 (ESP32)
#define OLED_RESET  21  // GPIO21 (ESP32)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// Konfigurasi BMP280
Adafruit_BMP280 bmp;

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Inisialisasi OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
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

#### **3. Praktikum 2: Komunikasi SPI Antara Dua ESP32/ESP8266**
**Tujuan:**  
Mengirim data antara dua ESP32/ESP8266 menggunakan SPI (satu sebagai master, satu sebagai slave).

**Komponen yang Dibutuhkan:**
- 2x ESP32/ESP8266
- Breadboard dan kabel jumper

**Rangkaian Elektronika:**
1. **Master:**
   - **MOSI** -> MOSI Slave
   - **MISO** -> MISO Slave
   - **SCK** -> SCK Slave
   - **SS** -> SS Slave
   - **GND** -> GND Slave

2. **Slave:**
   - **MOSI** -> MOSI Master
   - **MISO** -> MISO Master
   - **SCK** -> SCK Master
   - **SS** -> SS Master
   - **GND** -> GND Master

**Skema Rangkaian:**
```
Master (ESP32)          Slave (ESP32)
- GPIO23 (MOSI) -------> GPIO23 (MOSI)
- GPIO19 (MISO) -------> GPIO19 (MISO)
- GPIO18 (SCK)  -------> GPIO18 (SCK)
- GPIO5  (SS)   -------> GPIO5  (SS)
- GND           -------> GND
```

**Kode Program:**
**Master (Mengirim Data):**
```cpp
#include <SPI.h>

#define SS_PIN 5  // GPIO5 (ESP32)

void setup() {
  Serial.begin(115200);
  SPI.begin(); // Inisialisasi SPI sebagai master
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH); // Non-aktifkan slave
}

void loop() {
  digitalWrite(SS_PIN, LOW); // Aktifkan slave
  SPI.transfer('H');         // Kirim data 'H'
  SPI.transfer('i');         // Kirim data 'i'
  digitalWrite(SS_PIN, HIGH); // Non-aktifkan slave
  Serial.println("Data terkirim!");
  delay(1000);
}
```

**Slave (Menerima Data):**
```cpp
#include <SPI.h>

#define SS_PIN 5  // GPIO5 (ESP32)

void setup() {
  Serial.begin(115200);
  pinMode(SS_PIN, INPUT);
  SPI.begin(); // Inisialisasi SPI sebagai slave
}

void loop() {
  if (digitalRead(SS_PIN) == LOW) { // Cek jika slave dipilih
    char data1 = SPI.transfer(0);   // Baca data pertama
    char data2 = SPI.transfer(0);   // Baca data kedua
    Serial.print("Received: ");
    Serial.print(data1);
    Serial.println(data2);
  }
}
```

---

### **Tugas Praktikum**
1. **Tugas 1:**  
   Modifikasi kode untuk mengirim data suhu dari BMP280 ke OLED menggunakan SPI.
2. **Tugas 2:**  
   Buat komunikasi dua arah antara master dan slave menggunakan SPI.

---

### **Rangkuman Pertemuan 5**
1. **Teori:**
   - Memahami konsep SPI, pin, dan frame data.
2. **Praktik:**
   - Menggunakan OLED SSD1306 dengan SPI.
   - Komunikasi SPI antar-ESP32/ESP8266.
3. **Rangkaian:**
   - Skema koneksi SPI untuk OLED dan antar-ESP.
4. **Troubleshooting:**
   - Solusi untuk masalah umum SPI.

---

### **Referensi Tambahan**
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
- [ESP32 SPI Example](https://randomnerdtutorials.com/esp32-spi-communication-arduino-ide/)
