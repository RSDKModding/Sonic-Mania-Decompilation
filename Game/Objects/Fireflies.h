#ifndef OBJ_FIREFLIES_H
#define OBJ_FIREFLIES_H

//Object Class
class Fireflies : Object {

};

//Entity Class
class EntityFireflies : Entity {

};

//Entity Functions
void Fireflies_Update();
void Fireflies_EarlyUpdate();
void Fireflies_LateUpdate();
void Fireflies_Draw();
void Fireflies_Setup(void* subtype);
void Fireflies_StageLoad();
void Fireflies_GetAttributes();

#endif //!OBJ_FIREFLIES_H
