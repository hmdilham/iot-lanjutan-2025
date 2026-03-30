### **Proyek Sistem IoT Lanjutan: Ekosistem Smart Home Terintegrasi**

#### **Pengantar Proyek**
Membangun sistem *Internet of Things* di dunia nyata jarang sekali hanya melibatkan satu perangkat tunggal. Industri saat ini menuntut pemahaman tentang bagaimana puluhan bahkan ratusan perangkat yang berbeda dapat saling berbicara dalam satu ekosistem jaringan yang stabil. 

Proyek akhir ini dirancang untuk mensimulasikan lingkungan kerja tersebut. Alih-alih membuat proyek individu yang terisolasi, Anda akan bekerja dalam sebuah tim besar (satu kelas) untuk membangun satu ekosistem "Smart Home" yang utuh. Melalui proyek ini, Anda akan menguji pemahaman mengenai arsitektur jaringan, keandalan alat (redundansi), dan logika komunikasi antar mesin. Keberhasilan alat Anda tidak hanya ditentukan oleh apakah alat tersebut menyala, tetapi apakah alat Anda dapat merespons dan memberikan data kepada subsistem lain dengan tepat waktu.

#### **Deskripsi Proyek**
Dalam proyek ini, kelas akan dibagi menjadi beberapa kelompok kerja. Setiap kelompok bertindak sebagai "pengembang produk" yang bertanggung jawab untuk merancang, merakit, dan memprogram satu subsistem spesifik di dalam rumah pintar (misalnya: Sistem Keamanan Pintu, Sistem Mitigasi Bencana, atau Sistem Pengontrol Iklim).

Seluruh perangkat (node) yang dibangun oleh masing-masing kelompok wajib berkomunikasi satu sama lain melalui peladen (broker) MQTT terpusat, dan seluruh data akan divisualisasikan serta diotomatisasi melalui dasbor utama menggunakan platform Home Assistant.

Untuk memastikan standar industri terpenuhi, setiap subsistem yang dibuat harus memenuhi kriteria fungsionalitas dan keandalan berikut:

#### **Kriteria dan Spesifikasi Wajib Sistem**

* **Penyandingan Jaringan Dinamis (WiFi Manager):** Perangkat tidak boleh memiliki kredensial WiFi (SSID dan Kata Sandi) yang ditulis mati (hardcoded) di dalam program. Setiap ESP32 harus menggunakan pustaka WiFiManager sehingga alat dapat dikonfigurasi ulang secara nirkabel melalui ponsel apabila dipindahkan ke jaringan yang baru.
* **Komunikasi Pub/Sub (MQTT):**
    Perangkat wajib dapat mengirimkan (Publish) status sensor dan menerima (Subscribe) perintah aktuator dari peladen MQTT yang telah disediakan oleh dosen. Topik MQTT harus terstruktur dan disepakati antar kelompok.
* **Redundansi Akses (Web Server Lokal):**
    Sebagai bentuk mitigasi jika jaringan internet utama mati atau peladen MQTT terputus, setiap perangkat ESP32 harus menjalankan *Web Server* lokal. Antarmuka web ini harus dapat diakses melalui alamat IP masing-masing alat untuk memantau sensor dan mengendalikan aktuator secara manual.
* **Otomasi Lintas Subsistem (Interoperabilitas):**
    Sistem yang Anda buat tidak boleh egois. Anda wajib merancang skenario otomasi di dalam Home Assistant di mana pemicu (trigger) dari sensor kelompok Anda dapat menghasilkan tindakan (action) pada aktuator kelompok lain. Misalnya: Sensor hujan dari Kelompok A memicu jemuran otomatis dari Kelompok B.
* **Implementasi Visi Komputer (Khusus Node Tertentu):**
    Sistem harus mengintegrasikan minimal satu pemrosesan citra, baik untuk pengenalan wajah maupun deteksi objek, untuk meningkatkan tingkat keamanan atau kenyamanan pengguna.

---

### Rubrik Penilaian Proyek Akhir IoT Lanjutan

**1. Konektivitas Dinamis dan Jaringan (Bobot: 15%)**
* **Sangat Baik (86-100):** Perangkat murni menggunakan WiFiManager. Mampu membuat Access Point saat jaringan tidak tersedia, dan menyimpan kredensial baru dengan cepat tanpa perlu memuat ulang kode program.
* **Baik (71-85):** Menggunakan WiFiManager, namun proses koneksi lambat atau memerlukan intervensi manual tambahan saat memuat halaman portal captive.
* **Kurang (0-70):** Masih menggunakan metode penulisan langsung (hardcode) SSID dan kata sandi di dalam kode, atau perangkat gagal terhubung ke jaringan lokal yang baru.

**2. Komunikasi Data via MQTT (Bobot: 20%)**
* **Sangat Baik (86-100):** Perangkat berhasil mempublikasikan data sensor secara kontinu tanpa jeda yang mengganggu, dan merespons perintah (subscribe) dalam waktu kurang dari 1 detik. Penamaan topik terstruktur dengan standar yang disepakati antar kelompok.
* **Baik (71-85):** Pengiriman dan penerimaan data berhasil, namun terdapat latensi yang sesekali muncul, atau penamaan topik kurang rapi dan konsisten.
* **Kurang (0-70):** Perangkat sering terputus dari broker MQTT, gagal mengirimkan status terbaru, atau tidak merespons perintah dari dasbor sama sekali.

**3. Redundansi dan Web Server Lokal (Bobot: 15%)**
* **Sangat Baik (86-100):** Web server lokal dapat diakses dengan lancar melalui alamat IP perangkat. Antarmuka menampilkan pembacaan sensor dan tombol kontrol aktuator berfungsi sempurna sebagai cadangan jika MQTT terputus.
* **Baik (71-85):** Web server tersedia, tetapi tampilan bersifat statis (harus dimuat ulang secara manual untuk melihat data baru) atau respons kontrol aktuator lambat.
* **Kurang (0-70):** Web server gagal dimuat, perangkat sering mengalami crash saat diakses melalui peramban, atau fungsionalitas kontrol manual tidak bekerja.

**4. Interoperabilitas dan Otomasi Lintas Subsistem (Bobot: 20%)**
* **Sangat Baik (86-100):** Kelompok berhasil mengonfigurasi skenario otomasi di Home Assistant yang menghubungkan alat mereka dengan minimal satu alat dari kelompok lain secara logis dan eksekusi berjalan tanpa kegagalan.
* **Baik (71-85):** Skenario otomasi lintas kelompok dibuat, tetapi terkadang gagal terpicu atau logika otomasinya kurang relevan dengan skenario penggunaan fungsional sehari-hari.
* **Kurang (0-70):** Alat hanya bekerja sendiri (standalone) dan tidak memiliki interaksi atau pemicu silang sama sekali dengan subsistem dari kelompok lain di dalam ekosistem Home Assistant.

**5. Fungsionalitas Perangkat Keras (Sensor, Aktuator, atau Computer Vision) (Bobot: 15%)**
* **Sangat Baik (86-100):** Pembacaan sensor akurat, aktuator (servo/relay) bekerja sesuai spesifikasi teknis tanpa masalah daya. Khusus kelompok Computer Vision: mampu mendeteksi objek/wajah dengan tingkat pengenalan tinggi dan aliran video stabil.
* **Baik (71-85):** Alat berfungsi secara dasar, namun terdapat gangguan minor seperti pembacaan sensor yang berfluktuasi sesekali atau gerakan mekanis yang kurang halus.
* **Kurang (0-70):** Rangkaian keras sering bermasalah (komponen terlalu panas atau daya tidak stabil), sensor memberikan data acak, atau aktuator tidak merespons sinyal.

**6. Pemahaman Arsitektur dan Kelompok (Bobot: 15%)**
* **Sangat Baik (86-100):** Seluruh anggota kelompok mampu menjelaskan alur transmisi data (dari pembacaan sensor hingga tampil di Home Assistant), memahami arsitektur komunikasi yang digunakan, dan dapat melakukan identifikasi masalah (troubleshooting) secara sistematis saat diuji.
* **Baik (71-85):** Mampu mendemonstrasikan alat dengan baik, namun hanya sebagian anggota kelompok yang menguasai pemahaman arsitektur sistem secara mendalam.
* **Kurang (0-70):** Penjelasan kelompok tidak terstruktur, tidak mampu membedakan peran antara broker MQTT dan web server lokal, serta kesulitan menjawab pertanyaan teknis mendasar dari sistem yang mereka bangun.

---
### Smart Home Project

Konsep proyek akhir smart home untuk mencerminkan arsitektur Internet of Things yang dapat diterapkan rumah maupun industri. Dengan adanya Home Assistant sebagai pusat kendali dan MQTT sebagai jalur komunikasi, mahasiswa dapat fokus merancang node sensor yang fungsional.

Agar sistem terasa nyata dan saling berkaitan, pendekatan terbaik adalah membagi kelompok berdasarkan "Zona" atau "Sistem" di dalam rumah. Berikut adalah saran pembagian tugas untuk 5 kelompok yang saling terintegrasi:

**Kelompok 1: Sistem Keamanan dan Akses (Area Depan/Pintu Utama)**
* **Peran:** Mengatur akses masuk dan mendeteksi aktivitas di area depan rumah.
* **Sensor:** RFID atau Keypad (untuk identifikasi penghuni), Magnetic Switch (mendeteksi pintu terbuka/tertutup), dan PIR (mendeteksi gerakan manusia).
* **Aktuator:** Relay untuk Solenoid Door Lock (pengunci pintu) dan lampu teras.
* **Fungsi Web Server Lokal:** Menampilkan status pintu saat ini, log akses masuk terakhir, dan tombol buka/tutup pintu manual jika jaringan MQTT terputus.

**Kelompok 2: Sistem Iklim dan Kenyamanan (Ruang Keluarga)**
* **Peran:** Memantau dan mengendalikan sirkulasi udara serta suhu ruangan.
* **Sensor:** DHT22 (Suhu dan Kelembapan).
* **Aktuator:** Modul IR Transmitter (sebagai remote universal untuk menyalakan AC/Kipas) dan Relay untuk lampu utama.
* **Fungsi Web Server Lokal:** Menampilkan grafik atau angka suhu dan kelembapan saat ini, serta antarmuka tombol remote AC lokal.

**Kelompok 3: Sistem Keselamatan dan Mitigasi Bencana (Area Dapur)**
* **Peran:** Mendeteksi anomali yang membahayakan seperti kebocoran gas atau potensi kebakaran.
* **Sensor:** MQ-2 (Sensor gas mudah terbakar dan asap) dan Flame Sensor (Sensor api).
* **Aktuator:** Buzzer/Alarm keras, Relay untuk menyalakan Exhaust Fan, dan Servo untuk simulasi menutup katup tabung gas.
* **Fungsi Web Server Lokal:** Indikator kadar gas di udara, status sensor api, dan tombol untuk mematikan alarm secara lokal.

**Kelompok 4: Sistem Otomasi Taman dan Utilitas Luar (Taman/Halaman)**
* **Peran:** Menjaga kelembapan tanah tanaman dan memantau cuaca luar.
* **Sensor:** Soil Moisture (Kelembapan tanah) dan Rain Drop Sensor (Sensor rintik hujan).
* **Aktuator:** Relay untuk pompa air mini dan Relay untuk menjemur pakaian otomatis (bisa disimulasikan dengan motor DC untuk menarik jemuran).
* **Fungsi Web Server Lokal:** Memantau tingkat kekeringan tanah, status cuaca luar, dan tombol penyiraman manual.

**Kelompok 5: Sistem Pemantauan Visual dan Notifikasi Terpusat (Ruang Kerja/Kamar)**
* **Peran:** Bertindak sebagai node pengamat tambahan menggunakan Computer Vision (mengingat Anda berencana memasukkan materi ini).
* **Perangkat:** ESP32-CAM dan layar OLED/LCD kecil.
* **Peran Teknis:** Mendeteksi kehadiran orang yang tidak dikenal (Face Recognition) di area tertentu, atau sekadar melakukan streaming video lokal. Layar OLED digunakan untuk menampilkan ringkasan pesan dari MQTT (misalnya menampilkan teks "Pintu Terbuka" atau "Gas Bocor").
* **Fungsi Web Server Lokal:** Menampilkan antarmuka streaming video dari kamera dan pengaturan resolusi gambar.

**Skenario Integrasi Antar Kelompok (Melalui Home Assistant & MQTT):**
Untuk membuat proyek ini fungsional, Anda dapat menugaskan mahasiswa membuat automasi silang antar kelompok di Home Assistant. Beberapa contoh skenario operasional:

1.  **Skenario Pulang ke Rumah:** Saat Kelompok 1 mendeteksi RFID penghuni yang sah dan pintu terbuka, Home Assistant mengirim perintah MQTT ke Kelompok 2 untuk menyalakan AC dan lampu ruang keluarga secara otomatis.
2.  **Skenario Mitigasi Kebakaran:** Jika Kelompok 3 mendeteksi kebocoran gas yang tinggi, Home Assistant mengirim perintah ke Kelompok 1 untuk membuka kunci pintu utama secara otomatis (agar penghuni bisa evakuasi), menyalakan alarm pada perangkat Kelompok 3, dan meminta Kelompok 5 menampilkan peringatan di layar OLED.
3.  **Skenario Hujan:** Saat sensor hujan Kelompok 4 mendeteksi air, Home Assistant akan memeriksa apakah ada jendela atau pintu luar (Kelompok 1) yang masih terbuka, lalu memberikan notifikasi kepada pengguna jika lupa ditutup, sekaligus menarik jemuran mekanis.

**Penerapan Persyaratan Wajib (WiFi Manager dan Web Server):**
Setiap kelompok harus mendemonstrasikan bahwa perangkat mereka siap pakai (plug and play). Jika alat dipindah ke jaringan WiFi lab yang berbeda, WiFi Manager akan otomatis memunculkan Access Point untuk memasukkan kredensial baru tanpa harus memprogram ulang. Web Server lokal bertindak sebagai cadangan (redundancy); jika peladen Pub/Sub atau Home Assistant yang Anda siapkan sedang mati, setiap alat tetap bisa diakses dan dikendalikan langsung melalui alamat IP lokal masing-masing perangkat.

---
### **Topik MQTT per Kelompok**

Untuk memastikan seluruh perangkat dapat saling berkomunikasi dengan lancar dan mudah diintegrasikan ke dalam Home Assistant, kita perlu menetapkan standar penamaan topik MQTT. 

Praktik terbaik dalam industri (dan standar yang sangat disukai oleh platform Home Assistant) adalah menggunakan hierarki berdasarkan **Area** dan **Fungsi**, diakhiri dengan tipe tindakan (`state` untuk membaca data, `set` untuk mengirim perintah).

Struktur dasar yang disepakati adalah sebagai berikut:
`smarthome/[area]/[nama_perangkat]/[state_atau_set]`

Berikut adalah rancangan pembagian topik Pub/Sub untuk masing-masing kelompok:

**Kelompok 1: Sistem Keamanan (Area Depan)**
* **Publish (Mengirim Data Sensor ke Server):**
    * `smarthome/depan/pintu/state` (Muatan/Payload: "OPEN" atau "CLOSED")
    * `smarthome/depan/gerak/state` (Muatan/Payload: "MOTION" atau "CLEAR")
    * `smarthome/depan/rfid/state` (Muatan/Payload: Nomor ID Kartu, misal "A1B2C3D4")
* **Subscribe (Menerima Perintah dari Server):**
    * `smarthome/depan/kunci/set` (Perintah masuk: "LOCK" atau "UNLOCK")
    * `smarthome/depan/lampu/set` (Perintah masuk: "ON" atau "OFF")

**Kelompok 2: Sistem Iklim (Ruang Keluarga)**
* **Publish:**
    * `smarthome/keluarga/suhu/state` (Muatan/Payload: Angka desimal, misal "25.5")
    * `smarthome/keluarga/kelembapan/state` (Muatan/Payload: Persentase, misal "60")
* **Subscribe:**
    * `smarthome/keluarga/ac/set` (Perintah masuk: "ON", "OFF", atau "24C")
    * `smarthome/keluarga/lampu/set` (Perintah masuk: "ON" atau "OFF")

**Kelompok 3: Sistem Keselamatan (Area Dapur)**
* **Publish:**
    * `smarthome/dapur/gas/state` (Muatan/Payload: "SAFE" atau "LEAK")
    * `smarthome/dapur/api/state` (Muatan/Payload: "SAFE" atau "FIRE")
* **Subscribe:**
    * `smarthome/dapur/alarm/set` (Perintah masuk: "ON" atau "OFF")
    * `smarthome/dapur/exhaust/set` (Perintah masuk: "ON" atau "OFF")
    * `smarthome/dapur/katup_gas/set` (Perintah masuk: "OPEN" atau "CLOSE")

**Kelompok 4: Sistem Otomasi Taman (Area Luar)**
* **Publish:**
    * `smarthome/taman/tanah/state` (Muatan/Payload: "WET" atau "DRY")
    * `smarthome/taman/hujan/state` (Muatan/Payload: "RAINING" atau "CLEAR")
* **Subscribe:**
    * `smarthome/taman/pompa/set` (Perintah masuk: "ON" atau "OFF")
    * `smarthome/taman/jemuran/set` (Perintah masuk: "IN" atau "OUT")

**Kelompok 5: Pemantauan Visual & Notifikasi (Ruang Kerja)**
* **Publish:**
    * `smarthome/kamar/kamera/wajah/state` (Muatan/Payload: "KNOWN", "UNKNOWN", atau Nama Orang)
* **Subscribe:**
    * `smarthome/kamar/layar/set` (Perintah masuk: Teks bebas, misal "Awas Gas Bocor!" atau "Pintu Terbuka")

---

### **Bagaimana Mereka Saling Berkomunikasi (Contoh Kasus)**

Dengan struktur di atas, dosen atau mahasiswa dapat merancang logika di dalam Home Assistant. Home Assistant bertugas sebagai "otak" yang mendengarkan topik tertentu, lalu mengirimkan perintah ke topik lainnya.

**Contoh Skenario Hujan (Kelompok 4 memicu Kelompok 1):**
1.  Sensor Kelompok 4 mendeteksi air. ESP32 Kelompok 4 melakukan *Publish* ke topik:
    `smarthome/taman/hujan/state` dengan pesan "RAINING".
2.  Home Assistant menerima pesan tersebut.
3.  Sistem mengecek data terakhir dari `smarthome/depan/pintu/state`. Jika statusnya "OPEN", Home Assistant akan otomatis melakukan *Publish* ke topik `smarthome/kamar/layar/set` dengan pesan "Hujan! Tutup Pintu Depan".
4.  Layar OLED Kelompok 5 yang melakukan *Subscribe* pada topik layar tersebut akan menampilkan pesan peringatan.

**Contoh Skenario Kebakaran (Kelompok 3 memicu Kelompok 1 dan 5):**
1.  Sensor api Kelompok 3 mendeteksi api, lalu melakukan *Publish* "FIRE" ke topik `smarthome/dapur/api/state`.
2.  Home Assistant merespons dengan melakukan *Publish* beruntun:
    * Ke topik `smarthome/depan/kunci/set` dengan pesan "UNLOCK" (membuka pintu otomatis agar penghuni bisa lari).
    * Ke topik `smarthome/kamar/layar/set` dengan pesan "BAHAYA KEBAKARAN".

Penggunaan format `state` (untuk output sensor) dan `set` (untuk input aktuator) ini akan sangat memudahkan *debugging*. Jika mahasiswa ingin menguji alatnya secara manual, mereka cukup menggunakan aplikasi seperti MQTT Explorer di laptop, lalu memublikasikan kata "ON" ke topik `smarthome/dapur/alarm/set`, dan buzzer Kelompok 3 harusnya langsung berbunyi.