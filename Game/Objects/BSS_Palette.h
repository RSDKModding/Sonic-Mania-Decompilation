#ifndef OBJ_BSS_PALETTE_H
#define OBJ_BSS_PALETTE_H

#include "../../SonicMania.h"

//Object Class
class ObjectBSS_Palette : public Object {
public:

};

//Entity Class
class EntityBSS_Palette : public Entity {
public:

};

//Object Entity
ObjectBSS_Palette BSS_Palette;

//Entity Functions
void BSS_Palette_Update();
void BSS_Palette_EarlyUpdate();
void BSS_Palette_LateUpdate();
void BSS_Palette_Draw();
void BSS_Palette_Setup(void* subtype);
void BSS_Palette_StageLoad();
void BSS_Palette_GetAttributes();

#endif //!OBJ_BSS_PALETTE_H
