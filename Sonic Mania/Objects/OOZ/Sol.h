#ifndef OBJ_SOL_H
#define OBJ_SOL_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSol;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySol;

// Object Struct
extern ObjectSol *Sol;

// Standard Entity Events
void Sol_Update(void);
void Sol_LateUpdate(void);
void Sol_StaticUpdate(void);
void Sol_Draw(void);
void Sol_Create(void* data);
void Sol_StageLoad(void);
void Sol_EditorDraw(void);
void Sol_EditorLoad(void);
void Sol_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SOL_H
