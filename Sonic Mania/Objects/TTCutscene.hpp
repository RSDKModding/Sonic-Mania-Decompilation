#ifndef OBJ_TTCUTSCENE_H
#define OBJ_TTCUTSCENE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTTCutscene : Object {

};

// Entity Class
struct EntityTTCutscene : Entity {

};

// Object Struct
extern ObjectTTCutscene *TTCutscene;

// Standard Entity Events
void TTCutscene_Update();
void TTCutscene_LateUpdate();
void TTCutscene_StaticUpdate();
void TTCutscene_Draw();
void TTCutscene_Create(void* data);
void TTCutscene_StageLoad();
void TTCutscene_EditorDraw();
void TTCutscene_EditorLoad();
void TTCutscene_Serialize();

// Extra Entity Functions


#endif //!OBJ_TTCUTSCENE_H
