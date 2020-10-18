#ifndef __STEERING_H__
#define __STEERING_H__

extern uint16_t CenterPosition;
extern uint16_t FullSweepSteps;
extern uint16_t Position;
extern uint16_t DesiredPosition;
extern bool SteeringCalibrated;

void SteeringInit();
void SteerAbsolute();
void UpdateSteering();

void SteerAbsoluteHi(uint8_t reg);
void SteerAbsoluteLo(uint8_t reg);

#endif
