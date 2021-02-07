#ifndef OBJ_LRZSPIRAL_H
#define OBJ_LRZSPIRAL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZSpiral : Object{

};

// Entity Class
struct EntityLRZSpiral : Entity {

};

// Object Struct
extern ObjectLRZSpiral *LRZSpiral;

// Standard Entity Events
void LRZSpiral_Update();
void LRZSpiral_LateUpdate();
void LRZSpiral_StaticUpdate();
void LRZSpiral_Draw();
void LRZSpiral_Create(void* data);
void LRZSpiral_StageLoad();
void LRZSpiral_EditorDraw();
void LRZSpiral_EditorLoad();
void LRZSpiral_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZSPIRAL_H
