#ifndef OBJ_FOLDINGPLATFORM_H
#define OBJ_FOLDINGPLATFORM_H

#include "Game.h"

// Object Class
struct ObjectFoldingPlatform {
    RSDK_OBJECT
};

// Entity Class
struct EntityFoldingPlatform {
    MANIA_PLATFORM_BASE

    uint16 interval;
    uint16 intervalOffset;
    int16 duration;
};

// Object Struct
extern ObjectFoldingPlatform *FoldingPlatform;

// Standard Entity Events
void FoldingPlatform_Update(void);
void FoldingPlatform_LateUpdate(void);
void FoldingPlatform_StaticUpdate(void);
void FoldingPlatform_Draw(void);
void FoldingPlatform_Create(void *data);
void FoldingPlatform_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FoldingPlatform_EditorDraw(void);
void FoldingPlatform_EditorLoad(void);
#endif
void FoldingPlatform_Serialize(void);

// Extra Entity Functions
void FoldingPlatform_State_Appear(void);
void FoldingPlatform_State_Disappear(void);

#endif //! OBJ_FOLDINGPLATFORM_H
