#ifndef OBJ_BLADEPOLE_H
#define OBJ_BLADEPOLE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBladePole : Object{

};

// Entity Class
struct EntityBladePole : Entity {

};

// Object Struct
extern ObjectBladePole *BladePole;

// Standard Entity Events
void BladePole_Update();
void BladePole_LateUpdate();
void BladePole_StaticUpdate();
void BladePole_Draw();
void BladePole_Create(void* data);
void BladePole_StageLoad();
void BladePole_EditorDraw();
void BladePole_EditorLoad();
void BladePole_Serialize();

// Extra Entity Functions


#endif //!OBJ_BLADEPOLE_H
