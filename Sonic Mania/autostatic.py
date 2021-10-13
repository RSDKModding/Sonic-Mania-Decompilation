import os, sys, re
from typing import IO, Dict, Iterable, Iterator, List, Tuple
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
    "char": 1,
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
        curpos += len(r)
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
    readuntil(delim) #readjust to be sure
    t.append((name, tuple(TYPEMAP.keys()).index(type), asize, []))

for path in Path(sys.argv[1]).rglob("*.h"):
    mode = 0
    prepare(path)
    for l in readlines():
        if errflag:
            break
        if l.strip() == "typedef struct {":
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
                break
            backward()
            deduce(";")
        if mode == 3:
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
                    print(f"INCORRECT INT IN {t[-1][0]} TABLE OR TABLE TOO SMALL IN {os.path.basename(path)}")
                    errflag = 1
                    break
            if errflag:
                break
            try: t[-1][3].append(int(readuntil("}"), 0))
            except: 
                print(f"INCORRECT INT IN {t[-1][0]} TABLE OR TABLE TOO BIG IN {os.path.basename(path)}")
                break
            readuntil(";")
            mode = 2
            continue
        if mode == 4:
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

                





