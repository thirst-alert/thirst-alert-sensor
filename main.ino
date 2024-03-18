#include "src/WiFi/src/WiFi.h"
#include <NimBLEDevice.h>

#define STATUS_OK "O"
#define STATUS_NOTOK "N"

#define SERVICE_UUID "61806eea-01a4-4b11-b4bd-6e588e84eea3"
#define APCHARACTERISTIC_UUID "270e61b7-9986-4eb4-a686-d7750dcd1435"
#define IDCHARACTERISTIC_UUID "db4b9b09-0a07-44c7-8ebf-9ec351851aaa"
#define STATUSCHARACTERISTIC_UUID "a00879e9-ad2e-4431-b7fb-1fccebe98b6b"

static NimBLECharacteristic* apCharacteristic;
static NimBLECharacteristic* statusCharacteristic;

class ServerCallbacks: public NimBLEServerCallbacks {
    void onDisconnect(NimBLEServer* pServer) {
      delay(500);
      NimBLEDevice::startAdvertising();
    }
};

void checkAPWrite(void * pvParameters) {
  std::string value = apCharacteristic->getValue();
  // [Byte 1: SSID Length][Bytes 2 to (2 + SSID Length - 1): SSID][Remaining Bytes: Password]
  if (value.length() < 2) {
    statusCharacteristic->setValue(STATUS_NOTOK);
    statusCharacteristic->notify();
    vTaskDelete(NULL);
  }
  uint8_t ssidLength = value[0];
  if (ssidLength >= value.length() - 1) {
    statusCharacteristic->setValue(STATUS_NOTOK);
    statusCharacteristic->notify();
    vTaskDelete(NULL);
  }

  std::string ssid = value.substr(1, ssidLength);
  std::string password = value.substr(ssidLength + 1);

  WiFi.begin(ssid.c_str(), password.c_str());
  uint8_t status = WiFi.waitForConnectResult(10000);
  if (status != WL_CONNECTED) {
    statusCharacteristic->setValue(STATUS_NOTOK);
    statusCharacteristic->notify();
    WiFi.disconnect();
    vTaskDelete(NULL);
  }
  WiFi.disconnect();

  // save credentials to preferences

  statusCharacteristic->setValue(STATUS_OK);
  statusCharacteristic->notify();
  vTaskDelete(NULL);
}

class ApCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* characteristic) {
      xTaskCreatePinnedToCore(
        checkAPWrite,
        "checkAPWrite",
        4000,
        NULL,
        0,
        NULL,
        0
      );
    }
};

class IdCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* characteristic) {
      std::string value = characteristic->getValue();
      if (value.length() != 24) {
        statusCharacteristic->setValue(STATUS_NOTOK);
        statusCharacteristic->notify();
        return;
      }
      // save id to preferences
      statusCharacteristic->setValue(STATUS_OK);
      statusCharacteristic->notify();
    }
};

void setup() {
  NimBLEDevice::init("ThirstAlert Sensor");
  NimBLEServer* pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  NimBLEService* pService = pServer->createService(SERVICE_UUID);
  apCharacteristic = pService->createCharacteristic(
                       APCHARACTERISTIC_UUID,
                       NIMBLE_PROPERTY::READ |
                       NIMBLE_PROPERTY::WRITE
                     );
  apCharacteristic->setCallbacks(new ApCharacteristicCallbacks());
  NimBLECharacteristic* idCharacteristic = pService->createCharacteristic(
                                          IDCHARACTERISTIC_UUID,
                                          NIMBLE_PROPERTY::READ |
                                          NIMBLE_PROPERTY::WRITE
                                        );
  idCharacteristic->setCallbacks(new IdCharacteristicCallbacks());
  statusCharacteristic = pService->createCharacteristic(
                           STATUSCHARACTERISTIC_UUID,
                           NIMBLE_PROPERTY::READ |
                           NIMBLE_PROPERTY::NOTIFY
                         );
  pService->start();
   NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  pAdvertising->start();
}

void loop() {
  delay(2000);
}
