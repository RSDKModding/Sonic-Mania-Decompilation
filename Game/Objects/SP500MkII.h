#ifndef OBJ_SP500MKII_H
#define OBJ_SP500MKII_H

//Object Class
class SP500MkII : Object {

};

//Entity Class
class EntitySP500MkII : Entity {

};

//Entity Functions
void SP500MkII_Update();
void SP500MkII_EarlyUpdate();
void SP500MkII_LateUpdate();
void SP500MkII_Draw();
void SP500MkII_Setup(void* subtype);
void SP500MkII_StageLoad();
void SP500MkII_GetAttributes();

#endif //!OBJ_SP500MKII_H
