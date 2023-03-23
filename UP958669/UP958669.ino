#include <NewPing.h>

// Define pins for HC-SR04 sensor
#define TRIGGER_PIN 13
#define ECHO_PIN 12

// Define pins for LED lights
#define BLUE_PIN 2
#define GREEN_PIN 3
#define YELLOW_PIN 4
#define WHITE_PIN 5
#define RED_PIN 6
#define rgB_PIN 7
#define rGb_PIN 8
#define Rgb_PIN 9

// Define pin for buzzer
#define BUZZER_PIN 10

// Define constants for distance thresholds

#define Parachute_Deploy 200       // cm
#define Heat_Shield_Separation 170 // cm
#define Radar_Lock 123             // cm
#define Terrain_Relative 62        // cm
#define Backshell_Separation 33    // cm
#define Rover_Separation 10        // cm

// Define constants for Photoresistor(photocell)
#define PHOTO_RESISTOR_PIN A0

NewPing sonar(TRIGGER_PIN, ECHO_PIN, DISTANCE_MAX);

// Define a variable to store the photoresistor value
int photoresistorValue;

void setup()
{
  // Set trigger pin as output and echo pin as input
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);

  // Set LED pins as output and turn them off initially
  pinMode(BLUE_PIN, OUTPUT);
  digitalWrite(BLUE_PIN, LOW);

  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(GREEN_PIN, LOW);

  pinMode(YELLOW_PIN, OUTPUT);
  digitalWrite(YELLOW_PIN, LOW);

  pinMode(WHITE_PIN, OUTPUT);
  digitalWrite(WHITE_PIN, LOW);

  pinMode(BLUE_PIN, OUTPUT);
  digitalWrite(BLUE_PIN, LOW);

  pinMode(rgB_PIN, OUTPUT);

  pinMode(rGb_PIN, OUTPUT);
  // Set buzzer pin as output and turn it off initially
  pinMode(Rgb_PIN, OUTPUT);

  // Configure the photoresistor pin as an input
  pinMode(PHOTO_RESISTOR_PIN, INPUT);
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

  // // Print the photoresistor value to the serial monitor
  // Serial.print("Photoresistor value: ");
  // Serial.println(photoresistorValue);

  // Check the distance and turn on/off the LED lights accordingly
  Serial.print("P: ");
  Serial.print(photoresistorValue);
  Serial.print(",U: ");
  Serial.println(distance);

  if (distance > Parachute_Deploy)
  {
    // Turn on only blue light
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(RED_PIN, LOW);

    Serial.println("Parachute Deploy");
    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > DISTANCE_MIN && distance <= DISTANCE_MAX)
  {
    // Turn on blue and yellow lights
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);

    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > DISTANCE_MIN && distance <= DISTANCE_MAX)
  {
    // Turn on blue and yellow lights
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);

    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > DISTANCE_MIN && distance <= DISTANCE_MAX)
  {
    // Turn on blue and yellow lights
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);

    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance > DISTANCE_MIN && distance <= DISTANCE_MAX)
  {
    // Turn on blue and yellow lights
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);

    // Turn off buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance <= DISTANCE_MIN)
  {
    // Turn on all lights
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH); // active the buzzer
    delay(3000);                    // delay of 3000 millisecond = 3s
    digitalWrite(BUZZER_PIN, LOW);  // deactivat the buzzer
  }

  delay(1000);
}