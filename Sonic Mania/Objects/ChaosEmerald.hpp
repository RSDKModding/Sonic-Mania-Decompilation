#ifndef OBJ_CHAOSEMERALD_H
#define OBJ_CHAOSEMERALD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectChaosEmerald : Object{

};

// Entity Class
struct EntityChaosEmerald : Entity {

};

// Object Struct
extern ObjectChaosEmerald *ChaosEmerald;

// Standard Entity Events
void ChaosEmerald_Update();
void ChaosEmerald_LateUpdate();
void ChaosEmerald_StaticUpdate();
void ChaosEmerald_Draw();
void ChaosEmerald_Create(void* data);
void ChaosEmerald_StageLoad();
void ChaosEmerald_EditorDraw();
void ChaosEmerald_EditorLoad();
void ChaosEmerald_Serialize();

// Extra Entity Functions


#endif //!OBJ_CHAOSEMERALD_H
