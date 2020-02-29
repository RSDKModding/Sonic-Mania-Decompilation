#ifndef OBJ_SPECIALCLEAR_H
#define OBJ_SPECIALCLEAR_H

//Object Class
class SpecialClear : Object {

};

//Entity Class
class EntitySpecialClear : Entity {

};

//Entity Functions
void SpecialClear_Update();
void SpecialClear_EarlyUpdate();
void SpecialClear_LateUpdate();
void SpecialClear_Draw();
void SpecialClear_Setup(void* subtype);
void SpecialClear_StageLoad();
void SpecialClear_GetAttributes();

#endif //!OBJ_SPECIALCLEAR_H
