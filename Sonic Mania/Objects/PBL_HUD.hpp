#ifndef OBJ_PBL_HUD_H
#define OBJ_PBL_HUD_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectPBL_HUD : Object {

};

// Entity Class
struct EntityPBL_HUD : Entity {

};

// Object Struct
extern ObjectPBL_HUD *PBL_HUD;

// Standard Entity Events
void PBL_HUD_Update();
void PBL_HUD_LateUpdate();
void PBL_HUD_StaticUpdate();
void PBL_HUD_Draw();
void PBL_HUD_Create(void* data);
void PBL_HUD_StageLoad();
void PBL_HUD_EditorDraw();
void PBL_HUD_EditorLoad();
void PBL_HUD_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_HUD_H
