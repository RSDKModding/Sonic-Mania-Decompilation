#ifndef OBJ_TWISTINGSLIDE_H
#define OBJ_TWISTINGSLIDE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTwistingSlide : Object {

};

// Entity Class
struct EntityTwistingSlide : Entity {

};

// Object Entity
extern ObjectTwistingSlide TwistingSlide;

// Standard Entity Events
void TwistingSlide_Update();
void TwistingSlide_LateUpdate();
void TwistingSlide_StaticUpdate();
void TwistingSlide_Draw();
void TwistingSlide_Create(void* data);
void TwistingSlide_StageLoad();
void TwistingSlide_EditorDraw();
void TwistingSlide_EditorLoad();
void TwistingSlide_Serialize();

// Extra Entity Functions


#endif //!OBJ_TWISTINGSLIDE_H
