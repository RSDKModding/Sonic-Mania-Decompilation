#ifndef OBJ_WOODROW_H
#define OBJ_WOODROW_H

//Object Class
class Woodrow : Object {

};

//Entity Class
class EntityWoodrow : Entity {

};

//Entity Functions
void Woodrow_Update();
void Woodrow_EarlyUpdate();
void Woodrow_LateUpdate();
void Woodrow_Draw();
void Woodrow_Setup(void* subtype);
void Woodrow_StageLoad();
void Woodrow_GetAttributes();

#endif //!OBJ_WOODROW_H
