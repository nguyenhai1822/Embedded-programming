#define BLYNK_TEMPLATE_ID "TMPL6v1XekkCP"
#define BLYNK_TEMPLATE_NAME "testcambienmua"
#define BLYNK_AUTH_TOKEN "RwSLccBxwzrrzz37dzI-05Vr9325UzQm"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SFE_BMP180.h>

#define Rain 36

// Khởi tạo màn hình LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

BlynkTimer timer;

char ssid[] = "nguyenhai";
char pass[] = "23456788";

void setup() {
  // Bộ định tuyến đang dùng
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  lcd.init();
  lcd.backlight();
  pinMode(Rain, INPUT);
  analogReadResolution(12);

  lcd.setCursor(0, 0);
  lcd.print("System");
  lcd.setCursor(4, 1);
  lcd.print("Loading..");
  delay(4000);
  lcd.clear();
}


// Lấy giá trị từ cảm biến mưa
void rainSensor() {
  int Rvalue = analogRead(Rain);
  Rvalue = map(Rvalue, 0, 4095, 0, 100);
  Rvalue = (Rvalue - 100) * -1;
  Blynk.virtualWrite(V2, Rvalue);

  lcd.setCursor(0, 0);
  lcd.print("Luong mua R:");
  lcd.setCursor(1, 5);
  lcd.print(Rvalue);
}



void loop() {
  rainSensor();
  Blynk.run(); // Chạy thư viện Blynk
}
