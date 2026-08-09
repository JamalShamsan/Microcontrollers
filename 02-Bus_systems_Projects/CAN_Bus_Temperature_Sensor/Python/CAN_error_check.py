import serial
import canalystii
import time

# Serial connection to Arduino / temperature sensor
ser = serial.Serial('COM26', 19200)
ser.flushInput()

# CAN connection
dev = canalystii.CanalystDevice(bitrate=500000)

CAN_ID = 0x120
SENSOR_ID = 1
TOTAL_MESSAGES = 100

correct_messages = 0
error_messages = 0
lost_messages = 0

for counter in range(TOTAL_MESSAGES):

    # 1. Read temperature from Arduino
    ser_bytes = ser.readline()
    decoded = ser_bytes.decode().strip()
    parts = decoded.split()
    temperature = float(parts[0])

    # 2. Convert temperature to integer
    temp_scaled = int(round(temperature * 100))

    # 3. Split temperature into two bytes
    high_byte = (temp_scaled >> 8) & 0xFF
    low_byte = temp_scaled & 0xFF

    # 4. Build CAN payload
    sent_payload = (
        high_byte,
        low_byte,
        SENSOR_ID,
        counter & 0xFF,
        0, 0, 0, 0
    )

    # 5. Create CAN message
    new_message = canalystii.Message(
        can_id=CAN_ID,
        remote=False,
        extended=False,
        data_len=8,
        data=sent_payload
    )

    # 6. Send CAN message on channel 1
    dev.send(1, new_message)

    # 7. Try to receive message on channel 0
    time.sleep(0.05)

    received_message = None

    for msg in dev.receive(0):
        received_message = msg
        break

    # 8. Automatic check
    if received_message is None:
        lost_messages += 1
        print("Message", counter, ": LOST")

    else:
        received_payload = tuple(received_message.data)

        if received_message.can_id == CAN_ID and received_payload == sent_payload:
            correct_messages += 1
            print("Message", counter, ": OK",
                  "Temperature:", temperature,
                  "Sent:", sent_payload,
                  "Received:", received_payload)
        else:
            error_messages += 1
            print("Message", counter, ": ERROR")
            print("Sent ID:", hex(CAN_ID), "Sent payload:", sent_payload)
            print("Received:", received_message)

# 9. Final result
total_errors = error_messages + lost_messages
error_rate = (total_errors / TOTAL_MESSAGES) * 100

print("\n--- Final Result ---")
print("Sent messages:", TOTAL_MESSAGES)
print("Correct messages:", correct_messages)
print("Wrong messages:", error_messages)
print("Lost messages:", lost_messages)
print("Error rate:", error_rate, "%")

# Clean up
dev.stop(0)
dev.stop(1)
ser.close()
del dev