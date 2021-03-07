#ifndef OBJ_RING_H
#define OBJ_RING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectRing : Object {
    Hitbox hitbox;
    int pan;
    short spriteIndex;
    short sfx_Ring;
};

// Entity Class
struct EntityRing : Entity {
    void (*state)();
    void (*stateDraw)();
    int type;
    int planeFilter;
    int ringAmount;
    int timer;
    int maxFrameCount;
    int sparkleType;
    EntityPlayer *storedPlayer;
    int moveType;
    Vector2 amplitude;
    int speed;
    Vector2 offset;
    EntityAnimationData animData;
};

// Object Struct
extern ObjectRing *Ring;

// Standard Entity Events
void Ring_Update();
void Ring_LateUpdate();
void Ring_StaticUpdate();
void Ring_Draw();
void Ring_Create(void* data);
void Ring_StageLoad();
void Ring_EditorDraw();
void Ring_EditorLoad();
void Ring_Serialize();

// Extra Entity Functions
void Ring_DebugSpawn();
void Ring_DebugDraw();
void Ring_StateDraw_Normal();
void Ring_StateDraw_Oscillating();
void Ring_StateDraw_Sparkle();
void Ring_State_Attract();
void Ring_State_Big();
void Ring_State_Bounce();
void Ring_State_Circular();
void Ring_State_Grow();
void Ring_State_Move();
void Ring_State_Normal();
void Ring_State_Path();
void Ring_State_Sparkle();
void Ring_State_Track();
void Ring_CheckObjectCollisions(int offsetX, int offsetY);
byte Ring_CheckPlatformCollisions(EntityPlatform *platform);
void Ring_Collect();
void Ring_FakeLoseRings(Entity *entity, int ringCount, byte drawOrder);

#endif //!OBJ_RING_H
