#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

// These are uniquely generated UUIDs

#define SERVICE_UUID        "8dd85644-72c7-4e85-a642-4b77c4b63ae4"
#define CHARACTERISTIC_UUID "f3fc9e52-3f8e-4cf4-8a11-decfc919868d"

#define INDOOR_LOCATION     "1821"


void setup_ble() {
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
  pAdvertising->setMinPreferred(0x100);  // functions that help with iPhone connections issue
  //pAdvertising->setMinPreferred(0x12);  // interval x 0.625 = interval in ms.
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

}


#endif 