#ifndef OBJ_TUBESPRING_H
#define OBJ_TUBESPRING_H

//Object Class
class TubeSpring : Object {

};

//Entity Class
class EntityTubeSpring : Entity {

};

//Entity Functions
void TubeSpring_Update();
void TubeSpring_EarlyUpdate();
void TubeSpring_LateUpdate();
void TubeSpring_Draw();
void TubeSpring_Setup(void* subtype);
void TubeSpring_StageLoad();
void TubeSpring_GetAttributes();

#endif //!OBJ_TUBESPRING_H
