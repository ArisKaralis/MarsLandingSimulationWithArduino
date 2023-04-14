import re
import csv

# Read the data from the file
with open("D:\\DATA.TXT", "r", encoding="utf-8") as file:
    data = file.read()

# Normalize line endings
data = data.replace("\r\n", "\n")

# Define the regex pattern
pattern = r"(\d{2}:\d{2}:\d{2})\nLight Level: (\d+) Ohms\nDistance: ([\d.]+)cm\nTemperature: ([\d.]+)°C\nAcceleration X: ([-\d.]+)g\nAcceleration Y: ([-\d.]+)g\nAcceleration Z: ([-\d.]+)g\n={10,}"

# Find all matches
matches = re.findall(pattern, data)

if matches:
    # Write the extracted data to a CSV file
    with open("table.csv", "w", newline="", encoding="utf-8") as csvfile:
        csvwriter = csv.writer(csvfile)

        # Write the header row
        csvwriter.writerow(["Timestamp", "Light Level (Ohms)", "Distance (cm)",
                           "Temperature (°C)", "Acc X (g)", "Acc Y (g)", "Acc Z (g)"])

        # Iterate through the matches and write the data to the CSV file
        for match in matches:
            csvwriter.writerow(match)

    print("Data has been saved to table.csv.")
else:
    print("No match found.")
