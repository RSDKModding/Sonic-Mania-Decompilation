import os, sys, hashlib
from typing import IO, BinaryIO, Dict, Iterable, Iterator, List, Tuple
from pathlib import Path

USE64 = False

for arg in sys.argv:
    print(f"{arg}")

if len(sys.argv) < 2:
    print(f"Not enough arguments supplied... Exiting...")
    exit(1)

TYPEMAP = {
    "uint8": 1,
    "uint16": 2, 
    "uint32": 4, 
    "int8": 1,
    "int16": 2,
    "int32": 4,
    "bool32": 4,
    "Pointer": 0,
    "Vector2": 0,
    "String": 0,
    "Animator": 0,
    "Hitbox": 0,
    "SpriteFrame": 0
}

ALIASES = {
    "color": "int32",
    "char": "uint8"
}

DEFINES = {
    "PLAYER_COUNT": 4,
    "CAMERA_COUNT": 4,
    "SCREEN_COUNT": 4,
}

objects: Dict[str, List[Tuple[str, int, int, List[int]]]] = {}
currentfile: IO
curpos = 0
lastpos = 0
errflag = 0
path = ""

def relseek(where):
    currentfile.seek(currentfile.tell() + where, 0)

def prepare(filename):
    global currentfile, curpos, lastpos, errflag, path
    currentfile = open(filename)
    curpos = 0
    lastpos = 0
    errflag = 0
    path = filename

def readlines() -> Iterator[str]:
    global currentfile, curpos, lastpos
    while currentfile.readable():
        lastpos = curpos
        r = currentfile.readline()
        if not r:
            break
        curpos = currentfile.tell()
        if r.strip() == "" or r.strip().startswith("//"):
            continue
        yield r[:-1]

def readuntil(delim) -> str:
    r = ""
    c = ""
    global currentfile, curpos, lastpos
    lastpos = curpos
    while currentfile.readable():
        c = currentfile.read(1)
        curpos += 1
        if c == delim: break
        r += c
    return r

def readuntiltext():
    r = ""
    c = ""
    global currentfile, curpos, lastpos
    lastpos = curpos
    while currentfile.readable():
        c = currentfile.read(1)
        if c.isalpha(): 
            relseek(-1)
            break
        curpos += 1
        r += c
    return r

def infront():
    r = currentfile.read(1)
    relseek(-1)
    return r

def backward():
    global lastpos, curpos
    currentfile.seek(lastpos, 0)
    curpos = lastpos

t: List[Tuple[str, int, int, List[int]]] = []

def deduce(delim):
    global errflag, path
    readuntiltext()
    type = readuntil(" ")
    
    if infront() == "*":
        type = "Pointer"
    if type.endswith("*"):
        type = "Pointer"
        type.replace("*", "")

    if infront() == "(":
        type = "Pointer"
        readuntil("*") #da name!!!
    
    if type in ALIASES:
        type = ALIASES[type]

    if type not in TYPEMAP:
        print(f"UNKNOWN TYPE {type} IN {os.path.basename(path)} OBJECT STRUCT, info: {type}")
        errflag = 1
        return
    name = readuntil(delim)
    currentfile.seek(curpos - 2, 0) # relseek didn't wanna work so

    asize = 1
    if currentfile.read(1) == "]":
        backward()
        readuntil("[")
        sp = curpos
        try: asize = eval(readuntil("]").strip(), DEFINES)
        except: 
            print(f"INVALID ARRAY SIZE IN {os.path.basename(path)} OBJECT STRUCT, info: {name}")
            errflag = 1
            return

        name = name[:(sp - curpos - 1)]
    backward()
    readuntil(delim) #read just to be sure
    t.append((name, tuple(TYPEMAP.keys()).index(type), asize, []))

debugMode   = False if len(sys.argv) < 5 else sys.argv[4] == "debug"
plus        = len(sys.argv) < 4 or sys.argv[3] == "plus"
folder      = "" if len(sys.argv) < 3 else sys.argv[2]
staticCount = 0

try: os.mkdir(f"{folder}Static")
except: pass

for path in Path(sys.argv[1]).rglob("*.h"):
    mode        = 0
    prevMode    = 0
    objName     = "[Object]"
    prepare(path)
    if debugMode:
        print(f"Scanning '{path}'")
    for l in readlines():
        if errflag:
            break
        clnL = "".join(l.split())

        if mode == 5:
            if clnL.startswith("#else") or clnL.startswith("#endif"):
                mode = prevMode #return to where we left off
            continue
        
        if (clnL.startswith("#ifMANIA_USE_PLUS") and not plus) or (clnL.startswith("#if!MANIA_USE_PLUS") and plus):
            prevMode = mode
            mode = 5
            continue
        elif (clnL.startswith("#ifMANIA_USE_PLUS") and plus) or (clnL.startswith("#if!MANIA_USE_PLUS") and not plus):
            continue #ok idc
        elif clnL.startswith("#endif"):
            continue #ok idc
        elif clnL.startswith("#else"):
            prevMode = mode
            mode = 5
            continue #ok do care

        if mode < 5 and l.strip().startswith("struct Object"):
            backward()
            readuntil(" ")
            objName = readuntil(" ")
            readuntil("{")
            if debugMode:
                print(f"Found Object: '{objName}'")

            mode = 1
            continue

        if mode == 1 and l.strip() == "RSDK_OBJECT":
            mode = 2
            t = []
            continue

        if mode == 2:
            if l.strip().startswith("TABLE"):
                mode = 3
                backward()
                continue
            if l.strip().startswith("STATIC"):
                mode = 4
                backward()
                continue
            if l.strip().startswith("StateMachine"):
                backward()
                readuntil("(")
                stateName = readuntil(")")
                t.append((stateName, tuple(TYPEMAP.keys()).index("Pointer"), 1, []))
                readuntil("\n") #hack :LOL:
                continue
            if l.strip().startswith("}"):
                backward()
                readuntil(";")
                if not objName.startswith("Object"):
                    print(f"{objName} NOT AN OBJECT STRUCT BUT HAS RSDK_OBJECT ({os.path.basename(path)})")
                    break
                objName = objName[6:]
                if debugMode:
                    print(f"{objName} FINISHED")
                objects[objName] = t
                t = []
                mode = 0
                continue #I changed this specifically because of "SPZSetup" & "SPZ2Setup" sharing a file :LOL:
            backward()
            deduce(";")

        if mode == 3:
            backward()
            readuntil("(")
            deduce(",")
            if errflag:
                break
            if t[-1][1] > tuple(TYPEMAP.keys()).index("bool32"):
                print(f"INCORRECT TABLE TYPE {tuple(TYPEMAP.keys())[t[-1][1]]} ({t[-1][0]} of {os.path.basename(path)}")
                break
            expected = t[-1][2]
            readuntil("{")
            for i in range(expected - 1):
                try: t[-1][3].append(eval(readuntil(",").strip(), DEFINES))
                except: 
                    print(f"INCORRECT INT IN {t[-1][0]} TABLE OR TABLE TOO SMALL IN {os.path.basename(path)}, EXPECTED: {expected}")
                    errflag = 1
                    break
            if errflag:
                break
            try: t[-1][3].append(eval(readuntil("}").strip(), DEFINES))
            except: 
                print(f"INCORRECT INT IN {t[-1][0]} TABLE OR TABLE TOO BIG IN {os.path.basename(path)}, EXPECTED: {expected}")
                break
            readuntil(";")
            mode = 2
            continue

        if mode == 4:
            backward()
            readuntil("(")
            deduce(",")
            if t[-1][1] > tuple(TYPEMAP.keys()).index("bool32"):
                print(f"INCORRECT STATIC TYPE {tuple(TYPEMAP.keys())[t[-1][1]]} ({t[-1][0]} of {os.path.basename(path)}")
                break
            try: t[-1][3].append(eval(readuntil(")").strip(), DEFINES))
            except: 
                print(f"INCORRECT STATIC INT IN {t[-1][0]} OF {os.path.basename(path)}")
                break
            readuntil(";")
            mode = 2
            continue

for key in objects:
    b = bytearray([ord('O'), ord('B'), ord('J'), 0])
    hasVal = False

    hash = key
    if len(sys.argv) < 5 or sys.argv[4] != "debug":
        hash = hashlib.md5(key.encode("utf-8")).hexdigest()
        hash = (''.join([c[1] + c[0]
                         for c in zip(hash[::2], hash[1::2])])).upper()

    if debugMode:
        print(key, hash)

    for name, type, size, arr in objects[key]:
        if arr:
            hasVal = True
            type |= 0x80
        b.append(type)
        b.extend(size.to_bytes(4, 'little')) #array size
        if type >= 0x80:
            b.extend(size.to_bytes(4, 'little')) #data size
            bs = tuple(TYPEMAP.values())[type & (~0x80)]
            for val in arr:
                b.extend(val.to_bytes(bs, byteorder='little', signed=(type & (~0x80)) in range(4, 7))) #val
        
        if debugMode:
            print("  ", tuple(TYPEMAP.keys())[type & (~0x80)], name + (f"[{size}]" if arr else ""))

    if hasVal:
        if debugMode:
            print(f"Building Object: '{key} [{hash}]'")

        with open(f"{folder}Static/{hash}.bin", "wb") as file:
            file.write(b)
            file.close()
            staticCount = staticCount + 1
                

print(f"Built {staticCount} Static Objects in: {folder}Static/")




