#include <Arduino.h>
#include <Wire.h>
#include "Board_settings.hpp"
#include "DataTypes.hpp"

OperationMode op_mode = IDLE;
GPSData gps_data;
OrientationData o_data;

RequestType req_type;

void sample_data()
{
  // Update gps data and orientation data

  // Randomise it for the sake of having something to send.
  gps_data.latitude = random();
  gps_data.longutude = random();

  o_data.abs_rot_x = random();
  o_data.abs_rot_y = random();
  o_data.abs_rot_z = random();
  o_data.ang_rate_x = random();
  o_data.ang_rate_y = random();
  o_data.ang_rate_z = random();

  return;
}

void send_data()
{
  // Pack data into message
  ADCSData adcs_data;
  adcs_data.gps_data = gps_data;
  adcs_data.o_data = o_data;

  Wire.write(reinterpret_cast<uint8_t*>(&adcs_data), sizeof(ADCSData));
}

void process_command(int n_bytes)
{
  OBSRequest obs_req;

  // while (Wire.available() < sizeof(OBSRequest)) {}; // Not the nicest way to wait but fine for now

  Wire.readBytes(reinterpret_cast<uint8_t*>(&obs_req), sizeof(OBSRequest));

  req_type = obs_req.req;
  
  switch (obs_req.req)
  {
  case MISSION_CHANGE:
    op_mode = obs_req.op_mode;
    break;

  default:
    Serial.println("Unrecognised request type.");
    break;
  }
}

void setup()
{
  Wire.begin(ADCS_I2C_ADDR);
  Serial.begin(9600);

  Wire.onRequest(send_data);
  Wire.onReceive(process_command);
}

void loop()
{
  sample_data();
  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}