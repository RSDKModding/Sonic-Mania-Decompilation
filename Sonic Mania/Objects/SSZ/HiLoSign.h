#ifndef OBJ_HILOSIGN_H
#define OBJ_HILOSIGN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxes[2];
    uint16 sfxSignPost;
} ObjectHiLoSign;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(stateDraw);
    StateMachine(state);
    uint8 type;
    int32 spinSpeed;
    Animator animator1;
    Animator animator2;
} EntityHiLoSign;

// Object Struct
extern ObjectHiLoSign *HiLoSign;

// Standard Entity Events
void HiLoSign_Update(void);
void HiLoSign_LateUpdate(void);
void HiLoSign_StaticUpdate(void);
void HiLoSign_Draw(void);
void HiLoSign_Create(void* data);
void HiLoSign_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HiLoSign_EditorDraw(void);
void HiLoSign_EditorLoad(void);
#endif
void HiLoSign_Serialize(void);

// Extra Entity Functions
void HiLoSign_DebugSpawn(void);
void HiLoSign_DebugDraw(void);

void HiLoSign_State_Spinning(void);
void HiLoSign_State_Spinning2(void);

void HiLoSign_StateDraw_Horizontal(void);
void HiLoSign_StateDraw_Vertical(void);

#endif //!OBJ_HILOSIGN_H
