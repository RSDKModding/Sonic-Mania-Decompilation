import os, sys, re
from typing import IO, BinaryIO, Dict, Iterable, Iterator, List, Tuple
from pathlib import Path

USE64 = False

if len(sys.argv) < 2:
    exit(1)

try: os.mkdir("Static")
except: pass

TYPEMAP = {
    "byte": 1,
    "ushort": 2, 
    "uint": 4, 
    "sbyte": 1,
    "short": 2,
    "int": 4,
    "bool32": 4,
    "ptr": 0,
    "Vector2": 0,
    "TextInfo": 0,
    "Animator": 0,
    "Hitbox": 0,
    "----": 0
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
        type = "ptr"
    if type.endswith("*"):
        type = "ptr"
        type.replace("*", "")

    if infront() == "(":
        type = "ptr"
        readuntil("*") #da name!!!
    
    if type in ("color", "colour"):
        type = "uint" 
    if type not in TYPEMAP:
        print(f"UNKNOWN TYPE {type} IN {os.path.basename(path)} OBJECT STRUCT")
        errflag = 1
        return
    name = readuntil(delim)
    relseek(-2)
    asize = 1
    if currentfile.read(1) == "]":
        backward()
        readuntil("[")
        sp = curpos
        try: asize = int(readuntil("]"), 0)
        except: 
            print(f"INVALID ARRAY SIZE IN {os.path.basename(path)} OBJECT STRUCT")
            errflag = 1
            return

        name = name[:(sp - curpos - 1)]
    backward()
    readuntil(delim) #read just to be sure
    t.append((name, tuple(TYPEMAP.keys()).index(type), asize, []))

plus = len(sys.argv) < 3 or sys.argv[2] == "plus"
for path in Path(sys.argv[1]).rglob("*.h"):
    mode = 0
    # print(f"{path} STARTED")
    prepare(path)
    for l in readlines():
        if errflag:
            break
        
        if ("".join(l.split()).startswith("#ifRETRO_USE_PLUS") and not plus) or ("".join(l.split()).startswith("#if!RETRO_USE_PLUS") and plus):
            mode = 5
            continue
        elif ("".join(l.split()).startswith("#ifRETRO_USE_PLUS") and plus) or ("".join(l.split()).startswith("#if!RETRO_USE_PLUS") and not plus):
            continue #ok idc
        elif "".join(l.split()).startswith("#endif"):
            continue #ok idc
        elif "".join(l.split()).startswith("#else"):
            mode = 5
            continue #ok do care

        if mode < 5 and l.strip() == "typedef struct {":
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
                continue
            if l.strip().startswith("} "):
                backward()
                readuntil(" ")
                name = readuntil(";")
                if not name.startswith("Object"):
                    print(f"{name} NOT AN OBJECT STRUCT BUT HAS RSDK_OBJECT ({os.path.basename(path)})")
                    break
                name = name[6:]
                print(f"{name} FINISHED")
                objects[name] = t
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
                try: t[-1][3].append(int(readuntil(","), 0))
                except: 
                    print(f"INCORRECT INT IN {t[-1][0]} TABLE OR TABLE TOO SMALL IN {os.path.basename(path)}, EXPECTED: {expected}")
                    errflag = 1
                    break
            if errflag:
                break
            try: t[-1][3].append(int(readuntil("}"), 0))
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
            try: t[-1][3].append(int(readuntil(")"), 0))
            except: 
                print(f"INCORRECT STATIC INT IN {t[-1][0]} OF {os.path.basename(path)}")
                break
            readuntil(";")
            mode = 2
            continue
        if mode == 5:
            if l.strip().startswith("#else") or l.strip().startswith("#endif"):
                mode = 0 #STOP!!!!

print("\n\n\n\nStatic Objs")
for key in objects:
    b = bytearray([ord('O'), ord('B'), ord('J'), 0])
    hasVal = False

    for tup in objects[key]:
        type = tup[1]
        if tup[3]:
            hasVal = True
            type |= 0x80
        b.append(type)
        b.extend(tup[2].to_bytes(4, 'little')) #array size
        if type >= 0x80:
            b.extend(tup[2].to_bytes(4, 'little')) #data size
            size = tuple(TYPEMAP.values())[tup[1]]
            for val in tup[3]:
                b.extend(val.to_bytes(size, byteorder='little', signed=False if val >= 0 else True)) #val
    
    if hasVal:
        name = key
        if len(sys.argv) < 4 or sys.argv[3] != "debug":
            name = "00000000000000000000000000000000" #TODO: idk how to do hashes and I cba to learn rn :LOL

        with open(f"Static/{name}.bin", "wb") as file:
            file.write(b)
            file.close()
                





