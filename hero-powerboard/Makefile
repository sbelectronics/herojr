# NOTE: Customize the following to your environment!

AVR_TOOLS_DIR = /usr
AVRDUDE_CONF=/usr/share/arduino/hardware/tools/avrdude.conf
ARDUINO_DIR=/home/smbaker/projects/pi/arduino-build/arduino
ARDUINO_MK_FILE=/usr/share/arduino/Arduino.mk
ALTERNATE_CORE_PATH=/home/smbaker/projects/pi/MightyCore/avr

ISP_PROG=usbasp

BOARD_TAG=324
MCU=atmega324a
VARIANT=standard
F_CPU=8000000

ISP_LOCK_FUSE_PRE = 0xFF
ISP_LOCK_FUSE_POST = 0xFF
ISP_HIGH_FUSE = 0xDF
ISP_LOW_FUSE = 0xE2
ISP_EXT_FUSE = 0xFF

# fuses differences from minipro stock
#   CKDIV8 unprogrammed
#   BOOTSZ1 and BOOTSZ2 unprogrammed
#   JTAGEN unprogrammed

include $(ARDUINO_MK_FILE)
