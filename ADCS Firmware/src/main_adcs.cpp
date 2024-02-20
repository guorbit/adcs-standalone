#include <Arduino.h>
#include <Wire.h>
#include "Board_settings.hpp"
#include "DataTypes.hpp"

ADCS_State state = IDLE;
ADCS_State new_state;
GPSData gps_data;
OrientationData o_data;
RequestType req_type;

// State Machine Checking -------------------------------------------------------------------

// State transistion table provides transition lookups.
// Row index is the current state
// Column contains destination state
// Set to 0 if invalid, set to 1 if valid
int state_transition_table[NUM_STATES][NUM_STATES] = {
  {1,1},
  {1,1}
};
// All ones implies all transistions are valid.

ErrorReponse change_state()
{
  if (state_transition_table[state][new_state]) {
    state = new_state;
    return SUCCESS;
  } else {
    return FAIL;
  }
}

// Helper Functions ------------------------------------------------------------------------

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

  Serial.println("Sampled-------------------------");

  return;
}

// OSB Request Functions ------------------------------------------------------------------

void send_ADCS_data() {
  // Pack ADCS Data into buffer and
  ADCSData adcs_data;
  adcs_data.gps_data = gps_data;
  adcs_data.o_data = o_data;
  print_adcs_data(&adcs_data);
  Serial.println("Writin ADCS data");
  Wire.write(reinterpret_cast<uint8_t*>(&adcs_data), sizeof(adcs_data));
  Serial.println("Wrote data :)");
}

void send_status() {
  // Pack status into buffer and send
  Wire.write((char*)&state);
}

void change_mode() {
  ErrorReponse status = change_state();
  Wire.write((uint8_t*)&status, sizeof(status));
}

void (*request_functions[])() = {send_ADCS_data, send_status, change_mode};

void send_data()
{
  Serial.println("Data requested.");
  Serial.println((int)req_type);

  request_functions[req_type]();
}

void process_OBS_request(int n_bytes)
{
  // while (Wire.available() < sizeof(OBSRequest)) {}; // Not the nicest way to wait but fine for now
  Serial.println("Request incoming..");
  // Take the first byte to see what type of request it is.
  if (Wire.available()) {
    req_type = (RequestType)Wire.read();
    Serial.print("req_type set to ");
    Serial.println((int) req_type);
  }
  
  switch (req_type)
  {
  case MISSION_CHANGE:
    // Grab the next byte to read the new mission type
    if (Wire.available()) {
      new_state = (ADCS_State)Wire.read();
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