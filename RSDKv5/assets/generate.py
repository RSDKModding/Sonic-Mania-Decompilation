import os, enum, pathlib

if not os.path.exists("generated"):
    os.mkdir("generated")

class Types(enum.Enum):
    TEXT = 0
    IMAGE = 1

EXT_MAP = {
    ".vert": Types.TEXT,
    ".frag": Types.TEXT,
    ".gif": Types.IMAGE
}

for file in pathlib.Path(".").rglob("*"):
    if not file.is_file(): continue
    elif str(file).startswith("generated"): continue
    try:
        t = EXT_MAP[file.suffix]
    except: continue
    if not ("generated" / file.parent).exists():
        ("generated" / file.parent).mkdir()
    if t == Types.TEXT:
        s = open(file, "r").read()
        arr = [hex(ord(x)) for x in s]
        with open("generated" / file, "w") as f:
            name = file.name
            name = ''.join(name.split(".")[:-1]) + name.split(".")[-1].title()
            f.write(f"const char {name}[] = {{{', '.join(arr)}}};")
        print(file)

