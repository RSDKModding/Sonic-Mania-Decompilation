#ifndef OBJ_BARSTOOL_H
#define OBJ_BARSTOOL_H

//Object Class
class BarStool : Object {

};

//Entity Class
class EntityBarStool : Entity {

};

//Entity Functions
void BarStool_Update();
void BarStool_EarlyUpdate();
void BarStool_LateUpdate();
void BarStool_Draw();
void BarStool_Setup(void* subtype);
void BarStool_StageLoad();
void BarStool_GetAttributes();

#endif //!OBJ_BARSTOOL_H
