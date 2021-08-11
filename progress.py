import os, re
d = {}
completed = 0
count = 0
p = "Sonic Mania/Objects/"
for x in os.listdir(p):
    if os.path.isdir(p + x):
        d.update({x: []})
        for y in os.listdir(p + x):
            if y.endswith(".h"): continue
            c = not bool(re.search(
                r"void .*_Create\(void\* data\).*(\n|\s)?\{\n?\n?\}", open(p + x + "/" + y).read()))
            d[x].append((y[:-2], c))
            if c:
                completed += 1
            count += 1

for x in d:
    print(x)
    for n, b in d[x]:
        print(f"- [{'x' if b else ' '}] `{n}`")
    
print(f"\n{completed}/{count}")
print(completed / count * 100)



