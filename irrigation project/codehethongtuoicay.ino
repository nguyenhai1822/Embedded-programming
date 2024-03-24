
#define BLYNK_TEMPLATE_ID "TMPL6ujbdk8j1"
#define BLYNK_TEMPLATE_NAME "doamdat"
#define BLYNK_AUTH_TOKEN "nxmFlzQRwtEtsTJ9IOTLceyFFLsGLW7k"

char ssid[] = "nguyenhai";
char pass[] = "23456788"; 

int moistPerLow =   20 ; 
int moistPerHigh =   70 ; 

#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>  
#include <AceButton.h>
using namespace ace_button; 

// Define connections to sensor
#define SensorPin       34  //D34 chân cảm biến đất 
#define DHTPin          14  //D14
#define RelayPin        25  //D25 
#define wifiLed         2   //D2
#define RelayButtonPin  32  //D32
#define ModeSwitchPin   33  //D33
#define BuzzerPin       26  //D26
#define ModeLed         15  //D15

#define DHTTYPE DHT11    

#define VPIN_MoistPer    V1 
#define VPIN_TEMPERATURE V2
#define VPIN_HUMIDITY    V3
#define VPIN_MODE_SWITCH V4
#define VPIN_RELAY       V5

#define SCREEN_WIDTH 128 //định ngĩa oled 
#define SCREEN_HEIGHT 32 

#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int     sensorVal;  
int     moisturePercentage;
bool    toggleRelay = LOW;
bool    prevMode = true;
int     temperature1 = 0;
int     humidity1   = 0;
String  currMode  = "A";

char auth[] = BLYNK_AUTH_TOKEN;

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);

void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);

BlynkTimer timer;
DHT dht(DHTPin, DHTTYPE);

//Kiểm tra kết nối với Blynk 
void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    Serial.print("Blynk Not Connected ");
    digitalWrite(wifiLed, LOW);
  }
  if (isconnected == true) {
    digitalWrite(wifiLed, HIGH);
    //Serial.println("Blynk Connected");
  }
}

//Đồng bộ hóa dữ liệu với Blynk 
BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_MoistPer);
  Blynk.syncVirtual(VPIN_RELAY);
  Blynk.syncVirtual(VPIN_TEMPERATURE);
  Blynk.syncVirtual(VPIN_HUMIDITY);
  //Blynk.syncVirtual(VPIN_MODE_SWITCH);
  Blynk.virtualWrite(VPIN_MODE_SWITCH, prevMode);
}

BLYNK_WRITE(VPIN_RELAY) {
  if(!prevMode){
    toggleRelay = param.asInt();
    digitalWrite(RelayPin, toggleRelay);
  }
  else{
    Blynk.virtualWrite(VPIN_RELAY, toggleRelay);
  }
}

BLYNK_WRITE(VPIN_MODE_SWITCH) {
  if(prevMode !=  param.asInt()){
    prevMode =  param.asInt();
    currMode = prevMode ? "A" : "M";
    digitalWrite(ModeLed, prevMode);
    controlBuzzer(500);
    if(!prevMode && toggleRelay == HIGH){
      digitalWrite(RelayPin, LOW);
      toggleRelay = LOW;
      Blynk.virtualWrite(VPIN_RELAY, toggleRelay);
    }
   }   
}

void controlBuzzer(int duration){
  digitalWrite(BuzzerPin, HIGH);
  delay(duration);
  digitalWrite(BuzzerPin, LOW);
}

void displayData(String line1 , String line2){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(30,2);
  display.print(line1);
  display.setTextSize(1);
  display.setCursor(1,25);
  display.print(line2);
  display.display();
}

void getMoisture(){
  moisturePercentage = analogRead(SensorPin);
  moisturePercentage = map(moisturePercentage, 0, 4095, 0, 100); 
  moisturePercentage = 100 - moisturePercentage; 
  Serial.print("Moisture Percentage: ");
  Serial.print(moisturePercentage);
  Serial.println(" %");
  delay(50);
}

void getWeather(){
  
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else {
    humidity1 = int(h);
    temperature1 = int(t);
  }  
}

void sendSensor()
{ 
  getMoisture(); 
  getWeather(); 
  
  displayData(String(moisturePercentage) + " %", "T:" + String(temperature1) + " C,  H:" + String(humidity1) + " %  " + currMode);
  
  Blynk.virtualWrite(VPIN_MoistPer, moisturePercentage);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature1);
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity1);
}

void controlMoist(){
  if(prevMode){
    if (moisturePercentage < (moistPerLow)){
      if (toggleRelay == LOW){
        controlBuzzer(500);
        digitalWrite(RelayPin, HIGH);
        toggleRelay = HIGH;
        Blynk.virtualWrite(VPIN_RELAY, toggleRelay);
        delay(1000);
      }      
    }
    if (moisturePercentage > (moistPerHigh)){
      if (toggleRelay == HIGH){
        controlBuzzer(500);
        digitalWrite(RelayPin, LOW);
        toggleRelay = LOW;
        Blynk.virtualWrite(VPIN_RELAY, toggleRelay);
        delay(1000);
      } 
    } 
  }
  else{
    button1.check();
  }
}
 
void setup() {
  Serial.begin(115200);
  // Set pinmodes for GPIOs
  pinMode(RelayPin, OUTPUT);
  pinMode(wifiLed, OUTPUT);
  pinMode(ModeLed, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);

  pinMode(RelayButtonPin, INPUT_PULLUP);
  pinMode(ModeSwitchPin, INPUT_PULLUP);

  digitalWrite(wifiLed, LOW);
  digitalWrite(ModeLed, LOW);
  digitalWrite(BuzzerPin, LOW);

  dht.begin();    // Enabling DHT sensor

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  
  button1.init(RelayButtonPin);
  button2.init(ModeSwitchPin);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); 
  timer.setInterval(3000L, sendSensor); 
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //delay(1000);
  controlBuzzer(1000); 
  digitalWrite(ModeLed, prevMode);
}
 void loop() {

  Blynk.run();
  timer.run(); // Initiates SimpleTimer
  button2.check();
  controlMoist(); 
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventReleased:
      //Serial.println("kEventReleased");
      digitalWrite(RelayPin, !digitalRead(RelayPin));
      toggleRelay = digitalRead(RelayPin);
      Blynk.virtualWrite(VPIN_RELAY, toggleRelay);
      break;
  }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventReleased:
      if(prevMode && toggleRelay == HIGH){
        digitalWrite(RelayPin, LOW);
        toggleRelay = LOW;
        Blynk.virtualWrite(VPIN_RELAY, toggleRelay);
      }
      prevMode = !prevMode;
      currMode = prevMode ? "A" : "M";
      digitalWrite(ModeLed, prevMode);
      Blynk.virtualWrite(VPIN_MODE_SWITCH, prevMode);
      controlBuzzer(500);
      break;
  }
}
