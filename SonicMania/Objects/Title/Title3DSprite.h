#ifndef OBJ_TITLE3DSPRITE_H
#define OBJ_TITLE3DSPRITE_H

#include "Game.h"

typedef enum {
    TITLE3DSPRITE_MOUNTAIN_L,
    TITLE3DSPRITE_MOUNTAIN_M,
    TITLE3DSPRITE_MOUNTAIN_S,
    TITLE3DSPRITE_TREE,
    TITLE3DSPRITE_BUSH,
} Title3DSpriteFrames;

// Object Class
struct ObjectTitle3DSprite {
    RSDK_OBJECT
    int32 islandSize;
    int32 height;
    int32 baseDepth;
    uint16 aniFrames;
};

// Entity Class
struct EntityTitle3DSprite {
    RSDK_ENTITY
    int32 frame;
    Vector2 relativePos;
    Animator animator;
};

// Object Struct
extern ObjectTitle3DSprite *Title3DSprite;

// Standard Entity Events
void Title3DSprite_Update(void);
void Title3DSprite_LateUpdate(void);
void Title3DSprite_StaticUpdate(void);
void Title3DSprite_Draw(void);
void Title3DSprite_Create(void *data);
void Title3DSprite_StageLoad(void);
void Title3DSprite_EditorDraw(void);
void Title3DSprite_EditorLoad(void);
void Title3DSprite_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_TITLE3DSPRITE_H
