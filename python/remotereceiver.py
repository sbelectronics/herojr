import socket
import struct
import threading
import time


# minimum steering has to change for us to consider it a movement
STEER_THRESH = 20

SPEED_THRESH = 20

# dead zone plus and minus from center (fullscale is 0-1000)
CENTER_THRESH = 40

# constants from pi-remote
KEY_1 =         1
KEY_2 =         2
KEY_3 =         4
KEY_4 =         8
KEY_5 =      0x10
KEY_6 =      0x20
KEY_7 =      0x40
KEY_8 =      0x80
KEY_9 =     0x100
KEY_0 =     0x200
KEY_HASH =  0x400
KEY_STAR =  0x800
KEY_S =    0x1000
KEY_P =    0x2000
KEY_R =    0x4000
BUT0  =         1
BUT1  =         2

ALL_KEYS = [KEY_1 , KEY_2 , KEY_3 , KEY_4 , KEY_5 , KEY_6 , KEY_7 , KEY_8 , KEY_9 , KEY_0 , KEY_HASH , KEY_STAR, KEY_S, KEY_P, KEY_R ]


class RemoteReceiver(threading.Thread):
    def __init__(self, powerBoard=None, speechBoard=None):
        threading.Thread.__init__(self)
        self.daemon = True

        self.powerBoard = powerBoard
        self.speechBoard = speechBoard

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(("", 1234))

        self.x1 = 0
        self.y1 = 0
        self.x2 = 0
        self.y2 = 0
        self.centerX1 = 0
        self.centerY1 = 0
        self.centerX2 = 0
        self.centerY2 = 0
        self.buttons = 0
        self.keypad = 0

        self.lastKeys = 0

        self.lastSteerJoy = 0
        self.lastSpeedJoy = 0

    def keyUp(self, key):
        if (key == KEY_1):
            self.speechBoard.say("daisy")
        elif (key == KEY_2):
            self.speechBoard.say("america")
        elif (key == KEY_3):
            self.speechBoard.say("mary")
        elif (key == KEY_4):
            self.speechBoard.say("leia")
        elif (key == KEY_5):
            self.speechBoard.say("iamrobot")
        elif (key == KEY_6):
            self.speechBoard.say("brain")
        elif (key == KEY_7):
            self.speechBoard.say("intruder")
        
    def keyDown(self, key):
        pass

    def handleUpdate(self):
        if self.powerBoard:
            x2 = self.x2
            if (abs(x2-self.centerX2) < CENTER_THRESH):
                x2 = self.centerX2
            if abs(self.lastSteerJoy - x2) > STEER_THRESH:
                steer_to = float(self.powerBoard.readFullSweepSteps()) * float(1000-x2) / 1000.0
                steer_to = int(steer_to)
                self.powerBoard.setDesiredPosition(steer_to)
                self.lastSteerJoy = self.x2

            y2 = self.y2
            if (abs(y2-self.centerY2) < CENTER_THRESH):
                y2 = self.centerY2

            if abs(self.lastSpeedJoy - y2) > SPEED_THRESH:
                if y2 == self.centerY2:
                    speed_to = 0
                else:
                    speed_to = 510.0 * float(y2) / 1000.0
                    speed_to = int(speed_to)-255

                if (speed_to < 0):
                    self.powerBoard.setDesiredForward(True)
                else:
                    self.powerBoard.setDesiredForward(False)

                speed_to = min(255, int(abs(speed_to)*1.5))
                self.powerBoard.setDesiredSpeed(speed_to)

                self.lastSpeedJoy = self.y2

            if (self.buttons & BUT0)!=0:
                self.powerBoard.setHeadAutoRotate(1)
            elif (self.buttons & BUT1)!=0:
                self.powerBoard.setHeadAutoRotate(2)
            else:
                self.powerBoard.setHeadAutoRotate(0)

            if (self.lastKeys is not None):
                for key in ALL_KEYS:
                    if ((self.lastKeys & key)!=0) and ((self.keypad & key)==0):
                        self.keyUp(key)
                    if ((self.lastKeys & key)==0) and ((self.keypad & key)!=0):
                        self.keyDown(key)                 
            self.lastKeys = self.keypad

            if (self.keypad & KEY_S)!=0:
                self.powerBoard.setHeadDesiredPosition(self.powerBoard.readHeadCenterPosition())
        else:
            print "update", self.x1, self.y1, self.x2, self.y2

    def run(self):
        while True:
            data, addr = self.sock.recvfrom(1024)
            (self.x1, self.y1, self.x2, self.y2,
             self.centerX1, self.centerY1, self.centerX2, self.centerY2,
             self.buttons, self.keypad) = struct.unpack("!IIIIIIIIII", data)
            self.handleUpdate()


def main():
    remote = RemoteReceiver()
    remote.start()

    while True:
        time.sleep(1)


if __name__ == "__main__":
    main()
            
