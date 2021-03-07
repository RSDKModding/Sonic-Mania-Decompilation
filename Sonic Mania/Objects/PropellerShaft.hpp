#ifndef OBJ_PROPELLERSHAFT_H
#define OBJ_PROPELLERSHAFT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPropellerShaft : Object {

};

// Entity Class
struct EntityPropellerShaft : Entity {

};

// Object Struct
extern ObjectPropellerShaft *PropellerShaft;

// Standard Entity Events
void PropellerShaft_Update();
void PropellerShaft_LateUpdate();
void PropellerShaft_StaticUpdate();
void PropellerShaft_Draw();
void PropellerShaft_Create(void* data);
void PropellerShaft_StageLoad();
void PropellerShaft_EditorDraw();
void PropellerShaft_EditorLoad();
void PropellerShaft_Serialize();

// Extra Entity Functions


#endif //!OBJ_PROPELLERSHAFT_H
