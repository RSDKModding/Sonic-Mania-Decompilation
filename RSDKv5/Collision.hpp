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
byte CheckObjectCollisionBox(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox, bool32 setValues);
bool32 CheckObjectCollisionPlatform(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox, bool32 setValues);

bool32 ObjectTileCollision(Entity *entity, ushort cLayers, char cMode, char cPlane, int xOffset, int yOffset, bool32 setPos);
bool32 ObjectTileGrip(Entity *entity, ushort cLayers, char cMode, char cPlane, int xOffset, int yOffset, int tolerance);

#endif // !COLLISION_H