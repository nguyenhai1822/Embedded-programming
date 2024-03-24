#define BLYNK_TEMPLATE_ID "TMPL6SQQgPeca"
#define BLYNK_TEMPLATE_NAME "Du bao thoi tiet"
#define BLYNK_AUTH_TOKEN "KyHKrXtZK8LD4hRdMcv54QF1GB4IYRfs"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SFE_BMP180.h>

#define LDR 4
#define TH 5
#define Rain 36
#define LED_PIN 2

// Tạo biến cho áp suất
double T, P;
char status;

// Khởi tạo màn hình LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Tạo đối tượng cho cảm biến BMP180
SFE_BMP180 bmp;

DHT dht(TH, DHT22);  // Sử dụng DHT22 cho cảm biến DHT
BlynkTimer timer;

char ssid[] = "nguyenhai";
char pass[] = "23456788";

void setup() {
  pinMode(LED_PIN, OUTPUT);
  // Bộ định tuyến đang dùng
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  bmp.begin();
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(LDR, INPUT);
  pinMode(Rain, INPUT);
  analogReadResolution(12);

  lcd.setCursor(0, 0);
  lcd.print("   HT Theo Doi");
  lcd.setCursor(4, 1);
  lcd.print("Thoi Tiet");
  delay(4000);
  lcd.clear();
}

// Lấy giá trị từ cảm biến DHT22
void DHT22sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  int roundedT = int(t);  // Làm tròn nhiệt độ thành số nguyên
  int roundedH = int(h);  // Làm tròn độ ẩm thành số nguyên

  Blynk.virtualWrite(V0, roundedT);
  Blynk.virtualWrite(V1, roundedH);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(roundedT);

  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(roundedH);
}

// Lấy giá trị từ cảm biến mưa
void rainSensor() {
  int Rvalue = analogRead(Rain);
  Rvalue = map(Rvalue, 0, 4095, 0, 100);
  Rvalue = (Rvalue - 100) * -1;
  Blynk.virtualWrite(V2, Rvalue);

  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(Rvalue);
  lcd.print(" ");
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
  lcd.setCursor(8, 1);
  lcd.print("P:");
  lcd.print(P);
}

// Lấy giá trị từ cảm biến LDR
void LDRsensor() {
  bool value = digitalRead(LDR);
  if (value == 1) {
    WidgetLED LED(V4);
    LED.on();
    digitalWrite(LED_PIN, HIGH); // Bật LED
  } else {
    WidgetLED LED(V4);
    LED.off();
    digitalWrite(LED_PIN, LOW); // Tắt LED
  }
}

void loop() {
  DHT22sensor();
  rainSensor();
  pressure();
  LDRsensor();
  Blynk.run(); // Chạy thư viện Blynk
}
