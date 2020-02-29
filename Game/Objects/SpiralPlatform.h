#ifndef OBJ_SPIRALPLATFORM_H
#define OBJ_SPIRALPLATFORM_H

//Object Class
class SpiralPlatform : Object {

};

//Entity Class
class EntitySpiralPlatform : Entity {

};

//Entity Functions
void SpiralPlatform_Update();
void SpiralPlatform_EarlyUpdate();
void SpiralPlatform_LateUpdate();
void SpiralPlatform_Draw();
void SpiralPlatform_Setup(void* subtype);
void SpiralPlatform_StageLoad();
void SpiralPlatform_GetAttributes();

#endif //!OBJ_SPIRALPLATFORM_H
