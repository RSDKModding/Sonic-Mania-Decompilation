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

#if !RETRO_USE_ORIGINAL_CODE
#define DEBUG_HITBOX_MAX (0x400)

struct DebugHitboxInfo {
    byte type;
    byte collision;
    ushort entityID;
    Hitbox hitbox;
    Vector2 pos;
};

enum DebugHitboxTypes { H_TYPE_TOUCH, H_TYPE_CIRCLE, H_TYPE_BOX, H_TYPE_PLAT };

extern bool32 showHitboxes;
extern int debugHitboxCount;
extern DebugHitboxInfo debugHitboxList[DEBUG_HITBOX_MAX];

int addDebugHitbox(byte type, Entity *entity, Hitbox *hitbox);
#endif

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

#if !RETRO_USE_ORIGINAL_CODE
    if (showHitboxes) {
        bool32 collided = x * x + y * y < r * r;
        Hitbox thisHitbox;
        Hitbox otherHitbox;
        thisHitbox.left = thisOffset >> 16;
        otherHitbox.left = otherOffset >> 16;

        int thisHitboxID  = addDebugHitbox(H_TYPE_CIRCLE, thisEntity, &thisHitbox);
        int otherHitboxID = addDebugHitbox(H_TYPE_CIRCLE, otherEntity, &otherHitbox);

        if (thisHitboxID >= 0 && collided)
            debugHitboxList[thisHitboxID].collision |= 1 << (collided - 1);
        if (otherHitboxID >= 0 && collided)
            debugHitboxList[otherHitboxID].collision |= 1 << (collided - 1);
    }
#endif
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