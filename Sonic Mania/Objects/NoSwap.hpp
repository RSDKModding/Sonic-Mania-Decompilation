#ifndef OBJ_NOSWAP_H
#define OBJ_NOSWAP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectNoSwap : Object{

};

// Entity Class
struct EntityNoSwap : Entity {

};

// Object Struct
extern ObjectNoSwap *NoSwap;

// Standard Entity Events
void NoSwap_Update();
void NoSwap_LateUpdate();
void NoSwap_StaticUpdate();
void NoSwap_Draw();
void NoSwap_Create(void* data);
void NoSwap_StageLoad();
void NoSwap_EditorDraw();
void NoSwap_EditorLoad();
void NoSwap_Serialize();

// Extra Entity Functions


#endif //!OBJ_NOSWAP_H
