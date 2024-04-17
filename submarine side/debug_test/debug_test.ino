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

//struct Data {
//  float accelerationX;
//  float accelerationY;
//  bool pump;
//};

//Data receivedData; // to store received data
int receivedData;

int data[9];
unsigned int cursor = 0;


void setup() {
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

    data[cursor] = receivedData;
    cursor++;

    if(cursor == 9){
      cursor = 0;
      for(int i = 0; i < 9; i++){
        Serial.print(data[i]);
      }
      Serial.println("");
    }

    //Serial.print("Acceleration X: ");
    //Serial.println(receivedData.accelerationX);
    //Serial.print("Acceleration Y: ");
    //Serial.println(receivedData.accelerationY);
    //Serial.print("Pump Status: ");
    //Serial.println(receivedData.pump ? "ON" : "OFF");

    //for(int i = 0; i < 9; i++){
    //  Serial.print(receivedData[i]);
//
    //}
    //Serial.println("");





    

  }
}