#ifndef OBJ_SOL_H
#define OBJ_SOL_H

#include "SonicMania.h"

#define Sol_MaxFlameOrbs (4)

// Object Class
struct ObjectSol {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxOrb;
    uint16 aniFrames;
};

// Entity Class
struct EntitySol {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    uint8 activeOrbs;
    Vector2 positions[Sol_MaxFlameOrbs];
    int32 field_88;
    bool32 isFlameFX;
    bool32 fireOrbs;
    Animator animator1;
    Animator animator2;
};

// Object Struct
extern ObjectSol *Sol;

// Standard Entity Events
void Sol_Update(void);
void Sol_LateUpdate(void);
void Sol_StaticUpdate(void);
void Sol_Draw(void);
void Sol_Create(void* data);
void Sol_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Sol_EditorDraw(void);
void Sol_EditorLoad(void);
#endif
void Sol_Serialize(void);

// Extra Entity Functions
void Sol_DebugSpawn(void);
void Sol_DebugDraw(void);

void Sol_HandlePlayerInteractions(void);
void Sol_HandlePlayerHurt(void);
void Sol_HandleRotation(void);
void Sol_CheckOnScreen(void);

void Sol_State_Setup(void);
void Sol_Unknown5(void);
void Sol_Unknown6(void);
void Sol_Unknown7(void);
void Sol_Unknown8(void);
void Sol_Unknown9(void);

void Sol_Unknown10(void);
void Sol_Unknown11(void);
void Sol_Unknown12(void);

#endif //!OBJ_SOL_H
