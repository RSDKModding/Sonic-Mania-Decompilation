#ifndef OBJ_SDASHWHEEL_H
#define OBJ_SDASHWHEEL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSDashWheel : Object {

};

// Entity Class
struct EntitySDashWheel : Entity {

};

// Object Entity
extern ObjectSDashWheel SDashWheel;

// Standard Entity Events
void SDashWheel_Update();
void SDashWheel_LateUpdate();
void SDashWheel_StaticUpdate();
void SDashWheel_Draw();
void SDashWheel_Create(void* data);
void SDashWheel_StageLoad();
void SDashWheel_EditorDraw();
void SDashWheel_EditorLoad();
void SDashWheel_Serialize();

// Extra Entity Functions


#endif //!OBJ_SDASHWHEEL_H
