import serial
import csv
import re
import datetime

# Set up the serial connection to the Arduino
ser = serial.Serial('COM6', 9600)

# Create the CSV files for the data
distance_file = open('distance_data.csv', 'w', newline='')
distance_writer = csv.writer(distance_file)

light_file = open('light_data.csv', 'w', newline='')
light_writer = csv.writer(light_file)

acceleration_file = open('acceleration_data.csv', 'w', newline='')
acceleration_writer = csv.writer(acceleration_file)

# Write the headers for each file
distance_writer.writerow(['Timestamp', 'Distance (cm)'])
light_writer.writerow(['Timestamp', 'Light Level (Ohms)'])
acceleration_writer.writerow(['Timestamp', 'Acceleration X (g)', 'Acceleration Y (g)', 'Acceleration Z (g)'])

# Set up the regular expressions to extract the data from each line
distance_regex = re.compile(r"Distance:\s+(\d+)\.\d+\s+cm")
light_regex = re.compile(r"Light\s+Level:\s+(\d+)\s+Ohms")
acceleration_regex = re.compile(r"Acceleration X:\s+(-?\d+\.\d+)\s+g\s+\|\s+Acceleration Y:\s+(-?\d+\.\d+)\s+g\s+\|\s+Acceleration Z:\s+(-?\d+\.\d+)\s+g")

# Loop through the data from the Arduino and write it to the CSV files
while True:
    # Read a line of data from the serial port
    data = ser.readline().decode().strip()
    
    # Get the current timestamp
    timestamp = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')
    
    # Extract the distance data and write it to the CSV file
    distance_match = distance_regex.search(data)
    if distance_match:
        distance = distance_match.group(1)
        distance_writer.writerow([timestamp, distance])
    else:
        distance = None
    
    # Extract the light data and write it to the CSV file
    light_match = light_regex.search(data)
    if light_match:
        light = light_match.group(1)
        light_writer.writerow([timestamp, light])
    else:
        light = None
    
    # Extract the acceleration data and write it to the CSV file
    acceleration_match = acceleration_regex.search(data)
    if acceleration_match:
        acceleration_x = acceleration_match.group(1)
        acceleration_y = acceleration_match.group(2)
        acceleration_z = acceleration_match.group(3)
        acceleration_writer.writerow([timestamp, acceleration_x, acceleration_y, acceleration_z])
    else:
        acceleration_x = None
        acceleration_y = None
        acceleration_z = None
    
    # Print the data to the console
    print(f"{timestamp}\tDistance: {distance} cm\tLight Level: {light} Ohms\tAcceleration X: {acceleration_x} g\tAcceleration Y: {acceleration_y} g\tAcceleration Z: {acceleration_z} g")
