#ifndef OBJ_PULLCHAIN_H
#define OBJ_PULLCHAIN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPullChain : Object {

};

// Entity Class
struct EntityPullChain : Entity {

};

// Object Entity
extern ObjectPullChain PullChain;

// Standard Entity Events
void PullChain_Update();
void PullChain_LateUpdate();
void PullChain_StaticUpdate();
void PullChain_Draw();
void PullChain_Create(void* data);
void PullChain_StageLoad();
void PullChain_EditorDraw();
void PullChain_EditorLoad();
void PullChain_Serialize();

// Extra Entity Functions


#endif //!OBJ_PULLCHAIN_H
