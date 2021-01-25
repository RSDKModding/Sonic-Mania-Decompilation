#ifndef OBJ_AMOEBADROID_H
#define OBJ_AMOEBADROID_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectAmoebaDroid : Object {

};

// Entity Class
struct EntityAmoebaDroid : Entity {

};

// Object Entity
extern ObjectAmoebaDroid AmoebaDroid;

// Standard Entity Events
void AmoebaDroid_Update();
void AmoebaDroid_LateUpdate();
void AmoebaDroid_StaticUpdate();
void AmoebaDroid_Draw();
void AmoebaDroid_Create(void* data);
void AmoebaDroid_StageLoad();
void AmoebaDroid_EditorDraw();
void AmoebaDroid_EditorLoad();
void AmoebaDroid_Serialize();

// Extra Entity Functions


#endif //!OBJ_AMOEBADROID_H
