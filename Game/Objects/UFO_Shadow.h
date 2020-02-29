#ifndef OBJ_UFO_SHADOW_H
#define OBJ_UFO_SHADOW_H

//Object Class
class UFO_Shadow : Object {

};

//Entity Class
class EntityUFO_Shadow : Entity {

};

//Entity Functions
void UFO_Shadow_Update();
void UFO_Shadow_EarlyUpdate();
void UFO_Shadow_LateUpdate();
void UFO_Shadow_Draw();
void UFO_Shadow_Setup(void* subtype);
void UFO_Shadow_StageLoad();
void UFO_Shadow_GetAttributes();

#endif //!OBJ_UFO_SHADOW_H
