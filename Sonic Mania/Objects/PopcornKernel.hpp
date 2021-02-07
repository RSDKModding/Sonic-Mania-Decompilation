#ifndef OBJ_POPCORNKERNEL_H
#define OBJ_POPCORNKERNEL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPopcornKernel : Object{

};

// Entity Class
struct EntityPopcornKernel : Entity {

};

// Object Struct
extern ObjectPopcornKernel *PopcornKernel;

// Standard Entity Events
void PopcornKernel_Update();
void PopcornKernel_LateUpdate();
void PopcornKernel_StaticUpdate();
void PopcornKernel_Draw();
void PopcornKernel_Create(void* data);
void PopcornKernel_StageLoad();
void PopcornKernel_EditorDraw();
void PopcornKernel_EditorLoad();
void PopcornKernel_Serialize();

// Extra Entity Functions


#endif //!OBJ_POPCORNKERNEL_H
