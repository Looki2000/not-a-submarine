//// Adding Libraries 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define pins
#define L_PWM 5
#define L_DIR 4
#define R_PWM 6
#define R_DIR 9
#define PWM_MAX 165 // Max available power for motors (max 255)
#define BUZZER 10

RF24 radio(8, 7); // CE, CSN pins
const byte address[6] = "00009";

struct Data {
  float accelerationX;
  float accelerationY;
  bool pump;
};

Data receivedData; // to store received data

void setup() {
  // Configuration of H-bridge motor driver pins
  pinMode(L_DIR, OUTPUT);
  pinMode(R_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PWM, OUTPUT);

  // Additional pins setup
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, 0); // Turn off buzzer
  
  // Serial port and radio communication setup
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN); // set power level to MIN (options: MIN, LOW, HIGH)
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedData, sizeof(receivedData)); // Read data into struct
    Serial.print("Acceleration X: ");
    Serial.println(receivedData.accelerationX);
    Serial.print("Acceleration Y: ");
    Serial.println(receivedData.accelerationY);
    Serial.print("Pump Status: ");
    Serial.println(receivedData.pump ? "ON" : "OFF");

    // Control motors based on acceleration values
    controlMotors(receivedData.accelerationX, receivedData.accelerationY);
  }
}

void controlMotors(float accelX, float accelY) {
  int baseSpeed = map(abs(accelX), 0, 1, 0, PWM_MAX); // Map X acceleration to base speed
  int differential = map(accelY, -1, 1, -PWM_MAX, PWM_MAX); // Map Y acceleration to differential speed

  int leftSpeed = constrain(baseSpeed + differential, 0, PWM_MAX);
  int rightSpeed = constrain(baseSpeed - differential, 0, PWM_MAX);

// Set motor directions based on the sign of accelerationX
int leftDirection = accelX >= 0 ? HIGH : LOW; // Forward when accelX is positive
int rightDirection = accelX >= 0 ? HIGH : LOW; // Forward when accelX is positive

// Apply the speed and direction to the motors
digitalWrite(L_DIR, leftDirection);
analogWrite(L_PWM, leftSpeed);

digitalWrite(R_DIR, rightDirection);
analogWrite(R_PWM, rightSpeed);

// Optionally log motor values for debugging
Serial.print("Left Motor Speed: ");
Serial.print(leftSpeed);
Serial.print(", Direction: ");
Serial.println(leftDirection ? "Forward" : "Backward");

Serial.print("Right Motor Speed: ");
Serial.print(rightSpeed);
Serial.print(", Direction: ");
Serial.println(rightDirection ? "Forward" : "Backward");
}

void leftMotor(int V, int dir) {
digitalWrite(L_DIR, dir);
analogWrite(L_PWM, map(abs(V), 0, 100, 0, PWM_MAX));
}

void rightMotor(int V, int dir) {
digitalWrite(R_DIR, dir);
analogWrite(R_PWM, map(abs(V), 0, 100, 0, PWM_MAX));
}

void stopMotors() {
analogWrite(L_PWM, 0);
analogWrite(R_PWM, 0);
}
