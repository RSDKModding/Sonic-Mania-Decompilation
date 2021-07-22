#ifndef OBJ_DECORATION_H
#define OBJ_DECORATION_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectDecoration;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animator;
    byte type;
    int rotSpeed;
    Vector2 repeatTimes;
    Vector2 repeatSpacing;
    bool32 tmzFlag;
} EntityDecoration;

// Object Struct
extern ObjectDecoration *Decoration;

// Standard Entity Events
void Decoration_Update(void);
void Decoration_LateUpdate(void);
void Decoration_StaticUpdate(void);
void Decoration_Draw(void);
void Decoration_Create(void* data);
void Decoration_StageLoad(void);
void Decoration_EditorDraw(void);
void Decoration_EditorLoad(void);
void Decoration_Serialize(void);

// Extra Entity Functions
void Decoration_DrawSprite(void);

#endif //!OBJ_DECORATION_H
