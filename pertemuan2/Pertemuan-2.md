### **Pertemuan 2: Dasar Pemrograman ESP32/ESP8266**

#### **1. Struktur Program Arduino**
**Materi:**
- **Fungsi `setup()`:**
  - Fungsi ini dijalankan sekali saat mikrokontroler mulai.
  - Digunakan untuk inisialisasi, seperti mengatur mode pin, menginisialisasi komunikasi serial, atau mengatur konfigurasi awal.
  - Contoh:
    ```cpp
    void setup() {
      pinMode(LED, OUTPUT);  // Mengatur pin LED sebagai output
    }
    ```

- **Fungsi `loop()`:**
  - Fungsi ini dijalankan berulang-ulang setelah `setup()` selesai.
  - Digunakan untuk logika utama program, seperti membaca sensor, mengontrol aktuator, atau mengirim data.
  - Contoh:
    ```cpp
    void loop() {
      digitalWrite(LED, HIGH);  // Nyalakan LED
      delay(1000);              // Tunggu 1 detik
      digitalWrite(LED, LOW);   // Matikan LED
      delay(1000);              // Tunggu 1 detik
    }
    ```

**Referensi Bacaan:**
- [Arduino Programming Basics](https://www.arduino.cc/en/Guide/Introduction)

---

#### **2. GPIO (General Purpose Input/Output)**
**Materi:**
- **Apa itu GPIO?**
  - Pin yang dapat digunakan sebagai input atau output.
  - Input: Membaca sinyal dari komponen eksternal (contoh: tombol, sensor).
  - Output: Mengirim sinyal ke komponen eksternal (contoh: LED, relay).

- **Mengatur Mode Pin:**
  - `pinMode(pin, mode)`: Mengatur mode pin (INPUT, OUTPUT, INPUT_PULLUP).
  - Contoh:
    ```cpp
    pinMode(LED, OUTPUT);  // Mengatur pin LED sebagai output
    pinMode(BUTTON, INPUT_PULLUP);  // Mengatur pin BUTTON sebagai input dengan pull-up resistor
    ```

- **Membaca Input Digital:**
  - `digitalRead(pin)`: Membaca nilai digital dari pin (HIGH atau LOW).
  - Contoh:
    ```cpp
    int buttonState = digitalRead(BUTTON);  // Membaca nilai tombol
    ```

- **Menulis Output Digital:**
  - `digitalWrite(pin, value)`: Menulis nilai digital ke pin (HIGH atau LOW).
  - Contoh:
    ```cpp
    digitalWrite(LED, HIGH);  // Nyalakan LED
    ```

**Praktik:**
- Buat program untuk menyalakan LED saat tombol ditekan.

**Kode Program:**
```cpp
#define LED 2      // Pin LED
#define BUTTON 4   // Pin tombol

void setup() {
  pinMode(LED, OUTPUT);       // Set pin LED sebagai output
  pinMode(BUTTON, INPUT);     // Set pin tombol sebagai input
}

void loop() {
  int buttonState = digitalRead(BUTTON);  // Baca nilai tombol
  if (buttonState == LOW) {               // Jika tombol ditekan (LOW karena pull-up)
    digitalWrite(LED, HIGH);              // Nyalakan LED
  } else {
    digitalWrite(LED, LOW);               // Matikan LED
  }
}
```

**Referensi Bacaan:**
- [ESP32 GPIO Guide](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
- [ESP8266 GPIO Guide](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)

---

#### **3. Analog Input**
**Materi:**
- **Apa itu Analog Input?**
  - Membaca nilai analog (0-3.3V) dari pin yang mendukung ADC (Analog-to-Digital Converter).
  - ESP32: Memiliki 12-bit ADC (0-4095).
  - ESP8266: Memiliki 10-bit ADC (0-1023).

- **Membaca Nilai Analog:**
  - `analogRead(pin)`: Membaca nilai analog dari pin.
  - Contoh:
    ```cpp
    int sensorValue = analogRead(SENSOR_PIN);  // Membaca nilai sensor
    ```

**Praktik:**
- Buat program untuk membaca nilai potensiometer dan menampilkannya di Serial Monitor.

**Kode Program:**
```cpp
#define POTENTIOMETER 34  // Pin potensiometer (ESP32: GPIO34, ESP8266: A0)

void setup() {
  Serial.begin(115200);  // Inisialisasi Serial Monitor
}

void loop() {
  int sensorValue = analogRead(POTENTIOMETER);  // Baca nilai potensiometer
  Serial.println(sensorValue);                 // Tampilkan nilai di Serial Monitor
  delay(100);                                  // Tunggu 100 ms
}
```

**Referensi Bacaan:**
- [ESP32 Analog Input Guide](https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/)
- [ESP8266 Analog Input Guide](https://randomnerdtutorials.com/esp8266-adc-reading-analog-values-nodemcu/)

---

#### **4. PWM (Pulse Width Modulation)**
**Materi:**
- **Apa itu PWM?**
  - Teknik untuk menghasilkan sinyal analog menggunakan sinyal digital.
  - Digunakan untuk mengatur kecerahan LED, kecepatan motor, dll.
  - ESP32: Memiliki 16 channel PWM dengan resolusi 1-16 bit.
  - ESP8266: Memiliki 4 channel PWM dengan resolusi 10 bit.

- **Mengatur PWM:**
  - `analogWrite(pin, value)`: Menulis nilai PWM ke pin (0-255 untuk ESP8266, 0-4095 untuk ESP32).
  - Contoh:
    ```cpp
    analogWrite(LED, 128);  // Mengatur kecerahan LED ke 50%
    ```

**Praktik:**
- Buat program untuk mengatur kecerahan LED menggunakan potensiometer.

**Kode Program:**
```cpp
#define LED 2              // Pin LED
#define POTENTIOMETER 34   // Pin potensiometer

void setup() {
  pinMode(LED, OUTPUT);    // Set pin LED sebagai output
}

void loop() {
  int sensorValue = analogRead(POTENTIOMETER);  // Baca nilai potensiometer
  int brightness = map(sensorValue, 0, 4095, 0, 255);  // Map nilai ke 0-255
  analogWrite(LED, brightness);                 // Atur kecerahan LED
  delay(10);                                   // Tunggu 10 ms
}
```

**Referensi Bacaan:**
- [ESP32 PWM Guide](https://randomnerdtutorials.com/esp32-pwm-arduino-ide/)
- [ESP8266 PWM Guide](https://randomnerdtutorials.com/esp8266-nodemcu-pwm-arduino-ide/)

---

### **Rangkuman Pertemuan 2**
1. **Teori:**
   - Memahami struktur program Arduino (`setup()` dan `loop()`).
   - Mengenal GPIO, analog input, dan PWM.
2. **Praktik:**
   - Mengontrol LED menggunakan tombol.
   - Membaca nilai analog dari potensiometer.
   - Mengatur kecerahan LED menggunakan PWM.
3. **Troubleshooting:**
   - Memastikan pin yang digunakan sesuai dengan modul.
   - Memeriksa koneksi komponen eksternal.

---

### **Tugas Praktikum**
1. Buat program untuk mengontrol dua LED menggunakan dua tombol.
2. Buat program untuk membaca nilai dari sensor cahaya (LDR) dan menampilkannya di Serial Monitor.
3. Buat program untuk mengatur kecepatan motor DC menggunakan potensiometer.

---
Berikut adalah **penjabaran lengkap materi Pertemuan 2** yang dilengkapi dengan **rangkaian elektronika** untuk setiap praktikum. Rangkaian ini dirancang untuk memudahkan peserta dalam memahami dan mengimplementasikan program yang telah dibuat.

---

### **Pertemuan 2: Rangkaian Elektronika**

---

#### **1. Kontrol LED Menggunakan Tombol**
**Materi:**
- Mengontrol LED menggunakan tombol (input digital).

**Komponen yang Dibutuhkan:**
- ESP32 atau ESP8266
- LED (1 buah)
- Resistor 220Ω (1 buah)
- Push button (1 buah)
- Resistor 10kΩ (1 buah, untuk pull-up)
- Breadboard dan kabel jumper

**Rangkaian Elektronika:**
1. Hubungkan kaki panjang LED (anoda) ke pin GPIO2 (ESP32) atau D4 (ESP8266).
2. Hubungkan kaki pendek LED (katoda) ke resistor 220Ω, lalu ke GND.
3. Hubungkan salah satu kaki push button ke pin GPIO4 (ESP32) atau D3 (ESP8266).
4. Hubungkan kaki lainnya dari push button ke GND.
5. Pasang resistor 10kΩ antara pin GPIO4/D3 dan 3.3V (pull-up resistor).

**Skema Rangkaian:**
```
ESP32/ESP8266:
- GPIO2/D4 -> LED (+) -> Resistor 220Ω -> GND
- GPIO4/D3 -> Push Button -> GND
- 3.3V -> Resistor 10kΩ -> GPIO4/D3
```

**Kode Program:**
```cpp
#define LED 2      // Pin LED
#define BUTTON 4   // Pin tombol

void setup() {
  pinMode(LED, OUTPUT);       // Set pin LED sebagai output
  pinMode(BUTTON, INPUT);     // Set pin tombol sebagai input
}

void loop() {
  int buttonState = digitalRead(BUTTON);  // Baca nilai tombol
  if (buttonState == LOW) {               // Jika tombol ditekan (LOW karena pull-up)
    digitalWrite(LED, HIGH);              // Nyalakan LED
  } else {
    digitalWrite(LED, LOW);               // Matikan LED
  }
}
```

---

#### **2. Membaca Nilai Potensiometer**
**Materi:**
- Membaca nilai analog dari potensiometer dan menampilkannya di Serial Monitor.

**Komponen yang Dibutuhkan:**
- ESP32 atau ESP8266
- Potensiometer (10kΩ)
- Breadboard dan kabel jumper

**Rangkaian Elektronika:**
1. Hubungkan kaki tengah potensiometer ke pin GPIO34 (ESP32) atau A0 (ESP8266).
2. Hubungkan salah satu kaki luar potensiometer ke 3.3V.
3. Hubungkan kaki luar lainnya ke GND.

**Skema Rangkaian:**
```
ESP32/ESP8266:
- GPIO34/A0 -> Kaki tengah potensiometer
- 3.3V -> Kaki luar potensiometer
- GND -> Kaki luar potensiometer
```

**Kode Program:**
```cpp
#define POTENTIOMETER 34  // Pin potensiometer (ESP32: GPIO34, ESP8266: A0)

void setup() {
  Serial.begin(115200);  // Inisialisasi Serial Monitor
}

void loop() {
  int sensorValue = analogRead(POTENTIOMETER);  // Baca nilai potensiometer
  Serial.println(sensorValue);                 // Tampilkan nilai di Serial Monitor
  delay(100);                                  // Tunggu 100 ms
}
```

---

#### **3. Mengatur Kecerahan LED Menggunakan PWM**
**Materi:**
- Mengatur kecerahan LED menggunakan nilai analog dari potensiometer.

**Komponen yang Dibutuhkan:**
- ESP32 atau ESP8266
- LED (1 buah)
- Resistor 220Ω (1 buah)
- Potensiometer (10kΩ)
- Breadboard dan kabel jumper

**Rangkaian Elektronika:**
1. Hubungkan kaki panjang LED (anoda) ke pin GPIO2 (ESP32) atau D4 (ESP8266).
2. Hubungkan kaki pendek LED (katoda) ke resistor 220Ω, lalu ke GND.
3. Hubungkan kaki tengah potensiometer ke pin GPIO34 (ESP32) atau A0 (ESP8266).
4. Hubungkan salah satu kaki luar potensiometer ke 3.3V.
5. Hubungkan kaki luar lainnya ke GND.

**Skema Rangkaian:**
```
ESP32/ESP8266:
- GPIO2/D4 -> LED (+) -> Resistor 220Ω -> GND
- GPIO34/A0 -> Kaki tengah potensiometer
- 3.3V -> Kaki luar potensiometer
- GND -> Kaki luar potensiometer
```

**Kode Program:**
```cpp
#define LED 2              // Pin LED
#define POTENTIOMETER 34   // Pin potensiometer

void setup() {
  pinMode(LED, OUTPUT);    // Set pin LED sebagai output
}

void loop() {
  int sensorValue = analogRead(POTENTIOMETER);  // Baca nilai potensiometer
  int brightness = map(sensorValue, 0, 4095, 0, 255);  // Map nilai ke 0-255
  analogWrite(LED, brightness);                 // Atur kecerahan LED
  delay(10);                                   // Tunggu 10 ms
}
```

---

#### **4. Kontrol Dua LED Menggunakan Dua Tombol**
**Materi:**
- Mengontrol dua LED menggunakan dua tombol.

**Komponen yang Dibutuhkan:**
- ESP32 atau ESP8266
- LED (2 buah)
- Resistor 220Ω (2 buah)
- Push button (2 buah)
- Resistor 10kΩ (2 buah, untuk pull-up)
- Breadboard dan kabel jumper

**Rangkaian Elektronika:**
1. Hubungkan kaki panjang LED1 (anoda) ke pin GPIO2 (ESP32) atau D4 (ESP8266).
2. Hubungkan kaki pendek LED1 (katoda) ke resistor 220Ω, lalu ke GND.
3. Hubungkan kaki panjang LED2 (anoda) ke pin GPIO5 (ESP32) atau D5 (ESP8266).
4. Hubungkan kaki pendek LED2 (katoda) ke resistor 220Ω, lalu ke GND.
5. Hubungkan salah satu kaki push button1 ke pin GPIO4 (ESP32) atau D3 (ESP8266).
6. Hubungkan kaki lainnya dari push button1 ke GND.
7. Hubungkan salah satu kaki push button2 ke pin GPIO18 (ESP32) atau D6 (ESP8266).
8. Hubungkan kaki lainnya dari push button2 ke GND.
9. Pasang resistor 10kΩ antara pin GPIO4/D3 dan 3.3V (pull-up resistor).
10. Pasang resistor 10kΩ antara pin GPIO18/D6 dan 3.3V (pull-up resistor).

**Skema Rangkaian:**
```
ESP32/ESP8266:
- GPIO2/D4 -> LED1 (+) -> Resistor 220Ω -> GND
- GPIO5/D5 -> LED2 (+) -> Resistor 220Ω -> GND
- GPIO4/D3 -> Push Button1 -> GND
- GPIO18/D6 -> Push Button2 -> GND
- 3.3V -> Resistor 10kΩ -> GPIO4/D3
- 3.3V -> Resistor 10kΩ -> GPIO18/D6
```

**Kode Program:**
```cpp
#define LED1 2      // Pin LED1
#define LED2 5      // Pin LED2
#define BUTTON1 4   // Pin tombol1
#define BUTTON2 18  // Pin tombol2

void setup() {
  pinMode(LED1, OUTPUT);       // Set pin LED1 sebagai output
  pinMode(LED2, OUTPUT);       // Set pin LED2 sebagai output
  pinMode(BUTTON1, INPUT);     // Set pin tombol1 sebagai input
  pinMode(BUTTON2, INPUT);     // Set pin tombol2 sebagai input
}

void loop() {
  int buttonState1 = digitalRead(BUTTON1);  // Baca nilai tombol1
  int buttonState2 = digitalRead(BUTTON2);  // Baca nilai tombol2

  if (buttonState1 == LOW) {                // Jika tombol1 ditekan
    digitalWrite(LED1, HIGH);               // Nyalakan LED1
  } else {
    digitalWrite(LED1, LOW);                // Matikan LED1
  }

  if (buttonState2 == LOW) {                // Jika tombol2 ditekan
    digitalWrite(LED2, HIGH);               // Nyalakan LED2
  } else {
    digitalWrite(LED2, LOW);                // Matikan LED2
  }
}
```