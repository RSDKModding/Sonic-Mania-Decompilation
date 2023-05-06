#ifndef OBJ_INKWIPE_H
#define OBJ_INKWIPE_H

#include "Game.h"

// Object Class
struct ObjectInkWipe {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityInkWipe {
    RSDK_ENTITY
    Animator animator;
};

// Object Struct
extern ObjectInkWipe *InkWipe;

// Standard Entity Events
void InkWipe_Update(void);
void InkWipe_LateUpdate(void);
void InkWipe_StaticUpdate(void);
void InkWipe_Draw(void);
void InkWipe_Create(void *data);
void InkWipe_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void InkWipe_EditorDraw(void);
void InkWipe_EditorLoad(void);
#endif
void InkWipe_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_INKWIPE_H
