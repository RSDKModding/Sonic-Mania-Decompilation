#ifndef OBJ_BUTTON_H
#define OBJ_BUTTON_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectButton;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityButton;

// Object Struct
extern ObjectButton *Button;

// Standard Entity Events
void Button_Update();
void Button_LateUpdate();
void Button_StaticUpdate();
void Button_Draw();
void Button_Create(void* data);
void Button_StageLoad();
void Button_EditorDraw();
void Button_EditorLoad();
void Button_Serialize();

// Extra Entity Functions


#endif //!OBJ_BUTTON_H
