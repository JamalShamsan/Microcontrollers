import serial
import canalystii
import time

ser = serial.Serial('COM26', 19200)
ser.flushInput()

dev = canalystii.CanalystDevice(bitrate=500000)

counter = 0

while counter < 100:
    # 1. Read temperature from Arduino
    ser_bytes = ser.readline()
    decoded = ser_bytes.decode().strip()
    parts = decoded.split()
    temperature = float(parts[0])

    # 2. Convert temperature to integer
    temp_scaled = int(round(temperature * 100))

    # 3. Split into two bytes
    high_byte = (temp_scaled >> 8) & 0xFF
    low_byte = temp_scaled & 0xFF

    # 4. Create payload
    payload = (
        high_byte,
        low_byte,
        1,              # sensor ID
        counter & 0xFF, # counter
        0, 0, 0, 0
    )

    # 5. Create CAN message
    new_message = canalystii.Message(
        can_id=0x120,
        remote=False,
        extended=False,
        data_len=8,
        data=payload
    )

    # 6. Send on CAN1
    dev.send(1, new_message)

    # 7. Receive on CAN2
    received = None
    time.sleep(0.1)

    for msg in dev.receive(0):
        received = msg
        print("Temperature:", temperature, "°C")
        print("Sent payload:", payload)
        print("Received:", msg)

    counter += 1

dev.stop(0)
dev.stop(1)
ser.close()
del dev