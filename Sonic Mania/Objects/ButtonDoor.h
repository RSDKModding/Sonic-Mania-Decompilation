#ifndef OBJ_BUTTONDOOR_H
#define OBJ_BUTTONDOOR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectButtonDoor;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityButtonDoor;

// Object Struct
extern ObjectButtonDoor *ButtonDoor;

// Standard Entity Events
void ButtonDoor_Update();
void ButtonDoor_LateUpdate();
void ButtonDoor_StaticUpdate();
void ButtonDoor_Draw();
void ButtonDoor_Create(void* data);
void ButtonDoor_StageLoad();
void ButtonDoor_EditorDraw();
void ButtonDoor_EditorLoad();
void ButtonDoor_Serialize();

// Extra Entity Functions


#endif //!OBJ_BUTTONDOOR_H
