#ifndef OBJ_RPLANESHIFTER_H
#define OBJ_RPLANESHIFTER_H

#include "SonicMania.h"

// Object Class
struct ObjectRPlaneShifter {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
    uint16 sfxTicTock;
};

// Entity Class
struct EntityRPlaneShifter {
    RSDK_ENTITY
    StateMachine(state);
    uint32 height;
    uint8 flags;
    int32 field_64;
    uint8 activePlayers1;
    uint8 prevActivePlayers1;
    uint8 activePlayers2;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    Animator animator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectRPlaneShifter *RPlaneShifter;

// Standard Entity Events
void RPlaneShifter_Update(void);
void RPlaneShifter_LateUpdate(void);
void RPlaneShifter_StaticUpdate(void);
void RPlaneShifter_Draw(void);
void RPlaneShifter_Create(void* data);
void RPlaneShifter_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void RPlaneShifter_EditorDraw(void);
void RPlaneShifter_EditorLoad(void);
#endif
void RPlaneShifter_Serialize(void);

// Extra Entity Functions
void RPlaneShifter_DrawSprites(void);

void RPlaneShifter_HandlePlaneShift(EntityPlayer *player);

void RPlaneShifter_State_Setup(void);
void RPlaneShifter_Unknown3(void);
void RPlaneShifter_Unknown4(void);
void RPlaneShifter_Unknown5(void);

#endif //!OBJ_RPLANESHIFTER_H
