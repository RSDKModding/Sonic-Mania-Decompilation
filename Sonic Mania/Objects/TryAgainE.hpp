#ifndef OBJ_TRYAGAINE_H
#define OBJ_TRYAGAINE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTryAgainE : Object {

};

// Entity Class
struct EntityTryAgainE : Entity {

};

// Object Entity
extern ObjectTryAgainE TryAgainE;

// Standard Entity Events
void TryAgainE_Update();
void TryAgainE_LateUpdate();
void TryAgainE_StaticUpdate();
void TryAgainE_Draw();
void TryAgainE_Create(void* data);
void TryAgainE_StageLoad();
void TryAgainE_EditorDraw();
void TryAgainE_EditorLoad();
void TryAgainE_Serialize();

// Extra Entity Functions


#endif //!OBJ_TRYAGAINE_H
