#ifndef OBJ_BSS_HORIZON_H
#define OBJ_BSS_HORIZON_H

#include "Game.h"

// Object Class
struct ObjectBSS_Horizon {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityBSS_Horizon {
    RSDK_ENTITY
    Animator horizonAnimator;
};

// Object Struct
extern ObjectBSS_Horizon *BSS_Horizon;

// Standard Entity Events
void BSS_Horizon_Update(void);
void BSS_Horizon_LateUpdate(void);
void BSS_Horizon_StaticUpdate(void);
void BSS_Horizon_Draw(void);
void BSS_Horizon_Create(void *data);
void BSS_Horizon_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BSS_Horizon_EditorDraw(void);
void BSS_Horizon_EditorLoad(void);
#endif
void BSS_Horizon_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_BSS_HORIZON_H
