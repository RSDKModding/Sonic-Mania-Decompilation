#ifndef OBJ_WOODROW_H
#define OBJ_WOODROW_H

#include "../../SonicMania.h"

//Object Class
class ObjectWoodrow : public Object {
public:

};

//Entity Class
class EntityWoodrow : public Entity {
public:

};

//Object Entity
ObjectWoodrow Woodrow;

//Entity Functions
void Woodrow_Update();
void Woodrow_EarlyUpdate();
void Woodrow_LateUpdate();
void Woodrow_Draw();
void Woodrow_Setup(void* subtype);
void Woodrow_StageLoad();
void Woodrow_GetAttributes();

#endif //!OBJ_WOODROW_H
