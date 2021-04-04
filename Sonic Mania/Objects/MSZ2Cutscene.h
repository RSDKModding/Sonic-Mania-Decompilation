#ifndef OBJ_MSZ2CUTSCENE_H
#define OBJ_MSZ2CUTSCENE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMSZ2Cutscene;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMSZ2Cutscene;

// Object Struct
extern ObjectMSZ2Cutscene *MSZ2Cutscene;

// Standard Entity Events
void MSZ2Cutscene_Update(void);
void MSZ2Cutscene_LateUpdate(void);
void MSZ2Cutscene_StaticUpdate(void);
void MSZ2Cutscene_Draw(void);
void MSZ2Cutscene_Create(void* data);
void MSZ2Cutscene_StageLoad(void);
void MSZ2Cutscene_EditorDraw(void);
void MSZ2Cutscene_EditorLoad(void);
void MSZ2Cutscene_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MSZ2CUTSCENE_H
