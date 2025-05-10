//
// Created by boris on 28/10/2024.
//

#ifndef WHEELEDBASE_H
#define WHEELEDBASE_H

#include "DCMotor.h"
#include "Codewheel.h"
#include "Odometry.h"
#include "PID.h"
#include "VelocityController.h"
#include "PositionController.h"
#include "PurePursuit.h"
#include "TurnOnTheSpot.h"
#include "mathutils.h"
#include <math.h>

// Parameters identifiers

#define LEFTWHEEL_RADIUS_ID             0x10
#define LEFTWHEEL_CONSTANT_ID           0x11
#define LEFTWHEEL_MAXPWM_ID             0x12
#define RIGHTWHEEL_RADIUS_ID            0x20
#define RIGHTWHEEL_CONSTANT_ID          0x21
#define RIGHTWHEEL_MAXPWM_ID            0x22
#define LEFTCODEWHEEL_RADIUS_ID         0x40
#define LEFTCODEWHEEL_COUNTSPERREV_ID   0x41
#define RIGHTCODEWHEEL_RADIUS_ID        0x50
#define RIGHTCODEWHEEL_COUNTSPERREV_ID  0x51
#define ODOMETRY_AXLETRACK_ID           0x60
#define ODOMETRY_SLIPPAGE_ID            0x61
#define VELOCITYCONTROL_AXLETRACK_ID    0x80
#define VELOCITYCONTROL_MAXLINACC_ID    0x81
#define VELOCITYCONTROL_MAXLINDEC_ID    0x82
#define VELOCITYCONTROL_MAXANGACC_ID    0x83
#define VELOCITYCONTROL_MAXANGDEC_ID    0x84
#define VELOCITYCONTROL_SPINSHUTDOWN_ID 0x85
#define LINVELPID_KP_ID                 0xA0
#define LINVELPID_KI_ID                 0xA1
#define LINVELPID_KD_ID                 0xA2
#define LINVELPID_MINOUTPUT_ID          0xA3
#define LINVELPID_MAXOUTPUT_ID          0xA4
#define ANGVELPID_KP_ID                 0xB0
#define ANGVELPID_KI_ID                 0xB1
#define ANGVELPID_KD_ID                 0xB2
#define ANGVELPID_MINOUTPUT_ID          0xB3
#define ANGVELPID_MAXOUTPUT_ID          0xB4
#define POSITIONCONTROL_LINVELKP_ID     0xD0
#define POSITIONCONTROL_ANGVELKP_ID     0xD1
#define POSITIONCONTROL_LINVELMAX_ID    0xD2
#define POSITIONCONTROL_ANGVELMAX_ID    0xD3
#define POSITIONCONTROL_LINPOSTHRESHOLD_ID  0xD4
#define POSITIONCONTROL_ANGPOSTHRESHOLD_ID  0xD5
#define PUREPURSUIT_LOOKAHED_ID         0xE0
#define PUREPURSUIT_LOOKAHEADBIS_ID     0xE2

//define the distance and the deceleration at the end of goto
#define SLOWDOWN_FACTOR                 0.02
#define SLOWDOWN_DISTANCE               50

// Global variables
extern DCMotorsDriver driver;
extern DCMotor leftWheel;
extern DCMotor rightWheel;

extern Codewheel leftCodewheel;
extern Codewheel rightCodewheel;

extern Odometry odometry;

extern VelocityController velocityControl;

extern PID linVelPID;
extern PID angVelPID;

extern PositionController positionControl;

extern PurePursuit   purePursuit;
extern TurnOnTheSpot turnOnTheSpot;

namespace  Wheeledbase {
    // Instructions prototypes
    void DISABLE();

    void GOTO_DELTA(float dx, float dy);

    void SET_OPENLOOP_VELOCITIES(float leftWheelVel, float rightWheelVel);

    void GET_CODEWHEELS_COUNTERS(float *leftCodewheelCounter, float *rightCodewheelCounter);

    void SET_VELOCITIES(float linVelSetpoint, float angVelSetpoint);

    void RESET_PUREPURSUIT();

    void ADD_PUREPURSUIT_WAYPOINT(float x, float y);

    void START_PUREPURSUIT(int8_t direction, float finalAngle);

    void START_TURNONTHESPOT(bool dir, float theta);

    void START_TURNONTHESPOT_DIR(bool dir, float theta);

    void PUREPURSUIT(const Position** waypoints, uint16_t nb_waypoints, char dir, float finalAngle);

    void GOTO(Position* pos, char dir, float finalAngle);

    void GOTO_LIDAR(Position* pos, char dir, float finalAngle, const float* avant, const float* arrière);

    uint8_t POSITION_REACHED();

    void GET_VELOCITIES_WANTED(float *linOutput, float *angOutput, bool spin=true);

    void SET_POSITION(Position* pos);

    const Position* GET_POSITION();

    void GET_VELOCITIES(float *linVel, float *angVel);

    void SET_PARAMETER_VALUE(byte paramID, float value);

    float GET_PARAMETER_VALUE(byte paramID);

    void PRINT_PARAMS();
};



#endif //WHEELEDBASE_H
