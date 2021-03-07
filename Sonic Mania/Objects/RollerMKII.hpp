#ifndef OBJ_ROLLERMKII_H
#define OBJ_ROLLERMKII_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectRollerMKII : Object {

};

// Entity Class
struct EntityRollerMKII : Entity {

};

// Object Struct
extern ObjectRollerMKII *RollerMKII;

// Standard Entity Events
void RollerMKII_Update();
void RollerMKII_LateUpdate();
void RollerMKII_StaticUpdate();
void RollerMKII_Draw();
void RollerMKII_Create(void* data);
void RollerMKII_StageLoad();
void RollerMKII_EditorDraw();
void RollerMKII_EditorLoad();
void RollerMKII_Serialize();

// Extra Entity Functions


#endif //!OBJ_ROLLERMKII_H
