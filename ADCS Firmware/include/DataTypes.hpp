#pragma once

enum ADCS_State {
    IDLE,
    DETUMBLE,
    NUM_STATES
};

enum RequestType {
    STATUS,
    ADCS_DATA,
    MISSION_CHANGE
};

enum ErrorReponse {
    FAIL,
    SUCCESS
};

typedef struct GPSData {
    long longutude;
    long latitude;
} GPSData;

typedef struct OrientationData {
    long abs_rot_x;
    long abs_rot_y;
    long abs_rot_z;

    long ang_rate_x;
    long ang_rate_y;
    long ang_rate_z;
} OrientationData;

typedef struct ADCSData {
    GPSData gps_data;
    OrientationData o_data;
} ADCSData;