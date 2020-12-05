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

REG_HEADDESIREDPOSITION_HI = 20
REG_HEADDESIREDPOSITION_LO = 21
REG_HEADPOSITION_HI = 22
REG_HEADPOSITION_LO = 23
REG_HEADFULLSWEEPSTEPS_HI = 24
REG_HEADFULLSWEEPSTEPS_LO = 25
REG_HEADCENTERPOSITION_HI = 26
REG_HEADCENTERPOSITION_LO = 27
REG_HEADCALIBRATIONSTEPS_HI = 28
REG_HEADCALIBRATIONSTEPS_LO = 29

REG_WHEELENCODER_HI = 40
REG_WHEELENCODER_LO = 41
REG_LIMITS = 50
REG_CALIBRATIONSTATE = 51
REG_HEADCALIBRATIONSTATE = 52
REG_SPEED = 60
REG_DESIREDSPEED = 61
REG_FORWARD = 62
REG_DESIREDFORWARD = 63

class PowerBoard(I2CWithCrc):
    def __init__(self, bus=None, addr=0x4, pi=None, sdaPin=2, sclPin=3):
        I2CWithCrc.__init__(self, bus, addr, pi, sdaPin, sclPin)

    # body
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
    
    def readCalibrationState(self):
        return self.readReg(REG_CALIBRATIONSTATE)

    def readCalibrationSteps(self):
        return (self.readReg(REG_CALIBRATIONSTEPS_HI) << 8) + self.readReg(REG_CALIBRATIONSTEPS_LO)

    # body
    def readHeadDesiredPosition(self):
        return (self.readReg(REG_HEADDESIREDPOSITION_HI) << 8) + self.readReg(REG_HEADDESIREDPOSITION_LO)

    def setHeadDesiredPosition(self, v):
        self.writeReg(REG_HEADDESIREDPOSITION_HI, v>>8)
        self.writeReg(REG_HEADDESIREDPOSITION_LO, v&0xFF)

    def readHeadPosition(self):
        return (self.readReg(REG_HEADPOSITION_HI) << 8) + self.readReg(REG_HEADPOSITION_LO)

    def readHeadFullSweepSteps(self):
        return (self.readReg(REG_HEADFULLSWEEPSTEPS_HI) << 8) + self.readReg(REG_HEADFULLSWEEPSTEPS_LO)

    def readHeadCenterPosition(self):
        return (self.readReg(REG_HEADCENTERPOSITION_HI) << 8) + self.readReg(REG_HEADCENTERPOSITION_LO)
    
    def readHeadCalibrationState(self):
        return self.readReg(REG_HEADCALIBRATIONSTATE)

    def readHeadCalibrationSteps(self):
        return (self.readReg(REG_HEADCALIBRATIONSTEPS_HI) << 8) + self.readReg(REG_HEADCALIBRATIONSTEPS_LO)        

    # drive

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

    def readWheelEncoderCount(self):
        return (self.readReg(REG_WHEELENCODER_HI) << 8) + self.readReg(REG_WHEELENCODER_LO)

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
        headDesiredPosition = self.readHeadDesiredPosition()
        headPosition = self.readHeadPosition()
        headFullSweepPosition = self.readHeadFullSweepSteps()
        headCenterPosition = self.readHeadCenterPosition()        
        headCali = self.readHeadCalibrationState()
        headCaliSteps = self.readHeadCalibrationSteps()        
        desiredSpeed = self.readDesiredSpeed()
        speed = self.readSpeed()
        desiredForward = self.readDesiredForward()
        forward = self.readForward()
        cali = self.readCalibrationState()
        caliSteps = self.readCalibrationSteps()
        wheelEncoderCount = self.readWheelEncoderCount()
        print "dPos=%d, pos=%d, fPos=%d, cPos=%d, limits=%d, cali=%d, caliSteps=%d" % (desiredPosition, position, fullSweepPosition, centerPosition, lim, cali, caliSteps)
        print "hdPos=%d, hpos=%d, hfPos=%d, hcPos=%d, hcali=%d, hcaliSteps=%d" % (headDesiredPosition, headPosition, headFullSweepPosition, headCenterPosition, headCali, headCaliSteps)
        print "dSpeed=%d, speed=%d, dForward=%d, forward=%d, wEncoder=%d" % (desiredSpeed, speed, desiredForward, forward, wheelEncoderCount)
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