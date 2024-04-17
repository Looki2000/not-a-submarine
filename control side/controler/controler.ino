//Adding Libraries 
#include <SPI.h>                  /* to handle the communication interface with the modem*/
#include <nRF24L01.h>             /* to handle this particular modem driver*/
#include <RF24.h>                 /* the library which helps us to control the radio modem*/

#define pot_pin A0                /*Variable pin of POT is to be connected to analog pin 0 i.e.A0*/
RF24 radio(8,7);                    /* Creating instance 'radio'  ( CE , CSN )   CE -> D7 | CSN -> D8 */                              
const byte Address[6] = "00009" ;     /* Address to which data to be transmitted*/

void setup() {
  // put your setup code here, to    run once:
  Serial.begin(9600);

  radio.begin ();                 /* Activate the modem*/
  radio.openWritingPipe (Address); /* Sets the address of transmitter to which program will send the data */
  radio.setPALevel(RF24_PA_HIGH); /*RF24_PA_HIGH > RF24_PA_MID > RF24_PA_LOW > MIN*/
  radio.stopListening();
}


void loop() {
    String received_data;
    //int pot;
    if(Serial.available() > 0) { //If data acuired
        received_data = Serial.readStringUntil('\n'); //Jeśli tak, to odczytaj je do znaku końca linii i zapisz w zmiennej odebraneDane
        //Serial.println("I receive " + odebraneDane + " !"); //Wyświetl komunikat
        //pot = abs(odebraneDane.toInt());
        //radio.write(&pot, sizeof(pot)); 
        //Serial.print("Transmitting Data : ");
        //Serial.println(pot);   
        //delay(100);

        // send raw unprocessed data through the radio
        radio.write(&received_data, sizeof(received_data));
    
    }

}