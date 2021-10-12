#ifndef OBJ_RPLANESHIFTER_H
#define OBJ_RPLANESHIFTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Animator animator;
    ushort aniFrames;
    ushort sfxTicTock;
} ObjectRPlaneShifter;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint height;
    byte flags;
    int field_64;
    byte activePlayers1;
    byte prevActivePlayers1;
    byte activePlayers2;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    Animator animator;
    Hitbox hitbox;
} EntityRPlaneShifter;

// Object Struct
extern ObjectRPlaneShifter *RPlaneShifter;

// Standard Entity Events
void RPlaneShifter_Update(void);
void RPlaneShifter_LateUpdate(void);
void RPlaneShifter_StaticUpdate(void);
void RPlaneShifter_Draw(void);
void RPlaneShifter_Create(void* data);
void RPlaneShifter_StageLoad(void);
void RPlaneShifter_EditorDraw(void);
void RPlaneShifter_EditorLoad(void);
void RPlaneShifter_Serialize(void);

// Extra Entity Functions
void RPlaneShifter_DrawSprites(void);

void RPlaneShifter_HandlePlaneShift(EntityPlayer *player);

void RPlaneShifter_State_Setup(void);
void RPlaneShifter_Unknown3(void);
void RPlaneShifter_Unknown4(void);
void RPlaneShifter_Unknown5(void);

#endif //!OBJ_RPLANESHIFTER_H
