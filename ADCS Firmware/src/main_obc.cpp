#include <Arduino.h>
#include <Serial.h>
#include <Wire.h>
#include "Board_settings.hpp"
#include "DataTypes.hpp"

void get_ADCS_data(ADCSData *data)
{
    RequestType req_type = ADCS_DATA;

    // Set the transaction type in the ADCS
    Wire.beginTransmission(ADCS_I2C_ADDR);
    Wire.write((uint8_t*)&req_type, 1);
    Wire.endTransmission();

    // Request the data from ADCS
    Wire.requestFrom(ADCS_I2C_ADDR, sizeof(ADCSData));
    while (Wire.available() < sizeof(ADCSData)) {}
    Wire.readBytes(reinterpret_cast<uint8_t*>(&data), sizeof(ADCSData));
}

void set_ADCS_mode(ADCS_State mode)
{
    // Pack request type and new mode into a buffer
    char buffer[] = {
        (char) MISSION_CHANGE,
        (char) mode
    };

    Serial.println("Sending ADCS MISSION_CHANGE Request");

    // Send the buffer
    Wire.beginTransmission(ADCS_I2C_ADDR);
    Wire.write(buffer, sizeof(buffer));
    Wire.endTransmission();
}

ADCS_State get_ADCS_status()
{
    RequestType req_type = STATUS;

    Serial.println("Sending ADCS DATA Request");

    Wire.beginTransmission(ADCS_I2C_ADDR);
    Wire.write((uint8_t*)&req_type, 1);
    Wire.endTransmission();

    Wire.requestFrom(ADCS_I2C_ADDR, sizeof(ADCS_State));
    return (ADCS_State) Wire.read();
}

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
    delay(5000);
    
    Serial.begin(9600);
    Wire.begin();

    Serial.println("Setup complete.");
}

void loop()
{ 
    
    Serial.println("Starting loop.");
    set_ADCS_mode(DETUMBLE);
    Serial.print("ADCS status: ");
    Serial.println(get_ADCS_status());

    set_ADCS_mode(IDLE);
    Serial.print("ADCS status: ");
    Serial.println(get_ADCS_status());

    ADCSData adcs_data;
    get_ADCS_data(&adcs_data);
    print_adcs_data(&adcs_data);

    delay(500);
}