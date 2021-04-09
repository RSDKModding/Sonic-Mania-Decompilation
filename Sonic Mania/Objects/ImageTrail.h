#ifndef OBJ_IMAGETRAIL_H
#define OBJ_IMAGETRAIL_H

#include "../SonicMania.h"

//Extras lol
#define ImageTrail_trackCount 7

// Object Class
typedef struct {
    RSDK_OBJECT
} ObjectImageTrail;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Entity *player;
    int playerObjID;
    Vector2 currentPos;
    Vector2 statePos[ImageTrail_trackCount];
    int currentRotation;
    int stateRotation[ImageTrail_trackCount];
    byte currentDirection;
    byte stateDirection[ImageTrail_trackCount];
    byte currentVisible;
    byte stateVisible[ImageTrail_trackCount];
    int currentScale;
    int stateScale[ImageTrail_trackCount];
    Animator currentAnimData;
    Animator stateAnim[ImageTrail_trackCount];
    int baseAlpha;
    int fadeoutTimer;
} EntityImageTrail;

// Object Struct
extern ObjectImageTrail *ImageTrail;

// Standard Entity Events
void ImageTrail_Update(void);
void ImageTrail_LateUpdate(void);
void ImageTrail_StaticUpdate(void);
void ImageTrail_Draw(void);
void ImageTrail_Create(void* data);
void ImageTrail_StageLoad(void);
void ImageTrail_EditorDraw(void);
void ImageTrail_EditorLoad(void);
void ImageTrail_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_IMAGETRAIL_H
