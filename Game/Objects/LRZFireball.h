#ifndef OBJ_LRZFIREBALL_H
#define OBJ_LRZFIREBALL_H

//Object Class
class LRZFireball : Object {

};

//Entity Class
class EntityLRZFireball : Entity {

};

//Entity Functions
void LRZFireball_Update();
void LRZFireball_EarlyUpdate();
void LRZFireball_LateUpdate();
void LRZFireball_Draw();
void LRZFireball_Setup(void* subtype);
void LRZFireball_StageLoad();
void LRZFireball_GetAttributes();

#endif //!OBJ_LRZFIREBALL_H
