#ifndef OBJ_IMAGETRAIL_H
#define OBJ_IMAGETRAIL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectImageTrail : Object {

};

// Entity Class
struct EntityImageTrail : Entity {

};

// Object Entity
extern ObjectImageTrail ImageTrail;

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
