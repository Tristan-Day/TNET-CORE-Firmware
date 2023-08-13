#pragma once

#include <hardware/Bluetooth.hpp>

#include <Arduino.h>
#include <BLE2902.h>

#include <map>

using namespace std;

class Console : private BLECharacteristicCallbacks
{
  private:
    static constexpr char DELIMITER = 0x20;

    static const string SERVICE_UUID;

    static const string INPUT_UUID;
    static const string OUTPUT_UUID;

    BLEService* service;

    BLECharacteristic* inputCharacteristic;
    BLECharacteristic* outputCharacteristic;

    std::map<std::string, function<string()>> runnables;
    std::map<std::string, function<string(string)>> consumers;

    void onWrite(BLECharacteristic* characteristic);

  public:
    Console();

    void addCommand(string name, function<string()> function);

    void addCommand(string name, function<string(string)> function);
};