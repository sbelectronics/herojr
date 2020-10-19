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

// single-byte regs, steering
#define REG_LIMITS 50
// more single-byte regs follow

// single-byte regs, drive
#define REG_SPEED 60
#define REG_DESIREDSPEED 61
#define REG_FORWARD 62
#define REG_DESIREDFOWARD 63

#endif
