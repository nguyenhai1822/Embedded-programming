#define BLYNK_TEMPLATE_ID "TMPL6LfzOAMql"
#define BLYNK_TEMPLATE_NAME "test ap suat"
#define BLYNK_AUTH_TOKEN "QtcMurIar_MIK3RuaarPEPok1admrggh"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SFE_BMP180.h>

// Tạo biến cho áp suất
double T, P;
char status;

// Khởi tạo màn hình LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Tạo đối tượng cho cảm biến BMP180
SFE_BMP180 bmp;
BlynkTimer timer;

char ssid[] = "nguyenhai";
char pass[] = "23456788";

void setup() {
  // Bộ định tuyến đang dùng
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  bmp.begin();
  lcd.init();
  lcd.backlight();
  analogReadResolution(12);

  lcd.setCursor(0, 0);
  lcd.print("System");
  lcd.setCursor(4, 1);
  lcd.print("Loading..");
  delay(4000);
  lcd.clear();
}

// Lấy giá trị áp suất
void pressure() {
  status = bmp.startTemperature();
  if (status != 0) {
    delay(status);
    status = bmp.getTemperature(T);

    status = bmp.startPressure(3); // 0 đến 3
    if (status != 0) {
      delay(status);
      status = bmp.getPressure(P, T);
      if (status != 0) {
      }
    }
  }

  Blynk.virtualWrite(V3, P);
  lcd.setCursor(0, 0);
  lcd.print("Ap Suat P:");
  lcd.setCursor(1, 5);
  lcd.print(P);
}

void loop() {
  pressure();
  Blynk.run(); // Chạy thư viện Blynk
}
