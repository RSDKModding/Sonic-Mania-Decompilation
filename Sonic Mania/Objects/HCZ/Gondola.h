#ifndef OBJ_GONDOLA_H
#define OBJ_GONDOLA_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    int32 taggedBoatIDs[3];
    bool32 hasAchievement;
} ObjectGondola;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 minY;
    int32 maxY;
    uint8 boatID;
    uint8 activePlayers;
    Vector2 field_68;
    int32 field_70;
    int32 field_74;
    bool32 flag;
    bool32 flag2;
    int32 field_80;
    Vector2 field_84;
    Vector2 field_8C;
    Vector2 field_94;
    int32 field_9C;
    int32 field_A0;
    int32 field_A4;
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
int32 Gondola_GetWaterLevel(void);
void Gondola_Unknown2(void);
void Gondola_Unknown3(void);
void Gondola_Unknown4(void);
void Gondola_Unknown5(void);
void Gondola_Unknown6(void);

#endif //!OBJ_GONDOLA_H
