#ifndef OBJ_INKWIPE_H
#define OBJ_INKWIPE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectInkWipe;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animator;
} EntityInkWipe;

// Object Struct
extern ObjectInkWipe *InkWipe;

// Standard Entity Events
void InkWipe_Update(void);
void InkWipe_LateUpdate(void);
void InkWipe_StaticUpdate(void);
void InkWipe_Draw(void);
void InkWipe_Create(void* data);
void InkWipe_StageLoad(void);
void InkWipe_EditorDraw(void);
void InkWipe_EditorLoad(void);
void InkWipe_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_INKWIPE_H
