#ifndef OBJ_FBZSINKTRASH_H
#define OBJ_FBZSINKTRASH_H

#include "Game.h"

typedef enum {
    FBZSINKTRASH_SINK,
    FBZSINKTRASH_SOLID,
    FBZSINKTRASH_DECOR,
} FBZSinkTrashTypes;

// Object Class
struct ObjectFBZSinkTrash {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityFBZSinkTrash {
    RSDK_ENTITY
    int32 type;
    Vector2 size;
    Vector2 trashPos[64];
    uint8 trashFrame[64];
    uint8 trashDir[64];
    Hitbox hitboxTrash;
    Hitbox hitboxDebris;
    Animator trashAnimator;
    Animator topAnimator;
    Animator mainAnimator;
};

// Object Struct
extern ObjectFBZSinkTrash *FBZSinkTrash;

// Standard Entity Events
void FBZSinkTrash_Update(void);
void FBZSinkTrash_LateUpdate(void);
void FBZSinkTrash_StaticUpdate(void);
void FBZSinkTrash_Draw(void);
void FBZSinkTrash_Create(void *data);
void FBZSinkTrash_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FBZSinkTrash_EditorDraw(void);
void FBZSinkTrash_EditorLoad(void);
#endif
void FBZSinkTrash_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_FBZSINKTRASH_H
