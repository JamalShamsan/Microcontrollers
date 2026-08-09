# GNSS UART Interface with Arduino and Python

GNSS sensor project using a **Waveshare L76K GPS HAT**, **Arduino Nano**, and **Python**.

## Features

- UART communication at 9600 baud
- NMEA data acquisition
- Parsing of GGA and RMC messages
- Latitude and longitude conversion to decimal degrees
- GNSS data logging
- Mean and standard deviation calculation
- Position accuracy analysis with histograms

## Hardware

- Arduino Nano
- Waveshare L76K GPS HAT
- GNSS antenna
- Breadboard

## Software

- Arduino IDE
- Python
- PySerial
- NumPy
- Matplotlib


```

The Arduino forwards GNSS data via UART to the PC, where Python is used to record, parse, and analyze the NMEA messages.
