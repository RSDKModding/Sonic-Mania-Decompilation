#ifndef OBJ_MMZWHEEL_H
#define OBJ_MMZWHEEL_H

//Object Class
class MMZWheel : Object {

};

//Entity Class
class EntityMMZWheel : Entity {

};

//Entity Functions
void MMZWheel_Update();
void MMZWheel_EarlyUpdate();
void MMZWheel_LateUpdate();
void MMZWheel_Draw();
void MMZWheel_Setup(void* subtype);
void MMZWheel_StageLoad();
void MMZWheel_GetAttributes();

#endif //!OBJ_MMZWHEEL_H
