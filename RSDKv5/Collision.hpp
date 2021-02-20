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
    int angle;
    bool32 collided;
};

extern int collisionTolerance;
extern int collisionOffset;
extern int collisionMaskAir;

extern int collisionLeft_Outer;
extern int collisionTop_Outer;
extern int collisionRight_Outer;
extern int collisionBottom_Outer;

extern int collisionLeft_Inner;
extern int collisionTop_Inner;
extern int collisionRight_Inner;
extern int collisionBottom_Inner;

extern Entity *collisionEntity;

extern CollisionSensor sensors[6];

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