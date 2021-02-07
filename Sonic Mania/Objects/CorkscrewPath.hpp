#ifndef OBJ_CORKSCREWPATH_H
#define OBJ_CORKSCREWPATH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCorkscrewPath : Object{

};

// Entity Class
struct EntityCorkscrewPath : Entity {

};

// Object Struct
extern ObjectCorkscrewPath *CorkscrewPath;

// Standard Entity Events
void CorkscrewPath_Update();
void CorkscrewPath_LateUpdate();
void CorkscrewPath_StaticUpdate();
void CorkscrewPath_Draw();
void CorkscrewPath_Create(void* data);
void CorkscrewPath_StageLoad();
void CorkscrewPath_EditorDraw();
void CorkscrewPath_EditorLoad();
void CorkscrewPath_Serialize();

// Extra Entity Functions


#endif //!OBJ_CORKSCREWPATH_H
