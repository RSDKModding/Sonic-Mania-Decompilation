#ifndef COLLISION_H
#define COLLISION_H

enum {
    CMODE_FLOOR,
    CMODE_LWALL,
    CMODE_ROOF,
    CMODE_RWALL,
};

struct Hitbox {
    short left;
    short top;
    short right;
    short bottom;
};

#endif // !COLLISION_H