#ifndef OBJ_FXTRAIL_H
#define OBJ_FXTRAIL_H

#include "../SonicMania.h"

// Extras lol
#define FXTrail_trackCount 7

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFXTrail;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Entity *parent;
    Vector2 currentPos;
    Vector2 statePos[FXTrail_trackCount];
    int currentRotation;
    int stateRotation[FXTrail_trackCount];
    byte currentDirection;
    byte stateDirection[FXTrail_trackCount];
    byte currentVisible;
    byte stateVisible[FXTrail_trackCount];
    AnimationData currentAnim;
    AnimationData stateAnim[FXTrail_trackCount];
    AnimationData *dataPtr;
    int baseAlpha;
} EntityFXTrail;

// Object Struct
extern ObjectFXTrail *FXTrail;

// Standard Entity Events
void FXTrail_Update();
void FXTrail_LateUpdate();
void FXTrail_StaticUpdate();
void FXTrail_Draw();
void FXTrail_Create(void* data);
void FXTrail_StageLoad();
void FXTrail_EditorDraw();
void FXTrail_EditorLoad();
void FXTrail_Serialize();

// Extra Entity Functions


#endif //!OBJ_FXTRAIL_H
