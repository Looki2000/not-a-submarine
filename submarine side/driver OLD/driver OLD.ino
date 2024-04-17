//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte send_address[6] = "00002";
const byte recv_address[6] = "00001";

struct ControlData {
  float thrust[4];
  float steer[4];
  int pump[1];
};

void setup() {
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin();
  
  //set the address
  radio.openWritingPipe(send_address);
  radio.openReadingPipe(1, recv_address);

  //radio.startListening();
}

void loop() {
  //Read the data if available in buffer
  if (radio.available())
  {
    byte control_data[9];
    radio.read(&control_data, sizeof(control_data));

    if (strlen(control_data) > 0){
      //struct ControlData control_data_struct;
      //memcpy(&control_data_struct, control_data, sizeof(ControlData));

      Serial.print(" == ");
      Serial.println(control_data[0]);

      //Serial.print(control_data_struct.thrust[0]);
      //Serial.print(" | ");
      //Serial.print(control_data_struct.steer[0]);
      //Serial.print(" | ");
      //Serial.println(control_data_struct.pump[0]);

    }

  }
}
