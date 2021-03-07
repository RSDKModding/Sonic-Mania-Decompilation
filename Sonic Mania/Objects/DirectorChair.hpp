#ifndef OBJ_DIRECTORCHAIR_H
#define OBJ_DIRECTORCHAIR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDirectorChair : Object {

};

// Entity Class
struct EntityDirectorChair : Entity {

};

// Object Struct
extern ObjectDirectorChair *DirectorChair;

// Standard Entity Events
void DirectorChair_Update();
void DirectorChair_LateUpdate();
void DirectorChair_StaticUpdate();
void DirectorChair_Draw();
void DirectorChair_Create(void* data);
void DirectorChair_StageLoad();
void DirectorChair_EditorDraw();
void DirectorChair_EditorLoad();
void DirectorChair_Serialize();

// Extra Entity Functions


#endif //!OBJ_DIRECTORCHAIR_H
