#ifndef OBJ_CIRCLEBUMPER_H
#define OBJ_CIRCLEBUMPER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCircleBumper;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCircleBumper;

// Object Struct
extern ObjectCircleBumper *CircleBumper;

// Standard Entity Events
void CircleBumper_Update(void);
void CircleBumper_LateUpdate(void);
void CircleBumper_StaticUpdate(void);
void CircleBumper_Draw(void);
void CircleBumper_Create(void* data);
void CircleBumper_StageLoad(void);
void CircleBumper_EditorDraw(void);
void CircleBumper_EditorLoad(void);
void CircleBumper_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CIRCLEBUMPER_H
