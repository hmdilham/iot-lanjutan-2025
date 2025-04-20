### **Pertemuan 12: Dashboard dengan Home Assistant**
#### **1. Pengenalan Home Assistant**
**Apa itu Home Assistant?**
- Platform open-source untuk otomasi rumah berbasis Python.
- Fitur utama:
  - Integrasi dengan 1000+ perangkat IoT.
  - Dashboard customizable.
  - Dukungan MQTT, Zigbee, Z-Wave, dll.
- Kompatibel dengan ESP32/ESP8266 via MQTT.

**Instalasi Home Assistant**:
1. **Raspberry Pi** (Rekomendasi):
   ```bash
   # Install menggunakan Home Assistant OS
   wget https://github.com/home-assistant/operating-system/releases/download/10.5/ha_rpi4-64-10.5.img.xz
   ```
2. **Docker** (Untuk pengembangan):
   ```bash
   docker run -d --name="home-assistant" -v /path/to/config:/config -p 8123:8123 homeassistant/home-assistant
   ```
3. **Cloud** (Nabu Casa): Untuk akses remote berbayar.

---

### **2. Integrasi ESP32/ESP8266 dengan MQTT**
#### **Konfigurasi Broker MQTT di Home Assistant**
1. Buka `configuration.yaml` di Home Assistant:
   ```yaml
   mqtt:
     broker: 192.168.1.100  # Ganti dengan IP broker lokal
     username: mqtt_user
     password: mqtt_pass
   ```
2. Restart Home Assistant.

#### **Kode ESP32/ESP8266 (Publisher & Subscriber)**
**Publisher (Kirim Data Sensor DHT22)**:
```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASS";
const char* mqtt_server = "192.168.1.100"; // IP Home Assistant

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", "mqtt_user", "mqtt_pass")) {
      client.subscribe("home/livingroom/light/command"); // Subscribe untuk kontrol
    }
  }
}

void setup() {
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
  client.publish("home/livingroom/temperature", String(temp).c_str());
  client.publish("home/livingroom/humidity", String(hum).c_str());
  
  delay(10000); // Kirim setiap 10 detik
}
```

**Subscriber (Kontrol LED via Home Assistant)**:
```cpp
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i=0; i<length; i++) message += (char)payload[i];
  
  if (String(topic) == "home/livingroom/light/command") {
    if (message == "ON") digitalWrite(LED_PIN, HIGH);
    else digitalWrite(LED_PIN, LOW);
  }
}

void setup() {
  // ... (setup sebelumnya)
  client.setCallback(callback);
}
```

---

### **3. Membuat Dashboard di Home Assistant**
#### **Langkah 1: Definisikan Sensor di `configuration.yaml`**
```yaml
sensor:
  - platform: mqtt
    name: "Suhu Ruang Tamu"
    state_topic: "home/livingroom/temperature"
    unit_of_measurement: "°C"
    
  - platform: mqtt
    name: "Kelembaban Ruang Tamu"
    state_topic: "home/livingroom/humidity"
    unit_of_measurement: "%"

switch:
  - platform: mqtt
    name: "Lampu Ruang Tamu"
    command_topic: "home/livingroom/light/command"
    state_topic: "home/livingroom/light/status"
    payload_on: "ON"
    payload_off: "OFF"
```

#### **Langkah 2: Buat Dashboard**
1. Buka **Overview** → **Edit Dashboard** → **+ Tambah Kartu**.
2. Pilih tipe kartu:
   - **Gauge**: Untuk tampilan suhu/kelembaban.
   - **Entities**: Tampilkan nilai sensor.
   - **Button**: Kontrol lampu.
3. Contoh tampilan:
   ```yaml
   type: vertical-stack
   cards:
     - type: gauge
       entity: sensor.suhu_ruang_tamu
       name: Suhu
       min: 0
       max: 50
     - type: entities
       entities:
         - entity: sensor.kelembaban_ruang_tamu
           name: Kelembaban
     - type: button
       entity: switch.lampu_ruang_tamu
       icon: mdi:lightbulb
   ```

---

### **4. Hasil yang Diharapkan**
- **Dashboard Home Assistant**:
  ![Dashboard HA](https://i.imgur.com/JQ8W5vA.png)
  - Live data suhu/kelembaban.
  - Tombol ON/OFF untuk kontrol lampu.

- **ESP32/ESP8266**:
  - Mengirim data sensor setiap 10 detik.
  - Merespons perintah dari dashboard.

---

### **5. Troubleshooting**
| Masalah | Solusi |
|---------|--------|
| Data tidak muncul di HA | Cek `state_topic` di YAML dan kode ESP |
| Tombol tidak bekerja | Pastikan ESP subscribe ke `command_topic` |
| Koneksi MQTT gagal | Verifikasi username/password di `configuration.yaml` |

---

### **Tips Lanjutan**
1. **Otomasi**:
   Buat rule otomatis di HA (contoh: nyalakan lampu jika suhu > 30°C):
   ```yaml
   automation:
     - alias: "Nyalakan Lampu Saat Panas"
       trigger:
         platform: numeric_state
         entity_id: sensor.suhu_ruang_tamu
         above: 30
       action:
         service: switch.turn_on
         entity_id: switch.lampu_ruang_tamu
   ```

2. **Visualisasi**:
   Gunakan add-on **Grafana** untuk tampilan grafis lebih canggih.

3. **Keamanan**:
   - Enkripsi MQTT dengan TLS.
   - Gunakan password kuat untuk MQTT user.

---

### **Referensi**
- [Home Assistant MQTT Docs](https://www.home-assistant.io/integrations/mqtt/)
- [ESP32 + HA Tutorial](https://randomnerdtutorials.com/home-assistant-esp32-esp8266-mqtt/)