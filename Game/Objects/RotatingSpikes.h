#ifndef OBJ_ROTATINGSPIKES_H
#define OBJ_ROTATINGSPIKES_H

//Object Class
class RotatingSpikes : Object {

};

//Entity Class
class EntityRotatingSpikes : Entity {

};

//Entity Functions
void RotatingSpikes_Update();
void RotatingSpikes_EarlyUpdate();
void RotatingSpikes_LateUpdate();
void RotatingSpikes_Draw();
void RotatingSpikes_Setup(void* subtype);
void RotatingSpikes_StageLoad();
void RotatingSpikes_GetAttributes();

#endif //!OBJ_ROTATINGSPIKES_H
