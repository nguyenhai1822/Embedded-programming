
// Include the library files
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define sensor 33
#define relay 4

// Initialize the OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR    0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  // Debug console
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // Clear the display
  display.clearDisplay();

  // Set text color to white
  display.setTextColor(WHITE);

  // Set text size to 2
  display.setTextSize(2);

  // Display startup message on the OLED screen
  display.setCursor(1, 0);
  display.print("System Loading");
  display.display();
  delay(2000);

  // Clear the display
  display.clearDisplay();
}
//độ ẩm đất
void doamdat() {
  int value = analogRead(sensor);
  value = map(value, 0, 4095, 0, 100);
  value = (value - 100) * -1;
  Serial.println(value);

  // Display moisture value on the OLED screen
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Moisture :");
  display.setCursor(5, 20);
  display.print(value);
  display.display();
  if(value>50){
    digitalWrite(relay, LOW);
  }else {
    digitalWrite(relay, HIGH);
  }
}

void loop() {
  doamdat();
  delay(200);
}
