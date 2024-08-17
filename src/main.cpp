#include <Arduino.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxEPD2_3C.h>
#include <Adafruit_SHT31.h>
#include "images.h"

ADC_MODE(ADC_VCC);
Adafruit_SHT31 sht31 = Adafruit_SHT31();
GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display(GxEPD2_213_Z98c(D8, D3, D4, D6));

void battery() {
    char batteryVoltage[4];
    itoa(ESP.getVcc(), batteryVoltage, 10);
    long batteryLevel = map(ESP.getVcc(), 2200, 3041, 0, 100);
    Serial.printf("Battery voltage: %s\n", batteryVoltage);
    Serial.printf("Battery level: %ld\n", batteryLevel);

    if (batteryLevel > 30) {
        display.setTextColor(GxEPD_BLACK);
    } else {
        display.setTextColor(GxEPD_RED);
    }
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(60, 117);
    display.printf("%smV", batteryVoltage);
    display.setCursor(140, 117);
    display.printf("%ld%s", batteryLevel, "%");
}

void temperatureAndHumidity() {
    if (!sht31.begin(0x44)) {
        Serial.println("Couldn't find SHT31");
        display.setCursor(10, 35);
        display.setTextColor(GxEPD_RED);
        display.setFont(&FreeMonoBold9pt7b);
        display.print("Couldn't find SHT31");
    } else {
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold18pt7b);
        float t = sht31.readTemperature();
        float h = sht31.readHumidity();
        Serial.printf("Temperature: %fºC\n", t);
        Serial.printf("Humidity: %f%s\n", h, "%");
        display.drawBitmap(30, 5, temperature_ico, 40, 40, GxEPD_RED);
        display.setCursor(67, 35);
        display.printf("%2.2fC", t);
        display.drawBitmap(70, 55, humidity_ico, 40, 40, GxEPD_RED);
        display.setCursor(110, 85);
        display.printf("%d%s", (int)h, "%");
    }
}

void setup() {
    display.init(115200);
    display.setRotation(3);
    display.setFullWindow();
    display.firstPage();
    display.fillScreen(GxEPD_WHITE);
    battery();
    temperatureAndHumidity();
    while (display.nextPage()) {
    }
    display.hibernate();
    ESP.deepSleep(900000000);
}

void loop() {};
