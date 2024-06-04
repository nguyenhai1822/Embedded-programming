#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR    0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

char auth[] = "KyHKrXtZK8LD4hRdMcv54QF1GB4IYRfs";
char ssid[] = "nguyenhai";
char pass[] = "23456788";

void setup() {
  Serial.begin(115200);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Display startup message on the OLED screen
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(1, 0);
  display.print("System Loading");
  display.display();
  delay(2000);
  // Clear the display
  display.clearDisplay();
}

// Lấy giá trị từ cảm biến DHT11
void DHT11sensor() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  // Display temperature and humidity on OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("T: ");
  display.print(t);

  display.setCursor(8, 20);
  display.print("H: ");
  display.print(h);

  display.display();
}

void loop() {
  DHT11sensor();
  Blynk.run();
  timer.run();
  delay(200);
}
