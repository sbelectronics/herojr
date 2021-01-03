import sys

def removeNull(line):
    dest = ""
    for x in line:
        if x!=chr(0):
            dest = dest + x
    return dest


def getComment(line):
    # a comment line has a lot of whitespace
    if ("                ") not in line:
        return None

    if ("FCB") in line:
        return None

    line = line.split("                ")[1]

    line = line.lstrip(" *.,'\"=")
    if not line:
        return None

    if line.startswith("A") or line.startswith("B"):
        if len(line)==4:
            return "***JUNK"

    return line

def emitC(comment, vals):
    if not comment:
        return

    name = "SPK_" + comment.replace(" ", "_").replace(",","_")
    c = 1
    print "#define %s_LEN %d" % (name, len(vals))
    print "const uint8_t %s[%s_LEN] = {" % (name, name)
    print "       ",
    for val in vals:
        if (c==len(vals)):
            print "0x%02X" % val,
        else:
            print "0x%02X," % val,
        if (c%8) == 0:
            print ""
            print "       ",
        c=c+1
    if (c%8)!=1: print ""
    print "}"

def emit(comment, vals):
    if not comment:
        return

    name = "SPK_" + comment.replace(" ", "_").replace(",","_").replace(".","_")
    c = 1
    print "%s = [" % (name)
    print "       ",
    for val in vals:
        if (c==len(vals)):
            print "0x%02X" % val,
        else:
            print "0x%02X," % val,
        if (c%8) == 0:
            print ""
            print "       ",
        c=c+1
    if (c%8)!=1: print ""
    print "]"


lines = open(sys.argv[1]).readlines()
lineNo = 0
vals = []
curComment = None
for line in lines:
    lineNo += 1
    line = removeNull(line)
    line = line.strip()

    line = line.replace(":A76rvEN", "SEVEN")

    if not line:
        continue

    com = getComment(line)
    if com!=None:
        if (com != "***JUNK"):
            emit(curComment, vals)
            curComment = com
            vals = []
        continue

    line = line[:28]
    line = line.strip()

    line = line.replace("'", " ")

    parts = line.split(" ")
    for part in parts:
        if len(part)==4:
            continue
        part = part.replace("o", "a").replace("O", "0").replace("i","1").replace("S","5").replace("l","1").replace("s","a").replace("I","1")

        try:
            val = int("0x"+part, 16)
        except:
            print >> sys.stderr,  "ERROR: failed to convert value '%s' in line %d\n  line: %s" %(part, lineNo, line)
            continue

        if (val<0) or (val>255):
            print >> sys.stderr,  "ERROR: value out of limits '%s' in line %d\n  line: %s" %(part, lineNo, line)
            continue

        vals.append(val)

emit(curComment, vals)

#c = 1
#print "const uint8_t PHONEMES[%d] = {" % len(vals)
#print "       ",
#for val in vals:
#    print "0x%02X," % val,
#    if (c%8) == 0:
#        print ""
#        print "       ",
#    c=c+1
#if (c%8)!=1: print ""
#print "}"

