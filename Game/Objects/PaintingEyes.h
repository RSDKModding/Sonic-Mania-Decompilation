#ifndef OBJ_PAINTINGEYES_H
#define OBJ_PAINTINGEYES_H

#include "../../SonicMania.h"

//Object Class
class ObjectPaintingEyes : public Object {
public:

};

//Entity Class
class EntityPaintingEyes : public Entity {
public:

};

//Object Entity
ObjectPaintingEyes PaintingEyes;

//Entity Functions
void PaintingEyes_Update();
void PaintingEyes_EarlyUpdate();
void PaintingEyes_LateUpdate();
void PaintingEyes_Draw();
void PaintingEyes_Setup(void* subtype);
void PaintingEyes_StageLoad();
void PaintingEyes_GetAttributes();

#endif //!OBJ_PAINTINGEYES_H
