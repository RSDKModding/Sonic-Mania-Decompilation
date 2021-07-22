#ifndef OBJ_INVISIBLEBLOCK_H
#define OBJ_INVISIBLEBLOCK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    Animator animator;
} ObjectInvisibleBlock;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte width;
    byte height;
    int planeFilter;
    int noCrush;
    bool32 activeNormal;
    bool32 timeAttackOnly;
    bool32 noChibi;
    Hitbox hitbox;
} EntityInvisibleBlock;

// Object Struct
extern ObjectInvisibleBlock *InvisibleBlock;

// Standard Entity Events
void InvisibleBlock_Update(void);
void InvisibleBlock_LateUpdate(void);
void InvisibleBlock_StaticUpdate(void);
void InvisibleBlock_Draw(void);
void InvisibleBlock_Create(void* data);
void InvisibleBlock_StageLoad(void);
void InvisibleBlock_EditorDraw(void);
void InvisibleBlock_EditorLoad(void);
void InvisibleBlock_Serialize(void);

// Extra Entity Functions
void InvisibleBlock_DrawSprites(void);

#endif //!OBJ_INVISIBLEBLOCK_H
