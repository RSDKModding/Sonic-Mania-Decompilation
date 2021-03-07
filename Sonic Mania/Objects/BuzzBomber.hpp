#ifndef OBJ_BUZZBOMBER_H
#define OBJ_BUZZBOMBER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBuzzBomber : Object {

};

// Entity Class
struct EntityBuzzBomber : Entity {

};

// Object Struct
extern ObjectBuzzBomber *BuzzBomber;

// Standard Entity Events
void BuzzBomber_Update();
void BuzzBomber_LateUpdate();
void BuzzBomber_StaticUpdate();
void BuzzBomber_Draw();
void BuzzBomber_Create(void* data);
void BuzzBomber_StageLoad();
void BuzzBomber_EditorDraw();
void BuzzBomber_EditorLoad();
void BuzzBomber_Serialize();

// Extra Entity Functions


#endif //!OBJ_BUZZBOMBER_H
