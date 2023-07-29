import os

filenames = []

for dir_, _, files in os.walk("Objects"):
    for file_name in files:
        rel_dir = os.path.relpath(dir_, "Objects")
        filenames.append(f"{rel_dir}/{file_name}")

files = ["\t${GAME_NAME}/Objects/" + f + "\n" for f in filenames if f.endswith(".c") and not f.endswith("All.c")]

with open("Objects.cmake", "w") as f:
    f.writelines(["set(GENERATED_SOURCES\n"] + files + [")"])
