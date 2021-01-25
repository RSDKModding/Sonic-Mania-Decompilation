#ifndef OBJ_TRYAGAIN_H
#define OBJ_TRYAGAIN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTryAgain : Object {

};

// Entity Class
struct EntityTryAgain : Entity {

};

// Object Entity
extern ObjectTryAgain TryAgain;

// Standard Entity Events
void TryAgain_Update();
void TryAgain_LateUpdate();
void TryAgain_StaticUpdate();
void TryAgain_Draw();
void TryAgain_Create(void* data);
void TryAgain_StageLoad();
void TryAgain_EditorDraw();
void TryAgain_EditorLoad();
void TryAgain_Serialize();

// Extra Entity Functions


#endif //!OBJ_TRYAGAIN_H
