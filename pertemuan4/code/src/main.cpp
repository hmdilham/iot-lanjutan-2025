#include <Wire.h>                   // Library komunikasi I2C
#include <Adafruit_GFX.h>           // Library grafis untuk OLED
#include <Adafruit_SSD1306.h>       // Library untuk OLED SSD1306
#include <Adafruit_Sensor.h>        // Library sensor Adafruit
#include <DHT.h>                    // Library DHT22

// Konfigurasi OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C  // Alamat default OLED di I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Konfigurasi DHT22
#define DHTPIN 4       // Pin data DHT22
#define DHTTYPE DHT22  // Jenis sensor DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    
    // Inisialisasi OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("Gagal menginisialisasi OLED");
        for (;;); // Hentikan program jika gagal
    }
    
    // Inisialisasi DHT22
    dht.begin();
    
    // Tampilkan pesan awal di OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 10);
    display.println("DHT22 OLED Display");
    display.display();
    delay(2000);
}

void loop() {
    // Baca suhu dan kelembaban dari DHT22
    float suhu = dht.readTemperature();   // Suhu dalam Celcius
    float kelembaban = dht.readHumidity(); 

    // Periksa apakah pembacaan gagal
    if (isnan(suhu) || isnan(kelembaban)) {
        Serial.println("Gagal membaca dari sensor DHT22!");
        return;
    }

    // Tampilkan di Serial Monitor
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C, Kelembaban: ");
    Serial.print(kelembaban);
    Serial.println(" %");

    // Tampilkan di OLED
    display.clearDisplay();  
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Tampilkan Judul
    display.setCursor(10, 0);
    display.println("DHT22 OLED Display");

    // Tampilkan Suhu
    display.setCursor(10, 25);
    display.print("Temp: ");
    display.print(suhu);
    display.print(" C");

    // Tampilkan Kelembaban
    display.setCursor(10, 40);
    display.print("Hum: ");
    display.print(kelembaban);
    display.print(" %");

    // Perbarui tampilan OLED
    display.display();  

    delay(2000); // Tunggu 2 detik sebelum membaca ulang
}
