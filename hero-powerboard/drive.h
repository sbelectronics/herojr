#ifndef __DRIVE_H__
#define __DRIVE_H__

extern uint8_t Speed;
extern uint8_t DesiredSpeed;
extern bool Forward;
extern bool DesiredForward;

void DriveInit();
void DriveUpdate();

#endif
