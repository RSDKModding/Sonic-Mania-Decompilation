#ifndef OBJ_SPIRALPLATFORM_H
#define OBJ_SPIRALPLATFORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpiralPlatform : Object {

};

// Entity Class
struct EntitySpiralPlatform : Entity {

};

// Object Entity
extern ObjectSpiralPlatform SpiralPlatform;

// Standard Entity Events
void SpiralPlatform_Update();
void SpiralPlatform_LateUpdate();
void SpiralPlatform_StaticUpdate();
void SpiralPlatform_Draw();
void SpiralPlatform_Create(void* data);
void SpiralPlatform_StageLoad();
void SpiralPlatform_EditorDraw();
void SpiralPlatform_EditorLoad();
void SpiralPlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPIRALPLATFORM_H
