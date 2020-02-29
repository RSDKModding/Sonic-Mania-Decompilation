#ifndef OBJ_MMZLIGHTNING_H
#define OBJ_MMZLIGHTNING_H

//Object Class
class MMZLightning : Object {

};

//Entity Class
class EntityMMZLightning : Entity {

};

//Entity Functions
void MMZLightning_Update();
void MMZLightning_EarlyUpdate();
void MMZLightning_LateUpdate();
void MMZLightning_Draw();
void MMZLightning_Setup(void* subtype);
void MMZLightning_StageLoad();
void MMZLightning_GetAttributes();

#endif //!OBJ_MMZLIGHTNING_H
