#include <WiFi.h>
#include <PubSubClient.h>

// Konfigurasi Wi-Fi
const char* ssid = "Xperia XZP";
const char* password = "ilham12345";

// Konfigurasi MQTT
const char* mqtt_server = "broker.emqx.io"; // Broker publik
const int mqtt_port = 1883;
const char* mqtt_topic = "iot_lanjut/mat10/mqtt"; // Topik yang di-subscribe

// Konfigurasi LED
#define LED_PIN 2 // GPIO2 (ESP32) atau D4 (ESP8266)

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung ke Wi-Fi");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

// Fungsi untuk menangani pesan yang diterima
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Pesan diterima [");
  Serial.print(topic);
  Serial.print("]: ");
  
  // Konversi payload ke string
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Kontrol LED berdasarkan pesan
  if (message == "on") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON");
  } else if (message == "off") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF");
  }
}

void reconnect() {
  while (!client.connected()) {
    String client_id = "ESP32-Subscriber-" + String(random(0xffff), HEX);
    if (client.connect(client_id.c_str())) {
      Serial.println("Terhubung ke Broker MQTT!");
      client.subscribe(mqtt_topic); // Subscribe ke topik
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" Coba lagi dalam 5 detik...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback); // Set fungsi callback
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Jaga koneksi MQTT
}