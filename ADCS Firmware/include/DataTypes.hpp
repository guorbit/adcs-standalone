#pragma once

enum OperationMode {
    IDLE,
    DETUMBLE,
    EARTH_TRACKING,
    MISSION2,
    MISSION3
};

enum RequestType {
    STATUS,
    ADCS_DATA,
    GENERATES_RESPONSE,
    MISSION_CHANGE
};

typedef struct ADCSI2CTransaction {
    
} ADCSI2CTransaction;

typedef struct OBSRequest {
    RequestType req;
    OperationMode op_mode;
} OBSRequest;

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