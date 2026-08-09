# LSM6DS3 IMU Sensor Analysis

This project investigates the **STM LSM6DS3 6-axis IMU** using an **Arduino Nano**, **I2C**, and **Python**.

## Overview

The Arduino communicates with the LSM6DS3 over I2C, reads raw accelerometer and gyroscope data, and sends the measurements to a PC through USB serial. Python is then used to store, visualize, and analyze the sensor data.

## Main Topics

- I2C communication with the LSM6DS3
- Accelerometer and gyroscope measurements
- Measurement ranges: ±2 g, ±4 g, ±8 g, ±16 g
- Output Data Rate (ODR) analysis
- Sensor resolution, offset, and noise
- Oscilloscope analysis of I2C signals
- Time-series and histogram evaluation
- Mean, median, min/max, and standard deviation

## Hardware

- Arduino Nano
- Seeed Grove LSM6DS3 breakout board
- Breadboard
- Jumper wires
- Oscilloscope

## Connections

| LSM6DS3 | Arduino Nano |
|---|---|
| VCC | 5 V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

## Software

- Arduino IDE
- Python 3
- PySerial
- NumPy
- Matplotlib

## Project Structure

```text
LSM6DS3_IMU/
├── Arduino/
│   ├── LSM6DS3_lowlevel.ino
│   └── LSM6DS3.h
├── Python/
│   └── Read_USB_CSV_LimitedMeasurements.py
├── Documentation/
│   └── LSM6DS3_IMU_Lab_Report.pdf
├── Data/
│   └── measurements/
└── README.md
```

