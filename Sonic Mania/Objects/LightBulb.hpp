#ifndef OBJ_LIGHTBULB_H
#define OBJ_LIGHTBULB_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLightBulb : Object{

};

// Entity Class
struct EntityLightBulb : Entity {

};

// Object Struct
extern ObjectLightBulb *LightBulb;

// Standard Entity Events
void LightBulb_Update();
void LightBulb_LateUpdate();
void LightBulb_StaticUpdate();
void LightBulb_Draw();
void LightBulb_Create(void* data);
void LightBulb_StageLoad();
void LightBulb_EditorDraw();
void LightBulb_EditorLoad();
void LightBulb_Serialize();

// Extra Entity Functions


#endif //!OBJ_LIGHTBULB_H
