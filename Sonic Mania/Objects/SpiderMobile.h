#ifndef OBJ_SPIDERMOBILE_H
#define OBJ_SPIDERMOBILE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpiderMobile;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpiderMobile;

// Object Struct
extern ObjectSpiderMobile *SpiderMobile;

// Standard Entity Events
void SpiderMobile_Update();
void SpiderMobile_LateUpdate();
void SpiderMobile_StaticUpdate();
void SpiderMobile_Draw();
void SpiderMobile_Create(void* data);
void SpiderMobile_StageLoad();
void SpiderMobile_EditorDraw();
void SpiderMobile_EditorLoad();
void SpiderMobile_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPIDERMOBILE_H
