#ifndef OBJ_IMAGETRAIL_H
#define OBJ_IMAGETRAIL_H

#include "../SonicMania.hpp"

//Extras lol
#define ImageTrail_trackCount 7

// Object Class
struct ObjectImageTrail : Object {
	//straight nothin!
};

// Entity Class
struct EntityImageTrail : Entity {
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
    EntityAnimationData currentAnimData;
    EntityAnimationData stateAnim[ImageTrail_trackCount];
    int baseAlpha;
    int fadeoutTimer;
};

// Object Struct
extern ObjectImageTrail *ImageTrail;

// Standard Entity Events
void ImageTrail_Update();
void ImageTrail_LateUpdate();
void ImageTrail_StaticUpdate();
void ImageTrail_Draw();
void ImageTrail_Create(void* data);
void ImageTrail_StageLoad();
void ImageTrail_EditorDraw();
void ImageTrail_EditorLoad();
void ImageTrail_Serialize();

// Extra Entity Functions


#endif //!OBJ_IMAGETRAIL_H
