#ifndef COLLISION_H
#define COLLISION_H

enum {
    CMODE_FLOOR,
    CMODE_LWALL,
    CMODE_ROOF,
    CMODE_RWALL,
};

bool32 CheckObjectCollisionTouch(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox);

inline bool32 CheckObjectCollisionCircle(Entity *thisEntity, int thisOffset, Entity *otherEntity, int otherOffset)
{
    int y = (thisEntity->position.y - otherEntity->position.y) >> 16;
    return ((thisEntity->position.x - otherEntity->position.x) >> 16) * ((thisEntity->position.x - otherEntity->position.x) >> 16) + y * y
           < ((thisOffset + otherOffset) >> 16) * ((thisOffset + otherOffset) >> 16);
}

#endif // !COLLISION_H