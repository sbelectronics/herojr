from smbpi.i2c_with_crc import I2CWithCrc, I2CError, ReceiveCRCError, ReceiveSizeError, ReceiveUninitializedError, CRCError, NoMoreRetriesError
import time

REG_RESET = 0
REG_ECHO = 1
REG_DESIREDPOSITION_HI = 10
REG_DESIREDPOSITION_LO = 11
REG_POSITION_HI = 12
REG_POSITION_LO = 13
REG_FULLSWEEPSTEPS_HI = 14
REG_FULLSWEEPSTEPS_LO = 15
REG_CENTERPOSITION_HI = 16
REG_CENTERPOSITION_LO = 17
REG_CALIBRATIONSTEPS_HI = 18
REG_CALIBRATIONSTEPS_LO = 19
REG_LIMITS = 50
REG_CALIBRATIONSTATE = 51
REG_SPEED = 60
REG_DESIREDSPEED = 61
REG_FORWARD = 62
REG_DESIREDFORWARD = 63

class PowerBoard(I2CWithCrc):
    def __init__(self, bus=None, addr=0x4, pi=None, sdaPin=2, sclPin=3):
        I2CWithCrc.__init__(self, bus, addr, pi, sdaPin, sclPin)

    def readDesiredPosition(self):
        return (self.readReg(REG_DESIREDPOSITION_HI) << 8) + self.readReg(REG_DESIREDPOSITION_LO)

    def setDesiredPosition(self, v):
        self.writeReg(REG_DESIREDPOSITION_HI, v>>8)
        self.writeReg(REG_DESIREDPOSITION_LO, v&0xFF)

    def readPosition(self):
        return (self.readReg(REG_POSITION_HI) << 8) + self.readReg(REG_POSITION_LO)

    def readFullSweepSteps(self):
        return (self.readReg(REG_FULLSWEEPSTEPS_HI) << 8) + self.readReg(REG_FULLSWEEPSTEPS_LO)

    def readCenterPosition(self):
        return (self.readReg(REG_CENTERPOSITION_HI) << 8) + self.readReg(REG_CENTERPOSITION_LO)
    
    def readLimits(self):
        return self.readReg(REG_LIMITS)

    def readSpeed(self):
        return self.readReg(REG_SPEED)

    def readDesiredSpeed(self):
        return self.readReg(REG_DESIREDSPEED)

    def readForward(self):
        return self.readReg(REG_FORWARD)

    def readDesiredForward(self):
        return self.readReg(REG_DESIREDFORWARD)

    def readCalibrationState(self):
        return self.readReg(REG_CALIBRATIONSTATE)

    def readCalibrationSteps(self):
        return (self.readReg(REG_CALIBRATIONSTEPS_HI) << 8) + self.readReg(REG_CALIBRATIONSTEPS_LO)

    def setDesiredSpeed(self, speed):
        self.writeReg(REG_DESIREDSPEED, speed)

    def setDesiredForward(self, forward):
        self.writeReg(REG_DESIREDFORWARD, forward)

    def echo(self, v):
        self.writeReg(REG_ECHO, v)
        readBack = self.readReg(REG_ECHO)
        if readBack != v:
            print "ERROR! Echo failed", v, readBack

    def dumpRegs(self):
        desiredPosition = self.readDesiredPosition()
        position = self.readPosition()
        fullSweepPosition = self.readFullSweepSteps()
        centerPosition = self.readCenterPosition()
        lim = self.readLimits()
        desiredSpeed = self.readDesiredSpeed()
        speed = self.readSpeed()
        desiredForward = self.readDesiredForward()
        forward = self.readForward()
        cali = self.readCalibrationState()
        caliSteps = self.readCalibrationSteps()
        print "dPos=%d, pos=%d, fPos=%d, cPos=%d, limits=%d, cali=%d, caliSteps=%d" % (desiredPosition, position, fullSweepPosition, centerPosition, lim, cali, caliSteps)
        print "dSpeed=%d, speed=%d, dForward=%d, forward=%d" % (desiredSpeed, speed, desiredForward, forward)
        print ""


def diags(pb):
    while (True):
        pb.dumpRegs()
        time.sleep(1)


def main():
    import smbus
    bus = smbus.SMBus(1)
    pb = PowerBoard(bus=bus)

    #import pigpio
    #pi = pigpio.pi()
    #ups = UPS(pi=pi)

    diags(pb)


if __name__ == "__main__":
    main()