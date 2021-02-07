#ifndef OBJ_BSS_PALETTE_H
#define OBJ_BSS_PALETTE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBSS_Palette : Object {

};

// Entity Class
struct EntityBSS_Palette : Entity {

};

// Object Struct
extern ObjectBSS_Palette BSS_Palette;

// Standard Entity Events
void BSS_Palette_Update();
void BSS_Palette_LateUpdate();
void BSS_Palette_StaticUpdate();
void BSS_Palette_Draw();
void BSS_Palette_Create(void* data);
void BSS_Palette_StageLoad();
void BSS_Palette_EditorDraw();
void BSS_Palette_EditorLoad();
void BSS_Palette_Serialize();

// Extra Entity Functions


#endif //!OBJ_BSS_PALETTE_H
