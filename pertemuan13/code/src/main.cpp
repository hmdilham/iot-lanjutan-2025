#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

// Nilai default jika config.json tidak ditemukan
#define DEFAULT_SSID        "NAMA_WIFI"
#define DEFAULT_PASS        "PASSWORD_WIFI"
#define DEFAULT_MQTT_HOST   "broker.emqx.io"
#define DEFAULT_MQTT_PORT   1883
#define DEFAULT_MQTT_TOPIC  "iot/kelas/sensor"
#define DEFAULT_DEVICE_ID   "ESP32-001"

String cfg_ssid, cfg_pass, cfg_mqtt_host, cfg_mqtt_topic, cfg_device_id;
int    cfg_mqtt_port;

WiFiClient   espClient;
PubSubClient mqtt(espClient);

// Baca dan parse config.json dari LittleFS
bool loadConfig() {
  if (!LittleFS.exists("/config.json")) return false;

  File f = LittleFS.open("/config.json", "r");
  if (!f) return false;

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) {
    Serial.printf("JSON error: %s\n", err.c_str());
    return false;
  }

  cfg_ssid       = doc["wifi_ssid"]   | DEFAULT_SSID;
  cfg_pass       = doc["wifi_pass"]   | DEFAULT_PASS;
  cfg_mqtt_host  = doc["mqtt_server"] | DEFAULT_MQTT_HOST;
  cfg_mqtt_port  = doc["mqtt_port"]   | DEFAULT_MQTT_PORT;
  cfg_mqtt_topic = doc["mqtt_topic"]  | DEFAULT_MQTT_TOPIC;
  cfg_device_id  = doc["device_id"]   | DEFAULT_DEVICE_ID;
  return true;
}

// Simpan konfigurasi ke config.json (untuk update runtime)
void saveConfig() {
  JsonDocument doc;
  doc["wifi_ssid"]   = cfg_ssid;
  doc["wifi_pass"]   = cfg_pass;
  doc["mqtt_server"] = cfg_mqtt_host;
  doc["mqtt_port"]   = cfg_mqtt_port;
  doc["mqtt_topic"]  = cfg_mqtt_topic;
  doc["device_id"]   = cfg_device_id;

  File f = LittleFS.open("/config.json", "w");
  if (!f) {
    Serial.println("Gagal menyimpan config.json!");
    return;
  }
  serializeJsonPretty(doc, f);
  f.close();
  Serial.println("Konfigurasi disimpan ke LittleFS.");
}

void connectWiFi() {
  WiFi.begin(cfg_ssid.c_str(), cfg_pass.c_str());
  Serial.printf("Menghubungkan ke WiFi '%s'", cfg_ssid.c_str());
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nIP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\nGagal terhubung ke WiFi!");
  }
}

void reconnectMQTT() {
  while (!mqtt.connected()) {
    String clientId = cfg_device_id + "-" + String(random(0xffff), HEX);
    Serial.printf("Menghubungkan ke MQTT %s:%d ...", cfg_mqtt_host.c_str(), cfg_mqtt_port);
    if (mqtt.connect(clientId.c_str())) {
      Serial.println(" OK");
    } else {
      Serial.printf(" Gagal (rc=%d), coba lagi 5 detik\n", mqtt.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Mount LittleFS
  if (!LittleFS.begin(true)) {
    Serial.println("FATAL: Gagal mount LittleFS!");
    return;
  }
  Serial.printf("LittleFS OK — total: %u bytes, dipakai: %u bytes\n",
    LittleFS.totalBytes(), LittleFS.usedBytes());

  // Muat konfigurasi; jika tidak ada, pakai default dan simpan
  if (loadConfig()) {
    Serial.println("Konfigurasi dimuat dari LittleFS.");
  } else {
    Serial.println("config.json tidak ada, menggunakan nilai default.");
    cfg_ssid       = DEFAULT_SSID;
    cfg_pass       = DEFAULT_PASS;
    cfg_mqtt_host  = DEFAULT_MQTT_HOST;
    cfg_mqtt_port  = DEFAULT_MQTT_PORT;
    cfg_mqtt_topic = DEFAULT_MQTT_TOPIC;
    cfg_device_id  = DEFAULT_DEVICE_ID;
    saveConfig();
  }

  Serial.printf("SSID       : %s\n", cfg_ssid.c_str());
  Serial.printf("MQTT Host  : %s:%d\n", cfg_mqtt_host.c_str(), cfg_mqtt_port);
  Serial.printf("MQTT Topic : %s\n", cfg_mqtt_topic.c_str());
  Serial.printf("Device ID  : %s\n", cfg_device_id.c_str());

  connectWiFi();
  mqtt.setServer(cfg_mqtt_host.c_str(), cfg_mqtt_port);
}

void loop() {
  if (!mqtt.connected()) reconnectMQTT();
  mqtt.loop();

  // Simulasi data sensor suhu (ganti dengan pembacaan sensor nyata)
  float temp = 24.0 + random(-10, 50) / 10.0;
  char payload[128];
  snprintf(payload, sizeof(payload),
    "{\"device\":\"%s\",\"temp\":%.1f}",
    cfg_device_id.c_str(), temp);

  if (mqtt.publish(cfg_mqtt_topic.c_str(), payload)) {
    Serial.printf("Publish OK: %s\n", payload);
  } else {
    Serial.println("Publish gagal!");
  }

  delay(5000);
}