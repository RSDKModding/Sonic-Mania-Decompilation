#ifndef OBJ_MSZ1KINTRO_H
#define OBJ_MSZ1KINTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMSZ1KIntro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMSZ1KIntro;

// Object Struct
extern ObjectMSZ1KIntro *MSZ1KIntro;

// Standard Entity Events
void MSZ1KIntro_Update();
void MSZ1KIntro_LateUpdate();
void MSZ1KIntro_StaticUpdate();
void MSZ1KIntro_Draw();
void MSZ1KIntro_Create(void* data);
void MSZ1KIntro_StageLoad();
void MSZ1KIntro_EditorDraw();
void MSZ1KIntro_EditorLoad();
void MSZ1KIntro_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSZ1KINTRO_H
