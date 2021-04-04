#ifndef OBJ_STAIRCASE_H
#define OBJ_STAIRCASE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectStaircase;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityStaircase;

// Object Struct
extern ObjectStaircase *Staircase;

// Standard Entity Events
void Staircase_Update(void);
void Staircase_LateUpdate(void);
void Staircase_StaticUpdate(void);
void Staircase_Draw(void);
void Staircase_Create(void* data);
void Staircase_StageLoad(void);
void Staircase_EditorDraw(void);
void Staircase_EditorLoad(void);
void Staircase_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_STAIRCASE_H
