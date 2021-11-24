#ifndef OBJ_FBZSINKTRASH_H
#define OBJ_FBZSINKTRASH_H

#include "SonicMania.h"

typedef enum {
    FBZSINKTRASH_LOOSE,
    FBZSINKTRASH_SOLID,
    FBZSINKTRASH_DECOR,
}FBZSinkTrashTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectFBZSinkTrash;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 type;
    Vector2 size;
    Vector2 positions[64];
    uint8 frameIDs[64];
    uint8 directions[64];
    Hitbox hitboxTrash;
    Hitbox hitboxDebris;
    Animator trashAnimator;
    Animator topAnimator;
    Animator mainAnimator;
} EntityFBZSinkTrash;

// Object Struct
extern ObjectFBZSinkTrash *FBZSinkTrash;

// Standard Entity Events
void FBZSinkTrash_Update(void);
void FBZSinkTrash_LateUpdate(void);
void FBZSinkTrash_StaticUpdate(void);
void FBZSinkTrash_Draw(void);
void FBZSinkTrash_Create(void* data);
void FBZSinkTrash_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void FBZSinkTrash_EditorDraw(void);
void FBZSinkTrash_EditorLoad(void);
#endif
void FBZSinkTrash_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FBZSINKTRASH_H
