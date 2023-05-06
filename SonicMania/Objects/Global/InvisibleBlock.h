#ifndef OBJ_INVISIBLEBLOCK_H
#define OBJ_INVISIBLEBLOCK_H

#include "Game.h"

// Object Class
struct ObjectInvisibleBlock {
    RSDK_OBJECT
    uint16 aniFrames;
    Animator animator;
};

// Entity Class
struct EntityInvisibleBlock {
    RSDK_ENTITY
    uint8 width;
    uint8 height;
    PlaneFilterTypes planeFilter;
    bool32 noCrush;
    bool32 activeNormal;
    bool32 timeAttackOnly;
    bool32 noChibi;
    Hitbox hitbox;
};

// Object Struct
extern ObjectInvisibleBlock *InvisibleBlock;

// Standard Entity Events
void InvisibleBlock_Update(void);
void InvisibleBlock_LateUpdate(void);
void InvisibleBlock_StaticUpdate(void);
void InvisibleBlock_Draw(void);
void InvisibleBlock_Create(void *data);
void InvisibleBlock_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void InvisibleBlock_EditorDraw(void);
void InvisibleBlock_EditorLoad(void);
#endif
void InvisibleBlock_Serialize(void);

// Extra Entity Functions
void InvisibleBlock_DrawSprites(void);

#endif //! OBJ_INVISIBLEBLOCK_H
