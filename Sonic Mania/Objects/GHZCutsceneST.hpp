#ifndef OBJ_GHZCUTSCENEST_H
#define OBJ_GHZCUTSCENEST_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGHZCutsceneST : Object {

};

// Entity Class
struct EntityGHZCutsceneST : Entity {

};

// Object Struct
extern ObjectGHZCutsceneST *GHZCutsceneST;

// Standard Entity Events
void GHZCutsceneST_Update();
void GHZCutsceneST_LateUpdate();
void GHZCutsceneST_StaticUpdate();
void GHZCutsceneST_Draw();
void GHZCutsceneST_Create(void* data);
void GHZCutsceneST_StageLoad();
void GHZCutsceneST_EditorDraw();
void GHZCutsceneST_EditorLoad();
void GHZCutsceneST_Serialize();

// Extra Entity Functions


#endif //!OBJ_GHZCUTSCENEST_H
