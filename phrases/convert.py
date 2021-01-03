import sys

lines = open(sys.argv[1]).readlines()
lineNo = 0
vals = []
for line in lines:
    lineNo += 1
    line = line.strip()
    parts = line.split(",")
    for part in parts:
        if part[0]!="$":
            print "illegal character line %d, part %s\nline: %s" % (lineNo, part, line)
            sys.exit(-1)
        try:
            v = int(part[1:], 16)
        except:
            print "failed to convert to hex line %d, part %s\nline: %s" % (lineNo, part, line)
            sys.exit(-1)
        vals.append(v)

c = 1
print "const uint8_t PHONEMES[%d] = {" % len(vals)
print "       ",
for val in vals:
    print "0x%02X," % val,
    if (c%8) == 0:
        print ""
        print "       ",
    c=c+1
if (c%8)!=1: print ""
print "}"

