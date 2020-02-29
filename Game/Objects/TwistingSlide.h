#ifndef OBJ_TWISTINGSLIDE_H
#define OBJ_TWISTINGSLIDE_H

//Object Class
class TwistingSlide : Object {

};

//Entity Class
class EntityTwistingSlide : Entity {

};

//Entity Functions
void TwistingSlide_Update();
void TwistingSlide_EarlyUpdate();
void TwistingSlide_LateUpdate();
void TwistingSlide_Draw();
void TwistingSlide_Setup(void* subtype);
void TwistingSlide_StageLoad();
void TwistingSlide_GetAttributes();

#endif //!OBJ_TWISTINGSLIDE_H
