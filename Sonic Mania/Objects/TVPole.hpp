#ifndef OBJ_TVPOLE_H
#define OBJ_TVPOLE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTVPole : Object{

};

// Entity Class
struct EntityTVPole : Entity {

};

// Object Struct
extern ObjectTVPole *TVPole;

// Standard Entity Events
void TVPole_Update();
void TVPole_LateUpdate();
void TVPole_StaticUpdate();
void TVPole_Draw();
void TVPole_Create(void* data);
void TVPole_StageLoad();
void TVPole_EditorDraw();
void TVPole_EditorLoad();
void TVPole_Serialize();

// Extra Entity Functions


#endif //!OBJ_TVPOLE_H
