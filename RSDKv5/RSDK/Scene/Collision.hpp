#ifndef COLLISION_H
#define COLLISION_H

namespace RSDK
{

enum CollisionModes {
    CMODE_FLOOR,
    CMODE_LWALL,
    CMODE_ROOF,
    CMODE_RWALL,
};

struct CollisionSensor {
    Vector2 pos;
    bool32 collided;
    uint8 angle;
};

#if !RETRO_USE_ORIGINAL_CODE
#define DEBUG_HITBOX_COUNT (0x400)

struct DebugHitboxInfo {
    uint8 type;
    uint8 collision;
    uint16 entityID;
    Hitbox hitbox;
    Vector2 pos;
};

enum DebugHitboxTypes { H_TYPE_TOUCH, H_TYPE_CIRCLE, H_TYPE_BOX, H_TYPE_PLAT };

extern bool32 showHitboxes;
extern int32 debugHitboxCount;
extern DebugHitboxInfo debugHitboxList[DEBUG_HITBOX_COUNT];

int32 addDebugHitbox(uint8 type, uint8 dir, Entity *entity, Hitbox *hitbox);
#endif

extern int32 collisionTolerance;
extern int32 collisionOffset;
extern int32 collisionMaskAir;

extern int32 collisionOuter_Left;
extern int32 collisionOuter_Top;
extern int32 collisionOuter_Right;
extern int32 collisionOuter_Bottom;

extern int32 collisionInner_Left;
extern int32 collisionInner_Top;
extern int32 collisionInner_Right;
extern int32 collisionInner_Bottom;

extern Entity *collisionEntity;

extern CollisionSensor sensors[6];

bool32 CheckObjectCollisionTouch(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox);
inline bool32 CheckObjectCollisionCircle(Entity *thisEntity, int32 thisRadius, Entity *otherEntity, int32 otherRadius)
{
    int32 x = (thisEntity->position.x - otherEntity->position.x) >> 16;
    int32 y = (thisEntity->position.y - otherEntity->position.y) >> 16;
    int32 r = (thisRadius + otherRadius) >> 16;

#if !RETRO_USE_ORIGINAL_CODE
    if (showHitboxes) {
        bool32 collided = x * x + y * y < r * r;
        Hitbox thisHitbox;
        Hitbox otherHitbox;
        thisHitbox.left  = thisRadius >> 16;
        otherHitbox.left = otherRadius >> 16;

        int32 thisHitboxID  = addDebugHitbox(H_TYPE_CIRCLE, FLIP_NONE, thisEntity, &thisHitbox);
        int32 otherHitboxID = addDebugHitbox(H_TYPE_CIRCLE, FLIP_NONE, otherEntity, &otherHitbox);

        if (thisHitboxID >= 0 && collided)
            debugHitboxList[thisHitboxID].collision |= 1 << (collided - 1);
        if (otherHitboxID >= 0 && collided)
            debugHitboxList[otherHitboxID].collision |= 1 << (collided - 1);
    }
#endif

    return x * x + y * y < r * r;
}
uint8 CheckObjectCollisionBox(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox, bool32 setValues);
bool32 CheckObjectCollisionPlatform(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox, bool32 setValues);

bool32 ObjectTileCollision(Entity *entity, uint16 cLayers, uint8 cMode, uint8 cPlane, int32 xOffset, int32 yOffset, bool32 setPos);
bool32 ObjectTileGrip(Entity *entity, uint16 cLayers, uint8 cMode, uint8 cPlane, int32 xOffset, int32 yOffset, int32 tolerance);

void ProcessObjectMovement(Entity *entity, Hitbox *outerBox, Hitbox *innerBox);

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

} // namespace RSDK

#endif // !COLLISION_H
