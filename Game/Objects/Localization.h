#ifndef OBJ_LOCALIZATION_H
#define OBJ_LOCALIZATION_H

//Object Class
class Localization : Object {

};

//Entity Class
class EntityLocalization : Entity {

};

//Entity Functions
void Localization_Update();
void Localization_EarlyUpdate();
void Localization_LateUpdate();
void Localization_Draw();
void Localization_Setup(void* subtype);
void Localization_StageLoad();
void Localization_GetAttributes();

#endif //!OBJ_LOCALIZATION_H
