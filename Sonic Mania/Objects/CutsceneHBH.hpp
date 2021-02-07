#ifndef OBJ_CUTSCENEHBH_H
#define OBJ_CUTSCENEHBH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCutsceneHBH : Object{

};

// Entity Class
struct EntityCutsceneHBH : Entity {

};

// Object Struct
extern ObjectCutsceneHBH *CutsceneHBH;

// Standard Entity Events
void CutsceneHBH_Update();
void CutsceneHBH_LateUpdate();
void CutsceneHBH_StaticUpdate();
void CutsceneHBH_Draw();
void CutsceneHBH_Create(void* data);
void CutsceneHBH_StageLoad();
void CutsceneHBH_EditorDraw();
void CutsceneHBH_EditorLoad();
void CutsceneHBH_Serialize();

// Extra Entity Functions


#endif //!OBJ_CUTSCENEHBH_H
