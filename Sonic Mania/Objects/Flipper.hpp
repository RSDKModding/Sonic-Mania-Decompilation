#ifndef OBJ_FLIPPER_H
#define OBJ_FLIPPER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFlipper : Object{

};

// Entity Class
struct EntityFlipper : Entity {

};

// Object Struct
extern ObjectFlipper *Flipper;

// Standard Entity Events
void Flipper_Update();
void Flipper_LateUpdate();
void Flipper_StaticUpdate();
void Flipper_Draw();
void Flipper_Create(void* data);
void Flipper_StageLoad();
void Flipper_EditorDraw();
void Flipper_EditorLoad();
void Flipper_Serialize();

// Extra Entity Functions


#endif //!OBJ_FLIPPER_H
