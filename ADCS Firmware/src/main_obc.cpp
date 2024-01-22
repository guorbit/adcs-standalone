#include <Arduino.h>
#include <Serial.h>
#include <Wire.h>
#include "Board_settings.hpp"
#include "DataTypes.hpp"

void print_adcs_data(ADCSData *data)
{
    Serial.println("GPS Data");
    Serial.println(data->gps_data.latitude);
    Serial.println(data->gps_data.longutude);
    
    Serial.println("Orientaion Data");
    Serial.println(data->o_data.abs_rot_x);
    Serial.println(data->o_data.abs_rot_y);
    Serial.println(data->o_data.abs_rot_z);
    Serial.println(data->o_data.ang_rate_x);
    Serial.println(data->o_data.ang_rate_y);
    Serial.println(data->o_data.ang_rate_z);
}

void setup()
{
    Serial.begin(9600);
    Wire.begin();
}

void loop()
{ 
    // Example ADCS Data request
    
    // set request type in ADCS MCU
    OBSRequest new_request;
    new_request.req = ADCS_DATA;
    
    Wire.beginTransmission(ADCS_I2C_ADDR);
    Wire.write(reinterpret_cast<uint8_t*>(&new_request), sizeof(OBSRequest));
    Wire.endTransmission();

    // Request the data from ADCS
    ADCSData adcs_data;

    Wire.requestFrom(ADCS_I2C_ADDR, sizeof(ADCSData));
    while (Wire.available() < sizeof(ADCSData)) {}
    Wire.readBytes(reinterpret_cast<uint8_t*>(&adcs_data), sizeof(ADCSData));

    print_adcs_data(&adcs_data);

    delay(500);
}