#ifndef OBJ_ROTATINGSPIKES_H
#define OBJ_ROTATINGSPIKES_H

#include "../../SonicMania.h"

//Object Class
class ObjectRotatingSpikes : public Object {
public:

};

//Entity Class
class EntityRotatingSpikes : public Entity {
public:

};

//Object Entity
ObjectRotatingSpikes RotatingSpikes;

//Entity Functions
void RotatingSpikes_Update();
void RotatingSpikes_EarlyUpdate();
void RotatingSpikes_LateUpdate();
void RotatingSpikes_Draw();
void RotatingSpikes_Setup(void* subtype);
void RotatingSpikes_StageLoad();
void RotatingSpikes_GetAttributes();

#endif //!OBJ_ROTATINGSPIKES_H
