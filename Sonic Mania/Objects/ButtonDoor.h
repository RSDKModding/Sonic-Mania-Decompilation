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
void ButtonDoor_Update(void);
void ButtonDoor_LateUpdate(void);
void ButtonDoor_StaticUpdate(void);
void ButtonDoor_Draw(void);
void ButtonDoor_Create(void* data);
void ButtonDoor_StageLoad(void);
void ButtonDoor_EditorDraw(void);
void ButtonDoor_EditorLoad(void);
void ButtonDoor_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BUTTONDOOR_H
