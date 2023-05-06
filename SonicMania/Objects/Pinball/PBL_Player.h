#ifndef OBJ_PBL_PLAYER_H
#define OBJ_PBL_PLAYER_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectPBL_Player {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 unused1;
    Hitbox innerBox;
    Hitbox outerBox;
    uint16 sfxPlunger;
    uint16 jumpFrames;
    uint16 ballFrames;
    uint16 sceneIndex;
};

// Entity Class
struct EntityPBL_Player {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 angleX;
    int32 height;
    int32 unused1;
    int32 unused2;
    Matrix matRotate;
    Matrix matTranslate;
    Matrix matWorld;
    Matrix matNormal;
    StateMachine(stateInput);
    int32 controllerID;
    bool32 up;
    bool32 down;
    bool32 left;
    bool32 right;
    bool32 jumpPress;
    bool32 jumpHold; // completely unused, but probably pretty likely
    Animator animator;
};

// Object Struct
extern ObjectPBL_Player *PBL_Player;

// Standard Entity Events
void PBL_Player_Update(void);
void PBL_Player_LateUpdate(void);
void PBL_Player_StaticUpdate(void);
void PBL_Player_Draw(void);
void PBL_Player_Create(void *data);
void PBL_Player_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PBL_Player_EditorDraw(void);
void PBL_Player_EditorLoad(void);
#endif
void PBL_Player_Serialize(void);

// Extra Entity Functions
void PBL_Player_Input_P1(void);

// States
void PBL_Player_State_Launcher(void);
void PBL_Player_State_Ground(void);
void PBL_Player_State_Air(void);

#endif

#endif //! OBJ_PBL_PLAYER_H
