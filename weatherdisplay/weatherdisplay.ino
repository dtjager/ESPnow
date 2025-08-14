#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// Structure to hold our weather data. Must match the sender code!
typedef struct struct_message {
  float temperature;
  float humidity;
  float pressure; // New: added pressure reading
} struct_message;

struct_message incomingReadings;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(20, 50);
  tft.println("Weather Station");

  tft.setTextSize(4);
  tft.setCursor(20, 150);
  tft.printf("Temp: %.1f F", incomingReadings.temperature); // Updated unit to F

  tft.setCursor(20, 220);
  tft.printf("Hum : %.1f %%", incomingReadings.humidity);

  tft.setCursor(20, 290); // New: Display pressure
  tft.printf("Pres: %.1f hPa", incomingReadings.pressure);
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Receiver Initializing...");

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("Waiting for data...");

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW initialized. Waiting for data...");
}

void loop() {
}