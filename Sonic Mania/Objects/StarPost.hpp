#ifndef OBJ_STARPOST_H
#define OBJ_STARPOST_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectStarPost : Object{

};

// Entity Class
struct EntityStarPost : Entity {

};

// Object Struct
extern ObjectStarPost *StarPost;

// Standard Entity Events
void StarPost_Update();
void StarPost_LateUpdate();
void StarPost_StaticUpdate();
void StarPost_Draw();
void StarPost_Create(void* data);
void StarPost_StageLoad();
void StarPost_EditorDraw();
void StarPost_EditorLoad();
void StarPost_Serialize();

// Extra Entity Functions


#endif //!OBJ_STARPOST_H
