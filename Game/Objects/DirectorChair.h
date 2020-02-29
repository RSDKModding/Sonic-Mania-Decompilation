#ifndef OBJ_DIRECTORCHAIR_H
#define OBJ_DIRECTORCHAIR_H

//Object Class
class DirectorChair : Object {

};

//Entity Class
class EntityDirectorChair : Entity {

};

//Entity Functions
void DirectorChair_Update();
void DirectorChair_EarlyUpdate();
void DirectorChair_LateUpdate();
void DirectorChair_Draw();
void DirectorChair_Setup(void* subtype);
void DirectorChair_StageLoad();
void DirectorChair_GetAttributes();

#endif //!OBJ_DIRECTORCHAIR_H
