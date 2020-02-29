#ifndef OBJ_ROLLERMKII_H
#define OBJ_ROLLERMKII_H

//Object Class
class RollerMKII : Object {

};

//Entity Class
class EntityRollerMKII : Entity {

};

//Entity Functions
void RollerMKII_Update();
void RollerMKII_EarlyUpdate();
void RollerMKII_LateUpdate();
void RollerMKII_Draw();
void RollerMKII_Setup(void* subtype);
void RollerMKII_StageLoad();
void RollerMKII_GetAttributes();

#endif //!OBJ_ROLLERMKII_H
