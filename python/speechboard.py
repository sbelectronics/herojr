from smbpi.i2c_with_crc import I2CWithCrc, I2CError, ReceiveCRCError, ReceiveSizeError, ReceiveUninitializedError, CRCError, NoMoreRetriesError
from phrases import PHRASES

REG_RESET = 0
REG_ECHO = 1
REG_SPEECH_LAST_PHONEME_OUT = 10
REG_SPEECH_BUF_COUNT = 11
REG_SPEECH_STATE = 12
REG_SPEECH_QUEUE_PHONEME = 13

class SpeechBoard(I2CWithCrc):
    def __init__(self, bus=None, addr=0x5, pi=None, sdaPin=2, sclPin=3):
        I2CWithCrc.__init__(self, bus, addr, pi, sdaPin, sclPin)

    def readSpeechLastPhonemeOut(self):
        return self.readReg(REG_SPEECH_LAST_PHONEME_OUT)

    def readSpeechBufCount(self):
        return self.readReg(REG_SPEECH_BUF_COUNT)

    def readSpeechState(self):
        return self.readReg(REG_SPEECH_STATE)

    def addPhoneme(self, v):
        return self.writeReg(REG_SPEECH_QUEUE_PHONEME, v)

    def sayPhonemeList(self, phonemes, stop=True):
        lastPhoneme = 0x3F
        for phoneme in phonemes:
            if phoneme == 0xFF:
                continue
            self.addPhoneme(phoneme)
            lastPhoneme = phoneme
        
        if stop and (lastPhoneme not in [0x03, 0x3E, 0x3F]):
            self.addPhoneme(0x3F)

    def say(self, v):
        if v in PHRASES:
            self.sayPhonemeList(PHRASES[v])

    def echo(self, v):
        self.writeReg(REG_ECHO, v)
        readBack = self.readReg(REG_ECHO)
        if readBack != v:
            print "ERROR! Echo failed", v, readBack

    def dumpRegs(self):
        lastPhonemeOut = self.readSpeechLastPhonemeOut()
        bufCount = self.readSpeechBufCount()
        state = self.readSpeechState()
        print "lastPhoneme=%02X, bufCount=%d, state=%d" % (lastPhonemeOut, bufCount, state)
        print ""


# Note: this won't see repeated phonemes
def watchPhonemes(sb):
    lastPhon = 0
    while (True):
        phon = sb.readSpeechLastPhonemeOut()
        if (phon != lastPhon):
            print "%2X" % phon
            lastPhon = phon

def main():
    import smbus
    bus = smbus.SMBus(1)
    sb = SpeechBoard(bus=bus)

    diags(sb)


if __name__ == "__main__":
    main()