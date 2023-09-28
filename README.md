# TNET - HCI CORE

HCI CORE, is a custom hardware project built from the ESP32 MCU. Designed to expose various sensors and HCI devices over Bluetooth Low Energy (BLE), this firmware aims to maximise power efficiency using programming techniques and RTOS constructs.

## Bluetooth Low Energy

ESP32 Bluetooth LE functionality is implmented using the NimBLE library by h2zero. This provides many advantages over the inbuilt Bluedroid stack. 

All BLE services utilise standardised UUID's where possible, ensuring compatibility with other applications.

## Design Overview

The image below illustrates the current system design and BLE functionality.

![Software Design Overview](https://user-images.githubusercontent.com/41393868/216054765-8f7daae2-994f-4c5d-9077-1955b8a09a4e.png)