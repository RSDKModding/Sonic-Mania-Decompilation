#ifndef OBJ_FBZFAN_H
#define OBJ_FBZFAN_H

//Object Class
class FBZFan : Object {

};

//Entity Class
class EntityFBZFan : Entity {

};

//Entity Functions
void FBZFan_Update();
void FBZFan_EarlyUpdate();
void FBZFan_LateUpdate();
void FBZFan_Draw();
void FBZFan_Setup(void* subtype);
void FBZFan_StageLoad();
void FBZFan_GetAttributes();

#endif //!OBJ_FBZFAN_H
