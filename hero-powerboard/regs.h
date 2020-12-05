#ifndef __REGS_H__
#define __REGS_H__

#define REG_RESET 0
#define REG_ECHO 1

// two-byte regs, steering
#define REG_DESIREDPOSITION_HI 10
#define REG_DESIREDPOSITION_LO 11
#define REG_POSITION_HI 12
#define REG_POSITION_LO 13
#define REG_FULLSWEEPSTEPS_HI 14
#define REG_FULLSWEEPSTEPS_LO 15
#define REG_CENTERPOSITION_HI 16
#define REG_CENTERPOSITION_LO 17
#define REG_CALIBRATIONSTEPS_HI 18
#define REG_CALIBRATIONSTEPS_LO 19

// two-byte regs, head steering
#define REG_HEADDESIREDPOSITION_HI 20
#define REG_HEADDESIREDPOSITION_LO 21
#define REG_HEADPOSITION_HI 22
#define REG_HEADPOSITION_LO 23
#define REG_HEADFULLSWEEPSTEPS_HI 24
#define REG_HEADFULLSWEEPSTEPS_LO 25
#define REG_HEADCENTERPOSITION_HI 26
#define REG_HEADCENTERPOSITION_LO 27
#define REG_HEADCALIBRATIONSTEPS_HI 28
#define REG_HEADCALIBRATIONSTEPS_LO 29

// two-byte regs, encoders
#define REG_WHEELENCODER_HI 40
#define REG_WHEELENCODER_LO 41

// single-byte regs, steering
#define REG_LIMITS 50
#define REG_CALIBRATIONSTATE 51
#define REG_HEADCALIBRATIONSTATE 52
#define REG_HEADAUTOROTATE 53

// single-byte regs, drive
#define REG_SPEED 60
#define REG_DESIREDSPEED 61
#define REG_FORWARD 62
#define REG_DESIREDFOWARD 63

#endif
