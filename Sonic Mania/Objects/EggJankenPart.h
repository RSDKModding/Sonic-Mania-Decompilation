#ifndef OBJ_EGGJANKENPART_H
#define OBJ_EGGJANKENPART_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectEggJankenPart;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityEggJankenPart;

// Object Entity
extern ObjectEggJankenPart *EggJankenPart;

// Standard Entity Events
void EggJankenPart_Update();
void EggJankenPart_LateUpdate();
void EggJankenPart_StaticUpdate();
void EggJankenPart_Draw();
void EggJankenPart_Create(void* data);
void EggJankenPart_StageLoad();
void EggJankenPart_EditorDraw();
void EggJankenPart_EditorLoad();
void EggJankenPart_Serialize();

// Extra Entity Functions


#endif //!OBJ_EGGJANKENPART_H
