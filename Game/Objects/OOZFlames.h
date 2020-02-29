#ifndef OBJ_OOZFLAMES_H
#define OBJ_OOZFLAMES_H

//Object Class
class OOZFlames : Object {

};

//Entity Class
class EntityOOZFlames : Entity {

};

//Entity Functions
void OOZFlames_Update();
void OOZFlames_EarlyUpdate();
void OOZFlames_LateUpdate();
void OOZFlames_Draw();
void OOZFlames_Setup(void* subtype);
void OOZFlames_StageLoad();
void OOZFlames_GetAttributes();

#endif //!OBJ_OOZFLAMES_H
