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
void MSZ2Cutscene_Update();
void MSZ2Cutscene_LateUpdate();
void MSZ2Cutscene_StaticUpdate();
void MSZ2Cutscene_Draw();
void MSZ2Cutscene_Create(void* data);
void MSZ2Cutscene_StageLoad();
void MSZ2Cutscene_EditorDraw();
void MSZ2Cutscene_EditorLoad();
void MSZ2Cutscene_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSZ2CUTSCENE_H
