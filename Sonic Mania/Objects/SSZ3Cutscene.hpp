#ifndef OBJ_SSZ3CUTSCENE_H
#define OBJ_SSZ3CUTSCENE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSSZ3Cutscene : Object {

};

// Entity Class
struct EntitySSZ3Cutscene : Entity {

};

// Object Struct
extern ObjectSSZ3Cutscene SSZ3Cutscene;

// Standard Entity Events
void SSZ3Cutscene_Update();
void SSZ3Cutscene_LateUpdate();
void SSZ3Cutscene_StaticUpdate();
void SSZ3Cutscene_Draw();
void SSZ3Cutscene_Create(void* data);
void SSZ3Cutscene_StageLoad();
void SSZ3Cutscene_EditorDraw();
void SSZ3Cutscene_EditorLoad();
void SSZ3Cutscene_Serialize();

// Extra Entity Functions


#endif //!OBJ_SSZ3CUTSCENE_H
