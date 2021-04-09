#ifndef OBJ_RING_H
#define OBJ_RING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    int pan;
    ushort spriteIndex;
    ushort sfx_Ring;
} ObjectRing;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
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
    Animator animData;
} EntityRing;

// Object Struct
extern ObjectRing *Ring;

// Standard Entity Events
void Ring_Update(void);
void Ring_LateUpdate(void);
void Ring_StaticUpdate(void);
void Ring_Draw(void);
void Ring_Create(void* data);
void Ring_StageLoad(void);
void Ring_EditorDraw(void);
void Ring_EditorLoad(void);
void Ring_Serialize(void);

// Extra Entity Functions
void Ring_DebugSpawn(void);
void Ring_DebugDraw(void);
void Ring_StateDraw_Normal(void);
void Ring_StateDraw_Oscillating(void);
void Ring_StateDraw_Sparkle(void);
void Ring_State_Attract(void);
void Ring_State_Big(void);
void Ring_State_Bounce(void);
void Ring_State_Circular(void);
void Ring_State_Grow(void);
void Ring_State_Move(void);
void Ring_State_Normal(void);
void Ring_State_Path(void);
void Ring_State_Sparkle(void);
void Ring_State_Track(void);
void Ring_CheckObjectCollisions(int offsetX, int offsetY);
byte Ring_CheckPlatformCollisions(EntityPlatform *platform);
void Ring_Collect(void);
void Ring_FakeLoseRings(Entity *entity, int ringCount, byte drawOrder);

#endif //!OBJ_RING_H
