#ifndef OBJ_JAWZ_H
#define OBJ_JAWZ_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
} ObjectJawz;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte triggerDir;
    StateMachine(state);
    Vector2 startPos;
    Animator animator;
} EntityJawz;

// Object Struct
extern ObjectJawz *Jawz;

// Standard Entity Events
void Jawz_Update(void);
void Jawz_LateUpdate(void);
void Jawz_StaticUpdate(void);
void Jawz_Draw(void);
void Jawz_Create(void* data);
void Jawz_StageLoad(void);
void Jawz_EditorDraw(void);
void Jawz_EditorLoad(void);
void Jawz_Serialize(void);

// Extra Entity Functions
void Jawz_DebugSpawn(void);
void Jawz_DebugDraw(void);
void Jawz_CheckPlayerInteractions(void);
void Jawz_CheckPlayerTrigger(void);
void Jawz_State_Main(void);

#endif //!OBJ_JAWZ_H
