#ifndef OBJ_UFO_SPRINGBOARD_H
#define OBJ_UFO_SPRINGBOARD_H

#include "Game.h"

// Object Class
struct ObjectUFO_Springboard {
    RSDK_OBJECT
    Animator unusedAnimator1;
    uint16 aniFrames; // unused
    uint16 modelIndex;
    uint16 sceneIndex;
};

// Entity Class
struct EntityUFO_Springboard {
    RSDK_ENTITY
    int32 unused1;
    int32 height;
    uint8 rampCount;
    Matrix unusedMatrix1;
    Matrix matTemp;
    Matrix matWorld;
    Matrix matNormal;
    Animator animator;
};

// Object Struct
extern ObjectUFO_Springboard *UFO_Springboard;

// Standard Entity Events
void UFO_Springboard_Update(void);
void UFO_Springboard_LateUpdate(void);
void UFO_Springboard_StaticUpdate(void);
void UFO_Springboard_Draw(void);
void UFO_Springboard_Create(void *data);
void UFO_Springboard_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Springboard_EditorDraw(void);
void UFO_Springboard_EditorLoad(void);
#endif
void UFO_Springboard_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_UFO_SPRINGBOARD_H
