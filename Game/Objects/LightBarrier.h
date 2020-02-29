#ifndef OBJ_LIGHTBARRIER_H
#define OBJ_LIGHTBARRIER_H

//Object Class
class LightBarrier : Object {

};

//Entity Class
class EntityLightBarrier : Entity {

};

//Entity Functions
void LightBarrier_Update();
void LightBarrier_EarlyUpdate();
void LightBarrier_LateUpdate();
void LightBarrier_Draw();
void LightBarrier_Setup(void* subtype);
void LightBarrier_StageLoad();
void LightBarrier_GetAttributes();

#endif //!OBJ_LIGHTBARRIER_H
