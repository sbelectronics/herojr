#ifndef __HEADSTEERING_H__
#define __HEADSTEERING_H__

#define AUTOROTATE_NONE 0
#define AUTOROTATE_CW 1
#define AUTOROTATE_CCW 2

extern uint16_t HeadCenterPosition;
extern uint16_t HeadFullSweepSteps;
extern uint16_t HeadPosition;
extern uint16_t HeadDesiredPosition;
extern uint8_t HeadCalibrationState;
extern uint16_t HeadCalibrationSteps;

void HeadSteeringInit();
void HeadSteerAbsolute();
void HeadSteeringUpdate();

void HeadSteerAbsoluteHi(uint8_t reg);
void HeadSteerAbsoluteLo(uint8_t reg);

bool HeadHallDown();

#endif
