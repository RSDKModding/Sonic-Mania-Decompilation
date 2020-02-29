#ifndef OBJ_TWISTINGSLIDE_H
#define OBJ_TWISTINGSLIDE_H

#include "../../SonicMania.h"

//Object Class
class ObjectTwistingSlide : public Object {
public:

};

//Entity Class
class EntityTwistingSlide : public Entity {
public:

};

//Object Entity
ObjectTwistingSlide TwistingSlide;

//Entity Functions
void TwistingSlide_Update();
void TwistingSlide_EarlyUpdate();
void TwistingSlide_LateUpdate();
void TwistingSlide_Draw();
void TwistingSlide_Setup(void* subtype);
void TwistingSlide_StageLoad();
void TwistingSlide_GetAttributes();

#endif //!OBJ_TWISTINGSLIDE_H
