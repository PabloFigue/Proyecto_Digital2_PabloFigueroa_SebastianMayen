/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "PabloFig"
#define IO_KEY       "aio_Iert19LE9h4OZb6QP3PNMGqGsw6j"

/******************************* WIFI **************************************/

#define WIFI_SSID "Galaxy_S20_Figueroa"
#define WIFI_PASS "Figueroa101"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
