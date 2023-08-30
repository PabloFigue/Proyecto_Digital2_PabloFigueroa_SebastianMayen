// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <HardwareSerial.h>
HardwareSerial SerialPort(2);

String usart;

AdafruitIO_Feed *statusFeed = io.feed("status-sensores");
/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch

void setup() {

  // start the serial connection
  Serial.begin(9600);
  SerialPort.begin (9600, SERIAL_8N1, 16, 17);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  io.run();

    SerialPort.readStringUntil('\n');
    SerialPort.readStringUntil('\n');
    SerialPort.readStringUntil('\n');
    SerialPort.readStringUntil('\n');
    // Leer el último byte
    usart = SerialPort.readStringUntil('\n');
    Serial.println("imprimiendo");
    Serial.println(usart);
    statusFeed->save(usart);
    
    delay(2000);    // Añadir una pausa entre publicaciones

}

 
