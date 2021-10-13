#ifndef OBJ_UFO_SHADOW_H
#define OBJ_UFO_SHADOW_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 sceneID;
    uint16 modelIndex;
} ObjectUFO_Shadow;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int shadowScale;
    Entity *parent;
    Matrix matrix;
    Animator animator;
} EntityUFO_Shadow;

// Object Struct
extern ObjectUFO_Shadow *UFO_Shadow;

// Standard Entity Events
void UFO_Shadow_Update(void);
void UFO_Shadow_LateUpdate(void);
void UFO_Shadow_StaticUpdate(void);
void UFO_Shadow_Draw(void);
void UFO_Shadow_Create(void* data);
void UFO_Shadow_StageLoad(void);
void UFO_Shadow_EditorDraw(void);
void UFO_Shadow_EditorLoad(void);
void UFO_Shadow_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UFO_SHADOW_H
