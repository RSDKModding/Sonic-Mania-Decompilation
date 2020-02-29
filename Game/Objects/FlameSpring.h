#ifndef OBJ_FLAMESPRING_H
#define OBJ_FLAMESPRING_H

//Object Class
class FlameSpring : Object {

};

//Entity Class
class EntityFlameSpring : Entity {

};

//Entity Functions
void FlameSpring_Update();
void FlameSpring_EarlyUpdate();
void FlameSpring_LateUpdate();
void FlameSpring_Draw();
void FlameSpring_Setup(void* subtype);
void FlameSpring_StageLoad();
void FlameSpring_GetAttributes();

#endif //!OBJ_FLAMESPRING_H
