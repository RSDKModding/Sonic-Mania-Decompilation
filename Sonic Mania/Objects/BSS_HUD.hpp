#ifndef OBJ_BSS_HUD_H
#define OBJ_BSS_HUD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBSS_HUD : Object {

};

// Entity Class
struct EntityBSS_HUD : Entity {

};

// Object Struct
extern ObjectBSS_HUD BSS_HUD;

// Standard Entity Events
void BSS_HUD_Update();
void BSS_HUD_LateUpdate();
void BSS_HUD_StaticUpdate();
void BSS_HUD_Draw();
void BSS_HUD_Create(void* data);
void BSS_HUD_StageLoad();
void BSS_HUD_EditorDraw();
void BSS_HUD_EditorLoad();
void BSS_HUD_Serialize();

// Extra Entity Functions


#endif //!OBJ_BSS_HUD_H
