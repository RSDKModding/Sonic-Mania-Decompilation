#ifndef OBJ_PBL_TARGETBUMPER_H
#define OBJ_PBL_TARGETBUMPER_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectPBL_TargetBumper {
    RSDK_OBJECT
    Animator unusedAnimator1;
    Hitbox hitbox;
    int32 drawType;
    uint16 aniFrames; // prolly unused
    uint16 modelFrames;
    uint16 sceneIndex;
    uint16 sfxTargetBumper;
    uint16 sfxFlipper;
};

// Entity Class
struct EntityPBL_TargetBumper {
    RSDK_ENTITY
    StateMachine(state);
    int32 height;
    int32 color;
    Matrix unusedMatrix1;
    Matrix matTransform;
    Matrix matWorld;
    Matrix matNormal;
    Animator animator;
};

// Object Struct
extern ObjectPBL_TargetBumper *PBL_TargetBumper;

// Standard Entity Events
void PBL_TargetBumper_Update(void);
void PBL_TargetBumper_LateUpdate(void);
void PBL_TargetBumper_StaticUpdate(void);
void PBL_TargetBumper_Draw(void);
void PBL_TargetBumper_Create(void *data);
void PBL_TargetBumper_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PBL_TargetBumper_EditorDraw(void);
void PBL_TargetBumper_EditorLoad(void);
#endif
void PBL_TargetBumper_Serialize(void);

// Extra Entity Functions
void PBL_TargetBumper_HandlePlayerInteractions(void);
void PBL_TargetBumper_State_Active(void);
void PBL_TargetBumper_State_Reced(void);
void PBL_TargetBumper_State_Rise(void);

#endif

#endif //! OBJ_PBL_TARGETBUMPER_H
