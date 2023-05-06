#ifndef OBJ_PBL_BUMPER_H
#define OBJ_PBL_BUMPER_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectPBL_Bumper {
    RSDK_OBJECT
    Animator unusedAnimator1;
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 modelFrames;
    uint16 sceneIndex;
    uint16 sfxBumper;
};

// Entity Class
struct EntityPBL_Bumper {
    RSDK_ENTITY
    StateMachine(state);
    int32 height;
    int32 scaleFactor;
    int32 scaleVel;
    int32 timer;
    Matrix unusedMatrix1;
    Matrix matTransform;
    Matrix matWorld;
    Matrix matNormal;
    Animator animator;
};

// Object Struct
extern ObjectPBL_Bumper *PBL_Bumper;

// Standard Entity Events
void PBL_Bumper_Update(void);
void PBL_Bumper_LateUpdate(void);
void PBL_Bumper_StaticUpdate(void);
void PBL_Bumper_Draw(void);
void PBL_Bumper_Create(void *data);
void PBL_Bumper_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PBL_Bumper_EditorDraw(void);
void PBL_Bumper_EditorLoad(void);
#endif
void PBL_Bumper_Serialize(void);

// Extra Entity Functions
void PBL_Bumper_HandlePlayerInteractions(void);
void PBL_Bumper_State_CheckBumps(void);
void PBL_Bumper_State_Bumped(void);
void PBL_Bumper_State_FinishedBump(void);

#endif

#endif //! OBJ_PBL_BUMPER_H
