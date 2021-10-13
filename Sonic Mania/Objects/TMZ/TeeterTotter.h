#ifndef OBJ_TEETERTOTTER_H
#define OBJ_TEETERTOTTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectTeeterTotter;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 color;
    uint32 length;
    Vector2 origin;
    int32 field_6C;
    int32 playerIDs[4];
    int32 inactiveSegments;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    int32 field_90[32];
    int32 field_110[32];
    Hitbox hitbox;
    Animator animator;
} EntityTeeterTotter;

// Object Struct
extern ObjectTeeterTotter *TeeterTotter;

// Standard Entity Events
void TeeterTotter_Update(void);
void TeeterTotter_LateUpdate(void);
void TeeterTotter_StaticUpdate(void);
void TeeterTotter_Draw(void);
void TeeterTotter_Create(void* data);
void TeeterTotter_StageLoad(void);
void TeeterTotter_EditorDraw(void);
void TeeterTotter_EditorLoad(void);
void TeeterTotter_Serialize(void);

// Extra Entity Functions
int32 TeeterTotter_CheckPlayerCollisions(void);
void TeeterTotter_ProcessSegmentGravity(void);
void TeeterTotter_Unknown3(void);

void TeeterTotter_State_Setup(void);
void TeeterTotter_State_Unknown1(void);
void TeeterTotter_State_Unknown2(void);

#endif //!OBJ_TEETERTOTTER_H
