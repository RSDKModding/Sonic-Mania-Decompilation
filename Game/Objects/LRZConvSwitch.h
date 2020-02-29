#ifndef OBJ_LRZCONVSWITCH_H
#define OBJ_LRZCONVSWITCH_H

//Object Class
class LRZConvSwitch : Object {

};

//Entity Class
class EntityLRZConvSwitch : Entity {

};

//Entity Functions
void LRZConvSwitch_Update();
void LRZConvSwitch_EarlyUpdate();
void LRZConvSwitch_LateUpdate();
void LRZConvSwitch_Draw();
void LRZConvSwitch_Setup(void* subtype);
void LRZConvSwitch_StageLoad();
void LRZConvSwitch_GetAttributes();

#endif //!OBJ_LRZCONVSWITCH_H
