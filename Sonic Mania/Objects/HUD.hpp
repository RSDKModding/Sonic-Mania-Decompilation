#ifndef OBJ_HUD_H
#define OBJ_HUD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHUD : Object{

};

// Entity Class
struct EntityHUD : Entity {

};

// Object Struct
extern ObjectHUD *HUD;

// Standard Entity Events
void HUD_Update();
void HUD_LateUpdate();
void HUD_StaticUpdate();
void HUD_Draw();
void HUD_Create(void* data);
void HUD_StageLoad();
void HUD_EditorDraw();
void HUD_EditorLoad();
void HUD_Serialize();

// Extra Entity Functions


#endif //!OBJ_HUD_H
