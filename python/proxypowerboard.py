
class ProxyPowerBoard():
    def __init__(self, bus=None, addr=0x4, pi=None, sdaPin=2, sclPin=3):
        pass

    # body
    def readDesiredPosition(self):
        return 0

    def setDesiredPosition(self, v):
        pass

    def readPosition(self):
        return 0

    def readFullSweepSteps(self):
        return 0

    def readCenterPosition(self):
        return 0
    
    def readCalibrationState(self):
        return 0

    def readCalibrationSteps(self):
        return 0

    # body
    def readHeadDesiredPosition(self):
        return 0

    def setHeadDesiredPosition(self, v):
        pass

    def readHeadPosition(self):
        return 0

    def readHeadFullSweepSteps(self):
        return 0

    def readHeadCenterPosition(self):
        return 0
    
    def readHeadCalibrationState(self):
        return 0

    def readHeadCalibrationSteps(self):
        return 0

    def readHeadAutoRotate(self):
        return 0

    def setHeadAutoRotate(self, v):
        pass

    # drive

    def readLimits(self):
        return 0

    def readSpeed(self):
        return 0

    def readDesiredSpeed(self):
        return 0

    def readForward(self):
        return 0

    def readDesiredForward(self):
        return 0

    def readWheelEncoderCount(self):
        return 0

    def setDesiredSpeed(self, speed):
        pass

    def setDesiredForward(self, forward):
        pass

    def echo(self, v):
        pass

    def dumpRegs(self):
        pass


def main():
    pass


if __name__ == "__main__":
    main()