#include <Arduino.h>
#include <Serial.h>
#include <Wire.h>
#include "Board_settings.hpp"
#include "DataTypes.hpp"

void get_ADCS_data_blocking(ADCSData *data)
{
    RequestType req_type = ADCS_DATA;

    Serial.println("Setting ADCS_DATA req");

    // Set the transaction type in the ADCS
    Wire.beginTransmission(ADCS_I2C_ADDR);
    Wire.write((uint8_t*)&req_type, 1);
    Wire.endTransmission();

    
    Serial.println("ADCS_DATA req set");

    delay(50);

    // Request the data from ADCS
    Wire.requestFrom(ADCS_I2C_ADDR, sizeof(ADCSData));
    while (Wire.available() < sizeof(ADCSData)) {}
    Wire.readBytes(reinterpret_cast<uint8_t*>(data), sizeof(ADCSData));

    Serial.println("Read data");
}

ErrorReponse set_ADCS_mode_blocking(ADCS_State new_state)
{
    // Pack request type and new mode into a buffer
    char buffer[] = {
        (char) MISSION_CHANGE,
        (char) new_state
    };

    Serial.println("Sending ADCS MISSION_CHANGE Request");

    // Send the buffer
    Wire.beginTransmission(ADCS_I2C_ADDR);
    Wire.write(buffer, sizeof(buffer));
    Wire.endTransmission();

    // Check if the mode change was successful
    Wire.requestFrom(ADCS_I2C_ADDR, sizeof(ErrorReponse));
    while (Wire.available() < sizeof(ErrorReponse)) {}
    return (ErrorReponse)Wire.read();
}

ADCS_State get_ADCS_status_blocking()
{
    Serial.println("Getting Status");
    RequestType req_type = STATUS;

    delay(1000);

    Wire.beginTransmission(ADCS_I2C_ADDR);
    int n = Wire.write((uint8_t*)&req_type, 1);

    Serial.print(n);
    Serial.println(" bytes written.");
    int e = Wire.endTransmission();
    Serial.print("Error: ");
    Serial.println(e);

    Serial.println("Set request type in ADCS.");

    delay(50);

    int num = Wire.requestFrom(ADCS_I2C_ADDR, 1);
    Serial.println(num);

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
    set_ADCS_mode_blocking(DETUMBLE);
    delay(50);
    Serial.print("ADCS status: ");
    Serial.println(get_ADCS_status_blocking());

    set_ADCS_mode_blocking(IDLE);
    Serial.print("ADCS status: ");
    Serial.println((int)get_ADCS_status_blocking());
    delay(50);

    ADCSData adcs_data;
    get_ADCS_data_blocking(&adcs_data);
    print_adcs_data(&adcs_data);

    delay(500);
}