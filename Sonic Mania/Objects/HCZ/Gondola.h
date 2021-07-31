#ifndef OBJ_GONDOLA_H
#define OBJ_GONDOLA_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    int taggedBoatIDs[3];
    bool32 hasAchievement;
} ObjectGondola;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int minY;
    int maxY;
    byte boatID;
    byte activePlayers;
    Vector2 field_68;
    int field_70;
    int field_74;
    bool32 flag;
    bool32 flag2;
    int field_80;
    Vector2 field_84;
    Vector2 field_8C;
    Vector2 field_94;
    int field_9C;
    int field_A0;
    int field_A4;
    Hitbox hitbox;
    Animator animator;
} EntityGondola;

// Object Struct
extern ObjectGondola *Gondola;

// Standard Entity Events
void Gondola_Update(void);
void Gondola_LateUpdate(void);
void Gondola_StaticUpdate(void);
void Gondola_Draw(void);
void Gondola_Create(void* data);
void Gondola_StageLoad(void);
void Gondola_EditorDraw(void);
void Gondola_EditorLoad(void);
void Gondola_Serialize(void);

// Extra Entity Functions
int Gondola_GetWaterLevel(void);
void Gondola_Unknown2(void);
void Gondola_Unknown3(void);
void Gondola_Unknown4(void);
void Gondola_Unknown5(void);
void Gondola_Unknown6(void);

#endif //!OBJ_GONDOLA_H
