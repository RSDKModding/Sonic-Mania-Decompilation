#ifndef OBJ_CORKSCREWPATH_H
#define OBJ_CORKSCREWPATH_H

#include "../../SonicMania.h"

//Object Class
class ObjectCorkscrewPath : public Object {
public:

};

//Entity Class
class EntityCorkscrewPath : public Entity {
public:

};

//Object Entity
ObjectCorkscrewPath CorkscrewPath;

//Entity Functions
void CorkscrewPath_Update();
void CorkscrewPath_EarlyUpdate();
void CorkscrewPath_LateUpdate();
void CorkscrewPath_Draw();
void CorkscrewPath_Setup(void* subtype);
void CorkscrewPath_StageLoad();
void CorkscrewPath_GetAttributes();

#endif //!OBJ_CORKSCREWPATH_H
