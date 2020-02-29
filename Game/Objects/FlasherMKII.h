#ifndef OBJ_FLASHERMKII_H
#define OBJ_FLASHERMKII_H

//Object Class
class FlasherMKII : Object {

};

//Entity Class
class EntityFlasherMKII : Entity {

};

//Entity Functions
void FlasherMKII_Update();
void FlasherMKII_EarlyUpdate();
void FlasherMKII_LateUpdate();
void FlasherMKII_Draw();
void FlasherMKII_Setup(void* subtype);
void FlasherMKII_StageLoad();
void FlasherMKII_GetAttributes();

#endif //!OBJ_FLASHERMKII_H
