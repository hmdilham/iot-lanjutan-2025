#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 4      // Pin DATA DHT (GPIO4)
#define DHTTYPE DHT22 // Tipe sensor (DHT11 atau DHT22)

const char* ssid = "your_ssid";       // Nama jaringan Wi-Fi
const char* password = "ssid_pass"; // Password Wi-Fi

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80); // Port 80 untuk HTTP

void handleRoot() {
  float temperature = dht.readTemperature(); // Baca suhu (°C)
  float humidity = dht.readHumidity();       // Baca kelembaban (%)

  if (isnan(temperature) || isnan(humidity)) {
    server.send(500, "text/plain", "Gagal membaca data dari sensor DHT!");
    return;
  }

  // Buat halaman HTML
  String html = "<html><body>";
  html += "<h1>Data Sensor DHT</h1>";
  html += "<p>Suhu: " + String(temperature) + " °C</p>";
  html += "<p>Kelembaban: " + String(humidity) + " %</p>";
  html += "</body></html>";

  server.send(200, "text/html", html); // Kirim respons ke client
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Mulai koneksi Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nTerhubung ke Wi-Fi!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());

  // Atur routing web server
  server.on("/", handleRoot); // Tampilkan data sensor di root
  server.begin();
  Serial.println("Web server berjalan!");
}

void loop() {
  server.handleClient(); // Tangani request dari client
}