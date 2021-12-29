#ifndef OBJ_IMAGETRAIL_H
#define OBJ_IMAGETRAIL_H

#include "SonicMania.h"

//Extras lol
#define ImageTrail_TrackCount (7)

// Object Class
struct ObjectImageTrail {
    RSDK_OBJECT
};

// Entity Class
struct EntityImageTrail {
    RSDK_ENTITY
    Entity *player;
    int32 playerObjID;
    Vector2 currentPos;
    Vector2 statePos[ImageTrail_TrackCount];
    int32 currentRotation;
    int32 stateRotation[ImageTrail_TrackCount];
    uint8 currentDirection;
    uint8 stateDirection[ImageTrail_TrackCount];
    uint8 currentVisible;
    uint8 stateVisible[ImageTrail_TrackCount];
    int32 currentScale;
    int32 stateScale[ImageTrail_TrackCount];
    Animator currentAnimData;
    Animator stateAnim[ImageTrail_TrackCount];
    int32 baseAlpha;
    int32 fadeoutTimer;
};

// Object Struct
extern ObjectImageTrail *ImageTrail;

// Standard Entity Events
void ImageTrail_Update(void);
void ImageTrail_LateUpdate(void);
void ImageTrail_StaticUpdate(void);
void ImageTrail_Draw(void);
void ImageTrail_Create(void* data);
void ImageTrail_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ImageTrail_EditorDraw(void);
void ImageTrail_EditorLoad(void);
#endif
void ImageTrail_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_IMAGETRAIL_H
