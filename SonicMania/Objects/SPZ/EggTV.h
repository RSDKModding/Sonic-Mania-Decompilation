#ifndef OBJ_EGGTV_H
#define OBJ_EGGTV_H

#include "Game.h"

// Object Class
struct ObjectEggTV {
    RSDK_OBJECT
    uint16 aniFrames;
    Animator animator;
};

// Entity Class
struct EntityEggTV {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 size;
    bool32 drawOverTV;
    int32 scanlineAlpha;
    Hitbox hitbox;
    Animator unusedAnimator;
};

// Object Struct
extern ObjectEggTV *EggTV;

// Standard Entity Events
void EggTV_Update(void);
void EggTV_LateUpdate(void);
void EggTV_StaticUpdate(void);
void EggTV_Draw(void);
void EggTV_Create(void *data);
void EggTV_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void EggTV_EditorDraw(void);
void EggTV_EditorLoad(void);
#endif
void EggTV_Serialize(void);

// Extra Entity Functions
void EggTV_DrawScanlines(void);
void EggTV_DrawTV(void);

#endif //! OBJ_EGGTV_H
