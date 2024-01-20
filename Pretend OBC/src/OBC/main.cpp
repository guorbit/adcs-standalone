#include <Arduino.h>
#include <Serial.h>

void setup()
{
    Serial.begin(9600);
}

void loop() {
    Serial.println("yeet");
}