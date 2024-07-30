import os, sys, hashlib
import re
from typing import IO, BinaryIO, Dict, Iterable, Iterator, List, Tuple
from pathlib import Path

if len(sys.argv) < 2:
    print(f"No Objects path specified... Exiting...")
    exit(1)

events = ("Update", "LateUpdate", "StaticUpdate", "Draw", "Create", "StageLoad", "EditorLoad", "EditorDraw", "Serialize")

print("void InitPublicFunctions()\n{")
for path in Path(sys.argv[1]).rglob("*.h"):
    done = False
    prepros = ""
    hasPrepos = False
    f = open(path, "r")
    while (line := f.readline()) != "":
        if (line != line.lstrip() or line.startswith("//")): 
            continue
        line = line.rstrip()
        if (match := re.fullmatch(r"([a-zA-Z0-9]* ?\**) *([^(]*)\((.*)(,|\);)(\s*\/\/.*)?", line)) != None:
            ret, name, args, end, comment = match.groups()
            if name[len(path.stem) + 1:] in events:
                continue
            if (not done): 
                print(f"    // {path.parent.name}/{path.stem}")
                done = True
                if (prepros == "#endif"):
                    prepros = ""
                    hasPrepos = False
            if prepros and (prepros != "#endif"):
                print(prepros)
                prepros = ""
                hasPrepos = True
            if (prepros == "#endif" and hasPrepos == True):
                print(prepros)
                prepros = ""
                hasPrepos = False
            print(f"    ADD_PUBLIC_FUNC({name});")
            if (prepros == "#endif"):
                print(prepros)
                prepros = ""
                hasPrepos = False
        elif line.startswith("#"):
            prepros = line
        else:
            prepros = ""
    if (hasPrepos):
        print("#endif")
    if (done):
        print()
print("}")
