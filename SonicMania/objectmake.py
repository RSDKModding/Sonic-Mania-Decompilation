import os
from os import walk

filenames = []

for dir_, _, files in os.walk("Objects"):
    for file_name in files:
        rel_dir = os.path.relpath(dir_, "Objects")
        filenames.append(f"{rel_dir}/{file_name}")

files = [f"Game/Objects/" + f[:-2] + " \\\n" for f in filenames if f.endswith(".c") and not f.endswith("All.c")]

f = open("Objects.cfg", "w")
f.writelines(["GAME_SOURCES += \\\n"] + files)
