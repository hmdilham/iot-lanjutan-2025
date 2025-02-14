### **Pertemuan 1: Pengenalan ESP32 dan ESP8266**

#### **1. Perbandingan ESP32 dan ESP8266**
**Materi:**
- **ESP8266:**
  - Mikrokontroler dengan Wi-Fi terintegrasi.
  - Spesifikasi:
    - CPU: Tensilica L106 32-bit (80 MHz atau 160 MHz dengan overclock).
    - Memori: 64 KB instruksi RAM, 96 KB data RAM.
    - Flash: 4 MB (tergantung modul, contoh: ESP-12E).
    - Wi-Fi: 802.11 b/g/n (2.4 GHz).
    - GPIO: 17 pin (beberapa multifungsi).
    - Harga: Lebih murah dibanding ESP32.
  - Kelebihan:
    - Hemat biaya untuk proyek IoT sederhana.
    - Komunitas besar dan banyak sumber belajar.
  - Kekurangan:
    - Lebih sedikit pin GPIO.
    - Tidak memiliki Bluetooth.
    - Performa lebih rendah dibanding ESP32.

- **ESP32:**
  - Mikrokontroler dengan Wi-Fi dan Bluetooth terintegrasi.
  - Spesifikasi:
    - CPU: Dual-core Tensilica LX6 (240 MHz).
    - Memori: 520 KB SRAM.
    - Flash: 4 MB (tergantung modul, contoh: ESP32 DevKit V1).
    - Wi-Fi: 802.11 b/g/n (2.4 GHz).
    - Bluetooth: Classic dan BLE (Bluetooth Low Energy).
    - GPIO: 34 pin (beberapa multifungsi).
    - Fitur tambahan: Touch sensor, DAC, Hall effect sensor.
  - Kelebihan:
    - Performa lebih tinggi.
    - Lebih banyak pin GPIO dan fitur tambahan.
    - Mendukung Bluetooth.
  - Kekurangan:
    - Lebih mahal dibanding ESP8266.
    - Konsumsi daya lebih tinggi.

**Referensi Bacaan:**
- [ESP32 vs ESP8266 Comparison](https://www.electronicshub.org/esp32-vs-esp8266/)
- [ESP8266 Datasheet](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf)
- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)

---

#### **2. Pengenalan Platform IO**
**Materi:**
- **Apa itu Platform IO?**
  - Platform IO adalah ekosistem open-source untuk pengembangan IoT.
  - Mendukung berbagai mikrokontroler, termasuk ESP32 dan ESP8266.
  - Terintegrasi dengan Visual Studio Code (VS Code) untuk kemudahan pengembangan.

- **Instalasi Platform IO:**
  1. Unduh dan instal Visual Studio Code: [Download VS Code](https://code.visualstudio.com/).
  2. Buka VS Code, lalu buka Extensions (Ctrl+Shift+X).
  3. Cari "PlatformIO IDE" dan klik Install.
  4. Setelah instalasi selesai, Platform IO siap digunakan.

- **Membuat Project Baru:**
  1. Buka Platform IO Home (ikon rumah di sidebar kiri).
  2. Klik "New Project".
  3. Isi nama project, pilih board (contoh: `ESP32 Dev Module` atau `NodeMCU 1.0 (ESP-12E Module)`).
  4. Pilih framework (Arduino).
  5. Klik "Finish".

- **Struktur Project Platform IO:**
  - `src/`: Folder untuk menyimpan file kode utama (`main.cpp`).
  - `include/`: Folder untuk file header.
  - `lib/`: Folder untuk library tambahan.
  - `platformio.ini`: File konfigurasi project (board, framework, library, dll).

**Referensi Bacaan:**
- [Platform IO Installation Guide](https://platformio.org/install)
- [Platform IO Documentation](https://docs.platformio.org/)

---

#### **3. Upload Program Sederhana (Blink LED)**
**Materi:**
- **Blink LED:**
  - Program sederhana untuk menyalakan dan mematikan LED bawaan pada ESP32/ESP8266.
  - Tujuan: Memastikan lingkungan pengembangan berfungsi dengan baik.

- **Kode Program:**
  ```cpp
  void setup() {
    pinMode(LED_BUILTIN, OUTPUT); // Set pin LED sebagai output
  }

  void loop() {
    digitalWrite(LED_BUILTIN, HIGH); // Nyalakan LED
    delay(1000);                     // Tunggu 1 detik
    digitalWrite(LED_BUILTIN, LOW);  // Matikan LED
    delay(1000);                     // Tunggu 1 detik
  }
  ```

- **Langkah Upload:**
  1. Sambungkan ESP32/ESP8266 ke komputer menggunakan kabel USB.
  2. Pastikan driver USB-to-Serial terinstal (contoh: CP2102 atau CH340).
  3. Buka file `src/main.cpp` di Platform IO.
  4. Salin kode Blink LED di atas.
  5. Klik tombol "Upload" (ikon panah kanan di bagian bawah VS Code).
  6. Tunggu hingga proses upload selesai (LED akan berkedip).

**Referensi Bacaan:**
- [Blink LED Example](https://docs.platformio.org/en/latest/tutorials/espressif32/arduino_debugging_unit_testing.html)
- [ESP32 Blink LED Tutorial](https://randomnerdtutorials.com/esp32-blink-led/)

---

#### **4. Troubleshooting Umum**
**Materi:**
- **Masalah Driver USB-to-Serial:**
  - Pastikan driver terinstal dengan benar.
  - Driver yang umum digunakan:
    - CP2102: [Download CP2102 Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
    - CH340: [Download CH340 Driver](https://sparks.gogo.co.nz/ch340.html)
- **Error Upload:**
  - Pastikan board dan port yang benar dipilih di Platform IO.
  - Cek koneksi kabel USB.
  - Restart VS Code atau komputer jika diperlukan.

**Referensi Bacaan:**
- [Troubleshooting ESP32/ESP8266 Upload Issues](https://randomnerdtutorials.com/esp32-troubleshooting-guide/)

---

### **Rangkuman Pertemuan 1**
1. **Teori:**
   - Memahami perbedaan ESP32 dan ESP8266.
   - Mengenal fitur dan spesifikasi kedua mikrokontroler.
2. **Praktik:**
   - Instalasi Platform IO di VS Code.
   - Membuat project baru dan mengupload program Blink LED.
3. **Troubleshooting:**
   - Mengatasi masalah umum seperti driver dan error upload.

---

### **Tugas Praktikum**
1. Buat project baru di Platform IO untuk ESP32 atau ESP8266.
2. Upload program Blink LED dan amati LED bawaan berkedip.
3. Modifikasi kode untuk mengubah kecepatan kedip LED (contoh: 500 ms).

---

Dengan materi ini, peserta akan memiliki dasar yang kuat untuk melanjutkan ke pertemuan berikutnya.