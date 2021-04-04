#ifndef OBJ_TEETERTOTTER_H
#define OBJ_TEETERTOTTER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTeeterTotter;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTeeterTotter;

// Object Struct
extern ObjectTeeterTotter *TeeterTotter;

// Standard Entity Events
void TeeterTotter_Update(void);
void TeeterTotter_LateUpdate(void);
void TeeterTotter_StaticUpdate(void);
void TeeterTotter_Draw(void);
void TeeterTotter_Create(void* data);
void TeeterTotter_StageLoad(void);
void TeeterTotter_EditorDraw(void);
void TeeterTotter_EditorLoad(void);
void TeeterTotter_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TEETERTOTTER_H
