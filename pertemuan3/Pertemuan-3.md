### **Pertemuan 3: Protokol Komunikasi - Serial (UART)**

#### **1. Teori UART (Universal Asynchronous Receiver-Transmitter)**
**Materi:**
- **Apa itu UART?**
  - Protokol komunikasi serial asinkron yang menggunakan dua pin: **TX (Transmit)** dan **RX (Receive)**.
  - Digunakan untuk komunikasi antara dua perangkat (contoh: ESP32 dengan komputer, atau dua ESP32).
  - Tidak memerlukan sinyal clock (asinkron), sehingga perlu kesepakatan **baud rate** (kecepatan komunikasi).

- **Komponen Frame UART:**
  1. **Start Bit**: Menandakan awal transmisi data (logika LOW).
  2. **Data Bits**: Data yang dikirim (5-9 bit, umumnya 8 bit).
  3. **Parity Bit**: Untuk pengecekan error (opsional).
  4. **Stop Bit**: Menandakan akhir transmisi (logika HIGH).

- **Baud Rate:**
  - Kecepatan komunikasi (contoh: 9600, 115200 bps).
  - Kedua perangkat harus menggunakan baud rate yang sama.

**Referensi Bacaan:**
- [UART Communication Guide](https://www.circuitbasics.com/basics-uart-communication/)
- [ESP32 UART Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/uart.html)

---

#### **2. Praktikum 1: Komunikasi UART dengan Serial Monitor**
**Tujuan:**  
Mengirim dan menerima data antara ESP32/ESP8266 dan komputer melalui Serial Monitor.

**Komponen yang Dibutuhkan:**
- ESP32/ESP8266
- Kabel USB
- Komputer

**Rangkaian Elektronika:**
- ESP32/ESP8266 terhubung ke komputer via USB (komunikasi UART melalui chip USB-to-Serial onboard).

**Kode Program:**
```cpp
void setup() {
  Serial.begin(115200); // Inisialisasi UART dengan baud rate 115200
}

void loop() {
  Serial.println("Hello from ESP!"); // Kirim data ke Serial Monitor
  delay(1000);

  if (Serial.available()) { // Cek apakah ada data yang diterima
    String data = Serial.readStringUntil('\n'); // Baca data
    Serial.print("Received: ");
    Serial.println(data); // Tampilkan data yang diterima
  }
}
```

**Langkah Pengujian:**
1. Upload kode ke ESP32/ESP8266.
2. Buka Serial Monitor di Platform IO (baud rate: 115200).
3. Kirim pesan dari Serial Monitor ke ESP (contoh: "ON" atau "OFF").
4. Amati balasan dari ESP di Serial Monitor.

---

#### **3. Praktikum 2: Komunikasi UART Antara Dua ESP32/ESP8266**
**Tujuan:**  
Mengirim data antara dua ESP32/ESP8266 menggunakan UART.

**Komponen yang Dibutuhkan:**
- 2x ESP32 atau ESP8266
- Breadboard dan kabel jumper
- Kabel USB (untuk power dan upload program)

**Rangkaian Elektronika:**
1. **Perangkat 1 (Transmitter):**
   - TX -> RX Perangkat 2
   - RX -> TX Perangkat 2
   - GND -> GND Perangkat 2 (untuk common ground).
2. **Perangkat 2 (Receiver):**
   - TX -> RX Perangkat 1
   - RX -> TX Perangkat 1
   - GND -> GND Perangkat 1.

**Skema Rangkaian:**
```
Perangkat 1 (ESP32)       Perangkat 2 (ESP32)
TX (GPIO17) -----------> RX (GPIO16)
RX (GPIO16) -----------> TX (GPIO17)
GND -------------------> GND
```

**Kode Program:**
**Transmitter (Mengirim Data):**
```cpp
#define TX_PIN 17  // GPIO17 (ESP32)
#define RX_PIN 16  // GPIO16 (ESP32)

void setup() {
  Serial.begin(115200); // UART ke komputer
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); // UART ke perangkat lain
}

void loop() {
  Serial1.println("Hello from Transmitter!"); // Kirim data ke perangkat 2
  delay(1000);
}
```

**Receiver (Menerima Data):**
```cpp
#define TX_PIN 17  // GPIO17 (ESP32)
#define RX_PIN 16  // GPIO16 (ESP32)

void setup() {
  Serial.begin(115200); // UART ke komputer
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); // UART ke perangkat lain
}

void loop() {
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n'); // Baca data
    Serial.print("Received: ");
    Serial.println(data); // Tampilkan di Serial Monitor
  }
}
```

**Catatan untuk ESP8266:**
- Gunakan pin `D6 (GPIO12)` untuk TX dan `D7 (GPIO13)` untuk RX.
- Library: `SoftwareSerial` (karena ESP8266 hanya memiliki 1 UART hardware).
  ```cpp
  #include <SoftwareSerial.h>
  SoftwareSerial mySerial(D6, D7); // RX, TX

  void setup() {
    Serial.begin(115200);
    mySerial.begin(115200);
  }

  void loop() {
    mySerial.println("Hello from ESP8266!");
    delay(1000);
  }
  ```

---
**UART with Push Button**
---

### **Kode Program Transmitter (Mengirim Data)**

```cpp
#define TX_PIN 17  // GPIO17 (ESP32)
#define RX_PIN 16  // GPIO16 (ESP32)
#define BUTTON_PIN 4  // GPIO4 (ESP32) untuk push button

void setup() {
  Serial.begin(115200); // UART ke komputer
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); // UART ke perangkat lain
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set pin tombol sebagai input dengan pull-up
}

void loop() {
  // Cek apakah tombol ditekan
  if (digitalRead(BUTTON_PIN) == LOW) { // LOW karena menggunakan pull-up
    Serial1.println("tombol ditekan"); // Kirim pesan ke Receiver
    Serial.println("Tombol ditekan, pesan terkirim!"); // Tampilkan di Serial Monitor
    delay(500); // Debouncing
  }

  delay(100); // Tunggu sebentar sebelum membaca tombol lagi
}
```

---

### **Kode Program Receiver (Menerima Data)**

```cpp
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
```

---

### **Penjelasan Kode Program**

#### **Transmitter:**
1. **Push Button:**
   - Pin GPIO4 digunakan sebagai input untuk push button.
   - Menggunakan `INPUT_PULLUP`, sehingga tombol akan membaca `LOW` saat ditekan.
   - Jika tombol ditekan, pesan "tombol ditekan" dikirim melalui UART (`Serial1`).

2. **Debouncing:**
   - Delay 500 ms setelah tombol ditekan untuk menghindari bouncing (baca ganda).

3. **UART:**
   - Menggunakan `Serial1` untuk komunikasi UART dengan Receiver.

#### **Receiver:**
1. **LED Built-in:**
   - Pin GPIO2 (LED built-in ESP32) digunakan sebagai output.
   - Jika pesan "tombol ditekan" diterima, LED akan menyala selama 1 detik.

2. **UART:**
   - Menggunakan `Serial1` untuk menerima data dari Transmitter.

---

### **Rangkaian Elektronika**

#### **Transmitter:**
1. **Push Button:**
   - Hubungkan salah satu kaki push button ke GPIO4.
   - Hubungkan kaki lainnya ke GND.
   - Tidak perlu resistor eksternal karena menggunakan `INPUT_PULLUP`.

2. **UART:**
   - Hubungkan TX Transmitter (GPIO17) ke RX Receiver (GPIO16).
   - Hubungkan RX Transmitter (GPIO16) ke TX Receiver (GPIO17).
   - Hubungkan GND Transmitter ke GND Receiver.

#### **Receiver:**
1. **LED Built-in:**
   - LED built-in ESP32 sudah terhubung ke GPIO2, tidak perlu rangkaian tambahan.

2. **UART:**
   - Hubungkan TX Receiver (GPIO17) ke RX Transmitter (GPIO16).
   - Hubungkan RX Receiver (GPIO16) ke TX Transmitter (GPIO17).
   - Hubungkan GND Receiver ke GND Transmitter.

---

### **Skema Rangkaian**

```
Transmitter (ESP32)       Receiver (ESP32)
- GPIO17 (TX) -----------> GPIO16 (RX)
- GPIO16 (RX) -----------> GPIO17 (TX)
- GND -------------------> GND

Push Button (Transmitter):
- GPIO4 -> Push Button -> GND

LED Built-in (Receiver):
- GPIO2 (LED built-in) -> Tidak perlu rangkaian tambahan
```

---

### **Cara Kerja**
1. **Transmitter:**
   - Saat tombol ditekan, pesan "tombol ditekan" dikirim ke Receiver melalui UART.
   - Pesan juga ditampilkan di Serial Monitor.

2. **Receiver:**
   - Jika pesan "tombol ditekan" diterima, LED built-in akan menyala selama 1 detik.
   - Pesan yang diterima juga ditampilkan di Serial Monitor.

---

### **Troubleshooting**
1. **Pesan Tidak Terkirim:**
   - Periksa koneksi TX-RX (harus cross: TX ke RX, RX ke TX).
   - Pastikan baud rate sama di kedua perangkat (115200).

2. **LED Tidak Menyala:**
   - Pastikan pesan "tombol ditekan" diterima dengan benar.
   - Periksa koneksi GND antara Transmitter dan Receiver.

3. **Tombol Tidak Berfungsi:**
   - Pastikan tombol terhubung dengan benar (GPIO4 ke tombol ke GND).
   - Gunakan `INPUT_PULLUP` untuk menghindari resistor eksternal.

---

#### **4. Troubleshooting UART**
**Masalah Umum dan Solusi:**
1. **Data Tidak Terbaca:**
   - Pastikan baud rate sama di kedua perangkat.
   - Periksa koneksi TX-RX (harus cross: TX ke RX, RX ke TX).
   - Pastikan kabel GND terhubung.
2. **Error Komunikasi:**
   - Gunakan resistor pull-up jika diperlukan (terutama untuk jarak jauh).
   - Hindari interferensi dengan kabel yang terlalu panjang.
3. **ESP8266 Tidak Bekerja dengan SoftwareSerial:**
   - Pastikan library `SoftwareSerial` terinstal di Platform IO.
   - Gunakan pin GPIO yang mendukung SoftwareSerial (contoh: D6, D7).

---

#### **5. Tugas Praktikum**
1. **Tugas 1:**  
   Modifikasi kode Transmitter dan Receiver untuk mengontrol LED di perangkat Receiver berdasarkan perintah dari Transmitter (contoh: kirim "ON" untuk menyalakan LED).
2. **Tugas 2:**  
   Buat sistem komunikasi dua arah di mana kedua ESP bisa saling mengirim dan menerima data.

---

### **Rangkuman Pertemuan 3**
1. **Teori:**
   - Memahami konsep UART, frame data, dan baud rate.
2. **Praktik:**
   - Komunikasi UART dengan Serial Monitor.
   - Komunikasi UART antara dua ESP32/ESP8266.
3. **Rangkaian Elektronika:**
   - Skema koneksi TX-RX dan common ground.
4. **Troubleshooting:**
   - Solusi untuk masalah umum UART.

---

### **Referensi Tambahan**
- [ESP32 UART Example](https://randomnerdtutorials.com/esp32-uart-communication/)
- [ESP8266 SoftwareSerial Guide](https://arduino.stackexchange.com/questions/69970/how-to-use-software-serial-on-esp8266)
- [Platform IO Serial Monitor](https://docs.platformio.org/en/latest/plus/debug-tools/serial-monitor.html)

---