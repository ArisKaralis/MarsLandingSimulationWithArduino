#include <MPU6050_tockn.h>

#include <NewPing.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

// Define pins for HC-SR04 sensor
#define TRIGGER_PIN 4
#define ECHO_PIN 3

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

  // // Set LED pins as output and turn them off initially
  // pinMode(BLUE_PIN, OUTPUT);
  // digitalWrite(BLUE_PIN, LOW);

  // pinMode(GREEN_PIN, OUTPUT);
  // digitalWrite(GREEN_PIN, LOW);

  // pinMode(YELLOW_PIN, OUTPUT);
  // digitalWrite(YELLOW_PIN, LOW);

  // pinMode(WHITE_PIN, OUTPUT);
  // digitalWrite(WHITE_PIN, LOW);

  // pinMode(BLUE_PIN, OUTPUT);
  // digitalWrite(BLUE_PIN, LOW);

  pinMode(BLUE_PIN, OUTPUT);

  pinMode(GREEN_PIN, OUTPUT);
  // Set buzzer pin as output and turn it off initially
  pinMode(RED_PIN, OUTPUT);

  // Configure the photoresistor pin as an input
  pinMode(PHOTO_RESISTOR_PIN, INPUT);

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
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

  // Check the distance and turn on/off the LED lights accordingly
  Serial.print("Light Level: ");
  Serial.print(photoresistorValue);
  Serial.print(",Distance: ");
  Serial.println(distance);

  mpu6050.update();
  // Print the X, Y and Z
  Serial.print("Gyro X: ");
  Serial.print(mpu6050.getGyroX());
  Serial.print(" | Gyro Y: ");
  Serial.print(mpu6050.getGyroY());
  Serial.print(" | Gyro Z: ");
  Serial.println(mpu6050.getGyroZ());

  // Colour initialisation
  setColor(0, 0, 0);

  if (distance > Parachute_Deploy)
  { // Parachute Deployment
    // RGB BLUE
    setColor(0, 0, 255);

    Serial.println("Preparing for Parachute Deployment");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > Heat_Shield_Separation && distance <= Parachute_Deploy)
  { // Heat Shield Separation
    // TRGB GREEN
    setColor(0, 255, 0);

    Serial.println("Parachute Deployment Completed");
    Serial.println("Preparing for Heat Shield Separation");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > Radar_Lock && distance <= Heat_Shield_Separation)
  { // Radar Lock
    // RGB YELLOW
    setColor(255, 255, 0);

    Serial.println("Heat Shield Separation Completed");
    Serial.println("Preparing for Radar Lock");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > Terrain_Relative_Navigation && distance <= Radar_Lock)
  { // Terrain Relative Navigation
    // RGB WHITE
    setColor(255, 255, 255);
    Serial.println("Radar Lock Completed");
    Serial.println("Preparing for Terrain Relative Navigation");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > Backshell_Separation && distance <= Terrain_Relative_Navigation)
  { // Backshell Separation
    // RGB RED
    setColor(255, 0, 0);

    Serial.println("Terrain Relative Navigation Completed");
    Serial.println("Preparing for Backshell Separation");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > Rover_Separation && distance < Backshell_Separation)
  { // Rover Separation
    // RGB AQUA
    setColor(0, 255, 255);

    Serial.println("Backshell Separation Completed");
    Serial.println("Preparing for Rover Separation");
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

    Serial.println("Rover Separation Completed");
  }

  delay(1000);
}