/*
   Copyright (c) 2015 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <CurieBle.h>

/*
   This sketch example partially implements the standard Bluetooth Low-Energy Battery service.
   For more information: https://developer.bluetooth.org/gatt/services/Pages/ServicesHome.aspx
*/

/*  */
BLEPeripheral blePeripheral;       // BLE Peripheral Device (the board you're programming)
BLEService batteryService("180F"); // BLE Battery Service

// BLE Battery Level Characteristic"
BLECharacteristic batteryLevelChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify,2);     // remote clients will be able to
// get notifications if this characteristic changes

int oldBatteryLevel = 0;  // last battery level reading from analog input
long previousMillis = 0;  // last time the battery level was checked, in ms

int oldDValues[14];  // previous values for each digital pin
int oldAValues[6];  // previous values for each analog pin

const int pinD0 = 0; 
const int pinD1 = 1; 
const int pinD2 = 2; 
const int pinD3 = 3; 
const int pinD4 = 4; 
const int pinD5 = 5; 
const int pinD6 = 6; 
const int pinD7 = 7; 
const int pinD8 = 8; 
const int pinD9 = 9; 
const int pinD10 = 10; 
const int pinD11 = 11; 
const int pinD12 = 12; 
const int pinD13 = 13; 
const int pinA0 = A0;
const int pinA1 = A1;
const int pinA2 = A2;
const int pinA3 = A3;
const int pinA4 = A4;
const int pinA5 = A5;

int myDPins[14] = {pinD0, pinD1, pinD2, pinD3, pinD4, pinD5, pinD6, pinD7, pinD8, pinD9, pinD10, pinD11, pinD12, pinD13};
int myAPins[6] = {pinA0, pinA1, pinA2, pinA3, pinA4, pinA5};

void setup() {
  Serial.begin(9600);    // initialize serial communication
  pinMode(13, OUTPUT);   // initialize the LED on pin 13 to indicate when a central is connected

  // Intial previous values to 0 (used to detech changes)
  for (int i=0; i <= 13; i++){
    oldDValues[i] = 0;
  } 
  for (int i=0; i <= 5; i++){
    oldAValues[i] = 0;
  } 
  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet */
  blePeripheral.setLocalName("BatteryMonitorSketch");
  blePeripheral.setAdvertisedServiceUuid(batteryService.uuid());  // add the service UUID
  blePeripheral.addAttribute(batteryService);   // Add the BLE Battery service
  blePeripheral.addAttribute(batteryLevelChar); // add the battery level characteristic
  //batteryLevelChar.setValue(oldBatteryLevel);   // initial value for this characteristic
      const unsigned char pinCharArray[2] = { (char)12, (char)67 };
    batteryLevelChar.setValue(pinCharArray, 2);  // and update the heart rate measurement characteristic


  /* Now activate the BLE device.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */
  blePeripheral.begin();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  Serial.print("Looping"); // print it
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(13, HIGH);

    // check the battery level every 200ms
    // as long as the central is still connected:
    while (central.connected()) {
      long currentMillis = millis();
      // if 200ms have passed, check the battery level:
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        updateBatteryLevel();
      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(13, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void updateBatteryLevel() {
  /* Read the current voltage level on the A0 analog input pin.
     This is used here to simulate the charge level of a battery.
  */
  int battery = analogRead(A5);

  //int batteryLevel = map(battery, 0, 1023, 0, 1023);

  if (battery != oldBatteryLevel) {      // if the battery level has changed
    Serial.print("Battery Level % is now: "); // print it
    Serial.println(battery);
      
    int val2 = battery/256;
    int val1 = battery - val2*256;

    Serial.print("New value for analog pin "); // print it
    //Serial.print(i); // print it      
    Serial.print(" : "); // print it
    Serial.print(val2);
    Serial.print(" - "); // print it
    Serial.print(val1);
    Serial.print(" - "); // print it
    Serial.println(battery);

    //batteryLevelChar.setValue(260);  // and update the battery level characteristic
    const unsigned char pinCharArray[2] = { (char)val2, (char)val1 };
    batteryLevelChar.setValue(pinCharArray, 2);  // and update the heart rate measurement characteristic

    oldBatteryLevel = battery;           // save the level for next comparison
  }
}

bool haschangedAnalog(int pinNum) {
  
    int currentValue = analogRead(myAPins[pinNum]);
    int previousValue = oldAValues[pinNum];

    if (currentValue != previousValue) {
      Serial.print("CHANGE for analog pin "); // print it
      //Serial.print(i); // print it      
      Serial.print(" : "); // print it
      Serial.print(pinNum);
      Serial.print(" - "); // print it
      Serial.print(previousValue);
      Serial.print(" - "); // print it
      Serial.println(currentValue);
      
      oldAValues[pinNum] = currentValue;
      return true;
    } else {
      return false;
    }





}

