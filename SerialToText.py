import serial
import time

arduino_port = "COM6"
baud_rate = 9600

ser = serial.Serial(arduino_port, baud_rate)

with open("sensor_data.txt", "a") as file:
    try:
        while True:
            data = ser.readline().decode().strip()

            # Combine the timestamp and sensor data
            timestamped_data = f"{timestamp} - {data}"
            
            file.write(data + "\n")
            print(data)

            time.sleep(0.1)
    except KeyboardInterrupt:
        ser.close()
        print("\nSerial connection closed.")
