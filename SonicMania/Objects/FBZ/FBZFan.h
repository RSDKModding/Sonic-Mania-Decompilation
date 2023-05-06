#ifndef OBJ_FBZFAN_H
#define OBJ_FBZFAN_H

#include "Game.h"

// Object Class
struct ObjectFBZFan {
    RSDK_OBJECT
    int32 activePlayers;
    Hitbox hitboxFan;
    Hitbox hitboxSolid;
    Hitbox hitboxPlayer;
    uint16 aniFrames;
    Animator baseAnimator;
    Animator fanAnimator;
    Animator fan2Animator;
    uint16 sfxFan;
};

// Entity Class
struct EntityFBZFan {
    RSDK_ENTITY
    Hitbox hitboxTrigger;
    uint8 activePlayers;
};

// Object Struct
extern ObjectFBZFan *FBZFan;

// Standard Entity Events
void FBZFan_Update(void);
void FBZFan_LateUpdate(void);
void FBZFan_StaticUpdate(void);
void FBZFan_Draw(void);
void FBZFan_Create(void *data);
void FBZFan_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FBZFan_EditorDraw(void);
void FBZFan_EditorLoad(void);
#endif
void FBZFan_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_FBZFAN_H
