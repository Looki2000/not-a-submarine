//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte send_address[6] = "00001";
const byte recv_address[6] = "00002";

struct SensorData {
  float light[4];
};

void setup() {
  radio.begin();
  
  //set the address
  radio.openWritingPipe(send_address);
  //radio.openReadingPipe(1, recv_address);
  
  //radio.startListening();
  radio.stopListening();


  while (!Serial);
    Serial.begin(9600);
}

void loop() {
  //Send message to receiver
  //const char text[] = "Hello World2 test";
  //radio.write(&text, sizeof(text));

  //Read the data if available in buffer
  // receive SENSOR data from SUBMARINE
//  if (radio.available()) {
//    byte sensor_data[4];
//    radio.read(&sensor_data, sizeof(sensor_data));
//
//    if (strlen(sensor_data) > 0) {
//      struct SensorData sensor_data_struct;
//      //memcpy(&sensor_data_struct, sensor_data_struct, sizeof(sensor_data_struct));
//      memcpy(&sensor_data_struct, sensor_data, sizeof(SensorData));
//      //Serial.print("==");
//      //Serial.print(sensor_data_struct.data[0]);
//      //Serial.print("==");
//
//      // send it to the PC
//      Serial.write(sensor_data, sizeof(sensor_data));
//    }
//  }


  // receive CONTROLER data from PC
  if (Serial.available() > 0) {
    // send it to the SUBMARINE
    byte control_data[9];
    Serial.readBytes(control_data, sizeof(control_data));

    radio.write(control_data, sizeof(control_data));
  }






  
  delay(10);
}