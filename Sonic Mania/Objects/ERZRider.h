#ifndef OBJ_ERZRIDER_H
#define OBJ_ERZRIDER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectERZRider;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityERZRider;

// Object Struct
extern ObjectERZRider *ERZRider;

// Standard Entity Events
void ERZRider_Update(void);
void ERZRider_LateUpdate(void);
void ERZRider_StaticUpdate(void);
void ERZRider_Draw(void);
void ERZRider_Create(void* data);
void ERZRider_StageLoad(void);
void ERZRider_EditorDraw(void);
void ERZRider_EditorLoad(void);
void ERZRider_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ERZRIDER_H
