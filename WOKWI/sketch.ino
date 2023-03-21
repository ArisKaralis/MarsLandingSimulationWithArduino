// Define pins for HC-SR04 sensor
#define TRIG_PIN 13
#define ECHO_PIN 12

// Define pins for LED lights
#define RED_PIN 9
#define YELLOW_PIN 8
#define BLUE_PIN 7

// Define pin for buzzer
#define BUZZER_PIN 6

// Define constants for distance thresholds
#define DISTANCE_MAX 200 // cm
#define DISTANCE_MIN 100 // cm

void setup() {
  // Set trigger pin as output and echo pin as input
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Set LED pins as output and turn them off initially
  pinMode(RED_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  
  pinMode(YELLOW_PIN, OUTPUT);
  digitalWrite(YELLOW_PIN, LOW);

  pinMode(BLUE_PIN, OUTPUT);
  digitalWrite(BLUE_PIN, LOW);

  // Set buzzer pin as output and turn it off initially
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

}

void loop() {
  
   // Send a short pulse to trigger the sensor 
   digitalWrite(TRIG_PIN,HIGH); 
   delayMicroseconds(10); 
   digitalWrite(TRIG_PIN,LOW); 

   // Measure the duration of the echo pulse 
   long duration = pulseIn(ECHO_PIN,HIGH); 

   // Calculate the distance based on the speed of sound (343 m/s) 
   float distance = (duration/2) * (0.0343); 

   // Check the distance and turn on/off the LED lights accordingly 
   
   if (distance > DISTANCE_MAX) {
     // Turn on only blue light 
     digitalWrite(BLUE_PIN,HIGH); 
     digitalWrite(YELLOW_PIN,LOW); 
     digitalWrite(RED_PIN,LOW); 

     // Turn off buzzer 
     digitalWrite(BUZZER_PIN,LOW); 
     
   } else if (distance > DISTANCE_MIN && distance <= DISTANCE_MAX) {
     // Turn on blue and yellow lights 
     digitalWrite(BLUE_PIN,HIGH); 
     digitalWrite(YELLOW_PIN,HIGH); 
     digitalWrite(RED_PIN,LOW);

     // Turn off buzzer  
     digitalWrite(BUZZER_PIN,LOW); 

     
   } else if (distance <= DISTANCE_MIN) {
      // Turn on all lights  
      digitalWrite(BLUE_PIN,HIGH); 
      digitalWrite(YELLOW_PIN,HIGH); 
      digitalWrite(RED_PIN,HIGH);


      digitalWrite(BUZZER_PIN,HIGH); // active the buzzer
      delay(3000);              // delay of 3000 millisecond = 3s
      digitalWrite(BUZZER_PIN,LOW); // deactivat the buzzer
      
      
    }
}