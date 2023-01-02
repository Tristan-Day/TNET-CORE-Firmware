#include <Arduino.h>
#include <Wire.h>

#define DEBUG

// Hardware Services
#include <Devices.hpp>
#include <BLE.hpp>

// BLE Services
#include <services/PowerManagement.hpp>

#include <services/Metronome.hpp>
#include <services/EnvironmentalSensing.hpp>
#include <services/HeartRate.hpp>

#define DEVICE_NAME "TNET-CORE"

void setup()
{
  Serial.begin(115200);
  Wire.begin(23, 19);

  // Initalise System Devices
  BME280::initialise();
  MAX30105::initialise();
  Haptics::initialise();

#ifdef DEBUG
  Serial.println("[INFO] All System Devices Initialsed");
#endif

  // Initalise Bluetooth
  Bluetooth::initialise(DEVICE_NAME);

  // Seccondary Services
  Metronome::createService(Bluetooth::pServer);
  EnvironmentalSensing::createService(Bluetooth::pServer);
  HeartRateService::createService(Bluetooth::pServer, Bluetooth::pAdvertising);

#ifdef DEBUG
  Serial.println("[INFO] Starting Power Management Service");
#endif

  // Primary Services
  PowerManagementService::createService(Bluetooth::pServer);

#ifdef DEBUG
  Serial.println("[INFO] Starting Services");
#endif

  // Start Manual Services
  vTaskResume(EnvironmentalSensing::taskHandle);
  vTaskResume(HeartRateService::taskHandle);

#ifdef DEBUG
  Serial.println("[INFO] All BLE Services Started");
#endif

  // Start Advertising
  Bluetooth::startAdvertising();
}

void loop()
{
  delay(10000);
}