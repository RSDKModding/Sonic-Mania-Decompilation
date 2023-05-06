#ifndef OBJ_UFO_SHADOW_H
#define OBJ_UFO_SHADOW_H

#include "Game.h"

// Object Class
struct ObjectUFO_Shadow {
    RSDK_OBJECT
    uint16 sceneID;
    uint16 modelIndex;
};

// Entity Class
struct EntityUFO_Shadow {
    RSDK_ENTITY
    int32 shadowScale;
    Entity *parent;
    Matrix matrix;
    Animator animator;
};

// Object Struct
extern ObjectUFO_Shadow *UFO_Shadow;

// Standard Entity Events
void UFO_Shadow_Update(void);
void UFO_Shadow_LateUpdate(void);
void UFO_Shadow_StaticUpdate(void);
void UFO_Shadow_Draw(void);
void UFO_Shadow_Create(void *data);
void UFO_Shadow_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Shadow_EditorDraw(void);
void UFO_Shadow_EditorLoad(void);
#endif
void UFO_Shadow_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_UFO_SHADOW_H
