### **Pertemuan 11: Sensor dan Aktuator**
#### **1. Sensor DHT11/DHT22 (Suhu & Kelembaban)**
**Konsep**:
- **DHT11**:
  - Range suhu: 0-50°C (±2°C)
  - Range kelembaban: 20-80% (±5%)
  - Sampling rate: 1Hz
- **DHT22** (Lebih akurat):
  - Range suhu: -40-80°C (±0.5°C)
  - Range kelembaban: 0-100% (±2%)
  - Sampling rate: 0.5Hz

**Rangkaian**:
```
ESP32/ESP8266:
- 3.3V → VCC DHT
- GND → GND DHT
- GPIO4 → DATA DHT
- Resistor 10kΩ antara DATA dan VCC (pull-up)
```

**Kode Program**:
```cpp
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT22 // Ganti ke DHT11 jika menggunakan DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float temp = dht.readTemperature(); // Baca suhu (°C)
  float hum = dht.readHumidity();     // Baca kelembaban (%)

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Gagal membaca DHT!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temp);
  Serial.print("°C | Kelembaban: ");
  Serial.print(hum);
  Serial.println("%");

  delay(2000); // DHT22 butuh minimal 2 detik antar pembacaan
}
```

---

#### **2. Sensor HC-SR04 (Jarak)**
**Konsep**:
- Prinsip kerja: Ultrasonik (pancarkan gelombang 40kHz, hitung waktu pantul)
- Range: 2cm - 400cm
- Akurasi: ±3mm
- Pin:
  - VCC: 5V
  - Trig: Trigger pulse input
  - Echo: Pulse output
  - GND: Ground

**Rangkaian**:
```
ESP32:
- 5V → VCC HC-SR04
- GND → GND HC-SR04
- GPIO5 → Trig
- GPIO18 → Echo

ESP8266:
- 3.3V → VCC HC-SR04
- GND → GND HC-SR04
- D5 → Trig
- D6 → Echo
```

**Kode Program**:
```cpp
#define TRIG_PIN 5
#define ECHO_PIN 18

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Kirim pulse 10μs
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Hitung durasi echo
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; // Konversi ke cm (kecepatan suara 340m/s)

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}
```

---

#### **3. Motor Servo (SG90/MG996R)**
**Konsep**:
- Tipe:
  - **SG90**: 180° rotation, torque 1.8kgf·cm
  - **MG996R**: 180° rotation, torque 10kgf·cm
- Kontrol: PWM dengan pulse width 500-2500μs
  - 500μs = 0°
  - 1500μs = 90°
  - 2500μs = 180°

**Rangkaian**:
```
ESP32/ESP8266:
- 5V → VCC Servo
- GND → GND Servo
- GPIO13 → Signal Servo
```

**Kode Program**:
```cpp
#include <ESP32Servo.h> // Untuk ESP32
// #include <Servo.h> // Untuk ESP8266

#define SERVO_PIN 13
Servo myServo;

void setup() {
  myServo.attach(SERVO_PIN);
}

void loop() {
  // Gerakan servo 0° → 90° → 180°
  myServo.write(0);
  delay(1000);
  myServo.write(90);
  delay(1000);
  myServo.write(180);
  delay(1000);
}
```

---

### **Integrasi 3 Komponen dalam 1 Proyek**
**Contoh Proyek**: Sistem monitoring suhu & kontrol otomatis  
- Jika suhu > 30°C → Servo membuka kipas (posisi 180°)
- Jika jarak < 10cm → LED warning menyala

**Kode Integrasi**:
```cpp
#include <DHT.h>
#include <ESP32Servo.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 13
#define LED_PIN 2

DHT dht(DHTPIN, DHTTYPE);
Servo myServo;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  myServo.attach(SERVO_PIN);
}

void loop() {
  // Baca DHT22
  float temp = dht.readTemperature();
  
  // Baca HC-SR04
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float distance = pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;

  // Kontrol Servo berdasarkan suhu
  if (temp > 30) {
    myServo.write(180); // Buka kipas
  } else {
    myServo.write(0);   // Tutup kipas
  }

  // Kontrol LED berdasarkan jarak
  digitalWrite(LED_PIN, (distance < 10) ? HIGH : LOW);

  // Tampilkan data
  Serial.print("Suhu: "); Serial.print(temp); Serial.print("°C | ");
  Serial.print("Jarak: "); Serial.print(distance); Serial.println("cm");

  delay(1000);
}
```

---

### **Troubleshooting**
| Masalah | Solusi |
|---------|--------|
| DHT gagal membaca | Periksa koneksi pin DATA dan resistor pull-up |
| HC-SR04 selalu baca 0 | Pastikan VCC 5V dan pin Trig/Echo benar |
| Servo tidak bergerak | Cek sumber daya (servo butuh arus besar) |
| Interferensi sinyal | Beri kapasitor 100μF di VCC-GND servo |

---

### **Tips Optimalisasi**
1. **DHT22**:
   - Gunakan `dht.readTemperature(false)` untuk baca dalam Fahrenheit.
   - Hindari pembacaan terlalu cepat (<2 detik).

2. **HC-SR04**:
   - Filter noise dengan rata-rata 3 pembacaan:
     ```cpp
     float avgDistance = (distance1 + distance2 + distance3) / 3;
     ```

3. **Servo**:
   - Untuk ESP8266, gunakan `#include <Servo.h>` dan pin D1-D8.
   - Jika servo bergetar, tambahkan delay setelah `myServo.write()`.

---

### **Referensi**
- [DHT Library Docs](https://github.com/adafruit/DHT-sensor-library)
- [HC-SR04 Tutorial](https://randomnerdtutorials.com/esp32-hc-sr04-ultrasonic-arduino/)
- [ESP32Servo Library](https://github.com/jkb-git/ESP32Servo)
