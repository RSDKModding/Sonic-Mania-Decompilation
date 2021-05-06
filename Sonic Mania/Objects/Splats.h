#ifndef OBJ_SPLATS_H
#define OBJ_SPLATS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    StateMachine(state);
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    ushort aniFrames;
    ushort sfxSplatsSpawn;
    ushort sfxSplatsLand;
} ObjectSplats;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte bounceCount;
    byte activeCount;
    byte numActive;
    byte field_5F;
    ushort minDelay;
    ushort field_62;
    int delay;
    int field_68;
    Entity *parent;
    Vector2 startPos;
    int startDir;
    Animator animator1;
    Animator animator2;
} EntitySplats;

// Object Struct
extern ObjectSplats *Splats;

// Standard Entity Events
void Splats_Update(void);
void Splats_LateUpdate(void);
void Splats_StaticUpdate(void);
void Splats_Draw(void);
void Splats_Create(void* data);
void Splats_StageLoad(void);
void Splats_EditorDraw(void);
void Splats_EditorLoad(void);
void Splats_Serialize(void);

// Extra Entity Functions
void Splats_DebugDraw(void);
void Splats_DebugSpawn(void);

void Splats_CheckPlayerCollisions(void);
void Splats_CheckOnScreen(void);

void Splats_Unknown2(void);
void Splats_Unknown3(void);
void Splats_Unknown4(void);
void Splats_Unknown5(void);
void Splats_Unknown6(void);
void Splats_Unknown7(void);
void Splats_Unknown8(void);
void Splats_Unknown9(void);
void Splats_Unknown10(void);

#endif //!OBJ_SPLATS_H
