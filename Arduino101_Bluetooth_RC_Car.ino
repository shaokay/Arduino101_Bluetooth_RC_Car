/*
   Title: Arduino 101 - Bluetooth RC Car
   Author: Kwong Ming You
   Email: mingyou0621@gmail.com
*/

#include <BLEAttribute.h>
#include <BLECentral.h>
#include <BLECharacteristic.h>
#include <BLECommon.h>
#include <BLEDescriptor.h>
#include <BLEPeripheral.h>
#include <BLEService.h>
#include <BLETypedCharacteristic.h>
#include <BLETypedCharacteristics.h>
#include <BLEUuid.h>
#include <CurieBle.h>

BLEPeripheral ble; 

//Nordic BLE UART Service
BLEService UART("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"); 

//Tx Characterisitic
BLEUnsignedCharCharacteristic Tx("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLERead | BLEWrite);

const int EnA = 5;
const int motorL1 = 12; 
const int motorL2 = 11; 
const int EnB = 6;
const int motorR1 = 8; 
const int motorR2 = 7; 

void setup() {
  pinMode(EnA, OUTPUT); 
  pinMode(EnB, OUTPUT); 
  pinMode(motorL1, OUTPUT); 
  pinMode(motorL2, OUTPUT); 
  pinMode(motorR1, OUTPUT); 
  pinMode(motorR2, OUTPUT); 

  Serial.begin(9600); 
  
  // set advertised local name and service UUID:
  ble.setLocalName("Arduino101"); 
  ble.setAdvertisedServiceUuid(UART.uuid()); 
  
  // add service and characteristic:
  ble.addAttribute(UART); 
  ble.addAttribute(Tx); 
  
  // set the initial value for the characeristic:
  Tx.setValue(0); 
  
  // begin advertising BLE service:
  ble.begin();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect: 
  BLECentral central = ble.central(); 

  // if a central is connected to peripheral
  if(central) 
  {
    Serial.print("Connected to central: "); 
    Serial.println(central.address()); // print the central's MAC address

    while(central.connected())
    {
      // if the remote device wrote to the characteristic, 
      if(Tx.written())
      {
        switch(Tx.value())
        {
          case '2': // forward
            digitalWrite(motorL1, HIGH); 
            digitalWrite(motorL2, LOW); 
            digitalWrite(motorR1, LOW); 
            digitalWrite(motorR2, HIGH); 
            break; 
          case '8': // backward
            digitalWrite(motorL1, LOW); 
            digitalWrite(motorL2, HIGH); 
            digitalWrite(motorR1, HIGH); 
            digitalWrite(motorR2, LOW); 
            break; 
          case '4': // left
            digitalWrite(motorL1, LOW); 
            digitalWrite(motorL2, HIGH); 
            digitalWrite(motorR1, LOW); 
            digitalWrite(motorR2, HIGH); 
            break; 
          case '6': // right
            digitalWrite(motorL1, HIGH); 
            digitalWrite(motorL2, LOW); 
            digitalWrite(motorR1, HIGH); 
            digitalWrite(motorR2, LOW); 
            break; 
          default:
            digitalWrite(motorL1, HIGH); 
            digitalWrite(motorL2, HIGH); 
            digitalWrite(motorR1, HIGH); 
            digitalWrite(motorR2, HIGH); 
            break; 
        }
        delay(1); 
      }
    }
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
