#ifndef OBJ_PAINTINGEYES_H
#define OBJ_PAINTINGEYES_H

#include "Game.h"

// Object Class
struct ObjectPaintingEyes {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityPaintingEyes {
    RSDK_ENTITY
    int32 timer;
    int32 coverPos;
    bool32 hideEyes;
    Animator coverAnimator;
    Animator irisAnimator;
    Animator pupilAnimator;
};

// Object Struct
extern ObjectPaintingEyes *PaintingEyes;

// Standard Entity Events
void PaintingEyes_Update(void);
void PaintingEyes_LateUpdate(void);
void PaintingEyes_StaticUpdate(void);
void PaintingEyes_Draw(void);
void PaintingEyes_Create(void *data);
void PaintingEyes_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PaintingEyes_EditorDraw(void);
void PaintingEyes_EditorLoad(void);
#endif
void PaintingEyes_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_PAINTINGEYES_H
