#ifndef OBJ_ROTATINGSTAIR_H
#define OBJ_ROTATINGSTAIR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectRotatingStair : Object {

};

// Entity Class
struct EntityRotatingStair : Entity {

};

// Object Struct
extern ObjectRotatingStair *RotatingStair;

// Standard Entity Events
void RotatingStair_Update();
void RotatingStair_LateUpdate();
void RotatingStair_StaticUpdate();
void RotatingStair_Draw();
void RotatingStair_Create(void* data);
void RotatingStair_StageLoad();
void RotatingStair_EditorDraw();
void RotatingStair_EditorLoad();
void RotatingStair_Serialize();

// Extra Entity Functions


#endif //!OBJ_ROTATINGSTAIR_H
