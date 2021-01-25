#ifndef OBJ_SIGNPOST_H
#define OBJ_SIGNPOST_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSignPost : Object {

};

// Entity Class
struct EntitySignPost : Entity {

};

// Object Entity
extern ObjectSignPost SignPost;

// Standard Entity Events
void SignPost_Update();
void SignPost_LateUpdate();
void SignPost_StaticUpdate();
void SignPost_Draw();
void SignPost_Create(void* data);
void SignPost_StageLoad();
void SignPost_EditorDraw();
void SignPost_EditorLoad();
void SignPost_Serialize();

// Extra Entity Functions


#endif //!OBJ_SIGNPOST_H
