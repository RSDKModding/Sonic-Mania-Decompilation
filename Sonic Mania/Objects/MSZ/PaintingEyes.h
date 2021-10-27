#ifndef OBJ_PAINTINGEYES_H
#define OBJ_PAINTINGEYES_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectPaintingEyes;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 timer;
    int32 lookPos;
    bool32 flag;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityPaintingEyes;

// Object Struct
extern ObjectPaintingEyes *PaintingEyes;

// Standard Entity Events
void PaintingEyes_Update(void);
void PaintingEyes_LateUpdate(void);
void PaintingEyes_StaticUpdate(void);
void PaintingEyes_Draw(void);
void PaintingEyes_Create(void* data);
void PaintingEyes_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PaintingEyes_EditorDraw(void);
void PaintingEyes_EditorLoad(void);
#endif
void PaintingEyes_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PAINTINGEYES_H
