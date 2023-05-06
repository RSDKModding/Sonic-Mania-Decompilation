#ifndef OBJ_UFO_DECORATION_H
#define OBJ_UFO_DECORATION_H

#include "Game.h"

typedef enum {
    UFO_DECOR_TREE,
    UFO_DECOR_FLOWER1,
    UFO_DECOR_FLOWER2,
    UFO_DECOR_FLOWER3,
    UFO_DECOR_PILLAR1,
    UFO_DECOR_PILLAR2,
    UFO_DECOR_BIRD,
    UFO_DECOR_FISH,
} UFO_DecorationTypes;

// Object Class
struct ObjectUFO_Decoration {
    RSDK_OBJECT
    Animator unusedAnimator1;
    int32 drawType;
    uint16 aniFrames; // unused
    uint16 modelIndices[8];
    uint16 sceneIndex;
};

// Entity Class
struct EntityUFO_Decoration {
    RSDK_ENTITY
    int32 unused1;
    int32 type;
    int32 size;
    int32 height;
    Matrix unusedMatrix1;
    Matrix matTransform;
    Matrix matWorld;
    Matrix matNormal;
    Animator animator;
};

// Object Struct
extern ObjectUFO_Decoration *UFO_Decoration;

// Standard Entity Events
void UFO_Decoration_Update(void);
void UFO_Decoration_LateUpdate(void);
void UFO_Decoration_StaticUpdate(void);
void UFO_Decoration_Draw(void);
void UFO_Decoration_Create(void *data);
void UFO_Decoration_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Decoration_EditorDraw(void);
void UFO_Decoration_EditorLoad(void);
#endif
void UFO_Decoration_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_UFO_DECORATION_H
