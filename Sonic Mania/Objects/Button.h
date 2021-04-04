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
void Button_Update(void);
void Button_LateUpdate(void);
void Button_StaticUpdate(void);
void Button_Draw(void);
void Button_Create(void* data);
void Button_StageLoad(void);
void Button_EditorDraw(void);
void Button_EditorLoad(void);
void Button_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BUTTON_H
