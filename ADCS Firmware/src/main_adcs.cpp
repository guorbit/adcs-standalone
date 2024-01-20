#include <Arduino.h>
#include <Wire.h>
#include "Board_settings.hpp"

void request_event()
{
  Wire.write("hello!");
}

void setup()
{
  Wire.begin(ADCS_I2C_ADDR);
  Serial.begin(9600);
  Wire.onRequest(request_event);
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}