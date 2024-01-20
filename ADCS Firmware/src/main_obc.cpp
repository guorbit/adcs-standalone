#include <Arduino.h>
#include <Serial.h>
#include <Wire.h>
#include "Board_settings.hpp"

void setup()
{
    Serial.begin(9600);
    Wire.begin();
}

void loop() 
{ 
    int bytes = Wire.requestFrom(ADCS_I2C_ADDR, 6);

    Serial.println(bytes);

    if (Wire.available()) {
        Serial.println("available");
    }

    while (Wire.available())
    {
        char c = Wire.read();
        Serial.print(c);
    }
    Serial.println("test");
    delay(500);
}