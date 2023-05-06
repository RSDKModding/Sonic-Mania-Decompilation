#ifndef OBJ_BSS_HUD_H
#define OBJ_BSS_HUD_H

#include "Game.h"

// Object Class
struct ObjectBSS_HUD {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityBSS_HUD {
    RSDK_ENTITY
    Animator sphereAnimator;
    Animator ringAnimator;
    Animator numbersAnimator;
};

// Object Struct
extern ObjectBSS_HUD *BSS_HUD;

// Standard Entity Events
void BSS_HUD_Update(void);
void BSS_HUD_LateUpdate(void);
void BSS_HUD_StaticUpdate(void);
void BSS_HUD_Draw(void);
void BSS_HUD_Create(void *data);
void BSS_HUD_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BSS_HUD_EditorDraw(void);
void BSS_HUD_EditorLoad(void);
#endif
void BSS_HUD_Serialize(void);

// Extra Entity Functions
void BSS_HUD_DrawNumbers(int32 value, Vector2 *drawPos);

#endif //! OBJ_BSS_HUD_H
