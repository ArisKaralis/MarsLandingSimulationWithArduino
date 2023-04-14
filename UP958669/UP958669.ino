#include <MPU6050_tockn.h>
#include <NewPing.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

MPU6050 mpu6050(Wire);

// Define pins for HC-SR04 sensor
#define TRIGGER_PIN 10
#define ECHO_PIN 11

// Define pins for LED lights
#define BLUE_PIN 7
#define GREEN_PIN 8
#define RED_PIN 9

// Define pin for buzzer
#define BUZZER_PIN 2

// Define constants for distance thresholds
#define Parachute_Deploy 200           // cm
#define Heat_Shield_Separation 170     // cm
#define Radar_Lock 123                 // cm
#define Terrain_Relative_Navigation 62 // cm
#define Backshell_Separation 33        // cm
#define Rover_Separation 10            // cm

// Define constants for Photoresistor(photocell)s
#define PHOTO_RESISTOR_PIN A0

// Define pins for SD card module
#define SD_CS_PIN 53
#define SD_MISO_PIN 51
#define SD_MOSI_PIN 50
#define SD_SCK_PIN 52

NewPing sonar(TRIGGER_PIN, ECHO_PIN, Parachute_Deploy);

// Define a variable to store the photoresistor value
int photoresistorValue;

void setColor(int red, int green, int blue)
{
  analogWrite(BLUE_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(RED_PIN, blue);
}

void setup()
{
  // Set trigger pin as output and echo pin as input
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);

  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  // Set buzzer pin as output and turn it off initially
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Configure the photoresistor pin as an input
  pinMode(PHOTO_RESISTOR_PIN, INPUT);

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  // SD card initialisation
  Serial.print("Initialising SD card...");
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialisation failed!");
    return;
  }
  Serial.println("initialisation done.");
}


void loop()
{

  

  

  // Send a short pulse to trigger the sensor
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measure the duration of the echo pulse
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance based on the speed of sound (343 m/s)
  float distance = (duration / 2) * (0.0343);

  // Read the photoresistor value
  photoresistorValue = analogRead(PHOTO_RESISTOR_PIN);

  mpu6050.update();
  unsigned long currentTime = millis();
  int hours = (currentTime / 3600000) % 24;
  int minutes = (currentTime / 60000) % 60;
  int seconds = (currentTime / 1000) % 60;

  char timestamp[12]; // The buffer for the formatted timestamp
  sprintf(timestamp, "%02d:%02d:%02d", hours, minutes, seconds);
  // The photoresistor's measurement is in Ohms because it is a variable resistor.
  // Its resistance changes based on the amount of light it is exposed to. When the light
  // intensity increases, the resistance decreases, and vice versa. Therefore, the output
  // value of the photoresistor represents its resistance in Ohms, which directly
  // correlates with the light intensity.
  Serial.print("Timestamp: ");
  Serial.println(timestamp);
  Serial.print("Light Level: ");
  Serial.print(photoresistorValue);
  Serial.print(" Ohms");
  Serial.print("\t| Distance: ");
  Serial.print(distance);
  Serial.print("cm");
  Serial.print("\t\t| Temperature: ");
  Serial.print(mpu6050.getTemp());
  Serial.println("°C");
  // Acceleration is measured in units of gravity (g),
  // where 1g is approximately 9.81(m/(s^2))
  Serial.print("Acceleration X: ");
  Serial.print(mpu6050.getAccX());
  Serial.print("g");
  Serial.print("\t| Acceleration Y: ");
  Serial.print(mpu6050.getAccY());
  Serial.print("g");
  Serial.print("\t| Acceleration Z: ");
  Serial.print(mpu6050.getAccZ());
  Serial.println("g");



  // Colour initialisation
  setColor(0, 0, 0);

  if (distance > Parachute_Deploy)
  { // Parachute Deployment
    // RGB BLUE
    setColor(0, 0, 255);
    Serial.print("Next stage: ");
    Serial.println("Parachute Deployment");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > Heat_Shield_Separation && distance <= Parachute_Deploy)
  { // Heat Shield Separation
    // TRGB GREEN
    setColor(0, 255, 0);
    Serial.print("Completed stage: ");
    Serial.println("Parachute Deployment");
    Serial.print("Next stage: ");
    Serial.println("Heat Shield Separation");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > Radar_Lock && distance <= Heat_Shield_Separation)
  { // Radar Lock
    // RGB YELLOW
    setColor(255, 255, 0);

    Serial.print("Completed stage: ");
    Serial.println("Heat Shield Separation");
    Serial.print("Next stage: ");
    Serial.println("Radar Lock");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > Terrain_Relative_Navigation && distance <= Radar_Lock)
  { // Terrain Relative Navigation
    // RGB WHITE
    setColor(255, 255, 255);

    Serial.print("Completed stage: ");
    Serial.println("Radar Lock");
    Serial.print("Next stage: ");
    Serial.println("Terrain Relative Navigation");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > Backshell_Separation && distance <= Terrain_Relative_Navigation)
  { // Backshell Separation
    // RGB RED
    setColor(255, 0, 0);

    Serial.print("Completed stage: ");
    Serial.println("Terrain Relative Navigation");
    Serial.print("Next stage: ");
    Serial.println("Backshell Separation");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > Rover_Separation && distance < Backshell_Separation)
  { // Rover Separation
    // RGB AQUA
    setColor(0, 255, 255);

    Serial.print("Completed stage: ");
    Serial.println("Backshell Separation");
    Serial.print("Next stage: ");
    Serial.println("Rover Separation");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance <= Rover_Separation)
  {
    // Iterate through the colors
    for (int r = 0; r <= 255; r += 51)
    {
      for (int g = 0; g <= 255; g += 51)
      {
        for (int b = 0; b <= 255; b += 51)
        {
          setColor(r, g, b);
          delay(50); // Adjust this value to change the speed of color transitions
        }
      }
    }

    digitalWrite(BUZZER_PIN, HIGH); // active the buzzer
    delay(3000);                    // delay of 3000 millisecond = 3s
    digitalWrite(BUZZER_PIN, LOW);  // deactivat the buzzer

    Serial.print("Completed stage: ");
    Serial.println("Rover Separation");
  }
  Serial.println("=====================================================================================");

   // Open the file to write data to SD card
  File dataFile = SD.open("data.txt", FILE_WRITE);

  // Check if the file is available for writing
  if (dataFile) {
    dataFile.println(timestamp);
    dataFile.print("Light Level: ");
    dataFile.print(photoresistorValue);
    dataFile.println(" Ohms");
    dataFile.print("Distance: ");
    dataFile.print(distance);
    dataFile.println("cm");
    dataFile.print("Temperature: ");
    dataFile.print(mpu6050.getTemp());
    dataFile.println("°C");
    dataFile.print("Acceleration X: ");
    dataFile.print(mpu6050.getAccX());
    dataFile.println("g");
    dataFile.print("Acceleration Y: ");
    dataFile.print(mpu6050.getAccY());
    dataFile.println("g");
    dataFile.print("Acceleration Z: ");
    dataFile.print(mpu6050.getAccZ());
    dataFile.println("g");
    dataFile.println("==========================");
    // Close the file after writing
    dataFile.close();
  } else {
    // If the file isn't open, pop up an error
    Serial.println("Error opening data.txt");
  }
  delay(1000);
}