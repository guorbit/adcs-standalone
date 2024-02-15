#include <Arduino.h>
#include <Wire.h>
#include "Board_settings.hpp"
#include "DataTypes.hpp"

ADCS_State state = IDLE;
GPSData gps_data;
OrientationData o_data;
RequestType req_type;


// State transistion table provides transition lookups.
// Row index is the current state
// Column contains destination state
// Set to 0 if invalid, set to 1 if valid
int state_transition_table[NUM_STATES][NUM_STATES] = {
  {1,1},
  {1,1}
};
// All ones implies all transistions are valid.

TransError_t change_state(ADCS_State new_state)
{
  if (state_transition_table[state][new_state]) {
    state = new_state;
    return SUCCESS;
  } else {
    return FAIL;
  }
}

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
  switch (req_type)
  {
  case ADCS_DATA:
    // Pack ADCS Data into buffer and 
    ADCSData adcs_data;
    adcs_data.gps_data = gps_data;
    adcs_data.o_data = o_data;
    Wire.write(reinterpret_cast<uint8_t*>(&adcs_data), sizeof(adcs_data));
    break;

  case STATUS:
    // Pack status into buffer
    Wire.write((char*)&state);
    break;
  
  default:
    Serial.println("Unrecognised data send request type.");
  }
}

void process_OBS_request(int n_bytes)
{
  // while (Wire.available() < sizeof(OBSRequest)) {}; // Not the nicest way to wait but fine for now

  // Take the first byte to see what type of request it is.
  if (Wire.available()) {
    req_type = (RequestType)Wire.read();
  }
  
  switch (req_type)
  {
  case MISSION_CHANGE:
    // Grab the next byte to read the new mission type
    if (Wire.available()) {
      ADCS_State new_state = (ADCS_State)Wire.read();
      TransError_t status = change_state(new_state);
    }
    break;
  default:
    break;
  }
}

void setup()
{
  Wire.begin(ADCS_I2C_ADDR);
  Serial.begin(9600);

  Wire.onRequest(send_data);
  Wire.onReceive(process_OBS_request);
}

void loop()
{
  sample_data();
  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}