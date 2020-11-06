#ifndef __REGS_H__
#define __REGS_H__

#define REG_RESET 0
#define REG_ECHO 1

// two-byte regs
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

// two-byte regs, encoders
#define REG_WHEELENCODER_HI 30
#define REG_WHEELENCODER_LO 31

// single-byte regs, steering
#define REG_LIMITS 50
#define REG_CALIBRATIONSTATE 51

// single-byte regs, drive
#define REG_SPEED 60
#define REG_DESIREDSPEED 61
#define REG_FORWARD 62
#define REG_DESIREDFOWARD 63

#endif
