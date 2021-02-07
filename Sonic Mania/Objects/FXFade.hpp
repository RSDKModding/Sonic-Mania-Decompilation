#ifndef OBJ_FXFADE_H
#define OBJ_FXFADE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFXFade : Object{

};

// Entity Class
struct EntityFXFade : Entity {

};

// Object Struct
extern ObjectFXFade *FXFade;

// Standard Entity Events
void FXFade_Update();
void FXFade_LateUpdate();
void FXFade_StaticUpdate();
void FXFade_Draw();
void FXFade_Create(void* data);
void FXFade_StageLoad();
void FXFade_EditorDraw();
void FXFade_EditorLoad();
void FXFade_Serialize();

// Extra Entity Functions


#endif //!OBJ_FXFADE_H
