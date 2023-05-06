#ifndef OBJ_TITLESONIC_H
#define OBJ_TITLESONIC_H

#include "Game.h"

// Object Class
struct ObjectTitleSonic {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityTitleSonic {
    RSDK_ENTITY
    Animator animatorSonic;
    Animator animatorFinger;
};

// Object Struct
extern ObjectTitleSonic *TitleSonic;

// Standard Entity Events
void TitleSonic_Update(void);
void TitleSonic_LateUpdate(void);
void TitleSonic_StaticUpdate(void);
void TitleSonic_Draw(void);
void TitleSonic_Create(void *data);
void TitleSonic_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TitleSonic_EditorDraw(void);
void TitleSonic_EditorLoad(void);
#endif
void TitleSonic_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_TITLESONIC_H
