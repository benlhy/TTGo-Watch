/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define LILYGO_TWATCH_2020_V1        // If you are using T-Watch-2020 version, please open this macro definition

#include <TTGO.h>
#include "time.h"
#include "touchscreen.h"
#include "accel.h"
#include "power.h"

TTGOClass *ttgo;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

// These are uniquely generated UUIDs

#define SERVICE_UUID        "8dd85644-72c7-4e85-a642-4b77c4b63ae4"
#define CHARACTERISTIC_UUID "f3fc9e52-3f8e-4cf4-8a11-decfc919868d"

#define INDOOR_LOCATION     "1821"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE!");

  BLEDevice::init("ESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(INDOOR_LOCATION);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("Hello World says Neil");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  ttgo_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(5);
  get_touch(ttgo);
  draw_screen(ttgo);
  process_power_irq(ttgo);
}


void ttgo_init() {
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();
  ttgo->lvgl_begin();
  setup_rtc(ttgo);
  setup_bma(ttgo);
  setup_power(ttgo);
}


