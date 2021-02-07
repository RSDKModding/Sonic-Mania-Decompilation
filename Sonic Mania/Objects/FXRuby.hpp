#ifndef OBJ_FXRUBY_H
#define OBJ_FXRUBY_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFXRuby : Object{

};

// Entity Class
struct EntityFXRuby : Entity {

};

// Object Struct
extern ObjectFXRuby *FXRuby;

// Standard Entity Events
void FXRuby_Update();
void FXRuby_LateUpdate();
void FXRuby_StaticUpdate();
void FXRuby_Draw();
void FXRuby_Create(void* data);
void FXRuby_StageLoad();
void FXRuby_EditorDraw();
void FXRuby_EditorLoad();
void FXRuby_Serialize();

// Extra Entity Functions


#endif //!OBJ_FXRUBY_H
