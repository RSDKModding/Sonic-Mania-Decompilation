#ifndef OBJ_LIGHTBARRIER_H
#define OBJ_LIGHTBARRIER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLightBarrier : Object{

};

// Entity Class
struct EntityLightBarrier : Entity {

};

// Object Struct
extern ObjectLightBarrier *LightBarrier;

// Standard Entity Events
void LightBarrier_Update();
void LightBarrier_LateUpdate();
void LightBarrier_StaticUpdate();
void LightBarrier_Draw();
void LightBarrier_Create(void* data);
void LightBarrier_StageLoad();
void LightBarrier_EditorDraw();
void LightBarrier_EditorLoad();
void LightBarrier_Serialize();

// Extra Entity Functions


#endif //!OBJ_LIGHTBARRIER_H
