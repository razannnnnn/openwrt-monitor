#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Wifi Gratis";
const char* password = "12345678";
const char* apiURL = "http://192.168.1.1/api/api.php";

LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 2000;
bool boardPrinted = false;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("OPENWRT");
  lcd.setCursor(0, 1);
  lcd.print("MONITORING");
  delay(1000);
  lcd.clear();


  WiFi.begin(ssid, password);
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  lcd.clear();
  lcd.print("WiFi Connected");
  delay(1000);
  lcd.clear();
}

void loop() {
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();

    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      http.begin(client, apiURL);
      int httpCode = http.GET();

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
          float temp = doc["temperature_celsius"];
          long rx = doc["br_lan"]["rx_megabytes"];
          const char* board = doc["model"];
          const char* used = doc["bandwidth_used"];

          if (!boardPrinted) {
            Serial.print("Board: ");
            Serial.println(board);
            boardPrinted = true;
          }

          Serial.print("Suhu: ");
          Serial.print(temp);
          Serial.print(" °C, RX: ");
          Serial.print(rx);
          Serial.print(" MB, Total: ");
          Serial.println(used);

          lcd.setCursor(0, 0);
          lcd.print("Suhu: ");
          lcd.print(temp, 1);
          lcd.print((char)223);
          lcd.print("C");

          lcd.setCursor(0, 1);
          lcd.print("Total: ");
          lcd.print(used);
        }
      }

      http.end();
    }
  }
}
