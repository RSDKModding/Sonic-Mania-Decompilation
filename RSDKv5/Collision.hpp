#ifndef COLLISION_H
#define COLLISION_H

enum {
    CMODE_FLOOR,
    CMODE_LWALL,
    CMODE_ROOF,
    CMODE_RWALL,
};

struct CollisionSensor {
    Vector2 pos;
    bool32 collided;
    byte angle;
};

extern int collisionTolerance;
extern int collisionOffset;
extern int collisionMaskAir;

extern int collisionOuter_Left;
extern int collisionOuter_Top;
extern int collisionOuter_Right;
extern int collisionOuter_Bottom;

extern int collisionInner_Left;
extern int collisionInner_Top;
extern int collisionInner_Right;
extern int collisionInner_Bottom;

extern Entity *collisionEntity;

extern CollisionSensor sensors[6];

bool32 CheckObjectCollisionTouch(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox);
inline bool32 CheckObjectCollisionCircle(Entity *thisEntity, int thisOffset, Entity *otherEntity, int otherOffset)
{
    int x = (thisEntity->position.x - otherEntity->position.x) >> 16;
    int y = (thisEntity->position.y - otherEntity->position.y) >> 16;
    int r = (thisOffset + otherOffset) >> 16;
    return x * x + y * y < r * r;
}
byte CheckObjectCollisionBox(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox, bool32 setValues);
bool32 CheckObjectCollisionPlatform(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox, bool32 setValues);

bool32 ObjectTileCollision(Entity *entity, ushort cLayers, char cMode, char cPlane, int xOffset, int yOffset, bool32 setPos);
bool32 ObjectTileGrip(Entity *entity, ushort cLayers, char cMode, char cPlane, int xOffset, int yOffset, int tolerance);

void ProcessTileCollisions(Entity *entity, Hitbox *outerBox, Hitbox *innerBox);

void ProcessPathGrip();
void ProcessAirCollision();

void SetPathGripSensors(CollisionSensor *cSensors);

void FindFloorPosition(CollisionSensor *sensor);
void FindLWallPosition(CollisionSensor *sensor);
void FindRoofPosition(CollisionSensor *sensor);
void FindRWallPosition(CollisionSensor *sensor);

void FloorCollision(CollisionSensor *sensor);
void LWallCollision(CollisionSensor *sensor);
void RoofCollision(CollisionSensor *sensor);
void RWallCollision(CollisionSensor *sensor);

#endif // !COLLISION_H