#ifndef OBJ_PLANESEESAW_H
#define OBJ_PLANESEESAW_H

#include "Game.h"

// Object Class
struct ObjectPlaneSeeSaw {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxSpring;
    uint16 sfxFlipper;
};

// Entity Class
struct EntityPlaneSeeSaw {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused;
    int32 seeSawPos;
    Animator platformAnimator;
    Animator swingAnimator;
    Animator weightAnimator;
};

// Object Struct
extern ObjectPlaneSeeSaw *PlaneSeeSaw;

// Standard Entity Events
void PlaneSeeSaw_Update(void);
void PlaneSeeSaw_LateUpdate(void);
void PlaneSeeSaw_StaticUpdate(void);
void PlaneSeeSaw_Draw(void);
void PlaneSeeSaw_Create(void *data);
void PlaneSeeSaw_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PlaneSeeSaw_EditorDraw(void);
void PlaneSeeSaw_EditorLoad(void);
#endif
void PlaneSeeSaw_Serialize(void);

// Extra Entity Functions
void PlaneSeeSaw_State_WaitForPlayer(void);
void PlaneSeeSaw_State_PlayerPushDown(void);
void PlaneSeeSaw_State_Launch(void);

void PlaneSeeSaw_PlayerState_ToBG(void);
void PlaneSeeSaw_PlayerState_ToFG(void);

#endif //! OBJ_PLANESEESAW_H
