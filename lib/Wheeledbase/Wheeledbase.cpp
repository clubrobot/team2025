//
// Created by boris on 28/10/2024.
//

#include "Wheeledbase.h"

// Instructions


void Wheeledbase::DISABLE() {
    velocityControl.disable();
    positionControl.disable();
    leftWheel.setVelocity(0);
    rightWheel.setVelocity(0);
}

void Wheeledbase::GOTO_DELTA(float dx, float dy) {
    purePursuit.reset();
    positionControl.disable();

    Position initial_pos = *odometry.getPosition();

    Position target_pos;
    target_pos.x = initial_pos.x + dx * cos(initial_pos.theta) + dy * -1 * sin(initial_pos.theta);
    target_pos.y = initial_pos.y + dx * sin(initial_pos.theta) + dy * cos(initial_pos.theta);

    target_pos.theta = atan2(target_pos.y - initial_pos.y, target_pos.x - initial_pos.x);
    int direction;

    initial_pos.theta = inrange(initial_pos.theta, -M_PI,M_PI);

    if (fabs(inrange(target_pos.theta - initial_pos.theta, -M_PI,M_PI)) < (M_PI / 2)) {
        direction = PurePursuit::FORWARD;
    }
    else {
        direction = PurePursuit::BACKWARD;
    }

    purePursuit.setDirection((PurePursuit::Direction)direction);
    purePursuit.addWaypoint(PurePursuit::Waypoint(initial_pos.x, initial_pos.y));
    purePursuit.addWaypoint(PurePursuit::Waypoint(target_pos.x, target_pos.y));

    purePursuit.setFinalAngle(target_pos.theta);

    positionControl.setPosSetpoint(Position(target_pos.x, target_pos.y, target_pos.theta + direction * M_PI));

    // Enable PurePursuit controller
    velocityControl.enable();
    positionControl.setMoveStrategy(purePursuit);
    positionControl.enable();
}

void Wheeledbase::SET_OPENLOOP_VELOCITIES(float leftWheelVel, float rightWheelVel) {
    velocityControl.disable();
    positionControl.disable();
    leftWheel.setVelocity(leftWheelVel);
    rightWheel.setVelocity(rightWheelVel);
}

void Wheeledbase::GET_CODEWHEELS_COUNTERS(float* leftCodewheelCounter, float* rightCodewheelCounter) {
    *leftCodewheelCounter = leftCodewheel.getCounter();
    *rightCodewheelCounter = rightCodewheel.getCounter();
}

void Wheeledbase::SET_VELOCITIES(float linVelSetpoint, float angVelSetpoint) {
    positionControl.disable();
    velocityControl.enable();
    velocityControl.setSetpoints(linVelSetpoint, angVelSetpoint);
}

void Wheeledbase::RESET_PUREPURSUIT() {
    purePursuit.reset();
    positionControl.disable();
}

void Wheeledbase::START_PUREPURSUIT(int8_t direction, float finalAngle) {
    // Setup PurePursuit
    purePursuit.setFinalAngle(finalAngle);

    switch (direction) {
    case PurePursuit::FORWARD:
        purePursuit.setDirection(PurePursuit::FORWARD);
        break;
    case PurePursuit::BACKWARD:
        purePursuit.setDirection(PurePursuit::BACKWARD);
        direction = direction - M_PI;
        break;
    }

    // Compute final setpoint
    const PurePursuit::Waypoint wp0 = purePursuit.getWaypoint(purePursuit.getNumWaypoints() - 2);
    const PurePursuit::Waypoint wp1 = purePursuit.getWaypoint(purePursuit.getNumWaypoints() - 1);
    positionControl.setPosSetpoint(Position(wp1.x, wp1.y, atan2(wp1.y - wp0.y, wp1.x - wp0.x) + direction * M_PI));

    // Enable PurePursuit controller
    velocityControl.enable();
    positionControl.setMoveStrategy(purePursuit);
    positionControl.enable();
}

void Wheeledbase::ADD_PUREPURSUIT_WAYPOINT(float x, float y) {
    // Queue waypoint
    purePursuit.addWaypoint(PurePursuit::Waypoint(x, y));
}

void Wheeledbase::START_TURNONTHESPOT(bool dir, float theta) {
    Position posSetpoint = *odometry.getPosition();
    float initTheta = posSetpoint.theta;
    posSetpoint.theta = theta;
    float angPosSetpoint = inrange((posSetpoint.theta - initTheta), -M_PI, M_PI);
    velocityControl.enable();
    positionControl.setPosSetpoint(posSetpoint);
    if (dir) {
        if (angPosSetpoint > 0) turnOnTheSpot.setDirection(TurnOnTheSpot::CLOCK);
        else turnOnTheSpot.setDirection(TurnOnTheSpot::TRIG);
    }
    else {
        if (angPosSetpoint > 0) turnOnTheSpot.setDirection(TurnOnTheSpot::TRIG);
        else turnOnTheSpot.setDirection(TurnOnTheSpot::CLOCK);
    }
    positionControl.setMoveStrategy(turnOnTheSpot);
    positionControl.enable();
}

void Wheeledbase::START_TURNONTHESPOT_DIR(bool dir, float theta) {
    Position posSetpoint = *odometry.getPosition();
    posSetpoint.theta = theta;
    velocityControl.enable();
    positionControl.setPosSetpoint(posSetpoint);
    if (dir) {
        turnOnTheSpot.setDirection(TurnOnTheSpot::TRIG);
    }
    else {
        turnOnTheSpot.setDirection(TurnOnTheSpot::CLOCK);
    }
    positionControl.setMoveStrategy(turnOnTheSpot);
    positionControl.enable();
}

/**
* ret:
0b00 si pas reached pas spin urgency (0)
0b01 si reached (1)
0b10 si spin urgency (2)
0b11 si les deux (3)
*/
uint8_t Wheeledbase::POSITION_REACHED() {
    bool positionReached = positionControl.getPositionReached() && positionControl.isEnabled();
    bool spinUrgency = false;//!velocityControl.isEnabled();
    uint8_t ret = 0;
    ret = ret | positionReached;
    ret = ret | (spinUrgency << 1);
    return ret;
}

void Wheeledbase::PUREPURSUIT(const Position** waypoints, uint16_t nb_waypoints, char dir, float finalAngle) {
    if(nb_waypoints<2) {
        printf("[Purepursuit] not enough waypoints\n");
        return;
    }
    Wheeledbase::RESET_PUREPURSUIT();
    for(int i = 0; i < nb_waypoints; i++) {
        Wheeledbase::ADD_PUREPURSUIT_WAYPOINT(waypoints[i]->x, waypoints[i]->y);
    }
    Wheeledbase::START_PUREPURSUIT(dir, finalAngle);
}

void Wheeledbase::GOTO(Position* pos, char dir, float finalAngle) {

    float defaultMaxSpeed = Wheeledbase::GET_PARAMETER_VALUE(POSITIONCONTROL_LINVELMAX_ID);

    const Position *myPos = Wheeledbase::GET_POSITION();
    if (dir==PurePursuit::NONE) {
        if(cos(atan2(pos->y-myPos->y, pos->x-myPos->x)-myPos->theta)) {
            dir=PurePursuit::FORWARD;
            printf("forward\n");
        }else {
            dir=PurePursuit::BACKWARD;
            printf("back\n");
        }
    }
    const Position *posTab[2]={myPos, pos};
    Wheeledbase::PUREPURSUIT(posTab, 2, dir, finalAngle);//TODO

    double maxSpeed = defaultMaxSpeed;

    while(!(Wheeledbase::POSITION_REACHED() & 0b01)) {
        const Position *posi = Wheeledbase::GET_POSITION();
        int distance = sqrt(pow(posi->x-pos->x, 2)+pow(posi->y-pos->y, 2));
        if (distance<SLOWDOWN_DISTANCE){
            maxSpeed = maxSpeed > defaultMaxSpeed*0.1 ? maxSpeed * SLOWDOWN_FACTOR : maxSpeed;
            Wheeledbase::SET_PARAMETER_VALUE(POSITIONCONTROL_LINVELMAX_ID, maxSpeed);
        }
        //printf("%f %f %f %f, %f, %f\n", pos->x, pos->y, pos->theta, posi->x, posi->y, posi->theta);
        //Do nothing
    }

    Wheeledbase::SET_PARAMETER_VALUE(POSITIONCONTROL_LINVELMAX_ID, defaultMaxSpeed);

    if(pos->theta!=MAXFLOAT) {

        Wheeledbase::START_TURNONTHESPOT(0, pos->theta);
        while(!(Wheeledbase::POSITION_REACHED() & 0b01)) {
            //Todo: TimeOUT
        }
    }
}
int tester = 0;
void Wheeledbase::GOTO_LIDAR(Position* pos, char dir, float finalAngle, const float* avant, const float* arrière) {

    const Position *myPos = Wheeledbase::GET_POSITION();
    if (dir==PurePursuit::NONE) {
        if(cos(atan2(pos->y-myPos->y, pos->x-myPos->x))-myPos->theta) {
            dir=PurePursuit::FORWARD;
        }else {
            dir=PurePursuit::BACKWARD;
        }
    }
    const Position *posTab[2]={myPos, pos};
    Wheeledbase::PUREPURSUIT(posTab, 2, dir, finalAngle);//TODO


    while(Wheeledbase::POSITION_REACHED()!=0b01) {
        const Position *pos = Wheeledbase::GET_POSITION();
        //printf("%f, %f, %f\n", pos->x, pos->y, pos->theta);
       if (*avant<20){
           if (tester>5){
               SET_VELOCITIES(0,0);
               //printf("STOPPPP\n");
           }
           else tester++;
       }else{
           if (tester>5){
               const Position *myPos = Wheeledbase::GET_POSITION();
               if (dir==PurePursuit::NONE) {
                   if(cos(atan2(pos->y-myPos->y, pos->x-myPos->x))-myPos->theta) {
                       dir=PurePursuit::FORWARD;
                   }else {
                       dir=PurePursuit::BACKWARD;
                   }
               }
               const Position *posTab[2]={myPos, pos};
               Wheeledbase::PUREPURSUIT(posTab, 2, dir, finalAngle);//TODO
           }
           tester=0;
       }
    }

    if(pos->theta!=MAXFLOAT) {

        Wheeledbase::START_TURNONTHESPOT(0, pos->theta);
        while(Wheeledbase::POSITION_REACHED()!=0b01) {
            //Todo: TimeOUT
        }
    }
}

void Wheeledbase::GET_VELOCITIES_WANTED(float* linOutput, float* angOutput, bool spin) {
    if (spin) {
        *linOutput = velocityControl.getLinOutput();
        *angOutput = velocityControl.getAngOutput();
    }
    else {
        *linOutput = velocityControl.getLinSpinGoal();
        *angOutput = velocityControl.getAngSpinGoal();
    }
}


void Wheeledbase::SET_POSITION(Position* pos) {
    odometry.setPosition(pos->x, pos->y, pos->theta);
}

const Position* Wheeledbase::GET_POSITION() {
    const Position* pos = odometry.getPosition();
    return pos;
}

void Wheeledbase::GET_VELOCITIES(float* linVel, float* angVel) {
    *linVel = odometry.getLinVel();
    *angVel = odometry.getAngVel();
}

void Wheeledbase::SET_PARAMETER_VALUE(byte paramID, float value) {
    switch (paramID) {
    case LEFTWHEEL_RADIUS_ID:
        leftWheel.setWheelRadius(value);
        break;
    case LEFTWHEEL_CONSTANT_ID:
        leftWheel.setConstant(value);
        break;
    case LEFTWHEEL_MAXPWM_ID:
        leftWheel.setMaxPWM(value);
        break;
    case RIGHTWHEEL_RADIUS_ID:
        rightWheel.setWheelRadius(value);
        break;
    case RIGHTWHEEL_CONSTANT_ID:
        rightWheel.setConstant(value);
        break;
    case RIGHTWHEEL_MAXPWM_ID:
        rightWheel.setMaxPWM(value);
        break;
    case LEFTCODEWHEEL_RADIUS_ID:
        leftCodewheel.setWheelRadius(value);
        break;
    case LEFTCODEWHEEL_COUNTSPERREV_ID:
        leftCodewheel.setCountsPerRev(static_cast<long>(value));
        break;
    case RIGHTCODEWHEEL_RADIUS_ID:
        rightCodewheel.setWheelRadius(value);
        break;
    case RIGHTCODEWHEEL_COUNTSPERREV_ID:
        rightCodewheel.setCountsPerRev(static_cast<long>(value));
        break;
    case ODOMETRY_AXLETRACK_ID:
        odometry.setAxleTrack(value);
        break;
    case ODOMETRY_SLIPPAGE_ID:
        odometry.setSlippage(value);
        break;
    case VELOCITYCONTROL_AXLETRACK_ID:
        velocityControl.setAxleTrack(value);
        break;
    case VELOCITYCONTROL_MAXLINACC_ID:
        velocityControl.setMaxLinAcc(value);
        break;
    case VELOCITYCONTROL_MAXLINDEC_ID:
        velocityControl.setMaxLinDec(value);
        break;
    case VELOCITYCONTROL_MAXANGACC_ID:
        velocityControl.setMaxAngAcc(value);
        break;
    case VELOCITYCONTROL_MAXANGDEC_ID:
        velocityControl.setMaxAngDec(value);
        break;
    case VELOCITYCONTROL_SPINSHUTDOWN_ID:
        velocityControl.setSpinShutdown((bool)value);
        break;
    case LINVELPID_KP_ID:
        linVelPID.setTunings(value, linVelPID.getKi(), linVelPID.getKd());
        break;
    case LINVELPID_KI_ID:
        linVelPID.setTunings(linVelPID.getKp(), value, linVelPID.getKd());
        break;
    case LINVELPID_KD_ID:
        linVelPID.setTunings(linVelPID.getKp(), linVelPID.getKi(), value);
        break;
    case LINVELPID_MINOUTPUT_ID:
        linVelPID.setOutputLimits(value, linVelPID.getMaxOutput());
        break;
    case LINVELPID_MAXOUTPUT_ID:
        linVelPID.setOutputLimits(linVelPID.getMinOutput(), value);
        break;
    case ANGVELPID_KP_ID:
        angVelPID.setTunings(value, angVelPID.getKi(), angVelPID.getKd());
        break;
    case ANGVELPID_KI_ID:
        angVelPID.setTunings(angVelPID.getKp(), value, angVelPID.getKd());
        break;
    case ANGVELPID_KD_ID:
        angVelPID.setTunings(angVelPID.getKp(), angVelPID.getKi(), value);
        break;
    case ANGVELPID_MINOUTPUT_ID:
        angVelPID.setOutputLimits(value, angVelPID.getMaxOutput());
        break;
    case ANGVELPID_MAXOUTPUT_ID:
        angVelPID.setOutputLimits(angVelPID.getMinOutput(), value);
        break;
    case POSITIONCONTROL_LINVELKP_ID:
        positionControl.setVelTunings(value, positionControl.getAngVelKp());
        break;
    case POSITIONCONTROL_ANGVELKP_ID:
        positionControl.setVelTunings(positionControl.getLinVelKp(), value);
        break;
    case POSITIONCONTROL_LINVELMAX_ID:
        positionControl.setVelLimits(value, positionControl.getAngVelMax());
        break;
    case POSITIONCONTROL_ANGVELMAX_ID:
        positionControl.setVelLimits(positionControl.getLinVelMax(), value);
        break;
    case POSITIONCONTROL_LINPOSTHRESHOLD_ID:
        positionControl.setPosThresholds(value, positionControl.getAngPosThreshold());
        break;
    case POSITIONCONTROL_ANGPOSTHRESHOLD_ID:
        positionControl.setPosThresholds(positionControl.getLinPosThreshold(), value);
        break;
    case PUREPURSUIT_LOOKAHED_ID:
        purePursuit.setLookAhead(value);
        break;
    case PUREPURSUIT_LOOKAHEADBIS_ID:
        purePursuit.setLookAheadBis(value);
        break;
    }
}



float Wheeledbase::GET_PARAMETER_VALUE(byte paramID) {
    if (paramID == LEFTWHEEL_RADIUS_ID) {
        return leftWheel.getWheelRadius();
    }
    else if (paramID == LEFTWHEEL_CONSTANT_ID) {
        return leftWheel.getConstant();
    }
    else if (paramID == LEFTWHEEL_MAXPWM_ID) {
        return leftWheel.getMaxPWM();
    }

    else if (paramID == RIGHTWHEEL_RADIUS_ID) {
        return rightWheel.getWheelRadius();
    }
    else if (paramID == RIGHTWHEEL_CONSTANT_ID) {
        return rightWheel.getConstant();
    }
    else if (paramID == RIGHTWHEEL_MAXPWM_ID) {
        return rightWheel.getMaxPWM();
    }

    else if (paramID == LEFTCODEWHEEL_RADIUS_ID) {
        return leftCodewheel.getWheelRadius();
    }
    else if (paramID == LEFTCODEWHEEL_COUNTSPERREV_ID) {
        return leftCodewheel.getCountsPerRev();
    }

    else if (paramID == RIGHTCODEWHEEL_RADIUS_ID) {
        return rightCodewheel.getWheelRadius();
    }
    else if (paramID == RIGHTCODEWHEEL_COUNTSPERREV_ID) {
        return rightCodewheel.getCountsPerRev();
    }

    else if (paramID == ODOMETRY_AXLETRACK_ID) {
        return odometry.getAxleTrack();
    }
    else if (paramID == ODOMETRY_SLIPPAGE_ID) {
        return odometry.getSlippage();
    }

    else if (paramID == VELOCITYCONTROL_AXLETRACK_ID) {
        return velocityControl.getAxleTrack();
    }
    else if (paramID == VELOCITYCONTROL_MAXLINACC_ID) {
        return velocityControl.getMaxLinAcc();
    }
    else if (paramID == VELOCITYCONTROL_MAXLINDEC_ID) {
        return velocityControl.getMaxLinDec();
    }
    else if (paramID == VELOCITYCONTROL_MAXANGACC_ID) {
        return velocityControl.getMaxAngAcc();
    }
    else if (paramID == VELOCITYCONTROL_MAXANGDEC_ID) {
        return velocityControl.getMaxAngDec();
    }
    else if (paramID == VELOCITYCONTROL_SPINSHUTDOWN_ID) {
        return (float)velocityControl.getSpinShutdown();
    }

    else if (paramID == LINVELPID_KP_ID) {
        return linVelPID.getKp();
    }
    else if (paramID == LINVELPID_KI_ID) {
        return linVelPID.getKi();
    }
    else if (paramID == LINVELPID_KD_ID) {
        return linVelPID.getKd();
    }
    else if (paramID == LINVELPID_MINOUTPUT_ID) {
        return linVelPID.getMinOutput();
    }
    else if (paramID == LINVELPID_MAXOUTPUT_ID) {
        return linVelPID.getMaxOutput();
    }

    else if (paramID == ANGVELPID_KP_ID) {
        return angVelPID.getKp();
    }
    else if (paramID == ANGVELPID_KI_ID) {
        return angVelPID.getKi();
    }
    else if (paramID == ANGVELPID_KD_ID) {
        return angVelPID.getKd();
    }
    else if (paramID == ANGVELPID_MINOUTPUT_ID) {
        return angVelPID.getMinOutput();
    }
    else if (paramID == ANGVELPID_MAXOUTPUT_ID) {
        return angVelPID.getMaxOutput();
    }

    else if (paramID == POSITIONCONTROL_LINVELKP_ID) {
        return positionControl.getLinVelKp();
    }
    else if (paramID == POSITIONCONTROL_ANGVELKP_ID) {
        return positionControl.getAngVelKp();
    }
    else if (paramID == POSITIONCONTROL_LINVELMAX_ID) {
        return positionControl.getLinVelMax();
    }
    else if (paramID == POSITIONCONTROL_ANGVELMAX_ID) {
        return positionControl.getAngVelMax();
    }
    else if (paramID == POSITIONCONTROL_LINPOSTHRESHOLD_ID) {
        return positionControl.getLinPosThreshold();
    }
    else if (paramID == POSITIONCONTROL_ANGPOSTHRESHOLD_ID) {
        return positionControl.getAngPosThreshold();
    }
    else if (paramID == PUREPURSUIT_LOOKAHED_ID) {
        return purePursuit.getLookAhead();
    }
    else if (paramID == PUREPURSUIT_LOOKAHEADBIS_ID) {
        return purePursuit.getLookAheadBis();
    }
    return 0;
}

void Wheeledbase::PRINT_PARAMS() {
    Serial.print(F(" LEFTWHEEL_RADIUS_ID:")); Serial.println(leftWheel.getWheelRadius());
    Serial.print(F(" LEFTWHEEL_CONSTANT_ID:")); Serial.println(leftWheel.getConstant());
    Serial.print(F(" LEFTWHEEL_MAXPWM_ID:")); Serial.println(leftWheel.getMaxPWM());
    Serial.print(F(" RIGHTWHEEL_RADIUS_ID:")); Serial.println(rightWheel.getWheelRadius());
    Serial.print(F(" RIGHTWHEEL_CONSTANT_ID:")); Serial.println(rightWheel.getConstant());
    Serial.print(F(" RIGHTWHEEL_MAXPWM_ID:")); Serial.println(rightWheel.getMaxPWM());
    Serial.print(F(" LEFTCODEWHEEL_RADIUS_ID:")); Serial.println(leftCodewheel.getWheelRadius());
    Serial.print(F(" LEFTCODEWHEEL_COUNTSPERREV_ID:")); Serial.println(leftCodewheel.getCountsPerRev());
    Serial.print(F(" RIGHTCODEWHEEL_RADIUS_ID:")); Serial.println(rightCodewheel.getWheelRadius());
    Serial.print(F(" RIGHTCODEWHEEL_COUNTSPERREV_ID:")); Serial.println(rightCodewheel.getCountsPerRev());
    Serial.print(F(" ODOMETRY_AXLETRACK_ID:")); Serial.println(odometry.getAxleTrack());
    Serial.print(F(" ODOMETRY_SLIPPAGE_ID:")); Serial.println(odometry.getSlippage());
    Serial.print(F(" VELOCITYCONTROL_AXLETRACK_ID:")); Serial.println(velocityControl.getAxleTrack());
    Serial.print(F(" VELOCITYCONTROL_MAXLINACC_ID:")); Serial.println(velocityControl.getMaxLinAcc());
    Serial.print(F(" VELOCITYCONTROL_MAXLINDEC_ID:")); Serial.println(velocityControl.getMaxLinDec());
    Serial.print(F(" VELOCITYCONTROL_MAXANGACC_ID:")); Serial.println(velocityControl.getMaxAngAcc());
    Serial.print(F(" VELOCITYCONTROL_MAXANGDEC_ID:")); Serial.println(velocityControl.getMaxAngDec());
    Serial.print(F(" VELOCITYCONTROL_SPINSHUTDOWN_ID:")); Serial.println(velocityControl.getSpinShutdown());
    Serial.print(F(" LINVELPID_KP_ID:")); Serial.println(linVelPID.getKp());
    Serial.print(F(" LINVELPID_KI_ID:")); Serial.println(linVelPID.getKi());
    Serial.print(F(" LINVELPID_KD_ID:")); Serial.println(linVelPID.getKd());
    Serial.print(F(" LINVELPID_MINOUTPUT_ID:")); Serial.println(linVelPID.getMinOutput());
    Serial.print(F(" LINVELPID_MAXOUTPUT_ID:")); Serial.println(linVelPID.getMaxOutput());
    Serial.print(F(" ANGVELPID_KP_ID:")); Serial.println(angVelPID.getKp());
    Serial.print(F(" ANGVELPID_KI_ID:")); Serial.println(angVelPID.getKi());
    Serial.print(F(" ANGVELPID_KD_ID:")); Serial.println(angVelPID.getKd());
    Serial.print(F(" ANGVELPID_MINOUTPUT_ID:")); Serial.println(angVelPID.getMinOutput());
    Serial.print(F(" ANGVELPID_MAXOUTPUT_ID:")); Serial.println(angVelPID.getMaxOutput());
    Serial.print(F(" POSITIONCONTROL_LINVELKP_ID:")); Serial.println(positionControl.getLinVelKp());
    Serial.print(F(" POSITIONCONTROL_ANGVELKP_ID:")); Serial.println(positionControl.getAngVelKp());
    Serial.print(F(" POSITIONCONTROL_LINVELMAX_ID:")); Serial.println(positionControl.getLinVelMax());
    Serial.print(F(" POSITIONCONTROL_ANGVELMAX_ID:")); Serial.println(positionControl.getAngVelMax());
    Serial.print(F(" POSITIONCONTROL_LINPOSTHRESHOLD_ID:")); Serial.println(positionControl.getLinPosThreshold());
    Serial.print(F(" POSITIONCONTROL_ANGPOSTHRESHOLD_ID:")); Serial.println(positionControl.getAngPosThreshold());
    Serial.print(F(" PUREPURSUIT_LOOKAHED_ID:")); Serial.println(purePursuit.getLookAhead());
    Serial.print(F(" PUREPURSUIT_LOOKAHEADBIS_ID:")); Serial.println(purePursuit.getLookAheadBis());
}
