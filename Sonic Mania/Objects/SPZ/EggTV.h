#ifndef OBJ_EGGTV_H
#define OBJ_EGGTV_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectEggTV;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityEggTV;

// Object Struct
extern ObjectEggTV *EggTV;

// Standard Entity Events
void EggTV_Update(void);
void EggTV_LateUpdate(void);
void EggTV_StaticUpdate(void);
void EggTV_Draw(void);
void EggTV_Create(void* data);
void EggTV_StageLoad(void);
void EggTV_EditorDraw(void);
void EggTV_EditorLoad(void);
void EggTV_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_EGGTV_H
