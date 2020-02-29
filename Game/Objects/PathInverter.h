#ifndef OBJ_PATHINVERTER_H
#define OBJ_PATHINVERTER_H

//Object Class
class PathInverter : Object {

};

//Entity Class
class EntityPathInverter : Entity {

};

//Entity Functions
void PathInverter_Update();
void PathInverter_EarlyUpdate();
void PathInverter_LateUpdate();
void PathInverter_Draw();
void PathInverter_Setup(void* subtype);
void PathInverter_StageLoad();
void PathInverter_GetAttributes();

#endif //!OBJ_PATHINVERTER_H
