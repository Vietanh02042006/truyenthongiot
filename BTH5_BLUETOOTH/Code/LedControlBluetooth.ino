
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <Adafruit_NeoPixel.h>

// LED RGB onboard ESP32-S3
#define LED_PIN 48
#define LED_COUNT 1

Adafruit_NeoPixel rgb(
  LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800
);

#define SERVICE_UUID "12345678-1234-1234-1234-123456789abc"
#define CHAR_UUID "abcdefab-1234-1234-1234-abcdefabcdef"

class ServerCallbacks : public BLEServerCallbacks {
  void onDisconnect(BLEServer *server) override {
    BLEDevice::startAdvertising();
    Serial.println("Mat ket noi!");
  }

  void onConnect(BLEServer *server) override {
    Serial.println("Da ket noi Bluetooth!");
  }
};

void setRGB(int r, int g, int b) {
  rgb.setPixelColor(0, rgb.Color(r, g, b));
  rgb.show();
}

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) override {
    String data = characteristic->getValue();

    Serial.print("Nhan: ");
    Serial.println(data);

    if (data == "1") {
      setRGB(255, 0, 0);       // Do
    }
    else if (data == "2") {
      setRGB(0, 255, 0);       // Xanh la
    }
    else if (data == "3") {
      setRGB(0, 0, 255);       // Xanh duong
    }
    else if (data == "4") {
      setRGB(255, 255, 255);   // Trang
    }
    else if (data == "0") {
      setRGB(0, 0, 0);         // Tat
    }
  }
};

void setup() {
  Serial.begin(115200);

  rgb.begin();
  rgb.setBrightness(50);
  setRGB(0, 0, 0);

  BLEDevice::init("ESP32_LED");

  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  BLEService *service =
    server->createService(SERVICE_UUID);

  BLECharacteristic *characteristic =
    service->createCharacteristic(
      CHAR_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_WRITE
    );

  characteristic->setCallbacks(new MyCallbacks());
  characteristic->setValue("0");

  service->start();

  BLEAdvertising *advertising =
    BLEDevice::getAdvertising();

  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.println("ESP32 BLE RGB READY!");
}

void loop() {
  delay(100);
}
