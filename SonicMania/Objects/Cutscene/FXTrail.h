#ifndef OBJ_FXTRAIL_H
#define OBJ_FXTRAIL_H

#include "Game.h"

// Extras lol
#define FXTRAIL_TRACK_COUNT 7

// Object Class
struct ObjectFXTrail {
    RSDK_OBJECT
};

// Entity Class
struct EntityFXTrail {
    RSDK_ENTITY
    Entity *parent;
    Vector2 currentPos;
    Vector2 statePos[FXTRAIL_TRACK_COUNT];
    int32 currentRotation;
    int32 stateRotation[FXTRAIL_TRACK_COUNT];
    uint8 currentDirection;
    uint8 stateDirection[FXTRAIL_TRACK_COUNT];
    uint8 currentVisible;
    uint8 stateVisible[FXTRAIL_TRACK_COUNT];
    Animator currentAnim;
    Animator stateAnim[FXTRAIL_TRACK_COUNT];
    Animator *animatorPtr;
    int32 baseAlpha;
};

// Object Struct
extern ObjectFXTrail *FXTrail;

// Standard Entity Events
void FXTrail_Update(void);
void FXTrail_LateUpdate(void);
void FXTrail_StaticUpdate(void);
void FXTrail_Draw(void);
void FXTrail_Create(void *data);
void FXTrail_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FXTrail_EditorDraw(void);
void FXTrail_EditorLoad(void);
#endif
void FXTrail_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_FXTRAIL_H
