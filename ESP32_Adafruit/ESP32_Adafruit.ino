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

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
String usart;
String horario;
int distanciaValue = 0;
int sensorLuzValue = 0;
int sensorInfraValue = 0;

// set up the 'counter' feed
AdafruitIO_Feed *distanciaFeed = io.feed("distancia");
AdafruitIO_Feed *sensorInfraFeed = io.feed("sensor-infra");
AdafruitIO_Feed *sensorLuzFeed = io.feed("sensor-luz");
AdafruitIO_Feed *horarioFeed = io.feed("sensor-rtc");

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
  usart = SerialPort.readStringUntil('\n');
  Serial.println("imprimiendo");
  Serial.println(usart);

    if (usart.charAt(0) == '+') {
      horario = usart.substring(1, 9);
      horarioFeed->save(horario);
    } else {
      distanciaValue = usart.substring(0, 2).toInt();
      sensorLuzValue = usart.substring(3, 6).toInt();
      sensorInfraValue = usart.substring(7, 8).toInt();

      distanciaFeed->save(distanciaValue);
      sensorLuzFeed->save(sensorLuzValue);
      sensorInfraFeed->save(sensorInfraValue);

    }
    delay(12000);
}
    // Añadir una pausa entre publicaciones

  




