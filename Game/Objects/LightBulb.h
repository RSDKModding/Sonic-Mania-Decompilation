#ifndef OBJ_LIGHTBULB_H
#define OBJ_LIGHTBULB_H

//Object Class
class LightBulb : Object {

};

//Entity Class
class EntityLightBulb : Entity {

};

//Entity Functions
void LightBulb_Update();
void LightBulb_EarlyUpdate();
void LightBulb_LateUpdate();
void LightBulb_Draw();
void LightBulb_Setup(void* subtype);
void LightBulb_StageLoad();
void LightBulb_GetAttributes();

#endif //!OBJ_LIGHTBULB_H
