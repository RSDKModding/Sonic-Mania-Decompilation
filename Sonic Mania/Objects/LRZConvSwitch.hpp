#ifndef OBJ_LRZCONVSWITCH_H
#define OBJ_LRZCONVSWITCH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZConvSwitch : Object {

};

// Entity Class
struct EntityLRZConvSwitch : Entity {

};

// Object Entity
extern ObjectLRZConvSwitch LRZConvSwitch;

// Standard Entity Events
void LRZConvSwitch_Update();
void LRZConvSwitch_LateUpdate();
void LRZConvSwitch_StaticUpdate();
void LRZConvSwitch_Draw();
void LRZConvSwitch_Create(void* data);
void LRZConvSwitch_StageLoad();
void LRZConvSwitch_EditorDraw();
void LRZConvSwitch_EditorLoad();
void LRZConvSwitch_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZCONVSWITCH_H
