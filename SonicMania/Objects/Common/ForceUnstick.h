#ifndef OBJ_FORCEUNSTICK_H
#define OBJ_FORCEUNSTICK_H

#include "Game.h"

// Object Class
struct ObjectForceUnstick {
    RSDK_OBJECT
    uint16 aniFrames;
    Animator animator;
};

// Entity Class
struct EntityForceUnstick {
    RSDK_ENTITY
    uint8 width;
    uint8 height;
    bool32 breakClimb;
    Hitbox hitbox;
};

// Object Struct
extern ObjectForceUnstick *ForceUnstick;

// Standard Entity Events
void ForceUnstick_Update(void);
void ForceUnstick_LateUpdate(void);
void ForceUnstick_StaticUpdate(void);
void ForceUnstick_Draw(void);
void ForceUnstick_Create(void *data);
void ForceUnstick_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ForceUnstick_EditorDraw(void);
void ForceUnstick_EditorLoad(void);
#endif
void ForceUnstick_Serialize(void);

// Extra Entity Functions
void ForceUnstick_DrawSprites(void);

#endif //! OBJ_FORCEUNSTICK_H
