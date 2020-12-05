#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define K1 0x01
#define K2 0x02
#define K3 0x04
#define K4 0x08
#define K5 0x10
#define K6 0x20
#define K7 0x40
#define K8 0x80
#define K9 0x100
#define K0 0x200
#define KA 0x400
#define KB 0x800
#define KC 0x1000
#define KD 0x2000
#define KE 0x4000
#define KF 0x8000

void KeyboardInit();
void KeyboardUpdate();

#endif
