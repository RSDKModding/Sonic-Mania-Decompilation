#ifndef OBJ_DIRECTORCHAIR_H
#define OBJ_DIRECTORCHAIR_H

#include "../../SonicMania.h"

//Object Class
class ObjectDirectorChair : public Object {
public:

};

//Entity Class
class EntityDirectorChair : public Entity {
public:

};

//Object Entity
ObjectDirectorChair DirectorChair;

//Entity Functions
void DirectorChair_Update();
void DirectorChair_EarlyUpdate();
void DirectorChair_LateUpdate();
void DirectorChair_Draw();
void DirectorChair_Setup(void* subtype);
void DirectorChair_StageLoad();
void DirectorChair_GetAttributes();

#endif //!OBJ_DIRECTORCHAIR_H
