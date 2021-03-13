#ifndef OBJ_BUNGEE_H
#define OBJ_BUNGEE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBungee;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBungee;

// Object Struct
extern ObjectBungee *Bungee;

// Standard Entity Events
void Bungee_Update();
void Bungee_LateUpdate();
void Bungee_StaticUpdate();
void Bungee_Draw();
void Bungee_Create(void* data);
void Bungee_StageLoad();
void Bungee_EditorDraw();
void Bungee_EditorLoad();
void Bungee_Serialize();

// Extra Entity Functions


#endif //!OBJ_BUNGEE_H
