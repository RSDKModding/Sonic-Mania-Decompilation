import os, re
d = {}
d2 = {}
pc = 0
pd = 0
completed = 0
count = 0
p = "Sonic Mania/Objects/"
FORCETRUE = ['Pendulum', 'CompetitionSession', 'MenuParam', 'ParticleHelpers', 'FBZTrash']
for x in os.listdir(p):
    if os.path.isdir(p + x):
        d.update({x: []})
        for y in os.listdir(p + x):
            if y.endswith(".h"): continue
            t = open(p + x + "/" + y).read()
            c = not (re.search(r"void .*_Create\(void\* data\).*(\n|\s)?\{\n?\n?\}", t) and re.search(r"void .*_StageLoad\(void\).*(\n|\s)?\{\n?\n?\}", t))
            if y[:-2] in FORCETRUE: c = True
            d[x].append((y[:-2], c))
            if c:
                completed += 1
            count += 1
        d2.update({x: f" {completed - pd}/{count - pc}"})
        pd = completed
        pc = count

for x in d:
    print(x + d2[x])
    for n, b in d[x]:
        print(f"- [{'x' if b else ' '}] `{n}`")
    
print(f"\n{completed}/{count}")
print(completed / count * 100)



