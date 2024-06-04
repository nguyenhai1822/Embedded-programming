#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);
// SCL Chân 22 SDA chân 21

void setup() {
  // Khởi tạo màn hình OLED
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  // Xóa màn hình
  display.clearDisplay();

  // Đặt màu chữ là trắng
  display.setTextColor(WHITE);

  // Đặt kích thước chữ là 2
  display.setTextSize(2);

  // Đặt vị trí con trỏ văn bản
  display.setCursor(5, 5);

  // In ra màn hình
  display.println("Hai dep zai");

  // Hiển thị nội dung lên màn hình
  display.display();
}

void loop() {
  // Loop trống do không có chức năng gì cả
}
