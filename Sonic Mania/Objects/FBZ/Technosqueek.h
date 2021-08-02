#ifndef OBJ_TECHNOSQUEEK_H
#define OBJ_TECHNOSQUEEK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
} ObjectTechnosqueek;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte type;
    byte speed;
    ushort distance;
    byte field_60;
    int field_64;
    int field_68;
    int field_6C;
    bool32 flag;
    Vector2 startPos;
    byte startDir;
    Animator animator1;
    Animator animator2;
} EntityTechnosqueek;

// Object Struct
extern ObjectTechnosqueek *Technosqueek;

// Standard Entity Events
void Technosqueek_Update(void);
void Technosqueek_LateUpdate(void);
void Technosqueek_StaticUpdate(void);
void Technosqueek_Draw(void);
void Technosqueek_Create(void* data);
void Technosqueek_StageLoad(void);
void Technosqueek_EditorDraw(void);
void Technosqueek_EditorLoad(void);
void Technosqueek_Serialize(void);

// Extra Entity Functions
void Technosqueek_DebugSpawn(void);
void Technosqueek_DebugDraw(void);

void Technosqueek_HandlePlayerInteractions(void);
void Technosqueek_CheckOnScreen(void);

void Technosqueek_State_Unknown1(void);
void Technosqueek_State_Unknown2(void);
void Technosqueek_State_Unknown3(void);
void Technosqueek_State_Unknown4(void);
void Technosqueek_State_Unknown5(void);
void Technosqueek_State_Unknown6(void);

#endif //!OBJ_TECHNOSQUEEK_H
