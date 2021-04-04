#ifndef OBJ_WOODROW_H
#define OBJ_WOODROW_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWoodrow;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWoodrow;

// Object Struct
extern ObjectWoodrow *Woodrow;

// Standard Entity Events
void Woodrow_Update(void);
void Woodrow_LateUpdate(void);
void Woodrow_StaticUpdate(void);
void Woodrow_Draw(void);
void Woodrow_Create(void* data);
void Woodrow_StageLoad(void);
void Woodrow_EditorDraw(void);
void Woodrow_EditorLoad(void);
void Woodrow_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_WOODROW_H
