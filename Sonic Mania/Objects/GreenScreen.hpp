#ifndef OBJ_GREENSCREEN_H
#define OBJ_GREENSCREEN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGreenScreen : Object{

};

// Entity Class
struct EntityGreenScreen : Entity {

};

// Object Struct
extern ObjectGreenScreen *GreenScreen;

// Standard Entity Events
void GreenScreen_Update();
void GreenScreen_LateUpdate();
void GreenScreen_StaticUpdate();
void GreenScreen_Draw();
void GreenScreen_Create(void* data);
void GreenScreen_StageLoad();
void GreenScreen_EditorDraw();
void GreenScreen_EditorLoad();
void GreenScreen_Serialize();

// Extra Entity Functions


#endif //!OBJ_GREENSCREEN_H
