#ifndef OBJ_SP500_H
#define OBJ_SP500_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSP500;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySP500;

// Object Struct
extern ObjectSP500 *SP500;

// Standard Entity Events
void SP500_Update(void);
void SP500_LateUpdate(void);
void SP500_StaticUpdate(void);
void SP500_Draw(void);
void SP500_Create(void* data);
void SP500_StageLoad(void);
void SP500_EditorDraw(void);
void SP500_EditorLoad(void);
void SP500_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SP500_H
