#ifndef OBJ_FLASHERMKII_H
#define OBJ_FLASHERMKII_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFlasherMKII : Object {

};

// Entity Class
struct EntityFlasherMKII : Entity {

};

// Object Entity
extern ObjectFlasherMKII FlasherMKII;

// Standard Entity Events
void FlasherMKII_Update();
void FlasherMKII_LateUpdate();
void FlasherMKII_StaticUpdate();
void FlasherMKII_Draw();
void FlasherMKII_Create(void* data);
void FlasherMKII_StageLoad();
void FlasherMKII_EditorDraw();
void FlasherMKII_EditorLoad();
void FlasherMKII_Serialize();

// Extra Entity Functions


#endif //!OBJ_FLASHERMKII_H
