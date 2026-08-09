import serial
import canalystii
import time

# Arduino / temperature sensor
ser = serial.Serial('COM26', 19200)
ser.flushInput()

# CAN device
dev = canalystii.CanalystDevice(bitrate=500000)

# Use the CAN ID agreed with the other groups
CAN_ID = 0x120
SENSOR_ID = 1

counter = 0

while True:
    # Read temperature from Arduino
    ser_bytes = ser.readline()
    decoded = ser_bytes.decode().strip()
    parts = decoded.split()
    temperature = float(parts[0])

    # Convert temperature to integer
    temp_scaled = int(round(temperature * 100))

    # Split into two bytes
    high_byte = (temp_scaled >> 8) & 0xFF
    low_byte = temp_scaled & 0xFF

    # Create payload
    payload = (
        high_byte,
        low_byte,
        SENSOR_ID,
        counter & 0xFF,
        0, 0, 0, 0
    )

    # Create CAN message
    new_message = canalystii.Message(
        can_id=CAN_ID,
        remote=False,
        extended=False,
        data_len=8,
        data=payload
    )

    # Send on the common CAN bus
    dev.send(0, new_message)

    print("Sent CAN ID:", hex(CAN_ID))
    print("Temperature:", temperature, "°C")
    print("Payload decimal:", payload)
    print("Payload hex:", [hex(b) for b in payload])
    print("--------------------")

    counter += 1
    time.sleep(1)