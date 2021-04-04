#ifndef OBJ_WISP_H
#define OBJ_WISP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWisp;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWisp;

// Object Entity
extern ObjectWisp *Wisp;

// Standard Entity Events
void Wisp_Update(void);
void Wisp_LateUpdate(void);
void Wisp_StaticUpdate(void);
void Wisp_Draw(void);
void Wisp_Create(void* data);
void Wisp_StageLoad(void);
void Wisp_EditorDraw(void);
void Wisp_EditorLoad(void);
void Wisp_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_WISP_H
