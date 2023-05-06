#ifndef OBJ_PUYOLEVELSELECT_H
#define OBJ_PUYOLEVELSELECT_H

#include "Game.h"

// Object Class
struct ObjectPuyoLevelSelect {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxMenuBleep;
    uint16 sfxMenuAccept;
};

// Entity Class
struct EntityPuyoLevelSelect {
    RSDK_ENTITY
    StateMachine(state);
    int32 playerID;
    bool32 canSelectLevels;
    bool32 ready;
    int32 optionID;
    Animator frameAnimator;
    Animator labelAnimator;
    bool32 up;
    bool32 down;
    bool32 confirmPress;
    bool32 backPress;
};

// Object Struct
extern ObjectPuyoLevelSelect *PuyoLevelSelect;

// Standard Entity Events
void PuyoLevelSelect_Update(void);
void PuyoLevelSelect_LateUpdate(void);
void PuyoLevelSelect_StaticUpdate(void);
void PuyoLevelSelect_Draw(void);
void PuyoLevelSelect_Create(void *data);
void PuyoLevelSelect_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PuyoLevelSelect_EditorDraw(void);
void PuyoLevelSelect_EditorLoad(void);
#endif
void PuyoLevelSelect_Serialize(void);

// Extra Entity Functions
void PuyoLevelSelect_DrawSprites(void);
void PuyoLevelSelect_HandleMenuMovement(void);

#endif //! OBJ_PUYOLEVELSELECT_H
