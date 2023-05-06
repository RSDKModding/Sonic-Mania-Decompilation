#ifndef OBJ_PBL_FLIPPER_H
#define OBJ_PBL_FLIPPER_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectPBL_Flipper {
    RSDK_OBJECT
    Animator unusedAnimator1;
    Hitbox hitbox;
    Hitbox hitboxUnused;
    int32 drawType;
    uint16 aniFrames;
    uint16 modelFrames;
    uint16 sceneIndex;
    uint16 sfxFlipper;
    uint16 sfxFlipperStage;
};

// Entity Class
struct EntityPBL_Flipper {
    RSDK_ENTITY
    StateMachine(state);
    uint8 minCraneID;
    int32 height;
    bool32 retractable;
    Matrix unusedMatrix1;
    Matrix matTransform;
    Matrix matWorld;
    Matrix matNormal;
    bool32 buttonPress;
    bool32 buttonDown;
    bool32 flipperActive;
    Animator animator;
};

// Object Struct
extern ObjectPBL_Flipper *PBL_Flipper;

// Standard Entity Events
void PBL_Flipper_Update(void);
void PBL_Flipper_LateUpdate(void);
void PBL_Flipper_StaticUpdate(void);
void PBL_Flipper_Draw(void);
void PBL_Flipper_Create(void *data);
void PBL_Flipper_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PBL_Flipper_EditorDraw(void);
void PBL_Flipper_EditorLoad(void);
#endif
void PBL_Flipper_Serialize(void);

// Extra Entity Functions
void PBL_Flipper_HandlePlayerInteractions(void);
void PBL_Flipper_State_AwaitFlip(void);
void PBL_Flipper_State_RiseFlipper(void);
void PBL_Flipper_State_LowerFlipper(void);

#endif

#endif //! OBJ_PBL_FLIPPER_H
