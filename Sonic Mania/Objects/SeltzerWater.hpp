#ifndef OBJ_SELTZERWATER_H
#define OBJ_SELTZERWATER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSeltzerWater : Object {

};

// Entity Class
struct EntitySeltzerWater : Entity {

};

// Object Struct
extern ObjectSeltzerWater *SeltzerWater;

// Standard Entity Events
void SeltzerWater_Update();
void SeltzerWater_LateUpdate();
void SeltzerWater_StaticUpdate();
void SeltzerWater_Draw();
void SeltzerWater_Create(void* data);
void SeltzerWater_StageLoad();
void SeltzerWater_EditorDraw();
void SeltzerWater_EditorLoad();
void SeltzerWater_Serialize();

// Extra Entity Functions


#endif //!OBJ_SELTZERWATER_H
