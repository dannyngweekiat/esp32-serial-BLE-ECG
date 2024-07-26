/*
  Developed with reference to the following:
  - ESP32 BLE Arduino: BLE UART

  Created by: Danny Ng - 2023
*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "NotchFilter.hpp"
#include "MovingAverage.hpp"
#include "SimpleQueue.hpp"
#include "FIR.hpp"

#define ECG_ANALOG 4
#define ECG_LEADOFF_MINUS 16
#define ECG_LEADOFF_PLUS 17

#define LeadOffCheck(pin) (digitalRead(pin) == HIGH)

#define BUILTIN_LED 2

#define ECG_SAMPLING_RATE 1000
#define ECG_SAMPLETIME_IN_MS (1000 / ECG_SAMPLING_RATE)
#define ECG_FRAME_SIZE 100 / ECG_SAMPLETIME_IN_MS

#define MOVING_AVERAGE_LENGTH 128

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

hw_timer_t *acquisitionTimer = NULL;

FIRCoefficients *movingAverageCoefficients = createMovingAverageCoefficients(MOVING_AVERAGE_LENGTH);
FIRCoefficients *notchFilterCoefficients = createNotchFilterCoefficients();

FIR movingAverageFilter(movingAverageCoefficients);
FIR notchFilter(notchFilterCoefficients);

#define BLE_DEVICE_NAME "ECG Serial BLE"

// Enable only 1 out of the following 3 options
#define SHOW_NOTCH_FILTER_RESULT
// #define SHOW_MOVING_AVERAGE_RESULT
// #define SHOW_RAW_RESULT

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class ECGBLEServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

class ECGBLEReceiveCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string rxValue = pCharacteristic->getValue();

    // Show received data to serial. Not use for current code
    if (rxValue.length() > 0)
    {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++)
        Serial.print(rxValue[i]);

      Serial.println();
      Serial.println("*********");
    }
  }
};

bool isError = false;

SimpleQueue<int> ecgQueue(60);

bool isLeadOff()
{
  return LeadOffCheck(ECG_LEADOFF_MINUS) || LeadOffCheck(ECG_LEADOFF_PLUS);
}

void IRAM_ATTR onTimer()
{
  // Only start sampling if device is connected
  if (deviceConnected)
  {
    double value = analogRead(ECG_ANALOG);
    bool success = ecgQueue.enqueue(value);
  }
}

void setup()
{
  pinMode(ECG_LEADOFF_PLUS, INPUT);
  pinMode(ECG_LEADOFF_MINUS, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);

  digitalWrite(BUILTIN_LED, LOW);

  Serial.begin(115200);

  // Create timer
  acquisitionTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(acquisitionTimer, &onTimer, true);
  timerAlarmWrite(acquisitionTimer, ECG_SAMPLETIME_IN_MS * 1000, true);
  timerAlarmEnable(acquisitionTimer); // Just Enable

  // Create the BLE Device
  BLEDevice::init(BLE_DEVICE_NAME);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ECGBLEServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY);

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE);

  pRxCharacteristic->setCallbacks(new ECGBLEReceiveCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();

  Serial.println("Moving Average Filter Coefficients:");
  for (int i = 0; i < movingAverageCoefficients->size(); i++)
  {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print((*movingAverageCoefficients)[i]);
    if ((i + 1) % 10 == 0)
      Serial.println();
    else
      Serial.print(", ");
  }
  Serial.println();
  Serial.println("Notch Filter Coefficients:");
  for (int i = 0; i < notchFilterCoefficients->size(); i++)
  {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print((*notchFilterCoefficients)[i]);
    if ((i + 1) % 10 == 0)
      Serial.println();
    else
      Serial.print(", ");
  }
  Serial.println();

  Serial.println("Waiting a client connection to notify...");
}

void loop()
{
  static int dataCount = 0;
  static int ecgFrameCount = 0;
  static unsigned long prevMillis = millis();
  static String dataToSend = "";

  if (deviceConnected)
  {
    if (isLeadOff())
    {
      // Blink LED if lead off
      unsigned long currentMillis = millis();
      if (currentMillis - prevMillis > 500)
      {
        digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
        prevMillis = currentMillis;
      }
    }
    else
      digitalWrite(BUILTIN_LED, HIGH);

    while (ecgQueue.size() > 0)
    {
      int ecgValue;
      ecgQueue.dequeue(ecgValue);

      double averageResult = movingAverageFilter.compute(ecgValue);
      double notchResult = notchFilter.compute(averageResult);
#ifdef SHOW_NOTCH_FILTER_RESULT
      dataToSend += String((int)notchResult) + "\n";
#endif
#ifdef SHOW_MOVING_AVERAGE_RESULT
      dataToSend += String((int)averageResult) + "\n";
#endif
#ifdef SHOW_RAW_RESULT
      dataToSend += String((int)ecgValue) + "\n";
#endif
      if (++dataCount == ECG_FRAME_SIZE)
        break;
    }

    if (dataCount == ECG_FRAME_SIZE)
    {
      pTxCharacteristic->setValue(dataToSend.c_str());
      pTxCharacteristic->notify();
      Serial.print("Sending ECG frame #");
      Serial.print(ecgFrameCount++);
      Serial.print(" length: ");
      Serial.println(dataCount);
      Serial.print("Unprocessed Data: ");
      Serial.println(ecgQueue.size());
      dataToSend = "";
      dataCount = 0;
    }
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);                     // give the bluetooth stack the chance to get things ready
    ecgQueue.clear();               // clear queue
    dataCount = 0;                  // reset data count
    ecgFrameCount = 0;              // reset frame count
    digitalWrite(BUILTIN_LED, LOW); // turn off LED
    pServer->startAdvertising();    // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }

  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    oldDeviceConnected = deviceConnected;
  }
}
