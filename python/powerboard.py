import time

REG_RESET = 0
REG_DESIREDPOSITION_HI = 10
REG_DESIREDPOSITION_LO = 11
REG_POSITION_HI = 12
REG_POSITION_LO = 13
REG_FULLSWEEPSTEPS_HI = 14
REG_FULLSWEEPSTEPS_LO = 15
REG_CENTERPOSITION_HI = 16
REG_CENTERPOSITION_LO = 17
REG_LIMITS = 50

class PowerBoard:
    def __init__(self, bus=None, addr=0x4, pi=None, sdaPin=2, sclPin=3):
        self.bus = bus
        self.addr = addr
        self.pi = pi
        self.sdaPin = sdaPin
        self.sclPin = sclPin

        if (self.pi):
            try:
                self.pi.bb_i2c_close(self.sdaPin)
            except:
                pass
            # pigpiod
            self.pi.bb_i2c_open(self.sdaPin, self.sclPin, 100000)

    def readReg(self, reg):
        if self.bus:
            data = self.bus.read_byte_data(self.addr, reg)
        else:
            (count, i2cdata) = self.pi.bb_i2c_zip(self.sdaPin, 
                (4, self.addr,      # set addr to self.addr
                2, 7, 1, reg,       # start, write one byte (reg)
                2, 6, 1,            # (re)start, read one byte
                3,                  # stop
                0))                 # end 
            if count<0:
                raise I2CError("i2c error")
            if count!=2:
                raise I2CError("i2c wrong byte count")

            data = i2cdata[0]
        return data

    def writeReg(self, reg, v):
        print "writeReg", reg, v
        if self.bus:
            self.bus.write_byte_data(self.addr, reg, v)
        else:
            (count, i2cdata) = self.pi.bb_i2c_zip(self.sdaPin, 
                (4, self.addr,         # set addr to self.addr
                2, 7, 2, reg, v,    # start, write two bytes (reg, v)
                2, 6, 1,            # (re)start, read one byte
                3,                  # stop
                0))                 # end 
            if count<0:
                raise I2CError("i2c error")
            if count!=2:
                raise I2CError("i2c wrong byte count")

            #readBack = i2cdata[0]

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

    def dumpRegs(self):
        desiredPosition = self.readDesiredPosition()
        position = self.readPosition()
        fullSweepPosition = self.readFullSweepSteps()
        centerPosition = self.readCenterPosition()
        lim = self.readLimits()
        print "dPos=%d, pos=%d, fPos=%d, cPos=%d, limits=%d" % (desiredPosition, position, fullSweepPosition, centerPosition, lim)


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