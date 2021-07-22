#ifndef OBJ_SPIDERMOBILE_H
#define OBJ_SPIDERMOBILE_H

#include "SonicMania.h"

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
void SpiderMobile_Update(void);
void SpiderMobile_LateUpdate(void);
void SpiderMobile_StaticUpdate(void);
void SpiderMobile_Draw(void);
void SpiderMobile_Create(void* data);
void SpiderMobile_StageLoad(void);
void SpiderMobile_EditorDraw(void);
void SpiderMobile_EditorLoad(void);
void SpiderMobile_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPIDERMOBILE_H
