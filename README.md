# EDL Control System

This is the Entry, Descent and Landing Control System software for the Mars Landing Simulation project.

## Requirements

- Arduino MEGA2560
- MPU6050 gyroscope and accelerometer
- HC-SR04 ultrasonic distance sensor
- 3 LED lights (red, green, blue)
- Buzzer
- Photoresistor(photocell)
- SD card reader
- SD card module
- Install Arduino IDE
- Install Python

## Installation

1. Clone this repository
2. Open `UP958669.ino` in the Arduino IDE
3. Install the necessary libraries:
   - MPU6050_tockn
   - NewPing
   - SD
4. Connect the hardware components according to the schematic diagram
5. Upload the code to the Arduino MEGA2560
6. Monitor the output on the serial monitor and the SD card

## Usage

The Mission Control System reads data from the sensors and writes it to an SD card. It also controls the LED lights and buzzer based on the distance measured by the ultrasonic sensor.

The different stages of the mission are indicated by the LED lights and buzzer as follows:

- Parachute Deployment: Blue LED
- Heat Shield Separation: Green LED
- Radar Lock: Yellow LED
- Terrain Relative Navigation: White LED
- Backshell Separation: Red LED
- Rover Separation: Aqua LED

When the rover has landed, the LED lights will cycle through all colours and the buzzer will sound for 3 seconds.

## Contributing

Contributions to this project are welcome. If you find any issues or have suggestions for improvements, please create a branch and submit a pull request.

## License

This project was developed by Aris Karalis as part of the Final Year Project in Computer Science at the Univeristy of Portsmouth.
