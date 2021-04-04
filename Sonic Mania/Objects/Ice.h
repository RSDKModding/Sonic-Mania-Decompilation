#ifndef OBJ_ICE_H
#define OBJ_ICE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectIce;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityIce;

// Object Struct
extern ObjectIce *Ice;

// Standard Entity Events
void Ice_Update(void);
void Ice_LateUpdate(void);
void Ice_StaticUpdate(void);
void Ice_Draw(void);
void Ice_Create(void* data);
void Ice_StageLoad(void);
void Ice_EditorDraw(void);
void Ice_EditorLoad(void);
void Ice_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ICE_H
