# CAN Bus Temperature Sensor Network

This project investigates **CAN bus communication** using a USB-CAN interface and a **1-Wire temperature sensor** connected through an Arduino.

## Overview

The Arduino reads the temperature sensor and sends the measured value to the PC over USB serial. Python then converts the temperature into CAN payload bytes and transmits the data through a USB-CAN adapter.

The project also includes CAN loopback testing, CAN parameter configuration, automatic transmission checking, and communication on a larger CAN network.

## Main Topics

- CAN loopback communication
- CAN ID and payload configuration
- Standard and extended CAN frames
- Bitrate comparison
- CANH and CANL physical-layer measurements
- Temperature encoding into CAN payload bytes
- Automatic error and message-loss detection
- Communication on a multi-node CAN network

## Hardware

- Arduino
- 1-Wire temperature sensor
- USB-CAN adapter
- Oscilloscope
- Jumper wires

## Software

- Arduino IDE
- Python 3
- PySerial
- canalystii

## Project Structure

```text
CAN_Bus_Temperature_Sensor/
├── Arduino/
│   └── Temperature_Sensor_1Wire/
│       └── Temperature_Sensor_1Wire.ino
│
├── Python/
│   ├── CAN_loopback_test.py
│   ├── CAN_listener.py
│   ├── temperature_to_CAN.py
│   ├── CAN_error_check.py
│   └── CAN_network_sender.py
│
├── Documentation/
│   └── CAN_Bus_Lab_Report.pdf
│
└── README.md
```


## CAN Configuration

The project uses standard CAN frames with configurable bitrate, CAN identifier, payload length, and data bytes. Temperature values are scaled and split into payload bytes before transmission.
