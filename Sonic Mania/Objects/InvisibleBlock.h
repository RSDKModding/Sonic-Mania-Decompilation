#ifndef OBJ_INVISIBLEBLOCK_H
#define OBJ_INVISIBLEBLOCK_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    EntityAnimationData data;
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
void InvisibleBlock_Update();
void InvisibleBlock_LateUpdate();
void InvisibleBlock_StaticUpdate();
void InvisibleBlock_Draw();
void InvisibleBlock_Create(void* data);
void InvisibleBlock_StageLoad();
void InvisibleBlock_EditorDraw();
void InvisibleBlock_EditorLoad();
void InvisibleBlock_Serialize();

// Extra Entity Functions
void InvisibleBlock_DrawSprites();

#endif //!OBJ_INVISIBLEBLOCK_H
